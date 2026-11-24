#ifndef _UART_DRIVER_H
#define _UART_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define INVALID_REG_ADDR	0xFF
// Get UART buffer received data size
#define __UART_GET_DATA_SIZE(x)((u16UartRxIn[x] + u16UART_BufSize[x] - u16UartRxOut[x]) % u16UART_BufSize[x])
//#define CR		0x0d
#define LF		0x0a	//line feed
//#define UART_RX_BUFF_SIZE	256 // UART buf size, must be smaller than 65536
#define CMD_SIZE		64
#define CMD_SIZE_M1		63
#define UART0_BUF_SIZE    512
#define UART1_BUF_SIZE    10
#define UART2_BUF_SIZE    512
#define UART3_BUF_SIZE    10

//============================================================================//
// Extern
//============================================================================//
//extern xdata u8 UartRxBuf[UART_NUM_MAX][UART_RX_BUFF_SIZE];
extern xdata u8 *pu8UART_Buf[UART_NUM_MAX];
extern xdata u16 u16UartRxIn[UART_NUM_MAX];
extern xdata u16 u16UartRxOut[UART_NUM_MAX];
extern xdata u8 u8CmdReceived[UART_NUM_MAX];
extern xdata u8 u8ConnType; //UART0-3
extern xdata u8 u8CmdNdx[UART_NUM_MAX];
extern xdata u8 u8Command[UART_NUM_MAX][CMD_SIZE];
extern xdata const u16 u16UART_BufSize[];

//============================================================================//
// Function
//============================================================================//
void UART0_Init(bit bIntEn);
void UART0_DeInit(void);
void UART0_IntConfig(void);
#if ENABLE_UART1
void UART1_Init(bit bIntEn);
void UART1_DeInit(void);
void UART1_IntConfig(void);
#endif
#if ENABLE_UART2
void UART2_Init(bit bIntEn);
void UART2_DeInit(void);
void UART2_IntConfig(void);
#endif
#if ENABLE_UART3
void UART3_Init(bit bIntEn);
void UART3_DeInit(void);
void UART3_IntConfig(void);
#endif
void UART_SetBaudRate(u8 u8Port);
void UART_BufInit(void);
uint16_t UART_GetDataNum(u8 u8Port);
void UART_ClearData(u8 u8Port);
void UART_SendByte(u8 u8Port, u8 u8Byte);
void UART_SendData(u8 u8Port, u8 *pByte, u8 u8Size);
void UART_SendStr(u8 u8Port, u8* pStr);
//void Uart_SendHex(u8 u8Port, u8 u8ToSend);
	
#endif // _UART_DRIVER_H