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
/* _DWT_DELAY_H_ */
#ifndef _DWT_DELAY_H_
#define _DWT_DELAY_H_


//============================================================================//
// User Includes
//============================================================================//
#include <stdint.h>


//============================================================================//
// User Defines
//============================================================================//
#define DWT_DELAY_NEWBIE   0U


//============================================================================//
// User Function Prototypes
//============================================================================//
void DWT_Init(void);
void DWT_Delay(uint32_t u32_uS);


#endif /* _DWT_DELAY_H_ */
