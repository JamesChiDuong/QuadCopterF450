/*
 * USER_Gamepad.c
 *
 *  USB host class driver for gamepads (XInput, Xbox One GIP, HID) on top of
 *  the ST USB Host Library, plus the USB task and the HSE clock switch.
 *  See USER_Gamepad.h.
 *
 *  Threading: the USB stack runs only in Gamepad_Task(). The pad state is
 *  published with a short critical section and read with Gamepad_GetState().
 */
/*
 * ============================================================================
 *  OVERVIEW (USER_Gamepad.c)
 * ============================================================================
 *  The real code is only compiled for FC_VARIANT_GAMEPAD (=3). For the other
 *  variants the end of the file has empty "stub" functions so that the rest of
 *  the firmware still links.
 *
 *  What this module does:
 *    1. Gamepad_ClockInit(): moves the PLL source from HSI to HSE 8 MHz so the
 *       48 MHz USB clock is accurate (called once from main(), before the
 *       scheduler starts).
 *    2. It is the USB Host "class driver" for gamepads: it receives data from
 *       the pad plugged into CN6 and decodes it into Gamepad_StateTypeDef
 *       (sticks -1..1, triggers 0..1, GP_BTN_* button bitmask).
 *    3. Gamepad_Task(): the FreeRTOS task "usbTask" (osPriorityAboveNormal,
 *       created in main.c) runs USBH_Process() in a loop every 1 ms
 *       (osDelay(1)).
 *    4. Gamepad_SelfTest(): the @QC+GPTEST command. Runs the decoders on
 *       built-in sample data to check them on the chip (no pad needed).
 *
 *  ---- USB Host layers, bottom to top ----
 *    [OTG_HS hardware + embedded FS PHY]  pins PB14/PB15 (D-/D+), PC4 (VBUS)
 *          |  interrupt OTG_HS_IRQn
 *    [HAL HCD]  Drivers/.../stm32f4xx_hal_hcd.c: drives the hardware host
 *          |    "channels"; one channel = one "pipe" to one device endpoint.
 *    [usbh_conf.c]  glue layer: the library's USBH_LL_xxx functions call down
 *          |        into HAL_HCD_xxx; HAL_HCD_xxx_Callback calls back up.
 *    [ST USB Host core]  Middlewares/ST/STM32_USB_Host_Library/Core: the gState
 *          |        state machine: wait for attach -> port reset -> enumeration
 *          |        (read Device/Config descriptors, set address,
 *          |        SET_CONFIGURATION) -> HOST_CHECK_CLASS ->
 *          |        HOST_CLASS_REQUEST -> HOST_CLASS.
 *    [This file]  class driver: two USBH_ClassTypeDef tables (s_class_vendor,
 *                 s_class_hid) holding the callbacks Init/DeInit/Requests/
 *                 BgndProcess/SOFProcess.
 *
 *  ---- How the class driver is chosen ----
 *    In HOST_CHECK_CLASS the ST library compares the ClassCode of every
 *    registered class with bInterfaceClass of INTERFACE 0 (Itf_Desc[0]):
 *      0xFF (vendor specific) -> s_class_vendor: XInput (subclass 0x5D,
 *                                protocol 0x01) or Xbox One GIP (0x47/0xD0)
 *      0x03 (HID)             -> s_class_hid: Switch Pro (recognised by VID
 *                                057E / PID 2009) or a standard HID gamepad.
 *    No matching class, or Init returning USBH_FAIL -> HOST_ABORT_STATE, and
 *    Gamepad_Task prints a "NOT supported" line with class/subclass/protocol.
 *
 *  ---- The callbacks (called by the library; all in usbTask except SOF) ----
 *    Init        : once after enumeration. Finds the interface, opens the
 *                  interrupt IN pipe (and OUT if present). -> GP_XInputInit/GP_HidInit
 *    Requests    : called repeatedly until it returns USBH_OK. Used to send
 *                  init commands (XInput LED, GIP "start") or to read the HID
 *                  report descriptor over the control pipe. -> GP_Requests
 *    BgndProcess : called continuously once in HOST_CLASS. Submits IN
 *                  requests, waits for the result, decodes reports. -> GP_Process
 *    SOFProcess  : called from the INTERRUPT on every SOF (Start Of Frame,
 *                  1 ms at FS). Only used to time the next IN poll. -> GP_SOF
 *    DeInit      : on unplug. Closes and frees the pipes. -> GP_DeInit
 *
 *  ---- Interrupt IN polling and URB states ----
 *    A USB "interrupt transfer" is still polled by the HOST; the device never
 *    sends on its own. Each call to USBH_InterruptReceiveData() submits one
 *    URB (USB Request Block). Its progress is read with USBH_LL_GetURBState():
 *      USBH_URB_IDLE / NOTREADY : not finished yet, or the device answered NAK
 *                                 (no new data) -> wait for GP_SOF to re-arm.
 *      USBH_URB_DONE            : data received; length from
 *                                 USBH_LL_GetLastXferSize().
 *      USBH_URB_STALL           : endpoint reported an error -> CLEAR_FEATURE.
 *      USBH_URB_ERROR           : bus/transfer error.
 *    Poll period = the endpoint's bInterval (clamped, see GP_OpenPipes()).
 *
 *  ---- The 4 supported protocols ----
 *    XInput (Xbox 360)  : fixed 20-byte report, see XInput_Decode().
 *    Xbox One (GIP)     : the pad only sends 0x20 reports after we send the
 *                         "start" packet 05 20 00 01 00, see XboxOne_Decode().
 *    Standard HID pad   : stick/button positions are NOT fixed; they must be
 *                         read from the "HID report descriptor", see Hid_Parse().
 *    Switch Pro         : needs the handshake 80 01/80 02/80 03/80 02/80 04 and
 *                         then sub-command 03 30 before the pad sends 0x30
 *                         reports, see GP_SwitchPacket() and SwitchPro_Decode().
 *
 *  ---- Data flow and thread safety ----
 *    usbTask: USBH_Process -> GP_Process -> xxx_Decode() writes s_work
 *             -> GP_Publish() copies s_work to s_state in a critical section.
 *    BLDCTask (250 Hz), UI, telemetry: Gamepad_GetState() copies s_state out,
 *             also in a critical section.
 *    Why two copies? The decoders update s_work one field at a time. A task
 *    reading in the middle would see a "half old, half new" state (e.g. new lx
 *    but old ly). Copying the whole struct inside a critical section (interrupts
 *    off for a few us) guarantees readers always see one complete report.
 * ============================================================================
 */
#include "USER_Config.h"
#include "USER_Gamepad.h"

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)

#include "main.h"
#include "usbh_core.h"
#include "usbh_ctlreq.h"
#include "usbh_ioreq.h"
#include "usbh_pipes.h"

/* ========================================================================= */
/* Clock: PLL from HSE so that the 48 MHz USB clock is accurate (+-0.25 %)   */
/* ========================================================================= */
/*
 * Why switch to HSE?
 *   USB Full Speed (12 Mbit/s) requires a clock error of at most +-0.25 %
 *   (2500 ppm). HSI is the chip's internal RC oscillator, up to +-1 % over
 *   temperature/voltage, so a 48 MHz clock derived from it can be too far off:
 *   the pad then works only sometimes (CRC errors, failed enumeration). HSE
 *   8 MHz from the ST-LINK (MCO pin, BYPASS mode) or the X3 crystal (ON mode)
 *   is accurate to a few tens of ppm.
 */
static const char *s_clock_src = "HSI (default)";   /* text describing the clock source in use (for logs) */

/*
 * Gamepad_ClockInit: reconfigure the PLL to run from HSE 8 MHz.
 *   Call from main() right after SystemClock_Config(), before the peripherals
 *   are initialised (changing bus clocks later would break UART baud rates,
 *   timer settings, ...).
 *   Result: SYSCLK 72 MHz, USB 48 MHz, APB1/APB2 exactly as in the old HSI
 *   setup (HCLK/8 and HCLK/16), so no other module needs changes.
 *   Tries HSE_ON (crystal) first, then HSE_BYPASS (square wave from the
 *   ST-LINK MCO). If both fail it goes back to the old HSI PLL (USB may then
 *   be unreliable).
 *   Returns: text describing the clock source (also stored in s_clock_src).
 */
const char *Gamepad_ClockInit(void)
{
    static const uint32_t hse_modes[2] = { RCC_HSE_ON, RCC_HSE_BYPASS };
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    bool ok = false;
    int i;

    /* 1. Run from HSI while the PLL is reconfigured */
    /* (HAL refuses to change the PLL while it is the SYSCLK source, so SYSCLK
     *  is moved to HSI 16 MHz first. The APB dividers stay the same.) */
    clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV8;
    clk.APB2CLKDivider = RCC_HCLK_DIV16;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2) != HAL_OK)
        return s_clock_src;

    /* 2. HSE 8 MHz (ST-LINK MCO or X3 crystal): /8 * 144 = 144 MHz VCO,
     *    /2 = 72 MHz SYSCLK, /3 = 48 MHz USB. Same bus clocks as before. */
    /* Math: 8 MHz / PLLM(8) = 1 MHz -> x PLLN(144) = 144 MHz (VCO)
     *       -> / PLLP(2) = 72 MHz (SYSCLK), / PLLQ(3) = 48 MHz (USB, exact). */
    for (i = 0; (i < 2) && !ok; i++)
    {
        osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        osc.HSEState = hse_modes[i];
        osc.PLL.PLLState = RCC_PLL_ON;
        osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        osc.PLL.PLLM = 8;
        osc.PLL.PLLN = 144;
        osc.PLL.PLLP = RCC_PLLP_DIV2;
        osc.PLL.PLLQ = 3;
        if (HAL_RCC_OscConfig(&osc) == HAL_OK)
        {
            ok = true;
            s_clock_src = (i == 0) ? "HSE 8MHz (crystal/ON)" : "HSE 8MHz (MCO/BYPASS)";
        }
        else
        {
            /* HSE did not start (timeout): switch HSE fully off before trying
             * the next mode, otherwise HAL rejects the ON <-> BYPASS change. */
            RCC_OscInitTypeDef off = {0};
            off.OscillatorType = RCC_OSCILLATORTYPE_HSE;
            off.HSEState = RCC_HSE_OFF;
            off.PLL.PLLState = RCC_PLL_NONE;
            (void)HAL_RCC_OscConfig(&off);
        }
    }

    if (!ok)
    {
        /* Fall back to the original HSI PLL: 16/8*72 = 144 MHz VCO */
        osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        osc.HSIState = RCC_HSI_ON;
        osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        osc.PLL.PLLState = RCC_PLL_ON;
        osc.PLL.PLLSource = RCC_PLLSOURCE_HSI;
        osc.PLL.PLLM = 8;
        osc.PLL.PLLN = 72;
        osc.PLL.PLLP = RCC_PLLP_DIV2;
        osc.PLL.PLLQ = 3;
        (void)HAL_RCC_OscConfig(&osc);
        s_clock_src = "HSI (HSE failed: USB may be unreliable)";
    }

    /* 3. Back to the PLL with the original dividers */
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    (void)HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2);
    return s_clock_src;
}

const char *Gamepad_ClockSource(void)
{
    return s_clock_src;
}

/* ========================================================================= */
/* Shared state                                                              */
/* ========================================================================= */
static Gamepad_StateTypeDef s_state;          /* published copy            */
static Gamepad_StateTypeDef s_work;           /* USB task working copy     */
/* s_work: written only by usbTask (and by GPTEST when no pad is attached).
 * s_state: the "published" copy; other tasks read it via Gamepad_GetState(). */

/* Copy s_work -> s_state. taskENTER_CRITICAL() masks interrupts (up to
 * configMAX_SYSCALL) so no task can run in the middle of the ~60-byte copy. */
static void GP_Publish(void)
{
    taskENTER_CRITICAL();
    s_state = s_work;
    taskEXIT_CRITICAL();
}

/* Gamepad_GetState: take a consistent snapshot of the pad state.
 * Safe from any task (BLDCTask 250 Hz, UI, telemetry). Not for use in an ISR. */
void Gamepad_GetState(Gamepad_StateTypeDef *out)
{
    taskENTER_CRITICAL();
    *out = s_state;
    taskEXIT_CRITICAL();
}

const char *Gamepad_TypeName(GP_TypeDef type)
{
    switch (type)
    {
    case GP_TYPE_XINPUT:  return "XINPUT";
    case GP_TYPE_XBOXONE: return "XBOXONE";
    case GP_TYPE_HID:     return "HID";
    case GP_TYPE_SWITCHPRO: return "SWITCH";
    default:              return "NONE";
    }
}

/* ========================================================================= */
/* HID report descriptor parser (gamepad subset)                             */
/* ========================================================================= */
/*
 * BACKGROUND: the HID report descriptor
 * -------------------------------------
 *  A HID device (class 0x03) sends "input reports" that are just a string of
 *  bits. Where each field sits and what it means is NOT fixed; it is described
 *  by the "report descriptor" (read by the host with GET_DESCRIPTOR, type
 *  0x22). The descriptor is a list of "items":
 *
 *    first byte (prefix) = tttt TT SS
 *        SS   (bits 1..0): number of data bytes that follow: 0,1,2 or 4 (code 3 = 4)
 *        TT   (bits 3..2): type: 0 = Main, 1 = Global, 2 = Local
 *        tttt (bits 7..4): tag
 *    (prefix 0xFE = "long item", rare, skipped.)
 *
 *  Important items (common prefix -> meaning):
 *    05 xx     Usage Page (Global)   : usage group. 01 = Generic Desktop,
 *                                      02 = Simulation, 09 = Button.
 *    09 xx     Usage (Local)         : in Generic Desktop: 30 X, 31 Y, 32 Z,
 *                                      33 Rx, 34 Ry, 35 Rz, 39 Hat switch.
 *    19/29 xx  Usage Min/Max (Local) : a usage range, e.g. Button 1..15.
 *    15 xx     Logical Minimum       : smallest value (SIGNED).
 *    25/26 xx  Logical Maximum       : largest value.
 *    75 xx     Report Size (Global)  : bits per field.
 *    95 xx     Report Count (Global) : number of consecutive fields.
 *    85 xx     Report ID (Global)    : if present, byte 0 of every report is
 *                                      the report ID.
 *    81 xx     Input (Main)          : "creates" Report Count fields of Report
 *                                      Size bits each, appended to the report.
 *                                      Data bit 0 = 1 means Constant (padding
 *                                      bits that carry no data).
 *    A1/C0     Collection/End Collection: grouping only, ignored here.
 *
 *  Global items keep their value until overwritten; Local items (Usage...)
 *  are cleared after every Main item. The parser only has to accumulate a
 *  "bit offset" at each Input: offset += Report Size * Report Count (one
 *  offset per Report ID).
 *
 *  Small example (taken from Gamepad_SelfTest):
 *    05 01        Usage Page = Generic Desktop
 *    09 30 09 31  Usage X, Usage Y
 *    15 00        Logical Min = 0
 *    26 FF 00     Logical Max = 255   (2 bytes: FF 00 = 0x00FF)
 *    75 08 95 02  Report Size = 8 bits, Report Count = 2
 *    81 02        Input (Data, Variable, Absolute)
 *  => X sits at the current bit offset, 8 bits long; Y at offset + 8. After
 *     this item the offset grows by 16.
 *
 *  The result goes into HidMap s_hid: for every axis of interest we remember
 *  report_id, bit_off, bit count and the lmin..lmax range, so Hid_Decode()
 *  can later cut exactly those bits out of a report and scale them to -1..1.
 */
/* Describes one field inside an input report */
typedef struct
{
    bool     valid;
    uint8_t  report_id;
    uint16_t bit_off;
    uint8_t  bits;
    int32_t  lmin;
    int32_t  lmax;
} HidField;

/* Axes we look for in the descriptor (index into HidMap.f[]) */
enum { HF_X = 0, HF_Y, HF_Z, HF_RX, HF_RY, HF_RZ, HF_HAT, HF_BRAKE, HF_GAS, HF_COUNT };

/* "Map" of one HID pad: the result of Hid_Parse() */
typedef struct
{
    HidField f[HF_COUNT];
    HidField btn;              /* first button field: bit_off of button 1 */
    uint8_t  btn_count;
    uint8_t  btn_first;        /* usage of the first button (normally 1)  */
    bool     uses_ids;
    uint8_t  main_id;          /* report id that carries X                */
} HidMap;

static HidMap s_hid;

/* Read an item's data (1/2/4 bytes, little-endian) as UNSIGNED.
 * Example "26 FF 00": p = {FF, 00}, size 2 -> 0x00FF = 255. */
static uint32_t Hid_ItemU(const uint8_t *p, uint8_t size)
{
    switch (size)
    {
    case 1:  return p[0];
    case 2:  return (uint32_t)p[0] | ((uint32_t)p[1] << 8);
    case 4:  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
    default: return 0;
    }
}

/* Same as Hid_ItemU but SIGNED (two's complement). Used for Logical Min/Max.
 * Example "15 81": 0x81 -> (int8_t) = -127. But "25 FF" -> -1 (!), which is
 * why Logical Max is also kept unsigned in lmax_u (see Input in Hid_Parse). */
static int32_t Hid_ItemS(const uint8_t *p, uint8_t size)
{
    switch (size)
    {
    case 1:  return (int8_t)p[0];
    case 2:  return (int16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
    case 4:  return (int32_t)Hid_ItemU(p, 4);
    default: return 0;
    }
}

/* Parse a report descriptor; returns true if it looks like a gamepad */
/*
 * Hid_Parse: walk the whole report descriptor d[0..len-1] and fill HidMap *m.
 *   Returns true if at least X and Y were found (treated as a gamepad).
 *   Only the FIRST field found for each axis is kept. Buttons: the first run
 *   of consecutive 1-bit fields on the Button usage page.
 *   Not supported: Push/Pop, more than 16 single Usages before one Input,
 *   more than 8 Report IDs.
 */
static bool Hid_Parse(const uint8_t *d, uint16_t len, HidMap *m)
{
    /* global state */
    uint16_t page = 0;
    int32_t  lmin = 0, lmax = 0;
    uint32_t lmax_u = 0;
    uint8_t  lmax_size = 0;
    uint8_t  rsize = 0, rcount = 0;
    uint8_t  cur_id = 0;
    /* local state */
    uint32_t usages[16];
    uint8_t  n_usages = 0;
    uint32_t umin = 0, umax = 0;
    bool     have_range = false;
    /* bit offset per report id (ids are usually few and small) */
    struct { uint8_t id; uint16_t off; } offs[8];
    uint8_t  n_offs = 1;
    uint16_t i = 0;

    memset(m, 0, sizeof(*m));
    offs[0].id = 0;
    offs[0].off = 0;

    while (i < len)
    {
        uint8_t prefix = d[i];          /* item header byte: tttt TT SS */
        uint8_t size, type, tag;
        const uint8_t *data;

        if (prefix == 0xFE)                     /* long item: skip */
        {
            if ((i + 2U) >= len)
                break;
            i = (uint16_t)(i + 3U + d[i + 1]);
            continue;
        }
        size = prefix & 0x03U;
        if (size == 3U)
            size = 4U;
        type = (prefix >> 2) & 0x03U;
        tag = (prefix >> 4) & 0x0FU;
        if ((uint32_t)i + 1U + size > len)
            break;
        data = &d[i + 1];

        if (type == 1U)                         /* global */
        {
            switch (tag)
            {
            case 0x0: page = (uint16_t)Hid_ItemU(data, size); break;
            case 0x1: lmin = Hid_ItemS(data, size); break;
            case 0x2: lmax = Hid_ItemS(data, size); lmax_u = Hid_ItemU(data, size); lmax_size = size; break;
            case 0x7: rsize = (uint8_t)Hid_ItemU(data, size); break;
            case 0x9: rcount = (uint8_t)Hid_ItemU(data, size); break;
            case 0x8:                   /* Report ID: switch to (or create) this ID's own bit offset */
            {
                uint8_t k;
                cur_id = (uint8_t)Hid_ItemU(data, size);
                m->uses_ids = true;
                for (k = 0; k < n_offs; k++)
                    if (offs[k].id == cur_id)
                        break;
                if ((k == n_offs) && (n_offs < 8U))
                {
                    offs[n_offs].id = cur_id;
                    offs[n_offs].off = 0;
                    n_offs++;
                }
                break;
            }
            default: break;
            }
        }
        else if (type == 2U)                    /* local */
        {
            uint32_t u = Hid_ItemU(data, size);
            if (size < 4U)
                u |= (uint32_t)page << 16;      /* extended usage */
            /* Usages are stored as (page << 16) | id, e.g. X = 0x00010030,
             * Button 3 = 0x00090003. A 4-byte usage already carries its page. */
            switch (tag)
            {
            case 0x0: if (n_usages < 16U) usages[n_usages++] = u; break;
            case 0x1: umin = u; have_range = true; break;
            case 0x2: umax = u; have_range = true; break;
            default: break;
            }
        }
        else if (type == 0U)                    /* main */
        {
            if (tag == 0x8U)                    /* Input */
            {
                uint8_t k, slot = 0;
                bool is_const = (size > 0U) && (data[0] & 0x01U);
                int32_t fmax = lmax;

                /* logical max is unsigned when min >= 0 (e.g. 0..255 in 1 byte) */
                if ((lmin >= 0) && (lmax_size > 0U))
                    fmax = (int32_t)lmax_u;

                /* find the bit-offset slot of the current report ID */
                for (k = 0; k < n_offs; k++)
                    if (offs[k].id == cur_id)
                        slot = k;

                /* Constant (padding) fields only advance the offset (below) */
                if (!is_const)
                {
                    uint8_t n;
                    for (n = 0; n < rcount; n++)
                    {
                        uint32_t usage;
                        uint16_t bit = (uint16_t)(offs[slot].off + n * rsize);
                        int idx = -1;

                        /* Usage of field n: from the Usage list (the last one
                         * repeats if there are more fields than usages), or
                         * from Usage Min + n when a range was given. */
                        if (n_usages > 0U)
                            usage = usages[(n < n_usages) ? n : (n_usages - 1U)];
                        else if (have_range)
                            usage = umin + n;
                        else
                            continue;
                        if (have_range && (n_usages == 0U) && (usage > umax))
                            continue;

                        if ((usage >> 16) == 0x01U)             /* Generic Desktop */
                        {
                            switch (usage & 0xFFFFU)
                            {
                            case 0x30: idx = HF_X;  break;
                            case 0x31: idx = HF_Y;  break;
                            case 0x32: idx = HF_Z;  break;
                            case 0x33: idx = HF_RX; break;
                            case 0x34: idx = HF_RY; break;
                            case 0x35: idx = HF_RZ; break;
                            case 0x39: idx = HF_HAT; break;
                            default: break;
                            }
                        }
                        else if ((usage >> 16) == 0x02U)        /* Simulation */
                        {
                            if ((usage & 0xFFFFU) == 0xC5U) idx = HF_BRAKE;
                            if ((usage & 0xFFFFU) == 0xC4U) idx = HF_GAS;
                        }
                        else if ((usage >> 16) == 0x09U)        /* Buttons */
                        {
                            if (!m->btn.valid)
                            {
                                m->btn.valid = true;
                                m->btn.report_id = cur_id;
                                m->btn.bit_off = bit;
                                m->btn.bits = rsize;
                                m->btn_first = (uint8_t)(usage & 0xFFU);
                                m->btn_count = 0;
                            }
                            /* count only 1-bit buttons that directly follow the previous one */
                            if ((m->btn.report_id == cur_id) && (rsize == 1U) && (m->btn_count < 32U) &&
                                (bit == (uint16_t)(m->btn.bit_off + m->btn_count)))
                                m->btn_count++;
                        }

                        if ((idx >= 0) && !m->f[idx].valid)
                        {
                            m->f[idx].valid = true;
                            m->f[idx].report_id = cur_id;
                            m->f[idx].bit_off = bit;
                            m->f[idx].bits = rsize;
                            m->f[idx].lmin = lmin;
                            m->f[idx].lmax = fmax;
                        }
                    }
                }
                /* every Input item (data or padding) moves this ID's offset on */
                offs[slot].off = (uint16_t)(offs[slot].off + (uint16_t)rsize * rcount);
            }
            /* any main item clears the local state */
            n_usages = 0;
            have_range = false;
            umin = umax = 0;
        }
        i = (uint16_t)(i + 1U + size);
    }

    m->main_id = m->f[HF_X].report_id;          /* Hid_Decode only accepts this report ID */
    return m->f[HF_X].valid && m->f[HF_Y].valid;
}

/*
 * Hid_GetBits: extract 'bits' bits starting at bit 'off' (LSB first, as HID
 * defines it) from report rep[] of len_bits bits. Bits past the end read as 0.
 * Example: rep = {0x01, 0x08}, off = 11, bits = 1 -> byte 1 (11>>3), bit 3
 * (11&7) of 0x08 -> 1.
 */
static uint32_t Hid_GetBits(const uint8_t *rep, uint16_t len_bits, uint16_t off, uint8_t bits)
{
    uint32_t v = 0;
    uint8_t b;

    for (b = 0; (b < bits) && (b < 32U); b++)
    {
        uint16_t pos = (uint16_t)(off + b);
        if (pos >= len_bits)
            break;
        if (rep[pos >> 3] & (1U << (pos & 7U)))
            v |= (1UL << b);
    }
    return v;
}

/*
 * Hid_FieldValue: raw value of a field. If its Logical Minimum is negative the
 * field is signed, so the top bit is the sign and must be extended to 32 bits.
 * Example: 8-bit field, lmin = -127, raw 0xFF -> top bit set ->
 * 0xFFFFFFFF = -1 (without sign extension it would wrongly read 255).
 */
static int32_t Hid_FieldValue(const HidField *f, const uint8_t *rep, uint16_t len_bits)
{
    uint32_t raw;

    if (f->bits == 0U)
        return 0;
    raw = Hid_GetBits(rep, len_bits, f->bit_off, f->bits);
    if ((f->lmin < 0) && (f->bits < 32U) && (raw & (1UL << (f->bits - 1U))))
        return (int32_t)(raw | (~0UL << f->bits));   /* sign extend */
    return (int32_t)raw;
}

/* -1..1 */
/* Linear map lmin..lmax -> -1..+1. Example 0..255: 0 -> -1, 128 -> +0.004,
 * 255 -> +1. Returns 0 for a missing field. */
static float Hid_Axis(const HidField *f, const uint8_t *rep, uint16_t len_bits)
{
    float v, span;

    if (!f->valid || (f->lmax <= f->lmin))
        return 0.0f;
    v = (float)Hid_FieldValue(f, rep, len_bits);
    span = (float)(f->lmax - f->lmin);
    v = (v - (float)f->lmin) / span * 2.0f - 1.0f;
    if (v > 1.0f) v = 1.0f;
    if (v < -1.0f) v = -1.0f;
    return v;
}

/* Default button numbering = Android / Linux standard HID gamepad:
 * 1 A, 2 B, 3 C, 4 X, 5 Y, 6 Z, 7 L1, 8 R1, 9 L2, 10 R2, 11 Select,
 * 12 Start, 13 Mode/Home, 14 L3, 15 R3. Change in USER_Config.h if needed. */
static const struct { uint8_t hid_btn; uint32_t gp; } s_hid_btn_map[] =
{
    { GAMEPAD_HID_BTN_A,     GP_BTN_A },
    { GAMEPAD_HID_BTN_B,     GP_BTN_B },
    { GAMEPAD_HID_BTN_X,     GP_BTN_X },
    { GAMEPAD_HID_BTN_Y,     GP_BTN_Y },
    { GAMEPAD_HID_BTN_LB,    GP_BTN_LB },
    { GAMEPAD_HID_BTN_RB,    GP_BTN_RB },
    { GAMEPAD_HID_BTN_BACK,  GP_BTN_BACK },
    { GAMEPAD_HID_BTN_START, GP_BTN_START },
    { GAMEPAD_HID_BTN_HOME,  GP_BTN_HOME },
    { GAMEPAD_HID_BTN_LS,    GP_BTN_LS },
    { GAMEPAD_HID_BTN_RS,    GP_BTN_RS },
};

/*
 * Hid_Decode: decode one HID input report (buf, len bytes) into s_work using
 * the map s_hid built by Hid_Parse(). Reports with another report ID are
 * ignored. Y axes are negated because HID Y grows DOWNWARDS, while
 * Gamepad_StateTypeDef wants "up = +".
 *
 * Worked example: the self-test report (descriptor in Gamepad_SelfTest)
 *   03 01 08 02 FF 00 80 FF 00 FF
 *   03        report ID 3 = main_id -> stripped, rep[] starts at "01"
 *   01 08     buttons 1..15 at bits 0..14: 0x0801 -> bit 0 (button 1 = A)
 *             and bit 11 (button 12 = Start); bit 15 is padding
 *   02        hat at bits 16..19 = 2 -> East -> D-pad right (bits 20..23 pad)
 *   FF        X  (bits 24..31) = 255 -> lx = +1
 *   00        Y  = 0   -> -1, negated -> ly = +1 (stick pushed up)
 *   80        Z  = 128 -> rx ~ 0
 *   FF        Rz = 255 -> +1, negated -> ry = -1
 *   00 FF     Brake = 0 -> lt = 0, Gas = 255 -> rt = 1
 */
static void Hid_Decode(const uint8_t *buf, uint16_t len)
{
    const uint8_t *rep = buf;
    uint16_t bits;
    uint8_t k;
    bool right_zrz;

    if (s_hid.uses_ids)
    {
        if ((len < 1U) || (buf[0] != s_hid.main_id))
            return;
        rep = buf + 1;
        len--;
    }
    bits = (uint16_t)(len * 8U);

    s_work.lx = Hid_Axis(&s_hid.f[HF_X], rep, bits);
    s_work.ly = -Hid_Axis(&s_hid.f[HF_Y], rep, bits);

    /* Right stick: most DirectInput/Android pads use Z/Rz; others use Rx/Ry */
    right_zrz = s_hid.f[HF_Z].valid && s_hid.f[HF_RZ].valid;
    if (right_zrz)
    {
        s_work.rx = Hid_Axis(&s_hid.f[HF_Z], rep, bits);
        s_work.ry = -Hid_Axis(&s_hid.f[HF_RZ], rep, bits);
    }
    else
    {
        s_work.rx = Hid_Axis(&s_hid.f[HF_RX], rep, bits);
        s_work.ry = -Hid_Axis(&s_hid.f[HF_RY], rep, bits);
    }

    /* Triggers: Brake/Gas, or Rx/Ry when the right stick uses Z/Rz */
    if (s_hid.f[HF_BRAKE].valid || s_hid.f[HF_GAS].valid)
    {
        s_work.lt = (Hid_Axis(&s_hid.f[HF_BRAKE], rep, bits) + 1.0f) * 0.5f;
        s_work.rt = (Hid_Axis(&s_hid.f[HF_GAS], rep, bits) + 1.0f) * 0.5f;
    }
    else if (right_zrz && s_hid.f[HF_RX].valid && s_hid.f[HF_RY].valid)
    {
        s_work.lt = (Hid_Axis(&s_hid.f[HF_RX], rep, bits) + 1.0f) * 0.5f;
        s_work.rt = (Hid_Axis(&s_hid.f[HF_RY], rep, bits) + 1.0f) * 0.5f;
    }
    else
    {
        s_work.lt = 0.0f;
        s_work.rt = 0.0f;
    }

    /* Buttons */
    /* raw_buttons: HID button N -> bit N-1 (shown as RAW in DEBUG=2, used to
     * find the right GAMEPAD_HID_BTN_* numbers); then translate via the table */
    s_work.raw_buttons = 0;
    if (s_hid.btn.valid)
    {
        for (k = 0; k < s_hid.btn_count; k++)
            if (Hid_GetBits(rep, bits, (uint16_t)(s_hid.btn.bit_off + k), 1))
                s_work.raw_buttons |= 1UL << ((s_hid.btn_first - 1U + k) & 31U);
    }
    s_work.buttons = 0;
    for (k = 0; k < (sizeof(s_hid_btn_map) / sizeof(s_hid_btn_map[0])); k++)
    {
        uint8_t n = s_hid_btn_map[k].hid_btn;
        if ((n >= 1U) && (n <= 32U) && (s_work.raw_buttons & (1UL << (n - 1U))))
            s_work.buttons |= s_hid_btn_map[k].gp;
    }

    /* Hat switch: 0 = N, clockwise in 8 steps, out of range = centred */
    if (s_hid.f[HF_HAT].valid)
    {
        int32_t h = Hid_FieldValue(&s_hid.f[HF_HAT], rep, bits) - s_hid.f[HF_HAT].lmin;
        static const uint32_t hat_map[8] =
        {
            GP_BTN_DUP, GP_BTN_DUP | GP_BTN_DRIGHT, GP_BTN_DRIGHT, GP_BTN_DRIGHT | GP_BTN_DDOWN,
            GP_BTN_DDOWN, GP_BTN_DDOWN | GP_BTN_DLEFT, GP_BTN_DLEFT, GP_BTN_DLEFT | GP_BTN_DUP
        };
        if ((h >= 0) && (h < 8))
            s_work.buttons |= hat_map[h];
    }
}

/* ========================================================================= */
/* XInput / Xbox One decoders                                                */
/* ========================================================================= */
/* Little-endian int16 -> -1..1. Separate divisors so that both -32768 and
 * +32767 map exactly to -1 / +1. */
static float GP_S16(const uint8_t *p)
{
    int16_t v = (int16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
    return (v < 0) ? ((float)v / 32768.0f) : ((float)v / 32767.0f);
}

/* Xbox 360 wired report: 00 14 btnL btnH LT RT LX LY RX RY (int16 LE, Y up) */
/*
 * XInput report layout (20 bytes):
 *   [0]  0x00 = input report (other values: LED/rumble status -> ignored)
 *   [1]  0x14 = length 20
 *   [2]  Up 01, Down 02, Left 04, Right 08, Start 10, Back 20, LS 40, RS 80
 *   [3]  LB 01, RB 02, Guide 04, (unused 08), A 10, B 20, X 40, Y 80
 *   [4]  LT 0..255           [5]  RT 0..255
 *   [6..7] LX  [8..9] LY  [10..11] RX  [12..13] RY : int16 little-endian,
 *          Y already "up = +"
 *   [14..19] unused
 * Example: 00 14 10 10 00 FF 00 80 FF 7F -> Start + A, RT = 1,
 *          LX = 0x8000 = -32768 -> -1, LY = 0x7FFF -> +1.
 */
static void XInput_Decode(const uint8_t *r, uint16_t len)
{
    uint32_t b = 0;

    if ((len < 14U) || (r[0] != 0x00U))
        return;                                 /* LED / rumble status etc. */

    if (r[2] & 0x01U) b |= GP_BTN_DUP;
    if (r[2] & 0x02U) b |= GP_BTN_DDOWN;
    if (r[2] & 0x04U) b |= GP_BTN_DLEFT;
    if (r[2] & 0x08U) b |= GP_BTN_DRIGHT;
    if (r[2] & 0x10U) b |= GP_BTN_START;
    if (r[2] & 0x20U) b |= GP_BTN_BACK;
    if (r[2] & 0x40U) b |= GP_BTN_LS;
    if (r[2] & 0x80U) b |= GP_BTN_RS;
    if (r[3] & 0x01U) b |= GP_BTN_LB;
    if (r[3] & 0x02U) b |= GP_BTN_RB;
    if (r[3] & 0x04U) b |= GP_BTN_HOME;
    if (r[3] & 0x10U) b |= GP_BTN_A;
    if (r[3] & 0x20U) b |= GP_BTN_B;
    if (r[3] & 0x40U) b |= GP_BTN_X;
    if (r[3] & 0x80U) b |= GP_BTN_Y;

    s_work.buttons = b;
    s_work.raw_buttons = (uint32_t)r[2] | ((uint32_t)r[3] << 8);
    s_work.lt = (float)r[4] / 255.0f;
    s_work.rt = (float)r[5] / 255.0f;
    s_work.lx = GP_S16(&r[6]);
    s_work.ly = GP_S16(&r[8]);
    s_work.rx = GP_S16(&r[10]);
    s_work.ry = GP_S16(&r[12]);
}

/* Xbox One GIP input: 20 00 seq len btnL btnH LT(10b) RT(10b) LX LY RX RY */
/*
 * GIP (Gaming Input Protocol) messages start with a command byte:
 *   0x20 = input report: [4..5] buttons, [6..7] LT and [8..9] RT as 10-bit
 *          values 0..1023, [10..17] sticks int16 LE (Y up).
 *   0x07 = Guide (Xbox) button, sent separately: [4] bit 0 = pressed.
 * The Guide bit is therefore kept across 0x20 reports (see the last OR).
 */
static void XboxOne_Decode(const uint8_t *r, uint16_t len)
{
    uint32_t b = 0;

    if ((len >= 5U) && (r[0] == 0x07U))         /* guide button message */
    {
        if (r[4] & 0x01U)
            s_work.buttons |= GP_BTN_HOME;
        else
            s_work.buttons &= ~GP_BTN_HOME;
        return;
    }
    if ((len < 18U) || (r[0] != 0x20U))
        return;

    if (r[4] & 0x04U) b |= GP_BTN_START;
    if (r[4] & 0x08U) b |= GP_BTN_BACK;
    if (r[4] & 0x10U) b |= GP_BTN_A;
    if (r[4] & 0x20U) b |= GP_BTN_B;
    if (r[4] & 0x40U) b |= GP_BTN_X;
    if (r[4] & 0x80U) b |= GP_BTN_Y;
    if (r[5] & 0x01U) b |= GP_BTN_DUP;
    if (r[5] & 0x02U) b |= GP_BTN_DDOWN;
    if (r[5] & 0x04U) b |= GP_BTN_DLEFT;
    if (r[5] & 0x08U) b |= GP_BTN_DRIGHT;
    if (r[5] & 0x10U) b |= GP_BTN_LB;
    if (r[5] & 0x20U) b |= GP_BTN_RB;
    if (r[5] & 0x40U) b |= GP_BTN_LS;
    if (r[5] & 0x80U) b |= GP_BTN_RS;

    s_work.buttons = b | (s_work.buttons & GP_BTN_HOME);
    s_work.raw_buttons = (uint32_t)r[4] | ((uint32_t)r[5] << 8);
    s_work.lt = (float)((uint16_t)r[6] | ((uint16_t)r[7] << 8)) / 1023.0f;
    s_work.rt = (float)((uint16_t)r[8] | ((uint16_t)r[9] << 8)) / 1023.0f;
    s_work.lx = GP_S16(&r[10]);
    s_work.ly = GP_S16(&r[12]);
    s_work.rx = GP_S16(&r[14]);
    s_work.ry = GP_S16(&r[16]);
}

/* Switch Pro stick: 12-bit, centre ~2048, usable travel ~ +-1500 */
/* (Values are not calibrated per pad; anything beyond +-1500 is clamped.) */
static float GP_SwStick(uint16_t v)
{
    float f = ((float)v - 2048.0f) / 1500.0f;

    if (f > 1.0f) f = 1.0f;
    if (f < -1.0f) f = -1.0f;
    return f;
}

/*
 * Switch Pro full input report 0x30 (after the USB handshake):
 *   [0]=0x30 [1]=timer [2]=battery/conn
 *   [3] right: Y 01, X 02, B 04, A 08, SR 10, SL 20, R 40, ZR 80
 *   [4] shared: Minus 01, Plus 02, RStick 04, LStick 08, Home 10, Capture 20
 *   [5] left: Down 01, Up 02, Right 04, Left 08, SR 10, SL 20, L 40, ZL 80
 *   [6..8] left stick 12+12 bit, [9..11] right stick (Y up positive)
 * Buttons are mapped by position to the Xbox names.
 */
/*
 * Positional mapping: Nintendo and Xbox swap the labels A/B and X/Y. We keep
 * the PHYSICAL position, so "the bottom button" is always GP_BTN_A:
 *   Nintendo B (bottom) -> GP_BTN_A     Nintendo A (right) -> GP_BTN_B
 *   Nintendo Y (left)   -> GP_BTN_X     Nintendo X (top)   -> GP_BTN_Y
 *
 * 12-bit stick unpacking: 3 bytes hold two 12-bit values:
 *   byte0 = x[7:0], byte1 = y[3:0] x[11:8] (low nibble = x high bits),
 *   byte2 = y[11:4]
 *   x = byte0 | (byte1 & 0x0F) << 8
 *   y = (byte1 >> 4) | byte2 << 4
 * Example (from the self-test): DC 4D 22
 *   x = 0xDC | 0xD << 8 = 0xDDC = 3548 -> (3548-2048)/1500 = +1.0
 *   y = 0x4  | 0x22 << 4 = 0x224 = 548 -> (548-2048)/1500  = -1.0
 */
static void SwitchPro_Decode(const uint8_t *r, uint16_t len)
{
    uint32_t b = 0;

    if ((len < 12U) || (r[0] != 0x30U))
        return;                                 /* 0x81 / 0x21 replies etc. */

    if (r[3] & 0x01U) b |= GP_BTN_X;            /* Y (left)   */
    if (r[3] & 0x02U) b |= GP_BTN_Y;            /* X (top)    */
    if (r[3] & 0x04U) b |= GP_BTN_A;            /* B (bottom) */
    if (r[3] & 0x08U) b |= GP_BTN_B;            /* A (right)  */
    if (r[3] & 0x40U) b |= GP_BTN_RB;
    if (r[4] & 0x01U) b |= GP_BTN_BACK;         /* Minus */
    if (r[4] & 0x02U) b |= GP_BTN_START;        /* Plus  */
    if (r[4] & 0x04U) b |= GP_BTN_RS;
    if (r[4] & 0x08U) b |= GP_BTN_LS;
    if (r[4] & 0x10U) b |= GP_BTN_HOME;
    if (r[5] & 0x01U) b |= GP_BTN_DDOWN;
    if (r[5] & 0x02U) b |= GP_BTN_DUP;
    if (r[5] & 0x04U) b |= GP_BTN_DRIGHT;
    if (r[5] & 0x08U) b |= GP_BTN_DLEFT;
    if (r[5] & 0x40U) b |= GP_BTN_LB;

    s_work.buttons = b;
    s_work.raw_buttons = (uint32_t)r[3] | ((uint32_t)r[4] << 8) | ((uint32_t)r[5] << 16);
    s_work.lt = (r[5] & 0x80U) ? 1.0f : 0.0f;   /* ZL (digital) */
    s_work.rt = (r[3] & 0x80U) ? 1.0f : 0.0f;   /* ZR (digital) */
    s_work.lx = GP_SwStick((uint16_t)(r[6] | ((r[7] & 0x0FU) << 8)));
    s_work.ly = GP_SwStick((uint16_t)((r[7] >> 4) | ((uint16_t)r[8] << 4)));
    s_work.rx = GP_SwStick((uint16_t)(r[9] | ((r[10] & 0x0FU) << 8)));
    s_work.ry = GP_SwStick((uint16_t)((r[10] >> 4) | ((uint16_t)r[11] << 4)));
}

/* ========================================================================= */
/* On-target self test of the decoders (@QC+GPTEST)                          */
/* ========================================================================= */
/* |a - b| < 0.02: float compare with tolerance */
static bool GP_Near(float a, float b)
{
    return (a - b < 0.02f) && (b - a < 0.02f);
}

/*
 * Gamepad_SelfTest (@QC+GPTEST, runs in UARTTask):
 *   1. Hid_Parse() on a typical gamepad descriptor and check the bit offsets
 *      it finds (buttons @0, hat @16, X @24, Rz @48, gas @64).
 *   2. Hid_Decode() on a matching report (worked example above Hid_Decode).
 *   3. XInput_Decode() and SwitchPro_Decode() on hand-made reports.
 *   Prints one PASS/FAIL line per decoder; returns true if all pass.
 *   Side effects: temporarily overwrites s_work and s_hid (that is why no pad
 *   may be plugged in); s_work is restored and s_hid cleared at the end.
 */
bool Gamepad_SelfTest(void)
{
    /* Typical DirectInput / Android style gamepad with a report ID */
    static const uint8_t rd[] =
    {
        0x05, 0x01, 0x09, 0x05, 0xA1, 0x01,             /* GD, Gamepad, App   */
        0x85, 0x03,                                     /* Report ID 3        */
        0x05, 0x09, 0x19, 0x01, 0x29, 0x0F,             /* Buttons 1..15      */
        0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x0F, 0x81, 0x02,
        0x75, 0x01, 0x95, 0x01, 0x81, 0x03,             /* 1 bit padding      */
        0x05, 0x01, 0x09, 0x39, 0x15, 0x00, 0x25, 0x07, /* Hat 0..7           */
        0x35, 0x00, 0x46, 0x3B, 0x01, 0x65, 0x14,
        0x75, 0x04, 0x95, 0x01, 0x81, 0x42,
        0x75, 0x04, 0x95, 0x01, 0x81, 0x03,             /* 4 bit padding      */
        0x09, 0x01, 0xA1, 0x00,                         /* Pointer, Physical  */
        0x09, 0x30, 0x09, 0x31, 0x09, 0x32, 0x09, 0x35, /* X Y Z Rz           */
        0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x04, 0x81, 0x02,
        0xC0,
        0x05, 0x02, 0x09, 0xC5, 0x09, 0xC4,             /* Brake, Gas         */
        0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x02, 0x81, 0x02,
        0xC0
    };
    /* id, buttons A(1)+Start(12), hat=2 (E), X=255 Y=0 Z=128 Rz=255, brake 0 gas 255 */
    static const uint8_t rep[] = { 0x03, 0x01, 0x08, 0x02, 0xFF, 0x00, 0x80, 0xFF, 0x00, 0xFF };
    /* XInput: Start + A, RT 255, LX -32768, LY +32767 */
    static const uint8_t xr[20] = { 0x00, 0x14, 0x10, 0x10, 0x00, 0xFF, 0x00, 0x80, 0xFF, 0x7F };
    Gamepad_StateTypeDef save;
    bool ok_parse, ok_hid, ok_x;

    if (s_state.connected || (s_state.type != GP_TYPE_NONE) || s_state.unsupported)
    {
        RTOSprintf("\r\n@ GPTEST: unplug the gamepad first (the test reuses the decoder state)");
        return false;
    }
    save = s_work;

    ok_parse = Hid_Parse(rd, sizeof(rd), &s_hid) && s_hid.uses_ids && (s_hid.main_id == 3U) &&
               (s_hid.btn_count == 15U) && (s_hid.btn.bit_off == 0U) &&
               (s_hid.f[HF_HAT].bit_off == 16U) && (s_hid.f[HF_X].bit_off == 24U) &&
               (s_hid.f[HF_RZ].bit_off == 48U) && (s_hid.f[HF_GAS].bit_off == 64U) &&
               (s_hid.f[HF_X].lmax == 255);
    RTOSprintf("\r\n@ GPTEST HID parse : %s (btn %u @%u, hat @%u, X @%u max %ld, Rz @%u, gas @%u)",
               ok_parse ? "PASS" : "FAIL", s_hid.btn_count, s_hid.btn.bit_off,
               s_hid.f[HF_HAT].bit_off, s_hid.f[HF_X].bit_off, (long)s_hid.f[HF_X].lmax,
               s_hid.f[HF_RZ].bit_off, s_hid.f[HF_GAS].bit_off);

    memset(&s_work, 0, sizeof(s_work));
    Hid_Decode(rep, sizeof(rep));
    ok_hid = (s_work.buttons == (GP_BTN_A | GP_BTN_START | GP_BTN_DRIGHT)) &&
             GP_Near(s_work.lx, 1.0f) && GP_Near(s_work.ly, 1.0f) &&
             GP_Near(s_work.rx, 0.0f) && GP_Near(s_work.ry, -1.0f) &&
             GP_Near(s_work.lt, 0.0f) && GP_Near(s_work.rt, 1.0f);
    RTOSprintf("\r\n@ GPTEST HID decode: %s (btn %04lX lx %.2f ly %.2f rx %.2f ry %.2f lt %.2f rt %.2f)",
               ok_hid ? "PASS" : "FAIL", (unsigned long)s_work.buttons, s_work.lx, s_work.ly,
               s_work.rx, s_work.ry, s_work.lt, s_work.rt);

    memset(&s_work, 0, sizeof(s_work));
    XInput_Decode(xr, sizeof(xr));
    ok_x = (s_work.buttons == (GP_BTN_A | GP_BTN_START)) && GP_Near(s_work.rt, 1.0f) &&
           GP_Near(s_work.lx, -1.0f) && GP_Near(s_work.ly, 1.0f) && GP_Near(s_work.rx, 0.0f);
    RTOSprintf("\r\n@ GPTEST XInput    : %s (btn %04lX lx %.2f ly %.2f rt %.2f)",
               ok_x ? "PASS" : "FAIL", (unsigned long)s_work.buttons, s_work.lx, s_work.ly, s_work.rt);

    /* Switch Pro 0x30: Nintendo A (right) + Plus + ZR, left stick x=3548 y=548,
     * right stick centred (2048, 2048) */
    {
        static const uint8_t sw[12] = { 0x30, 0x00, 0x91, 0x88, 0x02, 0x00,
                                        0xDC, 0x4D, 0x22, 0x00, 0x08, 0x80 };
        bool ok_sw;

        memset(&s_work, 0, sizeof(s_work));
        SwitchPro_Decode(sw, sizeof(sw));
        ok_sw = (s_work.buttons == (GP_BTN_B | GP_BTN_START)) && GP_Near(s_work.rt, 1.0f) &&
                GP_Near(s_work.lx, 1.0f) && GP_Near(s_work.ly, -1.0f) &&
                GP_Near(s_work.rx, 0.0f) && GP_Near(s_work.ry, 0.0f);
        RTOSprintf("\r\n@ GPTEST Switch Pro: %s (btn %04lX lx %.2f ly %.2f rx %.2f ry %.2f rt %.2f)",
                   ok_sw ? "PASS" : "FAIL", (unsigned long)s_work.buttons, s_work.lx, s_work.ly,
                   s_work.rx, s_work.ry, s_work.rt);
        ok_x = ok_x && ok_sw;
    }

    /* restore live state (the test runs with no HID pad attached) */
    s_work = save;
    memset(&s_hid, 0, sizeof(s_hid));
    return ok_parse && ok_hid && ok_x;
}

/* ========================================================================= */
/* USB host class driver                                                     */
/* ========================================================================= */
/* State of the Requests callback (runs once after Init, before HOST_CLASS) */
typedef enum
{
    GPR_START = 0,          /* class requests (Requests callback) */
    GPR_INIT_SEND,
    GPR_INIT_WAIT,
    GPR_HID_RD,
    GPR_DONE,
} GP_ReqState;

/* Interrupt IN polling: GPP_GET = submit a new IN request now,
 * GPP_POLL = request submitted, waiting for the URB result. GP_SOF moves
 * POLL -> GET again once the poll interval has passed. */
typedef enum
{
    GPP_GET = 0,            /* background polling */
    GPP_POLL,
} GP_PollState;

/* Per-device class data (only one pad at a time is supported) */
typedef struct
{
    GP_TypeDef type;
    uint8_t    itf_idx;
    uint8_t    itf_num;
    uint8_t    in_ep, out_ep;
    uint8_t    in_pipe, out_pipe;
    uint16_t   in_mps, out_mps;
    uint8_t    poll_ms;
    uint32_t   timer;                   /* phost->Timer (ms) when the last IN was submitted */
    uint32_t   req_timer;               /* timeout for the init packet in GP_Requests */
    uint16_t   rd_len;                  /* HID report descriptor length */
    GP_ReqState  req;
    volatile GP_PollState poll;         /* volatile: also written by GP_SOF in the ISR */
    uint8_t    rx[64];                  /* IN buffer (one FS interrupt packet is at most 64 bytes) */
    uint8_t    tx[16];                  /* OUT buffer (must stay valid while the URB is in flight) */
    /* Switch Pro init script (runs in the background, interleaved with IN) */
    uint8_t    sw_step;
    bool       sw_busy;
    uint32_t   sw_timer;
    uint8_t    sw_counter;
} GP_Handle;

static GP_Handle          s_h;
static USBH_HandleTypeDef s_usbh;

static USBH_StatusTypeDef GP_XInputInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef GP_HidInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef GP_DeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef GP_Requests(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef GP_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef GP_SOF(USBH_HandleTypeDef *phost);

/* Class tables registered with the core. Field order: Name, ClassCode
 * (compared with bInterfaceClass of interface 0), Init, DeInit, Requests,
 * BgndProcess, SOFProcess, pData. Both share the same callbacks; the type is
 * decided inside Init. */
static USBH_ClassTypeDef s_class_vendor =
{
    "XINPUT", 0xFFU, GP_XInputInit, GP_DeInit, GP_Requests, GP_Process, GP_SOF, &s_h
};
static USBH_ClassTypeDef s_class_hid =
{
    "HIDPAD", 0x03U, GP_HidInit, GP_DeInit, GP_Requests, GP_Process, GP_SOF, &s_h
};

/* Open the interrupt IN (and optional OUT) pipes of interface 'idx' */
/*
 * Takes the first interrupt IN and first interrupt OUT endpoint of the
 * interface. Poll period: bInterval (ms at FS) clamped to at least 4 ms,
 * 8 ms if bInterval is out of 1..32. A "pipe" is a host channel of the OTG
 * core bound to (device address, endpoint, type, max packet size).
 * Data toggle starts at DATA0 for a freshly configured endpoint.
 * Returns USBH_FAIL if there is no interrupt IN endpoint.
 */
static USBH_StatusTypeDef GP_OpenPipes(USBH_HandleTypeDef *phost, uint8_t idx)
{
    USBH_InterfaceDescTypeDef *itf = &phost->device.CfgDesc.Itf_Desc[idx];
    uint8_t n = (itf->bNumEndpoints < USBH_MAX_NUM_ENDPOINTS) ? itf->bNumEndpoints : USBH_MAX_NUM_ENDPOINTS;
    uint8_t e;

    s_h.itf_idx = idx;
    s_h.itf_num = itf->bInterfaceNumber;
    s_h.in_ep = 0;
    s_h.out_ep = 0;
    s_h.in_pipe = 0xFF;
    s_h.out_pipe = 0xFF;
    s_h.poll_ms = 8;

    for (e = 0; e < n; e++)
    {
        USBH_EpDescTypeDef *ep = &itf->Ep_Desc[e];
        if ((ep->bmAttributes & 0x03U) != USB_EP_TYPE_INTR)
            continue;
        if ((ep->bEndpointAddress & 0x80U) && (s_h.in_ep == 0U))
        {
            s_h.in_ep = ep->bEndpointAddress;
            s_h.in_mps = ep->wMaxPacketSize;
            if ((ep->bInterval >= 1U) && (ep->bInterval <= 32U))
                s_h.poll_ms = (ep->bInterval < 4U) ? 4U : ep->bInterval;
        }
        else if (!(ep->bEndpointAddress & 0x80U) && (s_h.out_ep == 0U))
        {
            s_h.out_ep = ep->bEndpointAddress;
            s_h.out_mps = ep->wMaxPacketSize;
        }
    }
    if (s_h.in_ep == 0U)
        return USBH_FAIL;
    if (s_h.in_mps > sizeof(s_h.rx))
        s_h.in_mps = sizeof(s_h.rx);

    (void)USBH_SelectInterface(phost, idx);

    s_h.in_pipe = USBH_AllocPipe(phost, s_h.in_ep);
    (void)USBH_OpenPipe(phost, s_h.in_pipe, s_h.in_ep, phost->device.address,
                        phost->device.speed, USB_EP_TYPE_INTR, s_h.in_mps);
    (void)USBH_LL_SetToggle(phost, s_h.in_pipe, 0U);

    if (s_h.out_ep != 0U)
    {
        s_h.out_pipe = USBH_AllocPipe(phost, s_h.out_ep);
        (void)USBH_OpenPipe(phost, s_h.out_pipe, s_h.out_ep, phost->device.address,
                            phost->device.speed, USB_EP_TYPE_INTR, s_h.out_mps);
        (void)USBH_LL_SetToggle(phost, s_h.out_pipe, 0U);
    }

    s_h.req = GPR_START;
    s_h.poll = GPP_GET;
    return USBH_OK;
}

/* Init callback for class 0xFF: find an XInput (FF/5D/01) or Xbox One GIP
 * (FF/47/D0) interface. Real XInput pads have several interfaces (audio,
 * security...); only the one matching subclass/protocol is used. */
static USBH_StatusTypeDef GP_XInputInit(USBH_HandleTypeDef *phost)
{
    uint8_t idx = USBH_FindInterface(phost, 0xFFU, 0x5DU, 0x01U);

    if (idx != 0xFFU)
    {
        s_h.type = GP_TYPE_XINPUT;
    }
    else
    {
        idx = USBH_FindInterface(phost, 0xFFU, 0x47U, 0xD0U);
        if (idx == 0xFFU)
            return USBH_FAIL;                   /* some other vendor device */
        s_h.type = GP_TYPE_XBOXONE;
    }
    return GP_OpenPipes(phost, idx);
}

/* Init callback for class 0x03 (HID): 0xFF = "any" subclass/protocol, so the
 * first HID interface is taken. Switch Pro is told apart by VID/PID because
 * its report descriptor is not usable for decoding (vendor reports). */
static USBH_StatusTypeDef GP_HidInit(USBH_HandleTypeDef *phost)
{
    uint8_t idx = USBH_FindInterface(phost, 0x03U, 0xFFU, 0xFFU);

    if (idx == 0xFFU)
        return USBH_FAIL;

    /* Switch Pro protocol (genuine or emulated, e.g. GameSir in Switch mode) */
    if ((phost->device.DevDesc.idVendor == 0x057EU) && (phost->device.DevDesc.idProduct == 0x2009U))
        s_h.type = GP_TYPE_SWITCHPRO;
    else
        s_h.type = GP_TYPE_HID;

    s_h.sw_step = 0;
    s_h.sw_busy = false;
    s_h.sw_counter = 0;
    return GP_OpenPipes(phost, idx);
}

/*
 * Switch Pro USB init, one packet per step, sent on the interrupt OUT pipe
 * (sequence used by Linux hid-nintendo):
 *   80 01 status, 80 02 handshake, 80 03 3 Mbit, 80 02 handshake,
 *   80 04 "HID only, no timeout", then sub-command 03 30 (full report mode).
 * Replies (0x81 / 0x21) arrive on the IN pipe and are ignored by the decoder.
 */
/*
 * Meaning of the 0x80 USB commands (as documented by the Linux driver /
 * reverse-engineering notes):
 *   80 01  request status (MAC address, controller type)
 *   80 02  handshake
 *   80 03  switch UART link to 3 Mbit/s (internal link of the controller)
 *   80 02  handshake again at the new speed
 *   80 04  "USB HID only": always talk over USB, no Bluetooth timeout
 * Then output report 0x01 with sub-command 0x03 (set input report mode),
 * argument 0x30 = "standard full mode": the pad then streams 0x30 reports
 * (about 125 per second in practice, see README step 7c).
 */
#define SW_STEPS 6U

/* Build handshake packet number 'step' (0..5) into buf; returns its length */
static uint8_t GP_SwitchPacket(uint8_t step, uint8_t *buf)
{
    static const uint8_t usb_cmd[5] = { 0x01, 0x02, 0x03, 0x02, 0x04 };
    static const uint8_t rumble_off[8] = { 0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40 };

    if (step < 5U)
    {
        buf[0] = 0x80;
        buf[1] = usb_cmd[step];
        return 2U;
    }
    /* Output report 0x01: counter, neutral rumble, sub-command 0x03 arg 0x30 */
    /* Layout: [0]=0x01, [1]=packet counter 0..15, [2..9]=rumble data for
     * left/right motor (this pattern = motors idle), [10]=sub-command,
     * [11]=argument. Result: 01 0n 00 01 40 40 00 01 40 40 03 30 */
    buf[0] = 0x01;
    buf[1] = (uint8_t)(s_h.sw_counter++ & 0x0FU);
    memcpy(&buf[2], rumble_off, sizeof(rumble_off));
    buf[10] = 0x03;
    buf[11] = 0x30;
    return 12U;
}

/*
 * GP_SwitchInitStep: called from GP_Process on every pass (every ~1 ms).
 * Non-blocking mini state machine that sends one handshake packet at a time:
 *   idle (sw_busy = false): wait >= 50 ms since the previous step, then
 *                           submit the next packet on the OUT pipe.
 *   busy (sw_busy = true) : wait until the OUT URB finishes (DONE, STALL or
 *                           ERROR) or 200 ms pass, then go to the next step.
 * Errors are not fatal: the sequence just continues. The IN polling keeps
 * running in parallel, so the replies (0x81 / 0x21) are read and dropped.
 */
static void GP_SwitchInitStep(USBH_HandleTypeDef *phost)
{
    USBH_URBStateTypeDef urb;

    if ((s_h.type != GP_TYPE_SWITCHPRO) || (s_h.out_pipe == 0xFFU) || (s_h.sw_step >= SW_STEPS))
        return;

    if (!s_h.sw_busy)
    {
        if ((phost->Timer - s_h.sw_timer) >= 50U)        /* let the pad answer */
        {
            uint8_t n = GP_SwitchPacket(s_h.sw_step, s_h.tx);
            (void)USBH_InterruptSendData(phost, s_h.tx, n, s_h.out_pipe);
            s_h.sw_busy = true;
            s_h.sw_timer = phost->Timer;
        }
    }
    else
    {
        urb = USBH_LL_GetURBState(phost, s_h.out_pipe);
        if ((urb == USBH_URB_DONE) || (urb == USBH_URB_STALL) || (urb == USBH_URB_ERROR) ||
            ((phost->Timer - s_h.sw_timer) > 200U))
        {
            s_h.sw_busy = false;
            s_h.sw_step++;
            s_h.sw_timer = phost->Timer;
            if (s_h.sw_step >= SW_STEPS)
                RTOSprintf("\r\n> USB: Switch Pro handshake sent, waiting for 0x30 reports");
        }
    }
}

/* DeInit callback (device unplugged): close/free pipes so the host channels
 * can be reused by the next device. 0xFF marks "no pipe". */
static USBH_StatusTypeDef GP_DeInit(USBH_HandleTypeDef *phost)
{
    if (s_h.in_pipe != 0xFFU)
    {
        (void)USBH_ClosePipe(phost, s_h.in_pipe);
        (void)USBH_FreePipe(phost, s_h.in_pipe);
        s_h.in_pipe = 0xFF;
    }
    if (s_h.out_pipe != 0xFFU)
    {
        (void)USBH_ClosePipe(phost, s_h.out_pipe);
        (void)USBH_FreePipe(phost, s_h.out_pipe);
        s_h.out_pipe = 0xFF;
    }
    s_h.type = GP_TYPE_NONE;
    return USBH_OK;
}

/* Length of the HID report descriptor, from the raw configuration descriptor */
/*
 * The configuration descriptor is a chain of descriptors, each starting with
 * [bLength][bDescriptorType]. We look for the HID descriptor (type 0x21)
 * that follows OUR interface descriptor (type 0x04, byte 2 = bInterfaceNumber).
 * In the HID descriptor, bytes 7..8 = wDescriptorLength of the report
 * descriptor (little-endian). Returns 0 if not found.
 * (Needs USBH_KEEP_CFG_DESCRIPTOR = 1 in usbh_conf.h so CfgDesc_Raw exists.)
 */
static uint16_t GP_HidReportDescLen(USBH_HandleTypeDef *phost)
{
    const uint8_t *raw = phost->device.CfgDesc_Raw;
    uint16_t total = phost->device.CfgDesc.wTotalLength;
    uint16_t i = 0;
    bool in_itf = false;

    if (total > USBH_MAX_SIZE_CONFIGURATION)
        total = USBH_MAX_SIZE_CONFIGURATION;

    while ((i + 1U) < total)
    {
        uint8_t l = raw[i], t = raw[i + 1];
        if (l < 2U)
            break;
        if (t == USB_DESC_TYPE_INTERFACE)
            in_itf = ((i + 2U) < total) && (raw[i + 2] == s_h.itf_num);
        else if (in_itf && (t == 0x21U) && (l >= 9U) && ((i + 8U) < total))
            return (uint16_t)(raw[i + 7] | ((uint16_t)raw[i + 8] << 8));
        i = (uint16_t)(i + l);
    }
    return 0;
}

/* GET_DESCRIPTOR(HID report) addressed to OUR interface number */
/* Control request: bmRequestType 0x81 (device->host, standard, interface),
 * bRequest 6, wValue 0x2200 (type 0x22 = report descriptor, index 0),
 * wIndex = interface number. The setup packet is filled only on the first
 * call (CMD_SEND); USBH_CtlReq() then returns USBH_BUSY on later calls until
 * the transfer is done (USBH_OK) or fails. Data lands in phost->device.Data. */
static USBH_StatusTypeDef GP_GetReportDesc(USBH_HandleTypeDef *phost, uint16_t len)
{
    if (phost->RequestState == CMD_SEND)
    {
        phost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD;
        phost->Control.setup.b.bRequest = USB_REQ_GET_DESCRIPTOR;
        phost->Control.setup.b.wValue.w = USB_DESC_HID_REPORT;
        phost->Control.setup.b.wIndex.w = s_h.itf_num;
        phost->Control.setup.b.wLength.w = len;
    }
    return USBH_CtlReq(phost, phost->device.Data, len);
}

/*
 * Requests callback (state HOST_CLASS_REQUEST). Called repeatedly by
 * USBH_Process() while it returns USBH_BUSY:
 *   HID        : read and parse the report descriptor; not a gamepad -> FAIL
 *   Switch Pro : nothing here (handshake runs later inside GP_Process)
 *   XInput     : send "player 1 LED" packet; Xbox One: send GIP start packet
 * USBH_OK -> core goes to HOST_CLASS (pad marked connected and published),
 * USBH_FAIL -> HOST_ABORT_STATE ("NOT supported" in Gamepad_Task).
 */
static USBH_StatusTypeDef GP_Requests(USBH_HandleTypeDef *phost)
{
    USBH_StatusTypeDef st;
    USBH_URBStateTypeDef urb;

    switch (s_h.req)
    {
    case GPR_START:
        if (s_h.type == GP_TYPE_HID)
        {
            s_h.rd_len = GP_HidReportDescLen(phost);
            if ((s_h.rd_len == 0U) || (s_h.rd_len > USBH_MAX_DATA_BUFFER))
                s_h.rd_len = USBH_MAX_DATA_BUFFER;
            memset(phost->device.Data, 0, USBH_MAX_DATA_BUFFER);
            s_h.req = GPR_HID_RD;
        }
        else if (s_h.type == GP_TYPE_SWITCHPRO)
        {
            s_h.req = GPR_DONE;                 /* handshake runs in GP_Process */
        }
        else if (s_h.out_pipe != 0xFFU)
        {
            s_h.req = GPR_INIT_SEND;
        }
        else
        {
            s_h.req = GPR_DONE;
        }
        break;

    case GPR_INIT_SEND:
        if (s_h.type == GP_TYPE_XBOXONE)
        {
            /* GIP "power on / start input reports" */
            static const uint8_t gip_start[5] = { 0x05, 0x20, 0x00, 0x01, 0x00 };
            memcpy(s_h.tx, gip_start, sizeof(gip_start));
            (void)USBH_InterruptSendData(phost, s_h.tx, sizeof(gip_start), s_h.out_pipe);
        }
        else
        {
            /* XInput: player-1 LED on (also wakes up some clones) */
            static const uint8_t led[3] = { 0x01, 0x03, 0x06 };
            memcpy(s_h.tx, led, sizeof(led));
            (void)USBH_InterruptSendData(phost, s_h.tx, sizeof(led), s_h.out_pipe);
        }
        s_h.req_timer = phost->Timer;
        s_h.req = GPR_INIT_WAIT;
        break;

    case GPR_INIT_WAIT:
        urb = USBH_LL_GetURBState(phost, s_h.out_pipe);
        if ((urb == USBH_URB_DONE) || (urb == USBH_URB_STALL) || (urb == USBH_URB_ERROR) ||
            ((phost->Timer - s_h.req_timer) > 100U))
            s_h.req = GPR_DONE;                 /* best effort, not fatal */
        break;

    case GPR_HID_RD:
        st = GP_GetReportDesc(phost, s_h.rd_len);
        if (st == USBH_OK)
        {
            if (!Hid_Parse(phost->device.Data, s_h.rd_len, &s_hid))
            {
                s_work.unsupported = true;      /* HID, but not a gamepad */
                return USBH_FAIL;
            }
            s_h.req = GPR_DONE;
        }
        else if ((st == USBH_NOT_SUPPORTED) || (st == USBH_FAIL))
        {
            s_work.unsupported = true;
            return USBH_FAIL;
        }
        break;

    case GPR_DONE:
    default:
        s_work.connected = true;
        s_work.unsupported = false;
        s_work.type = s_h.type;
        GP_Publish();
        return USBH_OK;
    }
    return USBH_BUSY;
}

/*
 * BgndProcess callback (state HOST_CLASS), called each USBH_Process() pass,
 * i.e. about every 1 ms from usbTask:
 *   - advance the Switch Pro handshake if needed;
 *   - GPP_GET : submit an interrupt IN request, go to GPP_POLL;
 *   - GPP_POLL: on URB_DONE decode the report, count it, stamp the time
 *     (last_report_ms is used by the failsafe/UI) and publish.
 */
static USBH_StatusTypeDef GP_Process(USBH_HandleTypeDef *phost)
{
    USBH_URBStateTypeDef urb;
    uint32_t n;

    GP_SwitchInitStep(phost);

    switch (s_h.poll)
    {
    case GPP_GET:
        (void)USBH_InterruptReceiveData(phost, s_h.rx, (uint8_t)s_h.in_mps, s_h.in_pipe);
        s_h.timer = phost->Timer;
        s_h.poll = GPP_POLL;
        break;

    case GPP_POLL:
        urb = USBH_LL_GetURBState(phost, s_h.in_pipe);
        if (urb == USBH_URB_DONE)
        {
            n = USBH_LL_GetLastXferSize(phost, s_h.in_pipe);
            if (n > 0U)
            {
                if (s_h.type == GP_TYPE_XINPUT)
                    XInput_Decode(s_h.rx, (uint16_t)n);
                else if (s_h.type == GP_TYPE_XBOXONE)
                    XboxOne_Decode(s_h.rx, (uint16_t)n);
                else if (s_h.type == GP_TYPE_SWITCHPRO)
                    SwitchPro_Decode(s_h.rx, (uint16_t)n);
                else
                    Hid_Decode(s_h.rx, (uint16_t)n);
                s_work.reports++;
                s_work.last_report_ms = HAL_GetTick();
                GP_Publish();
            }
            s_h.poll = GPP_GET;
        }
        else if (urb == USBH_URB_STALL)
        {
            /* CLEAR_FEATURE(ENDPOINT_HALT) is a control transfer: it returns
             * USBH_BUSY for a few calls, so we stay in GPP_POLL until OK. */
            if (USBH_ClrFeature(phost, s_h.in_ep) == USBH_OK)
                s_h.poll = GPP_GET;
        }
        /* NAK / no data: GP_SOF re-arms the transfer every poll interval */
        break;

    default:
        s_h.poll = GPP_GET;
        break;
    }
    return USBH_OK;
}

/* Called from the USB interrupt on every SOF (1 ms): keep it short */
/* (Chain: OTG_HS_IRQHandler -> HAL_HCD_SOF_Callback -> USBH_LL_IncTimer ->
 *  phost->Timer++ and SOFProcess. So phost->Timer counts milliseconds.) */
static USBH_StatusTypeDef GP_SOF(USBH_HandleTypeDef *phost)
{
    if ((s_h.poll == GPP_POLL) && ((phost->Timer - s_h.timer) >= s_h.poll_ms))
        s_h.poll = GPP_GET;
    return USBH_OK;
}

/* ========================================================================= */
/* Host events + task                                                        */
/* ========================================================================= */
static volatile uint8_t s_last_event;          /* last HOST_USER_xxx event (debug aid) */

/* User callback given to USBH_Init(): the core reports events (SELECT
 * CONFIGURATION, CLASS_ACTIVE, CONNECTION, DISCONNECTION...). On unplug the
 * pad state is cleared immediately -> connected = false -> the flight code
 * sees the loss and goes to failsafe/disarm. */
static void GP_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    (void)phost;
    s_last_event = id;
    if (id == HOST_USER_DISCONNECTION)
    {
        memset(&s_work, 0, sizeof(s_work));
        GP_Publish();
    }
}

/*
 * Gamepad_Task: body of the FreeRTOS task "usbTask" (osPriorityAboveNormal).
 *   Starts the host stack (USBH_Init -> USBH_LL_Init in usbh_conf.c, registers
 *   both class drivers, USBH_Start powers VBUS on CN6), then forever:
 *   USBH_Process() + osDelay(1). USBH_USE_OS = 0, so the library is simply
 *   polled from here instead of using its own OS queue.
 *   It also watches gState changes to print diagnostics once per event:
 *   "ready", "NOT supported" (with class codes of interface 0), "removed".
 *   'reported' prevents repeating the same message.
 */
void Gamepad_Task(void const *argument)
{
    HOST_StateTypeDef last_state = HOST_IDLE;
    bool reported = false;

    (void)argument;

    RTOSprintf("\r\n> USB host: clock %s. Plug the gamepad into CN6 (OTG adapter).",
               s_clock_src);

    memset(&s_work, 0, sizeof(s_work));
    GP_Publish();
    s_h.in_pipe = 0xFF;
    s_h.out_pipe = 0xFF;

    (void)USBH_Init(&s_usbh, GP_UserProcess, HOST_FS);
    (void)USBH_RegisterClass(&s_usbh, &s_class_vendor);
    (void)USBH_RegisterClass(&s_usbh, &s_class_hid);
    (void)USBH_Start(&s_usbh);

    for (;;)
    {
        (void)USBH_Process(&s_usbh);

        /* Diagnostics on state changes */
        if (s_usbh.gState != last_state)
        {
            HOST_StateTypeDef st = s_usbh.gState;

            if ((st == HOST_CHECK_CLASS) || (st == HOST_ABORT_STATE) || (st == HOST_CLASS))
            {
                USBH_InterfaceDescTypeDef *i0 = &s_usbh.device.CfgDesc.Itf_Desc[0];
                taskENTER_CRITICAL();
                s_work.vid = s_usbh.device.DevDesc.idVendor;
                s_work.pid = s_usbh.device.DevDesc.idProduct;
                s_work.itf_class = i0->bInterfaceClass;
                s_work.itf_subclass = i0->bInterfaceSubClass;
                s_work.itf_protocol = i0->bInterfaceProtocol;
                taskEXIT_CRITICAL();
            }
            if (st == HOST_CLASS)
            {
                RTOSprintf("\r\n> USB: gamepad %s ready (VID %04X PID %04X)",
                           Gamepad_TypeName(s_h.type), s_work.vid, s_work.pid);
                reported = true;
            }
            else if ((st == HOST_ABORT_STATE) && !reported)
            {
                s_work.unsupported = true;
                GP_Publish();
                RTOSprintf("\r\n> USB: device VID %04X PID %04X class %02X/%02X/%02X NOT supported.",
                           s_work.vid, s_work.pid, s_work.itf_class, s_work.itf_subclass,
                           s_work.itf_protocol);
                RTOSprintf("\r\n>      Put the pad in Xbox/PC (XInput) mode, re-plug it.");
                reported = true;
            }
            else if (st == HOST_DEV_WAIT_FOR_ATTACHMENT)
            {
                if (reported)
                    RTOSprintf("\r\n> USB: device removed");
                reported = false;
            }
            last_state = st;
        }
        osDelay(1);
    }
}

#else  /* FC_VARIANT != FC_VARIANT_GAMEPAD : stubs */
/* Stubs: other variants never create usbTask; these only keep the linker
 * happy and report "no pad". */

#include <string.h>

const char *Gamepad_ClockInit(void)   { return "HSI"; }
const char *Gamepad_ClockSource(void) { return "HSI"; }
void Gamepad_Task(void const *argument) { (void)argument; for (;;) { } }
void Gamepad_GetState(Gamepad_StateTypeDef *out) { memset(out, 0, sizeof(*out)); }
const char *Gamepad_TypeName(GP_TypeDef type) { (void)type; return "NONE"; }
bool Gamepad_SelfTest(void) { return false; }

#endif /* FC_VARIANT_GAMEPAD */
