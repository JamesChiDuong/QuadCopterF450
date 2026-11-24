/*
 * Simple microseconds delay routine, utilizing ARM's DWT
 * (Data Watchpoint and Trace Unit) and HAL library.
 * Intended to use with gcc compiler, but I hope it can be used
 * with any other C compiler across the Universe (provided that
 * ARM and CMSIS already invented) :)
 * Max K
 *
 *
 * This file is part of DWT_Delay package.
 * DWT_Delay is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * us_delay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 * the GNU General Public License for more details.
 * http://www.gnu.org/licenses/.
 */

//============================================================================//
// User Includes
//============================================================================//

#include "DWT_Delay.h"

#include "stm32fxxx_hal_user.h"

//============================================================================//
// User Functions
//============================================================================//

/**
 * Initialization routine.
 * You might need to enable access to DWT registers on Cortex-M7
 *   DWT->LAR = 0xC5ACCE55
 */
void DWT_Init(void)
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

#if DWT_DELAY_NEWBIE
/**
 * If you are a newbie and see magic in DWT_Delay, consider this more
 * illustrative function, where you explicitly determine a counter
 * value when delay should stop while keeping things in bounds of uint32.
 */
void DWT_Delay(uint32_t u32_uS) // microseconds
{
    uint32_t u32StartTick = DWT->CYCCNT,
             u32TargetTick = DWT->CYCCNT + u32_uS * (SystemCoreClock / 1000000);

    // Must check if target tick is out of bounds and overflowed
    if (u32TargetTick > u32StartTick)
    {
        // Not overflowed
        while (DWT->CYCCNT < u32TargetTick)
            ;
    }
    else
    {
        // Overflowed
        while (DWT->CYCCNT > u32StartTick || DWT->CYCCNT < u32TargetTick)
            ;
    }
}
#else
/**
 * Delay routine itself.
 * Time is in microseconds (1/1000000th of a second), not to be
 * confused with millisecond (1/1000th).
 *
 * No need to check an overflow. Let it just tick :)
 *
 * @param uint32_t us  Number of microseconds to delay for
 */
void DWT_Delay(uint32_t u32_uS) // microseconds
{
    uint32_t u32StartTick = DWT->CYCCNT,
             u32DelayTicks = u32_uS * (SystemCoreClock / 1000000);

    while (DWT->CYCCNT - u32StartTick < u32DelayTicks)
        ;
}

#endif
