#ifndef _TIMER_DRIVER_H
#define _TIMER_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define TIMER_1T_1ms_TH	((65536-(u16)(float)(1000.0*((float)(MCU_SYSCLK)/(float)(1000000)))) /256) 			
#define TIMER_1T_1ms_TL	((65536-(u16)(float)(1000.0*((float)(MCU_SYSCLK)/(float)(1000000)))) %256)
//1000 Hz
#define TIMER_12T_1ms_TH	((65536-(u16)(float)(1000.0*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_1ms_TL	((65536-(u16)(float)(1000.0*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)
//200 Hz
#define TIMER_12T_5ms_TH	((65536-(u16)(float)(5000.0*((float)(MCU_SYSCLK)/(float)(12000000)))) /256) 			
#define TIMER_12T_5ms_TL	((65536-(u16)(float)(5000.0*((float)(MCU_SYSCLK)/(float)(12000000)))) %256)
	
//============================================================================//
// Extern
//============================================================================//
extern xdata u16 u16TickTime;
extern xdata u8 u8Tick1Hz;
extern xdata u8 u8Tick2Hz;
extern xdata u8 u8Tick5Hz;
extern xdata u8 u8Tick10Hz;
extern xdata u8 u8Tick20Hz;

//============================================================================//
// Function
//============================================================================//
void Timer0_Init(void);
void Timer0_DeInit(void);
void InitWDT(void);
void DeInitWDT(void);
void ResetWDT(void);

#endif // _TIMER_DRIVER_H