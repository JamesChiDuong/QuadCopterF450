/*
 * USER_Config.h
 *
 *  Central place for every tunable parameter of the flight controller.
 *  Change values here instead of hunting through the source files.
 *
 *  Frame convention used everywhere in the firmware (aerospace FRD):
 *      X = forward (nose), Y = right, Z = down
 *      roll  > 0 : right side goes down
 *      pitch > 0 : nose goes up
 *      yaw   > 0 : nose turns right (clockwise seen from above)
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_Config.h)
 * ===========================================================================
 *  Only #defines, no code. Included by almost every USER_*.c file, so after
 *  changing a value rebuild everything. Groups, from top to bottom:
 *    build variant -> gamepad -> LCD -> control loop timing -> motors/ESC
 *    -> mixer signs -> IMU mounting/filter -> RC receiver -> setpoint limits
 *    -> safety -> default PID gains -> telemetry.
 *  Units in the names: _US = microseconds of ESC/RC pulse, _MS = milliseconds,
 *  _DEG = degrees, _DPS = degrees per second, _HZ = hertz, _G = 1 g units.
 *  The PID gains can also be changed at run time over UART (RAM only); copy
 *  the good values back here so they survive a power cycle.
 */

#ifndef INC_USER_CONFIG_H_
#define INC_USER_CONFIG_H_

/* ========================================================================= */
/* BUILD VARIANT  -- choose ONE                                              */
/* ========================================================================= */
/*
 * FC_VARIANT_RC_FULL   : full version. RC transmitter + receiver (RX on
 *                        PE2..PE6). Arm with the CH5 switch, sticks fly.
 *                        UART is used for tuning / telemetry only.
 *
 * FC_VARIANT_UART_TEST : bench test version WITHOUT transmitter/receiver.
 *                        RC input is not initialised at all; the frame is
 *                        armed and driven only by UART commands
 *                        (@QC+ARM, @QC+SP=...). Throttle is limited to
 *                        UART_SP_THROTTLE_MAX_US. PROPS OFF / tethered only.
 *
 * FC_VARIANT_GAMEPAD   : USB gamepad (e.g. GameSir Nova 2 Lite, wired, in
 *                        Xbox/PC = XInput mode) plugged into the USB OTG
 *                        micro-AB connector CN6 through an OTG adapter.
 *                        No RC receiver. Arm/disarm and fly with the pad.
 *                        Supports Switch Pro protocol (GameSir "Switch"
 *                        mode, verified), XInput, Xbox One and HID pads.
 *                        NOTE: PC4 becomes the USB VBUS power switch
 *                        (BLDC3_VCC is no longer driven on PC4).
 *
 * You can also pass -DFC_VARIANT=2 from the compiler settings
 * (Project > Properties > C/C++ Build > Settings > Preprocessor) to keep
 * several build configurations in STM32CubeIDE without editing this file.
 */
/* The Makefile passes FC_VARIANT with "make VARIANT=n"; the #ifndef below is
 * only the default when nothing is passed. */
#define FC_VARIANT_RC_FULL          1
#define FC_VARIANT_UART_TEST        2
#define FC_VARIANT_GAMEPAD          3

#ifndef FC_VARIANT
#define FC_VARIANT                  FC_VARIANT_RC_FULL
#endif

#if (FC_VARIANT != FC_VARIANT_RC_FULL) && (FC_VARIANT != FC_VARIANT_UART_TEST) && \
    (FC_VARIANT != FC_VARIANT_GAMEPAD)
#error "FC_VARIANT must be FC_VARIANT_RC_FULL, FC_VARIANT_UART_TEST or FC_VARIANT_GAMEPAD"
#endif

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
#define FC_VARIANT_NAME             "RC FULL"
#elif (FC_VARIANT == FC_VARIANT_UART_TEST)
#define FC_VARIANT_NAME             "UART TEST"
#else
#define FC_VARIANT_NAME             "GAMEPAD"
#endif

/* ------------------------------------------------------------------------- */
/* USB gamepad (FC_VARIANT_GAMEPAD)                                           */
/* ------------------------------------------------------------------------- */
/*
 * Default mapping ("mode 2", like an RC transmitter):
 *   Left stick  Y : throttle (see GAMEPAD_THROTTLE_MODE)
 *   Left stick  X : yaw rate
 *   Right stick X : roll        Right stick Y : pitch (push up = nose down)
 *   START (Menu)  : ARM   (throttle must be at minimum, frame level)
 *   B or BACK     : DISARM (any time)  -- emergency stop
 *   X             : gyro calibration (disarmed)
 *   Y             : level trim       (disarmed)
 *
 * GAMEPAD_THROTTLE_MODE
 *   0 = "hold": left stick Y CHANGES the throttle, centred stick keeps it
 *       (gamepad sticks spring back to centre, so an absolute mapping would
 *       drop the throttle to 50 % whenever the stick is released).
 *       Throttle is reset to minimum on every arm/disarm.
 *   1 = "trigger": right trigger RT is the absolute throttle
 *       (release = minimum -> dead-man behaviour).
 */
#define GAMEPAD_THROTTLE_MODE       0
/* Higher rate = throttle reacts faster but is harder to hold steady. At 400,
 * going from 1000 to 1400 us takes 1 s of full stick. */
#define GAMEPAD_THR_RATE_US_S       400.0f  /* mode 0: full stick = 400 us/s  */
/* Bigger dead zone = stick drift ignored but less fine control near centre. */
#define GAMEPAD_DEADZONE            0.12f   /* fraction of full stick travel  */
#define GAMEPAD_ARM_HOLD_MS         500U    /* hold START this long to arm    */
/* Event log on the terminal: print a line whenever a button changes or a
 * stick moves by more than GAMEPAD_LOG_AXIS_STEP (rate limited). Toggle at
 * run time with @QC+PADLOG=0/1. */
/* @QC+PADTEST: motor differential (us) for a full stick deflection in the
 * open-loop stick -> motor test. Throttle is capped by MOTOR_TEST_MAX_US. */
#define PADTEST_TORQUE_US           100.0f
#define GAMEPAD_LOG_DEFAULT         1
#define GAMEPAD_LOG_AXIS_STEP       0.05f   /* stick / trigger change to log  */
#define GAMEPAD_LOG_AXIS_PERIOD_MS  50U     /* max 20 stick lines per second  */
#define USB_TASK_STACK_WORDS        512U
/* Pads only send reports when something changes, so silence is NOT a link
 * loss. Failsafe = USB disconnect (cable pulled / pad switched off). */

/* HID gamepads only (XInput/Xbox One pads have a fixed layout): HID button
 * number (1-based) for each function. Default = Android / Linux standard
 * gamepad: 1 A, 2 B, 3 C, 4 X, 5 Y, 6 Z, 7 L1, 8 R1, 9 L2, 10 R2,
 * 11 Select, 12 Start, 13 Home, 14 L3, 15 R3. Use @QC+DEBUG=2 to see which
 * raw bit lights up when you press a button. 0 = not mapped. */
#define GAMEPAD_HID_BTN_A           1
#define GAMEPAD_HID_BTN_B           2
#define GAMEPAD_HID_BTN_X           4
#define GAMEPAD_HID_BTN_Y           5
#define GAMEPAD_HID_BTN_LB          7
#define GAMEPAD_HID_BTN_RB          8
#define GAMEPAD_HID_BTN_BACK        11
#define GAMEPAD_HID_BTN_START       12
#define GAMEPAD_HID_BTN_HOME        13
#define GAMEPAD_HID_BTN_LS          14
#define GAMEPAD_HID_BTN_RS          15

/* ------------------------------------------------------------------------- */
/* On-board LCD (ILI9341 240x320 on STM32F429I-DISC1, driven over SPI5)       */
/* ------------------------------------------------------------------------- */
#ifndef UI_LCD_ENABLE
#define UI_LCD_ENABLE               1       /* 0 = no display task            */
#endif
#define UI_REFRESH_PERIOD_MS        100U    /* 10 Hz                          */
#define UI_TASK_STACK_WORDS         512U
/* ILI9341 MADCTL (memory access control). Bits: 0x80 MY (flip rows),
 * 0x40 MX (flip columns), 0x20 MV (swap X/Y = landscape), 0x08 BGR.
 *   0x48 = portrait, read with the USB-user connector (CN6) at the top
 *   0x88 = portrait rotated 180 deg (MX and MY both toggled): read from the
 *          ST-LINK side, i.e. with the gamepad cable pointing away from you
 * If the picture is mirrored try 0x08 / 0xC8. */
#define LCD_MADCTL_VALUE            0x88U

/* ------------------------------------------------------------------------- */
/* Control loop                                                              */
/* ------------------------------------------------------------------------- */
/* Period of the flight loop (Sys_FlightTask). Shorter = smoother control but
 * the I2C read (~1.6 ms at 100 kHz) and all the maths must fit inside it.
 * IMU_GYRO_CALIB_SAMPLES and IMU_MAX_READ_ERRORS count loops of this period. */
#define CTRL_LOOP_PERIOD_MS         4U      /* 250 Hz control loop            */
#define CTRL_TASK_STACK_WORDS       512U

/* ------------------------------------------------------------------------- */
/* Motor / ESC output (SimonK 30A)                                            */
/* ------------------------------------------------------------------------- */
/* ESC pulse range: 1000 us = stop, 2000 us = full power. Higher PWM frequency
 * = ESC gets new commands sooner (faster reaction); over ~490 Hz SimonK fails. */
#define MOTOR_PWM_FREQ_HZ           400U    /* SimonK accepts up to ~490 Hz   */
#define MOTOR_STOP_US               1000U   /* ESC "zero throttle" pulse      */
/* Idle must be high enough that every motor starts reliably (raise it if one
 * motor stutters when armed), but low enough that the quad stays on the ground.
 * It is also the lowest value the mixer will ever send while ARMED. */
#define MOTOR_IDLE_US               1100U   /* Spin speed when armed, thr low */
#define MOTOR_MAX_US                2000U
/* ESCs need a steady STOP pulse after power-up before they accept throttle. */
#define MOTOR_ESC_ARM_DELAY_MS      3000U   /* Hold STOP pulse at power up    */
#define MOTOR_TEST_MAX_US           1500U   /* Limit for manual test command  */
/* 0 (default): the old ESC "VCC" pins PG15 / PC8 / PC4 / PE14 are left in
 * high impedance. The ESC red wire is a +5 V OUTPUT (BEC) once the battery is
 * plugged in; driving such a pin HIGH made the MCU brown-out reset. ESCs only
 * need SIGNAL + GND. 1 = old behaviour (pins driven HIGH). */
#ifndef MOTOR_ESC_VCC_PINS_HIGH
#define MOTOR_ESC_VCC_PINS_HIGH     0
#endif

/*
 * ESC servo plug position on the DISC1 headers. A 3-pin servo plug sits on
 * three pins of the SAME header row (pin numbers n, n+2, n+4):
 *     SIGNAL  |  +5 V (red, ESC BEC output)  |  GND
 * GND pin   : driven LOW (reference for the PWM signal).
 * "VCC" pin : receives the ESC red +5 V wire -> left in high impedance
 *             (must be a 5 V tolerant "FT" pin, see the datasheet).
 * The PWM signal pins (PB4, PA8, PB0, PB10) are fixed by the timers.
 *
 *   Motor  header pins     SIGNAL  VCC(Hi-Z)  GND(LOW)
 *   M1     P1-25/27/29     PB4     PG15       PG13
 *   M2     P1-53/55/57     PA8     PC8        PC6
 *   M3     P2-28/30/32     PB0     PB2        PF11   (moved: PC4 is the USB
 *                                                     power switch in the
 *                                                     GAMEPAD build)
 *   M4     P2-48/46/44     PB10    PE14       PE12
 */
#define ESC1_GND_PORT   GPIOG
#define ESC1_GND_PIN    GPIO_PIN_13
#define ESC1_VCC_PORT   GPIOG
#define ESC1_VCC_PIN    GPIO_PIN_15

#define ESC2_GND_PORT   GPIOC
#define ESC2_GND_PIN    GPIO_PIN_6
#define ESC2_VCC_PORT   GPIOC
#define ESC2_VCC_PIN    GPIO_PIN_8

#define ESC3_GND_PORT   GPIOF
#define ESC3_GND_PIN    GPIO_PIN_11
#define ESC3_VCC_PORT   GPIOB
#define ESC3_VCC_PIN    GPIO_PIN_2

#define ESC4_GND_PORT   GPIOE
#define ESC4_GND_PIN    GPIO_PIN_12
#define ESC4_VCC_PORT   GPIOE
#define ESC4_VCC_PIN    GPIO_PIN_14

/*
 * Motor layout (X configuration, seen from above, nose up):
 *
 *        M1 (FL, CW)     M3 (FR, CCW)
 *                  \  ^  /
 *                   \ | /
 *                   /   \
 *                  /     \
 *        M2 (BL, CCW)    M4 (BR, CW)
 *
 * Motor POSITIONS on THIS frame (confirmed 2026-11-22, tilt test):
 *   M1 (PB4)  front-left,  CW      M3 (PB0)  front-right, CCW
 *   M2 (PA8)  back-left,   CCW     M4 (PB10) back-right,  CW
 * (The first version assumed M1 FR, M2 FL, M3 BR, M4 BL.)
 *
 * MIX_x = sign of (roll, pitch, yaw) contribution for each motor.
 * Positive roll torque  -> right side down -> LEFT motors faster (M1, M2).
 * Positive pitch torque -> nose up         -> FRONT motors faster (M1, M3).
 * Positive yaw torque   -> nose right      -> CCW props faster (M2, M3):
 *   a CCW prop pushes the frame CW (reaction torque) = nose right.
 * If your props spin the other way, flip the YAW column.
 *
 * Quick bench check (props OFF, armed, throttle a bit above idle): tilt the
 * frame right side down -> the RIGHT motors (M3, M4) must speed up to push it
 * back. If the opposite happens, the sign of that column (or the IMU axis) is
 * wrong and the quad will flip immediately on take-off.
 */
#define MIX_M1_ROLL   (+1.0f)     /* M1 front-left,  CW  */
#define MIX_M1_PITCH  (+1.0f)
#define MIX_M1_YAW    (-1.0f)

#define MIX_M2_ROLL   (+1.0f)     /* M2 back-left,   CCW */
#define MIX_M2_PITCH  (-1.0f)
#define MIX_M2_YAW    (+1.0f)

#define MIX_M3_ROLL   (-1.0f)     /* M3 front-right, CCW */
#define MIX_M3_PITCH  (+1.0f)
#define MIX_M3_YAW    (+1.0f)

#define MIX_M4_ROLL   (-1.0f)     /* M4 back-right,  CW  */
#define MIX_M4_PITCH  (-1.0f)
#define MIX_M4_YAW    (-1.0f)

/* ------------------------------------------------------------------------- */
/* IMU (MPU6050) mounting                                                     */
/* ------------------------------------------------------------------------- */
/*
 * Map sensor axes to body (FRD) axes.
 * THIS frame (measured by tilt test 2026-11-22, layout M1 FL / M3 FR /
 * M2 BL / M4 BR): GY-521 mounted flat, components up, printed X arrow
 * pointing to the TAIL (back, towards M2 / M4):
 *   nose (M1+M3) raised 28 deg   -> sensor X = -0.45 g  => X points back
 *   right (M3+M4) lowered 23 deg -> sensor Y = -0.40 g  => Y points right
 *   lying flat                   -> sensor Z = +1.0 g   => Z points up
 *   body x (forward) = -sx,  body y (right) = +sy,  body z (down) = -sz
 * Other common mountings (components up):
 *   X arrow to the nose : x =  sx, y = -sy, z = -sz
 *   X arrow to the right: x =  sy, y =  sx, z = -sz
 *   X arrow to the left : x = -sy, y = -sx, z = -sz
 * Verify with "@QC+DEBUG=1": tilt right side down -> roll positive,
 * nose up -> pitch positive, turn nose right -> yaw angle increasing.
 */
#define IMU_BODY_X(sx, sy, sz)      (-(sx))
#define IMU_BODY_Y(sx, sy, sz)      ( (sy))
#define IMU_BODY_Z(sx, sy, sz)      (-(sz))

/* More samples = more accurate bias but longer wait at boot. */
#define IMU_GYRO_CALIB_SAMPLES      500U    /* 2 s at 250 Hz                  */
/* Too small: calibration never finishes on a vibrating table (it keeps
 * restarting). Too big: a bias measured while the frame moved is accepted. */
#define IMU_GYRO_CALIB_MAX_DEV_DPS  2.0f    /* Board must be still            */
/* Larger tau = trust the gyro longer: smoother, less vibration noise, but
 * slower to correct gyro drift. Smaller tau = more accel noise in the angle. */
#define IMU_COMP_FILTER_TAU_S       0.5f    /* Complementary filter constant  */
/* Narrower window = accel ignored more often during manoeuvres (safer, more
 * gyro-only time); wider = more accel corrections, but some of them wrong. */
#define IMU_ACC_TRUST_MIN_G         0.85f   /* Ignore accel when not ~1 g     */
#define IMU_ACC_TRUST_MAX_G         1.15f
/* 10 errors x 4 ms = 40 ms of flying "blind" (last motor outputs held)
 * before FAILSAFE stops the motors. */
#define IMU_MAX_READ_ERRORS         10U     /* Consecutive I2C errors -> fail */
#define IMU_RETRY_PERIOD_MS         2000U   /* Re-detect a missing MPU6050    */
/* I2C2 clock for the MPU6050. 100 kHz = original, robust with long wires
 * (14-byte read ~1.6 ms). 400 kHz works with short wires + good pull-ups. */
#ifndef MPU6050_I2C_SPEED_HZ
#define MPU6050_I2C_SPEED_HZ        100000U
#endif

/* ------------------------------------------------------------------------- */
/* RC receiver (6 channel PWM receiver)                                       */
/* ------------------------------------------------------------------------- */
/*
 * Receiver PWM outputs are wired to PE2..PE6 (free pins on STM32F429I-DISC1).
 *   PE2 -> CH1 aileron  (roll)
 *   PE3 -> CH2 elevator (pitch)
 *   PE4 -> CH3 throttle
 *   PE5 -> CH4 rudder   (yaw)
 *   PE6 -> CH5 aux switch (arm)
 * Receiver GND must be common with the board GND.
 */
/* Channel index (0-based) inside the rc_us[] array, not the RX label. */
#define RC_CH_ROLL                  0U
#define RC_CH_PITCH                 1U
#define RC_CH_THROTTLE              2U
#define RC_CH_YAW                   3U
#define RC_CH_ARM                   4U
#define RC_NUM_CHANNELS             5U

/* 1 = invert that stick (use if moving the stick tilts the quad the wrong way). */
#define RC_REVERSE_ROLL             0
#define RC_REVERSE_PITCH            1       /* Stick forward = nose down      */
#define RC_REVERSE_YAW              0

/* Pulses outside this window are treated as glitches and ignored. */
#define RC_PULSE_MIN_VALID_US       900U
#define RC_PULSE_MAX_VALID_US       2100U
/* Shorter = faster failsafe, but a short radio glitch may disarm in flight. */
#define RC_SIGNAL_TIMEOUT_MS        100U    /* No pulse -> signal lost        */
/* Stick zone around 1500 us that reads as exactly 0 (hides centre jitter). */
#define RC_DEADBAND_US              10U
/* Below this throttle: arming is allowed, and while ARMED the PIDs are reset
 * and the motors only idle ("on the ground"). Also used for the gamepad. */
#define RC_THROTTLE_LOW_US          1050U   /* Below this = throttle "off"    */
/* CH5 pulse above this = arm switch ON. */
#define RC_ARM_SWITCH_ON_US         1700U

/* ------------------------------------------------------------------------- */
/* Setpoint limits                                                            */
/* ------------------------------------------------------------------------- */
/* Full stick = these values. Lower them for a calmer quad while learning. */
#define SP_MAX_ANGLE_DEG            30.0f   /* Max roll / pitch command       */
#define SP_MAX_YAW_RATE_DPS         150.0f
/* Fastest the angle loop may ask the quad to rotate (limits how violently it
 * levels itself after a big error). */
#define SP_MAX_RATE_DPS             200.0f  /* Angle loop output clamp        */
/* Kept below MOTOR_MAX_US so that at full throttle the PIDs can still speed
 * up one side: 2000 - 1850 = 150 us of headroom. */
#define THROTTLE_MAX_US             1850U   /* Leave headroom for control     */
#define UART_SP_THROTTLE_MAX_US     1300U   /* Bench test limit (UART source) */
/* UART TEST build: FAILSAFE if no @QC+SP is received for this long while
 * ARMED. 0 = disabled (typing commands by hand). drone_viewer.py --control
 * re-sends the setpoint every second, so 3000 is a good value with it. */
#ifndef UART_SP_TIMEOUT_MS
#define UART_SP_TIMEOUT_MS          0U
#endif

/* ------------------------------------------------------------------------- */
/* Safety                                                                     */
/* ------------------------------------------------------------------------- */
/* Crash tilt must stay well above SP_MAX_ANGLE_DEG (30), otherwise a normal
 * full-stick manoeuvre plus some overshoot would cut the motors in the air. */
#define SAFE_ARM_MAX_TILT_DEG       25.0f   /* Refuse to arm if tilted more   */
#define SAFE_CRASH_TILT_DEG         60.0f   /* Disarm in flight beyond this   */

/* ------------------------------------------------------------------------- */
/* Default PID gains — STARTING POINT ONLY, they must be tuned on the frame.  */
/* Rate loop output unit: microseconds of ESC pulse.                          */
/* ------------------------------------------------------------------------- */
/* Angle loop (P only): wanted rotation speed = Kp * angle error.
 * Example: 30 deg error x 4 = 120 deg/s. Higher = snaps back to level faster
 * but overshoots / wobbles slowly; lower = lazy, drifts. */
#define PID_ANGLE_ROLL_KP           4.0f
#define PID_ANGLE_PITCH_KP          4.0f

/* Rate loop: motor correction (us) from the rotation speed error (deg/s).
 * Example: 120 deg/s error x Kp 0.7 = 84 us. Kp too high -> fast buzzing
 * oscillation; Ki too high -> slow wobble; Kd too high -> hot motors, noise.
 * Tune order: rate Kp, then Kd, then Ki, then angle Kp (README section 6). */
#define PID_RATE_ROLL_KP            0.70f
#define PID_RATE_ROLL_KI            0.50f
#define PID_RATE_ROLL_KD            0.015f

#define PID_RATE_PITCH_KP           0.70f
#define PID_RATE_PITCH_KI           0.50f
#define PID_RATE_PITCH_KD           0.015f

/* Yaw torque comes only from prop drag, which is weak, so yaw needs a larger
 * Kp; D is not needed (yaw is naturally well damped). */
#define PID_RATE_YAW_KP             2.00f
#define PID_RATE_YAW_KI             0.50f
#define PID_RATE_YAW_KD             0.0f

/* Limits (anti-windup / authority). Larger I limit = can fix a bigger
 * imbalance but a bigger jump if it winds up. Yaw is limited more so a yaw
 * command never steals the thrust needed for roll/pitch. */
#define PID_RATE_OUT_LIMIT          300.0f  /* us */
#define PID_RATE_I_LIMIT            100.0f  /* us */
#define PID_YAW_OUT_LIMIT           200.0f  /* us */
/* D-term filter cut-off. Lower = less motor noise in D but more delay
 * (D works less well); higher = faster D but noisier motors. */
#define PID_D_LPF_HZ                40.0f

/* ------------------------------------------------------------------------- */
/* GPS + position hold (optional, all builds)                                 */
/* ------------------------------------------------------------------------- */
/*
 * GPS_ENABLE 0 : no GPS code is compiled, the firmware is exactly as before.
 * GPS_ENABLE 1 : u-blox style NMEA GPS (NEO-6M / M8N / M10, 3.3 V logic) on
 *                UART5: MCU TX = PC12 (P1-43) -> GPS RX,
 *                       MCU RX = PD2  (P1-40) <- GPS TX,
 *                GPS VCC = 5 V (module regulator) and GND. Both pins are
 *                "free I/O" on the DISC1 (UM1670 table 7).
 *
 * What the GPS adds to the controller: POSITION HOLD (POSHOLD mode).
 *   outer loop : position error (m, North/East) x POSHOLD_POS_P -> wanted
 *                ground speed (m/s), limited to POSHOLD_MAX_SPEED_MS
 *   middle loop: speed error x POSHOLD_VEL_P (+ I) -> wanted tilt (deg) in
 *                North/East, rotated into the body frame with the heading,
 *                limited to POSHOLD_MAX_ANGLE_DEG
 *   then the existing angle P -> rate PID -> mixer chain (unchanged).
 * The pilot keeps the THROTTLE (no barometer: no altitude hold) and the yaw.
 * Moving the roll/pitch stick out of the dead band = pilot flies, the hold
 * point follows; centred stick = hold again where the quad stopped.
 *
 * POSHOLD needs: ARMED, throttle above RC_THROTTLE_LOW_US, 3D fix with
 * >= GPS_MIN_SATS satellites and HDOP <= GPS_MAX_HDOP, and a heading.
 * GPS lost for GPS_TIMEOUT_MS while holding -> back to normal angle mode
 * (pilot), never a motor stop.
 *
 * Switch POSHOLD: gamepad RB (toggle), or @QC+POSHOLD=1 / =0 (any build).
 */
#ifndef GPS_ENABLE
#define GPS_ENABLE                  0
#endif

#define GPS_UART_BAUD               9600U   /* factory default of u-blox      */
/* 1 = at boot, configure a u-blox receiver with UBX messages: 38400 baud,
 * GPS_RATE_HZ updates, only GGA + RMC sentences (less UART load). Use 0 for a
 * non u-blox receiver (it must then already output GGA + RMC). */
#define GPS_UBX_CONFIG              1
#define GPS_UBX_BAUD                38400U
#define GPS_RATE_HZ                 5U      /* 1..10                          */

#define GPS_MIN_SATS                6U
#define GPS_MAX_HDOP                2.5f
#define GPS_TIMEOUT_MS              1500U   /* no good fix this long = lost   */

/* Heading (needed to turn "go North" into "pitch forward / roll right"):
 *   0 = compass QMC5883L (on most M8N GPS modules) on the I2C2 bus shared
 *       with the MPU6050 (PF0 SDA / PF1 SCL). Calibrate with @QC+MAGCAL.
 *   1 = no compass: point the NOSE TO NORTH before arming; the heading is
 *       then the gyro yaw (drifts a few deg / minute, short tests only). */
#ifndef GPS_HEADING_SOURCE
#define GPS_HEADING_SOURCE          0
#endif
/* Magnetic declination (true = magnetic + declination). Hanoi / HCMC are
 * about -1 deg; error here only bends the path slightly. */
#define COMPASS_DECLINATION_DEG     (-1.0f)
/* Compass axes -> body FRD, like IMU_BODY_* (module flat, components up,
 * "X" arrow to the nose). Check with @QC+GPS? : nose north -> heading ~0. */
#define MAG_BODY_X(sx, sy, sz)      ( (sx))
#define MAG_BODY_Y(sx, sy, sz)      ( (sy))
#define MAG_BODY_Z(sx, sy, sz)      ( (sz))
/* Heading filter: gyro yaw corrected towards the compass with this time
 * constant (s). Larger = smoother but slower to correct. */
#define COMPASS_TAU_S               2.0f

/* Position hold gains (starting point, tune on a calm day). */
#define POSHOLD_POS_P               0.8f    /* (m/s) per m of position error  */
#define POSHOLD_MAX_SPEED_MS        2.0f    /* fastest return to the point    */
#define POSHOLD_VEL_P               5.0f    /* deg tilt per m/s speed error   */
#define POSHOLD_VEL_I               1.0f    /* deg per (m/s * s): wind        */
#define POSHOLD_VEL_I_LIMIT         8.0f    /* deg (3 m/s wind needs ~6 in sim)*/
#define POSHOLD_MAX_ANGLE_DEG       12.0f   /* tilt limit while holding       */
/* Stick dead band (fraction of full stick) inside which POSHOLD flies. */
#define POSHOLD_STICK_DEADBAND      0.10f

/* ------------------------------------------------------------------------- */
/* Telemetry                                                                  */
/* ------------------------------------------------------------------------- */
#define TELEMETRY_PERIOD_MS         50U     /* 20 Hz                          */
/* DEBUG=6 machine readable line for host/drone_viewer.py:
 * $TLM,<state>,<roll>,<pitch>,<yaw>,<m1>,<m2>,<m3>,<m4>,<throttle>,<variant> */

#endif /* INC_USER_CONFIG_H_ */
