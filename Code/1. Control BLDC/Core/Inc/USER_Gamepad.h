/*
 * USER_Gamepad.h
 *
 *  USB gamepad input for FC_VARIANT_GAMEPAD (USB OTG connector CN6).
 *
 *  Supported devices (auto-detected from the USB descriptors):
 *    - XInput  (Xbox 360 protocol: class 0xFF / 0x5D / 0x01). Most PC pads in
 *              "Xbox / PC" mode, e.g. GameSir Nova 2 Lite with the blue LED.
 *    - Xbox One GIP (class 0xFF / 0x47 / 0xD0).
 *    - Standard HID gamepads (class 0x03), parsed from the report descriptor
 *              (X/Y/Z/Rx/Ry/Rz, hat switch, buttons, brake/gas).
 *    - Nintendo Switch Pro protocol (VID 057E PID 2009, e.g. GameSir pads in
 *              Switch mode): USB handshake + full input report 0x30.
 *              Buttons are mapped by POSITION to Xbox names (Nintendo B,
 *              bottom = GP_BTN_A; Nintendo A, right = GP_BTN_B; Plus = START,
 *              Minus = BACK; ZL/ZR = triggers 0/1).
 */
/*
 * OVERVIEW
 *   Public interface of the USB gamepad module (implementation and a long
 *   explanation of the USB host stack: USER_Gamepad.c).
 *   - main(): Gamepad_ClockInit() once, then creates usbTask = Gamepad_Task.
 *   - BLDCTask / UI / telemetry: Gamepad_GetState() to get a consistent copy
 *     of Gamepad_StateTypeDef (sticks, triggers, buttons, connection state).
 *   - UARTTask: Gamepad_SelfTest() for @QC+GPTEST.
 *   Every protocol is converted to the same "Xbox style" representation, so
 *   the flight code (Sys_ReadGamepad in USER_System.c) does not care which
 *   kind of pad is plugged in.
 *   In non-GAMEPAD builds all functions exist as stubs (no pad, HSI clock).
 */

#ifndef INC_USER_GAMEPAD_H_
#define INC_USER_GAMEPAD_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    GP_TYPE_NONE = 0,
    GP_TYPE_XINPUT,
    GP_TYPE_XBOXONE,
    GP_TYPE_HID,
    GP_TYPE_SWITCHPRO,    /* Nintendo Switch Pro protocol (VID 057E PID 2009) */
} GP_TypeDef;

/* Logical buttons (Xbox naming) */
/* One bit per button in Gamepad_StateTypeDef.buttons; test with e.g.
 * (gp.buttons & GP_BTN_START). */
#define GP_BTN_A        (1UL << 0)
#define GP_BTN_B        (1UL << 1)
#define GP_BTN_X        (1UL << 2)
#define GP_BTN_Y        (1UL << 3)
#define GP_BTN_LB       (1UL << 4)
#define GP_BTN_RB       (1UL << 5)
#define GP_BTN_BACK     (1UL << 6)    /* View / Select / "-"  */
#define GP_BTN_START    (1UL << 7)    /* Menu / Start  / "+"  */
#define GP_BTN_LS       (1UL << 8)
#define GP_BTN_RS       (1UL << 9)
#define GP_BTN_HOME     (1UL << 10)
#define GP_BTN_DUP      (1UL << 11)
#define GP_BTN_DDOWN    (1UL << 12)
#define GP_BTN_DLEFT    (1UL << 13)
#define GP_BTN_DRIGHT   (1UL << 14)

typedef struct
{
    bool       connected;     /* supported pad enumerated and active       */
    bool       unsupported;   /* something is plugged but not understood   */
    GP_TypeDef type;
    uint16_t   vid;
    uint16_t   pid;
    uint8_t    itf_class;     /* bInterfaceClass of interface 0 (diag)     */
    uint8_t    itf_subclass;
    uint8_t    itf_protocol;

    float      lx, ly;        /* left stick,  -1..1, right / up positive   */
    float      rx, ry;        /* right stick, -1..1, right / up positive   */
    float      lt, rt;        /* triggers 0..1                             */
    uint32_t   buttons;       /* GP_BTN_* bitmask                          */
    uint32_t   raw_buttons;   /* HID: button N -> bit N-1 (for remapping)  */

    uint32_t   reports;       /* number of input reports received          */
    uint32_t   last_report_ms;  /* HAL_GetTick() of the last report (ms) */
} Gamepad_StateTypeDef;

/* Optional: switch the PLL to HSE (8 MHz) for an accurate USB clock.
 * Call from main() right after SystemClock_Config(), before peripherals.
 * Returns a short text describing the clock source in use. */
const char *Gamepad_ClockInit(void);
const char *Gamepad_ClockSource(void);

/* USB host task body (never returns). */
void        Gamepad_Task(void const *argument);

/* Thread-safe snapshot of the current pad state. */
void        Gamepad_GetState(Gamepad_StateTypeDef *out);
const char *Gamepad_TypeName(GP_TypeDef type);

/* Runs the HID / XInput decoders on built-in sample data and prints
 * PASS/FAIL on the terminal (@QC+GPTEST). No pad must be attached. */
bool        Gamepad_SelfTest(void);

#endif /* INC_USER_GAMEPAD_H_ */
