/*
 * USER_DataCollect.c
 *
 *  Telemetry / debug output.
 */
/*
 * OVERVIEW
 *   Everything the firmware prints periodically on the terminal (USART1 via
 *   the ST-LINK virtual COM port). All functions run in UARTTask
 *   (StartUARTTask in main.c, loop every 10 ms):
 *     - Debug_Proc()      every TELEMETRY_PERIOD_MS (50 ms = 20 Hz): one line
 *                         of the stream selected with @QC+DEBUG=<n>.
 *     - Debug_PadEvents() every loop (GAMEPAD build): gamepad event log.
 *     - Debug_PrintStatus() on @QC+STATUS.
 *   Data source: the read-only FC_StatusTypeDef from Sys_GetStatus()
 *   (written by BLDCTask). No lock is taken: a printed line may mix values
 *   from two consecutive control cycles, which is fine for monitoring.
 *
 *   DEBUG=<n> stream modes:
 *     0 OFF
 *     1 ATTITUDE : "> ACC[g] X=.. | ANGLE[deg] P=.. R=.. Y=.." (3D-Visualize.py)
 *     2 RC       : RC channels in us, or in the GAMEPAD build the pad state
 *                  (sticks, triggers, BTN mask, RAW HID buttons) + setpoints
 *     3 MOTORS   : state, 4 motor pulses (us), mixer inputs
 *     4 PID_RP   : roll and pitch cascade: angle sp/meas, rate sp/meas, P/I/D
 *     5 PID_YAW  : yaw rate loop
 *     6 TLM      : machine-readable line for host/drone_viewer.py:
 *                  $TLM,<state>,<roll>,<pitch>,<yaw>,<m1>,<m2>,<m3>,<m4>,<thr>,<variant>
 *                  example: $TLM,ARMED,1.25,-0.40,30.00,1180,1175,1190,1185,1150,3
 *                  (angles in deg, motors and throttle in us). Do not change the
 *                  field order without updating the Python script.
 *
 *   Gamepad event log (PADLOG, GAMEPAD build only): prints a line only when
 *   something CHANGES, so the terminal stays readable:
 *     - plug / unplug;
 *     - every button press/release immediately, with what the FC does with it
 *       (ARM, DISARM, ...);
 *     - stick/trigger moves bigger than GAMEPAD_LOG_AXIS_STEP, at most once
 *       per GAMEPAD_LOG_AXIS_PERIOD_MS (rate limiting), together with the
 *       resulting roll/pitch/yaw setpoints.
 *     In Switch mode the Nintendo label is printed first, e.g.
 *     "PAD button B (A) pressed": the bottom button is labelled B on a
 *     Nintendo pad but is GP_BTN_A internally (positional mapping).
 */
#include "USER_GPS.h"
#include "main.h"
#include "USER_DataCollect.h"
#include "USER_System.h"
#include "USER_PID.h"

static volatile uint8_t s_debug_mode = DBG_MODE_OFF;

/* Select the stream (0..DBG_MODE_COUNT-1). Returns false if out of range. */
bool Debug_SetMode(uint8_t mode)
{
    if (mode >= DBG_MODE_COUNT)
        return false;
    s_debug_mode = mode;
    return true;
}

uint8_t Debug_GetMode(void)
{
    return s_debug_mode;
}

/* Keep this exact text format: host/3D-Visualize.py parses it.
 * X/Y/Z = accelerometer [g] (sensor axes), P/R/Y = estimated angles [deg]. */
static void Debug_ShowAttitude(const FC_StatusTypeDef *fc)
{
    RTOSprintf("\r\n> ACC[g] X=%.2f, Y=%.2f, Z=%.2f| ANGLE[deg] P=%.2f, R=%.2f, Y=%.2f",
               MPU6050.acc_x, MPU6050.acc_y, MPU6050.acc_z,
               fc->att.pitch, fc->att.roll, fc->att.yaw);
}

/* DEBUG=2. GAMEPAD build: pad line (the RC line below is then unreachable,
 * the early return skips it). Other builds: the 5 RC channels in us. */
static void Debug_ShowRc(const FC_StatusTypeDef *fc)
{
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    const Gamepad_StateTypeDef *gp = &fc->pad;

    RTOSprintf("\r\n> PAD %s %s LX=%+.2f LY=%+.2f RX=%+.2f RY=%+.2f LT=%.2f RT=%.2f BTN=%04lX RAW=%08lX"
               " | SP R=%.1f P=%.1f Yr=%.1f T=%.0f",
               gp->connected ? "OK  " : (gp->unsupported ? "UNSP" : "NONE"),
               Gamepad_TypeName(gp->type), gp->lx, gp->ly, gp->rx, gp->ry, gp->lt, gp->rt,
               (unsigned long)gp->buttons, (unsigned long)gp->raw_buttons,
               fc->sp_roll_deg, fc->sp_pitch_deg, fc->sp_yaw_rate_dps, fc->throttle_us);
    return;
#endif
    RTOSprintf("\r\n> RC %s CH1=%u CH2=%u CH3=%u CH4=%u CH5=%u | SP R=%.1f P=%.1f Yr=%.1f T=%.0f",
               fc->rc_ok ? "OK  " : "LOST",
               fc->rc_us[0], fc->rc_us[1], fc->rc_us[2], fc->rc_us[3], fc->rc_us[4],
               fc->sp_roll_deg, fc->sp_pitch_deg, fc->sp_yaw_rate_dps, fc->throttle_us);
}

static void Debug_ShowMotors(const FC_StatusTypeDef *fc)
{
    RTOSprintf("\r\n> %s M1=%u M2=%u M3=%u M4=%u | out R=%.1f P=%.1f Y=%.1f",
               Sys_StateName(fc->state),
               fc->motor_us[0], fc->motor_us[1], fc->motor_us[2], fc->motor_us[3],
               fc->out_roll, fc->out_pitch, fc->out_yaw);
}

/* One cascade line: outer angle loop (setpoint/measured deg) and inner rate
 * loop (setpoint/measured deg/s) with the P, I, D terms of the RATE PID. */
static void Debug_ShowPid(PID_IdTypeDef rate_id, const char *axis,
                          float sp_angle, float angle, float sp_rate, float rate)
{
    PID_TypeDef *pid = Sys_GetPid(rate_id);

    RTOSprintf("\r\n> %s ang sp=%.1f m=%.1f | rate sp=%.1f m=%.1f | P=%.1f I=%.1f D=%.1f out=%.1f",
               axis, sp_angle, angle, sp_rate, rate,
               pid->p_term, pid->i_term, pid->d_term, pid->output);
}

/* Print one line of the selected stream. Called by UARTTask at 20 Hz. */
bool Debug_Proc(void)
{
    const FC_StatusTypeDef *fc = Sys_GetStatus();

    switch (s_debug_mode)
    {
    case DBG_MODE_ATTITUDE:
        Debug_ShowAttitude(fc);
        break;
    case DBG_MODE_RC:
        Debug_ShowRc(fc);
        break;
    case DBG_MODE_MOTORS:
        Debug_ShowMotors(fc);
        break;
    case DBG_MODE_PID_RP:
        Debug_ShowPid(PID_ID_ROLL_RATE, "ROLL ", fc->sp_roll_deg, fc->att.roll,
                      fc->sp_roll_rate_dps, fc->att.roll_rate);
        Debug_ShowPid(PID_ID_PITCH_RATE, "PITCH", fc->sp_pitch_deg, fc->att.pitch,
                      fc->sp_pitch_rate_dps, fc->att.pitch_rate);
        break;
    case DBG_MODE_PID_YAW:
        Debug_ShowPid(PID_ID_YAW_RATE, "YAW  ", 0.0f, fc->att.yaw,
                      fc->sp_yaw_rate_dps, fc->att.yaw_rate);
        break;
    case DBG_MODE_TLM:
        /* $TLM,<state>,<roll>,<pitch>,<yaw>,<m1>,<m2>,<m3>,<m4>,<thr>,<variant> */
        RTOSprintf("\r\n$TLM,%s,%.2f,%.2f,%.2f,%u,%u,%u,%u,%.0f,%d",
                   Sys_StateName(fc->state), fc->att.roll, fc->att.pitch, fc->att.yaw,
                   fc->motor_us[0], fc->motor_us[1], fc->motor_us[2], fc->motor_us[3],
                   fc->throttle_us, (int)FC_VARIANT);
        break;
    case DBG_MODE_OFF:
    default:
        break;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Gamepad event log                                                         */
/* ------------------------------------------------------------------------- */
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
static volatile bool s_padlog = (GAMEPAD_LOG_DEFAULT != 0);

/* Button names for the log. The bit is the logical (positional) GP_BTN_*;
 * 'nintendo' is what is printed on the same physical button of a Switch pad
 * (A/B and X/Y are swapped compared with Xbox). */
typedef struct
{
    uint32_t    bit;
    const char *xbox;       /* name in XInput / HID / Xbox One mode */
    const char *nintendo;   /* label printed on a Switch-mode pad   */
} PadBtnName;

static const PadBtnName s_btn_names[] =
{
    { GP_BTN_A,      "A",      "B" },       /* bottom */
    { GP_BTN_B,      "B",      "A" },       /* right  */
    { GP_BTN_X,      "X",      "Y" },       /* left   */
    { GP_BTN_Y,      "Y",      "X" },       /* top    */
    { GP_BTN_LB,     "LB",     "L" },
    { GP_BTN_RB,     "RB",     "R" },
    { GP_BTN_BACK,   "BACK",   "-" },
    { GP_BTN_START,  "START",  "+" },
    { GP_BTN_LS,     "LS",     "L-stick" },
    { GP_BTN_RS,     "RS",     "R-stick" },
    { GP_BTN_HOME,   "HOME",   "HOME" },
    { GP_BTN_DUP,    "UP",     "UP" },
    { GP_BTN_DDOWN,  "DOWN",   "DOWN" },
    { GP_BTN_DLEFT,  "LEFT",   "LEFT" },
    { GP_BTN_DRIGHT, "RIGHT",  "RIGHT" },
};

/* What the flight controller does with a button */
static const char *Debug_BtnAction(uint32_t bit)
{
    switch (bit)
    {
    case GP_BTN_START: return " -> hold 0.5 s = ARM";
    case GP_BTN_B:
    case GP_BTN_BACK:  return " -> DISARM";
    case GP_BTN_X:     return " -> gyro calibration (disarmed)";
    case GP_BTN_Y:     return " -> level trim (disarmed)";
    default:           return "";
    }
}

/* true if |a - b| > GAMEPAD_LOG_AXIS_STEP (filters out stick noise) */
static bool Debug_AxisMoved(float a, float b)
{
    float d = a - b;
    return (d > GAMEPAD_LOG_AXIS_STEP) || (d < -GAMEPAD_LOG_AXIS_STEP);
}

bool Debug_SetPadLog(bool on)
{
    s_padlog = on;
    return true;
}

bool Debug_GetPadLog(void)
{
    return s_padlog;
}

/*
 * Gamepad event log, called every UARTTask loop (~10 ms).
 * Change detection: 'last' keeps the values that were last PRINTED (not the
 * last ones read). Buttons: XOR of new and old masks gives the bits that
 * changed, e.g. last 0x0000, now 0x0081 (A + START) -> changed = 0x0081 ->
 * two "pressed" lines. Axes: compared against the last printed value, so a
 * slow stick movement is still logged once it adds up to more than the step.
 * When the log is off, 'last' is kept up to date so turning it on does not
 * dump stale changes.
 */
void Debug_PadEvents(void)
{
    static Gamepad_StateTypeDef last;          /* last printed values */
    static uint32_t axis_tick;
    Gamepad_StateTypeDef gp;
    const FC_StatusTypeDef *fc = Sys_GetStatus();
    uint32_t changed;
    uint8_t i;

    Gamepad_GetState(&gp);

    if (!s_padlog)
    {
        last = gp;
        return;
    }

    /* Plug / unplug */
    if (gp.connected != last.connected)
    {
        if (gp.connected)
            RTOSprintf("\r\n> PAD connected: %s (VID %04X PID %04X)",
                       Gamepad_TypeName(gp.type), gp.vid, gp.pid);
        else
            RTOSprintf("\r\n> PAD disconnected");
        last = gp;
        return;
    }
    if (!gp.connected)
        return;

    /* Buttons: every press / release, immediately */
    changed = gp.buttons ^ last.buttons;
    if (changed != 0U)
    {
        for (i = 0; i < (sizeof(s_btn_names) / sizeof(s_btn_names[0])); i++)
        {
            uint32_t bit = s_btn_names[i].bit;
            bool down;

            if (!(changed & bit))
                continue;
            down = (gp.buttons & bit) != 0U;
            if (gp.type == GP_TYPE_SWITCHPRO)
                RTOSprintf("\r\n> PAD button %s (%s) %s%s", s_btn_names[i].nintendo,
                           s_btn_names[i].xbox, down ? "pressed" : "released",
                           down ? Debug_BtnAction(bit) : "");
            else
                RTOSprintf("\r\n> PAD button %s %s%s", s_btn_names[i].xbox,
                           down ? "pressed" : "released", down ? Debug_BtnAction(bit) : "");
        }
        last.buttons = gp.buttons;
    }

    /* PADTEST: announce ON/OFF, then log the motor outputs instead of the
     * flight setpoints. A line is also printed when the motors change by
     * >= 10 us (the held throttle ramps while the stick does not move). */
    {
        static bool last_test;
        static uint16_t last_m[4];

        if (fc->stick_test != last_test)
        {
            RTOSprintf("\r\n> PADTEST %s (%s)", fc->stick_test ? "ON - PROPS OFF!" : "OFF",
                       (fc->last_event != NULL) ? fc->last_event : "-");
            last_test = fc->stick_test;
        }

        if (fc->stick_test)
        {
            bool motors_moved = false;

            for (i = 0; i < 4U; i++)
            {
                int32_t d = (int32_t)fc->motor_us[i] - (int32_t)last_m[i];
                if ((d >= 10) || (d <= -10))
                    motors_moved = true;
            }
            if (((HAL_GetTick() - axis_tick) >= GAMEPAD_LOG_AXIS_PERIOD_MS) &&
                (motors_moved || Debug_AxisMoved(gp.lx, last.lx) || Debug_AxisMoved(gp.ly, last.ly) ||
                 Debug_AxisMoved(gp.rx, last.rx) || Debug_AxisMoved(gp.ry, last.ry)))
            {
                RTOSprintf("\r\n> PADTEST L(%+.2f,%+.2f) R(%+.2f,%+.2f) thr %4.0f | roll %+4.0f pitch %+4.0f"
                           " yaw %+4.0f us -> M1 %u M2 %u M3 %u M4 %u",
                           gp.lx, gp.ly, gp.rx, gp.ry, fc->throttle_us, fc->out_roll,
                           fc->out_pitch, fc->out_yaw, fc->motor_us[0], fc->motor_us[1],
                           fc->motor_us[2], fc->motor_us[3]);
                for (i = 0; i < 4U; i++)
                    last_m[i] = fc->motor_us[i];
                last.lx = gp.lx;
                last.ly = gp.ly;
                last.rx = gp.rx;
                last.ry = gp.ry;
                last.lt = gp.lt;
                last.rt = gp.rt;
                axis_tick = HAL_GetTick();
            }
            return;
        }
    }

    /* Sticks / triggers: when moved enough, rate limited */
    if ((HAL_GetTick() - axis_tick) >= GAMEPAD_LOG_AXIS_PERIOD_MS)
    {
        if (Debug_AxisMoved(gp.lx, last.lx) || Debug_AxisMoved(gp.ly, last.ly) ||
            Debug_AxisMoved(gp.rx, last.rx) || Debug_AxisMoved(gp.ry, last.ry) ||
            Debug_AxisMoved(gp.lt, last.lt) || Debug_AxisMoved(gp.rt, last.rt))
        {
            float sp_r, sp_p, sp_y;

            /* same conversion as the flight loop, so the log shows what the
             * FC would command (sp_r/sp_p in deg, sp_y in deg/s) */
            Sys_PadToSetpoints(&gp, &sp_r, &sp_p, &sp_y);
            RTOSprintf("\r\n> PAD L(%+.2f,%+.2f) R(%+.2f,%+.2f) LT %.2f RT %.2f"
                       " -> roll %+.1f pitch %+.1f deg, yaw %+.0f dps, thr %.0f us%s",
                       gp.lx, gp.ly, gp.rx, gp.ry, gp.lt, gp.rt, sp_r, sp_p, sp_y,
                       fc->throttle_us, (fc->state == FC_STATE_ARMED) ? "" : " (not armed)");
            last.lx = gp.lx;
            last.ly = gp.ly;
            last.rx = gp.rx;
            last.ry = gp.ry;
            last.lt = gp.lt;
            last.rt = gp.rt;
            axis_tick = HAL_GetTick();
        }
    }
}
#else
void Debug_PadEvents(void) { }
bool Debug_SetPadLog(bool on) { (void)on; return false; }
bool Debug_GetPadLog(void) { return false; }
#endif

/* @QC+STATUS: multi-line snapshot of the whole flight controller state */
void Debug_PrintStatus(void)
{
    const FC_StatusTypeDef *fc = Sys_GetStatus();

    RTOSprintf("\r\n@ Build   : %s", FC_VARIANT_NAME);
    RTOSprintf("\r\n@ State   : %s (source %s) - last event: %s",
               Sys_StateName(fc->state), (fc->source == FC_SRC_RC) ? "RC" : ((fc->source == FC_SRC_GAMEPAD) ? "GAMEPAD" : "UART"),
               (fc->last_event != NULL) ? fc->last_event : "-");
    RTOSprintf("\r\n@ Attitude: roll=%.2f pitch=%.2f yaw=%.2f deg | rates %.1f %.1f %.1f dps",
               fc->att.roll, fc->att.pitch, fc->att.yaw,
               fc->att.roll_rate, fc->att.pitch_rate, fc->att.yaw_rate);
    RTOSprintf("\r\n@ Gyro    : calibrated=%d bias=(%.2f, %.2f, %.2f) trim R=%.2f P=%.2f",
               fc->att.calibrated, fc->att.gyro_bias.x, fc->att.gyro_bias.y,
               fc->att.gyro_bias.z, fc->att.roll_trim, fc->att.pitch_trim);
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    RTOSprintf("\r\n@ Gamepad : %s type=%s VID=%04X PID=%04X class=%02X/%02X/%02X reports=%lu",
               fc->pad.connected ? "CONNECTED" : (fc->pad.unsupported ? "UNSUPPORTED" : "none"),
               Gamepad_TypeName(fc->pad.type), fc->pad.vid, fc->pad.pid, fc->pad.itf_class,
               fc->pad.itf_subclass, fc->pad.itf_protocol, (unsigned long)fc->pad.reports);
    RTOSprintf("\r\n@ Clock   : %s", Gamepad_ClockSource());
#else
    RTOSprintf("\r\n@ RC      : %s CH1..5 = %u %u %u %u %u",
               fc->rc_ok ? "OK" : "LOST",
               fc->rc_us[0], fc->rc_us[1], fc->rc_us[2], fc->rc_us[3], fc->rc_us[4]);
#endif
    RTOSprintf("\r\n@ Motors  : %u %u %u %u us",
               fc->motor_us[0], fc->motor_us[1], fc->motor_us[2], fc->motor_us[3]);
    RTOSprintf("\r\n@ Loop    : dt=%.2f ms, count=%lu, IMU errors=%lu, temp=%.1f C",
               fc->loop_dt_s * 1000.0f, (unsigned long)fc->loop_count,
               (unsigned long)fc->imu_err_count, MPU6050.temperature);
    Debug_PrintGps();
}

/* @QC+GPS? and last line of STATUS: GPS fix, local position, heading */
void Debug_PrintGps(void)
{
#if (GPS_ENABLE == 1)
    const FC_StatusTypeDef *fc = Sys_GetStatus();
    const Gps_DataTypeDef *g = Gps_Get();

    RTOSprintf("\r\n@ GPS     : %s fix=%u sats=%u hdop=%.1f lat=%.7f lon=%.7f alt=%.1f m",
               Gps_FixGood() ? "GOOD" : "no-good", g->fix_quality, g->sats, g->hdop,
               g->lat_deg, g->lon_deg, g->alt_m);
    RTOSprintf("\r\n@ GPS pos : home=%d N=%.2f E=%.2f m | speed N=%.2f E=%.2f m/s | %lu baud, NMEA %lu ok, %lu bad, %lu lost",
               g->has_home, g->pos_n_m, g->pos_e_m, g->vel_n_ms, g->vel_e_ms,
               (unsigned long)g->baud, (unsigned long)g->sentences,
               (unsigned long)g->cksum_errors, (unsigned long)g->rx_overruns);
    RTOSprintf("\r\n@ Heading : %.1f deg (%s, compass %s %.1f%s) | POSHOLD %s%s",
               g->heading_deg, g->heading_ok ? "ok" : "NOT ok",
               g->compass_ok ? "OK" : "none", g->mag_heading_deg,
               Gps_MagCalRunning() ? ", MAGCAL running" : "",
               fc->poshold_mode ? "ON" : "OFF", fc->poshold_engaged ? " (holding)" : "");
#else
    RTOSprintf("\r\n@ GPS     : disabled (GPS_ENABLE 0 in USER_Config.h)");
#endif
}
