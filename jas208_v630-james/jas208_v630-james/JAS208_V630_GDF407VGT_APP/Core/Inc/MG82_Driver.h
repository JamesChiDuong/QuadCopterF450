#ifndef _MG82_DRIVER_H_
#define _MG82_DRIVER_H_
//============================================================================//

//============================================================================//
// User Includes
//============================================================================//

#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "DataType.h"
#include "System.h"
#include "UART_Driver.h"

//============================================================================//
// User Defines
//============================================================================//

// MG82 slave address
#define MG82_SLAVE_ADDR 0x54 // 01010100

// MG82 address when communicating using STM32 HAL I2C Driver
#define MG82_ADDR_I2C (MG82_SLAVE_ADDR << 1) // 0xA8 10101000

// master block write, slave receive
#define MG82_CMD_UART_TX 0x01		 // master block write, UART TX
#define MG82_CMD_UART_BR_SET 0x02	 // master set UART Baudrate
#define MG82_CMD_UART_RST 0x03		 // master clear UART buffer
#define MG82_CMD_UART_RST_ALL 0x04	 // master clear all UART buffer
#define MG82_CMD_IO_SET 0x07		 // set IO-OUT
#define MG82_CMD_DT_SET 0x08		 // set date time
#define MG82_CMD_MCU_WDT_SET 0x09	 // MCU WDT enable/disable
#define MG82_CMD_MCU_ADC_SET 0x0A	 // ADC0 enable/disable
#define MG82_CMD_SW_RESET 0x0B		 // MCU software reset
#define MG82_CMD_PWR_MODE 0x0C		 // set device power mode
#define MG82_CMD_RS485_SET 0x0D		 // MCU UART5 mode select between RS232 and RS485
#define MG82_CMD_4G_DTR 0x0E		 // Change 4G DTR pin state
#define MG82_CMD_KEY_RST 0x0F		 // Enable/ Disable key reset
#define MG82_CMD_SHUTDOWN 0x10		 // Turn off MAIN_PWR and send MG82 to pwr down mode
#define MG82_CMD_RTC_WKUP 0x11		 // set alarm mode or periodic wake up setting
#define MG82_CMD_USBPROTECT_SET 0x12 // Set USB Protection circuit
// master block read, slave transmit
#define MG82_CMD_UART_CHK 0x80	   // get all UART available data size
#define MG82_CMD_UART0_RX 0x81	   // UART0 RX
#define MG82_CMD_UART1_RX 0x82	   // UART1 RX
#define MG82_CMD_UART2_RX 0x83	   // UART2 RX
#define MG82_CMD_UART3_RX 0x84	   // UART3 RX
#define MG82_CMD_UART_BR 0x85	   // get all UART BR
#define MG82_CMD_UART0_BR 0x86	   // UART0 BR
#define MG82_CMD_UART1_BR 0x87	   // UART1 BR
#define MG82_CMD_UART2_BR 0x88	   // UART2 BR
#define MG82_CMD_UART3_BR 0x89	   // UART3 BR
#define MG82_CMD_DT_GET 0x8A	   // get date time
#define MG82_CMD_IO_GET 0x8B	   // get IO status
#define MG82_CMD_MCU_WDT_GET 0x8C  // get MCU WDT Enable status
#define MG82_CMD_ADC_MODE 0x8D	   // get ADC0 Enable status
#define MG82_CMD_ADC_GET 0x8E	   // get ADC0 value
#define MG82_CMD_RS485_MODE 0x8F   // UART5 mode as RS232 or RS485
#define MG82_CMD_GET_VER 0x90	   // get MG82 FW version
#define MG82_CMD_OW_CHK_STAT 0x91  // check 1-Wire operation status
#define MG82_CMD_OW_GET_DATA 0x92  // get data from 1-Wire data buffer
#define MG82_CMD_OW_GET 0x93	   // get 1-Wire operation status and data from 1-Wire data buffer
#define MG82_CMD_GET_WKUP_SRC 0x94 // get MCU wake-up source
#define MG82_CMD_GET_KEYRST 0x95   // get key reset option
#define MG82_CMD_GPS_RMC 0x96	   // get GPS RMC message
#define MG82_CMD_GPS_RESP 0x97	   // get GPS PMTK response
#define MG82_CMD_RTC_WKUP_GET 0x98 // get current RTC wake up setting

#define MG82_I2C_HEADER_SIZE 4U											 // I2C packet header size
#define MG82_I2C_DATA_SIZE 128U											 // I2C packet data size max
#define MG82_I2C_PACKET_SIZE (MG82_I2C_HEADER_SIZE + MG82_I2C_DATA_SIZE) // I2C packet size max

#define MG82_MAX_TRIALS 5U

#define MG82_MAX_TIMEOUT 100U

#define MG82_ERR_CNT_MAX 200U

#define MG82_RESTART_CNT_MAX 2U

#define MG82_REPORT_ERR_TIMEOUT S_2_mS(120)

#define MG82_SHUTDOWN_DELAY_MAX MINUTE_2_SECOND(60U) // MG82 maximum delay time before shutting down main power in seconds

#define MG82_PED_WKUP_TIME_MAX DAY_2_SECOND(2U) // Maximum wakeup period time in seconds

#define MG82_UART_CHANNEL_MAX 4U // Maximum UART channel ID

#define MG82_IO_OUT_MAX 4U // Maximum IO out pin

#define MG82_EXT_UART0_BR BR_115200bps
#define MG82_EXT_UART1_BR BR_115200bps
#define MG82_EXT_UART2_BR BR_115200bps
#define MG82_EXT_UART3_BR BR_115200bps

#define MG82_PWR_MODE_DEF MG82_PWR_MODE_SUB_CLK

#if HW_TYPE == HW_JAS306SD
// OW commands
#define OW_CMD_READ_ROM 0x33
#define OW_CMD_MATCH_ROM 0x55
#define OW_CMD_SKIP_ROM 0xCC
#define OW_CMD_SEARCH_ALM 0xEC
#define OW_CMD_SEARCH_ROM 0xF0
#define OW_RESET 0xF1
#define OW_WRITE 0xF2
#define OW_READ 0xF3
#define OW_SEARCH_INIT 0xF4
#define OW_SET_PU 0xF5
#endif

//============================================================================//
// User Typedef
//============================================================================//
#if ENABLE_MG82
typedef enum
{
	MG82_PWR_MODE_NORMAL = 0U, // 0
	MG82_PWR_MODE_IDLE,		   // 1, Idle
	MG82_PWR_MODE_PD,		   // 2, Powerdown
	MG82_PWR_MODE_SLOW,		   // 3, Slow
	MG82_PWR_MODE_SUB_CLK,	   // 4, Sub-clock

} MG82_PwrTypeDef;

typedef enum
{
	MG82_WKUP_SRC_NONE = 0U,
	MG82_WKUP_SRC_ACC,
	MG82_WKUP_SRC_4G_RI,
	MG82_WKUP_SRC_G_SEN

} MG82_WkupSrcTypeDef;

typedef enum
{
	MG82_RTC_WKUP_NONE = 0,
	MG82_RTC_WKUP_ALM,
	MG82_RTC_WKUP_PERIODIC
} MG82_RTC_WKUP_TypeDef;

#pragma pack(1)
typedef struct
{
	uint8_t u8Command;
	uint8_t u8DataNum;
	uint8_t u8Param1;
	uint8_t u8Param2;
	uint8_t u8Data[MG82_I2C_DATA_SIZE];

} MG82_CmdTypeDef;

#if HW_TYPE == HW_JAS306SD
typedef struct
{
	uint8_t u8Cmd;
	uint8_t u8DataLen;
	uint8_t u8Data[MG82_I2C_DATA_SIZE];

} OW_DataTypeDef;
#endif
#pragma pack()
#endif

//============================================================================//
// User Externs
//============================================================================//
#if ENABLE_MG82
extern uint32_t u32MG82_ReportErrLastTick;
extern bool bMG82_ReportErrFlag;
extern bool bMG82_Stop;
extern bool bMG82_RTC_EN;
extern MG82_WkupSrcTypeDef u8WkupSrc;
#endif

//============================================================================//
// User Function Prototypes
//============================================================================//
#if ENABLE_MG82
void MG82_ScanBus(void);
void MG82_Wakeup(void);
bool MG82_Init(void);
void MG82_ReInit(void);
void MG82_DeInit(void);
void MG82_CheckExAlmSetting(void);
bool MG82_IsReady(void);
void MG82_Restart(void);
bool MG82_ReadReg(uint8_t u8Addr, uint8_t *pu8Dat);
bool MG82_ReadRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen);
bool MG82_WriteReg(uint8_t u8Addr, uint8_t u8Dat);
bool MG82_WriteRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen);

// Master write commands
bool MG82_UART_Send(uint8_t u8Port, uint8_t *pu8Dat, uint8_t u8DatLen);
bool MG82_UART_SetBR(uint8_t u8Port, UART_BaudRateDef_t tBR);
bool MG82_UART_Reset(uint8_t u8Port);
bool MG82_UART_ResetAll(void);
bool MG82_IO_Set(uint8_t u8IO, bool bVal);
bool MG82_RTC_SetDateTime(RTC_DateTypeDef *ptDate, RTC_TimeTypeDef *ptTime,
						  bool bIsBin);
bool MG82_WDG_Config(bool bEnable, uint16_t u16TimeOut);
bool MG82_ADC_Config(bool bEnable, uint16_t u16TimeOut);
bool MG82_SW_Reset(void);
bool MG82_PWR_Mode(MG82_PwrTypeDef tPwrMode);
bool MG82_KetRstEn(bool bOption);
bool MG82_4G_DTR(bool bVal);
bool MG82_Shutdown(MG82_PwrTypeDef tPwrMode, uint16_t u16Timeout);
bool MG82_RTC_Alarm(time_t ttCurrTime, ExternalAlarmTypeDef tAlarm);
bool MG82_RTC_PedWkup(ExternalPedWkupTypeDef tSetting);

bool MG82_USBPROTECTION_Set(bool bVal);
#if HW_TYPE == HW_JAS306SD
bool MG82_RS485_Config(bool bEnable);
bool MG82_OW_Cmd(OW_DataTypeDef *ptOW_Data);
#endif

// Master read commands
bool MG82_UART_GetLen(uint16_t *pu16RX_Len);
bool MG82_UART_Read(uint8_t u8Port, uint8_t *pu8Buf, uint8_t u8Len);
bool MG82_UART_GetBR_All(UART_BaudRateDef_t *ptBR);
bool MG82_UART_GetBR(uint8_t u8Port, UART_BaudRateDef_t *ptBR);
bool MG82_RTC_GetDateTime(RTC_DateTypeDef *ptDate, RTC_TimeTypeDef *ptTime,
						  bool bIsBin);
bool MG82_IO_Get(bool *pbIO_Val);
bool MG82_WDG_GetCfg(uint8_t *pu8En, uint8_t *pu8TimeOut);
bool MG82_ADC_GetCfg(uint8_t *pu8En);
bool MG82_ADC_Read(uint8_t *pu16Val);
bool MG82_GetVer(char *pcVer, uint8_t u8LenMax, uint8_t *pu8Len);
#if HW_TYPE == HW_JAS306SD
bool MG82_RS485_GetConfig(bool *pbEnable);
bool MG82_OW_CheckStatus(uint8_t *pu8Cmd, uint8_t *pu8Cplt,
						 uint8_t *pu8Res, uint8_t *pu8RetLen) bool MG82_OW_Read(uint8_t *pu8DatBuf, uint8_t u8ReadLen, uint8_t *pu8ActualLen) bool MG82_OW_Check_Read(uint8_t *pu8Cmd, uint8_t *pu8Cplt, uint8_t *pu8Res,
																																									  uint8_t *pu8DatBuf, uint8_t u8ReadLen, uint8_t *pu8ActualLen);
#endif
bool MG82_WKUP_Src(MG82_WkupSrcTypeDef *ptSrc);
bool MG82_KeyRstGet(bool *pbOption);
bool MG82_RTC_GetWkup(ExternalAlarmTypeDef *ptAlarm,
					  ExternalPedWkupTypeDef *ptSetting);
#endif

//============================================================================//
#endif //_MG82_DRIVER_H_
