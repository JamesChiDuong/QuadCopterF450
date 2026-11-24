//============================================================================//
// User Includes
//============================================================================//

#include "stm32fxxx_hal_user.h"
#include "PCF_Driver.h"
#include "CommFunc.h"
#include "HW_Config.h"
#include "System.h"


//============================================================================//
// User Functions
//============================================================================//

#if PCF_TYPE == PCF8563
//============================================================================//
// @brief  Make the PCF IC ready to communicate again.
// @param  None.
// @retval None.
//============================================================================//
void PCF_ReInit(void)
{ 
	HAL_I2C_DeInit(&hi2c2);
	
  HwCfg_PBClr(I2C2_SCL_GPIO);	
	HwCfg_PBInit(I2C2_SCL_GPIO, FALSE);
	HwCfg_PBInit(I2C2_SDA_GPIO, FALSE);
	
	for(int i = 0; i < 9; i++)
	{
	  HwCfg_PBSet(I2C2_SCL_GPIO);
		Delay_uS(10);
		HwCfg_PBClr(I2C2_SCL_GPIO);
		Delay_uS(10);
	}
	
	HwCfg_PBDeInit(I2C2_SCL_GPIO, FALSE);
	HwCfg_PBDeInit(I2C2_SDA_GPIO, FALSE);
	
	USER_I2C2_Init();
}

//============================================================================//
// @brief  Deinit the PCF communication interface.
// @param  None.
// @retval None.
//============================================================================//
void PCF_DeInit(void)
{  
	HAL_I2C_DeInit(&hi2c2);
}

//============================================================================//
// @brief  Check if the PCF IC is ready or not.
// @param  None.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_IsReady(void)
{
	HAL_StatusTypeDef halRes;
	
	
	//PCF_ReInit();
	
  halRes = HAL_I2C_IsDeviceReady(&hi2c2, PCF_ADDR_I2C, 
		                             PCF_MAX_TRIALS, PCF_MAX_TIMEOUT);
	
	if(halRes == HAL_OK)
	  return TRUE;
	else	
		return FALSE;
}

//============================================================================//
// @brief  Start or stop PCF source clock.
// @param  bRun: Clock start/stop: 1: Start
//                                 0: Stop.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_ClockCtrl(bool bRun)
{
	uint8_t u8RegAddr = PCF8563_REG_CTL1;
	uint8_t u8RegVal = 0;
	uint8_t u8RegBuf[2]; 
	
	
	// Send control register 1 address
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, &u8RegAddr, 
		                         1, PCF_MAX_TIMEOUT ) != HAL_OK)
	  return FALSE;
	
	//PCF_ReInit();
	
	// Read control register 1 value
	if(HAL_I2C_Master_Receive(&hi2c2, PCF_ADDR_I2C, &u8RegVal, 
		                        1, PCF_MAX_TIMEOUT) != HAL_OK)
	  return FALSE;
	
	u8RegBuf[0] = u8RegAddr;
	
	if(bRun)
		// Reset source clock control bit
	  u8RegBuf[1] = u8RegVal & PCF8563_CLOCK_START_MASK;
	else 
		// Set source clock control bit
		u8RegBuf[1] = u8RegVal | PCF8563_CLOCK_STOP_MASK;
	
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, u8RegBuf, 
		                         sizeof(u8RegBuf)/sizeof(u8RegBuf[0]), PCF_MAX_TIMEOUT ) == HAL_OK)
		return TRUE;
	else
	  return FALSE;
}

//============================================================================//
// @brief  Set time parameters for the PCF IC.
// @param  rtcInit_Time: Pointer to RTC_TimeTypeDef struct, using BCD format.
// @param  bIsBin: Is the input time struct in the Binary format or not.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_SetTime(RTC_TimeTypeDef *prtcInit_Time, bool bIsBin)
{ 
	uint8_t u8TimeBuf[4];
	
	
	// Make sure the pointer is not null
  if(!prtcInit_Time) 
		return FALSE;
	
	//PCF_ReInit();
		
	u8TimeBuf[0] = PCF8563_REG_TIME_START_ADDR; // 0x02
	u8TimeBuf[1] = prtcInit_Time->Seconds; // 0x02 Seconds
	u8TimeBuf[2] = prtcInit_Time->Minutes; // 0x03 Minute		
	u8TimeBuf[3] = prtcInit_Time->Hours; // 0x04 Hours
	
	// If the input time is in the Binary format
	if(bIsBin)
	  for(int i = 1; i < 4; i++)
		  u8TimeBuf[i] = Bin_2_BCD(u8TimeBuf[i]);
	
	// Reset bit 7 of the VL_seconds register
	u8TimeBuf[1] &= PCF8563_SEC_MASK;
	
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, u8TimeBuf, 
		                         sizeof(u8TimeBuf)/sizeof(u8TimeBuf[0]), PCF_MAX_TIMEOUT ) == HAL_OK)
		return TRUE;
	else
	  return FALSE;
}	

//============================================================================//
// @brief  Set date parameters for the PCF IC.
// @param  rtcInit_Date: Pointer to RTC_DateTypeDef struct, using BCD format.
// @param  bIsBin: Is the input date struct in the Binary format or not.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_SetDate(RTC_DateTypeDef *prtcInit_Date, bool bIsBin)
{ 
	uint8_t u8DateBuf[5];
	
	
	// Make sure the pointer is not null
  if(!prtcInit_Date) 
		return FALSE;
	
	//PCF_ReInit();
		
	u8DateBuf[0] = PCF8563_REG_DATE_START_ADDR; // 0x05
	u8DateBuf[1] = prtcInit_Date->Date; // 0x05 Day
	u8DateBuf[2] = prtcInit_Date->WeekDay; // 0x06 Week Day		
	u8DateBuf[3] = prtcInit_Date->Month; // 0x07 Month
	u8DateBuf[4] = prtcInit_Date->Year; // 0x08 Year
	
	// If the input date is in the Binary format
	if(bIsBin)
		for(int i = 1; i < 5; i++)
		  u8DateBuf[i] = Bin_2_BCD(u8DateBuf[i]);
	
	// Reset bit 7 in the Century_months register
	u8DateBuf[3] &= PCF8563_MON_MASK;
	
	// Send 3 bytes of time to PCF corresponding register
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, u8DateBuf, 
		                         sizeof(u8DateBuf)/sizeof(u8DateBuf[0]), PCF_MAX_TIMEOUT) == HAL_OK)
		return TRUE;
	else
	  return FALSE;
}	

//============================================================================//
// @brief  Get time values from the PCF IC.
// @param  rtcRet_Time: Pointer to RTC_TimeTypeDef struct.
// @param  bIsBin: Is the returned time struct required to be 
//                 in the Binary format or not.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_GetTime(RTC_TimeTypeDef *prtcRet_Time, bool bIsBin)
{
	uint8_t start_addr = PCF8563_REG_TIME_START_ADDR;
	uint8_t u8TimeBuf[3] = {0};
	
	
	// Make sure the pointer is not null
	if(!prtcRet_Time)
  	return FALSE;
	
	//PCF_ReInit();
	
	// Send start address
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, &start_addr, 
		                         1, PCF_MAX_TIMEOUT ) != HAL_OK)
	  return FALSE;
	
	// Read 3 bytes of seconds, minutes, hours
	if(HAL_I2C_Master_Receive(&hi2c2, (uint16_t)PCF_ADDR_I2C, u8TimeBuf, 
		                        3, PCF_MAX_TIMEOUT) != HAL_OK)
	  return FALSE;
														
	u8TimeBuf[0] &= PCF8563_SEC_MASK; // Seconds
	u8TimeBuf[1] &= PCF8563_MIN_MASK;	// Minutes
  u8TimeBuf[2] &= PCF8563_HOUR_MASK; // Hours	

	// If the returned time is required in the Binary format
  if(bIsBin)
    for(int i = 0; i < 3; i++)
      u8TimeBuf[i] = BCD_2_Bin(u8TimeBuf[i]);
	
	prtcRet_Time->Seconds = u8TimeBuf[0]; // Seconds
	prtcRet_Time->Minutes = u8TimeBuf[1]; // Minutes
  prtcRet_Time->Hours = u8TimeBuf[2]; // Hours

  return TRUE;														
}

//============================================================================//
// @brief  Set date values from the PCF IC.
// @param  rtcRet_Date: Pointer to RTC_DateTypeDef struct.
// @param  bIsBin: Is the returned date struct required to be 
//                 in the Binary format or not.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_GetDate(RTC_DateTypeDef *prtcRet_Date, bool bIsBin)
{
	uint8_t start_addr = PCF8563_REG_DATE_START_ADDR;
	uint8_t u8DateBuf[4] = {0};
	
	
	// Make sure the pointer is not null
	if(!prtcRet_Date)
		return FALSE;
	
	//PCF_ReInit();
	
	// Send start address
	if(HAL_I2C_Master_Transmit(&hi2c2, PCF_ADDR_I2C, &start_addr, 
		                         1, PCF_MAX_TIMEOUT ) != HAL_OK)
	  return FALSE;
	
	// Read 4 bytes of day, week day, month, year
	if(HAL_I2C_Master_Receive(&hi2c2, PCF_ADDR_I2C, u8DateBuf, 
		                        4, PCF_MAX_TIMEOUT) != HAL_OK)
	  return FALSE;
	
	u8DateBuf[0] &= PCF8563_DAY_MASK; // Day
	u8DateBuf[1] &= PCF8563_WDAY_MASK; // Week Day
	u8DateBuf[2] &= PCF8563_MON_MASK; // Month
	u8DateBuf[3] &= PCF8563_YEAR_MASK; // Year
	
	// If the returned date is required in the Binary format
	if(bIsBin)
    for(int i = 0; i < 4; i++)
      u8DateBuf[i] = BCD_2_Bin(u8DateBuf[i]);
														
	prtcRet_Date->Date = u8DateBuf[0]; // Day
	prtcRet_Date->WeekDay = u8DateBuf[1]; // Week Day
  prtcRet_Date->Month = u8DateBuf[2]; // Month
  prtcRet_Date->Year = u8DateBuf[3]; // Year

  return TRUE;														
}

//============================================================================//
// @brief  Save date time values from PCF IC to a backup register.
// @param  rtcRet_Date: Pointer to RTC_DateTypeDef struct.
// @retval Result: TRUE/ FALSE.
//============================================================================//
bool PCF_WriteDateTime_BKP(void)
{
  RTC_TimeTypeDef rtcTime = {0};
	RTC_DateTypeDef rtcDate = {0};
	struct tm tmPCFTimeInfo, *ptmLocalTimeInfo;
	time_t ttTimeNow;
	DWORD dwTemp, dwTimeBKP = 0, dwDateBKP = 0; 
	 
	
	// Get date time
	if(!PCF_GetTime(&rtcTime, 1) || !PCF_GetDate(&rtcDate, 1))
		return FALSE;
	
	// Year in tm struct is counted from 1900
	tmPCFTimeInfo.tm_year	= rtcDate.Year + 2000 - 1900U;
	// Month in tm struct is counted from 0 corresponding to Jan
	tmPCFTimeInfo.tm_mon	= rtcDate.Month - 1U;
	tmPCFTimeInfo.tm_mday	= rtcDate.Date;
	tmPCFTimeInfo.tm_hour	= rtcTime.Hours;
	tmPCFTimeInfo.tm_min	= rtcTime.Minutes;
	tmPCFTimeInfo.tm_sec	= rtcTime.Seconds;
	tmPCFTimeInfo.tm_isdst = -1;
	
	// Convert time from calendar format to time to second
	ttTimeNow = mktime(&tmPCFTimeInfo);
	
	// Add timezone offset to time in second 
	ttTimeNow += g_Setting.i32TimeZoneOffset;
	
	// Convert local time in second to local time in calendar format
	ptmLocalTimeInfo = localtime(&ttTimeNow);
	
	// Copy date values to dwDateBKP
	// bit15:9, Year from 1980 (0..127)
	dwTemp = ptmLocalTimeInfo->tm_year + 1900U - 1980U;
	dwDateBKP |= dwTemp << 9;
	
	// bit8:5, Month (1..12)
	dwTemp = ptmLocalTimeInfo->tm_mon + 1U;
	dwDateBKP |= dwTemp << 5;
	
	// bit4:0, Day in month(1..31)
	dwTemp = ptmLocalTimeInfo->tm_mday;
	dwDateBKP |= dwTemp;
	
	// Copy time values to dwTimeBKP
	// bit15:11, Hour (0..23)
	dwTemp = ptmLocalTimeInfo->tm_hour;
	dwTimeBKP |= dwTemp << 11;
	
	// bit10:5, Minute (0..59)
	dwTemp = ptmLocalTimeInfo->tm_min;
	dwTimeBKP |= dwTemp << 5;
	
	// bit4:0, Second / 2 (0..29)
	dwTemp = ptmLocalTimeInfo->tm_sec / 2;
	dwTimeBKP |= dwTemp;
	
	// Enable access to backup domain
	//HAL_PWR_EnableBkUpAccess();
    
	// Save date values to BKP_DR3
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, dwDateBKP);
	
	// Save time values to BKP_DR4
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, dwTimeBKP);
  
  // Disable access to backup domain  
	//HAL_PWR_DisableBkUpAccess();
	
	return TRUE;
}

//============================================================================//
// @brief  Read date time from backup register.
// @param  None.
// @retval Result: Date time in DWORD format.
//============================================================================//
DWORD PCF_ReadDateTime_BKP(void)
{
	DWORD dwDateTime = 0, dwTimeBKP, dwDateBKP; 
	
	
	// Read date values from BKP_DR3
  dwDateBKP = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3) ;
	
	// Read time values from BKP_DR3
	dwTimeBKP = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
	
	// Date values is stored in the upper 16 bits
	dwDateTime |= dwDateBKP << 16;
	
	// Time values is stored in the lower 16 bits
	dwDateTime |= dwTimeBKP;
	
	return dwDateTime;
}

//============================================================================//
// @brief  Reset date time values in backup register.
// @param  None.
// @retval None.
//============================================================================//
void PCF_ResetDateTime_BKP(void)
{
  // Enable access to backup domain
	//HAL_PWR_EnableBkUpAccess();
    
	// Reset BKP_DR3
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, 0);
	
	// Reset BKP_DR4
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, 0);
  
  // Disable access to backup domain  
	//HAL_PWR_DisableBkUpAccess();
}
#endif
