/*
 * USER_System.h
 *
 *  Flight controller core: state machine, cascaded attitude control,
 *  motor mixing and safety logic.
 *
 *  Control structure (runs in the flight task at 1000/CTRL_LOOP_PERIOD_MS Hz):
 *
 *   RC sticks --> angle setpoint --[Angle P]--> rate setpoint --[Rate PID]--+
 *                       ^                              ^                    |
 *                  roll/pitch angle              gyro rates                 v
 *                  (complementary filter)                        Mixer -> ESC
 *   Yaw stick ---------------------------------> yaw rate setpoint ---^
 */

/*
 * ===========================================================================
 * OVERVIEW (USER_System)
 * ===========================================================================
 *  The "brain" of the flight controller. Sys_Init() runs once from main()
 *  before the scheduler starts; Sys_FlightTask() is the body of the BLDCTask
 *  FreeRTOS task (priority High) and runs every CTRL_LOOP_PERIOD_MS = 4 ms
 *  (250 Hz). The other tasks only talk to it through small functions:
 *    - UART task  : Sys_RequestArm / Disarm / Calibration / Level,
 *                   Sys_SetUartSetpoint, Sys_MotorTest, Sys_GetPid (tuning),
 *                   Sys_GetStatus (telemetry)
 *    - UI / LED   : Sys_GetStatus (read only)
 *  Requests only set a volatile flag; the flight task acts on it in its next
 *  cycle, so all state changes happen in ONE task (no race conditions).
 *
 *  One loop cycle:
 *    read MPU6050 -> remap axes to FRD -> Attitude_Update (angles, rates)
 *    -> read inputs (RC / UART / gamepad) -> state machine
 *    -> ARMED: cascade PID + mixer -> ESC     otherwise: motors stopped
 *
 *  State machine (transitions):
 *    boot / calib request / IMU re-found ............. -> CALIBRATING
 *    CALIBRATING  --gyro still for 2 s---------------> DISARMED
 *    DISARMED     --arm request + all checks OK------> ARMED
 *    ARMED        --switch off / B / @QC+DISARM------> DISARMED
 *    ARMED        --fault (see failsafes below)------> FAILSAFE
 *    FAILSAFE     --pilot acknowledges---------------> DISARMED
 *
 *    - CALIBRATING : motors stopped (motor test allowed), measure gyro bias.
 *    - DISARMED    : motors stopped (or manual @QC+MOTOR test values).
 *    - ARMED       : flying. Motors at least MOTOR_IDLE_US.
 *    - FAILSAFE    : motors stopped because of a fault; the pilot must
 *                    acknowledge (arm switch OFF / START released / ARM cmd)
 *                    before arming again, so the quad never re-arms by itself.
 *
 *  Cascade control (roll and pitch; yaw uses only the rate loop):
 *    angle error  -> [Angle P]  -> wanted rotation speed (deg/s)
 *    rate error   -> [Rate PID] -> motor correction (us) -> mixer
 *    Worked example: stick asks 30 deg roll, quad is level:
 *      30 deg error x Kp 4 = 120 deg/s wanted. Quad not rotating yet:
 *      120 deg/s error x Kp 0.7 = 84 us -> left motors +84, right -84.
 *    Why two loops? The inner rate loop reacts fast to gyro disturbances
 *    (wind, motor imbalance); the outer loop only decides how fast to rotate.
 *
 *  Arming conditions (Sys_CheckArm): state DISARMED, gyro calibrated,
 *  |roll| and |pitch| <= SAFE_ARM_MAX_TILT_DEG, input link OK (RC/gamepad),
 *  throttle <= RC_THROTTLE_LOW_US. RC arms on the RISING edge of CH5 only.
 *
 *  Failsafes (ARMED -> FAILSAFE, motors stopped):
 *    RC signal lost > RC_SIGNAL_TIMEOUT_MS, gamepad unplugged,
 *    UART setpoint timeout (if UART_SP_TIMEOUT_MS > 0),
 *    IMU_MAX_READ_ERRORS consecutive IMU errors, tilt > SAFE_CRASH_TILT_DEG.
 *
 *  Build variants (FC_VARIANT in USER_Config.h) select the input source:
 *    RC_FULL   -> FC_SRC_RC      : RC receiver, arm with CH5 switch
 *    UART_TEST -> FC_SRC_UART    : @QC+ARM / @QC+SP commands, bench only,
 *                                  throttle limited to UART_SP_THROTTLE_MAX_US
 *    GAMEPAD   -> FC_SRC_GAMEPAD : USB pad, hold START to arm, B/BACK disarm
 *  For the gamepad, s_fc.rc_ok means "pad connected".
 */

#ifndef INC_USER_SYSTEM_H_
#define INC_USER_SYSTEM_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "USER_Config.h"
#include "USER_PID.h"
#include "USER_Attitude.h"
#include "USER_Gamepad.h"

typedef enum
{
    FC_STATE_CALIBRATING = 0,   /* Gyro calibration, keep the frame still   */
    FC_STATE_DISARMED,          /* Motors stopped (or manual motor test)    */
    FC_STATE_ARMED,             /* Flying                                   */
    FC_STATE_FAILSAFE,          /* Disarmed by a fault, needs arm switch off */
} FC_StateTypeDef;

typedef enum
{
    FC_SRC_RC = 0,              /* RC receiver (normal flight)              */
    FC_SRC_UART,                /* Setpoints from UART (BENCH TEST ONLY)    */
    FC_SRC_GAMEPAD,             /* USB gamepad on CN6                       */
    /* The source is fixed by FC_VARIANT (USER_Config.h) */
} FC_SourceTypeDef;

/* Index into the PID array; the order matches the short names "RA", "PA",
 * "RR", "PR", "YR" used by the UART tuning command. */
typedef enum
{
    PID_ID_ROLL_ANGLE = 0,
    PID_ID_PITCH_ANGLE,
    PID_ID_ROLL_RATE,
    PID_ID_PITCH_RATE,
    PID_ID_YAW_RATE,
    PID_ID_COUNT
} PID_IdTypeDef;

typedef struct
{
    FC_StateTypeDef  state;
    FC_SourceTypeDef source;
    const char      *last_event;   /* Human readable reason of last change */

    Attitude_TypeDef att;

    /* Setpoints */
    /* throttle in us; roll/pitch in deg; *_rate_dps in deg/s. The two
     * sp_*_rate_dps are produced by the angle loop, not by the pilot. */
    float throttle_us;
    float sp_roll_deg;
    float sp_pitch_deg;
    float sp_yaw_rate_dps;
    float sp_roll_rate_dps;
    float sp_pitch_rate_dps;

    /* Controller outputs (us) */
    float out_roll;
    float out_pitch;
    float out_yaw;

    uint16_t motor_us[4];                /* Last pulses sent to M1..M4      */
    uint16_t rc_us[RC_NUM_CHANNELS];     /* Raw RC pulses (0 = no pulse)    */
    bool     rc_ok;                      /* RC signal OK / gamepad connected */

    float    loop_dt_s;                  /* Measured loop period, seconds   */
    uint32_t loop_count;
    uint32_t imu_err_count;              /* Total IMU read errors since boot */

    Gamepad_StateTypeDef pad;   /* FC_VARIANT_GAMEPAD: last pad snapshot   */
    bool     stick_test;        /* PADTEST mode: sticks drive motors open-loop */

    /* GPS position hold (GPS_ENABLE = 1). mode = pilot switched it on;
     * engaged = actually holding (in the air, good fix and heading). */
    bool     poshold_mode;
    bool     poshold_engaged;
} FC_StatusTypeDef;

/* Called once from main() before the scheduler starts */
void Sys_Init(void);

/* Flight task body (never returns) */
void Sys_FlightTask(void const *argument);

/* Read-only snapshot for telemetry (may be updated while you read it) */
const FC_StatusTypeDef *Sys_GetStatus(void);
const char *Sys_StateName(FC_StateTypeDef state);

/* Commands (from the UART task). Return false + reason when refused. */
/* They only post a request; the flight task executes it on its next cycle
 * (up to 4 ms later), so "true" means "accepted", not "done" - check STATUS.
 * reason must not be NULL for the functions that take it. */
bool Sys_RequestArm(const char **reason);
void Sys_RequestDisarm(const char *reason);
bool Sys_RequestCalibration(const char **reason);
bool Sys_RequestLevel(const char **reason);
void Sys_SetUartSetpoint(float throttle_us, float roll_deg, float pitch_deg, float yaw_rate_dps);
bool Sys_MotorTest(uint8_t motor, uint16_t pulse_us, const char **reason);
void Sys_MotorTestStop(void);

/*
 * PADTEST (GAMEPAD build only): stick -> motor test without IMU and without
 * arming. The sticks go straight through the mixer (open loop, no PID):
 *   left stick Y = throttle (held, like in flight), left X = yaw,
 *   right X = roll, right Y = pitch; full stick = +-PADTEST_TORQUE_US.
 * Motors stay at MOTOR_STOP_US until throttle > RC_THROTTLE_LOW_US and are
 * limited to MOTOR_TEST_MAX_US. PROPELLERS OFF.
 * Ends on Sys_StickTest(false), DISARM, STOP_MOTOR, B/BACK, pad unplugged.
 */
bool Sys_StickTest(bool on, const char **reason);

/* @QC+POSHOLD=1/0 (or pad RB): GPS position hold on / off. Accepted only
 * while ARMED and when the firmware is built with GPS_ENABLE = 1. */
bool Sys_RequestPosHold(bool on, const char **reason);

#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
/* Stick -> roll / pitch / yaw-rate setpoints (pure function) */
void Sys_PadToSetpoints(const Gamepad_StateTypeDef *gp, float *roll_deg, float *pitch_deg,
                        float *yaw_rate_dps);
#endif

PID_TypeDef *Sys_GetPid(PID_IdTypeDef id);
const char  *Sys_PidName(PID_IdTypeDef id);

#endif /* INC_USER_SYSTEM_H_ */
