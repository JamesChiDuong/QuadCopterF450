#ifndef _SYSTEM_H
#define _SYSTEM_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Typedef
//============================================================================//
// System power mode notation
typedef enum
{
    PWR_MODE_NORMAL = 0U, // 0
    PWR_MODE_IDLE,        // 1
    PWR_MODE_PD,          // 2
    PWR_MODE_SLOW_CLK,    // 3
    PWR_MODE_SUB_CLK,     // 4

} PWR_ModeTypeDef;

typedef struct SYS_PARAM
{
    u8 u8IO_Out[8]; // P23,P24,P25
    u8 u8IO_In[8];  // P03,P04,P06
    u8 u8WakeupSource;
    u8 u8EnableWDT;
    u8 u8WDTRST;
    u8 u8EnableADC;
    u8 u8Debug;
    u8 u8PwrMode;
    u16 u16Tick1S;
    u8 u8Dump;
} SYS_PARAM;

typedef struct SHUT_DOWN
{
    u8 u8Request;
    u8 u8PwrMode;
    u16 u16Tick;
    u16 u16Timeout;

} ShutdownTypeDef;

//============================================================================//
// Extern
//============================================================================//
extern xdata SYS_PARAM SysParam;
extern xdata u8 u8MCU_WDT_Tick;
extern bit bExitSlowMode;
extern xdata u8 u8CheckInterrupt;
//============================================================================//
// Function
//============================================================================//
void InitClock(void);
void System_Init(void);
void System_DeInit(void);
void SetPowerMode(u8 u8Mode);
#endif // _SYSTEM_H