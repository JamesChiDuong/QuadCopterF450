/*
 * USER_UI.c
 *
 *  LCD status screen (240 x 320, portrait):
 *
 *   0   +------------------------------+
 *       | QUAD F450          RC FULL   |  title bar
 *   24  | DISARMED                     |  state (big, coloured)
 *   44  | +----------+  ROLL           |
 *       | | horizon  |   -12.3         |  attitude indicator + values
 *       | |  ---o--- |  PITCH          |
 *       | |          |    4.5          |
 *       | +----------+  YAW  ...       |
 *   170 | MOTORS (us)                  |
 *       | M1 1500 [#########     ]     |  4 motor bars
 *   250 | RC INPUT / UART SETPOINT     |
 *       | ...                          |
 *   290 | last event                   |
 *   306 | dt / loop / IMU errors       |
 *       +------------------------------+
 *
 *  The SPI link is slow (~2 MHz), so every element is redrawn only when its
 *  content changes (character by character for text).
 *  No printf-family calls with floats here (newlib reentrancy between tasks).
 */
/*
 * ============================================================================
 *  OVERVIEW - the status screen (UI task)
 * ============================================================================
 *  1. Who runs it?
 *     UI_Task is a FreeRTOS task (priority BelowNormal, created in main.c)
 *     that wakes every UI_REFRESH_PERIOD_MS (100 ms = 10 Hz). It only READS
 *     the flight status (Sys_GetStatus) and never changes it, so the screen
 *     cannot disturb flight control. Because its priority is lower than
 *     BLDCTask, the flight loop pre-empts it at any time, even in the middle
 *     of an SPI transfer.
 *
 *  2. Drawing strategy (why "change detection")
 *     The SPI link is slow (see USER_LCD.c: ~0.55 s for a full screen), so:
 *     - UI_DrawStatic draws everything that never changes (labels, frames)
 *       ONCE at start-up.
 *     - Each text area is a UI_Field that remembers what is currently on
 *       screen (last[]) and redraws only the characters that differ.
 *       Example: "  -12.3" -> "  -12.4": only one 16x16 char is sent.
 *     - Motor bars only draw the piece between the old and new length.
 *     - The artificial horizon is only redrawn if roll or pitch changed by
 *       more than 0.5 deg.
 *
 *  3. No printf with floats
 *     sprintf("%f") in newlib uses shared internal state (reentrancy struct,
 *     malloc) that is not safe when several tasks print at the same time and
 *     needs a lot of stack. So this file formats numbers itself
 *     (UI_PutInt, UI_PutFixed1) with integer maths only.
 *
 *  4. Artificial horizon (attitude indicator) maths
 *     The box is 120 x 120 px centred at (AI_CX, AI_CY). The horizon is a
 *     straight line, described as: centre point (ox, oy) + direction (dx, dy).
 *     - ROLL rotates the line. Direction = (cos r, -sin r); the minus sign is
 *       because screen y grows DOWNWARD. Rolling right (r > 0) makes the
 *       right end of the line go up = counter-clockwise on screen, exactly
 *       what a pilot sees out of the window.
 *     - PITCH shifts the line, perpendicular to itself, by
 *       pitch * AI_PX_PER_DEG pixels. Nose up -> the horizon moves down.
 *       Example: pitch 10 deg, roll 0 -> 13 px below the centre.
 *     - The line is drawn as points for t = -90..90 px along the direction;
 *       points outside the box are skipped. Each point is 2 px tall so a
 *       nearly flat line does not look broken.
 *     - The drawn points are stored (s_ai_px/s_ai_py) so the next update can
 *       erase exactly them instead of clearing the whole box (much faster).
 *
 *  5. Layout: see the drawing above (y coordinates in pixels).
 * ============================================================================
 */
#include "USER_Config.h"

#if (UI_LCD_ENABLE == 1)

#include "main.h"
#include "USER_UI.h"
#include "USER_LCD.h"
#include "USER_System.h"
#include <math.h>

#define UI_BG          LCD_BLACK
#define UI_FG          LCD_WHITE
#define UI_LABEL       LCD_GRAY
#define UI_TITLE_BG    LCD_NAVY

/* Attitude indicator box */
#define AI_X           4
#define AI_Y           44
#define AI_SIZE        120
#define AI_CX          (AI_X + AI_SIZE / 2)
#define AI_CY          (AI_Y + AI_SIZE / 2)
#define AI_PX_PER_DEG  (1.3f)          /* pitch scale                     */
#define AI_MAX_PTS     240
/* AI_PX_PER_DEG: 1.3 px per degree -> +-45 deg of pitch still fits in the
 * 120 px box (45 x 1.3 = 58 px from the centre).
 * AI_MAX_PTS: up to 120 columns x 2 px per point = 240 stored pixels. */

/* Motor bars */
#define BAR_X          72
#define BAR_W          160
#define BAR_H          10
#define BAR_Y0         184
#define BAR_DY         16
/* Bar length = (us - 1000) * 160 / 1000 px. Example: 1500 us -> 80 px (half). */

/* ------------------------------------------------------------------------- */
/* Text fields with change detection                                         */
/* ------------------------------------------------------------------------- */
#define FIELD_MAX 31

typedef struct
{
    int16_t  x;
    int16_t  y;
    uint8_t  scale;
    uint8_t  width;              /* characters, text is padded/truncated */
    uint16_t fg;
    char     last[FIELD_MAX + 1];
    bool     valid;
} UI_Field;
/* x, y   : top-left pixel of the field
 * scale  : font scale (1 = 8 px chars, 2 = 16 px chars)
 * fg     : colour currently on screen
 * last[] : text currently on screen (for change detection)
 * valid  : false until the field has been drawn once (forces a full draw) */

/* Shows `text` in field f with colour fg.
 * The text is padded with spaces (or truncated) to exactly f->width chars,
 * so a shorter new value also erases the tail of a longer old one.
 * Only characters that differ from last[] are sent to the LCD, unless the
 * field was never drawn or the colour changed (then all chars are redrawn). */
static void UI_FieldSet(UI_Field *f, const char *text, uint16_t fg)
{
    char buf[FIELD_MAX + 1];
    uint8_t i;
    bool force = (!f->valid) || (fg != f->fg);

    for (i = 0; i < f->width; i++)
        buf[i] = (*text != '\0') ? *text++ : ' ';
    buf[f->width] = '\0';

    for (i = 0; i < f->width; i++)
    {
        if (force || (buf[i] != f->last[i]))
        {
            LCD_DrawChar((int16_t)(f->x + i * 8 * f->scale), f->y, buf[i], fg, UI_BG, f->scale);
            f->last[i] = buf[i];
        }
    }
    f->last[f->width] = '\0';
    f->fg = fg;
    f->valid = true;
}

/* ------------------------------------------------------------------------- */
/* Small formatters (no printf)                                              */
/* ------------------------------------------------------------------------- */
/* All formatters below write at dst, add a '\0' terminator and return a
 * pointer to that terminator, so calls can be chained:
 *   p = UI_PutStr(a, "M"); p = UI_PutInt(p, 1);  ->  a = "M1"
 * The caller must make sure the buffer is large enough (32 bytes here). */
static char *UI_PutStr(char *dst, const char *s)
{
    while (*s != '\0')
        *dst++ = *s++;
    *dst = '\0';
    return dst;
}

/* Signed integer -> decimal text. Digits are produced last-first
 * (v % 10, v / 10, ...) into tmp[], then copied in reverse order.
 * Example: 1500 -> tmp = "0051" -> dst = "1500". */
static char *UI_PutInt(char *dst, int32_t v)
{
    char tmp[12];
    int n = 0;
    uint32_t u;

    if (v < 0)
    {
        *dst++ = '-';
        u = (uint32_t)(-v);
    }
    else
    {
        u = (uint32_t)v;
    }
    do
    {
        tmp[n++] = (char)('0' + (u % 10U));
        u /= 10U;
    } while (u != 0U);
    while (n > 0)
        *dst++ = tmp[--n];
    *dst = '\0';
    return dst;
}

/* One decimal: -12.3 */
/* Works in tenths as an integer: -12.34 -> x10 = -123 -> "-" + "12" + "." + "3".
 * The sign is handled separately so that e.g. -0.5 prints as "-0.5"
 * (x10 / 10 would be 0, which has no sign). */
static char *UI_PutFixed1(char *dst, float v)
{
    int32_t x10 = (int32_t)lroundf(v * 10.0f);

    if (x10 < 0)
    {
        *dst++ = '-';
        x10 = -x10;
    }
    dst = UI_PutInt(dst, x10 / 10);
    *dst++ = '.';
    *dst++ = (char)('0' + (x10 % 10));
    *dst = '\0';
    return dst;
}

/* Right align 'src' in a field of 'width' characters */
/* Example: src "4.5", width 6 -> "   4.5". Keeps the decimal point in the same
 * column, so only the changing digits get redrawn. */
static void UI_RightAlign(char *dst, const char *src, uint8_t width)
{
    uint8_t len = 0, i;

    while (src[len] != '\0')
        len++;
    for (i = 0; (int)i < ((int)width - (int)len); i++)
        *dst++ = ' ';
    UI_PutStr(dst, src);
}

/* ------------------------------------------------------------------------- */
/* Attitude indicator                                                        */
/* ------------------------------------------------------------------------- */
/* Pixels of the horizon line currently on screen (to erase them next time). */
static int16_t s_ai_px[AI_MAX_PTS];
static int16_t s_ai_py[AI_MAX_PTS];
static uint16_t s_ai_n;

/* true if (x, y) is strictly inside the box (the 1 px border is excluded,
 * so the line never overwrites the frame). */
static bool UI_AiInside(int16_t x, int16_t y)
{
    return (x > AI_X) && (x < (AI_X + AI_SIZE - 1)) &&
           (y > AI_Y) && (y < (AI_Y + AI_SIZE - 1));
}

static void UI_AiDrawAircraft(void)
{
    /* Fixed aircraft symbol: wings + centre dot */
    LCD_FillRect(AI_CX - 30, AI_CY - 1, 20, 3, LCD_YELLOW);
    LCD_FillRect(AI_CX + 11, AI_CY - 1, 20, 3, LCD_YELLOW);
    LCD_FillRect(AI_CX - 2, AI_CY - 2, 5, 5, LCD_YELLOW);
}

/* Box frame + pitch ticks + aircraft symbol. Called at start-up AND after
 * every horizon update, because erasing the old line may have wiped some of
 * these pixels. Example tick position: 10 deg x 1.3 = 13 px from the centre. */
static void UI_AiStatic(void)
{
    LCD_DrawRect(AI_X, AI_Y, AI_SIZE, AI_SIZE, LCD_DARKGRAY);
    /* tick marks for +-10 / +-20 deg of pitch */
    LCD_DrawHLine(AI_CX - 6, (int16_t)(AI_CY - 10 * AI_PX_PER_DEG), 13, LCD_DARKGRAY);
    LCD_DrawHLine(AI_CX - 6, (int16_t)(AI_CY + 10 * AI_PX_PER_DEG), 13, LCD_DARKGRAY);
    LCD_DrawHLine(AI_CX - 10, (int16_t)(AI_CY - 20 * AI_PX_PER_DEG), 21, LCD_DARKGRAY);
    LCD_DrawHLine(AI_CX - 10, (int16_t)(AI_CY + 20 * AI_PX_PER_DEG), 21, LCD_DARKGRAY);
    UI_AiDrawAircraft();
}

/*
 * Horizon line as seen from the cockpit:
 *  roll right (+) -> horizon rotates counter-clockwise on screen
 *  pitch up (+)   -> horizon moves down
 */
/* roll_deg / pitch_deg: attitude in degrees; color: line colour (cyan once
 * the gyro is calibrated, dark grey before). Erases the old line, draws the
 * new one, then redraws the static parts on top. */
static void UI_AiUpdate(float roll_deg, float pitch_deg, uint16_t color)
{
    /* deg -> rad: x pi/180 = 0.01745. (dx, dy) = unit vector ALONG the line,
     * (sin r, cos r) = unit vector PERPENDICULAR to it (their dot product is
     * 0). The line centre is moved along the perpendicular by the pitch
     * offset. Example: roll 30 deg, pitch 0 -> dx = 0.866, dy = -0.5, the
     * line passes through the box centre rising to the right. */
    float r = roll_deg * 0.0174532925f;
    float dx = cosf(r);
    float dy = -sinf(r);               /* screen y grows downward */
    float off = pitch_deg * AI_PX_PER_DEG;
    float ox = AI_CX + off * sinf(r);  /* perpendicular offset    */
    float oy = AI_CY + off * cosf(r);
    int16_t t, x, y, lx = -1000, ly = -1000;
    uint16_t i;

    /* Erase previous line (redraw ticks/aircraft afterwards) */
    for (i = 0; i < s_ai_n; i++)
        LCD_DrawPixel(s_ai_px[i], s_ai_py[i], UI_BG);
    s_ai_n = 0;

    /* Walk 1 px steps along the line. Skip: the same pixel as the previous
     * step (happens with steep lines after rounding), pixels outside the
     * box. Stop if the store is full. */
    for (t = -90; t <= 90; t++)
    {
        x = (int16_t)lroundf(ox + dx * t);
        y = (int16_t)lroundf(oy + dy * t);
        if (((x == lx) && (y == ly)) || !UI_AiInside(x, y) || !UI_AiInside(x, (int16_t)(y + 1)))
            continue;
        lx = x;
        ly = y;
        if (s_ai_n + 2U > AI_MAX_PTS)
            break;
        LCD_DrawPixel(x, y, color);
        LCD_DrawPixel(x, (int16_t)(y + 1), color);
        s_ai_px[s_ai_n] = x; s_ai_py[s_ai_n++] = y;
        s_ai_px[s_ai_n] = x; s_ai_py[s_ai_n++] = (int16_t)(y + 1);
    }

    UI_AiStatic();
}

/* ------------------------------------------------------------------------- */
/* Motor bars                                                                */
/* ------------------------------------------------------------------------- */
/* Bar length currently on screen in px (-1 = never drawn) and its colour. */
static int16_t  s_bar_len[4] = { -1, -1, -1, -1 };
static uint16_t s_bar_color[4];

/* Motor bar idx (0..3) for a pulse of `us`: coloured part = throttle,
 * dark grey = rest. Only the difference is drawn:
 *   longer : paint (old .. new) in colour   e.g. 80 -> 96 px: 16 px painted
 *   shorter: paint (new .. old) dark grey
 * A colour change (armed <-> disarmed) or the first call redraws all. */
static void UI_BarUpdate(uint8_t idx, uint16_t us, uint16_t color)
{
    int16_t y = (int16_t)(BAR_Y0 + idx * BAR_DY);
    int32_t len = ((int32_t)us - (int32_t)MOTOR_STOP_US) * BAR_W /
                  (int32_t)(MOTOR_MAX_US - MOTOR_STOP_US);
    int16_t old = s_bar_len[idx];

    if (len < 0) len = 0;
    if (len > BAR_W) len = BAR_W;

    if ((old < 0) || (color != s_bar_color[idx]))
    {
        LCD_FillRect(BAR_X, y, (int16_t)len, BAR_H, color);
        LCD_FillRect((int16_t)(BAR_X + len), y, (int16_t)(BAR_W - len), BAR_H, LCD_DARKGRAY);
    }
    else if (len > old)
    {
        LCD_FillRect((int16_t)(BAR_X + old), y, (int16_t)(len - old), BAR_H, color);
    }
    else if (len < old)
    {
        LCD_FillRect((int16_t)(BAR_X + len), y, (int16_t)(old - len), BAR_H, LCD_DARKGRAY);
    }
    s_bar_len[idx] = (int16_t)len;
    s_bar_color[idx] = color;
}

/* ------------------------------------------------------------------------- */
/* Fields                                                                    */
/* ------------------------------------------------------------------------- */
/* Initialiser order: { x, y, scale, width (chars), fg, last, valid }.
 * Width check: f_state 14 chars x 16 px = 224 px, at x = 8 -> ends at 232 < 240. */
static UI_Field f_state  = { 8,   24,  2, 14, 0, "", false };
static UI_Field f_roll   = { 136, 58,  2, 6,  0, "", false };
static UI_Field f_pitch  = { 136, 98,  2, 6,  0, "", false };
static UI_Field f_yaw    = { 136, 138, 2, 6,  0, "", false };
static UI_Field f_motor[4] =
{
    { 4, BAR_Y0 + 1 + 0 * BAR_DY, 1, 8, 0, "", false },
    { 4, BAR_Y0 + 1 + 1 * BAR_DY, 1, 8, 0, "", false },
    { 4, BAR_Y0 + 1 + 2 * BAR_DY, 1, 8, 0, "", false },
    { 4, BAR_Y0 + 1 + 3 * BAR_DY, 1, 8, 0, "", false },
};
static UI_Field f_in1    = { 4, 262, 1, 29, 0, "", false };
static UI_Field f_in2    = { 4, 274, 1, 29, 0, "", false };
static UI_Field f_event  = { 4, 290, 1, 29, 0, "", false };
static UI_Field f_loop   = { 4, 306, 1, 29, 0, "", false };

static uint16_t UI_StateColor(FC_StateTypeDef st)
{
    switch (st)
    {
    case FC_STATE_ARMED:       return LCD_GREEN;
    case FC_STATE_FAILSAFE:    return LCD_RED;
    case FC_STATE_CALIBRATING: return LCD_CYAN;
    case FC_STATE_DISARMED:
    default:                   return LCD_YELLOW;
    }
}

/* Draws everything that never changes (once, ~0.6 s because of the full
 * screen clear). The build variant name is right-aligned in the title bar:
 * sizeof("RC FULL") - 1 = 7 chars -> x = 240 - 6 - 56 = 178. */
static void UI_DrawStatic(void)
{
    LCD_FillScreen(UI_BG);

    LCD_FillRect(0, 0, LCD_WIDTH, 20, UI_TITLE_BG);
    LCD_DrawString(6, 6, "QUAD F450", UI_FG, UI_TITLE_BG, 1);
    LCD_DrawString((int16_t)(LCD_WIDTH - 6 - 8 * (int16_t)(sizeof(FC_VARIANT_NAME) - 1)), 6,
                   FC_VARIANT_NAME,
                   (FC_VARIANT == FC_VARIANT_RC_FULL) ? LCD_GREEN :
                   (FC_VARIANT == FC_VARIANT_GAMEPAD) ? LCD_CYAN : LCD_ORANGE,
                   UI_TITLE_BG, 1);

    LCD_DrawString(136, 48,  "ROLL",  UI_LABEL, UI_BG, 1);
    LCD_DrawString(136, 88,  "PITCH", UI_LABEL, UI_BG, 1);
    LCD_DrawString(136, 128, "YAW",   UI_LABEL, UI_BG, 1);

    LCD_DrawString(4, 172, "MOTORS (us)", UI_LABEL, UI_BG, 1);
#if (FC_VARIANT == FC_VARIANT_RC_FULL)
    LCD_DrawString(4, 250, "RC INPUT (us)", UI_LABEL, UI_BG, 1);
#elif (FC_VARIANT == FC_VARIANT_GAMEPAD)
    LCD_DrawString(4, 250, "GAMEPAD (USB CN6)", UI_LABEL, UI_BG, 1);
#else
    LCD_DrawString(4, 250, "UART SETPOINT", UI_LABEL, UI_BG, 1);
#endif
    LCD_DrawHLine(0, 286, LCD_WIDTH, LCD_DARKGRAY);

    UI_AiStatic();
}

/* One refresh (every 100 ms): format each value into text and hand it to
 * its field / bar; the change detection decides what is actually sent.
 * fc points to the live status of the flight task (read only). */
static void UI_Refresh(const FC_StatusTypeDef *fc)
{
    char a[32], b[32];
    char *p;
    uint8_t i;
    uint16_t state_color = UI_StateColor(fc->state);
    uint16_t bar_color = (fc->state == FC_STATE_ARMED) ? LCD_GREEN : LCD_GRAY;

    /* PADTEST: magenta title, orange bars, so it is never mistaken for flight */
    if (fc->stick_test)
    {
        state_color = LCD_MAGENTA;
        bar_color = LCD_ORANGE;
    }

    /* State */
    UI_FieldSet(&f_state, fc->stick_test ? "STICK TEST" : Sys_StateName(fc->state), state_color);

    /* Attitude */
    UI_PutFixed1(a, fc->att.roll);   UI_RightAlign(b, a, 6); UI_FieldSet(&f_roll, b, UI_FG);
    UI_PutFixed1(a, fc->att.pitch);  UI_RightAlign(b, a, 6); UI_FieldSet(&f_pitch, b, UI_FG);
    UI_PutFixed1(a, fc->att.yaw);    UI_RightAlign(b, a, 6); UI_FieldSet(&f_yaw, b, UI_FG);
    {
        /* 1000 = impossible angle, forces the first draw. */
        static float last_r = 1000.0f, last_p = 1000.0f;
        static uint16_t last_c;
        uint16_t c = fc->att.calibrated ? LCD_CYAN : LCD_DARKGRAY;

        if ((fabsf(fc->att.roll - last_r) > 0.5f) || (fabsf(fc->att.pitch - last_p) > 0.5f) ||
            (c != last_c))
        {
            UI_AiUpdate(fc->att.roll, fc->att.pitch, c);
            last_r = fc->att.roll;
            last_p = fc->att.pitch;
            last_c = c;
        }
    }

    /* Motors */
    for (i = 0; i < 4U; i++)
    {
        p = UI_PutStr(a, "M");
        p = UI_PutInt(p, (int32_t)i + 1);
        p = UI_PutStr(p, " ");
        UI_PutInt(p, fc->motor_us[i]);
        UI_FieldSet(&f_motor[i], a, UI_FG);
        UI_BarUpdate(i, fc->motor_us[i], bar_color);
    }

    /* Inputs */
#if (FC_VARIANT == FC_VARIANT_RC_FULL)
    p = UI_PutStr(a, fc->rc_ok ? "OK   " : "LOST ");
    p = UI_PutStr(p, "R");  p = UI_PutInt(p, fc->rc_us[RC_CH_ROLL]);
    p = UI_PutStr(p, " P"); p = UI_PutInt(p, fc->rc_us[RC_CH_PITCH]);
    UI_FieldSet(&f_in1, a, fc->rc_ok ? LCD_GREEN : LCD_RED);
    p = UI_PutStr(a, "T");  p = UI_PutInt(p, fc->rc_us[RC_CH_THROTTLE]);
    p = UI_PutStr(p, " Y"); p = UI_PutInt(p, fc->rc_us[RC_CH_YAW]);
    p = UI_PutStr(p, " SW"); UI_PutInt(p, fc->rc_us[RC_CH_ARM]);
    UI_FieldSet(&f_in2, a, UI_FG);
#elif (FC_VARIANT == FC_VARIANT_GAMEPAD)
    {
        const Gamepad_StateTypeDef *gp = &fc->pad;
        uint16_t c;

        if (gp->connected)
        {
            p = UI_PutStr(a, Gamepad_TypeName(gp->type));
            p = UI_PutStr(p, " OK  START=arm B=stop");
            c = LCD_GREEN;
        }
        else if (gp->unsupported)
        {
            p = UI_PutStr(a, "NOT SUPPORTED: use Xbox mode");
            c = LCD_RED;
        }
        else
        {
            p = UI_PutStr(a, "NO PAD - plug into CN6");
            c = LCD_ORANGE;
        }
        UI_FieldSet(&f_in1, a, c);

        p = UI_PutStr(a, "T");  p = UI_PutInt(p, (int32_t)fc->throttle_us);
        /* Sticks are -1.0..+1.0 floats; shown as -99..99 to stay short. */
        p = UI_PutStr(p, " L"); p = UI_PutInt(p, (int32_t)lroundf(gp->lx * 99.0f));
        p = UI_PutStr(p, "/");  p = UI_PutInt(p, (int32_t)lroundf(gp->ly * 99.0f));
        p = UI_PutStr(p, " R"); p = UI_PutInt(p, (int32_t)lroundf(gp->rx * 99.0f));
        p = UI_PutStr(p, "/");  UI_PutInt(p, (int32_t)lroundf(gp->ry * 99.0f));
        UI_FieldSet(&f_in2, a, UI_FG);
    }
#else
    p = UI_PutStr(a, "THR ");  UI_PutInt(p, (int32_t)fc->throttle_us);
    UI_FieldSet(&f_in1, a, UI_FG);
    p = UI_PutStr(a, "R ");    p = UI_PutFixed1(p, fc->sp_roll_deg);
    p = UI_PutStr(p, " P ");   p = UI_PutFixed1(p, fc->sp_pitch_deg);
    p = UI_PutStr(p, " Yr ");  UI_PutFixed1(p, fc->sp_yaw_rate_dps);
    UI_FieldSet(&f_in2, a, UI_FG);
#endif

    /* Event + loop */
    UI_FieldSet(&f_event, (fc->last_event != NULL) ? fc->last_event : "-", state_color);
    p = UI_PutStr(a, "dt ");     p = UI_PutFixed1(p, fc->loop_dt_s * 1000.0f);
    p = UI_PutStr(p, "ms  IMU err "); UI_PutInt(p, (int32_t)fc->imu_err_count);
    UI_FieldSet(&f_loop, a, UI_LABEL);
}

/* Task body: init the LCD, draw the static screen, then refresh at a fixed
 * 10 Hz. vTaskDelayUntil (not osDelay) keeps the period constant even
 * though one refresh may take a varying few tens of ms. */
void UI_Task(void const *argument)
{
    TickType_t last_wake;

    (void)argument;

    LCD_Init();
    UI_DrawStatic();

    last_wake = xTaskGetTickCount();
    for (;;)
    {
        UI_Refresh(Sys_GetStatus());
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(UI_REFRESH_PERIOD_MS));
    }
}

#endif /* UI_LCD_ENABLE */
