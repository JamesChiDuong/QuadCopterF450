/*
 * UART_Driver.h
 *
 *  Created on: Mar 1, 2025
 *      Author: PC
 */

#ifndef INC_USER_UART_DRIVER_H_
#define INC_USER_UART_DRIVER_H_
#include <USER_Ring_Buffer.h>
#include "stm32f4xx_hal.h"
#include "USER_Protocol.h"
typedef enum
{
	UART_PORT1 = 0x0,
	UART_PORT2,
	UART_PORT3,
	UART_PORT4,
	UART_PORT5
} UART_PortTypeDef;


#define UART_BUFFER_QUANTITY          5

#define EXT_COMPORT_DEF               UART_PORT5
#define GPS_COMPORT_DEF               UART_PORT2
#define SET_COMPORT_DEF               UART_PORT1
#define DB9_COMPORT_DEF               UART_PORT4
#define GSM_COMPORT_DEF               UART_PORT3

#define UART_BASE_BUFFER_SIZE         256

#define UART_RX_BUFFER_MAX		        (UART_BASE_BUFFER_SIZE * 4)

#define UART1_BUFFER_SIZE             (UART_BASE_BUFFER_SIZE * 6)
#define UART2_BUFFER_SIZE             (UART_BASE_BUFFER_SIZE * 4)
#define UART3_BUFFER_SIZE             (UART_BASE_BUFFER_SIZE * 2)
#define UART4_BUFFER_SIZE             (UART_BASE_BUFFER_SIZE * 2)
#define UART5_BUFFER_SIZE             (UART_BASE_BUFFER_SIZE * 0)
//bool UART_IsDebugMode(DebugModeTypeDef tDebugMode);
void UART_CheckInterrupt(void);
void UART_Setup(UART_PortTypeDef uartPort);
void UART_Reset(UART_PortTypeDef uartPort);
bool UART_SendBuffer(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
bool UART_SaveRecvData(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_GetRecvData(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_GetRecvDataPos(UART_PortTypeDef uartPort, uint16_t u16Pos,
                             uint8_t *pu8Dat, uint16_t u16DatLen);
uint16_t UART_PeekRecvData(UART_PortTypeDef uartPort, uint16_t u16Pos, uint8_t *pu8Dat,
                           uint16_t u16DatLen);
uint16_t UART_RecvDataLen(UART_PortTypeDef uartPort);
uint16_t UART_RecvDataLenPos(UART_PortTypeDef uartPort, uint16_t u16Pos);
bool UART_RecvFind(UART_PortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos);
bool UART_RecvFindPos(UART_PortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos,
                      uint16_t u16StartPos);
void UART_TerminalProc(void);


#endif /* INC_USER_UART_DRIVER_H_ */
