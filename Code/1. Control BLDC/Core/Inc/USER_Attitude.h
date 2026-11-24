/*
 * USER_Attitude.h
 *
 *  Attitude estimation from MPU6050 data:
 *   - gyro bias calibration (board must be still)
 *   - complementary filter for roll / pitch (gyro + accelerometer)
 *   - yaw angle by gyro integration (drifts, for display only)
 *
 *  Inputs must already be in body FRD axes (see USER_Config.h).
 *  Pure C, no HAL dependency.
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_Attitude)
 * ===========================================================================
 *  Estimates the attitude (tilt angles) of the quad from the MPU6050.
 *  Caller: Sys_FlightTask() (USER_System.c) in the BLDCTask task, every 4 ms
 *  (250 Hz), after reading the MPU6050 and remapping its axes to body FRD.
 *
 *  In : gyro (deg/s, bias NOT removed yet), accel (g), dt (seconds).
 *  Out: roll, pitch, yaw (deg) and roll_rate, pitch_rate, yaw_rate (deg/s,
 *       bias removed) in Attitude_TypeDef -> used by the angle / rate PIDs.
 *  FRD convention: roll > 0 right side down, pitch > 0 nose up,
 *                  yaw > 0 nose turns right.
 *
 *  Why combine two sensors?
 *   - The gyro measures ROTATION SPEED (deg/s). To get an angle it must be
 *     integrated (sum of rate * dt). Very accurate short term and not much
 *     disturbed by vibration, but any small error (bias, e.g. 0.5 deg/s) is
 *     summed forever -> the angle DRIFTS: 30 deg wrong after one minute.
 *   - The accelerometer sees the direction of gravity -> gives an ABSOLUTE
 *     roll/pitch that never drifts. But it is very NOISY (motor vibration)
 *     and wrong while the quad accelerates (inertial force adds to gravity).
 *     It cannot measure yaw at all.
 *   - Complementary filter: angle = alpha*(old angle + gyro*dt)
 *     + (1-alpha)*accel angle. alpha is close to 1 (~0.992) -> trust the gyro
 *     short term, while the accel slowly pulls the angle back to the truth
 *     (time constant IMU_COMP_FILTER_TAU_S = 0.5 s).
 *   - Yaw is gyro integration only (no compass) so it drifts -> display only.
 *     The yaw control loop uses yaw_rate, never the yaw angle.
 *
 *  Gyro calibration: keep the quad still for ~2 s, the average of 500 gyro
 *  samples becomes the bias (what the gyro reads while not rotating). If the
 *  quad moves during that time, calibration restarts by itself.
 *
 *  Level trim: if the MPU6050 is mounted slightly tilted on the frame, put
 *  the frame on a flat surface and call Attitude_SetLevel() to store that
 *  offset (RAM only, lost at power off).
 */

#ifndef INC_USER_ATTITUDE_H_
#define INC_USER_ATTITUDE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vec3_TypeDef;

typedef struct
{
    /* Estimated attitude, degrees */
    float roll;
    float pitch;
    float yaw;

    /* Bias-corrected body rates, deg/s */
    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    /* Level trim (subtracted from the estimate), degrees */
    float roll_trim;
    float pitch_trim;

    /* Gyro calibration */
    Vec3_TypeDef gyro_bias;
    bool         calibrated;
    bool         initialized;   /* First accel sample used to seed angles */

    /* Internal: calibration accumulators */
    uint32_t     calib_count;
    Vec3_TypeDef calib_sum;
    Vec3_TypeDef calib_min;
    Vec3_TypeDef calib_max;
} Attitude_TypeDef;

/* Clear the whole struct (bias and trim too) and start a gyro calibration. */
void Attitude_Init(Attitude_TypeDef *att);

/* Restart gyro calibration (clears 'calibrated') */
void Attitude_StartCalibration(Attitude_TypeDef *att);

/*
 * Feed one raw gyro sample (deg/s, body axes) to the calibration.
 * Returns true when calibration has just finished successfully.
 * If the board moves during calibration it restarts automatically.
 */
bool Attitude_CalibrationStep(Attitude_TypeDef *att, const Vec3_TypeDef *gyro_dps,
                              uint32_t n_samples, float max_dev_dps);

/*
 * Update the estimate.
 *   gyro_dps : raw gyro, deg/s, body axes (bias is removed internally)
 *   acc_g    : accelerometer specific force, g, body axes
 *   dt       : seconds
 */
void Attitude_Update(Attitude_TypeDef *att, const Vec3_TypeDef *gyro_dps,
                     const Vec3_TypeDef *acc_g, float dt);

/* Use the current attitude as "level" (call with the frame on flat ground) */
void Attitude_SetLevel(Attitude_TypeDef *att);

#endif /* INC_USER_ATTITUDE_H_ */
