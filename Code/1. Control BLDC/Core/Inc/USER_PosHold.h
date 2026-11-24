/*
 * USER_PosHold.h
 *
 *  GPS position hold controller. Pure C, no HAL: the same maths is ported
 *  line by line to host/fc_model.py (class PosHold) and checked in closed
 *  loop by host/sim_flight_controller.py.
 */
/*
 * ===========================================================================
 * OVERVIEW (USER_PosHold)
 * ===========================================================================
 *  Only used when GPS_ENABLE = 1 (USER_Config.h). Caller: Sys_RunControl()
 *  (USER_System.c) every 4 ms while ARMED and POSHOLD is active. Output: the
 *  roll / pitch ANGLE setpoints that the existing angle loop then follows.
 *
 *  Frames: positions / speeds in local NED metres (x North, y East) relative
 *  to the point where GPS home was taken; heading in degrees, 0 = nose North,
 *  90 = nose East.
 *
 *     target - pos ----> x POS_P ----> wanted speed (<= MAX_SPEED)
 *     wanted speed - speed ---> x VEL_P + I ----> wanted tilt North / East
 *     rotate by heading ----> forward / right tilt
 *     pitch_sp = -forward   (nose DOWN to go forward, FRD: pitch>0 = nose up)
 *     roll_sp  = +right     (right side down to go right)
 *
 *  Example: heading 0 (nose North), 2 m South of the target, still:
 *     error North = +2 m -> wanted 1.6 m/s North -> tilt North = 8 deg
 *     -> pitch_sp = -8 deg (nose down, flies North), roll_sp = 0.
 *  Same case with the nose pointing East (heading 90): forward = East, the
 *  target is on the LEFT -> roll_sp = -8 deg (left side down), pitch_sp = 0.
 * ===========================================================================
 */

#ifndef INC_USER_POSHOLD_H_
#define INC_USER_POSHOLD_H_

#include <stdbool.h>

typedef struct
{
    float target_n;      /* hold point, m North of home                     */
    float target_e;      /* hold point, m East of home                      */
    float i_n;           /* velocity integrators, deg of tilt               */
    float i_e;
    bool  active;        /* target valid                                    */
} PosHold_TypeDef;

/* Gains / limits (from USER_Config.h, passed in so the module stays pure) */
typedef struct
{
    float pos_p;         /* (m/s) per m                                     */
    float max_speed;     /* m/s                                             */
    float vel_p;         /* deg per (m/s)                                   */
    float vel_i;         /* deg per (m/s * s)                               */
    float vel_i_limit;   /* deg                                             */
    float max_angle;     /* deg                                             */
} PosHold_Gains;

/* Start holding at the current position (integrators cleared). */
void PosHold_Start(PosHold_TypeDef *ph, float pos_n, float pos_e);

/* Stop holding (active = false). */
void PosHold_Stop(PosHold_TypeDef *ph);

/* Pilot is flying: move the hold point to the current position, keep the
 * integrators (the wind estimate) so the hold is smooth when released. */
void PosHold_FollowPilot(PosHold_TypeDef *ph, float pos_n, float pos_e);

/*
 * One control step.
 *   pos_n/e : current position (m), vel_n/e: current ground speed (m/s)
 *   heading_deg : nose direction (0 = North, 90 = East)
 *   dt : seconds
 * Outputs the angle setpoints (deg, FRD sign convention).
 */
void PosHold_Update(PosHold_TypeDef *ph, const PosHold_Gains *g,
                    float pos_n, float pos_e, float vel_n, float vel_e,
                    float heading_deg, float dt,
                    float *roll_sp_deg, float *pitch_sp_deg);

#endif /* INC_USER_POSHOLD_H_ */
