/*
 * USER_PID.c
 *
 *  See USER_PID.h
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_PID.c)
 * ===========================================================================
 *  Implementation of the PID described in USER_PID.h (read its OVERVIEW
 *  first). Called by Sys_RunControl() (USER_System.c) 5 times per 4 ms cycle
 *  in the BLDCTask task. No HAL, no globals: all state lives in PID_TypeDef,
 *  so any number of independent PIDs can exist.
 *
 *  Order of work in PID_Update():
 *    error -> P -> I (with anti-windup clamp) -> D on measurement (filtered)
 *    -> sum -> clamp the output.
 *
 *  Worked example (roll rate loop, Kp=0.7, Ki=0.5, Kd=0.015, dt=0.004 s):
 *    setpoint = 100 deg/s, measured 60 deg/s -> error = 40 deg/s
 *    P = 0.7 * 40 = 28 us
 *    I grows by 0.5 * 40 * 0.004 = 0.08 us per cycle (20 us per second if
 *      the error stays), at most +-100 us (PID_RATE_I_LIMIT).
 *    If the measured rate rises by 1 deg/s in 4 ms -> d_raw = -250 deg/s^2
 *      -> D = -3.75 us (before filtering): D opposes fast changes and helps
 *      to brake overshoot.
 */
#include "USER_PID.h"

#define PID_PI 3.14159265f

/* Clamp v to [-limit, +limit] (symmetric limit). */
static float PID_Clamp(float v, float limit)
{
    if (v > limit)
        return limit;
    if (v < -limit)
        return -limit;
    return v;
}

/*
 * Store gains and limits, then Reset the state.
 *   i_limit, out_limit : same unit as the output (us or deg/s)
 *   d_lpf_hz           : D filter cut-off frequency (Hz), 0 = no filter
 */
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd,
              float i_limit, float out_limit, float d_lpf_hz)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->i_limit = i_limit;
    pid->out_limit = out_limit;
    pid->d_lpf_hz = d_lpf_hz;
    PID_Reset(pid);
}

/*
 * Change gains at run time (called from the UART task by the tune command).
 * Negative gains are refused (a negative gain = positive feedback = unstable).
 * Because integ is stored already multiplied by Ki, changing Ki does not make
 * I jump; only Ki = 0 (I disabled) also clears the accumulated I.
 */
void PID_SetTunings(PID_TypeDef *pid, float kp, float ki, float kd)
{
    if (kp < 0.0f || ki < 0.0f || kd < 0.0f)
        return;

    /* Keep the integral term continuous when Ki changes */
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    if (ki == 0.0f)
        pid->integ = 0.0f;
}

/*
 * Clear all state (I, D filter, terms). first_run = true so that the next
 * Update does not compute D from a stale prev_meas (that would be a big spike).
 */
void PID_Reset(PID_TypeDef *pid)
{
    pid->integ = 0.0f;
    pid->prev_meas = 0.0f;
    pid->d_filt = 0.0f;
    pid->first_run = true;
    pid->p_term = 0.0f;
    pid->i_term = 0.0f;
    pid->d_term = 0.0f;
    pid->output = 0.0f;
}

/*
 * One PID step.
 *   setpoint, measurement : same unit (angle loop: deg; rate loop: deg/s)
 *   dt                    : seconds since the previous call (~0.004 s)
 *   integrate             : false = do not add to I (I keeps its value)
 * Returns the clamped output (angle loop: deg/s; rate loop: us of ESC pulse).
 */
float PID_Update(PID_TypeDef *pid, float setpoint, float measurement,
                 float dt, bool integrate)
{
    float error;
    float d_raw;

    /* Protects the division by dt in the D part */
    if (dt <= 0.0f)
        return pid->output;

    error = setpoint - measurement;

    /* Proportional */
    pid->p_term = pid->kp * error;

    /* Integral (stored already scaled by ki, so retuning ki is bumpless) */
    if (integrate && (pid->ki > 0.0f))
    {
        /* Rectangle integration: add the area error*dt (already times Ki) */
        pid->integ += pid->ki * error * dt;
        /* Anti-windup: I never goes beyond +-i_limit */
        pid->integ = PID_Clamp(pid->integ, pid->i_limit);
    }
    pid->i_term = pid->integ;

    /* Derivative on measurement, low-pass filtered */
    if (pid->first_run)
    {
        /* First call: pretend the measurement did not change -> d_raw = 0 */
        pid->prev_meas = measurement;
        pid->d_filt = 0.0f;
        pid->first_run = false;
    }

    /* Minus sign: with a constant setpoint d(error)/dt = -d(measurement)/dt,
     * so this acts in the same direction as a classic D term, but without
     * the "kick" when the setpoint steps. */
    d_raw = -(measurement - pid->prev_meas) / dt;
    pid->prev_meas = measurement;

    if (pid->d_lpf_hz > 0.0f)
    {
        /* First order RC low-pass: y += alpha * (x - y).
         * Example: fc = 40 Hz -> rc = 3.98 ms; dt = 4 ms -> alpha = 0.50,
         * i.e. every cycle d_filt moves half way towards d_raw. */
        float rc = 1.0f / (2.0f * PID_PI * pid->d_lpf_hz);
        float alpha = dt / (rc + dt);
        pid->d_filt += alpha * (d_raw - pid->d_filt);
    }
    else
    {
        pid->d_filt = d_raw;
    }
    pid->d_term = pid->kd * pid->d_filt;

    /* Clamp the sum so one axis cannot use the whole motor range.
     * Note: I can still grow (up to i_limit) while the output is clamped. */
    pid->output = PID_Clamp(pid->p_term + pid->i_term + pid->d_term,
                            pid->out_limit);
    return pid->output;
}
