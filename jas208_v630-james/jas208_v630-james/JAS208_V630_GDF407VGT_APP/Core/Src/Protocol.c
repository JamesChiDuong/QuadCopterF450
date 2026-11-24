//============================================================================//
// User Includes
//============================================================================//
#include "Protocol.h"

#include <string.h>
#include <stdlib.h>
#include "stm32fxxx_hal_user.h"

#include "Buzzer.h"
#include "CAN_Driver.h"
#include "CommFunc.h"
#include "DataCollect.h"
#include "DataLog.h"
#include "Default.h"
#include "EXTIO.h"
#include "GPS_Driver.h"
#include "GSM_Driver.h"
#include "GSM_QtEC21.h"
#include "LCD_Key.h"
#include "main.h"
#include "MG82_Driver.h"
#if ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "Power.h"
#include "PRN_Driver.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"
#include "UART_Driver.h"
#include "USB_Module.h"

#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#include "math.h"
#endif
//============================================================================//
// User Defines
//============================================================================//
#define MAX_GET_PKG_TIMEOUT (500 / 100)

#define SPEED_ADJ_MAX 300
#define SPEED_TRIG_MIN 0
#define SPEED_TRIG_MAX 330
#define RPM_TRIG_MIN 0
#define RPM_TRIG_MAX 330
#define RPM_DIV_MIN 1
#define RPM_DIV_MAX 100
#define RPM_LIMIT_MIN 0
#define RPM_LIMIT_MAX 60000
#define GSM_SEND_NORMAL_MIN 10
#define GSM_SEND_NORMAL_MAX MINUTE_2_SECOND(1)
#define GSM_SEND_STANDBY_MIN 60
#define GSM_SEND_STANDBY_MAX HOUR_2_SECOND(12)
#define GSM_MIC_GAIN_MIN 0
#define GSM_MIC_GAIN_MAX 100
#define GSM_SPK_GAIN_MIN 0
#define GSM_SPK_GAIN_MAX 100
#define DRIVER_RESPONSE_MIN 0
#define DRIVER_RESPONSE_MAX HOUR_2_SECOND(4)

#define STR_CMD_OK "\r\n@ Command OK !!\r\n"
#define STR_CMD_ERR "\r\n@ Command ERROR !!\r\n"

#define ID_ADMIN "ADMIN"

//============================================================================//
// User Variables
//============================================================================//

char szCmdMsg[UART_BASE_BUFFER_SIZE];

static const char *PowerModeString[] =
    {
        "SAVE",
        "IMMO",
        "ON",
        "OFF",
        ""};

static const char *DebugModeString[] =
    {
        "OFF",
        "TIME",
        "DATA",
        "LOG",
        "GPS",
        "GPS1",
        "GPRS",
        "CAN",
        "DB9",
        "EXT",
        "GS",
        "ENG",
        "RESERVED1",
        "RESERVED2",
        "RESERVED3",
        "ALL"};

uint32_t const DebugModeValue[] =
    {
        0x00000000,
        0x00000001,
        0x00000002,
        0x00000004,
        0x00000008,
        0x00000010,
        0x00000020,
        0x00000040,
        0x00000080,
        0x00000100,
        0x00000200,
        0x00000400,
        0x00000800,
        0x00001000,
        0x00002000,
        0x00003FFF};

static const char *RecordModeString[] =
    {
        "OFF",
        "SPEED INT",
        "CAN DEBUG",
        "PULSE COUNT",
        "GPS SPEED",
        "ALL"};

uint32_t const RecordModeValue[] =
    {
        0x00000000,
        0x00000001,
        0x00000002,
        0x00000004,
        0x00000008,
        0x00000FFF};

static const char *SystemCmdString[] =
    {
        "UPGRADE NOW",
        "UPGRADE",
        "BACKUP",
        "PHOTO",
        "INFO",
        ""};

const char *szResetCmd[] =
    {
        "SYS",
        "GPS",
        "GPRS",
        "CAN",
        "LOG",
        "TEMP",
        "ALL",
        "DEFAULT",
        "SETTING"
        "UPGRADE",
        ""};

const char *szCanLinkType[] =
    {
        "AUTO DETECT",
        "SAE J1939-71, 250Kbps, 29bit",
        "ISO15765-4, 250Kbps, 11bit",
        "ISO15765-4, 250Kbps, 29bit",
        "ISO15765-4, 500Kbps, 11bit",
        "ISO15765-4, 500Kbps, 29bit",
        ""};

const uint8_t u8CarTypeIndex[] =
    {
        0U,  // 0
        1U,  // 1
        2U,  // 2
        3U,  // 3
        4U,  // 4
        5U,  // 5
        6U,  // 6
        7U,  // 7
        8U,  // 8
        9U,  // 9
        10U, // 10
        11U, // 11
        12U, // 12
        13U, // 13
        14U, // 14
        15U, // 15
        16U, // 16
        17U, // 17
        18U, // 18
        57U, // 19
        19U, // 20
        20U, // 21
        21U, // 22
        44U, // 23
        45U, // 24
        46U, // 25
        47U, // 26
        48U, // 27
        49U, // 28
        50U, // 29
        51U, // 30
        52U, // 31
        22U, // 32
        23U, // 33
        53U, // 34
        54U, // 35
        58U, // 36
        59U, // 37
        62U, // 38
        63U, // 39
        64U, // 40
        24U, // 41
        60U, // 42
        25U, // 43
        26U, // 44
        27U, // 45
        28U, // 46
        29U, // 47
        30U, // 48
        31U, // 49
        32U, // 50
        33U, // 51
        34U, // 52
        35U, // 53
        36U, // 54
        37U, // 55
        38U, // 56
        39U, // 57
        40U, // 58
        41U, // 59
        42U, // 60
        43U, // 61
        55U, // 62
        56U, // 63
        61U, // 64
        0U   // 65
};

const SetCarTypeTypeDef SetCarType[] =
    {
        {"AUTO_GAIN", 0U},          // 0
        {"DAWOO_2012", 75U},        // 1
        {"DAWOO_2018", 190U},       // 2
        {"HINO_500", 198U},         // 3
        {"HINO_700", 198U},         // 4
        {"HINO_SH1EESA", 198U},     // 5
        {"HINO_COASTER", 410U},     // 6
        {"FUSO_1841LSE2015", 125U}, // 7
        {"FUSO_2016", 198U},        // 8
        {"FUSO_FM173", 198U},       // 9
        {"FUSO_ROSA_BUS", 198U},    // 10
        {"UD", 198U},               // 11
        {"UD_PKC8YLS", 198U},       // 12
        {"DAF_280", 100U},          // 13
        {"DAF_360", 130U},          // 14
        {"DAF_380_AUTP", 107U},     // 15
        {"DAF_380_HAND", 168U},     // 16
        {"DAF_410", 107U},          // 17
        {"DAF_460", 96U},           // 18
        {"SITRAK", 60U},            // 19
        {"SCANIA_K380", 112U},      // 20
        {"SCANIA_K400", 112U},      // 21
        {"VOLVO_FM4B", 154U},       // 22
        {"VOLVO_FM", 155U},         // 23
        {"BENZ_O500", 58U},         // 24
        {"BENZ_2046S", 70U},        // 25
        {"BENZ_2545LS", 70U},       // 26
        {"BENZ_2553LS", 70U},       // 27
        {"BENZ_2653LS", 70U},       // 28
        {"BENZ_430PS", 57U},        // 29
        {"BENZ_1846LS", 90U},       // 30
        {"BENZ_2645L", 90U},        // 31
        {"BENZ_2651L", 90U},        // 32
        {"BENZ_2545LS_6", 90U},     // 33
        {"BENZ_2553LS_6", 90U},     // 34
        {"BENZ_1840LS", 120U},      // 35
        {"BENZ_2640L", 120U},       // 36
        {"BENZ_2540L", 120U},       // 37
        {"MAN_EURO5", 100U},        // 38
        {"MAN_EURO6", 82U},         // 39
        {"RAC", 50U},               // 40
        {"YUTONG_2014", 80U},       // 41
        {"DF_KL465", 58U},          // 42
        {"DF_KL480", 58U},          // 43
        {"SCANIA_K450", 112U},      // 44
        {"SCANIA_K410", 112U},      // 45
        {"SCANIA_P410", 112U},      // 46
        {"SCANIA_G500", 112U},      // 47
        {"SCANIA_P370", 112U},      // 48
        {"SCANIA_P450", 112U},      // 49
        {"SCANIA_P280", 112U},      // 50
        {"SCANIA_P320", 112U},      // 51
        {"SCANIA_R660", 112U},      // 52
        {"VOLVO_2.85", 5124U},      // 53
        {"VOLVO_3.08", 5452U},      // 54
        {"ISUZU_series_N", 198U},   // 55
        {"ISUZU_series_F", 198U},   // 56
        {"DAF_430", 190U},          // 57
        {"VOLVO_3.4", 5953U},       // 58
        {"VOLVO_4.13", 7530U},      // 59
        {"BENZ_OC500", 58U},        // 60
        {"SHUCHI", 65U},            // 61
        {"VOLVO_4.5", 10939U},      // 62
        {"VOLVO_5.14", 12495U},     // 63
        {"VOLVO_5.67", 13783U},     // 64
        {"CAR_TYPE_MAX", 255U}      // 65
};

/*
const char* szCarType[] =
{
    "AUTO_GAIN",
    "DAWOO_2012",
    "DAWOO_2018",
    "HINO_500",
    "HINO_700",
    "HINO_SH1EESA",
    "HINO_COASTER",
    "FUSO_1841LSE2015",
    "FUSO_2016",
    "FUSO_FM173",
    "FUSO_ROSA_BUS",
    "UD",
    "UD_PKC8YLS",
    "DAF_280",
    "DAF_360",
    "DAF_380_AUTP",
    "DAF_380_HAND",
    "DAF_410",
    "DAF_460",
    "SITRAK",
    "SCANIA_K380",
    "SCANIA_K400",
    "VOLVO_FM4B",
    "VOLVO_FM",
    "BENZ_O500",
    "BENZ_2046S",
    "BENZ_2545LS",
    "BENZ_2553LS",
    "BENZ_2653LS",
    "BENZ_430PS",
    "BENZ_1846LS",
    "BENZ_2645L",
    "BENZ_2651L",
    "BENZ_2545LS_6",
    "BENZ_2553LS_6",
    "BENZ_1840LS",
    "BENZ_2640L",
    "BENZ_2540L",
    "MAN_EURO5",
    "MAN_EURO6",
    "RAC",
    "YUTONG_2014",
    "DF_KL465",
    "DF_KL480",
    ""
};

const uint16_t u16CarTypeGain[] =
{
    0U,			// AUTO_GAIN
    75U,		// DAWOO_2012
    190U,		// DAWOO_2018
    198U,		// HINO_500
    198U,		// HINO_700
    198U,		// HINO_SH1EESA
    410U,		// HINO_COASTER
    125U,		// FUSO_1841LSE2015
    198U,		// FUSO_2016
    198U,		// FUSO_FM173
    198U,		// FUSO_ROSA_BUS
    198U,		// UD
    198U,		// UD_PKC8YLS
    100U,		// DAF_280
    130U,		// DAF_360
    107U,		// DAF_380_AUTP
    168U,		// DAF_380_HAND
    107U,		// DAF_410
    96U,		// DAF_460
    60U,		// SITRAK
    112U,		// SCANIA_K380
    112U,		// SCANIA_K400
    154U,		// VOLVO_FM4B
    155U,		// VOLVO_FM
    60U,		// BENZ_O500
    70U,		// BENZ_2046S
    70U,		// BENZ_2545LS
    70U,		// BENZ_2553LS
    70U,		// BENZ_2653LS
    57U,		// BENZ_430PS
    90U,		// BENZ_1846LS
    90U,		// BENZ_2645L
    90U,		// BENZ_2651L
    90U,		// BENZ_2545LS_6
    90U,		// BENZ_2553LS_6
    120U,		// BENZ_1840LS
    120U,		// BENZ_2640L
    120U,		// BENZ_2540L
    100U,		// MAN_EURO5
    82U,		// MAN_EURO6
    50U,		// RAC
    80U,		// YUTONG_2014
    200U,		// DF_KL465
    200U,		// DF_KL480
    0U			// MAX
};
*/

static char SettingCmdPkg[UART_BASE_BUFFER_SIZE] = {0};

char szTEMP[64] = {'\0'};

//============================================================================//
// User Functions
//============================================================================//
//============================================================================//
// @brief  Get the argument of a command.
// @param  pStr: Pointer to a string which stores the command.
// @retval Position where the argument of the command starts.
//============================================================================//
static uint8_t Protocl_CmdStopHead(char *pStr)
{
    StopChar(pStr, '=');

    // if(g_Setting.DebugMode != DBG_OFF)
    //{
    // RTOSprintf("> Command {%s} process...\r\n", pStr);
    // }
    return strlen(pStr) + 1;
}

// Device Information //

//============================================================================//
// @brief  Process the command related to the device version.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Version(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_VERSION, strlen(STR_AS_VERSION)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_VERSION);
        RTOSprintf("\r\n@ \t<OPTION>: ?");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s_%s_%s_FW-%d.%03d%s_%s", // Heinz
                STR_AS_VERSION,
                g_Setting.cJAS_Ver,
                HARDWARE_VERSION,
                STR_CUST,
                (VERSION_BUILD / 1000),
                (VERSION_BUILD % 1000),
                FW_VERSION_CHK,
                FIRMWARE_DATE);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the BUS ID.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_BusId(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_BUS_ID, strlen(STR_AS_BUS_ID)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ID>" STR_AS_BUS_ID);
        RTOSprintf("\r\n@ \t<ID>: BUS ID,  max. %d char",
                   OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_ID) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_BUS_ID, g_Setting.tBusInfo.cBus_ID);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        char cRetCharList[64] = {0};

        if (strlen(pStr) >= OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_ID))
        {
            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(68);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        if (ChkInputText(pStr, VALID_BUS_ID, cRetCharList) == FALSE)
        {
            sprintf(szCmdMsg, "@ INVALID CHAR( %s)", cRetCharList);

            RTOSprintf("\r\n%s", szCmdMsg);

            RTOSprintf("\r\n@ BUS ID can only be set with number, letters, and '-'");

            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(64);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        if (strcmp(pStr, DEFAULT_STRING) != 0)
        {
            memset(g_Setting.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));

            strcpy(g_Setting.tBusInfo.cBus_ID, pStr);

            System_SaveSetting();

            if (bSD_ReportErrFlag == FALSE)
                V161_RecordTechnical(1);

            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CHANG_BUS_ID);

            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_BUS_ID, g_Setting.tBusInfo.cBus_ID);

            bResult = TRUE;

#if ENABLE_GSM_NET
            // GSM_ResetWithCheckFTP();
            // bGSM_ResetCheckFTP_Flag = TRUE;
            bTCP_CloseFlag[0] = TRUE;
#if ENABLE_MULTI_SERVER
            bTCP_CloseFlag[1] = TRUE;
#endif
#endif
        }
        else
        {
            RTOSprintf("\r\n@ Bus ID not set default string");
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the BUS SN.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_BusSn(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_BUS_SN, strlen(STR_AS_BUS_SN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<SN>", STR_AS_BUS_SN);

        RTOSprintf("\r\n@ \t<SN>: BUS SN, max. %d char",
                   OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_SN) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_BUS_SN, g_Setting.tBusInfo.cBus_SN);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        char cRetCharList[64] = {0};

        if (strlen(pStr) >= OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_SN))
        {
            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(69);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        if (ChkInputText(pStr, VALID_BUS_SN, cRetCharList) == FALSE)
        {
            sprintf(szCmdMsg, "@ INVALID CHAR( %s)", cRetCharList);

            RTOSprintf("\r\n%s", szCmdMsg);

            RTOSprintf("\r\n@ BUS SN can only be set with number, letters, and '-'");

            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(65);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        memset(g_Setting.tBusInfo.cBus_SN, 0x00, sizeof(g_Setting.tBusInfo.cBus_SN));

        strcpy(g_Setting.tBusInfo.cBus_SN, pStr);

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_BUS_SN, g_Setting.tBusInfo.cBus_SN);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the VIN.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_VIN(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_VIN, strlen(STR_AS_VIN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<VIN>", STR_AS_VIN);
        RTOSprintf("\r\n@ \t<VIN>: Vehicle's VIN number, max. %d char",
                   OFFSET_SIZE(gSettingTypeDef, tBusInfo.cVIN) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_VIN, g_Setting.tBusInfo.cVIN);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;
        break;

    default:
    {
        char cRetCharList[64] = {0};

        if (strlen(pStr) >= OFFSET_SIZE(gSettingTypeDef, tBusInfo.cVIN))
        {
            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(70);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        if (ChkInputText(pStr, VALID_VIN, cRetCharList) == FALSE)
        {
            sprintf(szCmdMsg, "@ INVALID CHAR( %s)", cRetCharList);

            RTOSprintf("\r\n%s", szCmdMsg);

            RTOSprintf("\r\n@ VIN can only be set with number and letters (except I, O, Q)");

            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(66);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        strcpy(g_Setting.tBusInfo.cVIN, pStr);

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_VIN, g_Setting.tBusInfo.cVIN);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the vehicle type.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Car_Type(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_CAR_TYPE, strlen(STR_AS_CAR_TYPE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
        CarTypeTypeDef CarType;

    case '\0':
        RTOSprintf("\r\n@ %s<Type>", STR_AS_CAR_TYPE);
        RTOSprintf("\r\n@ \t<Type> Set CAR Type");

        for (CarType = (CarTypeTypeDef)0; CarType < CARTYPE_END; CarType++)
        {
            // RTOSprintf("\r\n@ %d : %s", CarType, szCarType[CarType]);
            RTOSprintf("\r\n@ %d : %s", CarType, SetCarType[CarType].cCarTypeName);
        }

        bResult = TRUE;

        break;

    case '?':
        if (g_Setting.tBusInfo.tCarType > CARTYPE_END)
        {
            g_Setting.tBusInfo.tCarType = CARTYPE_END;
        }

        // sprintf(szCmdMsg, "@ %s%s", STR_AS_CAR_TYPE, szCarType[g_Setting.tBusInfo.tCarType]);
        sprintf(szCmdMsg, "@ %s%s", STR_AS_CAR_TYPE, SetCarType[g_Setting.tBusInfo.tCarType].cCarTypeName);

        RTOSprintf("\r\n> tCarType: %d", g_Setting.tBusInfo.tCarType);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        CarType = (CarTypeTypeDef)atoi(pStr);

        if (CarType > CARTYPE_END)
            break;

        g_Setting.tBusInfo.tCarType = CarType;

        switch (CarType)
        {
        case CARTYPE_VOLVO_2_85:
        case CARTYPE_VOLVO_3_08:
        case CARTYPE_VOLVO_3_4:
        case CARTYPE_VOLVO_4_13:
        case CARTYPE_VOLVO_4_5:
        case CARTYPE_VOLVO_5_14:
        case CARTYPE_VOLVO_5_67:
            memset(szTEMP, 0x00, sizeof(szTEMP));
            sprintf(szTEMP, "$AS+KGAIN=%d\r\n", SetCarType[g_Setting.tBusInfo.tCarType].u16CarTypeGain);
            Protocl_ChkCmdPkg(szTEMP, NULL);
            break;

        default:
            g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
            g_SysInfo.tVehicleState.u32DistCount = 0;
            g_Setting.tSpeedSet.fGain = SetCarType[g_Setting.tBusInfo.tCarType].u16CarTypeGain;
            break;
        }

        bRS_FirstDetection = TRUE;
        u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

        System_SaveSetting();

        if (bSD_ReportErrFlag == FALSE)
            V161_SaveEvent(3);

        // sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_CAR_TYPE, szCarType[g_Setting.tBusInfo.tCarType]);
        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_CAR_TYPE, SetCarType[g_Setting.tBusInfo.tCarType].cCarTypeName);

        RTOSprintf("\r\n%s\r\n", szCmdMsg);

        RTOSprintf("\r\n> ### Gain: %f ###", g_Setting.tSpeedSet.fGain);

        switch (CarType)
        {
        case CARTYPE_VOLVO_2_85:
        case CARTYPE_VOLVO_3_08:
        case CARTYPE_VOLVO_3_4:
        case CARTYPE_VOLVO_4_13:
        case CARTYPE_VOLVO_4_5:
        case CARTYPE_VOLVO_5_14:
        case CARTYPE_VOLVO_5_67:
            memset(szTEMP, 0x00, sizeof(szTEMP));
            strcpy(szTEMP, "$AS+AUTO GAIN=OFF\r\n");
            RTOSprintf("\r\n> %s", szTEMP);
            Protocl_ChkCmdPkg(szTEMP, NULL);
            break;

        default:
            memset(szTEMP, 0x00, sizeof(szTEMP));
            strcpy(szTEMP, "$AS+AUTO GAIN=ON\r\n");
            RTOSprintf("\r\n> %s", szTEMP);
            Protocl_ChkCmdPkg(szTEMP, NULL);
            break;
        }

        bResult = TRUE;
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to configuration information.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DF2_INFO(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_DF2_INFO, strlen(STR_AS_DF2_INFO)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_DF2_INFO);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
        // RTOSprintf("\r\n> Device Type:%s", g_Setting.cJAS_Ver);
        // RTOSprintf("\r\n> Model ID:%d", MODEL_NAME);
        // RTOSprintf("\r\n> Customer ID:%d", CUSTOMER_TYPE);
        // RTOSprintf("\r\n> Hardware ID:%d", HW_VER);

        sprintf(szCmdMsg, "@ %sDevice Type:%s / Model ID:%d / Customer ID:%d / Hardware ID:%d",
                STR_AS_DF2_INFO,
                g_Setting.cJAS_Ver,
                MODEL_NAME,
                CUSTOMER_TYPE,
                HW_VER);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        break;
    }

    return bResult;
}

// Time Setting //

//============================================================================//
// @brief  Process the command related to time zone.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TimeZone(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TIME_ZONE, strlen(STR_AS_TIME_ZONE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<hh:mm>", STR_AS_TIME_ZONE);
        RTOSprintf("\r\n@ \t<hh> -12 to +13");
        RTOSprintf("\r\n@ \t<mm> 00 to 59");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%+03d:%02d",
                STR_AS_TIME_ZONE,
                (g_Setting.i32TimeZoneOffset / 60) / 60,
                abs((g_Setting.i32TimeZoneOffset / 60) % 60));

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        int8_t sign = 1;
        int8_t hh, mm;

        StopChar(pStr, ':');

        if (strlen(pStr) == 0)
            break;

        hh = atoi(pStr);

        if (hh < 0)
            sign = -1;

        hh = abs(hh);

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) == 0)
            break;

        mm = atoi(pStr);

        g_Setting.i32TimeZoneOffset = sign * (hh * 60 + mm) * 60;

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%+03d:%02d Set OK",
                STR_AS_TIME_ZONE,
                (g_Setting.i32TimeZoneOffset / 60) / 60,
                abs((g_Setting.i32TimeZoneOffset / 60) % 60));

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to RTC setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RTC_Calibration(char *pStr)
{
    bool bResult = FALSE;
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    if (strncmp(pStr, STR_AS_RTC_CALIBRATION, strlen(STR_AS_RTC_CALIBRATION)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_RTC_CALIBRATION);
        RTOSprintf("\r\n@ \t<OPTION>:ON");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            HAL_StatusTypeDef halRes;
            osStatus osRes;

            if (g_SysInfo.tGPS_Data.bStatus == TRUE)
            {
                memset(&rtcTime, 0x00, sizeof(rtcTime));
                memset(&rtcDate, 0x00, sizeof(rtcDate));

                rtcTime.Hours = g_SysInfo.tGPS_Data.utcTime.u8Hour + 8;
                rtcTime.Minutes = g_SysInfo.tGPS_Data.utcTime.u8Min;
                rtcTime.Seconds = g_SysInfo.tGPS_Data.utcTime.u8Sec;

                rtcDate.Date = g_SysInfo.tGPS_Data.utcTime.u8Day;
                rtcDate.Month = g_SysInfo.tGPS_Data.utcTime.u8Month;
                rtcDate.Year = g_SysInfo.tGPS_Data.utcTime.u16Year - 2000;
                // rtcDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
                rtcDate.WeekDay = GetWeekday(rtcDate.Year, rtcDate.Month, rtcDate.Date);

                osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

                if (osRes != osOK)
                    return FALSE;

                halRes = HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                if (halRes == HAL_OK)
                {
                    halRes = HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
                }

                osMutexRelease(mutex_ttnow_user);

                if (halRes == HAL_OK)
                {
                    // bGPS_DataFixed = TRUE;

                    if (bMCU_RTC_Updated == FALSE)
                        bMCU_RTC_Updated = TRUE;

#if ENABLE_MG82_RTC
                    if (bMG82_RTC_EN == TRUE)
                        bMG82_RTC_EN = FALSE;

                    MG82_RTC_SetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
                    if (bRX8130_EN == TRUE)
                        bRX8130_EN = FALSE;

                    RX8130_SetDateTime(&rtcDate, &rtcTime, 1);
#else
                    if (g_bPCF_EN == TRUE)
                        g_bPCF_EN = FALSE;

                    PCF_SetDate(&rtcDate, 1);
                    PCF_SetTime(&rtcTime, 1);
#endif

                    bResult = TRUE;
                }
            }
        }

        if (g_Setting.tENABLE_SET.bits.CORRECT_MODE == 0)
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to RTC time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RTC_TIME(char *pStr)
{
    bool bResult = FALSE;
    time_t rawtime;
    struct tm timeinfo;
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    if (strncmp(pStr, STR_AS_RTC_TIME, strlen(STR_AS_RTC_TIME)) != 0)
        return FALSE;

    rawtime = System_GetTime();

    localtime_r(&rawtime, &timeinfo);

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_RTC_TIME);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
    {
        char *pcPtr;

#if ENABLE_MG82_RTC
        MG82_RTC_GetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
        RX8130_GetDateTime(&rtcDate, &rtcTime, 1);
#else
        PCF_GetDate(&rtcDate, 1);
        PCF_GetTime(&rtcTime, 1);
#endif

        RTOSprintf("\r\n@ %s", STR_AS_RTC_TIME);

        RTOSprintf("\r\n@ RTC Time: %04d-%02d-%02d %02d:%02d:%02d %s",
                   rtcDate.Year + 2000, rtcDate.Month, rtcDate.Date, rtcTime.Hours,
                   rtcTime.Minutes, rtcTime.Seconds, cWeekDay[rtcDate.WeekDay]);

        RTOSprintf("\r\n@ GPS Time: %04d/%02d/%02d %02d:%02d:%02d",
                   g_SysInfo.tGPS_Data.utcTime.u16Year,
                   g_SysInfo.tGPS_Data.utcTime.u8Month,
                   g_SysInfo.tGPS_Data.utcTime.u8Day,
                   g_SysInfo.tGPS_Data.utcTime.u8Hour,
                   g_SysInfo.tGPS_Data.utcTime.u8Min,
                   g_SysInfo.tGPS_Data.utcTime.u8Sec);

        RTOSprintf("\r\n@ MCU Time: %04d/%02d/%02d %02d:%02d:%02d",
                   timeinfo.tm_year + 1900,
                   timeinfo.tm_mon + 1,
                   timeinfo.tm_mday,
                   timeinfo.tm_hour,
                   timeinfo.tm_min,
                   timeinfo.tm_sec);

        pcPtr = szCmdMsg;

        sprintf(pcPtr, "@ %sRTC Time: %04d-%02d-%02d %02d:%02d:%02d %s",
                STR_AS_RTC_TIME,
                rtcDate.Year + 2000, rtcDate.Month, rtcDate.Date, rtcTime.Hours,
                rtcTime.Minutes, rtcTime.Seconds, cWeekDay[rtcDate.WeekDay]);

        pcPtr += strlen(pcPtr);

        sprintf(pcPtr, ", GPS Time: %04d/%02d/%02d %02d:%02d:%02d",
                g_SysInfo.tGPS_Data.utcTime.u16Year,
                g_SysInfo.tGPS_Data.utcTime.u8Month,
                g_SysInfo.tGPS_Data.utcTime.u8Day,
                g_SysInfo.tGPS_Data.utcTime.u8Hour,
                g_SysInfo.tGPS_Data.utcTime.u8Min,
                g_SysInfo.tGPS_Data.utcTime.u8Sec);

        pcPtr += strlen(pcPtr);

        sprintf(pcPtr, ", MCU Time: %04d/%02d/%02d %02d:%02d:%02d",
                timeinfo.tm_year + 1900,
                timeinfo.tm_mon + 1,
                timeinfo.tm_mday,
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);

        bResult = TRUE;

        break;
    }

    default:
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to time adjustment.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TimeAdj(char *pStr)
{
    bool bResult = FALSE;
    AdjustDataTypeDef stAdjustBuf;
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;
    time_t ttGetTime;
    char BufStr[16];
    osStatus osRes;

    if (strncmp(pStr, STR_AS_TIME_ADJ, strlen(STR_AS_TIME_ADJ)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<TIME>", STR_AS_TIME_ADJ);
        RTOSprintf("\r\n@ \t<TIME>: Time in format YYMMDDhhmmss");

        bResult = TRUE;

        break;

    default:
    {
        ttGetTime = System_GetTime();

        if (strlen(pStr) != 12)
            break;

        /*
        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        rawtime = mktime(&timeinfo);

        // ���o UTC �ɶ�
        //rawtime -= g_Setting.i32TimeZoneOffset;

        localtime_r(&rawtime, &timeinfo);
        */

        strncpy(BufStr, pStr, 2);
        rtcDate.Year = atoi(BufStr);

        strncpy(BufStr, pStr + 2, 2);
        rtcDate.Month = atoi(BufStr);

        strncpy(BufStr, pStr + 4, 2);
        rtcDate.Date = atoi(BufStr);

        rtcDate.WeekDay = GetWeekday(rtcDate.Year, rtcDate.Month, rtcDate.Date);

        strncpy(BufStr, pStr + 6, 2);
        rtcTime.Hours = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        rtcTime.Minutes = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        rtcTime.Seconds = atoi(BufStr);

        rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

        osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;

        HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
        HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

        osMutexRelease(mutex_ttnow_user);

#if ENABLE_MG82_RTC
        if (bMG82_RTC_EN == TRUE)
            bMG82_RTC_EN = FALSE;

        MG82_RTC_SetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
        if (bRX8130_EN == TRUE)
            bRX8130_EN = FALSE;

        RX8130_SetDateTime(&rtcDate, &rtcTime, 1);
#else
        if (g_bPCF_EN == TRUE)
            g_bPCF_EN = FALSE;

        PCF_SetDate(&rtcDate, 1);
        PCF_SetTime(&rtcTime, 1);
#endif

        if (bSystemTimeReady == FALSE)
            bSystemTimeReady = TRUE;

        if (bSD_ReportErrFlag == FALSE)
        {
            // Record Table 7
            SecToBCDTime(ttGetTime, stAdjustBuf.u8BeforeDateBCD);
            stAdjustBuf.u8AfterDateBCD[0] = 0x20;
            stAdjustBuf.u8AfterDateBCD[1] = Bin_2_BCD(rtcDate.Year);
            stAdjustBuf.u8AfterDateBCD[2] = Bin_2_BCD(rtcDate.Month);
            stAdjustBuf.u8AfterDateBCD[3] = Bin_2_BCD(rtcDate.Date);
            stAdjustBuf.u8AfterDateBCD[4] = Bin_2_BCD(rtcTime.Hours);
            stAdjustBuf.u8AfterDateBCD[5] = Bin_2_BCD(rtcTime.Minutes);
            stAdjustBuf.u8AfterDateBCD[6] = Bin_2_BCD(rtcTime.Seconds);

            V161_RecordAdjust(stAdjustBuf);

            V161_SaveEvent(4);
        }

        // if(g_Setting.ENABLE_SET.bits.CORRECT_MODE == 0)
        //{
        //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        // }

        bResult = TRUE;
    }

    break;
    }

    return bResult;
}

// External Information Setting //

// Power

//============================================================================//
// @brief  Process the command related to power mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_PowerMode(char *pStr)
{
    bool bResult = FALSE;
    PowerModeTypeDef PowerMode;

    if (strncmp(pStr, STR_AS_POWER_MODE, strlen(STR_AS_POWER_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_POWER_MODE);
        RTOSprintf("\r\n@ \t<mode> set mode for power loss");
        RTOSprintf("\r\n@ \t%s\t: always power on", PowerModeString[PWR_ON]);
        RTOSprintf("\r\n@ \t%s\t: power shutdown at once", PowerModeString[PWR_OFF]);
        RTOSprintf("\r\n@ \t%s\t: system wake-up by alarm", PowerModeString[PWR_IMMO]);
        RTOSprintf("\r\n@ \t%s\t: system sleep", PowerModeString[PWR_SAVE]);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_POWER_MODE, PowerModeString[g_Setting.tPowerMode]);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        for (PowerMode = (PowerModeTypeDef)0; PowerMode < PWR_END; PowerMode++)
        {
            if (strcmp(pStr, PowerModeString[PowerMode]) == 0)
            {
                g_Setting.tPowerMode = PowerMode;

                if (g_Setting.tPowerMode == PWR_ON || g_Setting.tPowerMode == PWR_IMMO)
                {
                    g_Setting.tENABLE_SET.bits.ImmoCtrl = 1;
                }
                else
                {
                    g_Setting.tENABLE_SET.bits.ImmoCtrl = 0;
                }

                System_SaveSetting();

                sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_POWER_MODE, PowerModeString[g_Setting.tPowerMode]);

                bResult = TRUE;

                break;
            }
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to power down setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_PowerDown(char *pStr)
{
    bool bResult = FALSE;
    static uint16_t u16Delay;
    if (strncmp(pStr, STR_AS_POWER_DOWN, strlen(STR_AS_POWER_DOWN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>,<source of delay time>,<delay>", STR_AS_POWER_DOWN);
        RTOSprintf("\r\n@ \t<mode>: 0: Standby, 1: Shutdown");
        RTOSprintf("\r\n@ \t<source of delay times>:0-1: ACC delay time, 2: 4G delay time, 3: G-Sensor delay time");
        RTOSprintf("\r\n@ \t<delay>: Delay time in seconds in case shutdown. Range: 0-%d", MG82_SHUTDOWN_DELAY_MAX);
        bResult = TRUE;

        break;

    case '?':
        // sprintf(szCmdMsg, "@ %s%d,%d,%d", STR_AS_POWER_DOWN,
        //         g_Setting.tPwrDown.tMode ? 0 : 1,
        //         g_Setting.tPwrDown.u8WkupSource,
        //         u16Delay);
        sprintf(szCmdMsg, "@ %s\r\n@ \t<mode>: %d\r\n@ \t<ACC delay time>: %d (s)\r\n@ \t<4G delay time>: %d (s)\r\n@ \t<G-Sensor delay time>: %d (s)",
                szCmdMsg,
                g_Setting.tPwrDown.tMode,
                g_Setting.tPwrDown.u16Delay,
                g_Setting.tGSM_SET.u32DelayTimeOut,
                g_Setting.tGS_Setting.u32DelayTimeOut);
        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8Mode = 0;

        StopChar(pStr, ',');

        u8Mode = atoi(pStr);

        if (u8Mode > 1)
        {
            RTOSprintf("\r\n> Invalid power down mode!");

            break;
        }

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        g_Setting.tPwrDown.u8WkupSource = atoi(pStr);

        if (g_Setting.tPwrDown.u8WkupSource > 3)
        {
            RTOSprintf("\r\n> Invalid source wakup!");

            break;
        }

        pStr += strlen(pStr) + 1;

        u16Delay = atoi(pStr);

        if (u16Delay > MG82_SHUTDOWN_DELAY_MAX)
        {
            RTOSprintf("\r\n> Invalid delay time!");

            break;
        }

        if (u8Mode == 0)
        {
            g_Setting.tPwrDown.tMode = PWR_DOWN_STANDBY;
        }
        else
        {
            g_Setting.tPwrDown.tMode = PWR_DOWN_SHUTDOWN;
        }
        if (g_Setting.tPwrDown.u8WkupSource == (uint8_t)MG82_WKUP_SRC_4G_RI)
        {
            g_Setting.tGSM_SET.u32DelayTimeOut = u16Delay;
        }
        else if (g_Setting.tPwrDown.u8WkupSource == (uint8_t)MG82_WKUP_SRC_G_SEN)
        {
            g_Setting.tGS_Setting.u32DelayTimeOut = u16Delay;
        }
        else
        {
            g_Setting.tPwrDown.u16Delay = u16Delay;
        }

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%d%d%d Set OK",
                STR_AS_POWER_DOWN,
                u8Mode,
                g_Setting.tPwrDown.u8WkupSource,
                u16Delay);

        RTOSprintf(szCmdMsg);
        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

// Distance

//============================================================================//
// @brief  Process the command related to the travelling distance version 0.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Distance(char *pStr)
{
    bool bResult = FALSE;
    int i, j;
    double dBuf = 0;
    CalibrationDataTypeDef stCalDataBuf;
    time_t ttGetTimeBuf;
    uint8_t u8GetTimeBuf[7], u8BCDBuf;
    char cGetDataBuf[10];

    if (strncmp(pStr, STR_AS_DISTANCE, strlen(STR_AS_DISTANCE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_DISTANCE);
        RTOSprintf("\r\n@ \t<value> is set current distance, Unit: 100M");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%.2f KM", STR_AS_DISTANCE, M_2_KM(g_SysInfo.tVehicleState.dDistance));

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        strncpy((char *)stCalDataBuf.u8NameID, ID_ADMIN, sizeof(ID_ADMIN));

        ttGetTimeBuf = System_GetTime();

        SecToBCDTime(ttGetTimeBuf, u8GetTimeBuf);

        memcpy(stCalDataBuf.u8BeforeDateBCD, u8GetTimeBuf, 7);

        memcpy(stCalDataBuf.u8AfterDateBCD, u8GetTimeBuf, 7);

        sprintf(cGetDataBuf, "%08d", (int)M_2_100M(g_SysInfo.tVehicleState.dDistance));

        // RTOSprintf("\r\n> 1. %c %c %c %c %c %c %c %c",
        //									cGetDataBuf[0], cGetDataBuf[1], cGetDataBuf[2], cGetDataBuf[3],
        //									cGetDataBuf[4], cGetDataBuf[5], cGetDataBuf[6], cGetDataBuf[7]);

        j = 0;

        for (i = 0; i < 4; i++)
        {
            u8BCDBuf = cGetDataBuf[j];
            u8BCDBuf = (u8BCDBuf << 4) & 0xF0;
            u8BCDBuf = u8BCDBuf | (cGetDataBuf[j + 1] & 0x0F);

            stCalDataBuf.u8BeforeDistBCD[i] = u8BCDBuf;

            j += 2;
        }

        dBuf = atol(pStr);

        sprintf(cGetDataBuf, "%08.0f", dBuf);

        // sprintf(cGetDataBuf, "%08d", atoi(pStr));
        // RTOSprintf("\r\n> 2. %c %c %c %c %c %c %c %c",
        //									cGetDataBuf[0], cGetDataBuf[1], cGetDataBuf[2], cGetDataBuf[3],
        //									cGetDataBuf[4], cGetDataBuf[5], cGetDataBuf[6], cGetDataBuf[7]);

        j = 0;

        for (i = 0; i < 4; i++)
        {
            u8BCDBuf = cGetDataBuf[j];
            u8BCDBuf = (u8BCDBuf << 4) & 0xF0;
            u8BCDBuf = u8BCDBuf | (cGetDataBuf[j + 1] & 0x0F);

            stCalDataBuf.u8AfterDistBCD[i] = u8BCDBuf;

            j += 2;
        }

        if (bSD_ReportErrFlag == FALSE)
            V161_RecordCalibration(stCalDataBuf);

        dBuf = atol(pStr);

        g_SysInfo.tVehicleState.dDistance = HM_2_M(dBuf);
        // g_SysInfo.tVehicleState.dDistance = HM_2_M(atoi(pStr));
        g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
        g_SysInfo.tVehicleState.u32DistCount = 0;

        System_SaveSetting();

        System_SaveBackupSetting();

        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CHANG_DISTANCE);

        if (g_Setting.tENABLE_SET.bits.CORRECT_MODE == 0)
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        }

        sprintf(szCmdMsg, "@ %s%.2f Km Set OK", STR_AS_DISTANCE, M_2_KM(g_SysInfo.tVehicleState.dDistance));

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;
    }

    return bResult;
}

// Speed

/*
//============================================================================//
// @brief  Process the command related to speed trigger.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedTrig(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_SPEED_TRIG, strlen(STR_AS_SPEED_TRIG)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<VALUE>", STR_AS_SPEED_TRIG);
            RTOSprintf("\r\n@ \t<VALUE> range of %d to %d", SPEED_TRIG_MIN, SPEED_TRIG_MAX);

            bResult = TRUE;

      break;

        case '?':
            sprintf(szCmdMsg, "@ %s%d", STR_AS_SPEED_TRIG, g_Setting.tSpeedSet.u16Trig);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

          break;

        default:
        {
            uint16_t u16ValueTemp;


            u16ValueTemp = atoi(pStr);

            if(u16ValueTemp > SPEED_TRIG_MAX)
                break;

            g_Setting.tSpeedSet.u16Trig = u16ValueTemp;

            System_SaveSetting();

            bResult = TRUE;
        }

        break;
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to speed gain.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedGain(char *pStr)
{
    bool bResult = FALSE;
    float fGainBuf = 0;

    if (strncmp(pStr, STR_AS_SPEED_GAIN, strlen(STR_AS_SPEED_GAIN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_SPEED_GAIN);
        RTOSprintf("\r\n@ \t<value> = 0, show speed by GPS");
        RTOSprintf("\r\n@ \t<value> > 0, it's one speed pulse distance, Unit: MM");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%f", STR_AS_SPEED_GAIN, g_Setting.tSpeedSet.fGain);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
        g_SysInfo.tVehicleState.u32DistCount = 0;
        // g_SysInfo.tDrivingInfo.u16LCDT = 0 ; // Heinz

        fGainBuf = atof(pStr);

        if (fGainBuf >= 0)
        {
            g_Setting.tSpeedSet.fGain = atof(pStr);

            bRS_FirstDetection = TRUE;
            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

            g_Setting.tSpeedSet.u8Auto_Gain_Count = 128; // AUTO_GAIN_OK_COUNT; // Heinz

            System_SaveSetting();

            // RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CHANG_SPEED_GAIN);

            if (bSD_ReportErrFlag == FALSE)
                V161_SaveEvent(3);

            // if (g_Setting.ENABLE_SET.bits.CORRECT_MODE == 0)
            //{
            //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
            // }

            sprintf(szCmdMsg, "@ %s%.2f Set OK", STR_AS_SPEED_GAIN, g_Setting.tSpeedSet.fGain);

            bResult = TRUE;
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the speed adjustment.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedAdj(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SPEED_ADJ, strlen(STR_AS_SPEED_ADJ)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<value>", STR_AS_SPEED_ADJ);
        RTOSprintf("\r\n@ \t<value> = 0, calibration speed by GPS");
        RTOSprintf("\r\n@ \t<value> = Range of 1 to %d, calibration speed by value, Unit: KM", SPEED_ADJ_MAX);

        bResult = TRUE;

        break;

    default:
        if (atoi(pStr) >= SPEED_ADJ_MAX)
        {
            bResult = FALSE;
            break;
        }

        g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
        g_SysInfo.tVehicleState.u32DistCount = 0;

        if (atoi(pStr) == 0)
        {
            g_Setting.tSpeedSet.fGain = (float)M_2_MM(GPS_GetSpeed()) / HOUR_2_SECOND(1) / g_SysInfo.tSpeedInfo.u16Speed_Hz;
        }
        else
        {
            g_Setting.tSpeedSet.fGain = (float)KM_2_MM(atoi(pStr)) / HOUR_2_SECOND(1) / g_SysInfo.tSpeedInfo.u16Speed_Hz;
        }

        bRS_FirstDetection = TRUE;
        u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

        System_SaveSetting();

        // if(g_Setting.tENABLE_SET.bits.CORRECT_MODE == 0)
        //{
        //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        // }

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the speed offset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedOffset(char *pStr)
{
    bool bResult = FALSE;
    int16_t Speed_Offset_buf;

    if (strncmp(pStr, STR_AS_SPEED_OFFSET, strlen(STR_AS_SPEED_OFFSET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_SPEED_OFFSET);
        RTOSprintf("\r\n@ \t<value> = xx, show speed offset value,unite 0.1km");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_SPEED_OFFSET, g_Setting.tSpeedSet.u8Offset);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (g_Setting.tSpeedSet.u8Auto_Gain_Count < 128)
        {
            sprintf(szCmdMsg, "@ AutoGain is ON, please close!!");
        }
        else
        {
            Speed_Offset_buf = atoi(pStr);

            if ((Speed_Offset_buf > 100) || (Speed_Offset_buf < 0))
            {
                RTOSprintf("\r\n> Error <value> out off range 0 ~ 70.");

                bResult = FALSE;
            }
            else
            {
                g_Setting.tSpeedSet.u8Offset = Speed_Offset_buf;

                System_SaveSetting();

                sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_SPEED_OFFSET, g_Setting.tSpeedSet.u8Offset);

                g_SysInfo.tVehicleState.u32DistCount_AGain = 0; // Heinz
                g_SysInfo.tVehicleState.dDistBase_GPS = 0;
                g_SysInfo.tVehicleState.dDistance_GPS = 0;

                bResult = TRUE;
            }
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the speed star.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedStar(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_AUTOGAIN_SPEED, strlen(STR_AS_AUTOGAIN_SPEED)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_AUTOGAIN_SPEED);
        RTOSprintf("\r\n@ \t<value> = xx, Show auto gain star speed value, unite 0.1km");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_AUTOGAIN_SPEED, (g_Setting.tSpeedSet.u16Star));

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (g_Setting.tSpeedSet.u8Auto_Gain_Count < 128)
        {
            sprintf(szCmdMsg, "@ AutoGain is ON, please close!!");
        }
        else
        {
            g_Setting.tSpeedSet.u16Star = atoi(pStr);

            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_AUTOGAIN_SPEED, (g_Setting.tSpeedSet.u16Star));

            bResult = TRUE;
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the auto gain.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AutoGain(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_AUTO_GAIN, strlen(STR_AS_AUTO_GAIN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_AUTO_GAIN);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_AUTO_GAIN,
                (g_Setting.tSpeedSet.u8Auto_Gain_Count < 128) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (g_Setting.tSpeedSet.u8MergeSpeed == FALSE)
            {
                if ((g_Setting.tSpeedSet.u8Auto_Gain_Count == 128) &&
                    (g_Setting.tSpeedSet.u8AutoGainInit == 0))
                {
                    RTOSprintf("\r\n> ### AUTOGAIN Request Start ###");

                    g_Setting.u16AG_Dist_Limit_Temp = 512;
                    g_Setting.tSpeedSet.u8AutoGainInit = 1;

                    System_SaveSetting();

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_ON, STR_NULL);
#endif
                }
            }
            else
            {
                RTOSprintf("\r\n@ Merge GPS speed ON! Command is disabled!");
            }

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            RTOSprintf("\r\n> ### Auto Gain OFF ###");

            g_Setting.tSpeedSet.u8Auto_Gain_Count = 128; // AUTO_GAIN_OK_COUNT; // Heinz

            fAutoGainValue = 0;
            g_Setting.tSpeedSet.fAutoGain = 0;
            g_Setting.tSpeedSet.ttAutoGainTimeOut = 0;
            g_SysInfo.tVehicleState.dDistBase_GPS = 0;
            g_SysInfo.tVehicleState.dDistance_GPS = 0;
            g_SysInfo.tVehicleState.u32DistCount_AGain = 0;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_AUTOGAIN_OFF, STR_NULL);
#endif

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            if (g_Setting.tSpeedSet.u8MergeSpeed == FALSE)
            {
                sprintf(szCmdMsg, "@ %s%s Set OK",
                        STR_AS_AUTO_GAIN,
                        (g_Setting.tSpeedSet.u8Auto_Gain_Count < 128) ? "ON" : "OFF");
            }
            else
            {
                sprintf(szCmdMsg, "@ %sOFF (Merge GPS Speed ON!)", STR_AS_AUTO_GAIN);
            }
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the speed limit.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SpeedLim(char *pStr)
{
    bool bResult = FALSE;
    uint8_t idx;

    if (strncmp(pStr, STR_AS_SPEED_LIMIT, strlen(STR_AS_SPEED_LIMIT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<bank>,<value>", STR_AS_SPEED_LIMIT);
        RTOSprintf("\r\n@ \t<bank>range of 1 to %d", SPEED_LIMIT_BANK_MAX);
        RTOSprintf("\r\n@ \t<value>is speed limit, Unit: 100M");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s1:%.1fKM/H 2:%.1fKM/H 3:%.1fKM/H",
                STR_AS_SPEED_LIMIT,
                (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[0]),
                (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[1]),
                (float)M_2_KM(g_Setting.tSpeedSet.u32Limit[2]));

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case '*':
        sprintf(szCmdMsg, "@ %s1:%d 2:%d 3:%d",
                STR_AS_SPEED_LIMIT,
                M_2_100M(g_Setting.tSpeedSet.u32Limit[0]),
                M_2_100M(g_Setting.tSpeedSet.u32Limit[1]),
                M_2_100M(g_Setting.tSpeedSet.u32Limit[2]));

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        if (strlen(pStr) > 1)
            break;

        idx = atoi(pStr);

        if (idx == 0 || idx > SPEED_LIMIT_BANK_MAX)
            break;

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) == 0)
            break;

        g_Setting.tSpeedSet.u32Limit[idx - 1] = HM_2_M(atoi(pStr));

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%d:%d Set OK", STR_AS_SPEED_LIMIT, idx, M_2_100M(g_Setting.tSpeedSet.u32Limit[idx - 1]));

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to over speed events.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Over_Speed(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_OVER_SPEED, strlen(STR_AS_OVER_SPEED)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<Speed>,<Time>", STR_AS_OVER_SPEED);
        RTOSprintf("\r\n@ \t<Speed> speed limit, range of 900 to 1200 Unit: 100M");
        RTOSprintf("\r\n@ \t<Time> end time, range of 2 to 5 Sec");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d (100M/H),%d Sec", STR_AS_OVER_SPEED,
                g_Setting.tSpeedSet.u16SPEED_LIMIT,
                g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint16_t u16Buf1 = 0, u16Buf2 = 0;

        StopChar(pStr, ',');

        if ((strlen(pStr) < 2) || (strlen(pStr) > 5))
            break;

        u16Buf1 = atoi(pStr);

        if ((u16Buf1 > 1200) || (u16Buf1 < 900))
        {
            RTOSprintf("\r\n> Error: Speed limit out of range!");
            RTOSprintf("\r\n> Speed limit range: 90~120Km/h.");

            break;
        }

        // RTOSprintf("\r\n@ Speed limit = %d Km/h", g_Setting.tSpeedSet.u16SPEED_LIMIT);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) < 1) || (strlen(pStr) > 3))
            break;

        u16Buf2 = atoi(pStr);

        if ((u16Buf2 > 5) || (u16Buf2 < 2))
        {
            RTOSprintf("\r\n> Error: Over speed end time out of range!");
            RTOSprintf("\r\n> Over speed end time range: 2~5 Sec");

            break;
        }

        // RTOSprintf("\r\n@ Over Speed end time=%d Sec", g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX);

        g_Setting.tSpeedSet.u16SPEED_LIMIT = u16Buf1;

        g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX = u16Buf2;

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%d (100M/H),%d Sec Set OK",
                STR_AS_OVER_SPEED,
                g_Setting.tSpeedSet.u16SPEED_LIMIT,
                g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to acceleration/ deceleration limit.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AcceLim(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_ACCEL_LIMIT, strlen(STR_AS_ACCEL_LIMIT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<AccelAmount>,<DecelAmount>,<Duration>", STR_AS_ACCEL_LIMIT);
        RTOSprintf("\r\n@ \t<AccelAmount> Acceleration speed limit, range of 10 to 100 Unit: Km/h");
        RTOSprintf("\r\n@ \t<DecelAmount> Deceleration speed limit, range of 10 to 100 Unit: Km/h");
        RTOSprintf("\r\n@ \t<Duration> Acceleration/ deceleration duration for limit, range of 2 to 5 Sec");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %sAccelLimit:%d(Km/h), DecelLimit:%d(Km/h), Duration:%d(Sec)",
                STR_AS_ACCEL_LIMIT,
                g_Setting.tSpeedSet.u16ACCEL_LIMIT,
                g_Setting.tSpeedSet.u16DECEL_LIMIT,
                g_Setting.tSpeedSet.u8ACCEL_LIM_DURA);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint16_t u16Buf1 = 0, u16Buf2 = 0, u16Buf3 = 0;

        StopChar(pStr, ',');

        if ((strlen(pStr) < 2) || (strlen(pStr) > 3))
            break;

        u16Buf1 = atoi(pStr);

        if ((u16Buf1 > 100) || (u16Buf1 < 10))
        {
            RTOSprintf("\r\n> Error: Acceleration speed limit out of range!");
            RTOSprintf("\r\n> Acceleration speed limit range: 10~100 Km/h.");

            break;
        }

        // RTOSprintf("\r\n@ Acceleration speed limit = %d Km/h", g_Setting.tSpeedSet.u16ACCE_LIMIT);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if ((strlen(pStr) < 2) || (strlen(pStr) > 3))
            break;

        u16Buf2 = atoi(pStr);

        if ((u16Buf2 > 100) || (u16Buf2 < 10))
        {
            RTOSprintf("\r\n> Error: Deceleration speed limit out of range!");
            RTOSprintf("\r\n> Deceleration speed limit range: 10~100 Km/h.");

            break;
        }

        // RTOSprintf("\r\n@ Deceleration speed limit = %d Km/h", g_Setting.tSpeedSet.u16DECE_LIMIT);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) < 1) || (strlen(pStr) > 2))
            break;

        u16Buf3 = atoi(pStr);

        if ((u16Buf3 > 5) || (u16Buf3 < 2))
        {
            RTOSprintf("\r\n> Error: Acceleration/ Deceleration duration for limit out of range!");
            RTOSprintf("\r\n> Acceleration/ Deceleration duration for limit range: 2~5 Sec");

            break;
        }

        // RTOSprintf("\r\n@ Acceleration/deceleration duration for limit=%d Sec", g_Setting.tSpeedSet.u8ACCE_LIM_DUR);

        g_Setting.tSpeedSet.u16ACCEL_LIMIT = u16Buf1;

        g_Setting.tSpeedSet.u16DECEL_LIMIT = u16Buf2;

        g_Setting.tSpeedSet.u8ACCEL_LIM_DURA = u16Buf3;

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %sAccelLimit:%d(Km/h), DecelLimit:%d(Km/h), Duration:%d(Sec) Set OK",
                STR_AS_ACCEL_LIMIT,
                g_Setting.tSpeedSet.u16ACCEL_LIMIT,
                g_Setting.tSpeedSet.u16DECEL_LIMIT,
                g_Setting.tSpeedSet.u8ACCEL_LIM_DURA);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to acceleration/ deceleration limit.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AutoGain_Time(char *pStr)
{
    bool bResult = FALSE;
    char BufStr[20] = {0};
    time_t ttAGTime;

    if (strncmp(pStr, STR_AS_AUTO_GAIN_TIME, strlen(STR_AS_AUTO_GAIN_TIME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %sNOW", STR_AS_AUTO_GAIN_TIME);
        RTOSprintf("\r\n@ Set today's date two years from now");

        bResult = TRUE;

        break;

    case '?':
        // RTOSprintf("\r\n@ %s%d", STR_AS_AUTO_GAIN_TIME, g_Setting.tSpeedSet.ttAgain_Time);
        ttAGTime = g_Setting.tSpeedSet.ttAgain_Time + g_Setting.i32TimeZoneOffset;
        SecToBCDTime(ttAGTime, (uint8_t *)BufStr);
        sprintf(szCmdMsg, "@ Next AutoGain Time: %02X%02X/%02X/%02X %02X:%02X:%02X",
                BufStr[0], BufStr[1], BufStr[2], BufStr[3], BufStr[4], BufStr[5], BufStr[6]);

        RTOSprintf("\r\n %s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "NOW") == 0)
        {
            ttAGTime = System_GetTime();

            ttAGTime += DAY_2_SECOND(365) * 2;

            // RTOSprintf("\r\nTime Sec: %d", ttAGTime);

            // ���o UTC �ɶ�
            ttAGTime -= g_Setting.i32TimeZoneOffset;

            g_Setting.tSpeedSet.ttAgain_Time = ttAGTime;

            // Debug Msg

            ttAGTime = 0;

            ttAGTime = g_Setting.tSpeedSet.ttAgain_Time + g_Setting.i32TimeZoneOffset;

            SecToBCDTime(ttAGTime, (uint8_t *)BufStr);

            sprintf(szCmdMsg, "@ Next AutoGain Time: %02X%02X/%02X/%02X %02X:%02X:%02X Set OK",
                    BufStr[0], BufStr[1], BufStr[2], BufStr[3], BufStr[4], BufStr[5], BufStr[6]);

            RTOSprintf("\r\n> %s", szCmdMsg);

            bResult = TRUE;
        }
        else
        {
            sprintf(szCmdMsg, "@ Parameter Error");

            RTOSprintf("\r\n> %s", szCmdMsg);
        }
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to K factor setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_KGain(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_KGAIN, strlen(STR_AS_KGAIN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_KGAIN);
        RTOSprintf("\r\n@ \t<value> Vehicle K factor, unit: pulse/km");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_KGAIN, g_Setting.tSpeedSet.u32KGAIN_Val);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32KVal;

        u32KVal = atoi(pStr);

        if (u32KVal > 0.0)
        {
            g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
            g_SysInfo.tVehicleState.u32DistCount = 0;

            g_Setting.tSpeedSet.u32KGAIN_Val = u32KVal;

            g_Setting.tSpeedSet.fGain = (double)1000 * 1000 / u32KVal;

            bRS_FirstDetection = TRUE;
            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;

            g_Setting.tSpeedSet.u8Auto_Gain_Count = 128; // AUTO_GAIN_OK_COUNT; // Heinz

            System_SaveSetting();

            // RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CHANG_SPEED_GAIN);

            if (bSD_ReportErrFlag == FALSE)
                V161_SaveEvent(3);

            // if (g_Setting.ENABLE_SET.bits.CORRECT_MODE == 0)
            //{
            //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
            // }

            sprintf(szCmdMsg, "@ %s%d Set OK, SPEED GAIN: %.2f", STR_AS_KGAIN, u32KVal, g_Setting.tSpeedSet.fGain);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related enable/disable GAIN check function.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GAIN_ChkEn(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GAIN_CHK_EN, strlen(STR_AS_GAIN_CHK_EN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        sprintf(szCmdMsg, "@ %sON/OFF", STR_AS_GAIN_CHK_EN);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_GAIN_CHK_EN, (g_Setting.tSpeedSet.bGAIN_ChkDisable == FALSE) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.tSpeedSet.bGAIN_ChkDisable = FALSE;

            bResult = TRUE;
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tSpeedSet.bGAIN_ChkDisable = TRUE;

            bChkGAIN_Step1 = FALSE;
            bChkGAIN_Step2 = FALSE;

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_GAIN_CHK_EN, (g_Setting.tSpeedSet.bGAIN_ChkDisable == FALSE) ? "ON" : "OFF");

            RTOSprintf("\r\n%s", szCmdMsg);
        }

        break;
    }

    return bResult;
}

// RPM

/*
//============================================================================//
// @brief  Process the command related to RPM Trigger.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RpmTrig(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_RPM_TRIG, strlen(STR_AS_RPM_TRIG)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<VALUE>", STR_AS_RPM_TRIG);
            RTOSprintf("\r\n@ \t<VALUE> range of %d to %d", RPM_TRIG_MIN, RPM_TRIG_MAX);

            bResult = TRUE;

      break;

        case '?':
            sprintf(szCmdMsg, "@ %s%d", STR_AS_RPM_TRIG, g_Setting.tRPM_Set.u16Trig);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

          break;

        default:
        {
            uint16_t u16ValueTemp;


            u16ValueTemp = atoi(pStr);

            if(u16ValueTemp > RPM_TRIG_MAX)
                break;

            g_Setting.tRPM_Set.u16Trig = u16ValueTemp;

            System_SaveSetting();

            bResult = TRUE;
        }

        break;
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to RPM Div.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RpmDiv(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RPM_DIV, strlen(STR_AS_RPM_DIV)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_RPM_DIV);
        RTOSprintf("\r\n@ \t<value>range of %d to %d", RPM_DIV_MIN, RPM_DIV_MAX);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_RPM_DIV, (uint32_t)g_Setting.tRPM_Set.u16DIV);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint16_t ValueTemp;

        ValueTemp = atoi(pStr);

        if (ValueTemp > RPM_DIV_MAX)
            break;

        g_Setting.tRPM_Set.u16DIV = ValueTemp;

        System_SaveSetting();

        // RTOSprintf("> [SYS]%04X\r\n", SYS_STATE_CHANG_RPM_GAIN);

        if (bSD_ReportErrFlag == FALSE)
            V161_SaveEvent(3);

        // if(g_Setting.tENABLE_SET.bits.CORRECT_MODE == 0)
        //{
        //	RTOSprintf("> [SYS]%04X\r\n", SYS_STATE_NON_CAL);
        // }

        sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_RPM_DIV, (uint32_t)g_Setting.tRPM_Set.u16DIV);

        bResult = TRUE;
    }

    break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to RPM limit.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RpmLim(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RPM_LIMIT, strlen(STR_AS_RPM_LIMIT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<RPM>,<OnTime>,<OffTime>", STR_AS_RPM_LIMIT);
        RTOSprintf("\r\n@ \t<RPM> RPM limit, range of 1000 to 3000 Unit: RPM");
        RTOSprintf("\r\n@ \t<OnTime> start time, range of 30 to 120 Sec");
        RTOSprintf("\r\n@ \t<OffTime> end time, range of 2 to 5 Sec");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d(RPM), On:%d(Sec), Off:%d(Sec)",
                STR_AS_RPM_LIMIT,
                g_Setting.tRPM_Set.u16Limit,
                g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX,
                g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint16_t u16Buf1 = 0, u16Buf2 = 0, u16Buf3 = 0;

        StopChar(pStr, ',');

        if ((strlen(pStr) < 2) || (strlen(pStr) > 5))
            break;

        u16Buf1 = atoi(pStr);

        if ((u16Buf1 > 3000) || (u16Buf1 < 1000))
        {
            RTOSprintf("\r\n> Error: RPM limit out of range!");
            RTOSprintf("\r\n> RPM limit range: 1000~3000 RPM.");

            break;
        }

        // RTOSprintf("\r\n@ RPM limit = %d RPM", g_Setting.tRPM_Set.u16Limit);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if ((strlen(pStr) < 1) || (strlen(pStr) > 4))
            break;

        u16Buf2 = atoi(pStr);

        if ((u16Buf2 > 120) || (u16Buf2 < 30))
        {
            RTOSprintf("\r\n> Error: Over RPM start time out of range!");
            RTOSprintf("\r\n> Over RPM start time range: 30~120 Sec");

            break;
        }

        // RTOSprintf("\r\n@ Over RPM start time=%d Sec", g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) < 1) || (strlen(pStr) > 4))
            break;

        u16Buf3 = atoi(pStr);

        if ((u16Buf3 > 5) || (u16Buf3 < 2))
        {
            RTOSprintf("\r\n> Error: Over RPM end time out of range!");
            RTOSprintf("\r\n> Over RPM end time range: 2~5 Sec");

            break;
        }

        // RTOSprintf("\r\n@ Over RPM end time=%d Sec", g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX);

        g_Setting.tRPM_Set.u16Limit = u16Buf1;

        g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX = u16Buf2;

        g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX = u16Buf3;

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%d(RPM), On:%d(Sec), Off:%d(Sec) Set OK",
                STR_AS_RPM_LIMIT,
                g_Setting.tRPM_Set.u16Limit,
                g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX,
                g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to RPM events.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RPM_Idle(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RPM_IDLE, strlen(STR_AS_RPM_IDLE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("@ %s<Time>\r\n", STR_AS_RPM_IDLE);
        RTOSprintf("@ \t<Time>Range of 60 to 3600 Sec\r\n");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_RPM_IDLE, g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint16_t u16Buf = 0;

        if ((strlen(pStr) < 1) || (strlen(pStr) > 5))
            break;

        u16Buf = atoi(pStr);

        if ((u16Buf > 3600) || (u16Buf < 60))
        {
            RTOSprintf("\r\n> Error: RPM idle time out of range!");
            RTOSprintf("\r\n> Over RPM time range: 60~3600 Sec");

            break;
        }

        // RTOSprintf("\r\n@ RPM idle start time %d Sec", g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX);

        g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX = u16Buf;

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_RPM_IDLE, g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

// CAN Data
#if ENABLE_CAN == 1
//============================================================================//
// @brief  Process the command related to CAN LINK.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_Link(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_CAN_LINK, strlen(STR_AS_CAN_LINK)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
        CAN_LinkTypeDef CanLinkType;

    case '\0':
        RTOSprintf("\r\n@ %s<type>", STR_AS_CAN_LINK);
        RTOSprintf("\r\n@ \t<type> set CAN BUS link protocol");

        for (CanLinkType = (CAN_LinkTypeDef)0; CanLinkType <= LINK_END; CanLinkType++)
        {
            RTOSprintf("\r\n@ \t%d\t: %s", CanLinkType, szCanLinkType[CanLinkType]);
        }

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_CAN_LINK, szCanLinkType[g_Setting.tCanLinkType]);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        // if (strncmp(pStr, "ON", 2) != 0)
        // {
        CanLinkType = (CAN_LinkTypeDef)atoi(pStr);

        if (CanLinkType >= LINK_END)
            break;

        g_Setting.tCanLinkType = CanLinkType;

        RTOSprintf("\r\nCan link type: %d", g_Setting.tCanLinkType);
        System_SaveSetting();
        // }

        // CAN_LinkCount = COUNT_RELINK;

        bCAN_Ready = FALSE;

        CAN_Config();

        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_CAN_LINK, szCanLinkType[g_Setting.tCanLinkType]);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to CAN distance.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_Distance(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_CAN_DIST, strlen(STR_AS_CAN_DIST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_CAN_DIST);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CAN_DIST,
                (g_Setting.tENABLE_SET.bits.CAN_DIST == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (CAN_CheckEnable() == FALSE)
            {
                strcpy(szTEMP, "$AS+RESET=CAN\r\n");

                RTOSprintf("\r\n> %s", szTEMP);

                Protocl_ChkCmdPkg(szTEMP, NULL);
            }

            g_Setting.tENABLE_SET.bits.CAN_DIST = 1;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tENABLE_SET.bits.CAN_DIST = 0;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_CAN_DIST, (g_Setting.tENABLE_SET.bits.CAN_DIST == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to CAN speed.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_Speed(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_CAN_SPEED, strlen(STR_AS_CAN_SPEED)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_CAN_SPEED);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CAN_SPEED,
                (g_Setting.tENABLE_SET.bits.CAN_SPEED == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (CAN_CheckEnable() == FALSE)
            {
                strcpy(szTEMP, "$AS+RESET=CAN\r\n");

                RTOSprintf("\r\n> %s", szTEMP);

                Protocl_ChkCmdPkg(szTEMP, NULL);
            }

            g_Setting.tENABLE_SET.bits.CAN_SPEED = 1;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tENABLE_SET.bits.CAN_SPEED = 0;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_CAN_SPEED, (g_Setting.tENABLE_SET.bits.CAN_SPEED == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to CAN RPM.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_RPM(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_CAN_RPM, strlen(STR_AS_CAN_RPM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_CAN_RPM);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CAN_RPM,
                (g_Setting.tENABLE_SET.bits.CAN_RPM == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (CAN_CheckEnable() == FALSE)
            {
                strcpy(szTEMP, "$AS+RESET=CAN\r\n");

                RTOSprintf("\r\n> %s", szTEMP);

                Protocl_ChkCmdPkg(szTEMP, NULL);
            }

            g_Setting.tENABLE_SET.bits.CAN_RPM = 1;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tENABLE_SET.bits.CAN_RPM = 0;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_CAN_RPM, (g_Setting.tENABLE_SET.bits.CAN_RPM == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to other CAN information.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_Other(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_CAN_OTHER, strlen(STR_AS_CAN_OTHER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_CAN_OTHER);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CAN_OTHER,
                (g_Setting.tENABLE_SET.bits.CAN_OTHER == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (CAN_CheckEnable() == FALSE)
            {
                strcpy(szTEMP, "$AS+RESET=CAN\r\n");

                RTOSprintf("\r\n> %s", szTEMP);

                Protocl_ChkCmdPkg(szTEMP, NULL);
            }

            g_Setting.tENABLE_SET.bits.CAN_OTHER = 1;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tENABLE_SET.bits.CAN_OTHER = 0;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_CAN_OTHER, (g_Setting.tENABLE_SET.bits.CAN_OTHER == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to CAN Relink with all CAN types.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CAN_RelinkAll(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_CAN_RELINK_ALL, strlen(STR_AS_CAN_RELINK_ALL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_CAN_RELINK_ALL);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CAN_RELINK_ALL,
                (g_Setting.bCAN_RelinkAll == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.bCAN_RelinkAll = TRUE;

#if ENABLE_CAN == 1
            CAN_Config();
#endif

            bResult = System_SaveSetting();
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.bCAN_RelinkAll = FALSE;

#if ENABLE_CAN == 1
            CAN_Config();
#endif

            bResult = System_SaveSetting();
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_CAN_RELINK_ALL, (g_Setting.bCAN_RelinkAll == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}
#endif

// Mobile Eye CAN config commands
#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
//============================================================================//
// @brief  Process the command to set CAN MBEYE LDW cnt.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_MBEYE_LDW_Cnt(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Temp;

    if (strncmp(pStr, STR_AS_LDW_CNT, strlen(STR_AS_LDW_CNT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_LDW_CNT);
        RTOSprintf("\r\n@ \t<value> = 2 ~ 7, LDW Counter");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_LDW_CNT, g_Setting.tMBEYE_SET.LDWCnt);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        u8Temp = atoi(pStr);

        if ((u8Temp >= 2) && (u8Temp <= 7))
        {
            g_Setting.tMBEYE_SET.LDWCnt = u8Temp;

            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_LDW_CNT, g_Setting.tMBEYE_SET.LDWCnt);

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Input value not correct, range is 2 ~ 7.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command to set CAN MBEYE LDW time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_MBEYE_LDW_Time(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Temp;

    if (strncmp(pStr, STR_AS_LDW_TIME, strlen(STR_AS_LDW_TIME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_LDW_TIME);
        RTOSprintf("\r\n@ \t<value> = 30 ~ 180, LDW Time second.");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_LDW_TIME, g_Setting.tMBEYE_SET.LDWTime);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        u8Temp = atoi(pStr);

        if ((u8Temp >= 30) && (u8Temp <= 180))
        {
            g_Setting.tMBEYE_SET.LDWTime = u8Temp;

            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_LDW_TIME, g_Setting.tMBEYE_SET.LDWTime);

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Input value not correct, range is 30 ~ 180.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command to set CAN MBEYE HW Time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_MBEYE_HW_Time(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Temp;

    if (strncmp(pStr, STR_AS_HW_TIME, strlen(STR_AS_HW_TIME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<value>", STR_AS_HW_TIME);
        RTOSprintf("\r\n@ \t<value> = 10 ~ 120, HW Time second.");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_HW_TIME, g_Setting.tMBEYE_SET.HWTime);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        u8Temp = atoi(pStr);

        if ((u8Temp >= 10) && (u8Temp <= 120))
        {
            g_Setting.tMBEYE_SET.HWTime = u8Temp;

            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%d Set OK", STR_AS_HW_TIME, g_Setting.tMBEYE_SET.HWTime);

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Input value not correct, range is 10 ~ 120.");
        }

        break;
    }

    return bResult;
}
#endif

// Other External Information
#if HW_TYPE == HW_JAS211 || HW_TYPE == HW_JAS106 || HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
//============================================================================//
// @brief  Process the command related to EXTIO IO input reversed.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_ExinReverse(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EXIN_REVERSE, strlen(STR_AS_EXIN_REVERSE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<EXIO>,<ON/OFF>", STR_AS_EXIN_REVERSE);

        bResult = TRUE;

        break;

    case '?':
    {
        char *pMsg;

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "FDOOR",
                   (g_Setting.tReverse.bits.IN_1 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "BDOOR",
                   (g_Setting.tReverse.bits.IN_2 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "LEFT",
                   (g_Setting.tReverse.bits.IN_3 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "RIGHT",
                   (g_Setting.tReverse.bits.IN_4 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "LIGHT",
                   (g_Setting.tReverse.bits.IN_5 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "BRAKE",
                   (g_Setting.tReverse.bits.IN_7 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "IO7",
                   (g_Setting.tReverse.bits.IN_8 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "RES1",
                   (g_Setting.tReverse.bits.IN_9 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "RES2",
                   (g_Setting.tReverse.bits.IN_10 == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n@ %s%s,%s",
                   STR_AS_EXIN_REVERSE,
                   "RES3",
                   (g_Setting.tReverse.bits.IN_11 == 1) ? "ON" : "OFF");

        pMsg = szCmdMsg;

        sprintf(pMsg, "@ %s", STR_AS_EXIN_REVERSE);

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "FDOOR",
                (g_Setting.tReverse.bits.IN_1 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "BDOOR",
                (g_Setting.tReverse.bits.IN_2 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "LEFT",
                (g_Setting.tReverse.bits.IN_3 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "RIGHT",
                (g_Setting.tReverse.bits.IN_4 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "LIGHT",
                (g_Setting.tReverse.bits.IN_5 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "BRAKE",
                (g_Setting.tReverse.bits.IN_7 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "IO7",
                (g_Setting.tReverse.bits.IN_8 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "RES1",
                (g_Setting.tReverse.bits.IN_9 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "RES2",
                (g_Setting.tReverse.bits.IN_10 == 1) ? "ON" : "OFF");

        pMsg += strlen(pMsg);

        sprintf(pMsg, " %s:%s",
                "RES3",
                (g_Setting.tReverse.bits.IN_11 == 1) ? "ON" : "OFF");

        // pMsg += strlen(pMsg);

        // sprintf(pMsg, "\r\n");

        bResult = TRUE;

        break;
    }

    default:
        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        if (strcmp(pStr, "BRAKE") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_1 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_1 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, "@ %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "BRAKE",
                        (g_Setting.tReverse.bits.IN_1 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "LEFT") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_2 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_2 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "LEFT",
                        (g_Setting.tReverse.bits.IN_2 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "RIGHT") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_3 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_3 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "RIGHT",
                        (g_Setting.tReverse.bits.IN_3 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "FAR") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_4 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_4 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "FAR",
                        (g_Setting.tReverse.bits.IN_4 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "NEAR") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_5 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_5 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "NEAR",
                        (g_Setting.tReverse.bits.IN_5 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "FRONT") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_7 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_7 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "FRONT",
                        (g_Setting.tReverse.bits.IN_7 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "BACK") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_8 = 1;

                bResult = TRUE;
            }
            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_8 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "BACK", (g_Setting.tReverse.bits.IN_8 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "RES1") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_9 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {

                g_Setting.tReverse.bits.IN_9 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "RES1",
                        (g_Setting.tReverse.bits.IN_9 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "RES2") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_10 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_10 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "RES2",
                        (g_Setting.tReverse.bits.IN_10 == 1) ? "ON" : "OFF");
            }
        }

        if (strcmp(pStr, "RES3") == 0)
        {
            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tReverse.bits.IN_11 = 1;

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tReverse.bits.IN_11 = 0;

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, " %s%s:%s Set OK",
                        STR_AS_EXIN_REVERSE,
                        "RES3",
                        (g_Setting.tReverse.bits.IN_11 == 1) ? "ON" : "OFF");
            }
        }

        if (bResult == TRUE)
            System_SaveSetting();

        break;
    }

    return bResult;
}
#endif // HW_TYPE

//============================================================================//
// @brief  Process the command related to alarm beep.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AlarmBeep(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_ALARM_BEEP, strlen(STR_AS_ALARM_BEEP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<action>", STR_AS_ALARM_BEEP);
        RTOSprintf("\r\n@ \t<action>alarm beep user define type");
        RTOSprintf("\r\n@ \t1 : TYPE 1");
        RTOSprintf("\r\n@ \t2 : TYPE 2");
        RTOSprintf("\r\n@ \t3 : TYPE 3");
        RTOSprintf("\r\n@ \t4 : TYPE 4");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "1") == 0)
        {
            Buzzer_Beep(BEEP_LONG, 1);

            Buzzer_Beep(BEEP_SHORT, 2);

            bResult = TRUE;
        }

        if (strcmp(pStr, "2") == 0)
        {
            Buzzer_Beep(BEEP_SHORT, 2);

            bResult = TRUE;
        }

        if (strcmp(pStr, "3") == 0)
        {
            Buzzer_Beep(BEEP_LONG, 3);

            bResult = TRUE;
        }

        if (strcmp(pStr, "4") == 0)
        {
            Buzzer_Beep(BEEP_SHORT, 1);

            Buzzer_Beep(BEEP_LONG, 1);

            Buzzer_Beep(BEEP_SHORT, 1);

            Buzzer_Beep(BEEP_LONG, 1);

            Buzzer_Beep(BEEP_SHORT, 1);

            Buzzer_Beep(BEEP_LONG, 1);

            Buzzer_Beep(BEEP_SHORT, 1);

            Buzzer_Beep(BEEP_LONG, 1);

            bResult = TRUE;
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to correction mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CORRECT_MODE(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_CORRECT_MODE, strlen(STR_AS_CORRECT_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<PW>,<OPTION>", STR_AS_CORRECT_MODE);
        RTOSprintf("\r\n@ \t<PW>: Password");
        RTOSprintf("\r\n@ \t<OPTION>:ON/OFF");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_CORRECT_MODE,
                (g_Setting.tENABLE_SET.bits.CORRECT_MODE == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strncmp(pStr, "13091876", 8) == 0)
        {
            StopChar(pStr, ',');

            pStr += strlen(pStr) + 1;

            if (strcmp(pStr, "ON") == 0)
            {
                g_Setting.tENABLE_SET.bits.CORRECT_MODE = 1;

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CALIBRATION_ON);

                bResult = TRUE;
            }

            if (strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tENABLE_SET.bits.CORRECT_MODE = 0;

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_CALIBRATION_OFF);

                bResult = TRUE;
            }

            if (bResult == TRUE)
            {
                System_SaveSetting();

                sprintf(szCmdMsg, "@ %s%s Set OK",
                        STR_AS_CORRECT_MODE, (g_Setting.tENABLE_SET.bits.CORRECT_MODE == 1) ? "ON" : "OFF");

                RTOSprintf("\r\n%s", szCmdMsg);
            }
        }

        break;
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to anti-theft function.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Immobilizer(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_IMMOBILIZER, strlen(STR_AS_IMMOBILIZER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<OPTION>", STR_AS_IMMOBILIZER);
          RTOSprintf("\r\n@ \t<OPTION> ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_IMMOBILIZER,
                    (g_Setting.tENABLE_SET.bits.ImmoCtrl == 1) ? "ON" : "OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
            if(strcmp(pStr, "ON") == 0)
            {
                g_Setting.tENABLE_SET.bits.ImmoCtrl = 1;

                bResult = TRUE;
            }

            if(strcmp(pStr, "OFF") == 0)
            {
                g_Setting.tENABLE_SET.bits.ImmoCtrl = 0;

                bResult = TRUE;
            }

            if(bResult == TRUE)
                System_SaveSetting();

            break;
    }

    return bResult;
}
*/

// Driver Information //

//============================================================================//
// @brief  Process the command related to driver ID.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DriverId(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_DRIVER_ID, strlen(STR_AS_DRIVER_ID)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_DRIVER_ID);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_DRIVER_ID,
                (g_Setting.tENABLE_SET.bits.DRV_ID == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.tENABLE_SET.bits.DRV_ID = 1;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tENABLE_SET.bits.DRV_ID = 0;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_DRIVER_ID, (g_Setting.tENABLE_SET.bits.DRV_ID == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to driver ID number.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DriverIdNum(char *pStr)
{
    bool bResult = FALSE;
    uint8_t DrvID;

    if (strncmp(pStr, STR_AS_DRIVER_IN, strlen(STR_AS_DRIVER_IN)) != 0)
        return FALSE;

    // RTOSprintf("\r\n### CMD_DriverIdNum ###");

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<driver>,<value>", STR_AS_DRIVER_IN);
        RTOSprintf("\r\n@ \t<driver> is 1 or 2, corresponding to driver 1 or 2");
        RTOSprintf("\r\n@ \t<value> is driver's identification number");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s1,%08X;2,%08X",
                STR_AS_DRIVER_IN, g_Setting.tDrvInfo[0].u32DrvIdNum, g_Setting.tDrvInfo[1].u32DrvIdNum);

        // pMsg += strlen(pMsg);

        // sprintf(pMsg, "\r\n");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        char cRetCharList[64] = {0};

        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        DrvID = atoi(pStr);

        // RTOSprintf("\r\n### DrvID: %d ###", DrvID);

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) > 8)
        {
            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(75);

                bUSB_RunOne = FALSE;
            }
            break;
        }

        if (ChkInputText(pStr, VALID_DRIVER_ID, cRetCharList) == FALSE)
        {
            sprintf(szCmdMsg, "@ INVALID CHAR( %s)", cRetCharList);

            RTOSprintf("\r\n%s", szCmdMsg);

            RTOSprintf("\r\n@ DRIVER ID can only be set with number and letters A-F");

            if (bUSB_CheckSetupFlag == TRUE)
            {
                LCD_SetShowAutoTestPage(67);

                bUSB_RunOne = FALSE;
            }

            break;
        }

        if (1 == DrvID)
        {
            // RTOSprintf("\r\n### DrvID-1 ###");
            bResult = Info_CheckDrvId(strtoul(pStr, NULL, 16));
        }
        else if (2 == DrvID)
        {
            // RTOSprintf("\r\n### DrvID-2 ###");
            bResult = Info_CheckDrvId2(strtoul(pStr, NULL, 16));
        }
        else
        {
            RTOSprintf("\r\n> ### Input Driver ID ERROR!! ###");
        }

        if (bResult == TRUE)
        {
            RTOSprintf("> [SYS]%04X\r\n", SYS_STATE_CHANG_DRV_ID);

            sprintf(szCmdMsg, "@ %s%d,%08X Set OK",
                    STR_AS_DRIVER_IN, DrvID, g_Setting.tDrvInfo[DrvID].u32DrvIdNum);
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to driving time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DRIVE_TIME(char *pStr)
{
    bool bResult = FALSE;
    DrivingTimeTypeDef stDrivingTime;
    uint8_t u8Buf[2];
    uint32_t u32Buf;

    if (strncmp(pStr, STR_AS_DRIVE_TIME_THRESHOLD, strlen(STR_AS_DRIVE_TIME_THRESHOLD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<HOUR>,<MIN>.", STR_AS_DRIVE_TIME_THRESHOLD);
        RTOSprintf("\r\n@ \t<HOUR>: Hours in format HH");
        RTOSprintf("\r\n@ \t<MIN>: Minutes in format MM");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%02d hr %02d min.",
                STR_AS_DRIVE_TIME_THRESHOLD,
                g_Setting.tDRV_SET.u8DriveThreshold[0],
                g_Setting.tDRV_SET.u8DriveThreshold[1]);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        u8Buf[0] = atoi(pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        u8Buf[1] = atoi(pStr);

        u32Buf = System_GetTime();

        SecToBCDTime(u32Buf, stDrivingTime.u8BeforeDateBCD);

        memcpy(stDrivingTime.u8AfterDateBCD, stDrivingTime.u8BeforeDateBCD, 7);

        stDrivingTime.u8BeforeAdjustBCD[0] = Bin_2_BCD2(g_Setting.tDRV_SET.u8DriveThreshold[0]);
        stDrivingTime.u8BeforeAdjustBCD[1] = Bin_2_BCD2(g_Setting.tDRV_SET.u8DriveThreshold[1]);
        stDrivingTime.u8AfterAdjustBCD[0] = Bin_2_BCD2(u8Buf[0]);
        stDrivingTime.u8AfterAdjustBCD[1] = Bin_2_BCD2(u8Buf[1]);

        if (bSD_ReportErrFlag == FALSE)
            V161_RecordDrivingTime(stDrivingTime);

        g_Setting.tDRV_SET.u8DriveThreshold[0] = u8Buf[0];
        g_Setting.tDRV_SET.u8DriveThreshold[1] = u8Buf[1];

        bShowErrorFlag = FALSE;

        bPreWoringFlag = FALSE;

        System_SaveSetting();

        // if(g_Setting.ENABLE_SET.bits.CORRECT_MODE == 0)
        //{
        //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        // }

        sprintf(szCmdMsg, "@ %s%02d hr %02d min Set OK", STR_AS_DRIVE_TIME_THRESHOLD,
                g_Setting.tDRV_SET.u8DriveThreshold[0], g_Setting.tDRV_SET.u8DriveThreshold[1]);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to rest time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_REST_TIME(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Buf[2];
    RestDataTypeDef stRestTime;
    uint32_t u32NowSec;

    if (strncmp(pStr, STR_AS_REST_TIME_THRESHOLD, strlen(STR_AS_REST_TIME_THRESHOLD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<HOUR>,<MIN>.", STR_AS_REST_TIME_THRESHOLD);
        RTOSprintf("\r\n@ \t<HOUR>: Hours in format HH");
        RTOSprintf("\r\n@ \t<MIN>: Minutes in format MM");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%02d hr %02d min.",
                STR_AS_REST_TIME_THRESHOLD,
                g_Setting.tDRV_SET.u8RestThreshold[0],
                g_Setting.tDRV_SET.u8RestThreshold[1]);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        u8Buf[0] = atoi(pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        u8Buf[1] = atoi(pStr);

        u32NowSec = System_GetTime();

        SecToBCDTime(u32NowSec, stRestTime.u8BeforeDateBCD);

        memcpy(stRestTime.u8AfterDateBCD, stRestTime.u8BeforeDateBCD, 7);

        stRestTime.u8BeforeRestBCD[0] = Bin_2_BCD2(g_Setting.tDRV_SET.u8RestThreshold[0]);
        stRestTime.u8BeforeRestBCD[1] = Bin_2_BCD2(g_Setting.tDRV_SET.u8RestThreshold[1]);
        stRestTime.u8AfterRestBCD[0] = Bin_2_BCD2(u8Buf[0]);
        stRestTime.u8AfterRestBCD[1] = Bin_2_BCD2(u8Buf[1]);

        if (bSD_ReportErrFlag == FALSE)
            V161_RecordRestTime(stRestTime);

        g_Setting.tDRV_SET.u8RestThreshold[0] = u8Buf[0];
        g_Setting.tDRV_SET.u8RestThreshold[1] = u8Buf[1];

        // if(g_Setting.ENABLE_SET.bits.CORRECT_MODE == 0)
        //{
        //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_CAL);
        // }

        sprintf(szCmdMsg, "@ %s%02d hr %02d min Set OK", STR_AS_REST_TIME_THRESHOLD,
                g_Setting.tDRV_SET.u8RestThreshold[0], g_Setting.tDRV_SET.u8RestThreshold[1]);

        bResult = TRUE;

        break;
    }

    return bResult;
}

// GPRS Function //
#if ENABLE_GSM_NET
//============================================================================//
// @brief  Process the command related to the SIM Pin.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SimPin(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SIM_PIN, strlen(STR_AS_SIM_PIN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<SIM PIN>", STR_AS_SIM_PIN);
        RTOSprintf("\r\n@ \t<SIM PIN>:SIM PIN number, max. %d char",
                   sizeof(g_Setting.tGSM_SET.cSIM_PIN) - 1);

        bResult = TRUE;

        break;

    case '?':
        if (strlen(g_Setting.tGSM_SET.cSIM_PIN) > 0)
            sprintf(szCmdMsg, "@ %s%s", STR_AS_SIM_PIN, g_Setting.tGSM_SET.cSIM_PIN);
        else
            sprintf(szCmdMsg, "@ %sNONE", STR_AS_SIM_PIN);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strlen(pStr) >= sizeof(g_Setting.tGSM_SET.cSIM_PIN) - 1)
            break;

        strcpy(g_Setting.tGSM_SET.cSIM_PIN, pStr);

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_SIM_PIN, g_Setting.tGSM_SET.cSIM_PIN);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the GSM module IMEI.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmImei(char *pStr)
{
    bool bResult = FALSE;
    bool bRes;

    if (strncmp(pStr, STR_AS_GSM_IMEI, strlen(STR_AS_GSM_IMEI)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_IMEI);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
        if (strlen(g_Setting.tGSM_SET.tMobileInfo.cIMEI) != 15)
        {
            bRes = GSM_GetIMEI(g_Setting.tGSM_SET.tMobileInfo.cIMEI);

            System_SaveSetting();
        }
        else
        {
            bRes = TRUE;
        }

        if (bRes)
        {
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_GSM_IMEI,
                    g_Setting.tGSM_SET.tMobileInfo.cIMEI);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;
        }
        else
            bResult = FALSE;

        break;

    default:
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the GSM module IMSI.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmImsi(char *pStr)
{
    bool bResult = FALSE;
    bool bRes;

    if (strncmp(pStr, STR_AS_GSM_IMSI, strlen(STR_AS_GSM_IMSI)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_IMSI);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
        if (strlen(g_Setting.tGSM_SET.tMobileInfo.cIMSI) != 15)
        {
            bRes = GSM_GetIMSI(g_Setting.tGSM_SET.tMobileInfo.cIMSI);

            System_SaveSetting();
        }
        else
        {
            bRes = TRUE;
        }

        if (bRes)
        {
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_GSM_IMSI,
                    g_Setting.tGSM_SET.tMobileInfo.cIMSI);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;
        }
        else
            bResult = FALSE;

        break;

    default:
        bResult = FALSE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the CSQ.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmCsq(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GSM_CSQ, strlen(STR_AS_GSM_CSQ)) != 0)
        return bResult;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_CSQ);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d", STR_AS_GSM_CSQ, g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM APN.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmApn(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'}; // Heinz

    if (strncmp(pStr, STR_AS_GSM_APN, strlen(STR_AS_GSM_APN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<APN>,<User name>,<Password>", STR_AS_GSM_APN);
        RTOSprintf("\r\n@ \t<APN> max. %d char", OFFSET_SIZE(NetSetTypeDef, cAPN) - 1);
        RTOSprintf("\r\n@ \t<User name> max. %d char", OFFSET_SIZE(NetSetTypeDef, cAPN_UserName) - 1);
        RTOSprintf("\r\n@ \t<Password>  max. %d char", OFFSET_SIZE(NetSetTypeDef, cAPN_Password) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s,%s,%s",
                STR_AS_GSM_APN,
                g_Setting.tGSM_SET.tNetSet.cAPN,
                g_Setting.tGSM_SET.tNetSet.cAPN_UserName,
                g_Setting.tGSM_SET.tNetSet.cAPN_Password);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case ' ':
        memset(&g_Setting.tGSM_SET.tNetSet.cAPN, 0x00, OFFSET_SIZE(NetSetTypeDef, cAPN));
        memset(&g_Setting.tGSM_SET.tNetSet.cAPN_UserName, 0x00, OFFSET_SIZE(NetSetTypeDef, cAPN_UserName));
        memset(&g_Setting.tGSM_SET.tNetSet.cAPN_Password, 0x00, OFFSET_SIZE(NetSetTypeDef, cAPN_Password));

        System_SaveSetting();

        bTCP_CloseFlag[0] = TRUE;

#if ENABLE_MULTI_SERVER
        bTCP_CloseFlag[1] = TRUE;
#endif

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        if (strlen(pStr) >= OFFSET_SIZE(NetSetTypeDef, cAPN))
            break;

        strcpy(g_Setting.tGSM_SET.tNetSet.cAPN, pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) >= OFFSET_SIZE(NetSetTypeDef, cAPN_UserName))
            break;

        strcpy(g_Setting.tGSM_SET.tNetSet.cAPN_UserName, pStr);

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) >= OFFSET_SIZE(NetSetTypeDef, cAPN_Password))
            break;

        strcpy(g_Setting.tGSM_SET.tNetSet.cAPN_Password, pStr);

        System_SaveSetting();

        // strcpy(szTEMP, "$AS+RESET=GPRS\r\n");

        // RTOSprintf("\r\n> %s", szTEMP);

        // Protocl_ChkCmdPkg(szTEMP, NULL);

        bTCP_CloseFlag[0] = TRUE;

#if ENABLE_MULTI_SERVER
        bTCP_CloseFlag[1] = TRUE;
#endif

        sprintf(szCmdMsg, "@ %s%s,%s,%s Set OK",
                STR_AS_GSM_APN,
                g_Setting.tGSM_SET.tNetSet.cAPN,
                g_Setting.tGSM_SET.tNetSet.cAPN_UserName,
                g_Setting.tGSM_SET.tNetSet.cAPN_Password);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM Mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMode(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GSM_MODE, strlen(STR_AS_GSM_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_MODE);
        RTOSprintf("\r\n@ \t<OPTION>:?");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_GSM_MODE,
#if ENABLE_MULTI_SERVER
                (char *)"2 SERVERS"
#else
                (char *)"1 SERVER"
#endif
        );

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to server IP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmServerIp(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64]= {'\0'}; // Heinz

    if (strncmp(pStr, STR_AS_GSM_SERVER_IP, strlen(STR_AS_GSM_SERVER_IP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<IP>:<PORT>", STR_AS_GSM_SERVER_IP);
        RTOSprintf("\r\n@ \t<IP> is xxx.xxx.xxx.xxx, the xxx range of 0 to 255");
        RTOSprintf("\r\n@ \t<PORT> range of 0 to 65535");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d.%d.%d.%d:%d",
                STR_AS_GSM_SERVER_IP,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32IP_Byte[4];

        StopChar(pStr, ':');

#if ENABLE_MULTI_SERVER
        // for(int i = 1; i < MAX_SOCKET_BANK; i++)
        //{
        //  if(strncmp(pStr,
        //						 g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr,
        //						 strlen(g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr))
        //		 == 0)
        //	{
        //		RTOSprintf("\r\n@ New IP for server 0 is duplicated with server %d's IP. Cancel command!",
        //		           i);

        //		return FALSE;
        //	}
        //}
#endif

        for (int i = 0; i < 4; i++)
        {
            StopChar(pStr, '.');

            if (strlen(pStr) == 0)
                return FALSE;

            u32IP_Byte[3 - i] = atoi(pStr);

            pStr += strlen(pStr) + 1;
        }

        if ((strlen(pStr) == 0) || (strlen(pStr) > 5))
        {
            RTOSprintf("\r\n@ Port number invalid!");

            break;
        }

        for (int i = 0; i < 4; i++)
        {
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[i] = u32IP_Byte[i];
        }

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));

        sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                u32IP_Byte[3],
                u32IP_Byte[2],
                u32IP_Byte[1],
                u32IP_Byte[0]);

        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = atoi(pStr);

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, 0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));

        sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, "%s", (char *)TCP_DNS_NAME_DISABLE);

        System_SaveSetting();

        // strcpy(szTEMP, "$AS+RESET=GPRS\r\n");

        // RTOSprintf("\r\n> %s", szTEMP);

        // Protocl_ChkCmdPkg(szTEMP, NULL);

        bTCP_CloseFlag[SOCKET_BANK_DEF] = TRUE;

        sprintf(szCmdMsg, "@ %s%d.%d.%d.%d:%d Set OK",
                STR_AS_GSM_SERVER_IP,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to server name.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmServerName(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[128];	// Heinz

    if (strncmp(pStr, STR_AS_GSM_SERVER_NAME, strlen(STR_AS_GSM_SERVER_NAME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ADDRESS>:<PORT>", STR_AS_GSM_SERVER_NAME);
        RTOSprintf("\r\n@ \t<ADDRESS>:Server's domain name. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));
        RTOSprintf("\r\n@ \t<PORT>:Server's port. Range 0-65535");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s:%d",
                STR_AS_GSM_SERVER_NAME,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case ' ':
        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));

        System_SaveSetting();

        bResult = TRUE;

        break;

    default:
    {
        char cDNS_Temp[31] = {0};
        uint32_t u32Port_Temp = 0;
        uint8_t u8VarCnt;

        u8VarCnt = sscanf(pStr, "%[^:]:%d", cDNS_Temp, &u32Port_Temp);

        if (u8VarCnt != 2)
        {
            RTOSprintf("\r\n@ INVALID COMMAND FORMAT!");

            break;
        }

        if (strlen(cDNS_Temp) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName))
        {
            RTOSprintf("\r\n@ INVALID DNS!");

            break;
        }

        if (u32Port_Temp > 65535)
        {
            RTOSprintf("\r\n@ INVALID PORT!");

            break;
        }

        if ((cDNS_Temp[0] == ' ') || (cDNS_Temp[0] == NULL))
        {
            memset(cDNS_Temp, 0x00, sizeof(cDNS_Temp));

            sprintf(cDNS_Temp, "NONE");
        }

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, 0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));

        sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, "%s", cDNS_Temp);

        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = (uint16_t)u32Port_Temp;

        System_SaveSetting();

        bTCP_CloseFlag[SOCKET_BANK_DEF] = TRUE;

        sprintf(szCmdMsg, "@ %s%s:%d Set OK",
                STR_AS_GSM_SERVER_NAME,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM message headers.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmPkgHeader(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GSM_HEADER, strlen(STR_AS_GSM_HEADER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<CHK>,<LOG>,<EVENT>", STR_AS_GSM_HEADER);
        RTOSprintf("\r\n@ \t<CHK>: Chk message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));
        RTOSprintf("\r\n@ \t<LOG>: Log message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));
        RTOSprintf("\r\n@ \t<EVENT>: Event message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %sChk:%s, Log:%s, Event:%s",
                STR_AS_GSM_HEADER,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        char cChkTemp[10] = {0}, cLogTemp[10] = {0}, cEventTemp[10] = {0};

        StopChar(pStr, ',');

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk))
        {
            RTOSprintf("\r\n@ Chk header length invalid!");

            break;
        }

        strcpy(cChkTemp, pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog))
        {
            RTOSprintf("\r\n@ Log header length invalid!");

            break;
        }

        strcpy(cLogTemp, pStr);

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent))
        {
            RTOSprintf("\r\n@ Event header length invalid!");

            break;
        }

        strcpy(cEventTemp, pStr);

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));

        strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk, cChkTemp);

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));

        strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog, cLogTemp);

        memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));

        strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent, cEventTemp);

        System_SaveSetting();

        // GSM_ResetWithCheckFTP();
        // bGSM_ResetCheckFTP_Flag = TRUE;

        bTCP_CloseFlag[SOCKET_BANK_DEF] = TRUE;

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to GSM LOG enable options.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmLogEnable(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_LOG, strlen(STR_AS_GSM_LOG)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_LOG);
          RTOSprintf("\r\n@ \t<OPTION>: ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_GSM_LOG,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog = TRUE;

                System_SaveSetting();
                System_SaveBackupSetting();

                bResult = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog = FALSE;

                System_SaveSetting();
                System_SaveBackupSetting();

                bResult = TRUE;
            }
            else
            {
              RTOSprintf("\r\n@ Invalid parameter!");

                break;
            }

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_GSM_LOG,
                    g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF");

            break;
        }
    }

    return bResult;
}
*/

/*
//============================================================================//
// @brief  Process the command related to GSM message enable options.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmPkgEnable(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_PKG_EN, strlen(STR_AS_GSM_PKG_EN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<CHK EN>,<LOG EN>,<EVENT EN>", STR_AS_GSM_PKG_EN);
          RTOSprintf("\r\n@ \t<CHK>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG>: ON/OFF");
          RTOSprintf("\r\n@ \t<EVENT>: ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %sChk:%s, Log:%s, Event:%s",
                    STR_AS_GSM_PKG_EN,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent?(char *)"ON":(char *)"OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            bool bChkTemp = FALSE, bLogTemp = FALSE, bEventTemp = FALSE;


            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bChkTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bChkTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Chk enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bLogTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bLogTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Log enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bEventTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bEventTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Event enable option invalid!");

                break;
            }

            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk = bChkTemp;
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog = bLogTemp;
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent = bEventTemp;

            System_SaveSetting();

            //GSM_ResetWithCheckFTP();
            //bGSM_ResetCheckFTP_Flag = TRUE;

            bTCP_CloseFlag[SOCKET_BANK_DEF] = TRUE;

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM message sending in normal mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmSendNormal(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_SEND_NORMAL, strlen(STR_AS_GSM_SEND_NORMAL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<TIME>", STR_AS_GSM_SEND_NORMAL);
          RTOSprintf("\r\n@ \t<TIME>: Send datalog period. Unit: sec. Please choose among %d, %d, %d, %d, %d",
                     DATALOG_UPLOAD_MAX_10,
                     DATALOG_UPLOAD_MAX_15,
                     DATALOG_UPLOAD_MAX_20,
                     DATALOG_UPLOAD_MAX_30,
                     DATALOG_UPLOAD_MAX_60);

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg,
                  "@ %s%d sec",
                  STR_AS_GSM_SEND_NORMAL,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            uint16_t u16ValueTemp;


            u16ValueTemp = atoi(pStr);

            if((u16ValueTemp != DATALOG_UPLOAD_MAX_10) &&
                 (u16ValueTemp != DATALOG_UPLOAD_MAX_15) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_20) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_30) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_60))
            {
                RTOSprintf("\r\n@ Invalid time value!");

                break;
            }

            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod = u16ValueTemp;

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

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

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM message sending in standby mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmSendStandby(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_SEND_STANDBY, strlen(STR_AS_GSM_SEND_STANDBY)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<TIME>", STR_AS_GSM_SEND_STANDBY);
            RTOSprintf("\r\n@ \t<TIME> range of %d to %d seconds", GSM_SEND_STANDBY_MIN, GSM_SEND_STANDBY_MAX);

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%d sec", STR_AS_GSM_SEND_STANDBY, g_Setting.tGSM_SET.tNetSet.u16SendStandby);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            uint32_t u32ValueTemp;


            u32ValueTemp = atoi(pStr);

            if(u32ValueTemp &&
                 ((u32ValueTemp > GSM_SEND_STANDBY_MAX) ||
                (u32ValueTemp < GSM_SEND_STANDBY_MIN)))
            {
                break;
            }
            else if(u32ValueTemp == 0)
                break;

            g_Setting.tGSM_SET.tNetSet.u16SendStandby = u32ValueTemp;

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

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

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM server information.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmServerInfo(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_SERVER_INFO, strlen(STR_AS_GSM_SERVER_INFO)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<IP>:<PORT>,<CHK HEAD>:<CHK EN>,<LOG HEAD>:<LOG EN>,"\
                     "<EVENT HEAD>:<EVENT EN>,<LOG TIME>",
                     STR_AS_GSM_SERVER_INFO);
          RTOSprintf("\r\n@ \t<IP> is xxx.xxx.xxx.xxx, the xxx range of 0 to 255");
            RTOSprintf("\r\n@ \t<PORT> range of 0 to 65535");
          RTOSprintf("\r\n@ \t<CHK HEAD>: Chk message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));
          RTOSprintf("\r\n@ \t<CHK EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG HEAD>: Log message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));
          RTOSprintf("\r\n@ \t<LOG EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<EVENT HEAD>: Event message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));
          RTOSprintf("\r\n@ \t<EVENT EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG TIME>: Send datalog period. Unit: sec. Please choose among %d, %d, %d, %d, %d",
                     DATALOG_UPLOAD_MAX_10,
                     DATALOG_UPLOAD_MAX_15,
                     DATALOG_UPLOAD_MAX_20,
                     DATALOG_UPLOAD_MAX_30,
                     DATALOG_UPLOAD_MAX_60);

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%s:%d, Chk:%s(%s), Log:%s(%s), Event:%s(%s), Log time:%d sec",
                    STR_AS_GSM_SERVER_INFO,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk?(char *)"ON":(char *)"OFF",
                            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF",
                            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent?(char *)"ON":(char *)"OFF",
                          g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            uint32_t u32IP_Byte[4];
            uint16_t u16PortTemp;
            char cChkTemp[10] = {0}, cLogTemp[10] = {0}, cEventTemp[10] = {0};
            bool bChkTemp = FALSE, bLogTemp = FALSE, bEventTemp = FALSE;
          uint8_t u8TimeTemp;


            StopChar(pStr, ':');

            #if ENABLE_MULTI_SERVER
      //for(int i = 1; i < MAX_SOCKET_BANK; i++)
            //{
        //  if(strncmp(pStr,
            //						 g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr,
            //						 strlen(g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr))
            //		 == 0)
            //	{
            //		RTOSprintf("\r\n@ New IP for server 0 is duplicated with server %d's IP. Cancel command!",
            //		           i);

            //		return FALSE;
            //	}
          //}
            #endif

            for(int i = 0; i < 4; i++)
            {
                StopChar(pStr, '.');

                if(strlen(pStr) == 0)
                    return FALSE;

                u32IP_Byte[3 - i] = atoi(pStr);

                pStr += strlen(pStr) + 1;
            }

            StopChar(pStr, ',');

            if((strlen(pStr) == 0) || (strlen(pStr) > 5))
            {
                RTOSprintf("\r\n@ Port number invalid!");

                break;
            }

            u16PortTemp = atoi(pStr);

            pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk))
            {
                RTOSprintf("\r\n@ Chk header length invalid!");

                break;
      }

            strcpy(cChkTemp, pStr);

        pStr += strlen(pStr) + 1;

      StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bChkTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bChkTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Chk enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog))
            {
                RTOSprintf("\r\n@ Log header length invalid!");

                break;
            }

            strcpy(cLogTemp, pStr);

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bLogTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bLogTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Log enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent))
            {
                RTOSprintf("\r\n@ Event header length invalid!");

                break;
            }

            strcpy(cEventTemp, pStr);

            pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bEventTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bEventTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Event enable option invalid!");

                break;
            }

            pStr += strlen(pStr) + 1;

            u8TimeTemp = atoi(pStr);

            if((u8TimeTemp != DATALOG_UPLOAD_MAX_10) &&
                 (u8TimeTemp != DATALOG_UPLOAD_MAX_15) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_20) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_30) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_60))
            {
                RTOSprintf("\r\n@ Invalid time value!");

                break;
            }

            // Copy data to g_Setting
            for(int i = 0; i < 4; i++)
            {
              g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[i] = u32IP_Byte[i];
            }

            memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));

            sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                    u32IP_Byte[3],
                    u32IP_Byte[2],
                    u32IP_Byte[1],
                    u32IP_Byte[0]);

            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = u16PortTemp;

            memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));

            strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk, cChkTemp);

            memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));

            strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog, cLogTemp);

            memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));

            strcpy(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent, cEventTemp);

        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk = bChkTemp;
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog = bLogTemp;
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent = bEventTemp;

            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod = u8TimeTemp;

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

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

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to resend datalog to default server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GetDatalog(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_GET_DATALOG, strlen(STR_AS_GET_DATALOG)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<TIME BEGIN>,<TIME END>", STR_AS_GET_DATALOG);
        RTOSprintf("\r\n@ \t<TIME BEGIN> and <TIME END> format is YYMMDDhhmmss");

        bResult = TRUE;

        break;

    default:
    {
        char BufStr[16];
        struct tm timeinfo;
        time_t rawtime;
        time_t ttBegin, ttEnd;

        StopChar(pStr, ',');

        if (strlen(pStr) != 12)
            break;

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttBegin = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttBegin -= g_Setting.i32TimeZoneOffset;

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) != 12)
        {
            break;
        }

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttEnd = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttEnd -= g_Setting.i32TimeZoneOffset;

        RTOSprintf("\r\n@ CMD_GetDatalog ttBegin: %d, ttEnd: %d", ttBegin, ttEnd);

        if (ttBegin > ttEnd)
        {
            RTOSprintf("\r\n@ Input Start time more than the ttStop time, Error.");

            bResult = FALSE;

            break;
        }

        if ((ttEnd - ttBegin) > HOUR_2_SECOND(1))
        {
            RTOSprintf("\r\n@ Input time more than the 1 hour, too long. ");

            bResult = FALSE;

            break;
        }

        if ((ttBegin / DAY_2_SECOND(1)) != (ttEnd / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n@ Input time not the same day, Error. ");

            bResult = FALSE;

            break;
        }

        bResult = Datalog_ResendLog(ttBegin, ttEnd, SOCKET_BANK_DEF);
    }

    break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to resend event to default server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GetEvent(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_GET_EVENT, strlen(STR_AS_GET_EVENT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<TIME BEGIN>,<TIME END>", STR_AS_GET_EVENT);
        RTOSprintf("\r\n@ \t<TIME BEGIN> and <TIME END> format is YYMMDDhhmmss");

        bResult = TRUE;

        break;

    default:
    {
        char BufStr[16];
        struct tm timeinfo = {0};
        time_t rawtime;
        time_t ttBegin, ttEnd;

        StopChar(pStr, ',');

        if (strlen(pStr) != 12)
            break;

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttBegin = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttBegin -= g_Setting.i32TimeZoneOffset;

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) != 12)
        {
            break;
        }

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttEnd = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttEnd -= g_Setting.i32TimeZoneOffset;

        RTOSprintf("\r\n@ CMD_GetEvent ttBegin: %d, ttEnd: %d", ttBegin, ttEnd);

        if (ttBegin > ttEnd)
        {
            RTOSprintf("\r\n@ Input Start time more than the ttStop time, Error. ");

            bResult = FALSE;

            break;
        }

        if ((ttEnd - ttBegin) > HOUR_2_SECOND(1))
        {
            RTOSprintf("\r\n@ Input time more than the 1 hour, too long. ");

            bResult = FALSE;

            break;
        }

        if ((ttBegin / DAY_2_SECOND(1)) != (ttEnd / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n@ Input time not the same day, Error. ");

            bResult = FALSE;

            break;
        }

        bResult = Datalog_ResendEvent(ttBegin, ttEnd, SOCKET_BANK_DEF);
    }

    break;
    }

    return bResult;
}

#if ENABLE_MULTI_SERVER
//============================================================================//
// @brief  Process the command related to the multiple IPs mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiIp(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64]= {'\r'}; // Heinz

    if (strncmp(pStr, STR_AS_GSM_MULTI_IP, strlen(STR_AS_GSM_MULTI_IP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<SOCKET>,<IP>:<PORT>", STR_AS_GSM_MULTI_IP);
        RTOSprintf("\r\n@ \t<SOCKET> is socket number, the range of 0 to %d", MAX_SOCKET_BANK - 1);
        RTOSprintf("\r\n@ \t<IP> is xxx.xxx.xxx.xxx, the xxx range of 0 to 255");
        RTOSprintf("\r\n@ \t<PORT> range of 0 to 65535");

        bResult = TRUE;

        break;

    case '?':
    {
        char *pMsg;

        // RTOSprintf("\r\n@ %s%d,%d.%d.%d.%d:%d",
        //					 STR_AS_GSM_MULTI_IP,
        //					 SOCKET_BANK_DEF,
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        // RTOSprintf("\r\n@ %s%d,%d.%d.%d.%d:%d",
        //					 STR_AS_GSM_MULTI_IP,
        //					 SOCKET_BANK_EXT,
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0],
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port);

        pMsg = szCmdMsg;

        sprintf(pMsg, "@ %s", STR_AS_GSM_MULTI_IP);

        pMsg += strlen(pMsg);

        sprintf(pMsg, " IP %d:%d.%d.%d.%d:%d",
                SOCKET_BANK_DEF,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        pMsg += strlen(pMsg);

        sprintf(pMsg, "; IP %d:%d.%d.%d.%d:%d",
                SOCKET_BANK_EXT,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }

    default:
    {
        uint8_t SocketNum;
        uint32_t u32IP_Byte[4] = {0};

        StopChar(pStr, ',');

        if (strlen(pStr) == 0)
            break;

        SocketNum = atoi(pStr);

        if (SocketNum >= MAX_SOCKET_BANK)
            break;

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ':');

#if ENABLE_MULTI_SERVER
        // for(int i = 0; i < MAX_SOCKET_BANK; i++)
        //{
        //	if(i == SocketNum)
        //		continue;

        //	if(strncmp(pStr,
        //						 g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr,
        //						 strlen(g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr))
        //		 == 0)
        //	{
        //		RTOSprintf("\r\n@ New IP for server %d is duplicated with server %d's IP. Cancel command!",
        //		           SocketNum,
        //		           i);

        //		return FALSE;
        //	}
        //}
#endif

        for (int i = 0; i < 4; i++)
        {
            StopChar(pStr, '.');

            if (strlen(pStr) == 0)
                break;

            u32IP_Byte[3 - i] = atoi(pStr);

            pStr += strlen(pStr) + 1;
        }

        if ((strlen(pStr) == 0) || (strlen(pStr) > 5))
        {
            RTOSprintf("\r\n@ Port number invalid!");

            break;
        }

        for (int i = 0; i < 4; i++)
        {
            g_Setting.tGSM_SET.tServerInfo[SocketNum].u32IP_Addr._U8[i] = u32IP_Byte[i];
        }

        memset(g_Setting.tGSM_SET.tServerInfo[SocketNum].cIP_Addr,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SocketNum].cIP_Addr));

        sprintf(g_Setting.tGSM_SET.tServerInfo[SocketNum].cIP_Addr, "%d.%d.%d.%d",
                u32IP_Byte[3],
                u32IP_Byte[2],
                u32IP_Byte[1],
                u32IP_Byte[0]);

        g_Setting.tGSM_SET.tServerInfo[SocketNum].u16Port = atoi(pStr);

        memset(g_Setting.tGSM_SET.tServerInfo[SocketNum].cDomainName, 0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[SocketNum].cDomainName));

        sprintf(g_Setting.tGSM_SET.tServerInfo[SocketNum].cDomainName, "%s", (char *)TCP_DNS_NAME_DISABLE);

        System_SaveSetting();

        // strcpy(szTEMP, "$AS+RESET=GPRS\r\n");

        // RTOSprintf("\r\n> %s", szTEMP);

        // Protocl_ChkCmdPkg(szTEMP, NULL);

        bTCP_CloseFlag[SocketNum] = TRUE;

        sprintf(szCmdMsg, "@ %sIP %d:%d.%d.%d.%d:%d Set OK",
                STR_AS_GSM_MULTI_IP,
                SocketNum,
                g_Setting.tGSM_SET.tServerInfo[SocketNum].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SocketNum].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SocketNum].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SocketNum].u32IP_Addr._U8[0],
                g_Setting.tGSM_SET.tServerInfo[SocketNum].u16Port);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to DNS in multiple IPs mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiDns(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64]= {'\r'};

    if (strncmp(pStr, STR_AS_GSM_MULTI_DNS, strlen(STR_AS_GSM_MULTI_DNS)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<socket>,<address>:<port>", STR_AS_GSM_MULTI_DNS);
        RTOSprintf("\r\n@ \t<socket> is socket number, the range of 0 to %d", MAX_SOCKET_BANK - 1);
        RTOSprintf("\r\n@ \t<address> is server's domain name. max %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[0].cDomainName));
        RTOSprintf("\r\n@ \t<port> is server's port number, the range of 0 to %d", sizeof(uint16_t));

        bResult = TRUE;

        break;

    case '?':
    {
        char *pMsg;

        // RTOSprintf("@ %s%d:%s\r\n",
        //					 STR_AS_GSM_MULTI_DNS,
        //					 SOCKET_BANK_DEF,
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);

        // RTOSprintf("@ %s%d:%s\r\n",
        //					 STR_AS_GSM_MULTI_DNS,
        //					 SOCKET_BANK_EXT,
        //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);

        pMsg = szCmdMsg;

        sprintf(pMsg, "@%s", STR_AS_GSM_MULTI_DNS);

        pMsg += strlen(pMsg);

        sprintf(pMsg, " DNS %d:%s:%d",
                SOCKET_BANK_DEF,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);

        pMsg += strlen(pMsg);

        sprintf(pMsg, "; DNS %d:%s:%d",
                SOCKET_BANK_EXT,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }

    default:
    {
        uint32_t u32SocketNum = 0;
        char cDNS_Temp[31] = {0};
        uint32_t u32Port_Temp = 0;
        uint8_t u8VarCnt;

        u8VarCnt = sscanf(pStr, "%d,%[^:]:%d", &u32SocketNum, cDNS_Temp, &u32Port_Temp);

        if (u8VarCnt != 3)
        {
            RTOSprintf("\r\n@ INVALID COMMAND FORMAT!");

            break;
        }

        if (u32SocketNum > (MAX_SOCKET_BANK - 1))
        {
            RTOSprintf("\r\n@ INVALID SERVER ID!");

            break;
        }

        if (strlen(cDNS_Temp) > sizeof(g_Setting.tGSM_SET.tServerInfo[u32SocketNum].cDomainName))
        {
            RTOSprintf("\r\n@ INVALID DNS!");

            break;
        }

        if (u32Port_Temp > 65535)
        {
            RTOSprintf("\r\n@ INVALID PORT!");

            break;
        }

        if ((cDNS_Temp[0] == ' ') || (cDNS_Temp[0] == NULL))
        {
            memset(cDNS_Temp, 0x00, sizeof(cDNS_Temp));

            sprintf(cDNS_Temp, "NONE");
        }

        memset(g_Setting.tGSM_SET.tServerInfo[u32SocketNum].cDomainName, 0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[u32SocketNum].cDomainName));

        sprintf(g_Setting.tGSM_SET.tServerInfo[u32SocketNum].cDomainName, "%s", cDNS_Temp);

        g_Setting.tGSM_SET.tServerInfo[u32SocketNum].u16Port = (uint16_t)u32Port_Temp;

        System_SaveSetting();

        bTCP_CloseFlag[u32SocketNum] = TRUE;

        sprintf(szCmdMsg, "@ %s%d,%s:%d Set OK",
                STR_AS_GSM_MULTI_DNS,
                u32SocketNum,
                g_Setting.tGSM_SET.tServerInfo[u32SocketNum].cDomainName,
                g_Setting.tGSM_SET.tServerInfo[u32SocketNum].u16Port);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM message headers in multi server mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiHeader(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GSM_MULTI_HEADER, strlen(STR_AS_GSM_MULTI_HEADER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ID>,<CHK>,<LOG>,<EVENT>", STR_AS_GSM_MULTI_HEADER);
        RTOSprintf("\r\n@ \t<ID> Server ID, 0 or 1");
        RTOSprintf("\r\n@ \t<CHK>: Chk message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));
        RTOSprintf("\r\n@ \t<LOG>: Log message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));
        RTOSprintf("\r\n@ \t<EVENT>: Event message header. max. %d char",
                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %sServer %d: Chk:%s, Log:%s, Event:%s; Server %d: Chk:%s, Log:%s, Event:%s ",
                STR_AS_GSM_MULTI_HEADER,
                SOCKET_BANK_DEF,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent,
                SOCKET_BANK_EXT,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderChk,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderLog,
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderEvent);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8ServerID;
        char cChkTemp[10] = {0}, cLogTemp[10] = {0}, cEventTemp[10] = {0};

        StopChar(pStr, ',');

        if (strlen(pStr) != 1)
        {
            RTOSprintf("\r\n@ Invalid server ID!");

            break;
        }

        u8ServerID = atoi(pStr);

        if (u8ServerID > MAX_SOCKET_BANK)
        {
            RTOSprintf("\r\n@ Invalid server ID!");

            break;
        }

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk))
        {
            RTOSprintf("\r\n@ Chk header length invalid!");

            break;
        }

        strcpy(cChkTemp, pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog))
        {
            RTOSprintf("\r\n@ Log header length invalid!");

            break;
        }

        strcpy(cLogTemp, pStr);

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent))
        {
            RTOSprintf("\r\n@ Event header length invalid!");

            break;
        }

        strcpy(cEventTemp, pStr);

        memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk));

        strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk, cChkTemp);

        memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog));

        strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog, cLogTemp);

        memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent,
               0x00,
               sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent));

        strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent, cEventTemp);

        System_SaveSetting();

        // GSM_ResetWithCheckFTP();
        // bGSM_ResetCheckFTP_Flag = TRUE;

        bTCP_CloseFlag[u8ServerID] = TRUE;

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM external socket enable/disable.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSM_SocketEXT(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_GSM_SOCKET_EXT, strlen(STR_AS_GSM_SOCKET_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_GSM_SOCKET_EXT);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_GSM_SOCKET_EXT,
                (g_Setting.tGSM_SET.bDisableSocketEXT == FALSE) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.tGSM_SET.bDisableSocketEXT = FALSE;

            bResult = TRUE;
        }

        if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tGSM_SET.bDisableSocketEXT = TRUE;

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            System_SaveSetting();

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_GSM_SOCKET_EXT, (g_Setting.tGSM_SET.bDisableSocketEXT == FALSE) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to GSM message enable options in
//         multi server mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiPkgEnable(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_MULTI_EN, strlen(STR_AS_GSM_MULTI_EN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<ID>,<CHK EN>,<LOG EN>,<EVENT EN>", STR_AS_GSM_MULTI_EN);
          RTOSprintf("\r\n@ \t<ID> Server ID, 0 or 1");
          RTOSprintf("\r\n@ \t<CHK>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG>: ON/OFF");
          RTOSprintf("\r\n@ \t<EVENT>: ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %sServer %d: Chk:%s, Log:%s, Event:%s; Server %d: Chk:%s, Log:%s, Event:%s",
                    STR_AS_GSM_MULTI_EN,
                  SOCKET_BANK_DEF,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent?(char *)"ON":(char *)"OFF",
                            SOCKET_BANK_EXT,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableChk?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog?(char *)"ON":(char *)"OFF",
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableEvent?(char *)"ON":(char *)"OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            uint8_t u8ServerID;
            bool bChkTemp = FALSE, bLogTemp = FALSE, bEventTemp = FALSE;


            StopChar(pStr, ',');

            if(strlen(pStr) != 1)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            u8ServerID = atoi(pStr);

            if(u8ServerID > MAX_SOCKET_BANK)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bChkTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bChkTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Chk enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bLogTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bLogTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Log enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bEventTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bEventTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Event enable option invalid!");

                break;
            }

            g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableChk = bChkTemp;
            g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableLog = bLogTemp;
            g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableEvent = bEventTemp;

            System_SaveSetting();

            //GSM_ResetWithCheckFTP();
            //bGSM_ResetCheckFTP_Flag = TRUE;
            bTCP_CloseFlag[u8ServerID] = TRUE;

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM message sending in normal mode
//         when multiple servers mode is used.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiSend(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_MULTI_SEND, strlen(STR_AS_GSM_MULTI_SEND)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<ID>,<TIME>", STR_AS_GSM_MULTI_SEND);
          RTOSprintf("\r\n@ \t<ID>: Server ID, 0 or 1.");
          RTOSprintf("\r\n@ \t<TIME>: Send datalog period. Unit: sec. Please choose among %d, %d, %d, %d, %d",
                     DATALOG_UPLOAD_MAX_10,
                     DATALOG_UPLOAD_MAX_15,
                     DATALOG_UPLOAD_MAX_20,
                     DATALOG_UPLOAD_MAX_30,
                     DATALOG_UPLOAD_MAX_60);

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg,
                  "@ %sServer 0: %d sec, Server 1: %d sec",
                  STR_AS_GSM_MULTI_SEND,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u8LogPeriod);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            uint8_t u8ServerID;
            uint16_t u16ValueTemp;


            StopChar(pStr, ',');

            if(strlen(pStr) != 1)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            u8ServerID = atoi(pStr);

            if(u8ServerID > MAX_SOCKET_BANK - 1)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            pStr += strlen(pStr) + 1;

            u16ValueTemp = atoi(pStr);

            if((u16ValueTemp != DATALOG_UPLOAD_MAX_10) &&
                 (u16ValueTemp != DATALOG_UPLOAD_MAX_15) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_20) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_30) &&
               (u16ValueTemp != DATALOG_UPLOAD_MAX_60))
            {
                RTOSprintf("\r\n@ Invalid time value!");

                break;
            }

            g_Setting.tGSM_SET.tServerInfo[u8ServerID].u8LogPeriod = u16ValueTemp;

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

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

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM server information in multi server mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmMultiServerInfo(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_MULTI_INFO, strlen(STR_AS_GSM_MULTI_INFO)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
        case '?':
            RTOSprintf("\r\n@ QUERY: %s<ID>", STR_AS_GSM_MULTI_INFO);
          RTOSprintf("\r\n@ \t<ID>: Server ID, 0 or 1");
          RTOSprintf("\r\n@");
            RTOSprintf("\r\n@ SET: %s<ID>,<IP>:<PORT>,<CHK HEAD>:<CHK EN>,<LOG HEAD>:<LOG EN>,"\
                     "<EVENT HEAD>:<EVENT EN>,<LOG TIME>",
                     STR_AS_GSM_MULTI_INFO);
          RTOSprintf("\r\n@ \t<ID>: Server ID, 0 or 1");
          RTOSprintf("\r\n@ \t<IP> is xxx.xxx.xxx.xxx, the xxx range of 0 to 255");
            RTOSprintf("\r\n@ \t<PORT> is range of 0 to 65535");
          RTOSprintf("\r\n@ \t<CHK HEAD>: Chk message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk));
          RTOSprintf("\r\n@ \t<CHK EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG HEAD>: Log message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog));
          RTOSprintf("\r\n@ \t<LOG EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<EVENT HEAD>: Event message header. max. %d char",
                     sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent));
          RTOSprintf("\r\n@ \t<EVENT EN>: ON/OFF");
          RTOSprintf("\r\n@ \t<LOG TIME>: Send datalog period. Unit: sec. Please choose among %d, %d, %d, %d, %d",
                     DATALOG_UPLOAD_MAX_10,
                     DATALOG_UPLOAD_MAX_15,
                     DATALOG_UPLOAD_MAX_20,
                     DATALOG_UPLOAD_MAX_30,
                     DATALOG_UPLOAD_MAX_60);

            bResult = TRUE;

            break;

            //sprintf(szCmdMsg,
          //        "@ %sServer %d: %s:%d, Chk:%s(%s), Log:%s(%s), Event:%s(%s), Log time:%d sec; " \
          //        "\r\nServer %d: %s:%d, Chk:%s(%s), Log:%s(%s), Event:%s(%s), Log time:%d sec",
            //        STR_AS_GSM_MULTI_INFO,
          //        SOCKET_BANK_DEF,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderChk,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableChk?(char *)"ON":(char *)"OFF",
            //				g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderLog,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableLog?(char *)"ON":(char *)"OFF",
            //				g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cHeaderEvent,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].bEnableEvent?(char *)"ON":(char *)"OFF",
            //			  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u8LogPeriod,
            //				SOCKET_BANK_EXT,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderChk,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableChk?(char *)"ON":(char *)"OFF",
            //				g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderLog,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog?(char *)"ON":(char *)"OFF",
            //				g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cHeaderEvent,
          //        g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableEvent?(char *)"ON":(char *)"OFF",
            //			  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u8LogPeriod);

        default:
        {
            uint8_t u8ServerID;
            uint32_t u32IP_Byte[4];
            uint16_t u16PortTemp;
            char cChkTemp[10] = {0}, cLogTemp[10] = {0}, cEventTemp[10] = {0};
            bool bChkTemp = FALSE, bLogTemp = FALSE, bEventTemp = FALSE;
          uint8_t u8TimeTemp;


            StopChar(pStr, ',');

            if(strlen(pStr) != 1)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            u8ServerID = atoi(pStr);

            if(u8ServerID > MAX_SOCKET_BANK - 1)
            {
              RTOSprintf("\r\n@ Invalid server ID!");

                break;
            }

            pStr += strlen(pStr) + 1;

            if(strlen(pStr) == 0)
            {
              sprintf(szCmdMsg,
                                "@ %sServer %d: %s:%d, Chk:%s(%s), Log:%s(%s), Event:%s(%s), Log time:%d sec",
                                STR_AS_GSM_MULTI_INFO,
                                u8ServerID,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].cIP_Addr,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].u16Port,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableChk?(char *)"ON":(char *)"OFF",
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableLog?(char *)"ON":(char *)"OFF",
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent,
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableEvent?(char *)"ON":(char *)"OFF",
                                g_Setting.tGSM_SET.tServerInfo[u8ServerID].u8LogPeriod);

                RTOSprintf("\r\n%s", szCmdMsg);

                bResult = TRUE;

                break;
            }

            StopChar(pStr, ':');

            //for(int i = 0; i < MAX_SOCKET_BANK; i++)
            //{
            //	if(i == u8ServerID)
            //		continue;

            //	if(strncmp(pStr,
            //						 g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr,
            //						 strlen(g_Setting.tGSM_SET.tServerInfo[i].cIP_Addr))
            //		 == 0)
            //	{
            //		RTOSprintf("\r\n@ New IP for server %d is duplicated with server %d's IP. Cancel command!",
            //		           u8ServerID,
            //		           i);
            //
            //		return FALSE;
            //	}
          //}

            for(int i = 0; i < 4; i++)
            {
                StopChar(pStr, '.');

                if(strlen(pStr) == 0)
                    return FALSE;

                u32IP_Byte[3 - i] = atoi(pStr);

                pStr += strlen(pStr) + 1;
            }

            StopChar(pStr, ',');

            if((strlen(pStr) == 0) || (strlen(pStr) > 5))
            {
                RTOSprintf("\r\n@ Port number invalid!");

                break;
            }

            u16PortTemp = atoi(pStr);

            pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk))
            {
                RTOSprintf("\r\n@ Chk header length invalid!");

                break;
      }

            strcpy(cChkTemp, pStr);

        pStr += strlen(pStr) + 1;

      StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bChkTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bChkTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Chk enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog))
            {
                RTOSprintf("\r\n@ Log header length invalid!");

                break;
            }

            strcpy(cLogTemp, pStr);

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bLogTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bLogTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Log enable option invalid!");

                break;
            }

        pStr += strlen(pStr) + 1;

            StopChar(pStr, ':');

            if(strlen(pStr) > sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent))
            {
                RTOSprintf("\r\n@ Event header length invalid!");

                break;
            }

            strcpy(cEventTemp, pStr);

            pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              bEventTemp = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              bEventTemp = FALSE;
            }
            else
            {
              RTOSprintf("\r\n@ Event enable option invalid!");

                break;
            }

            pStr += strlen(pStr) + 1;

            u8TimeTemp = atoi(pStr);

            if((u8TimeTemp != DATALOG_UPLOAD_MAX_10) &&
                 (u8TimeTemp != DATALOG_UPLOAD_MAX_15) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_20) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_30) &&
               (u8TimeTemp != DATALOG_UPLOAD_MAX_60))
            {
                RTOSprintf("\r\n@ Invalid time value!");

                break;
            }

            // Copy data to g_Setting
            for(int i = 0; i < 4; i++)
            {
              g_Setting.tGSM_SET.tServerInfo[u8ServerID].u32IP_Addr._U8[i] = u32IP_Byte[i];
            }

            memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cIP_Addr,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cIP_Addr));

            sprintf(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cIP_Addr, "%d.%d.%d.%d",
                    u32IP_Byte[3],
                    u32IP_Byte[2],
                    u32IP_Byte[1],
                    u32IP_Byte[0]);

            g_Setting.tGSM_SET.tServerInfo[u8ServerID].u16Port = u16PortTemp;

            memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk));

            strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderChk, cChkTemp);

            memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog));

            strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderLog, cLogTemp);

            memset(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent,
                   0x00,
                   sizeof(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent));

            strcpy(g_Setting.tGSM_SET.tServerInfo[u8ServerID].cHeaderEvent, cEventTemp);

        g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableChk = bChkTemp;
            g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableLog = bLogTemp;
            g_Setting.tGSM_SET.tServerInfo[u8ServerID].bEnableEvent = bEventTemp;

            g_Setting.tGSM_SET.tServerInfo[u8ServerID].u8LogPeriod = u8TimeTemp;

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

            #if ENABLE_GSM_NET
            QtEC21_CheckRedoFTP();
            #endif

            if(bRTOS_En)
            {
                osMutexWait(mutex_setting_user, S_2_mS(5));
            }

            System_SaveSettingFinal();
        System_SaveBackupSettingAll(VAR_ALL_ITEMS);

            System_Reset();

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}
*/

/*
//============================================================================//
// @brief  Process the command related to GSM LOG EXT enable options.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmLogExtEnable(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_GSM_LOG_EXT, strlen(STR_AS_GSM_LOG_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<OPTION>", STR_AS_GSM_LOG_EXT);
          RTOSprintf("\r\n@ \t<OPTION>: ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_GSM_LOG_EXT,
                  g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog?(char *)"ON":(char *)"OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
        {
            if(strncmp(pStr, (char *)"ON", 2) == 0)
            {
              g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog = TRUE;

                System_SaveSetting();
                System_SaveBackupSetting();

                bResult = TRUE;
            }
            else if(strncmp(pStr, (char *)"OFF", 3) == 0)
            {
              g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog = FALSE;

                System_SaveSetting();
                System_SaveBackupSetting();

                bResult = TRUE;
            }
            else
            {
              RTOSprintf("\r\n@ Invalid parameter!");

                break;
            }

            sprintf(szCmdMsg, "@ %s%s Set OK",
                    STR_AS_GSM_LOG_EXT,
                    g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].bEnableLog?(char *)"ON":(char *)"OFF");

            break;
        }
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to resend datalog to extended server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GetDatalogExt(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GET_DATALOG_EXT, strlen(STR_AS_GET_DATALOG_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<TIME BEGIN>,<TIME END>", STR_AS_GET_DATALOG_EXT);
        RTOSprintf("\r\n@ \t<TIME BEGIN> and <TIME END> format is YYMMDDhhmmss");

        bResult = TRUE;

        break;

    default:
    {
        char BufStr[16];
        struct tm timeinfo = {0};
        time_t rawtime;
        time_t ttBegin, ttEnd;

        StopChar(pStr, ',');

        if (strlen(pStr) != 12)
            break;

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttBegin = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttBegin -= g_Setting.i32TimeZoneOffset;

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) != 12)
        {
            break;
        }

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttEnd = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttEnd -= g_Setting.i32TimeZoneOffset;

        RTOSprintf("\r\n@ CMD_GetDatalog ttBegin: %d, ttEnd: %d", ttBegin, ttEnd);

        if (ttBegin > ttEnd)
        {
            RTOSprintf("\r\n@ Input Start time more than the ttStop time, Error.");

            bResult = FALSE;

            break;
        }

        if ((ttEnd - ttBegin) > HOUR_2_SECOND(1))
        {
            RTOSprintf("\r\n@ Input time more than the 1 hour, too long. ");

            bResult = FALSE;

            break;
        }

        if ((ttBegin / DAY_2_SECOND(1)) != (ttEnd / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n@ Input time not the same day, Error. ");

            bResult = FALSE;

            break;
        }

        bResult = Datalog_ResendLog(ttBegin, ttEnd, SOCKET_BANK_EXT);
    }

    break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to resend event to extended server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GetEventExt(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_GET_EVENT_EXT, strlen(STR_AS_GET_EVENT_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<TIME BEGIN>,<TIME END>", STR_AS_GET_EVENT_EXT);
        RTOSprintf("\r\n@ \t<TIME BEGIN> and <TIME END> format is YYMMDDhhmmss");

        bResult = TRUE;

        break;

    default:
    {
        char BufStr[16];
        struct tm timeinfo = {0};
        time_t rawtime;
        time_t ttBegin, ttEnd;

        StopChar(pStr, ',');

        if (strlen(pStr) != 12)
            break;

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttBegin = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttBegin -= g_Setting.i32TimeZoneOffset;

        pStr += strlen(pStr) + 1;

        if (strlen(pStr) != 12)
        {
            break;
        }

        localtime_r(&rawtime, &timeinfo);

        strncpy(BufStr, pStr, 2);
        timeinfo.tm_year = atoi(BufStr) + 100;

        strncpy(BufStr, pStr + 2, 2);
        timeinfo.tm_mon = atoi(BufStr) - 1;

        strncpy(BufStr, pStr + 4, 2);
        timeinfo.tm_mday = atoi(BufStr);

        strncpy(BufStr, pStr + 6, 2);
        timeinfo.tm_hour = atoi(BufStr);

        strncpy(BufStr, pStr + 8, 2);
        timeinfo.tm_min = atoi(BufStr);

        strncpy(BufStr, pStr + 10, 2);
        timeinfo.tm_sec = atoi(BufStr);

        ttEnd = mktime(&timeinfo);

        // ���o UTC �ɶ�
        ttEnd -= g_Setting.i32TimeZoneOffset;

        RTOSprintf("\r\n@ CMD_GetEvent ttBegin: %d, ttEnd: %d", ttBegin, ttEnd);

        if (ttBegin > ttEnd)
        {
            RTOSprintf("\r\n@ Input Start time more than the ttStop time, Error. ");

            bResult = FALSE;

            break;
        }

        if ((ttEnd - ttBegin) > HOUR_2_SECOND(1))
        {
            RTOSprintf("\r\n@ Input time more than the 1 hour, too long. ");

            bResult = FALSE;

            break;
        }

        if ((ttBegin / DAY_2_SECOND(1)) != (ttEnd / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n@ Input time not the same day, Error. ");

            bResult = FALSE;

            break;
        }

        bResult = Datalog_ResendEvent(ttBegin, ttEnd, SOCKET_BANK_EXT);
    }

    break;
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to the TCP/IP CHK message.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_StatusChk(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_STATUS_CHECK, strlen(STR_AS_STATUS_CHECK)) != 0)
        return bResult;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<ID>", STR_AS_STATUS_CHECK);
#if ENABLE_MULTI_SERVER
        RTOSprintf("\r\n@ \t<ID>: Server ID, 0 or 1");
#else
        RTOSprintf("\r\n@ \t<ID>: Server ID, 0");
#endif

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8ServerID;

        if (strlen(pStr) != 1)
            break;

        u8ServerID = atoi(pStr);

        if (u8ServerID >= MAX_SOCKET_BANK)
        {
            RTOSprintf("\r\n@ Invalid server ID!");

            break;
        }

        // sprintf(szCmdMsg, "@ $CHK,%s,%s,%s,FW-%d.%03d%s,%d,%c%d,%d,%s,%d.%d.%d.%d:%d,%s:%d,%d,%d,%d,%f,%d,%d,%d #",
        //        //STR_AS_STATUS_CHECK,
        //         g_Setting.tGSM_SET.tMobileInfo.cIMEI,
        //         g_Setting.tBusInfo.cBus_ID,
        //         g_Setting.cJAS_Ver,
        //         (VERSION_BUILD / 1000),
        //				(VERSION_BUILD % 1000),
        //         FW_VERSION_CHK,
        //         g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
        //         (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
        //         g_SysInfo.tGPS_Data.u8StlNum,
        //         g_SysInfo.tEXTIO.bits.IN_ACC,
        //         g_Setting.tGSM_SET.tNetSet.cAPN,
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
        //         g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
        //         g_Setting.tGSM_SET.tNetSet.u16SendNormal,
        //         g_Setting.tGSM_SET.tNetSet.u16SendStandby,
        //         g_Setting.tSpeedSet.u16Trig, // Heinz
        //         g_Setting.tSpeedSet.fGain,
        //         g_Setting.tRPM_Set.u16Trig,
        //         g_Setting.tRPM_Set.u16DIV,
        //         g_Setting.tENABLE_SET.bits.CAN_SPEED);	// Gary added

        // RTOSprintf("\r\n%s", szCmdMsg);

        // GSM_TCP_ReportEvent(STR_AS_STATUS_CHECK, STR_NULL);

        if (bTCPIP_Connect[u8ServerID] == FALSE)
        {
            RTOSprintf("\r\n@ Currently not connect to server %d. Command cancel!", u8ServerID);

            break;
        }

        GSM_TCP_SendCHK(u8ServerID);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM FTP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmFtp(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64]= {'\r'};

    if (strncmp(pStr, STR_AS_GSM_FTP, strlen(STR_AS_GSM_FTP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<IP or ADDRESS>:<PORT>,<User name>,<Password>", STR_AS_GSM_FTP);
        RTOSprintf("\r\n@ \t<IP or ADDRESS> max. %d char", sizeof(g_Setting.tGSM_SET.tFTP_Set[0].cAddr) - 1);
        RTOSprintf("\r\n@ \t<PORT> range of 0 to 65535");
        RTOSprintf("\r\n@ \t<User name> max. %d char", OFFSET_SIZE(FTP_SetTypeDef, cUserName) - 1);
        RTOSprintf("\r\n@ \t<Password>  max. %d char", OFFSET_SIZE(FTP_SetTypeDef, cPassword) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s:%d,%s,%s\r\n",
                STR_AS_GSM_FTP,
                g_Setting.tGSM_SET.tFTP_Set[0].cAddr,
                g_Setting.tGSM_SET.tFTP_Set[0].u16Port,
                g_Setting.tGSM_SET.tFTP_Set[0].cUserName,
                g_Setting.tGSM_SET.tFTP_Set[0].cPassword);

        RTOSprintf("%s", szCmdMsg);

        bResult = TRUE;

        break;

    case ' ':
        memset(&g_Setting.tGSM_SET.tFTP_Set, 0x00, sizeof(FTP_SetTypeDef));

        System_SaveSetting();

        bResult = TRUE;

        break;

    default:
    {
        FTP_SetTypeDef tFTP_SetTemp;

        tFTP_SetTemp = g_Setting.tGSM_SET.tFTP_Set[0];

        StopChar(pStr, ':');

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= sizeof(g_Setting.tGSM_SET.tFTP_Set[0].cAddr)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cAddr, pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) == 0 || atoi(pStr) > 0xFFFF)
        {
            break;
        }

        tFTP_SetTemp.u16Port = atoi(pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= OFFSET_SIZE(FTP_SetTypeDef, cUserName)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cUserName, pStr);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= OFFSET_SIZE(FTP_SetTypeDef, cPassword)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cPassword, pStr);

        g_Setting.tGSM_SET.tFTP_Set[0] = tFTP_SetTemp;

#if ENABLE_GSM_NET
        // bGSM_ResetCheckFTP_Flag = TRUE;
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSetting();

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to GSM FTP EXT.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GsmFtpExt(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64]= {'\r'};

    if (strncmp(pStr, STR_AS_GSM_FTP_EXT, strlen(STR_AS_GSM_FTP_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<IP or ADDRESS>:<PORT>,<User name>,<Password>", STR_AS_GSM_FTP_EXT);
        RTOSprintf("\r\n@ \t<IP or ADDRESS> max. %d char", sizeof(g_Setting.tGSM_SET.tFTP_Set[1].cAddr) - 1);
        RTOSprintf("\r\n@ \t<PORT> range of 0 to 65535");
        RTOSprintf("\r\n@ \t<User name> max. %d char", OFFSET_SIZE(FTP_SetTypeDef, cUserName) - 1);
        RTOSprintf("\r\n@ \t<Password>  max. %d char", OFFSET_SIZE(FTP_SetTypeDef, cPassword) - 1);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s:%d,%s,%s\r\n",
                STR_AS_GSM_FTP,
                g_Setting.tGSM_SET.tFTP_Set[1].cAddr,
                g_Setting.tGSM_SET.tFTP_Set[1].u16Port,
                g_Setting.tGSM_SET.tFTP_Set[1].cUserName,
                g_Setting.tGSM_SET.tFTP_Set[1].cPassword);

        RTOSprintf("%s", szCmdMsg);

        bResult = TRUE;

        break;

    case ' ':
        memset(&g_Setting.tGSM_SET.tFTP_Set, 0x00, sizeof(FTP_SetTypeDef));

        System_SaveSetting();

        bResult = TRUE;

        break;

    default:
    {
        FTP_SetTypeDef tFTP_SetTemp;

        tFTP_SetTemp = g_Setting.tGSM_SET.tFTP_Set[1];

        StopChar(pStr, ':');

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= sizeof(g_Setting.tGSM_SET.tFTP_Set[1].cAddr)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cAddr, pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if (strlen(pStr) == 0 || atoi(pStr) > 0xFFFF)
        {
            break;
        }

        tFTP_SetTemp.u16Port = atoi(pStr);

        pStr += strlen(pStr) + 1;

        StopChar(pStr, ',');

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= OFFSET_SIZE(FTP_SetTypeDef, cUserName)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cUserName, pStr);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) == 0) ||
            (strlen(pStr) >= OFFSET_SIZE(FTP_SetTypeDef, cPassword)))
        {
            break;
        }

        strcpy(tFTP_SetTemp.cPassword, pStr);

        g_Setting.tGSM_SET.tFTP_Set[1] = tFTP_SetTemp;

#if ENABLE_GSM_NET
        // bGSM_ResetCheckFTP_Flag = TRUE;
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSetting();

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to uploading files to FTP server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FTP_UPLOAD(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FTP_UPLOAD, strlen(STR_AS_FTP_UPLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<SrcDir>/<SrcFile>,<DstDir>/<DstFile>", STR_AS_FTP_UPLOAD);
        RTOSprintf("\r\n@ \t<SrcDir>: Name of the directory storing the source file.");
        RTOSprintf("\r\n@ \t<SrcFile>: Name of the source file.");
        RTOSprintf("\r\n@ \t<DstDir>: Name of the directory to upload the file to in FTP server.");
        RTOSprintf("\r\n@ \t<DstFile>: Name of the file after uploaded to FTP server.");
        // RTOSprintf("\r\n@ \t<Mode>: Upload mode:");
        // RTOSprintf("\r\n@ \tMode = 0: File is uploaded via intermediate file in GSM module's storage.");
        // RTOSprintf("\r\n@ \tMode = 1: File is uploaded directly.");

        bResult = TRUE;

        break;

    default:
    {
        char cSrcFile[64], cSrcDir[64], cDstFile[64], cDstDir[64];

        StopChar(pStr, ',');

        bResult = SplitPath(pStr, cSrcDir, cSrcFile, sizeof(cSrcDir), sizeof(cSrcFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cSrcFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        pStr += strlen(pStr) + 1;

        bResult = SplitPath(pStr, cDstDir, cDstFile, sizeof(cDstDir), sizeof(cDstFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cDstDir) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cDstFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        bResult = GSM_FTP_FileUploadRequest(cSrcFile, cDstFile, cSrcDir, cDstDir, 1, 0);

        if (strlen(cSrcDir) == 0)
        {
            RTOSprintf("\r\n@ Request to upload file %s to FTP server 1 as %s/%s %s.",
                       cSrcFile, cDstDir, cDstFile, bResult ? (char *)"OK" : (char *)"ERROR");
        }
        else
        {
            RTOSprintf("\r\n@ Request to upload file %s/%s to FTP server 1 as %s/%s %s.",
                       cSrcDir, cSrcFile, cDstDir, cDstFile, bResult ? (char *)"OK" : (char *)"ERROR");
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to uploading files to FTP server ext.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FTP_UPLOAD_EXT(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FTP_UPLOAD_EXT, strlen(STR_AS_FTP_UPLOAD_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<SrcDir>/<SrcFile>,<DstDir>/<DstFile>", STR_AS_FTP_UPLOAD_EXT);
        RTOSprintf("\r\n@ \t<SrcDir>: Name of the directory storing the source file.");
        RTOSprintf("\r\n@ \t<SrcFile>: Name of the source file.");
        RTOSprintf("\r\n@ \t<DstDir>: Name of the directory to upload the file to in FTP server.");
        RTOSprintf("\r\n@ \t<DstFile>: Name of the file after uploaded to FTP server.");
        // RTOSprintf("\r\n@ \t<Mode>: Upload mode:");
        // RTOSprintf("\r\n@ \tMode = 0: File is uploaded via intermediate file in GSM module's storage.");
        // RTOSprintf("\r\n@ \tMode = 1: File is uploaded directly.");

        bResult = TRUE;

        break;

    default:
    {
        char cSrcFile[64], cSrcDir[64], cDstFile[64], cDstDir[64];

        StopChar(pStr, ',');

        bResult = SplitPath(pStr, cSrcDir, cSrcFile, sizeof(cSrcDir), sizeof(cSrcFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cSrcFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        pStr += strlen(pStr) + 1;

        bResult = SplitPath(pStr, cDstDir, cDstFile, sizeof(cDstDir), sizeof(cDstFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cDstDir) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(cDstFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        bResult = GSM_FTP_FileUploadRequest(cSrcFile, cDstFile, cSrcDir, cDstDir, 2, 0);

        if (strlen(cSrcDir) == 0)
        {
            RTOSprintf("\r\n@ Request to upload file %s to FTP server 2 as %s/%s %s.",
                       cSrcFile, cDstDir, cDstFile, bResult ? (char *)"OK" : (char *)"ERROR");
        }
        else
        {
            RTOSprintf("\r\n@ Request to upload file %s/%s to FTP server 2 as %s/%s %s.",
                       cSrcDir, cSrcFile, cDstDir, cDstFile, bResult ? (char *)"OK" : (char *)"ERROR");
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to downloading files from FTP server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FTP_DOWNLOAD(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FTP_DOWNLOAD, strlen(STR_AS_FTP_DOWNLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<SrcDir>/<SrcFile>,<DstDir>/<DstFile>", STR_AS_FTP_DOWNLOAD);
        RTOSprintf("\r\n@ \t<SrcDir>: Name of the directory storing the source file on FTP server.");
        RTOSprintf("\r\n@ \t<SrcFile>: Name of the source file.");
        RTOSprintf("\r\n@ \t<DstDir>: Name of the directory in SD card to download the file to.");
        RTOSprintf("\r\n@ \t<DstFile>: Name of the file after downloaded to SD card.");

        bResult = TRUE;

        break;

    default:
    {
        GSM_DownloadRequestTypeDef gsmDlHandle = {0};

        StopChar(pStr, ',');

        bResult = SplitPath(pStr,
                            gsmDlHandle.cSrcDir,
                            gsmDlHandle.cSrcFile,
                            sizeof(gsmDlHandle.cSrcDir),
                            sizeof(gsmDlHandle.cSrcFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(gsmDlHandle.cSrcFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        pStr += strlen(pStr) + 1;

        bResult = SplitPath(pStr,
                            gsmDlHandle.cDstDir,
                            gsmDlHandle.cDstFile,
                            sizeof(gsmDlHandle.cDstDir),
                            sizeof(gsmDlHandle.cDstFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(gsmDlHandle.cDstFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        gsmDlHandle.u8ServerID = 1;

        gsmDlHandle.dlType = FTP_DOWNLOAD_NORMAL;

        bResult = GSM_FTP_FileDownloadRequest(&gsmDlHandle);

        RTOSprintf("\r\n@ Request to download file %s%s%s from FTP server 1 as %s%s%s %s.",
                   (strlen(gsmDlHandle.cSrcDir) > 0) ? gsmDlHandle.cSrcDir : (char *)"",
                   (strlen(gsmDlHandle.cSrcDir) > 0) ? (char *)"/" : (char *)"",
                   gsmDlHandle.cSrcFile,
                   (strlen(gsmDlHandle.cDstDir) > 0) ? gsmDlHandle.cDstDir : (char *)"",
                   (strlen(gsmDlHandle.cDstDir) > 0) ? (char *)"/" : (char *)"",
                   gsmDlHandle.cDstFile,
                   bResult ? (char *)"OK" : (char *)"ERROR");

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to downloading files from FTP server ext.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FTP_DOWNLOAD_EXT(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FTP_DOWNLOAD_EXT, strlen(STR_AS_FTP_DOWNLOAD_EXT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<SrcDir>/<SrcFile>,<DstDir>/<DstFile>", STR_AS_FTP_DOWNLOAD_EXT);
        RTOSprintf("\r\n@ \t<SrcDir>: Name of the directory storing the source file on FTP server.");
        RTOSprintf("\r\n@ \t<SrcFile>: Name of the source file.");
        RTOSprintf("\r\n@ \t<DstDir>: Name of the directory in SD card to download the file to.");
        RTOSprintf("\r\n@ \t<DstFile>: Name of the file after downloaded to SD card.");

        bResult = TRUE;

        break;

    default:
    {
        GSM_DownloadRequestTypeDef gsmDlHandle = {0};

        StopChar(pStr, ',');

        bResult = SplitPath(pStr,
                            gsmDlHandle.cSrcDir,
                            gsmDlHandle.cSrcFile,
                            sizeof(gsmDlHandle.cSrcDir),
                            sizeof(gsmDlHandle.cSrcFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(gsmDlHandle.cSrcFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        pStr += strlen(pStr) + 1;

        bResult = SplitPath(pStr,
                            gsmDlHandle.cDstDir,
                            gsmDlHandle.cDstFile,
                            sizeof(gsmDlHandle.cDstDir),
                            sizeof(gsmDlHandle.cDstFile));

        if (bResult == FALSE)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        if (strlen(gsmDlHandle.cDstFile) == 0)
        {
            RTOSprintf("\r\n@ Wrong command format!");

            bResult = FALSE;

            break;
        }

        gsmDlHandle.u8ServerID = 2;

        gsmDlHandle.dlType = FTP_DOWNLOAD_NORMAL;

        bResult = GSM_FTP_FileDownloadRequest(&gsmDlHandle);

        RTOSprintf("\r\n@ Request to download file %s%s%s from FTP server 2 as %s%s%s %s.",
                   (strlen(gsmDlHandle.cSrcDir) > 0) ? gsmDlHandle.cSrcDir : (char *)"",
                   (strlen(gsmDlHandle.cSrcDir) > 0) ? (char *)"/" : (char *)"",
                   gsmDlHandle.cSrcFile,
                   (strlen(gsmDlHandle.cDstDir) > 0) ? gsmDlHandle.cDstDir : (char *)"",
                   (strlen(gsmDlHandle.cDstDir) > 0) ? (char *)"/" : (char *)"",
                   gsmDlHandle.cDstFile,
                   bResult ? (char *)"OK" : (char *)"ERROR");

        break;
    }
    }

    return bResult;
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
//============================================================================//
// @brief  Process the command related to checking EPO file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EPO_CHECK(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EPO_CHECK, strlen(STR_AS_EPO_CHECK)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_EPO_CHECK);

        bResult = TRUE;

        break;

    default:
    {
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            GPS_EPO_Check();

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to downloading EPO file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EPO_DOWNLOAD(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EPO_DOWNLOAD, strlen(STR_AS_EPO_DOWNLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_EPO_DOWNLOAD);

        bResult = TRUE;

        break;

    default:
    {
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            GPS_EPO_RequestDownload();

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to update EPO file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EPO_UPDATE(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EPO_UPDATE, strlen(STR_AS_EPO_UPDATE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_EPO_UPDATE);

        bResult = TRUE;

        break;

    default:
    {
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            char cFilePath[32] = {0};
            uint32_t u32FileSize = 0;

            sprintf(cFilePath, "%s/%s%s", (char *)GPS_EPO_DIR, (char *)GPS_EPO_FILE_NAME, (char *)GPS_EPO_FILE_NAME_EXT);

            if (SD_CheckFileExist(cFilePath) == FALSE)
            {
                RTOSprintf("\r\n> File %s does not exist.", cFilePath);

                break;
            }

            u32FileSize = SD_GetFileSize(cFilePath);

            GPS_EPO_RequestUpdate(cFilePath, u32FileSize);

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
//============================================================================//
// @brief  Process the command related to downloading XTRA file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_XTRA_DOWNLOAD(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_XTRA_DOWNLOAD, strlen(STR_AS_XTRA_DOWNLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_XTRA_DOWNLOAD);

        bResult = TRUE;

        break;

    default:
    {
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            GPS_XTRA_RequestDownload();

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to update XTRA file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_XTRA_UPDATE(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_XTRA_UPDATE, strlen(STR_AS_XTRA_UPDATE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_XTRA_UPDATE);

        bResult = TRUE;

        break;

    default:
    {
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            GSM_XTRA_RequestUpdate((char *)GPS_XTRA_FILE_NAME, 100U);

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to enable date classification in VDR path.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSM_FTP_VDR_SUBDIR(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_FTP_VDR_SUBDIR, strlen(STR_AS_FTP_VDR_SUBDIR)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_FTP_VDR_SUBDIR);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_FTP_VDR_SUBDIR,
                (g_Setting.u8FTP_VDR_Mode == 0) ? "OFF" : "ON");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.u8FTP_VDR_Mode = 1;

            bResult = System_SaveSetting();
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.u8FTP_VDR_Mode = FALSE;

            bResult = System_SaveSetting();
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_FTP_VDR_SUBDIR,
                    (g_Setting.u8FTP_VDR_Mode == 0) ? "OFF" : "ON");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to get SMS list.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SMS_LIST(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SMS_LIST, strlen(STR_AS_SMS_LIST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_SMS_LIST);
        RTOSprintf("\r\n@ \t<OPTION>:");
        RTOSprintf("\r\n@ \tREC UNREAD: Received unread messages.");
        RTOSprintf("\r\n@ \tREC READ: Received read messages.");
        RTOSprintf("\r\n@ \tSTO UNSENT: Stored unsent messages.");
        RTOSprintf("\r\n@ \tSTO SENT: Stored sent messages.");
        RTOSprintf("\r\n@ \tALL: All messages.");

        bResult = TRUE;

        break;

    default:
    {
        bool bRes;
        char *pMsg = szCmdMsg;

        memset(u8SMS_List, 0x00, sizeof(u8SMS_List));

        bRes = GSM_SMS_GetList(pStr, u8SMS_List);

        if (bRes)
        {
            RTOSprintf("\r\n@ %s:\r\n%s", STR_AS_SMS_LIST, u8SMS_List);

            RTOSprintf("\r\n%s", pMsg);

            bResult = TRUE;
        }
        else
        {
            sprintf(pMsg, "@ Get SMS list ERROR.");

            RTOSprintf("\r\n%s", pMsg);

            bResult = FALSE;
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to IOT SIM mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSM_IOT_SIM(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[64] = {'\0'};

    if (strncmp(pStr, STR_AS_GSM_IOT_SIM, strlen(STR_AS_GSM_IOT_SIM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<ON/OFF>", STR_AS_GSM_IOT_SIM);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_GSM_IOT_SIM,
                (g_Setting.bIOT_SIM_En == 1) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.bIOT_SIM_En = TRUE;

            bResult = System_SaveSetting();
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.bIOT_SIM_En = FALSE;

            bResult = System_SaveSetting();
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_GSM_IOT_SIM,
                    (g_Setting.bIOT_SIM_En == 1) ? "ON" : "OFF");
        }

        break;
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to the SOS message.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Sos(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_SOS, strlen(STR_AS_SOS)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
          RTOSprintf("\r\n@ %s<OPTION>", STR_AS_SOS);
          RTOSprintf("\r\n@ \t<OPTION>:ON");

      bResult = TRUE;

      break;

        case '?':
          if(strncmp(pStr, (char *)"ON", 2) != 0)
                break;

            for(int i = 0; i < 5; i++)
            {
                GSM_TCP_ReportEvent((char *)STR_EVENT_PING, (char *)STR_NULL);

                HAL_Delay(S_2_mS(1));
            }

            bResult = TRUE;

            break;

        default:
            break;
    }

    return bResult;
}
*/
#endif

// System Control //

//============================================================================//
// @brief  Process the command related to the system.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_System(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SYSTEM, strlen(STR_AS_SYSTEM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<action>", STR_AS_SYSTEM);
        RTOSprintf("\r\n@ \t<action> System action");
        RTOSprintf("\r\n@ \t%s: Do system upgrade immediately", SystemCmdString[SYS_UPGRADE_NOW]);
        RTOSprintf("\r\n@ \t%s: Do system upgrade when ACC OFF", SystemCmdString[SYS_UPGRADE]);
        // RTOSprintf("\r\n@ \t%s\t: Backup datalog", SystemCmdString[SYS_BACKUP]);
        // RTOSprintf("\r\n@ \t%s\t: Get system photo", SystemCmdString[SYS_PHOTO]);
        RTOSprintf("\r\n@ \t%s: Show system information", SystemCmdString[SYS_INFO]);

        bResult = TRUE;

        break;

    default:
    {
        SystemCmdTypeDef SysCmd;

        for (SysCmd = (SystemCmdTypeDef)0; SysCmd < SYS_END; SysCmd++)
        {
            if (strncmp(pStr, SystemCmdString[SysCmd], strlen(SystemCmdString[SysCmd])) == 0)
            {
                break;
            }
        }

        switch (SysCmd)
        {
#if ENABLE_GSM_NET
        case SYS_UPGRADE_NOW:
            g_Setting.tUpgrade.bUpgradeNow = TRUE;
            g_Setting.tUpgrade.bFOTA_Flag = TRUE;

            if (bSystemMode == SYSTEM_MODE_STEALTH)
                bStealthFOTA_En = TRUE;

            System_SaveSetting();

            gsmCmdSrc = GSM_CMD_SRC_NULL;

            RTOSprintf("\r\n> ### SYS_UPGRADE_NOW Function. ###");

            bResult = TRUE;

            break;

        case SYS_UPGRADE:
            g_Setting.tUpgrade.bUpgradeNow = FALSE;
            g_Setting.tUpgrade.bFOTA_Flag = TRUE;

            if (bSystemMode == SYSTEM_MODE_STEALTH)
                bStealthFOTA_En = TRUE;

            System_SaveSetting();

            gsmCmdSrc = GSM_CMD_SRC_NULL;

            RTOSprintf("\r\n> ### SYS_UPGRADE Function. ###");

            bResult = TRUE;

            break;
#endif

        case SYS_INFO:
        {
            char *pMsg;

            pMsg = szCmdMsg;

#if ENABLE_GSM_NET
            sprintf(pMsg, "@ %s%s, SPEED: %03d, GPS: %c %+09.6f %+010.6f, GPRS: %s, ACC:%d IN1:%d IN2:%d SOS:%d",
#else
            sprintf(pMsg, "@ %s%s, SPEED: %03d, GPS: %c %+09.6f %+010.6f, ACC:%d IN1:%d IN2:%d SOS:%d",
#endif
                    STR_AS_SYSTEM,
                    SystemCmdString[SYS_INFO],
                    M_2_KM(g_SysInfo.tVehicleState.u32Speed),
                    (GPS_GetFixed() == TRUE) ? 'A' : 'V',
                    g_SysInfo.tGPS_Data.gpsLocation.dLatitude,
                    g_SysInfo.tGPS_Data.gpsLocation.dLongitude,
#if ENABLE_GSM_NET
                    (bTCPIP_Connect[0] == TRUE) ? "CONNECT" : "DISCONN",
#endif
                    g_SysInfo.tEXTIO.bits.IN_ACC,
                    g_SysInfo.tEXTIO.bits.IN_1,
                    g_SysInfo.tEXTIO.bits.IN_2,
                    g_SysInfo.tEXTIO.bits.IN_SOS);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;
        }

        default:
            break;
        }

        break;
    }
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to the last installation.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_LastInstall(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_INST_TIME, strlen(STR_AS_INST_TIME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<TIME>", STR_AS_INST_TIME);
          RTOSprintf("\r\n@ \t<TIME>: Time in format YYMMDD");

            bResult = TRUE;

            break;

        case '?':
        {
            char BufStr[16];


            if(g_Setting.tUpgrade.ttLastInstall == NULL
                 || g_Setting.tUpgrade.ttLastInstall == INVALID_VALUE_DEF
                 || g_Setting.tUpgrade.ttLastInstall == 0)
            {
                RTOSprintf("\r\n@ %sNONE", STR_AS_INST_TIME);
            }
            else
            {
              struct tm tmTime = {0};


              localtime_r((const time_t *)&g_Setting.tUpgrade.ttLastInstall, &tmTime);

                strftime(BufStr, sizeof(BufStr), "%y/%m/%d", &tmTime);

                sprintf(szCmdMsg, "@ %s%s", STR_AS_INST_TIME, BufStr);

                RTOSprintf("\r\n%s", szCmdMsg);
            }

            bResult = TRUE;

            break;
        }

      default:
        {
            char BufStr[16];
            time_t rawtime;
            struct tm timeinfo = {0};

            if(strlen(pStr) != 6)
                break;

            localtime_r(&rawtime, &timeinfo);

            strncpy(BufStr, pStr, 2);

            timeinfo.tm_year = atoi(BufStr) + 100;

            strncpy(BufStr, pStr + 2, 2);

            timeinfo.tm_mon = atoi(BufStr) - 1;

            strncpy(BufStr, pStr + 4, 2);

            timeinfo.tm_mday = atoi(BufStr);

            rawtime = mktime(timeinfo);

            g_Setting.tUpgrade.ttLastInstall = rawtime;

            System_SaveSetting();

            bResult = TRUE;

            break;
        }
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to system reset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_SYS(void)
{
    RTOSprintf("\r\n@ Command OK !!\r\n");

#if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
#endif

    System_SaveSettingFinal();
    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();

    return TRUE;
}

//============================================================================//
// @brief  Process the command related to reset TEMP directory.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_TEMP(void)
{
    // .tmp files
    SD_ClearUpDir((char *)DIRNAME_TEMP, (char *)"*.tmp", 0);

    // .TMP files
    SD_ClearUpDir((char *)DIRNAME_TEMP, (char *)"*.TMP", 0);

    RTOSprintf("\r\n@ Command OK !!\r\n");

    // #if ENABLE_GSM_NET
    // QtEC21_CheckRedoFTP();
    // #endif

    System_SaveSettingFinal();
    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();

    return TRUE;
}

//============================================================================//
// @brief  Process the command related to reset SD card and system.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_ALL(void)
{
    bSD_FormatFlag = TRUE;

    bCheckVDR_TmpExist = FALSE;

    SD_Format();

#if ENABLE_INDEX_BUFFER
    u8SD_RetryCnt = 1;
#endif

    RTOSprintf("\r\n@ Command OK !!\r\n");

    // #if ENABLE_GSM_NET
    // QtEC21_CheckRedoFTP();
    // #endif

    System_SaveSettingFinal();
    System_SaveBackupSettingFinal();

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();

    return TRUE;
}

//============================================================================//
// @brief  Process the command related to system reset default.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_DEFAULT(void)
{
#if ENABLE_INDEX_BUFFER
    u8SD_RetryCnt = 1;
#endif

    // System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    System_SetupDefault();

    // System_DelBackupSetting();

    // System_RestoreBackupSetting(VAR_ALL_ITEMS);

#if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
#endif

    System_SaveSettingAll();

    RTOSprintf("\r\n@ Command OK !!\r\n");

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();

    return TRUE;
}

//============================================================================//
// @brief  Process the command related to system reset default.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_SETTING(void)
{
#if ENABLE_INDEX_BUFFER
    u8SD_RetryCnt = 1;
#endif

    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    System_SetupDefault();

    System_RestoreBackupSetting(VAR_ALL_ITEMS);

#if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
#endif

    System_SaveSettingAll();

    RTOSprintf("\r\n@ Command OK !!\r\n");

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();

    return TRUE;
}

/*
//============================================================================//
// @brief  Process the command related to system reset to upgrade.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool CmdReset_UPGRADE(void)
{
    //HAL_RTCEx_BKUPWrite(&hrtc, BKP_DFU_CHK, BKP_RST_VALUE);

  RTOSprintf("\r\n@ Command OK !!\r\n");

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

    return TRUE;
}
*/

//============================================================================//
// @brief  Process the command related to reset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Reset(char *pStr)
{
    bool bResult = FALSE;
    ResetCmdTypeDef ResetCmd;

    if (strncmp(pStr, STR_AS_RESET, strlen(STR_AS_RESET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<action>", STR_AS_RESET);
        RTOSprintf("\r\n@ \t<action> Reset action");
        RTOSprintf("\r\n@ \t%s: Reset system", szResetCmd[RST_SYS]);
        RTOSprintf("\r\n@ \t%s: Reset GPS module", szResetCmd[RST_GPS]);
        RTOSprintf("\r\n@ \t%s: Reset GSM module", szResetCmd[RST_GSM]);
        RTOSprintf("\r\n@ \t%s: Reset CAN-BUS link", szResetCmd[RST_CAN]);
        RTOSprintf("\r\n@ \t%s: Reset datalog", szResetCmd[RST_LOG]);
        RTOSprintf("\r\n@ \t%s: Clear all temp file", szResetCmd[RST_TEMP]);
        RTOSprintf("\r\n@ \t%s: Clear all file", szResetCmd[RST_ALL]);
        RTOSprintf("\r\n@ \t%s: Reset settings to factory", szResetCmd[RST_DEFAULT]);
        RTOSprintf("\r\n@ \t%s: Reset settings to factory then restore important settings", szResetCmd[RST_SETTING]);
        // RTOSprintf("\r\n@ \t%s: Reset to upgrade", szResetCmd[RST_UPGRADE]);

        bResult = TRUE;

        break;

    default:
        for (ResetCmd = (ResetCmdTypeDef)0; ResetCmd < RST_END; ResetCmd++)
        {
            if (strncmp(pStr, szResetCmd[ResetCmd], strlen(szResetCmd[ResetCmd])) == 0)
            {
                break;
            }
        }

        switch (ResetCmd)
        {
        case RST_SYS:
            RTOSprintf("\r\n@ Reset system after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_SYS();

            bResult = TRUE;

            break;

#if ENABLE_GPS_MODULE
        case RST_GPS:
            RTOSprintf("\r\n@ Reset GPS after 1 second.");

            GPS_Reset(1);

            bResult = TRUE;

            break;
#endif

#if ENABLE_GSM_NET
        case RST_GSM:
            // GSM_ResetWithCheckFTP();
            bGSM_ResetCheckFTP_Flag = TRUE;

            bResult = TRUE;

            break;
#endif

#if ENABLE_CAN == 1
        case RST_CAN:
            CAN_Config();

            bResult = TRUE;

            break;
#endif

        case RST_LOG:
            if (bSD_ReportErrFlag == TRUE)
                break;

            Datalog_Reset();

            bResult = TRUE;

            break;

        case RST_TEMP:
            if (bSD_ReportErrFlag == TRUE)
                break;

            RTOSprintf("\r\n@ Clear all temp file after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_TEMP();

            bResult = TRUE;

            break;

        case RST_ALL:
            if (bSD_ReportErrFlag == TRUE)
                break;

            RTOSprintf("\r\n@ Clear all file after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_ALL();

            bResult = TRUE;

            break;

        case RST_DEFAULT:
            RTOSprintf("\r\n@ Reset default after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_DEFAULT();

            bResult = TRUE;

            break;

        case RST_SETTING:
            RTOSprintf("\r\n@ Reset settings after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_SETTING();

            bResult = TRUE;

            break;

            /*
            case RST_UPGRADE:
                RTOSprintf("\r\n@ Reset to upgrade after 1 second.");

                SD_RemoveFile(FILENAME_UPGRADE_DFU);

                HAL_Delay(S_2_mS(1));

                CmdReset_UPGRADE();

                bResult = TRUE;

                break;
            */

        default:
            break;
        }

        break;
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to reset for booting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Reset_Boot(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_REBOOT, strlen(STR_AS_REBOOT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
        case '?':
          RTOSprintf("\r\n@ %s<OPTION>", STR_AS_REBOOT);
          RTOSprintf("\r\n@ \t<OPTION>:ON");

          bResult = TRUE;

          break;

        default:
            if(strncmp(pStr, (char *)"ON", 2) != 0)
                break;

            RTOSprintf("\r\n@ Reset system after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_SYS();

            bResult = TRUE;

            break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to reset default.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Reset_Def(char *pStr)
{
    bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_REDEFAULT, strlen(STR_AS_REDEFAULT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
        case '?':
            RTOSprintf("\r\n@ %s<OPTION>", STR_AS_REDEFAULT);
          RTOSprintf("\r\n@ \t<OPTION>:ON");

          bResult = TRUE;

          break;

        default:
            if(strncmp(pStr, (char *)"ON", 2) != 0)
                break;

            RTOSprintf("\r\n@ Reset to factory after 1 second.");

            HAL_Delay(S_2_mS(1));

            CmdReset_DEFAULT();

            bResult = TRUE;

            break;
    }

    return bResult;
}
*/

// External wakeup setting //
//============================================================================//
// @brief  Process the command related to external alarm setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EX_ALM(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_EX_ALM, strlen(STR_AS_EX_ALM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<MODE>,<REPEAT>,<TIME>", STR_AS_EX_ALM);
        RTOSprintf("\r\n@ \t<MODE>: ON/OFF");
        RTOSprintf("\r\n@ \t<REPEAT>: ON: Alarm will be repeated, OFF: Alarm only once");
        RTOSprintf("\r\n@ \t<TIME>: Alarm time in format YYMMDDHHMMSS");
        RTOSprintf("\r\n@ \tYear/Month/Date == 0 -> Don't care");
        RTOSprintf("\r\n@	\tHour > 23 -> Don't care");
        RTOSprintf("\r\n@ \tMinute/Second > 59 -> Don't care");

        bResult = TRUE;

        break;

    case '\?':
        sprintf(szCmdMsg, "%s%s,%s,%02d%02d%02d%02d%02d%02d",
                STR_AS_EX_ALM,
                g_Setting.tExAlarm.bEnableAlm ? (char *)"ON" : (char *)"OFF",
                g_Setting.tExAlarm.bRepeat ? (char *)"ON" : (char *)"OFF",
                g_Setting.tExAlarm.tAlmDate.Year,
                g_Setting.tExAlarm.tAlmDate.Month,
                g_Setting.tExAlarm.tAlmDate.Date,
                g_Setting.tExAlarm.tAlmTime.Hours,
                g_Setting.tExAlarm.tAlmTime.Minutes,
                g_Setting.tExAlarm.tAlmTime.Seconds);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32Cnt = 0;
        ExternalAlarmTypeDef tAlmTemp = {0};
        uint8_t u8Year = 0;
        char cEnable[5] = {0}, cRepeat[5] = {0};
        uint32_t u32Mask = 0;

        if (strncmp(pStr, (char *)"OFF", 3) == 0)
        {
            tAlmTemp.bEnableAlm = FALSE;

            System_SetExAlmSetting(&tAlmTemp);

#if RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
            MG82_RTC_Alarm(System_GetTime(), tAlmTemp);
#endif

            bResult = TRUE;

            break;
        }

        u32Cnt = sscanf(pStr, "%4[^,],%4[^,],%02d%02d%02d%02d%02d%02d",
                        cEnable, cRepeat,
                        (int *)&u8Year,
                        (int *)&tAlmTemp.tAlmDate.Month,
                        (int *)&tAlmTemp.tAlmDate.Date,
                        (int *)&tAlmTemp.tAlmTime.Hours,
                        (int *)&tAlmTemp.tAlmTime.Minutes,
                        (int *)&tAlmTemp.tAlmTime.Seconds);

        if (u32Cnt != 8)
        {
            RTOSprintf("\r\n> Invalid parameters number: %d", u32Cnt);

            break;
        }

        tAlmTemp.tAlmDate.Year = u8Year;

        if ((strncmp(cEnable, (char *)"ON", 2) != 0) &&
            (strncmp(cEnable, (char *)"OFF", 3) != 0))
        {
            RTOSprintf("\r\n> Invalid Enable option: %s", cEnable);

            break;
        }

        if ((strncmp(cRepeat, (char *)"ON", 2) != 0) &&
            (strncmp(cRepeat, (char *)"OFF", 3) != 0))
        {
            RTOSprintf("\r\n> Invalid Repeat option: %s", cRepeat);

            break;
        }

        if (tAlmTemp.tAlmDate.Month > 31)
        {
            RTOSprintf("\r\n> Invalid Month option: %s", tAlmTemp.tAlmDate.Month);

            break;
        }

        if (tAlmTemp.tAlmDate.Date > 31)
        {
            RTOSprintf("\r\n> Invalid Date option: %s", tAlmTemp.tAlmDate.Date);

            break;
        }

        if (tAlmTemp.tAlmDate.Date == 0)
        {
            u32Mask |= RTC_ALARMMASK_DATEWEEKDAY;
        }

        if (tAlmTemp.tAlmTime.Hours > 23)
        {
            u32Mask |= RTC_ALARMMASK_HOURS;
        }

        if (tAlmTemp.tAlmTime.Minutes > 59)
        {
            u32Mask |= RTC_ALARMMASK_MINUTES;
        }

        if (tAlmTemp.tAlmTime.Seconds > 59)
        {
            u32Mask |= RTC_ALARMMASK_SECONDS;
        }

        if (strncmp(cEnable, (char *)"ON", 2) == 0)
        {
            tAlmTemp.bEnableAlm = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Invalid Enable option: %s", cEnable);

            break;
        }

        if (strncmp(cRepeat, (char *)"ON", 2) == 0)
        {
            tAlmTemp.bRepeat = TRUE;
        }
        else
        {
            tAlmTemp.bRepeat = FALSE;
        }

        tAlmTemp.u32Mask = u32Mask;

        if (g_Setting.tExPedWkup.bEnableWkup == TRUE)
        {
            g_Setting.tExPedWkup.bEnableWkup = FALSE;

#if RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
            MG82_RTC_PedWkup(g_Setting.tExPedWkup);
#endif
        }

        System_SetExAlmSetting(&tAlmTemp);

        sprintf(szCmdMsg, "@ %s%s,%s,%02d%02d%02d%02d%02d%02d Set OK",
                STR_AS_EX_ALM,
                cEnable,
                cRepeat,
                tAlmTemp.tAlmDate.Year,
                tAlmTemp.tAlmDate.Month,
                tAlmTemp.tAlmDate.Date,
                tAlmTemp.tAlmTime.Hours,
                tAlmTemp.tAlmTime.Minutes,
                tAlmTemp.tAlmTime.Seconds);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to external periodic wakeup setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EX_PED_WKUP(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_EX_PED_WKUP, strlen(STR_AS_EX_PED_WKUP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<MODE>,<TIME>", STR_AS_EX_PED_WKUP);
        RTOSprintf("\r\n@ \t<MODE>: ON/OFF");
        RTOSprintf("\r\n@ \t<TIME>: Wakeup time period in seconds, range: 0 - %d", MG82_PED_WKUP_TIME_MAX);

        bResult = TRUE;

        break;

    case '\?':
        sprintf(szCmdMsg, "%s%s,%d",
                STR_AS_EX_PED_WKUP,
                g_Setting.tExPedWkup.bEnableWkup ? (char *)"ON" : (char *)"OFF",
                g_Setting.tExPedWkup.u32Timeout);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32Cnt = 0;
        char cEnable[5] = {0};
        uint32_t u32TimeoutTemp = 0;

        if (strncmp(pStr, (char *)"OFF", 3) == 0)
        {
            g_Setting.tExPedWkup.bEnableWkup = FALSE;

            System_SaveSetting();

#if RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
            MG82_RTC_PedWkup(g_Setting.tExPedWkup);
#endif

            bResult = TRUE;

            break;
        }

        u32Cnt = sscanf(pStr, "%4[^,],%08d", cEnable, &u32TimeoutTemp);

        if (u32Cnt != 2)
        {
            RTOSprintf("\r\n> Invalid parameters number: %d", u32Cnt);

            break;
        }

        if (strncmp(cEnable, (char *)"ON", 2) != 0)
        {
            RTOSprintf("\r\n> Invalid Enable option: %s", cEnable);

            break;
        }

        if (u32TimeoutTemp > MG82_PED_WKUP_TIME_MAX)
        {
            RTOSprintf("\r\n> Invalid Wakeup time period: %d", u32TimeoutTemp);

            break;
        }

        g_Setting.tExPedWkup.u32Timeout = u32TimeoutTemp;

        g_Setting.tExPedWkup.bEnableWkup = TRUE;

        if (g_Setting.tExAlarm.bEnableAlm == TRUE)
        {
            ExternalAlarmTypeDef tAlmTemp = {0};

            tAlmTemp.bEnableAlm = FALSE;

            System_SetExAlmSetting(&tAlmTemp);

#if RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
            MG82_RTC_Alarm(System_GetTime(), tAlmTemp);
#endif
        }

        System_SaveSetting();

        sprintf(szCmdMsg, "@ %s%s,%d Set OK",
                STR_AS_EX_PED_WKUP,
                cEnable,
                g_Setting.tExPedWkup.u32Timeout);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

// External wakeup setting //
//============================================================================//
// @brief  Process the command related to external alarm setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TEST_ALM(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_ALM, strlen(STR_AS_ALM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<MODE>,<TIME>", STR_AS_ALM);
        RTOSprintf("\r\n@ \t<MODE>: ON/OFF");
        RTOSprintf("\r\n@ \t<TIME>: Alarm time in format DDHHMMSS");
        RTOSprintf("\r\n@ \tDate == 0 -> Don't care");
        RTOSprintf("\r\n@	\tHour > 23 -> Don't care");
        RTOSprintf("\r\n@ \tMinute/Second > 59 -> Don't care");

        bResult = TRUE;

        break;

    case '\?':
    {
        RTC_AlarmTypeDef sAlarm = {0};

        HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

        sprintf(szCmdMsg, "%s%02d%02d%02d%02d,%d",
                STR_AS_ALM,
                sAlarm.AlarmDateWeekDay,
                sAlarm.AlarmTime.Hours,
                sAlarm.AlarmTime.Minutes,
                sAlarm.AlarmTime.Seconds,
                sAlarm.AlarmMask);

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    default:
    {
        uint32_t u32Cnt = 0;
        RTC_AlarmTypeDef sAlarm = {0};
        char cEnable[5] = {0};
        uint32_t u32Mask = 0;

        u32Cnt = sscanf(pStr, "%4[^,],%02d%02d%02d%02d",
                        cEnable,
                        (int *)&sAlarm.AlarmDateWeekDay,
                        (int *)&sAlarm.AlarmTime.Hours,
                        (int *)&sAlarm.AlarmTime.Minutes,
                        (int *)&sAlarm.AlarmTime.Seconds);

        if (u32Cnt != 5)
        {
            RTOSprintf("\r\n> Invalid parameters number: %d", u32Cnt);

            break;
        }

        if ((strncmp(cEnable, (char *)"ON", 2) != 0) &&
            (strncmp(cEnable, (char *)"OFF", 3) != 0))
        {
            RTOSprintf("\r\n> Invalid Enable option: %s", cEnable);

            break;
        }

        if (sAlarm.AlarmDateWeekDay > 31)
        {
            RTOSprintf("\r\n> Invalid Date option: %s", sAlarm.AlarmDateWeekDay);

            break;
        }

        if (sAlarm.AlarmDateWeekDay == 0)
        {
            u32Mask |= RTC_ALARMMASK_DATEWEEKDAY;
        }

        if (sAlarm.AlarmTime.Hours > 23)
        {
            u32Mask |= RTC_ALARMMASK_HOURS;
        }

        if (sAlarm.AlarmTime.Minutes > 59)
        {
            u32Mask |= RTC_ALARMMASK_MINUTES;
        }

        if (sAlarm.AlarmTime.Seconds > 59)
        {
            u32Mask |= RTC_ALARMMASK_SECONDS;
        }

        sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
        sAlarm.AlarmMask = u32Mask;
        sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
        sAlarm.Alarm = RTC_ALARM_A;

        if (strncmp(cEnable, (char *)"ON", 2) == 0)
        {
            HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);

            if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_ALRAF))
            {
                RTOSprintf("\r\n> ### System waken up by RTC alarm A ###");

                __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
            }

            HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

            if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) == HAL_OK)
            {
                bResult = TRUE;

                HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

                bRTOS_En = FALSE;

                // LCD_Stop();

                RTOSprintf("\r\n> ### SET RTC ALARM OK, SYSTEM ENTER STANDBY MODE ###");

                System_Reset();
            }
        }
        else
        {
            if (HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A) == HAL_OK)
            {
                bResult = TRUE;
            }
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to external periodic wakeup setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TEST_PED_WKUP(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_PED_WKUP, strlen(STR_AS_PED_WKUP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<MODE>,<TIME>", STR_AS_PED_WKUP);
        RTOSprintf("\r\n@ \t<MODE>: ON/OFF");
        RTOSprintf("\r\n@ \t<TIME>: Wakeup time period in seconds, range: 0 - 131072");

        bResult = TRUE;

        break;

    case '\?':
    {
        sprintf(szCmdMsg, "%s%d",
                STR_AS_PED_WKUP,
                HAL_RTCEx_GetWakeUpTimer(&hrtc));

        RTOSprintf("\r\n> %s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    default:
    {
        uint32_t u32Cnt = 0;
        char cEnable[5] = {0};
        uint32_t u32TimeoutTemp = 0;

        u32Cnt = sscanf(pStr, "%4[^,],%08d", cEnable, &u32TimeoutTemp);

        if (u32Cnt != 2)
        {
            RTOSprintf("\r\n> Invalid parameters number: %d", u32Cnt);

            break;
        }

        if ((strncmp(cEnable, (char *)"ON", 2) != 0) &&
            (strncmp(cEnable, (char *)"OFF", 3) != 0))
        {
            RTOSprintf("\r\n> Invalid Enable option: %s", cEnable);

            break;
        }

        if ((u32TimeoutTemp == 0) || (u32TimeoutTemp > 131072))
        {
            RTOSprintf("\r\n> Invalid Wakeup time period: %d", u32TimeoutTemp);

            break;
        }

        if (strncmp(cEnable, (char *)"ON", 2) == 0)
        {
            HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);

            if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF) != RESET)
            {
                RTOSprintf("\r\n> ### RTC wake up flag set. Reset ###");

                __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
            }

            HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

            if (u32TimeoutTemp <= 65536)
            {
                if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, u32TimeoutTemp - 1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS))
                {
                    RTOSprintf("\r\n> RTC set wake up timer (%d) ERROR", u32TimeoutTemp);

                    break;
                }
            }
            else
            {
                if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, u32TimeoutTemp - 65536 - 1, RTC_WAKEUPCLOCK_CK_SPRE_17BITS))
                {
                    RTOSprintf("\r\n> RTC set wake up timer (%d) ERROR", u32TimeoutTemp);

                    break;
                }
            }

            bResult = TRUE;

            HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

            bRTOS_En = FALSE;

            // LCD_Stop();

            RTOSprintf("\r\n> ### SET RTC WAKE UP (%d) OK, SYSTEM ENTER STANDBY MODE ###", u32TimeoutTemp);

            System_Reset();

            // System_EnterStandbyMode();
        }
        else
        {
            HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}

// Data File //

//============================================================================//
// @brief  Process the command related to downloading file in SD card to USB.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FILE_DOWNLOAD(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_USB_DOWNLOAD, strlen(STR_AS_USB_DOWNLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<PATH>", STR_AS_USB_DOWNLOAD);
        RTOSprintf("\r\n@ \t<PATH>: Path of the file to download into USB. Max 64 char");

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n@ Input file path invalid!");

            bResult = FALSE;

            break;
        }

        USB_DownloadFile(pStr, pStr);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to listing the files in SD card.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FILE_GET_LIST(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FIL_LIST, strlen(STR_AS_FIL_LIST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_FIL_LIST);
        RTOSprintf("\r\n@ \t<OPTION>:");
        RTOSprintf("\r\n@ \t?: Print a list of all files and directories in the root directory of the SD card");
        RTOSprintf("\r\n@ \t<DIR NAME>: Print a list of all files and directories "
                   "in the directory <DIR NAME> in the SD card if it exists");

        bResult = TRUE;

        break;

    case '?':
        bResult = SD_FileList();

        break;

    default:
        bResult = SD_FileListDir(pStr);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to formatting SD card.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_CARD_FORMAT(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FORMAT, strlen(STR_AS_FORMAT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_FORMAT);
        RTOSprintf("\r\n@ \t<OPTION>:ON");

        bResult = TRUE;

        break;

    default:
        if (strncmp(pStr, (char *)"ON", 2) != 0)
            break;

        bSD_FormatFlag = TRUE;

        bCheckVDR_TmpExist = FALSE;

        SD_Format();

#if ENABLE_INDEX_BUFFER
        u8SD_RetryCnt = 1;
#endif

        RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingFinal();

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();

        bResult = TRUE;

        break;
    }

    return bResult;
}

// Debug //

//============================================================================//
// @brief  Process the command related to the debug mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DebugMode(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_DEBUG_MODE, strlen(STR_AS_DEBUG_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_DEBUG_MODE);
        RTOSprintf("\r\n@ \t<mode> set output mode for debug");
        RTOSprintf("\r\n@ \t%s\t: Debug function off", DebugModeString[DBG_OFF]);
        RTOSprintf("\r\n@ \t%s\t: Current time data output", DebugModeString[DBG_TIME]);
        RTOSprintf("\r\n@ \t%s\t: System detail data output", DebugModeString[DBG_DATA]);
        RTOSprintf("\r\n@ \t%s\t: System log process output", DebugModeString[DBG_LOG]);
        RTOSprintf("\r\n@ \t%s\t: GPS NMEA code output", DebugModeString[DBG_GPS]);
        RTOSprintf("\r\n@ \t%s\t: GPS detail data output", DebugModeString[DBG_GPS1]);
        RTOSprintf("\r\n@ \t%s\t: GSM module AT command output", DebugModeString[DBG_GSM]);
        RTOSprintf("\r\n@ \t%s\t: CAN data output", DebugModeString[DBG_CAN]);
        RTOSprintf("\r\n@ \t%s\t: RS232 transmission output", DebugModeString[DBG_DB9]);
        RTOSprintf("\r\n@ \t%s\t: EXT-BOX communication output", DebugModeString[DBG_EXT]);
        RTOSprintf("\r\n@ \t%s\t: G-SENSOR data output", DebugModeString[DBG_GS]);
        RTOSprintf("\r\n@ \t%s\t: ENG DATA output", DebugModeString[DBG_ENG]);
// RTOSprintf("\r\n@ \t%s\t: RESERVED DEBUG 1 output", DebugModeString[DBG_RESERVED1]);
// RTOSprintf("\r\n@ \t%s\t: RESERVED DEBUG 2 output", DebugModeString[DBG_RESERVED2]);
// RTOSprintf("\r\n@ \t%s\t: RESERVED DEBUG 3 output", DebugModeString[DBG_RESERVED3]);
#if ENABLE_MULTI_DEBUG == TRUE
        RTOSprintf("\r\n@ \t%s\t: ALL debug output", DebugModeString[DBG_ALL]);
#endif

        bResult = TRUE;

        break;

    case '?':
    {
        char *pcPtr;

        pcPtr = szCmdMsg;

        sprintf(pcPtr, "@ %s", STR_AS_DEBUG_MODE);

#if ENABLE_MULTI_DEBUG == FALSE
        for (uint32_t u32DebugMode = (DebugModeTypeDef)0; u32DebugMode < DBG_ALL; u32DebugMode++)
        {
            if (g_Setting.u32DebugMode == DebugModeValue[u32DebugMode])
            {
                pcPtr += strlen(pcPtr);

                sprintf(pcPtr, "%s", DebugModeString[u32DebugMode]);

                bResult = TRUE;

                break;
            }
        }
#else
        bool bDbgOff = TRUE;
        bool bSingleMode = TRUE;

        for (uint32_t u32DebugMode = (DebugModeTypeDef)1; u32DebugMode < DBG_ALL; u32DebugMode++)
        {
            if (g_Setting.u32DebugMode & DebugModeValue[u32DebugMode])
            {
                bDbgOff = FALSE;

                pcPtr += strlen(pcPtr);

                if (bSingleMode == TRUE)
                    sprintf(pcPtr, "%s", DebugModeString[u32DebugMode]);
                else
                    sprintf(pcPtr, "|%s", DebugModeString[u32DebugMode]);

                bSingleMode = FALSE;
            }
        }

        if (bDbgOff == TRUE)
        {
            pcPtr += strlen(pcPtr);

            sprintf(pcPtr, "%s", DebugModeString[DBG_OFF]);
        }
#endif

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    default:
    {
        DebugModeTypeDef DebugMode;

#if ENABLE_MULTI_DEBUG == FALSE
        for (DebugMode = (DebugModeTypeDef)0; DebugMode < DBG_ALL; DebugMode++)
        {
            if (strcmp(pStr, DebugModeString[DebugMode]) == 0)
            {
                if (DebugMode == DBG_OFF)
                {
                    g_Setting.u32DebugMode = DebugModeValue[DBG_OFF];

                    sprintf(szCmdMsg, "@ DEBUG OFF");

                    RTOSprintf("\r\n%s", szCmdMsg);
                }
                else
                {
                    g_Setting.u32DebugMode = DebugModeValue[DebugMode];

                    sprintf(szCmdMsg, "@ DEBUG %s ON", DebugModeString[DebugMode]);

                    RTOSprintf("\r\n%s", szCmdMsg);
                }

                bResult = TRUE;

                break;
            }
        }
#else
        uint8_t u8OptionCnt = 0;

        while ((strlen(pStr) > 0) && (u8OptionCnt < 17))
        {
            StopChar(pStr, '|');

            // RTOSprintf("\r\n> Require debug mode:%s", pStr);

            for (DebugMode = (DebugModeTypeDef)0; DebugMode <= DBG_ALL; DebugMode++)
            {
                if (strcmp(pStr, DebugModeString[DebugMode]) == 0)
                {
                    if (DebugMode == DBG_OFF)
                    {
                        g_Setting.u32DebugMode = DebugModeValue[DBG_OFF];

                        sprintf(szCmdMsg, "@ DEBUG ALL OFF");

                        RTOSprintf("\r\n%s", szCmdMsg);
                    }
                    else if (DebugMode == DBG_ALL)
                    {
                        g_Setting.u32DebugMode = DebugModeValue[DBG_ALL];

                        sprintf(szCmdMsg, "@ DEBUG ALL ON");

                        RTOSprintf("\r\n%s", szCmdMsg);
                    }
                    else
                    {
                        if (UART_IsDebugMode(DebugMode) == FALSE)
                        {
                            g_Setting.u32DebugMode |= DebugModeValue[DebugMode];

                            sprintf(szCmdMsg, "@ DEBUG %s ON", DebugModeString[DebugMode]);

                            RTOSprintf("\r\n%s", szCmdMsg);
                        }
                        else
                        {
                            g_Setting.u32DebugMode &= ~DebugModeValue[DebugMode];

                            sprintf(szCmdMsg, "@ DEBUG %s OFF", DebugModeString[DebugMode]);

                            RTOSprintf("\r\n%s", szCmdMsg);
                        }
                    }

                    bResult = TRUE;

                    break;
                }
            }

            pStr += strlen(pStr) + 1;

            u8OptionCnt++;
        }
#endif

        System_SaveSetting();

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the record mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RecordMode(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RECORD_MODE, strlen(STR_AS_RECORD_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_RECORD_MODE);
        RTOSprintf("\r\n@ \t<mode> set record mode for data recording");
        RTOSprintf("\r\n@ \t%s\t: All record functions are disabled", RecordModeString[REC_OFF]);
        RTOSprintf("\r\n@ \t%s\t: Speed input interrupt (CAN/PULSE)", RecordModeString[REC_SPD_INT]);
        RTOSprintf("\r\n@ \t%s\t: CAN debug messages", RecordModeString[REC_CAN_DBG]);
        RTOSprintf("\r\n@ \t%s\t: Speed pulse count", RecordModeString[REC_PULSE_CNT]);
        RTOSprintf("\r\n@ \t%s\t: GPS speed", RecordModeString[REC_GPS_SPD]);
        RTOSprintf("\r\n@ \t%s\t: All record functions are enabled", RecordModeString[REC_ALL]);

        bResult = TRUE;

        break;

    case '?':
    {
        char *pcPtr;

        pcPtr = szCmdMsg;

        sprintf(pcPtr, "@ %s", STR_AS_RECORD_MODE);

        bool bRecOff = TRUE;
        bool bSingleMode = TRUE;

        for (uint32_t u32RecMode = (RecordModeTypeDef)1; u32RecMode < REC_ALL; u32RecMode++)
        {
            if (g_Setting.u32RecordMode & RecordModeValue[u32RecMode])
            {
                bRecOff = FALSE;

                pcPtr += strlen(pcPtr);

                if (bSingleMode == TRUE)
                    sprintf(pcPtr, "%s", RecordModeString[u32RecMode]);
                else
                    sprintf(pcPtr, "|%s", RecordModeString[u32RecMode]);

                bSingleMode = FALSE;
            }
        }

        if (bRecOff == TRUE)
        {
            pcPtr += strlen(pcPtr);

            sprintf(pcPtr, "%s", RecordModeString[REC_OFF]);
        }

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    default:
    {
        RecordModeTypeDef RecMode;
        uint8_t u8OptionCnt = 0;

        while ((strlen(pStr) > 0) && (u8OptionCnt < 7))
        {
            StopChar(pStr, '|');

            // RTOSprintf("\r\n> Require debug mode:%s", pStr);

            for (RecMode = (RecordModeTypeDef)0; RecMode <= REC_ALL; RecMode++)
            {
                if (strcmp(pStr, RecordModeString[RecMode]) == 0)
                {
                    if (RecMode == REC_OFF)
                    {
                        g_Setting.u32RecordMode = RecordModeValue[REC_OFF];

                        RTOSprintf("\r\n@ RECORD ALL OFF");

                        for (int i = REC_SPD_INT; i < REC_ALL; i++)
                        {
                            SD_RemoveFile((char *)RecordModeFile[i]);
                        }
                    }
                    else if (RecMode == REC_ALL)
                    {
                        g_Setting.u32RecordMode = RecordModeValue[REC_ALL];

                        RTOSprintf("\r\n@ RECORD ALL ON");
                    }
                    else
                    {
                        if (Datalog_IsRecordMode(RecMode) == FALSE)
                        {
                            g_Setting.u32RecordMode |= RecordModeValue[RecMode];

                            RTOSprintf("\r\n@ RECORD %s ON", RecordModeString[RecMode]);
                        }
                        else
                        {
                            g_Setting.u32RecordMode &= ~RecordModeValue[RecMode];

                            RTOSprintf("\r\n@ RECORD %s OFF", RecordModeString[RecMode]);

                            SD_RemoveFile((char *)RecordModeFile[RecMode]);
                        }
                    }

                    bResult = TRUE;

                    break;
                }
            }

            pStr += strlen(pStr) + 1;

            u8OptionCnt++;
        }

        System_SaveSetting();

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to driver list.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Multi_Set(char *pStr)
{
    bool bResult = FALSE;
    char cBusId[11] = {0}, cCmdBuf[40] = {0};
    float fBuf = 0;
    uint16_t u16Buf = 0;
    double dBuf = 0;

    if (strncmp(pStr, STR_MULTI_SET, strlen(STR_MULTI_SET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %sBUS_ID,SPEED_GAIN,RPM_DIV,DISTANCE", STR_MULTI_SET);
        RTOSprintf("\r\n@ Parameter 1: Bus ID, max 10 char");
        RTOSprintf("\r\n@ Parameter 2: Speed Gain");
        RTOSprintf("\r\n@ Parameter 3: Rpm Div, %d ~ %d", RPM_DIV_MIN, RPM_DIV_MAX);
        RTOSprintf("\r\n@ Parameter 4: Distance, Unit: 100M");

        bResult = TRUE;
        break;

    case '?':
        RTOSprintf("\r\n> This Command is Multi Setting Parameter.");

        bResult = TRUE;
        break;

    default:
        StopChar(pStr, ',');
        strcpy(cBusId, pStr);
        if ((strlen(cBusId) >= 11) || (strlen(pStr) == 0))
        {
            RTOSprintf("\r\n Parameter 1 Error.");
            break;
        }

        pStr += strlen(pStr) + 1;
        StopChar(pStr, ',');
        fBuf = atof(pStr);
        if ((fBuf <= 0) || (strlen(pStr) == 0))
        {
            RTOSprintf("\r\n Parameter 2 Error.");
            break;
        }

        pStr += strlen(pStr) + 1;
        StopChar(pStr, ',');
        u16Buf = atof(pStr);
        if ((u16Buf < RPM_DIV_MIN) || (u16Buf > RPM_DIV_MAX) || (strlen(pStr) == 0))
        {
            RTOSprintf("\r\n Parameter 3 Error.");
            break;
        }

        pStr += strlen(pStr) + 1;
        StopChar(pStr, ',');
        if (strlen(pStr) == 0)
        {
            RTOSprintf("\r\n Parameter 4 Error.");
            break;
        }
        dBuf = atof(pStr);

        sprintf(cCmdBuf, "$AS+SPEED GAIN=%f\r\n", fBuf);
        RTOSprintf("\r\n> %s", cCmdBuf);
        Protocl_ChkCmdPkg(cCmdBuf, NULL);

        sprintf(cCmdBuf, "$AS+RPM DIV=%d\r\n", u16Buf);
        RTOSprintf("\r\n> %s", cCmdBuf);
        Protocl_ChkCmdPkg(cCmdBuf, NULL);

        sprintf(cCmdBuf, "$AS+DISTANCE=%.0f\r\n", dBuf);
        RTOSprintf("\r\n> %s", cCmdBuf);
        Protocl_ChkCmdPkg(cCmdBuf, NULL);

        sprintf(cCmdBuf, "$AS+BUS ID=%s\r\n", cBusId);
        RTOSprintf("\r\n> %s", cCmdBuf);
        Protocl_ChkCmdPkg(cCmdBuf, NULL);

        sprintf(szCmdMsg, "@ %s%f,%d,%0.f,%s Set OK", STR_MULTI_SET, fBuf, u16Buf, dBuf, cBusId);

        bResult = TRUE;
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  .
// @param  .
// @retval .
//============================================================================//
bool CMD_Merge_Speed(char *pStr)
{
    bool bResult = FALSE;
    char *pCmdMsg;

    if (strncmp(pStr, STR_AS_MERGE_SPEED, strlen(STR_AS_MERGE_SPEED)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_MERGE_SPEED);
        RTOSprintf("\r\n@ ON: Merge Speed");
        RTOSprintf("\r\n@ OFF: Not Merge Speed");

        bResult = TRUE;
        break;

    case '?':
        pCmdMsg = szCmdMsg;

        if (g_Setting.tSpeedSet.u8MergeSpeed == 1)
        {
            sprintf(pCmdMsg, "\r\n@ %sON", STR_AS_MERGE_SPEED);
        }
        else if (g_Setting.tSpeedSet.u8MergeSpeed == 0)
        {
            sprintf(pCmdMsg, "\r\n@ %sOFF", STR_AS_MERGE_SPEED);
        }
        else
        {
            sprintf(pCmdMsg, "\r\n@ Parameter Error.");
        }

        RTOSprintf(szCmdMsg);

        bResult = TRUE;
        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.tSpeedSet.u8MergeSpeed = 1;

            sprintf(szCmdMsg, "\r\n@ %sON Set OK", STR_AS_MERGE_SPEED);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.tSpeedSet.u8MergeSpeed = 0;

            sprintf(szCmdMsg, "\r\n@ %sOFF Set OK", STR_AS_MERGE_SPEED);

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Input Error.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  .
// @param  .
// @retval .
//============================================================================//
bool CMD_DB9_Func(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Select;

    if (strncmp(pStr, STR_AS_DB9_FUNCTION, strlen(STR_AS_DB9_FUNCTION)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>, Mode=0 or 1", STR_AS_DB9_FUNCTION);

        bResult = TRUE;
        break;

    case '?':
        sprintf(szCmdMsg, "\r\n@ %s%d", STR_AS_DB9_FUNCTION, g_Setting.u8DB9Func);

        RTOSprintf(szCmdMsg);

        bResult = TRUE;
        break;

    default:
        u8Select = atoi(pStr);

        if (u8Select == 0)
        {
            g_Setting.u8DB9Func = u8Select;

            if (UART_IsTerminalUsingSET() == FALSE)
            {
                UART_Reset(SET_COMPORT_DEF);
                UART_ReInit(SET_COMPORT_DEF, BR_9600bps);
            }
            else
            {
                UART_Reset(DB9_COMPORT_DEF);
                UART_ReInit(DB9_COMPORT_DEF, BR_9600bps);
            }

            bResult = TRUE;
        }
        else if (u8Select == 1)
        {
            g_Setting.u8DB9Func = u8Select;

            if (UART_IsTerminalUsingSET() == FALSE)
            {
                UART_Reset(SET_COMPORT_DEF);
                UART_ReInit(SET_COMPORT_DEF, BR_115200bps);
            }
            else
            {
                UART_Reset(DB9_COMPORT_DEF);
                UART_ReInit(DB9_COMPORT_DEF, BR_115200bps);
            }

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "\r\n@ %s%d Set OK", STR_AS_DB9_FUNCTION, g_Setting.u8DB9Func);
        }
        else
        {
            sprintf(szCmdMsg, "\r\n@ Mode Set NG");
        }

        RTOSprintf(szCmdMsg);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  .
// @param  .
// @retval .
//============================================================================//
bool CMD_AG_DistLimit(char *pStr)
{
    bool bResult = FALSE;
    uint16_t u16Buf;

    if (strncmp(pStr, STR_AS_AG_DIST_LIMIT, strlen(STR_AS_AG_DIST_LIMIT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<Value>", STR_AS_AG_DIST_LIMIT);
        RTOSprintf("\r\n@ Value: X meter");

        bResult = TRUE;
        break;

    case '?':
        sprintf(szCmdMsg, "\r\n@ %s%d meter", STR_AS_AG_DIST_LIMIT, g_Setting.u16AG_Dist_Limit);

        RTOSprintf(szCmdMsg);

        bResult = TRUE;
        break;

    default:
        u16Buf = atoi(pStr);

        g_Setting.u16AG_Dist_Limit = u16Buf;

        bResult = TRUE;
        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to saving Backup Setting to ExFlash and
//         SD.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Save_Setting(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SAVE_SETTING, strlen(STR_AS_SAVE_SETTING)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_SAVE_SETTING);
        RTOSprintf("\r\n@ ON: Save ALL Backup Setting.");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            bResult = System_SaveBackupSettingAll(VAR_ALL_ITEMS);

            if (bResult == TRUE)
            {
                sprintf(szCmdMsg, "@ Save ALL Backup Setting to ExFlash and SD OK.");

                RTOSprintf("\r\n%s", szCmdMsg);
            }
            else
            {
                sprintf(szCmdMsg, "@ Save ALL Backup Setting to ExFlash and SD ERROR.");

                RTOSprintf("\r\n%s", szCmdMsg);
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to enabling RDP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Set_RDP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RDP_MODE, strlen(STR_AS_RDP_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_RDP_MODE);
        RTOSprintf("\r\n@ ON: Enable RDP.");

        bResult = TRUE;

        break;

    case '?':
    {
        bResult = System_CheckRDP();

        sprintf(szCmdMsg, "@ %s%s",
                STR_AS_RDP_MODE,
                bResult ? (char *)"ON" : (char *)"OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (System_CheckRDP() == TRUE)
            {
                RTOSprintf("\r\n> RDP is already enabled");

                bResult = TRUE;
            }
            else
            {
#if ENABLE_GSM_NET
                QtEC21_CheckRedoFTP();
#endif

                System_SaveSettingFinal();
                System_SaveBackupSettingAll(VAR_ALL_ITEMS);

                RTOSprintf("\r\n> ENABLE RDP OK");

                bResult = System_SetRDP();
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to disabling RDP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Clr_RDP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_CLEAR_RDP, strlen(STR_AS_CLEAR_RDP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_CLEAR_RDP);
        RTOSprintf("\r\n@ ON: Disable RDP.");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (System_CheckRDP() == FALSE)
            {
                RTOSprintf("\r\n> RDP is already disabled");

                bResult = TRUE;
            }
            else
            {
                HW_WriteBackupRegister(RDP_DISABLE_CHK, BKP_RDP_OFF_VALUE);

#if ENABLE_GSM_NET
                QtEC21_CheckRedoFTP();
#endif

                System_SaveSettingFinal();
                System_SaveBackupSettingAll(VAR_ALL_ITEMS);

                RTOSprintf("\r\n> DISABLE RDP");

                if (bRTOS_En)
                {
                    osMutexWait(mutex_setting_user, S_2_mS(5));
                }

                System_Reset();
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to enabling WRP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Set_WRP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_WRP_MODE, strlen(STR_AS_WRP_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_WRP_MODE);
        RTOSprintf("\r\n@ ON: Enable WRP.");
        RTOSprintf("\r\n@ OFF: Disable WRP.");

        bResult = TRUE;

        break;

    case '?':
    {
        bResult = System_CheckWRP();

        RTOSprintf("\r\n@ %s%s", STR_AS_WRP_MODE, bResult ? (char *)"ON" : (char *)"OFF");

        bResult = TRUE;

        break;
    }
    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (System_CheckWRP() == TRUE)
            {
                RTOSprintf("\r\n> WRP is already enabled");

                bResult = TRUE;
            }
            else
            {
                bResult = System_EnableWRP();
            }
        }
        if (strcmp(pStr, "OFF") == 0)
        {
            if (System_CheckWRP() == FALSE)
            {
                RTOSprintf("\r\n> WRP is already disabled");

                bResult = TRUE;
            }
            else
            {
                bResult = System_DisableWRP();
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to disabling WRP.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Clr_WRP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_CLEAR_WRP, strlen(STR_AS_CLEAR_WRP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_CLEAR_WRP);
        RTOSprintf("\r\n@ ON: Disable WRP.");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (System_CheckWRP() == FALSE)
            {
                RTOSprintf("\r\n> WRP is already disabled");

                bResult = TRUE;
            }
            else
            {
                bResult = System_DisableWRP();
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to delete Backup Setting in ExFlash and
//         SD.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Delete_Setting(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_BKP_SETTING_DEL, strlen(STR_AS_BKP_SETTING_DEL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_BKP_SETTING_DEL);
        RTOSprintf("\r\n@ ON: Delete ALL Backup Setting.");

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            System_DelBackupSetting();

            sprintf(szCmdMsg, "@ Delete ALL Backup Setting in ExFlash and SD OK.");

            RTOSprintf("\r\n%s", szCmdMsg);
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        bResult = TRUE;

        break;
    }

    return bResult;
}

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
//============================================================================//
// @brief  Process the command related to upload backup setting file to FTP server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_UploadSettingFTP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SETTING_FTP_UPLOAD, strlen(STR_AS_SETTING_FTP_UPLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_SETTING_FTP_UPLOAD);
        RTOSprintf("\r\n@ ON: Upload backup setting file to FTP server.");

        bResult = TRUE;

        break;

    default:
    {
        time_t ttTime;

        if (strcmp(pStr, "ON") == 0)
        {
            ttTime = System_GetTime();

            System_UploadBackupSettingToFTP(ttTime);

            sprintf(szCmdMsg, "@ %success.", STR_AS_SETTING_FTP_UPLOAD);

            RTOSprintf("\r\n%s", szCmdMsg);
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to restore setting from backup file on
//         FTP server.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DownloadSettingFTP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SETTING_FTP_DOWNLOAD, strlen(STR_AS_SETTING_FTP_DOWNLOAD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<DATE>", STR_AS_SETTING_FTP_DOWNLOAD);
        RTOSprintf("\r\n@ <DATE>: Backup file date, format: YYYYMMDD.");

        bResult = TRUE;

        break;

    default:
    {
        GSM_DownloadRequestTypeDef gsmDownloadHandle = {0};

        if (strlen(pStr) == 8)
        {
            if (strcmp(g_Setting.tBusInfo.cBus_ID, DEFAULT_STRING) == 0)
            {
                RTOSprintf("\r\n> System download backup setting file from FTP server BUS ID invalid.");

                return FALSE;
            }

            sprintf(gsmDownloadHandle.cSrcFile, "%s_%s.%s",
                    g_Setting.tBusInfo.cBus_ID,
                    pStr,
                    (char *)BACKUP_SETTING_FTP_EXT);

            sprintf(gsmDownloadHandle.cDstFile, "%s", FILENAME_BACKUP_SETTING_TEMP);

            sprintf(gsmDownloadHandle.cSrcDir, "%s", DIRNAME_FTP_BACKUP_SETTING);

            sprintf(gsmDownloadHandle.cDstDir, "%s", DIRNAME_SETTING);

            gsmDownloadHandle.u8ServerID = 1;

            gsmDownloadHandle.dlType = FTP_DOWNLOAD_SETTING;

            bResult = GSM_FTP_FileDownloadRequest(&gsmDownloadHandle);

            if (bResult)
            {
                sprintf(szCmdMsg, "@ %ssuccess.", STR_AS_SETTING_FTP_DOWNLOAD);

                RTOSprintf("\r\n%s", szCmdMsg);
            }
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        break;
    }
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to LCD saving.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_LCD_Saver(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_LCD_SAVER, strlen(STR_AS_LCD_SAVER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        sprintf(szCmdMsg, "@ %s<mode>", STR_AS_LCD_SAVER);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_LCD_SAVER, (g_Setting.bLCDSaver == TRUE) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            g_Setting.bLCDSaver = TRUE;

            bResult = TRUE;
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.bLCDSaver = FALSE;

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n@ Parameter INVALID.");
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_LCD_SAVER, (g_Setting.bLCDSaver == TRUE) ? "ON" : "OFF");

            RTOSprintf("\r\n%s", szCmdMsg);
        }

        break;
    }

    return bResult;
}

// For Gary Test
//============================================================================//
// @brief  Process the command related to driver list.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DRIVE_LIST(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Buf, i;

    if (strncmp(pStr, STR_AS_DRIVE_LIST, strlen(STR_AS_DRIVE_LIST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n> This Command is Drive Time list.");

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        u8Buf = atoi(pStr);

        if (u8Buf == 1)
        {
            for (i = 0; i < 14; i++)
            {
                RTOSprintf("\r\n### No. %d ###", i + 1);
                RTOSprintf("\r\n> Work: %d", g_Setting.tDrvInfo[0].stDrvRecDay[i].u32WorkTime);
                RTOSprintf("\r\n> Pause: %d", g_Setting.tDrvInfo[0].stDrvRecDay[i].u32PauseTime);
                RTOSprintf("\r\n> Standby: %d", g_Setting.tDrvInfo[0].stDrvRecDay[i].u32StandByTime);
                RTOSprintf("\r\n> Rest: %d", g_Setting.tDrvInfo[0].stDrvRecDay[i].u32RestTime);
            }

            bResult = TRUE;
        }
        else if (u8Buf == 2)
        {
            for (i = 0; i < 14; i++)
            {
                RTOSprintf("\r\n### No. %d ###", i + 1);
                RTOSprintf("\r\nWork: %d", g_Setting.tDrvInfo[1].stDrvRecDay[i].u32WorkTime);
                RTOSprintf("\r\nPause: %d", g_Setting.tDrvInfo[1].stDrvRecDay[i].u32PauseTime);
                RTOSprintf("\r\nStandby: %d", g_Setting.tDrvInfo[1].stDrvRecDay[i].u32StandByTime);
                RTOSprintf("\r\nRest: %d", g_Setting.tDrvInfo[1].stDrvRecDay[i].u32RestTime);
            }

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Driver ID Error.");
        }

        break;
    }

    return bResult;
}

// Gary's Test Commands
//============================================================================//
// @brief  Process the command related to parameter setting.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SET_PARAMETER(char *pStr)
{
    bool bResult = FALSE;
    // char szTEMP[30];
    uint8_t u8Buf, i, j;
    uint32_t u32Buff;

    if (strncmp(pStr, STR_AS_PARAMETER, strlen(STR_AS_PARAMETER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n> This Command is Gary test use.");
        RTOSprintf("\r\n> (0) Auto Set Parameter ");
        RTOSprintf("\r\n> (1) Reset All Work & Rest Time");
        RTOSprintf("\r\n> (2) Reset DISTANCE");
        RTOSprintf("\r\n> (3) Dec Driver Work Time");
        RTOSprintf("\r\n> (4) Add Driver Work Time");
        RTOSprintf("\r\n> (5) Add PreError Driver Work Time");
        RTOSprintf("\r\n> (6) Add Error Driver Work Time");

        bResult = TRUE;

        break;

    default:
        StopChar(pStr, ',');

        u8Buf = atoi(pStr);

        if (u8Buf == 0)
        {
            sprintf(szTEMP, "$AS+SPEED GAIN=196.07\r\n");

            RTOSprintf("\r\n> %s", szTEMP);

            Protocl_ChkCmdPkg(szTEMP, NULL);

            sprintf(szTEMP, "$AS+DRIVER IN=1,12345678\r\n");

            RTOSprintf("\r\n> %s", szTEMP);

            Protocl_ChkCmdPkg(szTEMP, NULL);

            sprintf(szTEMP, "$AS+DRIVER IN=2,87654321\r\n");

            RTOSprintf("\r\n> %s", szTEMP);

            Protocl_ChkCmdPkg(szTEMP, NULL);

            // sprintf(szTEMP, "$AS+BUS SN=JAS20899\r\n");

            // RTOSprintf("\r\n> %s", szTEMP);

            // Protocl_ChkCmdPkg(szTEMP, NULL);

            sprintf(szTEMP, "$AS+BUS ID=MARK01\r\n");

            RTOSprintf("\r\n> %s", szTEMP);

            Protocl_ChkCmdPkg(szTEMP, NULL);
        }
        else if (u8Buf == 1)
        {
            RTOSprintf("\r\n### Reset All Work & Rest Time. ###");

            for (j = 0; j < 2; j++)
            {
                for (i = 0; i < 14; i++)
                {
                    g_Setting.tDrvInfo[j].stDrvRecDay[i].u32WorkTime = 0;
                    g_Setting.tDrvInfo[j].stDrvRecDay[i].u32PauseTime = 0;
                    g_Setting.tDrvInfo[j].stDrvRecDay[i].u32StandByTime = 0;
                    g_Setting.tDrvInfo[j].stDrvRecDay[i].u32RestTime = 0;
                }

                g_Setting.tDrvInfo[j].u32DrWorkTime = 0;
                g_Setting.tDrvInfo[j].u32DrRestTime = 0;
                g_Setting.tDrvInfo[j].dDriverDistance = 0;
            }

            System_SaveSetting();
        }
        else if (u8Buf == 2)
        {
            sprintf(szTEMP, "$AS+DISTANCE=0\r\n");

            RTOSprintf("\r\n> %s", szTEMP);

            Protocl_ChkCmdPkg(szTEMP, NULL);

            System_SetupDefault();

            Delay_mS(S_2_mS(3));

            RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

#if ENABLE_GSM_NET
            QtEC21_CheckRedoFTP();
#endif

            System_SaveSettingFinal();
            System_SaveBackupSettingAll(VAR_ALL_ITEMS);

            if (bRTOS_En)
            {
                osMutexWait(mutex_setting_user, S_2_mS(5));
            }

            System_Reset();
        }
        else if (u8Buf == 3)
        {
            pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            u32Buff = atoi(pStr);

            g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime - u32Buff;

            System_SaveSetting();
        }
        else if (u8Buf == 4)
        {
            pStr += strlen(pStr) + 1;

            StopChar(pStr, ',');

            u32Buff = atoi(pStr);

            g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrWorkTime + u32Buff;

            System_SaveSetting();
        }

        bResult = TRUE;

        break;
    }

    return bResult;
}

//[Nancy add bus id]
#if ENABLE_GSM_NET
//============================================================================//
// @brief  Process the command related to set default BUS ID as IMEI.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_BusId_Set(char *pStr)
{
    bool bResult = FALSE;
    char cEBuf[11] = {0};

    if (strncmp(pStr, STR_AS_BUSID_SET, strlen(STR_AS_BUSID_SET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_BUSID_SET);

        RTOSprintf("\r\n@ IMEI: Enable BUS ID default setting, default is IMEI data.");

        bResult = TRUE;

        break;

    case '?':
        memcpy(cEBuf, &g_Setting.tGSM_SET.tMobileInfo.cIMEI[5], sizeof(cEBuf));

        if (strcmp(g_Setting.tBusInfo.cBus_ID, &g_Setting.tGSM_SET.tMobileInfo.cIMEI[5]) == 0)
        {
            sprintf(szCmdMsg, "@ %sBUS ID is IMEI.", STR_AS_BUSID_SET);
        }
        else
        {
            sprintf(szCmdMsg, "@ %sBUS ID is not IMEI.", STR_AS_BUSID_SET);
        }

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (bIsIMEIReady == TRUE)
        {
            if (strcmp(pStr, "IMEI") == 0)
            {
                // searching IMEI from g_setting
                memset(g_Setting.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));

                memcpy(g_Setting.tBusInfo.cBus_ID, &g_Setting.tGSM_SET.tMobileInfo.cIMEI[5], sizeof(g_Setting.tBusInfo.cBus_ID));

                System_SaveSetting();

                if (bSD_ReportErrFlag == FALSE)
                    V161_RecordTechnical(1);

                RTOSprintf("\r\n@ BUS ID set to IMEI OK");
            }
            else
            {
                RTOSprintf("\r\n@ Parameter INVALID");
            }

            bResult = TRUE;
        }
        else
        {
            sprintf(szCmdMsg, "@ %s4G module not Ready", STR_AS_BUSID_SET);
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to set VDR Now.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_VdrNow_Set(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_VDR_NOW, strlen(STR_AS_VDR_NOW)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s[ON/OFF]", STR_AS_VDR_NOW);

        RTOSprintf("\r\n@ This command Set VDR Now or VDR download.");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_VDR_NOW, (g_Setting.bVdrNow == TRUE) ? "ON" : "OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if (strcmp(pStr, "ON") == 0)
        {
            if (g_Setting.bVdrNow == FALSE)
            {
                V161_CreateVDRNowFile();
            }

            g_Setting.bVdrNow = TRUE;

            bResult = TRUE;
        }
        else if (strcmp(pStr, "OFF") == 0)
        {
            g_Setting.bVdrNow = FALSE;

            SD_RemoveFile(FILENAME_VDR_NOW_TMP);

            bResult = TRUE;
        }

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_VDR_NOW, (g_Setting.bVdrNow == TRUE) ? "ON" : "OFF");
        }
        else
        {
            sprintf(szCmdMsg, "@ Parameters Error!!");
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to set XTRA file URL.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_XTRA_URL(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_XTRA_URL, strlen(STR_AS_XTRA_URL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<URL>", STR_AS_XTRA_URL);

        RTOSprintf("\r\n@ <URL>: XTRA file URL.");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_XTRA_URL, g_Setting.cXTRA_URL);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) == 0) || (strlen(pStr) > sizeof(g_Setting.cXTRA_URL)))
        {
            RTOSprintf("\r\n@ INVALID XTRA URL");

            break;
        }

        sprintf(g_Setting.cXTRA_URL, "%s", pStr);

        bResult = System_SaveSetting();

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_XTRA_URL, g_Setting.cXTRA_URL);
        }
        else
        {
            sprintf(szCmdMsg, "@ Parameters Error!!");
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to set EPO file URL.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_EPO_URL(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EPO_URL, strlen(STR_AS_EPO_URL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<URL>", STR_AS_EPO_URL);

        RTOSprintf("\r\n@ <URL>: EPO file URL.");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%s", STR_AS_EPO_URL, g_Setting.cEPO_URL);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) == 0) || (strlen(pStr) > sizeof(g_Setting.cEPO_URL)))
        {
            RTOSprintf("\r\n@ INVALID EPO URL");

            break;
        }

        sprintf(g_Setting.cEPO_URL, "%s", pStr);

        bResult = System_SaveSetting();

        if (bResult == TRUE)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_EPO_URL, g_Setting.cEPO_URL);
        }
        else
        {
            sprintf(szCmdMsg, "@ Parameters Error!!");
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}

#if ENABLE_MG82
//============================================================================//
// @brief  Process the command related to enable/disable key reset function.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_KEY_RST(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_KEY_RST, strlen(STR_AS_KEY_RST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_KEY_RST);

        RTOSprintf("\r\n@ <OPTION>: ON/OFF");

        bResult = TRUE;

        break;

    case '?':
    {
        bool bOption = FALSE;

        MG82_KeyRstGet(&bOption);

        sprintf(szCmdMsg, "@ %s%s", STR_AS_KEY_RST, bOption ? (char *)"ON" : (char *)"OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }

    default:
        if (strncmp(pStr, (char *)"ON", 2) == 0)
        {
            bResult = MG82_KetRstEn(TRUE);

            if (bResult)
                sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_KEY_RST, pStr);
        }
        else if (strncmp(pStr, (char *)"OFF", 3) == 0)
        {
            bResult = MG82_KetRstEn(FALSE);

            if (bResult)
                sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_KEY_RST, pStr);
        }
        else
        {
            sprintf(szCmdMsg, "@ Parameters Error!!");
        }

        RTOSprintf("\r\n> %s", szCmdMsg);

        break;
    }

    return bResult;
}
#endif

// For Gary Test
//============================================================================//
// @brief  Process the command related to driver status.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DRIVE_STATUS(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_DRIVE_STATUS, strlen(STR_AS_DRIVE_STATUS)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n> Driver 1:");
        RTOSprintf("\r\n> WorkTime: %d", g_Setting.tDrvInfo[0].u32DrWorkTime);
        RTOSprintf("\r\n> RestTime: %d", g_Setting.tDrvInfo[0].u32DrRestTime);
        RTOSprintf("\r\n> dDriverDistance: %f", g_Setting.tDrvInfo[0].dDriverDistance);
        RTOSprintf("\r\n> Day_PauseTime: %d", g_Setting.tDrvInfo[0].stDrvRecDay[13].u32PauseTime);
        RTOSprintf("\r\n> Day_WorkTime: %d", g_Setting.tDrvInfo[0].stDrvRecDay[13].u32WorkTime);
        RTOSprintf("\r\n> Day_StandByTime: %d", g_Setting.tDrvInfo[0].stDrvRecDay[13].u32StandByTime);
        RTOSprintf("\r\n> Day_RestTime: %d", g_Setting.tDrvInfo[0].stDrvRecDay[13].u32RestTime);

        RTOSprintf("\r\n> Driver 2:");
        RTOSprintf("\r\n> WorkTime: %d", g_Setting.tDrvInfo[1].u32DrWorkTime);
        RTOSprintf("\r\n> RestTime: %d", g_Setting.tDrvInfo[1].u32DrRestTime);
        RTOSprintf("\r\n> dDriverDistance: %f", g_Setting.tDrvInfo[1].dDriverDistance);
        RTOSprintf("\r\n> Day_PauseTime: %d", g_Setting.tDrvInfo[1].stDrvRecDay[13].u32PauseTime);
        RTOSprintf("\r\n> Day_WorkTime: %d", g_Setting.tDrvInfo[1].stDrvRecDay[13].u32WorkTime);
        RTOSprintf("\r\n> Day_StandByTime: %d", g_Setting.tDrvInfo[1].stDrvRecDay[13].u32StandByTime);
        RTOSprintf("\r\n> Day_RestTime: %d", g_Setting.tDrvInfo[1].stDrvRecDay[13].u32RestTime);

        bResult = TRUE;

        break;

    default:
        RTOSprintf("\r\n> This Command is check drive status.");

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to RS232 test.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TEST_RS232(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Msg[15];

    if (strncmp(pStr, STR_AS_TEST_RS232, strlen(STR_AS_TEST_RS232)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
        RTOSprintf("\r\n> This command is test rs232.");
        bResult = TRUE;

        break;

    case '\0':
    default:
        UART_Reset(DB9_COMPORT_DEF);

        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)"TEST_RS232", 10);

        UART_GetRecvDataPos(DB9_COMPORT_DEF, 0, u8Msg, 10);

        if (strncmp((char *)u8Msg, "TEST_RS232", 10) == 0)
        {
            RTOSprintf("\r\n> DB9 Test OK.");
            if (g_bPTestMode == TRUE)
            {
                LCD_SetShowAutoTestPage(92);
            }
        }
        else
        {
            RTOSprintf("\r\n> DB9 Test NG.");
            if (g_bPTestMode == TRUE)
            {
                LCD_SetShowAutoTestPage(93);
            }
        }
        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to LCD backlight control.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TEST_BackLight(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TEST_BL, strlen(STR_AS_TEST_BL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n> This command is test LCD back light.");
        bResult = TRUE;

        break;

    default:
        if (strncmp(pStr, "ON", 8) == 0)
        {
            HW_PBClr(LCD_BL);
        }
        else if (strncmp(pStr, "OFF", 8) == 0)
        {
            HW_PBSet(LCD_BL);
        }

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to device test mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TEST_Mode(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TEST_MODE, strlen(STR_AS_TEST_MODE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
        if (g_bPTestMode == TRUE)
        {
            sprintf(szCmdMsg, "@ %sON", STR_AS_TEST_MODE);
        }
        else
        {
            sprintf(szCmdMsg, "@ %sOFF", STR_AS_TEST_MODE);
        }

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    case '\0':
        RTOSprintf("\r\n> This command is Set Test Mode.");

        bResult = TRUE;

        break;

    default:
        if (strncmp(pStr, "ON", 8) == 0)
        {
            g_bPTestMode = TRUE;
        }
        else if (strncmp(pStr, "OFF", 8) == 0)
        {
            g_bPTestMode = FALSE;
        }

        sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_TEST_MODE, (g_bPTestMode == TRUE) ? "ON" : "OFF");

        bResult = TRUE;

        break;
    }

    return bResult;
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
//============================================================================//
// @brief  Process the command related to sending GPS AT commands.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GPS_CMD(char *pStr)
{
    bool bResult = FALSE;
    char cBuf[20];

    if (strncmp(pStr, STR_AS_GPS_CMD, strlen(STR_AS_GPS_CMD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n> This command is Send GPS AT Command.");
        bResult = TRUE;
        break;

    default:
        if (strncmp(pStr, "COOL", 4) == 0)
        {
            sprintf(cBuf, "$PMTK103*30\r\n");

            GPS_SendBuffer((uint8_t *)cBuf, sizeof(cBuf));

            memset(&g_SysInfo.tGPS_Data, 0x00, sizeof(GPS_DataTypeDef));
        }
        bResult = TRUE;

        break;
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to sending GPS AT commands.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_Check_Header(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8Header[2];

    if (strncmp(pStr, STR_AS_CHECK_HEADER, strlen(STR_AS_CHECK_HEADER)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n> This command is check header Command.");
        bResult = TRUE;
        break;

    default:
        if (strncmp(pStr, "ON", 2) == 0)
        {
            SD_ReadFileToBuff(FILENAME_MIN_VDR_TMP, u8Header, 2);

            RTOSprintf("\r\n> ### Header: %02X %02X ###", u8Header[0], u8Header[1]);
        }
        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  .
// @param  .
// @retval .
//============================================================================//
bool CMD_AutoGain_Parameter(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_AUTOGAIN_PARA, strlen(STR_AS_AUTOGAIN_PARA)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        sprintf(szCmdMsg, "@ This command is check AutoGain parameter Command.");

        bResult = TRUE;
        break;

    default:
    case '?':
        sprintf(szCmdMsg, "@ %sC:%d S:%d O:%d DD:%d D:%.0f",
                STR_AS_AUTOGAIN_PARA,
                g_Setting.tSpeedSet.u8Auto_Gain_Count,
                g_Setting.tSpeedSet.u16Star,
                g_Setting.tSpeedSet.u8Offset,
                g_Setting.u16AG_Dist_Limit,
                g_SysInfo.tVehicleState.dDistance_GPS);

        bResult = TRUE;
        break;
    }

    RTOSprintf("\r\n> %s", szCmdMsg);

    return bResult;
}

//============================================================================//
// @brief  Process the command related to the engine mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
/*
bool CMD_ENG_MODE(char *pStr)
{
    bool bResult = FALSE;
    EventDataTypeDef stRecEvent;
    uint32_t u32NowSec;


    if(strncmp(pStr, STR_AS_ENG_MODE, strlen(STR_AS_ENG_MODE)) != 0)
        return FALSE;

    //RTOSprintf("### CMD_ENG_MODE ###\r\n");

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
        case '\0':
            RTOSprintf("\r\n@ %s<OPTION>", STR_AS_ENG_MODE);
          RTOSprintf("\r\n@ \t<OPTION>:ON/OFF");

            bResult = TRUE;

            break;

        case '?':
            sprintf(szCmdMsg, "@ %s%s",
                    STR_AS_ENG_MODE,
                    (g_Setting.tENABLE_SET.bits.ENG_MODE == 1) ? "ON" : "OFF");

            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = TRUE;

            break;

        default:
            if(strncmp(pStr, "13091876",8) == 0)
            {
                StopChar(pStr, ',');

                pStr += strlen(pStr) + 1;

                if(strcmp(pStr, "ON") == 0)
                {
                    g_Setting.tENABLE_SET.bits.ENG_MODE = 1;

                    RTOSprintf("\r\n@ %sON", STR_AS_ENG_MODE);

                    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_ADMIN_ON);

                    stRecEvent.u8Code = 2;

                    u32NowSec = System_GetTime();

                    SecToBCDTime(u32NowSec, stRecEvent.u8DateStartBCD);

                    memcpy(stRecEvent.u8DateStopBCD, stRecEvent.u8DateStartBCD, 7);

          if(bSD_ReportErrFlag == FALSE)
                      V161_RecordEvent(stRecEvent);

                    bResult = TRUE;
                }

                if(strcmp(pStr, "OFF") == 0)
                {
                    g_Setting.tENABLE_SET.bits.ENG_MODE = 0;

                    RTOSprintf("\r\n@ %sOFF", STR_AS_ENG_MODE);

                    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_ADMIN_OFF);

                    bResult = TRUE;
                }

                if(bResult == TRUE)
                    System_SaveSetting();
            }

            break;
    }

    return bResult;
}
*/
// Brian's Test Commands

//============================================================================//
// @brief  Process the command related to Ring buffer reset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_RINGBUF_RESET(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_RINGBUF_RESET, strlen(STR_AS_RINGBUF_RESET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_RINGBUF_RESET);
        RTOSprintf("\r\n@ \t<OPTION>: UART port ID, range of 1-%c",
#if HW_TYPE == HW_JAS208
                   '5'
#elif HW_TYPE == HW_JAS306
                   '6'
#endif
        );

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8PortID;

        if (strlen(pStr) != 1)
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }

        u8PortID = atoi(pStr);

#if HW_TYPE == HW_JAS208
        if ((u8PortID == 0) || (u8PortID > 5))
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }
#elif HW_TYPE == HW_JAS306
        if ((u8PortID == 0) || (u8PortID > 6))
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }
#endif

        RingBuf_Reset(&rbBuf[u8PortID - 1]);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to UART reset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_UART_INT(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_UART_INT, strlen(STR_AS_UART_INT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_UART_INT);
        RTOSprintf("\r\n@ \t<OPTION>: UART port ID, range of 1-%c",
#if HW_TYPE == HW_JAS208
                   '5'
#elif HW_TYPE == HW_JAS306
                   '6'
#endif
        );

        bResult = TRUE;

        break;

    case '?':
    {
        bool bRXNE_En1 = FALSE, bERR_En1 = FALSE;
#if GNSS_ENABLE_GET_LOC == 0
        bool bRXNE_En2 = FALSE, bERR_En2 = FALSE;
#endif
        bool bRXNE_En3 = FALSE, bERR_En3 = FALSE;

        bRXNE_En1 = __HAL_UART_GET_IT_SOURCE(puartHandle[DB9_COMPORT_DEF], UART_IT_RXNE);
        bERR_En1 = __HAL_UART_GET_IT_SOURCE(puartHandle[DB9_COMPORT_DEF], UART_IT_ERR);

#if GNSS_ENABLE_GET_LOC == 0
        bRXNE_En2 = __HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
        bERR_En2 = __HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);
#endif

        bRXNE_En3 = __HAL_UART_GET_IT_SOURCE(puartHandle[GSM_COMPORT_DEF], UART_IT_RXNE);
        bERR_En3 = __HAL_UART_GET_IT_SOURCE(puartHandle[GSM_COMPORT_DEF], UART_IT_ERR);

        RTOSprintf("\r\n> %sDB9 INT RXNE: %d, ERR: %d", STR_AS_UART_INT, bRXNE_En1, bERR_En1);
#if GNSS_ENABLE_GET_LOC == 0
        RTOSprintf("\r\n> GPS INT RXNE: %d, ERR: %d", bRXNE_En2, bERR_En2);
#endif
        RTOSprintf("\r\n> GSM INT RXNE: %d, ERR: %d", bRXNE_En3, bERR_En3);

        bResult = TRUE;

        break;
    }

    default:
    {
        uint8_t u8PortID;

        if (strlen(pStr) != 1)
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }

        u8PortID = atoi(pStr);

#if HW_TYPE == HW_JAS208
        if ((u8PortID == 0) || (u8PortID > 5))
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }
#elif HW_TYPE == HW_JAS306
        if ((u8PortID == 0) || (u8PortID > 6))
        {
            RTOSprintf("\r\n@ Invalid UART port ID!");

            break;
        }
#endif

        // UART_ReInit((UART_PortTypeDef)(u8PortID - 1), BR_115200bps);
        __HAL_UART_DISABLE_IT(puartHandle[u8PortID - 1], UART_IT_RXNE);
        __HAL_UART_DISABLE_IT(puartHandle[u8PortID - 1], UART_IT_ERR);

        HAL_Delay(100);

        __HAL_UART_ENABLE_IT(puartHandle[u8PortID - 1], UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(puartHandle[u8PortID - 1], UART_IT_ERR);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to printf enable.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_PRINTF_EN(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_PRINTF_EN, strlen(STR_AS_PRINTF_EN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<OPTION>", STR_AS_PRINTF_EN);
        RTOSprintf("\r\n@ \t<OPTION>: ON, OFF");

        bResult = TRUE;

        break;

    case '?':
        RTOSprintf("\r\n@ %s%s", STR_AS_PRINTF_EN, bPrintfEn ? "ON" : "OFF");

        bResult = TRUE;

        break;

    default:
        if (strncmp(pStr, "ON", 2) == 0)
        {
            if (bPrintfEn == FALSE)
                bPrintfEn = TRUE;

            RTOSprintf("\r\n@ %sON.", STR_AS_PRINTF_EN);

            bResult = TRUE;
        }
        else if (strncmp(pStr, "OFF", 3) == 0)
        {
            if (bPrintfEn == TRUE)
            {
                RTOSprintf("\r\n@ %sOFF.", STR_AS_PRINTF_EN);

                bPrintfEn = FALSE;
            }

            bResult = TRUE;
        }
        else
            bResult = FALSE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to renaming file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FILE_RENAME(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FIL_RENAME, strlen(STR_AS_FIL_RENAME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<OLD NAME>,<NEW NAME>", STR_AS_FIL_RENAME);
        RTOSprintf("\r\n@ \t<OLD NAME>: File's current name, including path.");
        RTOSprintf("\r\n@ \t<NEW NAME>: New file name to rename to, including path.");

        bResult = TRUE;

        break;

    default:
    {
        uint16_t u16FindPos;
        char cFileName[60] = {0};

        if (!FindSequence(pStr, (char *)",", strlen(pStr), 1, &u16FindPos))
        {
            RTOSprintf("\r\n> Command format wrong!");

            bResult = FALSE;

            break;
        }

        StopChar(pStr, ',');

        if ((strlen(pStr) < 1) || (strlen(pStr) > sizeof(cFileName)))
        {
            RTOSprintf("\r\n@ Input old file name invalid!");

            bResult = FALSE;

            break;
        }

        strcpy(cFileName, pStr);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) < 1) || (strlen(pStr) > 80))
        {
            RTOSprintf("\r\n@ Input new file name invalid!");

            bResult = FALSE;

            break;
        }

        bResult = SD_Rename(cFileName, pStr);

        if (bResult == TRUE)
            RTOSprintf(STR_CMD_OK);
        else
            RTOSprintf(STR_CMD_ERR);

        RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingFinal();

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

#if ENABLE_INDEX_BUFFER
        for (int i = 0; i < MAX_INDEXED_FILE_NUM; i++)
        {
            if (strncmp(cFileName, cIndexedFileName[i], strlen(cIndexedFileName[i])) == 0)
            {
                memset(cIndexedFileName[i], 0x00, sizeof(cIndexedFileName[i]));
            }
        }
#endif

        System_Reset();

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to getting size of a file in SD card.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FILE_SIZE(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FIL_SIZE, strlen(STR_AS_FIL_SIZE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<FILE NAME>", STR_AS_FIL_SIZE);
        RTOSprintf("\r\n@ \t<FILE NAME>: Name of the file to get size from SD card, including path.");

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32FileSize = 0;

        if ((strlen(pStr) == 0) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n@ Input file name invalid!");

            bResult = FALSE;

            break;
        }

        u32FileSize = SD_GetFileSize(pStr);

        sprintf(szCmdMsg, "@ %s%d", STR_AS_FIL_SIZE, u32FileSize);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to finding a file with a given pattern.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FIND_FILE(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FIND_FILE, strlen(STR_AS_FIND_FILE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<DIR>/<PATTERN>", STR_AS_FIND_FILE);
        RTOSprintf("\r\n@ \t<DIR>: Path of the directory to find the file.");
        RTOSprintf("\r\n@ \t<PATTERN>: Pattern to find the file.");

        bResult = TRUE;

        break;

    default:
    {
        char cDir[64] = {0},
             cPattern[64] = {0},
             cReturnFile[128] = {0};

        if (sscanf(pStr, "%64[^/]/%64s", cDir, cPattern) == 2)
        {
            bResult = TRUE;
        }
        else if (sscanf(pStr, "/%64s", cPattern) == 1)
        {
            memset(cDir, 0x00, sizeof(cDir));

            bResult = TRUE;
        }
        else
        {
            bResult = FALSE;

            RTOSprintf("\r\n@ INVALID PARAMETER!");

            break;
        }

        bResult = SD_FindFileName(cDir, cPattern, cReturnFile);

        if (bResult == FALSE)
            bResult = SD_FindFilePattern(cDir, cPattern, cReturnFile);

        if (bResult)
        {
            sprintf(szCmdMsg, "@ %s%s", STR_AS_FIND_FILE, cReturnFile);
        }
        else
        {
            sprintf(szCmdMsg, "@ %sNo file", STR_AS_FIND_FILE);
        }

        RTOSprintf("\r\n%s", szCmdMsg);

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to removing file in SD card.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_FILE_REMOVE(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_FIL_DEL, strlen(STR_AS_FIL_DEL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<FILE NAME>", STR_AS_FIL_DEL);
        RTOSprintf("\r\n@ \t<FILE NAME>: Name of the file to delete from SD card, including path.");

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n@ Input file name invalid!");

            bResult = FALSE;

            break;
        }

        bResult = SD_RemoveFile(pStr);

        if (bResult == TRUE)
            RTOSprintf(STR_CMD_OK);
        else
            RTOSprintf(STR_CMD_ERR);

        RTOSprintf("\r\n@ Command OK !! Reset sytem\r\n");

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingFinal();

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

#if ENABLE_INDEX_BUFFER
        for (int i = 0; i < MAX_INDEXED_FILE_NUM; i++)
        {
            if (strncmp(pStr, cIndexedFileName[i], strlen(cIndexedFileName[i])) == 0)
            {
                memset(cIndexedFileName[i], 0x00, sizeof(cIndexedFileName[i]));
            }
        }
#endif

        System_Reset();

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to removing a directory in SD card.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DIR_REMOVE(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_DIR_DEL, strlen(STR_AS_DIR_DEL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<DIR NAME>", STR_AS_DIR_DEL);
        RTOSprintf("\r\n@ \t<DIR NAME>: Name of the directory to delete from SD card, including path.");

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n@ Input directory name invalid!");

            bResult = FALSE;

            break;
        }

        bResult = SD_RemoveDir(pStr);

        if (bResult == TRUE)
            RTOSprintf(STR_CMD_OK);
        else
            RTOSprintf(STR_CMD_ERR);

        RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingFinal();

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to removing file in VDR directory.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_VDR_DEL(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_VDR_DEL, strlen(STR_AS_VDR_DEL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<DEL_OPTION>", STR_AS_VDR_DEL);
        RTOSprintf("\r\n@ \t<DEL_OPTION>:");
        RTOSprintf("\r\n@ \tALL: Delete all files in VDR directory.");
        RTOSprintf("\r\n@ \t<DETAIL_STRING>: Delete all files having <DETAIL_STRING> in their name.");

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n> Input option invalid!");

            bResult = FALSE;

            break;
        }

        if (strncmp(pStr, "ALL", 3) == 0)
        {
            SD_ClearUpDir((char *)"VDR", (char *)"*.*", 0);
        }
        else
        {
            sprintf(pStr + strlen(pStr), ".VDR");

            SD_ClearUpDir((char *)"VDR", pStr, 0);
        }

        bResult = TRUE;

        if (bResult == TRUE)
            RTOSprintf(STR_CMD_OK);
        else
            RTOSprintf(STR_CMD_ERR);

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingAll(VAR_ALL_ITEMS);

        RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to removing file in DTR directory.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_DTR_DEL(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_DTR_DEL, strlen(STR_AS_DTR_DEL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<DEL_OPTION>", STR_AS_DTR_DEL);
        RTOSprintf("\r\n@ \t<DEL_OPTION>:");
        RTOSprintf("\r\n@ \tALL: Delete all files in DTR directory.");
        RTOSprintf("\r\n@ \t<DETAIL_STRING>: Delete all files having <DETAIL_STRING> in their name.");

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 50))
        {
            RTOSprintf("\r\n@ Input option invalid!");

            bResult = FALSE;

            break;
        }

        if (strncmp(pStr, "ALL", 3) == 0)
        {
            bResult = SD_ClearUpDir((char *)"DTR", (char *)"*.*", 0);
        }
        else
        {
            sprintf(pStr + strlen(pStr), ".DTR");

            bResult = SD_ClearUpDir((char *)"DTR", pStr, 0);
        }

        if (bResult == TRUE)
            RTOSprintf(STR_CMD_OK);
        else
            RTOSprintf(STR_CMD_ERR);

        RTOSprintf("\r\n@ Command OK !! Reset system\r\n");

#if ENABLE_GSM_NET
        QtEC21_CheckRedoFTP();
#endif

        System_SaveSettingFinal();
        System_SaveBackupSettingAll(VAR_ALL_ITEMS);

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to making VDR file.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_VDR_MAKING(char *pStr)
{
    bool bResult = FALSE;

    if (bSD_ReportErrFlag == TRUE)
        return FALSE;

    if (strncmp(pStr, STR_AS_VDR_MAKING, strlen(STR_AS_VDR_MAKING)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ %s<VDR_OPTION>", STR_AS_VDR_MAKING);
        RTOSprintf("\r\n@ \t<VDR_OPTION>:");
        RTOSprintf("\r\n@ \t1: Making daily VDR file.");
        RTOSprintf("\r\n@ \t2: Making hour VDR file.");

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8Option;

        if (strlen(pStr) != 1)
        {
            RTOSprintf("\r\n@ Input option invalid!");

            bResult = FALSE;

            break;
        }

        u8Option = atoi(pStr);

        if ((u8Option < 1) || (u8Option > 2))
        {
            RTOSprintf("\r\n@ Input option invalid!");

            bResult = FALSE;

            break;
        }

        if (u8Option == 1)
        {
            bDoVDR_DayFlag = TRUE;
        }
        else if (u8Option == 2)
        {
            bDoVDR_HourFlag = TRUE;
        }

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

#if ENABLE_GSM_NET
//============================================================================//
// @brief  Process the command related to resend Event report messages.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SEND_EVENT(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_EVENT, strlen(STR_AS_EVENT)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<EVENT CODE>", STR_AS_EVENT);

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 5))
        {
            RTOSprintf("\r\n@ Input event code invalid!");

            bResult = FALSE;

            break;
        }

        GSM_TCP_ReportEvent(pStr, STR_NULL);

        bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to making phone call.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSM_CALL(char *pStr)
{
    bool bResult = FALSE;
    GSM_StatusTypeDef gsmRes;

    if (strncmp(pStr, STR_AS_CALL, strlen(STR_AS_CALL)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '?':
    case '\0':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<PHONE NUMBER>", STR_AS_CALL);

        bResult = TRUE;

        break;

    default:
        if ((strlen(pStr) < 1) || (strlen(pStr) > 12))
        {
            RTOSprintf("\r\n@ Input phone number invalid!");

            bResult = FALSE;

            break;
        }

        gsmRes = GSM_Call_Dial(pStr);

        if (gsmRes == GSM_OK)
            bResult = TRUE;

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to send SMS.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_SEND_SMS(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SEND_SMS, strlen(STR_AS_SEND_SMS)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<PHONE NUMBER>,<SMS content>", STR_AS_SEND_SMS);
        RTOSprintf("\r\n@ \t<PHONE NUMBER>: Destination phone number.");
        RTOSprintf("\r\n@ \t<SMS content>: SMS content, max. 64 char.");

        bResult = TRUE;

        break;

    default:
    {
        GSM_MsgDataTypeDef smsHandle;
        char cPhoneNum[15] = {0};

        StopChar(pStr, ',');

        strcpy(cPhoneNum, pStr);

        pStr += strlen(pStr) + 1;

        if ((strlen(pStr) == 0) || (strlen(pStr) > 64))
        {
            RTOSprintf("\r\n@ Invalid SMS content length!");

            break;
        }

        smsHandle.pcPhoneNum = cPhoneNum;
        smsHandle.pcData = pStr;
        smsHandle.u16DataLen = strlen(pStr);

        // GSM_SMS_Send(&smsHandle);
        GSM_SMS_SavePkg(&smsHandle);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to send AT CMD.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AT_CMD(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_AT_CMD, strlen(STR_AS_AT_CMD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<command>", STR_AS_AT_CMD);
        RTOSprintf("\r\n@ \t<command> AT full command");

        bResult = TRUE;

        break;

    default:
    {
        char cCommand[80] = {0};

        // Add terminate sequence to the command
        sprintf(cCommand, "%s%s", pStr, CMD_END);

        RTOSprintf("\r\n> %s", cCommand);

        QtEC21_SendBuffer((uint8_t *)cCommand, (uint16_t)strlen(cCommand));

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to get a task's high watermark.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TASK_RAM(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TASK_RAM, strlen(STR_AS_TASK_RAM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<TASK_NAME>", STR_AS_TASK_RAM);
        RTOSprintf("\r\n@ \t<TASK_NAME>: ");
        RTOSprintf("%s", taskInfo[0].cTaskName);

        for (int i = 1; i < TASK_QUANTITY; i++)
            RTOSprintf(", %s", taskInfo[i].cTaskName);

        bResult = TRUE;

        break;

    default:
    {
        for (int i = 0; i < TASK_QUANTITY; i++)
        {
            if (strncmp(pStr, taskInfo[i].cTaskName, strlen(taskInfo[i].cTaskName)) == 0)
            {
                UBaseType_t ubaseVal;

                ubaseVal = uxTaskGetStackHighWaterMark(taskInfo[i].taskID);

                RTOSprintf("\r\n@ Task %s high water mark: %d words.", taskInfo[i].cTaskName, ubaseVal);

                bResult = TRUE;
            }
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to get a task's current step.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TASK_STEP(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TASK_STEP, strlen(STR_AS_TASK_STEP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<TASK_NAME>", STR_AS_TASK_STEP);
        RTOSprintf("\r\n@ \t<TASK_NAME>: ");
        RTOSprintf("%s", taskInfo[0].cTaskName);

        for (int i = 1; i < TASK_QUANTITY; i++)
            RTOSprintf(", %s", taskInfo[i].cTaskName);

        bResult = TRUE;

        break;

    default:
    {
        for (int i = 0; i < TASK_QUANTITY; i++)
        {
            if (strncmp(pStr, taskInfo[i].cTaskName, strlen(taskInfo[i].cTaskName)) == 0)
            {
                RTOSprintf("\r\n> Task %s is currently at step %d.",
                           taskInfo[i].cTaskName,
                           taskInfo[i].u16TaskStep);

                bResult = TRUE;
            }
        }

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to delaying a task.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_TASK_DELAY(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_TASK_DELAY, strlen(STR_AS_TASK_DELAY)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<TASK_NAME>", STR_AS_TASK_DELAY);
        RTOSprintf("\r\n@ \t<TASK_NAME>: ");
        RTOSprintf("%s", taskInfo[0].cTaskName);

        for (int i = 1; i < TASK_QUANTITY; i++)
            RTOSprintf(", %s", taskInfo[i].cTaskName);

        bResult = TRUE;

        break;

    default:
    {
        for (int i = 0; i < TASK_QUANTITY; i++)
        {
            if (strncmp(pStr, taskInfo[i].cTaskName, strlen(taskInfo[i].cTaskName)) == 0)
            {
                RTOSprintf("\r\n> Task %s is delayed now.", taskInfo[i].cTaskName);

                taskInfo[i].bTaskDelay = TRUE;

                bResult = TRUE;
            }
        }

        break;
    }
    }

    return bResult;
}

/*
//============================================================================//
// @brief  Process the command related to reading a file in USB.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_USB_READ(char *pStr)
{
  bool bResult = FALSE;


    if(strncmp(pStr, STR_AS_USB_READ, strlen(STR_AS_USB_READ)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch(*pStr)
    {
      case '\0':
        case '?':
            RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
          RTOSprintf("\r\n@ %s<USB FILE NAME>,<SIZE>", STR_AS_USB_READ);

          bResult = TRUE;

          break;

        default:
        {
            bool bRes;
            char cUSB_FileName[60] = {0};
            uint32_t u32GetSize, u32FileSize, u32DatCnt, u32DatSize;


          StopChar(pStr, ',');

            sprintf(cUSB_FileName, "%s", pStr);

            pStr += strlen(pStr) + 1;

            u32GetSize = atoi(pStr);

            if(u32GetSize == 0)
            {
              RTOSprintf("\r\n@ Invalid reading size!");

                break;
            }

            if(bUSB_Ready == FALSE)
            {
              RTOSprintf("\r\n@ USB is not ready!");

                break;
            }

            u32FileSize = USB_GetFileSize(cUSB_FileName);

            if(u32FileSize == 0)
            {
              RTOSprintf("\r\n@ USB file invalid!");

                break;
            }

            if(u32GetSize > u32FileSize)
            {
              u32GetSize = u32FileSize;

                break;
            }

            u32DatCnt = 0;

          RTOSprintf("\r\n@ Start reading file %s", cUSB_FileName);

            while(u32DatCnt < u32FileSize)
            {
                u32DatSize = u32FileSize - u32DatCnt;

                if(u32DatSize > sizeof(u8USB_ReadBuf))
                {
                  u32DatSize = sizeof(u8USB_ReadBuf);
                }

              bRes = USB_ReadAddrToBuff(cUSB_FileName, u8USB_ReadBuf, u32DatSize, u32DatCnt);

                if(bRes == FALSE)
                {
                  RTOSprintf("\r\n@ Read file %s at address %d ERROR", cUSB_FileName, u32DatCnt);

                    bResult = TRUE;

                    break;
                }

                u32DatCnt += u32DatSize;

                if(u32DatCnt % (sizeof(u8USB_ReadBuf) * 4) == 0)
                {
                  RTOSprintf("\r\n@ Finish reading %d bytes", u32DatCnt);
                }
            }

            RTOSprintf("\r\n@ Finish reading file %s", cUSB_FileName);

            bResult = TRUE;

          break;
        }
    }

    return bResult;
}
*/

//============================================================================//
// @brief  Process the command related to displaying an LCD page.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_LCD_SHOW(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_SHOW_LCD, strlen(STR_AS_SHOW_LCD)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<PAGE ID>", STR_AS_SHOW_LCD);

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8ID = atoi(pStr);

        if ((u8ID == 0) || (u8ID > 100))
        {
            RTOSprintf("\r\n@ Invalid LCD page ID!");

            break;
        }

        LCD_SetShowAutoTestPage(u8ID);

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

#if ENABLE_GSM_NET
//============================================================================//
// @brief  Process the command related to 4G module power.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_4G_PWR(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8ParamCnt = 0;

    if (strncmp(pStr, STR_AS_4G_PWR, strlen(STR_AS_4G_PWR)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_4G_PWR);
        RTOSprintf("\r\n@ <mode>: ON/OFF");

        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "%s%s\r\n",
                STR_AS_4G_PWR,
                bQtEC21_Pwr ? (char *)"ON" : (char *)"OFF");

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        u8ParamCnt++;

        if (strcmp(pStr, (char *)"ON") == 0)
        {
            // QtEC21_PwrEnable();
            bGSM_PwrOnFlag = TRUE;

            RTOSprintf("\r\n> 4G MODULE POWER ON");

            bResult = TRUE;
        }
        else if (strcmp(pStr, (char *)"OFF") == 0)
        {
            // QtEC21_PwrDisable();
            bGSM_PwrOffFlag = TRUE;

            RTOSprintf("\r\n> 4G MODULE POWER OFF");

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to 4G module reset.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_4G_RESET(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8ParamCnt = 0;

    if (strncmp(pStr, STR_AS_4G_RST, strlen(STR_AS_4G_RST)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_4G_RST);
        RTOSprintf("\r\n@ <mode>: ON");

        bResult = TRUE;

        break;

    default:
        u8ParamCnt++;

        if (strcmp(pStr, (char *)"ON") == 0)
        {
            QtEC21_Reset();

            RTOSprintf("\r\n> 4G MODULE RESET");

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        break;
    }

    return bResult;
}
#endif

//============================================================================//
// @brief  Process the command related to printing.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_PRINT(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_PRINT_ITEM, strlen(STR_AS_PRINT_ITEM)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %s<OPTION>,<HOURS>", STR_AS_PRINT_ITEM);
        RTOSprintf("\r\n@ \t<OPTION>: Printing item, range of 1-5, except 4");
        RTOSprintf("\r\n@ \t<HOURS>: Hours range if the printing item is 5");

        bResult = TRUE;

        break;

    default:
    {
        uint8_t u8Item, u8Hour;

        StopChar(pStr, ',');

        if (strlen(pStr) != 1)
        {
            RTOSprintf("\r\n@ Invalid printing item!");

            break;
        }

        u8Item = atoi(pStr);

        // if((u8Item == 0) || (u8Item == 4) || (u8Item > 5))
        if ((u8Item == 0) || (u8Item > 5))
        {
            RTOSprintf("\r\n@ Invalid printing item!");

            break;
        }

        if (u8Item == 5)
        {
            pStr += strlen(pStr) + 1;

            if (strlen(pStr) > 2)
            {
                RTOSprintf("\r\n@ Invalid printing hours!");

                break;
            }

            u8Hour = atoi(pStr);

            if ((u8Hour == 0) || (u8Hour > 12))
            {
                RTOSprintf("\r\n@ Invalid printing hours!");

                break;
            }

            u8HourDataCnt = u8Hour;

            bGetSpeedRecord = TRUE;

            while (bGetSpeedRecord)
            {
                PRN_GetSpeedRecord();
            }
        }
        else
        {
            PRN_Printing(u8Item);
        }

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to sleeping mode.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_MCU_SLEEP(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8ParamCnt = 0;

    if (strncmp(pStr, STR_AS_SLEEP, strlen(STR_AS_SLEEP)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
    case '?':
        RTOSprintf("\r\n@ THIS IS EXPERIMENTAL COMMAND!!!");
        RTOSprintf("\r\n@ %sON", STR_AS_SLEEP);

        bResult = TRUE;

        break;

    default:
        u8ParamCnt++;

        if (strcmp(pStr, (char *)"ON") == 0)
        {
            RTOSprintf("\r\n> ### SYSTEM ENTER STANDBY MODE ###");

#if ENABLE_MG82
            MG82_USBPROTECTION_Set(0);

            MG82_PWR_Mode(MG82_PWR_MODE_DEF);
#endif

            // HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

            // System_Reset();

            System_EnterStandbyMode();

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        break;
    }

    return bResult;
}

#if ENABLE_MG82
//============================================================================//
// @brief  Process the command related to 8051 WDT.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_805T_WDT_EN(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8ParamCnt = 0;
    uint8_t u8WDT_En, u8WDT_TimeOut;

    if (strncmp(pStr, STR_AS_8051_WDT_EN, strlen(STR_AS_8051_WDT_EN)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_8051_WDT_EN);
        RTOSprintf("\r\n@ <mode>: ON/OFF");

        bResult = TRUE;

        break;

    case '?':
        bResult = MG82_WDG_GetCfg(&u8WDT_En, &u8WDT_TimeOut);

        if (bResult == FALSE)
        {
            u8WDT_En = TRUE;
            u8WDT_TimeOut = 10;
        }

        sprintf(szCmdMsg, "%s%s,%d\r\n",
                STR_AS_8051_WDT_EN,
                u8WDT_En ? (char *)"ON" : (char *)"OFF",
                u8WDT_TimeOut);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
        u8ParamCnt++;

        if (strcmp(pStr, (char *)"ON") == 0)
        {
            MG82_WDG_Config(1, 10);

            RTOSprintf("\r\n> 8051 WDT ON");

            bResult = TRUE;
        }
        else if (strcmp(pStr, (char *)"OFF") == 0)
        {
            MG82_WDG_Config(0, 10);

            RTOSprintf("\r\n> 8051 WDT OFF");

            bResult = TRUE;
        }
        else
        {
            RTOSprintf("\r\n> Invalid parameter!");
        }

        break;
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to setting 8051 WDT time.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_8051_WDT_TIME(char *pStr)
{
    bool bResult = FALSE;
    uint8_t u8WDT_En, u8WDT_TimeOut;

    if (strncmp(pStr, STR_AS_8051_WDT_TIME, strlen(STR_AS_8051_WDT_TIME)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<time>", STR_AS_8051_WDT_TIME);
        RTOSprintf("\r\n@ <time>: WDT timeout in seconds, range 5-255");

        bResult = TRUE;

        break;

    case '?':
        bResult = MG82_WDG_GetCfg(&u8WDT_En, &u8WDT_TimeOut);

        if (bResult == FALSE)
        {
            u8WDT_En = TRUE;
            u8WDT_TimeOut = 10;
        }

        sprintf(szCmdMsg, "%s%d\r\n",
                STR_AS_8051_WDT_TIME,
                u8WDT_TimeOut);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:
    {
        uint32_t u32Timeout;

        u32Timeout = atoi(pStr);

        if ((u32Timeout < 5) || (u32Timeout > 255))
        {
            RTOSprintf("\r\n> Invalid timeout!");

            break;
        }

        bResult = MG82_WDG_GetCfg(&u8WDT_En, &u8WDT_TimeOut);

        if (bResult == FALSE)
        {
            u8WDT_En = TRUE;
        }

        bResult = MG82_WDG_Config((bool)u8WDT_En, (uint16_t)u32Timeout);

        if (bResult)
        {
            sprintf(szCmdMsg, "@ %s%s Set OK", STR_AS_8051_WDT_TIME, pStr);

            RTOSprintf("\r\n> %s", szCmdMsg);
        }

        bResult = TRUE;

        break;
    }
    }

    return bResult;
}

//============================================================================//
// @brief  Process the command related to setting 8051 USB protection
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_8051_USBPROTECT_SET(char *pStr)
{
    bool bResult = FALSE;
    //	uint8_t u8WDT_En, u8WDT_TimeOut;

    if (strncmp(pStr, STR_AS_8051_USBPROTECT_SET, strlen(STR_AS_8051_USBPROTECT_SET)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<mode>", STR_AS_8051_USBPROTECT_SET);
        RTOSprintf("\r\n@ <mode>: ON");

        bResult = TRUE;

        break;

    case '?':
        RTOSprintf("\r\n@ %sON", STR_AS_8051_USBPROTECT_SET);

        bResult = TRUE;

        break;

    default:
    {
        if (strcmp(pStr, (char *)"ON") == 0)
        {
            RTOSprintf("\r\n> ### 8051 Set USB Protection ###");
            MG82_USBPROTECTION_Set(1);

            bResult = TRUE;
        }
        else if (strcmp(pStr, (char *)"OFF") == 0)
        {
            RTOSprintf("\r\n> ### 8051 Disable USB Protection ###");
            MG82_USBPROTECTION_Set(0);
            bResult = TRUE;
        }

        break;
    }
    }

    return bResult;
}
//============================================================================//
// @brief  Process the command related to simulate MCU crashed.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_MCU_CRASH(char *pStr)
{
    bool bResult = FALSE;
    //	uint8_t u8Time = 0;
    if (strncmp(pStr, STR_AS_MCU_CRASH, strlen(STR_AS_MCU_CRASH)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);
    switch (*pStr)
    {
    case '\0':

        RTOSprintf("\r\n@ %s<mode>", STR_AS_MCU_CRASH);
        RTOSprintf("\r\n@ <mode>: ON");

        bResult = TRUE;

        break;
    case '?':
        RTOSprintf("\r\n@ %sON", STR_AS_MCU_CRASH);

        bResult = TRUE;
        break;
    default:
        if (strcmp(pStr, (char *)"ON") == 0)
        {
            RTOSprintf("\r\n> ### MCU CRASHED NOW ###");
            vTaskEndScheduler();
            // while (u8Time < 100)
            // {

            // 	u8Time++;
            // 	if (u8Time % 10)
            // 		RTOSprintf("\r\n> MCU CRASH MODE %d time: ", u8Time);
            // }
            bResult = TRUE;
        }
        break;
    }
    return bResult;
}
#endif
#if ENABLE_LSM6DSL
//============================================================================//
// @brief  Process the command related to CMD_GSENSOR_WKUP_THRE out.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSENSOR_WKUP_SETTING(char *pStr)
{
    bool bResult = FALSE;
    float fScale = 0.0;
    float fThreSold = 0.0;
    uint8_t u8Data = 0;
    if (strncmp(pStr, STR_AS_LSM6DSL_GSENSOR_WKUP_SETTING, strlen(STR_AS_LSM6DSL_GSENSOR_WKUP_SETTING)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<Scale>,<Thresold>", STR_AS_LSM6DSL_GSENSOR_WKUP_SETTING);
        RTOSprintf("\r\n@ \t<Scale>: Scale of Accelation +-2g, +-4g, +-8g, +-16g");
        RTOSprintf("\r\n@ \t<Thresold>: Thresold of Accelation, the range from 0 -> Scale. Unit g");
        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%.4f,%.4f", STR_AS_LSM6DSL_GSENSOR_WKUP_SETTING,
                g_Setting.tGS_Setting.fScale,
                g_Setting.tGS_Setting.fThreSold);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:

        StopChar(pStr, ',');

        fScale = fabs(strtof(pStr, NULL));

        if ((fScale > 16.0f) || (fmod(fScale, 2.0f) != 0))
        {
            RTOSprintf("\r\n> Invalid GS setting mode!");
            break;
        }

        pStr += strlen(pStr) + 1;

        fThreSold = fabs(strtof(pStr, NULL));

        if (fThreSold > fScale)
        {
            RTOSprintf("\r\n> Invalid GS setting mode!");
            break;
        }

        g_Setting.tGS_Setting.fScale = fScale;
        g_Setting.tGS_Setting.fThreSold = fThreSold;

        /* Full scale selection. */
        if (LSM6DSL_SetFullScaleXL(g_Setting.tGS_Setting.fScale) == FALSE)
        {
            RTOSprintf("\r\n> Set up Scale Failed");
            break;
        }

        u8Data = (uint8_t)((g_Setting.tGS_Setting.fThreSold * 64) / (g_Setting.tGS_Setting.fScale));
        /*Set wake up threshold*/
        if (LSM6DSL_ACC_GYRO_WK_THS(u8Data) == FALSE)
        {
            RTOSprintf("\r\n> Set up Thresold Failed");
            break;
        }
        bResult = TRUE;
        break;
    }
    return bResult;
}

//============================================================================//
// @brief  Process the command related to CMD_GSENSOR_AWTILT_SETTING out. It can be driven to INT2 pin
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSENSOR_AWTILT_SETTING(char *pStr)
{
    bool bResult = FALSE;
    float fThreSoldAngle = 0.0f;
    uint8_t u8Data = 0;

    if (strncmp(pStr, STR_AS_LSM6DSL_GSENSOR_AWTILT_SETTING, strlen(STR_AS_LSM6DSL_GSENSOR_AWTILT_SETTING)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
    {
    case '\0':
        RTOSprintf("\r\n@ %s<Thresold Angle>,<Axis>", STR_AS_LSM6DSL_GSENSOR_AWTILT_SETTING);
        RTOSprintf("\r\n@ \t<Thresold Angle>: Threasold angle with range from 0 -> 90 degree");
        RTOSprintf("\r\n@ \t<Axis>: P->P-axis, R->R-axis, Y->Y-axis, PR->PR-axis, PY->PY-axis, RY->RY-axis, PRY-> PRY axis");
        bResult = TRUE;

        break;

    case '?':
        sprintf(szCmdMsg, "@ %s%d,%s", STR_AS_LSM6DSL_GSENSOR_AWTILT_SETTING,
                g_Setting.tGS_Setting.u8ThresoldAngle,
                g_Setting.tGS_Setting.cAxis_Str);

        RTOSprintf("\r\n%s", szCmdMsg);

        bResult = TRUE;

        break;

    default:

        StopChar(pStr, ',');

        g_Setting.tGS_Setting.u8ThresoldAngle = atoi(pStr);

        if ((g_Setting.tGS_Setting.u8ThresoldAngle > 90))
        {
            RTOSprintf("\r\n> Invalid GS setting mode!");
            break;
        }

        pStr += strlen(pStr) + 1;

        if (strncmp(pStr, "P", strlen(pStr)) == 0)
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "P", strlen("P"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg);
        }
        else if (strncmp(pStr, "R", strlen(pStr)) == 0)
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "R", strlen("R"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg);
        }
        else if (strncmp(pStr, "Y", strlen(pStr)) == 0)
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "Y", strlen("Y"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg);
        }
        else if ((strncmp(pStr, "PR", strlen(pStr)) == 0) || (strncmp(pStr, "RP", strlen(pStr)) == 0))
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "PR", strlen("PR"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg);
        }
        else if ((strncmp(pStr, "PY", strlen(pStr)) == 0) || (strncmp(pStr, "YP", strlen(pStr)) == 0))
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "PY", strlen("PY"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg);
        }
        else if ((strncmp(pStr, "RY", strlen(pStr)) == 0) || (strncmp(pStr, "YR", strlen(pStr)) == 0))
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "RY", strlen("RY"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg);
        }
        else if (strncmp(pStr, "PRY", strlen(pStr)) == 0)
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            memcpy(g_Setting.tGS_Setting.cAxis_Str, "PRY", strlen("PRY"));
            g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg);
        }
        else
        {
            memset(g_Setting.tGS_Setting.cAxis_Str, '\0', sizeof(g_Setting.tGS_Setting.cAxis_Str));
            RTOSprintf("\r\n> Invalid GS setting mode!");
            return FALSE;
        }

        fThreSoldAngle = (float)(g_Setting.tGS_Setting.u8ThresoldAngle * 3.14) / 180;

        u8Data = (uint8_t)(sin(fThreSoldAngle) * 64);
        LSM6DSL_EnableAWTiltDetection();
        RTOSprintf("\r\n> GS Set up Thresold data: %d, Mask Data: %d", u8Data, g_Setting.tGS_Setting.u8Axis);
        bResult = TRUE;
        break;
    }
    return bResult;
}

//============================================================================//
// @brief  Process the command related to CMD_GSENSOR_AWTILT_SETTING out. It can be driven to INT2 pin
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSENSOR_DISABLE(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_LSM6DSL_GSENSOR_DISABLE, strlen(STR_AS_LSM6DSL_GSENSOR_DISABLE)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);
    switch (*pStr)
    {
    case '\0':

        RTOSprintf("\r\n@ %s<mode>", STR_AS_LSM6DSL_GSENSOR_DISABLE);
        RTOSprintf("\r\n@ <mode>: ON");

        bResult = TRUE;

        break;
    case '?':
        RTOSprintf("\r\n@ %sOFF", STR_AS_LSM6DSL_GSENSOR_DISABLE);

        bResult = TRUE;
        break;
    default:
        if (strcmp(pStr, (char *)"ON") == 0)
        {
            RTOSprintf("\r\n> ###G-Sensor disable NOW ###");
            bResult = TRUE;
            LSM6DSL_DeInit();
        }
        else if (strcmp(pStr, (char *)"OFF") == 0)
        {
            RTOSprintf("\r\n> ###G-Sensor enable NOW ###");
            bResult = TRUE;

            LSM6DSL_Init();
        }
        break;
    }
    return bResult;
}

//============================================================================//
// @brief  Process the command related to CMD_GSENSOR_SET_CURRENT_ORIENTATION out
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_GSENSOR_SET_CURRENT_ORIENTATION(char *pStr)
{
    bool bResult = FALSE;

    if (strncmp(pStr, STR_AS_LSM6DSL_GSENSOR_SET_CURRENT_ORIENTATION, strlen(STR_AS_LSM6DSL_GSENSOR_SET_CURRENT_ORIENTATION)) != 0)
        return FALSE;

    pStr += Protocl_CmdStopHead(pStr);
    switch (*pStr)
    {
    case '\0':

        RTOSprintf("\r\n@ %s<mode>", STR_AS_LSM6DSL_GSENSOR_SET_CURRENT_ORIENTATION);
        RTOSprintf("\r\n@ <mode>: ON");

        bResult = TRUE;

        break;
    case '?':
        RTOSprintf("\r\n@ %sOFF", STR_AS_LSM6DSL_GSENSOR_SET_CURRENT_ORIENTATION);

        bResult = TRUE;
        break;
    default:
        if (strcmp(pStr, (char *)"ON") == 0)
        {
            RTOSprintf("\r\n> ###G-Sensor set current orientation NOW ###");
            bSetCurrOrien = TRUE;
            bResult = TRUE;
        }
        // else if (strcmp(pStr, (char *)"OFF") == 0)
        // {
        //     RTOSprintf("\r\n> ###G-Sensor enable NOW ###");
        //     bResult = TRUE;
        // }
        break;
    }
    return bResult;
}
#endif
// Processing Functions //

//============================================================================//
// @brief  Check if a command will reset GSM module.
// @param  pcStr: Pointer to a string which stores the command string.
// @retval TRUE: The command will reset GSM module.
//         FALSE: The command will not reset the GSM module.
//============================================================================//
bool Protocl_IsRstGSM_Cmd(char *pcCmdStr, char *pcCmdArg)
{
    bool bRes = FALSE;

    if ((strlen(pcCmdArg) == 0) || (*pcCmdArg == '?'))
        return FALSE;

    // #if ENABLE_GSM_NET
    // if((strncmp(pcCmdStr, STR_AS_GSM_MULTI_INFO, strlen(STR_AS_GSM_MULTI_INFO)) == 0) &&
    //	 (strlen(pcCmdArg) == 1))
    //	return FALSE;
    // #endif

    if ((strncmp(pcCmdStr, STR_AS_FORMAT, strlen(STR_AS_FORMAT)) == 0) ||
#if ENABLE_GSM_NET
    //(strncmp(pcCmdStr, STR_AS_GSM_APN, strlen(STR_AS_GSM_APN)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_SERVER_IP, strlen(STR_AS_GSM_SERVER_IP)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_SERVER_NAME, strlen(STR_AS_GSM_SERVER_NAME)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_HEADER, strlen(STR_AS_GSM_HEADER)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_PKG_EN, strlen(STR_AS_GSM_PKG_EN)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_SEND_NORMAL, strlen(STR_AS_GSM_SEND_NORMAL)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_SERVER_INFO, strlen(STR_AS_GSM_SERVER_INFO)) == 0) ||
#if ENABLE_MULTI_SERVER
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_IP, strlen(STR_AS_GSM_MULTI_IP)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_DNS, strlen(STR_AS_GSM_MULTI_DNS)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_HEADER, strlen(STR_AS_GSM_MULTI_HEADER)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_EN, strlen(STR_AS_GSM_MULTI_EN)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_SEND, strlen(STR_AS_GSM_MULTI_SEND)) == 0) ||
//(strncmp(pcCmdStr, STR_AS_GSM_MULTI_INFO, strlen(STR_AS_GSM_MULTI_INFO)) == 0) ||
#endif
//(strncmp(pcCmdStr, STR_AS_GSM_FTP, strlen(STR_AS_GSM_FTP)) == 0) ||
#endif
        (strncmp(pcCmdStr, STR_AS_FIL_RENAME, strlen(STR_AS_FIL_RENAME)) == 0) ||
        (strncmp(pcCmdStr, STR_AS_FIL_DEL, strlen(STR_AS_FIL_DEL)) == 0) ||
        (strncmp(pcCmdStr, STR_AS_DIR_DEL, strlen(STR_AS_DIR_DEL)) == 0) ||
        (strncmp(pcCmdStr, STR_AS_VDR_DEL, strlen(STR_AS_VDR_DEL)) == 0) ||
        (strncmp(pcCmdStr, STR_AS_DTR_DEL, strlen(STR_AS_DTR_DEL)) == 0))
    {
        bRes = TRUE;
    }
    else if (strncmp(pcCmdStr, STR_AS_RESET, strlen(STR_AS_RESET)) == 0)
    {
        if ((strncmp(pcCmdArg, szResetCmd[RST_LOG], strlen(szResetCmd[RST_LOG])) != 0) &&
#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
            (strncmp(pcCmdArg, szResetCmd[RST_GPS], strlen(szResetCmd[RST_GPS])) != 0) &&
#endif
            (strncmp(pcCmdArg, szResetCmd[RST_CAN], strlen(szResetCmd[RST_CAN])) != 0))
        {
            bRes = TRUE;
        }
    }
    else if (strncmp(pcCmdStr, STR_AS_PARAMETER, strlen(STR_AS_PARAMETER)) == 0)
    {
        if (*pcCmdArg == '5')
        {
            bRes = TRUE;
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Process an input command.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool Protocl_ProcCmd(char *pStr)
{
    StopChar(pStr, '\n');

    StopChar(pStr, '\r');

    memset(szCmdMsg, 0x00, sizeof(szCmdMsg));

    if (strncmp(pStr, CMD_HEAD_STD, strlen(CMD_HEAD_STD)) == 0)
    {
        // Not need admin
        if (CMD_Version(pStr) == TRUE)
            return TRUE;

        if (CMD_DebugMode(pStr) == TRUE)
            return TRUE;

        if (CMD_RecordMode(pStr) == TRUE)
            return TRUE;

        if (CMD_DriverIdNum(pStr) == TRUE)
            return TRUE;

        if (CMD_RTC_TIME(pStr) == TRUE)
            return TRUE;

        // if(CMD_ENG_MODE(pStr) == TRUE)
        //	return TRUE;

        // if(CMD_CORRECT_MODE(pStr) == TRUE)
        //	return TRUE;

        /*
        if(g_Setting.ENABLE_SET.bits.ENG_MODE == 0)
        {
            RTOSprintf("\r\nSorry, This command not authorization.");

            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_NON_AUTH);

            return TRUE;
        }
        */

        // Need admin
        if (CMD_TimeZone(pStr) == TRUE)
            return TRUE;

        if (CMD_BusId(pStr) == TRUE)
            return TRUE;

        if (CMD_BusSn(pStr) == TRUE)
            return TRUE;

        if (CMD_Distance(pStr) == TRUE)
            return TRUE;

        if (CMD_SpeedGain(pStr) == TRUE)
            return TRUE;

        // if(CMD_SpeedTrig(pStr) == TRUE)
        //	return TRUE;

        if (CMD_SpeedLim(pStr) == TRUE)
            return TRUE;

        if (CMD_SpeedAdj(pStr) == TRUE)
            return TRUE;

        // if(CMD_RpmTrig(pStr) == TRUE)
        //	return TRUE;

        if (CMD_RpmDiv(pStr) == TRUE)
            return TRUE;

        if (CMD_RpmLim(pStr) == TRUE)
            return TRUE;

        if (CMD_Over_Speed(pStr) == TRUE)
            return TRUE;

        if (CMD_RPM_Idle(pStr) == TRUE)
            return TRUE;

        if (CMD_AcceLim(pStr) == TRUE)
            return TRUE;

        if (CMD_AutoGain_Time(pStr) == TRUE)
            return TRUE;

        if (CMD_KGain(pStr) == TRUE)
            return TRUE;

        if (CMD_GAIN_ChkEn(pStr) == TRUE)
            return TRUE;

        if (CMD_PowerMode(pStr) == TRUE)
            return TRUE;

        if (CMD_PowerDown(pStr) == TRUE)
            return TRUE;

        if (CMD_System(pStr) == TRUE)
            return TRUE;

        // if(CMD_LastInstall(pStr) == TRUE)
        //	return TRUE;

        if (CMD_Reset(pStr) == TRUE)
            return TRUE;

        if (CMD_EX_ALM(pStr) == TRUE)
            return TRUE;

        if (CMD_EX_PED_WKUP(pStr) == TRUE)
            return TRUE;

        if (CMD_TEST_ALM(pStr) == TRUE)
            return TRUE;

        if (CMD_TEST_PED_WKUP(pStr) == TRUE)
            return TRUE;

        if (CMD_RTC_Calibration(pStr) == TRUE)
            return TRUE;

        if (CMD_DRIVE_TIME(pStr) == TRUE)
            return TRUE;

        if (CMD_REST_TIME(pStr) == TRUE)
            return TRUE;

        // if(CMD_Reset_Boot(pStr) == TRUE) // Heinz EUPFIN
        //	return TRUE;

        // if(CMD_Reset_Def(pStr) == TRUE)	// Heinz EUPFIN
        //	return TRUE;

        if (CMD_TimeAdj(pStr) == TRUE)
            return TRUE;

        if (CMD_ExinReverse(pStr) == TRUE)
            return TRUE;

        // if(CMD_Immobilizer(pStr) == TRUE)
        //	return TRUE;

        if (CMD_VIN(pStr) == TRUE)
            return TRUE;

        if (CMD_DriverId(pStr) == TRUE)
            return TRUE;

        // Auto Gain Command
        if (CMD_SpeedOffset(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_SpeedStar(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_AutoGain(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_Car_Type(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_Multi_Set(pStr) == TRUE)
            return TRUE;

        if (CMD_DB9_Func(pStr) == TRUE)
            return TRUE;

        if (CMD_Merge_Speed(pStr) == TRUE)
            return TRUE;

        // Auto Gain Fix 512m
        // if(CMD_AG_DistLimit(pStr) == TRUE)
        //	return TRUE;

        if (CMD_Save_Setting(pStr) == TRUE)
            return TRUE;

        if (CMD_Set_RDP(pStr) == TRUE)
            return TRUE;

        if (CMD_Clr_RDP(pStr) == TRUE)
            return TRUE;

        if (CMD_Set_WRP(pStr) == TRUE)
            return TRUE;

        if (CMD_Clr_WRP(pStr) == TRUE)
            return TRUE;

        if (CMD_Delete_Setting(pStr) == TRUE)
            return TRUE;

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
        if (CMD_UploadSettingFTP(pStr) == TRUE)
            return TRUE;

        if (CMD_DownloadSettingFTP(pStr) == TRUE)
            return TRUE;
#endif

#if ENABLE_CAN
        if (CMD_CAN_Link(pStr) == TRUE)
            return TRUE;

        if (CMD_CAN_Distance(pStr) == TRUE)
            return TRUE;

        if (CMD_CAN_Speed(pStr) == TRUE)
            return TRUE;

        if (CMD_CAN_RPM(pStr) == TRUE)
            return TRUE;

        if (CMD_CAN_Other(pStr) == TRUE)
            return TRUE;

        if (CMD_CAN_RelinkAll(pStr) == TRUE)
            return TRUE;

#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_MBEYE
        if (CMD_MBEYE_LDW_Cnt(pStr) == TRUE)
            return TRUE;

        if (CMD_MBEYE_LDW_Time(pStr) == TRUE)
            return TRUE;

        if (CMD_MBEYE_HW_Time(pStr) == TRUE)
            return TRUE;
#endif
#endif

        if (CMD_AlarmBeep(pStr) == TRUE)
            return TRUE;

// Just JAS208U
#if ENABLE_GSM_NET
        if (CMD_GsmCsq(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_SimPin(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_StatusChk(pStr) == TRUE) // Heinz
            return TRUE;

        if (CMD_GsmMode(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmApn(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmFtp(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmFtpExt(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmServerIp(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmServerName(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmPkgHeader(pStr) == TRUE)
            return TRUE;

        // if(CMD_GsmLogEnable(pStr) == TRUE)
        //	return TRUE;

        /*
        if(CMD_GsmPkgEnable(pStr) == TRUE)
            return TRUE;

        if(CMD_GsmSendNormal(pStr) == TRUE)
            return TRUE;

        if(CMD_GsmSendStandby(pStr) == TRUE)
            return TRUE;

        if(CMD_GsmServerInfo(pStr) == TRUE)
            return TRUE;
        */

        if (CMD_GsmImei(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmImsi(pStr) == TRUE)
            return TRUE;

#if ENABLE_MULTI_SERVER
        if (CMD_GsmMultiIp(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmMultiDns(pStr) == TRUE)
            return TRUE;

        if (CMD_GsmMultiHeader(pStr) == TRUE)
            return TRUE;

        if (CMD_GSM_SocketEXT(pStr) == TRUE)
            return TRUE;

/*
if(CMD_GsmMultiPkgEnable(pStr) == TRUE)
    return TRUE;

if(CMD_GsmMultiSend(pStr) == TRUE)
    return TRUE;

if(CMD_GsmMultiServerInfo(pStr) == TRUE)
    return TRUE;
*/

// if(CMD_GsmLogExtEnable(pStr) == TRUE)
//	return TRUE;
#endif

        if (CMD_GSM_CALL(pStr) == TRUE)
            return TRUE;

        if (CMD_FTP_UPLOAD(pStr) == TRUE)
            return TRUE;

        if (CMD_FTP_UPLOAD_EXT(pStr) == TRUE)
            return TRUE;

        if (CMD_FTP_DOWNLOAD(pStr) == TRUE)
            return TRUE;

        if (CMD_FTP_DOWNLOAD_EXT(pStr) == TRUE)
            return TRUE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
        if (CMD_EPO_CHECK(pStr) == TRUE)
            return TRUE;

        if (CMD_EPO_DOWNLOAD(pStr) == TRUE)
            return TRUE;

        if (CMD_EPO_UPDATE(pStr) == TRUE)
            return TRUE;
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
        if (CMD_XTRA_DOWNLOAD(pStr) == TRUE)
            return TRUE;

        if (CMD_XTRA_UPDATE(pStr) == TRUE)
            return TRUE;
#endif

        if (CMD_SEND_EVENT(pStr) == TRUE)
            return TRUE;

        if (CMD_AT_CMD(pStr) == TRUE)
            return TRUE;

        if (CMD_SEND_SMS(pStr) == TRUE)
            return TRUE;

        if (CMD_GSM_FTP_VDR_SUBDIR(pStr) == TRUE)
            return TRUE;

        if (CMD_SMS_LIST(pStr) == TRUE)
            return TRUE;

        if (CMD_GSM_IOT_SIM(pStr) == TRUE)
            return TRUE;

        if (CMD_GetDatalog(pStr) == TRUE)
            return TRUE;

        if (CMD_GetEvent(pStr) == TRUE)
            return TRUE;

#if ENABLE_MULTI_SERVER
        if (CMD_GetDatalogExt(pStr) == TRUE)
            return TRUE;

        if (CMD_GetEventExt(pStr) == TRUE)
            return TRUE;
#endif

        // if(CMD_Sos(pStr) == TRUE) // Heinz
        //	return TRUE;

        if (CMD_BusId_Set(pStr) == TRUE)
            return TRUE;
#endif

        // VDR Now
        if (CMD_VdrNow_Set(pStr) == TRUE)
            return TRUE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
        if (CMD_XTRA_URL(pStr) == TRUE)
            return TRUE;
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
        if (CMD_EPO_URL(pStr) == TRUE)
            return TRUE;
#endif

#if ENABLE_MG82
        if (CMD_KEY_RST(pStr) == TRUE)
            return TRUE;
#endif

        // For gary test Rule V16.1
        if (CMD_DRIVE_LIST(pStr) == TRUE)
            return TRUE;

        if (CMD_SET_PARAMETER(pStr) == TRUE)
            return TRUE;

        if (CMD_DRIVE_STATUS(pStr) == TRUE)
            return TRUE;

        if (CMD_TEST_RS232(pStr) == TRUE)
            return TRUE;

        if (CMD_TEST_BackLight(pStr) == TRUE)
            return TRUE;

        if (CMD_LCD_Saver(pStr) == TRUE)
            return TRUE;

        if (CMD_TEST_Mode(pStr) == TRUE)
            return TRUE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
        if (CMD_GPS_CMD(pStr) == TRUE)
            return TRUE;
#endif

        if (CMD_Check_Header(pStr) == TRUE)
            return TRUE;

        if (CMD_AutoGain_Parameter(pStr) == TRUE)
            return TRUE;

        if (CMD_DF2_INFO(pStr) == TRUE)
            return TRUE;

        if (CMD_RINGBUF_RESET(pStr) == TRUE)
            return TRUE;

        if (CMD_UART_INT(pStr) == TRUE)
            return TRUE;

        if (CMD_PRINTF_EN(pStr) == TRUE)
            return TRUE;

        if (CMD_FILE_GET_LIST(pStr) == TRUE)
            return TRUE;

        if (CMD_FILE_DOWNLOAD(pStr) == TRUE)
            return TRUE;

        if (CMD_FILE_RENAME(pStr) == TRUE)
            return TRUE;

        if (CMD_FILE_SIZE(pStr) == TRUE)
            return TRUE;

        if (CMD_FIND_FILE(pStr) == TRUE)
            return TRUE;

        if (CMD_FILE_REMOVE(pStr) == TRUE)
            return TRUE;

        if (CMD_DIR_REMOVE(pStr) == TRUE)
            return TRUE;

        if (CMD_CARD_FORMAT(pStr) == TRUE)
            return TRUE;

        if (CMD_VDR_DEL(pStr) == TRUE)
            return TRUE;

        if (CMD_DTR_DEL(pStr) == TRUE)
            return TRUE;

        if (CMD_VDR_MAKING(pStr) == TRUE)
            return TRUE;

        if (CMD_TASK_RAM(pStr) == TRUE)
            return TRUE;

        if (CMD_TASK_STEP(pStr) == TRUE)
            return TRUE;

        if (CMD_TASK_DELAY(pStr) == TRUE)
            return TRUE;

        // if(CMD_USB_READ(pStr) == TRUE)
        //   return TRUE;

        if (CMD_LCD_SHOW(pStr) == TRUE)
            return TRUE;

#if ENABLE_GSM_NET
        if (CMD_4G_PWR(pStr) == TRUE)
            return TRUE;

        if (CMD_4G_RESET(pStr) == TRUE)
            return TRUE;
#endif
        if (CMD_PRINT(pStr) == TRUE)
            return TRUE;

        if (CMD_MCU_SLEEP(pStr) == TRUE)
            return TRUE;

        // #if ENABLE_MG82
        if (CMD_805T_WDT_EN(pStr) == TRUE)
            return TRUE;

        if (CMD_8051_WDT_TIME(pStr) == TRUE)
            return TRUE;

        if (CMD_8051_USBPROTECT_SET(pStr) == TRUE)
        {
            return TRUE;
        }
        if (CMD_MCU_CRASH(pStr) == TRUE)
            return TRUE;
        // #endif

        if (CMD_AllCmd(pStr) == TRUE)
            return TRUE;
#if ENABLE_LSM6DSL
        if (CMD_GSENSOR_WKUP_SETTING(pStr) == TRUE)
            return TRUE;
        if (CMD_GSENSOR_AWTILT_SETTING(pStr) == TRUE)
            return TRUE;
        if (CMD_GSENSOR_DISABLE(pStr) == TRUE)
            return TRUE;
        if (CMD_GSENSOR_SET_CURRENT_ORIENTATION(pStr) == TRUE)
            return TRUE;
#endif
    }

    return FALSE;
}

//============================================================================//
// @brief  Process the command related to command testing.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool CMD_AllCmd(char *pStr)
{
    char BufStr[32];

    if (strncmp(pStr, STR_AS_ALL_CMD, strlen(STR_AS_ALL_CMD)) != 0)
        return FALSE;

    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_VERSION);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_TIME_ZONE);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_BUS_ID);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_BUS_SN);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_VIN);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_DISTANCE);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_SPEED_GAIN);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_SPEED_LIMIT);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_RPM_DIV);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_RPM_LIMIT);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    /*
    sprintf(BufStr, "%s?", STR_AS_GSM_APN);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_GSM_SERVER_IP);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_GSM_SEND_NORMAL);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_GSM_SEND_STANDBY);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");
    */

    /*
    sprintf(BufStr, "%s?", STR_AS_CAN_DIST);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_CAN_SPEED);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_CAN_RPM);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");
    */

    sprintf(BufStr, "%s?", STR_AS_DEBUG_MODE);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_POWER_MODE);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s?", STR_AS_EXIN_REVERSE);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    /*
    // Heinz 12 // 092016
    sprintf(BufStr, "%s", STR_AS_DRIVER_RESPONSE); // Heinz
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s", STR_AS_AIR_INTERVAL_FRONT); // Heinz
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s", STR_AS_AIR_INTERVAL_BACK); // Heinz
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");

    sprintf(BufStr, "%s", STR_AS_SPEED_OFFSET);
    Protocl_ProcCmd(BufStr);
    RTOSprintf("\n");
    */

    Protocl_CmdStopHead(pStr);

    return TRUE;
}

//============================================================================//
// @brief  Get a command from the RS232 buffer.
// @param  None.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
static bool Protocl_GetCmdPkg(void)
{
    uint16_t i, u16BufLen, u16CmdPkgIdx = 0;
    uint8_t Buf[UART_BASE_BUFFER_SIZE];
    char *pBuf, ch;

    memset(Buf, 0x00, sizeof(Buf));

    if (UART_IsTerminalUsingSET())
        u16BufLen = UART_GetRecvData(SET_COMPORT_DEF, Buf, sizeof(Buf));
    else
        u16BufLen = UART_GetRecvData(DB9_COMPORT_DEF, Buf, sizeof(Buf));

    pBuf = (char *)Buf;

    for (i = 0; i < u16BufLen; i++)
    {
        ch = *pBuf++;

        SettingCmdPkg[u16CmdPkgIdx++] = ch;

        if (ch == '\n')
        {
            SettingCmdPkg[u16CmdPkgIdx] = '\0';

            u16CmdPkgIdx = 0;

            return TRUE;
        }

        if (u16CmdPkgIdx >= UART_BASE_BUFFER_SIZE)
        {
            u16CmdPkgIdx = 0;

            continue;
        }
    }

    return FALSE;
}

//============================================================================//
// @brief  Process the command related to command testing.
// @param  pcCmdPkg: Pointer to a string storing a RS232 command.
//         pcCmdMsg: Pointer to a string buffer to store the returned message.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg)
{
    bool bResult;

    bResult = Protocl_ProcCmd(pcCmdPkg);

    if (pcCmdMsg != NULL)
    {
        strcpy(pcCmdMsg, szCmdMsg);
    }

    if (bResult == TRUE)
        RTOSprintf(STR_CMD_OK);
    else
        RTOSprintf(STR_CMD_ERR);

    return bResult;
}

//============================================================================//
// @brief  Process RS232 command.
// @param  None.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool Protocl_CommandCheck_Proc(void)
{
    if (Protocl_GetCmdPkg() == TRUE)
    {
        Protocl_ChkCmdPkg(SettingCmdPkg, NULL);

        memset(SettingCmdPkg, 0x00, sizeof(SettingCmdPkg));
    }

    return TRUE;
}
