/*
 * USER_Motor_Driver.h
 *
 *  ESC output driver. Every motor command is an ESC pulse width in
 *  microseconds (1000 = stop, 2000 = full throttle).
 *
 *  Motor -> timer mapping (from CubeMX configuration):
 *      M1 front left  : TIM3 CH1 (PB4)
 *      M2 back left   : TIM1 CH1 (PA8)
 *      M3 front right : TIM3 CH3 (PB0)
 *      M4 back right  : TIM2 CH3 (PB10)
 */
/*
 * OVERVIEW
 *  - ESC driver interface: every motor command is a pulse width in us
 *    (1000 = stop, 2000 = full throttle), PWM 400 Hz, 1 us resolution.
 *  - Motors are numbered 1..4 (M1 front-left, M2 back-left, M3 front-right,
 *    M4 back-right), see the diagram in the README.
 *  - Motor_Init is called once at start-up (blocks 3 s); the Write functions
 *    are called from BLDCTask (250 Hz); Motor_GetUs is used by UI / telemetry.
 *  - Timer clock details and the prescaler maths: see USER_Motor_Driver.c.
 */

#ifndef INC_USER_MOTOR_DRIVER_H_
#define INC_USER_MOTOR_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#define MOTOR_COUNT 4U

/* Configure timers (1 us resolution), start PWM, hold STOP so ESCs arm.
 * Blocks MOTOR_ESC_ARM_DELAY_MS: call before the scheduler starts. */
void     Motor_Init(void);

/* Put the old ESC "VCC" pins (PG15, PC8, PC4, PE14) in high impedance so a
 * connected ESC +5 V wire cannot fight / short the MCU (see .c). Called from
 * main() right after the CubeMX GPIO init. */
void     Motor_ReleaseVccPins(void);

/* motor: 1..4 */
/* Writes a pulse (us, clamped to 1000..2000). Returns false for a bad motor
 * number. Motor_GetUs returns 0 for a bad motor number. */
bool     Motor_WriteUs(uint8_t motor, uint16_t pulse_us);
void     Motor_WriteAllUs(const uint16_t pulse_us[MOTOR_COUNT]);
void     Motor_StopAll(void);
uint16_t Motor_GetUs(uint8_t motor);

#endif /* INC_USER_MOTOR_DRIVER_H_ */
