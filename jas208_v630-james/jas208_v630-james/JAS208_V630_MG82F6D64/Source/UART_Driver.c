//============================================================================//
// Include
//============================================================================//
#include "UART_Driver.h"

#include <string.h>

#include "API_Macro_MG82F6D64.H"
#include "API_Uart_BRGRL_MG82F6D64.H"
#include "IAP_Driver.h"
#include "Config.h"
#include "REG_MG82F6D64.H"
#include "Utility.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
//xdata u8 UartRxBuf[UART_NUM_MAX][UART_RX_BUFF_SIZE];
xdata u8 *pu8UART_Buf[UART_NUM_MAX] = {0};
xdata u16 u16UartRxIn[UART_NUM_MAX]={0};
xdata u16 u16UartRxOut[UART_NUM_MAX]={0};
xdata u8 u8CmdReceived[UART_NUM_MAX] = {0};
xdata u8 u8ConnType = 0; //UART0-3
xdata u8 u8CmdNdx[UART_NUM_MAX] = {0};
xdata u8 u8Command[UART_NUM_MAX][CMD_SIZE];
xdata u8 u8UART0_Buf[UART0_BUF_SIZE] = {0};
xdata u8 u8UART1_Buf[UART1_BUF_SIZE] = {0};
#if UART_NUM_MAX > 2 
xdata u8 u8UART2_Buf[UART2_BUF_SIZE] = {0};
#endif
#if UART_NUM_MAX > 3 
xdata u8 u8UART3_Buf[UART3_BUF_SIZE] = {0};
#endif
xdata const u16 u16UART_BufSize[] =
{
	UART0_BUF_SIZE,
	UART1_BUF_SIZE,
	UART2_BUF_SIZE,
	UART3_BUF_SIZE
};

//============================================================================//
// Extern
//============================================================================//
//extern u8 HexToChar(u8 x);

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  UART0 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void UART0_IntConfig(void)
{
	INT_SetIPHighest(IP_UART0);
	//UART0_EnTI0toS0Interrupt();
	INT_EnUART0();
}

//============================================================================//
// @brief  UART0 init.
// @param  bIntEn: 0: Disable UART0 interrupt, 1: Enable UART0 interrupt.
// @retval None.
//============================================================================//
void UART0_Init(bit bIntEn)
{
	UART0_SetAccess_S0CR1();			// Enable access S0CR1 
	UART0_SetMode8bitUARTVar();			// UART0 Mode: 8-bit, Variable B.R.
	UART0_EnReception();				// Enable reception
	UART0_SetBRGFromS0BRG();			// B.R. source: S0BRG
	UART0_SetS0BRGBaudRateX2();			// S0BRG x2
	UART0_SetS0BRGSelSYSCLK();			// S0BRG clock source: SYSCLK
	UART0_SetRxTxP30P31();
	// Sets B.R. value
#if EN_110592 == 1
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_11059200_1T);
	//UART0_SetS0BRGValue(S0BRG_BRGRL_57600_2X_11059200_1T);
	//UART0_SetS0BRGValue(S0BRG_BRGRL_38400_2X_11059200_1T);
	//UART0_SetS0BRGValue(S0BRG_BRGRL_19200_2X_11059200_1T);
	//UART0_SetS0BRGValue(S0BRG_BRGRL_9600_2X_11059200_1T);
#elif EN_221184 == 1
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_22118400_1T);
#elif EN_294912 == 1
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_29491200_1T);
#elif EN_442368 == 1
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_44236800_1T);
#else
	UART0_SetS0BRGValue(S0BRG_BRGRL_115200_2X_12000000_1T);
#endif
	UART0_EnS0BRG();					// Enable S0BRG
	if(bIntEn)
		UART0_IntConfig();
}

//============================================================================//
// @brief  UART0 deinit.
// @param  None.
// @retval None.
//============================================================================//
void UART0_DeInit(void)
{
	INT_DisUART0();
	UART0_DisReception();
	UART0_DisS0BRG();
}

#if ENABLE_UART1
//============================================================================//
// @brief  UART1 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void UART1_IntConfig(void)
{
	INT_SetIPXHighest(IPX_UART1);
	INT_EnUART1();			// Enable UART1 interrupt
}

//============================================================================//
// @brief  UART1 init.
// @param  bIntEn: 0: Disable UART1 interrupt, 1: Enable UART1 interrupt.
// @retval None.
//============================================================================//
void UART1_Init(bit bIntEn)
{
	UART1_SetMode8bitUARTVar();								// UART1 Mode: 8-bit, Variable B.R
	UART1_EnS1BRG();										//Enable S1BRG
	UART1_SetBaudRateX2();									//S1BRG x2
	//UART1_SetRxTxP34P35();									//UART1 Pin RX:P34 TX:P35
	UART1_SetRxTxP12P13();								//UART1 Pin RX:P12 TX:P13
	UART1_EnReception();									//Enable reception
	UART1_SetS1BRGSelSYSCLK();								//S1BRG clock source?SYSCLK1T
#if EN_110592 == 1
	UART1_SetS1BRGValue(S1BRG_BRGRL_115200_2X_11059200_1T);
	//UART1_SetS1BRGValue(S1BRG_BRGRL_57600_2X_11059200_1T);
#elif EN_221184 == 1
	UART1_SetS1BRGValue(S1BRG_BRGRL_115200_2X_22118400_1T);
#elif EN_294912 == 1
	UART1_SetS1BRGValue(S1BRG_BRGRL_115200_2X_29491200_1T);
#elif EN_442368 == 1
	UART1_SetS1BRGValue(S1BRG_BRGRL_115200_2X_44236800_1T);
#else
	UART1_SetS1BRGValue(S1BRG_BRGRL_115200_2X_12000000_1T);	//Sets B.R. value	
#endif
	if(bIntEn)
		UART1_IntConfig();
}

//============================================================================//
// @brief  UART1 deinit.
// @param  None.
// @retval None.
//============================================================================//
void UART1_DeInit(void)
{
	INT_DisUART1();
	UART1_DisReception();
	UART1_DisS1BRG();
}
#endif

#if ENABLE_UART2
//============================================================================//
// @brief  UART2 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void UART2_IntConfig(void)
{
	//INT_SetIPX2Highest(IPX2_UART2); // UART2 only supports highest prio
	INT_EnUART2();			// Enable UART2 interrupt
}

//============================================================================//
// @brief  UART2 init.
// @param  bIntEn: 0: Disable UART2 interrupt, 1: Enable UART2 interrupt.
// @retval None.
//============================================================================//
void UART2_Init(bit bIntEn)
{
	UART2_SetMode8bitUARTVar();								// UART2 Mode: 8-bit, Variable B.R
	UART2_EnS2BRG();										//Enable S2BRG
	UART2_SetBaudRateX2();									//S2BRG x2
	UART2_SetRxTxP32P33();									//UART2 Pin?RX:P32 TX:P33
	UART2_EnReception();									//Enable reception
	UART2_SetS2BRGSelSYSCLK();								//S2BRG clock source?SYSCLK
#if EN_110592 == 1
	UART2_SetS2BRGValue(S2BRG_BRGRL_115200_2X_11059200_1T);
#elif EN_221184 == 1
	UART2_SetS2BRGValue(S2BRG_BRGRL_115200_2X_22118400_1T);
#elif EN_294912 == 1
	UART2_SetS2BRGValue(S2BRG_BRGRL_115200_2X_29491200_1T);
#elif EN_442368 == 1
	UART2_SetS2BRGValue(S2BRG_BRGRL_115200_2X_44236800_1T);
#else
	UART2_SetS2BRGValue(S2BRG_BRGRL_115200_2X_12000000_1T);	//Sets B.R. value	
#endif
	if(bIntEn)
		UART2_IntConfig();
}

//============================================================================//
// @brief  UART2 deinit.
// @param  None.
// @retval None.
//============================================================================//
void UART2_DeInit(void)
{
	INT_DisUART2();
	UART2_DisReception();
	UART2_DisS2BRG();	
}
#endif

#if ENABLE_UART3
//============================================================================//
// @brief  UART3 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void UART3_IntConfig(void)
{
	//INT_SetIPX2Highest(IPX2_UART3); // UART3 only supports highest prio
	INT_EnUART3();			// Enable UART3 interrupt
}

//============================================================================//
// @brief  UART3 init.
// @param  bIntEn: 0: Disable UART3 interrupt, 1: Enable UART3 interrupt.
// @retval None.
//============================================================================//
void UART3_Init(bit bIntEn)
{
	UART3_SetMode8bitUARTVar();								// UART3 Mode: 8-bit, Variable B.R
	UART3_EnS3BRG();										//Enable S3BRG
	UART3_SetBaudRateX2();									//S3BRG x2
	UART3_SetRxTxP36P37();									//UART3 Pin?RX:P36 TX:P37
	UART3_EnReception();									//Enable reception
	UART3_SetS3BRGSelSYSCLK();								//S3BRG clock source?SYSCLK
#if EN_110592 == 1
	UART3_SetS3BRGValue(S3BRG_BRGRL_115200_2X_11059200_1T);
#elif EN_221184 == 1
	UART3_SetS3BRGValue(S3BRG_BRGRL_115200_2X_22118400_1T);
#elif EN_294912 == 1
	UART3_SetS3BRGValue(S3BRG_BRGRL_115200_2X_29491200_1T);
#elif EN_442368 == 1
	UART3_SetS3BRGValue(S3BRG_BRGRL_115200_2X_44236800_1T);
#else
	UART3_SetS3BRGValue(S3BRG_BRGRL_115200_2X_12000000_1T);	//Sets B.R. value
#endif
	if(bIntEn)
		UART3_IntConfig();
}

//============================================================================//
// @brief  UART3 deinit.
// @param  None.
// @retval None.
//============================================================================//
void UART3_DeInit(void)
{
	INT_DisUART3();
	UART3_DisReception();
	UART3_DisS3BRG();
}
#endif

//============================================================================//
// @brief  Set the baud rate for a UART port.
//         2: 9600, 4: 19200, 12: 57600, 
// @param  u8Port: UART port ID 0-3.
// @retval None.
//============================================================================//
void UART_SetBaudRate(u8 u8Port)
{
	u8 u8V;
	
	if (u8Port == 0) {
#if EN_110592 == 1
		u8V = (NV_Set.u8Baud[0] == 2) ? S0BRG_BRGRL_9600_2X_11059200_1T :
			(NV_Set.u8Baud[0] == 4) ? S0BRG_BRGRL_19200_2X_11059200_1T :
			(NV_Set.u8Baud[0] == 12) ? S0BRG_BRGRL_57600_2X_11059200_1T : S0BRG_BRGRL_115200_2X_11059200_1T;
#elif EN_221184 == 1
		u8V = (NV_Set.u8Baud[0] == 2) ? S0BRG_BRGRL_9600_2X_22118400_1T :
			(NV_Set.u8Baud[0] == 4) ? S0BRG_BRGRL_19200_2X_22118400_1T :
			(NV_Set.u8Baud[0] == 12) ? S0BRG_BRGRL_57600_2X_22118400_1T : S0BRG_BRGRL_115200_2X_22118400_1T;
#elif EN_294912 == 1
		u8V = (NV_Set.u8Baud[0] == 2) ? S0BRG_BRGRL_9600_2X_29491200_1T :
			(NV_Set.u8Baud[0] == 4) ? S0BRG_BRGRL_19200_2X_29491200_1T :
			(NV_Set.u8Baud[0] == 12) ? S0BRG_BRGRL_57600_2X_29491200_1T : S0BRG_BRGRL_115200_2X_29491200_1T;
#elif EN_442368 == 1
		u8V = (NV_Set.u8Baud[0] == 4) ? S0BRG_BRGRL_19200_2X_44236800_1T :
			(NV_Set.u8Baud[0] == 12) ? S0BRG_BRGRL_57600_2X_44236800_1T : S0BRG_BRGRL_115200_2X_44236800_1T;
#else //12000000
		u8V = (NV_Set.u8Baud[0] == 2) ? S0BRG_BRGRL_9600_2X_12000000_1T :
			(NV_Set.u8Baud[0] == 12) ? S0BRG_BRGRL_57600_2X_12000000_1T : S0BRG_BRGRL_115200_2X_12000000_1T;
#endif
		UART0_SetS0BRGValue(u8V);
	}
#if ENABLE_UART1 == 1
	else if (u8Port == 1) {
#if EN_110592 == 1 //12000000
		u8V = (NV_Set.u8Baud[1] == 2) ? S1BRG_BRGRL_9600_2X_11059200_1T :
			(NV_Set.u8Baud[1] == 4) ? S1BRG_BRGRL_19200_2X_11059200_1T :
			(NV_Set.u8Baud[1] == 12) ? S1BRG_BRGRL_57600_2X_11059200_1T : S1BRG_BRGRL_115200_2X_11059200_1T;
#elif EN_221184 == 1
		u8V = (NV_Set.u8Baud[1] == 2) ? S1BRG_BRGRL_9600_2X_22118400_1T :
			(NV_Set.u8Baud[1] == 4) ? S1BRG_BRGRL_19200_2X_22118400_1T :
			(NV_Set.u8Baud[1] == 12) ? S1BRG_BRGRL_57600_2X_22118400_1T : S1BRG_BRGRL_115200_2X_22118400_1T;
#elif EN_294912 == 1
		u8V = (NV_Set.u8Baud[1] == 2) ? S1BRG_BRGRL_9600_2X_29491200_1T :
			(NV_Set.u8Baud[1] == 4) ? S1BRG_BRGRL_19200_2X_29491200_1T :
			(NV_Set.u8Baud[1] == 12) ? S1BRG_BRGRL_57600_2X_29491200_1T : S1BRG_BRGRL_115200_2X_29491200_1T;
#elif EN_442368 == 1
		u8V = (NV_Set.u8Baud[1] == 4) ? S1BRG_BRGRL_19200_2X_44236800_1T :
			(NV_Set.u8Baud[1] == 12) ? S1BRG_BRGRL_57600_2X_44236800_1T : S1BRG_BRGRL_115200_2X_44236800_1T;
#else
		u8V = (NV_Set.u8Baud[1] == 2) ? S1BRG_BRGRL_9600_2X_12000000_1T :
			(NV_Set.u8Baud[1] == 12) ? S1BRG_BRGRL_57600_2X_12000000_1T : S1BRG_BRGRL_115200_2X_12000000_1T;
#endif
		UART1_SetS1BRGValue(u8V);
	}
#endif
#if ENABLE_UART2 == 1
	else if (u8Port == 2) {
#if EN_110592 == 1
		u8V = (NV_Set.u8Baud[2] == 2) ? S2BRG_BRGRL_9600_2X_11059200_1T :
			(NV_Set.u8Baud[2] == 4) ? S2BRG_BRGRL_19200_2X_11059200_1T :
			(NV_Set.u8Baud[2] == 12) ? S2BRG_BRGRL_57600_2X_11059200_1T : S2BRG_BRGRL_115200_2X_11059200_1T;
#elif EN_221184 == 1
		u8V = (NV_Set.u8Baud[2] == 2) ? S2BRG_BRGRL_9600_2X_22118400_1T :
			(NV_Set.u8Baud[2] == 4) ? S2BRG_BRGRL_19200_2X_22118400_1T :
			(NV_Set.u8Baud[2] == 12) ? S2BRG_BRGRL_57600_2X_22118400_1T : S2BRG_BRGRL_115200_2X_22118400_1T;
#elif EN_294912 == 1
		u8V = (NV_Set.u8Baud[2] == 2) ? S2BRG_BRGRL_9600_2X_29491200_1T :
			(NV_Set.u8Baud[2] == 4) ? S2BRG_BRGRL_19200_2X_29491200_1T :
			(NV_Set.u8Baud[2] == 12) ? S2BRG_BRGRL_57600_2X_29491200_1T : S2BRG_BRGRL_115200_2X_29491200_1T;
#elif EN_442368 == 1
		u8V = (NV_Set.u8Baud[2] == 4) ? S2BRG_BRGRL_19200_2X_44236800_1T :
			(NV_Set.u8Baud[2] == 12) ? S2BRG_BRGRL_57600_2X_44236800_1T : S2BRG_BRGRL_115200_2X_44236800_1T;
#else //12000000
		u8V = (NV_Set.u8Baud[2] == 2) ? S2BRG_BRGRL_9600_2X_12000000_1T :
			(NV_Set.u8Baud[2] == 12) ? S2BRG_BRGRL_57600_2X_12000000_1T : S2BRG_BRGRL_115200_2X_12000000_1T;
#endif
		UART2_SetS2BRGValue(u8V);
	}
#endif
#if ENABLE_UART3 == 1
	else if (u8Port == 3) {
#if EN_110592 == 1
		u8V = (NV_Set.u8Baud[3] == 2) ? S3BRG_BRGRL_9600_2X_11059200_1T :
			(NV_Set.u8Baud[3] == 4) ? S3BRG_BRGRL_19200_2X_11059200_1T :
			(NV_Set.u8Baud[3] == 12) ? S3BRG_BRGRL_57600_2X_11059200_1T : S3BRG_BRGRL_115200_2X_11059200_1T;
#elif EN_221184 == 1
		u8V = (NV_Set.u8Baud[3] == 2) ? S3BRG_BRGRL_9600_2X_22118400_1T :
			(NV_Set.u8Baud[3] == 4) ? S3BRG_BRGRL_19200_2X_22118400_1T :
			(NV_Set.u8Baud[3] == 12) ? S3BRG_BRGRL_57600_2X_22118400_1T : S3BRG_BRGRL_115200_2X_22118400_1T;
#elif EN_294912 == 1
		u8V = (NV_Set.u8Baud[3] == 2) ? S3BRG_BRGRL_9600_2X_29491200_1T :
			(NV_Set.u8Baud[3] == 4) ? S3BRG_BRGRL_19200_2X_29491200_1T :
			(NV_Set.u8Baud[3] == 12) ? S3BRG_BRGRL_57600_2X_29491200_1T : S3BRG_BRGRL_115200_2X_29491200_1T;
#elif EN_442368 == 1
		u8V = (NV_Set.u8Baud[3] == 4) ? S3BRG_BRGRL_19200_2X_44236800_1T :
			(NV_Set.u8Baud[3] == 12) ? S3BRG_BRGRL_57600_2X_44236800_1T : S3BRG_BRGRL_115200_2X_44236800_1T;
#else //12000000
		u8V = (NV_Set.u8Baud[3] == 2) ? S3BRG_BRGRL_9600_2X_12000000_1T :
			(NV_Set.u8Baud[3] == 12) ? S3BRG_BRGRL_57600_2X_12000000_1T : S3BRG_BRGRL_115200_2X_12000000_1T;
#endif
		UART3_SetS3BRGValue(u8V);
	}
#endif
}

//============================================================================//
// @brief  Init UART RX buffer.
// @param  None.
// @retval None.
//============================================================================//
void UART_BufInit(void)
{
	xdata u8 i;
	
	pu8UART_Buf[0] = u8UART0_Buf;
	pu8UART_Buf[1] = u8UART1_Buf;
	#if UART_NUM_MAX > 2
	pu8UART_Buf[2] = u8UART2_Buf;
	#endif
	#if UART_NUM_MAX > 3
	pu8UART_Buf[3] = u8UART3_Buf;
	#endif
	
	for(i = 0; i < UART_NUM_MAX; i++)
	{
		memset(pu8UART_Buf[i], 0x00, u16UART_BufSize[i]);
		u16UartRxIn[i] = 0;
		u16UartRxOut[i] = 0;
	}
}

//============================================================================//
// @brief  Get the number of bytes available for read.
// @param  u8Port: UART port ID 0-3.
// @retval None.
//============================================================================//
uint16_t UART_GetDataNum(u8 u8Port)
{
	xdata uint16_t u16Num;
	
	u16Num = (u16UartRxIn[u8Port] + u16UART_BufSize[u8Port] - u16UartRxOut[u8Port]) % u16UART_BufSize[u8Port];
	return u16Num;
}

//============================================================================//
// @brief  Reset the buffer of a UART port.
// @param  u8Port: UART port ID 0-3.
// @retval None.
//============================================================================//
void UART_ClearData(u8 u8Port)
{
	u16UartRxIn[u8Port] = u16UartRxOut[u8Port] = 0;
}

//============================================================================//
// @brief  Send a byte through a UART port.
// @param  u8Port: UART port ID 0-3.
//         u8Byte: Byte to send.
// @retval None.
//============================================================================//
void UART_SendByte(u8 u8Port, u8 u8Byte)
{
	if (u8Port == 0) { //UART0
		//_push_(SFRPI);		   
		//SFR_SetPage(0);	
		S0BUF=u8Byte;
		while(TI0==0);
		TI0=0;
		//_pop_(SFRPI);	
	}
#if ENABLE_UART1 == 1
	else if (u8Port == 1) { //UART1
		SFR_SetPage(1);
		S1BUF=u8Byte;
		while (TI1 == 0);
		TI1 = 0;
		SFR_SetPage(0);
	}
#endif
#if ENABLE_UART2 == 1
	else if (u8Port == 2) { //UART2
		SFR_SetPage(3);
		S2BUF=u8Byte;
		while (TI2 == 0);
		TI2 = 0;
		SFR_SetPage(0);
	}
#endif
#if ENABLE_UART3 == 1
	else if (u8Port == 3) { //UART3
		SFR_SetPage(4);
		S3BUF=u8Byte;
		while (TI3 == 0);
		TI3 = 0;
		SFR_SetPage(0);
	}
#endif
}

//============================================================================//
// @brief  Send data through a UART port.
// @param  u8Port: UART port ID 0-3.
//         pByte: Pointer to the data buffer.
//         u8Size: Data length.
// @retval None.
//============================================================================//
void UART_SendData(u8 u8Port, u8 *pByte, u8 u8Size)
{
	u8 u8i;
	
	for (u8i = 0; u8i < u8Size; u8i++)
		UART_SendByte(u8Port, pByte[u8i]);
}

//============================================================================//
// @brief  Send a string through a UART port.
// @param  u8Port: UART port ID 0-3.
//         pStr: Pointer to the string buffer.
// @retval None.
//============================================================================//
void UART_SendStr(u8 u8Port, u8* pStr)
{
	while(*pStr != 0)
	{
		UART_SendByte(u8Port, *pStr);
		pStr ++;
	}
}

#if 0
//============================================================================//
// @brief  Send a byte as HEX code to a UART port.
// @param  u8Port: UART port ID 0-3.
//         u8ToSend: Byte to send
// @retval None.
//============================================================================//
void Uart_SendHex(u8 u8Port, u8 u8ToSend)
{
	xdata u8 x;
	xdata u8 u8Char;
	
	x = u8ToSend>>4;
	x &= 0x0F;
	u8Char = HexToChar(x); //x=0-15
	UART_SendByte(u8Port, u8Char);
	x = u8ToSend&0x0F;
	u8Char = HexToChar(x);
	UART_SendByte(u8Port, u8Char);
}
#endif
