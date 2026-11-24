//============================================================================//
// Includes
//============================================================================//
#include "DataCollect.h"

#include <string.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "Buzzer.h"
#include "CAN_Driver.h"
#include "Default.h"
#include "EXTIO.h"
#include "GPS_Driver.h"
#include "GSM_Driver.h"
#include "LCD_Key.h"
#if ENABLE_MG82_RTC
#include "MG82_Driver.h"
#elif ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "SD_Module.h"

#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#endif
//============================================================================//
// User Defines
//============================================================================//
#define OVER_SPEED_COUNT (S_2_mS(30) / TIMER_GET_INFO)

#define OVER_SPEED_OFFSET KM_2_M(1)

#define OVER_RPM_COUNT (S_2_mS(30) / TIMER_GET_INFO)

#define OVER_RPM_OFFSET 100

#if CUSTOMER_TYPE == CUSTOMER_CPTI
#undef OVER_SPEED_COUNT
#define OVER_SPEED_COUNT (S_2_mS(15) / TIMER_GET_INFO)

#undef OVER_RPM_COUNT
#define OVER_RPM_COUNT (S_2_mS(15) / TIMER_GET_INFO)
#endif

//============================================================================//
// User Variables
//============================================================================//
typedef enum
{
    SPEED_LIMIT_L0,
    SPEED_LIMIT_L1,
    SPEED_LIMIT_L2,
    SPEED_LIMIT_L3

} SPEED_LIMIT_t;

//============================================================================//
// User Variables
//============================================================================//
static char szBuff[UART_BASE_BUFFER_SIZE];

uint16_t u16SpeedBuf1 = 0;
uint16_t u16SpeedBuf2 = 0;
uint16_t u16SpeedBuf3 = 0;
uint16_t u16SpeedBuf4 = 0;
uint16_t u16_100M_SpeedAvg = 0;

//============================================================================//
// User Functions
//============================================================================//
#if 0
//============================================================================//
// @brief  Perform logout for driver.
// @param  None.
// @retval None.
//============================================================================//
static void Info_DrvLogout(void)
{
	sprintf(szBuff, "%08X", g_Setting.u32DrvIdNum);

	GSM_TCP_ReportEvent(STR_EVENT_DRIVER_ID0, szBuff);

	g_Setting.u32DrvIdNum = INVALID_VALUE_DEF;
}

//============================================================================//
// @brief  Check driver ID.
// @param  DrvId: Driver ID.
// @retval TRUE: Driver ID is enabled and correct.
//         FALSE: Driver ID is disabled or incorrect.
//============================================================================//
bool Info_CheckDrvId(uint32_t DrvId)
{
	if(g_Setting.tENABLE_SET.bits.DRV_ID == 0)
		return FALSE;

	if(DrvId == INVALID_VALUE_DEF)
		return FALSE;

	// �Y�r�p���w login �B���ƿ�J���ѽX, �r�p�� logout
	if(DrvId == g_Setting.u32DrvIdNum)
	{
		Info_DrvLogout();

		return TRUE;
	}

	// �Y�w�g���r�p�� login , �N���e�r�p�� logout
	if(g_Setting.u32DrvIdNum != INVALID_VALUE_DEF)
	{
		Info_DrvLogout();
	}

	g_Setting.u32DrvIdNum = DrvId;

	sprintf(szBuff, "%08X", g_Setting.u32DrvIdNum);

	GSM_TCP_ReportEvent(STR_EVENT_DRIVER_ID1, szBuff);

	Buzzer_Beep(BEEP_SHORT, 3);

	return TRUE;
}
#endif

//============================================================================//
// @brief  Check driver ID 1.
// @param  DrvId: Driver ID.
// @retval TRUE: Driver ID is enabled and correct.
//         FALSE: Driver ID is disabled or incorrect.
//============================================================================//
bool Info_CheckDrvId(uint32_t DrvId)
{
    if (g_Setting.tENABLE_SET.bits.DRV_ID == 0)
        return FALSE;

    if (DrvId == INVALID_VALUE_DEF)
        return FALSE;

    // �Y�r�p���w login �B���ƿ�J���ѽX, ���_�ʧ@
    if (DrvId == g_Setting.tDrvInfo[0].u32DrvIdNum)
        return TRUE;

    g_Setting.tDrvInfo[0].u32DrvIdNum = DrvId;

    // sprintf(szBuff, "%08X", g_Setting.u32DrvIdNum);

    // GSM_TCP_ReportEvent(STR_EVENT_DRIVER_ID0, szBuff);

    g_Setting.tDrvInfo[0].u32DrWorkTime = 0;
    g_Setting.tDrvInfo[0].u32DrRestTime = 0;
    g_Setting.tDrvInfo[0].dDriverDistance = 0;

    for (int i = 0; i < 14; i++)
    {
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32WorkTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32PauseTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32StandByTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32RestTime = 0;
    }

    if (EXTIO_IsMainPowerNormal())
    {
        System_SaveSetting();
    }

    g_Setting.tDRV_SET.ttDrvWorkStartTime = System_GetTime();

    Buzzer_Beep(BEEP_SHORT, 3);

    return TRUE;
}

//============================================================================//
// @brief  Check driver ID 2.
// @param  DrvId: Driver ID.
// @retval TRUE: Driver ID is enabled and correct.
//         FALSE: Driver ID is disabled or incorrect.
//============================================================================//
bool Info_CheckDrvId2(uint32_t DrvId)
{
    if (g_Setting.tENABLE_SET.bits.DRV_ID == 0)
        return FALSE;

    if (DrvId == INVALID_VALUE_DEF)
        return FALSE;

    // �Y�r�p���w login �B���ƿ�J``���ѽX, ���_�ʧ@
    if (DrvId == g_Setting.tDrvInfo[1].u32DrvIdNum)
        return TRUE;

    g_Setting.tDrvInfo[1].u32DrvIdNum = DrvId;

    // sprintf(szBuff, "%08X", g_Setting.u32DrvIdNum2);

    // GSM_TCP_ReportEvent(STR_EVENT_DRIVER_ID1, szBuff);

    g_Setting.tDrvInfo[1].u32DrWorkTime = 0;
    g_Setting.tDrvInfo[1].u32DrRestTime = 0;
    g_Setting.tDrvInfo[1].dDriverDistance = 0;

    for (int i = 0; i < 14; i++)
    {
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32WorkTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32PauseTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32StandByTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32RestTime = 0;
    }

    if (EXTIO_IsMainPowerNormal())
    {
        System_SaveSetting();
    }

    Buzzer_Beep(BEEP_SHORT, 3);

    return TRUE;
}

//============================================================================//
// @brief  Process driver response timeout event.
// @param  None.
// @retval TRUE/ FALSE.
//============================================================================//
bool DriverResponseTimeoutEvent_Proc(void)
{
    // Ĳ�o�ƥ� : �w�ɦ^��ĵ���Ұ�
    // GSM_TCP_ReportEvent(FPTC_EVENT_710, STR_EVENT_MSG);

    return TRUE;
}

//============================================================================//
// @brief  Check driver status.
// @param  None.
// @retval None.
//============================================================================//
void Info_ChkDrvStatus(void)
{
    // #if CUSTOMER_TYPE == CUSTOMER_FORMOSA
#if 0
	static uint8_t BID_DrvResponse = BUZZER_INVALID_ID;
	static uint8_t TID_DrvResponse = TIMER_INVALID_ID;
	char szTEMP[64];


	if(g_Setting.TempSet.DrvResponse == 0)
	{
		if((BID_DrvResponse != BUZZER_INVALID_ID) && (TID_DrvResponse != TIMER_INVALID_ID))
		{
			g_SysInfo.DrvInfo.Response = FALSE;

			Buzzer_ClrRing(&BID_DrvResponse);

			SWTIM_ClrTimer(TID_DrvResponse);

			TID_DrvResponse = TIMER_INVALID_ID;
		}

		return;
	}

	if(g_SysInfo.DrvInfo.Response == TRUE)
		return;

	if((BID_DrvResponse != BUZZER_INVALID_ID) && (TID_DrvResponse != TIMER_INVALID_ID))	// Heinz
	{
		Buzzer_ClrRing(&BID_DrvResponse);

		g_SysInfo.DrvInfo.LCDT = 0;

		SWTIM_ClrTimer(TID_DrvResponse);

		TID_DrvResponse = TIMER_INVALID_ID;

		sprintf(szTEMP, "%s�r�p�w�ɦ^������", STR_AS_LCD_MSG);

		Protocl_ChkCmdPkg(szTEMP, NULL);

		// Ĳ�o�ƥ� : �w�ɦ^��ĵ���Ѱ�
		GSM_TCP_ReportEvent(FPTC_EVENT_711, STR_EVENT_MSG);
	}

	if((g_SysInfo.DrvInfo.LCDT >= g_Setting.TempSet.DrvResponse) && (g_Setting.TempSet.DrvResponse >= 600))
	{
		g_SysInfo.DrvInfo.Response = TRUE;

		Buzzer_ClrRing(&BID_DrvResponse);

		Buzzer_SetRing(&BID_DrvResponse, BEEP_LONG, BUZZER_BEEP_CONTINUE);

		sprintf(szTEMP, "%s�оr�p�w�ɦ^��", STR_AS_LCD_MSG);

		Protocl_ChkCmdPkg(szTEMP, NULL);

		SWTIM_ClrTimer(TID_DrvResponse);

		TID_DrvResponse = TIMER_INVALID_ID;

		TID_DrvResponse = SWTIM_SetTimer(S_2_mS(MINUTE_2_SECOND(3)), TIMER_CONTINUE, DriverResponseTimeoutEvent_Proc);
	}
#endif
}

//============================================================================//
// @brief  Debug time messages.
// @param  None.
// @retval None.
//============================================================================//
static void Debug_ShowTime(void)
{
    time_t rawtime;
    char *pStr, BufStr[32];
    struct tm tmTime = {0};

    pStr = szBuff;

    sprintf(pStr, "* ");

    pStr += strlen(pStr);

    rawtime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    localtime_r(&rawtime, &tmTime);

    strftime(BufStr, sizeof(BufStr), "RTC: %Y/%m/%d %H:%M:%S | ", &tmTime);

    strcpy(pStr, BufStr);

    pStr += strlen(pStr);

    rawtime += g_Setting.i32TimeZoneOffset;

    strftime(BufStr, sizeof(BufStr), "LOCAL: %Y/%m/%d %H:%M:%S | ", &tmTime);

    strcpy(pStr, BufStr);

    pStr += strlen(pStr);

    if (GPS_GetUTC(&rawtime) == TRUE)
        strftime(BufStr, sizeof(BufStr), "UTC: %Y/%m/%d %H:%M:%S", &tmTime);
    else
        sprintf(BufStr, "UTC: N/A");

    strcpy(pStr, BufStr);

    pStr += strlen(pStr);

    sprintf(pStr, "#\r\n");

    RTOSprintf("\r\n> %s", szBuff);
}

//============================================================================//
// @brief  Debug data messages.
// @param  None.
// @retval None.
//============================================================================//
static void Debug_ShowData(void)
{
    time_t rawtime;
    char szTime[32];
    struct tm tmTime = {0};

    rawtime = System_GetTime();

    localtime_r(&rawtime, &tmTime);

    strftime(szTime, sizeof(szTime), "%Y/%m/%d %H:%M:%S", &tmTime);

#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306 // HW_JAS208 HW_JAS306
#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_DAF
    RTOSprintf("\r\n> * ID(%s),(%s),(%+09.4f),(%+010.4f),Acc(%d),Io(%d%d%d%d%d%d%d),"
               "Bat(%d%d),S(%04d),S(%04d)Hz,G-S(%04d),R(%05d),Dist(%08.0f),"
               "DAF DATA: Brake(%3s-%d%%),Door(0x%02X-%d%d%d%d%d%d%d%d%d%d),"
               "HB(0x%02X),LB(0x%02X),TS(0x%02X),AEBS(0x%02X),LDWS_EN(0x%02X),LDWS(0x%02X)#",
               g_Setting.tBusInfo.cBus_ID,                 // ID(%s)
               szTime,                                     // (%s)
               g_SysInfo.tGPS_Data.gpsLocation.dLatitude,  // (%+09.4f)
               g_SysInfo.tGPS_Data.gpsLocation.dLongitude, // (%+010.4f)
               g_SysInfo.tEXTIO.bits.IN_ACC,               // Acc(%d)
               g_SysInfo.tEXTIO.bits.IN_1,                 // Io(%d%d%d%d)
               g_SysInfo.tEXTIO.bits.IN_2,
               g_SysInfo.tEXTIO.bits.IN_3,
               g_SysInfo.tEXTIO.bits.IN_4,
               g_SysInfo.tEXTIO.bits.IN_5,
               g_SysInfo.tEXTIO.bits.IN_6,
               g_SysInfo.tEXTIO.bits.IN_7,
               g_SysInfo.tEXTIO.bits.IN_PWR, // Bat(%d%d)
               g_SysInfo.tEXTIO.bits.IN_CHG,
               M_2_100M(g_SysInfo.tSpeedInfo.u32RealSpeed), // S(%04d)
               g_SysInfo.tSpeedInfo.u16Speed_Hz,            // S(%04d)Hz
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),      // G-S(%04d)
               g_SysInfo.tVehicleState.u16RPM,              // R(%05d)
               M_2_100M(g_SysInfo.tVehicleState.dDistance)  // Dist(%08.0f)
               ,
               tCAN_DATA.tBrakeState.bBrakeState == 1 ? "ON" : "OFF", // Brake(%3s-%d%%)
               tCAN_DATA.tBrakeState.u8BrakePos,
               tCAN_DATA.tDoorControl.u8AllDoorStat, // Door(0x%02X-%d%d%d%d%d%d%d%d%d%d)
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_1,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_2,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_3,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_4,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_5,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_6,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_7,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_8,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_9,
               tCAN_DATA.tDoorControl.tDoorState.bits.DOOR_10,
               tCAN_DATA.tLightState.u8HighHeadBeam, // HB(0x%02X)
               tCAN_DATA.tLightState.u8LowHeadBeam,  // LB(0x%02X)
               tCAN_DATA.tLightState.u8TurnSignal,   // TS(0x%02X)
               tCAN_DATA.tBrakeState.u8AEBS_State,   // AEBS(0x%02X)
               tCAN_DATA.bLDWS_En,                   // LDWS_EN(0x%02X)
               tCAN_DATA.u8LDWS_Inc                  // LDWS(0x%02X)
    );
#elif ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_GDBUS
    RTOSprintf("\r\n> * ID(%s),(%s),(%+09.4f),(%+010.4f),Acc(%d),Io(%d%d%d%d%d%d%d),"
               "Bat(%d%d),S(%04d),S(%04d)Hz,G-S(%04d),R(%05d),Dist(%08.0f),"
               "BAT DATA: M%05d,S%02X,V%04d,C%04d,SOC%03d,VH%03.2f,VL%03.2f,TH%02d,TL%02d#",
               g_Setting.tBusInfo.cBus_ID,                 // ID(%s)
               szTime,                                     // (%s)
               g_SysInfo.tGPS_Data.gpsLocation.dLatitude,  // (%+09.4f)
               g_SysInfo.tGPS_Data.gpsLocation.dLongitude, // (%+010.4f)
               g_SysInfo.tEXTIO.bits.IN_ACC,               // Acc(%d)
               g_SysInfo.tEXTIO.bits.IN_1,                 // Io(%d%d%d%d)
               g_SysInfo.tEXTIO.bits.IN_2,
               g_SysInfo.tEXTIO.bits.IN_3,
               g_SysInfo.tEXTIO.bits.IN_4,
               g_SysInfo.tEXTIO.bits.IN_5,
               g_SysInfo.tEXTIO.bits.IN_6,
               g_SysInfo.tEXTIO.bits.IN_7,
               g_SysInfo.tEXTIO.bits.IN_PWR, // Bat(%d%d)
               g_SysInfo.tEXTIO.bits.IN_CHG,
               M_2_100M(g_SysInfo.tSpeedInfo.u32RealSpeed), // S(%04d)
               g_SysInfo.tSpeedInfo.u16Speed_Hz,            // S(%04d)Hz
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),      // G-S(%04d)
               g_SysInfo.tVehicleState.u16RPM,              // R(%05d)
               M_2_100M(g_SysInfo.tVehicleState.dDistance), // Dist(%08.0f)
               CAN_GetMotSpeed(),                           // M%05d
               CAN_GetBatChg(),                             // S%02X
               CAN_GetBatVoltage(),                         // V%04d
               CAN_GetBatCurrent(),                         // C%04d
               CAN_GetSOC(),                                // SOC%03d
               ((float)CAN_GetCellVolMax() / 10),           // VH%03.2f
               ((float)CAN_GetCellVolMin() / 10),           // VL%03.2f
               CAN_GetCellTmpMax(),                         // TH%02d
               CAN_GetCellTmpMin()                          // TL%02d
    );
#elif ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
    RTOSprintf("\r\n> * ID(%s),(%s),(%+09.4f),(%+010.4f),Acc(%d),Io(%d%d%d%d%d%d%d),"
               "Bat(%d%d),S(%04d),S(%04d)Hz,G-S(%04d),R(%05d),Dist(%08.0f),"
               "MBEYE DATA: L-LDW:%d,R-LDW:%d,FCW:%d,HW-LV:%d#",
               g_Setting.tBusInfo.cBus_ID,                 // ID(%s)
               szTime,                                     // (%s)
               g_SysInfo.tGPS_Data.gpsLocation.dLatitude,  // (%+09.4f)
               g_SysInfo.tGPS_Data.gpsLocation.dLongitude, // (%+010.4f)
               g_SysInfo.tEXTIO.bits.IN_ACC,               // Acc(%d)
               g_SysInfo.tEXTIO.bits.IN_1,                 // Io(%d%d%d%d)
               g_SysInfo.tEXTIO.bits.IN_2,
               g_SysInfo.tEXTIO.bits.IN_3,
               g_SysInfo.tEXTIO.bits.IN_4,
               g_SysInfo.tEXTIO.bits.IN_5,
               g_SysInfo.tEXTIO.bits.IN_6,
               g_SysInfo.tEXTIO.bits.IN_7,
               g_SysInfo.tEXTIO.bits.IN_PWR, // Bat(%d%d)
               g_SysInfo.tEXTIO.bits.IN_CHG,
               M_2_100M(g_SysInfo.tSpeedInfo.u32RealSpeed), // S(%04d)
               g_SysInfo.tSpeedInfo.u16Speed_Hz,            // S(%04d)Hz
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),      // G-S(%04d)
               g_SysInfo.tVehicleState.u16RPM,              // R(%05d)
               M_2_100M(g_SysInfo.tVehicleState.dDistance), // Dist(%08.0f)
               MBEYE_LeftLDW(),                             // L-LDW:%d
               MBEYE_RightLDW(),                            // R-LDW:%d
               MBEYE_FCWOn(),                               // FCW:%d
               MBEYE_HeadwayWarningLevel()                  // HW-LV:%d
    );
#else
    RTOSprintf("\r\n> * ID(%s),(%s),(%+09.4f),(%+010.4f),Acc(%d),Io(%d%d%d%d%d%d%d),"
               "Bat(%d%d),S(%04d),S(%04d)Hz,G-S(%04d),R(%05d),Dist(%08.0f)#",
               g_Setting.tBusInfo.cBus_ID,                 // ID(%s)
               szTime,                                     // (%s)
               g_SysInfo.tGPS_Data.gpsLocation.dLatitude,  // (%+09.4f)
               g_SysInfo.tGPS_Data.gpsLocation.dLongitude, // (%+010.4f)
               g_SysInfo.tEXTIO.bits.IN_ACC,               // Acc(%d)
               g_SysInfo.tEXTIO.bits.IN_1,                 // Io(%d%d%d%d)
               g_SysInfo.tEXTIO.bits.IN_2,
               g_SysInfo.tEXTIO.bits.IN_3,
               g_SysInfo.tEXTIO.bits.IN_4,
               g_SysInfo.tEXTIO.bits.IN_5,
               g_SysInfo.tEXTIO.bits.IN_6,
               g_SysInfo.tEXTIO.bits.IN_7,
               g_SysInfo.tEXTIO.bits.IN_PWR, // Bat(%d%d)
               g_SysInfo.tEXTIO.bits.IN_CHG,
               M_2_100M(g_SysInfo.tSpeedInfo.u32RealSpeed), // S(%04d)
               g_SysInfo.tSpeedInfo.u16Speed_Hz,            // S(%04d)Hz
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),      // G-S(%04d)
               g_SysInfo.tVehicleState.u16RPM,              // R(%05d)
               M_2_100M(g_SysInfo.tVehicleState.dDistance)  // Dist(%08.0f)
    );
#endif

    /*
    // For test
    RTOSprintf("\r\n> * ID(%s),(%s),GD-C(%d),Acc(%d),S(%04d),G-S(%04d),Dist(%08.0f),Dist_GPS(%08.0f),S(%04d)Hz,gain(%f) Hgain(%f),%d#",
                         g_Setting.szBus_ID,
                         szTime,
                         g_SysInfo.VehicleState.u32DistCount,
             g_SysInfo.EXTIO.bits.IN_ACC,
                         M_2_10M(g_SysInfo.VehicleState.u32Speed),
                         M_2_10M(g_SysInfo.tGPS_Data.u32Speed),
                         g_SysInfo.VehicleState.dDistance,
                         //M_2_100M(g_SysInfo.VehicleState.dDistance),
                         g_SysInfo.VehicleState.dDistance_GPS,
                         g_SysInfo.u16Speed_Hz,
                         //g_SysInfo.u16RPM_Hz,
                         //GAIN_Heinz
                         g_Setting.tSpeedSet.fGain,
                         g_Setting.tSpeedSet.fAutoGain,
                         g_Setting.tSpeedSet.u8Auto_Gain_Count
                        );
    */
#endif // HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
}

//============================================================================//
// @brief  Debug data messages version 2.
// @param  None.
// @retval None.
//============================================================================//
static void ENG_ShowData(void)
{
    time_t rawtime;
    char szTime[32];
    struct tm tmTime = {0};

    rawtime = System_GetTime();

    localtime_r(&rawtime, &tmTime);

    strftime(szTime, sizeof(szTime), "%Y/%m/%d %H:%M:%S", &tmTime);

    RTOSprintf("\r\n> * ID(%s),(%s),Acc(%s),S(%04d),S-Gain(%f),GPS-S(%04d),S-Can(%s),R(%05d),R-Div(%d),R-Can(%s),Speed-Limit(1:%.1f / 2:%.1f / 3:%.1f KM/H),S(%04d)Hz,R(%04d)Hz,0x%02X#",
               g_Setting.tBusInfo.cBus_ID,                                 // ID(%s)
               szTime,                                                     // (%s)
               (g_SysInfo.tEXTIO.bits.IN_ACC == TRUE) ? "1" : "0",         // Acc(%s)
               M_2_100M(g_SysInfo.tVehicleState.u32Speed),                 // S(%04d)
               g_Setting.tSpeedSet.fGain,                                  // S-Gain(%f)
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),                     // GPS-S(%04d)
               (g_Setting.tENABLE_SET.bits.CAN_SPEED == TRUE) ? "1" : "0", // S-Can(%s)
               g_SysInfo.tVehicleState.u16RPM,                             // R(%05d)
               g_Setting.tRPM_Set.u16DIV,                                  // R-Div(%d)
               (g_Setting.tENABLE_SET.bits.CAN_RPM == TRUE) ? "1" : "0",   // R-Can(%s)
               (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[0]),             // Speed-Limit(1:%.1f / 2:%.1f / 3:%.1f KM/H)
               (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[1]),
               (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[2]),
               g_SysInfo.tSpeedInfo.u16Speed_Hz, // S(%04d)Hz
               g_SysInfo.tSpeedInfo.u16RPM_Hz,   // R(%04d)Hz
               g_u8HwInfo                        // 0x%02X
    );
}

//============================================================================//
// @brief  Debug GPS messages.
// @param  None.
// @retval None.
//============================================================================//
static void Debug_ShowGps(void)
{
    RTOSprintf("\r\n> * GPS DATA: %04d/%02d/%02d %02d:%02d:%02d,%+09.4f,%+010.4f,%c,%d,%d,%d,%d,%d,S%05d,%03d#",
               g_SysInfo.tGPS_Data.utcTime.u16Year, // %04d/%02d/%02d
               g_SysInfo.tGPS_Data.utcTime.u8Month,
               g_SysInfo.tGPS_Data.utcTime.u8Day,
               g_SysInfo.tGPS_Data.utcTime.u8Hour, // %02d:%02d:%02d
               g_SysInfo.tGPS_Data.utcTime.u8Min,
               g_SysInfo.tGPS_Data.utcTime.u8Sec,
               g_SysInfo.tGPS_Data.gpsLocation.dLatitude,         // %+09.4f
               g_SysInfo.tGPS_Data.gpsLocation.dLongitude,        // %+010.4f
               (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V', // %c
               g_SysInfo.tGPS_Data.u8StlNum,                      // %d
               (int)(g_SysInfo.tGPS_Data.fPDOP),                  // %d
               (int)(g_SysInfo.tGPS_Data.fHDOP),                  // %d
               (int)(g_SysInfo.tGPS_Data.fVDOP),                  // %d
               (int)(g_SysInfo.tGPS_Data.fAltitude),              // %d
               M_2_100M(g_SysInfo.tGPS_Data.u32Speed),            // S%05d
               g_SysInfo.tGPS_Data.u16Course);                    // %03d

    /*
    RTOSprintf("\r\n> RTC IC Time: 20%02X-%02X-%02X %02X:%02X:%02X %s",
               u8RtcTime[6], u8RtcTime[5], u8RtcTime[3],  u8RtcTime[2],
               u8RtcTime[1], u8RtcTime[0], szWeeklyString[u8RtcTime[4]]);
    */
}

/*
#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_GDBUS
//============================================================================//
// @brief  Show CAN battery data.
// @param  None.
// @retval None.
//============================================================================//
static void Debug_ShowBat(void)
{
    RTOSprintf("\r\n> * BAT DATA: M%05d,S%02X,V%04d,C%04d,SOC%03d,VH%03.2f,VL%03.2f,TH%02d,TL%02d#",
                         CAN_GetMotSpeed(),
                         CAN_GetBatChg(),
                         CAN_GetBatVoltage(),
                         CAN_GetBatCurrent(),
                         CAN_GetSOC(),
                         ((float)CAN_GetCellVolMax() / 10),
                         ((float)CAN_GetCellVolMin() / 10),
                         CAN_GetCellTmpMax(),
                         CAN_GetCellTmpMin());
}
#endif

#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
static void Debug_ShowMbEye(void)
{
    RTOSprintf("\r\n> * MBEYE DATA: L-LDW:%d, R-LDW:%d, FCW:%d HW-LV:%d#",
                 MBEYE_LeftLDW(),
                 MBEYE_RightLDW(),
                 MBEYE_FCWOn(),
                 MBEYE_HeadwayWarningLevel()
              );
}
#endif
*/

#if ENABLE_LSM6DSL
static void Debug_ShowGSensor(void)
{
    uint32_t u32Speed;
    time_t rawtime;
    char szTime[32];
    struct tm tmTime = {0};

    u32Speed = GPS_GetSpeed();

    rawtime = System_GetTime();

    localtime_r(&rawtime, &tmTime);

    strftime(szTime, sizeof(szTime), "%H:%M:%S", &tmTime);

    GSprintf("\r\n> G-SENSOR Accelerometer[mg] XYZ %s: X=%.4d, Y=%.4d, Z=%.4d - Sensitive : %.4f | Gyroscope[mdps] angle PRY : P=%.4d, R=%.4d, Y=%.4d - Sensitive : %.4f | SPEED-GPS:%03d/KM-%05d-GSS%05d",
             szTime,
             tAccData.i32AccX,
             tAccData.i32AccY,
             tAccData.i32AccZ,
             tAccData.fSensitive,
             tGyroData.i32AngP,
             tGyroData.i32AngR,
             tGyroData.i32AngY,
             tGyroData.fSensitive,
             M_2_KM(u32Speed),
             M_2_100M(u32Speed),
             M_2_100M(g_SysInfo.tVehicleState.u32Speed));
    //  u16NowGSensorMete,
    //  MC321X_METE_MA,
    //  u16MeteMA,
    //  M_2_KM(u32Speed),
    //  M_2_100M(u32Speed),
    //  M_2_100M(g_SysInfo.tVehicleState.u32Speed));
}
#endif
//============================================================================//
// @brief  Handle debug process.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool Debug_Proc(void)
{
    if (UART_IsDebugMode(DBG_TIME))
        Debug_ShowTime();

    if (UART_IsDebugMode(DBG_DATA))
        Debug_ShowData();

    if (UART_IsDebugMode(DBG_GPS1))
        Debug_ShowGps();

    if (UART_IsDebugMode(DBG_ENG))
        ENG_ShowData();
#if ENABLE_LSM6DSL
    if (UART_IsDebugMode(DBG_GS))
        Debug_ShowGSensor();
#endif
    return TRUE;
}
