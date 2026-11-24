//============================================================================//
// User Includes
//============================================================================//

#include "Power.h"

#include "stm32fxxx_hal_user.h"
#include "DataLog.h"
#include "Default.h"
#include "GPS_Driver.h"
#include "main.h"
#include "MG82_Driver.h"

//============================================================================//
// User Function Prototypes
//============================================================================//

//============================================================================//
// @brief  Check if the program is set to enter standby mode.
// @param  None.
// @retval None.
//============================================================================//
// static void Power_ChkStandby(void)
//{
//	if(HAL_RTCEx_BKUPRead(&hrtc, BKP_PWR_CHK) != BKP_JUMP_VALUE)
//		return;

//	HAL_RTCEx_BKUPWrite(&hrtc, BKP_PWR_CHK, BKP_RST_VALUE);

//	System_EnterStandbyMode();
//}

//============================================================================//
// @brief  Initialize power functions.
// @param  None.
// @retval None.
//============================================================================//
void Power_Init(void)
{
    PWR_PVDTypeDef pwrPVD_Handle;

    // Power_ChkStandby();

    // HW_PBClr(PWR_SAVE_EN);

    // Enable PWR and BKP clocks
    __HAL_RCC_PWR_CLK_ENABLE();

    // Allow access to BKP Domain
    // HAL_PWR_EnableBkUpAccess();

#if HW_TYPE == HW_JAS201
    // Configure the PVD Level
    pwrPVD_Handle.PVDLevel = PWR_PVDLEVEL_5; // Level 2.7V

    pwrPVD_Handle.Mode = PWR_PVD_MODE_NORMAL;

    HAL_PWR_ConfigPVD(&pwrPVD_Handle);

    // Enable the PVD Output
    HAL_PWR_EnablePVD();
#endif

#if HW_TYPE == HW_JAS211 || HW_TYPE == HW_JAS106 || \
    HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
    // Configure the PVD Level
    pwrPVD_Handle.PVDLevel = PWR_PVDLEVEL_6; // Level 2.8V

    pwrPVD_Handle.Mode = PWR_PVD_MODE_NORMAL;

    HAL_PWR_ConfigPVD(&pwrPVD_Handle);

    // Enable the PVD Output
    HAL_PWR_EnablePVD();
#endif
}

//============================================================================//
// @brief  Enable main input power source.
// @param  None.
// @retval None.
//============================================================================//
void Power_SourceEnable(void)
{
#if ENABLE_MG82
    MG82_IO_Set(0, 1);
#else
    HW_PBSet(MOTOR_PWR_EN);
#endif
}

//============================================================================//
// @brief  Disable main input power source.
// @param  None.
// @retval None.
//============================================================================//
void Power_SourceDisable(void)
{
#if ENABLE_MG82
    MG82_IO_Set(0, 0);
#else
    HW_PBClr(MOTOR_PWR_EN);
#endif
}

/*
//============================================================================//
// @brief  Prepare to enter standby mode.
// @param  None.
// @retval None.
//============================================================================//
static void Power_Shutdown(void)
{
    HAL_RTCEx_BKUPWrite(&hrtc, BKP_PWR_CHK, BKP_JUMP_VALUE);

    RTOSprintf("\r\n> Power shutdown. Reset system.")

  #if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
    #endif

  System_SaveSettingFinal();
    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

  if(bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();
}

//============================================================================//
// @brief  Enter standby mode if ACC is off.
// @param  None.
// @retval TRUE: The operation is successfully.
//         FALSE: The operation is failed.
//============================================================================//
static bool Power_Standby_Proc(void)
{
    if(g_SysInfo.VehicleState.ACC_CHK == TRUE)
        return FALSE;

    //Datalog_BuildAll();	// Heinz

    RTOSprintf("\r\n> System power enter standby mode.");

    Power_Shutdown();

    return TRUE;
}

//============================================================================//
// @brief  Enter standby mode if PWR is off.
// @param  None.
// @retval TRUE: The operation is successfully.
//         FALSE: The operation is failed.
//============================================================================//
static bool Power_Down(void)
{
    if(g_SysInfo.VehicleState.PWR_CHK == TRUE)
        return FALSE;

  //Datalog_BuildAll();	// Heinz

    RTOSprintf("\r\n> System power down.\r\n");

    Power_Shutdown();

    return TRUE;
}

//============================================================================//
// @brief  Check power status.
// @param  None.
// @retval TRUE: Power is on.
//         FALSE: Power is off.
//============================================================================//
static bool Power_Check(void)
{
    RTC_AlarmTypeDef rtcAlarmHandle;
    struct tm tmLocalTimeInfo = {0};
    RTC_TimeTypeDef rtcTime;
    time_t ttTime;


    if(g_SysInfo.VehicleState.PWR_ON == TRUE)
        return FALSE;

    if(__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO) == SET)
    {
        ttTime = INVALID_VALUE_DEF;

        localtime_r(&ttTime, &tmLocalTimeInfo);

        rtcTime.Hours = tmLocalTimeInfo.tm_hour;

        rtcTime.Minutes = tmLocalTimeInfo.tm_min;

        rtcTime.Seconds = tmLocalTimeInfo.tm_sec;

        rtcAlarmHandle.AlarmTime = rtcTime;

        rtcAlarmHandle.AlarmMask = RTC_ALARMMASK_NONE;

        rtcAlarmHandle.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;

        rtcAlarmHandle.AlarmDateWeekDay = ptmLocalTimeInfo->tm_mday;

        rtcAlarmHandle.Alarm = RTC_ALARM_A;

        HAL_RTC_SetAlarm(&hrtc, &rtcAlarmHandle, RTC_FORMAT_BIN);

        // Wait until last write operation on RTC registers has finished
        //RTC_WaitForLastTask();

        RTOSprintf("\r\n> System power shutdown.\r\n");

        Power_Shutdown();
    }

    return TRUE;
}
*/
