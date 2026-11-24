#ifndef _DATA_COLLECT_H_
#define _DATA_COLLECT_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "DataType.h"
#include "System.h"


//============================================================================//
// Extern Variables
//============================================================================//
extern uint16_t u16SpeedBuf1; 
extern uint16_t u16SpeedBuf2;
extern uint16_t u16SpeedBuf3;
extern uint16_t u16SpeedBuf4;

extern uint16_t u16_100M_SpeedAvg; // unit HM/hr


//============================================================================//
// User Function Prototypes
//============================================================================//
extern bool Info_CheckDrvId(uint32_t DrvId);
extern bool Info_CheckDrvId2(uint32_t DrvId);
extern bool Debug_Proc(void);

//============================================================================//
#endif /* _DATA_COLLECT_H_ */
