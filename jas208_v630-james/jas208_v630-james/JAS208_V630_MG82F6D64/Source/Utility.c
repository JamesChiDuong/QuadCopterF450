//============================================================================//
// Include
//============================================================================//
#include "Utility.h"

#include <Absacc.h>
#include "ctype.h" 
#include <Intrins.h>
#include <stdio.h>
#include <string.h>

#include "API_Macro_MG82F6D64.H"
#include "API_Uart_BRGRL_MG82F6D64.H"
#include "Config.h"
#include "I2C_Driver.h"
#include "IAP_Driver.h"
#include "IO_Driver.h"
#include "main.h"
#include "MG82F6D64_COMMON_DRV.h"
#include "MG82F6D64_INT_DRV.h"
#include "MG82F6D64_INT_VECTOR.h"
#include "MG82F6D64_RTC_DRV.h"
#include "REG_MG82F6D64.H"
#include "RTC_Driver.h"
#include "System.h"
#include "Timer_Driver.h"
#include "Type.h"
#include "UART_Driver.h"
#include "Version.h"

//============================================================================//
// Extern
//============================================================================//
extern void SetPowerMode(u8 u8Mode);

//============================================================================//
// Function prototype
//============================================================================//
void PrintDateTime(void);
void PrintSysInfo(void);
void PrintBaudRate(void);
void PrintUartRecv(void);

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  Delay in us.
// @param  xUs: Number of us to delay.
// @retval None.
//============================================================================//
#if ENABLE_DELAYUS == 1
void DelayXus(u8 xUs)
{
	while(xUs!=0)
	{
#if (MCU_CPUCLK>=11059200)
		_nop_();
#endif
#if (MCU_CPUCLK>=14745600)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=16000000)
		_nop_();
#endif

#if (MCU_CPUCLK>=22118400)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=24000000)
		_nop_();
		_nop_();
#endif		
#if (MCU_CPUCLK>=29491200)
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#endif
#if (MCU_CPUCLK>=32000000)
		_nop_();
		_nop_();
#endif

		xUs--;
	}
}
#endif

//============================================================================//
// @brief  Delay in ms.
// @param  xMs: Number of Ms to delay.
// @retval None.
//============================================================================//
#if ENABLE_DELAYUS == 1 && ENABLE_DELAYMS == 1
void DelayXms(u16 xMs)
{
	while(xMs!=0)
	{
		CLRWDT();
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		DelayXus(200);
		xMs--;
	}
}
#endif

#if 0
//============================================================================//
// @brief  Change a hex value to corresponding character value.
// @param  x: Hex value to change.
// @retval u8: The corresponding character value.
//============================================================================//
u8 HexToChar(u8 x)
{
	if(x < 10) { //0-9
		return ('0'+x);
	}
	else { //A-F
		return ('A'+x-10);
	}
}
#endif

//============================================================================//
// @brief  Convert all charaters in a string to upper case.
// @param  szText: Input string.
// @retval None.
//============================================================================//
static void strupr(char *szText)
{
	xdata int i, nLen;
	nLen = strlen(szText);
	for (i = 0; i < nLen; i++)
		szText[i] = toupper(szText[i]);
}

//============================================================================//
// @brief  Decipher external command.
// @param  szCommand: Command buffer.
// @retval None.
//============================================================================//
void DecipherCommand(char *szCommand)
{
	char *p;
	xdata int nPN;
	xdata u8 u8i;
	xdata int nV1, nV2, nV3,
	          nV4, nV5, nV6;
	xdata char cBuf[100] = {0};
	//
	strupr(szCommand);
#define CMD_BAUD	"BAUD:"
#define LEN_BAUD	(sizeof(CMD_BAUD)-1)
	if (strncmp(szCommand, CMD_BAUD, LEN_BAUD) == 0) {
		p = &szCommand[LEN_BAUD];
		if (*p == '?') {
			PrintBaudRate();
			return;
		}
		nPN = sscanf(p, "%d:%d", &nV1, &nV2);
		if ((nPN != 2) || (nV1 >= UART_NUM_MAX))
			goto DecipherFail;
		NV_Set.u8Baud[nV1] = (u8)nV2;
		UART_SetBaudRate((u8)nV1);
		UART_SendStr(u8ConnType, "BAUD OK\r\n");
		return;
	}
#define CMD_CLRNV	"CLRNV"
#define LEN_CLRNV	(sizeof(CMD_CLRNV)-1)
	if (strncmp(szCommand, CMD_CLRNV, LEN_CLRNV) == 0) {
		ClearNV();
		UART_SendStr(u8ConnType, "CLRNV OK\r\n");
		return;
	}
#define CMD_DATE	"DATE:"
#define LEN_DATE	(sizeof(CMD_DATE)-1)
	if (strncmp(szCommand, CMD_DATE, LEN_DATE) == 0) {
		p = &szCommand[LEN_DATE];
		if (*p == '?') {
			PrintDateTime();
			return;
		}
		nPN = sscanf(p, "%d-%d-%d", &nV1, &nV2, &nV3);
		if (nPN != 3)
			goto DecipherFail;
		if (nV1 >= 2000)
			DateTime.u8Year = (u8)(nV1 - 2000);
		else
			DateTime.u8Year = (u8)nV1;
		DateTime.u8Mon = (u8)nV2;
		DateTime.u8Day = (u8)nV3;
		UART_SendStr(u8ConnType, "DATE OK\r\n");
		return;
	}
#define CMD_DATETIME	"DATETIME:"
#define LEN_DATETIME	(sizeof(CMD_DATETIME)-1)
	if (strncmp(szCommand, CMD_DATETIME, LEN_DATETIME) == 0) {
		p = &szCommand[LEN_DATETIME];
		if (*p == '?') {
			PrintDateTime();
			return;
		}
		nPN = sscanf(p, "%d-%d-%d", 
		             &nV1, &nV2, &nV3);
		if (nPN != 3)
			goto DecipherFail;
		for(u8i = 0; u8i < strlen(p); u8i++)
		{
		  if(p[u8i] == ',')
				p[u8i] = 0;
		}
		p += strlen(p) + 1;
		nPN = sscanf(p, "%d:%d:%d", 
		             &nV4, &nV5, &nV6);
		if (nPN != 3)
			goto DecipherFail;
		if (nV1 >= 2000)
			DateTimeTemp.u8Year = (u8)(nV1 - 2000);
		else
			DateTimeTemp.u8Year = (u8)nV1;
		// Check date time validality 
		if((nV2 > 12) || (nV3 > 31) ||
			 (nV4 > 23) || (nV5 > 59) || (nV6 > 59))
			goto DecipherFail;
		DateTimeTemp.u8Mon = (u8)nV2;
		DateTimeTemp.u8Day = (u8)nV3;
		DateTimeTemp.u8Hour = (u8)nV4;
		DateTimeTemp.u8Min = (u8)nV5;
		DateTimeTemp.u8Sec = (u8)nV6;
		bRTC_SetTimeFlag = TRUE;
		memcpy(&DateTime, &DateTimeTemp, sizeof(DATE_TIME));
		memset(cBuf, 0x00, sizeof(cBuf));
		UART_SendStr(u8ConnType, "DATETIME OK\r\n");
		return;
	}
#define CMD_DEBUG	"DEBUG:"
#define LEN_DEBUG	(sizeof(CMD_DEBUG)-1)
	if (strncmp(szCommand, CMD_DEBUG, LEN_DEBUG) == 0) {
		p = &szCommand[LEN_DEBUG];
		if (*p == '?') {
			sprintf(cBuf, "DEBUG=%d\r\n", (int)SysParam.u8Debug);
			UART_SendStr(u8ConnType, cBuf);
			UART_SendStr(u8ConnType, "1: date time\r\n");
			UART_SendStr(u8ConnType, "2: sys info\r\n");
			UART_SendStr(u8ConnType, "3: slave i2c block write\r\n");
			UART_SendStr(u8ConnType, "4: UART0 TX\r\n");
			UART_SendStr(u8ConnType, "5: UART1 TX\r\n");
			UART_SendStr(u8ConnType, "6: UART2 TX\r\n");
			UART_SendStr(u8ConnType, "7: UART3 TX\r\n");
			return;
		}
		nPN = sscanf(p, "%d", &nV1);
		if (nPN != 1)
			goto DecipherFail;
		SysParam.u8Debug = (u8)nV1;
		UART_SendStr(u8ConnType, "DEBUG OK\r\n");
		return;
	}
#define CMD_ENWDT	"ENWDT:"
#define LEN_ENWDT	(sizeof(CMD_ENWDT)-1)
	if (strncmp(szCommand, CMD_ENWDT, LEN_ENWDT) == 0) {
		p = &szCommand[LEN_ENWDT];
		nPN = sscanf(p, "%d", &nV1);
		if (nPN != 1)
			goto DecipherFail;
		SysParam.u8EnableWDT = nV1;
		UART_SendStr(u8ConnType, "ENWDT OK\r\n");
		return;
	}
#define CMD_GETSYS	"GETSYS"
#define LEN_GETSYS	(sizeof(CMD_GETSYS)-1)
	if (strncmp(szCommand, CMD_GETSYS, LEN_GETSYS) == 0) {
		PrintSysInfo();
		return;
	}
#define CMD_GETVER	"GETVER"
#define LEN_GETVER	(sizeof(CMD_GETVER)-1)
	if (strncmp(szCommand, CMD_GETVER, LEN_GETVER) == 0) {
		sprintf(cBuf, "%d-%d-%d ", FW_DATE_Y, (int)FW_DATE_M, (int)FW_DATE_D);
		UART_SendStr(u8ConnType, cBuf);
		sprintf(cBuf, "%d.%d.%d\r\n", (int)VER_MAJOR, (int)VER_MINOR, (int)VER_BUILD);
		UART_SendStr(u8ConnType, cBuf);
		return;
	}
#define CMD_HELP	"HELP"
#define LEN_HELP	(sizeof(CMD_HELP)-1)
	if (strncmp(szCommand, CMD_HELP, LEN_HELP) == 0) {
		UART_SendStr(u8ConnType, "BAUD:<P>:<NV>   P=0-3\r\n"); 
		UART_SendStr(u8ConnType, "CLRNV\r\n");
		UART_SendStr(u8ConnType, "DATE:Y-M-D\r\n");
		UART_SendStr(u8ConnType, "DATETIME:Y-M-D,hh:mm:ss\r\n");
		UART_SendStr(u8ConnType, "ENWDT:<NV>\r\n");
		UART_SendStr(u8ConnType, "GETSYS   get system information\r\n");
		UART_SendStr(u8ConnType, "GETVER   get version\r\n");
		UART_SendStr(u8ConnType, "IN2OUT:<NV>\r\n");
		UART_SendStr(u8ConnType, "OUT:<NV1>:<NV2> NV1=0-7,NV2=0-1\r\n");
		UART_SendStr(u8ConnType, "POWER:<NV>      1:idle,2:down\r\n");
		UART_SendStr(u8ConnType, "RESET           reset UART\r\n");
		UART_SendStr(u8ConnType, "SAVENV\r\n");
		UART_SendStr(u8ConnType, "TIME:hh:mm:ss\r\n");
		return;
	}
#define CMD_I2CRST	"I2CRST"
#define LEN_I2CRST	(sizeof(CMD_I2CRST)-1)
	if (strncmp(szCommand, CMD_I2CRST, LEN_I2CRST) == 0) {
		TWI0_DeInit();
		DelayXms(1);
		TWI0_Init(1);
		UART_SendStr(u8ConnType, "I2CRST OK\r\n");
		return;
	}
#define CMD_KEYRST	"KEYRST:"
#define LEN_KEYRST	(sizeof(CMD_KEYRST)-1)
	if (strncmp(szCommand, CMD_KEYRST, LEN_KEYRST) == 0) {
		p = &szCommand[LEN_KEYRST];
		if (*p == '?') {
			sprintf(cBuf, "KEYRST EN: %d", (int)NV_Set.u8KeyResetEn);
			UART_SendStr(u8ConnType, cBuf);
			return;
		}
		nPN = sscanf(p, "%d", &nV1);
		if ((nPN != 1) || (nV1 > 1))
			goto DecipherFail;
		NV_Set.u8KeyResetEn = (u8)nV1;
		UART_SendStr(u8ConnType, "KEYRST OK\r\n");
		return;
	}
#define CMD_LOADNV	"LOADNV"
#define LEN_LOADNV	(sizeof(CMD_LOADNV)-1)
	if (strncmp(szCommand, CMD_LOADNV, LEN_LOADNV) == 0) {
		LoadNV();
		UART_SendStr(u8ConnType, "LOADNV OK\r\n");
		return;
	}
#define CMD_OUT	"OUT:"
#define LEN_OUT	(sizeof(CMD_OUT)-1)
	if (strncmp(szCommand, CMD_OUT, LEN_OUT) == 0) {
		p = &szCommand[LEN_OUT];
		nPN = sscanf(p, "%d:%d", &nV1, &nV2);
		if (nPN != 2)
			goto DecipherFail;
		SetOutPin(nV1, nV2);
		UART_SendStr(u8ConnType, "OUT OK\r\n");
		return;
	}
#define CMD_POWER	"POWER:"
#define LEN_POWER	(sizeof(CMD_POWER)-1)
	if (strncmp(szCommand, CMD_POWER, LEN_POWER) == 0) {
		p = &szCommand[LEN_POWER];
		nPN = sscanf(p, "%d", &nV1);
		if (nPN != 1)
			goto DecipherFail;
		UART_SendStr(u8ConnType, "POWER OK\r\n");
		SaveNV();
		SetPowerMode((u8)nV1); //1:IDLE,2:DOWN
		return;
	}
#define CMD_PEDWKUP	"PEDWKUP:"
#define LEN_PEDWKUP	(sizeof(CMD_PEDWKUP)-1)
	if (strncmp(szCommand, CMD_PEDWKUP, LEN_PEDWKUP) == 0) {
		p = &szCommand[LEN_PEDWKUP];
		if (*p == '?') {
			memset(cBuf, 0x00, sizeof(cBuf));
			sprintf(cBuf, "PED WKUP: %d-%d-%d\r\n",
			        (int)NV_Set.tPedWkup.u8Enable,
							(int)NV_Set.tPedWkup.u32Timeout,
							(int)NV_Set.tPedWkup.u32Counter);
			UART_SendStr(u8ConnType, cBuf);
		}
		UART_SendStr(u8ConnType, "PEDWKUP OK\r\n");
		return;
	}
#define CMD_RESET	"RESET"
#define LEN_RESET	(sizeof(CMD_RESET)-1)
	if (strncmp(szCommand, CMD_RESET, LEN_RESET) == 0) {
		for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
			UART_ClearData(u8i);
		UART_SendStr(u8ConnType, "RESET OK\r\n");
		return;
	}
#define CMD_RESET_MCU	"RSTMCU"
#define LEN_RESET_MCU	(sizeof(CMD_RESET_MCU)-1)
	if (strncmp(szCommand, CMD_RESET_MCU, LEN_RESET_MCU) == 0) {
		SendRstPulse(N_RESET_MCU_PIN, 1);
		UART_SendStr(u8ConnType, "RESET MCU OK\r\n");
	}
#define CMD_SAVENV	"SAVENV"
#define LEN_SAVENV	(sizeof(CMD_SAVENV)-1)
	if (strncmp(szCommand, CMD_SAVENV, LEN_SAVENV) == 0) {
		SaveNV();
		UART_SendStr(u8ConnType, "SAVENV OK\r\n");
		return;
	}
#define CMD_TIME	"TIME:"
#define LEN_TIME	(sizeof(CMD_TIME)-1)
	if (strncmp(szCommand, CMD_TIME, LEN_TIME) == 0) {
		p = &szCommand[LEN_TIME];
		if (*p == '?') {
			PrintDateTime();
			return;
		}
		nPN = sscanf(p, "%d-%d-%d", &nV1, &nV2, &nV3);
		if (nPN != 3)
			goto DecipherFail;
		memcpy(&DateTimeTemp, &DateTime, sizeof(DATE_TIME));
		DateTimeTemp.u8Hour = (u8)nV1;
		DateTimeTemp.u8Min = (u8)nV2;
		DateTimeTemp.u8Sec = (u8)nV3;
		bRTC_SetTimeFlag = TRUE;
		return;
	}
#define CMD_WKUP_MCU	"WKUPMCU"
#define LEN_WKUP_MCU	(sizeof(CMD_WKUP_MCU)-1)
	if (strncmp(szCommand, CMD_WKUP_MCU, LEN_WKUP_MCU) == 0) {
		SendWkupPulse(MCU_WKUP_PIN, 1);
	}
DecipherFail:
	return;
}

//============================================================================//
// @brief  Print datetime.
// @param  None.
// @retval None.
//============================================================================//
void PrintDateTime(void)
{
	xdata char cBuf[32] = {0};
	
	sprintf(cBuf, "20%02d-%02d-%02d ", (int)DateTime.u8Year, (int)DateTime.u8Mon, (int)DateTime.u8Day);
	UART_SendStr(u8ConnType, cBuf);
	sprintf(cBuf, "%02d:%02d:%02d\r\n", (int)DateTime.u8Hour, (int)DateTime.u8Min, (int)DateTime.u8Sec);
	UART_SendStr(u8ConnType, cBuf);
}

//============================================================================//
// @brief  Print system info.
// @param  None.
// @retval None.
//============================================================================//
void PrintSysInfo(void)
{
	xdata u8 u8i;
	xdata u8 u8V;
	xdata char cBuf[32] = {0};
	
	sprintf(cBuf, "DEBUG=%d\r\n", (int)SysParam.u8Debug);
	UART_SendStr(u8ConnType, cBuf);
	u8V = P0;
	sprintf(cBuf, "P0=0X%02X\r\n", (int)u8V);
	UART_SendStr(u8ConnType, cBuf);
	for (u8i = 3; u8i < 6; u8i++) { //P2
		sprintf(cBuf, "OUT%d=%d\r\n", (int)u8i, (int)SysParam.u8IO_Out[u8i]);
		UART_SendStr(u8ConnType, cBuf);
	}
	u8V = P1;
	sprintf(cBuf, "P1=0X%02X\r\n", (int)u8V);
	UART_SendStr(u8ConnType, cBuf);
	PrintUartRecv();
	sprintf(cBuf, "WDTRST=%d\r\n", (int)SysParam.u8WDTRST);
	UART_SendStr(u8ConnType, cBuf);
}

//============================================================================//
// @brief  Print baudrate.
// @param  None.
// @retval None.
//============================================================================//
void PrintBaudRate(void)
{
	xdata u8 u8i;
	xdata char cBuf[32] = {0};
	
	for (u8i = 0; u8i < UART_NUM_MAX; u8i++) {
		sprintf(cBuf, "BAUD=%d,%d\r\n", (int)u8i, (int)NV_Set.u8Baud[u8i]);
		UART_SendStr(u8ConnType, cBuf);
	}
}

//============================================================================//
// @brief  Print received UART data bytes.
// @param  None.
// @retval None.
//============================================================================//
void PrintUartRecv(void)
{
	xdata u8 u8i;
	xdata char cBuf[32] = {0};
	
	sprintf(cBuf, "UART=%d\r\n", (int)u8ConnType);
	UART_SendStr(u8ConnType, cBuf);
	for (u8i = 0; u8i < UART_NUM_MAX; u8i++) { //Uart receive buffer
		sprintf(cBuf, "UART_RECV_BUF=%d,%d\r\n", (int)u8i, UART_GetDataNum(u8i));
		UART_SendStr(u8ConnType, cBuf);
	}
}

