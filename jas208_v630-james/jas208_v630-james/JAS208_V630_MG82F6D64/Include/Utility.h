#ifndef _UTILITY_H
#define _UTILITY_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define mS_2_uS(mS)				          ((mS) * 1000) // Millisecond to microsecond

#define mS_2_S(mS)				          ((mS) / 1000) // Millisecond to second

#define mS_2_Hz(mS)				          ((float)1000 / (mS)) // Millisecond to Hertz

#define S_2_mS(S)				            ((S) * 1000) // Second to millisecond

#define S_2_uS(S)				            ((S) * 1000000) // Second to micro second

#define S_2_MIN(S)				          ((S) / 60) // Second to minute

#define S_2_HOUR(S)				          ((S) / 3600) // Second to hour

#define S_2_DAY(S)				          ((S) / 86400) // Second to day

#define MINUTE_2_SECOND(MIN)	      ((MIN) * 60) // Minute to second

#define HOUR_2_SECOND(HOUR)		      ((HOUR) * 3600) // Hour to second

#define DAY_2_SECOND(DAY)		        ((DAY) * 86400) // Day to second

//============================================================================//
// Extern
//============================================================================/

//============================================================================//
// Function
//============================================================================//
#if ENABLE_DELAYUS == 1
void DelayXus(u8 xUs);
#endif
#if ENABLE_DELAYUS == 1 && ENABLE_DELAYMS == 1
void DelayXms(u16 xMs);
#endif
//u8 HexToChar(u8 x);
void DecipherCommand(char *szCommand);
void PrintDateTime(void);
void PrintSysInfo(void);

#endif // _UTILITY_H