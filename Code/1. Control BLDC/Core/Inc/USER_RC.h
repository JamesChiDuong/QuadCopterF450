/*
 * USER_RC.h
 *
 *  PWM RC receiver input (one wire per channel) on PE2..PE6.
 *  Pulse widths are measured with EXTI (both edges) + the DWT cycle counter,
 *  so no extra hardware timer is needed.
 */
/*
 * OVERVIEW
 *  - Reads a PWM RC receiver on PE2..PE6 (channels 0..4: roll, pitch,
 *    throttle, yaw, ARM switch). Pulse widths are measured with both-edge
 *    EXTI interrupts + the DWT cycle counter, no timer needed.
 *  - BLDCTask (250 Hz) calls RC_GetUs / RC_IsSignalOk. The ISRs update the
 *    data in the background; the task only reads the latest values.
 *  - This module also provides Time_Cycles / Time_CyclesToSec (CPU cycle
 *    clock) so the control loop can measure dt precisely; that is why
 *    RC_Init must be called in EVERY build variant (builds without RC only
 *    enable the DWT).
 *  - Measurement details, validity window, timeout: see USER_RC.c.
 */

#ifndef INC_USER_RC_H_
#define INC_USER_RC_H_

#include <stdint.h>
#include <stdbool.h>
#include "USER_Config.h"

void     RC_Init(void);

/* Last valid pulse width in us (0 if never received) */
uint16_t RC_GetUs(uint8_t channel);

/* true if every channel received a valid pulse within RC_SIGNAL_TIMEOUT_MS */
bool     RC_IsSignalOk(void);

/* Free running CPU cycle counter (DWT). Measure intervals as
 * (Time_Cycles() - start), which is wrap-safe for intervals < ~59 s. */
uint32_t Time_Cycles(void);
float    Time_CyclesToSec(uint32_t cycles);

#endif /* INC_USER_RC_H_ */
