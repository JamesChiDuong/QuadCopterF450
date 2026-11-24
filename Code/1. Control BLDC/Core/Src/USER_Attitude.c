/*
 * USER_Attitude.c
 *
 *  See USER_Attitude.h
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_Attitude.c)
 * ===========================================================================
 *  Implementation of the attitude estimator (read the OVERVIEW in
 *  USER_Attitude.h first). Called from Sys_FlightTask() every 4 ms:
 *    - state CALIBRATING : Attitude_Update() + Attitude_CalibrationStep()
 *    - other states      : Attitude_Update()
 *
 *  Data flow inside Attitude_Update():
 *    gyro - bias -> p, q, r (rates around body X, Y, Z)
 *    -> converted to Euler angle rates (roll/pitch/yaw rate) -> integrated * dt
 *    -> blended with the accel angles (complementary filter)
 *    -> level trim subtracted -> att->roll, att->pitch.
 *
 *  Note: s_roll_raw / s_pitch_raw are static module variables, so this module
 *  can serve only ONE Attitude_TypeDef (enough for one quad).
 */
#include "USER_Attitude.h"
#include "USER_Config.h"
#include <math.h>
#include <string.h>

#define RAD2DEG (57.2957795f)
#define DEG2RAD (0.0174532925f)

/* Raw (untrimmed) filter state kept privately so trim does not feed back */
/* "Raw" angles (trim not subtracted, deg). The filter runs on these; trim is
 * only subtracted at the output, otherwise the accel would pull the trimmed
 * angle back to its own value and the trim would slowly disappear. */
static float s_roll_raw;
static float s_pitch_raw;

/* Reset the whole estimate + bias + trim, then start a gyro calibration. */
void Attitude_Init(Attitude_TypeDef *att)
{
    memset(att, 0, sizeof(*att));
    s_roll_raw = 0.0f;
    s_pitch_raw = 0.0f;
    Attitude_StartCalibration(att);
}

/* Restart calibration. The old gyro_bias is kept (and still used by
 * Attitude_Update) until the new calibration has finished. */
void Attitude_StartCalibration(Attitude_TypeDef *att)
{
    att->calibrated = false;
    att->calib_count = 0;
    memset(&att->calib_sum, 0, sizeof(att->calib_sum));
}

/* Update the per-axis minimum / maximum (to measure how much the gyro moves). */
static void Attitude_MinMax(Vec3_TypeDef *mn, Vec3_TypeDef *mx, const Vec3_TypeDef *v)
{
    if (v->x < mn->x) mn->x = v->x;
    if (v->y < mn->y) mn->y = v->y;
    if (v->z < mn->z) mn->z = v->z;
    if (v->x > mx->x) mx->x = v->x;
    if (v->y > mx->y) mx->y = v->y;
    if (v->z > mx->z) mx->z = v->z;
}

/*
 * Feed one gyro sample (deg/s, body axes) to the bias calibration.
 *   n_samples   : samples needed (IMU_GYRO_CALIB_SAMPLES = 500 -> 2 s at 250 Hz)
 *   max_dev_dps : allowed deviation around the mean (deg/s)
 * Returns true exactly ONCE, on the sample that completes the calibration;
 * afterwards it always returns false.
 * Bias = average of the samples (a still gyro should read 0).
 */
bool Attitude_CalibrationStep(Attitude_TypeDef *att, const Vec3_TypeDef *gyro_dps,
                              uint32_t n_samples, float max_dev_dps)
{
    if (att->calibrated)
        return false;

    if (att->calib_count == 0)
    {
        /* First sample (or just restarted): initialise min/max and the sum */
        att->calib_min = *gyro_dps;
        att->calib_max = *gyro_dps;
        memset(&att->calib_sum, 0, sizeof(att->calib_sum));
    }

    att->calib_sum.x += gyro_dps->x;
    att->calib_sum.y += gyro_dps->y;
    att->calib_sum.z += gyro_dps->z;
    Attitude_MinMax(&att->calib_min, &att->calib_max, gyro_dps);
    att->calib_count++;

    /* Board moved: peak-to-peak too large -> restart */
    /* Example max_dev = 2 deg/s: if max - min > 4 deg/s on any axis the quad
     * was touched / moved -> count again from zero (no error reported). */
    if (((att->calib_max.x - att->calib_min.x) > 2.0f * max_dev_dps) ||
        ((att->calib_max.y - att->calib_min.y) > 2.0f * max_dev_dps) ||
        ((att->calib_max.z - att->calib_min.z) > 2.0f * max_dev_dps))
    {
        att->calib_count = 0;
        return false;
    }

    if (att->calib_count >= n_samples)
    {
        att->gyro_bias.x = att->calib_sum.x / (float)att->calib_count;
        att->gyro_bias.y = att->calib_sum.y / (float)att->calib_count;
        att->gyro_bias.z = att->calib_sum.z / (float)att->calib_count;
        att->calibrated = true;
        return true;
    }
    return false;
}

/*
 * One estimator step (see the data flow in the OVERVIEW above).
 *   gyro_dps : raw gyro, deg/s, body FRD axes
 *   acc_g    : accelerometer, g, body FRD axes
 *   dt       : seconds since the previous call (~0.004 s)
 * Side effects: writes att->roll/pitch/yaw and the three rates, updates the
 * private raw angles. The very first call seeds roll/pitch from the accel.
 */
void Attitude_Update(Attitude_TypeDef *att, const Vec3_TypeDef *gyro_dps,
                     const Vec3_TypeDef *acc_g, float dt)
{
    float p, q, r;               /* body rates, deg/s */
    float acc_norm;
    float acc_roll, acc_pitch;
    float sin_r, cos_r, tan_p;
    float roll_rad, pitch_rad;
    float alpha;

    /* Remove the bias measured during calibration */
    p = gyro_dps->x - att->gyro_bias.x;
    q = gyro_dps->y - att->gyro_bias.y;
    r = gyro_dps->z - att->gyro_bias.z;

    /* The rate loops use body rates directly (good enough for small angles) */
    att->roll_rate = p;
    att->pitch_rate = q;
    att->yaw_rate = r;

    /* Angles from accelerometer (specific force f = -g in body frame) */
    /* Level and still, FRD: acc = (0, 0, -1 g) -> acc_roll = acc_pitch = 0.
     * Right side down by angle phi: acc.y = -sin(phi) -> atan2 gives +phi.
     * Nose up by theta: acc.x = +sin(theta) -> acc_pitch = +theta. */
    acc_roll  = atan2f(-acc_g->y, -acc_g->z) * RAD2DEG;
    acc_pitch = atan2f(acc_g->x, sqrtf(acc_g->y * acc_g->y + acc_g->z * acc_g->z)) * RAD2DEG;
    /* Length of the accel vector: ~1 g when the quad is not accelerating */
    acc_norm  = sqrtf(acc_g->x * acc_g->x + acc_g->y * acc_g->y + acc_g->z * acc_g->z);

    if (!att->initialized)
    {
        /* First sample: start from the accel angle instead of 0, so the
         * filter does not need ~1 s to converge after boot */
        s_roll_raw = acc_roll;
        s_pitch_raw = acc_pitch;
        att->yaw = 0.0f;
        att->initialized = true;
    }
    else
    {
        /* Euler angle rates from body rates */
        /* Body rates (p,q,r) are not the same as Euler angle rates when the
         * quad is tilted; these are the standard kinematic equations:
         *   roll_dot  = p + (q*sin(roll) + r*cos(roll)) * tan(pitch)
         *   pitch_dot = q*cos(roll) - r*sin(roll)
         *   yaw_dot   = (q*sin(roll) + r*cos(roll)) / cos(pitch)
         * At level (roll = pitch = 0) they reduce to p, q, r. */
        roll_rad  = s_roll_raw * DEG2RAD;
        pitch_rad = s_pitch_raw * DEG2RAD;
        sin_r = sinf(roll_rad);
        cos_r = cosf(roll_rad);

        /* Avoid tan() blow-up near +-90 deg pitch */
        /* 1.4 rad = about 80 deg */
        if (pitch_rad > 1.4f)
            pitch_rad = 1.4f;
        else if (pitch_rad < -1.4f)
            pitch_rad = -1.4f;
        tan_p = tanf(pitch_rad);

        /* Gyro integration (prediction step) */
        s_roll_raw  += (p + (q * sin_r + r * cos_r) * tan_p) * dt;
        s_pitch_raw += (q * cos_r - r * sin_r) * dt;
        att->yaw    += ((q * sin_r + r * cos_r) / cosf(pitch_rad)) * dt;

        /* Wrap */
        /* Keep roll and yaw in [-180, 180] */
        if (s_roll_raw > 180.0f)  s_roll_raw -= 360.0f;
        if (s_roll_raw < -180.0f) s_roll_raw += 360.0f;
        if (att->yaw > 180.0f)    att->yaw -= 360.0f;
        if (att->yaw < -180.0f)   att->yaw += 360.0f;

        /* Complementary filter: trust the accelerometer only near 1 g */
        /* Outside 0.85..1.15 g the quad is accelerating (or shaking hard),
         * so the accel angle is wrong: use the gyro alone for that sample.
         * alpha = tau / (tau + dt) = 0.5 / 0.504 = 0.992 -> each cycle the
         * angle moves 0.8 % of the way towards the accel angle. */
        if ((acc_norm > IMU_ACC_TRUST_MIN_G) && (acc_norm < IMU_ACC_TRUST_MAX_G))
        {
            alpha = IMU_COMP_FILTER_TAU_S / (IMU_COMP_FILTER_TAU_S + dt);
            s_roll_raw  = alpha * s_roll_raw  + (1.0f - alpha) * acc_roll;
            s_pitch_raw = alpha * s_pitch_raw + (1.0f - alpha) * acc_pitch;
        }
    }

    /* Output = raw angle minus the level trim */
    att->roll  = s_roll_raw - att->roll_trim;
    att->pitch = s_pitch_raw - att->pitch_trim;
}

/* Store the current raw angles as the trim: from now on this attitude reads
 * roll = pitch = 0. Call only with the frame standing on flat ground. */
void Attitude_SetLevel(Attitude_TypeDef *att)
{
    att->roll_trim = s_roll_raw;
    att->pitch_trim = s_pitch_raw;
}
