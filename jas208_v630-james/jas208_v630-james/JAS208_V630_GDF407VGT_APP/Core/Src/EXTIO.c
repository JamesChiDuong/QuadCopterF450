//============================================================================//
// User Includes
//============================================================================//

#include "EXTIO.h"

#include <cstdlib>
#include <math.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "Buzzer.h"
#include "CAN_Driver.h"
#include "DataCollect.h"
#include "DataLog.h"
#include "Default.h"
#include "LCD_Key.h"
#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#endif
#include "main.h"
#include "MC321X_Driver.h"
#if ENABLE_MG82
#include "MG82_Driver.h"
#endif
#include "MX25L_Driver.h"
#include "PCF_Driver.h"
#include "Power.h"
#include "RuleV161.h"
#include "System.h"

#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#endif
//============================================================================//
// User Defines
//============================================================================//
#if ENABLE_POWER_CHECK_VOLT
//(VIN * (75K / (750K + 75K))) / (3.3 / 2^12)
// #define POWER_VOLT_CONV(VOLT)	  (VOLT * 40960 / 363)
#define POWER_VOLT_CONV(VOLT) (VOLT * 4096 / 363)

#define MOTOR_POWER_VOLT_DEF POWER_VOLT_CONV(8)
#endif

#define SET_DATASTREAM_DELAY MINUTE_2_SECOND(5)

#if CUSTOMER_TYPE == CUSTOMER_FORMOSA || CUSTOMER_TYPE == CUSTOMER_FPTC
#undef SET_DATASTREAM_DELAY
#define SET_DATASTREAM_DELAY HOUR_2_SECOND(1) // Heinz 20160512
// #define SET_DATASTREAM_DELAY	 MINUTE_2_SECOND(5) // Heinz 20161123
#endif

#define CHECK_EXTIO_DETECT (0x0304)

// Auto Gain
#define AUTO_GAIN_MAX_COUNT 3
#define AUTO_GAIN_OK_COUNT 128
#define AUTO_GAIN_MAX_DIST 3000

// Event Real Speed Lose
#define REALSPEED_LOSE_COUNT MINUTE_2_SECOND(30)
#define SPEED_ABNORMAL_COUNT 120

#define LCD_BACKLIGHT_COUNT 60
//============================================================================//
// User Variables
//============================================================================//
PwrDownStepTypeDef tPwrDownStep = PWR_DOWN_STEP_NONE;

bool bLastACC_State = FALSE;

uint32_t u32PwrDownDelayTick = 0;

EXTIO_StateTypeDef CurrState;

bool bBackupPwrStable = TRUE;

uint16_t u16PoweADCValue = 0;

// uint16_t u16PoweADCValue2 = 0;

bool bPowerCutFlag = FALSE;

// bool bMessageFlag = FALSE;

uint16_t uiSPD_TickIdx = 0;

uint16_t uiRPM_TickIdx = 0;

uint32_t u32RealSpeedTemp = 0;

bool bEVENT_SPEED_LINE = FALSE; // Heinz

bool bWaitingEnterStandby = FALSE;

float fAccelSpeedSlot[6] = {0.0f};

uint16_t Again_AutoGain_Count = 0;

// bool bWaitStartProcFlag = FALSE;

bool bIsSaveSettingFlag = FALSE;

bool bIsCheckDistanceFlag = FALSE;

time_t ttBacklightCnt = LCD_BACKLIGHT_COUNT;

bool bIsBacklightFlag = FALSE;

bool bRS_FirstDetection = TRUE;

uint32_t u32ErrorPageTick = 0;

uint32_t u32ErrorPageTick2 = 0;

uint32_t u32ErrorPageTick3 = 0;

bool bIsShowAGMsgFlag = TRUE;

time_t ttAccOnTime = 0;

time_t ttAG_DelayBegin = 0;

uint8_t u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

uint32_t u32DistCntAG_Old = 0;
double dDistanceCAN_Old = 0.0;

uint8_t u8DistBaseCAN_Init = 0;

uint32_t u32FinalPulseCnt = 0, u32FinalDist = 0;

bool bChkGAIN_Step1 = TRUE;
bool bChkGAIN_Step2 = FALSE;

bool bAGainNgFlag = FALSE;
float fNgGainValue = 0;

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Return ADC value of a channel in ADC 1
// @param  u8Channel: ADC channel number.
// @retval ADC value in uint16_t format.
//============================================================================//
uint16_t EXTIO_GetADC1(uint8_t u8Channel)
{
    HAL_StatusTypeDef halRes;
    uint16_t u16ADC_Res;
    ADC_ChannelConfTypeDef sConfig = {0};

    switch (u8Channel)
    {
    case 9:
        sConfig.Channel = ADC_CHANNEL_9;

        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

        break;

    case 14:
        sConfig.Channel = ADC_CHANNEL_14;

        sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

        break;

    case 16:
        sConfig.Channel = ADC_CHANNEL_16;

        sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;

        break;

    default:
        return 0;
    }

    sConfig.Rank = 1;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        return 0;

    HAL_ADC_Start(&hadc1);

    halRes = HAL_ADC_PollForConversion(&hadc1, 1000);

    if (halRes == HAL_OK)
    {
        u16ADC_Res = HAL_ADC_GetValue(&hadc1);
    }
    else
    {
        RTOSprintf("\r\n> ADC conversion ERROR %d", halRes);

        u16ADC_Res = 3300;
    }

    HAL_ADC_Stop(&hadc1);

    return u16ADC_Res;
}

//============================================================================//
// @brief  Get the ambient temperature of the MCU.
// @param  None
// @retval Temperature based on C degree unit in UINT format.
//============================================================================//
UINT EXTIO_GetTemp(void)
{
    uint16_t u16ADC_Res;
    UINT uTemp;

    u16ADC_Res = EXTIO_GetADC1(16);

    uTemp = (unsigned int)(((float)u16ADC_Res * TEMP_V_SENSE - TEMP_V_25) / TEMP_SLOPE_AVG + 25);

    return uTemp;
}

//============================================================================//
// @brief  Initialize EXTIO function.
// @param  None
// @retval None.
//============================================================================//
void EXTIO_Init(void)
{
    CurrState.word = 0x00;

    ttAG_DelayBegin = HAL_GetTick();

#if ENABLE_GS_MC3210
    MC321X_Init();
#endif

#if ENABLE_CAN == 1
    // if(CAN_CheckEnable())
    {
        CAN_Config();
    }
#endif

#if ENABLE_LSM6DSL
    LSM6DSL_Init();
#endif

    // bCAN_GetDist = TRUE;
}

/*
//============================================================================//
// @brief  Perform necessary steps before sending the MCU to power down mode
//         or shutdown.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_PwrDown(void)
{
    if(g_Setting.tPwrDown.tMode == PWR_DOWN_SHUTDOWN)
    {
        #if ENABLE_ADJUST_OFF_TIME
        if(g_SysInfo.tVehicleState.bPWR_ON == 1)
        {
            #if ENABLE_MG82
            MG82_Shutdown(MG82_PWR_MODE_DEF, g_Setting.tPwrDown.u16Delay);
            #endif

            osTimerStart(timerACC_OffHandle, S_2_mS(60));
        }
        else
        {
            #if ENABLE_MG82
            MG82_Shutdown(MG82_PWR_MODE_DEF, 10);
            #endif

            osTimerStart(timerACC_OffHandle, S_2_mS(20));
        }
        #else
        #if ENABLE_MG82
        MG82_Shutdown(MG82_PWR_MODE_DEF, g_Setting.tPwrDown.u16Delay);
        #endif

        osTimerStart(timerACC_OffHandle, S_2_mS(20));
        #endif
    }
    else
    {
        #if ENABLE_MG82
        #if ENABLE_EX_WKUP
        if(g_Setting.tExPedWkup.bEnableWkup == TRUE)
        {
            #if RTC_WKUP_SRC == RTC_WKUP_SRC_MCU
            if(g_Setting.tExPedWkup.u32Timeout >= 10)
            {
                System_SetWakeupTimer(g_Setting.tExPedWkup.u32Timeout);
            }
            #elif RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
            MG82_RTC_PedWkup(g_Setting.tExPedWkup);
            #endif
        }
        #endif

        MG82_PWR_Mode(MG82_PWR_MODE_DEF);
        #endif

        #if HW_TYPE == HW_JAS208
        #if ENABLE_GSM_NET
        bGSM_SetSleep = TRUE;
        #endif
        #endif

        #if ENABLE_ADJUST_OFF_TIME
        if(g_SysInfo.tVehicleState.bPWR_ON == 1)
        {
            osTimerStart(timerACC_OffHandle, S_2_mS(60));
        }
        else
        {
            osTimerStart(timerACC_OffHandle, S_2_mS(10));
        }
        #else
        osTimerStart(timerACC_OffHandle, S_2_mS(20));
        #endif
    }
}
*/

//============================================================================//
// @brief  Handle power down process.
// @param  None
// @retval None.
//============================================================================//
static void EXTIO_PwrDownHandle(void)
{
    struct tm tmTime = {0};
    char szTime[40];
    static uint32_t u32DelayTimeout = 0;
    static uint32_t u32DelayTimeout_4G = 0;
    static uint32_t u32DelayTimeout_GS = 0;
    if ((bWaitingEnterStandby == FALSE) ||
        (tPwrDownStep == PWR_DOWN_STEP_NONE))
        return;

    switch ((int)tPwrDownStep)
    {
    case PWR_DOWN_STEP_1:
    {
        HAL_Delay(S_2_mS(1));

        if (g_SysInfo.tVehicleState.bPWR_ON == TRUE) // read ADC, power on
        {
            // If ACC ON->OFF do final data recording
            if (bLastACC_State == TRUE)
            {
                LCD_SetShowAutoTestPage(39);

#if ENABLE_ADJUST_OFF_TIME
                osTimerStart(timerACC_OffHandle, S_2_mS(60));
#else
                osTimerStart(timerACC_OffHandle, S_2_mS(20));
#endif

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_KEY_OFF);

                g_Setting.ttAccOffTime = System_GetTime();

#if ENABLE_GSM_NET
                for (int i = 0; i < MAX_SOCKET_BANK; i++)
                {
                    GSM_TCP_SendEVENT_Direct((char *)EVENT_KEY_OFF, STR_NULL, i);

                    GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_OFF, STR_NULL, i);

                    if (bDataLogFlag[i] == FALSE)
                    {
                        datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._7 = 0;

                        memcpy(&datalogHandleTemp, &datalogHandle, sizeof(DatalogUploadTypeDef));

                        ttDataLogTime[i] += 30;
                    }
                    else
                    {
                        datalogHandleTemp[i].DATA[29].EXTIO.bit._7 = 0;
                    }

                    GSM_LOG_Proc(i);
                }
#endif

                // if(g_SysInfo.tVehicleState.bPWR_ON == 1)
                {
                    V161_LastMinLogProc();

                    RTOSprintf("\r\n> ### ACC-OFF Save Last GPS ###");
                    GPS_LastCoordinateProcess(GPS_POS_SAVE);

                    System_SaveSettingAll();
                    System_SaveBackupSettingAll(VAR_ALL_ITEMS);
                }
            }
            else
            {
#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_OFF, (char *)"0");
#endif
            }

            if (strlen((char *)FW_VERSION_CHK) > 0)
            {
                System_TaskCheckWaterMark();
            }

            localtime_r(&g_Setting.ttAccOffTime, &tmTime);

            strftime(szTime, sizeof(szTime), "%Y/%m/%d %H:%M:%S", &tmTime);

            RTOSprintf("\r\n> [%s] System set to enter power-down mode now.", szTime);

            if (g_Setting.tPwrDown.tMode == PWR_DOWN_SHUTDOWN)
            {
                u32DelayTimeout = g_Setting.tPwrDown.u16Delay;
            }
            else // standby mode
            {
#if ENABLE_ADJUST_OFF_TIME
                u32DelayTimeout = g_Setting.tPwrDown.u16Delay;
                u32DelayTimeout_GS = g_Setting.tGS_Setting.u32DelayTimeOut; // 3 minute
                u32DelayTimeout_4G = g_Setting.tGSM_SET.u32DelayTimeOut;    // 5 minute
#else
                u32DelayTimeout = 30;
#endif
            }
        }
        else // ACC off nd power off
        {
            char szTime[40];

            LCD_SetShowAutoTestPage(39);

            osTimerStart(timerACC_OffHandle, S_2_mS(10));

            g_Setting.ttAccOffTime = System_GetTime();

            RTOSprintf("\r\n> ### ACC-OFF Save Last GPS ###");
            GPS_LastCoordinateProcess(GPS_POS_SAVE);

            System_SaveSettingAll();
            System_SaveBackupSettingAll(VAR_ALL_ITEMS);

            u32TickBuf = HAL_GetTick();

            localtime_r(&g_Setting.ttAccOffTime, &tmTime);

            strftime(szTime, sizeof(szTime), "%Y/%m/%d %H:%M:%S", &tmTime);

            RTOSprintf("\r\n> [%s] System set to enter power-down mode now.", szTime);

            u32DelayTimeout = 0;
        }

        tPwrDownStep = PWR_DOWN_STEP_2;

        u32PwrDownDelayTick = HAL_GetTick();

        break;
    }

    case PWR_DOWN_STEP_2:
    {
        if (g_SysInfo.tVehicleState.bPWR_ON == FALSE) // turn off power
        {
#if ENABLE_GSM_NET
            QtEC21_CheckRedoFTP();

            if (bQtEC21_Pwr == TRUE)
            {
                RTOSprintf("\r\n> GSM power turn off.");

                bGSM_PwrOffFlag = TRUE;

                HAL_Delay(500);
            }
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
            RTOSprintf("\r\n> GPS power turn off.");

            GPS_PowerOff();
#endif

#if ENABLE_MG82
            MG82_Shutdown(MG82_PWR_MODE_DEF, 0);

            HAL_Delay(500);
#else
            Power_SourceDisable();
#endif
#if ENABLE_LSM6DSL
            LSM6DSL_DeInit();
#endif
            tPwrDownStep = PWR_DOWN_STEP_NONE;

            HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

            bRTOS_En = FALSE;

            RTOSprintf("\r\n> ### SYSTEM ENTER SHUTDOWN MODE ###");

            System_Reset();
        }

        if ((HW_PBGetState(ACC_IN) == 0) && (g_SysInfo.tVehicleState.bPWR_ON == TRUE)) // ACC on
        {
            tPwrDownStep = PWR_DOWN_STEP_NONE;
            break;
        }

        if (u8WkupSrc == MG82_WKUP_SRC_G_SEN)
        {
            if (HAL_GetTick() < (u32PwrDownDelayTick + S_2_mS(u32DelayTimeout_GS)))
            {
                break;
            }
            u8WkupSrc = MG82_WKUP_SRC_NONE; // change this one
        }
        else if (u8WkupSrc == MG82_WKUP_SRC_4G_RI)
        {
            if (HAL_GetTick() < (u32PwrDownDelayTick + S_2_mS(u32DelayTimeout_4G)))
            {
                break;
            }
            u8WkupSrc = MG82_WKUP_SRC_NONE;
        }
        else
        {
            if (HAL_GetTick() < (u32PwrDownDelayTick + S_2_mS(u32DelayTimeout)))
            {
                break;
            }
            u8WkupSrc = MG82_WKUP_SRC_NONE;
        }

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
        RTOSprintf("\r\n> GPS power turn off.");

        GPS_PowerOff();
#endif

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();

        // #if HW_VER != HW_JAS208_V630
        if (g_Setting.tPwrDown.tMode == PWR_DOWN_STANDBY)
        {
            bGSM_SetSleep = TRUE;

            HAL_Delay(S_2_mS(2));

// #if HW_TYPE == HW_JAS208
//  Drive 4G DTR to high to let 4G module sleep
// #if ENABLE_MG82
// 			QtEC21_SetSleep();

// 			HAL_Delay(10);
// #else
// 			bGSM_SetSleep = TRUE;

// 			HAL_Delay(S_2_mS(2));
// #endif
#elif HW_TYPE == HW_JAS306SD || HW_TYPE == HW_JAS411
        // Drive 4G DTR to high to let 4G module sleep
        HW_PBClr(GSM_SLEEP);
#endif
        }
        // #endif
        // #endif

        if (g_Setting.tPwrDown.tMode == PWR_DOWN_SHUTDOWN)
        {
#if ENABLE_LSM6DSL
            LSM6DSL_DeInit();
#endif
            RTOSprintf("\r\n> ### SYSTEM ENTER SHUTDOWN MODE ###");
#if ENABLE_MG82
            // MG82_Shutdown(MG82_PWR_MODE_DEF, 0);

            MG82_PWR_Mode(MG82_PWR_MODE_DEF);

            HAL_Delay(500);
#else
            Power_SourceDisable();
#endif
        }
        else
        {
#if ENABLE_MG82
#if ENABLE_EX_WKUP
            if ((g_Setting.tExPedWkup.bEnableWkup == TRUE) && (bLastACC_State == TRUE))
            {
#if RTC_WKUP_SRC == RTC_WKUP_SRC_MCU
                if (g_Setting.tExPedWkup.u32Timeout >= 10)
                {
                    System_SetWakeupTimer(g_Setting.tExPedWkup.u32Timeout);
                }
#elif RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
                MG82_RTC_PedWkup(g_Setting.tExPedWkup);

                HAL_Delay(100);
#endif
            }
            else if ((g_Setting.tExAlarm.bEnableAlm == TRUE) && (bLastACC_State == TRUE))
            {
                System_CheckExAlmSetting();

                HAL_Delay(100);
            }
#endif
#if ENABLE_LSM6DSL
            LSM6DSL_SetStandbyMode();
#endif
            MG82_USBPROTECTION_Set(0);

            MG82_PWR_Mode(MG82_PWR_MODE_DEF);

            HAL_Delay(500);
#endif

            RTOSprintf("\r\n> ### SYSTEM ENTER STANDBY MODE ###");
        }

        tPwrDownStep = PWR_DOWN_STEP_NONE;

        HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

        bRTOS_En = FALSE;

        System_Reset();

        break;
    }

    default:
        break;
    }
}

//============================================================================//
// @brief  Detect MCU power status.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_POWER_CHECK_VOLT // Check power by measuring voltage
void EXTIO_DetectPWR(void)
{
    static uint8_t u8LowToHighCnt = 0, u8HighToLowCnt = 0;

    u16PoweADCValue = EXTIO_GetADC1(ADC_MOTOR_PWR_CHANNEL);

    // if(ADC_GetConversionValue(ADC1) < MOTOR_POWER_VOLT_DEF) //AMBO ///Heinz
    // if(ADC_GetConversionValue(ADC1) < 0x0f0)
    // if(u16PoweADCValue < 0x0F0)
    if (u16PoweADCValue < 0x200)
    {
        if (CurrState.bits.IN_PWR == 1)
        {
            CurrState.bits.IN_PWR = 0;

            u8HighToLowCnt = 0;
        }
        else
        {
            if (g_SysInfo.tEXTIO.bits.IN_PWR == 1)
            {
                u8HighToLowCnt++;

                if (u8HighToLowCnt < 10)
                    return;

                g_SysInfo.tEXTIO.bits.IN_PWR = 0;

                g_SysInfo.tVehicleState.bPWR_ON = FALSE;

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_MOTOR_POWER_DOWN);

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_EXTERNAL_POWER_ISSUE, STR_NULL);
#endif
            }

            // If ACC is still ON
            if ((HW_PBGetState(ACC_IN) == 0) && (bWaitingEnterStandby == FALSE))
            {
                tPwrDownStep = PWR_DOWN_STEP_1;

                bWaitingEnterStandby = TRUE;
            }
        }
    }
    else
    {
        if (CurrState.bits.IN_PWR == 0)
        {
            CurrState.bits.IN_PWR = 1;

            u8LowToHighCnt = 0;
        }
        else
        {
            if (g_SysInfo.tEXTIO.bits.IN_PWR == 0)
            {
                u8LowToHighCnt++;

                if (u8LowToHighCnt < 10)
                    return;

                g_SysInfo.tEXTIO.bits.IN_PWR = 1;

                g_SysInfo.tVehicleState.bPWR_ON = TRUE;

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_MOTOR_POWER_ON);

                // GSM_TCP_ReportEvent((char *)EVENT_CAR_POWER_ON, STR_NULL);

                if ((HW_PBGetState(ACC_IN) == 0) && (bLastACC_State == TRUE))
                {
                    if (bWaitingEnterStandby == TRUE)
                    {
                        osTimerStop(timerACC_OffHandle);

                        HAL_Delay(S_2_mS(1));

                        bRS_FirstDetection = TRUE;
                        u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

                        bWaitingEnterStandby = FALSE;

                        RTOSprintf("\r\n> System stop enter standby mode.");

#if ENABLE_MG82
                        bMG82_Stop = FALSE;

                        MG82_CheckExAlmSetting();
#endif

                        if (bSystemMode == SYSTEM_MODE_NORMAL)
                        {
                            if (bLCD_Running == FALSE)
                            {
                                bLCD_TurnOn = TRUE;
                            }
                            else
                            {
                                LCD_SetShowAutoTestPage(50);
                            }
                        }
                    }
                }
            }
        }
    }
}
#else // Check power by logic
void EXTIO_DetectPWR(void)
{
    if (HW_PBGetState(PWR_SEL) == 1)
    {
        CurrState.bits.IN_PWR = 0;

        g_SysInfo.EXTIO.bits.IN_PWR = 0;

        if (g_SysInfo.VehicleState.PWR_ON == TRUE)
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_MOTOR_POWER_DOWN);
        }

        g_SysInfo.VehicleState.PWR_ON = FALSE;
    }
    else
    {
        if (CurrState.bits.IN_PWR == 0)
        {
            CurrState.bits.IN_PWR = 1;
        }
        else
        {
            g_SysInfo.EXTIO.bits.IN_PWR = 1;

            if (g_SysInfo.VehicleState.PWR_ON == FALSE)
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_MOTOR_POWER_ON);
            }

            g_SysInfo.VehicleState.PWR_ON = TRUE;
        }
    }
}
#endif

//============================================================================//
// @brief  Detect ACC status.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectACC(void)
{
    time_t ttRestTime = 0;
    static uint8_t u8OffToOnCnt = 0, u8OnToOffCnt = 0;

    if (HW_PBGetState(ACC_IN) == 1) // ACC off
    {
        if (CurrState.bits.IN_ACC == 1)
        {
            CurrState.bits.IN_ACC = 0;

            u8OnToOffCnt = 0;
        }
        else
        {
            if (g_SysInfo.tVehicleState.bACC_ON == TRUE)
            {
                u8OnToOffCnt++;

                if (u8OnToOffCnt < 30)
                    return;

                u8OffToOnCnt = 0;

                g_SysInfo.tEXTIO.bits.IN_ACC = 0;

                g_SysInfo.tVehicleState.bACC_ON = FALSE;

#if ENABLE_GSM_NET
                QtEC21_CheckRedoFTP();
#endif

                if (bWaitingEnterStandby == FALSE)
                {
                    bLastACC_State = TRUE;

                    tPwrDownStep = PWR_DOWN_STEP_1;

                    bWaitingEnterStandby = TRUE;
                }
            }
            else
            {
                g_SysInfo.tEXTIO.bits.IN_ACC = 0;

                if (bWaitingEnterStandby == FALSE)
                {
                    bLastACC_State = FALSE;

                    tPwrDownStep = PWR_DOWN_STEP_1;

                    bWaitingEnterStandby = TRUE;
                }
            }
        }
        // RTOSprintf("\r\n> ACC OFF.");
    }
    else // ACC on
    {
        if (CurrState.bits.IN_ACC == 0)
        {
            CurrState.bits.IN_ACC = 1;

            u8OffToOnCnt = 0;
        }
        else
        {
            if ((bWaitingEnterStandby == TRUE) && (bSystemMode == SYSTEM_MODE_NORMAL))
            {
                u8OffToOnCnt++;

                if (u8OffToOnCnt < 30)
                    return;
            }

            g_SysInfo.tEXTIO.bits.IN_ACC = 1;

            if (g_SysInfo.tVehicleState.bACC_ON == FALSE)
            {
                // Check for any 1 min flag set when waiting to enter standby
                if (bWaitingEnterStandby == TRUE)
                {
                    if ((bSD_ReportErrFlag == FALSE) && (bOneMinFlag == TRUE))
                    {
                        bOneMinFlag = FALSE;

                        if (V161_CheckVDRHeader() == TRUE)
                        {
                            // Do 1 min SD task for table 6, 8, 9 here
                            V161_1MinLogProc();
                        }
                    }
                }

#if ENABLE_GSM_NET
                bFirstACC = TRUE;
#endif

                g_SysInfo.tVehicleState.bACC_ON = TRUE;

                bLastACC_State = TRUE;

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_KEY_ON);

                ttAccOnTime = System_GetTime();

                g_Setting.tDRV_SET.ttDrvWorkStartTime = System_GetTime();

                if (g_Setting.ttAccOffTime > 0)
                {
                    ttRestTime = g_Setting.tDRV_SET.ttDrvWorkStartTime - g_Setting.ttAccOffTime;

                    RTOSprintf("\r\n> ### OFF_TO_ON: %d sec ###", ttRestTime);

                    if (ttRestTime > 0)
                    {
                        g_Setting.tDrvInfo[0].u32DrRestTime += ttRestTime;
                        g_Setting.tDrvInfo[1].u32DrRestTime += ttRestTime;
                        g_Setting.tDrvInfo[0].stDrvRecDay[13].u32RestTime += ttRestTime;
                        g_Setting.tDrvInfo[1].stDrvRecDay[13].u32RestTime += ttRestTime;

                        System_SaveSetting();
                    }
                    else
                    {
                        RTOSprintf("\r\n> ### Rest Time Less than zero ###", ttRestTime);
                    }
                }
                else
                {
                    RTOSprintf("\r\n> ### ACC Off time Less than zero ###", ttRestTime);
                }

                u32TickBuf = 0;

                if ((bWaitingEnterStandby == TRUE) && (g_SysInfo.tVehicleState.bPWR_ON == TRUE))
                {
                    osTimerStop(timerACC_OffHandle);

                    HAL_Delay(S_2_mS(1));

                    bRS_FirstDetection = TRUE;
                    u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

                    bWaitingEnterStandby = FALSE;

                    RTOSprintf("\r\n> System stop enter standby mode.");

#if ENABLE_MG82
                    bMG82_Stop = FALSE;

                    MG82_CheckExAlmSetting();
#endif

                    if (bSystemMode == SYSTEM_MODE_NORMAL)
                    {
                        if (bLCD_Running == FALSE)
                        {
                            bLCD_TurnOn = TRUE;
                        }
                        else
                        {
                            LCD_SetShowAutoTestPage(50);
                        }
                    }
                }

#if ENABLE_GSM_NET
#if ENABLE_FOTA_ACC_OFF
                bGSM_ResetCheckFTP_Flag = TRUE;
#endif

                GSM_TCP_ReportEvent((char *)EVENT_KEY_ON, STR_NULL);
#endif
            }

            if (bSystemMode == SYSTEM_MODE_STEALTH)
            {
                if (bLCD_Running == FALSE)
                    bLCD_TurnOn = TRUE;

                bSystemMode = SYSTEM_MODE_NORMAL;
            }
        }
    }
}

//============================================================================//
// @brief  Detect ACC status when the system is stopped.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_DetectACC_2(void)
{
    if (HW_PBGetState(ACC_IN) == 1) // ACC off
    {
        if (CurrState.bits.IN_ACC == 1)
        {
            CurrState.bits.IN_ACC = 0;
        }

        g_SysInfo.tEXTIO.bits.IN_ACC = 0;

        g_SysInfo.tVehicleState.bACC_ON = FALSE;

        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_KEY_OFF);

        // #if ENABLE_GSM_NET
        // if(bQtEC21_Pwr == TRUE)
        //{
        //	bGSM_PwrOffFlag = TRUE;

        //	HAL_Delay(S_2_mS(3));
        //}
        // #endif

        if (g_Setting.tPwrDown.tMode == PWR_DOWN_SHUTDOWN)
        {
#if ENABLE_MG82
            MG82_Shutdown(MG82_PWR_MODE_DEF, 5);
#else
            Power_SourceDisable();
#endif
        }
        else
        {
#if ENABLE_MG82
            MG82_PWR_Mode(MG82_PWR_MODE_DEF);
#endif
        }
#if ENABLE_LSM6DSL
        LSM6DSL_SetStandbyMode();
#endif
        HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

        bRTOS_En = FALSE;

        RTOSprintf("\r\n> ### SYSTEM ENTER STANDBY MODE ###");

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();
    }
}

//============================================================================//
// @brief  Detect battery power status.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_DetectCHG(void)
{
    static uint8_t u8BatIn = 0, u8BatOut = 0;
    static uint8_t u8ChgFull = 0, u8ChgEmpty = 0;
    // static float fUC_Power = 0;
    // static uint32_t u32UC_DetectTick = 0;

    // u16PoweADCValue2 = EXTIO_GetADC1(ADC_CHANNEL_14);

    // Calculate UC voltage (for 2 UCs in serial)
    // fUC_Power = (float)u16PoweADCValue2 / 4095 * (float)3.3 * 2;

    // if((HAL_GetTick() - u32UC_DetectTick) > 2000)
    //{
    //   EXTprintf("\r\n> UC voltage:%.2f", fUC_Power);

    //	u32UC_DetectTick = HAL_GetTick();
    //}

    if (HW_PBGetState(CHG_STATE))
    {
        u8ChgEmpty = 0;

        u8ChgFull++;

        if (u8ChgFull > 10)
        {
            u8ChgFull = 0;

            g_SysInfo.bBatChgState = TRUE;
        }
    }
    else
    {
        u8ChgFull = 0;

        u8ChgEmpty++;

        if (u8ChgEmpty > 10)
        {
            u8ChgEmpty = 0;

            g_SysInfo.bBatChgState = FALSE;
        }
    }

    // if(fUC_Power > (float)2.5)
    if (1)
    {
        u8BatIn++;

        if (u8BatIn > 10)
        {
            u8BatIn = 0;

            if (g_SysInfo.tEXTIO.bits.IN_CHG == 0)
            {
                g_SysInfo.tEXTIO.bits.IN_CHG = 1;
            }

            if (bBackupPwrStable == FALSE)
            {
                bBackupPwrStable = TRUE;

                // #if ENABLE_INDEX_BUFFER
                // bFileIdxIsSaved = FALSE;
                // #endif
            }
        }
    }
    else
    {
        u8BatIn = 0;

        u8BatOut++;

        if (u8BatOut > 10)
        {
            u8BatOut = 0;

            if (g_SysInfo.tEXTIO.bits.IN_CHG == 1)
            {
                g_SysInfo.tEXTIO.bits.IN_CHG = 0;
            }

            if (bBackupPwrStable == TRUE)
            {
#if ENABLE_INDEX_BUFFER
                if (bSD_ReportErrFlag == FALSE)
                {
                    EXTprintf("\r\n> Save file index all due to unstable power.");

                    V161_SaveFileIdxAll();
                }
#endif

                bBackupPwrStable = FALSE;
            }
        }
    }
}

#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306 // HW_JAS208 HW_JAS306
//============================================================================//
// @brief  Detect IO IN1.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN1(void)
{
    static uint32_t u32OnTick = 0, u32OffTick = 0;

    if (HW_PBGetState(IO1_IN) == 0) // IN_1 = 0, real value = 1
    {
        u32OffTick = 0;

        if (g_Setting.tReverse.bits.IN_1 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_1 == 1)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_1 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_FDOOR_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_1 == 0)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_1 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_FDOOR_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
    }
    else // IN_1 = 1, real value = 0
    {
        u32OnTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_1 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_1 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_1 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_FDOOR_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_1 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_1 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_FDOOR_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN2.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN2(void)
{
    static uint32_t u32OnTick = 0, u32OffTick = 0;

    if (HW_PBGetState(IO2_IN) == 0) // IN_2 = 0, real value = 1
    {
        u32OffTick = 0;

        if (g_Setting.tReverse.bits.IN_2 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_2 == 1)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_2 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BDOOR_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_2 == 0)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_2 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BDOOR_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
    }
    else // IN_2 = 1, real value = 0
    {
        u32OnTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_2 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_2 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_2 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BDOOR_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_2 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_2 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BDOOR_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN3.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN3(void)
{
    static uint32_t u32OffTick = 0;

    if (HW_PBGetState(IO3_IN) == 0) // IN_3 = 0, real value = 1
    {
        u32OffTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_3 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_3 == 1)
            {
                g_SysInfo.tEXTIO.bits.IN_3 = 0;

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_LEFT_SIGNAL_ON, STR_NULL);
#endif
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_3 == 0)
            {
                g_SysInfo.tEXTIO.bits.IN_3 = 1;

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_LEFT_SIGNAL_ON, STR_NULL);
#endif
            }
        }
    }
    else // IN_3 = 1, real value = 0
    {
        if (g_Setting.tReverse.bits.IN_3 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_3 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(5)))
                {
                    g_SysInfo.tEXTIO.bits.IN_3 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_LEFT_SIGNAL_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_3 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(5)))
                {
                    g_SysInfo.tEXTIO.bits.IN_3 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_LEFT_SIGNAL_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN4.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN4(void)
{
    static uint32_t u32OffTick = 0;

    if (HW_PBGetState(IO4_IN) == 0) // IN_4 = 0, real value = 1
    {
        u32OffTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_4 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_4 == 1)
            {
                g_SysInfo.tEXTIO.bits.IN_4 = 0;

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_RIGHT_SIGNAL_ON, STR_NULL);
#endif
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_4 == 0)
            {
                g_SysInfo.tEXTIO.bits.IN_4 = 1;

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_RIGHT_SIGNAL_ON, STR_NULL);
#endif
            }
        }
    }
    else // IN_4 = 1, real value = 0
    {
        if (g_Setting.tReverse.bits.IN_4 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_4 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(5)))
                {
                    g_SysInfo.tEXTIO.bits.IN_4 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_RIGHT_SIGNAL_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_4 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(5)))
                {
                    g_SysInfo.tEXTIO.bits.IN_4 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_RIGHT_SIGNAL_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN5.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN5(void)
{
    static uint32_t u32OnTick = 0, u32OffTick = 0;

    if (HW_PBGetState(IO5_IN) == 0) // IN_5 = 0, real value = 1
    {
        u32OffTick = 0;

        if (g_Setting.tReverse.bits.IN_5 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_5 == 1)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_5 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_HEADLIGHT_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_5 == 0)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_5 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_HEADLIGHT_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
    }
    else // IN_5 = 1, real value = 0
    {
        u32OnTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_5 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_5 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_5 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_HEADLIGHT_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_5 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_5 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_HEADLIGHT_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN6.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN6(void)
{
    static uint32_t u32OnTick = 0, u32OffTick = 0;

    if (HW_PBGetState(IO6_IN) == 0) // IN_6 = 0, real value = 1
    {
        u32OffTick = 0;

        if (g_Setting.tReverse.bits.IN_6 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_6 == 1)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_6 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BRAKE_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_6 == 0)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_6 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BRAKE_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
    }
    else // IN_6 = 1, real value = 0
    {
        u32OnTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_6 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_6 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_6 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BRAKE_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_6 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_6 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_BRAKE_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}

//============================================================================//
// @brief  Detect IO IN7.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_DetectIN7(void)
{
    static uint32_t u32OnTick = 0, u32OffTick = 0;

    if (HW_PBGetState(IO7_IN) == 0) // IN_7 = 0, real value = 1
    {
        u32OffTick = 0;

        if (g_Setting.tReverse.bits.IN_7 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_7 == 1)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_7 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_IO7_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_7 == 0)
            {
                if (HAL_GetTick() > (u32OnTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_7 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_IO7_ON, STR_NULL);
#endif
                }
            }
            else
            {
                u32OnTick = HAL_GetTick();
            }
        }
    }
    else // IN_7 = 1, real value = 0
    {
        u32OnTick = HAL_GetTick();

        if (g_Setting.tReverse.bits.IN_7 == 1) // Use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_7 == 0)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_7 = 1;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_IO7_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
        else // Not use reversed bit
        {
            if (g_SysInfo.tEXTIO.bits.IN_7 == 1)
            {
                if (HAL_GetTick() > (u32OffTick + S_2_mS(3)))
                {
                    g_SysInfo.tEXTIO.bits.IN_7 = 0;

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_IO7_OFF, STR_NULL);
#endif
                }
            }
            else
            {
                u32OffTick = HAL_GetTick();
            }
        }
    }
}
#endif

//============================================================================//
// @brief  EXTIO detect process.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_Detect_Proc(void)
{
    EXTIO_DetectPWR();

    EXTIO_DetectACC();

    EXTIO_PwrDownHandle();

    EXTIO_DetectCHG();

    if (EXTIO_IsMainPowerNormal() == TRUE)
    {
#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306 // HW_JAS208 HW_JAS306
        EXTIO_DetectIN1();

        EXTIO_DetectIN2();

        EXTIO_DetectIN3();

        EXTIO_DetectIN4();

        EXTIO_DetectIN5();

        EXTIO_DetectIN6();

        EXTIO_DetectIN7();
#endif
    }
}

// #if ENABLE_GSM_NET
//============================================================================//
//  @brief  Process speed and RPM data.
//  @param  None.
//  @retval None.
//============================================================================//
static void EXTIO_SpeedRPM_EventProc(void)
{
    static uint16_t u16OverSpeedOn = 0, u16OverSpeedOff = 0;
    static uint16_t u16RPM_IdleOn = 0, u16RPM_IdleOff = 0;
    static uint16_t u16Over_RPM_On = 0, u16Over_RPM_Off = 0;
    uint8_t u8AccelTotalSlot, u8AccelPreSlot;
    static uint8_t u8AccelCurrSlot = 0;
    float fAccelAmount;
    static bool bOverSpeedFlag = FALSE, bRPM_IdleFlag = FALSE, bOverRPM_Flag = FALSE,
                bOverAccelFlag = FALSE, bOverDecelFlag = FALSE;

    if ((M_2_KM(g_SysInfo.tVehicleState.u32Speed) > 3) && (bEVENT_SPEED_LINE == FALSE))
    {
        bEVENT_SPEED_LINE = TRUE;

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SPEED_INIT, STR_NULL);
#endif
    }

    // OVER SPEED EVENT
    if ((M_2_100M(g_SysInfo.tVehicleState.u32Speed) >= g_Setting.tSpeedSet.u16SPEED_LIMIT) &&
        (g_Setting.tSpeedSet.u16SPEED_LIMIT >= 900)) // Over speed event
    {
        u16OverSpeedOn++;
        u16OverSpeedOff = 0;

        // if((u16OverSpeedOn == g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX) && (bOverSpeedFlag == 0)) // Over speed start time reaches threshold
        if ((u16OverSpeedOn == 30) && (bOverSpeedFlag == 0)) // Over speed start time reaches threshold
        {
            // EXTprintf("\r\n> Over speed On");

            bOverSpeedFlag = 1;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_SPEED_START, STR_NULL);
#endif

            g_SysInfo.tSpeedInfo.bIsOverSpeed = TRUE; // Over speed TRUE
        }

        if (u16OverSpeedOn > 100)
            u16OverSpeedOn = 120;
    }
    else
    {
        u16OverSpeedOn = 0;
        u16OverSpeedOff++;

        if ((u16OverSpeedOff == g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX) && (bOverSpeedFlag)) // Over speed end time reaches threshold
        {
            // EXTprintf("\r\n> Over speed Off");

            bOverSpeedFlag = 0;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_SPEED_END, STR_NULL);
#endif

            g_SysInfo.tSpeedInfo.bIsOverSpeed = FALSE; // Over speed OFF
        }

        if (u16OverSpeedOff > 10)
            u16OverSpeedOff = 15;
    }

    // RPM IDLE EVENT
    // if(M_2_KM(g_SysInfo.tVehicleState.u32Speed) == 0)
    if ((g_Setting.tSpeedSet.u8MergeSpeed == FALSE) && (M_2_KM(g_SysInfo.tVehicleState.u32Speed) == 0))
    {
        // if(g_SysInfo.tVehicleState.u16RPM >= 400)
        if (g_SysInfo.tVehicleState.u16RPM > 0)
        {
            u16RPM_IdleOn++;
            u16RPM_IdleOff = 0;

            if ((u16RPM_IdleOn == g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX) && (bRPM_IdleFlag == FALSE)) // RPM idle start time reaches threshold
            {
                EXTprintf("\r\n> RPM IDLE ON.");

                bRPM_IdleFlag = TRUE;

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent(EVENT_RPM_IDLE_START, STR_NULL);
#endif

                g_SysInfo.tSpeedInfo.bIsRPM_Idle = TRUE; // RPM IDLE TRUE
            }

            if (u16RPM_IdleOn > g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX)
                u16RPM_IdleOn = 5000;
        }
    }

    if ((M_2_KM(g_SysInfo.tVehicleState.u32Speed) > 0) || (g_SysInfo.tVehicleState.bACC_ON == FALSE))
    {
        u16RPM_IdleOff++;
        u16RPM_IdleOn = 0;

        if ((u16RPM_IdleOff == 5) && (bRPM_IdleFlag == TRUE)) // RPM idle end time reaches threshold
        {
            EXTprintf("\r\n> RPM IDLE OFF.");

            bRPM_IdleFlag = FALSE;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent(EVENT_RPM_IDLE_END, STR_NULL);
#endif

            g_SysInfo.tSpeedInfo.bIsRPM_Idle = FALSE; // RPM IDLE OFF
        }

        if (u16RPM_IdleOff > 200)
            u16RPM_IdleOff = 500;
    }

    // OVER RPM EVENT
    // if(g_SysInfo.tVehicleState.u16RPM > RPM_LIMIT)
    if ((g_SysInfo.tVehicleState.u16RPM > g_Setting.tRPM_Set.u16Limit) &&
        (g_Setting.tRPM_Set.u16Limit >= 1000)) // Over RPM event
    {
        u16Over_RPM_On++;
        u16Over_RPM_Off = 0;

        if ((u16Over_RPM_On == g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX) && (bOverRPM_Flag == FALSE)) // Over RPM start time reaches threshold
        {
            // EXTprintf("\r\n> Over RPM On");

            bOverRPM_Flag = TRUE;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_RPM_START, STR_NULL);
#endif

            g_SysInfo.tSpeedInfo.bIsOver_RPM = TRUE; // Over RPM TRUE
        }

        if (g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX > 200)
            u16Over_RPM_On = 250;
    }
    else
    {
        u16Over_RPM_On = 0;
        u16Over_RPM_Off++;

        if ((u16Over_RPM_Off == g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX) && (bOverRPM_Flag == TRUE)) // Over RPM end time reaches threshold
        {
            // EXTprintf("\r\n> Over RPM Off");

            bOverRPM_Flag = FALSE;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_RPM_END, STR_NULL);
#endif

            g_SysInfo.tSpeedInfo.bIsOver_RPM = FALSE; // Over RPM OFF
        }

        if (u16Over_RPM_Off > 10)
            u16Over_RPM_Off = 15;
    }

    // OVER ACCELERATION/DECELERATION EVENT
    u8AccelTotalSlot = g_Setting.tSpeedSet.u8ACCEL_LIM_DURA + 1;

    fAccelSpeedSlot[u8AccelCurrSlot] = (float)(M_2_KM(g_SysInfo.tVehicleState.u32Speed));

    u8AccelPreSlot = (u8AccelCurrSlot + 1) % u8AccelTotalSlot;

    fAccelAmount = fAccelSpeedSlot[u8AccelCurrSlot] - fAccelSpeedSlot[u8AccelPreSlot];

    // If over acceleration detected
    if ((fAccelAmount >= (float)(g_Setting.tSpeedSet.u16ACCEL_LIMIT)) && (g_Setting.tSpeedSet.u16ACCEL_LIMIT >= 10))
    {
        if (bOverAccelFlag == FALSE)
        {
            bOverAccelFlag = TRUE;

            // RTOSprintf("\r\n> Over acceleration detected. Amount:%04.1f Km/h", fAccelAmount);

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_ACCEL_START, STR_NULL);
#endif
        }
    }
    else
    {
        if (bOverAccelFlag == TRUE)
        {
            // RTOSprintf("\r\n> Over acceleration end.");

            bOverAccelFlag = FALSE;
        }
    }

    // If over deceleration detected
    if ((fAccelAmount <= (0 - (float)(g_Setting.tSpeedSet.u16DECEL_LIMIT))) && (g_Setting.tSpeedSet.u16DECEL_LIMIT > 10) &&
        (g_SysInfo.tVehicleState.bACC_ON == TRUE) && (g_SysInfo.tVehicleState.bPWR_ON == TRUE))
    {
        if (bOverDecelFlag == FALSE)
        {
            bOverDecelFlag = TRUE;

            // RTOSprintf("\r\n> Over deceleration detected. Amount: %05.1f Km/h", fAccelAmount);

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_OVER_DECEL_START, STR_NULL);
#endif
        }
    }
    else
    {
        if (bOverDecelFlag == TRUE)
        {
            // RTOSprintf("\r\n> Over deceleration end.");

            bOverDecelFlag = FALSE;
        }
    }

    u8AccelCurrSlot++;

    if (u8AccelCurrSlot > (u8AccelTotalSlot - 1))
        u8AccelCurrSlot = 0;
}
// #endif

//============================================================================//
// @brief  Do half sec task.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_HalfSecTaskProc(void)
{
    if (M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) >= 4)
    {
        if (g_Setting.u8CurrDrvID == 0)
        {
            if (g_Setting.tDrvInfo[0].u8DrvStatus == 0)
            {
                g_Setting.tDrvInfo[0].u8DrvStatus = 1;
            }
            else if (g_Setting.tDrvInfo[0].u8DrvStatus == 1)
            {
                if (g_Setting.tDrvInfo[1].u8DrvStatus != 2)
                {
                    g_Setting.tDrvInfo[1].u8DrvStatus = 2;
                }
            }
            else if (g_Setting.tDrvInfo[0].u8DrvStatus == 2)
            {
                g_Setting.tDrvInfo[0].u8DrvStatus = 1;
            }
            else if (g_Setting.tDrvInfo[0].u8DrvStatus == 3)
            {
                g_Setting.tDrvInfo[0].u8DrvStatus = 1;
            }
        }
        else if (g_Setting.u8CurrDrvID == 1)
        {
            if (g_Setting.tDrvInfo[1].u8DrvStatus == 0)
            {
                g_Setting.tDrvInfo[1].u8DrvStatus = 1;
            }
            else if (g_Setting.tDrvInfo[1].u8DrvStatus == 1)
            {
                if (g_Setting.tDrvInfo[0].u8DrvStatus != 2)
                {
                    g_Setting.tDrvInfo[0].u8DrvStatus = 2;
                }
            }
            else if (g_Setting.tDrvInfo[1].u8DrvStatus == 2)
            {
                g_Setting.tDrvInfo[1].u8DrvStatus = 1;
            }
            else if (g_Setting.tDrvInfo[1].u8DrvStatus == 3)
            {
                g_Setting.tDrvInfo[1].u8DrvStatus = 1;
            }
        }
    }
    else if (M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) >= 3)
    {
        if (u32TickBuf == 0)
        {
            u32TickBuf = HAL_GetTick();
        }
        else if (HAL_GetTick() > (u32TickBuf + mS_2_SysTick(S_2_mS(4))))
        {
            if (g_Setting.u8CurrDrvID == 0)
            {
                if (g_Setting.tDrvInfo[0].u8DrvStatus == 0)
                {
                    g_Setting.tDrvInfo[0].u8DrvStatus = 1;
                }
                else if (g_Setting.tDrvInfo[0].u8DrvStatus == 1)
                {
                    if (g_Setting.tDrvInfo[1].u8DrvStatus != 2)
                    {
                        g_Setting.tDrvInfo[1].u8DrvStatus = 2;
                    }
                }
                else if (g_Setting.tDrvInfo[0].u8DrvStatus == 2)
                {
                    g_Setting.tDrvInfo[0].u8DrvStatus = 1;
                }
                else if (g_Setting.tDrvInfo[0].u8DrvStatus == 3)
                {
                    g_Setting.tDrvInfo[0].u8DrvStatus = 1;
                }
            }
            else if (g_Setting.u8CurrDrvID == 1)
            {
                if (g_Setting.tDrvInfo[1].u8DrvStatus == 0)
                {
                    g_Setting.tDrvInfo[1].u8DrvStatus = 1;
                }
                else if (g_Setting.tDrvInfo[1].u8DrvStatus == 1)
                {
                    if (g_Setting.tDrvInfo[0].u8DrvStatus != 2)
                    {
                        g_Setting.tDrvInfo[0].u8DrvStatus = 2;
                    }
                }
                else if (g_Setting.tDrvInfo[1].u8DrvStatus == 2)
                {
                    g_Setting.tDrvInfo[1].u8DrvStatus = 1;
                }
                else if (g_Setting.tDrvInfo[1].u8DrvStatus == 3)
                {
                    g_Setting.tDrvInfo[0].u8DrvStatus = 1;
                }
            }
        }
    }
    else
    {
        u32TickBuf = 0;

        if (g_Setting.u8CurrDrvID == 0)
        {
            if (g_Setting.tDrvInfo[0].u8DrvStatus == 1)
            {
                g_Setting.tDrvInfo[0].u8DrvStatus = 0;
            }
        }
        else if (g_Setting.u8CurrDrvID == 1)
        {
            if (g_Setting.tDrvInfo[1].u8DrvStatus == 1)
            {
                g_Setting.tDrvInfo[1].u8DrvStatus = 0;
            }
        }
    }
}

//============================================================================//
// @brief  Do one sec task.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_OneSecTaskProc(void)
{
    uint8_t i;
    uint32_t u32Buf, u32Buf2;
    static uint32_t u8SpeedBk = 0;
    static double dLastDistance = 0;
    DriveDataTypeDef stRecDrvStatus;
    uint8_t u8BCDTime[7];
    time_t ttTNow;

    // EXTprintf("\r\n> EXTIO 1 sec task.");

    for (i = 0; i < 2; i++)
    {
        if (g_Setting.tDrvInfo[i].u8DrvStatus == 0)
        {
            g_Setting.tDrvInfo[i].stDrvRecDay[13].u32PauseTime++;
        }
        else if (g_Setting.tDrvInfo[i].u8DrvStatus == 1)
        {
            g_Setting.tDrvInfo[i].stDrvRecDay[13].u32WorkTime++;

            g_Setting.tDrvInfo[i].u32DrWorkTime++;
        }
        else if (g_Setting.tDrvInfo[i].u8DrvStatus == 2)
        {
            g_Setting.tDrvInfo[i].stDrvRecDay[13].u32StandByTime++;
        }
        else if (g_Setting.tDrvInfo[i].u8DrvStatus == 3)
        {
            g_Setting.tDrvInfo[i].stDrvRecDay[13].u32RestTime++;

            g_Setting.tDrvInfo[i].u32DrRestTime++;
        }

        if (u8DrvPreStatus[i] != g_Setting.tDrvInfo[i].u8DrvStatus)
        {
            memset(&stRecDrvStatus, '\0', sizeof(DriveDataTypeDef));
            ttTNow = System_GetTime();
            SecToBCDTime(ttTNow, u8BCDTime);
            memcpy(stRecDrvStatus.u8DriveDateBCD, u8BCDTime, 7);
            stRecDrvStatus.u8DriveCode = g_Setting.tDrvInfo[i].u8DrvStatus;
            memcpy(stRecDrvStatus.u8DriverName, &g_Setting.tDrvInfo[i].u32DrvIdNum, 4);
            V161_RecordDriving(stRecDrvStatus);
            u8DrvPreStatus[i] = g_Setting.tDrvInfo[i].u8DrvStatus;
        }
    }

    /*
        if(g_Setting.tDrvInfo[0].u8DrvStatus == 0)
        {
            g_Setting.tDrvInfo[0].stDrvRecDay[13].u32PauseTime++;
        }
        else if(g_Setting.tDrvInfo[0].u8DrvStatus == 1)
        {
            g_Setting.tDrvInfo[0].stDrvRecDay[13].u32WorkTime++;

            g_Setting.tDrvInfo[0].u32DrWorkTime++;
        }
        else if(g_Setting.tDrvInfo[0].u8DrvStatus == 2)
        {
            g_Setting.tDrvInfo[0].stDrvRecDay[13].u32StandByTime++;
        }
        else if(g_Setting.tDrvInfo[0].u8DrvStatus == 3)
        {
            g_Setting.tDrvInfo[0].stDrvRecDay[13].u32RestTime++;

            g_Setting.tDrvInfo[0].u32DrRestTime++;
        }

        if(g_Setting.tDrvInfo[1].u8DrvStatus == 0)
        {
            g_Setting.tDrvInfo[1].stDrvRecDay[13].u32PauseTime++;
        }
        else if(g_Setting.tDrvInfo[1].u8DrvStatus == 1)
        {
            g_Setting.tDrvInfo[1].stDrvRecDay[13].u32WorkTime++;

            g_Setting.tDrvInfo[1].u32DrWorkTime++;
        }
        else if(g_Setting.tDrvInfo[1].u8DrvStatus == 2)
        {
            g_Setting.tDrvInfo[1].stDrvRecDay[13].u32StandByTime++;
        }
        else if(g_Setting.tDrvInfo[1].u8DrvStatus == 3)
        {
            g_Setting.tDrvInfo[1].stDrvRecDay[13].u32RestTime++;

            g_Setting.tDrvInfo[1].u32DrRestTime++;
        }
    */
    u32Buf = ((g_Setting.tDRV_SET.u8DriveThreshold[0] * 60) + g_Setting.tDRV_SET.u8DriveThreshold[1]) * 60;
    if (u32Buf >= 900)
    {
        u32Buf2 = u32Buf - 900;
    }
    else
    {
        u32Buf2 = 0;
    }
    // RTOSprintf("\r\n> g_Setting.tDRV_SET.u8DriveThreshold 1: %d Sec, 2: %d Sec", u32Buf, u32Buf2);

    // EXTprintf("\r\n> g_Setting.tDrvInfo[%d].u32DrWorkTime: %d Sec", g_Setting.u8CurrDrvID, g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime);

    if ((g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime >= u32Buf2) &&
        (g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime < u32Buf))
    {
        if (bPreWoringFlag == FALSE)
        {
            RTOSprintf("\r\n> ### Show Pre Drv Warning Page ###");

            LCD_SetShowAutoTestPage(49);

            u32ErrorPageTick3 = HAL_GetTick();

            bPreWoringFlag = TRUE;
        }

        if (u32ErrorPageTick3 > 0)
        {
            if (HAL_GetTick() > (u32ErrorPageTick3 + S_2_mS(30)))
            {
                u32ErrorPageTick3 = 0;

                LCD_SetShowAutoTestPage(50);
            }
        }
    }
    else if (g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime >= u32Buf)
    {
        if (bShowErrorFlag == FALSE)
        {
            RTOSprintf("\r\n> ### Show Drv Warning Page ###");

            LCD_SetShowAutoTestPage(28);

            u32ErrorPageTick = 0;

            u32ErrorPageTick2 = HAL_GetTick();

            bShowErrorFlag = TRUE;
        }

        if (u32ErrorPageTick2 > 0)
        {
            if (HAL_GetTick() > (u32ErrorPageTick2 + S_2_mS(30)))
            {
                u32ErrorPageTick = HAL_GetTick();

                u32ErrorPageTick2 = 0;

                LCD_SetShowAutoTestPage(50);
            }
        }

        if (u32ErrorPageTick > 0)
        {
            if (HAL_GetTick() > (u32ErrorPageTick + S_2_mS(600)))
            {
                bShowErrorFlag = FALSE;

                u32ErrorPageTick2 = 0;
            }
        }
    }
    else
    {
        if ((bShowErrorFlag == TRUE) || (bPreWoringFlag == TRUE))
        {
            RTOSprintf("\r\n> ### Clear Drv Warning Page ###");

            LCD_SetShowAutoTestPage(50);

            bShowErrorFlag = FALSE;

            bPreWoringFlag = FALSE;
        }
    }

    u32Buf = ((g_Setting.tDRV_SET.u8RestThreshold[0] * 60) + g_Setting.tDRV_SET.u8RestThreshold[1]) * 60;

    if (g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrRestTime >= u32Buf)
    {
        g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime = 0;

        g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrRestTime = 0;
    }

    if (M_2_KM(g_SysInfo.tVehicleState.dDistance - dLastDistance) >= 2)
    {
        // RTOSprintf("\r\n> ### 2 KM ###");

        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_SD_BUFFER
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE) &&
                ((V161_SDBuf_IsHighLoading() == TRUE) || ((V161_SDBuf_IsHighLoading() == FALSE) && (bSD_SaveDatRequest == FALSE))))
#else
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE))
#endif
                V161_RecordExternslData();
        }

        dLastDistance = g_SysInfo.tVehicleState.dDistance;
    }
    else if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) == 0) && (M_2_KM(u8SpeedBk) > 0))
    {
        // RTOSprintf("\r\n> ### SPEED == 0 ###");

        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_SD_BUFFER
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE) &&
                ((V161_SDBuf_IsHighLoading() == TRUE) || ((V161_SDBuf_IsHighLoading() == FALSE) && (bSD_SaveDatRequest == FALSE))))
#else
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE))
#endif
                V161_RecordExternslData();
        }

        u8SpeedBk = g_SysInfo.tSpeedInfo.u32RealSpeed;
    }
    else if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) > 0) && (M_2_KM(u8SpeedBk) == 0))
    {
        // RTOSprintf("\r\n> ### SPEED > 0 ###");

        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_SD_BUFFER
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE) &&
                ((V161_SDBuf_IsHighLoading() == TRUE) || ((V161_SDBuf_IsHighLoading() == FALSE) && (bSD_SaveDatRequest == FALSE))))
#else
            if ((bSystemTimeReady == TRUE) && (EXTIO_IsMainPowerNormal() == TRUE) && (bIsCheckVDRProcFlag == TRUE))
#endif
                V161_RecordExternslData();
        }

        u8SpeedBk = g_SysInfo.tSpeedInfo.u32RealSpeed;
    }

    if (g_Setting.bLCDSaver == TRUE)
    {
        // If Speed = 0, close LCM backlight
        if (M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) > 0)
        {
            HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_RESET);

            bIsBacklightFlag = FALSE;

            ttBacklightCnt = LCD_BACKLIGHT_COUNT;
        }
        else
        {
            if (ttBacklightCnt == 0)
            {
                if (bIsBacklightFlag == FALSE)
                {
                    HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_SET);

                    bIsBacklightFlag = TRUE;
                }

                ttBacklightCnt = LCD_BACKLIGHT_COUNT;
            }
            else
            {
                ttBacklightCnt--;
            }
        }
    }
    else
    {
        if ((bIsBacklightFlag == TRUE) && (M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) > 0))
        {
            HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_RESET);

            bIsBacklightFlag = FALSE;

            ttBacklightCnt = LCD_BACKLIGHT_COUNT;
        }
    }
}

//============================================================================//
// @brief  Check system power every 200ms.
// @param  None.
// @retval None.
//============================================================================//
/*
void EXTIO_CheckPower200ms(void)
{
    static uint32_t ttPowerCut;
    uint32_t u32Buf;


    if(u16PoweADCValue < ADC_VALUE_DOWN_LEVEL)
    {
        // Motor Power High to Low
        if (bPowerCutFlag == FALSE)
        {
            ttPowerCut = HAL_GetTick();

            bPowerCutFlag = TRUE;
        }
    }
    else
    {
        // Motor Power Low to High
        if(bPowerCutFlag == TRUE)
        {
            u32Buf = HAL_GetTick();

            if(u32Buf > (ttPowerCut + 100))
            {
                RTOSprintf("\r\n> PowerCut: %d ms", u32Buf - ttPowerCut);
            }

            if(u32Buf > 205)
            {
                bMessageFlag = TRUE;
            }

            bPowerCutFlag = FALSE;
        }
    }

    if(bMessageFlag == TRUE)
    {
        bMessageFlag = FALSE;

        EXTprintf("\r\n> Power Cut > 200ms");

        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_POWER_OFF_200MS);

        V161_SaveEvent(1);

        LCD_SetShowAutoTestPage(63);
    }
}
*/
//============================================================================//
// @brief  Get vehicle's acceleration.
// @param  None.
// @retval Vehicle's acceleration in float format.
//============================================================================//
float EXTIO_GetAccel(void)
{
    float fAccelRate;
    static float fLastAccelRate = 0;
    static float fCurrSpeed = 0.0f, fOldSpeed = 0.0f;
    static uint32_t u32LastSampleTime = 0;

    fCurrSpeed = (float)g_SysInfo.tVehicleState.u32Speed / HOUR_2_SECOND(1); // Get float value of speed in m/s unit

    if (fOldSpeed != 0.0f)
    {
        // Get acceleration in unit m/s2
        uint32_t u32Sample_CurrTick = HAL_GetTick();

        if (u32Sample_CurrTick >= u32LastSampleTime)
        {
            fAccelRate = (fCurrSpeed - fOldSpeed) / ((u32Sample_CurrTick - u32LastSampleTime) / 1000);

            fLastAccelRate = fAccelRate;
        }
        else
        {
            fAccelRate = fLastAccelRate;
        }
    }
    else
    {
        fAccelRate = 0.0f;
    }

    u32LastSampleTime = HAL_GetTick();

    fOldSpeed = fCurrSpeed;

    return fAccelRate;
}

#if ENABLE_GSM_NET
//============================================================================//
// @brief  Process vehicle's acceleration data.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_AccelProc(void)
{
    static uint16_t u16OverAccelOn = 0, u16OverAccelOff = 0;
    static uint16_t u16OverDecelOn = 0, u16OverDecelOff = 0;
    static bool bOverAccelFlag = 0, bOverDecelFlag = 0;

    g_SysInfo.tSpeedInfo.fAccelRate = EXTIO_GetAccel();

    // RTOSprintf("\r\n> Accel:%02.1f", g_SysInfo.tSpeedInfo.fAccelRate);

    if (g_SysInfo.tSpeedInfo.fAccelRate > ACCEL_LIMIT) // Over acceleration
    {
        u16OverAccelOn++;
        u16OverAccelOff = 0;
        u16OverDecelOn = 0;
        u16OverDecelOff = 0;

        if ((u16OverAccelOn == 3) && (bOverAccelFlag == FALSE)) // Over acceleration event detected
        {
            if (bOverDecelFlag == TRUE)
            {
                // EXTprintf("\r\n> Over Deceleration Off");

                bOverAccelFlag = FALSE;

                g_SysInfo.tSpeedInfo.bIsOverDecel = TRUE; // Over deceleration OFF

                // GSM_TCP_ReportEvent((char *)EVENT_OVER_DECEL_END , STR_NULL);
            }

            // EXTprintf("\r\n> Over Acceleration On");

            bOverAccelFlag = TRUE;

            g_SysInfo.tSpeedInfo.bIsOverAccel = TRUE; // Over acceleration TRUE

            GSM_TCP_ReportEvent((char *)EVENT_OVER_ACCEL_START, STR_NULL);
        }

        if (u16OverAccelOn > 5)
            u16OverAccelOn = 6;
    }
    else if (g_SysInfo.tSpeedInfo.fAccelRate < DECEL_LIMIT) // Over deceleration
    {
        u16OverDecelOn++;
        u16OverDecelOff = 0;
        u16OverAccelOn = 0;
        u16OverAccelOff = 0;

        if ((u16OverDecelOn == 3) && (bOverDecelFlag == FALSE)) // Over deceleration event detected
        {
            if (bOverAccelFlag == TRUE)
            {
                // EXTprintf("\r\n> Over Acceleration Off");

                bOverAccelFlag = FALSE;

                g_SysInfo.tSpeedInfo.bIsOverAccel = FALSE; // Over acceleration OFF

                // GSM_TCP_ReportEvent((char *)EVENT_OVER_ACCEL_END , STR_NULL);
            }

            // EXTprintf("\r\n> Over Deceleration On");

            bOverDecelFlag = TRUE;

            g_SysInfo.tSpeedInfo.bIsOverDecel = TRUE; // Over deceleration TRUE

            GSM_TCP_ReportEvent((char *)EVENT_OVER_DECEL_START, STR_NULL);
        }

        if (u16OverDecelOn > 5)
            u16OverDecelOn = 6;
    }
    else
    {
        u16OverAccelOn = 0;
        u16OverAccelOff++;
        u16OverDecelOn = 0;
        u16OverDecelOff++;

        if ((u16OverAccelOff == 4) && (bOverAccelFlag == TRUE))
        {
            // EXTprintf("\r\n> Over Acceleration Off");

            bOverAccelFlag = FALSE;

            // GSM_TCP_ReportEvent((char *)EVENT_OVER_ACCEL_END , STR_NULL);

            g_SysInfo.tSpeedInfo.bIsOverAccel = FALSE; // Over Acceleration OFF
        }

        if (u16OverAccelOff > 5)
            u16OverAccelOff = 6;

        if ((u16OverDecelOff == 4) && (bOverDecelFlag == TRUE))
        {
            // EXTprintf("\r\n> Over Deceleration Off");

            bOverDecelFlag = FALSE;

            // GSM_TCP_ReportEvent((char *)EVENT_OVER_DECEL_END , STR_NULL);

            g_SysInfo.tSpeedInfo.bIsOverDecel = FALSE; // Over Deceleration OFF
        }

        if (u16OverDecelOff > 5)
            u16OverDecelOff = 6;
    }
}
#endif

//============================================================================//
// @brief  Check vehicle speed.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_SpeedLimitProc(void)
{
    static uint32_t u32MyTick = 0;
    uint32_t u32Speed;
    static uint8_t u8OldBzState = 0, u8CurrBzState = 0;

    if (u8OldBzState > 3)
        return;

    u32Speed = g_SysInfo.tSpeedInfo.u32RealSpeed;

    // EXTprintf("\r\n> Speed:%d", u32Speed);

    if ((u32Speed > g_Setting.tSpeedSet.u32Limit[2]) && (g_Setting.tSpeedSet.u32Limit[2] > 0))
    {
        u8CurrBzState = 3;
    }
    else if ((u32Speed > g_Setting.tSpeedSet.u32Limit[1]) && (g_Setting.tSpeedSet.u32Limit[1] > 0))
    {
        u8CurrBzState = 2;
    }
    else if ((u32Speed > g_Setting.tSpeedSet.u32Limit[0]) && (g_Setting.tSpeedSet.u32Limit[0] > 0))
    {
        u8CurrBzState = 1;
    }
    else
    {
        u8CurrBzState = 0;
    }

    // EXTprintf("\r\n> CurrBzState:%d, OldBzState:%d", u8CurrBzState, u8OldBzState);

    if (u8CurrBzState != u8OldBzState)
    {
        u8OldBzState = u8CurrBzState;

        if (u8CurrBzState > 0)
        {
            EXTprintf("\r\n> Over speed level %d detected.", u8CurrBzState);

            if (u8CurrBzState >= 2)
            {
                Buzzer_SetRing2(u8CurrBzState);
            }
            else
            {
                Buzzer_ClrRing2();
                Buzzer_Beep(BEEP_LONG, 2);
            }
        }
        else
        {
            EXTprintf("\r\n> Speed returned to below limit.");

            Buzzer_ClrRing2();
        }
    }

    if (HAL_GetTick() > (u32MyTick + 2000))
    {
        // EXTprintf("\r\n> ### T2: %d ms ###", HAL_GetTick() - u32MyTick2);

        u32MyTick = HAL_GetTick();

        // BUZZER Process
        Buzzer_Handle_Proc2();
    }

    // HAL_Delay(S_2_mS(1));
}

//============================================================================//
// @brief  Enable Battery power source.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_BatteryEnable(void)
{
    // HAL_GPIO_WritePin(CHG_STATE_GPIO_Port, CHG_STATE_Pin, GPIO_PIN_SET);

    HAL_Delay(10);
}

//============================================================================//
// @brief  Disable Battery power source.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_BatteryDisable(void)
{
    // HAL_GPIO_WritePin(CHG_STATE_GPIO_Port, CHG_STATE_Pin, GPIO_PIN_RESET);

    HAL_Delay(10);
}

//============================================================================//
// @brief  Record special EXTIO Log.
// @param  pcLog: Log.
//         u16Len: Log length.
// @retval None.
//============================================================================//
void EXTIO_AGAIN_DebugLog(char *pcLog, uint16_t u16Len)
{
    time_t ttTime;
    uint8_t u8BCD[7] = {0};
    char cDebugLog[100] = {0};
    char *pcPtr;

    ttTime = System_GetTime();

    SecToBCDTime(ttTime, u8BCD);

    pcPtr = cDebugLog;

    sprintf(pcPtr, "\r\n> (%02X:%02X:%02X) ",
            u8BCD[4],
            u8BCD[5],
            u8BCD[6]);

    pcPtr += strlen(pcPtr);

    strncpy(pcPtr, pcLog, u16Len);

    GSMprintf("%s", cDebugLog);

    SD_AddBuffToFile((char *)FILENAME_AGAIN_LOG_FILE, (uint8_t *)cDebugLog, strlen(cDebugLog));
}

//============================================================================//
// @brief  Perform autogain using CAN.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_AutoGainCAN(void)
{
    static double dDistBase1_CAN = 0, dDistBase2_CAN = 0;
    char cMsg[64] = {0};
    static uint32_t u32DebugTick = 0;
    static uint32_t u32CAN_DistTimeOut = 0;

    if ((EXTIO_IsMainPowerNormal() == FALSE) ||
        (g_Setting.tSpeedSet.u8Auto_Gain_Count == AUTO_GAIN_OK_COUNT) ||
        (g_Setting.tSpeedSet.u8AutoGainSrc != AUTOGAIN_SOURCE_CAN) ||
        (HAL_GetTick() < (ttAG_DelayBegin + S_2_mS(303))))
    {
        u32CAN_DistTimeOut = HAL_GetTick();

        return;
    }

    if (bCAN_GetDist == TRUE)
    {
        u32CAN_DistTimeOut = HAL_GetTick();

        bCAN_GetDist = FALSE;
    }

    if (HAL_GetTick() > (u32CAN_DistTimeOut + S_2_mS(300)))
    {
        RTOSprintf("\r\n> ### CAN AUTOGAIN GET DIST TIMEOUT! ###");

        g_Setting.tSpeedSet.u8Auto_Gain_Count = AUTO_GAIN_OK_COUNT;

        bIsShowAGMsgFlag = TRUE;

        RTOSprintf("\r\n> ### AUTOGAIN Request Start ###");

        // ttAG_DelayBegin = HAL_GetTick() + S_2_mS(60);

        g_Setting.u16AG_Dist_Limit_Temp = 512;
        g_Setting.tSpeedSet.u8AutoGainInit = 1;

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_NG, (char *)"AGCAN");
#endif

        return;
    }

    if (g_Setting.tSpeedSet.u8Auto_Gain_Count >= AUTO_GAIN_MAX_COUNT)
    {
        if (g_Setting.tSpeedSet.u8Auto_Gain_Count != 128)
        {
            g_Setting.tSpeedSet.u8Auto_Gain_Count = AUTO_GAIN_OK_COUNT;

            g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
            g_SysInfo.tVehicleState.u32DistCount = 0;

            g_Setting.tSpeedSet.fGain = fAutoGainValue;

            // u32AG_ReStartDelayTick = HAL_GetTick();

            bRS_FirstDetection = TRUE;
            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

            // RTOSprintf("\r\n> ### CAN AUTOGAIN set Gain: %f ###\r\n", g_Setting.tSpeedSet.fGain);

            bIsSaveSettingFlag = TRUE;
        }

        return;
    }

    if (u8DistBaseCAN_Init == 0)
    {
        // RTOSprintf("\r\n> CAN set distbase step 1");
        dDistBase1_CAN = CAN_GetDist();
        dDistBase2_CAN = dDistBase1_CAN;
        u8DistBaseCAN_Init = 1;
    }

    if (u8DistBaseCAN_Init == 1)
    {
        dDistBase2_CAN = CAN_GetDist();
    }

    if ((dDistBase1_CAN != dDistBase2_CAN) && (u8DistBaseCAN_Init == 1))
    {
        u8DistBaseCAN_Init = 2;
        g_SysInfo.tVehicleState.dDistBase_CAN = dDistBase2_CAN;
        g_SysInfo.tVehicleState.u32DistCount_AGain = 0;

        RTOSprintf("\r\n> ### AutoGAIN CAN distance count begin (%.2f) ###", g_SysInfo.tVehicleState.dDistBase_CAN);

        memset(cMsg, 0x00, sizeof(cMsg));

        sprintf(cMsg, "AGAIN CAN Loop dist cnt begin (%d-%d)", g_Setting.tSpeedSet.u8Auto_Gain_Count, (int)g_SysInfo.tVehicleState.dDistBase_CAN);

        EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));
    }

    if (g_SysInfo.tVehicleState.dDistBase_CAN == 0)
        return;

    g_SysInfo.tVehicleState.dDistance_CAN = CAN_GetDist() - g_SysInfo.tVehicleState.dDistBase_CAN;

    if (HAL_GetTick() > (u32DebugTick + S_2_mS(20)))
    {
        u32DebugTick = HAL_GetTick();

        float fTempGAIN;

        if (g_SysInfo.tVehicleState.u32DistCount_AGain > 0)
            fTempGAIN = ((g_SysInfo.tVehicleState.dDistance_CAN / g_SysInfo.tVehicleState.u32DistCount_AGain) * 1000);
        else
            fTempGAIN = 0;

        // RTOSprintf("\r\n> AUTOGAIN pulse cnt: %d, CAN DIST: %d",
        //					 g_SysInfo.tVehicleState.u32DistCount_AGain,
        //				  (int)g_SysInfo.tVehicleState.dDistance_CAN);

        memset(cMsg, 0x00, sizeof(cMsg));

        sprintf(cMsg, "AGAIN CAN... (%d-%d-%.2f)",
                g_SysInfo.tVehicleState.u32DistCount_AGain,
                (int)g_SysInfo.tVehicleState.dDistance_CAN,
                fTempGAIN);

        EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));
    }

    /*
        // GPS speed must higher than 20km/h
        //if((g_SysInfo.tVehicleState.u32Speed_GPS >= 20000) &&
        if((CAN_GetSpeed() >= 20000) &&
             (g_SysInfo.tGPS_Data.bStatus == TRUE) &&
           //(g_SysInfo.tVehicleState.dDistance_CAN > dDistanceCAN_Old) &&
           (g_SysInfo.tVehicleState.u32DistCount_AGain > u32DistCntAG_Old))
        {
            dDistanceCAN_Old = g_SysInfo.tVehicleState.dDistance_CAN;
            u32DistCntAG_Old = g_SysInfo.tVehicleState.u32DistCount_AGain;
        }
        else // If condition is not satisfied discard accumulated values in the previous second
        {
            if(dDistanceCAN_Old > 0)
            {
                g_SysInfo.tVehicleState.dDistBase_CAN += (g_SysInfo.tVehicleState.dDistance_CAN - dDistanceCAN_Old);
            }
            else
            {
                g_SysInfo.tVehicleState.dDistBase_CAN = CAN_GetDist();
            }
            g_SysInfo.tVehicleState.dDistance_CAN = dDistanceCAN_Old;
            g_SysInfo.tVehicleState.u32DistCount_AGain = u32DistCntAG_Old;
        }
    */

    if ((g_SysInfo.tVehicleState.dDistance_CAN >= g_Setting.u16AG_Dist_Limit_Temp) &&
        (g_SysInfo.tVehicleState.dDistBase_CAN > 0))
    {
        if (g_Setting.tSpeedSet.u8Auto_Gain_Count < AUTO_GAIN_MAX_COUNT)
        {
            if (g_Setting.tSpeedSet.u8Auto_Gain_Count == (AUTO_GAIN_MAX_COUNT - 1))
                g_Setting.tSpeedSet.u8Auto_Gain_Count = AUTO_GAIN_OK_COUNT;

            // u32AG_ReStartDelayTick = HAL_GetTick();

            RTOSprintf("\r\n> ### AutoGAIN CAN distance count end (%.2f) ###", CAN_GetDist());
            // RTOSprintf("\r\n> ### AutoGAIN accumulated CAN distance (%.2f) ###", g_SysInfo.tVehicleState.dDistance_CAN);

            memset(cMsg, 0x00, sizeof(cMsg));

            sprintf(cMsg, "AGAIN CAN Loop dist cnt end (%d)", (int)CAN_GetDist());

            EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

            // Fix CAN distance step decreased from 128->96
            if (((int)g_SysInfo.tVehicleState.dDistance_CAN % 128) == 96)
            {
                uint32_t u32TempDist = (int)g_SysInfo.tVehicleState.dDistance_CAN - 96 + 128;

                memset(cMsg, 0x00, sizeof(cMsg));

                sprintf(cMsg, "AGAIN CAN dist fix %d->%d",
                        (int)g_SysInfo.tVehicleState.dDistance_CAN,
                        u32TempDist);

                EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

                g_SysInfo.tVehicleState.dDistance_CAN = u32TempDist;
            }

            // fAutoGain unit M/Hz*1000 = MM/Hz
            if (g_SysInfo.tVehicleState.u32DistCount_AGain > 0)
            {
                g_Setting.tSpeedSet.fAutoGain = (((g_SysInfo.tVehicleState.dDistance_CAN) /
                                                  g_SysInfo.tVehicleState.u32DistCount_AGain) *
                                                 1000);
            }
            else
            {
                g_Setting.tSpeedSet.fAutoGain = 0;
            }

            RTOSprintf("\r\n> ### AutoGAIN calculated GAIN: %.2f (DistCnt: %d, Dist: %d) ###",
                       g_Setting.tSpeedSet.fAutoGain,
                       g_SysInfo.tVehicleState.u32DistCount_AGain,
                       (int)g_SysInfo.tVehicleState.dDistance_CAN);

            memset(cMsg, 0x00, sizeof(cMsg));

            sprintf(cMsg, "AGAIN CAN res: %.2f-%d-%d",
                    g_Setting.tSpeedSet.fAutoGain,
                    g_SysInfo.tVehicleState.u32DistCount_AGain,
                    (int)g_SysInfo.tVehicleState.dDistance_CAN);

            EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

            // Event Auto gain ok
            //  In the interrupt don't printf any thing.
            // RTOSprintf("\r\n> ### SYS_STATE_AUTO_GAIN_OK, Cnt: %d ###", g_Setting.tSpeedSet.u8Auto_Gain_Count);

            /*
            g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
            g_SysInfo.tVehicleState.u32DistCount = 0;

            if(g_Setting.tSpeedSet.fGain == 0)
            {
                g_Setting.tSpeedSet.fGain = g_Setting.tSpeedSet.fAutoGain;
            }
            else
            {
                g_Setting.tSpeedSet.fGain = (g_Setting.tSpeedSet.fGain + g_Setting.tSpeedSet.fAutoGain) / 2;
            }

            bRS_FirstDetection = TRUE;
            */

            if (fAutoGainValue == 0)
            {
                fAutoGainValue = g_Setting.tSpeedSet.fAutoGain;
            }
            else
            {
                fAutoGainValue = (fAutoGainValue + g_Setting.tSpeedSet.fAutoGain) / 2;
            }
        }

        g_Setting.tSpeedSet.u8Auto_Gain_Count++;

        g_SysInfo.tVehicleState.u32DistCount_AGain = 0;
        g_SysInfo.tVehicleState.dDistBase_GPS = 0;
        g_SysInfo.tVehicleState.dDistance_GPS = 0;
        g_SysInfo.tVehicleState.dDistBase_CAN = 0;
        g_SysInfo.tVehicleState.dDistance_CAN = 0;
        dDistanceCAN_Old = 0;
        u32DistCntAG_Old = 0;
        u8DistBaseCAN_Init = 0;

        bIsShowAGMsgFlag = FALSE;

        bIsSaveSettingFlag = TRUE;

        return;
    }
}

//============================================================================//
// @brief  Check Auto Gain Calibration.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_AutoGainProc(void)
{
    time_t ttTime;
    // int iValue = 0;
    char cCmdBuf[40] = {0};

    // If enable GPS merge speed disable Autogain function
    if (g_Setting.tSpeedSet.u8MergeSpeed == 1)
    {
        g_Setting.tSpeedSet.u8Auto_Gain_Count = 128; // AUTO_GAIN_OK_COUNT; // Heinz
        fAutoGainValue = 0;
        g_Setting.tSpeedSet.fAutoGain = 0;
        g_Setting.tSpeedSet.ttAutoGainTimeOut = 0;
        g_SysInfo.tVehicleState.dDistBase_GPS = 0;
        g_SysInfo.tVehicleState.dDistance_GPS = 0;
        g_SysInfo.tVehicleState.dDistBase_CAN = 0;
        g_SysInfo.tVehicleState.dDistance_CAN = 0;
        g_SysInfo.tVehicleState.u32DistCount_AGain = 0;
        dDistanceCAN_Old = 0;
        u32DistCntAG_Old = 0;
        u8DistBaseCAN_Init = 0;

        return;
    }

    ttTime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    if ((g_Setting.tSpeedSet.fGain > 0) && (g_SysInfo.tEXTIO.bits.IN_ACC == 1))
    {
        if (ttTime >= g_Setting.tSpeedSet.ttAgain_Time)
        {
            // Debug
            // RTOSprintf("\r\n> ### NowTime: %d, AutoGainTime: %d ###", ttTime, g_Setting.tSpeedSet.ttAgain_Time);

            RTOSprintf("\r\n> ### AUTOGAIN TimeUp, Request Start ###");

            // Do Auto gain
            g_Setting.tSpeedSet.ttAgain_Time += 63072000;

            g_Setting.u16AG_Dist_Limit_Temp = 512;
            g_Setting.tSpeedSet.u8AutoGainInit = 1;

            System_SaveSetting();

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_2Y, STR_NULL);
#endif

            System_SaveSetting();

            V161_SaveCalibration();
        }
    }

    /*
        if((g_Setting.tSpeedSet.fGain > 0) &&
             (g_Setting.tSpeedSet.u8Auto_Gain_Count == 128) &&
             (M_2_100M(g_SysInfo.tVehicleState.u32Speed_GPS) >= 660))
    */
    /*
        if((fAutoGainValue > 0) && (g_Setting.tSpeedSet.u8Auto_Gain_Count == 128) &&
             (M_2_100M(g_SysInfo.tVehicleState.u32Speed_GPS) >= 660))
        {
            iValue = M_2_KM(g_SysInfo.tVehicleState.u32Speed_GPS) - M_2_KM(g_SysInfo.tVehicleState.u32Speed);
        }

        if(iValue >= 10)
        {
            Again_AutoGain_Count++;
        }
        else
        {
            if(iValue <= -10)
            {
                Again_AutoGain_Count++;
            }
            else
            {
                Again_AutoGain_Count = 0;
            }
        }

        if(Again_AutoGain_Count >= 300) // 7 Min
        {
            RTOSprintf(">> Again auto gain..\r\n");

            g_Setting.u16AG_Dist_Limit_Temp = 512;
            g_Setting.tSpeedSet.u8AutoGainInit = 1;

            System_SaveSetting();
        }
    */

    EXTIO_AutoGainCAN();

    if (g_Setting.tSpeedSet.u8Auto_Gain_Count == 1)
    {
        if (bIsShowAGMsgFlag == FALSE)
        {
            RTOSprintf("\r\n> ### Auto Gain Loop 1 ###");
#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_LOOP1, STR_NULL);
#endif

            bIsShowAGMsgFlag = TRUE;

            if (g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS)
            {
                memset(cCmdBuf, 0x00, sizeof(cCmdBuf));

                sprintf(cCmdBuf, "AGAIN CAN res (0): %.2f-%d-%d",
                        g_Setting.tSpeedSet.fAutoGain,
                        u32FinalPulseCnt,
                        u32FinalDist);

                EXTIO_AGAIN_DebugLog(cCmdBuf, strlen(cCmdBuf));
            }
        }
    }
    else if (g_Setting.tSpeedSet.u8Auto_Gain_Count == 2)
    {
        if (bIsShowAGMsgFlag == FALSE)
        {
            RTOSprintf("\r\n> ### Auto Gain Loop 2 ###");
#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_LOOP2, STR_NULL);
#endif

            bIsShowAGMsgFlag = TRUE;

            if (g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS)
            {
                memset(cCmdBuf, 0x00, sizeof(cCmdBuf));

                sprintf(cCmdBuf, "AGAIN CAN res (1): %.2f-%d-%d",
                        g_Setting.tSpeedSet.fAutoGain,
                        u32FinalPulseCnt,
                        u32FinalDist);

                EXTIO_AGAIN_DebugLog(cCmdBuf, strlen(cCmdBuf));
            }
        }
    }
    else if (g_Setting.tSpeedSet.u8Auto_Gain_Count == 128)
    {
        if (bIsShowAGMsgFlag == FALSE)
        {
            sprintf(cCmdBuf, "AG=%.2f-%d-%d", g_Setting.tSpeedSet.fGain, g_Setting.tSpeedSet.u8AutoGainSrc, g_Setting.u16AG_Dist_Limit_Temp);
            RTOSprintf("\r\n> ### Auto Gain OK, %s ###", cCmdBuf);
#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_OK, cCmdBuf);
#endif

            bIsShowAGMsgFlag = TRUE;

            // if(g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS)
            {
                memset(cCmdBuf, 0x00, sizeof(cCmdBuf));

                sprintf(cCmdBuf, "AGAIN CAN final res: %.2f-%d-%d",
                        g_Setting.tSpeedSet.fGain,
                        u32FinalPulseCnt,
                        u32FinalDist);

                EXTIO_AGAIN_DebugLog(cCmdBuf, strlen(cCmdBuf));
            }

            // Check to do accurate AutoGAIN
            if (g_Setting.u16AG_Dist_Limit_Temp < 3000)
            {
                RTOSprintf("\r\n> ### ACCURATE AUTOGAIN Request Start ###");

                // ttAG_DelayBegin = HAL_GetTick();
                g_Setting.u16AG_Dist_Limit_Temp = 3000;
                g_Setting.tSpeedSet.u8AutoGainInit = 1;

                System_SaveSetting();
            }
            else
            {
                g_Setting.u16AG_Dist_Limit_Temp = 512;

                System_SaveSetting();
            }
        }
    }

    if (bAGainNgFlag == TRUE)
    {
        sprintf(cCmdBuf, "Gain: %.2f NG", fNgGainValue);

        RTOSprintf("\r\n> ### %s ###", cCmdBuf);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_NG, cCmdBuf);
#endif

        bAGainNgFlag = FALSE;
    }

    /*
    if((g_Setting.tSpeedSet.u8Auto_Gain_Count != 128) &&
         (ttTime > g_Setting.tSpeedSet.ttAutoGainTimeOut) &&
         (g_Setting.tSpeedSet.ttAutoGainTimeOut > 0))
    {
        RTOSprintf("\r\n> ### Auto Gain TimeOut, Stop Process. ###");

        sprintf(cCmdBuf, "$AS+AUTO GAIN=OFF\r\n");
        RTOSprintf("\r\n> %s", cCmdBuf);
        Protocl_ChkCmdPkg(cCmdBuf, NULL);

        #if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_NG, STR_NULL);
        #endif
    }
    */
}

//============================================================================//
// @brief  Get GPS distance.
// @param  None.
// @retval GPS distance in format double.
//============================================================================//
double EXTIO_GetDistGPS(void)
{
    double dDist_GPS;

    if (g_SysInfo.tVehicleState.bACC_ON == FALSE)
        return 0;

    if ((g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS) &&
        (g_Setting.tSpeedSet.u8Auto_Gain_Count >= AUTO_GAIN_MAX_COUNT))
    {
        if (g_Setting.tSpeedSet.u8Auto_Gain_Count != 128)
        {
            // DataBuild_SKYEYES(SOCKET_BANK_DEF, STR_EVENT_AUTO_GAIN_OK, BufStr);

            g_Setting.tSpeedSet.u8Auto_Gain_Count = AUTO_GAIN_OK_COUNT;

            if ((fAutoGainValue > 10) && (fAutoGainValue < 1000))
            {
                g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
                g_SysInfo.tVehicleState.u32DistCount = 0;
                g_Setting.tSpeedSet.fGain = fAutoGainValue;

                bIsSaveSettingFlag = TRUE;
            }
            else
            {
                fNgGainValue = fAutoGainValue;
                bAGainNgFlag = TRUE;
            }

            bRS_FirstDetection = TRUE;
            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

            // In the interrupt don't printf any thing.
            // RTOSprintf("\r\n> ### AUTO GAIN OK, Gain: %f ###\r\n", g_Setting.tSpeedSet.fGain);
        }
        return 0;
    }

    // if(g_SysInfo.tVehicleState.dDistance_GPS >= AUTO_GAIN_MAX_DIST)
    if ((g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS) &&
        (g_SysInfo.tVehicleState.dDistance_GPS >= g_Setting.u16AG_Dist_Limit_Temp))
    {
        if ((g_Setting.tSpeedSet.u8Auto_Gain_Count < AUTO_GAIN_MAX_COUNT) && (g_SysInfo.tVehicleState.bACC_ON))
        {
            if (g_Setting.tSpeedSet.u8Auto_Gain_Count == (AUTO_GAIN_MAX_COUNT - 1))
                g_Setting.tSpeedSet.u8Auto_Gain_Count = AUTO_GAIN_OK_COUNT;

            u32FinalPulseCnt = g_SysInfo.tVehicleState.u32DistCount_AGain;
            u32FinalDist = (int)g_SysInfo.tVehicleState.dDistance_GPS;

            // fAutoGain unit M/Hz*1000 = MM/Hz
            if (g_SysInfo.tVehicleState.u32DistCount_AGain > 0)
            {
                g_Setting.tSpeedSet.fAutoGain = (((g_SysInfo.tVehicleState.dDistance_GPS) /
                                                  g_SysInfo.tVehicleState.u32DistCount_AGain) *
                                                 1000);
            }
            else
            {
                g_Setting.tSpeedSet.fAutoGain = 0;
            }

            // Event Auto gain ok
            //  In the interrupt don't printf any thing.
            // RTOSprintf("\r\n> ### SYS_STATE_AUTO_GAIN_OK, Cnt: %d ###", g_Setting.tSpeedSet.u8Auto_Gain_Count);

            /*
                        g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
                        g_SysInfo.tVehicleState.u32DistCount = 0;

                        if(g_Setting.tSpeedSet.fGain == 0)
                        {
                            g_Setting.tSpeedSet.fGain = g_Setting.tSpeedSet.fAutoGain;
                        }
                        else
                        {
                            g_Setting.tSpeedSet.fGain = (g_Setting.tSpeedSet.fGain + g_Setting.tSpeedSet.fAutoGain) / 2;
                        }

                        bRS_FirstDetection = TRUE;
            */

            if (fAutoGainValue == 0)
            {
                fAutoGainValue = g_Setting.tSpeedSet.fAutoGain;
            }
            else
            {
                fAutoGainValue = (fAutoGainValue + g_Setting.tSpeedSet.fAutoGain) / 2;
            }

            bIsSaveSettingFlag = TRUE;
        }

        g_Setting.tSpeedSet.u8Auto_Gain_Count++;

        g_SysInfo.tVehicleState.u32DistCount_AGain = 0;
        g_SysInfo.tVehicleState.dDistBase_GPS = 0;
        g_SysInfo.tVehicleState.dDistance_GPS = 0;

        dDist_GPS = 0;

        bIsShowAGMsgFlag = FALSE;

        return 0;
    }
    else
    {
        // g_Setting.tSpeedSet.fAutoGain = 0;

        // if((g_SysInfo.tVehicleState.u32Speed_GPS >= (g_Setting.tSpeedSet.u16Star * 100)) &&
        //	 (g_SysInfo.tGPS_Data.bStatus == TRUE) && (g_SysInfo.tSpeedInfo.u16Speed_Hz > 0))
        if ((g_SysInfo.tVehicleState.u32Speed_GPS >= (30 * 1000)) &&
            (g_SysInfo.tGPS_Data.bStatus == TRUE) && (g_SysInfo.tSpeedInfo.u16Speed_Hz > 0))
        {
            dDist_GPS = (float)g_SysInfo.tVehicleState.u32Speed_GPS / (S_2_mS(3600) / 1000);

            g_SysInfo.tVehicleState.dDistBase_GPS += dDist_GPS;

            // return g_SysInfo.tVehicleState.dDistBase_GPS;
        }
        // else
        //{
        //	g_SysInfo.tVehicleState.dDistBase_GPS = 0;
        //	dDist_GPS = 0;
        //	return 0;
        // }

        return g_SysInfo.tVehicleState.dDistBase_GPS;
    }
}

//============================================================================//
// @brief  EXTIO check GAIN value.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_CheckGAIN(void)
{
    static uint32_t u32ActSpeedTotal = 0,
                    u32GpsSpeedTotal = 0;
    int iActSpeedAvg, iGpsSpeedAvg;
    static uint16_t u16AddCnt = 0;
    const uint16_t CHK_GAIN_MIN = 20, CHK_GAIN_MAX = 1000;
    const uint32_t CHK_GPS_SPD_MIN = 50000, CHK_GPS_SPD_MAX = 90000;
    const int SPD_CNT_TOTAL = 240;
    int iSpdDiff = 0;
    char cMsg[80] = {0};
    static uint32_t u32DebugTick = 0;

    if (g_Setting.tSpeedSet.bGAIN_ChkDisable == TRUE)
        return;

    if (bChkGAIN_Step1 == TRUE)
    {
        sprintf(cMsg, "CHK GAIN-1 BEGIN");

        EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

        bChkGAIN_Step1 = FALSE;

        // Only check GAIN if use speed pulse
        if ((g_Setting.tENABLE_SET.bits.CAN_SPEED == FALSE) &&
            (g_Setting.tSpeedSet.u8MergeSpeed == 0))
        {
            if ((g_Setting.tSpeedSet.fGain < CHK_GAIN_MIN) ||
                (g_Setting.tSpeedSet.fGain > CHK_GAIN_MAX))
            {
                RTOSprintf("\r\n> Incorrect speed GAIN value detected: %.2f", g_Setting.tSpeedSet.fGain);

                memset(cMsg, 0x00, sizeof(cMsg));

                sprintf(cMsg, "CHK GAIN-1 RES ERR (%.2f)", g_Setting.tSpeedSet.fGain);

                EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

                if ((g_Setting.tSpeedSet.u8Auto_Gain_Count == AUTO_GAIN_OK_COUNT) &&
                    (g_Setting.tSpeedSet.u8AutoGainInit == 0))
                {
                    memset(cMsg, 0x00, sizeof(cMsg));

                    sprintf(cMsg, "%.2f", g_Setting.tSpeedSet.fGain);

                    g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
                    g_SysInfo.tVehicleState.u32DistCount = 0;

                    g_Setting.tSpeedSet.fGain = 0;

                    RTOSprintf("\r\n> ### AUTOGAIN Request Start ###");

                    g_Setting.u16AG_Dist_Limit_Temp = 512;
                    g_Setting.tSpeedSet.u8AutoGainInit = 1;

                    System_SaveSetting();

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_ON, cMsg);
#endif
                }
            }
            else
            {
                memset(cMsg, 0x00, sizeof(cMsg));

                sprintf(cMsg, "CHK GAIN-2 BEGIN");

                EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

                u32ActSpeedTotal = 0;
                u32GpsSpeedTotal = 0;
                u16AddCnt = 0;

                bChkGAIN_Step2 = TRUE;
            }
        }
    }

    if (bChkGAIN_Step2 == TRUE)
    {
        if ((g_SysInfo.tGPS_Data.bStatus == TRUE) &&
            (g_SysInfo.tGPS_Data.u32Speed > CHK_GPS_SPD_MIN) && (g_SysInfo.tGPS_Data.u32Speed < CHK_GPS_SPD_MAX))
        {
            u32ActSpeedTotal += g_SysInfo.tSpeedInfo.u32RealSpeed;
            u32GpsSpeedTotal += g_SysInfo.tGPS_Data.u32Speed;
            u16AddCnt++;

            if (HAL_GetTick() > (u32DebugTick + S_2_mS(20)))
            {
                u32DebugTick = HAL_GetTick();

                memset(cMsg, 0x00, sizeof(cMsg));

                sprintf(cMsg, "CHK GAIN... (%d-%d-%d)",
                        u32ActSpeedTotal,
                        u32GpsSpeedTotal,
                        u16AddCnt);

                EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));
            }

            if (u16AddCnt >= SPD_CNT_TOTAL)
            {
                // Only check GAIN if use speed pulse
                if ((g_Setting.tENABLE_SET.bits.CAN_SPEED == FALSE) &&
                    (g_Setting.tSpeedSet.u8MergeSpeed == 0))
                {
                    iActSpeedAvg = u32ActSpeedTotal / u16AddCnt;
                    iGpsSpeedAvg = u32GpsSpeedTotal / u16AddCnt;
                    iSpdDiff = abs(iActSpeedAvg - iGpsSpeedAvg) * 100 / iGpsSpeedAvg;

                    if ((iActSpeedAvg >= 100000) ||
                        ((iActSpeedAvg > 0) &&
                         (iSpdDiff >= 10) && (iSpdDiff <= 500)))
                    {
                        RTOSprintf("\r\n> ### INCORRECT SPEED GAIN DETECTED ###");
                        RTOSprintf("\r\n> Speed GAIN: %.2f, Real Speed Avg: %d, GPS Speed Avg: %d",
                                   g_Setting.tSpeedSet.fGain,
                                   iActSpeedAvg,
                                   iGpsSpeedAvg);

                        memset(cMsg, 0x00, sizeof(cMsg));

                        sprintf(cMsg, "CHK GAIN-2 RES ERR (%.2f-%d-%d-%d)",
                                g_Setting.tSpeedSet.fGain,
                                iActSpeedAvg,
                                iGpsSpeedAvg,
                                iSpdDiff);

                        EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

                        if ((g_Setting.tSpeedSet.u8Auto_Gain_Count == AUTO_GAIN_OK_COUNT) &&
                            (g_Setting.tSpeedSet.u8AutoGainInit == 0))
                        {
                            memset(cMsg, 0x00, sizeof(cMsg));

                            sprintf(cMsg, "%.2f-%d-%d",
                                    g_Setting.tSpeedSet.fGain,
                                    iActSpeedAvg / 1000,
                                    iGpsSpeedAvg / 1000);

                            RTOSprintf("\r\n> ### AUTOGAIN Request Start ###");

                            // ttAG_DelayBegin = HAL_GetTick() + S_2_mS(60);

                            g_Setting.u16AG_Dist_Limit_Temp = 512;
                            g_Setting.tSpeedSet.u8AutoGainInit = 1;

                            System_SaveSetting();

#if ENABLE_GSM_NET
                            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_ON, cMsg);
#endif
                        }
                    }
                    else
                    {
                        RTOSprintf("\r\n> ### CHECK SPEED GAIN OK ###");

                        memset(cMsg, 0x00, sizeof(cMsg));

                        sprintf(cMsg, "CHK GAIN OK (%d-%d)", iActSpeedAvg, iSpdDiff);

                        EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

                        memset(cMsg, 0x00, sizeof(cMsg));

                        sprintf(cMsg, "%.2f-%d-%d",
                                g_Setting.tSpeedSet.fGain,
                                iActSpeedAvg / 1000,
                                iGpsSpeedAvg / 1000);

#if ENABLE_GSM_NET
                        GSM_TCP_ReportEvent((char *)EVENT_CHKGAIN_OK, cMsg);
#endif
                    }
                }

                u32ActSpeedTotal = 0;
                u32GpsSpeedTotal = 0;
                u16AddCnt = 0;
                bChkGAIN_Step2 = FALSE;
            }
        }

        // u32AG_ReStartDelayTick = HAL_GetTick();
    }
}

//============================================================================//
// @brief  Check to init the autogain process.
// @param  None.
// @retval None.
//============================================================================//
static void EXTIO_AutoGainInitChk(void)
{
    if ((g_Setting.tSpeedSet.u8AutoGainInit) &&
        (HAL_GetTick() > (ttAG_DelayBegin + S_2_mS(300))) &&
        EXTIO_IsMainPowerNormal())
    {
        g_Setting.tSpeedSet.u8AutoGainInit = 0;

        // Only activate autogain if it is currently inactive
        // if(g_Setting.tSpeedSet.u8Auto_Gain_Count == AUTO_GAIN_OK_COUNT)
        {
            if (bCAN_GetDist == TRUE)
            {
                g_Setting.tSpeedSet.u8AutoGainSrc = AUTOGAIN_SOURCE_CAN;
            }
            else
            {
                g_Setting.tSpeedSet.u8AutoGainSrc = AUTOGAIN_SOURCE_GPS;
            }

            fAutoGainValue = 0;

            g_Setting.tSpeedSet.u8Auto_Gain_Count = 0;
            g_Setting.tSpeedSet.fAutoGain = 0;
            g_Setting.tSpeedSet.ttAutoGainTimeOut = System_GetTime() + DAY_2_SECOND(7);

            g_SysInfo.tVehicleState.dDistBase_GPS = 0;
            g_SysInfo.tVehicleState.dDistance_GPS = 0;
            g_SysInfo.tVehicleState.dDistBase_CAN = 0;
            g_SysInfo.tVehicleState.dDistance_CAN = 0;
            g_SysInfo.tVehicleState.u32DistCount_AGain = 0;
            dDistanceCAN_Old = 0;
            u32DistCntAG_Old = 0;
            u8DistBaseCAN_Init = 0;

            bIsShowAGMsgFlag = FALSE;

            bChkGAIN_Step1 = FALSE;
            bChkGAIN_Step2 = FALSE;

            RTOSprintf("\r\n> AUTOGAIN BEGIN, AG SOURCE: %d", g_Setting.tSpeedSet.u8AutoGainSrc);

            char cMsg[20] = {0};

            sprintf(cMsg, "AGAIN start");

            EXTIO_AGAIN_DebugLog(cMsg, strlen(cMsg));

            // GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_ON, STR_NULL);
        }

        System_SaveSetting();
    }
}

//============================================================================//
// @brief  EXTIO check speed process.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_CheckSpeedProc(void)
{
    static uint16_t u16TimeCnt = 0, u16TimeCnt2 = 0;
    static bool bDoOneFlag = FALSE;
    uint32_t u32Buf = 0;

    // RTOSprintf("\r\n ### EXTIO_CheckSpeedProc Cnt1: %d, Cnt2: %d, ###", u16TimeCnt, u16TimeCnt2);
#if 0		
	// Event 601
	if((u32RealSpeedTemp == 0) && (g_SysInfo.tVehicleState.u32Speed_GPS > 0))
	{
		u16TimeCnt++;
		
		if(u16TimeCnt > REALSPEED_LOSE_COUNT)
		{
#if ENABLE_GSM_NET
			RTOSprintf("\r\n ### Send Event 601 ###");
			GSM_TCP_ReportEvent((char *)EVENT_REALSPEED_LOSE, STR_NULL);
#endif
			
			u16TimeCnt = 0;
		}
	}
	else
	{
		u16TimeCnt = 0;
	}
#endif

    // Event 601: real Vehicle speed disappear detect
    if (u32RealSpeedTemp == 0)
    {
        if (g_SysInfo.tVehicleState.u32Speed_GPS > 0)
        {
            u16TimeCnt++;

            if (u16TimeCnt > REALSPEED_LOSE_COUNT)
            {
#if ENABLE_GSM_NET
                RTOSprintf("\r\n ### Send Event 601 ###");
                GSM_TCP_ReportEvent((char *)EVENT_REALSPEED_LOSE, STR_NULL);
#endif

                u16TimeCnt = 0;
            }
        }
        else /// When Vehicle stop, count freezed.
            u16TimeCnt = u16TimeCnt;
    }
    else
    {
        u16TimeCnt = 0;
    }

    // Event 602
    if (g_SysInfo.tGPS_Data.bStatus == TRUE)
    {
        if (u32RealSpeedTemp > g_SysInfo.tVehicleState.u32Speed_GPS)
        {
            u32Buf = u32RealSpeedTemp - g_SysInfo.tVehicleState.u32Speed_GPS;
        }
        else
        {
            u32Buf = g_SysInfo.tVehicleState.u32Speed_GPS - u32RealSpeedTemp;
        }

        if (u32Buf > 10000)
        {
            u16TimeCnt2++;

            if (u16TimeCnt2 > SPEED_ABNORMAL_COUNT)
            {
                if (bDoOneFlag == FALSE)
                {
#if ENABLE_GSM_NET
                    RTOSprintf("\r\n ### Send Event 602 ###");
                    GSM_TCP_ReportEvent((char *)EVENT_SPEED_ABNORMAL, STR_NULL);
#endif

                    bDoOneFlag = TRUE;
                }

                u16TimeCnt2 = 0;
            }
        }
        else
        {
            u16TimeCnt2 = 0;
        }
    }
    else
    {
        u16TimeCnt2 = 0;
    }

    // RTOSprintf("\r\n ### RS: %d, GS: %d, Diff: %d ###", u32RealSpeedTemp, g_SysInfo.tVehicleState.u32Speed_GPS, u32Buf);
}

//============================================================================//
// @brief  EXTIO process.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_Proc(void)
{
    // static uint8_t u8BzCnt = 0;
    static uint32_t u32MyTick = 0;
#if ENABLE_LSM6DSL
    uint32_t u32GetGSenTick = 0;
#endif
// static uint32_t u32MyTick2 = 0;
#if ENABLE_GS_MC3210
    static uint32_t u32MyTick3 = 0;
#endif

    if (HAL_GetTick() > (u32MyTick + 100))
    {
        // EXTprintf("\r\n> ### T: %d ms ###", HAL_GetTick() - u32MyTick);

        u32MyTick = HAL_GetTick();

        // EXTIO Process
        EXTIO_Detect_Proc();
    }

    if (bSD_ReportErrFlag == TRUE)
        return;

    taskInfo[TASK_EXTIO_PROC].u16TaskStep++;

    if (bSecProcFlag == TRUE)
    {
        bSecProcFlag = FALSE;

        if (EXTIO_IsMainPowerNormal() == TRUE)
        {
            // #if ENABLE_GSM_NET
            EXTIO_SpeedRPM_EventProc();
            // #endif

            // Do one sec task here
            EXTIO_OneSecTaskProc();

            EXTIO_AutoGainInitChk();

            EXTIO_AutoGainProc();

            EXTIO_CheckSpeedProc();

            EXTIO_CheckGAIN();
        }
    }

    taskInfo[TASK_EXTIO_PROC].u16TaskStep++;

    if (EXTIO_IsMainPowerNormal() == TRUE)
    {
        EXTIO_SpeedLimitProc();

        taskInfo[TASK_EXTIO_PROC].u16TaskStep++;

        if (bHalfSecFlag == TRUE)
        {
            bHalfSecFlag = FALSE;

            // Do 500ms task here
            EXTIO_HalfSecTaskProc();
        }

        taskInfo[TASK_EXTIO_PROC].u16TaskStep++;

#if ENABLE_GS_MC3210
        if (HAL_GetTick() > (u32MyTick3 + 50))
        {
            u32MyTick3 = HAL_GetTick();

            MC3210_Process();
        }
#endif
#if ENABLE_LSM6DSL
        if (HAL_GetTick() > (u32GetGSenTick + LSM6DSL_READ_INTERVAL))
        {
            u32GetGSenTick = HAL_GetTick();
            LSM6DSL_Proc();
        }
#endif
        taskInfo[TASK_EXTIO_PROC].u16TaskStep++;

#if ENABLE_CAN == 1
        // if(CAN_CheckEnable())
        {
            CAN_Proc();
        }
#endif
    }
}

//============================================================================//
// @brief  Check if the system main power is in normal state.
// @param  None.
// @retval TRUE: The system power is in normal state.
//         FALSE: The system power is not stable or is running on battery.
//============================================================================//
bool EXTIO_IsMainPowerNormal(void)
{
    if ((g_SysInfo.tVehicleState.bACC_ON == 1) &&
        (g_SysInfo.tVehicleState.bPWR_ON == 1))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Check if the system backup power is in normal state.
// @param  None.
// @retval TRUE: The system power is in normal state.
//         FALSE: The system power is not stable or is running on battery.
//============================================================================//
bool EXTIO_IsBkpPowerNormal(void)
{
    if (bBackupPwrStable == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Check if the system power is in normal state.
// @param  None.
// @retval TRUE: The system power is in normal state.
//         FALSE: The system power is not stable or is running on battery.
//============================================================================//
bool EXTIO_IsPowerNormal(void)
{
    // if((EXTIO_IsMainPowerNormal() == TRUE) ||
    //	 ((EXTIO_IsMainPowerNormal() == FALSE) && (EXTIO_IsBkpPowerNormal() == TRUE)))
    if ((g_SysInfo.tVehicleState.bPWR_ON == 1) ||
        ((g_SysInfo.tVehicleState.bPWR_ON == 0) && (EXTIO_IsBkpPowerNormal() == TRUE)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if 1
//============================================================================//
// @brief  Correct real speed in case of speed surge.
// @param  u32Speed: Original real speed in uint32_t format, uint: m/h
// @retval Corrected real speed in format uint32_t, uint: m/h
//============================================================================//
static uint32_t EXTIO_SpeedCorrection(uint32_t u32Speed)
{
    static int iSpeedBuf[3] = {0};
    int iMeanSpeed;
    int u32Threshold1, u32Threshold2;

    iSpeedBuf[0] = iSpeedBuf[1];
    iSpeedBuf[1] = iSpeedBuf[2];
    iSpeedBuf[2] = u32Speed;

    if (u8SpdFilDelayCnt > 0)
    {
        u8SpdFilDelayCnt--;

        return (uint32_t)iSpeedBuf[1];
    }

    iMeanSpeed = (iSpeedBuf[0] + iSpeedBuf[2]) / 2;

    if (iSpeedBuf[2] > 15000)
    {
        u32Threshold1 = 30000;
        u32Threshold2 = 10000;
    }
    else
    {
        u32Threshold1 = 15000;
        u32Threshold2 = 5000;
    }

    if (abs(iSpeedBuf[2] - iSpeedBuf[0]) > u32Threshold1)
        return (uint32_t)iSpeedBuf[1];

    if (abs(iSpeedBuf[1] - iMeanSpeed) > u32Threshold2)
        iSpeedBuf[1] = iMeanSpeed;

    return (uint32_t)iSpeedBuf[1];
}
#else
//============================================================================//
// @brief  Correct real speed in case of speed surge.
// @param  u32Speed: Original real speed in uint32_t format, uint: m/h
// @retval Corrected real speed in format uint32_t, uint: m/h
//============================================================================//
static uint32_t EXTIO_SpeedCorrection(uint32_t u32Speed)
{
    static int iCurrSpeed, iOldSpeed = 0, iCurrSpeedRes = 0;
    int iSpeedDiff;
    static Real_SpeedStateTypeDef speedState = REAL_SPEED_NORMAL;
    static float fAvgData = 0.0;
    const float fAvgRatio = 0.35;
    static uint32_t u32PeakCnt = 0;
    static bool bPeakFlag = FALSE;
    int iRandNum = 0;

    iCurrSpeed = u32Speed;

    iRandNum = (rand() % 6 - 3) * 1000;

    if (bRS_FirstDetection == FALSE)
    {
        iSpeedDiff = iCurrSpeed - iOldSpeed;
    }
    else
    {
        iSpeedDiff = 0;
    }

    iOldSpeed = iCurrSpeed;

    if (bPeakFlag)
    {
        if ((iCurrSpeed < (fAvgData + 10000)) || (u32PeakCnt > GPS_STATE_LOCK_CNT) ||
            (bRS_FirstDetection == TRUE))
        {
            speedState = REAL_SPEED_PEAK_END;
        }
        else
        {
            speedState = REAL_SPEED_PEAK_GOING;
        }
    }
    else
    {
        if (iSpeedDiff > 18000)
        {
            speedState = REAL_SPEED_PEAK_BEGIN;
        }
        else if (iCurrSpeed >= 150000)
        {
            speedState = REAL_SPEED_OVER_TH;
        }
        else
        {
            speedState = REAL_SPEED_NORMAL;
        }
    }

    switch ((int)speedState)
    {
    case REAL_SPEED_NORMAL:
        iCurrSpeedRes = iCurrSpeed;

        fAvgData = (1 - fAvgRatio) * fAvgData + fAvgRatio * iCurrSpeed;

        break;

    case REAL_SPEED_PEAK_BEGIN:
        u32PeakCnt++;

        bPeakFlag = TRUE;

        iCurrSpeedRes = (int)fAvgData;

        // RTOSprintf("\r\n> ### REAL SPEED PEAK EVENT BEGIN, DIFF: %d (KM/H) ###", M_2_KM(iSpeedDiff));

        break;

    case REAL_SPEED_PEAK_GOING:
        speedState = REAL_SPEED_PEAK_GOING;

        u32PeakCnt++;

        iCurrSpeedRes = fAvgData + iRandNum;

        if (iCurrSpeedRes < 0)
            iCurrSpeedRes = 0;

        break;

    case REAL_SPEED_PEAK_END:
        u32PeakCnt = 0;

        bPeakFlag = FALSE;

        iCurrSpeedRes = iCurrSpeed;

        // RTOSprintf("\r\n> ### REAL SPEED PEAK EVENT END ###");

        break;

    case GPS_SPEED_OVER_TH:

        // iCurrSpeedRes = iOldSpeed;
        iCurrSpeedRes = fAvgData + iRandNum;

        break;

    default:
        break;
    }

    if (bRS_FirstDetection == TRUE)
    {
        bRS_FirstDetection = FALSE;
    }

    return (uint32_t)iCurrSpeedRes;
}
#endif

//============================================================================//
// @brief  Callback function for EXTIO timer.
// @param  None.
// @retval None.
//============================================================================//
void EXTIO_CallBackHandle(void)
{
    static double dDistBk;
    uint32_t u32RPM, u32Speed, u32SpeedGPS_Buf = 0, u32Buf = 0;
    double dDIST;
    bool bMergeSpeed = FALSE;

    // Get vehicle speed
    if (uiSPD_TickIdx < 2000)
    {
        // RTOSprintf("JAMES:TEST HERE");
        g_SysInfo.tSpeedInfo.u16Speed_Hz = uiSPD_TickIdx;
    }

    uiSPD_TickIdx = 0;

    g_SysInfo.tVehicleState.u32Speed_GPS = GPS_GetSpeed(); // m/s
    // g_SysInfo.tVehicleState.u32Speed_GPS = 116 * 1000;

    // If GPSspeed < 3Km/h -> Set GPSspeed to 0Km/h
    if (g_SysInfo.tVehicleState.u32Speed_GPS <= 3000)
    {
        g_SysInfo.tVehicleState.u32Speed_GPS = 0;
    }

    if (g_Setting.tSpeedSet.u8Auto_Gain_Count != 128)
    {
        u32SpeedGPS_Buf = g_Setting.tSpeedSet.u8Offset * 100;

        g_SysInfo.tVehicleState.u32Speed_GPS = g_SysInfo.tVehicleState.u32Speed_GPS + u32SpeedGPS_Buf;

        if (g_SysInfo.tVehicleState.u32Speed_GPS == u32SpeedGPS_Buf)
            g_SysInfo.tVehicleState.u32Speed_GPS = 0;
    }

    if (g_Setting.tENABLE_SET.bits.CAN_SPEED == TRUE)
    {
        u32Buf = CAN_GetSpeed(); // m/s

        u32RealSpeedTemp = u32Buf;
    }
    else
    {
        // If fGain = 0 use GPS speed
        if (g_Setting.tSpeedSet.fGain == 0)
        {
            u32Buf = g_SysInfo.tVehicleState.u32Speed_GPS;

            bMergeSpeed = TRUE;
        }
        else
        {
            u32Buf = (float)MM_2_M(g_SysInfo.tSpeedInfo.u16Speed_Hz * HOUR_2_SECOND(1) * g_Setting.tSpeedSet.fGain);

            // u32Speed = EXTIO_SpeedCorrection(u32Speed);

            u32RealSpeedTemp = u32Buf;
        }
    }

    // if(g_SysInfo.tVehicleState.bACC_ON == FALSE)
    //{
    //	u32Speed = 0;
    //	g_SysInfo.tVehicleState.u32Speed_GPS = 0;
    //	g_SysInfo.tVehicleState.u32Speed = 0;
    // }

    u32Speed = EXTIO_SpeedCorrection(u32Buf); // m/s

    // Select merge GPS speed
    if (g_Setting.tSpeedSet.u8MergeSpeed == 1)
    {
        // If CAN or Real Speed is zero, but Gps Speed not zero, Use Gps Speed
        if ((u32Speed == 0) && (g_SysInfo.tVehicleState.u32Speed_GPS > 0))
        {
            u32Speed = g_SysInfo.tVehicleState.u32Speed_GPS;

            bMergeSpeed = TRUE;
        }
    }

    g_SysInfo.tVehicleState.u32Speed = u32Speed; // m/s

    g_SysInfo.tSpeedInfo.u32RealSpeed = u32Speed; // m/s

    u16SpeedBuf1 = u16SpeedBuf2;
    u16SpeedBuf2 = u16SpeedBuf3;
    u16SpeedBuf3 = u16SpeedBuf4;
    u16SpeedBuf4 = M_2_KM(u32Speed);
    u16SpeedBuf4 = u16SpeedBuf4 * 10;

    u16_100M_SpeedAvg = ((u16SpeedBuf1 + u16SpeedBuf2 + u16SpeedBuf1) / 3);

#if CUSTOMER_TYPE == CUSTOMER_FORMOSA || CUSTOMER_TYPE == CUSTOMER_FCFC
    if (u16_100M_SpeedAvg >= 900)
        u16_100M_SpeedAvg = 900;
#endif

    // Get RPM speed
    if (g_Setting.tENABLE_SET.bits.CAN_RPM == 1)
    {
        u32RPM = CAN_GetRPM();
    }
    else
    {
        g_SysInfo.tSpeedInfo.u16RPM_Hz = uiRPM_TickIdx;

        uiRPM_TickIdx = 0;

        u32RPM = (g_SysInfo.tSpeedInfo.u16RPM_Hz * 60) / g_Setting.tRPM_Set.u16DIV;

        u32RPM += g_SysInfo.tVehicleState.u16RPM;

        u32RPM /= 2;
    }

    // if(g_SysInfo.tVehicleState.bACC_ON == FALSE)
    //{
    //   u32RPM = 0;
    //	g_SysInfo.tVehicleState.u16RPM = 0;
    // }

    g_SysInfo.tVehicleState.u16RPM = u32RPM;

    // If CAN rpm Enable, and rpm is zero, speed set zero
    // if((g_Setting.tENABLE_SET.bits.CAN_RPM == 1) && (g_SysInfo.tVehicleState.u16RPM == 0))
    //{
    //	g_SysInfo.tVehicleState.u32Speed = 0;
    //	g_SysInfo.tSpeedInfo.u32RealSpeed = 0;
    //}

    // Get travelling distance
    if (g_Setting.tENABLE_SET.bits.CAN_DIST == TRUE)
    {
        g_SysInfo.tVehicleState.dDistance = CAN_GetDist(); // unit: m
    }
    else
    {
        if ((g_Setting.tSpeedSet.fGain == 0) ||
            (g_Setting.tENABLE_SET.bits.CAN_SPEED == 1) ||
            (bMergeSpeed == TRUE))
        {
            dDIST = (float)g_SysInfo.tVehicleState.u32Speed / (S_2_mS(HOUR_2_SECOND(1)) / 1000);

            g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistBase + dDIST;

            g_SysInfo.tVehicleState.dDistance = g_SysInfo.tVehicleState.dDistBase; // unit: m

            // In the interrupt don't printf any thing.
            // printf("\r\n> dDIST: %f", dDIST);
        }
        else
        {
            dDIST = (float)MM_2_M(g_SysInfo.tVehicleState.u32DistCount * g_Setting.tSpeedSet.fGain);

            g_SysInfo.tVehicleState.dDistance = g_SysInfo.tVehicleState.dDistBase + dDIST; // unit: m

            // In the interrupt don't printf any thing.
            // printf("\r\n> dDIST: %f, Base: %f, dDDD: %f", g_SysInfo.tVehicleState.dDistance, g_SysInfo.tVehicleState.dDistBase, dDIST);
        }
    }

    // In the interrupt don't printf any thing.
    // printf("\r\n> dDistance: %f", (g_SysInfo.VehicleState.dDistance - dDistBk));

    g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].dDriverDistance += (g_SysInfo.tVehicleState.dDistance - dDistBk); // unit: m

    dDistBk = g_SysInfo.tVehicleState.dDistance; // unit: m

    g_SysInfo.tVehicleState.dDistance_GPS = EXTIO_GetDistGPS(); // unit: m
}
