/*
 * USER_PID.h
 *
 *  Small PID controller for the flight loops.
 *  - explicit dt (seconds) so it does not depend on the 1 ms HAL tick
 *  - derivative on measurement (no kick on setpoint change) + low pass filter
 *  - integrator clamp (anti wind-up) and output clamp
 *
 *  Pure C, no HAL dependency (can be unit tested on a PC).
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_PID)
 * ===========================================================================
 *  One PID implementation shared by the 5 control loops of the quad:
 *    - 2 ANGLE loops (roll angle, pitch angle)        : P only, output deg/s
 *    - 3 RATE loops  (roll / pitch / yaw rate)         : P + I + D, output us
 *  Caller: Sys_RunControl() in USER_System.c, inside the BLDCTask FreeRTOS
 *  task, 250 times per second (CTRL_LOOP_PERIOD_MS = 4 ms). The UART PID
 *  tuning command calls PID_SetTunings() from the UART task.
 *
 *  Input : setpoint (wanted value), measurement (measured value),
 *          dt (seconds, really measured with the CPU cycle counter, the code
 *          does not assume exactly 4 ms).
 *  Output: P + I + D, clamped to [-out_limit, +out_limit].
 *
 *  Basic concepts:
 *   - error = setpoint - measurement.
 *   - P (proportional) = Kp * error. Bigger error -> bigger push. Kp too high
 *                   -> fast oscillation (wobble), too low -> sluggish.
 *   - I (integral)  = sum of Ki * error * dt. Removes a steady error that
 *                   lasts (centre of gravity off, one motor weaker...).
 *                   Ki too high -> slow oscillation / overshoot.
 *   - D (derivative) = Kd * rate of change. Acts like a damper and brakes
 *                   oscillations. Very sensitive to noise, so it goes through
 *                   a low-pass filter (d_lpf_hz).
 *   - Anti-windup : on the ground, or when the motors saturate, the error can
 *                   stay for a long time and I keeps growing ("windup"), then
 *                   the quad jumps at take-off. Two protections here: |I| is
 *                   clamped to i_limit, and integrate=false freezes I (used
 *                   while the throttle is low).
 *   - Derivative on measurement: D uses -d(measurement)/dt instead of
 *                   d(error)/dt. When the pilot snaps a stick (setpoint step),
 *                   d(error)/dt is huge -> "derivative kick" (motor spike).
 *                   The derivative of the measurement does not see setpoint
 *                   steps.
 *   - integ is stored ALREADY multiplied by Ki, so changing Ki in flight does
 *     not make the I term jump.
 */

#ifndef INC_USER_PID_H_
#define INC_USER_PID_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    /* Tuning */
    float kp;
    float ki;
    float kd;
    float i_limit;     /* |integral term| <= i_limit                          */
    float out_limit;   /* |output| <= out_limit                               */
    float d_lpf_hz;    /* Derivative low-pass cut-off, 0 = no filter          */

    /* State */
    float integ;       /* Integral term (already multiplied by ki)            */
    float prev_meas;   /* Measurement of the previous call (for derivative)   */
    float d_filt;      /* Derivative after the low-pass filter                */
    bool  first_run;   /* true after Reset: no prev_meas yet -> D = 0         */

    /* Last computed terms (for telemetry / tuning) */
    float p_term;
    float i_term;
    float d_term;
    float output;
} PID_TypeDef;

/* Set gains + limits and clear the state (called once from Sys_Init). */
void  PID_Init(PID_TypeDef *pid, float kp, float ki, float kd,
               float i_limit, float out_limit, float d_lpf_hz);
/* Change gains at run time (UART tuning command). Negative gains are ignored. */
void  PID_SetTunings(PID_TypeDef *pid, float kp, float ki, float kd);
/* Clear I, D filter and terms (on arm/disarm, or while the throttle is low). */
void  PID_Reset(PID_TypeDef *pid);

/*
 * Compute one step.
 *  setpoint, measurement : same unit
 *  dt                    : seconds since last call (> 0)
 *  integrate             : false freezes the integrator (e.g. on the ground)
 *
 *  Returns the clamped output (same unit as out_limit) and also stores
 *  p_term / i_term / d_term in the struct for telemetry.
 *  If dt <= 0 nothing is computed and the previous output is returned.
 */
float PID_Update(PID_TypeDef *pid, float setpoint, float measurement,
                 float dt, bool integrate);

#endif /* INC_USER_PID_H_ */
