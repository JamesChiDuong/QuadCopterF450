#ifndef _RX8130_DRIVER_H_
#define _RX8130_DRIVER_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//

// RX8130CE slave address
#define RX8130_SLAVE_ADDR     0x32 // 00110010

// RX8130CE address when communicating using STM32 HAL I2C Driver
#define RX8130_ADDR_I2C       (RX8130_SLAVE_ADDR << 1) // 0x64 01100100

// RX-8130 Register definitions
#define RX8130_REG_SEC      	0x10
#define RX8130_REG_MIN      	0x11
#define RX8130_REG_HOUR     	0x12
#define RX8130_REG_WDAY 			0x13
#define RX8130_REG_MDAY 			0x14
#define RX8130_REG_MONTH 			0x15
#define RX8130_REG_YEAR 			0x16

#define RX8130_REG_ALMIN 			0x17
#define RX8130_REG_ALHOUR 		0x18
#define RX8130_REG_ALWDAY 		0x19
#define RX8130_REG_TCOUNT0 		0x1A
#define RX8130_REG_TCOUNT1 		0x1B
#define RX8130_REG_EXT 				0x1C
#define RX8130_REG_FLAG 			0x1D
#define RX8130_REG_CTRL0 			0x1E
#define RX8130_REG_CTRL1 			0x1F

#define RX8130_REG_DT_OFST    0x30
#define RX8130_REG_EXT_1      0x31

// Extension Register (1Ch) bit positions
#define RX8130_BIT_EXT_TSEL 		(7 << 0)
#define RX8130_BIT_EXT_WADA 		(1 << 3)
#define RX8130_BIT_EXT_TE 			(1 << 4)
#define RX8130_BIT_EXT_USEL 		(1 << 5)
#define RX8130_BIT_EXT_FSEL 		(3 << 6)

// Flag Register (1Dh) bit positions
#define RX8130_BIT_FLAG_VBFF    (1 << 0)
#define RX8130_BIT_FLAG_VLF 		(1 << 1)
#define RX8130_BIT_FLAG_AF 			(1 << 3)
#define RX8130_BIT_FLAG_TF 			(1 << 4)
#define RX8130_BIT_FLAG_UF 			(1 << 5)
#define RX8130_BIT_FLAG_VBLF    (1 << 7)

// Control 0 Register (1?h) bit positions
#define RX8130_BIT_CTRL_TSTP 		(1 << 2)
#define RX8130_BIT_CTRL_AIE 		(1 << 3)
#define RX8130_BIT_CTRL_TIE 		(1 << 4)
#define RX8130_BIT_CTRL_UIE 		(1 << 5)
#define RX8130_BIT_CTRL_STOP 		(1 << 6)
#define RX8130_BIT_CTRL_TEST 		(1 << 7)

// Control 1 Register (1Fh) bit positions
#define RX8130_BIT_CTRL_BFVSEL0 		(1 << 0)
#define RX8130_BIT_CTRL_BFVSEL1 		(1 << 1)
#define RX8130_BIT_CTRL_RSVSEL 			(1 << 2)
#define RX8130_BIT_CTRL_INIEN 			(1 << 4)
#define RX8130_BIT_CTRL_CHGEN 			(1 << 5)
#define RX8130_BIT_CTRL_SMPTSEL0 		(1 << 6)
#define RX8130_BIT_CTRL_SMPTSEL1 		(1 << 7)

// Date time register mask
#define RX8130_SEC_MASK                0x7F
#define RX8130_MIN_MASK                0x7F
#define RX8130_HOUR_MASK               0x3F
#define RX8130_WDAY_MASK               0x7F
#define RX8130_DAY_MASK                0x3F
#define RX8130_MON_MASK                0x1F
#define RX8130_YEAR_MASK               0xFF

#define RX8130_MAX_TRIALS             5U

#define RX8130_MAX_TIMEOUT            100U

#define RX8130_ERR_CNT_MAX            5U

#define RX8130_RESTART_CNT_MAX        2U

#define RX8130_REPORT_ERR_TIMEOUT     S_2_mS(120)


//============================================================================//
// User Externs
//============================================================================//
extern bool bRX8130_EN;
extern uint32_t u32RX8130_ReportErrLastTick;
extern bool bRX8130_ReportErrFlag;
extern bool bRX8130_Stop;


//============================================================================//
// User Function Prototypes
//============================================================================//
void RX8130_ScanBus(void);
void RX8130_ReInit(void);
void RX8130_DeInit(void);
bool RX8130_IsReady(void);
void RX8130_Restart(void);
bool RX8130_ReadReg(uint8_t u8Addr, uint8_t *pu8Dat);
bool RX8130_WriteReg(uint8_t u8Addr, uint8_t u8Dat);
bool RX8130_Init(void);
bool RX8130_InitTime(RTC_DateTypeDef *prtcInitDate, RTC_TimeTypeDef *prtcInitTime, 
	                   bool bIsBin);
bool RX8130_SetDateTime(RTC_DateTypeDef *prtcInitDate, RTC_TimeTypeDef *prtcInitTime, 
	                      bool bIsBin);
bool RX8130_GetDateTime(RTC_DateTypeDef *prtcRetDate, RTC_TimeTypeDef *prtcRetTime,
	                      bool bIsBin);
bool RX8130_CheckVBFF(void);
bool RX8130_CheckVBLF(void);


//============================================================================//
#endif //_RX8130_DRIVER_H_
