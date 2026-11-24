#ifndef _UART_EXTDRIVER_H_
#define _UART_EXTDRIVER_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include <stdint.h>

#include "DataType.h"
#include "Protocol.h"
#include "RingBuf.h"
#include "MG82_Driver.h"


//============================================================================//
// User Defines
//============================================================================//

#define UART_EXT_BUFFER_QUANTITY      		MG82_UART_CHANNEL_MAX // 4

#define UART_EXT_BASE_BUFFER_SIZE         256

#define UART_EXT_RX_BUFFER_MAX		        (UART_EXT_BASE_BUFFER_SIZE * 4)

#define UART0_EXT_BUFFER_SIZE             (UART_EXT_BASE_BUFFER_SIZE * 4)
#define UART1_EXT_BUFFER_SIZE             (UART_EXT_BASE_BUFFER_SIZE * 4)
#define UART2_EXT_BUFFER_SIZE             (UART_EXT_BASE_BUFFER_SIZE * 4)
#define UART3_EXT_BUFFER_SIZE             (UART_EXT_BASE_BUFFER_SIZE * 4)

#define UART_EXT_TX_LEN_MAX               MG82_I2C_DATA_SIZE // Maximum UART EXT TX size

#define UART_EXT_RX_CHK_TIME              100U // Check UART EXT RX buffer in MG82 IC period,
																							 // unit: ms.


//============================================================================//
// User Typedef Variables
//============================================================================//
#if ENABLE_MG82
typedef enum
{
	UART_EXT_PORT0 = 0x0,
	UART_EXT_PORT1,
	UART_EXT_PORT2,
	UART_EXT_PORT3
} UART_ExtPortTypeDef;

/*
// UART baud rate define
typedef enum
{
	BR_NONE	= 0U,
	BR_4800bps	= 4800,
	BR_9600bps	= 9600,
	BR_19200bps = 19200,
	BR_38400bps = 38400,
	BR_57600bps = 57600,
	BR_115200bps = 115200,
	BR_230400bps = 230400,
	BR_460800bps = 460800,
	BR_921600bps = 921600,
} UART_EXT_BaudRateDef_t;
*/
#endif
//============================================================================//
// User Externs
//============================================================================//
#if ENABLE_MG82
extern RingBufTypeDef rbBufExt[4];
#endif


//============================================================================//
// User Function Prototypes
//============================================================================//
#if ENABLE_MG82
void UART_EXT_Setup(UART_ExtPortTypeDef uartPort);
void UART_EXT_SetupAll(void);
bool UART_EXT_ReInit(UART_ExtPortTypeDef uartPort, uint32_t u32BaudRate);
void UART_EXT_Reset(UART_ExtPortTypeDef uartPort);
bool UART_EXT_SendBuffer(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
void UART_EXT_GetRX(void);
bool UART_EXT_SaveRecvData(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_EXT_GetRecvData(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_EXT_GetRecvDataPos(UART_ExtPortTypeDef uartPort, uint16_t u16Pos,
																 uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_EXT_PeekRecvData(UART_ExtPortTypeDef uartPort, uint16_t u16Pos, uint8_t *pu8Dat,
                               uint16_t u16DatLen);
uint16_t UART_EXT_RecvDataLen(UART_ExtPortTypeDef uartPort);
uint16_t UART_EXT_RecvDataLenPos(UART_ExtPortTypeDef uartPort, uint16_t u16Pos);
bool UART_EXT_RecvFind(UART_ExtPortTypeDef uartPort, char* pcDat, uint16_t* pu16Pos);
bool UART_EXT_RecvFindPos(UART_ExtPortTypeDef uartPort, char* pcDat, uint16_t* pu16Pos,
                          uint16_t u16StartPos);
#endif

//============================================================================//
#endif //_UART_EXTDRIVER_H_
