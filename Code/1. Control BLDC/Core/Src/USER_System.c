/*
 * USER_System.c
 *
 *  Flight controller core. See USER_System.h for the control structure.
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_System.c)
 * ===========================================================================
 *  Read the OVERVIEW in USER_System.h first (state machine, cascade, arming
 *  conditions, failsafes, build variants). This file is organised as:
 *    Private data   : s_fc (status shown to other tasks), s_pid[5], request
 *                     flags written by the UART task
 *    Helpers        : clamp, stick normalisation, arm / disarm
 *    Input handling : RC receiver, UART setpoints, USB gamepad
 *    State machine  : Sys_UpdateStateMachine()
 *    Control        : Sys_RunControl() = angle P -> rate PID -> mixer -> ESC
 *    Public         : Sys_Init(), Sys_FlightTask() (the 250 Hz loop) and the
 *                     request functions called from other tasks
 *
 *  Threading rule: only the flight task (BLDCTask) changes s_fc.state and
 *  the motors. Other tasks set volatile s_req_* flags or s_uart_* / s_test_us
 *  values, which the flight task picks up on its next cycle.
 */
#include "main.h"
#include "USER_System.h"
#include "USER_Mixer.h"
#include "USER_RC.h"
#include "USER_Motor_Driver.h"
#include "USER_UART_Driver.h"
#include "USER_GPS.h"
#include "USER_PosHold.h"
#include <math.h>

/* ------------------------------------------------------------------------- */
/* Private data                                                              */
/* ------------------------------------------------------------------------- */
static FC_StatusTypeDef s_fc;
static PID_TypeDef      s_pid[PID_ID_COUNT];

static const char *const s_pid_name[PID_ID_COUNT] =
{
    "RA",   /* roll angle  */
    "PA",   /* pitch angle */
    "RR",   /* roll rate   */
    "PR",   /* pitch rate  */
    "YR",   /* yaw rate    */
};

/* Requests from the UART task, consumed by the flight task */
static volatile bool        s_req_arm;
static volatile bool        s_req_disarm;
static volatile const char *s_req_disarm_reason;
static volatile bool        s_req_calib;
static volatile bool        s_req_level;
/* POSHOLD switch request from UART / pad: -1 none, 0 off, 1 on */
static volatile int8_t      s_req_poshold = -1;

#if (GPS_ENABLE == 1)
static PosHold_TypeDef s_ph;
static const PosHold_Gains s_ph_gains =
{
    POSHOLD_POS_P, POSHOLD_MAX_SPEED_MS, POSHOLD_VEL_P,
    POSHOLD_VEL_I, POSHOLD_VEL_I_LIMIT, POSHOLD_MAX_ANGLE_DEG
};
#endif

/* UART setpoint source (bench test) */
static volatile float s_uart_thr_us = MOTOR_STOP_US;
static volatile float s_uart_roll_deg;
static volatile float s_uart_pitch_deg;
static volatile float s_uart_yaw_dps;
static volatile uint32_t s_uart_sp_tick;   /* HAL tick of the last @QC+SP */

/* Manual motor test values (only applied while disarmed) */
static volatile uint16_t s_test_us[MOTOR_COUNT];

/* RC arm switch edge detection */
/* Arming needs an OFF -> ON change of CH5. If the switch is already ON at
 * power up the quad will NOT arm until the pilot switches it OFF and ON. */
static bool s_arm_sw_prev = true;   /* assume ON at boot: must see OFF first */

/* ------------------------------------------------------------------------- */
/* Helpers                                                                   */
/* ------------------------------------------------------------------------- */
static float Sys_Clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* RC pulse -> [-1, 1] with deadband around centre */
/* Example (deadband 10): 1500 -> 0, 1505 -> 0, 1750 -> (250-10)/490 = 0.49,
 * 2000 -> 1.0. Removing the deadband from the value keeps the output
 * continuous (no jump from 0 to 0.02 at the edge of the deadband).
 * reverse = true flips the sign (e.g. pitch: stick forward = nose down). */
static float Sys_StickNorm(uint16_t us, bool reverse)
{
    float d = (float)us - 1500.0f;

    /* 0 = no pulse received on that channel -> treat as centred */
    if (us == 0U)
        return 0.0f;

    if (fabsf(d) < (float)RC_DEADBAND_US)
        d = 0.0f;
    else
        d = (d > 0.0f) ? (d - RC_DEADBAND_US) : (d + RC_DEADBAND_US);

    d = Sys_Clampf(d / (500.0f - RC_DEADBAND_US), -1.0f, 1.0f);
    return reverse ? -d : d;
}

/* Clear all 5 PIDs (integrators, D filters) and the last torque outputs.
 * Called on arm, on disarm and every cycle while the throttle is low, so the
 * I terms never wind up while the quad sits on the ground. */
static void Sys_ResetControllers(void)
{
    uint8_t i;

    for (i = 0; i < PID_ID_COUNT; i++)
        PID_Reset(&s_pid[i]);

    s_fc.out_roll = 0.0f;
    s_fc.out_pitch = 0.0f;
    s_fc.out_yaw = 0.0f;
}

/* Stop all motors at once and go to new_state (DISARMED or FAILSAFE).
 * reason is shown on the LCD / STATUS as last_event (must be a string
 * literal or otherwise live forever: only the pointer is stored). */
static void Sys_Disarm(FC_StateTypeDef new_state, const char *reason)
{
    Motor_StopAll();
    Sys_ResetControllers();
    /* POSHOLD is a flight mode: always off after landing / failsafe */
    s_fc.poshold_mode = false;
    s_fc.poshold_engaged = false;
#if (GPS_ENABLE == 1)
    PosHold_Stop(&s_ph);
#endif
    s_fc.state = new_state;
    s_fc.last_event = reason;
}

/*
 * Check every arming condition, in order; the first failing one is reported.
 *   reason (out, may be NULL): text of the failed check, or "OK".
 * Returns true when it is safe to arm. No side effects.
 */
static bool Sys_CheckArm(const char **reason)
{
    const char *r = NULL;

    if (s_fc.state != FC_STATE_DISARMED)
        r = (s_fc.state == FC_STATE_ARMED) ? "Already armed" : "Not in DISARMED state";
    else if (s_fc.stick_test)
        r = "Exit PADTEST first";      /* never jump from the open-loop test into flight */
    else if (!s_fc.att.calibrated)
        r = "Gyro not calibrated";
    else if ((fabsf(s_fc.att.roll) > SAFE_ARM_MAX_TILT_DEG) ||
             (fabsf(s_fc.att.pitch) > SAFE_ARM_MAX_TILT_DEG))
        r = "Frame is tilted";
    else if ((s_fc.source != FC_SRC_UART) && !s_fc.rc_ok)
        r = (s_fc.source == FC_SRC_GAMEPAD) ? "No gamepad" : "No RC signal";
    else if (s_fc.throttle_us > (float)RC_THROTTLE_LOW_US)
        r = "Throttle not low";

    if (reason != NULL)
        *reason = (r != NULL) ? r : "OK";

    return (r == NULL);
}

/* Enter ARMED. Call only after Sys_CheckArm() returned true. */
static void Sys_Arm(void)
{
    uint8_t i;

    /* Forget any manual motor test value, so it cannot come back on disarm */
    for (i = 0; i < MOTOR_COUNT; i++)
        s_test_us[i] = MOTOR_STOP_US;

    Sys_ResetControllers();
    s_fc.att.yaw = 0.0f;              /* displayed heading starts at 0 */
    s_uart_sp_tick = HAL_GetTick();   /* restart the UART setpoint timeout */
#if (GPS_ENABLE == 1)
    /* Home = arming point (local North/East = 0 here). Without a fix yet,
     * home is taken when POSHOLD first engages. */
    (void)Gps_SetHome();
#if (GPS_HEADING_SOURCE == 1)
    Gps_AlignHeadingNorth();          /* no compass: the nose points North */
#endif
#endif
    s_fc.poshold_mode = false;
    s_fc.poshold_engaged = false;
    s_fc.state = FC_STATE_ARMED;
    s_fc.last_event = "Armed";
}

/* ------------------------------------------------------------------------- */
/* Input handling                                                            */
/* ------------------------------------------------------------------------- */
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
static float    s_gp_thr = MOTOR_STOP_US;    /* throttle held by the pad (mode 0) */
static uint32_t s_gp_prev_btn;
static uint32_t s_gp_start_ms;               /* START pressed since (0 = not)     */
static bool     s_gp_start_armed;            /* START hold already used           */

/* Stick with dead zone, rescaled so the output still reaches +-1 */
/* v in [-1, 1]. Example (dead zone 0.12): 0.10 -> 0, 0.56 -> 0.5, 1 -> 1. */
static float Sys_PadAxis(float v)
{
    float a = fabsf(v);

    if (a < GAMEPAD_DEADZONE)
        return 0.0f;
    a = (a - GAMEPAD_DEADZONE) / (1.0f - GAMEPAD_DEADZONE);
    if (a > 1.0f)
        a = 1.0f;
    return (v < 0.0f) ? -a : a;
}

/* Pure stick -> setpoint mapping (no side effects, also used by the log) */
void Sys_PadToSetpoints(const Gamepad_StateTypeDef *gp, float *roll_deg, float *pitch_deg,
                        float *yaw_rate_dps)
{
    *roll_deg = Sys_PadAxis(gp->rx) * SP_MAX_ANGLE_DEG;
    *pitch_deg = -Sys_PadAxis(gp->ry) * SP_MAX_ANGLE_DEG;      /* push up = nose down */
    *yaw_rate_dps = Sys_PadAxis(gp->lx) * SP_MAX_YAW_RATE_DPS;
}

/*
 * Gamepad -> setpoints + button actions.
 *   START held GAMEPAD_ARM_HOLD_MS : arm request (needs a fresh press)
 *   B or BACK (press)              : disarm request (always honoured)
 *   X / Y (press, disarmed)        : gyro calibration / level trim
 */
/*
 * dt (seconds) is needed for throttle mode 0, where the stick sets the SPEED
 * at which the throttle changes (us/s), so throttle += stick * rate * dt.
 * Side effects: writes s_fc setpoints / throttle and may set s_req_arm,
 * s_req_calib, s_req_level or post a disarm request.
 */
static void Sys_ReadGamepad(float dt)
{
    Gamepad_StateTypeDef *gp = &s_fc.pad;
    uint32_t pressed;

    Gamepad_GetState(gp);             /* copy of the latest USB report */
    s_fc.rc_ok = gp->connected;

    if (!gp->connected)
    {
        /* Pad gone: neutral setpoints, throttle off. The state machine turns
         * this into FAILSAFE if we were ARMED. */
        s_gp_thr = MOTOR_STOP_US;
        s_gp_prev_btn = 0;
        s_gp_start_ms = 0;
        s_gp_start_armed = true;      /* a held START after re-plug must not arm */
        s_fc.throttle_us = MOTOR_STOP_US;
        s_fc.sp_roll_deg = 0.0f;
        s_fc.sp_pitch_deg = 0.0f;
        s_fc.sp_yaw_rate_dps = 0.0f;
        return;
    }

    /* Bits that are 1 now and were 0 last cycle = buttons just pressed */
    pressed = gp->buttons & ~s_gp_prev_btn;
    s_gp_prev_btn = gp->buttons;

    /* Attitude / yaw setpoints: right stick roll/pitch, left stick X yaw */
    Sys_PadToSetpoints(gp, &s_fc.sp_roll_deg, &s_fc.sp_pitch_deg, &s_fc.sp_yaw_rate_dps);

    /* Throttle */
    /* Mode 0 example: stick half up (0.5) for 1 s at 400 us/s -> +200 us.
     * Not armed -> throttle forced to STOP so arming always starts at 0. */
#if (GAMEPAD_THROTTLE_MODE == 0)
    if (s_fc.state == FC_STATE_ARMED)
        s_gp_thr += Sys_PadAxis(gp->ly) * GAMEPAD_THR_RATE_US_S * dt;
    else
        s_gp_thr = MOTOR_STOP_US;
    s_gp_thr = Sys_Clampf(s_gp_thr, (float)MOTOR_STOP_US, (float)THROTTLE_MAX_US);
    s_fc.throttle_us = s_gp_thr;
#else
    (void)dt;
    s_fc.throttle_us = (float)MOTOR_STOP_US +
                       gp->rt * (float)(THROTTLE_MAX_US - MOTOR_STOP_US);
#endif

    /* Disarm: B or BACK, edge triggered */
    if ((pressed & (GP_BTN_B | GP_BTN_BACK)) && (s_fc.state == FC_STATE_ARMED))
        Sys_RequestDisarm("Disarmed by gamepad");

    /* Arm: hold START */
    if (gp->buttons & GP_BTN_START)
    {
        /* "| 1" makes sure the stored time is never 0, because 0 means
         * "START not pressed" */
        if (s_gp_start_ms == 0U)
            s_gp_start_ms = HAL_GetTick() | 1U;
        /* s_gp_start_armed: one long press = one arm request. START must be
         * released before it can arm again. */
        if (!s_gp_start_armed && ((HAL_GetTick() - s_gp_start_ms) >= GAMEPAD_ARM_HOLD_MS))
        {
            s_req_arm = true;
            s_gp_start_armed = true;
        }
    }
    else
    {
        s_gp_start_ms = 0;
        s_gp_start_armed = false;
    }

    /* RB: POSHOLD on / off (toggle, only meaningful while ARMED) */
    if ((pressed & GP_BTN_RB) && (s_fc.state == FC_STATE_ARMED))
        s_req_poshold = s_fc.poshold_mode ? 0 : 1;

    /* Ground utilities */
    if (s_fc.state != FC_STATE_ARMED)
    {
        if (pressed & GP_BTN_X)
            s_req_calib = true;
        if (pressed & GP_BTN_Y)
            s_req_level = true;
    }
}
#endif /* FC_VARIANT_GAMEPAD */

/*
 * Read the pilot inputs of the active source into s_fc:
 * throttle_us, sp_roll_deg, sp_pitch_deg, sp_yaw_rate_dps.
 *   arm_switch_on (out): RC CH5 switch position (always false if not RC).
 *   dt                 : seconds, used only by the gamepad throttle.
 * Called once per flight loop cycle, before the state machine.
 */
static void Sys_ReadInputs(bool *arm_switch_on, float dt)
{
    uint8_t i;

    (void)dt;
    /* RC pulses are always copied (even in the other builds) so that the
     * LCD / telemetry can show them */
    for (i = 0; i < RC_NUM_CHANNELS; i++)
        s_fc.rc_us[i] = RC_GetUs(i);
    s_fc.rc_ok = RC_IsSignalOk();

    *arm_switch_on = s_fc.rc_ok && (s_fc.rc_us[RC_CH_ARM] > RC_ARM_SWITCH_ON_US);

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    /* Gamepad build: the pad replaces the RC; rc_ok is overwritten with
     * "pad connected" inside Sys_ReadGamepad() */
    *arm_switch_on = false;
    Sys_ReadGamepad(dt);
    return;
#endif

    if (s_fc.source == FC_SRC_RC)
    {
        if (s_fc.rc_ok)
        {
            /* Throttle is used directly as a pulse (1000..1850 us); sticks
             * become angles: full stick = +-SP_MAX_ANGLE_DEG (30 deg) */
            s_fc.throttle_us = Sys_Clampf((float)s_fc.rc_us[RC_CH_THROTTLE],
                                          (float)MOTOR_STOP_US, (float)THROTTLE_MAX_US);
            s_fc.sp_roll_deg = Sys_StickNorm(s_fc.rc_us[RC_CH_ROLL], RC_REVERSE_ROLL) * SP_MAX_ANGLE_DEG;
            s_fc.sp_pitch_deg = Sys_StickNorm(s_fc.rc_us[RC_CH_PITCH], RC_REVERSE_PITCH) * SP_MAX_ANGLE_DEG;
            s_fc.sp_yaw_rate_dps = Sys_StickNorm(s_fc.rc_us[RC_CH_YAW], RC_REVERSE_YAW) * SP_MAX_YAW_RATE_DPS;
        }
        else
        {
            /* Signal lost: neutral inputs (the state machine will FAILSAFE) */
            s_fc.throttle_us = MOTOR_STOP_US;
            s_fc.sp_roll_deg = 0.0f;
            s_fc.sp_pitch_deg = 0.0f;
            s_fc.sp_yaw_rate_dps = 0.0f;
        }
    }
    else
    {
        /* UART source: values from @QC+SP, clamped to the bench limits */
        s_fc.throttle_us = Sys_Clampf(s_uart_thr_us, (float)MOTOR_STOP_US,
                                      (float)UART_SP_THROTTLE_MAX_US);
        s_fc.sp_roll_deg = Sys_Clampf(s_uart_roll_deg, -SP_MAX_ANGLE_DEG, SP_MAX_ANGLE_DEG);
        s_fc.sp_pitch_deg = Sys_Clampf(s_uart_pitch_deg, -SP_MAX_ANGLE_DEG, SP_MAX_ANGLE_DEG);
        s_fc.sp_yaw_rate_dps = Sys_Clampf(s_uart_yaw_dps, -SP_MAX_YAW_RATE_DPS, SP_MAX_YAW_RATE_DPS);
    }
}

/* Arm / disarm / failsafe decisions */
/*
 * Runs once per cycle after Sys_ReadInputs(). Handles DISARMED -> ARMED,
 * ARMED -> DISARMED / FAILSAFE and FAILSAFE -> DISARMED (see the list in
 * USER_System.h). CALIBRATING is handled in Sys_FlightTask() instead.
 *   arm_sw_on : RC CH5 position (only meaningful for the RC source)
 */
static void Sys_UpdateStateMachine(bool arm_sw_on)
{
    const char *reason;

    /* Explicit disarm always wins */
    if (s_req_disarm)
    {
        s_req_disarm = false;
        if (s_fc.state == FC_STATE_ARMED)
            Sys_Disarm(FC_STATE_DISARMED, (const char *)s_req_disarm_reason);
    }

    switch (s_fc.state)
    {
    case FC_STATE_DISARMED:
        if (s_fc.source == FC_SRC_RC)
        {
            /* Rising edge of the arm switch */
            if (arm_sw_on && !s_arm_sw_prev)
            {
                if (Sys_CheckArm(&reason))
                    Sys_Arm();
                else
                    s_fc.last_event = reason;
            }
        }
        else if (s_req_arm)
        {
            /* UART @QC+ARM or gamepad START hold */
            if (Sys_CheckArm(&reason))
                Sys_Arm();
            else
                s_fc.last_event = reason;
        }
        break;

    case FC_STATE_ARMED:
        if (s_fc.source == FC_SRC_RC)
        {
            if (!s_fc.rc_ok)
                Sys_Disarm(FC_STATE_FAILSAFE, "FAILSAFE: RC signal lost");
            else if (!arm_sw_on)
                Sys_Disarm(FC_STATE_DISARMED, "Disarmed by switch");
        }
        else if (s_fc.source == FC_SRC_GAMEPAD)
        {
            if (!s_fc.rc_ok)
                Sys_Disarm(FC_STATE_FAILSAFE, "FAILSAFE: gamepad disconnected");
        }
#if (UART_SP_TIMEOUT_MS > 0)
        else if ((s_fc.source == FC_SRC_UART) &&
                 ((HAL_GetTick() - s_uart_sp_tick) > UART_SP_TIMEOUT_MS))
        {
            Sys_Disarm(FC_STATE_FAILSAFE, "FAILSAFE: UART setpoint timeout");
        }
#endif
        /* Crash detection: more than 60 deg of tilt is never a normal flight
         * attitude in angle mode -> stop the props before they hit anything */
        if ((s_fc.state == FC_STATE_ARMED) &&
            ((fabsf(s_fc.att.roll) > SAFE_CRASH_TILT_DEG) ||
             (fabsf(s_fc.att.pitch) > SAFE_CRASH_TILT_DEG)))
        {
            Sys_Disarm(FC_STATE_FAILSAFE, "FAILSAFE: tilt limit (crash?)");
        }
        break;

    case FC_STATE_FAILSAFE:
        /* Leave failsafe only when the pilot has switched the arm switch off
         * (RC) or explicitly via a UART disarm/arm cycle (UART source). */
        /* Gamepad: pad connected again and START not held. Note that this
         * only returns to DISARMED; a NEW arm request is needed to fly
         * (with UART, the first @QC+ARM only clears the failsafe). */
        if (((s_fc.source == FC_SRC_RC) && s_fc.rc_ok && !arm_sw_on) ||
            ((s_fc.source == FC_SRC_UART) && s_req_arm) ||
            ((s_fc.source == FC_SRC_GAMEPAD) && s_fc.rc_ok &&
             !(s_fc.pad.buttons & GP_BTN_START)))
        {
            s_fc.state = FC_STATE_DISARMED;
            s_fc.last_event = "Failsafe cleared";
        }
        break;

    case FC_STATE_CALIBRATING:
    default:
        break;
    }

    /* An arm request is valid for this cycle only (refused requests are not
     * kept for later, otherwise the quad could arm unexpectedly) */
    s_req_arm = false;

    /* Without a valid signal treat the switch as ON, so that a receiver
     * coming back with the switch already ON does NOT create a rising edge. */
    s_arm_sw_prev = s_fc.rc_ok ? arm_sw_on : true;
}

/* ------------------------------------------------------------------------- */
/* Control                                                                   */
/* ------------------------------------------------------------------------- */
/*
 * The cascade, called every cycle while ARMED. dt in seconds.
 * Writes sp_*_rate_dps, out_roll/pitch/yaw, motor_us and the ESC outputs.
 *
 * Worked example (roll, default gains, quad level and not rotating,
 * throttle 1400 us, stick asks +30 deg = right side down):
 *   angle loop : error 30 deg  x Kp 4   = 120 deg/s  (limit 200 deg/s)
 *   rate loop  : error 120 deg/s x Kp 0.7 = 84 us     (+ small I and D)
 *   mixer      : M1, M2 (left) = 1400 + 84 = 1484 us,
 *                M3, M4 (right) = 1400 - 84 = 1316 us
 *   -> left side lifts, the quad rolls right; as roll approaches 30 deg the
 *      angle error and therefore the wanted rate shrink to 0.
 */
#if (GPS_ENABLE == 1)
/*
 * GPS position hold, called by Sys_RunControl() while in the air.
 * Replaces s_fc.sp_roll_deg / sp_pitch_deg when POSHOLD is engaged and the
 * sticks are centred. Never touches throttle or yaw (pilot keeps them).
 */
static void Sys_PosHoldStep(float dt)
{
    const Gps_DataTypeDef *g = Gps_Get();
    float lim = POSHOLD_STICK_DEADBAND * SP_MAX_ANGLE_DEG;
    float roll_sp, pitch_sp;

    if (!s_fc.poshold_mode)
    {
        if (s_fc.poshold_engaged)
        {
            PosHold_Stop(&s_ph);
            s_fc.poshold_engaged = false;
        }
        return;
    }

    if (!Gps_FixGood() || !g->heading_ok)
    {
        /* Lost GPS or heading: the pilot flies (angle mode), never a stop.
         * POSHOLD re-engages by itself when both come back. */
        if (s_fc.poshold_engaged)
        {
            PosHold_Stop(&s_ph);
            s_fc.poshold_engaged = false;
            s_fc.last_event = Gps_FixGood() ? "POSHOLD paused: no heading"
                                            : "POSHOLD paused: GPS lost";
        }
        return;
    }

    if (!g->has_home)
        (void)Gps_SetHome();

    if (!s_ph.active)
    {
        PosHold_Start(&s_ph, g->pos_n_m, g->pos_e_m);
        s_fc.poshold_engaged = true;
        s_fc.last_event = "POSHOLD engaged";
    }

    /* Roll/pitch stick moved: the pilot flies, the hold point follows */
    if ((fabsf(s_fc.sp_roll_deg) > lim) || (fabsf(s_fc.sp_pitch_deg) > lim))
    {
        PosHold_FollowPilot(&s_ph, g->pos_n_m, g->pos_e_m);
        return;
    }

    PosHold_Update(&s_ph, &s_ph_gains, g->pos_n_m, g->pos_e_m, g->vel_n_ms, g->vel_e_ms,
                   g->heading_deg, dt, &roll_sp, &pitch_sp);
    s_fc.sp_roll_deg = roll_sp;
    s_fc.sp_pitch_deg = pitch_sp;
}
#endif

static void Sys_RunControl(float dt)
{
    /* "In the air" is only guessed from the throttle (no altitude sensor) */
    bool in_air = (s_fc.throttle_us > (float)RC_THROTTLE_LOW_US);

    if (!in_air)
    {
        /* On the ground: idle, no integration, no attitude correction */
        Sys_ResetControllers();
        s_fc.sp_roll_rate_dps = 0.0f;
        s_fc.sp_pitch_rate_dps = 0.0f;
        Mixer_QuadX(MOTOR_IDLE_US, 0.0f, 0.0f, 0.0f, MOTOR_IDLE_US, MOTOR_MAX_US, s_fc.motor_us);
        Motor_WriteAllUs(s_fc.motor_us);
#if (GPS_ENABLE == 1)
        /* On the ground: drop the hold point / wind integrator, re-engage
         * at the take-off point */
        if (s_ph.active)
        {
            PosHold_Stop(&s_ph);
            s_fc.poshold_engaged = false;
        }
#endif
        return;
    }

#if (GPS_ENABLE == 1)
    /* GPS position hold may replace the roll / pitch angle setpoints */
    Sys_PosHoldStep(dt);
#endif

    /* Outer loop: angle -> rate setpoint */
    /* integrate = false: the angle PIDs have Ki = 0 anyway (P only) */
    s_fc.sp_roll_rate_dps = PID_Update(&s_pid[PID_ID_ROLL_ANGLE], s_fc.sp_roll_deg,
                                       s_fc.att.roll, dt, false);
    s_fc.sp_pitch_rate_dps = PID_Update(&s_pid[PID_ID_PITCH_ANGLE], s_fc.sp_pitch_deg,
                                        s_fc.att.pitch, dt, false);

    /* Inner loop: rate -> torque command (us) */
    /* Yaw has no angle loop: the yaw stick directly gives a rate setpoint,
     * centred stick = "hold the current heading rate at 0" */
    s_fc.out_roll = PID_Update(&s_pid[PID_ID_ROLL_RATE], s_fc.sp_roll_rate_dps,
                               s_fc.att.roll_rate, dt, true);
    s_fc.out_pitch = PID_Update(&s_pid[PID_ID_PITCH_RATE], s_fc.sp_pitch_rate_dps,
                                s_fc.att.pitch_rate, dt, true);
    s_fc.out_yaw = PID_Update(&s_pid[PID_ID_YAW_RATE], s_fc.sp_yaw_rate_dps,
                              s_fc.att.yaw_rate, dt, true);

    Mixer_QuadX(s_fc.throttle_us, s_fc.out_roll, s_fc.out_pitch, s_fc.out_yaw,
                MOTOR_IDLE_US, MOTOR_MAX_US, s_fc.motor_us);
    Motor_WriteAllUs(s_fc.motor_us);
}

/* Motor output when NOT armed: the manual test values (@QC+MOTOR, default
 * MOTOR_STOP_US) in DISARMED / CALIBRATING, always STOP in FAILSAFE.
 * Written every cycle so the ESCs keep receiving a valid pulse. */
/* PADTEST request from the UART task: -1 none, 0 stop, 1 start. Executed by
 * the flight task in Sys_OutputDisarmed() so only one task changes state. */
static volatile int8_t s_req_stick_test = -1;

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
static float s_st_thr = MOTOR_STOP_US;   /* PADTEST throttle, held like in flight */

static void Sys_StickTestEnd(const char *reason)
{
    s_fc.stick_test = false;
    s_st_thr = MOTOR_STOP_US;
    s_fc.throttle_us = MOTOR_STOP_US;
    s_fc.out_roll = s_fc.out_pitch = s_fc.out_yaw = 0.0f;
    s_fc.last_event = reason;
}

/*
 * One PADTEST cycle (called instead of the normal disarmed output).
 * Open loop: sticks -> torque commands -> the same mixer as in flight, so
 * the motor that speeds up for a stick direction is exactly the one the
 * PID would use first. No IMU needed.
 *   throttle <= RC_THROTTLE_LOW_US : all motors MOTOR_STOP_US (safe start)
 *   otherwise                      : mixer, range MOTOR_IDLE_US..MOTOR_TEST_MAX_US
 * Example: throttle 1300, right stick full right ->
 *   roll torque +100 -> M2/M4 1400, M1/M3 1200.
 */
static void Sys_StickTestStep(void)
{
    const Gamepad_StateTypeDef *gp = &s_fc.pad;     /* refreshed at loop start */
    float dt = s_fc.loop_dt_s;

    if (!gp->connected)
    {
        Sys_StickTestEnd("PADTEST ended: pad disconnected");
        Motor_StopAll();
        return;
    }
    if (gp->buttons & (GP_BTN_B | GP_BTN_BACK))
    {
        Sys_StickTestEnd("PADTEST ended by B/BACK");
        Motor_StopAll();
        return;
    }

    s_st_thr += Sys_PadAxis(gp->ly) * GAMEPAD_THR_RATE_US_S * dt;
    s_st_thr = Sys_Clampf(s_st_thr, (float)MOTOR_STOP_US, (float)MOTOR_TEST_MAX_US);
    s_fc.throttle_us = s_st_thr;

    /* Same sign conventions as the flight setpoints (USER_Config.h) */
    s_fc.out_roll = Sys_PadAxis(gp->rx) * PADTEST_TORQUE_US;
    s_fc.out_pitch = -Sys_PadAxis(gp->ry) * PADTEST_TORQUE_US;   /* push up = nose down */
    s_fc.out_yaw = Sys_PadAxis(gp->lx) * PADTEST_TORQUE_US;

    if (s_st_thr <= (float)RC_THROTTLE_LOW_US)
    {
        uint8_t i;
        for (i = 0; i < MOTOR_COUNT; i++)
            s_fc.motor_us[i] = MOTOR_STOP_US;
    }
    else
    {
        Mixer_QuadX(s_st_thr, s_fc.out_roll, s_fc.out_pitch, s_fc.out_yaw,
                    MOTOR_IDLE_US, MOTOR_TEST_MAX_US, s_fc.motor_us);
    }
    Motor_WriteAllUs(s_fc.motor_us);
}
#endif /* FC_VARIANT_GAMEPAD */

static void Sys_OutputDisarmed(void)
{
    uint8_t i;

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    if (s_req_stick_test == 1)
    {
        s_req_stick_test = -1;
        if (!s_fc.stick_test)
        {
            for (i = 0; i < MOTOR_COUNT; i++)
                s_test_us[i] = MOTOR_STOP_US;      /* forget CONTROL_MOTOR values */
            s_st_thr = MOTOR_STOP_US;              /* always start at zero throttle */
            s_fc.stick_test = true;
            s_fc.last_event = "PADTEST ON - PROPS OFF!";
        }
    }
    else if (s_req_stick_test == 0)
    {
        s_req_stick_test = -1;
        if (s_fc.stick_test)
            Sys_StickTestEnd("PADTEST OFF");
    }

    if (s_fc.stick_test)
    {
        if ((s_fc.state == FC_STATE_DISARMED) || (s_fc.state == FC_STATE_CALIBRATING))
        {
            Sys_StickTestStep();
            return;
        }
        Sys_StickTestEnd("PADTEST ended (state change)");
    }
#endif

    for (i = 0; i < MOTOR_COUNT; i++)
    {
        s_fc.motor_us[i] = ((s_fc.state == FC_STATE_DISARMED) || (s_fc.state == FC_STATE_CALIBRATING)) ? s_test_us[i] : MOTOR_STOP_US;
    }
    Motor_WriteAllUs(s_fc.motor_us);
}

/*
 * Print why the MCU restarted (RCC->CSR reset flags), then clear the flags.
 * Helps to diagnose resets when the battery / motors are connected:
 *   BOR  : supply voltage dipped (brown-out) -> power / ground wiring problem
 *   POR  : full power-on (supply went away completely)
 *   PIN  : NRST pin pulled low (RESET button, ST-LINK, or noise on NRST).
 *          On STM32F4 the PIN flag is also set together with every other
 *          reset source, so "PIN only" means an external reset.
 *   IWDG / WWDG : watchdog,  SFT : software reset,  LPWR : low-power reset
 */
static void Sys_PrintResetCause(void)
{
    uint32_t csr = RCC->CSR;

    RTOSprintf("\r\n> Reset cause:%s%s%s%s%s%s%s",
               (csr & RCC_CSR_BORRSTF)  ? " BOR(brown-out: supply dip!)" : "",
               (csr & RCC_CSR_PORRSTF)  ? " POR(power-on)" : "",
               (csr & RCC_CSR_PINRSTF)  ? " PIN(NRST)" : "",
               (csr & RCC_CSR_IWDGRSTF) ? " IWDG" : "",
               (csr & RCC_CSR_WWDGRSTF) ? " WWDG" : "",
               (csr & RCC_CSR_SFTRSTF)  ? " SOFTWARE" : "",
               (csr & RCC_CSR_LPWRRSTF) ? " LOW-POWER" : "");
    RCC->CSR |= RCC_CSR_RMVF;        /* clear, so the next reset is reported alone */
}

/* ------------------------------------------------------------------------- */
/* Public                                                                    */
/* ------------------------------------------------------------------------- */
/*
 * One-time initialisation, called from main() (USER CODE RTOS_THREADS) after
 * the tasks are created but BEFORE osKernelStart(), so it runs without the
 * scheduler. Blocks for MOTOR_ESC_ARM_DELAY_MS (3 s) inside Motor_Init()
 * while the ESCs receive the STOP pulse. Order: state + source, UART, RC,
 * motors, MPU6050, the 5 PIDs, attitude estimator.
 */
void Sys_Init(void)
{
    uint8_t i;

    memset(&s_fc, 0, sizeof(s_fc));
    s_fc.state = FC_STATE_CALIBRATING;
#if (FC_VARIANT == FC_VARIANT_RC_FULL)
    s_fc.source = FC_SRC_RC;
#elif (FC_VARIANT == FC_VARIANT_GAMEPAD)
    s_fc.source = FC_SRC_GAMEPAD;
#else
    s_fc.source = FC_SRC_UART;
#endif
    s_fc.last_event = "Boot";
    s_fc.throttle_us = MOTOR_STOP_US;

    for (i = 0; i < MOTOR_COUNT; i++)
    {
        s_test_us[i] = MOTOR_STOP_US;
        s_fc.motor_us[i] = MOTOR_STOP_US;
    }

    /* General functions */
    UART_Setup(SET_COMPORT_DEF);
    UART_CheckInterrupt();

    RTOSprintf("\r\n> Build variant: %s", FC_VARIANT_NAME);
    Sys_PrintResetCause();

    RC_Init();          /* UART test build: only the timing counter */

    RTOSprintf("\r\n> Motor init: holding %u us for %u ms (ESC arming)...",
               (unsigned)MOTOR_STOP_US, (unsigned)MOTOR_ESC_ARM_DELAY_MS);
    Motor_Init();

    MPU6050_Initialization();

#if (GPS_ENABLE == 1)
    Gps_Init();         /* UART5 GPS + compass, blocks ~1.5-4 s */
#endif

    /* Controllers */
    /* Angle PIDs: P only, output = rate setpoint clamped to SP_MAX_RATE_DPS.
     * Rate PIDs : full PID, output in us; yaw has its own smaller limit. */
    PID_Init(&s_pid[PID_ID_ROLL_ANGLE], PID_ANGLE_ROLL_KP, 0.0f, 0.0f,
             0.0f, SP_MAX_RATE_DPS, 0.0f);
    PID_Init(&s_pid[PID_ID_PITCH_ANGLE], PID_ANGLE_PITCH_KP, 0.0f, 0.0f,
             0.0f, SP_MAX_RATE_DPS, 0.0f);
    PID_Init(&s_pid[PID_ID_ROLL_RATE], PID_RATE_ROLL_KP, PID_RATE_ROLL_KI, PID_RATE_ROLL_KD,
             PID_RATE_I_LIMIT, PID_RATE_OUT_LIMIT, PID_D_LPF_HZ);
    PID_Init(&s_pid[PID_ID_PITCH_RATE], PID_RATE_PITCH_KP, PID_RATE_PITCH_KI, PID_RATE_PITCH_KD,
             PID_RATE_I_LIMIT, PID_RATE_OUT_LIMIT, PID_D_LPF_HZ);
    PID_Init(&s_pid[PID_ID_YAW_RATE], PID_RATE_YAW_KP, PID_RATE_YAW_KI, PID_RATE_YAW_KD,
             PID_RATE_I_LIMIT, PID_YAW_OUT_LIMIT, PID_D_LPF_HZ);

    Attitude_Init(&s_fc.att);
}

/*
 * Body of the BLDCTask FreeRTOS task (priority High). Never returns.
 * Every CTRL_LOOP_PERIOD_MS (4 ms):
 *   1. measure the real dt with the CPU cycle counter
 *   2. read the MPU6050 (on error: count, failsafe if ARMED too long,
 *      re-detect the sensor if not ARMED)
 *   3. remap sensor axes to body FRD (IMU_BODY_x macros)
 *   4. handle calibration / level requests, run the attitude estimator
 *   5. read inputs -> state machine -> control (ARMED) or stopped motors
 */
void Sys_FlightTask(void const *argument)
{
    TickType_t last_wake;
    uint32_t prev_cyc, now_cyc;
    uint32_t imu_consecutive_err = 0;
    bool arm_sw_on = false;
    float dt;
    Vec3_TypeDef gyro, acc;

    (void)argument;

    RTOSprintf("\r\n> Flight task started. Keep the frame still: calibrating gyro...");

    last_wake = xTaskGetTickCount();
    prev_cyc = Time_Cycles();

    for (;;)
    {
        /* DelayUntil (not Delay) keeps a fixed 4 ms rhythm: the time spent
         * in the loop body is not added to the period */
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(CTRL_LOOP_PERIOD_MS));

        /* Real elapsed time from the DWT cycle counter (unsigned subtraction
         * handles the counter wrap-around) */
        now_cyc = Time_Cycles();
        dt = Time_CyclesToSec(now_cyc - prev_cyc);
        prev_cyc = now_cyc;
        /* Guard against a crazy dt (e.g. after a long blocking call) that
         * would make the integrators / D term jump: 0.5 .. 20 ms */
        dt = Sys_Clampf(dt, 0.0005f, 0.02f);
        s_fc.loop_dt_s = dt;
        s_fc.loop_count++;

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
        /* Keep the pad snapshot fresh for UI / telemetry even when the loop
         * exits early (IMU missing, calibrating) */
        Gamepad_GetState(&s_fc.pad);
        s_fc.rc_ok = s_fc.pad.connected;
#endif

#if (GPS_ENABLE == 1)
        /* Parse GPS bytes, read the compass, update the heading (uses the
         * attitude of the previous cycle, 4 ms old: fine for a heading) */
        Gps_Process(s_fc.att.roll, s_fc.att.pitch, s_fc.att.yaw_rate, dt);
#endif

        /* POSHOLD switch (UART command / pad RB): only while ARMED */
        if (s_req_poshold >= 0)
        {
            if (s_fc.state == FC_STATE_ARMED)
            {
                s_fc.poshold_mode = (s_req_poshold == 1);
                s_fc.last_event = s_fc.poshold_mode ? "POSHOLD on (waiting GPS/air)" : "POSHOLD off";
            }
            s_req_poshold = -1;
        }

        /* ---- Sensor ---- */
        if (!bMPU6050_Present || !MPU6050_ProcessData(&MPU6050))
        {
            s_fc.imu_err_count++;
            imu_consecutive_err++;
            if ((imu_consecutive_err >= IMU_MAX_READ_ERRORS) && (s_fc.state == FC_STATE_ARMED))
                Sys_Disarm(FC_STATE_FAILSAFE, "FAILSAFE: IMU read error");

            if (s_fc.state != FC_STATE_ARMED)
            {
                /* Keep motor test commands working without the IMU */
                Sys_OutputDisarmed();

                /* Sensor missing / lost: try to detect + configure it again */
                /* errors x 4 ms = time without IMU; retry every 2 s */
                if ((imu_consecutive_err * CTRL_LOOP_PERIOD_MS) >= IMU_RETRY_PERIOD_MS)
                {
                    imu_consecutive_err = 0;
                    s_fc.last_event = "IMU not found (retrying)";
                    if (MPU6050_Configure(false))
                    {
                        s_fc.last_event = "IMU found, calibrating";
                        Attitude_Init(&s_fc.att);
                        s_fc.state = FC_STATE_CALIBRATING;
                    }
                    /* MPU6050_Configure() blocks for a while: restart the
                     * timing so the next cycle does not try to "catch up" */
                    last_wake = xTaskGetTickCount();
                    prev_cyc = Time_Cycles();
                }
            }
            continue;   /* armed: keep last motor outputs for a few missed samples */
        }
        imu_consecutive_err = 0;

        /* Sensor axes -> body FRD axes (mounting defined in USER_Config.h).
         * gyro in deg/s, acc in g. */
        gyro.x = IMU_BODY_X(MPU6050.gyro_x, MPU6050.gyro_y, MPU6050.gyro_z);
        gyro.y = IMU_BODY_Y(MPU6050.gyro_x, MPU6050.gyro_y, MPU6050.gyro_z);
        gyro.z = IMU_BODY_Z(MPU6050.gyro_x, MPU6050.gyro_y, MPU6050.gyro_z);
        acc.x  = IMU_BODY_X(MPU6050.acc_x, MPU6050.acc_y, MPU6050.acc_z);
        acc.y  = IMU_BODY_Y(MPU6050.acc_x, MPU6050.acc_y, MPU6050.acc_z);
        acc.z  = IMU_BODY_Z(MPU6050.acc_x, MPU6050.acc_y, MPU6050.acc_z);

        /* ---- Requests that need the frame on the ground ---- */
        if (s_req_calib)
        {
            s_req_calib = false;
            if (s_fc.state != FC_STATE_ARMED)
            {
                Attitude_StartCalibration(&s_fc.att);
                s_fc.state = FC_STATE_CALIBRATING;
                s_fc.last_event = "Calibrating gyro";
            }
        }

        if (s_fc.state == FC_STATE_CALIBRATING)
        {
            Sys_OutputDisarmed();
            Attitude_Update(&s_fc.att, &gyro, &acc, dt);   /* keeps accel angle fresh */
            if (Attitude_CalibrationStep(&s_fc.att, &gyro, IMU_GYRO_CALIB_SAMPLES,
                                         IMU_GYRO_CALIB_MAX_DEV_DPS))
            {
                s_fc.state = FC_STATE_DISARMED;
                s_fc.last_event = "Gyro calibrated";
            }
            /* Inputs / state machine are skipped: arming is impossible
             * while calibrating */
            continue;
        }

        Attitude_Update(&s_fc.att, &gyro, &acc, dt);

        if (s_req_level)
        {
            s_req_level = false;
            if (s_fc.state != FC_STATE_ARMED)
            {
                Attitude_SetLevel(&s_fc.att);
                s_fc.last_event = "Level trim saved (RAM)";
            }
        }

        /* ---- Inputs, state machine, outputs ---- */
        Sys_ReadInputs(&arm_sw_on, dt);
        Sys_UpdateStateMachine(arm_sw_on);

        if (s_fc.state == FC_STATE_ARMED)
            Sys_RunControl(dt);
        else
            Sys_OutputDisarmed();
    }
}

const FC_StatusTypeDef *Sys_GetStatus(void)
{
    return &s_fc;
}

const char *Sys_StateName(FC_StateTypeDef state)
{
    switch (state)
    {
    case FC_STATE_CALIBRATING: return "CALIBRATING";
    case FC_STATE_DISARMED:    return "DISARMED";
    case FC_STATE_ARMED:       return "ARMED";
    case FC_STATE_FAILSAFE:    return "FAILSAFE";
    default:                   return "?";
    }
}

/* UART @QC+ARM. Accepted in the UART TEST and GAMEPAD builds; the RC build
 * refuses (the CH5 switch must stay the only way to arm, otherwise the switch
 * position would no longer match the state). In the GAMEPAD build the pad
 * must be connected: throttle, sticks, B = disarm and the unplug failsafe
 * still come from the pad, the command only replaces "hold START".
 * Sys_CheckArm still applies (gyro calibrated, tilt < 25 deg, throttle low,
 * no PADTEST). Also used to acknowledge FAILSAFE in the UART build. */
bool Sys_RequestArm(const char **reason)
{
    if (s_fc.source == FC_SRC_RC)
    {
        *reason = "RC FULL build: arm with the CH5 switch";
        return false;
    }
    if ((s_fc.source == FC_SRC_GAMEPAD) && !s_fc.rc_ok)
    {
        *reason = "GAMEPAD build: connect the pad first";
        return false;
    }
    if ((s_fc.state != FC_STATE_DISARMED) && (s_fc.state != FC_STATE_FAILSAFE))
    {
        *reason = "Not disarmed";
        return false;
    }
    s_req_arm = true;
    *reason = "Arm requested (check STATUS)";
    return true;
}

/* Disarm request from any source (UART, gamepad B/BACK). Always accepted.
 * reason must be a string that stays valid (only the pointer is kept). */
void Sys_RequestDisarm(const char *reason)
{
    s_req_disarm_reason = reason;
    s_req_disarm = true;
    /* Also kill test outputs immediately */
    Sys_MotorTestStop();
}

bool Sys_RequestCalibration(const char **reason)
{
    if (s_fc.state == FC_STATE_ARMED)
    {
        *reason = "Disarm first";
        return false;
    }
    s_req_calib = true;
    *reason = "Calibrating, keep the frame still";
    return true;
}

bool Sys_RequestLevel(const char **reason)
{
    if ((s_fc.state == FC_STATE_ARMED) || !s_fc.att.calibrated)
    {
        *reason = "Must be disarmed and calibrated";
        return false;
    }
    s_req_level = true;
    *reason = "Level trim requested";
    return true;
}

/* @QC+SP=... (UART TEST build). Units: us, deg, deg, deg/s. Values are
 * clamped later in Sys_ReadInputs(); the time stamp feeds the UART failsafe. */
void Sys_SetUartSetpoint(float throttle_us, float roll_deg, float pitch_deg, float yaw_rate_dps)
{
    s_uart_thr_us = throttle_us;
    s_uart_roll_deg = roll_deg;
    s_uart_pitch_deg = pitch_deg;
    s_uart_yaw_dps = yaw_rate_dps;
    s_uart_sp_tick = HAL_GetTick();
}

/*
 * Manual motor test (@QC+MOTOR), only when not ARMED / FAILSAFE.
 *   motor    : 1..4 = M1..M4, 5 = all motors
 *   pulse_us : clamped to MOTOR_STOP_US .. MOTOR_TEST_MAX_US (1000..1500)
 * The value is applied by Sys_OutputDisarmed() on the next flight cycle.
 */
bool Sys_MotorTest(uint8_t motor, uint16_t pulse_us, const char **reason)
{
    uint8_t i;

    if ((s_fc.state != FC_STATE_DISARMED) && (s_fc.state != FC_STATE_CALIBRATING))
    {
        *reason = "Only allowed when DISARMED or CALIBRATING";
        return false;
    }
    if (s_fc.stick_test)
    {
        *reason = "PADTEST is running (PADTEST=0 first)";
        return false;
    }
    if (pulse_us > MOTOR_TEST_MAX_US)
        pulse_us = MOTOR_TEST_MAX_US;
    if (pulse_us < MOTOR_STOP_US)
        pulse_us = MOTOR_STOP_US;

    if (motor == 5U)
    {
        for (i = 0; i < MOTOR_COUNT; i++)
            s_test_us[i] = pulse_us;
    }
    else if ((motor >= 1U) && (motor <= MOTOR_COUNT))
    {
        s_test_us[motor - 1U] = pulse_us;
    }
    else
    {
        *reason = "Motor must be 1..4 or 5 (all)";
        return false;
    }
    *reason = "OK";
    return true;
}

void Sys_MotorTestStop(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_COUNT; i++)
        s_test_us[i] = MOTOR_STOP_US;

    /* STOP_MOTOR and DISARM also end the stick -> motor test */
    if (s_fc.stick_test)
        s_req_stick_test = 0;
}

/* @QC+PADTEST=1/0. Start needs: GAMEPAD build, pad connected, not ARMED /
 * FAILSAFE. The change is applied by the flight task on its next cycle. */
bool Sys_StickTest(bool on, const char **reason)
{
#if (FC_VARIANT != FC_VARIANT_GAMEPAD)
    (void)on;
    *reason = "Only in the GAMEPAD build (FC_VARIANT 3)";
    return false;
#else
    if (!on)
    {
        s_req_stick_test = 0;
        *reason = "PADTEST stopping, motors to 1000 us";
        return true;
    }
    if ((s_fc.state != FC_STATE_DISARMED) && (s_fc.state != FC_STATE_CALIBRATING))
    {
        *reason = "Only when DISARMED or CALIBRATING";
        return false;
    }
    if (!s_fc.pad.connected)
    {
        *reason = "No gamepad connected";
        return false;
    }
    s_req_stick_test = 1;
    *reason = "PADTEST ON - PROPELLERS OFF! Raise throttle with left stick up, B/BACK to stop";
    return true;
#endif
}

PID_TypeDef *Sys_GetPid(PID_IdTypeDef id)
{
    if (id >= PID_ID_COUNT)
        return NULL;
    return &s_pid[id];
}

const char *Sys_PidName(PID_IdTypeDef id)
{
    if (id >= PID_ID_COUNT)
        return "?";
    return s_pid_name[id];
}

/* @QC+POSHOLD=1/0. Only queues the request; the flight task applies it
 * (single writer of s_fc). */
bool Sys_RequestPosHold(bool on, const char **reason)
{
#if (GPS_ENABLE == 1)
    if (s_fc.state != FC_STATE_ARMED)
    {
        *reason = "POSHOLD: arm first";
        return false;
    }
    s_req_poshold = on ? 1 : 0;
    *reason = on ? "POSHOLD on requested (engages in the air with a good fix)"
                 : "POSHOLD off requested";
    return true;
#else
    (void)on;
    *reason = "GPS_ENABLE = 0 in USER_Config.h";
    return false;
#endif
}
