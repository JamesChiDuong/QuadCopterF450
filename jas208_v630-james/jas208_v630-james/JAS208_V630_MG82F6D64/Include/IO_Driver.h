#ifndef _IO_DRIVER_H
#define _IO_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Extern
//============================================================================//
extern code u8 u8IOMask[8];

//============================================================================//
// Function
//============================================================================//
#if ENABLE_INT0
void INT0_Init(void);
#endif
#if ENABLE_INT1
void INT1_Init(void);
#endif
#if ENABLE_INT2
void INT2_Init(void);
#endif
#if ENABLE_INT3
void INT3_Init(void);
#endif
void InitPort(void);
void SetOutPin(int nPin, int nOn);
void SendWkupPulse(int nWkupPin, int nDuration);
void SendRstPulse(int nRstPin, int nDuration);

#endif // _IO_DRIVER_H