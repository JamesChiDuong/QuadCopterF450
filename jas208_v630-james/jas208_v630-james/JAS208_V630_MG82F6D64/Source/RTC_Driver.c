//============================================================================//
// Include
//============================================================================//
#include "RTC_Driver.h"

#include <Intrins.h>

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "IAP_Driver.h"
#include "IO_Driver.h"
#include "REG_MG82F6D64.H"
#include "System.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
bit bRTC_SetTimeFlag = 0;
xdata DATE_TIME DateTime, DateTimeTemp;

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  RTC init.
// @param  None
// @retval None.
//============================================================================//
void RTC_Init(void)
{	
	RTC_Disable();
	/* use external 32.768 kHz
	x=PageP_Read(CKCON2_P);
	x=(x&0x3F)|(ENABLE_XTAL|GAIN_FOR_32K);
	PageP_Write(CKCON2_P,x);
	while(CKCON1 & XTOR == 0x00);		// wait for XTAL stable
	RTC_SetClock_XTAL2_ECKI_P60();		// RTC Clock source: XTAL 32.768KHz
	*/
	//11059200/32768=337.5
	#if EN_RTC_SRC_32000
	RTC_SetClock_ILRCO();		// RTC Clock source: ILRCO 32KHz
	#else
	RTC_SetClock_XTAL2_ECKI_P60(); // RTC Clock source: External input to pin P6.0
	#endif
	RTC_SetClock_Div_32768();	// RTC Clock prescale: 32768
	RTC_SetReload(64-1);		// RTC Reload Value: (64-1)=63
	RTC_SetCounter(64-1);
	RTC_EnRTCO_OutP45();//		// enable RTCKO output 
	RTC_ClearRTCF();			// clear RTCF
	RTC_Enable();				// Enable RTC
	INT_SetIPXHigh(IPX_SF);
	INT_EnSF_RTC();			// Enable RTC interrupt, must enable SF interrupt
	INT_EnSF();				// Enable SF interrupt
}

#if !EN_RTC_WKUP
//============================================================================//
// @brief  RTC deinit.
// @param  None
// @retval None.
//============================================================================//
void RTC_DeInit(void)
{	
	INT_DisSF_RTC();
	RTC_Disable();
}
#endif

//============================================================================//
// @brief  Init system date time.
// @param  None
// @retval None.
//============================================================================//
void InitDateTime(void)
{
	DateTime.u8Year = 23;
	DateTime.u8Mon = 05;
	DateTime.u8Day = 01;
	DateTime.u8Hour = 00;
	DateTime.u8Min = 00;
	DateTime.u8Sec = 00;
}

//============================================================================//
// @brief  Increase date time by 1 second.
// @param  None
// @retval None.
//============================================================================//
void IncDateTime(void)
{
	code u8 u8MonDay[13] = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	xdata u8 u8DayPerMon = 30;
	xdata u8 u8LeapYear = 0;
	
	DateTime.u8Sec = (DateTime.u8Sec + 1) % 60;		
	if (DateTime.u8Sec != 0) return;
	DateTime.u8Min = (DateTime.u8Min + 1) % 60;		
	if (DateTime.u8Min != 0) return;
	DateTime.u8Hour = (DateTime.u8Hour + 1) % 24;		
	if (DateTime.u8Hour != 0) return;
	u8LeapYear = (DateTime.u8Year % 4 == 0) ? 1 : 0;
	u8DayPerMon = u8MonDay[DateTime.u8Mon];
	if ((DateTime.u8Mon == 2) && (u8LeapYear == 1))
		u8DayPerMon++;
	DateTime.u8Day = (DateTime.u8Day + 1) % u8DayPerMon + 1;		
	if (DateTime.u8Day != 1) return;
	DateTime.u8Mon = (DateTime.u8Mon + 1) % 12 + 1;		
	if (DateTime.u8Mon != 1) return;
	DateTime.u8Year++;
}

#if EN_RTC_WKUP
//============================================================================//
// @brief  Increase date time by 1 second.
// @param  None
// @retval None.
//============================================================================//
void RTC_Wkup_Handle(void)
{
	if(NV_Set.tAlarm.u8Enable > 0)
	{
		// u8Year == 0 -> Don't care when compare
		// u8Mon == 0 -> Don't care when compare
		// u8Day == 0 -> Don't care when compare
		// u8Hour > 23 -> Don't care when compare
		// u8Min > 59 -> Don't care when compare
		// u8Sec > 59 -> Don't care when compare
		if(((DateTime.u8Year == NV_Set.tAlarm.tAlarmTime.u8Year) || (NV_Set.tAlarm.tAlarmTime.u8Year == 0)) &&
			 ((DateTime.u8Mon == NV_Set.tAlarm.tAlarmTime.u8Mon) || (NV_Set.tAlarm.tAlarmTime.u8Mon == 0)) &&
		   ((DateTime.u8Day == NV_Set.tAlarm.tAlarmTime.u8Day) || (NV_Set.tAlarm.tAlarmTime.u8Day == 0)) &&
			 ((DateTime.u8Hour == NV_Set.tAlarm.tAlarmTime.u8Hour) || (NV_Set.tAlarm.tAlarmTime.u8Hour > 23)) &&
			 ((DateTime.u8Min == NV_Set.tAlarm.tAlarmTime.u8Min) || (NV_Set.tAlarm.tAlarmTime.u8Min > 59)) &&
			 ((DateTime.u8Sec == NV_Set.tAlarm.tAlarmTime.u8Sec) || (NV_Set.tAlarm.tAlarmTime.u8Sec > 59)))
		{
			if((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
				 (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
			{
				//SendWkupPulse(MCU_WKUP_PIN, 1);
				SysParam.u8WakeupSource = WKUP_SRC_ALM;
				bExitSlowMode = TRUE;
			}
			
			if(NV_Set.tAlarm.u8Repeat == 0)
			{
				NV_Set.tAlarm.u8Enable = 0;
				
				SaveNV();
			}
		}
	}
	else if(NV_Set.tPedWkup.u8Enable > 0)
	{
		if(NV_Set.tPedWkup.u32Counter >= NV_Set.tPedWkup.u32Timeout)
		{
			NV_Set.tPedWkup.u32Counter = 0;
			//if(N_ACC_IN == 1)
			if((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
				 (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
			{
				//SendWkupPulse(MCU_WKUP_PIN, 1);
				SysParam.u8WakeupSource = WKUP_SRC_TIMER;
				bExitSlowMode = TRUE;
			}
		}
	}
}
#endif
