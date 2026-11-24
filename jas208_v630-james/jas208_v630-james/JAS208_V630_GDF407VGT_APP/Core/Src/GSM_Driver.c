//============================================================================//
// Includes
//============================================================================//
#include "GSM_Driver.h"

#include "ff.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "cmsis_os.h"
#include "Config.h"
#include "DataLog.h"
#include "Default.h"
#include "EXTIO.h"
#include "LCD_Key.h"
#include "main.h"
#include "MG82_Driver.h"
#include "MX25L_Driver.h"
#if ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "Protocol.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"
#if ENABLE_GSM_QTEC21
#include "GSM_QtEC21.h"
#endif
#include "task.h"
#include "USB_Module.h"
#include "Version.h"

//============================================================================//
// User Variables
//============================================================================//
#if ENABLE_GSM_NET
const char *const cGSM_ModelString[] =
    {
        "EC21",
        "EC25",
        "EC200"};

static const MsgCmdTypeDef MSG_CMD[] =
    {
        // Cmd No.: 0
        {NULL, NULL},

        // Cmd No.: 1
        {"VER", STR_AS_VERSION},

        // Cmd No.: 2
        {"VID", STR_AS_BUS_ID},

        // Cmd No.: 3
        {"CAR", STR_AS_CAR_TYPE},

        // Cmd No.: 4
        {"SPEED GAIN", STR_AS_SPEED_GAIN},

        // Cmd No.: 5
        {"SPEED ADJ", STR_AS_SPEED_ADJ},

        // Cmd No.: 6
        {"KGAIN", STR_AS_KGAIN},

        // Cmd No.: 7
        {"SPEED LIM", STR_AS_SPEED_LIMIT},

        // Cmd No.: 8
        {"DISTANCE", STR_AS_DISTANCE},

        // Cmd No.: 9
        {"IP", STR_AS_GSM_SERVER_IP},

        // Cmd No.: 10
        {"APN", STR_AS_GSM_APN},

        // Cmd No.: 11
        {"RTC CAL", STR_AS_RTC_CALIBRATION},

        // Cmd No.: 12
        {"RTC TIME", STR_AS_RTC_TIME},

        // Cmd No.: 13
        {"EXIN", STR_AS_EXIN_REVERSE},

        // Cmd No.: 14
        {"VIN", STR_AS_VIN},

        // Cmd No.: 15 // Reserved
        //{"IMM",		      STR_AS_IMMOBILIZER},
        {NULL, NULL},

        // Cmd No.: 16
        {"SYS", STR_AS_SYSTEM},

        // Cmd No.: 17
        {"TIME ADJ", STR_AS_TIME_ADJ},

        // Cmd No.: 18
        {"PWR", STR_AS_POWER_MODE},

        // Cmd No.: 19
        {"MULTI IP", STR_AS_GSM_MULTI_IP},

        // Cmd No.: 20
        {"RELINK ALL", STR_AS_CAN_RELINK_ALL},

        // Cmd No.: 21
        {"RECORD", STR_AS_RECORD_MODE},

        // Cmd No.: 22
        {"DF2 INFO", STR_AS_DF2_INFO},

        // Cmd No.: 23 // Reserved
        {"FORMAT", STR_AS_FORMAT},

        // Cmd No.: 24
        {"IMEI", STR_AS_GSM_IMEI},

        // Cmd No.: 25
        {"IMSI", STR_AS_GSM_IMSI},

        // Cmd No.: 26
        {"OVER SPEED", STR_AS_OVER_SPEED},

        // Cmd No.: 27
        {"ZONE", STR_AS_TIME_ZONE},

        // Cmd No.: 28
        {"ACCEL LIM", STR_AS_ACCEL_LIMIT},

        // Cmd No.: 29
        {"RESET", STR_AS_RESET},

        // Cmd No.: 30
        {"SN", STR_AS_BUS_SN},

        // Cmd No.: 31
        {"RPM DIV", STR_AS_RPM_DIV},

        // Cmd No.: 32
        {"RPM LIM", STR_AS_RPM_LIMIT},

        // Cmd No.: 33 // Reserved
        //{"RPM TRIG",    STR_AS_RPM_TRIG},
        {NULL, NULL},

        // Cmd No.: 34
        {"DRV ID", STR_AS_DRIVER_ID},

        // Cmd No.: 35
        {"DRV IN", STR_AS_DRIVER_IN},

        // Cmd No.: 36
        {"RPM IDLE", STR_AS_RPM_IDLE},

        // Cmd No.: 37
        {"CORRECT", STR_AS_CORRECT_MODE},

        // Cmd No.: 38
        {"ALM", STR_AS_ALARM_BEEP},

        // Cmd No.: 39
        {"DRV TIME", STR_AS_DRIVE_TIME_THRESHOLD},

        // Cmd No.: 40
        {"REST TIME", STR_AS_REST_TIME_THRESHOLD},

        // Cmd No.: 41
        {"CSQ", STR_AS_GSM_CSQ},

        // Cmd No.: 42
        {"CHK", STR_AS_STATUS_CHECK},

        // Cmd No.: 43
        {"GSM MODE", STR_AS_GSM_MODE},

        // Cmd No.: 44
        {"GET EVENT", STR_AS_GET_EVENT},

        // Cmd No.: 45
        {"LOG EXT", STR_AS_GET_DATALOG_EXT},

        // Cmd No.: 46 // Reserved
        //{"PIN",	        STR_AS_SIM_PIN},
        {NULL, NULL},

        // Cmd No.: 47
        {"CAN LINK", STR_AS_CAN_LINK},

        // Cmd No.: 48
        {"EVENT EXT", STR_AS_GET_EVENT_EXT},

        // Cmd No.: 49
        {"FTP", STR_AS_GSM_FTP},

        // Cmd No.: 50
        {"UPLOAD", STR_AS_FTP_UPLOAD},

        // Cmd No.: 51 // Reserved
        {"SOS", STR_AS_SOS},

        // Cmd No.: 52
        {"USB DWL", STR_AS_USB_DOWNLOAD},

        // Cmd No.: 53
        {"DNS", STR_AS_GSM_SERVER_NAME},

        // Cmd No.: 54
        {"MULTI DNS", STR_AS_GSM_MULTI_DNS},

        // Cmd No.: 55
        {"MULTI SET", STR_MULTI_SET},

        // Cmd No.: 56
        {"MERGE SPEED", STR_AS_MERGE_SPEED},

        // Cmd No.: 57
        {"RDP", STR_AS_RDP_MODE},

        // Cmd No.: 58
        {"IOT SIM", STR_AS_GSM_IOT_SIM},

        // Cmd No.: 59
        {"LCD SAVE", STR_AS_LCD_SAVER},

        // Cmd No.: 60
        {"SPEED OFT", STR_AS_SPEED_OFFSET},

        // Cmd No.: 61
        {"AG SPEED", STR_AS_AUTOGAIN_SPEED},

        // Cmd No.: 62
        {"AUTO GAIN", STR_AS_AUTO_GAIN},

        // Cmd No.: 63
        {"GET LOG", STR_AS_GET_DATALOG},

        // Cmd No.: 64
        {"UP SETTING", STR_AS_SETTING_FTP_UPLOAD},

        // Cmd No.: 65
        {"GET SETTING", STR_AS_SETTING_FTP_DOWNLOAD},

        // Cmd No.: 66
        {"FTP EXT", STR_AS_GSM_FTP_EXT},

        // Cmd No.: 67
        {"CAN DIST", STR_AS_CAN_DIST},

        // Cmd No.: 68
        {"CAN SPEED", STR_AS_CAN_SPEED},

        // Cmd No.: 69
        {"CAN RPM", STR_AS_CAN_RPM},

        // Cmd No.: 70
        {"CAN OTHER", STR_AS_CAN_OTHER},

        // Cmd No.: 71
        {"UPLOAD EXT", STR_AS_FTP_UPLOAD_EXT},

        // Cmd No.: 72
        {"DOWNLOAD", STR_AS_FTP_DOWNLOAD},

        // Cmd No.: 73
        {"DL EXT", STR_AS_FTP_DOWNLOAD_EXT},

        // Cmd No.: 74
        {"VDR SUB", STR_AS_FTP_VDR_SUBDIR},

        // Cmd No.: 75
        {"VDR NOW", STR_AS_VDR_NOW},

        // Cmd No.: 76
        {"DB9 FUNC", STR_AS_DB9_FUNCTION},

        // Cmd No.: 77
        {"AG PARA", STR_AS_AUTOGAIN_PARA},

        // Cmd No.: 78
        {"AG DIST", STR_AS_AG_DIST_LIMIT},

        // Cmd No.: 79
        {"XTRA URL", STR_AS_XTRA_URL},

        // Cmd No.: 80
        {"FIL DEL", STR_AS_FIL_DEL},

        // Cmd No.: 81
        {"FIL SIZE", STR_AS_FIL_SIZE},

        // Cmd No.: 82
        {"EPO URL", STR_AS_EPO_URL},

        // Cmd No.: 83
        {"GSM EXT" STR_AS_GSM_SOCKET_EXT},

        // Cmd No.: 84
        {"PWR DOWN", STR_AS_POWER_DOWN},

        // Cmd No.: 85
        {"EX ALM", STR_AS_EX_ALM},

        // Cmd No.: 86
        {"EX WKUP", STR_AS_EX_PED_WKUP},

        // Cmd No.: 87 // Reserved
        {"KEY RST", STR_AS_KEY_RST},

        // Cmd No.: 88
        {"GAINCHK", STR_AS_GAIN_CHK_EN},

        // Cmd No.: 89 // Reserved
        {NULL, NULL},

        // Cmd No.: 90 // Reserved
        {NULL, NULL},

}; // Maximum command ID number must match CMD_ID_MAX

/*
// Unused IP list
static const IP_InvalidTypeDef tInvalidIP[] =
{
    // Server JASS
  {SERVER_JASS_IP_ADDR,   SERVER_JASS_PORT}
};
*/

QtEC21_BlockCmdBuildTypeDef qtec21_BlockCmdInstance;

QtEC21_NonBlockCmdBuildTypeDef qtec21_NonBlockCmdInstance;

QTEC21_NonBlockCMD_TypeDef gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

static uint16_t u16CurrGSM_ByteNum = 0;

GSM_CmdSrcTypeDef gsmCmdSrc = GSM_CMD_SRC_NULL;

ProcStepTypeDef procGSM_SetupStep = PROC_STEP_NULL;

PDP_ActStepTypeDef procGSM_PDP_Step = PDP_STEP_NULL;

TCP_ConnectStepTypeDef procGSM_TCP_Step[MAX_SOCKET_BANK] = {CONN_STEP_NULL};

uint8_t u8PDP_ActRetryCnt = 0;

// static uint8_t u8TCP_SocketID[MAX_SOCKET_BANK] = {TCP_SOCKET_NUM_MAX};

#if ENABLE_MULTI_SERVER
const uint8_t u8SocketID_Min[MAX_SOCKET_BANK] = {TCP_SERVER_0_SOCKET_MIN, TCP_SERVER_1_SOCKET_MIN};
const uint8_t u8SocketID_Max[MAX_SOCKET_BANK] = {TCP_SERVER_0_SOCKET_MAX, TCP_SERVER_1_SOCKET_MAX};
#else
const uint8_t u8SocketID_Min[MAX_SOCKET_BANK] = {TCP_SERVER_0_SOCKET_MIN};
const uint8_t u8SocketID_Max[MAX_SOCKET_BANK] = {TCP_SERVER_0_SOCKET_MAX};
#endif

bool bChangeSocketFlag[MAX_SOCKET_BANK] = {FALSE};

static char cTCP_IP_Temp[20] = {0};

bool bPDP_DeactFlag = FALSE, bTCP_CloseFlag[MAX_SOCKET_BANK] = {0};

bool bFTP_ResetFlag = FALSE;

FOTA_StepTypeDef procGSM_FOTA_Step = FOTA_STEP_NULL;

#if ENABLE_FTP_DOWNLOAD
GSM_DownloadRequestTypeDef gsmDownloadRequest; // __attribute__ ((section("ccmram_var")));

uint16_t u16DownloadFileNum = 0;

Download_StepTypeDef procGSM_Download_Step = DOWNLOAD_STEP_NULL;
#endif

GSM_UploadRequestTypeDef gsmUploadRequest, gsmUploadRequestTemp; // __attribute__ ((section("ccmram_var")));

uint16_t u16UnsendFileNum = 0;

FTP_UploadStepTypeDef procGSM_Upload_Step = UPLOAD_STEP_NULL;

#if ENABLE_UPLOAD_CHK
bool bDoUploadChk = FALSE;

FIFO_IndexTypeDef tUploadChkHandle = {0};

GSM_UploadChkTypeDef gsmUploadChkData;

FTP_UploadChkStepTypeDef procGSM_UploadChk_Step = UPLOAD_CHK_STEP_NULL;
#endif

HTTP_GET_TypeDef tGET_Resp = {0};

HTTP_DownloadRequestTypeDef tHTTP_DlRequest = {0};

HTTP_DownloadStepTypeDef procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
// GPS_XTRA_TypeDef tGPS_Xtra = {0};

// XTRA_DownloadStepTypeDef procXTRA_Download_Step = XTRA_STEP_NULL;

XTRA_RequestTypeDef tXTRA_Update = {0};

XTRA_UpdateStepTypeDef procXTRA_Update_Step = XTRA_UPDATE_STEP_NULL;
#endif

GSM_CurrServTypeDef gsmCurrServerType = CURR_SERVER_NONE;

static char cGetFileLastDate[20] = {0};

static uint32_t u32GetFileSize = 0;

static uint8_t u8FileTransRetryCnt = 0;

static uint16_t u16GSM_ErrCode;

static char cFW_NewVer[20] = {0};

char cCmdPhoneNum[20] = {0};

FIFO_IndexTypeDef tSMS_Index = {0};

SMS_PkgTypeDef tSMS_SavePkg = {0},
               tSMS_GetPkg = {0};

uint8_t u8GSM_RX_Buf[1536] = {0};
uint8_t u8GSM_EVENT_Buf[512] = {0};
uint8_t u8GSM_LOG_Buf[1024] = {0};
uint8_t u8GSM_LOG_Buf2[1024] = {0};

uint16_t u16UnsendPkgNum[MAX_SOCKET_BANK] = {0};

uint8_t u8SendLogFailCnt[MAX_SOCKET_BANK] = {0};

uint8_t u8GSM_TempPkgBuf[TEMP_PKG_SIZE];

bool bGSM_FunctionStop = FALSE;

GSM_SocketDataTypeDef gsmTCP_CHK[MAX_SOCKET_BANK];
GSM_SocketDataTypeDef gsmTCP_DATA[MAX_SOCKET_BANK];
GSM_SocketDataTypeDef gsmTCP_LOG[MAX_SOCKET_BANK];

bool bIsIMEIReady = FALSE;

bool bCSQ_IsGood = TRUE;

bool bGSM_SetSleep = FALSE;
bool bGSM_WakeUp = FALSE;

bool bStealthFOTA_En = FALSE;
bool bStealthDownloadEn = FALSE;
bool bStealthUploadEn = FALSE;
#endif

//============================================================================//
// User Functions
//============================================================================//
#if ENABLE_GSM_NET
//============================================================================//
// @brief  Initialize the required parameters to handle the GSM blocking requests.
// @param  None.
// @retval None.
//============================================================================//
void GSM_BlockCmdInit(void)
{
    qtec21_BlockCmdInstance.bInUse = FALSE;
    qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

    for (int i = 0; i < 5; i++)
        qtec21_BlockCmdInstance.u32Arg[i] = 0;

    qtec21_BlockCmdInstance.pu8RetVal = NULL;
}

//============================================================================//
// @brief  Initialize the required parameters to handle the GSM non-blocking requests.
// @param  None.
// @retval None.
//============================================================================//
void GSM_NonBlockCmdInit(void)
{
    qtec21_NonBlockCmdInstance.bInUse = FALSE;
    qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

    for (int i = 0; i < 5; i++)
        qtec21_NonBlockCmdInstance.u32Arg[i] = 0;

    qtec21_NonBlockCmdInstance.pu8RetVal = NULL;
}

//============================================================================//
// @brief  Create a GSM blocking request.
// @param  qtec21Cmd: Command type.
//         pu8RetVal: Pointer to a uint8_t variable to store the returned value.
//         u8ArgNum: Number of following variables.
//         ...: Variables list.
// @retval None.
//============================================================================//
void GSM_BuildBlockCmd(QTEC21_BlockCMD_TypeDef qtec21Cmd, uint8_t *pu8RetVal, uint8_t u8ArgNum, ...)
{
    va_list valist;

    if (qtec21Cmd == QTEC21_BLOCK_NULL)
        return;

    uint32_t u32LastTick = HAL_GetTick();

    while (qtec21_BlockCmdInstance.bInUse == TRUE)
    {
        HAL_Delay(1);

        if (HAL_GetTick() > (u32LastTick + S_2_mS(150)))
        {
            *pu8RetVal = 0;

            return;
        }
    }

    // GSMprintf("\r\n> Prepared GSM command code: %d", (uint8_t)qtec21Cmd);

    qtec21_BlockCmdInstance.bInUse = TRUE;

    qtec21_BlockCmdInstance.pu8RetVal = pu8RetVal;

    va_start(valist, u8ArgNum);

    for (int i = 0; i < u8ArgNum; i++)
    {
        qtec21_BlockCmdInstance.u32Arg[i] = va_arg(valist, uint32_t);
    }

    va_end(valist);

    qtec21_BlockCmdInstance.qtec21Cmd = qtec21Cmd;
}

//============================================================================//
// @brief  Create a GSM non-blocking request.
// @param  qtec21Cmd: Command type.
//         pu8RetVal: Pointer to a uint8_t variable to store the returned value.
//         u8ArgNum: Number of following variables.
//         ...: Variables list.
// @retval None.
//============================================================================//
static void GSM_BuildNonBlockCmd(QTEC21_NonBlockCMD_TypeDef qtec21Cmd, uint8_t *pu8RetVal, uint8_t u8ArgNum, ...)
{
    va_list valist;

    if (qtec21Cmd == QTEC21_NONBLOCK_NULL)
        return;

    uint32_t u32LastTick = HAL_GetTick();

    while (qtec21_NonBlockCmdInstance.bInUse == TRUE)
    {
        HAL_Delay(1);

        if (HAL_GetTick() > (u32LastTick + S_2_mS(200)))
        {
            *pu8RetVal = 0;

            return;
        }
    }

    qtec21_NonBlockCmdInstance.bInUse = TRUE;

    qtec21_NonBlockCmdInstance.pu8RetVal = pu8RetVal;

    va_start(valist, u8ArgNum);

    for (int i = 0; i < u8ArgNum; i++)
    {
        qtec21_NonBlockCmdInstance.u32Arg[i] = va_arg(valist, uint32_t);
    }

    va_end(valist);

    qtec21_NonBlockCmdInstance.qtec21Cmd = qtec21Cmd;
}

//============================================================================//
// @brief  Handle the GSM blocking requests.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_BlockCmdHandle(void)
{
    uint32_t u32Arg[5];
    uint8_t u8RetVal;
    osStatus osRes;

    if (qtec21_BlockCmdInstance.qtec21Cmd == QTEC21_BLOCK_NULL)
        return;

    u32Arg[0] = qtec21_BlockCmdInstance.u32Arg[0];
    u32Arg[1] = qtec21_BlockCmdInstance.u32Arg[1];
    u32Arg[2] = qtec21_BlockCmdInstance.u32Arg[2];
    u32Arg[3] = qtec21_BlockCmdInstance.u32Arg[3];
    u32Arg[4] = qtec21_BlockCmdInstance.u32Arg[4];

    osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

    if (osRes != osOK)
    {
        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = 0;

        osMessagePut(GSM_BlockCpltHandle, (uint8_t)qtec21_BlockCmdInstance.qtec21Cmd, 0);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        return;
    }

    // GSMprintf("\r\n> GSM block command code: %d", (uint8_t)(qtec21_BlockCmdInstance.qtec21Cmd));

    switch ((uint8_t)(qtec21_BlockCmdInstance.qtec21Cmd))
    {
    case QTEC21_PWR_ON:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_PwrEnable();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PWR_ON, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PWR_OFF:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_PwrDisable();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PWR_OFF, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SWITCH_ON:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_TurnOnPwrKey();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SWITCH_ON, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SWITCH_OFF:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_TurnOffPwrKey();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SWITCH_OFF, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TURNOFF:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_TurnOffCmd();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TURNOFF, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_RESET:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_Reset();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_RESET, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_RESET_CHECK_FTP:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        QtEC21_ResetWithCheckFTP();

        osMessagePut(GSM_BlockCpltHandle, QTEC21_RESET_CHECK_FTP, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_AT_TEST:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = QtEC21_AT_Test();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_AT_TEST, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_READ_CGMI:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_ReadCGMI((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_READ_CGMI, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_READ_CGMM:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_ReadCGMM((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_READ_CGMM, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_READ_CGMR:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_ReadCGMR((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_READ_CGMR, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_GET_IMEI:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GetIMEI((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_GET_IMEI, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_SET_FUNC:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SetFunc((uint8_t)u32Arg[0], (bool)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_SET_FUNC, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_SAVE_PARA:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SaveConfigProf((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_SAVE_PARA, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_SET_PARA:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SetConfigProf((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_SET_PARA, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_ECHO_CONFIG:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_ConfigEcho((bool)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_ECHO_CONFIG, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_RESULT_FORMAT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_ConnCodeFormat((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_RESULT_FORMAT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_ERR_FORMAT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SetErrFormat((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_ERR_FORMAT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_SEL_TESET:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TECharSet((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_SEL_TESET, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_BASE_URC_OUTPUT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_URC_Outport((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_BASE_URC_OUTPUT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_COM_DTR_FUNCTION:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_COM_SetDTR((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_COM_DTR_FUNCTION, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_COM_SET_BAUDRATE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_COM_SetBaudRate((uint32_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_COM_SET_BAUDRATE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_STAT_CONFIG_RI:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Stat_ConfigRI((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_STAT_CONFIG_RI, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SIM_GET_IMSI:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GetIMSI((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SIM_GET_IMSI, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SIM_QUERY_USIM:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SIM_QueryUSIM((GSM_SIM_StatTypedef *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SIM_QUERY_USIM, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SIM_SET_PIN:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SIM_SetPIN((char *)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SIM_SET_PIN, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_NET_QUERY_CS:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Net_QueryCS();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_NET_QUERY_CS, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_NET_GET_CSQ:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Net_GetCSQ((uint8_t *)u32Arg[0], (uint8_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_NET_GET_CSQ, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_NET_GET_TIME:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Net_GetTime((uint8_t)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_NET_GET_TIME, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_DAT_2_CMD:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Dat_2_Cmd();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_DAT_2_CMD, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    case QTEC21_GPS_OUTPORT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigOutport();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_OUTPORT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_NMEASRC:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigNMEA_Src();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_NMEASRC, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GPSNMEATYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigGPS_NMEA_Type();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GPSNMEATYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GLONASSNMEATYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigGLONASS_NMEA_Type();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GLONASSNMEATYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GALILEONMEATYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigGalileo_NMEA_Type();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GALILEONMEATYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_BEIDOUNMEATYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigBeidou_NMEA_Type();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_BEIDOUNMEATYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GSVEXTNMEATYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigGSV_Ext();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GSVEXTNMEATYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GNSSCONFIG:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigGNSS();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GNSSCONFIG, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_AUTOGPS:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigAutoGPS();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_AUTOGPS, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_DELETE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_DelMsg();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_DELETE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_ON:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_TurnOn();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_ON, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_OFF:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_TurnOff();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_OFF, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_LOCATION:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_GetLoc((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_LOCATION, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_GETNMEA:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_GetNMEA((char *)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_GETNMEA, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_CONFIGXTRA:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_ConfigXTRA((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_CONFIGXTRA, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_XTRATIME:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_XTRA_Time((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_XTRATIME, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_XTRADATA:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_XTRA_Data((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_XTRADATA, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_GPS_XTRADATA_QUERY:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_GPS_XTRA_DataQuery((uint16_t *)u32Arg[0], (time_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_GPS_XTRADATA_QUERY, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }
#endif

    case QTEC21_SMS_SEL_SERVICE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_SelectServ((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_SEL_SERVICE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_SEL_FORMAT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_SetFormat((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_SEL_FORMAT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_SERV_CENTER:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_SCA((char *)u32Arg[0], (uint16_t)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_SERV_CENTER, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_SEL_STORAGE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_SetStorage((char *)u32Arg[0], (char *)u32Arg[1], (char *)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_SEL_STORAGE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_DELETE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_Del((uint16_t)u32Arg[0], (uint8_t)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_DELETE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_GET_LIST:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_GetList((char *)u32Arg[0], (uint8_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_GET_LIST, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_READ:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_Read((uint16_t)u32Arg[0], (uint8_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_READ, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_EVENT_REPORT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_ReportConfig((uint8_t)u32Arg[0], (uint8_t)u32Arg[1], (uint8_t)u32Arg[2],
                                                    (uint8_t)u32Arg[3], (uint8_t)u32Arg[4]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_EVENT_REPORT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_SMS_SEL_TEXTMODE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_SetParam((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_SMS_SEL_TEXTMODE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_CALL_DIAL:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_Dial((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_CALL_DIAL, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_CALL_CMD_2_DAT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_Cmd_2_Dat();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_CALL_CMD_2_DAT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_CALL_CONN_TIMEOUT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_ConnWaitTime((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_CALL_CONN_TIMEOUT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_CALL_GET_LIST:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_GetList((uint8_t *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_CALL_GET_LIST, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PD_PDP_CFG:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_ConfigPDP((uint8_t)u32Arg[0], (char *)u32Arg[1], (char *)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PD_PDP_CFG, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PD_CGQ_REQ:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_QualityReq((uint8_t)u32Arg[0], (TCP_PDP_ContextStruct *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PD_CGQ_REQ, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PD_CGQ_MIN:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_QualityReq((uint8_t)u32Arg[0], (TCP_PDP_ContextStruct *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PD_CGQ_MIN, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PD_PDP_ADDR:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_GetAddr((uint8_t)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PD_PDP_ADDR, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_PD_QUERY_PS:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_QueryPS();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_PD_QUERY_PS, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HW_GET_RTC:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HW_GetRTC((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HW_GET_RTC, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_PDP_CONFIG:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_ConfigPDP((uint8_t)u32Arg[0], (char *)u32Arg[1], (char *)u32Arg[2], (char *)u32Arg[3]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_PDP_CONFIG, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_PDP_ACT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_ActivPDP((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_PDP_ACT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_GET_IP:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_GetIP((uint8_t)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_GET_IP, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_PDP_DEACT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_DeactivPDP((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_PDP_DEACT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_CONN_CLOSE:
    {
        u8RetVal = (uint8_t)QtEC21_TCP_CloseConn((uint8_t)u32Arg[0], (uint16_t)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_CONN_CLOSE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_CONN_STATE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_IsConnect((bool)u32Arg[0], (uint8_t)u32Arg[1], (bool *)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_CONN_STATE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_SEND:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_Send((GSM_SocketDataTypeDef *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_SEND, 0);

        // HAL_Delay(3);
        HAL_Delay(100);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_SEND_STATUS:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_SendingStatus((uint8_t)u32Arg[0], (uint16_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_SEND_STATUS, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_RECV_CHECK:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_RecvDataLen((uint8_t)u32Arg[0], (uint16_t *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_RECV_CHECK, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_READ:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_GetRecvData((uint8_t)u32Arg[0], (uint8_t *)u32Arg[1], (uint16_t)u32Arg[2], (uint16_t *)u32Arg[3]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_READ, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_TCP_CMD_2_DAT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_Cmd_2_Dat((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_TCP_CMD_2_DAT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_PDP_ID:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgPDP((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_PDP_ID, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_ACC:
    {
        u8RetVal = (uint8_t)QtEC21_FTP_CfgAcc((char *)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_ACC, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_FILETYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgFileType();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_FILETYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_TRANSMODE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgTransMode();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_TRANSMODE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_TIMEOUT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgTimeout((uint16_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_TIMEOUT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_SSL_TYPE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgSSL_Type((bool)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_SSL_TYPE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_CONFIG_SSL_ID:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_CfgSSL_ID((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_CONFIG_SSL_ID, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_SSL_VERSION:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_SSL_Ver((uint8_t)u32Arg[0], (uint8_t)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_SSL_VERSION, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_SSL_CIPHER:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_SSL_Cipher((uint8_t)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_SSL_CIPHER, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_SSL_CACERT:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_SSL_CA((uint8_t)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_SSL_CACERT, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FTP_SSL_SECLV:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_SSL_SecLV((uint8_t)u32Arg[0], (uint8_t)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FTP_SSL_SECLV, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HTTP_CFG_PDP_ID:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_CfgPDP((uint8_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HTTP_CFG_PDP_ID, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HTTP_CFG_RESP_HEADER:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_RespHeader((bool)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HTTP_CFG_RESP_HEADER, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HTTP_URL:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_SetURL((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HTTP_URL, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HTTP_STOP:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_Cancel();

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HTTP_STOP, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_LIST:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FileList((char *)u32Arg[0], (uint8_t *)u32Arg[1], (uint16_t *)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_LIST, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_UPLOAD:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FileUpload((char *)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_UPLOAD, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_OPEN:
    {
        u8RetVal = (uint8_t)QtEC21_FileOpen((char *)u32Arg[0], (uint32_t *)u32Arg[1], (uint8_t)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_OPEN, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_SEEK:
    {
        u8RetVal = (uint8_t)QtEC21_FileSeek((uint32_t)u32Arg[0], (uint32_t)u32Arg[1]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_SEEK, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_READ:
    {
        u8RetVal = (uint8_t)QtEC21_FileRead((uint32_t)u32Arg[0], (uint32_t)u32Arg[1], (uint32_t *)u32Arg[2], (char *)u32Arg[3]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_READ, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_WRITE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FileWrite((uint32_t)u32Arg[0], (uint8_t *)u32Arg[1], (uint16_t)u32Arg[2]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_WRITE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_CLOSE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FileClose((uint32_t)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_CLOSE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_FIL_DELETE:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FileDelete((char *)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_FIL_DELETE, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    case QTEC21_HW_SLEEP_EN:
    {
        qtec21_BlockCmdInstance.qtec21Cmd = QTEC21_BLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HW_SleepEn((bool)u32Arg[0]);

        if (qtec21_BlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_BlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_BlockCpltHandle, QTEC21_HW_SLEEP_EN, 0);

        HAL_Delay(3);

        qtec21_BlockCmdInstance.bInUse = FALSE;

        break;
    }

    default:
        break;
    }

    osMutexRelease(mutex_gsm_rx_user);
}

//============================================================================//
// @brief  Handle the GSM non-blocking requests.
// @param  None.
// @retval None.
//============================================================================//
void GSM_NonBlockCmdHandle(void)
{
    uint32_t u32Arg[5];
    uint8_t u8RetVal;

    if (qtec21_NonBlockCmdInstance.qtec21Cmd == QTEC21_NONBLOCK_NULL)
        return;

    u32Arg[0] = qtec21_NonBlockCmdInstance.u32Arg[0];
    u32Arg[1] = qtec21_NonBlockCmdInstance.u32Arg[1];
    u32Arg[2] = qtec21_NonBlockCmdInstance.u32Arg[2];
    u32Arg[3] = qtec21_NonBlockCmdInstance.u32Arg[3];
    u32Arg[4] = qtec21_NonBlockCmdInstance.u32Arg[4];

    switch (qtec21_NonBlockCmdInstance.qtec21Cmd)
    {
    case QTEC21_SMS_SEND:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_SMS_SEND, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_SMS_Send((GSM_MsgDataTypeDef *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_SMS_SEND, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_SMS_SEND;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(GSM_SMS_SEND_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_CALL_ANS:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_CALL_ANS, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_Ans();

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_CALL_ANS, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_CALL_ANS;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(GSM_CALL_ANS_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_CALL_DISCONN:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_CALL_DISCONN, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_Call_Disconn();

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_CALL_DISCONN, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_CALL_DISCONN;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(GSM_CALL_DISCONN_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_PD_PS_STATE:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(180));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_PD_PS_STATE, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_Attach((bool)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_PD_PS_STATE, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_PD_PS_STATE;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(TCP_PDP_ATTACH_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_PD_PDP_ACT:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(180));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_PD_PDP_ACT, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_PD_Activ((uint8_t)u32Arg[0], (bool)u32Arg[1]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_PD_PDP_ACT, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_PD_PDP_ACT;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(TCP_PDP_ACTIV_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_TCP_CONN_OPEN:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_TCP_CONN_OPEN, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_TCP_OpenConn((uint8_t)u32Arg[0],
                                                (uint8_t)u32Arg[1],
                                                (GSM_SocketConnTypeDef *)u32Arg[2]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_TCP_CONN_OPEN, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_TCP_CONN_OPEN;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(TCP_OPEN_CONN_TIMEOUT + 5));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_OPEN:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_OPEN, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_Login((char *)u32Arg[0], (uint16_t)u32Arg[1]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_OPEN, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_OPEN;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_SET_DIR:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_SET_DIR, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_SetDir((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_SET_DIR, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_SET_DIR;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_MAKE_DIR:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_MAKE_DIR, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_MakeDir((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_MAKE_DIR, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_MAKE_DIR;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_UPLOAD:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_UPLOAD, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_UploadFile((char *)u32Arg[0], (char *)u32Arg[1]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_UPLOAD, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_UPLOAD;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_UPLOAD_TIMEOUT));

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_UPLOAD_DIRECT:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_UPLOAD_DIRECT, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_UploadDirect((uint8_t *)u32Arg[0], (char *)u32Arg[1], (uint32_t)u32Arg[2],
                                                    (uint16_t)u32Arg[3], (bool)u32Arg[4]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_UPLOAD_DIRECT, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_UPLOAD_DIRECT;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_UPLOAD_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_DOWNLOAD:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_DOWNLOAD, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_DownloadFile((char *)u32Arg[0], (char *)u32Arg[1], (uint32_t)u32Arg[2]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_DOWNLOAD, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_DOWNLOAD;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_DOWNLOAD_TIMEOUT));

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_FILE_SIZE:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_SIZE, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_FileSize((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_SIZE, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_FILE_SIZE;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_FILE_DATE:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_DATE, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_FileDate((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_DATE, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_FILE_DATE;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_TRANS_LEN:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_TRANS_LEN, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_TransLen();

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_TRANS_LEN, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_TRANS_LEN;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_FILE_DEL:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_DEL, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_FileDel((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_FILE_DEL, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_FILE_DEL;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(20));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_FTP_CLOSE:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_CLOSE, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_FTP_Logout();

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_FTP_CLOSE, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_FTP_CLOSE;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(FTP_FUNCTION_TIMEOUT));
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_HTTP_GET:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_HTTP_GET, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_Get();

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_HTTP_GET, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_HTTP_GET;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(HTTP_FUNCTION_TIMEOUT + 10));

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    case QTEC21_HTTP_READ_FILE:
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

            if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
                *(qtec21_NonBlockCmdInstance.pu8RetVal) = 0;

            osMessagePut(GSM_NonBlockCpltHandle, QTEC21_HTTP_READ_FILE, 0);

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;

            return;
        }

        qtec21_NonBlockCmdInstance.qtec21Cmd = QTEC21_NONBLOCK_NULL;

        u8RetVal = (uint8_t)QtEC21_HTTP_ReadFile((char *)u32Arg[0]);

        if (qtec21_NonBlockCmdInstance.pu8RetVal != NULL)
            *(qtec21_NonBlockCmdInstance.pu8RetVal) = u8RetVal;

        osMessagePut(GSM_NonBlockCpltHandle, QTEC21_HTTP_READ_FILE, 0);

        if (u8RetVal)
        {
            gsmCurrNonBlockCmd = QTEC21_HTTP_READ_FILE;

            osTimerStart(timerRTOS_GSM_NonBlockHandle, S_2_mS(HTTP_FUNCTION_TIMEOUT + 10));

            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }
        else
        {
            HAL_Delay(3);

            qtec21_NonBlockCmdInstance.bInUse = FALSE;
        }

        osMutexRelease(mutex_gsm_rx_user);

        break;
    }

    default:
        break;
    }
}

//============================================================================//
// @brief  Handle when getting message from GSM BlockComplete queue get timeout.
// @param  None.
// @retval None.
//============================================================================//
void GSM_Handle_Block_osEventTimeout(void)
{
    xQueueReset(GSM_BlockCpltHandle);
}

//============================================================================//
// @brief  Handle when getting message from GSM NonBlockComplete queue get timeout.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_Handle_NonBlock_osEventTimeout(void)
{
    xQueueReset(GSM_NonBlockCpltHandle);
}

//============================================================================//
// @brief  Handle when getting message from GSM NonBlockResult queue get timeout.
// @param  None.
// @retval None.
//============================================================================//
void GSM_Handle_NonBlockRes_osEventTimeout(void)
{
    xQueueReset(GSM_NonBlockResHandle);
}

// Hardware control functions

//============================================================================//
// @brief  Turn on GSM module power.
// @param  None.
// @retval None.
//============================================================================//
void GSM_PwrEnable(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PWR_ON, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_PwrEnable get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PWR_ON)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_PwrEnable get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Turn off GSM module power.
// @param  None.
// @retval None.
//============================================================================//
void GSM_PwrDisable(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PWR_OFF, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_PwrDisable get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PWR_OFF)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_PwrDisable get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Switch on GSM module.
// @param  None.
// @retval None.
//============================================================================//
void GSM_TurnOnPwrKey(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SWITCH_ON, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_TurnOnPwrKey get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SWITCH_ON)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_TurnOnPwrKey get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Switch off GSM module.
// @param  None.
// @retval None.
//============================================================================//
void GSM_TurnOffPwrKey(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SWITCH_OFF, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_TurnOffPwrKey get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SWITCH_OFF)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_TurnOffPwrKey get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Turn off GSM module.
// @param  None.
// @retval None.
//============================================================================//
void GSM_TurnOffCmd(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TURNOFF, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_TurnOffCmd get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TURNOFF)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_TurnOffCmd get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Reset GSM module.
// @param  None.
// @retval None.
//============================================================================//
void GSM_Reset(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_RESET, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_Reset get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_RESET)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_Reset get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Reset GSM module with check FTP processes.
// @param  None.
// @retval None.
//============================================================================//
void GSM_ResetWithCheckFTP(void)
{
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_RESET_CHECK_FTP, NULL, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_ResetWithCheckFTP get timeout.");

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_RESET_CHECK_FTP)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            GSMprintf("\r\n> GSM_ResetWithCheckFTP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);
}

// Basic functions

//============================================================================//
// @brief  Test AT command.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_AT_Test(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_AT_TEST, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_AT_Test get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_AT_TEST)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_AT_Test get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read CGMI.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_ReadCGMI(char *pcCGMI)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_READ_CGMI, &u8RetVal, 1, pcCGMI);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_ReadCGMI get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_READ_CGMI)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_ReadCGMI get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read CGMM.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_ReadCGMM(char *pcCGMM)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_READ_CGMM, &u8RetVal, 1, pcCGMM);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_ReadCGMM get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_READ_CGMM)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_ReadCGMM get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read CGMR.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_ReadCGMR(char *pcCGMR)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_READ_CGMR, &u8RetVal, 1, pcCGMR);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_ReadCGMR get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_READ_CGMR)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_ReadCGMR get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read the IMEI number.
// @param  pcIMEI: Pointer to a char array to store the IMEI number.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_GetIMEI(char *pcIMEI)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_GET_IMEI, &u8RetVal, 1, pcIMEI);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_GetIMEI get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_GET_IMEI)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_GetIMEI get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Control the functionality level.
// @param  u8Func: Functionality level.
//                 0: Minimum functionality.
//                 1: Full functionality (Default).
//                 4: Disable both TX and RX.
//         bRst: Reset flag.
//               0: Do not reset ME before setting it to the new functionality level.
//               1: Reset the ME before setting it to the new functionality level.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_SetFunc(uint8_t u8Func, bool bRst)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_SET_FUNC, &u8RetVal, 2, u8Func, bRst);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SetFunc get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_SET_FUNC)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SetFunc get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Save the current parameters configuration to a profile in the memory.
// @param  u8ConfigID: ID to assign to the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_SaveConfigProf(uint8_t u8ConfigID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_SAVE_PARA, &u8RetVal, 1, u8ConfigID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SaveConfigProf get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_SAVE_PARA)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SaveConfigProf get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the parameters from a profile as the current parameters.
// @param  u8ConfigID: ID of the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_SetConfigProf(uint8_t u8ConfigID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_SET_PARA, &u8RetVal, 1, u8ConfigID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SetConfigProf get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_SET_PARA)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SetConfigProf get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set command echo mode.
// @param  bOption: 1: Command echo on.
//                  0: Command echo off.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool GSM_ConfigEcho(bool bOption)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_ECHO_CONFIG, &u8RetVal, 1, bOption);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_ConfigEcho get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_ECHO_CONFIG)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_ConfigEcho get event status ERROR: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            // QtEC21_CheckRedoFTP();

            // RTOSprintf("\r\n> Reset system after 1 second.");

            // QtEC21_CheckRedoFTP();

            // if(bRTOS_En)
            //{
            //	osMutexWait(mutex_setting_user, S_2_mS(5));
            // }

            // System_Reset();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the format for the return code of the call connection.
// @param  u8Option: Option of the format.
//                   0: CONNECT result code only returned, dial tone and busy
//                   detection are both disabled
//                   1: CONNECT <text> result code only returned, dial tone and
//                   busy detection are both disabled
//                   2: CONNECT <text> result code returned, dial tone detection
//                   is enabled, busy detection is disabled
//                   3: CONNECT <text> result code returned, dial tone detection
//                   is disabled, busy detection is enabled
//                   4: CONNECT <text> result code returned, dial tone and busy
//                   detection are both enabled
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_ConnCodeFormat(uint8_t u8Option)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_RESULT_FORMAT, &u8RetVal, 1, u8Option);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_ConnCodeFormat get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_RESULT_FORMAT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_ConnCodeFormat get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Select TE character set.
// @param  pcCharSet: Pointer to the character set option as a string.
//                    "GSM": Default alphabet
//                    "IRA": International reference alphabet
//                    "UCS2": UCS2 alphabet.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_TECharSet(char *pcCharSet)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_SEL_TESET, &u8RetVal, 1, pcCharSet);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_TECharSet get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_SEL_TESET)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TECharSet get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Save the current parameters configuration to a profile in the memory.
// @param  u8ConfigID: ID to assign to the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_SetErrFormat(uint8_t u8Option)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_ERR_FORMAT, &u8RetVal, 1, u8Option);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SetErrFormat get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_ERR_FORMAT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SetErrFormat get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the URC outport.
// @param  pcOutPort: Pointer to a char array to indicate the outport.
//         "usbat": USB AT port
//         "usbmodem": USB modem port
//         "uart1": Main UART
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool GSM_URC_Outport(char *pcOutPort)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_BASE_URC_OUTPUT, &u8RetVal, 1, pcOutPort);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_URC_Outport get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_BASE_URC_OUTPUT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_URC_Outport get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Serial interface function

//============================================================================//
// @brief  Set the mode for the DTR function.
// @param  u8Option: Option for the DTR function.
//                   0: TA ignores status on DTR.
//                   1: Low -> High on DTR: Change to command mode while remaining
//                   the connected call.
//                   2: Low-> High on DTR: Disconnect data call, and change to
//                   command mode. When DTR is at high level, auto-answer function is disabled.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_COM_SetDTR(uint8_t u8Option)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_COM_DTR_FUNCTION, &u8RetVal, 1, u8Option);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_COM_SetDTR get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_COM_DTR_FUNCTION)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_COM_SetDTR get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the operating baud rate for the QtEC21.
// @param  u32BaudRate: The desired baudrate.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_COM_SetBaudRate(uint32_t u32BaudRate)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_COM_SET_BAUDRATE, &u8RetVal, 1, u32BaudRate);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_COM_SetBaudRate get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_COM_SET_BAUDRATE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_COM_SetBaudRate get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Status control function

//============================================================================//
// @brief  Configure the function of pin RI.
// @param  pcOption: Pointer to the variable indicating the function for pin RI.
//                   "off": Inactive when URC coming.
//                   "pulse": Create pulse when URC coming.
//                   "always": Change to always active.
//                   "auto": Change to active when there is incoming call.
//                           Change to inactive when call is finished.
//                   "wave": Create square wave when there is incoming call.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_Stat_ConfigRI(char *pcOption)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_STAT_CONFIG_RI, &u8RetVal, 1, pcOption);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_Stat_ConfigRI get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_STAT_CONFIG_RI)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Stat_ConfigRI get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// SIM functions

//============================================================================//
// @brief  Get the IMSI number.
// @param  pcIMSI: Pointer to a char array to store the IMSI number.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_GetIMSI(char *pcIMSI)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SIM_GET_IMSI, &u8RetVal, 1, pcIMSI);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_GetIMSI get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SIM_GET_IMSI)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_GetIMSI get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Query SIM status.
// @param  pu8RetCode: Pointer to a unit8_t variable to store the returned code.
// @retval True: SIM is detected.
//         False: SIM is not detected/ locked.
//============================================================================//
bool GSM_SIM_QueryUSIM(GSM_SIM_StatTypedef *simRetCode)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SIM_QUERY_USIM, &u8RetVal, 1, simRetCode);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GSM_USIM_QUERY_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_SIM_QueryUSIM get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GSM_USIM_QUERY_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SIM_QUERY_USIM)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SIM_QueryUSIM get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Change SIM pin.
// @param  pcOldPIN: Current PIN.
//         pcNewPIN: New PIN.
// @retval True: The operation is successful.
//         False: The operation is fail.
//============================================================================//
bool GSM_SIM_SetPIN(char *pcOldPIN, char *pcNewPIN)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SIM_SET_PIN, &u8RetVal, 2, pcOldPIN, pcNewPIN);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SIM_SetPIN get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SIM_SET_PIN)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SIM_SetPIN get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Network functions

//============================================================================//
// @brief  Query network registration on CS domain status.
// @param  None
// @retval True: Network registered on CS domain.
//         False: Network not registered on CS domain.
//============================================================================//
bool GSM_Net_QueryCS(void)
{
    uint8_t u8RetVal;
    osEvent event;
    // uint8_t u8QueryCnt = 0;

    GSM_BuildBlockCmd(QTEC21_NET_QUERY_CS, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_Net_QueryCS get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_NET_QUERY_CS)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n GSM_Net_QueryCS get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return u8RetVal;
}

//============================================================================//
// @brief  Get signal quality report.
// @param  pu8RSSI: Pointer to the variable to store the RSSI value.
//         pu8Ber: Pointer to the variable to store the BER value.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_Net_GetCSQ(uint8_t *pu8RSSI, uint8_t *pu8Ber)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_NET_GET_CSQ, &u8RetVal, 2, pu8RSSI, pu8Ber);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_Net_GetCSQ get timeout.");

            bGSM_ResetCheckFTP_Flag = TRUE;

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_NET_GET_CSQ)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Net_GetCSQ get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Obtain the latest time synchronized through network.
//         u8Option: Query option:
//                   0: Get the latest synchronized time.
//                   1: Get the time from RTC in UTC time.
//                   2: Get the time from RTC in local time.
// @param  pcBuf: Pointer to a char array to store the returned time.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_Net_GetTime(uint8_t u8Option, char *pcBuf)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_NET_GET_TIME, &u8RetVal, 2, u8Option, pcBuf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_Net_GetTime get timeout.");

            bGSM_ResetCheckFTP_Flag = TRUE;

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_NET_GET_TIME)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Net_GetTime get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Data mode to command mode

//============================================================================//
// @brief  Change the operation mode from data to command.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_Dat_2_Cmd(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_DAT_2_CMD, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_Dat_2_Cmd get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_DAT_2_CMD)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Dat_2_Cmd get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Messaging functions

//============================================================================//
// @brief  Select message service.
// @param  u8Type: Type of message service. Value: 0/1.
//                 Refer to the AT commands datasheet for the meaning of the
//                 options.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_SelectServ(uint8_t u8Type)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_SEL_SERVICE, &u8RetVal, 1, u8Type);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_SelectServ get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SEL_SERVICE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_SelectServ get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the input and output format of the short messages.
// @param  u8Type: Type of format.
//                 0: PDU mode.
//                 1: Text mode.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_SetFormat(uint8_t u8Type)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_SEL_FORMAT, &u8RetVal, 1, u8Type);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_SetFormat get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SEL_FORMAT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_SetFormat get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the service center address.
// @param  pcSCA: Pointer to the service center address as a string.
//         u16Type: Type of service center address.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_SCA(char *pcSCA, uint16_t u16Type)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_SERV_CENTER, &u8RetVal, 2, pcSCA, u16Type);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_SCA get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SERV_CENTER)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_SCA get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the preferred storages for SMS.
// @param  pcMem1: Messages to be read and deleted from this memory storage.
//         pcMem2: Messages will be written and sent to this memory storage.
//         pcMem3: Received messages will be placed in this memory storage if
//         routing to PC is not set.
//         Available message storage:
//         "SM": (U)SIM message storage.
//         "ME": Mobile equipment message storage.
//         "MT": Same as ME storage.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_SMS_SetStorage(char *pcMem1, char *pcMem2, char *pcMem3)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_SEL_STORAGE, &u8RetVal, 3, pcMem1, pcMem2, pcMem3);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_SetStorage get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SEL_STORAGE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_SetStorage get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Delete message.
// @param  u16Idx: ID of the message in the memory.
//         u8DelFlag: Delete option.
//         0: Delete the message specified in u16Idx.
//         1: Delete all read messages from the preferred message storage.
//         2: Delete all read messages from the preferred message storage and
//         sent mobile originated messages.
//         3: Delete all read messages from the preferred message storage, sent
//         and unsent mobile originated messages.
//         4: Delete all messages from the preferred message storage.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_Del(uint16_t u16Idx, uint8_t u8DelFlag)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_DELETE, &u8RetVal, 2, u16Idx, u8DelFlag);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_Del get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_DELETE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_Del get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the list of the messages stored in the preferred message storage.
// @param  pcStat: Pointer to the option for the type of messages listed.
//         pu8Buf: Pointer to the buffer storing message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_GetList(char *pcStat, uint8_t *pu8Buf)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_GET_LIST, &u8RetVal, 2, pcStat, pu8Buf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_SMS_GetList get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_GET_LIST)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_GetList get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read a message from the preferred message storage.
// @param  u8Idx: Index of the message to read.
//         pu8Buf: Pointer to the buffer to store the read message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_Read(uint16_t u16Idx, uint8_t *pu8Buf)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_READ, &u8RetVal, 2, u16Idx, pu8Buf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_Read get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_READ)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_Read get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Send a short message.
// @param  pcDstNbr: Pointer to ther destination phone number as a string.
//         u16Type: Type of service center address.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_Send(GSM_MsgDataTypeDef *pgsmMsgData)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_SMS_SEND, &u8RetVal, 1, pgsmMsgData);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(10)))
        {
            GSMprintf("\r\n> GSM_SMS_Send get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(10));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SEND)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_Send get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the SMS event report parameters.
// @param  u8Mode: Event report mode.
//         u8Mt: The rules for storing received SMS depend on its data coding scheme.
//         u8Bm: The rules for storing received CBMs depend on its data coding scheme.
//         u8Ds: SMS status report setting.
//         u8Bfr: TA buffer of unsolicited result codes setting.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_ReportConfig(uint8_t u8Mode, uint8_t u8Mt, uint8_t u8Bm,
                          uint8_t u8Ds, uint8_t u8Bfr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_EVENT_REPORT, &u8RetVal, 5, u8Mode, u8Mt, u8Bm, u8Ds, u8Bfr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_ReportConfig get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_EVENT_REPORT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_ReportConfig get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set including option for SMS text mode parameter.
// @param  u8Mode: Text mode parameter option.
//                 0: Not show additional parameters in result code.
//                 1: Show additional parameters in result code.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_SMS_SetParam(uint8_t u8Mode)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_SMS_SEL_TEXTMODE, &u8RetVal, 1, u8Mode);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_SMS_SetParam get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_SMS_SEL_TEXTMODE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_SMS_SetParam get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Call functions

//============================================================================//
// @brief  Connect to answer a phone call.
// @param  None
// @retval True: Connection is established successfully.
//         False: Connection is not established.
//============================================================================//
bool GSM_Call_Ans(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_CALL_ANS, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GSM_CALL_ANS_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_Call_Ans get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(GSM_CALL_ANS_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_ANS)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_Ans get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Dial and connect for a call.
// @param  pcPhoneNbr: Pointer to the destination phone number as a string.
// @retval Operation status in GSM_StatusTypeDef format.
//============================================================================//
GSM_StatusTypeDef GSM_Call_Dial(char *pcPhoneNbr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_CALL_DIAL, &u8RetVal, 1, pcPhoneNbr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GSM_CALL_DIAL_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_Call_Dial get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GSM_CALL_DIAL_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_DIAL)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_Dial get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (GSM_StatusTypeDef)u8RetVal;
}

//============================================================================//
// @brief  Disconnect a call.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_Call_Disconn(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_CALL_DISCONN, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GSM_CALL_DISCONN_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_Call_Disconn get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(GSM_CALL_DISCONN_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_DISCONN)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_Disconn get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Change the operation mode from command to data.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_Call_Cmd_2_Dat(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_CALL_CMD_2_DAT, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_Call_Cmd_2_Dat get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_CMD_2_DAT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_Cmd_2_Dat get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the waiting time for a connection to be established.
// @param  u8Time: The amount of waiting time in seconds.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_Call_ConnWaitTime(uint8_t u8Time)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_CALL_CONN_TIMEOUT, &u8RetVal, 1, u8Time);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_Call_ConnWaitTime get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_CONN_TIMEOUT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_ConnWaitTime get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the list of the current calls.
// @param  pu8Buf: Pointer to the buffer to store the call list.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_Call_GetList(uint8_t *pu8Buf)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_CALL_GET_LIST, &u8RetVal, 1, pu8Buf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_Call_GetList get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_CALL_GET_LIST)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_Call_GetList get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Packet domain functions

//============================================================================//
// @brief  Attach or detach the ME from the domain service.
// @param  bOption: 1: Attach the ME to the domain service.
//                  0: Detach the ME from the domain service.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_Attach(bool bOption)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_PD_PS_STATE, &u8RetVal, 1, bOption);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_Attach get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_PS_STATE)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_Attach get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Define parameter values for a PDP context.
// @param  u8ID: ID of the PDP context.
//         pcAPN: Pointer to the access point name.
//         pcAddr: Address to identify the MT, can be NULL.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_ConfigPDP(uint8_t u8ID, char *pcAPN, char *pcAddr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PD_PDP_CFG, &u8RetVal, 3, u8ID, pcAPN, pcAddr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_ConfigPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_PDP_CFG)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_ConfigPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Specify the quality of service profile when a PDP context is activated.
// @param  u8ID: ID of the PDP context.
//         pdpInstance: Pointer to a TCP_PDP_ContextStruct instance.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_QualityReq(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PD_CGQ_REQ,
                      &u8RetVal,
                      2,
                      u8ID,
                      pdpInstance);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_QualityReq get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_CGQ_REQ)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_QualityReq get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Allow specify Quality of Service Profile that used when the terminal
//         send an Activate PDP Context request message to the network.
// @param  u8ID: ID of the PDP context.
//         pdpInstance: Pointer to a TCP_PDP_ContextStruct instance.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_QualityMin(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PD_CGQ_MIN,
                      &u8RetVal,
                      2,
                      u8ID,
                      pdpInstance);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_QualityMin get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_CGQ_MIN)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_QualityMin get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Activate a PDP context.
// @param  u8ID: ID of the PDP context.
//         bOption: 1: Activate PDP context, 0: Deactivate PDP context.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_Activ(uint8_t u8ID, bool bOption)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_PD_PDP_ACT, &u8RetVal, 2, u8ID, bOption);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_Activ get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_PDP_ACT)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_PDP_Activ get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get IP address of a PDP context.
// @param  u8ID: ID of the PDP context.
//         pcAddr: Pointer to a char array to store the returned address.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_GetAddr(uint8_t u8ID, char *pcAddr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_PD_PDP_ADDR, &u8RetVal, 2, u8ID, pcAddr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_PD_GetAddr get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_PDP_ADDR)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_GetAddr get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Query network registration on PS domain status.
// @param  None.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool GSM_PD_QueryPS(void)
{
    uint8_t u8RetVal;
    osEvent event;
    // uint8_t u8QueryCnt = 0;

    GSM_BuildBlockCmd(QTEC21_PD_QUERY_PS, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_PD_QueryPS get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_PD_QUERY_PS)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_PD_QueryPS get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return u8RetVal;
}

// HW function

//============================================================================//
// @brief  Obtain the 4G module's RTC time.
// @param  pcBuf: Pointer to a char array to store the returned time.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_HW_GetRTC(char *pcBuf)
{
    uint8_t u8RetVal;
    osEvent event;
    // uint8_t u8QueryCnt = 0;

    GSM_BuildBlockCmd(QTEC21_HW_GET_RTC, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_HW_GetRTC get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HW_GET_RTC)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HW_GetRTC get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return u8RetVal;
}

// TCP/IP functions

//============================================================================//
// @brief  Config PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
//         pcAPN: Pointer to the APN name.
//         pcUser_1_Name: Pointer to the user name.
//         pcPwd: Pointer to the password.
// @retval True: The PDP context configured successfully.
//         False: The PDP context configuration fail.
//============================================================================//
bool GSM_TCP_ConfigPDP(uint8_t u8PDP_ID, char *pcAPN, char *pcUser_1_Name, char *pcPwd)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_PDP_CONFIG, &u8RetVal, 4, u8PDP_ID, pcAPN, pcUser_1_Name, pcPwd);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_TCP_ConfigPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_PDP_CONFIG)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_ConfigPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Activate a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
// @retval True: The PDP context is activated successfully.
//         False: The PDP context activation fail.
//============================================================================//
bool GSM_TCP_ActivPDP(uint8_t u8PDP_ID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_PDP_ACT, &u8RetVal, 1, u8PDP_ID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(TCP_PDP_ACTIV_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_TCP_ActivPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(TCP_PDP_ACTIV_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_PDP_ACT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_ActivPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get IP of a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
//         pcIP_Addr: Pointer to a char array to store the acquired IP.
// @retval True: PDP context IP is retrieved successfully.
//         False: PDP context IP retrieval fail.
//============================================================================//
bool GSM_TCP_GetIP(uint8_t u8PDP_ID, char *pcIP_Addr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_GET_IP, &u8RetVal, 2, u8PDP_ID, pcIP_Addr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(TCP_PDP_ACTIV_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_TCP_GetIP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(TCP_PDP_ACTIV_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_GET_IP)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_GetIP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Deactivate a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
// @retval True: The PDP context is deactivated successfully.
//         False: The PDP context deactivation fail.
//============================================================================//
bool GSM_TCP_DeactivPDP(uint8_t u8PDP_ID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_PDP_DEACT, &u8RetVal, 1, u8PDP_ID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(TCP_PDP_DEACTIV_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GSM_TCP_DeactivPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(TCP_PDP_DEACTIV_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_PDP_DEACT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_DeactivPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Open a connection for a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1-16).
//         u8ConnID: ID of the connection socket (0-11).
//         pcServiType: Pointer to the service type.
//         pcRemoteAddr: Pointer to the remote address.
//         u16RemotePort: Remote port address.
//         u16LocalPort: Local port address.
//         u8ConnMode: Access mode(0~2).
// @retval True: Connection is opened successfully.
//         False: Connection open fail.
//============================================================================//
bool GSM_TCP_OpenConn(uint8_t u8PDP_ID, uint8_t u8ConnID, GSM_SocketConnTypeDef *pgsmConnHandle)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_TCP_CONN_OPEN,
                         &u8RetVal,
                         3,
                         u8PDP_ID,
                         u8ConnID,
                         pgsmConnHandle);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_TCP_OpenConn get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_CONN_OPEN)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_OpenConn get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Close a connection of a PDP context.
// @param  u8SocketID: ID of the socket (0~11).
//         u16TimeOut: Timeout value (Second).
// @retval True: Connection is closed successfully.
//         False: Connection close fail.
//============================================================================//
bool GSM_TCP_CloseConn(uint8_t u8SocketID, uint16_t u16TimeOut)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_CONN_CLOSE, &u8RetVal, 2, u8SocketID, u16TimeOut);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(u16TimeOut + 5)))
        {
            GSMprintf("\r\n> GSM_TCP_CloseConn get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(u16TimeOut + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_CONN_CLOSE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_CloseConn get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Check the status of a connection.
// @param  bQueryType: Query type (0, 1).
//         u8ID: ID of the PDP context or the socket.
//         pbIsConnect: Pointer to a bool variable to indicate the connection.
//         False: PDP context configured fail.
// @retval True: Operation success.
//         False: Operation fail.
//============================================================================//
bool GSM_TCP_IsConnect(bool bQueryType, uint8_t u8ID, bool *bIsConnect)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_CONN_STATE, &u8RetVal, 3, bQueryType, u8ID, bIsConnect);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(7)))
        {
            GSMprintf("\r\n> GSM_TCP_IsConnect get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(7));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_CONN_STATE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_IsConnect get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Send data through a socket.
// @param  u8SocketID: Socket ID.
//         pu8Dat: Pointer to the data array.
//         u16Len: Length of the data to send.
// @retval True: Send successfully.
//         False: Send failed.
//============================================================================//
MsgSendStatusTypeDef GSM_TCP_Send(GSM_SocketDataTypeDef *pgsmSocketData)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_SEND, &u8RetVal, 1, pgsmSocketData);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_TCP_Send get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_SEND)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_Send get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (MsgSendStatusTypeDef)u8RetVal;
}

//============================================================================//
// @brief  Check the number of bytes sent and acknowledged successfully.
// @param  u8SocketID: Socket ID.
//         pu16ACK_Bytes: Pointer to a variable indicating the number of bytes
//                        acknowledged.
//         pu16NACK_Bytes: Pointer to a variable indicating the number of bytes
//                         not acknowledged.
// @retval True: Operation success.
//         False: Operation failed.
//============================================================================//
bool GSM_TCP_SendingStatus(uint8_t u8SocketID, uint16_t *pu16NACK_Bytes)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_SEND_STATUS, &u8RetVal, 2, u8SocketID, pu16NACK_Bytes);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_TCP_SendingStatus get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_SEND_STATUS)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_SendingStatus get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Check the number of TCP bytes received.
// @param  u8SocketID: Socket ID.
//         pu16RecvBytesNum: Pointer to the variable indicating the number of
//                           bytes received.
// @retval True: Operation success.
//         False: Operation failed.
//============================================================================//
bool GSM_TCP_RecvDataLen(uint8_t u8SocketID, uint16_t *pu16RecvBytesNum)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_RECV_CHECK, &u8RetVal, 2, u8SocketID, pu16RecvBytesNum);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_TCP_RecvDataLen get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_RECV_CHECK)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_RecvDataLen get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get received TCP data.
// @param  u8SocketID: Socket ID.
//         pu8Buf: Pointer to the buffer to store the data.
//         u16Len: Number of bytes to read.
//         u16ReadLen: Number of bytes read successfully.
// @retval True: Operation success.
//         False: Operation failed.
//============================================================================//
bool GSM_TCP_GetRecvData(uint8_t u8SocketID, uint8_t *pu8Buf, uint16_t u16Len,
                         uint16_t *u16ReadLen)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_READ, &u8RetVal, 4, u8SocketID, pu8Buf, u16Len, u16ReadLen);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_TCP_GetRecvData get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_READ)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_GetRecvData get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Change the operation mode from command to data for TCP/IP functions.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_TCP_Cmd_2_Dat(uint8_t u8SocketID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_TCP_CMD_2_DAT, &u8RetVal, 1, u8SocketID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GSM_TCP_Cmd_2_Dat get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(2));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_TCP_CMD_2_DAT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_TCP_Cmd_2_Dat get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// FTP functions

//============================================================================//
// @brief  Configure the PDP context for FTP.
// @param  u8PDP_ID: ID of the selected context.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgPDP(uint8_t u8PDP_ID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_PDP_ID, &u8RetVal, 1, u8PDP_ID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_PDP_ID)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the account for FTP.
// @param  pcUser_1_Name: Pointer to a char array storing the username.
//         pcPwd: Pointer to a char array storing the password.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgAcc(char *pcUser_1_Name, char *pcPwd)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_ACC, &u8RetVal, 2, pcUser_1_Name, pcPwd);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(FTP_ACC_CONFIG_TIMEOUT + 3)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgAcc get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(FTP_ACC_CONFIG_TIMEOUT + 3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_ACC)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgAcc get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the file to to binary.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgFileType(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_FILETYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgFileType get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_FILETYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgFileType get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the transfer mode.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgTransMode(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_TRANSMODE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgTransMode get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_TRANSMODE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgTransMode get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure timeout value.
// @param  u16TimeOut: Timeout value.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgTimeout(uint16_t u16TimeOut)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_TIMEOUT, &u8RetVal, 1, u16TimeOut);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgTimeout get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_TIMEOUT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgTimeout get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL mode.
// @param  bSSL_Type: 0: Disable SSL mode.
//                    1: Enable SSL mode.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgSSL_Type(bool bSSL_Type)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_SSL_TYPE, &u8RetVal, 1, bSSL_Type);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgSSL_Type get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_SSL_TYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgSSL_Type get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL context ID.
// @param  u8SSL_ID: Context ID for SSL, range of 0-5
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_CfgSSL_ID(uint8_t u8SSL_ID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_CONFIG_SSL_ID, &u8RetVal, 1, u8SSL_ID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_CfgSSL_ID get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CONFIG_SSL_ID)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_CfgSSL_ID get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL version.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_Ver: SSL version, range of 0-4.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_SSL_Ver(uint8_t u8SSL_ID, uint8_t u8SSL_Ver)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_SSL_VERSION, &u8RetVal, 2, u8SSL_ID, u8SSL_Ver);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_SSL_Ver get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_SSL_VERSION)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_SSL_Ver get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL cipher suit.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_Cipher: Code for SSL cipher suit.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_SSL_Cipher(uint8_t u8SSL_ID, char *pcCipher)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_SSL_CIPHER, &u8RetVal, 2, u8SSL_ID, pcCipher);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_SSL_Cipher get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_SSL_CIPHER)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_SSL_Cipher get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL path for CA certificate.
// @param  u8SSL_ID: Selected SSL context ID.
//         pcSSL_CA_Path: Pointer to the path for SSL CA.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_SSL_CA(uint8_t u8SSL_ID, char *pcSSL_CA_Path)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_SSL_CACERT, &u8RetVal, 2, u8SSL_ID, pcSSL_CA_Path);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_SSL_CA get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_SSL_CACERT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_SSL_CA get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure SSL security level.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_SecLV: SSL security level, range of 0-2.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_SSL_SecLV(uint8_t u8SSL_ID, uint8_t u8SSL_SecLV)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FTP_SSL_SECLV, &u8RetVal, 2, u8SSL_ID, u8SSL_SecLV);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GSM_FTP_SSL_SecLV get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_SSL_SECLV)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_SSL_SecLV get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Login the FTP server.
// @param  pcHostName: Domain name of the server.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_Login(char *pcHostName, uint16_t u16Port)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_OPEN, &u8RetVal, 2, pcHostName, u16Port);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_Login get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_OPEN)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_Login get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the working directory on FTP server.
// @param  pcDirPath: Pointer to a char array storing the directory path.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_SetDir(char *pcDirPath)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_SET_DIR, &u8RetVal, 1, pcDirPath);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_SetDir get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_SET_DIR)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_SetDir get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Make 1 or multiple directories on the FTP server.
// @param  pcDirPath: Pointer to a char array storing the directory path.
//         Any directory and sub-directory in the path which does not exist on
//         FTP server will be created.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_MakeDir(char *pcDirPath)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_MAKE_DIR, &u8RetVal, 1, pcDirPath);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_MakeDir get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_MAKE_DIR)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_MakeDir get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Upload a file in QTEC21 local storage to the FTP server.
// @param  pcLocalName: Pointer to a char array storing the name of the file in
//         the local storage.
//         pcFileName: Pointer to a char array storing the name of the file in the
//         FTP server.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_UploadFile(char *pcLocalName, char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_UPLOAD, &u8RetVal, 2, pcLocalName, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_UploadFile get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_UPLOAD)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_UploadFile get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Upload a file in QTEC21 local storage to the FTP server.
// @param  pu8DatBuf: Pointer to a uint8_t buffer storing upload data.
//         pcUploadFile: Pointer to a char array storing the name of the file
//         in the server to upload to.
//         u32UploadPos: Position in the file in server to start store data from.
//         u16UploadLen: The size of the data to upload.
//         bUploadFinish: TRUE: The current amount of data is the last.
//                        FALSE: The current amount of data is not the last.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_UploadDirect(uint8_t *pu8DatBuf, char *pcUploadFile, uint32_t u32UploadPos,
                          uint16_t u16UploadLen, bool bUploadFinish)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_UPLOAD_DIRECT, &u8RetVal, 5, pu8DatBuf, pcUploadFile,
                         u32UploadPos, u16UploadLen, bUploadFinish);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(14)))
        {
            GSMprintf("\r\n> GSM_FTP_UploadDirect get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(14));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_UPLOAD_DIRECT)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_UploadDirect get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Download a file from the FTP server.
// @param  pcFileName: Pointer to a char array storing the name of the file to download.
//         pcLocalDest: Pointer to a char array storing the destination storage.
//         u32FilePtr: Start address of the content of the file to start download from.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_DownloadFile(char *pcFileName, char *pcLocalDest, uint32_t u32FilePtr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_DOWNLOAD, &u8RetVal, 3, pcFileName, pcLocalDest, u32FilePtr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_DownloadFile get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_DOWNLOAD)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_DownloadFile get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the size of a file on the FTP server.
// @param  pcFileName: Pointer to a char array storing the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileSize(char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_FILE_SIZE, &u8RetVal, 1, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_FileSize get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_FILE_SIZE)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_FileSize get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the latest modifying date of a file on server.
// @param  pcFileName: Pointer to a char array storing the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileDate(char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_FILE_DATE, &u8RetVal, 1, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_FileDate get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_FILE_DATE)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_FileDate get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the number of bytes have been transferred from server to module.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_TransLen(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_TRANS_LEN, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_TransLen get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_TRANS_LEN)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_TransLen get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Delete a file in the FTP server.
// @param  pcFileName: Pointer to a char array storing the name of the file to
//         delete in the FTP server.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileDel(char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_FILE_DEL, &u8RetVal, 1, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_FileDel get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_FILE_DEL)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_FileDel get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Logout FTP server.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_Logout(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_FTP_CLOSE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FTP_Logout get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FTP_CLOSE)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FTP_Logout get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// HTTP functions

//============================================================================//
// @brief  Configure the PDP context for HTTP.
// @param  u8PDP_ID: ID of the selected context.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_CfgPDP(uint8_t u8PDP_ID)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_HTTP_CFG_PDP_ID, &u8RetVal, 1, u8PDP_ID);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_HTTP_CfgPDP get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_CFG_PDP_ID)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_CfgPDP get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the response header for HTTP.
// @param  bEnable: 0: Disable response header.
//                  1: Enable response header.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_RespHeader(bool bEnable)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_HTTP_CFG_RESP_HEADER, &u8RetVal, 1, bEnable);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_HTTP_RespHeader get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_CFG_RESP_HEADER)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_RespHeader get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the HTTP URL.
// @param  pcURL: URL to set.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_SetURL(char *pcURL)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_HTTP_URL, &u8RetVal, 1, pcURL);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_HTTP_SetURL get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_URL)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_SetURL get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Send a GET request to an HTTP server.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_Get(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_HTTP_GET, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_HTTP_Get get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_GET)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_Get get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read the data as the GET request response and store it into a file.
// @param  pcFileName: Path of the file to store the response data.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_ReadFile(char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildNonBlockCmd(QTEC21_HTTP_READ_FILE, &u8RetVal, 1, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_HTTP_ReadFile get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_NonBlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_READ_FILE)
            {
                osMessageGet(GSM_NonBlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_ReadFile get event status: %d", event.status);

            GSM_Handle_NonBlock_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  End an HTTP request.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_HTTP_Cancel(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_HTTP_STOP, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(15)))
        {
            GSMprintf("\r\n> GSM_HTTP_Cancel get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(15));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HTTP_STOP)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HTTP_Cancel get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// File operation functions

//============================================================================//
// @brief  List the information of a file/ files in a storage.
// @param  pcPattern: Pointer to a char array storing the file path pattern.
//         pu8Resp: Pointer to the buffer storing the returned response.
//         pu16RetLen: Pointer to a variable storing the length of the response.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileList(char *pcPattern, uint8_t *pu8Resp, uint16_t *pu16RetLen)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_LIST, &u8RetVal, 3, pcPattern, pu8Resp, pu16RetLen);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_FileList get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_LIST)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileList get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Upload a file to QTEC21 local storage.
// @param  pcFileName: Name of the file to upload to the storage.
//         pcLocalName: Name of the file in the local storage.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileUpload(char *pcFileName, char *pcLocalName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_UPLOAD, &u8RetVal, 2, pcFileName, pcLocalName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(30)))
        {
            GSMprintf("\r\n> GSM_FileUpload get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(30));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_UPLOAD)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileUpload get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Open a file in the module storage.
// @param  pcFileName: Pointer to a char array storing the file name.
//         pu32FileHandle: Pointer to a uint32_t variable to store the assigned file ID.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileOpen(char *pcFileName, uint32_t *pu32FileHandle, uint8_t u8Mode)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_OPEN, &u8RetVal, 3, pcFileName, pu32FileHandle, u8Mode);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_FileOpen get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_OPEN)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileOpen get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set the file pointer to an address in a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
//         u32FilePtr: Address of the file to set the file pointer to.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileSeek(uint32_t u32FileHandle, uint32_t u32FilePtr)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_SEEK, &u8RetVal, 2, u32FileHandle, u32FilePtr);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(8)))
        {
            GSMprintf("\r\n> GSM_FileSeek get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(8));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_SEEK)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileSeek get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Read a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
//         u32ReadLen: The length of data to read.
//         pu32ActualLen: Pointer to a uint32_t variable to store the actual read length.
//         pcReadContent: Pointer to a char array to store the read data.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileRead(uint32_t u32FileHandle, uint32_t u32ReadLen, uint32_t *pu32ActualLen,
                  char *pcReadContent)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_READ, &u8RetVal, 4, u32FileHandle, u32ReadLen, pu32ActualLen, pcReadContent);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(FILE_READ_TIMEOUT + 10)))
        {
            GSMprintf("\r\n> GSM_FileRead get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(FILE_READ_TIMEOUT + 10));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_READ)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileRead get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Write to a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
//         pu8DatBuf: Pointer to a uint8_t array storing data to write.
//         u16WriteLen: Number of bytes to write to the data buffer.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileWrite(uint32_t u32FileHandle, uint8_t *pu8DatBuf, uint16_t u16WriteLen)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_WRITE, &u8RetVal, 3, u32FileHandle, pu8DatBuf, u16WriteLen);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(20)))
        {
            GSMprintf("\r\n> GSM_FileWrite get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(20));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_WRITE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileWrite get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Close a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileClose(uint32_t u32FileHandle)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_CLOSE, &u8RetVal, 1, u32FileHandle);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_FileClose get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_CLOSE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileClose get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Delete a file in the module storage.
// @param  pcFileName: Pointer to a char array store the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FileDelete(char *cFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_FIL_DELETE, &u8RetVal, 1, cFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(15)))
        {
            GSMprintf("\r\n> GSM_FileDelete get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(15));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_FIL_DELETE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_FileDelete get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

// Hardware related functions

//============================================================================//
// @brief  Configure the sleep mode enable.
// @param  bOption: Sleep mode enable, 1 Enable, 0: Disable.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GSM_HW_SleepEn(bool bOption)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_HW_SLEEP_EN, &u8RetVal, 1, bOption);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GSM_HW_SleepEn get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(3));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_HW_SLEEP_EN)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GSM_HW_SleepEn get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Handle the callback of the timer for GSM block requests.
// @param  None.
// @retval None.
//============================================================================//
void GSM_BlockCallBackHandle(void)
{
    bGSMTimeOut = TRUE;

    // printf("\r\n> GSM block function timeout!");

    if ((qtec21_BlockCmdInstance.qtec21Cmd != QTEC21_FTP_CONFIG_ACC) &&
        (qtec21_BlockCmdInstance.qtec21Cmd != QTEC21_FIL_OPEN) &&
        (qtec21_BlockCmdInstance.qtec21Cmd != QTEC21_FIL_SEEK) &&
        (qtec21_BlockCmdInstance.qtec21Cmd != QTEC21_FIL_READ))
        QtEC21_RetryCheck();
}

//============================================================================//
// @brief  Handle the callback of the timer for GSM non block requests.
// @param  None.
// @retval None.
//============================================================================//
void GSM_NonBlockCallBackHandle(void)
{
    uint16_t u16Res;
    uint32_t u32ResCode;

    // printf("\r\n> GSM non-block function timeout!");

    switch ((uint8_t)gsmCurrNonBlockCmd)
    {
    case QTEC21_SMS_SEND:
        // printf("\r\n> GSM send SMSLE910  timeout.");

        break;

    case QTEC21_CALL_ANS:
        // printf("\r\n> GSM call answer timeout.");

        break;

    case QTEC21_CALL_DISCONN:
        // printf("\r\n> GSM call disconnect timeout.");

        break;

    case QTEC21_PD_PS_STATE:
        // printf("\r\n> GSM PD attach PDP timeout.");

        break;

    case QTEC21_PD_PDP_ACT:
        // printf("\r\n> GSM PD activate PDP timeout.");

        break;

    case QTEC21_TCP_CONN_OPEN:
        // printf("\r\n> GSM TCP open connection timeout.");

        break;

    case QTEC21_FTP_OPEN:
        // printf("\r\n> GSM FTP open connection timeout.");

        break;

    case QTEC21_FTP_SET_DIR:
        // printf("\r\n> GSM FTP set working directory timeout.");

        break;

    case QTEC21_FTP_MAKE_DIR:
        // printf("\r\n> GSM FTP create directory timeout.");

        break;

    case QTEC21_FTP_UPLOAD:
    case QTEC21_FTP_UPLOAD_DIRECT:
        // printf("\r\n> GSM FTP upload timeout.");

        break;

    case QTEC21_FTP_DOWNLOAD:
        // printf("\r\n> GSM FTP download timeout.");

        break;

    case QTEC21_FTP_FILE_SIZE:
        // printf("\r\n> GSM FTP file size timeout.");

        break;

    case QTEC21_FTP_FILE_DATE:
        // printf("\r\n> GSM PD activate PDP timeout.");

        break;

    case QTEC21_FTP_TRANS_LEN:
        // printf("\r\n> GSM FTP file transfer length timeout.");

        break;

    case QTEC21_FTP_FILE_DEL:
        // printf("\r\n> GSM FTP file delete timeout.");

        break;

    case QTEC21_FTP_CLOSE:
        // printf("\r\n> GSM FTP close timeout.");

        break;

    case QTEC21_HTTP_GET:
        // printf("\r\n> GSM HTTP GET timeout.");

        break;

    case QTEC21_HTTP_READ_FILE:
        // printf("\r\n> GSM HTTP read file timeout.");

        break;

    default:
        break;
    }

    if (gsmCurrNonBlockCmd != QTEC21_SMS_SEND)
    {
        u32ResCode = 0;

        osMessagePut(GSM_NonBlockResCodeHandle, u32ResCode, 0);

        u16Res = ((uint16_t)gsmCurrNonBlockCmd << 8) | GSM_NONBLOCK_RES_NO_RESPONSE;

        osMessagePut(GSM_NonBlockResHandle, u16Res, 0);
    }

    gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

    qtec21_NonBlockCmdInstance.bInUse = FALSE;

    QtEC21_RetryCheck();
}

//============================================================================//
// @brief  Init parameters for GSM functions.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FuncInit(void)
{
    GSMprintf("\r\n> ### GSM FUNCTION INIT ###");

    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    xQueueReset(GSM_BlockCpltHandle);

    xQueueReset(GSM_NonBlockCpltHandle);

    osTimerStop(timerRTOS_GSM_BlockHandle);

    osTimerStop(timerRTOS_GSM_NonBlockHandle);

    u8GSM_NoRespCnt = 0;

    procGSM_PDP_Step = PDP_STEP_NULL;

    procGSM_FOTA_Step = FOTA_STEP_NULL;

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
    procGSM_Download_Step = DOWNLOAD_STEP_NULL;
#endif

    procGSM_Upload_Step = UPLOAD_STEP_NULL;

#if ENABLE_UPLOAD_CHK
    procGSM_UploadChk_Step = UPLOAD_CHK_STEP_NULL;
#endif

    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
    procXTRA_Update_Step = XTRA_UPDATE_STEP_NULL;
#endif

    QtEC21_TCP_Init();

    HAL_Delay(S_2_mS(1));

    GSMprintf("\r\n> ### RESET GSM RX BUF ###");

    RingBuf_Reset(&rbBuf[GSM_COMPORT_DEF]);

    u16CurrGSM_ByteNum = 0;

    // char cLog[20] = {0};

    // sprintf(cLog, "GSM Func Init");

    // GPRS_DebugLog(cLog, strlen(cLog));
}

//============================================================================//
// @brief  Send 4G module to sleep.
// @param  None.
// @retval None.
//============================================================================//
void GSM_SetSleep(void)
{
    bQtEC21_State = FALSE;

    bQtEC21_Ready = FALSE;

    bGPS_Ready = FALSE;

    bQtEC21_InitFinish = FALSE;

    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    QtEC21_TCP_Init();

    u8GSM_NoRespCnt = 0;

    osTimerStop(timerRTOS_GSM_BlockHandle);
    osTimerStop(timerRTOS_GSM_NonBlockHandle);

    bGSMTimeOut = TRUE;

    if (gsmCurrNonBlockCmd != QTEC21_NONBLOCK_NULL)
    {
        if (gsmCurrNonBlockCmd != QTEC21_SMS_SEND)
        {
            osMessagePut(GSM_NonBlockResHandle, ((uint16_t)gsmCurrNonBlockCmd << 8) | GSM_NONBLOCK_RES_NULL, 0);
        }

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;
    }

    GSM_HW_SleepEn(1);
    // #if ENABLE_MG82
    // 	QtEC21_SetSleep();
    // #else
    // 	GSM_HW_SleepEn(1);
    // #endif
}

//============================================================================//
// @brief  Get the $AS command from an SMS message.
// @param  pMsg: Pointer to a char array storing the message.
//         pCmd: Pointer to a char array to store the returned command.
// @retval None.
//============================================================================//
static void GSM_GetMsgCmd(char *pMsg, char *pCmd)
{
    uint8_t CmdNum, CmdCnt;

    CmdCnt = sizeof(MSG_CMD) / sizeof(MsgCmdTypeDef);

    for (CmdNum = 0; CmdNum < CmdCnt; CmdNum++)
    {
        if (MSG_CMD[CmdNum].pcShort == NULL)
            continue;

        if (strncmp(pMsg, MSG_CMD[CmdNum].pcShort, strlen(MSG_CMD[CmdNum].pcShort)) == 0)
        {
            strcpy(pCmd, MSG_CMD[CmdNum].pcSET);

            break;
        }
    }

    if (strlen(pCmd) == 0 && strlen(pMsg) > 0)
    {
        CmdNum = atoi(pMsg);

        if (CmdNum > CMD_ID_MAX)
            return;

        if (MSG_CMD[CmdNum].pcSET != NULL)
        {
            strcpy(pCmd, MSG_CMD[CmdNum].pcSET);
        }
    }
}

//============================================================================//
// @brief  Save an SMS package to temp file.
// @param  pSMS_Data: Pointer to a variable of type GSM_MsgDataTypeDef
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_SMS_SavePkg(GSM_MsgDataTypeDef *pSMS_Data)
{
    char cFilePath[64] = {0};
    uint32_t u32Addr = 0;
    osStatus osRes;
    bool bRes;

    if (pSMS_Data == NULL)
        return FALSE;

    // If the command request file is already full
    if ((((tSMS_Index.u16Tail + 1) % GSM_SMS_NUM_MAX) ==
         tSMS_Index.u16Head) ||
        (tSMS_Index.u16Size >= (GSM_SMS_NUM_MAX - 1)))
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SMS_PKG_FULL, STR_NULL);
#endif

        return FALSE;
    }

    if (strlen(pSMS_Data->pcData) != pSMS_Data->u16DataLen)
    {
        GSMprintf("\r\n> GSM SMS save command package length incorrect (%d-%d).", strlen(pSMS_Data->pcData), pSMS_Data->u16DataLen);

        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(&tSMS_SavePkg, 0x00, sizeof(SMS_PkgTypeDef));

    tSMS_SavePkg.u8Chk1 = SMS_PKG_CHK_1;
    tSMS_SavePkg.u8Chk2 = SMS_PKG_CHK_2;
    sprintf(tSMS_SavePkg.cPhoneNum, "%s", pSMS_Data->pcPhoneNum);
    sprintf(tSMS_SavePkg.cSMS, "%s", pSMS_Data->pcData);
    tSMS_SavePkg.u16Len = pSMS_Data->u16DataLen;
    tSMS_SavePkg.u8RetryCnt = 0;

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SMS_PKG_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);
#endif

    u32Addr = sizeof(FIFO_IndexTypeDef) + (uint32_t)tSMS_Index.u16Tail * sizeof(SMS_PkgTypeDef);

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)&tSMS_SavePkg,
                              sizeof(SMS_PkgTypeDef),
                              u32Addr);

    if (bRes)
    {
        GSMprintf("\r\n> GSM SMS save package %d OK", tSMS_Index.u16Tail);

        GSM_SMS_PkgIncTail();
    }
    else
    {
        GSMprintf("\r\n> GSM SMS save package %d ERROR", tSMS_Index.u16Tail);
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Get an SMS package from temp file.
// @param  pSMS_Pkg: Pointer to a variable of type SMS_PkgTypeDef to store the
//         SMS package.
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_SMS_GetPkg(SMS_PkgTypeDef *pSMS_Pkg)
{
    char cFilePath[128];
    uint32_t u32Addr = 0;
    osStatus osRes;
    bool bRes;

    if (pSMS_Pkg == NULL)
        return FALSE;

    // If the SMS request file is empty
    if ((tSMS_Index.u16Head == tSMS_Index.u16Tail) ||
        (tSMS_Index.u16Size == 0))
    {
        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SMS_PKG_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);
#endif

    if (SD_CheckFileExist(cFilePath) == FALSE)
    {
        GSMprintf("\r\n> SMS package request file does not exist.");

        if (bRTOS_En)
            // Release GSM_SD synchronization
            osRes = osMutexRelease(mutex_gsm_sd_user);

        return FALSE;
    }

    memset(pSMS_Pkg, 0x00, sizeof(SMS_PkgTypeDef));

    u32Addr = sizeof(FIFO_IndexTypeDef) + (uint32_t)tSMS_Index.u16Head * sizeof(SMS_PkgTypeDef);

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)pSMS_Pkg,
                             sizeof(SMS_PkgTypeDef),
                             u32Addr);

    if (bRes)
    {
        if (strlen(pSMS_Pkg->cSMS) != pSMS_Pkg->u16Len)
        {
            GSMprintf("\r\n> GSM SMS get SMS package length incorrect (%d-%d).", strlen(pSMS_Pkg->cSMS), pSMS_Pkg->u16Len);

            GSM_SMS_PkgIncHead();

            bRes = FALSE;
        }

        if (pSMS_Pkg->u8Chk1 != SMS_PKG_CHK_1)
        {
            GSMprintf("\r\n> GSM SMS get SMS package %d CHK 1 ERROR: 0x%02X", tSMS_Index.u16Head, pSMS_Pkg->u8Chk1);

            GSM_SMS_PkgIncHead();

            bRes = FALSE;
        }

        if (pSMS_Pkg->u8Chk2 != SMS_PKG_CHK_2)
        {
            GSMprintf("\r\n> GSM SMS get SMS package %d CHK 2 ERROR: 0x%02X", tSMS_Index.u16Head, pSMS_Pkg->u8Chk2);

            GSM_SMS_PkgIncHead();

            bRes = FALSE;
        }

        if (bRes)
        {
            GSMprintf("\r\n> GSM SMS get SMS package %d OK.", tSMS_Index.u16Head);

            GSM_SMS_PkgIncHead();
        }
    }
    else
    {
        GSMprintf("\r\n> GSM SMS get SMS package %d ERROR.", tSMS_Index.u16Head);
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Increase the tail index of the pending SMS request file.
// @param  None.
// @retval None.
//============================================================================//
void GSM_SMS_PkgIncTail(void)
{
    bool bRes;
    char cFilePath[64] = {0};

    // If the SMS request file is already full
    if ((((tSMS_Index.u16Tail + 1) % GSM_SMS_NUM_MAX) == tSMS_Index.u16Head) ||
        (tSMS_Index.u16Size >= (GSM_SMS_NUM_MAX - 1)))
    {
        return;
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SMS_PKG_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);
#endif

    tSMS_Index.u16Tail = (tSMS_Index.u16Tail + 1) % GSM_SMS_NUM_MAX;

    tSMS_Index.u16Size++;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFilePath, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Increase the head index of the pending SMS request file.
// @param  None.
// @retval None.
//============================================================================//
void GSM_SMS_PkgIncHead(void)
{
    bool bRes;
    char cFilePath[64] = {0};

    // If the SMS request file is empty
    if ((tSMS_Index.u16Head == tSMS_Index.u16Tail) ||
        (tSMS_Index.u16Size == 0))
    {
        return;
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SMS_PKG_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);
#endif

    tSMS_Index.u16Head = (tSMS_Index.u16Head + 1) % GSM_SMS_NUM_MAX;

    tSMS_Index.u16Size--;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFilePath, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Check if the device is openning connection to TCP server.
// @param  None.
// @retval TRUE: The device is openning connection to TCP server.
//         FALSE: The device is not openning connection to TCP server.
//============================================================================//
static bool GSM_IsOpenConnectTCP(void)
{
    if ((procGSM_TCP_Step[SOCKET_BANK_DEF] >= CONN_STEP_OPEN) && (procGSM_TCP_Step[SOCKET_BANK_DEF] <= CONN_STEP_OPEN_WAIT))
        return TRUE;
#if ENABLE_MULTI_SERVER
    else if ((procGSM_TCP_Step[SOCKET_BANK_EXT] >= CONN_STEP_OPEN) && (procGSM_TCP_Step[SOCKET_BANK_EXT] <= CONN_STEP_OPEN_WAIT))
        return TRUE;
#endif

    return FALSE;
}

//============================================================================//
// @brief  Check if the 4G function is being busy.
// @param  None.
// @retval TRUE: The 4G function is being busy.
//         FALSE: The 4G function is not busy.
//============================================================================//
static bool GSM_IsBusy(void)
{
    if (((procGSM_FOTA_Step >= FOTA_STEP_OPEN) && (procGSM_FOTA_Step <= FOTA_STEP_CLOSE_WAIT)) ||
#if ENABLE_FTP_DOWNLOAD == 1 && ENABLE_FTP_BACKUP == 1
        ((procGSM_Download_Step >= DOWNLOAD_STEP_OPEN) && (procGSM_Download_Step <= DOWNLOAD_STEP_CLOSE_WAIT)) ||
#endif
        // #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA == 1
        //((procXTRA_Download_Step >= XTRA_STEP_HTTP_GET) && (procXTRA_Download_Step <= XTRA_STEP_HTTP_READ_WAIT)) ||
        // #endif
        ((procHTTP_Download_Step >= HTTP_DOWNLOAD_STEP_GET) && (procHTTP_Download_Step <= HTTP_DOWNLOAD_STEP_READ_WAIT)) ||
#if ENABLE_UPLOAD_CHK
        ((procGSM_UploadChk_Step >= UPLOAD_CHK_STEP_OPEN) && (procGSM_UploadChk_Step <= UPLOAD_CHK_STEP_CLOSE_WAIT)) ||
#endif
        ((procGSM_TCP_Step[SOCKET_BANK_DEF] >= CONN_STEP_OPEN) && (procGSM_TCP_Step[SOCKET_BANK_DEF] <= CONN_STEP_OPEN_WAIT)) ||
#if ENABLE_MULTI_SERVER
        ((procGSM_TCP_Step[SOCKET_BANK_EXT] >= CONN_STEP_OPEN) && (procGSM_TCP_Step[SOCKET_BANK_EXT] <= CONN_STEP_OPEN_WAIT)) ||
#endif
        ((procGSM_Upload_Step >= UPLOAD_STEP_OPEN) && (procGSM_Upload_Step <= UPLOAD_STEP_CLOSE_WAIT)) ||
        (gsmCurrNonBlockCmd == QTEC21_SMS_SEND))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Check if the 4G function is waiting for a response of an FTP function
//         or HTTP function.
// @param  None.
// @retval TRUE: Is waiting for a response.
//         FALSE: Is not waiting for a response.
//============================================================================//
static bool GSM_IsWaitResp(void)
{
    if (procGSM_FOTA_Step != FOTA_STEP_NULL)
    {
        switch ((int)procGSM_FOTA_Step)
        {
        case FOTA_STEP_OPEN_WAIT:
        case FOTA_STEP_SET_DIR_WAIT:
        case FOTA_STEP_GET_SIZE_WAIT:
        case FOTA_STEP_DOWNLOAD_WAIT:
        case FOTA_STEP_CLOSE_WAIT:
            return TRUE;

        default:
            return FALSE;
        }
    }
#if ENABLE_FTP_DOWNLOAD == 1 && ENABLE_FTP_BACKUP == 1
    else if (procGSM_Download_Step != DOWNLOAD_STEP_NULL)
    {
        switch ((int)procGSM_Download_Step)
        {
        case DOWNLOAD_STEP_OPEN_WAIT:
        case DOWNLOAD_STEP_SET_DIR_WAIT:
        case DOWNLOAD_STEP_GET_SIZE_WAIT:
        case DOWNLOAD_STEP_DOWNLOAD_WAIT:
        case DOWNLOAD_STEP_CLOSE_WAIT:
            return TRUE;

        default:
            return FALSE;
        }
    }
#endif
    else if (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_NULL)
    {
        switch ((int)procHTTP_Download_Step)
        {
        case HTTP_DOWNLOAD_STEP_GET_WAIT:
        case HTTP_DOWNLOAD_STEP_READ_WAIT:
            return TRUE;

        default:
            return FALSE;
        }
    }
#if ENABLE_UPLOAD_CHK
    else if (procGSM_UploadChk_Step != UPLOAD_CHK_STEP_NULL)
    {
        switch ((int)procGSM_UploadChk_Step)
        {
        case UPLOAD_CHK_STEP_OPEN_WAIT:
        case UPLOAD_CHK_STEP_SET_DIR_WAIT:
        case UPLOAD_CHK_STEP_GET_SIZE_WAIT:
        case UPLOAD_CHK_STEP_CLOSE_WAIT:
            return TRUE;

        default:
            return FALSE;
        }
    }
#endif
    /*
    #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA == 1
    else if(procXTRA_Download_Step != XTRA_STEP_NULL)
    {
      switch((int)procGSM_Download_Step)
        {
            case XTRA_STEP_HTTP_GET_WAIT:
            case XTRA_STEP_HTTP_READ_WAIT:
                return TRUE;

          default:
                return FALSE;
        }
    }
    #endif
    */
    else if (procGSM_Upload_Step != UPLOAD_STEP_NULL)
    {
        switch ((int)procGSM_Upload_Step)
        {
        case UPLOAD_STEP_OPEN_WAIT:
        case UPLOAD_STEP_SET_DIR_WAIT:
        case UPLOAD_STEP_MAKE_DIR_WAIT:
        case UPLOAD_STEP_GET_SIZE_WAIT:
        case UPLOAD_STEP_FILE_DEL_WAIT:
        case UPLOAD_STEP_UPLOAD_WAIT:
        case UPLOAD_STEP_CHECK_SIZE_WAIT:
        case UPLOAD_STEP_CLOSE_WAIT:
            return TRUE;

        default:
            return FALSE;
        }
    }

    return FALSE;
}

#if ENABLE_SMS_EVENT
//============================================================================//
// @brief  Send SMS response as an EVENT message to server.
// @param  pStr: Pointer to an array storing the string.
//         pcMobileNum: Pointer to an array storing the sender phone number.
// @retval None.
//============================================================================//
static bool GSM_SMS_RespEvent(GSM_MsgDataTypeDef *pSMS_Data)
{
    uint32_t u8TotalSize, u8DatCnt, u8DatSize;
    char cEventMsg[20] = {0};
    char *pcPtr;
    const uint8_t u8DAT_SIZE_MAX = 17;
    uint8_t u8MsgCnt;

    if ((pSMS_Data->u16DataLen == 0) || (pSMS_Data->u16DataLen > 160))
        return FALSE;

    if (pSMS_Data->pcData == NULL)
        return FALSE;

    pcPtr = pSMS_Data->pcData;
    u8TotalSize = pSMS_Data->u16DataLen;
    u8DatCnt = 0;
    u8MsgCnt = 1;

    while (u8DatCnt < u8TotalSize)
    {
        u8DatSize = u8TotalSize - u8DatCnt;

        if (u8DatSize > u8DAT_SIZE_MAX)
            u8DatSize = u8DAT_SIZE_MAX;

        memset(cEventMsg, 0x00, sizeof(cEventMsg));

        snprintf(cEventMsg, u8DatSize + 3, "%d.%s", u8MsgCnt, pcPtr);

        GSM_TCP_ReportEvent((char *)EVENT_SMS_RESPONSE, cEventMsg);

        pcPtr += u8DatSize;
        u8DatCnt += u8DatSize;
        u8MsgCnt++;
    }

    return TRUE;
}
#endif

//============================================================================//
// @brief  Send pending SMS request.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_SMS_SendProc(void)
{
    bool bRes;
    GSM_MsgDataTypeDef tSMS_Data = {0};

    if ((bSD_ReportErrFlag == TRUE) ||
        (bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE) ||
        (bGSM_ResetFlag == TRUE) || (bGSM_ResetCheckFTP_Flag == TRUE) ||
        (GSM_IsBusy() == TRUE))
    {
        return;
    }

    if (tSMS_Index.u16Size == 0)
        return;

    memset(&tSMS_GetPkg, 0x00, sizeof(SMS_PkgTypeDef));

    bRes = GSM_SMS_GetPkg(&tSMS_GetPkg);

    if (bRes == FALSE)
    {
        return;
    }

    tSMS_Data.pcPhoneNum = tSMS_GetPkg.cPhoneNum;
    tSMS_Data.pcData = tSMS_GetPkg.cSMS;
    tSMS_Data.u16DataLen = tSMS_GetPkg.u16Len;

    GSM_SMS_Send(&tSMS_Data);

    // #if ENABLE_SMS_EVENT
    // GSM_SMS_RespEvent(&tSMS_Data);
    // #endif
}

//============================================================================//
// @brief  Process an SMS, not using GSM command request.
// @param  pStr: Pointer to an array storing the string.
//         pcMobileNum: Pointer to an array storing the sender phone number.
// @retval None.
//============================================================================//
static void GSM_SMS_Proc(char *pStr, char *pcMobileNum)
{
    char cMsgCmdBuf[128] = {0};
    char cCmdStr[32] = {0};
    char cCmdArg[96] = {0};
    char cCmdMsg[128] = {0};
    GSM_MsgDataTypeDef smsHandle = {0};
    bool bPreResp = FALSE;

    StopChar(pStr, '@');

    if (strncmp(pStr, STR_PASS_ADMIN, strlen(STR_PASS_ADMIN)) != 0)
    {
        if (strncmp(pStr, STR_PASS_USER, strlen(STR_PASS_USER)) != 0)
        {
            return;
        }
    }

    pStr += strlen(pStr) + 1;

    StopChar(pStr, '\n');

    StopChar(pStr, '\r');

    StopChar(pStr, ':');

    memset(cCmdStr, 0x00, sizeof(cCmdStr));

    GSM_GetMsgCmd(pStr, cCmdStr);

    if (strlen(cCmdStr) == 0)
    {
        GSMprintf("\r\n> GSM fail to get SMS command.");

        sprintf(cMsgCmdBuf, "Warning: Command error!");

        smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cMsgCmdBuf;

        smsHandle.u16DataLen = strlen(cMsgCmdBuf);

        if (g_Setting.bIOT_SIM_En == FALSE)
            // GSM_SMS_Send(&smsHandle);
            // QtEC21_SMS_Send(&smsHandle);
            GSM_SMS_SavePkg(&smsHandle);

#if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
#endif

        HAL_Delay(S_2_mS(1));

        return;
    }

    // GSMprintf("\r\n> SMS command:%s", cCmdStr);

    pStr += strlen(pStr) + 1;

    if (strlen(pStr) == 0)
    {
        strcpy(cCmdArg, "?");

        GSMprintf("\r\n> GSM SMS command:%s%s", cCmdStr, cCmdArg);
    }
    else
    {
        if (strlen(pStr) > sizeof(cCmdArg))
        {
            GSMprintf("\r\n> GSM SMS command:%s. Command argument is too long: %s", cCmdStr, pStr);

            return;
        }

        strcpy(cCmdArg, pStr);

        GSMprintf("\r\n> GSM SMS command:%s%s", cCmdStr, cCmdArg);

        if (strcmp(cCmdStr, STR_AS_RESET) == 0)
        {
            if (strncmp(cCmdArg, szResetCmd[RST_UPGRADE], strlen(szResetCmd[RST_UPGRADE])) == 0)
            {
                sprintf(cMsgCmdBuf, "Warning: Command cancel!!");

                smsHandle.pcPhoneNum = pcMobileNum;

                smsHandle.pcData = cMsgCmdBuf;

                smsHandle.u16DataLen = strlen(cMsgCmdBuf);

                if (g_Setting.bIOT_SIM_En == FALSE)
                    // GSM_SMS_Send(&smsHandle);
                    // QtEC21_SMS_Send(&smsHandle);
                    GSM_SMS_SavePkg(&smsHandle);

#if ENABLE_SMS_EVENT
                GSM_SMS_RespEvent(&smsHandle);
#endif

                HAL_Delay(S_2_mS(1));
            }
        }
    }

    sprintf(cMsgCmdBuf, "%s%s\r\n", cCmdStr, cCmdArg);

    if (Protocl_IsRstGSM_Cmd(cCmdStr, cCmdArg) == TRUE)
        bPreResp = TRUE;

    if (bPreResp == TRUE)
    {
        char cCmdStr_Short[32] = {0};
        char *pcCmd;

        strcpy(cCmdStr_Short, cCmdStr);

        pcCmd = cCmdStr_Short;

        StopChar(pcCmd, '$');

        pcCmd += strlen(pcCmd) + 1;

        StopChar(pcCmd, '\n');

        StopChar(pcCmd, '\r');

        StopChar(pcCmd, '=');

        sprintf(cCmdMsg, "%s received.", pcCmd);

        smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cCmdMsg;

        smsHandle.u16DataLen = strlen(cCmdMsg);

        osStatus osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> GSM couldn't acquire GSM RX mutex to send SMS. osRes:%d", osRes);
        }
        else
        {
            if (g_Setting.bIOT_SIM_En == FALSE)
                // GSM_SMS_Send(&smsHandle);
                // QtEC21_SMS_Send2(&smsHandle);
                GSM_SMS_SavePkg(&smsHandle);

            osMutexRelease(mutex_gsm_rx_user);
        }

#if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
#endif
    }

    if (Protocl_ChkCmdPkg(cMsgCmdBuf, cCmdMsg) == TRUE)
    {
        if (bPreResp == FALSE)
        {
            char *pcCmd;

            if (strlen(cCmdMsg) > 0)
            {
                pcCmd = cCmdMsg;

                StopChar(pcCmd, '$');

                pcCmd += strlen(pcCmd) + 1;

                ChangeChar(pcCmd, '\n', ' ');

                ChangeChar(pcCmd, '\r', ' ');

                sprintf(cMsgCmdBuf, "%s", pcCmd);
            }
            else
            {
                pcCmd = cCmdStr;

                StopChar(pcCmd, '$');

                pcCmd += strlen(pcCmd) + 1;

                StopChar(pcCmd, '=');

                sprintf(cMsgCmdBuf, "%s success.", pcCmd);
            }

            smsHandle.pcPhoneNum = pcMobileNum;

            smsHandle.pcData = cMsgCmdBuf;

            smsHandle.u16DataLen = strlen(cMsgCmdBuf);

            if (g_Setting.bIOT_SIM_En == FALSE)
                // GSM_SMS_Send(&smsHandle);
                // QtEC21_SMS_Send(&smsHandle);
                GSM_SMS_SavePkg(&smsHandle);

#if ENABLE_SMS_EVENT
            GSM_SMS_RespEvent(&smsHandle);
#endif

            HAL_Delay(S_2_mS(1));
        }
    }
    else
    {
        GSMprintf("\r\n> GSM fail to process SMS command.");

        sprintf(cMsgCmdBuf, "Warning: Command error!");

        smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cMsgCmdBuf;

        smsHandle.u16DataLen = strlen(cMsgCmdBuf);

        if (g_Setting.bIOT_SIM_En == FALSE)
            // GSM_SMS_Send(&smsHandle);
            // QtEC21_SMS_Send(&smsHandle);
            GSM_SMS_SavePkg(&smsHandle);

#if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
#endif

        HAL_Delay(S_2_mS(1));
    }
}

//============================================================================//
// @brief  Process an SMS, not using GSM command request.
// @param  pStr: Pointer to an array storing the string.
//         pcMobileNum: Pointer to an array storing the sender phone number.
// @retval None.
//============================================================================//
/*
static void GSM_SMS_Proc2(char *pStr, char *pcMobileNum)
{
    char cMsgCmdBuf[128] = {0};
    char cCmdStr[32] = {0};
    char cCmdArg[96] = {0};
    char cCmdMsg[128] = {0};
    GSM_MsgDataTypeDef smsHandle;
    bool bPreResp = FALSE;


    StopChar(pStr, '@');

    if(strncmp(pStr, STR_PASS_ADMIN, strlen(STR_PASS_ADMIN)) != 0)
    {
        if(strncmp(pStr, STR_PASS_USER, strlen(STR_PASS_USER)) != 0)
        {
            return;
        }
    }

    pStr += strlen(pStr) + 1;

    StopChar(pStr, '\n');

    StopChar(pStr, '\r');

    StopChar(pStr, ':');

    memset(cCmdStr, 0x00, sizeof(cCmdStr));

    GSM_GetMsgCmd(pStr, cCmdStr);

    if(strlen(cCmdStr) == 0)
    {
        GSMprintf("\r\n> GSM fail to get SMS command.");

        sprintf(cMsgCmdBuf, "Warning: Command error!");

        smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cMsgCmdBuf;

        smsHandle.u16DataLen = strlen(cMsgCmdBuf);

        if(g_Setting.bIOT_SIM_En == FALSE)
            //GSM_SMS_Send(&smsHandle);
            QtEC21_SMS_Send2(&smsHandle);

        #if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
        #endif

        return;
    }

    //GSMprintf("\r\n> SMS command:%s", cCmdStr);

    pStr += strlen(pStr) + 1;

    if(strlen(pStr) == 0)
    {
        strcpy(cCmdArg, "?");

        GSMprintf("\r\n> GSM SMS command:%s%s", cCmdStr, cCmdArg);
    }
    else
    {
        if(strlen(pStr) > sizeof(cCmdArg))
        {
            GSMprintf("\r\n> GSM SMS command:%s. Command argument is too long: %s", cCmdStr, pStr);

            return;
    }

        strcpy(cCmdArg, pStr);

        GSMprintf("\r\n> GSM SMS command:%s%s", cCmdStr, cCmdArg);

        if(strcmp(cCmdStr, STR_AS_RESET) == 0)
        {
            if(strncmp(cCmdArg, szResetCmd[RST_UPGRADE], strlen(szResetCmd[RST_UPGRADE])) == 0)
            {
                sprintf(cMsgCmdBuf, "Warning: Command cancel!!");

                smsHandle.pcPhoneNum = pcMobileNum;

                smsHandle.pcData = cMsgCmdBuf;

                smsHandle.u16DataLen = strlen(cMsgCmdBuf);

                if(g_Setting.bIOT_SIM_En == FALSE)
                    //GSM_SMS_Send(&smsHandle);
                    QtEC21_SMS_Send2(&smsHandle);

                #if ENABLE_SMS_EVENT
                GSM_SMS_RespEvent(&smsHandle);
                #endif
            }
        }
    }

    sprintf(cMsgCmdBuf, "%s%s\r\n", cCmdStr, cCmdArg);

    if(Protocl_IsRstGSM_Cmd(cCmdStr, cCmdArg) == TRUE)
        bPreResp = TRUE;

    if(bPreResp == TRUE)
    {
        char cCmdStr_Short[32] = {0};
        char *pcCmd;


        strcpy(cCmdStr_Short, cCmdStr);

        pcCmd = cCmdStr_Short;

        StopChar(pcCmd, '$');

        pcCmd += strlen(pcCmd) + 1;

      StopChar(pcCmd, '\n');

        StopChar(pcCmd, '\r');

        StopChar(pcCmd, '=');

        sprintf(cCmdMsg, "%s received.", pcCmd);

      smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cCmdMsg;

        smsHandle.u16DataLen = strlen(cCmdMsg);

    if(g_Setting.bIOT_SIM_En == FALSE)
            //GSM_SMS_Send(&smsHandle);
            QtEC21_SMS_Send2(&smsHandle);

        #if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
        #endif
    }

    if(Protocl_ChkCmdPkg(cMsgCmdBuf, cCmdMsg) == TRUE)
    {
        if(bPreResp == FALSE)
        {
          char *pcCmd;


            if(strlen(cCmdMsg) > 0)
            {
                pcCmd = cCmdMsg;

                StopChar(pcCmd, '$');

                pcCmd += strlen(pcCmd) + 1;

                ChangeChar(pcCmd, '\n', ' ');

            ChangeChar(pcCmd, '\r', ' ');

                sprintf(cMsgCmdBuf, "%s", pcCmd);
            }
            else
            {
                pcCmd = cCmdStr;

                StopChar(pcCmd, '$');

                pcCmd += strlen(pcCmd) + 1;

                StopChar(pcCmd, '=');

                sprintf(cMsgCmdBuf, "%s success.", pcCmd);
            }

            smsHandle.pcPhoneNum = pcMobileNum;

            smsHandle.pcData = cMsgCmdBuf;

            smsHandle.u16DataLen = strlen(cMsgCmdBuf);

            if(g_Setting.bIOT_SIM_En == FALSE)
                //GSM_SMS_Send(&smsHandle);
                QtEC21_SMS_Send2(&smsHandle);

            #if ENABLE_SMS_EVENT
            GSM_SMS_RespEvent(&smsHandle);
            #endif
        }
    }
    else
    {
        GSMprintf("\r\n> GSM fail to process SMS command.");

        sprintf(cMsgCmdBuf, "Warning: Command error!");

        smsHandle.pcPhoneNum = pcMobileNum;

        smsHandle.pcData = cMsgCmdBuf;

        smsHandle.u16DataLen = strlen(cMsgCmdBuf);

        if(g_Setting.bIOT_SIM_En == FALSE)
            //GSM_SMS_Send(&smsHandle);
            QtEC21_SMS_Send2(&smsHandle);

        #if ENABLE_SMS_EVENT
        GSM_SMS_RespEvent(&smsHandle);
        #endif
    }
}
*/

//============================================================================//
// @brief  Check unread SMS, using GSM command request.
// @param  None.
// @retval None.
//============================================================================//
void GSM_SMS_CheckUnread(void)
{
    char cSignal1[] = "+CMGL:";
    char cSignal2[] = "REC UNREAD";
    char *pcPtr;
    uint16_t u16FindPos;
    uint16_t u16MsgIdx, u16MsgLen;
    char cPhoneNbr[15] = {0};
    char cRecvTime[20] = {0};
    char cSMS_Content[256] = {0};
    char cSMS_Content2[128] = {0};
    char *pcStartPoint;
    uint32_t u32Offset = 0;
    bool bRes;

    memset(u8SMS_List, 0x00, sizeof(u8SMS_List));

    pcPtr = (char *)u8SMS_List;

    pcStartPoint = pcPtr;

    bRes = GSM_SMS_GetList(cSignal2, (uint8_t *)pcPtr);
    // bRes = QtEC21_SMS_GetList(cSignal2, (uint8_t *)pcPtr);

    if (!bRes)
    {
        GSMprintf("\r\n> GSM get SMS list failed.");

        GSM_SMS_Del(0, 4);
        // QtEC21_SMS_Del(0, 4);

        return;
    }

    while ((FindSequence(pcPtr, cSignal1, sizeof(u8SMS_List) - u32Offset, 6, &u16FindPos)) &&
           (bQtEC21_Ready == TRUE))
    {
        memset(cPhoneNbr, 0x00, sizeof(cPhoneNbr));
        memset(cRecvTime, 0x00, sizeof(cRecvTime));
        memset(cSMS_Content, 0x00, sizeof(cSMS_Content));

        pcPtr += u16FindPos + 7;

        StopChar(pcPtr, ',');

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) > 0)
        {
            u16MsgIdx = atoi(pcPtr);
        }
        else
        {
            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, '\"');

        // Check stat
        if (strcmp(pcPtr, cSignal2) != 0)
        {
            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        // Get phone number
        pcPtr += strlen(pcPtr) + 3;

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) >= 9)
            strcpy(cPhoneNbr, pcPtr);
        else
        {
            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        // Get receiving time
        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        // Store receiving time
        if (strlen(pcPtr) >= 19)
        {
            strncpy(cRecvTime, pcPtr, 17);
        }
        else
        {
            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        // Get message length
        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, ',');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\r');

        // Save message length
        if (strlen(pcPtr) > 0)
        {
            u16MsgLen = atoi(pcPtr);
        }
        else
        {
            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        if (u16MsgLen >= 256)
        {
            GSMprintf("\r\n> GSM SMS length %d is unaccepted.", u16MsgLen);

            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        // Get message content
        pcPtr += strlen(pcPtr) + 2;

        // Save message content
        strncpy(cSMS_Content, pcPtr, u16MsgLen);

        cSMS_Content[u16MsgLen] = 0;

        GSMprintf("\r\n> GSM SMS Get Unread - Index: %d - Length:%d", u16MsgIdx, u16MsgLen);

        // GSMprintf("\r\n> SMS content:\r\n%s\r\nSMS content end.", cSMS_Content);

        if (strncmp(cSMS_Content, (char *)"1111", 4) != 0)
        {
            GSMprintf("\r\n> GSM SMS command incorrect format!");

            GSM_SMS_Del(u16MsgIdx, 0);
            // QtEC21_SMS_Del(u16MsgIdx, 0);

            u32Offset = pcPtr - pcStartPoint;

            continue;
        }

        // Check character set
        if (cSMS_Content[4] != '@')
        {
            if (tQtEC21_CharSet == CHAR_SET_IRA)
            {
                GSMprintf("\r\n> GSM incorrect character set. Change to GSM");

                GSM_TECharSet("GSM");

                tQtEC21_CharSet = CHAR_SET_GSM;
            }
            else
            {
                GSMprintf("\r\n> GSM incorrect character set. Change to IRA");

                GSM_TECharSet("IRA");

                tQtEC21_CharSet = CHAR_SET_IRA;
            }

            bRes = GSM_SMS_Read(u16MsgIdx, (uint8_t *)cSMS_Content);

            if (bRes)
            {
                memset(cPhoneNbr, 0x00, sizeof(cPhoneNbr));
                memset(cRecvTime, 0x00, sizeof(cRecvTime));
                memset(cSMS_Content2, 0x00, sizeof(cSMS_Content2));

                // GSMprintf("\r\n> SMS content:\r\n%s\r\nSMS content end.", u8GSM_RX_Buf);

                if (strlen((char *)cSMS_Content) < 256)
                {
                    bRes = QtEC21_SMS_Parse(cSMS_Content, cPhoneNbr, cRecvTime, cSMS_Content2);

                    if (bRes)
                    {
                        GSMprintf("\r\n> GSM SMS sender:%s", cPhoneNbr);

                        GSMprintf("\r\n> GSM SMS receiving time:%s", cRecvTime);

                        GSMprintf("\r\n> GSM SMS content:\n%s\nSMS end.", cSMS_Content2);

                        GSM_SMS_Del(u16MsgIdx, 0);
                        // QtEC21_SMS_Del(u16SMS_ID, 0);

                        memset(cSMS_Content, 0x00, sizeof(cSMS_Content));

                        sprintf(cSMS_Content, "%s", cSMS_Content2);
                    }
                    else
                    {
                        GSMprintf("\r\n> GSM SMS parse fail.");

                        GSM_SMS_Del(u16MsgIdx, 0);
                        // QtEC21_SMS_Del(u16SMS_ID, 0);

                        u32Offset = pcPtr - pcStartPoint;

                        continue;
                    }
                }
                else
                {
                    GSMprintf("\r\n> GSM SMS length %d is unaccepted.", strlen((char *)u8GSM_RX_Buf));

                    GSM_SMS_Del(u16MsgIdx, 0);
                    // QtEC21_SMS_Del(u16SMS_ID, 0);

                    u32Offset = pcPtr - pcStartPoint;

                    continue;
                }
            }
            else
            {
                GSM_SMS_Del(u16MsgIdx, 0);
                // QtEC21_SMS_Del(u16SMS_ID, 0);

                u32Offset = pcPtr - pcStartPoint;

                continue;
            }
        }

        gsmCmdSrc = GSM_CMD_SRC_SMS;

        if ((bSystemMode == SYSTEM_MODE_STEALTH) && (bWaitingEnterStandby == TRUE))
        {
            RTOSprintf("\r\n> GSM SMS refresh enter standby timer.");

            // osTimerStop(timerACC_OffHandle);

            // osTimerStart(timerACC_OffHandle, S_2_mS(30));

            u32PwrDownDelayTick = HAL_GetTick();
        }

        sprintf(cCmdPhoneNum, "%s", cPhoneNbr);

        GSM_SMS_Proc(cSMS_Content, cPhoneNbr);

        // if(g_Setting.tUpgrade.bFOTA_Flag == TRUE)
        //{
        //	sprintf(cCmdPhoneNum, "%s", cPhoneNbr);
        // }

        memset(cCmdPhoneNum, 0x00, sizeof(cCmdPhoneNum));

        u32Offset = pcPtr - pcStartPoint;

        GSM_SMS_Del(u16MsgIdx, 0);
        // QtEC21_SMS_Del(u16MsgIdx, 0);
    }
}

//============================================================================//
// @brief  Process a TCP/IP message.
// @param  pcTCP_Msg: Pointer to a char array storing the TCP/IP message.
//         u8SrcID: ID of the connection socket where the message came from.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
static void GSM_TCP_MsgProc(char *pcTCP_Msg, uint8_t u8SrcID)
{
    GSM_SocketDataTypeDef gsmTCP_Handle;
    char cCmdHeader[] = "$AS";
    //     cCmdEnd[] = "\r\n";
    char cCmd[128] = {0};
    char cCmdStr[64] = {0};
    char cRetMsg[200] = {0};
    char *pcRespPtr;
    bool bRes;
    bool bIsPreResp = FALSE;
    uint8_t u8CmdHeaderLen = 3;
    uint16_t u16FindPos1, u16FindPos2;

    // If found a command in the data buffer
    while (FindSequence(pcTCP_Msg,
                        cCmdHeader,
                        strlen(pcTCP_Msg),
                        u8CmdHeaderLen,
                        &u16FindPos1) == TRUE)
    {
        memset(cCmd, 0x00, sizeof(cCmd));

        // If found no other command
        if (FindSequence(pcTCP_Msg + u16FindPos1 + u8CmdHeaderLen,
                         cCmdHeader,
                         strlen(pcTCP_Msg) - u16FindPos1 - u8CmdHeaderLen,
                         u8CmdHeaderLen,
                         &u16FindPos2) == FALSE)
        {
            if (strlen(pcTCP_Msg) > sizeof(cCmd))
            {
                memset(cRetMsg, 0x00, sizeof(cRetMsg));

                sprintf(cRetMsg, "@ TCP/IP command is too long!");

                gsmTCP_Handle.SocketNum = u8SrcID;
                gsmTCP_Handle.pcData = cRetMsg;
                gsmTCP_Handle.u16DataLen = strlen(cRetMsg);

                GSM_TCP_Send(&gsmTCP_Handle);
                // QtEC21_TCP_Send(&gsmTCP_Handle);

                HAL_Delay(S_2_mS(1));

                return;
            }

            strcpy(cCmd, pcTCP_Msg + u16FindPos1);

            pcTCP_Msg += u8CmdHeaderLen;
        }
        // If found at least another command
        else
        {
            if ((u16FindPos2 + u8CmdHeaderLen - u16FindPos1) > sizeof(cCmd))
            {
                memset(cRetMsg, 0x00, sizeof(cRetMsg));

                sprintf(cRetMsg, "@ TCP/IP command is too long!");

                gsmTCP_Handle.SocketNum = u8SrcID;
                gsmTCP_Handle.pcData = cRetMsg;
                gsmTCP_Handle.u16DataLen = strlen(cRetMsg);

                GSM_TCP_Send(&gsmTCP_Handle);
                // QtEC21_TCP_Send(&gsmTCP_Handle);

                HAL_Delay(S_2_mS(1));

                continue;
            }

            strncpy(cCmd, (pcTCP_Msg + u16FindPos1), (u16FindPos2 + u8CmdHeaderLen));

            pcTCP_Msg += u16FindPos1 + u8CmdHeaderLen + u16FindPos2;
        }

        sprintf(cCmd + strlen(cCmd), "\r\n");

        GSMprintf("\r\n> GSM process TCP/IP command:\r\n%s\r\nTCP/IP command end.", cCmd);

        memset(cRetMsg, 0x00, sizeof(cRetMsg));

        strcpy(cRetMsg, cCmd);

        pcRespPtr = cRetMsg;

        StopChar(pcRespPtr, '=');

        pcRespPtr += strlen(pcRespPtr) + 1;

        if (Protocl_IsRstGSM_Cmd(cCmd, pcRespPtr) == TRUE)
        {
            bIsPreResp = TRUE;
        }

        if (bIsPreResp == TRUE)
        {
            strcpy(cCmdStr, cRetMsg);

            pcRespPtr = cCmdStr;

            StopChar(pcRespPtr, '$');

            pcRespPtr += strlen(pcRespPtr) + 1;

            memset(cRetMsg, 0x00, sizeof(cRetMsg));

            sprintf(cRetMsg, "%s received.", pcRespPtr);

            gsmTCP_Handle.SocketNum = u8SrcID;
            gsmTCP_Handle.pcData = cRetMsg;
            gsmTCP_Handle.u16DataLen = strlen(cRetMsg);

            GSM_TCP_Send(&gsmTCP_Handle);
            // QtEC21_TCP_Send(&gsmTCP_Handle);

            HAL_Delay(S_2_mS(1));
        }

        if ((strncmp(cCmd, STR_AS_SMS_LIST, strlen(STR_AS_SMS_LIST)) == 0) ||
            (strncmp(cCmd, STR_AS_FIL_LIST, strlen(STR_AS_FIL_LIST)) == 0) ||
            (strncmp(cCmd, STR_AS_DEBUG_MODE, strlen(STR_AS_DEBUG_MODE)) == 0))
        {
            bRes = FALSE;
        }
        else
        {
            bRes = Protocl_ChkCmdPkg(cCmd, cRetMsg);
        }

        if (bRes == TRUE)
        {
            if (bIsPreResp == FALSE)
            {
                GSMprintf("\r\n@ Process TCP/IP command OK.");

                if (strlen(cRetMsg) == 0)
                {
                    strcpy(cCmdStr, cCmd);

                    pcRespPtr = cCmdStr;

                    StopChar(pcRespPtr, '$');

                    pcRespPtr += strlen(pcRespPtr) + 1;

                    // ChangeChar(pcRespPtr, '\n', ' ');

                    // ChangeChar(pcRespPtr, '\r', ' ');

                    StopChar(pcRespPtr, '=');

                    memset(cRetMsg, 0x00, sizeof(cRetMsg));

                    sprintf(cRetMsg, "%s success.", pcRespPtr);
                }

                gsmTCP_Handle.SocketNum = u8SrcID;
                gsmTCP_Handle.pcData = cRetMsg;
                gsmTCP_Handle.u16DataLen = strlen(cRetMsg);

                GSM_TCP_Send(&gsmTCP_Handle);
                // QtEC21_TCP_Send(&gsmTCP_Handle);

                HAL_Delay(S_2_mS(1));
            }
        }
        else
        {
            GSMprintf("\r\n@ Process TCP command ERROR.");

            memset(cRetMsg, '\0', sizeof(cRetMsg));

            sprintf(cRetMsg, "@ Warning: TCP/IP command error!");

            gsmTCP_Handle.SocketNum = u8SrcID;
            gsmTCP_Handle.pcData = cRetMsg;
            gsmTCP_Handle.u16DataLen = strlen(cRetMsg);

            GSM_TCP_Send(&gsmTCP_Handle);
            // QtEC21_TCP_Send(&gsmTCP_Handle);

            HAL_Delay(S_2_mS(1));
        }
    }
}

//============================================================================//
// @brief  Process to setup QtEC21 module using commands.
// @param  None.
// @retval None
//============================================================================//
static void GSM_SetupProc(void)
{
    bool bRes = TRUE;
    static uint8_t u8GSM_SetupErrCnt = 0;

    if ((procGSM_SetupStep == PROC_STEP_NULL) ||
        (bGSM_PwrOffFlag == TRUE) ||
        (bGSM_ResetFlag == TRUE) ||
        (bGSM_ResetCheckFTP_Flag == TRUE))
        return;

    if (bQtEC21_Ready == FALSE)
    {
        procGSM_SetupStep = PROC_STEP_NULL;

        return;
    }

    switch ((int)procGSM_SetupStep)
    {
    // Step 1:
    case PROC_STEP_1:
    {
        char cRespBuf[64] = {0};

        for (int i = 0; i < 3; i++)
        {
            bRes = GSM_ConfigEcho(0);
            // QtEC21_ConfigEcho(0);

            if (bRes == TRUE)
                break;

            HAL_Delay(100);
        }

        if (bRes == TRUE)
        {
            if (g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt > 0)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt = 0;

                System_SaveSetting();
            }
        }
        else
        {
            if (bQtEC21_Ready == TRUE)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt++;

                if (g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt >= 3)
                {
                    g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt = 0;

                    bQtEC21_State = FALSE;

                    RTOSprintf("\r\n> GSM function ERROR! Maximum GSM module reset reached. Stop GSM functions!");

                    LCD_SetShowAutoTestPage(52);

                    char cLog[20] = {0};

                    sprintf(cLog, "ATE ERROR");

                    GPRS_DebugLog(cLog, strlen(cLog));
                }
                else
                {
                    RTOSprintf("\r\n> GSM function ERROR! Reset GSM module.");

                    HAL_Delay(S_2_mS(1));

                    // GSM_Reset();
                    bGSM_ResetFlag = TRUE;
                }
            }

            System_SaveSetting();

            procGSM_SetupStep = PROC_STEP_NULL;

            return;
        }

        bRes = GSM_ReadCGMM(cRespBuf);

        if (bRes)
        {
            if (strstr(cRespBuf, (char *)cGSM_ModelString[GSM_MODULE_EC200]) != NULL)
            {
                tGSM_ModuleType = GSM_MODULE_EC200;
            }
            else if (strstr(cRespBuf, (char *)cGSM_ModelString[GSM_MODULE_EC25]) != NULL)
            {
                tGSM_ModuleType = GSM_MODULE_EC25;
            }
            else
            {
                tGSM_ModuleType = GSM_MODULE_EC21;
            }
        }
        else
        {
            tGSM_ModuleType = GSM_MODULE_EC21;
        }

        GSMprintf("\r\n> ### GSM MODULE MODEL: %s ###", cGSM_ModelString[tGSM_ModuleType]);

        // memset(cRespBuf, 0x00, sizeof(cRespBuf));

        // GSM_Net_GetTime(0, cRespBuf);

        // GSMprintf("\r\n> GSM synchronized time: %s", cRespBuf);

        // memset(cRespBuf, 0x00, sizeof(cRespBuf));

        // GSM_HW_GetRTC(cRespBuf);

        // GSMprintf("\r\n> GSM RTC time: %s", cRespBuf);

        break;
    }

    case PROC_STEP_2:
    {
        GSM_SIM_StatTypedef simErrCode;

        bRes = GSM_SIM_QueryUSIM(&simErrCode);
        // bRes = QtEC21_SIM_QueryUSIM();

        if (bRes == TRUE)
        {
            if (g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt > 0)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;

                System_SaveSetting();
            }
        }
        else
        {
            if (bQtEC21_Ready == TRUE)
            {
                // If the current version is JAS208U then change it to JAS208S
                // if(strncmp(g_Setting.cJAS_Ver, (char *)JAS208_GSM, 7) == 0)
                //{
                //	GSMprintf("\r\n> SIM card is not detected, change to version JAS208S.");

                //	memset(g_Setting.cJAS_Ver, 0x00, sizeof(g_Setting.cJAS_Ver));

                //	sprintf(g_Setting.cJAS_Ver, "%s", JAS208_NO_GSM);

                //	System_SaveSetting();
                //}

                // HAL_Delay(S_2_mS(10));

                // GSM_Reset();
                // bGSM_ResetFlag = TRUE;

                GSMprintf("\r\n> GSM query USIM ERROR %d", simErrCode);

                g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt++;

                // GSMprintf("\r\n> GSM SIM Error: %d", g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt);

                if (g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt >= GSM_SIM_QUERY_MAX)
                {
                    g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;

                    bQtEC21_State = FALSE;

                    RTOSprintf("\r\n> GSM SIM ERROR! Maximum recheck reached! Stop GSM functions!");

                    LCD_SetShowAutoTestPage(51);

                    char cLog[20] = {0};

                    sprintf(cLog, "SIM ERROR(1)");

                    GPRS_DebugLog(cLog, strlen(cLog));
                }
                else
                {
                    if ((bQtEC21_Ready == TRUE) &&
                        (bGSM_ResetFlag == FALSE) &&
                        (bGSM_ResetCheckFTP_Flag == FALSE))
                    {
                        RTOSprintf("\r\n> GSM SIM ERROR! Reset GSM module module.");

                        HAL_Delay(S_2_mS(1));

                        // GSM_Reset();
                        bGSM_ResetFlag = TRUE;
                    }
                }
            }

            System_SaveSetting();

            procGSM_SetupStep = PROC_STEP_NULL;

            return;
        }

        break;
    }

    case PROC_STEP_3:
    {
        char cIMEI[16] = {0};

        bRes = GSM_GetIMEI(cIMEI);
        // bRes = QtEC21_GetIMEI(cIMEI);

        if (bRes)
        {
            if (strncmp(cIMEI, g_Setting.tGSM_SET.tMobileInfo.cIMEI, 15) != 0)
            {
                memset(g_Setting.tGSM_SET.tMobileInfo.cIMEI, 0x00, sizeof(g_Setting.tGSM_SET.tMobileInfo.cIMEI));

                strncpy(g_Setting.tGSM_SET.tMobileInfo.cIMEI, cIMEI, 15);

                System_SaveSetting();
            }

            if (strcmp(g_Setting.tBusInfo.cBus_ID, DEFAULT_STRING) == 0)
            {
                memset(g_Setting.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));

                memcpy(g_Setting.tBusInfo.cBus_ID, &g_Setting.tGSM_SET.tMobileInfo.cIMEI[5], sizeof(g_Setting.tBusInfo.cBus_ID));

                System_SaveSetting();

                if (bSD_ReportErrFlag == FALSE)
                    V161_RecordTechnical(1);

                RTOSprintf("\r\n> ### BUS ID is default string, set BUS ID to IMEI ###");
            }

            bIsIMEIReady = TRUE;
        }

        break;
    }

    case PROC_STEP_4:
    {
        char cIMSI[16] = {0};

        bRes = GSM_GetIMSI(cIMSI);
        // bRes = QtEC21_GetIMSI(cIMSI);

        if (bRes)
        {
            if (strncmp(cIMSI, g_Setting.tGSM_SET.tMobileInfo.cIMSI, 15) != 0)
            {
                memset(g_Setting.tGSM_SET.tMobileInfo.cIMSI, 0x00, sizeof(g_Setting.tGSM_SET.tMobileInfo.cIMSI));

                strncpy(g_Setting.tGSM_SET.tMobileInfo.cIMSI, cIMSI, 15);

                System_SaveSetting();
            }
        }

        break;
    }

    case PROC_STEP_5:
        if (tGSM_ModuleType == GSM_MODULE_EC200)
        {
            GSM_TECharSet("GSM");

            tQtEC21_CharSet = CHAR_SET_GSM;
        }
        else
        {
            bRes = GSM_TECharSet("IRA");

            tQtEC21_CharSet = CHAR_SET_IRA;
        }

        break;

    case PROC_STEP_6:
        bRes = GSM_SMS_SetFormat(1);
        // bRes = QtEC21_SMS_SetFormat(1);

        break;

    case PROC_STEP_7:
        GSM_SMS_SetStorage((char *)"SM", (char *)"SM", (char *)"SM");
        // QtEC21_SMS_SetStorage((char *)"SM", (char *)"SM", (char *)"SM");

        break;

    case PROC_STEP_8:
        bRes = GSM_SMS_ReportConfig(2, 1, 0, 0, 0);
        // bRes = QtEC21_SMS_ReportConfig(2, 1, 0, 0, 0);

        break;

    case PROC_STEP_9:
        bRes = GSM_SMS_SetParam(1);
        // bRes = QtEC21_SMS_SetParam(1);

        break;

    case PROC_STEP_10:
        bRes = GSM_Stat_ConfigRI((char *)"ring\",\"off\",1,1");
        // bRes = GSM_Stat_ConfigRI((char *)"ring\",\"off\",1,1,1,\"off\",1");
        // bRes = GSM_Stat_ConfigRI((char *)"ring\",\"pulse\",120,1000,5000,\"off\",1");

        break;

    case PROC_STEP_11:
        // bRes = GSM_Stat_ConfigRI((char *)"smsincoming\",\"off\",1");
        bRes = GSM_Stat_ConfigRI((char *)"smsincoming\",\"pulse\",120,1");

        break;

    case PROC_STEP_12:
        bRes = GSM_Stat_ConfigRI((char *)"other\",\"off\",1,1");
        // bRes = GSM_Stat_ConfigRI((char *)"other\",\"pulse\",120,1");

        break;

    case PROC_STEP_13:
        bRes = GSM_COM_SetBaudRate(GSM_BAUDRATE_APPLY);
        // bRes = QtEC21_COM_SetBaudRate(GSM_BAUDRATE_APPLY);

        if (bRes)
        {
            UART_ReInit(GSM_COMPORT_DEF, GSM_BAUDRATE_APPLY);

            // Enable only RXNE and ERR interrupt
            __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_RXNE);
            __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_ERR);
        }

        break;

    case PROC_STEP_14:
        bRes = GSM_URC_Outport((char *)"uart1");
        // bRes = QtEC21_URC_Outport((char *)"uart1");

        break;

    case PROC_STEP_15:
        GSM_SMS_CheckUnread();

        break;

    case PROC_STEP_16:
        GSM_SMS_Del(0, 4);
        // QtEC21_SMS_Del(0,4);

        break;

    case PROC_STEP_17:
#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
        HAL_Delay(10);

        GPS_Start();

#if GNSS_ENABLE_GET_LOC == FALSE
        HAL_Delay(10);

        GPS_EnableRecvMsg();

        GPS_EnableSendMsg();

        HAL_Delay(10);
#endif
#endif

        break;

    case PROC_STEP_18:
#if HW_TYPE == HW_JAS208
#if ENABLE_MG82
        bRes = GSM_HW_SleepEn(1);
#else
        bRes = GSM_HW_SleepEn(0);
#endif
#elif HW_TYPE == HW_JAS306SD || HW_TYPE == HW_JAS411
        bRes = GSM_HW_SleepEn(1);
#endif
        // bRes = QtEC21_HW_SleepEn(1);
        bRes = TRUE;

        break;

    case PROC_STEP_19:
        // #if ENABLE_AUDIO
        // GSM_AudioInit();
        // #endif

        break;

    case PROC_STEP_20:
    {
        char cLog[20] = {0};

        sprintf(cLog, "GSM Function ON");

        GPRS_DebugLog(cLog, strlen(cLog));

        bQtEC21_InitFinish = TRUE;

        RTOSprintf("\r\n> ### GSM FUNCTION ON ###");

        break;
    }

    default:
        break;
    }

    if (bRes == TRUE)
    {
        if (procGSM_SetupStep >= 20)
        {
            procGSM_SetupStep = PROC_STEP_NULL;
        }
        else
        {
            procGSM_SetupStep++;

            HAL_Delay(10);
        }
    }
    else
    {
        if (bQtEC21_Ready == TRUE)
        {
            u8GSM_SetupErrCnt++;

            if (u8GSM_SetupErrCnt >= 2)
            {
                u8GSM_SetupErrCnt = 0;

                if (procGSM_SetupStep >= 20)
                {
                    procGSM_SetupStep = PROC_STEP_NULL;
                }
                else
                {
                    procGSM_SetupStep++;
                }
            }
        }
        else
        {
            procGSM_SetupStep = PROC_STEP_NULL;
        }
    }

    HAL_Delay(10);
}

//============================================================================//
// @brief  Handle the data received from the GSM module.
// @param  None.
// @retval None
//============================================================================//
void GSM_RX_Proc(void)
{
    GSM_EventTypeDef gsmEvent;
    osStatus osRes;
    char *pcPtr;
    uint16_t u16DatLen, u16FindPos1, u16FindPos2, u16FindPos3;
    // static uint16_t u16TurnCnt1 = 0, u16TurnCnt2 = 0, u16TurnCnt3 = 0;
    static uint16_t u16SMS_ID;
    uint8_t u8ConnSrcID;
    uint8_t u8DeactPDP_ID;
    uint16_t u16GSM_ErrCode1, u16GSM_ErrCode2;
    uint16_t u16NonBlockRes;
    uint32_t u32NonBlockResCode;
    uint32_t u32GSM_LastTickTemp, u32GSM_CurrTickTemp;
    static uint32_t u32CSQ_QueryLastTick = 0,
                    u32SMS_QueryLastTick = 0,
                    u32TCP_QueryLastTick = 0;
    bool bRes;

    if (bQtEC21_State == FALSE)
        return;

    u32GSM_LastTickTemp = u32GSM_ResetLastTick;

    u32GSM_CurrTickTemp = HAL_GetTick();

    if ((bQtEC21_Ready == FALSE) &&
        (u32GSM_CurrTickTemp > (u32GSM_LastTickTemp + S_2_mS(GSM_READY_TIMEOUT))))
    {
        g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt++;

        if (g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt >= 3)
        {
            g_Setting.tGSM_SET.tGSM_Err.u8Function_ErrCnt = 0;

            bQtEC21_State = FALSE;

            RTOSprintf("\r\n> GSM module NOT ACTIVATE! Maximum GSM module reset reached. Stop GSM functions!");

            char cLog[30] = {0};

            sprintf(cLog, "GSM module stop activation");

            GPRS_DebugLog(cLog, strlen(cLog));

            LCD_SetShowAutoTestPage(52);
        }
        else
        {
            GSMprintf("\r\n> GSM module NOT ACTIVATE! Reset GSM module.");

            GSMprintf("\r\n> GSM last reset time: %d, current time: %d", u32GSM_LastTickTemp, u32GSM_CurrTickTemp);

            char cLog[30] = {0};

            sprintf(cLog, "GSM module is not activated");

            GPRS_DebugLog(cLog, strlen(cLog));

            // QtEC21_WakeUp();

            // HAL_Delay(100);

            // QtEC21_TurnOnPwrKey();
            bGSM_PwrOnFlag = TRUE;

            HAL_Delay(S_2_mS(1));

            // GSM_Reset();
            bGSM_ResetFlag = TRUE;
        }

        System_SaveSetting();

        return;
    }

    gsmEvent = GSM_EVENT_PROCESS;

    taskInfo[TASK_GSM_RX].u16TaskStep++;

    osRes = osMutexWait(mutex_gsm_rx_user, S_2_mS(120));

    if (osRes != osOK)
    {
        GSMprintf("\r\n> Can't acquire GSM RX mutex. osRes:%d", osRes);

        return;
    }

    taskInfo[TASK_GSM_RX].u16TaskStep++;

    u16DatLen = UART_RecvDataLen(GSM_COMPORT_DEF);

    taskInfo[TASK_GSM_RX].u16TaskStep++;

    u16GSM_ErrCode1 = 0;
    u16GSM_ErrCode2 = 0;

    if (u16DatLen == 0)
    {
        if (u16CurrGSM_ByteNum > 0)
            u16CurrGSM_ByteNum = 0;
    }
    else
    {
        // if(u16DatLen != u16CurrGSM_ByteNum)
        {
            uint16_t u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Tail;

            UART_PeekRecvData(GSM_COMPORT_DEF, u16StartPos, u8GSM_RX_Buf, u16DatLen);

            u8GSM_RX_Buf[u16DatLen] = 0;

            if (u16DatLen != u16CurrGSM_ByteNum)
            {
                if (u16DatLen == 1)
                {
                    GSMprintf("\r\n> There is 1 byte in GSM RX buffer: 0x%02X", u8GSM_RX_Buf[0]);

                    // GSMprintf("\r\n> GSM peek data:\n0x%02X\r\n> GSM peek data end.", u8GSM_RX_Buf[0]);
                }
                else
                {
                    GSMprintf("\r\n> There are %d bytes in GSM RX buffer.", u16DatLen);

                    GSMprintf("\r\n> GSM peek data:\n%s\r\n> GSM peek data end.", u8GSM_RX_Buf);
                }

                u16CurrGSM_ByteNum = u16DatLen;

                GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
            }

            // If found "\r\nRDY\r\n"
            if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_MODULE_START, &u16FindPos1))
            {
                GSMprintf("\r\n> QTEC21 ready to receive commands.");

                // char cLog[30] = {0};

                // sprintf(cLog, "GSM Ready");

                // GPRS_DebugLog(cLog, strlen(cLog));

                u16DatLen = UART_RecvDataLen(GSM_COMPORT_DEF);

                UART_GetRecvData(GSM_COMPORT_DEF, u8GSM_RX_Buf, u16DatLen);

                if (u16CurrGSM_ByteNum >= u16DatLen)
                    u16CurrGSM_ByteNum -= u16DatLen;
                else
                    u16CurrGSM_ByteNum = 0;

                u8GSM_RX_Buf[u16DatLen] = 0;

                if (u8GSM_RX_Buf[0] == 0)
                    u8GSM_RX_Buf[0] = '\n';

                GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                if (bQtEC21_Ready == FALSE)
                {
                    GSM_FuncInit();

                    bQtEC21_Ready = TRUE;

                    gsmEvent = GSM_EVENT_INIT;
                }
            }
            else if (bQtEC21_State && bQtEC21_InitFinish) // Check for the other types of messages
            {
                // If find "\r\nOK\r\n"
                if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_OK, &u16FindPos1))
                {
                    // GSMprintf("\r\n> GSM found OK response.");

                    switch ((int)gsmCurrNonBlockCmd)
                    {
                    case QTEC21_CALL_ANS:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_ANS_SUCCESS;

                        u16DatLen = 6;

                        break;
                    }

                    case QTEC21_CALL_DISCONN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_DISCONN_SUCCESS;

                        u16DatLen = 6;

                        break;
                    }

                    case QTEC21_PD_PS_STATE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PSSTATE_SUCCESS;

                        u16DatLen = 6;

                        break;
                    }

                    case QTEC21_PD_PDP_ACT:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PDPACT_SUCCESS;

                        u16DatLen = 6;

                        break;
                    }

                    case QTEC21_TCP_CONN_OPEN:
                    {
                        uint16_t u16Cnt = 0;

                        // GSMprintf("\r\n> GSM TCP open conn found OK response.");

                        // osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        // GSMprintf("\r\n> Get opening connection response.");

                        while (u16Cnt < 1000)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)STR_RESPONSE_OPEN, &u16FindPos2, u16FindPos1);

                            if (bRes)
                                break;

                            u16Cnt++;
                        }

                        // If find "\r\n+QIOPEN: "
                        if (bRes)
                        {
                            osTimerStop(timerRTOS_GSM_NonBlockHandle);

                            u16Cnt = 0;

                            while (u16Cnt < 1000)
                            {
                                HAL_Delay(1);

                                bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3,
                                                        (u16FindPos2 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size);

                                if (bRes)
                                    break;

                                u16Cnt++;
                            }

                            // If find "\r\n"
                            if (bRes)
                            {
                                if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1))
                                {
                                    u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                    // Get the preview data
                                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                    pcPtr = (char *)u8GSM_RX_Buf;

                                    StopChar(pcPtr, ',');

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if (strlen(pcPtr) > 0)
                                    {
                                        uint16_t u16RetCode = atoi(pcPtr);

                                        u16GSM_ErrCode1 = u16RetCode;

                                        // GSMprintf("\r\n> Opening connection return code:%d", u16RetCode);

                                        if (u16RetCode == 0)
                                        {
                                            gsmEvent = GSM_EVENT_TCP_OPEN_SUCCESS;
                                        }
                                        else if (u16RetCode == 563)
                                        {
                                            gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                        }
                                        else
                                        {
                                            gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                        }
                                    }
                                    else
                                        gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;

                                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                            rbBuf[GSM_COMPORT_DEF].u16Size;
                            }
                            else
                            {
                                GSMprintf("\r\n> GSM TCP open conn not found \\r\\n");

                                gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;

                                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 11 - u16FindPos1) %
                                            rbBuf[GSM_COMPORT_DEF].u16Size;
                            }
                        }
                        else
                        {
                            // GSMprintf("\r\n> GSM TCP open conn not found +QIOPEN");

                            // gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                            // gsmEvent = GSM_EVENT_TCP_OPEN_SUCCESS;

                            u16DatLen = 6;
                        }

                        break;
                    }

                    default:
                    {
                        u16DatLen = 6;

                        break;
                    }
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\nERROR\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_ERROR, &u16FindPos1))
                {
                    switch ((int)gsmCurrNonBlockCmd)
                    {
                    case QTEC21_SMS_SEND:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_SMS_SEND_ERROR;

                        break;
                    }

                    case QTEC21_CALL_ANS:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_ANS_ERROR;

                        break;
                    }

                    case QTEC21_CALL_DISCONN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_DISCONN_ERROR;

                        break;
                    }

                    case QTEC21_PD_PS_STATE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PSSTATE_ERROR;

                        break;
                    }

                    case QTEC21_PD_PDP_ACT:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PDPACT_ERROR;

                        break;
                    }

                    case QTEC21_TCP_CONN_OPEN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;

                        // GSMprintf("\r\n> Get opening connection response error.");

                        break;
                    }

                    case QTEC21_FTP_OPEN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;

                        break;
                    }

                    case QTEC21_FTP_SET_DIR:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;

                        break;
                    }

                    case QTEC21_FTP_MAKE_DIR:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;

                        break;
                    }

                    case QTEC21_FTP_UPLOAD:
                    case QTEC21_FTP_UPLOAD_DIRECT:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;

                        break;
                    }

                    case QTEC21_FTP_DOWNLOAD:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;

                        break;
                    }

                    case QTEC21_FTP_FILE_SIZE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;

                        break;
                    }

                    case QTEC21_FTP_TRANS_LEN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;

                        break;
                    }

                    case QTEC21_FTP_FILE_DEL:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;

                        break;
                    }

                    case QTEC21_FTP_CLOSE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;

                        break;
                    }

                    case QTEC21_HTTP_GET:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_HTTP_GET_ERROR;

                        break;
                    }

                    case QTEC21_HTTP_READ_FILE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_HTTP_READFILE_ERROR;

                        break;
                    }

                    default:
                    {
                        break;
                    }
                    }

                    u16DatLen = 9;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+CME ERROR:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_CME_ERROR, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                    // Get the preview data
                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    pcPtr = (char *)u8GSM_RX_Buf;

                    StopChar(pcPtr, ':');

                    pcPtr += strlen(pcPtr) + 2;

                    StopChar(pcPtr, '\r');

                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                    {
                        u16GSM_ErrCode1 = atoi(pcPtr);
                    }

                    switch ((int)gsmCurrNonBlockCmd)
                    {
                    case QTEC21_PD_PS_STATE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PSSTATE_ERROR;

                        break;
                    }

                    case QTEC21_PD_PDP_ACT:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PDPACT_ERROR;

                        break;
                    }

                    case QTEC21_FTP_OPEN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;

                        break;
                    }

                    case QTEC21_FTP_SET_DIR:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;

                        break;
                    }

                    case QTEC21_FTP_MAKE_DIR:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;

                        break;
                    }

                    case QTEC21_FTP_UPLOAD:
                    case QTEC21_FTP_UPLOAD_DIRECT:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;

                        break;
                    }

                    case QTEC21_FTP_DOWNLOAD:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;

                        break;
                    }

                    case QTEC21_FTP_FILE_SIZE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;

                        break;
                    }

                    case QTEC21_FTP_TRANS_LEN:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;

                        break;
                    }

                    case QTEC21_FTP_FILE_DEL:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;

                        break;
                    }

                    case QTEC21_FTP_CLOSE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;

                        break;
                    }

                    case QTEC21_HTTP_GET:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_HTTP_GET_ERROR;

                        break;
                    }

                    case QTEC21_HTTP_READ_FILE:
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_HTTP_READFILE_ERROR;

                        break;
                    }

                    default:
                        break;
                    }

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+CMS ERROR:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_CMS_ERROR, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                    // Get the preview data
                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    pcPtr = (char *)u8GSM_RX_Buf;

                    StopChar(pcPtr, ':');

                    pcPtr += strlen(pcPtr) + 2;

                    StopChar(pcPtr, '\r');

                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                    {
                        u16GSM_ErrCode1 = atoi(pcPtr);
                    }

                    if (gsmCurrNonBlockCmd == QTEC21_SMS_SEND)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_SMS_SEND_ERROR;
                    }

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+CMGS:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_SEND, &u16FindPos1))
                {
                    if (gsmCurrNonBlockCmd == QTEC21_SMS_SEND)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_SMS_SEND_SUCCESS;
                    }

                    HAL_Delay(10);

                    // If find "\r\nOK\r\n"
                    if (UART_RecvFindPos(GSM_COMPORT_DEF, STR_RESPONSE_OK, &u16FindPos2, (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    {
                        // Message length
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    // If find only "\r\n"
                    else if (UART_RecvFindPos(GSM_COMPORT_DEF, RESP_END, &u16FindPos2, (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    {
                        // Message length
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        u16DatLen = 9;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n> "
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)"\r\n> ", &u16FindPos1))
                {
                    u16DatLen = 4;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    // uint8_t u8Cmd = SEND_MSG_CMD;

                    // Send "Ctrl+Z" signal to indicate the end of the message content
                    QtEC21_SendBuffer((uint8_t *)SEND_MSG_CMD, strlen(SEND_MSG_CMD));
                }
                // If find "+QIOPEN: "
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_OPEN, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    // If the string is actually "\r\n+QIOPEN: "
                    if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_OPEN_2, &u16FindPos2))
                    {
                        u16FindPos1 = u16FindPos2;
                    }

                    while (u8Cnt < 250)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3,
                                                (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1))
                        {
                            u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                            // Get the preview data
                            UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                            pcPtr = (char *)u8GSM_RX_Buf;

                            StopChar(pcPtr, ',');

                            pcPtr += strlen(pcPtr) + 1;

                            StopChar(pcPtr, '\r');

                            if (strlen(pcPtr) > 0)
                            {
                                uint16_t u16RetCode = atoi(pcPtr);

                                // GSMprintf("\r\n> Opening connection return code:%d", u16RetCode);

                                u16GSM_ErrCode1 = u16RetCode;

                                if (u16RetCode == 0)
                                {
                                    if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                        gsmEvent = GSM_EVENT_TCP_OPEN_SUCCESS;
                                }
                                else if (u16RetCode == 563)
                                {
                                    if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                        gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                }
                                else if (u16RetCode == 567)
                                {
                                    if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                        gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                }
                                else
                                {
                                    if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                        gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                                }
                            }
                            else
                            {
                                if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                    gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                            }
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                                gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;
                        }

                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                            gsmEvent = GSM_EVENT_TCP_OPEN_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\nCONNECT\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_CONNECT, &u16FindPos1))
                {
                    if (gsmCurrNonBlockCmd == QTEC21_CALL_ANS)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_ANS_SUCCESS;
                    }
                    else if (gsmCurrNonBlockCmd == QTEC21_TCP_CONN_OPEN)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_TCP_OPEN_SUCCESS;
                    }
                    else if (gsmCurrNonBlockCmd == QTEC21_FTP_DOWNLOAD)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_FTP_DOWNLOAD_CONNECT;
                    }

                    u16DatLen = 11;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPOPEN:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_OPEN, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_OPEN)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_OPEN)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 13;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                    {
                                        u16GSM_ErrCode2 = atoi(pcPtr);
                                    }

                                    if (u16ErrCode == 0)
                                        gsmEvent = GSM_EVENT_FTP_OPEN_SUCCESS;
                                    else
                                        gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_OPEN)
                                gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_OPEN)
                            gsmEvent = GSM_EVENT_FTP_OPEN_ERROR;

                        u16DatLen = 12;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPCWD:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_CWD, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_SET_DIR)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_SET_DIR)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 12;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                    {
                                        u16GSM_ErrCode2 = atoi(pcPtr);
                                    }

                                    if (u16ErrCode == 0)
                                        gsmEvent = GSM_EVENT_FTP_SETDIR_SUCCESS;
                                    else
                                        gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_SET_DIR)
                                gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_SET_DIR)
                            gsmEvent = GSM_EVENT_FTP_SETDIR_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\r\n> %s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPMKDIR:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_MKDIR, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_MAKE_DIR)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_MAKE_DIR)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 14;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                    {
                                        u16GSM_ErrCode2 = atoi(pcPtr);
                                    }

                                    if (u16ErrCode == 0)
                                        gsmEvent = GSM_EVENT_FTP_MAKEDIR_SUCCESS;
                                    else
                                        gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_MAKE_DIR)
                                gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_MAKE_DIR)
                            gsmEvent = GSM_EVENT_FTP_MAKEDIR_ERROR;

                        u16DatLen = 13;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\r\n> %s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPPUT:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_PUT, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if ((gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD) ||
                        (gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD_DIRECT))
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if ((gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD) ||
                                (gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD_DIRECT))
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 12;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    if (u16ErrCode == 0)
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if (strlen(pcPtr) > 0)
                                        {
                                            // u32GetTransLen = atoi(pcPtr);

                                            gsmEvent = GSM_EVENT_FTP_UPLOAD_SUCCESS;
                                        }
                                        else
                                            gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;
                                    }
                                    else
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                        {
                                            u16GSM_ErrCode2 = atoi(pcPtr);
                                        }

                                        gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;
                                    }
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;
                            }

                            // GSMprintf("\r\n> Get late FTP put response.");

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if ((gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD) ||
                                (gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD_DIRECT))
                                gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if ((gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD) ||
                            (gsmCurrNonBlockCmd == QTEC21_FTP_UPLOAD_DIRECT))
                            gsmEvent = GSM_EVENT_FTP_UPLOAD_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPGET:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_GET, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_DOWNLOAD)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_DOWNLOAD)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 12;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    if (u16ErrCode == 0)
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if (strlen(pcPtr) > 0)
                                        {
                                            // u32GetTransLen = atoi(pcPtr);

                                            gsmEvent = GSM_EVENT_FTP_DOWNLOAD_SUCCESS;
                                        }
                                        else
                                            gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;
                                    }
                                    else
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                        {
                                            u16GSM_ErrCode2 = atoi(pcPtr);
                                        }

                                        gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;
                                    }
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_DOWNLOAD)
                                gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_DOWNLOAD)
                            gsmEvent = GSM_EVENT_FTP_DOWNLOAD_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPSIZE:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_SIZE, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_SIZE)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_SIZE)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 13;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    if (u16ErrCode == 0)
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if (strlen(pcPtr) > 0)
                                        {
                                            u32GetFileSize = atoi(pcPtr);

                                            gsmEvent = GSM_EVENT_FTP_GETSIZE_SUCCESS;
                                        }
                                        else
                                            gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;
                                    }
                                    else
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                        {
                                            u16GSM_ErrCode2 = atoi(pcPtr);
                                        }

                                        gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;
                                    }
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_SIZE)
                                gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_SIZE)
                            gsmEvent = GSM_EVENT_FTP_GETSIZE_ERROR;

                        u16DatLen = 12;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPMDTM:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_DATE, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DATE)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DATE)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 13;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    if (u16ErrCode == 0)
                                    {
                                        pcPtr += strlen(pcPtr) + 2;

                                        StopChar(pcPtr, '\r');

                                        if (strlen(pcPtr) >= 14)
                                        {
                                            memset(cGetFileLastDate, 0x00, sizeof(cGetFileLastDate));

                                            strncpy(cGetFileLastDate, pcPtr, 14);

                                            gsmEvent = GSM_EVENT_FTP_GETDATE_SUCCESS;
                                        }
                                        else
                                            gsmEvent = GSM_EVENT_FTP_GETDATE_ERROR;
                                    }
                                    else
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                        {
                                            u16GSM_ErrCode2 = atoi(pcPtr);
                                        }

                                        gsmEvent = GSM_EVENT_FTP_GETDATE_ERROR;
                                    }
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_GETDATE_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DATE)
                                gsmEvent = GSM_EVENT_FTP_GETDATE_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DATE)
                            gsmEvent = GSM_EVENT_FTP_GETDATE_ERROR;

                        u16DatLen = 12;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPLEN:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_LEN, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_TRANS_LEN)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_TRANS_LEN)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 12;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    if (u16ErrCode == 0)
                                    {
                                        pcPtr += strlen(pcPtr) + 1;

                                        StopChar(pcPtr, '\r');

                                        if (strlen(pcPtr) > 0)
                                        {
                                            // u32GetTransLen = atoi(pcPtr);

                                            gsmEvent = GSM_EVENT_FTP_TRANSLEN_SUCCESS;
                                        }
                                        else
                                            gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;
                                    }
                                    else
                                    {
                                        gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;
                                    }
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_TRANS_LEN)
                                gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_TRANS_LEN)
                            gsmEvent = GSM_EVENT_FTP_TRANSLEN_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPDEL:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_DEL, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DEL)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DEL)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 12;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                    {
                                        u16GSM_ErrCode2 = atoi(pcPtr);
                                    }

                                    if (u16ErrCode == 0)
                                        gsmEvent = GSM_EVENT_FTP_FILEDEL_SUCCESS;
                                    else
                                        gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DEL)
                                gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_FILE_DEL)
                            gsmEvent = GSM_EVENT_FTP_FILEDEL_ERROR;

                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QFTPCLOSE:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_FTP_CLOSE, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_FTP_CLOSE)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_CLOSE)
                            {
                                u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                pcPtr = (char *)u8GSM_RX_Buf + 14;

                                StopChar(pcPtr, ',');

                                if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                {
                                    uint16_t u16ErrCode = atoi(pcPtr);

                                    u16GSM_ErrCode1 = u16ErrCode;

                                    pcPtr += strlen(pcPtr) + 1;

                                    StopChar(pcPtr, '\r');

                                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                                    {
                                        u16GSM_ErrCode2 = atoi(pcPtr);
                                    }

                                    if (u16ErrCode == 0)
                                        gsmEvent = GSM_EVENT_FTP_CLOSE_SUCCESS;
                                    else
                                        gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;
                                }
                                else
                                    gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_FTP_CLOSE)
                                gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_FTP_CLOSE)
                            gsmEvent = GSM_EVENT_FTP_CLOSE_ERROR;

                        u16DatLen = 13;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QHTTPGET:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_HTTP_GET, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_HTTP_GET)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)",", &u16FindPos2, u16FindPos1);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find ","
                    if (bRes)
                    {
                        u8Cnt = 0;

                        while (u8Cnt < 50)
                        {
                            HAL_Delay(1);

                            bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos3, u16FindPos2);

                            if (bRes)
                                break;

                            u8Cnt++;
                        }

                        // If find "\r\n"
                        if (bRes)
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_HTTP_GET)
                            {
                                uint32_t u32Err = 0, u32HTTP_Err = 0, u32ContentLen = 0;

                                // u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);
                                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                            rbBuf[GSM_COMPORT_DEF].u16Size;

                                // Get the preview data
                                UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                                // GSMprintf("\r\n> GSM HTTP GET response: %s", u8GSM_RX_Buf);

                                pcPtr = (char *)u8GSM_RX_Buf + 13;

                                if (sscanf(pcPtr, "%d,%d,%d", &u32Err, &u32HTTP_Err, &u32ContentLen) == 3)
                                {
                                    u16GSM_ErrCode1 = u32Err;
                                    u16GSM_ErrCode2 = u32HTTP_Err;

                                    // #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
                                    tGET_Resp.u16Err = (uint16_t)u32Err;
                                    tGET_Resp.u16HTTP_Err = u32HTTP_Err;
                                    tGET_Resp.u32ContentLen = u32ContentLen;
                                    // #endif

                                    if ((u16GSM_ErrCode1 == 0) && (u32ContentLen > 0))
                                    {
                                        gsmEvent = GSM_EVENT_HTTP_GET_SUCCESS;
                                    }
                                    else
                                    {
                                        gsmEvent = GSM_EVENT_HTTP_GET_ERROR;
                                    }
                                }
                                else if (sscanf(pcPtr, "%d,%d", &u32Err, &u32HTTP_Err) == 2)
                                {
                                    u16GSM_ErrCode1 = u32Err;
                                    u16GSM_ErrCode2 = u32HTTP_Err;

                                    // #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
                                    tGET_Resp.u16Err = (uint16_t)u32Err;
                                    tGET_Resp.u16HTTP_Err = u32HTTP_Err;
                                    // #endif

                                    if (u16GSM_ErrCode1 == 0)
                                    {
                                        gsmEvent = GSM_EVENT_HTTP_GET_SUCCESS;
                                    }
                                    else
                                    {
                                        gsmEvent = GSM_EVENT_HTTP_GET_ERROR;
                                    }
                                }
                                else
                                {
                                    gsmEvent = GSM_EVENT_HTTP_GET_ERROR;
                                }
                            }

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                        else
                        {
                            if (gsmCurrNonBlockCmd == QTEC21_HTTP_GET)
                                gsmEvent = GSM_EVENT_HTTP_GET_ERROR;

                            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 1 - u16FindPos1) %
                                        rbBuf[GSM_COMPORT_DEF].u16Size;
                        }
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_HTTP_GET)
                            gsmEvent = GSM_EVENT_HTTP_GET_ERROR;

                        u16DatLen = 12;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QHTTPREADFILE:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_HTTP_READFILE, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    if (gsmCurrNonBlockCmd == QTEC21_HTTP_READ_FILE)
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_HTTP_READ_FILE)
                        {
                            u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                            // Get the preview data
                            UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                            pcPtr = (char *)u8GSM_RX_Buf + 18;

                            StopChar(pcPtr, '\r');

                            if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 4))
                            {
                                uint16_t u16ErrCode = atoi(pcPtr);

                                u16GSM_ErrCode1 = u16ErrCode;

                                GSMprintf("\r\n> GSM HTTP read file error code: %d", u16ErrCode);

                                // #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
                                // tGPS_Xtra.u16ReadFileErr = u16ErrCode;
                                // #endif

                                if (u16GSM_ErrCode1 == 0)
                                {
                                    gsmEvent = GSM_EVENT_HTTP_READFILE_SUCCESS;
                                }
                                else
                                {
                                    gsmEvent = GSM_EVENT_HTTP_READFILE_ERROR;
                                }
                            }
                            else
                            {
                                gsmEvent = GSM_EVENT_HTTP_READFILE_ERROR;
                            }
                        }

                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        if (gsmCurrNonBlockCmd == QTEC21_HTTP_READ_FILE)
                            gsmEvent = GSM_EVENT_HTTP_READFILE_ERROR;

                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\nRING\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RING, &u16FindPos1))
                {
                    gsmEvent = GSM_EVENT_RECV_CALL;

                    u16DatLen = 8;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+CMTI: "SM","
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_CMTI_MSG, &u16FindPos1))
                {
                    GSMprintf("\r\n> Found SMS message header.");

                    uint8_t u8Cnt = 0;

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2, u16FindPos1 + 2);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

                        // Get the preview data
                        UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                        pcPtr = (char *)u8GSM_RX_Buf;

                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr) + 1;

                        StopChar(pcPtr, '\r');

                        if (strlen(pcPtr) > 0)
                        {
                            u16SMS_ID = atoi(pcPtr);

                            // if((EXTIO_IsMainPowerNormal() == 1) && (GSM_IsBusy() == FALSE))
                            {
                                gsmEvent = GSM_EVENT_RECV_MSG;
                            }
                        }

                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        u16DatLen = 14;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QIURC: "recv","
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RECEIVE_PACKET, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) + rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Get the preview data
                        UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                        pcPtr = (char *)u8GSM_RX_Buf;

                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr) + 1;

                        StopChar(pcPtr, '\r');

                        if (strlen(pcPtr) > 0)
                        {
                            u8ConnSrcID = atoi(pcPtr);

                            if (g_SysInfo.tVehicleState.bPWR_ON == TRUE)
                            {
                                if (u8ConnSrcID == u8CurrTCP_Socket[0])
                                {
                                    gsmEvent = GSM_EVENT_RECV_PACKET;
                                }
#if ENABLE_MULTI_SERVER
                                else if (u8ConnSrcID == u8CurrTCP_Socket[1])
                                {
                                    gsmEvent = GSM_EVENT_RECV_PACKET;
                                }
#endif
                            }
                        }
                    }
                    else
                    {
                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QIURC: "closed","
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_CONN_CLOSED, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) + rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Get the preview data
                        UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                        pcPtr = (char *)u8GSM_RX_Buf;

                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr) + 1;

                        StopChar(pcPtr, '\r');

                        if (strlen(pcPtr) > 0)
                        {
                            u8ConnSrcID = atoi(pcPtr);

                            if (u8ConnSrcID == u8CurrTCP_Socket[0])
                            {
                                gsmEvent = GSM_EVENT_CONN_CLOSED;
                            }
#if ENABLE_MULTI_SERVER
                            else if (u8ConnSrcID == u8CurrTCP_Socket[1])
                            {
                                gsmEvent = GSM_EVENT_CONN_CLOSED;
                            }
#endif
                        }
                    }
                    else
                    {
                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QIURC: "pdpdeact","
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_MSG_PDP_DEACT, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) + rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Get the preview data
                        UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                        pcPtr = (char *)u8GSM_RX_Buf;

                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr) + 1;

                        StopChar(pcPtr, '\r');

                        if (strlen(pcPtr) > 0)
                        {
                            u8DeactPDP_ID = atoi(pcPtr);

                            if (u8DeactPDP_ID == u8CurrPDP_ID)
                            {
                                gsmEvent = GSM_EVENT_PDP_DEACT;
                            }
                        }
                    }
                    else
                    {
                        u16DatLen = 17;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\n+QGPSURC:"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_URC_GPS_XTRA, &u16FindPos1))
                {
                    uint8_t u8Cnt = 0;

                    while (u8Cnt < 50)
                    {
                        HAL_Delay(1);

                        bRes = UART_RecvFindPos(GSM_COMPORT_DEF, (char *)RESP_END, &u16FindPos2,
                                                (u16FindPos1 + 2) + rbBuf[GSM_COMPORT_DEF].u16Size);

                        if (bRes)
                            break;

                        u8Cnt++;
                    }

                    // If find "\r\n"
                    if (bRes)
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;
                    }
                    else
                    {
                        u16DatLen = 11;
                    }

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "\r\nNO CARRIER\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_RESPONSE_NO_CARRIER, &u16FindPos1))
                {
                    if (gsmCurrNonBlockCmd == QTEC21_CALL_ANS)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_CALL_ANS_ERROR;
                    }
                    else if (gsmCurrNonBlockCmd == QTEC21_PD_PDP_ACT)
                    {
                        osTimerStop(timerRTOS_GSM_NonBlockHandle);

                        gsmEvent = GSM_EVENT_PD_PDPACT_ERROR;
                    }
                    else
                        gsmEvent = GSM_EVENT_NO_CARRIER;

                    u16DatLen = 14;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);
                }
                // If find "r\n+CFUN: 1\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_ME_FUNC_READY, &u16FindPos1))
                {
                    u16DatLen = 12;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_ME_FUNC_READY;
                }
                // If find "\r\n+CPIN: READY\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_SIM_READY, &u16FindPos1))
                {
                    u16DatLen = 16;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_SIM_READY;
                }
                // If find "\r\n+CPIN: NOT READY\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_SIM_NOT_READY, &u16FindPos1))
                {
                    u16DatLen = 20;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_SIM_NOT_READY;
                }
                // If find "\r\n+CPIN: NOT INSERTED\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_SIM_NOT_INSERTED, &u16FindPos1))
                {
                    u16DatLen = 23;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_SIM_NOT_INSERTED;
                }
                // If find "\r\n+QUSIM: 1\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_QUSIM_OK, &u16FindPos1))
                {
                    u16DatLen = 13;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_QUSIM_OK;
                }
                // If find "\r\n+QIND: SMS DONE\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_SMS_READY, &u16FindPos1))
                {
                    u16DatLen = 19;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_SMS_READY;
                }
                // If find "\r\n+QIND: PB DONE\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_PHONEBOOK_READY, &u16FindPos1))
                {
                    u16DatLen = 18;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_PHONEBOOK_READY;
                }
                // If find "\r\nNORMAL POWER DOWN\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_NORMAL_POWER_DOWN, &u16FindPos1))
                {
                    u16DatLen = 21;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_NULL;
                }
                // If find "\r\nPOWERED DOWN\r\n"
                else if (UART_RecvFind(GSM_COMPORT_DEF, (char *)STR_POWERED_DOWN, &u16FindPos1))
                {
                    u16DatLen = 16;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, u8GSM_RX_Buf, u16DatLen);

                    if (u16CurrGSM_ByteNum >= u16DatLen)
                        u16CurrGSM_ByteNum -= u16DatLen;
                    else
                        u16CurrGSM_ByteNum = 0;

                    u8GSM_RX_Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> GSM data:\n%s\r\n> GSM data end.", u8GSM_RX_Buf);

                    GSMprintf("\r\n> GSM RX current unprocessed bytes: %d", u16CurrGSM_ByteNum);

                    gsmEvent = GSM_EVENT_POWERED_DOWN;
                }
                // If find no special string
                else
                {
                    if (u16DatLen > (rbBuf[GSM_COMPORT_DEF].u16Size * 3 / 4))
                    {
                        u16DatLen = UART_RecvDataLen(GSM_COMPORT_DEF);

                        UART_GetRecvData(GSM_COMPORT_DEF, u8GSM_RX_Buf, u16DatLen);

                        u16CurrGSM_ByteNum = 0;

                        u8GSM_RX_Buf[u16DatLen] = 0;

                        GSMprintf("\r\n> GSM ring buffer has %d bytes. Clear GSM ring buffer.", u16DatLen);

                        // GSMprintf("\r\n> GSM RX buffer data:\n%s\r\n> GSM RX buffer data end.", u8GSM_RX_Buf);
                    }
                }
            }
        }
    }

    osMutexRelease(mutex_gsm_rx_user);

    taskInfo[TASK_GSM_RX].u16TaskStep++;

    switch ((uint8_t)gsmEvent)
    {
    case GSM_EVENT_INIT:
    {
        // bQtEC21_Ready = TRUE;

        HAL_Delay(200);

        procGSM_SetupStep = PROC_STEP_1;

        break;
    }

    case GSM_EVENT_PROCESS:
    {
        if ((EXTIO_IsMainPowerNormal() == TRUE) ||
            (bSystemMode == SYSTEM_MODE_STEALTH))
        {
            if (HAL_GetTick() >= (u32CSQ_QueryLastTick + S_2_mS(20)))
            {
                u32CSQ_QueryLastTick = HAL_GetTick();

                if ((bQtEC21_State == TRUE) && (bQtEC21_InitFinish == TRUE) &&
                    (bGSM_ResetFlag == FALSE) && (bGSM_ResetCheckFTP_Flag == FALSE))
                //(GSM_IsBusy() == FALSE))
                {
                    uint8_t u8RSSI, u8Ber;
                    static uint8_t u8CSQ_GoodCnt = 0;
                    static uint8_t u8CSQ_PoorCnt = 0;

                    bRes = GSM_Net_GetCSQ(&u8RSSI, &u8Ber);
                    // bRes = QtEC21_Net_GetCSQ(&u8RSSI, &u8Ber);

                    if (bRes)
                    {
                        GSMprintf("\r\n> RSSI:%d \r\n", u8RSSI);

                        // GSMprintf("\r\n> BER:%d", u8Ber);

                        g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ = u8RSSI;

                        if (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ >= CSQ_GOOD_THRESHOLD)
                        {
                            if (bCSQ_IsGood == FALSE)
                            {
                                u8CSQ_GoodCnt++;

                                if (u8CSQ_GoodCnt >= 2)
                                {
                                    u8CSQ_GoodCnt = 0;

                                    bCSQ_IsGood = TRUE;

                                    GSMprintf("\r\n> GSM CSQ changed from Average to Good");
                                }
                            }
                        }
                        else
                        {
                            u8CSQ_GoodCnt = 0;
                        }

                        if (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ <= CSQ_POOR_THRESHOLD)
                        {
                            if (bCSQ_IsGood == TRUE)
                            {
                                u8CSQ_PoorCnt++;

                                if (u8CSQ_PoorCnt >= 2)
                                {
                                    u8CSQ_PoorCnt = 0;

                                    bCSQ_IsGood = FALSE;
                                }
                            }
                        }
                        else
                        {
                            u8CSQ_PoorCnt = 0;
                        }
                    }
                    else
                        GSMprintf("\r\n> Get CSQ FAIL.\r\n");
                }
            }

            if (HAL_GetTick() >= (u32SMS_QueryLastTick + S_2_mS(30)))
            {
                u32SMS_QueryLastTick = HAL_GetTick();

                if ((bQtEC21_State == TRUE) && (bQtEC21_InitFinish == TRUE) &&
                    (bGSM_ResetFlag == FALSE) && (bGSM_ResetCheckFTP_Flag == FALSE) &&
                    (GSM_IsBusy() == FALSE))
                {
                    GSM_SMS_CheckUnread();

                    HAL_Delay(100);
                }
            }

            if (HAL_GetTick() >= (u32TCP_QueryLastTick + S_2_mS(30)))
            {
                u32TCP_QueryLastTick = HAL_GetTick();

                for (int i = 0; i < MAX_SOCKET_BANK; i++)
                {
                    if ((bQtEC21_State == TRUE) && (bQtEC21_InitFinish == TRUE) &&
                        (bGSM_ResetFlag == FALSE) && (bGSM_ResetCheckFTP_Flag == FALSE) &&
                        (GSM_IsBusy() == FALSE) && (bTCPIP_Connect[i] == TRUE))
                    {
                        uint16_t u16RecvDatLen, u16ReadLen;

                        bRes = GSM_TCP_RecvDataLen(u8CurrTCP_Socket[i], &u16RecvDatLen);

                        if (bRes && u16RecvDatLen)
                        {
                            HAL_Delay(50);

                            bRes = GSM_TCP_RecvDataLen(u8CurrTCP_Socket[i], &u16RecvDatLen);
                            // bRes = QtEC21_TCP_RecvDataLen(u8ConnSrcID, &u16RecvDatLen);

                            if (!bRes)
                            {
                                GSMprintf("\r\n> Fail to get the number of bytes received from TCP/IP connection.");

                                return;
                            }

                            if (u16RecvDatLen == 0)
                            {
                                GSMprintf("\r\n> TCP/IP message contains no byte.");

                                return;
                            }

                            memset(u8GSM_RX_Buf, 0x00, sizeof(u8GSM_RX_Buf));

                            bRes = GSM_TCP_GetRecvData(u8CurrTCP_Socket[i], u8GSM_RX_Buf, u16RecvDatLen, &u16ReadLen);
                            // bRes = QtEC21_TCP_GetRecvData(u8ConnSrcID, u8GSM_RX_Buf, u16RecvDatLen, &u16ReadLen);

                            if (!bRes)
                            {
                                GSMprintf("\r\n> Fail to get the received TCP/IP message.");

                                return;
                            }

                            GSMprintf("\r\n> GSM check TCP/IP get message:\r\n%s\r\n> TCP/IP message end.", u8GSM_RX_Buf);

                            GSM_TCP_MsgProc((char *)u8GSM_RX_Buf, u8CurrTCP_Socket[i]);

                            HAL_Delay(100);
                        }
                    }
                }
            }
        }

        break;
    }

    case GSM_EVENT_ME_FUNC_READY:
    case GSM_EVENT_SIM_READY:
    case GSM_EVENT_SIM_NOT_READY:
    case GSM_EVENT_SIM_NOT_INSERTED:
    case GSM_EVENT_QUSIM_OK:
    case GSM_EVENT_SMS_READY:
    case GSM_EVENT_PHONEBOOK_READY:
        break;

    case GSM_EVENT_POWERED_DOWN:
        GSMprintf("\r\n> Turn on GSM module.");

        // GSM_ResetWithCheckFTP();
        bGSM_ResetCheckFTP_Flag = TRUE;

        HAL_Delay(S_2_mS(1));

        QtEC21_TurnOnPwrKey();

        HAL_Delay(S_2_mS(1));

        // GSM_ResetWithCheckFTP();
        bGSM_ResetCheckFTP_Flag = TRUE;

        break;

    case GSM_EVENT_SMS_SEND_SUCCESS:
    {
        GSMprintf("\r\n> GSMS send SMS successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        // u16NonBlockRes = ((uint16_t)LE910_SMS_SEND << 8) | GSM_NONBLOCK_RES_SUCCESS;

        // osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_SMS_SEND_ERROR:
    {
        GSMprintf("\r\n> GSM send SMS error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        // u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        // osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        // u16NonBlockRes = ((uint16_t)LE910_SMS_SEND << 8) | GSM_NONBLOCK_RES_ERROR;

        // osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_CALL_ANS_SUCCESS:
    {
        GSMprintf("\r\n> GSM call answer successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_CALL_ANS << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_CALL_ANS_ERROR:
    {
        GSMprintf("\r\n> GSM call answer error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_CALL_ANS << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_CALL_DISCONN_SUCCESS:
    {
        GSMprintf("\r\n> GSM call disconnect successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_CALL_DISCONN << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_CALL_DISCONN_ERROR:
    {
        GSMprintf("\r\n> GSM call disconnect error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_CALL_DISCONN << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_PD_PSSTATE_SUCCESS:
    {
        GSMprintf("\r\n> GSM PD attach PDP context successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_PD_PS_STATE << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_PD_PSSTATE_ERROR:
    {
        GSMprintf("\r\n> GSM PD attach PDP context error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_PD_PS_STATE << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_PD_PDPACT_SUCCESS:
    {
        GSMprintf("\r\n> GSM PD activate PDP context successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_PD_PDP_ACT << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_PD_PDPACT_ERROR:
    {
        GSMprintf("\r\n> GSM PD activate PDP context error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_PD_PDP_ACT << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_TCP_OPEN_SUCCESS:
    {
        GSMprintf("\r\n> GSM TCP open connection successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_TCP_CONN_OPEN << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_TCP_OPEN_ERROR:
    {
        GSMprintf("\r\n> GSM TCP open connection error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_TCP_CONN_OPEN << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_OPEN_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP open connection successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_OPEN << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_OPEN_ERROR:
    {
        GSMprintf("\r\n> GSM FTP open connection error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_OPEN << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_SETDIR_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP set working directory successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_SET_DIR << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_SETDIR_ERROR:
    {
        GSMprintf("\r\n> GSM FTP set working directory error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_SET_DIR << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_MAKEDIR_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP create directory successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_MAKE_DIR << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_MAKEDIR_ERROR:
    {
        GSMprintf("\r\n> GSM FTP create directory error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_MAKE_DIR << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_UPLOAD_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP upload file successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_UPLOAD << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);
        break;
    }

    case GSM_EVENT_FTP_UPLOAD_ERROR:
    {
        GSMprintf("\r\n> GSM FTP upload file error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_UPLOAD << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_DOWNLOAD_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP download file successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_DOWNLOAD << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_DOWNLOAD_ERROR:
    {
        GSMprintf("\r\n> GSM FTP download file error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_DOWNLOAD << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_GETSIZE_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP get file size successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_SIZE << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_GETSIZE_ERROR:
    {
        GSMprintf("\r\n> GSM FTP get file size error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_SIZE << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_GETDATE_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP get file date successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_DATE << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_GETDATE_ERROR:
    {
        GSMprintf("\r\n> GSM FTP get file date error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_DATE << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_TRANSLEN_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP get number of bytes tranferred successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_TRANS_LEN << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_TRANSLEN_ERROR:
    {
        GSMprintf("\r\n> GSM FTP get number of bytes tranferred error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_TRANS_LEN << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_FILEDEL_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP delete file successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_DEL << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_FILEDEL_ERROR:
    {
        // GSMprintf("\r\n> GSM FTP delete file failed.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_FILE_DEL << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_CLOSE_SUCCESS:
    {
        GSMprintf("\r\n> GSM FTP close connection successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_CLOSE << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_FTP_CLOSE_ERROR:
    {
        // GSMprintf("\r\n> GSM FTP close connection error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_FTP_CLOSE << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_HTTP_GET_SUCCESS:
    {
        GSMprintf("\r\n> GSM HTTP GET successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_HTTP_GET << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_HTTP_GET_ERROR:
    {
        GSMprintf("\r\n> GSM HTTP GET error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_HTTP_GET << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_HTTP_READFILE_SUCCESS:
    {
        GSMprintf("\r\n> GSM HTTP read file successfully.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_HTTP_READ_FILE << 8) | GSM_NONBLOCK_RES_SUCCESS;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_HTTP_READFILE_ERROR:
    {
        GSMprintf("\r\n> GSM HTTP read file error.");

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;

        qtec21_NonBlockCmdInstance.bInUse = FALSE;

        u32NonBlockResCode = ((uint32_t)u16GSM_ErrCode1 << 16) | u16GSM_ErrCode2;

        osMessagePut(GSM_NonBlockResCodeHandle, u32NonBlockResCode, 0);

        u16NonBlockRes = ((uint16_t)QTEC21_HTTP_READ_FILE << 8) | GSM_NONBLOCK_RES_ERROR;

        osMessagePut(GSM_NonBlockResHandle, u16NonBlockRes, 0);

        break;
    }

    case GSM_EVENT_RECV_CALL:
    {
        // GSMprintf("\r\n> GSM incoming call!.");

        GSM_Call_Disconn();

        break;
    }

    case GSM_EVENT_RECV_MSG:
    {
        char cPhoneNbr[15] = {0};
        char cRecvTime[20] = {0};
        char cSMS_Content[256] = {0};

        GSMprintf("\r\n> GSM received SMS.");

        memset(u8GSM_RX_Buf, 0x00, sizeof(u8GSM_RX_Buf));

        bRes = GSM_SMS_Read(u16SMS_ID, u8GSM_RX_Buf);
        // bRes = QtEC21_SMS_Read(u16SMS_ID, u16GSM_RX_Buf);

        GSMprintf("\r\n> GSM read SMS result:%d", bRes);

        if (bRes)
        {
            bRes = QtEC21_SMS_Parse((char *)u8GSM_RX_Buf, cPhoneNbr, cRecvTime, cSMS_Content);

            // Check character set
            if ((bRes == TRUE) && (cSMS_Content[4] != '@'))
            {
                if (tQtEC21_CharSet == CHAR_SET_IRA)
                {
                    GSMprintf("\r\n> GSM incorrect character set. Change to GSM");

                    GSM_TECharSet("GSM");

                    tQtEC21_CharSet = CHAR_SET_GSM;
                }
                else
                {
                    GSMprintf("\r\n> GSM incorrect character set. Change to IRA");

                    GSM_TECharSet("IRA");

                    tQtEC21_CharSet = CHAR_SET_IRA;
                }

                bRes = GSM_SMS_Read(u16SMS_ID, u8GSM_RX_Buf);
            }
        }

        if (bRes)
        {
            memset(cPhoneNbr, 0x00, sizeof(cPhoneNbr));
            memset(cRecvTime, 0x00, sizeof(cRecvTime));
            memset(cSMS_Content, 0x00, sizeof(cSMS_Content));

            GSMprintf("\r\n> GSM SMS length:%d", strlen((char *)u8GSM_RX_Buf));

            // GSMprintf("\r\n> SMS content:\r\n%s\r\nSMS content end.", u8GSM_RX_Buf);

            if (strlen((char *)u8GSM_RX_Buf) < 256)
            {
                bRes = QtEC21_SMS_Parse((char *)u8GSM_RX_Buf, cPhoneNbr, cRecvTime, cSMS_Content);

                if (bRes)
                {
                    GSMprintf("\r\n> GSM SMS sender:%s", cPhoneNbr);

                    GSMprintf("\r\n> GSM SMS receiving time:%s", cRecvTime);

                    GSMprintf("\r\n> GSM SMS content:\n%s\nShort message end.", cSMS_Content);

                    GSM_SMS_Del(u16SMS_ID, 0);
                    // QtEC21_SMS_Del(u16SMS_ID, 0);

                    gsmCmdSrc = GSM_CMD_SRC_SMS;

                    if ((bSystemMode == SYSTEM_MODE_STEALTH) && (bWaitingEnterStandby == TRUE))
                    {
                        RTOSprintf("\r\n> GSM SMS refresh enter standby timer.");

                        // osTimerStop(timerACC_OffHandle);

                        // osTimerStart(timerACC_OffHandle, S_2_mS(30));

                        u32PwrDownDelayTick = HAL_GetTick();
                    }

                    sprintf(cCmdPhoneNum, "%s", cPhoneNbr);

                    GSM_SMS_Proc(cSMS_Content, cPhoneNbr);

                    // if(g_Setting.tUpgrade.bFOTA_Flag == TRUE)
                    //{
                    //	sprintf(cCmdPhoneNum, "%s", cPhoneNbr);
                    // }

                    memset(cCmdPhoneNum, 0x00, sizeof(cCmdPhoneNum));
                }
                else
                {
                    GSMprintf("\r\n> GSM SMS parse fail.");

                    GSM_SMS_Del(u16SMS_ID, 0);
                    // QtEC21_SMS_Del(u16SMS_ID, 0);
                }
            }
            else
            {
                GSMprintf("\r\n> GSM SMS length %d is unaccepted.", strlen((char *)u8GSM_RX_Buf));

                GSM_SMS_Del(u16SMS_ID, 0);
                // QtEC21_SMS_Del(u16SMS_ID, 0);
            }
        }

        break;
    }

    case GSM_EVENT_RECV_PACKET:
    {
        uint16_t u16RecvDatLen, u16ReadLen;

        HAL_Delay(100);

        // GSMprintf("\r\n> Received a TCP/IP data packet.");

        bRes = GSM_TCP_RecvDataLen(u8ConnSrcID, &u16RecvDatLen);
        // bRes = QtEC21_TCP_RecvDataLen(u8ConnSrcID, &u16RecvDatLen);

        if (!bRes)
        {
            GSMprintf("\r\n> GSM failed to get the length of received TCP message.");

            break;
        }

        if (u16RecvDatLen == 0)
        {
            GSMprintf("\r\n> GSM TCP message contains no byte.");

            break;
        }

        memset(u8GSM_RX_Buf, 0x00, sizeof(u8GSM_RX_Buf));

        bRes = GSM_TCP_GetRecvData(u8ConnSrcID, u8GSM_RX_Buf, u16RecvDatLen, &u16ReadLen);
        // bRes = QtEC21_TCP_GetRecvData(u8ConnSrcID, u8GSM_RX_Buf, u16RecvDatLen, &u16ReadLen);

        if (!bRes)
        {
            GSMprintf("\r\n> GSM fail to get the received TCP/IP message.");

            break;
        }

        // u8GSM_RX_Buf[u16ReadLen] = 0;

        // GSMprintf("\r\n> GSM get TCP/IP message:\r\n%s\r\n> TCP/IP message end.", u8GSM_RX_Buf);

        if (u8ConnSrcID == u8CurrTCP_Socket[0])
            gsmCmdSrc = GSM_CMD_SRC_TCP_1;
#if ENABLE_MULTI_SERVER
        else if (u8ConnSrcID == u8CurrTCP_Socket[1])
            gsmCmdSrc = GSM_CMD_SRC_TCP_2;
#endif

        GSM_TCP_MsgProc((char *)u8GSM_RX_Buf, u8ConnSrcID);

        break;
    }

    case GSM_EVENT_CONN_CLOSED:
    {
        char cLog[30] = {0};

        sprintf(cLog, "Socket %d close flag ON", u8ConnSrcID);

        GPRS_DebugLog(cLog, strlen(cLog));

        if (u8ConnSrcID == u8CurrTCP_Socket[0])
        {
            if (bTCPIP_Connect[0])
            {
                bTCP_CloseFlag[0] = TRUE;
            }
        }
#if ENABLE_MULTI_SERVER
        else if (u8ConnSrcID == u8CurrTCP_Socket[1])
        {
            if (bTCPIP_Connect[1])
            {
                bTCP_CloseFlag[1] = TRUE;
            }
        }
#endif

        break;
    }

    case GSM_EVENT_PDP_DEACT:
    {
        char cLog[30] = {0};

        sprintf(cLog, "PDP close flag ON");

        GPRS_DebugLog(cLog, strlen(cLog));

        bPDP_DeactFlag = TRUE;

        // GSM_TCP_DeactivPDP(u8DeactPDP_ID);
        // QtEC21_TCP_DeactivPDP(u8DeactPDP_ID);

        // tcp_pdpContext[u8DeactPDP_ID].bActiv = FALSE;

        // tcp_pdpContext[u8DeactPDP_ID].bInUse = FALSE;

        // tcpSocket[u8CurrTCP_Socket[0]].bConnect = FALSE;

        // if(bTCPIP_Connect[0])
        //{
        //   bTCP_CloseFlag[0] = TRUE;
        // }

        // u8CurrTCP_Socket[0] = TCP_SOCKET_NUM_MAX;

        // #if ENABLE_MULTI_SERVER
        // tcpSocket[u8CurrTCP_Socket[1]].bConnect = FALSE;

        // if(bTCPIP_Connect[1])
        //{
        //   bTCP_CloseFlag[1] = TRUE;
        //}

        // u8CurrTCP_Socket[1] = TCP_SOCKET_NUM_MAX;
        // #endif

        // bFTP_Connect = FALSE;

        break;
    }

    case GSM_EVENT_NO_CARRIER:
    {
        // GSMprintf("\r\n> Call disconnected.");

        break;
    }

    default:
        break;
    }
}

//============================================================================//
// @brief  Handle the GSM data transmission.
// @param  None.
// @retval None.
//============================================================================//
void GSM_TX_Proc(void)
{
    GSM_BlockCmdHandle();

    taskInfo[TASK_GSM_TX].u16TaskStep++;

    GSM_NonBlockCmdHandle();
}

//============================================================================//
// @brief  Handle the GSM functions at low-level.
// @param  None.
// @retval None.
//============================================================================//
void GSM_LL_Proc(void)
{
    GSM_TX_Proc();

    taskInfo[TASK_GSM_RX].u16TaskStep++;

    HAL_Delay(1);

    GSM_RX_Proc();
}

//============================================================================//
// @brief  Check to directly report an event when an emergency failure occurs.
// @param  None.
// @retval None.
//============================================================================//
void GSM_CheckEmergEvent(void)
{
    bool bRes;

    if ((bTCPIP_Connect[SOCKET_BANK_DEF] == FALSE) || (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ < 8) ||
        (bGSM_PwrOffFlag == TRUE) ||
        (bGSM_ResetFlag == TRUE) || (bGSM_ResetCheckFTP_Flag == TRUE))
        return;

#if ENABLE_MG82
    if (bMG82_ReportErrFlag == TRUE)
    {
        bRes = FALSE;

        bRes = GSM_TCP_SendEVENT_Direct(EVENT_I2C2_FAIL, (char *)"1", SOCKET_BANK_DEF);

        if ((bRes == TRUE) || (HAL_GetTick() >= (u32MG82_ReportErrLastTick + MG82_REPORT_ERR_TIMEOUT)))
        {
            bMG82_ReportErrFlag = FALSE;
        }
    }
#endif
#if ENABLE_RX8130
    if (bRX8130_ReportErrFlag == TRUE)
    {
        bRes = FALSE;

        bRes = GSM_TCP_SendEVENT_Direct(EVENT_I2C2_FAIL, (char *)"2", SOCKET_BANK_DEF);

        if ((bRes == TRUE) || (HAL_GetTick() >= (u32RX8130_ReportErrLastTick + RX8130_REPORT_ERR_TIMEOUT)))
        {
            bRX8130_ReportErrFlag = FALSE;
        }
    }
#else
    if (bPCF_ReportErrFlag == TRUE)
    {
        bRes = FALSE;

        bRes = GSM_TCP_SendEVENT_Direct(EVENT_I2C2_FAIL, (char *)"3", SOCKET_BANK_DEF);

        if ((bRes == TRUE) || (HAL_GetTick() >= (u32PCF_ReportErrLastTick + PCF_REPORT_ERR_TIMEOUT)))
        {
            bPCF_ReportErrFlag = FALSE;
        }
    }
#endif

    if (bSD_ReportErrFlag == TRUE)
    {
        bRes = FALSE;

        bRes = GSM_TCP_SendEVENT_Direct(EVENT_SD_FAIL, (char *)"4", SOCKET_BANK_DEF);

        if ((bRes == TRUE) || (HAL_GetTick() >= (u32SD_ReportErrLastTick + SD_REPORT_ERR_TIMEOUT)))
        {
            if (bMX25L_ReportErrFlag == FALSE)
            {
                System_Stop();

                bQtEC21_InitFinish = FALSE;

                HAL_Delay(20000);
            }

            HAL_Delay(1000);

            bSD_ReportErrFlag = FALSE;
        }
    }

    if (bMX25L_ReportErrFlag == TRUE)
    {
        bRes = FALSE;

        bRes = GSM_TCP_SendEVENT_Direct(EVENT_MX25L_FAIL, (char *)"5", SOCKET_BANK_DEF);

        if ((bRes == TRUE) || (HAL_GetTick() >= (u32MX25L_ReportErrLastTick + MX25L_REPORT_ERR_TIMEOUT)))
        {
            System_Stop();

            bQtEC21_InitFinish = FALSE;

            HAL_Delay(20000);

            bMX25L_ReportErrFlag = FALSE;
        }
    }
}

//============================================================================//
// @brief  Check if all FTP functions are idle.
// @param  None.
// @retval TRUE: All FTP functions are idle.
//         FALSE: There is one FTP function running.
//============================================================================//
static bool GSM_FTP_CheckIdle(void)
{
    if ((procGSM_FOTA_Step == FOTA_STEP_NULL) &&
#if ENABLE_FTP_DOWNLOAD
        (procGSM_Download_Step == DOWNLOAD_STEP_NULL) &&
#endif
#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
        //(procXTRA_Download_Step == XTRA_STEP_NULL) &&
        (procXTRA_Update_Step == XTRA_UPDATE_STEP_NULL) &&
#endif
#if ENABLE_UPLOAD_CHK
        (procGSM_UploadChk_Step == UPLOAD_CHK_STEP_NULL) &&
#endif
        (procGSM_Upload_Step == UPLOAD_STEP_NULL) &&
        (procHTTP_Download_Step == HTTP_DOWNLOAD_STEP_NULL))
    {
        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  In stealth mode, if there is any FTP function request or on-going FTP
//         operation refresh the waiting enter standby timer every 10 seconds.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_FTP_CheckEnterStandby(void)
{
    static uint32_t u32RefreshTick = 0;

#if ENABLE_FOTA_ACC_OFF
    if (((bSystemMode == SYSTEM_MODE_NORMAL) && (bWaitingEnterStandby == FALSE)) ||
        (tcp_pdpContext[u8CurrPDP_ID].bActiv == FALSE) ||
        (g_SysInfo.tVehicleState.bPWR_ON == FALSE))
#else
    if ((bSystemMode == SYSTEM_MODE_NORMAL) ||
        (tcp_pdpContext[u8CurrPDP_ID].bActiv == FALSE) ||
        (bWaitingEnterStandby == FALSE))
#endif
    {
        return;
    }

    if (HAL_GetTick() >= (u32RefreshTick + S_2_mS(10)))
    {
        if ((procGSM_FOTA_Step != FOTA_STEP_NULL) ||
            (procGSM_Download_Step != DOWNLOAD_STEP_NULL) ||
            (procGSM_Upload_Step != UPLOAD_STEP_NULL))
        {
            RTOSprintf("\r\n> GSM FTP refresh enter standby timer.");

            // osTimerStop(timerACC_OffHandle);

            // osTimerStart(timerACC_OffHandle, S_2_mS(30));

            u32PwrDownDelayTick = HAL_GetTick();
        }

        u32RefreshTick = HAL_GetTick();
    }
}

//============================================================================//
// @brief  Process the FTP tasks.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_FTP_Proc(void)
{
    if ((bGSM_ResetFlag == TRUE) ||
        (bGSM_PwrOffFlag == TRUE) ||
        (bGSM_ResetCheckFTP_Flag == TRUE))
        return;

    GSM_FTP_CheckEnterStandby();

    // If the PDP context has been activated and this function currently is not delayed
    // and the TCP connection has been established
    if ((tcp_pdpContext[u8CurrPDP_ID].bActiv == TRUE) && (FuncSkip_IsEnable(FUNC_GSM_FTP) == FALSE))
    //(((bTCPIP_Connect[0] == TRUE) && (bSystemMode == SYSTEM_MODE_NORMAL)) ||
    //(bSystemMode == SYSTEM_MODE_STEALTH)))
    {
        // If the current CSQ is good enough check for
        // FTP fucntions initialization
        if (bCSQ_IsGood == TRUE)
        {
// FOTA init
#if ENABLE_FOTA_ACC_OFF
            if ((g_Setting.tUpgrade.bFOTA_Flag == TRUE) &&
                (bMX25L_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (((bSystemMode == SYSTEM_MODE_NORMAL) && ((bWaitingEnterStandby == TRUE) || (g_Setting.tUpgrade.bUpgradeNow == TRUE))) ||
                 ((bSystemMode == SYSTEM_MODE_STEALTH) && (bStealthFOTA_En == TRUE))))
#else
            if ((g_Setting.tUpgrade.bFOTA_Flag == TRUE) &&
                (bMX25L_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (((bSystemMode == SYSTEM_MODE_NORMAL) && (g_SysInfo.tVehicleState.bACC_ON == TRUE)) ||
                 ((bSystemMode == SYSTEM_MODE_STEALTH) && (bStealthFOTA_En == TRUE))))
#endif
            {
                procGSM_FOTA_Step = FOTA_STEP_CFG_PDP;

                g_Setting.tUpgrade.bFOTA_Flag = FALSE;

                System_SaveSetting();
            }

// FTP download init
#if ENABLE_FTP_DOWNLOAD
            if ((u16DownloadFileNum > 0) &&
                (bSD_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (((bSystemMode == SYSTEM_MODE_NORMAL) && (g_SysInfo.tVehicleState.bACC_ON == TRUE)) ||
                 ((bSystemMode == SYSTEM_MODE_STEALTH) && (bStealthDownloadEn == TRUE))))
            {
                GSM_FTP_FileDownloadInit();
            }
#endif

            // FTP upload init
            if ((u16UnsendFileNum > 0) &&
                (bSD_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (((bSystemMode == SYSTEM_MODE_NORMAL) && (g_SysInfo.tVehicleState.bACC_ON == TRUE)) ||
                 ((bSystemMode == SYSTEM_MODE_STEALTH) && (bStealthUploadEn == TRUE))))
            {
                GSM_FTP_FileUploadInit();
            }

// FTP upload check init
#if ENABLE_UPLOAD_CHK
            if ((tUploadChkHandle.u16Size > 0) &&
                (u16UnsendFileNum == 0) &&
                (bSD_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (bSystemMode == SYSTEM_MODE_NORMAL) &&
                (g_SysInfo.tVehicleState.bACC_ON == TRUE))
            {
                GSM_FTP_InitUploadChk();

                // bDoUploadChk = FALSE;
            }
#endif

            if ((tHTTP_DlRequest.bRequest == TRUE) &&
                (bSD_ReportErrFlag == FALSE) &&
                (GSM_FTP_CheckIdle() == TRUE) &&
                (bSystemMode == SYSTEM_MODE_NORMAL) &&
                (g_SysInfo.tVehicleState.bACC_ON == TRUE))
            {
                GSM_HTTP_DownloadInit();
            }

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
// XTRA download init
/*
if((tGPS_Xtra.bGPS_XTRA_Update == TRUE) &&
     (GSM_FTP_CheckIdle() == TRUE) &&
   (bSystemMode == SYSTEM_MODE_NORMAL) &&
   (g_SysInfo.tVehicleState.bACC_ON == TRUE))
{
    procXTRA_Download_Step = XTRA_STEP_HTTP_CFG_PDP;

  tGPS_Xtra.bGPS_XTRA_Update = FALSE;
}
*/
#endif
        }

        // FOTA proc
        if ((bMX25L_ReportErrFlag == FALSE) &&
            (procGSM_FOTA_Step != FOTA_STEP_NULL))
        {
            HAL_Delay(1);
            GSM_FOTA_Proc();
        }

// FTP download proc
#if ENABLE_FTP_DOWNLOAD
        if ((bSD_ReportErrFlag == FALSE) &&
            (procGSM_Download_Step != DOWNLOAD_STEP_NULL))
        {
            HAL_Delay(1);

            GSM_FTP_Download_Proc();
        }
#endif

        // FTP upload proc
        if ((bSD_ReportErrFlag == FALSE) &&
            (procGSM_Upload_Step != UPLOAD_STEP_NULL))
        {
            HAL_Delay(1);

            GSM_FTP_Upload_Proc();
        }

// FTP upload check proc
#if ENABLE_UPLOAD_CHK
        if ((bSD_ReportErrFlag == FALSE) &&
            (procGSM_UploadChk_Step != UPLOAD_CHK_STEP_NULL))
        {
            HAL_Delay(1);

            GSM_FTP_UploadChk_Proc();
        }
#endif

        if ((bSD_ReportErrFlag == FALSE) &&
            (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_NULL))
        {
            HAL_Delay(1);

            GSM_HTTP_Download_Proc();
        }

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
        /*
        if(procXTRA_Download_Step != XTRA_STEP_NULL)
        {
          HAL_Delay(1);

            GSM_XTRA_Download_Proc();
        }
        */

        if (procXTRA_Update_Step != XTRA_UPDATE_STEP_NULL)
        {
            HAL_Delay(1);

            GSM_XTRA_Update_Proc();
        }
#endif
    }
}

//============================================================================//
// @brief  Process the TCP resend task.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_TCP_Resend_Proc(void)
{
    bool bRes = FALSE;

    if ((bSD_ReportErrFlag == TRUE) ||
        (bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE) ||
        (bGSM_ResetFlag == TRUE) || (bGSM_ResetCheckFTP_Flag == TRUE) ||
        (GSM_IsBusy() == TRUE) ||
        (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ < 8))
    {
        return;
    }

    for (int u8Dst = 0; u8Dst < MAX_SOCKET_BANK; u8Dst++)
    {
        uint16_t u16PkgLen;
        MsgSendStatusTypeDef smsRes;
        GSM_SocketDataTypeDef gsmTCP_Data;
        uint16_t u16TempPkgIdx;

        if ((u16UnsendPkgNum[u8Dst] == 0) ||
            (bTCPIP_Connect[u8Dst] == FALSE))
        {
            continue;
        }

        if (bRTOS_En)
        {
            osStatus osRes;

            // Acquire GSM_SD synchronization
            osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

            if (osRes != osOK)
            {
                return;
            }
        }

        u16TempPkgIdx = u16UnsendPkgNum[u8Dst];

        bRes = GSM_GetTempPkg(u8GSM_TempPkgBuf, &u16PkgLen, u8Dst);

        if (bRTOS_En)
            osMutexRelease(mutex_gsm_sd_user);

        if (bRes)
        {
            // GSMprintf("\r\n> GSM resend get unsent packet %d for server %d OK.", u16TempPkgIdx, u8Dst);

            if (u16PkgLen > 0)
            {
                // If the server has a valid IP address
                if ((strlen(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr) >= 7) &&
                    (strlen(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr) <=
                     sizeof(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr)))
                {
                    gsmTCP_Data.SocketNum = u8CurrTCP_Socket[u8Dst];
                    gsmTCP_Data.pcData = (char *)u8GSM_TempPkgBuf;
                    gsmTCP_Data.u16DataLen = u16PkgLen;

                    smsRes = GSM_TCP_Send(&gsmTCP_Data);

                    if (smsRes != SEND_OK)
                    {
                        GSMprintf("\r\n> GSM send packet %d-%d ERROR.\r\n", u8Dst, u16TempPkgIdx);

                        bRes = FALSE;

                        u8SendLogFailCnt[u8Dst]++;

                        if (u8SendLogFailCnt[u8Dst] >= GSM_SEND_LOG_RETRY_MAX)
                        {
                            u8SendLogFailCnt[u8Dst] = 0;

                            if (bTCPIP_Connect[u8Dst])
                            {
                                char cLog[30] = {0};

                                sprintf(cLog, "Conn %d close flag ON(2)", u8Dst);

                                GPRS_DebugLog(cLog, strlen(cLog));

                                bTCP_CloseFlag[u8Dst] = TRUE;
                            }
                        }
                    }
                    else
                    {
                        uint16_t u16UnAckBytes;
                        uint8_t u8QueryCnt = 0;

                        // GSMprintf("\r\n> GSM send packet %d-%d OK.", u8Dst, u16TempPkgIdx);

                        while (u8QueryCnt < 5)
                        {
                            HAL_Delay(1000);

                            if (bTCPIP_Connect[u8Dst] == FALSE)
                            {
                                bRes = FALSE;

                                break;
                            }

                            bRes = GSM_TCP_SendingStatus(u8CurrTCP_Socket[u8Dst], &u16UnAckBytes);

                            if (bRes)
                            {
                                if (u16UnAckBytes == 0)
                                {
                                    break;
                                }
                            }

                            u8QueryCnt++;
                        }

                        // GSMprintf("\r\n> Number of unsent bytes in connection socket %d: %d",
                        //           u8CurrTCP_Socket[u8Dst], u16UnAckBytes);

                        if (bRes)
                        {
                            if (u16UnAckBytes > 0)
                            {
                                // GSMprintf("\r\n> GSM send packet %d-%d has %d bytes unsent.\r\n",
                                //					u8Dst, u16TempPkgIdx, u16UnAckBytes);

                                bRes = FALSE;

                                u8SendLogFailCnt[u8Dst]++;

                                if (u8SendLogFailCnt[u8Dst] >= GSM_SEND_LOG_RETRY_MAX)
                                {
                                    u8SendLogFailCnt[u8Dst] = 0;

                                    if (bTCPIP_Connect[u8Dst])
                                    {
                                        char cLog[30] = {0};

                                        sprintf(cLog, "Conn %d close flag ON(3)", u8Dst);

                                        GPRS_DebugLog(cLog, strlen(cLog));

                                        bTCP_CloseFlag[u8Dst] = TRUE;
                                    }
                                }
                            }
                            else
                            {
                                GSMprintf("\r\n> GSM send packet %d-%d OK.\r\n", u8Dst, u16TempPkgIdx);
                            }
                        }
                    }

                    if (bRes == FALSE)
                    {
                        if (bRTOS_En)
                        {
                            osStatus osRes;

                            // Acquire GSM_SD synchronization
                            osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

                            if (osRes != osOK)
                            {
                                return;
                            }
                        }

                        for (int i = 0; i < 3; i++)
                        {
                            bRes = GSM_SaveTempPkg(u8GSM_TempPkgBuf, u16PkgLen, u8Dst);

                            if (bRes)
                                break;
                        }

                        if (bRTOS_En)
                            osMutexRelease(mutex_gsm_sd_user);
                    }
                }
            }
            else
            {
                GSMprintf("\r\n GSM packet %d-%d is in wrong format!\r\n", u8Dst, u16TempPkgIdx);
            }
        }
        else
        {
            GSMprintf("\r\n> GSM get packet %d-%d ERROR!\r\n", u8Dst, u16TempPkgIdx);
        }

        HAL_Delay(100);
    }

    /*
      #if ENABLE_MULTI_SERVER
      if(GSM_IsBusy() == FALSE)
      {
          bRes = FALSE;

          if((bTCPIP_Connect[SOCKET_BANK_DEF] == TRUE) && (bTCPIP_Connect[SOCKET_BANK_EXT] == TRUE))
              bRes = TRUE;

          if((bTCPIP_Connect[SOCKET_BANK_DEF] == TRUE) &&
               (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr) < 7) &&
               (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr) >
                  sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr)))
              bRes = TRUE;

          if((bTCPIP_Connect[SOCKET_BANK_EXT] == TRUE) &&
               (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr) < 7) &&
               (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr) >
                  sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr)))
              bRes = TRUE;
      }
      #else
      if((GSM_IsBusy() == FALSE) && (bTCPIP_Connect[0] == TRUE))
      {
          bRes = TRUE;
      }
      #endif

      if(bRes == TRUE)
      {
          if(bRTOS_En)
          {
              osStatus osRes;


              // Acquire GSM_SD synchronization
              osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

              if(osRes != osOK)
              {
                  return;
              }
          }

          if((u16UnsendPkgNum > 0) && (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ > 7))
          {
              uint8_t u8Dst;
              uint16_t u16PkgLen;
              MsgSendStatusTypeDef smsRes;
              GSM_SocketDataTypeDef gsmTCP_Data;


              uint16_t u16TempPkgIdx = u16UnsendPkgNum;

              bRes = GSM_GetTempPkg(u8GSM_TempPkgBuf, &u16PkgLen, &u8Dst);

              if(bRTOS_En)
                  osMutexRelease(mutex_gsm_sd_user);

              //#if ENABLE_MULTI_SERVER
              //if((u8Dst == SOCKET_BANK_EXT) &&
              //	 (strcmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
              //           g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr) == 0) &&
              //	 (g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port ==
              //		g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port))
              //{
              //	GSMprintf("\r\n> GSM discard packet %d for server %d", u16TempPkgIdx, u8Dst);

              //  return;
              //}
              //#else
              #if ENABLE_MULTI_SERVER == FALSE
              if(u8Dst == SOCKET_BANK_EXT)
              {
                  GSMprintf("\r\n> GSM discard packet %d for server %d", u16TempPkgIdx, u8Dst);

                return;
              }
              #endif

              if(bRes)
              {
                  //GSMprintf("\r\n> GSM resend get unsent packet %d for server %d OK.", u16TempPkgIdx, u8Dst);

                  if(u16PkgLen > 0)
                  {
                      // If the server has a valid IP address
                      if((strlen(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr) >= 7) &&
                           (strlen(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr) <=
                              sizeof(g_Setting.tGSM_SET.tServerInfo[u8Dst].cIP_Addr)))
                      {
                          gsmTCP_Data.SocketNum = u8CurrTCP_Socket[u8Dst];
                          gsmTCP_Data.pcData = (char *)u8GSM_TempPkgBuf;
                          gsmTCP_Data.u16DataLen = u16PkgLen;

                          smsRes = GSM_TCP_Send(&gsmTCP_Data);

                          if(smsRes != SEND_OK)
                          {
                              GSMprintf("\r\n> GSM send packet %d to server %d ERROR.\r\n", u16TempPkgIdx, u8Dst);

                              bRes = FALSE;

                              u8SendLogFailCnt[u8Dst]++;

                              if(u8SendLogFailCnt[u8Dst] >= GSM_SEND_LOG_RETRY_MAX)
                              {
                                  u8SendLogFailCnt[u8Dst] = 0;

                  if(bTCPIP_Connect[u8Dst])
                                  {
                                    bTCP_CloseFlag[u8Dst] = TRUE;
                                  }
                              }
                          }
                          else
                          {
                              uint16_t u16UnAckBytes;
                              uint8_t u8QueryCnt = 0;


                              //GSMprintf("\r\n> GSM send packet %d to server %d OK.", u16TempPkgIdx, u8Dst);

                              while(u8QueryCnt < 5)
                              {
                                  HAL_Delay(1000);

                                  if(bTCPIP_Connect[u8Dst] == FALSE)
                                  {
                                      bRes = FALSE;

                                      break;
                                  }

                                  bRes = GSM_TCP_SendingStatus(u8CurrTCP_Socket[u8Dst], &u16UnAckBytes);

                                  if(bRes)
                                  {
                                      if(u16UnAckBytes == 0)
                                      {
                                          break;
                                      }
                                  }

                                  u8QueryCnt++;
                              }

                              //GSMprintf("\r\n> Number of unsent bytes in connection socket %d: %d",
                              //          u8CurrTCP_Socket[u8Dst], u16UnAckBytes);

                              if(bRes)
                              {
                                  if(u16UnAckBytes > 0)
                                  {
                                      //GSMprintf("\r\n> GSM send packet %d to server %d has %d bytes unsent.\r\n",
                                      //					u16TempPkgIdx, u8Dst, u16UnAckBytes);

                                      bRes = FALSE;

                                      u8SendLogFailCnt[u8Dst]++;

                                      if(u8SendLogFailCnt[u8Dst] >= GSM_SEND_LOG_RETRY_MAX)
                                      {
                                          u8SendLogFailCnt[u8Dst] = 0;

                      if(bTCPIP_Connect[u8Dst])
                                          {
                                            bTCP_CloseFlag[u8Dst] = TRUE;
                                          }
                                      }
                                  }
                                  else
                                  {
                                      GSMprintf("\r\n> GSM send packet %d to server %d OK.\r\n", u16TempPkgIdx, u8Dst);
                                  }
                              }
                          }

                          if(bRes == FALSE)
                          {
                              if(bRTOS_En)
                              {
                                  osStatus osRes;


                                  // Acquire GSM_SD synchronization
                                  osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

                                  if(osRes != osOK)
                                  {
                                      return;
                                  }
                              }

                              for(int i = 0; i < 3; i++)
                              {
                                  bRes = GSM_SaveTempPkg(u8GSM_TempPkgBuf, u16PkgLen, u8Dst);

                                  if(bRes)
                                      break;
                              }

                              if(bRTOS_En)
                                osMutexRelease(mutex_gsm_sd_user);
                          }
                      }
                  }
                  else
                  {
                      GSMprintf("\r\n GSM unsent packet %d is in wrong format!\r\n", u16TempPkgIdx);
                  }
              }
              else
              {
                  GSMprintf("\r\n> GSM get unsent packet %d FAIL!\r\n", u16TempPkgIdx);
              }

              HAL_Delay(100);
          }
          else
          {
            if(bRTOS_En)
                  osMutexRelease(mutex_gsm_sd_user);
          }
      }
      */
}

//============================================================================//
// @brief  Handling PDP context.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_PDP_ContextHandle(void)
{
    static uint8_t u8PDP_ActFailCnt = 0;

    if ((EXTIO_IsMainPowerNormal() == FALSE) &&
        (bSystemMode == SYSTEM_MODE_NORMAL))
        return;

    if ((bGSM_ResetFlag == TRUE) ||
        (bGSM_PwrOffFlag == TRUE) ||
        (bGSM_ResetCheckFTP_Flag == TRUE))
        return;

    // if(EXTIO_IsMainPowerNormal() == TRUE)
    {
        // Check if this function is currently delayed
        if (FuncSkip_IsEnable(FUNC_GSM_PDP) == TRUE)
            return;

        if ((tcp_pdpContext[u8CurrPDP_ID].bActiv == FALSE) && (procGSM_PDP_Step == PDP_STEP_NULL))
        {
            // If reach GSM_PDP_ACTIV_RETRY_MAX then must delay STOP_PDP_ACT_TIMEOUT seconds before making another retry
            // otherwise retry immediately
            if (u8PDP_ActFailCnt < 3)
            {
                // Initiate the process to activate PDP context
                procGSM_PDP_Step = PDP_STEP_QUERY_USIM;

                u8PDP_ActFailCnt++;
            }
            else
            {
                u8PDP_ActFailCnt = 0;

                g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt++;

                // Make a delay of STOP_PDP_ACT_TIMEOUT seconds
                GSMprintf("\r\n> GSM having issue to activate PDP context. Waiting %d seconds before trying again.",
                          STOP_PDP_ACT_TIMEOUT);

                FuncSkip_Enable(FUNC_GSM_PDP, STOP_PDP_ACT_TIMEOUT);
            }
        }
        else
        {
            if ((tcp_pdpContext[u8CurrPDP_ID].bActiv == TRUE) && (u8PDP_ActFailCnt > 0))
                u8PDP_ActFailCnt = 0;
        }

        // If the PDP context need to be deactivated
        if ((bPDP_DeactFlag == TRUE) &&
            (procGSM_PDP_Step == PDP_STEP_NULL))
        {
            for (uint8_t u8Conn = 0; u8Conn < MAX_SOCKET_BANK; u8Conn++)
            {
                // If the socket has been opened close it
                if (u8CurrTCP_Socket[u8Conn] != TCP_SOCKET_NUM_MAX)
                // if((bTCPIP_Connect[u8Conn] == TRUE) ||
                //	 (procGSM_TCP_Step[u8Conn] == CONN_STEP_OPEN) ||
                //    (procGSM_TCP_Step[u8Conn] == CONN_STEP_SEND_CHK))
                {
                    // osEvent event;
                    bool bRes;

                    HAL_Delay(10);

                    for (int i = 0; i < 3; i++)
                    {
                        bRes = GSM_TCP_CloseConn(u8CurrTCP_Socket[u8Conn], TCP_CLOSE_CONN_TIMEOUT + 10);

                        if (bRes)
                            break;
                    }

                    char cLog[30] = {0};

                    sprintf(cLog, "Conn %d close socket %d(4)",
                            u8Conn,
                            u8CurrTCP_Socket[u8Conn]);

                    GPRS_DebugLog(cLog, strlen(cLog));

                    tcpSocket[u8CurrTCP_Socket[u8Conn]].bConnect = FALSE;
                    u8CurrTCP_Socket[u8Conn] = TCP_SOCKET_NUM_MAX;
                }

                bTCPIP_Connect[u8Conn] = FALSE;
                procGSM_TCP_Step[u8Conn] = CONN_STEP_NULL;
            }

            if (tcp_pdpContext[u8CurrPDP_ID].bActiv == TRUE)
            {
                tcp_pdpContext[u8CurrPDP_ID].bActiv = FALSE;
                tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

                // Initiate the process to deactivate PDP context
                procGSM_PDP_Step = PDP_STEP_DEACT;
            }

            bPDP_DeactFlag = FALSE;
        }

        // Do the steps to activate or deactivate the PDP context
        if (procGSM_PDP_Step != PDP_STEP_NULL)
        {
            HAL_Delay(1);

            GSM_PDP_ActProc();
        }
    }
}

#if ENABLE_MULTI_SERVER
//============================================================================//
// @brief  In multi servers mode check if the IPs or DNSs of the servers are
//         duplicate.
// @param  None.
// @retval TRUE: The IPs/ DNSs of the servers are duplicate.
//         FALSE: The IPs/ DNSs of the servers are not duplicate.
//============================================================================//
bool GSM_TCP_CheckServDuplicate(void)
{
    // If at least 1 DNS are not set
    if ((strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName) == 0) ||
        (strncmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                 (char *)TCP_DNS_NAME_DISABLE,
                 strlen((char *)TCP_DNS_NAME_DISABLE)) == 0) ||
        (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) == 0) ||
        (strncmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                 (char *)TCP_DNS_NAME_DISABLE,
                 strlen((char *)TCP_DNS_NAME_DISABLE)) == 0))
    {
        // If both IPs and Ports are matched
        if ((strcmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
                    g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr) == 0) &&
            (g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port ==
             g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else // If both DNSs are set
    {
        // If both DNSs and Ports are matched
        if ((strcmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                    g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) == 0) &&
            (g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port ==
             g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
#endif

//============================================================================//
// @brief  Handling TCP connection.
// @param  None.
// @retval None.
//============================================================================//
static void GSM_TCP_Connection_Handle(void)
{
    bool bRes;

    if ((bSystemMode == SYSTEM_MODE_NORMAL) && (EXTIO_IsMainPowerNormal() == FALSE))
        return;

    // Wait for the PDP context to be activated
    if ((tcp_pdpContext[u8CurrPDP_ID].bActiv == FALSE) ||
        (bGSM_PwrOffFlag == TRUE) ||
        (bGSM_ResetFlag == TRUE) ||
        (bGSM_ResetCheckFTP_Flag == TRUE))
        return;

    // if(EXTIO_IsMainPowerNormal() == TRUE)
    {
        for (uint8_t u8Conn = 0; u8Conn < MAX_SOCKET_BANK; u8Conn++)
        {
            // If the server has no valid IP
            if ((strlen(g_Setting.tGSM_SET.tServerInfo[u8Conn].cIP_Addr) < 8) ||
                (strlen(g_Setting.tGSM_SET.tServerInfo[u8Conn].cIP_Addr) >
                 sizeof(g_Setting.tGSM_SET.tServerInfo[u8Conn].cIP_Addr)))
            {
                if (u8Conn == 0)
                {
                    if ((strlen((char *)SERVER_1_IP_ADDR) > 7) &&
                        (strlen((char *)SERVER_1_IP_ADDR) < sizeof(g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr)))
                    {
                        GSMprintf("\r\n> GSM server 0 IP: %s. Reset IP.", g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr);

                        memset(g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr, 0x00,
                               sizeof(g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr));

                        sprintf(g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr, "%s", (char *)SERVER_1_IP_ADDR);
                        g_Setting.tGSM_SET.tServerInfo[0].u16Port = SERVER_1_PORT;

                        System_SaveSetting();
                    }
                }
#if ENABLE_MULTI_SERVER
                else if (u8Conn == 1)
                {
                    if ((strlen((char *)SERVER_2_IP_ADDR) > 7) &&
                        (strlen((char *)SERVER_2_IP_ADDR) < sizeof(g_Setting.tGSM_SET.tServerInfo[u8Conn].cIP_Addr)))
                    {
                        GSMprintf("\r\n> GSM server 1 IP: %s. Reset IP.", g_Setting.tGSM_SET.tServerInfo[1].cIP_Addr);

                        memset(g_Setting.tGSM_SET.tServerInfo[1].cIP_Addr, 0x00,
                               sizeof(g_Setting.tGSM_SET.tServerInfo[1].cIP_Addr));

                        sprintf(g_Setting.tGSM_SET.tServerInfo[1].cIP_Addr, "%s", (char *)SERVER_2_IP_ADDR);
                        g_Setting.tGSM_SET.tServerInfo[1].u16Port = SERVER_2_PORT;

                        System_SaveSetting();
                    }
                }
#endif

                continue;
            }

            // If the server has no valid DNS
            if ((strlen(g_Setting.tGSM_SET.tServerInfo[u8Conn].cDomainName) == 0) ||
                (strlen(g_Setting.tGSM_SET.tServerInfo[u8Conn].cDomainName) >
                 sizeof(g_Setting.tGSM_SET.tServerInfo[u8Conn].cDomainName)))
            {
                if (u8Conn == 0)
                {
                    if ((strlen((char *)TCP_DNS_NAME_DEF) > 0) &&
                        (strlen((char *)TCP_DNS_NAME_DEF) < sizeof(g_Setting.tGSM_SET.tServerInfo[0].cDomainName)))
                    {
                        GSMprintf("\r\n> GSM server 0 DNS: %s. Reset DNS.", g_Setting.tGSM_SET.tServerInfo[0].cDomainName);

                        memset(g_Setting.tGSM_SET.tServerInfo[0].cDomainName, 0x00,
                               sizeof(g_Setting.tGSM_SET.tServerInfo[0].cDomainName));

                        sprintf(g_Setting.tGSM_SET.tServerInfo[0].cDomainName, "%s", (char *)TCP_DNS_NAME_DEF);
                        g_Setting.tGSM_SET.tServerInfo[0].u16Port = TCP_DNS_PORT_DEF;

                        System_SaveSetting();
                    }
                }
#if ENABLE_MULTI_SERVER
                else if (u8Conn == 1)
                {
                    if ((strlen((char *)TCP_DNS_NAME_DEF) > 0) &&
                        (strlen((char *)TCP_DNS_NAME_DEF) < sizeof(g_Setting.tGSM_SET.tServerInfo[1].cDomainName)))
                    {
                        GSMprintf("\r\n> GSM server 1 DNS: %s. Reset DNS.", g_Setting.tGSM_SET.tServerInfo[1].cDomainName);

                        memset(g_Setting.tGSM_SET.tServerInfo[1].cDomainName, 0x00,
                               sizeof(g_Setting.tGSM_SET.tServerInfo[1].cDomainName));

                        sprintf(g_Setting.tGSM_SET.tServerInfo[1].cDomainName, "%s", (char *)TCP_DNS_NAME_DEF);
                        g_Setting.tGSM_SET.tServerInfo[1].u16Port = TCP_DNS_PORT_DEF;

                        System_SaveSetting();
                    }
                }
#endif

                continue;
            }

            /*
            #if ENABLE_MULTI_SERVER
            // Check if the IP of the EXT server is in blocklist
            if(u8Conn == SOCKET_BANK_EXT)
            {
              for(int i = 0; i < (sizeof(tInvalidIP) / sizeof(IP_InvalidTypeDef)); i++)
                {
                    // If the server info does not have valid IP
                  if((strncmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, (char *)"NONE", 4) == 0) ||
                     (strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) == 0))
                    {
                        // If the IP is in blocklist
                      if(strncmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr,
                             tInvalidIP[i].cIP_Addr, sizeof(tInvalidIP[i].cIP_Addr)) == 0)
                        {
                            GSMprintf("\r\n> GSM server %d IP is in block list: %s:%d",
                                                SOCKET_BANK_EXT,
                                      g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr,
                                      g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port);

                            GSMprintf("\r\n> GSM change server %d IP to server %d IP", SOCKET_BANK_EXT, SOCKET_BANK_DEF);

                          memset(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, 0x00,
                                   sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr));

                            sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, "%s",
                                    g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr);

                            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr = g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr;

                            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;

                            System_SaveSetting();

                            break;
                        }
                    }
                }
            }
            #endif
            */

            if ((bTCP_CloseFlag[u8Conn] == TRUE) && (procGSM_TCP_Step[u8Conn] == CONN_STEP_NULL))
            {
                if (u8CurrTCP_Socket[u8Conn] != TCP_SOCKET_NUM_MAX)
                // if(bTCPIP_Connect[u8Conn] == TRUE)
                {
                    HAL_Delay(10);

                    for (int i = 0; i < 3; i++)
                    {
                        bRes = GSM_TCP_CloseConn(u8CurrTCP_Socket[u8Conn], TCP_CLOSE_CONN_TIMEOUT + 10);

                        if (bRes)
                            break;
                    }

                    char cLog[30] = {0};

                    sprintf(cLog, "Conn %d close socket %d(3)",
                            u8Conn,
                            u8CurrTCP_Socket[u8Conn]);

                    GPRS_DebugLog(cLog, strlen(cLog));

                    tcpSocket[u8CurrTCP_Socket[u8Conn]].bConnect = FALSE;
                    u8CurrTCP_Socket[u8Conn] = TCP_SOCKET_NUM_MAX;

                    HAL_Delay(100);
                }

                bTCPIP_Connect[u8Conn] = FALSE;
                procGSM_TCP_Step[u8Conn] = CONN_STEP_NULL;

                bTCP_CloseFlag[u8Conn] = FALSE;
            }

#if ENABLE_MULTI_SERVER
            // Only open server 1 connection if its address & port are different than server 2's address & port
            if ((u8Conn == SOCKET_BANK_EXT) &&
                ((GSM_TCP_CheckServDuplicate() == TRUE) || (g_Setting.tGSM_SET.bDisableSocketEXT == TRUE)))
            {
                if (bTCPIP_Connect[SOCKET_BANK_EXT] == TRUE)
                {
                    bTCP_CloseFlag[SOCKET_BANK_EXT] = TRUE;
                }

                if (u16UnsendPkgNum[SOCKET_BANK_EXT] > 0)
                {
                    char cFileName[64] = {0};

                    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_EXT_TEMP_FILE);

                    u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

                    for (int i = 0; i < 3; i++)
                    {
                        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);

                        if (bRes)
                        {
                            break;
                        }
                    }
                }

                continue;
            }
#endif

            // Check if this function is currently delayed
            if (u8Conn == SOCKET_BANK_DEF)
            {
                if (FuncSkip_IsEnable(FUNC_GSM_TCP_SERVER_DEF) == TRUE)
                    continue;
            }
            else if (u8Conn == SOCKET_BANK_EXT)
            {
                if (FuncSkip_IsEnable(FUNC_GSM_TCP_SERVER_EXT) == TRUE)
                    continue;
            }

            if ((bTCPIP_Connect[u8Conn] == FALSE) && (procGSM_TCP_Step[u8Conn] == CONN_STEP_NULL))
            {
                // If reach TCP_CONNECT_RETRY_MAX then must delay STOP_TCP_CONNECT_TIMEOUT seconds before making another retry
                // otherwise retry immediately
                if (u8TCP_ConnectRetryCnt[u8Conn] < TCP_CONNECT_RETRY_MAX)
                {
                    HAL_Delay(3);

                    // If the socket has not been closed
                    if (u8CurrTCP_Socket[u8Conn] != TCP_SOCKET_NUM_MAX)
                    {
                        HAL_Delay(10);

                        for (int i = 0; i < 3; i++)
                        {
                            bRes = GSM_TCP_CloseConn(u8CurrTCP_Socket[u8Conn], TCP_CLOSE_CONN_TIMEOUT + 10);

                            if (bRes)
                                break;
                        }

                        char cLog[30] = {0};

                        sprintf(cLog, "Conn %d close socket %d(2)",
                                u8Conn,
                                u8CurrTCP_Socket[u8Conn]);

                        GPRS_DebugLog(cLog, strlen(cLog));

                        tcpSocket[u8CurrTCP_Socket[u8Conn]].bConnect = FALSE;
                        u8CurrTCP_Socket[u8Conn] = TCP_SOCKET_NUM_MAX;

                        HAL_Delay(100);
                    }

                    // Initiate the process to open TCP connection
                    procGSM_TCP_Step[u8Conn] = CONN_STEP_SOCKET_SEL;

                    u8TCP_ConnectRetryCnt[u8Conn]++;
                }
                else
                {
                    u8TCP_ConnectRetryCnt[u8Conn] = 0;

                    g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt++;

                    // If the number of delay times has reached TCP_CONNECT_STOP_MAX then reset module GSM
                    // Otherwise make a delay of STOP_TCP_CONNECT_TIMEOUT seconds
                    if (g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt < TCP_CONNECT_STOP_MAX)
                    {
                        GSMprintf("\r\n> GSM having issue to open TCP/IP connection to server %d. Waiting %d seconds before trying again.",
                                  u8Conn,
                                  STOP_TCP_CONNECT_TIMEOUT);

                        if (u8Conn == SOCKET_BANK_DEF)
                        {
                            FuncSkip_Enable(FUNC_GSM_TCP_SERVER_DEF, STOP_TCP_CONNECT_TIMEOUT);
                        }
                        else
                        {
                            FuncSkip_Enable(FUNC_GSM_TCP_SERVER_EXT, STOP_TCP_CONNECT_TIMEOUT);
                        }
                    }
                    else
                    {
                        g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt = 0;

                        GSMprintf("\r\n> GSM having issue to open TCP/IP connection. Reset module QtEC21.");

                        // GSM_ResetWithCheckFTP();
                        bGSM_ResetCheckFTP_Flag = TRUE;
                    }

                    System_SaveSetting();
                }
            }

            // Do the steps to open TCP connection
            if (procGSM_TCP_Step[u8Conn] != CONN_STEP_NULL)
            {
                HAL_Delay(1);

                GSM_TCP_Connect_Proc(u8Conn);
            }
        }
    }
}

//============================================================================//
// @brief  Process the GSM task.
// @param  None.
// @retval None.
//============================================================================//
void GSM_Task_Proc(void)
{
    if (bGSM_PwrOnFlag == TRUE)
    {
        if (bRTOS_En)
            GSM_PwrEnable();
        else
            QtEC21_PwrEnable();

        bGSM_PwrOnFlag = FALSE;
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if (bGSM_PwrOffFlag == TRUE)
    {
        if (bRTOS_En)
            GSM_PwrDisable();
        else
            QtEC21_PwrDisable();

        procGSM_SetupStep = PROC_STEP_NULL;

        bGSM_PwrOffFlag = FALSE;
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if (bGSM_SetSleep == TRUE)
    {
        bGSM_SetSleep = FALSE;

        GSM_SetSleep();
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if (bGSM_WakeUp == TRUE)
    {
        bGSM_WakeUp = FALSE;

        QtEC21_WakeUp();
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if (bGSM_ResetFlag == TRUE)
    {
        if (bRTOS_En)
            GSM_Reset();
        else
            QtEC21_Reset();

        procGSM_SetupStep = PROC_STEP_NULL;

        bGSM_ResetFlag = FALSE;

        char cLog[20] = {0};

        sprintf(cLog, "GSM Reset");

        GPRS_DebugLog(cLog, strlen(cLog));
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if (bGSM_ResetCheckFTP_Flag == TRUE)
    {
        if (bRTOS_En)
            GSM_ResetWithCheckFTP();
        else
            QtEC21_ResetWithCheckFTP();

        procGSM_SetupStep = PROC_STEP_NULL;

        bGSM_ResetCheckFTP_Flag = FALSE;

        char cLog[20] = {0};

        sprintf(cLog, "GSM Reset Check FTP");

        GPRS_DebugLog(cLog, strlen(cLog));
    }

    if (bQtEC21_State == FALSE)
        return;

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if ((procGSM_SetupStep != PROC_STEP_NULL) && (bQtEC21_Ready == TRUE))
    {
        GSM_SetupProc();
    }

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    GSM_CheckEmergEvent();

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE) ||
        (g_SysInfo.tVehicleState.bPWR_ON == FALSE))
        return;

    GSM_FTP_Proc();

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    if ((g_SysInfo.tVehicleState.bACC_ON == 1) || (u8WkupSrc == MG82_WKUP_SRC_G_SEN))
    {
        GSM_TCP_Resend_Proc();

        taskInfo[TASK_GSM_PROC].u16TaskStep++;
    }

    if ((g_SysInfo.tVehicleState.bACC_ON == 1) ||
        (bSystemMode == SYSTEM_MODE_STEALTH))
    {
        GSM_SMS_SendProc();

        taskInfo[TASK_GSM_PROC].u16TaskStep++;
    }

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    if (bGPS_InitFlag == TRUE)
    {
        bGPS_InitFlag = FALSE;

        bGPS_DataFixed = FALSE;

        bGPS_FirstDetection = TRUE;

        GPS_Start();

#if GNSS_ENABLE_GET_LOC == FALSE
        HAL_Delay(10);

        GPS_EnableRecvMsg();

        GPS_EnableSendMsg();
#endif
    }
#endif

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    GSM_PDP_ContextHandle();

    taskInfo[TASK_GSM_PROC].u16TaskStep++;

    GSM_TCP_Connection_Handle();
}

//============================================================================//
// @brief  Poll for result of a non-block for a period of time.
// @param  gsmCmd: Command in LE910_NonBlockCMD_TypeDef.
//         u32Timeout: Result polling timeout in seconds.
//         pgsmRes: Pointer to a GSM_NonBlockResp_ResTypeDef variable to store the result.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool GSM_NonBlockCmd_CheckRes(QTEC21_NonBlockCMD_TypeDef gsmCmd, uint32_t u32Timeout,
                              GSM_NonBlockResp_ResTypeDef *pgsmRes, uint16_t *pu16ErrCode)
{
    bool bRes;
    osEvent event;
    QTEC21_NonBlockCMD_TypeDef gsmRecvCmd;
    GSM_NonBlockCMD_Res_Typedef gsmRecvRes;
    uint16_t u16RetCode1 = 0; //, u16RetCode2 = 0;
    uint32_t u32StartTick;
    bool bGotResponse = FALSE;

    if (pgsmRes != NULL)
        *pgsmRes = GSMRES_NULL;

    if (pu16ErrCode != NULL)
        *pu16ErrCode = 0;

    if (gsmCmd != QTEC21_FTP_CLOSE)
        u16GSM_ErrCode = 0;

    u32StartTick = HAL_GetTick();

    while (HAL_GetTick() < (u32StartTick + S_2_mS(u32Timeout)))
    {
        event = osMessagePeek(GSM_NonBlockResHandle, 1);

        if (event.status == osEventMessage)
        {
            gsmRecvCmd = (QTEC21_NonBlockCMD_TypeDef)((uint16_t)event.value.v >> 8);

            gsmRecvRes = (GSM_NonBlockCMD_Res_Typedef)(uint16_t)event.value.v;

            if (gsmRecvCmd == QTEC21_NONBLOCK_NULL)
            {
                event = osMessageGet(GSM_NonBlockResHandle, 1);

                event = osMessageGet(GSM_NonBlockResCodeHandle, 1);

                continue;
            }

            if (gsmRecvCmd != gsmCmd)
                continue;

            event = osMessageGet(GSM_NonBlockResHandle, 1);

            event = osMessagePeek(GSM_NonBlockResCodeHandle, 1);

            if (event.status == osEventMessage)
            {
                u16RetCode1 = (uint16_t)((uint32_t)event.value.v >> 16);

                // u16RetCode2 = (uint16_t)(uint32_t)event.value.v;

                if (gsmCmd != QTEC21_FTP_CLOSE)
                    u16GSM_ErrCode = u16RetCode1;

                event = osMessageGet(GSM_NonBlockResCodeHandle, 1);
            }

            bGotResponse = TRUE;

            if (gsmRecvRes == GSM_NONBLOCK_RES_SUCCESS)
            {
                bRes = TRUE;
            }
            else if (gsmRecvRes == GSM_NONBLOCK_RES_ERROR)
            {
                bRes = FALSE;
            }
            else if (gsmRecvRes == GSM_NONBLOCK_RES_NO_RESPONSE)
            {
                if (pgsmRes != NULL)
                    *pgsmRes = GSMRES_FAIL_REDO_LIMIT_RST;

                return FALSE;
            }
            else if (gsmRecvRes == GSM_EVENT_NULL)
            {
                if (pgsmRes != NULL)
                    *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

                return FALSE;
            }
            else
            {
                if (pgsmRes != NULL)
                    *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

                return FALSE;
            }

            break;
        }

        HAL_Delay(1);
    }

    if (bGotResponse == FALSE)
    {
        GSM_Handle_NonBlockRes_osEventTimeout();

        if (pgsmRes != NULL)
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

        return FALSE;
    }

    // If there is no variable to store error code then return immediately
    if (pgsmRes == NULL)
        return bRes;

    if (bRes == TRUE)
    {
        *pgsmRes = GSMRES_SUCCESS;
    }
    else
    {
        if (pu16ErrCode != NULL)
            *pu16ErrCode = u16RetCode1;

        // Check returned CME error code
        switch (u16RetCode1)
        {
        // No error
        case 0:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Invalid parameters
        case 552:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                // GSM_TCP_ReportEvent((char *)EVENT_TCP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                // GSM_TCP_ReportEvent((char *)EVENT_TCP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Create socket failed
        case 554:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Socket bind failed
        case 556:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Socket identitity has been used
        case 563:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // DNS busy
        case 564:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // DNS parse failed
        case 565:
        {
            // char cEventMsg[20] = {0};

            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_TCP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_DNS);

                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, "%s", TCP_DNS_NAME_DEF);

                //	g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = TCP_DNS_PORT_DEF;
                //}
                // else
                //{
                //  System_RestoreBackupSetting(VAR_GPRS_SERVER_PORT);
                //}

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_TCP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_DNS_2);

                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, "%s", TCP_DNS_NAME_DEF);
                //
                //	g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = TCP_DNS_PORT_DEF;
                // }
                // else
                //{
                //   System_RestoreBackupSetting(VAR_GPRS_SERVER_PORT_2);
                // }

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }

        // Socket connect failed
        case 566:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT_DELAY_RST;

            break;

        // FTP server blocked
        case 602:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // FTP server busy
        case 603:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // DNS parse failed
        case 604:
        {
            // char cEventMsg[20] = {0};

            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_FTP:
                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_FTP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_FTP_ADDR);

                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME);
                // }

                break;

#if ENABLE_MULTI_FTP_SERVER
            case CURR_SERVER_FTP_EXT:
                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_FTP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_FTP_ADDR_2);

                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME_EXT);
                // }

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }

        // Network error
        case 605:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Control connection closed
        case 606:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Data connection closed
        case 607:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Timeout error
        case 609:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Invalid parameter
        case 610:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_FTP:
                // GSM_TCP_ReportEvent((char *)EVENT_FTP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;

#if ENABLE_MULTI_FTP_SERVER
            case CURR_SERVER_FTP_EXT:
                // GSM_TCP_ReportEvent((char *)EVENT_FTP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // File error
        case 613:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Open data connection failed
        case 615:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Connection closed, transfer aborted
        case 616:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

            // Not logged in
        case 625:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

            // Requested action not taken
        case 627:
            *pgsmRes = GSMRES_FAIL_NO_REDO;

            break;

        // SSL authentication failed
        case 631:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        default:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Poll for result of a non-block function once.
// @param  gsmCmd: Command in LE910_NonBlockCMD_TypeDef.
//         pgsmRes: Pointer to a GSM_NonBlockResp_ResTypeDef variable to store the result.
// @retval TRUE: The result is obtained.
//         FALSE: The result is not available yet.
//============================================================================//
bool GSM_NonBlockCmd_CheckResOnce(QTEC21_NonBlockCMD_TypeDef gsmCmd,
                                  GSM_NonBlockResp_ResTypeDef *pgsmRes,
                                  uint16_t *pu16ErrCode)
{
    bool bRes;
    osEvent event;
    QTEC21_NonBlockCMD_TypeDef gsmRecvCmd;
    GSM_NonBlockCMD_Res_Typedef gsmRecvRes;
    uint16_t u16RetCode1 = 0; //, u16RetCode2 = 0;

    if (pgsmRes != NULL)
        *pgsmRes = GSMRES_NULL;

    if (pu16ErrCode != NULL)
        *pu16ErrCode = 0;

    if (gsmCmd != QTEC21_FTP_CLOSE)
        u16GSM_ErrCode = 0;

    event = osMessagePeek(GSM_NonBlockResHandle, 1);

    if (event.status == osEventMessage)
    {
        gsmRecvCmd = (QTEC21_NonBlockCMD_TypeDef)((uint16_t)event.value.v >> 8);

        gsmRecvRes = (GSM_NonBlockCMD_Res_Typedef)(uint16_t)event.value.v;

        // GSMprintf("\r\n> Receive NonBlock message for command: %d", gsmRecvCmd);

        if (gsmRecvCmd == QTEC21_NONBLOCK_NULL)
        {
            event = osMessageGet(GSM_NonBlockResHandle, 1);

            event = osMessageGet(GSM_NonBlockResCodeHandle, 1);

            return FALSE;
        }

        if (gsmRecvCmd != gsmCmd)
        {
            return FALSE;
        }

        event = osMessageGet(GSM_NonBlockResHandle, 1);

        event = osMessagePeek(GSM_NonBlockResCodeHandle, 1);

        if (event.status == osEventMessage)
        {
            u16RetCode1 = (uint16_t)((uint32_t)event.value.v >> 16);

            // u16RetCode2 = (uint16_t)(uint32_t)event.value.v;

            if (gsmCmd != QTEC21_FTP_CLOSE)
                u16GSM_ErrCode = u16RetCode1;

            event = osMessageGet(GSM_NonBlockResCodeHandle, 1);
        }

        if (gsmRecvRes == GSM_NONBLOCK_RES_SUCCESS)
        {
            bRes = TRUE;
        }
        else if (gsmRecvRes == GSM_NONBLOCK_RES_ERROR)
        {
            bRes = FALSE;
        }
        else if (gsmRecvRes == GSM_NONBLOCK_RES_NO_RESPONSE)
        {
            if (pgsmRes != NULL)
                *pgsmRes = GSMRES_FAIL_REDO_LIMIT_RST;

            return TRUE;
        }
        else if (gsmRecvRes == GSM_EVENT_NULL)
        {
            if (pgsmRes != NULL)
                *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            return TRUE;
        }
        else
        {
            if (pgsmRes != NULL)
                *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    // If there is no variable to store error code then return immediately
    if (pgsmRes == NULL)
        return TRUE;

    if (bRes == TRUE)
    {
        *pgsmRes = GSMRES_SUCCESS;
    }
    else
    {
        if (pu16ErrCode != NULL)
            *pu16ErrCode = u16RetCode1;

        // Check returned CME error code
        switch (u16RetCode1)
        {
        // No error
        case 0:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Invalid parameters
        case 552:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                // GSM_TCP_ReportEvent((char *)EVENT_TCP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                // GSM_TCP_ReportEvent((char *)EVENT_TCP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        case 554:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Socket bind failed
        case 556:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Socket identitity has been used
        case 563:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                bChangeSocketFlag[SOCKET_BANK_DEF] = TRUE;

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                bChangeSocketFlag[SOCKET_BANK_EXT] = TRUE;

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // DNS busy
        case 564:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // DNS parse failed
        case 565:
        {
            // char cEventMsg[20] = {0};

            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_TCP:
                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_TCP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_DNS);

                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, "%s", TCP_DNS_NAME_DEF);

                //	g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = TCP_DNS_PORT_DEF;
                //}
                // else
                //{
                //  System_RestoreBackupSetting(VAR_GPRS_SERVER_PORT);
                //}

                break;

#if ENABLE_MULTI_SERVER
            case CURR_SERVER_TCP_EXT:
                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_TCP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_DNS_2);

                // if(strlen(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, "%s", TCP_DNS_NAME_DEF);

                //	g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = TCP_DNS_PORT_DEF;
                //}
                // else
                //{
                //  System_RestoreBackupSetting(VAR_GPRS_SERVER_PORT_2);
                //}

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }

        // Socket connect failed
        case 566:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT_DELAY_RST;

            break;

        // FTP server blocked
        case 602:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // FTP server busy
        case 603:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // DNS parse failed
        case 604:
        {
            // char cEventMsg[20] = {0};

            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_FTP:
                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_FTP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_FTP_ADDR);

                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME);
                // }

                break;

#if ENABLE_MULTI_FTP_SERVER
            case CURR_SERVER_FTP_EXT:
                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr) < 16)
                //{
                //	sprintf(cEventMsg, "%d:%s", gsmCurrServerType, g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
                // }
                // else
                //{
                //	sprintf(cEventMsg, "%d: ", gsmCurrServerType);

                //	memcpy(&cEventMsg[3], g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, 16);
                //}

                // GSM_TCP_ReportEvent((char *)EVENT_FTP_DNS_ERR, cEventMsg);

                // System_RestoreBackupSetting(VAR_GPRS_FTP_ADDR_2);

                // if(strlen(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr) == 0)
                //{
                //   sprintf(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME_EXT);
                // }

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }

        // Network error
        case 605:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Control connection closed
        case 606:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Data connection closed
        case 607:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Timeout error
        case 609:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Invalid parameter
        case 610:
            switch ((int)gsmCurrServerType)
            {
            case CURR_SERVER_FTP:
                // GSM_TCP_ReportEvent((char *)EVENT_FTP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;

#if ENABLE_MULTI_FTP_SERVER
            case CURR_SERVER_FTP_EXT:
                // GSM_TCP_ReportEvent((char *)EVENT_FTP_PARAM_ERR, (char *)(gsmCurrServerType + 0x30));

                break;
#endif

            default:
                break;
            }

            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // File error
        case 613:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        // Open data connection failed
        case 615:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        // Connection closed, transfer aborted
        case 616:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

            // Not logged in
        case 625:
            *pgsmRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

            // Requested action not taken
        case 627:
            *pgsmRes = GSMRES_FAIL_NO_REDO;

            break;

        // SSL authentication failed
        case 631:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;

        default:
            *pgsmRes = GSMRES_FAIL_REDO_LIMIT;

            break;
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Handle the PDP context activation process.
// @param  None.
// @retval Result in TCP_StatusTypeDef format.
//============================================================================//
TCP_StatusTypeDef GSM_PDP_ActProc(void)
{
    TCP_StatusTypeDef tcpRes = TCP_OK;
    bool bRes = FALSE;
    static uint32_t u32PDP_LastTick = 0;
    static uint8_t u8PDP_ActRetryCnt;

    if (procGSM_PDP_Step == PDP_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    switch ((int)(procGSM_PDP_Step))
    {
    // PDP_STEP_QUERY_USIM: Query USIM
    case PDP_STEP_QUERY_USIM:
    {
        GSM_SIM_StatTypedef simStat;

        tcp_pdpContext[u8CurrPDP_ID].bInUse = TRUE;

        // Query USIM status
        for (int i = 0; i < 3; i++)
        {
            bRes = GSM_SIM_QueryUSIM(&simStat);

            if (bRes)
                break;
        }

        // Reset QtEC21 module if failed to query USIM status
        if (bRes == TRUE)
        {
            if (g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt > 0)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;

                System_SaveSetting();
            }
        }
        else
        {
            procGSM_PDP_Step = PDP_STEP_NULL;

            tcpRes = TCP_NO_USIM;

            g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt++;

            // GSMprintf("\r\n> GSM SIM Error: %d", g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt);

            if (g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt >= GSM_SIM_QUERY_MAX)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;

                bQtEC21_State = FALSE;

                RTOSprintf("\r\n> GSM PDP ACT SIM ERROR! Maximum QtEC21 reset reached. Stop GSM functions!");

                LCD_SetShowAutoTestPage(51);

                char cLog[30] = {0};

                sprintf(cLog, "SIM ERROR(2)");

                GPRS_DebugLog(cLog, strlen(cLog));
            }
            else
            {
                if ((bQtEC21_Ready == TRUE) &&
                    (bGSM_ResetFlag == FALSE) &&
                    (bGSM_ResetCheckFTP_Flag == FALSE))
                {
                    GSMprintf("\r\n> GSM PDP ACT SIM ERROR! Reset GSM module.");

                    HAL_Delay(S_2_mS(1));

                    // GSM_ResetWithCheckFTP();
                    bGSM_ResetCheckFTP_Flag = TRUE;
                }
            }

            System_SaveSetting();

            break;
        }

        procGSM_PDP_Step = PDP_STEP_QUERY_CS;

        tcpRes = TCP_OK;

        u32PDP_LastTick = HAL_GetTick();

        break;
    }

    // PDP_STEP_QUERY_CS: Query CS service
    case PDP_STEP_QUERY_CS:
    {
        if (HAL_GetTick() < (u32PDP_LastTick + S_2_mS(GSM_CREG_QUERY_TIMEOUT)))
        {
            // Query CS service
            bRes = GSM_Net_QueryCS();

            if (!bRes)
            {
                // GSMprintf("\r\n> PDP ACT query CS again");

                procGSM_PDP_Step = PDP_STEP_QUERY_CS;

                tcpRes = TCP_OK;

                break;
            }
        }
        else
        {
            tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

            GSMprintf("\r\n> GSM PDP ACT Query CS ERROR! Reset GSM module.");

            char cLog[30] = {0};

            sprintf(cLog, "CS ERROR");

            GPRS_DebugLog(cLog, strlen(cLog));

            // GSM_ResetWithCheckFTP();
            bGSM_ResetCheckFTP_Flag = TRUE;

            procGSM_PDP_Step = PDP_STEP_NULL;

            tcpRes = TCP_NO_CS;

            break;
        }

        if (bMCU_RTC_Updated == FALSE)
        {
            char cRespBuf[64] = {0};
            struct tm tDateTime;
            RTC_DateTypeDef rtcDate;
            RTC_TimeTypeDef rtcTime;
            time_t ttTime;
            HAL_StatusTypeDef halRes;
            osStatus osRes;

            memset(cRespBuf, 0x00, sizeof(cRespBuf));

            bRes = GSM_Net_GetTime(2, cRespBuf); // Get in local time

            if (bRes)
            {
                GSMprintf("\r\n> GSM synchronized time: %s", cRespBuf);

                StopChar(cRespBuf, '+');

                if (sscanf(cRespBuf, "\"%d/%d/%d,%d:%d:%d",
                           &tDateTime.tm_year,
                           &tDateTime.tm_mon,
                           &tDateTime.tm_mday,
                           &tDateTime.tm_hour,
                           &tDateTime.tm_min,
                           &tDateTime.tm_sec) == 6)
                {
                    if (tDateTime.tm_year > 2000)
                        rtcDate.Year = tDateTime.tm_year - 2000;
                    else
                        rtcDate.Year = tDateTime.tm_year;
                    rtcDate.Month = tDateTime.tm_mon;
                    rtcDate.Date = tDateTime.tm_mday;
                    rtcTime.Hours = tDateTime.tm_hour;
                    rtcTime.Minutes = tDateTime.tm_min;
                    rtcTime.Seconds = tDateTime.tm_sec;
                    rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                    rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

                    tDateTime.tm_year -= 1900;
                    tDateTime.tm_mon -= 1;

                    ttTime = mktime(&tDateTime);

                    localtime_r(&ttTime, &tDateTime);

                    rtcDate.WeekDay = tDateTime.tm_wday;

                    osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

                    if (osRes == osOK)
                    {
                        halRes = HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                        // GSMprintf("\r\n> GSM set date result: %d", halRes);

                        if (halRes == HAL_OK)
                        {
                            halRes = HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

                            // GSMprintf("\r\n> GSM set time result: %d", halRes);
                        }

                        osMutexRelease(mutex_ttnow_user);

                        // HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
                        // HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                        // GSMprintf("\r\n> GSM set date time:%s, %02d/%02d/%02d, %02d:%02d:%02d",
                        //            cWeekDay[rtcDate.WeekDay], rtcDate.Year, rtcDate.Month, rtcDate.Date,
                        //            rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);

                        if (halRes == HAL_OK)
                        {
                            bMCU_RTC_Updated = TRUE;

                            RTOSprintf("\r\n> ### MCU RTC TIME VALID ###");

#if ENABLE_MG82_RTC
                            if (bMG82_RTC_EN == TRUE)
                                bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
                            if (bRX8130_EN == TRUE)
                                bRX8130_EN = FALSE;
#else
                            if (g_bPCF_EN == TRUE)
                                g_bPCF_EN = FALSE;
#endif
                        }
                    }

#if ENABLE_MG82_RTC
                    MG82_RTC_SetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
                    RX8130_SetDateTime(&rtcDate, &rtcTime, 1);
#else
                    PCF_SetDate(&rtcDate, 1);
                    PCF_SetTime(&rtcTime, 1);
#endif

                    HAL_Delay(10);

                    if (bSystemTimeReady == FALSE)
                        bSystemTimeReady = TRUE;
                }
            }

            // memset(cRespBuf, 0x00, sizeof(cRespBuf));

            // GSM_HW_GetRTC(cRespBuf);

            // GSMprintf("\r\n> GSM RTC time: %s", cRespBuf);
        }

        procGSM_PDP_Step = PDP_STEP_QUERY_PS;

        tcpRes = TCP_OK;

        u32PDP_LastTick = HAL_GetTick();

        break;
    }

    // PDP_STEP_QUERY_PS: Query PS service
    case PDP_STEP_QUERY_PS:
    {
        if (HAL_GetTick() < (u32PDP_LastTick + S_2_mS(GSM_CGREG_QUERY_TIMEOUT)))
        {
            // Query PS service
            bRes = GSM_PD_QueryPS();

            if (!bRes)
            {
                GSMprintf("\r\n> GSM PDP ACT query PS again");

                procGSM_PDP_Step = PDP_STEP_QUERY_PS;

                tcpRes = TCP_OK;

                break;
            }
        }
        else
        {
            tcpRes = TCP_NO_PS;

            tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

            procGSM_PDP_Step = PDP_STEP_NULL;

            g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt++;

            System_SaveSetting();

            // GSMprintf("\r\n> GSM Query PS Error: %d", g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt);

            if (g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt >= GSM_SIM_QUERY_MAX)
            {
                g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;

                System_SaveSetting();

                bQtEC21_State = FALSE;

                GSMprintf("\r\n> GSM PDP ACT Query PS ERROR! Maximum GSM module reset reached. Stop GSM function!");

                LCD_SetShowAutoTestPage(51);

                char cLog[30] = {0};

                sprintf(cLog, "PS ERROR");

                GPRS_DebugLog(cLog, strlen(cLog));
            }
            else
            {
                GSMprintf("\r\n> GSM Query PS ERROR! Reset GSM module.");

                // GSM_ResetWithCheckFTP();
                bGSM_ResetCheckFTP_Flag = TRUE;
            }

            break;
        }

        procGSM_PDP_Step = PDP_STEP_CONFIG;
        // procGSM_PDP_Step = PDP_STEP_ACT;

        tcpRes = TCP_OK;

        break;
    }

    // PDP_STEP_CONFIG: Config the selected PDP context
    case PDP_STEP_CONFIG:
    {
        // Config PDP context
        bRes = GSM_TCP_ConfigPDP(u8CurrPDP_ID,
                                 (char *)(g_Setting.tGSM_SET.tNetSet.cAPN),
                                 (char *)TCP_USER_NAME,
                                 (char *)TCP_USER_PW);

        // If failed to config the PDP context then return
        if (!bRes)
        {
            tcpRes = TCP_PDP_CONFIG_ERR;

            u8PDP_ActRetryCnt++;

            if (u8PDP_ActRetryCnt >= GSM_PDP_RETRY_MAX)
            {
                u8PDP_ActRetryCnt = 0;

                tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

                GSMprintf("\r\n> GSM PDP ACT config PDP context maximum loop retries reached. Reset GSM module.");

                char cLog[30] = {0};

                sprintf(cLog, "PDP config ERROR");

                GPRS_DebugLog(cLog, strlen(cLog));

                // GSM_ResetWithCheckFTP();
                bGSM_ResetCheckFTP_Flag = TRUE;

                procGSM_PDP_Step = PDP_STEP_NULL;
            }
            else
            {
                GSMprintf("\r\n> GSM PDP ACT config PDP context ERROR. Retry in the next loop.");

                procGSM_PDP_Step = PDP_STEP_CONFIG;
            }

            break;
        }

        u8PDP_ActRetryCnt = 0;

        procGSM_PDP_Step = PDP_STEP_ACT;

        tcpRes = TCP_OK;

        break;
    }

        // PDP_STEP_ACT: Send command to activate PDP context
    case PDP_STEP_ACT:
    {
        HAL_Delay(10);

        // Activate PDP context
        bRes = GSM_TCP_ActivPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            u8PDP_ActRetryCnt++;

            tcpRes = TCP_PDP_ACTV_ERR;

            if (u8PDP_ActRetryCnt >= GSM_PDP_RETRY_MAX)
            {
                u8PDP_ActRetryCnt = 0;

                tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

                GSMprintf("\r\n> GSM PDP ACT activate PDP context maximum loop retries reached. Reset GSM module");

                char cLog[30] = {0};

                sprintf(cLog, "PDP %d Act ERROR", u8CurrPDP_ID);

                GPRS_DebugLog(cLog, strlen(cLog));

                // GSM_ResetWithCheckFTP();
                bGSM_ResetCheckFTP_Flag = TRUE;

                procGSM_PDP_Step = PDP_STEP_NULL;
            }
            else
            {
                GSMprintf("\r\n> GSM PDP ACT activate PDP context ERROR. Retry in the next loop.");

                procGSM_PDP_Step = PDP_STEP_ACT;
                // procGSM_PDP_Step = PDP_STEP_CONFIG;
            }

            break;
        }

        u8PDP_ActRetryCnt = 0;

        tcpRes = TCP_OK;

        strcpy(cGSM_CurrIP, cTCP_IP_Temp);

        tcp_pdpContext[u8CurrPDP_ID].bActiv = TRUE;

        procGSM_PDP_Step = PDP_STEP_NULL;

        char cLog[30] = {0};

        sprintf(cLog, "PDP %d Act OK", u8CurrPDP_ID);

        GPRS_DebugLog(cLog, strlen(cLog));

        break;
    }

    // PDP_STEP_DEACT: Send command to deactivate PDP context connection
    case PDP_STEP_DEACT:
    {
        HAL_Delay(10);

        QtEC21_CheckRedoFTP();

        // Deactivate the PDP context
        bRes = GSM_TCP_DeactivPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            if (u8PDP_ActRetryCnt >= GSM_PDP_RETRY_MAX)
            {
                u8PDP_ActRetryCnt = 0;

                tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

                tcp_pdpContext[u8CurrPDP_ID].bActiv = FALSE;

                GSMprintf("\r\n> GSM PDP ACT deactivate PDP context maximum loop retries reached. Reset GSM module");

                char cLog[30] = {0};

                sprintf(cLog, "PDP %d Deact ERROR", u8CurrPDP_ID);

                GPRS_DebugLog(cLog, strlen(cLog));

                // GSM_ResetWithCheckFTP();
                bGSM_ResetCheckFTP_Flag = TRUE;

                procGSM_PDP_Step = PDP_STEP_NULL;
            }
            else
            {
                GSMprintf("\r\n> GSM PDP ACT deactivate PDP context ERROR. Retry in the next loop.");

                procGSM_PDP_Step = PDP_STEP_DEACT;
            }

            break;
        }

        u8PDP_ActRetryCnt = 0;

        tcpRes = TCP_OK;

        tcp_pdpContext[u8CurrPDP_ID].bInUse = FALSE;

        tcp_pdpContext[u8CurrPDP_ID].bActiv = FALSE;

        procGSM_PDP_Step = PDP_STEP_NULL;

        char cLog[30] = {0};

        sprintf(cLog, "PDP %d Deact OK", u8CurrPDP_ID);

        GPRS_DebugLog(cLog, strlen(cLog));

        break;
    }

    default:
        break;
    }

    return tcpRes;
}

//============================================================================//
// @brief  Handle the TCP/IP connecting process.
// @param  u8ConnDst: Destination code. 0: Connect to server 0.
//                                      1: Connect to server 1.
// @retval Result in TCP_StatusTypeDef format.
//============================================================================//
TCP_StatusTypeDef GSM_TCP_Connect_Proc(uint8_t u8ConnDst)
{
    TCP_StatusTypeDef tcpRes = TCP_OK;
    bool bRes = FALSE;
    static uint8_t u8TCP_SocketID[MAX_SOCKET_BANK] = {TCP_SOCKET_NUM_MAX};
    static uint32_t u32ConnLastTick = 0;
    char cLog[64] = {0};

    if (procGSM_TCP_Step[u8ConnDst] == PROC_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    switch ((int)(procGSM_TCP_Step[u8ConnDst]))
    {
    // CONN_STEP_SOCKET_SEL: Find socket slot
    case CONN_STEP_SOCKET_SEL:
    {
        uint8_t u8SocketID = TCP_SOCKET_NUM_MAX;

        bTCPIP_Connect[u8ConnDst] = FALSE;

        // Find an available socket ID
        for (int i = u8SocketID_Min[u8ConnDst]; i <= u8SocketID_Max[u8ConnDst]; i++)
        {
            if (tcpSocket[i].bConnect == FALSE)
            {
                // If the socket ID is valid
                if ((bChangeSocketFlag[u8ConnDst] == FALSE) ||
                    ((bChangeSocketFlag[u8ConnDst] == TRUE) && (i != u8TCP_SocketID[u8ConnDst])))
                {
                    u8SocketID = i;

                    // memset(cLog, 0x00, sizeof(cLog));

                    // sprintf(cLog, "Conn %d select socket %d",
                    //				u8ConnDst,
                    //				u8SocketID);

                    // GPRS_DebugLog(cLog, strlen(cLog));

                    break;
                }
            }
        }

        // If there is no socket ID available then return
        if (u8SocketID == TCP_SOCKET_NUM_MAX)
        {
            GSMprintf("\r\n> GSM OPEN CONN %d no socket available.", u8ConnDst);

            procGSM_TCP_Step[u8ConnDst] = CONN_STEP_NULL;

            tcpRes = TCP_NO_SOCKET;

            memset(cLog, 0x00, sizeof(cLog));

            sprintf(cLog, "Conn %d no socket", u8ConnDst);

            GPRS_DebugLog(cLog, strlen(cLog));

            break;
        }

        u8TCP_SocketID[u8ConnDst] = u8SocketID;

        procGSM_TCP_Step[u8ConnDst] = CONN_STEP_OPEN;

        tcpSocket[u8TCP_SocketID[u8ConnDst]].bConnect = TRUE;

        tcpRes = TCP_OK;

        break;
    }

    // CONN_STEP_OPEN: Open the TCP/IP connection
    case CONN_STEP_OPEN:
    {
        GSM_SocketConnTypeDef gsmConnHandle;

        if (GSM_IsWaitResp() == TRUE)
            return TCP_OK;

#if ENABLE_MULTI_SERVER
        if ((u8ConnDst == SOCKET_BANK_DEF) && (procGSM_TCP_Step[SOCKET_BANK_EXT] == CONN_STEP_OPEN_WAIT))
            return TCP_OK;
        else if ((u8ConnDst == SOCKET_BANK_EXT) && (procGSM_TCP_Step[SOCKET_BANK_DEF] == CONN_STEP_OPEN_WAIT))
            return TCP_OK;
#endif

        memset(gsmConnHandle.cServType, 0x00, sizeof(gsmConnHandle.cServType));
        memset(gsmConnHandle.cRemoteAddr, 0x00, sizeof(gsmConnHandle.cRemoteAddr));

        sprintf(gsmConnHandle.cServType, "%s", (char *)TCP_SERVICE_TYPE);

        // If the DNS of the current server is valid
        if ((strncmp(g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cDomainName, (char *)"NONE", 4) != 0) &&
            (strlen(g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cDomainName) > 0) &&
            (strlen(g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cDomainName) < sizeof(g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cDomainName)))
        {
            sprintf(gsmConnHandle.cRemoteAddr, "%s", g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cDomainName);
        }
        else
        {
            sprintf(gsmConnHandle.cRemoteAddr, "%s", g_Setting.tGSM_SET.tServerInfo[u8ConnDst].cIP_Addr);
        }

        gsmConnHandle.u16LocalPort = (uint16_t)TCP_LOCAL_PORT;

        gsmConnHandle.u16RemotePort = g_Setting.tGSM_SET.tServerInfo[u8ConnDst].u16Port;

        gsmConnHandle.u8ConnMode = 0;

        HAL_Delay(10);

        if (u8ConnDst == SOCKET_BANK_DEF)
        {
            gsmCurrServerType = CURR_SERVER_TCP;
        }
        else
        {
            gsmCurrServerType = CURR_SERVER_TCP_EXT;
        }

        memset(cLog, 0x00, sizeof(cLog));

        sprintf(cLog, "Conn %d open socket %d (%s:%d)",
                u8ConnDst,
                u8TCP_SocketID[u8ConnDst],
                gsmConnHandle.cRemoteAddr,
                gsmConnHandle.u16RemotePort);

        GPRS_DebugLog(cLog, strlen(cLog));

        bRes = GSM_TCP_OpenConn(u8CurrPDP_ID, u8TCP_SocketID[u8ConnDst], &gsmConnHandle);

        if (!bRes)
        {
            u8TCP_RetryCnt[u8ConnDst]++;

            if (u8TCP_RetryCnt[u8ConnDst] >= TCP_OPEN_CONN_RETRY_MAX)
            {
                u8TCP_RetryCnt[u8ConnDst] = 0;

                GSMprintf("\r\n> GSM OPEN CONN open connection %d maximum loop retries reached.", u8ConnDst);

                memset(cLog, 0x00, sizeof(cLog));

                sprintf(cLog, "Conn %d stop open[1]", u8ConnDst);

                GPRS_DebugLog(cLog, strlen(cLog));

                tcpSocket[u8TCP_SocketID[u8ConnDst]].bConnect = FALSE;
                // u8CurrTCP_Socket[u8ConnDst] = TCP_SOCKET_NUM_MAX;

                procGSM_TCP_Step[u8ConnDst] = CONN_STEP_NULL;

                tcpRes = TCP_OPEN_ERR;

                break;
            }

            procGSM_TCP_Step[u8ConnDst] = CONN_STEP_OPEN;

            tcpRes = TCP_OPEN_ERR;

            GSMprintf("\r\n> GSM OPEN CONN open connection %d ERROR. Retry in the next loop.", u8ConnDst);

            memset(cLog, 0x00, sizeof(cLog));

            sprintf(cLog, "Conn %d open socket %d ERROR(1)",
                    u8ConnDst,
                    u8TCP_SocketID[u8ConnDst]);

            GPRS_DebugLog(cLog, strlen(cLog));

            break;
        }

        u32ConnLastTick = HAL_GetTick();

        // GSMprintf("\r\n> GSM OPEN CONN open connection %d wait result.", u8ConnDst);

        procGSM_TCP_Step[u8ConnDst] = CONN_STEP_OPEN_WAIT;

        tcpRes = TCP_OK;

        break;
    }

    // CONN_STEP_OPEN_WAIT: Polling TCP/IP open connection result
    case CONN_STEP_OPEN_WAIT:
    {
        uint16_t u16ErrCode;
        GSM_NonBlockResp_ResTypeDef connRes;

        if (HAL_GetTick() < (u32ConnLastTick + S_2_mS(TCP_OPEN_CONN_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_TCP_CONN_OPEN, &connRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_TCP_Step[u8ConnDst] = CONN_STEP_OPEN_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> GSM TCP open connection %d wait result TIMEOUT!", u8ConnDst);

            tcpRes = TCP_OPEN_ERR;

            connRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;
        }

        if (connRes != GSMRES_SUCCESS)
        {
            u8TCP_RetryCnt[u8ConnDst]++;

            if (u16ErrCode != 563)
            {
                for (int i = 0; i < 2; i++)
                {
                    bRes = GSM_TCP_CloseConn(u8TCP_SocketID[u8ConnDst], TCP_CLOSE_CONN_TIMEOUT + 10);

                    if (bRes)
                        break;
                }

                memset(cLog, 0x00, sizeof(cLog));

                sprintf(cLog, "Conn %d close socket %d(1)",
                        u8ConnDst,
                        u8TCP_SocketID[u8ConnDst]);

                GPRS_DebugLog(cLog, strlen(cLog));
            }

            if (u8TCP_RetryCnt[u8ConnDst] >= TCP_OPEN_CONN_RETRY_MAX)
            {
                u8TCP_RetryCnt[u8ConnDst] = 0;

                GSMprintf("\r\n> GSM OPEN CONN open connection %d maximum loop retries reached. Waiting %d seconds before trying again.",
                          u8ConnDst, 20U);

                if (u8ConnDst == SOCKET_BANK_DEF)
                {
                    FuncSkip_Enable(FUNC_GSM_TCP_SERVER_DEF, 20);
                }
                else
                {
                    FuncSkip_Enable(FUNC_GSM_TCP_SERVER_EXT, 20);
                }

                memset(cLog, 0x00, sizeof(cLog));

                sprintf(cLog, "Conn %d stop open[1]", u8ConnDst);

                GPRS_DebugLog(cLog, strlen(cLog));

                tcpSocket[u8TCP_SocketID[u8ConnDst]].bConnect = FALSE;
                // u8CurrTCP_Socket[u8ConnDst] = TCP_SOCKET_NUM_MAX;

                procGSM_TCP_Step[u8ConnDst] = CONN_STEP_NULL;

                tcpRes = TCP_OPEN_ERR;

                break;
            }

            GSMprintf("\r\n> OPEN CONN open connection %d ERROR %d. Retry in the next loop.", u8ConnDst, u16ErrCode);

            procGSM_TCP_Step[u8ConnDst] = CONN_STEP_OPEN;

            tcpRes = TCP_OPEN_ERR;

            memset(cLog, 0x00, sizeof(cLog));

            sprintf(cLog, "Conn %d open socket %d ERROR %d(2)",
                    u8ConnDst,
                    u8TCP_SocketID[u8ConnDst],
                    u16ErrCode);

            GPRS_DebugLog(cLog, strlen(cLog));

            break;
        }

        tcpSocket[u8TCP_SocketID[u8ConnDst]].ptcp_pdpHandle = &tcp_pdpContext[u8CurrPDP_ID];

        // tcpSocket[u8SocID].bConnect = TRUE;

        u8CurrTCP_Socket[u8ConnDst] = u8TCP_SocketID[u8ConnDst];

        // GSMprintf("\r\n> Open TCP/IP connection successfully.");

        // GSMprintf("\r\n> Connection PDP context ID:%d", u8CurrPDP_ID);

        // GSMprintf("\r\n> GSM connection socket ID:%d", u8SocID);

        u8TCP_RetryCnt[u8ConnDst] = 0;

        procGSM_TCP_Step[u8ConnDst] = CONN_STEP_SEND_CHK;

        tcpRes = TCP_OK;

        // memset(cLog, 0x00, sizeof(cLog));

        // sprintf(cLog, "Conn %d open OK", u8ConnDst);

        // GPRS_DebugLog(cLog, strlen(cLog));

        break;
    }

    // CONN_STEP_SEND_CHK: Send CHK message to server
    case CONN_STEP_SEND_CHK:
    {
        MsgSendStatusTypeDef gsmRes;
        char cLog[64] = {0};

        if ((GSM_IsWaitResp() == TRUE) || (gsmCurrNonBlockCmd == QTEC21_SMS_SEND))
            return TCP_OK;

#if ENABLE_MULTI_SERVER
        if ((u8ConnDst == SOCKET_BANK_DEF) && (procGSM_TCP_Step[SOCKET_BANK_EXT] == CONN_STEP_OPEN_WAIT))
            return TCP_OK;
        else if ((u8ConnDst == SOCKET_BANK_EXT) && (procGSM_TCP_Step[SOCKET_BANK_DEF] == CONN_STEP_OPEN_WAIT))
            return TCP_OK;
#endif

        HAL_Delay(10);

        // if(g_Setting.tGSM_SET.tServerInfo[u8ConnDst].bEnableChk == TRUE)
        {
            gsmRes = GSM_TCP_SendCHK(u8ConnDst);
        }
        /*
        else
        {
            if(strlen(g_Setting.tGSM_SET.tMobileInfo.cIMEI) != 15)
            {
                bRes = GSM_GetIMEI(g_Setting.tGSM_SET.tMobileInfo.cIMEI);

                System_SaveSetting();
            }

            if(strlen(g_Setting.tGSM_SET.tMobileInfo.cIMSI) != 15)
            {
                bRes = GSM_GetIMSI(g_Setting.tGSM_SET.tMobileInfo.cIMSI);

                System_SaveSetting();
            }

          gsmRes = SEND_OK;
        }
        */

        if (gsmRes != SEND_OK)
        {
            u8TCP_RetryCnt[u8ConnDst]++;

            if (u8TCP_RetryCnt[u8ConnDst] == 3)
            {
                u8TCP_RetryCnt[u8ConnDst] = 0;

                GSMprintf("\r\n> GSM OPEN CONN send CHK maximum loop retries reached.");

                if (bTCPIP_Connect[u8ConnDst])
                    bTCPIP_Connect[u8ConnDst] = FALSE;

                procGSM_TCP_Step[u8ConnDst] = CONN_STEP_NULL;

                tcpRes = TCP_SEND_ERR;

                memset(cLog, 0x00, sizeof(cLog));

                sprintf(cLog, "Conn %d send CHK ERROR.", u8ConnDst);

                GPRS_DebugLog(cLog, strlen(cLog));

                break;
            }

            procGSM_TCP_Step[u8ConnDst] = CONN_STEP_SEND_CHK;

            tcpRes = TCP_SEND_ERR;

            break;
        }

        bTCPIP_Connect[u8ConnDst] = TRUE;

        u8TCP_ConnectRetryCnt[u8ConnDst] = 0;

        if (g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt > 0)
        {
            g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt = 0;

            System_SaveSetting();
        }

        u8TCP_RetryCnt[u8ConnDst] = 0;

        procGSM_TCP_Step[u8ConnDst] = CONN_STEP_NULL;

        tcpRes = TCP_OK;

        memset(cLog, 0x00, sizeof(cLog));

        sprintf(cLog, "Conn %d send CHK OK. Unsent: %d",
                u8ConnDst,
                u16UnsendPkgNum[u8ConnDst]);

        GPRS_DebugLog(cLog, strlen(cLog));

        break;
    }

    default:
        tcpRes = TCP_OK;

        break;
    }

    return tcpRes;
}

//============================================================================//
// @brief  Handle firmware upgrade through FTP.
// @param  None.
// @retval Result in TCP_StatusTypeDef.
//============================================================================//
TCP_StatusTypeDef GSM_FOTA_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    static FOTA_StepTypeDef procFOTA_LastStep;
    static GSM_NonBlockResp_ResTypeDef fotaRes;
    const uint32_t u32ExFlashTempAddr = (uint32_t)EXFLASH_TEMP_START_ADDR;
    static uint32_t u32FileSize, u32FileID, u32DatCnt, u32DatSize;
    static uint16_t u16CRC16_Check, u16CRC16_Cal = 0;
    static uint32_t u32FOTA_LastTick = 0;

    if (procGSM_FOTA_Step == FOTA_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    if ((procGSM_FOTA_Step != FOTA_STEP_CLOSE) &&
        (procGSM_FOTA_Step != FOTA_STEP_CLOSE_WAIT) &&
        (procGSM_FOTA_Step != FOTA_STEP_RAM_CLOSE) &&
        (procGSM_FOTA_Step != FOTA_STEP_REDO))
    {
        procFOTA_LastStep = procGSM_FOTA_Step;
    }

    switch ((int)procGSM_FOTA_Step)
    {
    // FOTA_STEP_CFG_PDP: Config PDP context for the FTP connection
    case FOTA_STEP_CFG_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_PDP_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA config PDP maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA config PDP ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CFG_PDP;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_CFG_ACC;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_CFG_ACC: Config account for the FTP connection
    case FOTA_STEP_CFG_ACC:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgAcc(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName,
                              g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);

        if (!bRes)
        {
            tcpRes = TCP_FTP_ACC_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA config account maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA config account ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CFG_ACC;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_CFG_FILE_TYPE: Config file type for the FTP connection
    case FOTA_STEP_CFG_FILE_TYPE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgFileType();

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILETYPE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA config file type maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA config file type ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CFG_FILE_TYPE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_CFG_TRANS_MODE;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_CFG_TRANS_MODE: Config transferring mode for the FTP connection
    case FOTA_STEP_CFG_TRANS_MODE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTransMode();

        if (!bRes)
        {
            tcpRes = TCP_FTP_TRANSMODE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA config transferring mode maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA config transferring mode ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CFG_TRANS_MODE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_CFG_TIMEOUT: Config timeout for the FTP connection
    case FOTA_STEP_CFG_TIMEOUT:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTimeout((uint16_t)FTP_FUNCTION_TIMEOUT);

        if (!bRes)
        {
            tcpRes = TCP_FTP_TIMEOUT_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA config timeout maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA config timeout ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CFG_TIMEOUT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_OPEN;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_OPEN: Send command to open FTP connection
    case FOTA_STEP_OPEN:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        gsmCurrServerType = CURR_SERVER_FTP;

        // Open FTP connection
        bRes = GSM_FTP_Login(g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr,
                             g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port);

        if (!bRes)
        {
            tcpRes = TCP_FTP_LOGIN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FOTA open connection maximum loop retries reached.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FOTA open connection ERROR. Retry in the next loop.");

            procGSM_FOTA_Step = FOTA_STEP_OPEN;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32FOTA_LastTick = HAL_GetTick();

        procGSM_FOTA_Step = FOTA_STEP_OPEN_WAIT;

        break;
    }

    // FOTA_STEP_OPEN_WAIT: Polling FTP open connection result
    case FOTA_STEP_OPEN_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32FOTA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_OPEN, &fotaRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_OPEN_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FOTA open connection wait result TIMEOUT!");

            tcpRes = TCP_FTP_LOGIN_ERR;

            fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            return tcpRes;
        }

        if (fotaRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FOTA open connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGIN_ERR;

            // If fail and no need to retry
            if (fotaRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FOTA open connection ERROR. Terminate operation.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((fotaRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FOTA open connection maximum loop retries reached.");

                    procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_FOTA_Step = FOTA_STEP_OPEN;

                // If need to delay before retry
                if ((fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FOTA open connection ERROR. Retry in the next loop after %d seconds.", FOTA_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FOTA_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FOTA open connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FOTA open connection ERROR.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_SET_DIR;

        break;
    }

    // FOTA_STEP_SET_DIR: Send command to set working directory on FTP server
    case FOTA_STEP_SET_DIR:
    {
        char cFilePath[30] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cFilePath, "%s%s", FTP_SERVER_DIR, DEVICE_TYPE);

        HAL_Delay(10);

        // Set working directory on the FTP server
        bRes = GSM_FTP_SetDir((char *)cFilePath);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SETDIR_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FOTA set working directory maximum loop retries reached.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FOTA set working directory ERROR. Retry in the next loop.");

            procGSM_FOTA_Step = FOTA_STEP_SET_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32FOTA_LastTick = HAL_GetTick();

        procGSM_FOTA_Step = FOTA_STEP_SET_DIR_WAIT;

        break;
    }

    // FOTA_STEP_SET_DIR_WAIT: Polling FTP set working directory result
    case FOTA_STEP_SET_DIR_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32FOTA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_SET_DIR, &fotaRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_SET_DIR_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FOTA set working directory wait result TIMEOUT.");

            tcpRes = TCP_FTP_SETDIR_ERR;

            fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            return tcpRes;
        }

        if (fotaRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FOTA set working directory get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_SETDIR_ERR;

            // If fail and no need to retry
            if (fotaRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FOTA set working directory ERROR. Terminate operation.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((fotaRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FOTA set working directory maximum loop retries reached.");

                    procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_FOTA_Step = FOTA_STEP_SET_DIR;

                // If need to delay before retry
                if ((fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FOTA set working directory ERROR. Retry in the next loop after %d seconds.", FOTA_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FOTA_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FOTA set working directory ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FOTA set working directory ERROR.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_GET_SIZE;

        break;
    }

    // FOTA_STEP_GET_SIZE: Check the size of the upgrade file on the FTP server
    case FOTA_STEP_GET_SIZE:
    {
        char cDF2_FileName[30] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cDF2_FileName, "%s_%02d_%02d_%02d.%s", DEVICE_TYPE, MODEL_NAME,
                CUSTOMER_TYPE, HW_VER, UPGRADE_FILE_EXT);

        GSMprintf("\r\n> FOTA upgrade file name: %s", cDF2_FileName);

        HAL_Delay(10);

        // Get size of the upgrade file
        bRes = GSM_FTP_FileSize(cDF2_FileName);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILESIZE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FOTA get file size maximum loop retries reached.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FOTA get file size ERROR. Retry in the next loop.");

            procGSM_FOTA_Step = FOTA_STEP_GET_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32FOTA_LastTick = HAL_GetTick();

        procGSM_FOTA_Step = FOTA_STEP_GET_SIZE_WAIT;

        break;
    }

    // FOTA_STEP_GET_SIZE_WAIT: Polling for FTP get size result
    case FOTA_STEP_GET_SIZE_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32FOTA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_SIZE, &fotaRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_GET_SIZE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FOTA get file size wait result TIMEOUT.");

            tcpRes = TCP_FTP_FILESIZE_ERR;

            fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            return tcpRes;
        }

        if (fotaRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FOTA get file size get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_FILESIZE_ERR;

            // If fail and no need to retry
            if (fotaRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FOTA get file size ERROR. Terminate operation.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((fotaRes == GSMRES_FAIL_REDO_LIMIT) ||
                (fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FOTA get file size maximum loop retries reached.");

                    procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_FOTA_Step = FOTA_STEP_GET_SIZE;

                // If need to delay before retry
                if ((fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FOTA get file size ERROR. Retry in the next loop after %d seconds.", FOTA_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FOTA_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FOTA get file size ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FOTA get file size ERROR.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
        }

        if (u32GetFileSize == 0)
        {
            GSMprintf("\r\n> FOTA size of upgrade file is 0. Terminate the operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            tcpRes = TCP_OK;

            break;
        }
        else if (u32GetFileSize > (uint32_t)PROGRAM_SIZE)
        {
            GSMprintf("\r\n> FOTA upgrade file size is %d, which is larger than MCU program size of %d. Terminate the operation.",
                      u32GetFileSize,
                      (uint32_t)PROGRAM_SIZE);

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            tcpRes = TCP_OK;

            break;
        }

        u32FileSize = u32GetFileSize;

        GSMprintf("\r\n> FOTA size of upgrade file is: %d", u32FileSize);

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_RAM_DEL;

        break;
    }

    // FOTA_STEP_RAM_DEL: Delete RAM content to prepare to download upgrade file
    case FOTA_STEP_RAM_DEL:
    {
        char cDeletePath[20] = {0};

        sprintf(cDeletePath, "%s%s", FTP_LOCAL_STORAGE, STR_STORAGE_DEL_ALL);

        HAL_Delay(10);

        bRes = GSM_FileDelete(cDeletePath);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEDEL_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA RAM delete maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FOTA RAM delete ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_RAM_DEL;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_DOWNLOAD;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_DOWNLOAD: Download upgrade file
    case FOTA_STEP_DOWNLOAD:
    {
        char cDF2_FileName[30] = {0};
        char cTempFileName[20] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cDF2_FileName, "%s_%02d_%02d_%02d.%s", DEVICE_TYPE, MODEL_NAME,
                CUSTOMER_TYPE, HW_VER, UPGRADE_FILE_EXT);

        sprintf(cTempFileName, "%s%s.%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE,
                UPGRADE_FILE_EXT);

        HAL_Delay(10);

        // Download the upgrade file to device RAM
        bRes = GSM_FTP_DownloadFile(cDF2_FileName, cTempFileName, 0);

        if (!bRes)
        {
            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FOTA download file %s maximum loop retries reached.", cDF2_FileName);

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FOTA download file %s ERROR. Retry in the next loop.", cDF2_FileName);

            procGSM_FOTA_Step = FOTA_STEP_DOWNLOAD;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32FOTA_LastTick = HAL_GetTick();

        procGSM_FOTA_Step = FOTA_STEP_DOWNLOAD_WAIT;

        break;
    }

    // FOTA_STEP_DOWNLOAD_WAIT: Polling for FTP download result
    case FOTA_STEP_DOWNLOAD_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32FOTA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_DOWNLOAD, &fotaRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_DOWNLOAD_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FOTA download file wait result TIMEOUT.");

            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            return tcpRes;
        }

        if (fotaRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FOTA download file get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            // If fail and no need to retry
            if (fotaRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FOTA download file ERROR. Terminate operation.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((fotaRes == GSMRES_FAIL_REDO_LIMIT) ||
                (fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FOTA download file maximum loop retries reached.");

                    procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_FOTA_Step = FOTA_STEP_DOWNLOAD;

                // If need to delay before retry
                if ((fotaRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (fotaRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FOTA download file ERROR. Retry in the next loop after %d seconds.", FOTA_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FOTA_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FOTA download file ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FOTA download file ERROR.");

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_CLOSE;

        break;
    }

    // FOTA_STEP_CLOSE: Send command to close FTP connection
    case FOTA_STEP_CLOSE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Close connection to FTP server
        bRes = GSM_FTP_Logout();

        if (!bRes)
        {
            tcpRes = TCP_FTP_LOGOUT_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA close FTP connection maximum loop retries reached.");

                if (fotaRes == GSMRES_SUCCESS)
                    procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
                else
                    procGSM_FOTA_Step = FOTA_STEP_REDO;

                return tcpRes;
            }

            GSMprintf("\r\n> FOTA close FTP connection ERROR. Retry in the next loop.");

            procGSM_FOTA_Step = FOTA_STEP_CLOSE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32FOTA_LastTick = HAL_GetTick();

        procGSM_FOTA_Step = FOTA_STEP_CLOSE_WAIT;

        break;
    }

    // FOTA_STEP_CLOSE_WAIT: Polling FTP close connection result
    case FOTA_STEP_CLOSE_WAIT:
    {
        uint16_t u16ErrCode;
        GSM_NonBlockResp_ResTypeDef fotaCloseRes;

        if (HAL_GetTick() < (u32FOTA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_CLOSE, &fotaCloseRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_CLOSE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FOTA close FTP connection wait result TIMEOUT.");

            tcpRes = TCP_FTP_LOGOUT_ERR;

            fotaCloseRes = GSMRES_FAIL_REDO_LIMIT_RST;

            if (fotaRes == GSMRES_SUCCESS)
                procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
            else
                procGSM_FOTA_Step = FOTA_STEP_REDO;

            return tcpRes;
        }

        if (fotaCloseRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FOTA close FTP connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGOUT_ERR;

            // If fail and no need to retry
            if (fotaCloseRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FOTA close FTP connection ERROR.");

                if (fotaRes == GSMRES_SUCCESS)
                    procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
                else
                    procGSM_FOTA_Step = FOTA_STEP_REDO;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((fotaCloseRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (fotaCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (fotaCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FOTA close FTP connection maximum loop retries reached.");

                    if (fotaRes == GSMRES_SUCCESS)
                        procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
                    else
                        procGSM_FOTA_Step = FOTA_STEP_REDO;

                    return tcpRes;
                }

                procGSM_FOTA_Step = FOTA_STEP_CLOSE;

                // If need to delay before retry
                if ((fotaCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (fotaCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    FuncSkip_Enable(FUNC_GSM_FTP, FOTA_STOP_TIMEOUT);

                    GSMprintf("\r\n> FOTA close FTP connection ERROR. Retry in the next loop after %d seconds.", FOTA_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FOTA close FTP connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FOTA close FTP connection ERROR.");

                if (fotaRes == GSMRES_SUCCESS)
                    procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
                else
                    procGSM_FOTA_Step = FOTA_STEP_REDO;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        if (fotaRes == GSMRES_SUCCESS)
            procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
        else
            procGSM_FOTA_Step = FOTA_STEP_REDO;

        break;
    }

    // FOTA_STEP_RAM_OPEN: Open upgrade file in device RAM
    case FOTA_STEP_RAM_OPEN:
    {
        char cTempFileName[20] = {0};

        GSMprintf("\r\n> FOTA open upgrade file in device RAM");

        sprintf(cTempFileName, "%s%s.%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE,
                UPGRADE_FILE_EXT);

        HAL_Delay(10);

        bRes = GSM_FileOpen(cTempFileName, &u32FileID, 2);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA open DF2 file maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA open DF2 file ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_RAM_OPEN;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_FOTA_Step = FOTA_STEP_CHECK_INFO;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_CHECK_INFO: Checking upgrade file information
    case FOTA_STEP_CHECK_INFO:
    {
        uint8_t u8FW_Header[FLASH_HEADER_SIZE];
        char cCurr_FW_Ver[20] = {0};
        char cFW_Ver[20] = {0};
        uint32_t u32FW_Size;
        uint16_t u16ModelID, u16CustID, u16HW_ID;
        uint8_t u8FW_ID[3];
        uint32_t u32ActualReadLen;

        HAL_Delay(10);

        bRes = GSM_FileRead(u32FileID, (uint32_t)FLASH_HEADER_SIZE, &u32ActualReadLen, (char *)u8FW_Header);

        if ((!bRes) || (bRes && (u32ActualReadLen != (uint32_t)FLASH_HEADER_SIZE)))
        {
            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA read upgrade file header maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FOTA read upgrade file header ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_CHECK_INFO;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        // Get firmware CRC checksum
        u16CRC16_Check = ((uint16_t)(u8FW_Header[3]) << 8) | (uint16_t)u8FW_Header[2];

        // Get firmware size
        u32FW_Size = ((uint32_t)(u8FW_Header[7]) << 24) |
                     ((uint32_t)(u8FW_Header[6]) << 16) |
                     ((uint32_t)(u8FW_Header[5]) << 8) |
                     (uint32_t)(u8FW_Header[4]);

        u16ModelID = ((uint16_t)(u8FW_Header[9]) << 8) | (uint16_t)u8FW_Header[8];

        u16CustID = ((uint16_t)(u8FW_Header[11]) << 8) | (uint16_t)u8FW_Header[10];

        u16HW_ID = ((uint16_t)(u8FW_Header[13]) << 8) | (uint16_t)u8FW_Header[12];

        u8FW_ID[0] = u8FW_Header[14];
        u8FW_ID[1] = u8FW_Header[15];
        u8FW_ID[2] = u8FW_Header[16];

        if (u8FW_ID[2] == 0)
        {
            sprintf(cFW_Ver, "%d.%03d", u8FW_ID[0], u8FW_ID[1]);
        }
        else
        {
            sprintf(cFW_Ver, "%d.%03d_Alpha%d", u8FW_ID[0], u8FW_ID[1], u8FW_ID[2]);
        }

        GSMprintf("\r\n> FOTA firmware version:%s", cFW_Ver);
        GSMprintf("\r\n> FOTA firmware for model ID:%d", u16ModelID);
        GSMprintf("\r\n> FOTA firmware for customer ID:%d", u16CustID);
        GSMprintf("\r\n> FOTA firmware for hardware ID:%d", u16HW_ID);
        GSMprintf("\r\n> FOTA firmware size:%d", u32FW_Size);
        GSMprintf("\r\n> FOTA firmware CRC16:0x%04x", u16CRC16_Check);

        sprintf(cCurr_FW_Ver, "%s%s", FIRMWARE_VERSION, FW_VERSION_CHK);

        if (strcmp(cCurr_FW_Ver, cFW_Ver) == 0)
        {
            GSMprintf("\r\n> FOTA Firmware version is the same. Terminate operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;

            return TCP_OK;
        }

        if (u16ModelID != (uint16_t)MODEL_NAME)
        {
            GSMprintf("\r\n> FOTA model ID is not matched. Terminate operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;

            return TCP_FTP_CHECKFW_ERR;
        }

        if (u16CustID != (uint16_t)CUSTOMER_TYPE)
        {
            GSMprintf("\r\n> FOTA customer type is not matched. Terminate operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;

            return TCP_FTP_CHECKFW_ERR;
        }

        if (u16HW_ID != (uint16_t)HW_VER)
        {
            GSMprintf("\r\n> FOTA hardware ID is not matched. Terminate operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;

            return TCP_FTP_CHECKFW_ERR;
        }

        if (u32FW_Size != (u32FileSize - (uint32_t)FLASH_HEADER_SIZE))
        {
            GSMprintf("\r\n> FOTA firmware size is not matched. Terminate operation.");

            fotaRes = GSMRES_FAIL_NO_REDO;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;

            return TCP_FTP_CHECKFW_ERR;
        }

        u32FileSize = u32FW_Size;

        sprintf(cFW_NewVer, "%s", cFW_Ver);

        GSMprintf("\r\n> FOTA start to save firmware to ExFlash.");

        u8FileTransRetryCnt = 0;

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_SAVE_FILE_INIT;

        break;
    }

    // FOTA_STEP_SAVE_FILE_INIT: Prepare to save the file to device Exflash
    case FOTA_STEP_SAVE_FILE_INIT:
    {
        uint16_t u16SectEraseNum;
        MX25L_StatusTypeDef mxlRes;

        u16SectEraseNum = (uint16_t)(u32FileSize / MX25L_SECTOR_SIZE);

        if (u32FileSize % MX25L_SECTOR_SIZE)
            u16SectEraseNum++;

        // GSMprintf("\r\n> FOTA first erase sector: %d, sector num: %d", (u32ExFlashTempAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);
        GSMprintf("\r\n> FOTA erase ExFlash APP...");

        mxlRes = MX25L_MultiSectorsErase((u32ExFlashTempAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);

        if (mxlRes != MX25L_OK)
        {
            GSMprintf("\r\n> FOTA erase ExFlash ERROR %d.", mxlRes);

            tcpRes = TCP_FTP_SAVEFILE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA erase ExFlash maximum loop retries reached.");

                fotaRes = GSMRES_FAIL_REDO_LIMIT;

                procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FOTA erase ExFlash ERROR %d. Retry in the next loop.", mxlRes);

                procGSM_FOTA_Step = FOTA_STEP_SAVE_FILE_INIT;
            }

            return tcpRes;
        }

        GSMprintf("\r\n> FOTA erase ExFlash OK.");

        u32DatCnt = 0;

        CRC16_Reset(&u16CRC16_Cal);

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_SET_POINTER;

        break;
    }

    // FOTA_STEP_SET_POINTER: Set file pointer to read the data.
    case FOTA_STEP_SET_POINTER:
    {
        bRes = GSM_FileSeek(u32FileID, (uint32_t)FLASH_HEADER_SIZE + u32DatCnt);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILESEEK_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt == 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> FOTA set file pointer maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> FOTA set file pointer ERROR. Retry from the beginning of the file in the next loop.");

                    procGSM_FOTA_Step = FOTA_STEP_SAVE_FILE_INIT;
                }
            }
            else
            {
                GSMprintf("\r\n> FOTA set file pointer ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_SET_POINTER;
            }

            return tcpRes;
        }

        // GSMprintf("\r\n> Set pointer for DF2 file in device RAM successfully.");

        procGSM_FOTA_Step = FOTA_STEP_SAVE_DATA;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_SAVE_DATA: Read data from the device RAM and write to the Exflash and calculate CRC
    case FOTA_STEP_SAVE_DATA:
    {
        const int iDownloadSize = 1024;
        uint8_t u8DatBuf[1024];
        uint32_t u32ActualReadLen;
        MX25L_StatusTypeDef mxlRes;
        uint32_t u32PageOffset;
        static uint32_t u32FirstByteBuf = 0;

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > iDownloadSize)
            u32DatSize = iDownloadSize;

        memset(u8DatBuf, 0x00, sizeof(u8DatBuf));

        bRes = GSM_FileRead(u32FileID, u32DatSize, &u32ActualReadLen, (char *)u8DatBuf);

        // GSMprintf("\r\n> FOTA read %d bytes from DF2 file in device RAM.", u32ActualReadLen);

        if ((!bRes) || (bRes && (u32ActualReadLen != u32DatSize)))
        {
            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt == 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> FOTA read data maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> FOTA read data ERROR. Retry from the beginning of the file in the next loop.");

                    procGSM_FOTA_Step = FOTA_STEP_SAVE_FILE_INIT;
                }
            }
            else
            {
                GSMprintf("\r\n> FOTA read data ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_SET_POINTER;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        // Save the first 4 bytes to write at last
        if (u32DatCnt == 0)
        {
            memcpy(&u32FirstByteBuf, u8DatBuf, 4);

            memset(u8DatBuf, 0xFF, 4);
        }

        u32PageOffset = (u32ExFlashTempAddr + u32DatCnt) / (uint32_t)MX25L_PAGE_SIZE;

        mxlRes = MX25L_MultiPagesProgram(u32PageOffset, u8DatBuf, u32DatSize);
        // mxlRes = MX25L_OK;

        if (u32DatCnt == 0)
        {
            memcpy(u8DatBuf, &u32FirstByteBuf, 4);
        }

        if (mxlRes != MX25L_OK)
        {
            tcpRes = TCP_FTP_SAVEFILE_ERR;

            u8FileTransRetryCnt++;

            if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                GSMprintf("\r\n> FOTA write to ExFlash maximum loop retries reached.");

                u8FileTransRetryCnt = 0;

                fotaRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FOTA write to ExFlash at position %d ERROR. Retry in the next loop.", u32DatCnt);

                procGSM_FOTA_Step = FOTA_STEP_SET_POINTER;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u8FileTransRetryCnt = 0;

        u32DatCnt += u32DatSize;

        for (int i = 0; i < u32DatSize; i++)
        {
            u16CRC16_Cal = CRC16_Calc(u16CRC16_Cal, u8DatBuf[i]);
        }

        // GSMprintf("\r\n> FOTA finish write %d bytes to SD card.");

        if ((u32DatCnt % (sizeof(u8DatBuf) * 10)) == 0)
            GSMprintf("\r\n\r\n> FOTA bytes transferred: %d\r\n", u32DatCnt);

        if (u32DatCnt >= u32FileSize)
        {
            for (int i = 0; i < 3; i++)
            {
                mxlRes = MX25L_MultiSectorsErase((EXFLASH_TEMP_SIZE_START_ADDR / MX25L_SECTOR_SIZE), 1);

                if (mxlRes != MX25L_OK)
                    continue;

                u32PageOffset = EXFLASH_TEMP_SIZE_START_ADDR / (uint32_t)MX25L_PAGE_SIZE;

                mxlRes = MX25L_MultiPagesProgram(u32PageOffset, (uint8_t *)&u32FileSize, 4);

                if (mxlRes == MX25L_OK)
                    break;
            }

            if (mxlRes != MX25L_OK)
            {
                GSMprintf("\r\n> FOTA write temp firmware size ERROR: %d", mxlRes);
            }

            // If the first 4 bytes buffer still have a valid value
            if ((u32FirstByteBuf & 0x2FFE0000) == 0x20000000)
            {
                for (int i = 0; i < 3; i++)
                {
                    u32PageOffset = u32ExFlashTempAddr / (uint32_t)MX25L_PAGE_SIZE;

                    // GSMprintf("\r\n> FOTA write first 4 byte 0x%08X to page: %d", u32FirstByteBuf, u32PageOffset);

                    mxlRes = MX25L_MultiPagesProgram(u32PageOffset, (uint8_t *)&u32FirstByteBuf, 4);

                    if (mxlRes == MX25L_OK)
                        break;
                }
            }
            else
                mxlRes = MX25L_WRITE_ERROR;

            if (mxlRes != MX25L_OK)
            {
                tcpRes = TCP_FTP_SAVEFILE_ERR;

                GSMprintf("\r\n> FOTA write to ExFlash at position %d ERROR. Retry from the beginning of the file.", u32PageOffset);

                procGSM_FOTA_Step = FOTA_STEP_SAVE_FILE_INIT;

                return tcpRes;
            }

            fotaRes = GSMRES_SUCCESS;

            procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
        }
        else
            procGSM_FOTA_Step = FOTA_STEP_SET_POINTER;
        // procGSM_FOTA_Step = FOTA_STEP_SAVE_DATA;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_RAM_CLOSE: Close the upgrade file in device RAM
    case FOTA_STEP_RAM_CLOSE:
    {
        HAL_Delay(10);

        bRes = GSM_FileClose(u32FileID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILECLOSE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FOTA close DF2 file maximum loop retries reached.");

                if (fotaRes == GSMRES_SUCCESS)
                    procGSM_FOTA_Step = FOTA_STEP_RESET_UPGRADE;
                else
                    procGSM_FOTA_Step = FOTA_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FOTA close DF2 file ERROR. Retry in the next loop.");

                procGSM_FOTA_Step = FOTA_STEP_RAM_CLOSE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        if (fotaRes == GSMRES_SUCCESS)
            procGSM_FOTA_Step = FOTA_STEP_RESET_UPGRADE;
        else
            procGSM_FOTA_Step = FOTA_STEP_REDO;

        tcpRes = TCP_OK;

        break;
    }

    // FOTA_STEP_RESET_UPGRADE: Check the CRC and proceed to upgrade firmware
    case FOTA_STEP_RESET_UPGRADE:
    {
        if (u16CRC16_Cal != u16CRC16_Check)
        {
            GSMprintf("\r\n> FOTA check CRC16 ERROR. Calculated CRC:0x%04x, original CRC:0x%04x", u16CRC16_Cal, u16CRC16_Check);

            fotaRes = GSMRES_FAIL_REDO_LIMIT;

            procGSM_FOTA_Step = FOTA_STEP_REDO;

            return TCP_FTP_CRC_ERR;
        }
        else
            GSMprintf("\r\n> FOTA check CRC16 OK.");

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_NULL;

        // Set and save upgrade flag
        if (bSD_ReportErrFlag == FALSE)
        {
            V161_RecordTechnical(2);
        }

        if ((g_SysInfo.tVehicleState.bACC_ON == 1) ||
            (bSystemMode == SYSTEM_MODE_STEALTH))
        {
            g_Setting.tUpgrade.bUpgradeFlag = TRUE;

            g_Setting.tUpgrade.ttLastInstall = System_GetTime();

            System_SaveSettingAll();
            System_SaveBackupSettingAll(VAR_ALL_ITEMS);
        }

        // Write the new firmware size to BKP register
        HW_WriteBackupRegister(BKP_FW_SIZE, u32FileSize);

        // Notify the bootloader to download new firmware from the ExFlash after reset.
        HW_WriteBackupRegister(BKP_DFU_CHK, BKP_LOAD_VALUE);

        // Reset the system
        GSMprintf("\r\n> FOTA reset system to upgrade to firmware version %s.", cFW_NewVer);

        if (bRTOS_En)
        {
            osMutexWait(mutex_setting_user, S_2_mS(5));
        }

        System_Reset();

        break;
    }

    // FOTA_STEP_REDO: If the operation failed then make another request
    case FOTA_STEP_REDO:
    {
        bool bFOTA_RedoLimit = FALSE, bFOTA_RedoDelay = FALSE, bFOTA_RedoRst = FALSE;
        char cFOTA_DelayMsg[20] = {0};
        static uint8_t u8StealthRetry = 0;

        if (fotaRes != GSMRES_SUCCESS)
        {
            sprintf(cFOTA_DelayMsg, "S:%d-E:%d-C:%d", procFOTA_LastStep, fotaRes, u16GSM_ErrCode);

            GSM_TCP_ReportEvent((char *)EVENT_FOTA_FAIL, cFOTA_DelayMsg);

            memset(cFOTA_DelayMsg, 0x00, sizeof(cFOTA_DelayMsg));
        }

        if ((fotaRes == GSMRES_SUCCESS) || (fotaRes == GSMRES_FAIL_NO_REDO))
        {
            tcpRes = TCP_OK;

            procGSM_FOTA_Step = FOTA_STEP_NULL;

            if (bSystemMode == SYSTEM_MODE_STEALTH)
                bStealthFOTA_En = FALSE;

            break;
        }

#if ENABLE_FOTA_ACC_OFF
        // In stealth mode or ACC off mode for other errors always retry until reaching limit
        if ((bSystemMode == SYSTEM_MODE_STEALTH) ||
            ((bSystemMode == SYSTEM_MODE_NORMAL) && (bWaitingEnterStandby == TRUE)))
#else
        // In stealth mode for other errors always retry until reaching limit
        if (bSystemMode == SYSTEM_MODE_STEALTH)
#endif
        {
            u8StealthRetry++;

            tcpRes = TCP_OK;

            procGSM_FOTA_Step = FOTA_STEP_NULL;

            g_Setting.tUpgrade.bFOTA_Flag = TRUE;

            if (u8StealthRetry >= 2)
            {
                if (bSystemMode == SYSTEM_MODE_NORMAL)
                {
                    g_Setting.tUpgrade.bFOTA_Flag = FALSE;
                }

                u8StealthRetry = 0;

                bStealthFOTA_En = FALSE;
            }

            System_SaveSetting();

            break;
        }

        // Check for number of redo
        switch (fotaRes)
        {
            // For these cases the number of redo is limited
        case GSMRES_NULL:
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            bFOTA_RedoLimit = TRUE;

            break;

        default:
            break;
        }

        // Check for delay
        switch (fotaRes)
        {
        // For these cases add a delay before redo
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bFOTA_RedoDelay = TRUE;

            break;

        default:
            break;
        }

        // Check for reset GSM module
        switch (fotaRes)
        {
            // For these case reset the GSM module before redo
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bFOTA_RedoRst = TRUE;

            break;

        default:
            break;
        }

        if (bFOTA_RedoLimit == TRUE)
        {
            g_Setting.tUpgrade.u8FOTA_RetryCnt++;

            if (g_Setting.tUpgrade.u8FOTA_RetryCnt >= FOTA_RETRY_MAX)
            {
                g_Setting.tUpgrade.u8FOTA_RetryCnt = 0;

                System_SaveSetting();

                GSMprintf("\r\n> FOTA maximum redo reached. Terminate operation.");

                tcpRes = TCP_OK;

                procGSM_FOTA_Step = FOTA_STEP_NULL;

                break;
            }

            System_SaveSetting();
        }

        if (bFOTA_RedoDelay)
            sprintf(cFOTA_DelayMsg, " after %d seconds", STOP_FTP_ACCESS_TIMEOUT);

        GSMprintf("\r\n> FOTA%s request to redo%s%s.",
                  bFOTA_RedoRst ? (char *)" reset GSM module and" : (char *)"",
                  bFOTA_RedoDelay ? cFOTA_DelayMsg : (char *)"",
                  bFOTA_RedoLimit ? (char *)"" : (char *)" (Unlimited)");

        if (bFOTA_RedoDelay)
            FuncSkip_Enable(FUNC_GSM_FTP, STOP_FTP_ACCESS_TIMEOUT);

        if (bFOTA_RedoRst)
            // GSM_Reset();
            bGSM_ResetFlag = TRUE;

        g_Setting.tUpgrade.bFOTA_Flag = TRUE;

        System_SaveSetting();

        tcpRes = TCP_OK;

        procGSM_FOTA_Step = FOTA_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}

#if ENABLE_FTP_DOWNLOAD
//============================================================================//
// @brief  Put a request to download a file from the FTP server to SD card.
// @param  pDlHandle: Pointer to a GSM_DownloadRequestTypeDef variable.
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_FileDownloadRequest(GSM_DownloadRequestTypeDef *pDlHandle)
{
    char cFilePath[130];
    osStatus osRes;
    bool bRes;

    if ((strlen(pDlHandle->cSrcFile) == 0) ||
        (strlen(pDlHandle->cSrcFile) > sizeof(gsmDownloadRequest.cSrcFile)) ||
        (strlen(pDlHandle->cSrcDir) == 0) ||
        (strlen(pDlHandle->cSrcDir) > sizeof(gsmDownloadRequest.cSrcDir)) ||
        (strlen(pDlHandle->cDstFile) == 0) ||
        (strlen(pDlHandle->cDstFile) > sizeof(gsmDownloadRequest.cDstFile)) ||
        (strlen(pDlHandle->cDstDir) > sizeof(gsmDownloadRequest.cDstDir)) ||
        ((pDlHandle->u8ServerID != 1) && (pDlHandle->u8ServerID != 2)))
    {
        GSMprintf("\r\n> FTP download request has unsuitable parameter.");

        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> FTP download can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", FTP_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);
#endif

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)pDlHandle,
                              sizeof(GSM_DownloadRequestTypeDef),
                              2 + (uint32_t)u16DownloadFileNum * sizeof(GSM_DownloadRequestTypeDef));

    if (bRes)
    {
        GSM_FTP_IncDownloadIdx();

        GSMprintf("\r\n> GSM add FTP download request for file %s/%s OK.", pDlHandle->cSrcDir, pDlHandle->cSrcFile);

        if (bSystemMode == SYSTEM_MODE_STEALTH)
            bStealthDownloadEn = TRUE;
    }
    else
    {
        GSMprintf("\r\n> GSM add FTP download request for file %s/%s ERROR.", pDlHandle->cSrcDir, pDlHandle->cSrcFile);
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Get a file download request from the request list and initialize the
//         download process.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileDownloadInit(void)
{
    bool bRes;
    osStatus osRes;
    char cFilePath[128] = {0};

    if (u16DownloadFileNum == 0)
        return FALSE;

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", FTP_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);
#endif

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)&gsmDownloadRequest,
                             sizeof(GSM_DownloadRequestTypeDef),
                             2 + ((uint32_t)u16DownloadFileNum - 1) * sizeof(GSM_DownloadRequestTypeDef));

    if (bRes)
    {
        GSMprintf("\r\n> GSM get FTP download request %d OK.", u16DownloadFileNum);

        procGSM_Download_Step = DOWNLOAD_STEP_INIT;

        GSM_FTP_DecDownloadIdx();
    }
    else
    {
        GSMprintf("\r\n> GSM get FTP download request ERROR.");
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Increase the index of pending download request number by 1 then write
//         the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_IncDownloadIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

#ifndef DIRNAME_TEMP
    sprintf(cFileName, "%s", FTP_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);
#endif

    u16DownloadFileNum++;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Decrease the index of the pending download request number by 1 then
//         write the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_DecDownloadIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

    if (u16DownloadFileNum == 0)
        return;

#ifndef DIRNAME_TEMP
    sprintf(cFileName, "%s", FTP_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);
#endif

    u16DownloadFileNum--;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Handle FTP file download process.
// @param  None.
// @retval Result in TCP_StatusTypeDef.
//============================================================================//
TCP_StatusTypeDef GSM_FTP_Download_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    static Download_StepTypeDef procDownloadLastStep;
    static GSM_NonBlockResp_ResTypeDef dlRes;
    static char cSrcFilePath[128] = {0};
    static char cDstFilePath[128] = {0};
    static uint32_t u32FileSize, u32FileID, u32DatCnt, u32DatSize;
    static uint32_t u32DownloadLastTick = 0;
    static uint32_t u32StartTime = 0, u32EndTime = 0;

    if (procGSM_Download_Step == DOWNLOAD_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    if ((procGSM_Download_Step != DOWNLOAD_STEP_CLOSE) &&
        (procGSM_Download_Step != DOWNLOAD_STEP_CLOSE_WAIT) &&
        (procGSM_Download_Step != DOWNLOAD_STEP_RAM_CLOSE) &&
        (procGSM_Download_Step != DOWNLOAD_STEP_REDO))
    {
        procDownloadLastStep = procGSM_Download_Step;
    }

    switch ((int)procGSM_Download_Step)
    {
    // DOWNLOAD STEP INIT: Initializing the FTP file download process
    case DOWNLOAD_STEP_INIT:
    {
        if ((strlen(gsmDownloadRequest.cSrcFile) == 0) ||
            (strlen(gsmDownloadRequest.cSrcFile) > sizeof(gsmDownloadRequest.cSrcFile)) ||
            (strlen(gsmDownloadRequest.cSrcDir) > sizeof(gsmDownloadRequest.cSrcDir)) ||
            (strlen(gsmDownloadRequest.cDstFile) == 0) ||
            (strlen(gsmDownloadRequest.cDstFile) > sizeof(gsmDownloadRequest.cDstFile)) ||
            (strlen(gsmDownloadRequest.cDstDir) > sizeof(gsmDownloadRequest.cDstDir)))
        {
            GSMprintf("\r\n> FTP DOWNLOAD request parameters incorrect. Terminate operation.");

            dlRes = GSMRES_FAIL_NO_REDO;

            procGSM_Download_Step = DOWNLOAD_STEP_REDO;

            tcpRes = TCP_FTP_OTHER_ERR;

            break;
        }

        if ((gsmDownloadRequest.u8ServerID != 1) && (gsmDownloadRequest.u8ServerID != 2))
        {
            gsmDownloadRequest.u8ServerID = 1;
        }

        u32StartTime = HAL_GetTick();

        GSMprintf("\r\n> FTP DOWNLOAD file name: %s", cSrcFilePath);

        u8FTP_RetryCnt = 0;

        dlRes = GSMRES_NULL;

        tcpRes = TCP_OK;

        procGSM_Download_Step = DOWNLOAD_STEP_CFG_PDP;

        break;
    }

    // DOWNLOAD_STEP_CFG_PDP: Config PDP context for the FTP connection
    case DOWNLOAD_STEP_CFG_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_PDP_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD config PDP maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD config PDP ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_CFG_PDP;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_CFG_ACC;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_CFG_ACC: Config account for the FTP connection
    case DOWNLOAD_STEP_CFG_ACC:
    {
        char cUserName[21] = {0};
        char cPassword[21] = {0};

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmDownloadRequest.u8ServerID == 2)
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
        }
        else
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
        }
#else
        sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
        sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
#endif

        bRes = GSM_FTP_CfgAcc(cUserName, cPassword);

        if (!bRes)
        {
            tcpRes = TCP_FTP_ACC_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD config account maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD config account ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_CFG_ACC;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_CFG_FILE_TYPE: Config file type for the FTP connection
    case DOWNLOAD_STEP_CFG_FILE_TYPE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgFileType();

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILETYPE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD config file type maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD config file type ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_CFG_FILE_TYPE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_CFG_TRANS_MODE;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_CFG_TRANS_MODE: Config transferring mode for the FTP connection
    case DOWNLOAD_STEP_CFG_TRANS_MODE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTransMode();

        if (!bRes)
        {
            tcpRes = TCP_FTP_TRANSMODE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD config transferring mode maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD config transferring mode ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_CFG_TRANS_MODE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_CFG_TIMEOUT: Config timeout for the FTP connection
    case DOWNLOAD_STEP_CFG_TIMEOUT:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTimeout((uint16_t)FTP_FUNCTION_TIMEOUT);

        if (!bRes)
        {
            tcpRes = TCP_FTP_TIMEOUT_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD config timeout maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD config timeout ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_CFG_TIMEOUT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        bFTP_SetupFinish = TRUE;

        procGSM_Download_Step = DOWNLOAD_STEP_OPEN;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_OPEN: Send command to open FTP connection
    case DOWNLOAD_STEP_OPEN:
    {
        char cServerAddr[31] = {0};
        uint16_t u16ServerPort = 0;

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmDownloadRequest.u8ServerID == 2)
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP_EXT;
        }
        else
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP;
        }
#else
        sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
        u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

        gsmCurrServerType = CURR_SERVER_FTP;
#endif

        // Open FTP connection
        bRes = GSM_FTP_Login(cServerAddr, u16ServerPort);

        if (!bRes)
        {
            tcpRes = TCP_FTP_LOGIN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP DOWNLOAD open connection maximum loop retries reached.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP DOWNLOAD open connection ERROR. Retry in the next loop.");

            procGSM_Download_Step = DOWNLOAD_STEP_OPEN;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32DownloadLastTick = HAL_GetTick();

        procGSM_Download_Step = DOWNLOAD_STEP_OPEN_WAIT;

        break;
    }

    // DOWNLOAD_STEP_OPEN_WAIT: Polling FTP open connection result
    case DOWNLOAD_STEP_OPEN_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32DownloadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_OPEN, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_OPEN_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP DOWNLOAD open connection wait result TIMEOUT!");

            tcpRes = TCP_FTP_LOGIN_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP DOWNLOAD open connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGIN_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP DOWNLOAD open connection ERROR. Terminate operation.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP DOWNLOAD open connection maximum loop retries reached.");

                    procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Download_Step = DOWNLOAD_STEP_OPEN;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP DOWNLOAD open connection ERROR. Retry in the next loop after %d seconds.", FILE_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP DOWNLOAD open connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD open connection ERROR.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        if (strlen(gsmDownloadRequest.cSrcDir) == 0)
            procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE;
        else
            procGSM_Download_Step = DOWNLOAD_STEP_SET_DIR;

        break;
    }

    // DOWNLOAD_STEP_SET_DIR: Send command to set working directory on FTP server
    case DOWNLOAD_STEP_SET_DIR:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Set working directory on the FTP server
        bRes = GSM_FTP_SetDir(gsmDownloadRequest.cSrcDir);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SETDIR_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP DOWNLOAD set working directory maximum loop retries reached.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> DOWNLOAD set working directory ERROR. Retry in the next loop.");

            procGSM_Download_Step = DOWNLOAD_STEP_SET_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32DownloadLastTick = HAL_GetTick();

        procGSM_Download_Step = DOWNLOAD_STEP_SET_DIR_WAIT;

        break;
    }

    // DOWNLOAD_STEP_SET_DIR_WAIT: Polling FTP set working directory result
    case DOWNLOAD_STEP_SET_DIR_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32DownloadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_SET_DIR, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_SET_DIR_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP DOWNLOAD set working directory wait result TIMEOUT.");

            tcpRes = TCP_FTP_SETDIR_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP DOWNLOAD set working directory get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_SETDIR_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP DOWNLOAD set working directory ERROR. Terminate operation.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP DOWNLOAD set working directory maximum loop retries reached.");

                    procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Download_Step = DOWNLOAD_STEP_SET_DIR;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP DOWNLOAD set working directory ERROR. Retry in the next loop after %d seconds.", FILE_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP DOWNLOAD set working directory ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD set working directory ERROR.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE;

        break;
    }

    // DOWNLOAD_STEP_GET_SIZE: Check the size of the file on the FTP server
    case DOWNLOAD_STEP_GET_SIZE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Get size of the upgrade file
        bRes = GSM_FTP_FileSize(gsmDownloadRequest.cSrcFile);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILESIZE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP DOWNLOAD get file size maximum loop retries reached.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP DOWNLOAD get file size ERROR. Retry in the next loop.");

            procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32DownloadLastTick = HAL_GetTick();

        procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE_WAIT;

        break;
    }

    // DOWNLOAD_STEP_GET_SIZE_WAIT: Polling for FTP get size result
    case DOWNLOAD_STEP_GET_SIZE_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32DownloadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_SIZE, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP DOWNLOAD get file size wait result TIMEOUT.");

            tcpRes = TCP_FTP_FILESIZE_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP DOWNLOAD get file size get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_FILESIZE_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP DOWNLOAD get file size ERROR. Terminate operation.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP DOWNLOAD get file size maximum loop retries reached.");

                    procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Download_Step = DOWNLOAD_STEP_GET_SIZE;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP DOWNLOAD get file size ERROR. Retry in the next loop after %d seconds.", FILE_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP DOWNLOAD get file size ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD get file size ERROR.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        if (u32GetFileSize == 0)
        {
            GSMprintf("\r\n> FTP DOWNLOAD size of download file is 0. Terminate the operation.");

            dlRes = GSMRES_FAIL_NO_REDO;

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            tcpRes = TCP_OK;

            break;
        }

        u32FileSize = u32GetFileSize;

        GSMprintf("\r\n> FTP DOWNLOAD size of download file: %d", u32FileSize);

        tcpRes = TCP_OK;

        procGSM_Download_Step = DOWNLOAD_STEP_RAM_DEL;

        break;
    }

    // DOWNLOAD_STEP_RAM_DEL: Delete RAM content to prepare to download file
    case DOWNLOAD_STEP_RAM_DEL:
    {
        char cDeletePath[20] = {0};

        sprintf(cDeletePath, "%s%s", FTP_LOCAL_STORAGE, STR_STORAGE_DEL_ALL);

        HAL_Delay(10);

        bRes = GSM_FileDelete(cDeletePath);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEDEL_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD RAM delete maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD RAM delete ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_RAM_DEL;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_DOWNLOAD;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_DOWNLOAD: Download required file
    case DOWNLOAD_STEP_DOWNLOAD:
    {
        char cTempFileName[64] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cTempFileName, "%s%s.%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE,
                UPGRADE_FILE_EXT);

        HAL_Delay(10);

        // Download the upgrade file to device RAM
        bRes = GSM_FTP_DownloadFile(gsmDownloadRequest.cSrcFile, cTempFileName, 0);

        if (!bRes)
        {
            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP DOWNLOAD download file %s maximum loop retries reached.", gsmDownloadRequest.cSrcFile);

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP DOWNLOAD download file %s ERROR. Retry in the next loop.", gsmDownloadRequest.cSrcFile);

            procGSM_Download_Step = DOWNLOAD_STEP_DOWNLOAD;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32DownloadLastTick = HAL_GetTick();

        procGSM_Download_Step = DOWNLOAD_STEP_DOWNLOAD_WAIT;

        break;
    }

    // DOWNLOAD_STEP_DOWNLOAD_WAIT: Polling for FTP download result
    case DOWNLOAD_STEP_DOWNLOAD_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32DownloadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_DOWNLOAD, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_DOWNLOAD_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP DOWNLOAD download file wait result TIMEOUT.");

            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP DOWNLOAD download file get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_DOWNLOAD_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP DOWNLOAD download file ERROR. Terminate operation.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP DOWNLOAD download file maximum loop retries reached.");

                    procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Download_Step = DOWNLOAD_STEP_DOWNLOAD;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP DOWNLOAD download file ERROR. Retry in the next loop after %d seconds.", FILE_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP DOWNLOAD download file ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD download file ERROR.");

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

        break;
    }

    // DOWNLOAD_STEP_CLOSE: Send command to close FTP connection
    case DOWNLOAD_STEP_CLOSE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Close connection to FTP server
        bRes = GSM_FTP_Logout();

        if (!bRes)
        {
            tcpRes = TCP_FTP_LOGOUT_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD close FTP connection maximum loop retries reached.");

                if (dlRes == GSMRES_SUCCESS)
                    procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
                else
                    procGSM_Download_Step = DOWNLOAD_STEP_REDO;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP DOWNLOAD close FTP connection ERROR. Retry in the next loop.");

            procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32DownloadLastTick = HAL_GetTick();

        procGSM_Download_Step = DOWNLOAD_STEP_CLOSE_WAIT;

        break;
    }

    // DOWNLOAD_STEP_CLOSE_WAIT: Polling FTP close connection result
    case DOWNLOAD_STEP_CLOSE_WAIT:
    {
        uint16_t u16ErrCode;
        GSM_NonBlockResp_ResTypeDef dlCloseRes;

        if (HAL_GetTick() < (u32DownloadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_CLOSE, &dlCloseRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP DOWNLOAD close FTP connection wait result TIMEOUT.");

            tcpRes = TCP_FTP_LOGOUT_ERR;

            dlCloseRes = GSMRES_FAIL_REDO_LIMIT_RST;

            if (dlRes == GSMRES_SUCCESS)
                procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
            else
                procGSM_Download_Step = DOWNLOAD_STEP_REDO;

            return tcpRes;
        }

        if (dlCloseRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP DOWNLOAD close FTP connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGOUT_ERR;

            // If fail and no need to retry
            if (dlCloseRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP DOWNLOAD close FTP connection ERROR.");

                if (dlRes == GSMRES_SUCCESS)
                    procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
                else
                    procGSM_Download_Step = DOWNLOAD_STEP_REDO;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((dlCloseRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (dlCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (dlCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD close FTP connection maximum loop retries reached.");

                    if (dlRes == GSMRES_SUCCESS)
                        procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
                    else
                        procGSM_Download_Step = DOWNLOAD_STEP_REDO;

                    return tcpRes;
                }

                procGSM_Download_Step = DOWNLOAD_STEP_CLOSE;

                // If need to delay before retry
                if ((dlCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_DOWNLOAD_STOP_TIMEOUT);

                    GSMprintf("\r\n> FTP DOWNLOAD close FTP connection ERROR. Retry in the next loop after %d seconds.", FILE_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP DOWNLOAD close FTP connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD close FTP connection ERROR.");

                if (dlRes == GSMRES_SUCCESS)
                    procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
                else
                    procGSM_Download_Step = DOWNLOAD_STEP_REDO;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        if (dlRes == GSMRES_SUCCESS)
            procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
        else
            procGSM_Download_Step = DOWNLOAD_STEP_REDO;

        break;
    }

    // DOWNLOAD_STEP_RAM_OPEN: Open downloaded file in device RAM
    case DOWNLOAD_STEP_RAM_OPEN:
    {
        char cTempFileName[20] = {0};

        sprintf(cTempFileName, "%s%s.%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE,
                UPGRADE_FILE_EXT);

        HAL_Delay(10);

        bRes = GSM_FileOpen(cTempFileName, &u32FileID, 2);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD open temp file maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD open temp file ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_RAM_OPEN;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE_INIT;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_SAVE_FILE_INIT: Initialize the process to transfer the file to SD card
    case DOWNLOAD_STEP_SAVE_FILE_INIT:
    {
        memset(cDstFilePath, 0x00, sizeof(cDstFilePath));

        if (strlen(gsmDownloadRequest.cDstDir) == 0)
        {
            sprintf(cDstFilePath, "%s", gsmDownloadRequest.cDstFile);

            if (SD_CheckFileExist(cDstFilePath) == TRUE)
            {
                bRes = SD_RemoveFile(cDstFilePath);

                if (bRes)
                {
                    GSMprintf("\r\n> FTP DOWNLOAD remove file %s OK.", cDstFilePath);
                }
                else
                {
                    GSMprintf("\r\n> FTP DOWNLOAD remove file %s ERROR.", cDstFilePath);
                }
            }
            else
            {
                bRes = TRUE;
            }
        }
        else
        {
            sprintf(cDstFilePath, "%s/%s", gsmDownloadRequest.cDstDir, gsmDownloadRequest.cDstFile);

            if (SD_CheckDirExist(gsmDownloadRequest.cDstDir) == TRUE)
            {
                if (SD_CheckFileExist(cDstFilePath) == TRUE)
                {
                    bRes = SD_RemoveFile(cDstFilePath);

                    if (bRes)
                    {
                        GSMprintf("\r\n> FTP DOWNLOAD remove file %s OK.", cDstFilePath);
                    }
                    else
                    {
                        GSMprintf("\r\n> FTP DOWNLOAD remove file %s ERROR.", cDstFilePath);
                    }
                }
                else
                {
                    bRes = TRUE;
                }
            }
            else
            {
                bRes = SD_MakeDir(gsmDownloadRequest.cDstDir);
            }
        }

        if (!bRes)
        {
            tcpRes = TCP_FTP_DF2FILEDEL_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD init save file maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD init save file ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE_INIT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u32DatCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_SAVE_FILE: Read data from the device RAM and write to the Exflash and calculate CRC
    case DOWNLOAD_STEP_SAVE_FILE:
    {
        const uint16_t u16DownloadSize = 1024;
        uint8_t u8DatBuf[1024];
        uint32_t u32ActualReadLen;

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > u16DownloadSize)
            u32DatSize = u16DownloadSize;

        memset(u8DatBuf, 0x00, sizeof(u8DatBuf));

        bRes = GSM_FileRead(u32FileID, u32DatSize, &u32ActualReadLen, (char *)u8DatBuf);

        // GSMprintf("\r\n> FTP DOWNLOAD read %d bytes from temp file in device RAM.", u32ActualReadLen);

        if ((!bRes) || (bRes && (u32ActualReadLen != u32DatSize)))
        {
            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt == 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> FTP DOWNLOAD read data maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procGSM_Download_Step = DOWNLOAD_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> FTP DOWNLOAD read data ERROR. Retry from the beginning of the file in the next loop.");

                    procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE_INIT;
                }
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD read data at address %d ERROR. Retry in the next loop.", u32DatCnt);

                procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        bRes = SD_WriteBuffToAddr(cDstFilePath, u8DatBuf, u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            tcpRes = TCP_FTP_SAVEFILE_ERR;

            u8FileTransRetryCnt++;

            if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                GSMprintf("\r\n> FTP DOWNLOAD write to destination file maximum loop retries reached.");

                u8FileTransRetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Download_Step = DOWNLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD write to destination file at address %d ERROR. Retry in the next loop.", u32DatCnt);

                procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u8FileTransRetryCnt = 0;

        u32DatCnt += u32DatSize;

        // GSMprintf("\r\n> FTP DOWNLOAD finish write %d bytes to destination file.");

        if ((u32DatCnt % (sizeof(u8DatBuf) * 10)) == 0)
            GSMprintf("\r\n\r\n> FTP DOWNLOAD bytes transferred: %d\r\n", u32DatCnt);

        if (u32DatCnt >= u32FileSize)
        {
            GSMprintf("\r\n> FTP DOWNLOAD finish transfer data from device RAM to destination file.");

            dlRes = GSMRES_SUCCESS;

            procGSM_Download_Step = DOWNLOAD_STEP_RAM_CLOSE;
        }
        else
            procGSM_Download_Step = DOWNLOAD_STEP_SAVE_FILE;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_RAM_CLOSE: Close the temp file in device RAM
    case DOWNLOAD_STEP_RAM_CLOSE:
    {
        HAL_Delay(10);

        bRes = GSM_FileClose(u32FileID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILECLOSE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD close temp file maximum loop retries reached.");

                procGSM_Download_Step = DOWNLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP DOWNLOAD close temp file ERROR. Retry in the next loop.");

                procGSM_Download_Step = DOWNLOAD_STEP_RAM_CLOSE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Download_Step = DOWNLOAD_STEP_REDO;

        tcpRes = TCP_OK;

        break;
    }

    // DOWNLOAD_STEP_REDO: If the operation failed then make another request, otherwise conclude
    case DOWNLOAD_STEP_REDO:
    {
        bool bDownloadRedoLimit = FALSE, bDownloadRedoDelay = FALSE, bDownloadRedoRst = FALSE;
        char cDownloadDelayMsg[20] = {0};
        static uint8_t u8StealthRetry = 0;

        if ((dlRes == GSMRES_SUCCESS) || (dlRes == GSMRES_FAIL_NO_REDO))
        {
            switch ((int)gsmDownloadRequest.dlType)
            {
            case FTP_DOWNLOAD_NORMAL:
                break;

            case FTP_DOWNLOAD_FOTA:
                break;

            case FTP_DOWNLOAD_SETTING:
                if (dlRes == GSMRES_SUCCESS)
                {
#if ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
                    bRestoreSettingFlag = TRUE;
#endif
                }
                else
                {
                    GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 0");
                }

                break;

            default:
                break;
            }

            // break;
        }

        if (dlRes == GSMRES_SUCCESS)
        {
            u32EndTime = HAL_GetTick();

            GSMprintf("\r\n> FTP DOWNLOAD OK. Time used: %dms", u32EndTime - u32StartTime);

            GSM_TCP_ReportEvent((char *)EVENT_FTP_DOWNLOAD_OK, STR_NULL);

            if (bSystemMode == SYSTEM_MODE_STEALTH)
                bStealthDownloadEn = FALSE;

            tcpRes = TCP_OK;

            procGSM_Download_Step = DOWNLOAD_STEP_NULL;

            break;
        }
        else
        {
            sprintf(cDownloadDelayMsg, "S:%d-E:%d-C:%d", procDownloadLastStep, dlRes, u16GSM_ErrCode);

            GSM_TCP_ReportEvent((char *)EVENT_FTP_DOWNLOAD_FAIL, cDownloadDelayMsg);

            memset(cDownloadDelayMsg, 0x00, sizeof(cDownloadDelayMsg));

            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                if (bSystemMode == SYSTEM_MODE_STEALTH)
                    bStealthDownloadEn = FALSE;

                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_NULL;

                break;
            }
        }

        // if(gsmDownloadRequest.dlType == FTP_DOWNLOAD_SETTING)
        //	break;

        // In stealth mode always retry until reaching limit
        if (bSystemMode == SYSTEM_MODE_STEALTH)
        {
            u8StealthRetry++;

            GSM_FTP_FileDownloadRequest(&gsmDownloadRequest);

            tcpRes = TCP_OK;

            procGSM_Download_Step = DOWNLOAD_STEP_NULL;

            if (u8StealthRetry >= 2)
            {
                u8StealthRetry = 0;

                bStealthDownloadEn = FALSE;
            }

            break;
        }

        // Check for number of redo
        switch (dlRes)
        {
            // For these cases the number of redo is limited
        case GSMRES_NULL:
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            bDownloadRedoLimit = TRUE;

            break;

        default:
            break;
        }

        // Check for delay
        switch (dlRes)
        {
        // For these cases add a delay before redo
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bDownloadRedoDelay = TRUE;

            break;

        default:
            break;
        }

        // Check for reset GSM module
        switch (dlRes)
        {
            // For these case reset the GSM module before redo
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bDownloadRedoRst = TRUE;

            break;

        default:
            break;
        }

        if (bDownloadRedoLimit == TRUE)
        {
            gsmDownloadRequest.u8ReDownloadCnt++;

            if (gsmDownloadRequest.u8ReDownloadCnt >= FILE_DOWNLOAD_RETRY_MAX)
            {
                gsmDownloadRequest.u8ReDownloadCnt = 0;

                GSMprintf("\r\n> FTP DOWNLOAD maximum redo reached. Terminate operation.");

                tcpRes = TCP_OK;

                procGSM_Download_Step = DOWNLOAD_STEP_NULL;

                break;
            }
        }

        if (bDownloadRedoDelay)
            sprintf(cDownloadDelayMsg, " after %d seconds", STOP_FTP_ACCESS_TIMEOUT);

        GSMprintf("\r\n> FTP DOWNLOAD%s request to redo%s%s.",
                  bDownloadRedoRst ? (char *)" reset GSM module and" : (char *)"",
                  bDownloadRedoDelay ? cDownloadDelayMsg : (char *)"",
                  bDownloadRedoLimit ? (char *)"" : (char *)" (Unlimited)");

        if (bDownloadRedoDelay)
            FuncSkip_Enable(FUNC_GSM_FTP, STOP_FTP_ACCESS_TIMEOUT);

        GSM_FTP_FileDownloadRequest(&gsmDownloadRequest);

        if (bDownloadRedoRst)
            // GSM_Reset();
            bGSM_ResetFlag = TRUE;

        tcpRes = TCP_OK;

        procGSM_Download_Step = DOWNLOAD_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}
#endif

//============================================================================//
// @brief  Put a request to upload a file from the SD card to the FTP server.
// @param  pcSrcFile: Pointer to a char array storing the name of the source file.
//         pcDstFile: Pointer to a char array storing the name of the file on
//         the FTP server.
//         pcSrcDir: Pointer to a char array storing the name of the source directory.
//         pcDstDir: Pointer to a char array storing the name of the destination
//         directory on the FTP server.
//         bReUpload: FALSE: This is a new upload request for a file.
//                    TRUE: This is a reupload request for a file.
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_FileUploadRequest(char *pcSrcFile, char *pcDstFile, char *pcSrcDir,
                               char *pcDstDir, uint8_t u8ServerID, uint8_t u8RetryCnt)
{
    GSM_UploadRequestTypeDef gsmUploadHandle = {0};
    char cFilePath[130];
    uint32_t u32FileSize;
    osStatus osRes;
    bool bRes;

    if ((strlen(pcSrcFile) == 0) ||
        (strlen(pcSrcFile) > sizeof(gsmUploadRequest.cSrcFile)) ||
        (strlen(pcSrcDir) > sizeof(gsmUploadRequest.cSrcDir)) ||
        (strlen(pcDstFile) == 0) ||
        (strlen(pcDstFile) > sizeof(gsmUploadRequest.cDstFile)) ||
        (strlen(pcDstDir) == 0) ||
        (strlen(pcDstDir) > sizeof(gsmUploadRequest.cDstDir)) ||
        ((u8ServerID != 1) && (u8ServerID != 2)))
    {
        GSMprintf("\r\n> FTP upload request has unsuitable parameter.");

        // procGSM_Upload_Step = UPLOAD_STEP_NULL;

        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

    if (strlen(pcSrcDir) == 0)
        sprintf(cFilePath, "%s", pcSrcFile);
    else
        sprintf(cFilePath, "%s/%s", pcSrcDir, pcSrcFile);

    if (SD_CheckFileExist(cFilePath) == FALSE)
    {
        GSMprintf("\r\n> Source file does not exist.");

        if (bRTOS_En)
            // Release GSM_SD synchronization
            osRes = osMutexRelease(mutex_gsm_sd_user);

        return FALSE;
    }

    u32FileSize = SD_GetFileSize(cFilePath);

    if ((u32FileSize == 0) || (u32FileSize > FTP_FILE_UPLOAD_SIZE_MAX))
    {
        GSMprintf("\r\n> Source file size is insufficient.");

        if (bRTOS_En)
            // Release GSM_SD synchronization
            osRes = osMutexRelease(mutex_gsm_sd_user);

        return FALSE;
    }

    strcpy(gsmUploadHandle.cSrcFile, pcSrcFile);
    strcpy(gsmUploadHandle.cSrcDir, pcSrcDir);
    strcpy(gsmUploadHandle.cDstFile, pcDstFile);
    strcpy(gsmUploadHandle.cDstDir, pcDstDir);
    gsmUploadHandle.u32UploadSize = u32FileSize;
    gsmUploadHandle.u8ServerID = u8ServerID;
    gsmUploadHandle.u8ReUploadCnt = u8RetryCnt;

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SEND_FILE_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);
#endif

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)gsmUploadHandle.cSrcFile,
                              sizeof(GSM_UploadRequestTypeDef),
                              2 + (uint32_t)u16UnsendFileNum * sizeof(GSM_UploadRequestTypeDef));

    if (bRes)
    {
        GSM_FTP_IncUploadIdx();

        GSMprintf("\r\n> GSM add FTP upload request for file %s/%s", pcSrcDir, pcSrcFile);

        if (bSystemMode == SYSTEM_MODE_STEALTH)
            bStealthUploadEn = TRUE;
    }
    else
    {
        GSMprintf("\r\n> Write FTP upload request to %s file failed!", cFilePath);
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Get a file upload request from the request list and initialize the
//         upload process.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileUploadInit(void)
{
    bool bRes;
    osStatus osRes;
    char cFilePath[128] = {0};

    if (u16UnsendFileNum == 0)
        return FALSE;

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SEND_FILE_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);
#endif

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)gsmUploadRequest.cSrcFile,
                             sizeof(GSM_UploadRequestTypeDef),
                             2 + ((uint32_t)u16UnsendFileNum - 1) * sizeof(GSM_UploadRequestTypeDef));

    if (bRes)
    {
        GSMprintf("\r\n> Get FTP upload request %d OK.", u16UnsendFileNum);

        procGSM_Upload_Step = UPLOAD_STEP_INIT;

        GSM_FTP_DecUploadIdx();
    }
    else
    {
        GSMprintf("\r\n> Get FTP upload request failed!");
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Peek a file upload request from the request list without remove that
//         file from the request list.
// @param  u16Index: Request index, count from 0.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool GSM_FTP_FileUploadPeek(uint16_t u16Index, GSM_UploadRequestTypeDef *pRequest)
{
    bool bRes;
    osStatus osRes;
    char cFilePath[128] = {0};

    if (u16UnsendFileNum == 0)
        return FALSE;

    if (bRTOS_En)
    {
        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire GSM_SD mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", SEND_FILE_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);
#endif

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)pRequest,
                             sizeof(GSM_UploadRequestTypeDef),
                             2 + (uint32_t)u16Index * sizeof(GSM_UploadRequestTypeDef));

    if (bRes)
    {
        GSMprintf("\r\n> Peek FTP upload request %d OK.", u16Index);
    }
    else
    {
        GSMprintf("\r\n> Peek FTP upload request %d failed!", u16Index);
    }

    if (bRTOS_En)
        // Release GSM_SD synchronization
        osRes = osMutexRelease(mutex_gsm_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Increase the index of pending upload request number by 1 then write
//         the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_IncUploadIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);

    u16UnsendFileNum++;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Decrease the index of the pending upload request number by 1 then
//         write the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_DecUploadIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

    if (u16UnsendFileNum == 0)
        return;

    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);

    u16UnsendFileNum--;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Process the current file upload request.
// @param  None.
// @retval Result in TCP_StatusTypedef format.
//============================================================================//
TCP_StatusTypeDef GSM_FTP_Upload_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    static FTP_UploadStepTypeDef procUploadLastStep;
    static GSM_NonBlockResp_ResTypeDef filupRes;
    static char cSrcFilePath[130];
#if ENABLE_FTPS
    static bool bDownloadCA_Cert = FALSE;
#endif
    static uint32_t u32FileSize, u32FileID, u32DatCnt, u32DatSize;
    static uint32_t u32StartTime = 0, u32FinishTime = 0;
    static uint32_t u32UploadLastTick = 0;

    if (procGSM_Upload_Step == UPLOAD_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    if ((procGSM_Upload_Step != UPLOAD_STEP_RAM_CLOSE) &&
        (procGSM_Upload_Step != UPLOAD_STEP_CLOSE) &&
        (procGSM_Upload_Step != UPLOAD_STEP_CLOSE_WAIT) &&
        (procGSM_Upload_Step != UPLOAD_STEP_REDO))
    {
        procUploadLastStep = procGSM_Upload_Step;
    }

    switch ((int)procGSM_Upload_Step)
    {
    // UPLOAD_STEP_INIT: Check upload parameters
    case UPLOAD_STEP_INIT:
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_FTP_UPLOAD_BEGIN, STR_NULL);
#endif

        if ((strlen(gsmUploadRequest.cSrcFile) == 0) ||
            (strlen(gsmUploadRequest.cSrcFile) > sizeof(gsmUploadRequest.cSrcFile)) ||
            (strlen(gsmUploadRequest.cSrcDir) > sizeof(gsmUploadRequest.cSrcDir)) ||
            (strlen(gsmUploadRequest.cDstFile) == 0) ||
            (strlen(gsmUploadRequest.cDstFile) > sizeof(gsmUploadRequest.cDstFile)) ||
            (strlen(gsmUploadRequest.cDstDir) == 0) ||
            (strlen(gsmUploadRequest.cDstDir) > sizeof(gsmUploadRequest.cDstDir)) ||
            (gsmUploadRequest.u32UploadSize == 0) ||
            (gsmUploadRequest.u32UploadSize > FTP_FILE_UPLOAD_SIZE_MAX))
        {
            GSMprintf("\r\n> FTP UPLOAD request parameters incorrect. Terminal operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_Upload_Step = UPLOAD_STEP_REDO;

            tcpRes = TCP_FTP_OTHER_ERR;

            break;
        }

        u32StartTime = HAL_GetTick();

        memset(cSrcFilePath, 0x00, sizeof(cSrcFilePath));

        if (strlen(gsmUploadRequest.cSrcDir) == 0)
            sprintf(cSrcFilePath, "%s", gsmUploadRequest.cSrcFile);
        else
            sprintf(cSrcFilePath, "%s/%s", gsmUploadRequest.cSrcDir, gsmUploadRequest.cSrcFile);

        if (SD_CheckFileExist(cSrcFilePath) == FALSE)
        {
            GSMprintf("\r\n> FTP UPLOAD source file not found. Terminate operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_Upload_Step = UPLOAD_STEP_REDO;

            tcpRes = TCP_FTP_NO_SRC_FILE;

            break;
        }

        u32FileSize = SD_GetFileSize(cSrcFilePath);

        if ((u32FileSize == 0) || (u32FileSize > FTP_FILE_UPLOAD_SIZE_MAX))
        {
            GSMprintf("\r\n> FTP UPLOAD insufficient source file size. Terminate operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_Upload_Step = UPLOAD_STEP_REDO;

            tcpRes = TCP_FTP_NO_SRC_FILE;

            break;
        }

        if (gsmUploadRequest.u32UploadSize > u32FileSize)
        {
            GSMprintf("\r\n> FTP UPLOAD insufficient source file size. Terminate operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_Upload_Step = UPLOAD_STEP_REDO;

            tcpRes = TCP_FTP_OTHER_ERR;

            break;
        }

        if ((gsmUploadRequest.u8ServerID != 1) && (gsmUploadRequest.u8ServerID != 2))
        {
            gsmUploadRequest.u8ServerID = 2;
        }

        GSMprintf("\r\n> FTP UPLOAD file name %s, size: %d", cSrcFilePath, u32FileSize);

        u8FTP_RetryCnt = 0;

        filupRes = GSMRES_NULL;

        tcpRes = TCP_OK;

        procGSM_Upload_Step = UPLOAD_STEP_RAM_DEL;

        break;
    }

    // UPLOAD_STEP_RAM_DEL: Delete RAM content to prepare to download upgrade file
    case UPLOAD_STEP_RAM_DEL:
    {
        char cDeletePath[20] = {0};

        sprintf(cDeletePath, "%s", STR_STORAGE_DEL_ALL);

        HAL_Delay(10);

        bRes = GSM_FileDelete(cDeletePath);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEDEL_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD RAM delete maximum loop retries reached.");

                procGSM_Upload_Step = UPLOAD_STEP_RAM_OPEN;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD RAM delete ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_RAM_DEL;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_RAM_OPEN,

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_RAM_OPEN: Create a new temp file in the device RAM
    case UPLOAD_STEP_RAM_OPEN:
    {
        char cTempFileName[20] = {0};

#if ENABLE_FTPS
        if (bDownloadCA_Cert == FALSE)
        {
            sprintf(cTempFileName, "%s%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE_2);
        }
        else
        {
            sprintf(cTempFileName, "%s", FTP_SSL_CA_DEF);
        }
#else
        sprintf(cTempFileName, "%s%s", FTP_LOCAL_STORAGE, FTP_LOCAL_STORAGE_FILE_2);
#endif

        HAL_Delay(10);

        bRes = GSM_FileOpen(cTempFileName, &u32FileID, 1);
        // bRes = GSM_FileUpload(cFileName, cTempFileName);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD create TEMP file maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD create TEMP file ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_RAM_OPEN;
            }

            return tcpRes;
        }

#if ENABLE_FTPS
        if (bDownloadCA_Cert == FALSE)
        {
            GSMprintf("\r\n> FTP UPLOAD create TEMP file in device RAM successfully with ID: %d", u32FileID);
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD create CA certificate in device RAM successfully with ID: %d", u32FileID);
        }
#else
        GSMprintf("\r\n> FTP UPLOAD create TEMP file in device RAM successfully with ID: %d", u32FileID);
#endif

        u8FTP_RetryCnt = 0;

        u32DatCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SET_POINTER,

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SET_POINTER: Set file pointer to read the data.
    case UPLOAD_STEP_SET_POINTER:
    {
        bRes = GSM_FileSeek(u32FileID, u32DatCnt);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILESEEK_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> FTP UPLOAD set file pointer maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> UPLOAD STEP set file pointer ERROR. Retry from the beginning of the file in the next loop.");

                    procGSM_Upload_Step = UPLOAD_STEP_RAM_DEL;
                }
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD set file pointer ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SET_POINTER;
            }

            return tcpRes;
        }

        // u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_LOAD_FILE;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_LOAD_FILE: Load the requested file to upload from SD card to device RAM
    case UPLOAD_STEP_LOAD_FILE:
    {
        osStatus osRes = osMutexWait(mutex_data_buf2_user, S_2_mS(20));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> FTP UPLOAD couldn't acquire DATA BUF2 mutex to read file from SD card. osRes:%d", osRes);

            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FileTransRetryCnt++;

            if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                GSMprintf("\r\n> FTP UPLOAD read file from SD card maximum loop retries reached.");

                u8FileTransRetryCnt = 0;

                filupRes = GSMRES_FAIL_REDO_LIMIT;

                procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD read file from SD card at position %d ERROR. Retry in the next loop.", u32DatCnt);

                procGSM_Upload_Step = UPLOAD_STEP_LOAD_FILE;
            }

            return tcpRes;
        }

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > sizeof(u8PublicDataBuf))
            u32DatSize = sizeof(u8PublicDataBuf);

        memset(u8PublicDataBuf, 0x00, sizeof(u8PublicDataBuf));

        bRes = SD_ReadAddrToBuff(cSrcFilePath, u8PublicDataBuf, u32DatSize, u32DatCnt);

        if (!bRes)
        {
            // GSMprintf("\r\n> Read file %s in SD card fail.", cSrcFilePath);

            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FileTransRetryCnt++;

            if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                GSMprintf("\r\n> FTP UPLOAD read file from SD card maximum loop retries reached.");

                u8FileTransRetryCnt = 0;

                filupRes = GSMRES_FAIL_REDO_LIMIT;

                procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD read file from SD card at position %d ERROR. Retry in the next loop.", u32DatCnt);

                procGSM_Upload_Step = UPLOAD_STEP_LOAD_FILE;
            }

            osMutexRelease(mutex_data_buf2_user);

            return tcpRes;
        }

        bRes = GSM_FileWrite(u32FileID, u8PublicDataBuf, u32DatSize);

        osMutexRelease(mutex_data_buf2_user);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt == 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> FTP Upload write data maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> FTP Upload write data ERROR. Retry from the beginning of the file in the next loop.");

                    procGSM_Upload_Step = UPLOAD_STEP_RAM_DEL;
                }
            }
            else
            {
                GSMprintf("\r\n> FTP Upload write data ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SET_POINTER;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u8FileTransRetryCnt = 0;

        u32DatCnt += u32DatSize;

        if ((u32DatCnt % (sizeof(u8PublicDataBuf) * 2)) == 0)
            GSMprintf("\r\n\r\n> FTP UPLOAD bytes loaded: %d.\r\n", u32DatCnt);

        if (u32DatCnt >= u32FileSize)
        {
            GSMprintf("\r\n> FTP UPLOAD finished transfer data from SD card to device RAM.");

            filupRes = GSMRES_SUCCESS;

            procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
        }
        else
            procGSM_Upload_Step = UPLOAD_STEP_SET_POINTER;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_RAM_CLOSE: Close the upload file in device RAM
    case UPLOAD_STEP_RAM_CLOSE:
    {
        HAL_Delay(10);

        bRes = GSM_FileClose(u32FileID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILECLOSE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD close DF2 file maximum loop retries reached.");

                if (filupRes == GSMRES_SUCCESS)
                {
                    procGSM_Upload_Step = UPLOAD_STEP_CFG_PDP;
                }
                else
                    procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD close DF2 file ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_RAM_CLOSE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

#if ENABLE_FTPS
        if (bDownloadCA_Cert == FALSE)
        {
            GSMprintf("\r\n> FTP UPLOAD close TEMP file in device RAM successfully.");

            if (filupRes == GSMRES_SUCCESS)
            {
                memset(cSrcFilePath, 0x00, sizeof(cSrcFilePath));

                sprintf(cSrcFilePath, "%s", (char *)FTP_SSL_CA_CERT_SD);

                u32FileSize = SD_GetFileSize(cSrcFilePath);

                GSMprintf("\r\n> FTP UPLOAD CA Certificate size: %dB", u32FileSize);

                if (u32FileSize == 0)
                {
                    GSMprintf("\r\n> FTP UPLOAD no CA certificate. Terminate operation.");

                    filupRes = GSMRES_FAIL_NO_REDO;

                    procGSM_Upload_Step = UPLOAD_STEP_REDO;

                    tcpRes = TCP_FTP_NO_SRC_FILE;

                    break;
                }

                bDownloadCA_Cert = TRUE;

                procGSM_FTP_Upload_Step = UPLOAD_STEP_RAM_OPEN;
            }
            else
            {
                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD close CA certificate in device RAM successfully.");

            if (filupRes == GSMRES_SUCCESS)
            {
                procGSM_Upload_Step = UPLOAD_STEP_CFG_PDP;
            }
            else
                procGSM_Upload_Step = UPLOAD_STEP_REDO;
        }
#else
        GSMprintf("\r\n> FTP UPLOAD close TEMP file in device RAM successfully.");

        if (filupRes == GSMRES_SUCCESS)
        {
            procGSM_Upload_Step = UPLOAD_STEP_CFG_PDP;
        }
        else
            procGSM_Upload_Step = UPLOAD_STEP_REDO;
#endif

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_CFG_PDP: Config PDP context for the FTP connection
    case UPLOAD_STEP_CFG_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_PDP_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD config PDP maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD config PDP ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_CFG_PDP;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_CFG_ACC;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_CFG_ACC: Config account for the FTP connection
    case UPLOAD_STEP_CFG_ACC:
    {
        char cUserName[21] = {0};
        char cPassword[21] = {0};

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmUploadRequest.u8ServerID == 1)
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
        }
        else
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
        }
#else
        sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
        sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
#endif

        bRes = GSM_FTP_CfgAcc(cUserName, cPassword);

        if (!bRes)
        {
            tcpRes = TCP_FTP_ACC_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD config account maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD config account ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_CFG_ACC;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_CFG_FILE_TYPE: Config file type for the FTP connection
    case UPLOAD_STEP_CFG_FILE_TYPE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgFileType();

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILETYPE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD config file type maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD config file type ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_CFG_FILE_TYPE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_CFG_TRANS_MODE;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_CFG_TRANS_MODE: Config transferring mode for the FTP connection
    case UPLOAD_STEP_CFG_TRANS_MODE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTransMode();

        if (!bRes)
        {
            tcpRes = TCP_FTP_TRANSMODE_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD config transferring mode maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD config transferring mode ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_CFG_TRANS_MODE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_CFG_TIMEOUT: Config timeout for the FTP connection
    case UPLOAD_STEP_CFG_TIMEOUT:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTimeout((uint16_t)FTP_FUNCTION_TIMEOUT);

        if (!bRes)
        {
            tcpRes = TCP_FTP_TIMEOUT_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD config timeout maximum loop retries reached.");

#if ENABLE_FTPS
                procGSM_FTP_Upload_Step = UPLOAD_STEP_SSL_MODE;
#else
                bFTP_SetupFinish = TRUE;

                procGSM_Upload_Step = UPLOAD_STEP_OPEN;
#endif
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD config timeout ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_CFG_TIMEOUT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

#if ENABLE_FTPS
        procGSM_FTP_Upload_Step = UPLOAD_STEP_SSL_MODE;
#else
        bFTP_SetupFinish = TRUE;

        procGSM_Upload_Step = UPLOAD_STEP_OPEN;
#endif

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_MODE: Select FTP SSL mode
    case UPLOAD_STEP_SSL_MODE:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgSSL_Type(1);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_TYPE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD select SSL mode maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD select SSL mode ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_MODE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SSL_PDP;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_PDP: Select FTP SSL context ID
    case UPLOAD_STEP_SSL_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgSSL_ID(FTP_SSL_ID_DEF);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_ID_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD select SSL PDP ID maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD select SSL PDP ID ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_PDP;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SSL_VER;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_VER: Select FTP SSL version
    case UPLOAD_STEP_SSL_VER:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_SSL_Ver(FTP_SSL_ID_DEF, FTP_SSL_VERSION_DEF);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_VER_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD select SSL version maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD select SSL version ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_VER;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SSL_CIPHER;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_CIPHER: Select FTP SSL cipher suit
    case UPLOAD_STEP_SSL_CIPHER:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_SSL_Cipher(FTP_SSL_ID_DEF, (char *)FTP_SSL_CIPHER_SUIT_DEF);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_CIPHER_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD select SSL cipher suit maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD select SSL cipher suit ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_CIPHER;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SSL_CA;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_CA: Configure FTP SSL CA path
    case UPLOAD_STEP_SSL_CA:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_SSL_CA(FTP_SSL_ID_DEF, (char *)FTP_SSL_CA_DEF);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_CACERT_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD configure SSL CA path maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD configure SSL CA path ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_CA;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_Upload_Step = UPLOAD_STEP_SSL_SEC;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_SSL_SEC: Configure FTP SSL security level
    case UPLOAD_STEP_SSL_SEC:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_SSL_SecLV(FTP_SSL_ID_DEF, FTP_SSL_SEC_LEVEL_DEF);

        if (!bRes)
        {
            tcpRes = TCP_FTP_SSL_SECLV_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD configure SSL security level maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procGSM_Upload_Step = UPLOAD_STEP_REDO;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD configure SSL security level ERROR. Retry in the next loop.");

                procGSM_Upload_Step = UPLOAD_STEP_SSL_SEC;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        bFTP_SetupFinish = TRUE;

        procGSM_Upload_Step = UPLOAD_STEP_OPEN;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_STEP_OPEN: Send command to open FTP connection
    case UPLOAD_STEP_OPEN:
    {
        char cServerAddr[31] = {0};
        uint16_t u16ServerPort = 0;

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmUploadRequest.u8ServerID == 1)
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP;
        }
        else
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP_EXT;
        }
#else
        sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
        u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

        gsmCurrServerType = CURR_SERVER_FTP;
#endif

        // Open FTP connection
        bRes = GSM_FTP_Login(cServerAddr, u16ServerPort);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_LOGIN_ERR;

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP UPLOAD open connection maximum loop retries reached.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }

            tcpRes = TCP_FTP_LOGIN_ERR;

            GSMprintf("\r\n> FTP UPLOAD open connection ERROR. Retry in the next loop.");

            procGSM_Upload_Step = UPLOAD_STEP_OPEN;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_OPEN_WAIT;

        break;
    }

    // UPLOAD_STEP_OPEN_WAIT: Polling FTP open connection result
    case UPLOAD_STEP_OPEN_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_OPEN, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_OPEN_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD open connection wait result TIMEOUT!");

            tcpRes = TCP_FTP_LOGIN_ERR;

            filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD open connection get error code %d", u16ErrCode);

            if (u16ErrCode == 627)
            {
                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;
            }

            tcpRes = TCP_FTP_LOGIN_ERR;

            // If fail and no need to retry
            if (filupRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP UPLOAD open connection ERROR. Terminate operation.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((filupRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD open connection maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Upload_Step = UPLOAD_STEP_OPEN;

                // If need to delay before retry
                if ((filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP UPLOAD open connection ERROR. Retry in the next loop after %d seconds.", FILE_UPLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_UPLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP UPLOAD open connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD open connection ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_Upload_Step = UPLOAD_STEP_SET_DIR;

        break;
    }

    // UPLOAD_STEP_SET_DIR: Send command to set working directory on FTP server
    case UPLOAD_STEP_SET_DIR:
    {
        char cFilePath[30] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cFilePath, "%s", gsmUploadRequest.cDstDir);

        HAL_Delay(10);

        bRes = GSM_FTP_SetDir(cFilePath);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_SETDIR_ERR;

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP UPLOAD set working directory maximum loop retries reached.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }

            tcpRes = TCP_FTP_SETDIR_ERR;

            GSMprintf("\r\n> FTP UPLOAD set working directory ERROR. Retry in the next loop.");

            procGSM_Upload_Step = UPLOAD_STEP_SET_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_SET_DIR_WAIT;

        break;
    }

    // UPLOAD_STEP_SET_DIR_WAIT: Polling FTP set working directory result
    case UPLOAD_STEP_SET_DIR_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_SET_DIR, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_SET_DIR_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD set working directory wait result TIMEOUT.");

            tcpRes = TCP_FTP_SETDIR_ERR;

            filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            // if(u16ErrCode != 0)
            //	GSMprintf("\r\n> FTP UPLOAD set working directory get error code %d", u16ErrCode);

            if (u16ErrCode == 627)
            {
                tcpRes = TCP_FTP_SETDIR_ERR;

                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    filupRes = GSMRES_FAIL_REDO_LIMIT;

                    GSMprintf("\r\n> FTP UPLOAD set working directory maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                GSMprintf("\r\n> FTP UPLOAD set working directory ERROR 627. Proceed to create new directory.");

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_MAKE_DIR;

                return tcpRes;
            }

            tcpRes = TCP_FTP_SETDIR_ERR;

            // If fail and no need to retry
            if (filupRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP UPLOAD set working directory ERROR. Terminate operation.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((filupRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD set working directory maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Upload_Step = UPLOAD_STEP_SET_DIR;

                // If need to delay before retry
                if ((filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP UPLOAD set working directory ERROR. Retry in the next loop after %d seconds.", FILE_UPLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_UPLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP UPLOAD set working directory ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD set working directory ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        // procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;
        // procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;
        procGSM_Upload_Step = UPLOAD_STEP_GET_SIZE;

        break;
    }

    // UPLOAD_STEP_MAKE_DIR: Send command to set working directory on FTP server
    case UPLOAD_STEP_MAKE_DIR:
    {
        char cFilePath[30] = {0};

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        sprintf(cFilePath, "%s", gsmUploadRequest.cDstDir);

        HAL_Delay(10);

        bRes = GSM_FTP_MakeDir(cFilePath);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_MAKEDIR_ERR;

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP UPLOAD create directory maximum loop retries reached.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }

            tcpRes = TCP_FTP_MAKEDIR_ERR;

            GSMprintf("\r\n> FTP UPLOAD create directory ERROR. Retry in the next loop.");

            procGSM_Upload_Step = UPLOAD_STEP_MAKE_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_MAKE_DIR_WAIT;

        break;
    }

    // UPLOAD_STEP_MAKE_DIR_WAIT: Polling FTP create directory result
    case UPLOAD_STEP_MAKE_DIR_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_MAKE_DIR, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_MAKE_DIR_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD create directory wait result TIMEOUT.");

            tcpRes = TCP_FTP_MAKEDIR_ERR;

            filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            // if(u16ErrCode != 0)
            //	GSMprintf("\r\n> FTP UPLOAD create directory get error code %d", u16ErrCode);

            if (u16ErrCode == 627)
            {
                tcpRes = TCP_FTP_MAKEDIR_ERR;

                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    filupRes = GSMRES_FAIL_REDO_LIMIT;

                    GSMprintf("\r\n> FTP UPLOAD create directory maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                GSMprintf("\r\n> FTP UPLOAD create directory ERROR 627. Proceed to set working directory.");

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_SET_DIR;

                return tcpRes;
            }

            tcpRes = TCP_FTP_MAKEDIR_ERR;

            // If fail and no need to retry
            if (filupRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP UPLOAD create directory ERROR. Terminate operation.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((filupRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD create directory maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Upload_Step = UPLOAD_STEP_MAKE_DIR;

                // If need to delay before retry
                if ((filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP UPLOAD create directory ERROR. Retry in the next loop after %d seconds.", FILE_UPLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_UPLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP UPLOAD create directory ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD create directory ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        procGSM_Upload_Step = UPLOAD_STEP_SET_DIR;

        break;
    }

    // UPLOAD_STEP_GET_SIZE: Get size of the file on the FTP server
    case UPLOAD_STEP_GET_SIZE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Get file size
        bRes = GSM_FTP_FileSize(gsmUploadRequest.cDstFile);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILESIZE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD check file %s size maximum loop retries reached. Proceed to delete file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD check file %s size ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_GET_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_GET_SIZE_WAIT;

        break;
    }

    // UPLOAD_STEP_GET_SIZE_WAIT: Polling for FTP get file size
    case UPLOAD_STEP_GET_SIZE_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_SIZE, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_GET_SIZE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD check file size wait result TIMEOUT. Proceed to delete file.");

            tcpRes = TCP_FTP_FILESIZE_ERR;

            // filupRes = GSMRES_FAIL_NO_REDO;
            filupRes = GSMRES_SUCCESS;

            procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16GSM_ErrCode == 627)
            {
                tcpRes = TCP_FTP_FILESIZE_ERR;

                // GSMprintf("\r\n> FTP UPLOAD check file %s size ERROR 627. Proceed to upload file.", gsmUploadRequest.cDstFile);
                GSMprintf("\r\n> FTP UPLOAD file %s does not exist. Proceed to upload file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

                return tcpRes;
            }

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_FILESIZE_ERR;

                GSMprintf("\r\n> FTP UPLOAD check file %s size maximum loop retries reached. Proceed to delete file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;

                return tcpRes;
            }

            tcpRes = TCP_FTP_FILESIZE_ERR;

            GSMprintf("\r\n> FTP UPLOAD check file %s size ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_GET_SIZE;

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        tcpRes = TCP_OK;

        filupRes = GSMRES_SUCCESS;

        if (u32GetFileSize >= u32FileSize)
        {
            GSMprintf("\r\n> FTP UPLOAD upload file already exists, size: %d. Terminate operation.",
                      u32GetFileSize);

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD upload file is incomplete, size: %d. Proceed to delete and reupload file.",
                      u32GetFileSize);

            procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;
        }

        break;
    }

    // UPLOAD_STEP_FILE_DEL: Delete any file of the same name before upload it to the server
    case UPLOAD_STEP_FILE_DEL:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Upload file to server
        bRes = GSM_FTP_FileDel(gsmUploadRequest.cDstFile);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_FILEDEL_ERR;

                GSMprintf("\r\n> FTP UPLOAD delete file %s maximum loop retries reached. Proceed to upload file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

                return tcpRes;
            }

            tcpRes = TCP_FTP_FILEDEL_ERR;

            GSMprintf("\r\n> FTP UPLOAD delete file %s ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL_WAIT;

        break;
    }

    // UPLOAD_STEP_FILE_DEL_WAIT: Polling for FTP delete file
    case UPLOAD_STEP_FILE_DEL_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_DEL, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD delete file wait result TIMEOUT. Proceed to upload file.");

            tcpRes = TCP_FTP_FILEDEL_ERR;

            // filupRes = GSMRES_FAIL_NO_REDO;
            filupRes = GSMRES_SUCCESS;

            procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16GSM_ErrCode == 627)
            {
                tcpRes = TCP_FTP_FILEDEL_ERR;

                // GSMprintf("\r\n> FTP UPLOAD delete file %s ERROR 627. Proceed to upload file.", gsmUploadRequest.cDstFile);
                GSMprintf("\r\n> FTP UPLOAD file %s does not exist. Proceed to upload file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

                return tcpRes;
            }

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_FILEDEL_ERR;

                GSMprintf("\r\n> FTP UPLOAD delete file %s maximum loop retries reached. Proceed to upload file.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

                return tcpRes;
            }

            tcpRes = TCP_FTP_FILEDEL_ERR;

            GSMprintf("\r\n> FTP UPLOAD delete file %s ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_FILE_DEL;

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        tcpRes = TCP_OK;

        filupRes = GSMRES_SUCCESS;

        procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

        break;
    }

    // UPLOAD_STEP_UPLOAD: Upload file to the server
    case UPLOAD_STEP_UPLOAD:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Upload file to server
        bRes = GSM_FTP_UploadFile((char *)FTP_LOCAL_STORAGE_FILE_2, gsmUploadRequest.cDstFile);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_UPLOAD_ERR;

                filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

                GSMprintf("\r\n> FTP UPLOAD upload file %s maximum loop retries reached.", gsmUploadRequest.cDstFile);

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }

            tcpRes = TCP_FTP_UPLOAD_ERR;

            GSMprintf("\r\n> FTP UPLOAD upload file %s ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_UPLOAD_WAIT;

        break;
    }

    // UPLOAD_STEP_UPLOAD_WAIT: Polling for FTP file upload
    case UPLOAD_STEP_UPLOAD_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_UPLOAD, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD upload file wait result TIMEOUT.");

            tcpRes = TCP_FTP_UPLOAD_ERR;

            filupRes = GSMRES_FAIL_REDO_LIMIT_RST;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD upload file get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_UPLOAD_ERR;

            // If fail and no need to retry
            if (filupRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP UPLOAD upload file ERROR. Terminate operation.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((filupRes == GSMRES_FAIL_REDO_LIMIT) ||
                (filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD upload file maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                    return tcpRes;
                }

                procGSM_Upload_Step = UPLOAD_STEP_UPLOAD;

                // If need to delay before retry
                if ((filupRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> FTP UPLOAD upload file ERROR. Retry in the next loop after %d seconds.", FILE_UPLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_UPLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP UPLOAD upload file ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD upload file ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }
        }

        u32DatCnt = 0;

        tcpRes = TCP_OK;

        procGSM_Upload_Step = UPLOAD_STEP_CHECK_SIZE;

        break;
    }

    // UPLOAD_STEP_CHECK_SIZE: Get size of the file uploaded to the server
    case UPLOAD_STEP_CHECK_SIZE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Get file size
        bRes = GSM_FTP_FileSize(gsmUploadRequest.cDstFile);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_FILESIZE_ERR;

                GSMprintf("\r\n> FTP UPLOAD check file %s size maximum loop retries reached. Operation OK.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_LIMIT_RST;
                filupRes = GSMRES_SUCCESS;

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                return tcpRes;
            }

            tcpRes = TCP_FTP_UPLOAD_ERR;

            GSMprintf("\r\n> FTP UPLOAD check file %s size ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_Upload_Step = UPLOAD_STEP_CHECK_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_CHECK_SIZE_WAIT;

        break;
    }

    // UPLOAD_STEP_CHECK_SIZE_WAIT: Polling for FTP get file size
    case UPLOAD_STEP_CHECK_SIZE_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_SIZE, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_CHECK_SIZE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD check file size wait result TIMEOUT. Operation OK.");

            tcpRes = TCP_FTP_FILESIZE_ERR;

            // filupRes = GSMRES_FAIL_NO_REDO;
            filupRes = GSMRES_SUCCESS;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            GSMprintf("\r\n> FTP UPLOAD check file size ERROR. Operation OK.");

            filupRes = GSMRES_SUCCESS;

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        if (u32GetFileSize != u32FileSize)
        {
            GSMprintf("\r\n> FTP UPLOAD uploaded file size is not correct(%d != %d)! Operation failed!",
                      u32GetFileSize,
                      u32FileSize);

            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;
            // filupRes = GSMRES_FAIL_NO_REDO;
        }
        else
        {
            filupRes = GSMRES_SUCCESS;
        }

        procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

        break;
    }

    // UPLOAD_STEP_CLOSE: Send command to close FTP connection
    case UPLOAD_STEP_CLOSE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Close connection to FTP server
        bRes = GSM_FTP_Logout();

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                tcpRes = TCP_FTP_LOGOUT_ERR;

                GSMprintf("\r\n> FTP UPLOAD close FTP connection maximum loop retries reached.");

                procGSM_Upload_Step = UPLOAD_STEP_REDO;

                return tcpRes;
            }

            tcpRes = TCP_FTP_LOGOUT_ERR;

            GSMprintf("\r\n> FTP UPLOAD close FTP connection ERROR. Retry in the next loop.");

            procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_Upload_Step = UPLOAD_STEP_CLOSE_WAIT;

        break;
    }

    // UPLOAD_STEP_CLOSE_WAIT: Polling FTP close connection result
    case UPLOAD_STEP_CLOSE_WAIT:
    {
        uint16_t u16ErrCode;
        GSM_NonBlockResp_ResTypeDef filupCloseRes;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_CLOSE, &filupCloseRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD close FTP connection wait result TIMEOUT.");

            tcpRes = TCP_FTP_LOGOUT_ERR;

            procGSM_Upload_Step = UPLOAD_STEP_REDO;

            return tcpRes;
        }

        if (filupCloseRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD close FTP connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGOUT_ERR;

            // If fail and no need to retry
            if (filupCloseRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> FTP UPLOAD close FTP connection ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_REDO;

                return tcpRes;
            }
            // If no need to reset the GSM module
            else if ((filupCloseRes == GSMRES_FAIL_REDO_LIMIT) ||
                     (filupCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                     (filupCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8FTP_RetryCnt++;

                if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8FTP_RetryCnt = 0;

                    GSMprintf("\r\n> FTP UPLOAD close FTP connection maximum loop retries reached.");

                    procGSM_Upload_Step = UPLOAD_STEP_REDO;

                    return tcpRes;
                }

                procGSM_Upload_Step = UPLOAD_STEP_CLOSE;

                // If need to delay before retry
                if ((filupCloseRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (filupCloseRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    FuncSkip_Enable(FUNC_GSM_FTP, FILE_UPLOAD_STOP_TIMEOUT);

                    GSMprintf("\r\n> FTP UPLOAD close FTP connection ERROR. Retry in the next loop after %d seconds.", FILE_UPLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> FTP UPLOAD close FTP connection ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD close FTP connection ERROR.");

                procGSM_Upload_Step = UPLOAD_STEP_REDO;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        if (filupRes != GSMRES_SUCCESS)
            procGSM_Upload_Step = UPLOAD_STEP_REDO;
        else
        {
            // u32FinishTime = HAL_GetTick();

            // GSMprintf("\r\n >>> ### FTP UPLOAD OK, time: %d (ms) ###", u32FinishTime - u32StartTime);

            procGSM_Upload_Step = UPLOAD_STEP_REDO;
        }

        break;
    }

    // UPLOAD_STEP_REDO: If the operation is failed then make another request
    case UPLOAD_STEP_REDO:
    {
        bool bUploadRedoLimit = FALSE, bUploadRedoDelay = FALSE, bUploadRedoRst = FALSE;
        bool bIsVDR_File = FALSE;
        char cUploadDelayMsg[80] = {0};
        // #if ENABLE_UPLOAD_CHK
        char cFileNameTmp[128] = {0};
        char *pcPtr;
        // #endif
        static uint8_t u8StealthRetry = 0;

        if ((filupRes == GSMRES_SUCCESS) ||
            (filupRes == GSMRES_FAIL_NO_REDO))
        {
            tcpRes = TCP_OK;

            u32FinishTime = HAL_GetTick();

            if (filupRes == GSMRES_SUCCESS)
            {
                GSMprintf("\r\n >>> ### FTP UPLOAD OK, time: %d (ms) ###", u32FinishTime - u32StartTime);

                GSM_TCP_ReportEvent((char *)EVENT_FTP_UPLOAD_OK, STR_NULL);
            }
            else
            {
                GSMprintf("\r\n ### FTP UPLOAD ERROR NO REDO ###");

                sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d",
                        procUploadLastStep,
                        filupRes,
                        u16GSM_ErrCode);

                GSM_TCP_ReportEvent((char *)EVENT_FTP_UPLOAD_FAIL, cUploadDelayMsg);

                memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
            }

            procGSM_Upload_Step = UPLOAD_STEP_NULL;

            if (bSystemMode == SYSTEM_MODE_STEALTH)
                bStealthUploadEn = FALSE;

            // If request to upload VDR file then do upload check
            // #if ENABLE_UPLOAD_CHK
            // memset(cFileNameTmp, 0x00, sizeof(cFileNameTmp));

            // strcpy(cFileNameTmp, gsmUploadRequest.cSrcFile);

            // pcPtr = cFileNameTmp;

            // StopChar(pcPtr, '.');

            // pcPtr += strlen(pcPtr) + 1;

            // if(strncmp(pcPtr, (char *)"VDR", 3) == 0)
            //{
            //	if(bDoUploadChk == FALSE)
            //     GSM_FTP_UploadChkHandle();
            // }
            // #endif

            break;
        }

        memset(cFileNameTmp, 0x00, sizeof(cFileNameTmp));

        strcpy(cFileNameTmp, gsmUploadRequest.cSrcFile);

        pcPtr = cFileNameTmp;

        StopChar(pcPtr, '.');

        pcPtr += strlen(pcPtr) + 1;

        if (strncmp(pcPtr, (char *)"VDR", 3) == 0)
        {
            bIsVDR_File = TRUE;

            switch (filupRes)
            {
            // For the cases where the number of redo is limited
            case GSMRES_FAIL_REDO_LIMIT:
            case GSMRES_FAIL_REDO_LIMIT_DELAY:
                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

                break;

            case GSMRES_FAIL_REDO_LIMIT_RST:
            case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                break;

            default:
                break;
            }
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d",
                    procUploadLastStep,
                    filupRes,
                    u16GSM_ErrCode);

            GSM_TCP_ReportEvent((char *)EVENT_FTP_UPLOAD_FAIL, cUploadDelayMsg);

            memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
        }

        // In stealth mode always retry until reaching limit
        if (bSystemMode == SYSTEM_MODE_STEALTH)
        {
            u8StealthRetry++;

            gsmUploadRequest.u8ReUploadCnt = 0;

            GSM_FTP_FileUploadRequest(gsmUploadRequest.cSrcFile,
                                      gsmUploadRequest.cDstFile,
                                      gsmUploadRequest.cSrcDir,
                                      gsmUploadRequest.cDstDir,
                                      gsmUploadRequest.u8ServerID,
                                      gsmUploadRequest.u8ReUploadCnt);

            tcpRes = TCP_OK;

            procGSM_Upload_Step = UPLOAD_STEP_NULL;

            if (u8StealthRetry >= 2)
            {
                u8StealthRetry = 0;

                bStealthUploadEn = FALSE;
            }

            break;
        }

        // Check for number of redo
        switch (filupRes)
        {
            // For these cases the number of redo is limited
        case GSMRES_NULL:
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            bUploadRedoLimit = TRUE;

            break;

        default:
            break;
        }

        // Check for delay
        switch (filupRes)
        {
        // For these cases add a delay before redo
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bUploadRedoDelay = TRUE;

            break;

        default:
            break;
        }

        // Check for reset GSM module
        switch (filupRes)
        {
            // For these case reset the GSM module before redo
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bUploadRedoRst = TRUE;

            break;

        default:
            break;
        }

        if (bUploadRedoLimit == TRUE)
        {
            gsmUploadRequest.u8ReUploadCnt++;

            if (gsmUploadRequest.u8ReUploadCnt >= FILE_UPLOAD_RETRY_MAX)
            {
                gsmUploadRequest.u8ReUploadCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD maximum redo reached. Terminate operation.");

                // GSM_TCP_ReportEvent(EVENT_FTP_UPLOAD_FAIL, gsmUploadRequest.cSrcFile);

                tcpRes = TCP_OK;

                procGSM_Upload_Step = UPLOAD_STEP_NULL;

                // If request to upload VDR file then do upload check
                // #if ENABLE_UPLOAD_CHK
                // memset(cFileNameTmp, 0x00, sizeof(cFileNameTmp));

                // strcpy(cFileNameTmp, gsmUploadRequest.cSrcFile);

                // pcPtr = cFileNameTmp;

                // StopChar(pcPtr, '.');

                // pcPtr += strlen(pcPtr) + 1;

                // if(strncmp(pcPtr, (char *)"VDR", 3) == 0)
                //{
                //	if(bDoUploadChk == FALSE)
                //		GSM_FTP_UploadChkHandle();
                // }
                // #endif

                break;
            }

            System_SaveSetting();
        }

        if (bUploadRedoDelay)
        {
            uint32_t u32DelayTime = 0;

            if ((bIsVDR_File == TRUE) && (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST))
                u32DelayTime = STOP_UPLOAD_VDR_TIMEOUT;
            else
                u32DelayTime = STOP_FTP_ACCESS_TIMEOUT;

            sprintf(cUploadDelayMsg, " after %d seconds", u32DelayTime);

            GSMprintf("\r\n> FTP UPLOAD%s request to redo%s%s.",
                      bUploadRedoRst ? (char *)" reset GSM module and" : (char *)"",
                      bUploadRedoDelay ? cUploadDelayMsg : (char *)"",
                      bUploadRedoLimit ? (char *)"" : (char *)" (Unlimited)");

            if (bUploadRedoDelay)
                FuncSkip_Enable(FUNC_GSM_FTP, u32DelayTime);
        }

        if (bUploadRedoRst)
            // GSM_Reset();
            bGSM_ResetFlag = TRUE;

        GSM_FTP_FileUploadRequest(gsmUploadRequest.cSrcFile,
                                  gsmUploadRequest.cDstFile,
                                  gsmUploadRequest.cSrcDir,
                                  gsmUploadRequest.cDstDir,
                                  gsmUploadRequest.u8ServerID,
                                  gsmUploadRequest.u8ReUploadCnt);

        tcpRes = TCP_OK;

        procGSM_Upload_Step = UPLOAD_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}

#if ENABLE_UPLOAD_CHK
/*
//============================================================================//
// @brief  Save the upload check request to temp file.
// @param  pgsmUploadChk: Pointer to a variable of type GSM_UploadChkTypeDef
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_SaveUploadChk(GSM_UploadChkTypeDef *pgsmUploadChk)
{
  char cFilePath[128];
    osStatus osRes;
    bool bRes;


    if((strlen(pgsmUploadChk -> cSrcFile) == 0) ||
         (strlen(pgsmUploadChk -> cDstFile) == 0) ||
         (strlen(pgsmUploadChk -> cDstDir) == 0) ||
       ((pgsmUploadChk -> u8ServerID != 1) && (pgsmUploadChk -> u8ServerID != 2)))
    {
        GSMprintf("\r\n> GSM UPLOAD CHK request has unsuitable parameter.");

        return FALSE;
    }

    if(bRTOS_En)
    {
        // Acquire UPLOAD_CHK synchronization
        osRes = osMutexWait(mutex_upload_chk_user, S_2_mS(10));

        if(osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire UPLOAD_CHK mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

    #ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
    #else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
    #endif

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)pgsmUploadChk,
                              sizeof(GSM_UploadChkTypeDef),
                                                        0);

  if(bRes)
    {
        if(strlen(pgsmUploadChk -> cSrcDir) == 0)
        {
            GSMprintf("\r\n> GSM UPLOAD CHK update check request for file %s OK, require check: %d",
                    pgsmUploadChk -> cSrcFile,
                    pgsmUploadChk -> bRequireChk);
        }
        else
        {
          GSMprintf("\r\n> GSM UPLOAD CHK update check request for file %s/%s OK, require check: %d",
                    pgsmUploadChk -> cSrcDir,
                    pgsmUploadChk -> cSrcFile,
                    pgsmUploadChk -> bRequireChk);
        }
    }
    else
    {
        if(strlen(pgsmUploadChk -> cSrcDir) == 0)
        {
            GSMprintf("\r\n> GSM UPLOAD CHK update check request for file %s ERROR",
                    pgsmUploadChk -> cSrcFile);
        }
        else
        {
          GSMprintf("\r\n> GSM UPLOAD CHK update check request for file %s/%s ERROR",
                    pgsmUploadChk -> cSrcDir,
                    pgsmUploadChk -> cSrcFile);
        }
    }

    if(bRTOS_En)
        // Release UPLOAD_CHK synchronization
        osRes = osMutexRelease(mutex_upload_chk_user);

    return bRes;
}

//============================================================================//
// @brief  Get the upload check request from temp file.
// @param  pgsmUploadChk: Pointer to a variable of type GSM_UploadChkTypeDef
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_GetUploadChk(GSM_UploadChkTypeDef *pgsmUploadChk)
{
  char cFilePath[128];
    osStatus osRes;
    bool bRes;


    if(bRTOS_En)
    {
        // Acquire UPLOAD_CHK synchronization
        osRes = osMutexWait(mutex_upload_chk_user, S_2_mS(10));

        if(osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire UPLOAD_CHK mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

    #ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
    #else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
    #endif

    if(SD_CheckFileExist(cFilePath) == FALSE)
    {
        GSMprintf("\r\n> GSM UPLOAD CHK request file not exist.");

        if(bRTOS_En)
          // Release UPLOAD_CHK synchronization
          osRes = osMutexRelease(mutex_upload_chk_user);

        return FALSE;
    }

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)pgsmUploadChk,
                             sizeof(GSM_UploadChkTypeDef),
                                                     0);

  if(bRes)
    {
        GSMprintf("\r\n> GSM UPLOAD CHK get check data OK.");
    }
    else
    {
        GSMprintf("\r\n> GSM UPLOAD CHK get check data ERROR.");
    }

    if(bRTOS_En)
        // Release UPLOAD_CHK synchronization
        osRes = osMutexRelease(mutex_upload_chk_user);

    return bRes;
}
*/

//============================================================================//
// @brief  Save the upload check request to temp file.
// @param  pgsmUploadChk: Pointer to a variable of type GSM_UploadChkTypeDef
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_SaveUploadChk(GSM_UploadChkTypeDef *pgsmUploadChk)
{
    char cFilePath[128];
    uint32_t u32Addr = 0;
    osStatus osRes;
    bool bRes;

    // If the upload check file is already full
    if ((((tUploadChkHandle.u16Tail + 1) % UPLOAD_CHK_NUM_MAX) ==
         tUploadChkHandle.u16Head) ||
        (tUploadChkHandle.u16Size >= (UPLOAD_CHK_NUM_MAX - 1)))
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_FULL, STR_NULL);
#endif

        return FALSE;
    }

    if ((strlen(pgsmUploadChk->cSrcFile) == 0) ||
        (strlen(pgsmUploadChk->cDstFile) == 0) ||
        (strlen(pgsmUploadChk->cDstDir) == 0) ||
        ((pgsmUploadChk->u8ServerID != 1) && (pgsmUploadChk->u8ServerID != 2)))
    {
        GSMprintf("\r\n> GSM UPLOAD CHK request has unsuitable parameter.");

        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire UPLOAD_CHK synchronization
        osRes = osMutexWait(mutex_upload_chk_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire UPLOAD_CHK mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
#endif

    u32Addr = sizeof(FIFO_IndexTypeDef) + (uint32_t)tUploadChkHandle.u16Tail * sizeof(GSM_UploadChkTypeDef);

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)pgsmUploadChk,
                              sizeof(GSM_UploadChkTypeDef),
                              u32Addr);

    if (bRes)
    {
        if (strlen(pgsmUploadChk->cSrcDir) == 0)
        {
            GSMprintf("\r\n> GSM UPLOAD CHK save check request %d for file %s OK",
                      tUploadChkHandle.u16Tail,
                      pgsmUploadChk->cSrcFile);
        }
        else
        {
            GSMprintf("\r\n> GSM UPLOAD CHK update check request %d for file %s/%s OK",
                      tUploadChkHandle.u16Tail,
                      pgsmUploadChk->cSrcDir,
                      pgsmUploadChk->cSrcFile);
        }

        GSM_FTP_UploadChkIncTail();
    }
    else
    {
        if (strlen(pgsmUploadChk->cSrcDir) == 0)
        {
            GSMprintf("\r\n> GSM UPLOAD CHK save check request for file %s ERROR",
                      pgsmUploadChk->cSrcFile);
        }
        else
        {
            GSMprintf("\r\n> GSM UPLOAD CHK save check request for file %s/%s ERROR",
                      pgsmUploadChk->cSrcDir,
                      pgsmUploadChk->cSrcFile);
        }
    }

    if (bRTOS_En)
        // Release UPLOAD_CHK synchronization
        osRes = osMutexRelease(mutex_upload_chk_user);

    return bRes;
}

//============================================================================//
// @brief  Get the upload check request from temp file.
// @param  pgsmUploadChk: Pointer to a variable of type GSM_UploadChkTypeDef
// @result True: The operation is successful
//         False: The operation is failed.
//============================================================================//
bool GSM_FTP_InitUploadChk(void)
{
    char cFilePath[128];
    uint32_t u32Addr = 0;
    osStatus osRes;
    bool bRes;

    // If the upload check file is empty
    if ((tUploadChkHandle.u16Head == tUploadChkHandle.u16Tail) ||
        (tUploadChkHandle.u16Size == 0))
    {
        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire UPLOAD_CHK synchronization
        osRes = osMutexWait(mutex_upload_chk_user, S_2_mS(10));

        if (osRes != osOK)
        {
            GSMprintf("\r\n> Can't acquire UPLOAD_CHK mutex. osRes:%d", osRes);

            return FALSE;
        }
    }

    memset(cFilePath, 0x00, sizeof(cFilePath));

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
#endif

    if (SD_CheckFileExist(cFilePath) == FALSE)
    {
        GSMprintf("\r\n> GSM UPLOAD CHK request file not exist.");

        if (bRTOS_En)
            // Release UPLOAD_CHK synchronization
            osRes = osMutexRelease(mutex_upload_chk_user);

        return FALSE;
    }

    memset(&gsmUploadChkData, 0x00, sizeof(GSM_UploadChkTypeDef));

    u32Addr = sizeof(FIFO_IndexTypeDef) + (uint32_t)tUploadChkHandle.u16Head * sizeof(GSM_UploadChkTypeDef);

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)&gsmUploadChkData,
                             sizeof(GSM_UploadChkTypeDef),
                             u32Addr);

    if (bRes)
    {
        GSMprintf("\r\n> GSM UPLOAD CHK get check request %d OK.", tUploadChkHandle.u16Head);

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_INIT;

        // GSM_FTP_UploadChkIncHead();
    }
    else
    {
        GSMprintf("\r\n> GSM UPLOAD CHK get check request %d ERROR.", tUploadChkHandle.u16Head);
    }

    if (bRTOS_En)
        // Release UPLOAD_CHK synchronization
        osRes = osMutexRelease(mutex_upload_chk_user);

    return bRes;
}

//============================================================================//
// @brief  Increase the tail index of the pending upload check request file.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_UploadChkIncTail(void)
{
    bool bRes;
    char cFilePath[64] = {0};

    // If the upload check file is already full
    if ((((tUploadChkHandle.u16Tail + 1) % UPLOAD_CHK_NUM_MAX) ==
         tUploadChkHandle.u16Head) ||
        (tUploadChkHandle.u16Size >= (UPLOAD_CHK_NUM_MAX - 1)))
    {
        return;
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
#endif

    tUploadChkHandle.u16Tail = (tUploadChkHandle.u16Tail + 1) % UPLOAD_CHK_NUM_MAX;

    tUploadChkHandle.u16Size++;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFilePath, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);

        if (bRes)
            break;
    }
}

//============================================================================//
// @brief  Increase the head index of the pending upload check request file.
// @param  None.
// @retval None.
//============================================================================//
void GSM_FTP_UploadChkIncHead(void)
{
    bool bRes;
    char cFilePath[64] = {0};

    // If the upload check file is empty
    if ((tUploadChkHandle.u16Head == tUploadChkHandle.u16Tail) ||
        (tUploadChkHandle.u16Size == 0))
    {
        return;
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", UPLOAD_CHK_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);
#endif

    tUploadChkHandle.u16Head = (tUploadChkHandle.u16Head + 1) % UPLOAD_CHK_NUM_MAX;

    tUploadChkHandle.u16Size--;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFilePath, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);

        if (bRes)
            break;
    }
}

/*
//============================================================================//
// @brief  Check for any request to recheck an uploaded file on FTP server.
// @param  None.
// @result None.
//============================================================================//
void GSM_FTP_UploadChkHandle(void)
{
    bool bRes;


  memset(&gsmUploadChkData, 0x00, sizeof(GSM_UploadChkTypeDef));

    bRes = GSM_FTP_GetUploadChk(&gsmUploadChkData);

    if(bRes == FALSE)
    {
      //GSMprintf("\r\n> GSM UPLOAD CHK check request get data ERROR.");

        return;
    }

    if(gsmUploadChkData.bRequireChk == TRUE)
    {
        if(strlen(gsmUploadChkData.cSrcDir) == 0)
        {
          GSMprintf("\r\n> GSM UPLOAD CHK init check upload file %s", gsmUploadChkData.cSrcFile);
        }
        else
        {
          GSMprintf("\r\n> GSM UPLOAD CHK init check upload file %s/%s", gsmUploadChkData.cSrcDir, gsmUploadChkData.cSrcFile);
        }

      bDoUploadChk = TRUE;
    }
}
*/

//============================================================================//
// @brief  Process the current checking uploaded file request.
// @param  None.
// @retval Result in TCP_StatusTypedef format.
//============================================================================//
TCP_StatusTypeDef GSM_FTP_UploadChk_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    static FTP_UploadChkStepTypeDef procUploadChkLastStep;
    static GSM_NonBlockResp_ResTypeDef filupRes;
    static char cSrcFilePath[130];
    static uint32_t u32FileSize;
    static uint32_t u32UploadLastTick;

    if (procGSM_UploadChk_Step == UPLOAD_CHK_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    if ((procGSM_UploadChk_Step != UPLOAD_CHK_STEP_CLOSE) &&
        (procGSM_UploadChk_Step != UPLOAD_CHK_STEP_CLOSE_WAIT) &&
        (procGSM_UploadChk_Step != UPLOAD_CHK_STEP_FINISH))
    {
        procUploadChkLastStep = procGSM_UploadChk_Step;
    }

    switch ((int)procGSM_UploadChk_Step)
    {
    // UPLOAD_CHK_STEP_INIT: Check required file
    case UPLOAD_CHK_STEP_INIT:
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_BEGIN, STR_NULL);
#endif

        if ((strlen(gsmUploadChkData.cSrcFile) == 0) ||
            (strlen(gsmUploadChkData.cDstDir) == 0) ||
            (strlen(gsmUploadChkData.cDstFile) == 0))
        {
            GSMprintf("\r\n> FTP UPLOAD CHK request parameters incorrect. Terminal operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

            tcpRes = TCP_FTP_OTHER_ERR;

            break;
        }

        memset(cSrcFilePath, 0x00, sizeof(cSrcFilePath));

        if (strlen(gsmUploadChkData.cSrcDir) == 0)
            sprintf(cSrcFilePath, "%s", gsmUploadChkData.cSrcFile);
        else
            sprintf(cSrcFilePath, "%s/%s", gsmUploadChkData.cSrcDir, gsmUploadChkData.cSrcFile);

        if (SD_CheckFileExist(cSrcFilePath) == FALSE)
        {
            GSMprintf("\r\n> FTP UPLOAD CHK source file not found. Terminate operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

            tcpRes = TCP_FTP_NO_SRC_FILE;

            break;
        }

        u32FileSize = SD_GetFileSize(cSrcFilePath);

        if (u32FileSize == 0)
        {
            GSMprintf("\r\n> FTP UPLOAD CHK insufficient source file size. Terminate operation.");

            filupRes = GSMRES_FAIL_NO_REDO;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

            tcpRes = TCP_FTP_NO_SRC_FILE;

            break;
        }

        GSMprintf("\r\n> FTP UPLOAD CHK file name %s, size: %d", cSrcFilePath, u32FileSize);

        if ((gsmUploadChkData.u8ServerID != 1) && (gsmUploadChkData.u8ServerID != 2))
        {
            gsmUploadChkData.u8ServerID = 2;
        }

        u8FTP_RetryCnt = 0;

        filupRes = GSMRES_NULL;

        tcpRes = TCP_OK;

        u32UploadLastTick = 0;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_PDP;

        break;
    }

    // UPLOAD_CHK_STEP_CFG_PDP: Config PDP context for the FTP connection
    case UPLOAD_CHK_STEP_CFG_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_PDP_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK config PDP maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD CHK config PDP ERROR. Retry in the next loop.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_PDP;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_ACC;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_CHK_STEP_CFG_ACC: Config account for the FTP connection
    case UPLOAD_CHK_STEP_CFG_ACC:
    {
        char cUserName[21] = {0};
        char cPassword[21] = {0};

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmUploadChkData.u8ServerID == 1)
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
        }
        else
        {
            sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
            sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
        }
#else
        sprintf(cUserName, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
        sprintf(cPassword, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
#endif

        bRes = GSM_FTP_CfgAcc(cUserName, cPassword);

        if (!bRes)
        {
            tcpRes = TCP_FTP_ACC_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK config account maximum loop retries reached.");

                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD CHK config account ERROR. Retry in the next loop.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_ACC;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_TIMEOUT;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_CHK_STEP_CFG_TIMEOUT: Config timeout for the FTP connection
    case UPLOAD_CHK_STEP_CFG_TIMEOUT:
    {
        HAL_Delay(10);

        bRes = GSM_FTP_CfgTimeout((uint16_t)FTP_FUNCTION_TIMEOUT);

        if (!bRes)
        {
            tcpRes = TCP_FTP_TIMEOUT_CONFIG_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK config timeout maximum loop retries reached.");

                bFTP_SetupFinish = TRUE;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN;
            }
            else
            {
                GSMprintf("\r\n> FTP UPLOAD CHK config timeout ERROR. Retry in the next loop.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CFG_TIMEOUT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        bFTP_SetupFinish = TRUE;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN;

        tcpRes = TCP_OK;

        break;
    }

    // UPLOAD_CHK_STEP_OPEN: Send command to open FTP connection
    case UPLOAD_CHK_STEP_OPEN:
    {
        char cServerAddr[31] = {0};
        uint16_t u16ServerPort = 0;

        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

#if ENABLE_MULTI_FTP_SERVER
        if (gsmUploadChkData.u8ServerID == 1)
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP;
        }
        else
        {
            sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
            u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;

            gsmCurrServerType = CURR_SERVER_FTP_EXT;
        }
#else
        sprintf(cServerAddr, "%s", g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
        u16ServerPort = g_Setting.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;

        gsmCurrServerType = CURR_SERVER_FTP;
#endif

        // Open FTP connection
        bRes = GSM_FTP_Login(cServerAddr, u16ServerPort);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            tcpRes = TCP_FTP_LOGIN_ERR;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                GSMprintf("\r\n> FTP UPLOAD CHK open connection maximum loop retries reached.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK open connection ERROR. Retry in the next loop.");

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN_WAIT;

        break;
    }

    // UPLOAD_CHK_STEP_OPEN_WAIT: Polling FTP open connection result
    case UPLOAD_CHK_STEP_OPEN_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_OPEN, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD CHK open connection wait result TIMEOUT!");

            tcpRes = TCP_FTP_LOGIN_ERR;

            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD CHK open connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGIN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK open connection maximum loop retries reached.");

                // filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK open connection ERROR. Retry in the next loop.");

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_OPEN;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_SET_DIR;

        break;
    }

    // UPLOAD_CHK_STEP_SET_DIR: Send command to set working directory on FTP server
    case UPLOAD_CHK_STEP_SET_DIR:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        bRes = GSM_FTP_SetDir(gsmUploadChkData.cDstDir);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            tcpRes = TCP_FTP_SETDIR_ERR;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                GSMprintf("\r\n> FTP UPLOAD CHK set working directory maximum loop retries reached.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK set working directory ERROR. Retry in the next loop.");

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_SET_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_SET_DIR_WAIT;

        break;
    }

    // UPLOAD_CHK_STEP_SET_DIR_WAIT: Polling FTP set working directory result
    case UPLOAD_CHK_STEP_SET_DIR_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_SET_DIR, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_SET_DIR_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD CHK set working directory wait result TIMEOUT.");

            tcpRes = TCP_FTP_SETDIR_ERR;

            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD CHK set working directory get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_SETDIR_ERR;

            if (u16GSM_ErrCode == 627)
            {
                GSMprintf("\r\n> FTP UPLOAD CHK dir %s not found. Request to reupload file.", gsmUploadChkData.cDstDir);

                GSM_FTP_FileUploadRequest(gsmUploadChkData.cSrcFile,
                                          gsmUploadChkData.cDstFile,
                                          gsmUploadChkData.cSrcDir,
                                          gsmUploadChkData.cDstDir,
                                          gsmUploadChkData.u8ServerID,
                                          0);

                GSM_FTP_SaveUploadChk(&gsmUploadChkData);

                filupRes = GSMRES_SUCCESS;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK set working directory maximum loop retries reached.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK set working directory ERROR. Retry in the next loop.");

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_SET_DIR;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_GET_SIZE;

        break;
    }

    // UPLOAD_CHK_STEP_GET_SIZE: Get size of the file uploaded to the server
    case UPLOAD_CHK_STEP_GET_SIZE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        u32GetFileSize = 0;

        // Get file size
        bRes = GSM_FTP_FileSize(gsmUploadChkData.cDstFile);

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            tcpRes = TCP_FTP_FILESIZE_ERR;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK get file %s size maximum loop retries reached.", gsmUploadRequest.cDstFile);

                filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK get file %s size ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_GET_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_GET_SIZE_WAIT;

        break;
    }

    // UPLOAD_CHK_STEP_GET_SIZE_WAIT: Polling for FTP get file size
    case UPLOAD_CHK_STEP_GET_SIZE_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_FILE_SIZE, &filupRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_GET_SIZE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD CHK get file size wait result TIMEOUT.");

            tcpRes = TCP_FTP_FILESIZE_ERR;

            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

            return tcpRes;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD CHK get file %s size get error code %d", gsmUploadRequest.cDstFile, u16ErrCode);

            tcpRes = TCP_FTP_SETDIR_ERR;

            if (u16GSM_ErrCode == 627)
            {
                GSMprintf("\r\n> FTP UPLOAD CHK file %s not found. Request to reupload file.", gsmUploadRequest.cDstFile);

                GSM_FTP_FileUploadRequest(gsmUploadChkData.cSrcFile,
                                          gsmUploadChkData.cDstFile,
                                          gsmUploadChkData.cSrcDir,
                                          gsmUploadChkData.cDstDir,
                                          gsmUploadChkData.u8ServerID,
                                          0);

                GSM_FTP_SaveUploadChk(&gsmUploadChkData);

                filupRes = GSMRES_SUCCESS;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK get file %s size maximum loop retries reached.", gsmUploadRequest.cDstFile);

                // filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK get file %s size ERROR. Retry in the next loop.", gsmUploadRequest.cDstFile);

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_GET_SIZE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        if (u32GetFileSize < u32FileSize)
        {
            GSMprintf("\r\n> FTP UPLOAD CHK uploaded file %s size is not correct (%d < %d). Request to reupload file.",
                      gsmUploadRequest.cDstFile,
                      u32GetFileSize,
                      u32FileSize);

            GSM_FTP_FileUploadRequest(gsmUploadChkData.cSrcFile,
                                      gsmUploadChkData.cDstFile,
                                      gsmUploadChkData.cSrcDir,
                                      gsmUploadChkData.cDstDir,
                                      gsmUploadChkData.u8ServerID,
                                      0);

            GSM_FTP_SaveUploadChk(&gsmUploadChkData);
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD CHK uploaded file %s is OK.", gsmUploadChkData.cDstFile);
        }

        filupRes = GSMRES_SUCCESS;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

        break;
    }

    // UPLOAD_CHK_STEP_CLOSE: Send command to close FTP connection
    case UPLOAD_CHK_STEP_CLOSE:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Close connection to FTP server
        bRes = GSM_FTP_Logout();

        if (!bRes)
        {
            u8FTP_RetryCnt++;

            tcpRes = TCP_FTP_LOGOUT_ERR;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection maximum loop retries reached.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

                return tcpRes;
            }

            GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection ERROR. Retry in the next loop.");

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32UploadLastTick = HAL_GetTick();

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE_WAIT;

        break;
    }

    // UPLOAD_CHK_STEP_CLOSE_WAIT: Polling FTP close connection result
    case UPLOAD_CHK_STEP_CLOSE_WAIT:
    {
        uint16_t u16ErrCode;
        GSM_NonBlockResp_ResTypeDef filupCloseRes;

        if (HAL_GetTick() < (u32UploadLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_FTP_CLOSE, &filupCloseRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection wait result TIMEOUT.");

            tcpRes = TCP_FTP_LOGOUT_ERR;

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

            return tcpRes;
        }

        if (filupCloseRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection get error code %d", u16ErrCode);

            tcpRes = TCP_FTP_LOGOUT_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection maximum loop retries reached.");

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

                return tcpRes;
            }

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_CLOSE;

            GSMprintf("\r\n> FTP UPLOAD CHK close FTP connection ERROR. Retry in the next loop.");

            return tcpRes;
        }

        tcpRes = TCP_OK;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_FINISH;

        break;
    }

    // UPLOAD_CHK_STEP_FINISH: Finish the operation
    case UPLOAD_CHK_STEP_FINISH:
    {
        bool bUploadRedoLimit = FALSE, bUploadRedoDelay = FALSE, bUploadRedoRst = FALSE;
        // GSM_UploadChkTypeDef gsmUploadChkTemp = {0};
        char cUploadDelayMsg[80] = {0};

        if (bRTOS_En)
        {
            // Acquire UPLOAD_CHK synchronization
            osMutexWait(mutex_upload_chk_user, S_2_mS(5));
        }

        GSM_FTP_UploadChkIncHead();

        if (bRTOS_En)
            osMutexRelease(mutex_upload_chk_user);

        if ((filupRes == GSMRES_SUCCESS) ||
            (filupRes == GSMRES_FAIL_NO_REDO))
        {
            tcpRes = TCP_OK;

            if (filupRes == GSMRES_SUCCESS)
            {
                GSMprintf("\r\n >>> ### FTP UPLOAD CHK OK ###");

                GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_OK, STR_NULL);
            }
            else
            {
                GSMprintf("\r\n ### FTP UPLOAD CHK ERROR NO REDO ###");

                sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d",
                        procUploadChkLastStep,
                        filupRes,
                        u16GSM_ErrCode);

                GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_FAIL, cUploadDelayMsg);

                memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
            }

            procGSM_UploadChk_Step = UPLOAD_CHK_STEP_NULL;

            break;
        }

        switch (filupRes)
        {
        // For the cases where the number of redo is limited
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY;

            break;

        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            filupRes = GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST;

            break;

        default:
            break;
        }

        if (filupRes != GSMRES_SUCCESS)
        {
            sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d",
                    procUploadChkLastStep,
                    filupRes,
                    u16GSM_ErrCode);

            GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_FAIL, cUploadDelayMsg);

            memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
        }

        // Check for number of redo
        switch (filupRes)
        {
            // For these cases the number of redo is limited
        case GSMRES_NULL:
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            bUploadRedoLimit = TRUE;

            break;

        default:
            break;
        }

        // Check for delay
        switch (filupRes)
        {
        // For these cases add a delay before redo
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bUploadRedoDelay = TRUE;

            break;

        default:
            break;
        }

        // Check for reset GSM module
        switch (filupRes)
        {
            // For these case reset the GSM module before redo
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bUploadRedoRst = TRUE;

            break;

        default:
            break;
        }

        if (bUploadRedoLimit == TRUE)
        {
            gsmUploadChkData.u8ReChkCnt++;

            if (gsmUploadChkData.u8ReChkCnt >= 3)
            {
                gsmUploadChkData.u8ReChkCnt = 0;

                GSMprintf("\r\n> FTP UPLOAD CHK maximum redo reached. Terminate operation.");

                // GSM_TCP_ReportEvent(EVENT_FTP_UPLOAD_FAIL, gsmUploadRequest.cSrcFile);

                tcpRes = TCP_OK;

                procGSM_UploadChk_Step = UPLOAD_CHK_STEP_NULL;

                break;
            }

            System_SaveSetting();
        }

        if (bUploadRedoDelay)
        {
            uint32_t u32DelayTime = 0;

            if (filupRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST)
                u32DelayTime = STOP_UPLOAD_VDR_TIMEOUT;
            else
                u32DelayTime = STOP_FTP_ACCESS_TIMEOUT;

            sprintf(cUploadDelayMsg, " after %d seconds", u32DelayTime);

            GSMprintf("\r\n> FTP UPLOAD CHK%s request to redo%s%s.",
                      bUploadRedoRst ? (char *)" reset GSM module and" : (char *)"",
                      bUploadRedoDelay ? cUploadDelayMsg : (char *)"",
                      bUploadRedoLimit ? (char *)"" : (char *)" (Unlimited)");

            if (bUploadRedoDelay)
                FuncSkip_Enable(FUNC_GSM_FTP, u32DelayTime);
        }

        if (bUploadRedoRst)
            // GSM_Reset();
            bGSM_ResetFlag = TRUE;

        GSM_FTP_SaveUploadChk(&gsmUploadChkData);

        /*
        for(int i = 0; i < 3; i++)
        {
          bRes = GSM_FTP_GetUploadChk(&gsmUploadChkTemp);

            if(bRes)
                break;
        }

        if(bRes)
        {
            // If there is no new check request then clear the current check request
          if((strcmp(gsmUploadChkData.cSrcDir, gsmUploadChkTemp.cSrcDir) == 0) &&
                 (strcmp(gsmUploadChkData.cSrcFile, gsmUploadChkTemp.cSrcFile) == 0) &&
               (strcmp(gsmUploadChkData.cDstDir, gsmUploadChkTemp.cDstDir) == 0) &&
               (strcmp(gsmUploadChkData.cDstFile, gsmUploadChkTemp.cDstFile) == 0))
            {
                //if(strlen(gsmUploadChkData.cSrcDir) == 0)
                //  GSMprintf("\r\n> GSM FTP CHK clear check request for file %s", gsmUploadChkTemp.cSrcFile);
                //else
                //	GSMprintf("\r\n> GSM FTP CHK clear check request for file %s/%s",
                //            gsmUploadChkTemp.cSrcDir,
                //            gsmUploadChkTemp.cSrcFile);

                gsmUploadChkData.bRequireChk = FALSE;

                GSM_FTP_SaveUploadChk(&gsmUploadChkData);
            }
        }
        */

        tcpRes = TCP_OK;

        procGSM_UploadChk_Step = UPLOAD_CHK_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}
#endif

//============================================================================//
// @brief  Request to download file from an HTTP link.
// @param  tRequest: Download request.
// @retval TRUE: OK, FALSE: Error.
//============================================================================//
bool GSM_HTTP_DownloadRequest(HTTP_DownloadRequestTypeDef *ptRequest)
{
    if ((tHTTP_DlRequest.bRequest == TRUE) ||
        (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_NULL))
        return TRUE;

    if ((strlen(ptRequest->cSrcPath) == 0) ||
        (strlen(ptRequest->cSrcPath) > sizeof(tHTTP_DlRequest.cSrcPath)) ||
        (strlen(ptRequest->cSrcFileName) == 0) ||
        (strlen(ptRequest->cSrcFileName) > sizeof(tHTTP_DlRequest.cSrcFileName)) ||
        (strlen(ptRequest->cDstDir) > sizeof(tHTTP_DlRequest.cDstDir)) ||
        (strlen(ptRequest->cDstFileName) > sizeof(tHTTP_DlRequest.cDstFileName)))
    {
        GSMprintf("\r\n> GSM HTTP download request has invalid parameter!");

        return FALSE;
    }

    memcpy(&tHTTP_DlRequest, ptRequest, sizeof(HTTP_DownloadRequestTypeDef));

    tHTTP_DlRequest.bRequest = TRUE;

    return TRUE;
}

//============================================================================//
// @brief  Init to download file from an HTTP link.
// @param  None.
// @retval None.
//============================================================================//
void GSM_HTTP_DownloadInit(void)
{
    if (tHTTP_DlRequest.bRequest == TRUE)
    {
        tHTTP_DlRequest.bRequest = FALSE;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_INIT;
    }
}

//============================================================================//
// @brief  Handle HTTP file download process.
// @param  None.
// @retval Result in TCP_StatusTypeDef.
//============================================================================//
TCP_StatusTypeDef GSM_HTTP_Download_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    static HTTP_DownloadStepTypeDef procHTTP_DlLastStep;
    static GSM_NonBlockResp_ResTypeDef dlRes;
    static uint32_t u32HTTP_DlLastTick = 0;
    static uint32_t u32FileSize = 0, u32FileID = 0,
                    u32DatCnt = 0, u32DatSize = 0;
    static char cDstFilePath[128] = {0};
    static uint32_t u32StartTime = 0, u32EndTime = 0;

    if (procHTTP_Download_Step == HTTP_DOWNLOAD_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    if ((procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_STOP) &&
        (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_FINISH) &&
        (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_RAM_CLOSE))
    {
        procHTTP_DlLastStep = procHTTP_Download_Step;
    }

    switch ((int)procHTTP_Download_Step)
    {
    // HTTP_DOWNLOAD STEP INIT: Initializing the HTTP file download process
    case HTTP_DOWNLOAD_STEP_INIT:
    {
        if ((strlen(tHTTP_DlRequest.cSrcPath) == 0) ||
            (strlen(tHTTP_DlRequest.cSrcPath) > sizeof(tHTTP_DlRequest.cSrcPath)) ||
            (strlen(tHTTP_DlRequest.cSrcFileName) == 0) ||
            (strlen(tHTTP_DlRequest.cSrcFileName) > sizeof(tHTTP_DlRequest.cSrcFileName)) ||
            (strlen(tHTTP_DlRequest.cDstDir) > sizeof(tHTTP_DlRequest.cDstDir)) ||
            (strlen(tHTTP_DlRequest.cDstFileName) > sizeof(tHTTP_DlRequest.cDstFileName)))
        {
            GSMprintf("\r\n> GSM HTTP DOWNLOAD request parameters incorrect. Terminate operation.");

            dlRes = GSMRES_FAIL_NO_REDO;

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

            tcpRes = TCP_FTP_OTHER_ERR;

            break;
        }

        u32StartTime = HAL_GetTick();

        GSMprintf("\r\n> GSM HTTP DOWNLOAD file name: %s", tHTTP_DlRequest.cSrcFileName);

        u8FTP_RetryCnt = 0;

        dlRes = GSMRES_NULL;

        tcpRes = TCP_OK;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_CFG_PDP;

        break;
    }

    // HTTP_DOWNLOAD_STEP_HTTP_CFG_PDP: Config HTTP PDP ID
    case HTTP_DOWNLOAD_STEP_CFG_PDP:
    {
        HAL_Delay(10);

        bRes = GSM_HTTP_CfgPDP(u8CurrPDP_ID);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_CFG_PDP_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD config PDP maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD config PDP ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_CFG_PDP;
            }

            return tcpRes;
        }

        u8HTTP_RetryCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_CFG_RESP;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_CFG_RESP: Config HTTP response header
    case HTTP_DOWNLOAD_STEP_CFG_RESP:
    {
        HAL_Delay(10);

        bRes = GSM_HTTP_RespHeader(0);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_CFG_RESP_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD config response maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD config response ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_CFG_RESP;
            }

            return tcpRes;
        }

        u8HTTP_RetryCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SET_URL;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_SET_URL: Set HTTP URL
    case HTTP_DOWNLOAD_STEP_SET_URL:
    {
        HAL_Delay(10);

        bRes = GSM_HTTP_SetURL(tHTTP_DlRequest.cSrcPath);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_URL_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD set URL maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD set URL ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SET_URL;
            }

            return tcpRes;
        }

        u8HTTP_RetryCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_GET: Send GET request to HTTP server
    case HTTP_DOWNLOAD_STEP_GET:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        // Send GET request
        bRes = GSM_HTTP_Get();

        if (!bRes)
        {
            tcpRes = TCP_HTTP_GET_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request maximum loop retries reached.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }

            GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request ERROR. Retry in the next loop.");

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32HTTP_DlLastTick = HAL_GetTick();

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_WAIT;

        break;
    }

    // HTTP_DOWNLOAD_STEP_GET_WAIT: Polling for HTTP GET request result
    case HTTP_DOWNLOAD_STEP_GET_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32HTTP_DlLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_HTTP_GET, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request wait result TIMEOUT.");

            tcpRes = TCP_HTTP_GET_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT;

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request get error code %d", u16ErrCode);

            tcpRes = TCP_HTTP_GET_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request ERROR. Terminate operation.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8HTTP_RetryCnt++;

                if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8HTTP_RetryCnt = 0;

                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET requests maximum loop retries reached.");

                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                    return tcpRes;
                }

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request ERROR. Retry in the next loop after %d seconds.", HTTP_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, HTTP_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send GET request ERROR.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }
        }

        /*
        if(tGET_Resp.u32ContentLen == 0)
        {
          GSMprintf("\r\n> HTTP DOWNLOAD file size is 0. Terminate the operation.");

            dlRes = GSMRES_FAIL_NO_REDO;

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

            tcpRes = TCP_OK;

            break;
        }

        GSMprintf("\r\n> HTTP DOWNLOAD file size: %d", tGET_Resp.u32ContentLen);
        */

        u32FileSize = tGET_Resp.u32ContentLen;

        tcpRes = TCP_OK;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_READ;

        break;
    }

    // HTTP_DOWNLOAD_STEP_READ: Send READ request to HTTP server
    case HTTP_DOWNLOAD_STEP_READ:
    {
        if (GSM_IsOpenConnectTCP() == TRUE)
            return TCP_OK;

        HAL_Delay(10);

        GSM_FileDelete(tHTTP_DlRequest.cSrcFileName);

        // Send READ request
        bRes = GSM_HTTP_ReadFile(tHTTP_DlRequest.cSrcFileName);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_READ_FILE_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request maximum loop retries reached.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }

            GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request ERROR. Retry in the next loop.");

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_READ;

            return tcpRes;
        }

        tcpRes = TCP_OK;

        u32HTTP_DlLastTick = HAL_GetTick();

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_READ_WAIT;

        break;
    }

    // HTTP_DOWNLOAD_STEP_READ_WAIT: Polling for HTTP READ request result
    case HTTP_DOWNLOAD_STEP_READ_WAIT:
    {
        uint16_t u16ErrCode;

        if (HAL_GetTick() < (u32HTTP_DlLastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
        {
            bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_HTTP_READ_FILE, &dlRes, &u16ErrCode);

            if (!bRes)
            {
                tcpRes = TCP_OK;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_READ_WAIT;

                break;
            }
        }
        else
        {
            GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request wait result TIMEOUT.");

            tcpRes = TCP_HTTP_READ_FILE_ERR;

            dlRes = GSMRES_FAIL_REDO_LIMIT;

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

            return tcpRes;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            if (u16ErrCode != 0)
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request get error code %d", u16ErrCode);

            tcpRes = TCP_HTTP_READ_FILE_ERR;

            // If fail and no need to retry
            if (dlRes == GSMRES_FAIL_NO_REDO)
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request ERROR. Terminate operation.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }
            // If no need to reset the GSM module
            if ((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
                (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
            {
                u8HTTP_RetryCnt++;

                if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    u8HTTP_RetryCnt = 0;

                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ requests maximum loop retries reached.");

                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                    return tcpRes;
                }

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET;

                // If need to delay before retry
                if ((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
                    (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request ERROR. Retry in the next loop after %d seconds.", HTTP_DOWNLOAD_STOP_TIMEOUT);

                    FuncSkip_Enable(FUNC_GSM_FTP, HTTP_DOWNLOAD_STOP_TIMEOUT);
                }
                else
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request ERROR. Retry in the next loop.");

                return tcpRes;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD send READ request ERROR.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

                return tcpRes;
            }
        }

        tcpRes = TCP_OK;

        u32EndTime = HAL_GetTick();

        if (strlen(tHTTP_DlRequest.cDstFileName) == 0)
        {
            memset(cDstFilePath, 0x00, sizeof(cDstFilePath));

            sprintf(cDstFilePath, "%s", tHTTP_DlRequest.cSrcFileName);

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
        }
        else
            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_SIZE;

        break;
    }

    // HTTP_DOWNLOAD_STEP_STOP: Cancel HTTP request
    case HTTP_DOWNLOAD_STEP_STOP:
    {
        HAL_Delay(10);

        bRes = GSM_HTTP_Cancel();

        if (!bRes)
        {
            tcpRes = TCP_HTTP_STOP_ERR;

            u8HTTP_RetryCnt++;

            if (u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8HTTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD stop maximum loop retries reached.");

                if (dlRes != GSMRES_SUCCESS)
                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
                else
                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_OPEN;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD stop ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_STOP;
            }

            return tcpRes;
        }

        u8HTTP_RetryCnt = 0;

        if (dlRes != GSMRES_SUCCESS)
            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
        else
            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_OPEN;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_GET_SIZE: Get size of the downloaded file
    case HTTP_DOWNLOAD_STEP_GET_SIZE:
    {
        char cPattern[32] = {0};
        uint8_t u8RespBuf[128] = {0};
        uint32_t u32FileSizeTmp = 0;
        uint16_t u16RetLen = 0;
        char *pcPtr1;
        uint8_t u8Num = 0;

        HAL_Delay(10);

        sprintf(cPattern, "%s", tHTTP_DlRequest.cSrcFileName);

        bRes = GSM_FileList(cPattern, u8RespBuf, &u16RetLen);

        if (bRes == FALSE)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_SIZE;
            }

            return tcpRes;
        }

        // In case the storage is UFS, the 4G module might not include storage name in the response,
        // so remove "UFS:" from the pattern
        if (strncmp(cPattern, (char *)FTP_LOCAL_STORAGE, strlen((char *)FTP_LOCAL_STORAGE)) == 0)
        {
            pcPtr1 = cPattern + strlen((char *)FTP_LOCAL_STORAGE);

            sprintf(cPattern, pcPtr1, strlen(pcPtr1));
        }

        pcPtr1 = strstr((char *)u8RespBuf, cPattern);

        if (pcPtr1 == NULL)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size response ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_SIZE;
            }

            return tcpRes;
        }

        u8Num = sscanf(pcPtr1, "%[^,],%d", cPattern, &u32FileSizeTmp);

        if ((u8Num != 2) || (u32FileSizeTmp == 0) || (u32FileSizeTmp > GPS_EPO_SIZE_MAX))
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD get file size ERROR (%d-%d). Retry in the next loop.", u8Num, u32FileSizeTmp);

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_GET_SIZE;
            }

            return tcpRes;
        }

        u32FileSize = u32FileSizeTmp;

        GSMprintf("\r\n> GSM HTTP DOWNLOAD file size: %d", u32FileSize);

        u8FTP_RetryCnt = 0;

        // procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;
        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_OPEN;

        break;
    }

    // HTTP_DOWNLOAD_STEP_RAM_OPEN: Open downloaded file in device RAM
    case HTTP_DOWNLOAD_STEP_RAM_OPEN:
    {
        HAL_Delay(10);

        bRes = GSM_FileOpen(tHTTP_DlRequest.cSrcFileName, &u32FileID, 2);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILEOPEN_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD open temp file maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD open temp file ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_OPEN;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT: Initialize the process to transfer the file to SD card
    case HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT:
    {
        memset(cDstFilePath, 0x00, sizeof(cDstFilePath));

        if (strlen(tHTTP_DlRequest.cDstDir) == 0)
        {
            sprintf(cDstFilePath, "%s", tHTTP_DlRequest.cDstFileName);

            if (SD_CheckFileExist(cDstFilePath) == TRUE)
            {
                bRes = SD_RemoveFile(cDstFilePath);

                if (bRes)
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD remove file %s OK.", cDstFilePath);
                }
                else
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD remove file %s ERROR.", cDstFilePath);
                }
            }
            else
            {
                bRes = TRUE;
            }
        }
        else
        {
            sprintf(cDstFilePath, "%s/%s", tHTTP_DlRequest.cDstDir, tHTTP_DlRequest.cDstFileName);

            if (SD_CheckDirExist(tHTTP_DlRequest.cDstDir) == TRUE)
            {
                if (SD_CheckFileExist(cDstFilePath) == TRUE)
                {
                    bRes = SD_RemoveFile(cDstFilePath);

                    if (bRes)
                    {
                        GSMprintf("\r\n> GSM HTTP DOWNLOAD remove file %s OK.", cDstFilePath);
                    }
                    else
                    {
                        GSMprintf("\r\n> GSM HTTP DOWNLOAD remove file %s ERROR.", cDstFilePath);
                    }
                }
                else
                {
                    bRes = TRUE;
                }
            }
            else
            {
                bRes = SD_MakeDir(tHTTP_DlRequest.cDstDir);
            }
        }

        if (!bRes)
        {
            tcpRes = TCP_FTP_DF2FILEDEL_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD init save file maximum loop retries reached.");

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD init save file ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u32DatCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_SAVE_FILE: Read data from the device RAM and write to the Exflash and calculate CRC
    case HTTP_DOWNLOAD_STEP_SAVE_FILE:
    {
        const uint16_t u16DownloadSize = 1024;
        uint8_t u8DatBuf[1024];
        uint32_t u32ActualReadLen;

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > u16DownloadSize)
            u32DatSize = u16DownloadSize;

        memset(u8DatBuf, 0x00, sizeof(u8DatBuf));

        bRes = GSM_FileRead(u32FileID, u32DatSize, &u32ActualReadLen, (char *)u8DatBuf);

        // GSMprintf("\r\n> GSM HTTP DOWNLOAD read %d bytes from temp file in device RAM.", u32ActualReadLen);

        if ((!bRes) || (bRes && (u32ActualReadLen != u32DatSize)))
        {
            tcpRes = TCP_FTP_FILEREAD_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt == 3)
            {
                u8FTP_RetryCnt = 0;

                u8FileTransRetryCnt++;

                if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD read data maximum loop retries reached.");

                    u8FileTransRetryCnt = 0;

                    dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_CLOSE;
                }
                else
                {
                    GSMprintf("\r\n> GSM HTTP DOWNLOAD read data ERROR. Retry from the beginning of the file in the next loop.");

                    procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE_INIT;
                }
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD read data at address %d ERROR. Retry in the next loop.", u32DatCnt);

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        bRes = SD_WriteBuffToAddr(cDstFilePath, u8DatBuf, u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            tcpRes = TCP_FTP_SAVEFILE_ERR;

            u8FileTransRetryCnt++;

            if (u8FileTransRetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD write to destination file maximum loop retries reached.");

                u8FileTransRetryCnt = 0;

                dlRes = GSMRES_FAIL_REDO_LIMIT_RST;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_CLOSE;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD write to destination file at address %d ERROR. Retry in the next loop.", u32DatCnt);

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        u8FileTransRetryCnt = 0;

        u32DatCnt += u32DatSize;

        // GSMprintf("\r\n> GSM HTTP DOWNLOAD finish write %d bytes to destination file.");

        if ((u32DatCnt % (sizeof(u8DatBuf) * 10)) == 0)
            GSMprintf("\r\n\r\n> GSM HTTP DOWNLOAD bytes transferred: %d\r\n", u32DatCnt);

        if (u32DatCnt >= u32FileSize)
        {
            GSMprintf("\r\n> GSM HTTP DOWNLOAD finish transfer data from device RAM to destination file.");

            dlRes = GSMRES_SUCCESS;

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_CLOSE;
        }
        else
            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_SAVE_FILE;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_RAM_CLOSE: Close the temp file in device RAM
    case HTTP_DOWNLOAD_STEP_RAM_CLOSE:
    {
        HAL_Delay(10);

        bRes = GSM_FileClose(u32FileID);

        if (!bRes)
        {
            tcpRes = TCP_FTP_FILECLOSE_ERR;

            u8FTP_RetryCnt++;

            if (u8FTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8FTP_RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP DOWNLOAD close temp file maximum loop retries reached.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD close temp file ERROR. Retry in the next loop.");

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_RAM_CLOSE;
            }

            return tcpRes;
        }

        u8FTP_RetryCnt = 0;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_FINISH;

        tcpRes = TCP_OK;

        break;
    }

    // HTTP_DOWNLOAD_STEP_FINISH: Complete the operation
    case HTTP_DOWNLOAD_STEP_FINISH:
    {
        bool bHTTP_RedoLimit = FALSE, bHTTP_RedoDelay = FALSE, bHTTP_RedoRst = FALSE;
        char cHTTP_Msg[80] = {0};

        if ((dlRes == GSMRES_SUCCESS) ||
            (dlRes == GSMRES_FAIL_NO_REDO))
        {
            tcpRes = TCP_OK;

            if (dlRes == GSMRES_SUCCESS)
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD %s OK", tHTTP_DlRequest.cSrcFileName);

                GSMprintf("\r\n> GSM HTTP DOWNLOAD time: %d", u32EndTime - u32StartTime);

                GSM_TCP_ReportEvent((char *)EVENT_FTP_DOWNLOAD_OK, STR_NULL);

                if (tHTTP_DlRequest.callback != NULL)
                {
                    tHTTP_DlRequest.callback(cDstFilePath, u32FileSize);
                }
            }
            else
            {
                GSMprintf("\r\n> GSM HTTP DOWNLOAD ERROR NO REDO");

                sprintf(cHTTP_Msg, "S:%d-E:%d-C:%d",
                        procHTTP_DlLastStep,
                        dlRes,
                        u16GSM_ErrCode);

                GSM_TCP_ReportEvent((char *)EVENT_FTP_DOWNLOAD_FAIL, cHTTP_Msg);

                memset(cHTTP_Msg, 0x00, sizeof(cHTTP_Msg));
            }

            procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

            break;
        }

        switch (dlRes)
        {
        // For the cases where the number of redo is unlimited
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
            dlRes = GSMRES_FAIL_REDO_LIMIT_DELAY;

            break;

        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            dlRes = GSMRES_FAIL_REDO_LIMIT_DELAY_RST;

            break;

        default:
            break;
        }

        if (dlRes != GSMRES_SUCCESS)
        {
            sprintf(cHTTP_Msg, "S:%d-E:%d-C:%d",
                    procHTTP_DlLastStep,
                    dlRes,
                    u16GSM_ErrCode);

            GSM_TCP_ReportEvent((char *)EVENT_FTP_DOWNLOAD_FAIL, cHTTP_Msg);

            memset(cHTTP_Msg, 0x00, sizeof(cHTTP_Msg));
        }

        // Check for number of redo
        switch (dlRes)
        {
            // For these cases the number of redo is limited
        case GSMRES_NULL:
        case GSMRES_FAIL_REDO_LIMIT:
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
            bHTTP_RedoLimit = TRUE;

            break;

        default:
            break;
        }

        // Check for delay
        switch (dlRes)
        {
        // For these cases add a delay before redo
        case GSMRES_FAIL_REDO_LIMIT_DELAY:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bHTTP_RedoDelay = TRUE;

            break;

        default:
            break;
        }

        // Check for reset GSM module
        switch (dlRes)
        {
            // For these case reset the GSM module before redo
        case GSMRES_FAIL_REDO_LIMIT_RST:
        case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
        case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
            bHTTP_RedoRst = TRUE;

            break;

        default:
            break;
        }

        if (bHTTP_RedoLimit == TRUE)
        {
            tHTTP_DlRequest.u8RetryCnt++;

            if (tHTTP_DlRequest.u8RetryCnt >= 3)
            {
                tHTTP_DlRequest.u8RetryCnt = 0;

                GSMprintf("\r\n> GSM HTTP maximum redo reached. Terminate operation.");

                tcpRes = TCP_OK;

                procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

                break;
            }

            System_SaveSetting();
        }

        if (bHTTP_RedoDelay)
        {
            uint32_t u32DelayTime = 0;

            if (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST)
                u32DelayTime = STOP_UPLOAD_VDR_TIMEOUT;
            else
                u32DelayTime = STOP_FTP_ACCESS_TIMEOUT;

            sprintf(cHTTP_Msg, " after %d seconds", u32DelayTime);

            GSMprintf("\r\n> GSM HTTP DOWNLOAD%s request to redo%s%s.",
                      bHTTP_RedoRst ? (char *)" reset GSM module and" : (char *)"",
                      bHTTP_RedoDelay ? cHTTP_Msg : (char *)"",
                      bHTTP_RedoLimit ? (char *)"" : (char *)" (Unlimited)");

            if (bHTTP_RedoDelay)
                FuncSkip_Enable(FUNC_GSM_FTP, u32DelayTime);
        }

        if (bHTTP_RedoRst)
            // GSM_Reset();
            bGSM_ResetFlag = TRUE;

        tHTTP_DlRequest.bRequest = TRUE;

        tcpRes = TCP_OK;

        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
#if 0
//============================================================================//
// @brief  Handle XTRA file download process.
// @param  None.
// @retval Result in TCP_StatusTypeDef.
//============================================================================//
TCP_StatusTypeDef GSM_XTRA_Download_Proc(void)
{
	TCP_StatusTypeDef tcpRes;
	bool bRes;
	static XTRA_DownloadStepTypeDef procXTRA_LastStep;
	static GSM_NonBlockResp_ResTypeDef dlRes;
	static uint32_t u32XTRA_LastTick = 0;


	if(procXTRA_Download_Step == XTRA_STEP_NULL)
		return TCP_OK;
		
	if((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
		return TCP_OK;
	
	if((procXTRA_Download_Step != XTRA_STEP_HTTP_STOP) &&
		 (procXTRA_Download_Step != XTRA_STEP_FINISH))
	{
		procXTRA_LastStep = procXTRA_Download_Step;
	}

	switch((int)procXTRA_Download_Step)
	{
		// XTRA_STEP_HTTP_CFG_PDP: Config HTTP PDP ID
		case XTRA_STEP_HTTP_CFG_PDP:
		{
		  HAL_Delay(10);

			bRes = GSM_HTTP_CfgPDP(u8CurrPDP_ID);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_CFG_PDP_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA HTTP config PDP maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA HTTP config PDP ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_HTTP_CFG_PDP;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			procXTRA_Download_Step = XTRA_STEP_HTTP_CFG_RESP;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_HTTP_CFG_RESP: Config HTTP response header
		case XTRA_STEP_HTTP_CFG_RESP:
		{
		  HAL_Delay(10);

			bRes = GSM_HTTP_RespHeader(0);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_CFG_RESP_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA HTTP config response maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA HTTP config response ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_HTTP_CFG_RESP;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			procXTRA_Download_Step = XTRA_STEP_HTTP_URL;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_HTTP_URL: Set HTTP URL
		case XTRA_STEP_HTTP_URL:
		{
		  HAL_Delay(10);
			
			if(strlen(g_Setting.cXTRA_URL) == 0)
			{
			  System_RestoreBackupSetting(VAR_XTRA_URL);
				
				if(strlen(g_Setting.cXTRA_URL) == 0)
				{
				  sprintf(g_Setting.cXTRA_URL, "%s", GPS_XTRA_FILE_URL);
					
					System_SaveSetting();
				}
			}

			bRes = GSM_HTTP_SetURL(g_Setting.cXTRA_URL);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_URL_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA HTTP set URL maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA HTTP set URL ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_HTTP_URL;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			procXTRA_Download_Step = XTRA_STEP_HTTP_GET;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_HTTP_GET: Send GET request to HTTP server
		case XTRA_STEP_HTTP_GET:
		{
			if(GSM_IsOpenConnectTCP() == TRUE)
				return TCP_OK;
				
			HAL_Delay(10);

			// Send GET request
			bRes = GSM_HTTP_Get();

			if(!bRes)
			{
				tcpRes = TCP_HTTP_GET_ERR;
				
				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					GSMprintf("\r\n> XTRA HTTP send GET request maximum loop retries reached.");

					procXTRA_Download_Step = XTRA_STEP_FINISH;

					return tcpRes;
				}
				
				GSMprintf("\r\n> XTRA HTTP send GET request ERROR. Retry in the next loop.");

				procXTRA_Download_Step = XTRA_STEP_HTTP_GET;

				return tcpRes;
			}

			tcpRes = TCP_OK;
			
			u32XTRA_LastTick = HAL_GetTick();

			procXTRA_Download_Step = XTRA_STEP_HTTP_GET_WAIT;

			break;
		}

		// XTRA_STEP_HTTP_GET_WAIT: Polling for HTTP GET request result
		case XTRA_STEP_HTTP_GET_WAIT:
		{
			uint16_t u16ErrCode;
			
			
			if(HAL_GetTick() < (u32XTRA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
			{
			  bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_HTTP_GET, &dlRes, &u16ErrCode);
				
				if(!bRes)
				{
				  tcpRes = TCP_OK;

					procXTRA_Download_Step = XTRA_STEP_HTTP_GET_WAIT;

					break;
				}
			}
			else
			{
				GSMprintf("\r\n> XTRA HTTP send GET request wait result TIMEOUT.");

				tcpRes = TCP_HTTP_GET_ERR;
				
				dlRes = GSMRES_FAIL_REDO_LIMIT;
				
				procXTRA_Download_Step = XTRA_STEP_FINISH;
				
				return tcpRes;
			}
			
			if(dlRes != GSMRES_SUCCESS)
			{
				if(u16ErrCode != 0)
					GSMprintf("\r\n> XTRA HTTP send GET request get error code %d", u16ErrCode);
				
				tcpRes = TCP_HTTP_GET_ERR;
				
				// If fail and no need to retry
				if(dlRes == GSMRES_FAIL_NO_REDO)
				{
					GSMprintf("\r\n> XTRA HTTP send GET request ERROR. Terminate operation.");
					
				  procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					return tcpRes;
				}
				// If no need to reset the GSM module
			  if((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
					 (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
				   (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
			  {
				  u8HTTP_RetryCnt++;
					
					if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
					{
						u8HTTP_RetryCnt = 0;
						
						GSMprintf("\r\n> XTRA HTTP send GET requests maximum loop retries reached.");
						
						procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					  return tcpRes;
					}
				
					procXTRA_Download_Step = XTRA_STEP_HTTP_GET;
					
					// If need to delay before retry
					if((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
				     (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
					{
						GSMprintf("\r\n> XTRA HTTP send GET request ERROR. Retry in the next loop after %d seconds.", XTRA_DOWNLOAD_STOP_TIMEOUT);
						
					  FuncSkip_Enable(FUNC_GSM_FTP, XTRA_DOWNLOAD_STOP_TIMEOUT);
					}
					else
						GSMprintf("\r\n> XTRA HTTP send GET request ERROR. Retry in the next loop.");

					return tcpRes;
				}
				else
				{
					GSMprintf("\r\n> XTRA HTTP send GET request ERROR.");
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					return tcpRes;
				}
			}
			
			if(tGET_Resp.u32ContentLen == 0)
			{
			  GSMprintf("\r\n> XTRA file size is 0. Terminate the operation.");
				
				dlRes = GSMRES_FAIL_NO_REDO;
				
				procXTRA_Download_Step = XTRA_STEP_FINISH;
				
				tcpRes = TCP_OK;
				
				break;
			}
			
			GSMprintf("\r\n> XTRA file size: %d", tGET_Resp.u32ContentLen);
			
			tcpRes = TCP_OK;

			procXTRA_Download_Step = XTRA_STEP_HTTP_READ;

			break;
		}
		
		// XTRA_STEP_HTTP_READ: Send READ request to HTTP server
		case XTRA_STEP_HTTP_READ:
		{
			if(GSM_IsOpenConnectTCP() == TRUE)
				return TCP_OK;
				
			HAL_Delay(10);
			
			GSM_FileDelete((char *)GPS_XTRA_FILE_NAME);

			// Send READ request
			bRes = GSM_HTTP_ReadFile((char *)GPS_XTRA_FILE_NAME);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_READ_FILE_ERR;
				
				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					GSMprintf("\r\n> XTRA HTTP send READ request maximum loop retries reached.");

					procXTRA_Download_Step = XTRA_STEP_FINISH;

					return tcpRes;
				}
				
				GSMprintf("\r\n> XTRA HTTP send READ request ERROR. Retry in the next loop.");

				procXTRA_Download_Step = XTRA_STEP_HTTP_READ;

				return tcpRes;
			}

			tcpRes = TCP_OK;
			
			u32XTRA_LastTick = HAL_GetTick();

			procXTRA_Download_Step = XTRA_STEP_HTTP_READ_WAIT;

			break;
		}

		// XTRA_STEP_HTTP_READ_WAIT: Polling for HTTP READ request result
		case XTRA_STEP_HTTP_READ_WAIT:
		{
			uint16_t u16ErrCode;
			
			
			if(HAL_GetTick() < (u32XTRA_LastTick + S_2_mS(FTP_FUNCTION_TIMEOUT + 10)))
			{
			  bRes = GSM_NonBlockCmd_CheckResOnce(QTEC21_HTTP_READ_FILE, &dlRes, &u16ErrCode);
				
				if(!bRes)
				{
				  tcpRes = TCP_OK;

					procXTRA_Download_Step = XTRA_STEP_HTTP_READ_WAIT;

					break;
				}
			}
			else
			{
				GSMprintf("\r\n> XTRA HTTP send READ request wait result TIMEOUT.");

				tcpRes = TCP_HTTP_READ_FILE_ERR;
				
				dlRes = GSMRES_FAIL_REDO_LIMIT;
				
				procXTRA_Download_Step = XTRA_STEP_FINISH;
				
				return tcpRes;
			}
			
			if(dlRes != GSMRES_SUCCESS)
			{
				if(u16ErrCode != 0)
					GSMprintf("\r\n> XTRA HTTP send READ request get error code %d", u16ErrCode);
				
				tcpRes = TCP_HTTP_READ_FILE_ERR;
				
				// If fail and no need to retry
				if(dlRes == GSMRES_FAIL_NO_REDO)
				{
					GSMprintf("\r\n> XTRA HTTP send READ request ERROR. Terminate operation.");
					
				  procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					return tcpRes;
				}
				// If no need to reset the GSM module
			  if((dlRes == GSMRES_FAIL_REDO_LIMIT) ||
					 (dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
				   (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
			  {
				  u8HTTP_RetryCnt++;
					
					if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
					{
						u8HTTP_RetryCnt = 0;
						
						GSMprintf("\r\n> XTRA HTTP send READ requests maximum loop retries reached.");
						
						procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					  return tcpRes;
					}
				
					procXTRA_Download_Step = XTRA_STEP_HTTP_GET;
					
					// If need to delay before retry
					if((dlRes == GSMRES_FAIL_REDO_LIMIT_DELAY) ||
				     (dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY))
					{
						GSMprintf("\r\n> XTRA HTTP send READ request ERROR. Retry in the next loop after %d seconds.", XTRA_DOWNLOAD_STOP_TIMEOUT);
						
					  FuncSkip_Enable(FUNC_GSM_FTP, XTRA_DOWNLOAD_STOP_TIMEOUT);
					}
					else
						GSMprintf("\r\n> XTRA HTTP send READ request ERROR. Retry in the next loop.");

					return tcpRes;
				}
				else
				{
					GSMprintf("\r\n> XTRA HTTP send READ request ERROR.");
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
					
					return tcpRes;
				}
			}
			
			tcpRes = TCP_OK;

			procXTRA_Download_Step = XTRA_STEP_SET_TIME;

			break;
		}
		
		// XTRA_STEP_HTTP_STOP: Cancel HTTP request
		case XTRA_STEP_HTTP_STOP:
		{
		  HAL_Delay(10);

			bRes = GSM_HTTP_Cancel();

			if(!bRes)
			{
				tcpRes = TCP_HTTP_STOP_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA HTTP stop maximum loop retries reached.");
					
					if(dlRes != GSMRES_SUCCESS)
						procXTRA_Download_Step = XTRA_STEP_FINISH;
					else
						procXTRA_Download_Step = XTRA_STEP_SET_TIME;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA HTTP stop ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_HTTP_STOP;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			if(dlRes != GSMRES_SUCCESS)
				procXTRA_Download_Step = XTRA_STEP_FINISH;
			else
				procXTRA_Download_Step = XTRA_STEP_SET_TIME;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_SET_TIME: Inject GPS time
		case XTRA_STEP_SET_TIME:
		{
			char cTimeStamp[32] = {0};
			time_t ttCurrTime;
			struct tm tmTime = {0};

	
			ttCurrTime = System_GetTime() - g_Setting.i32TimeZoneOffset;
			
			localtime_r(&ttCurrTime, &tmTime);

			strftime(cTimeStamp, sizeof(cTimeStamp), "%Y/%m/%d,%H:%M:%S", &tmTime);
			
			if(bGPS_Ready)
			{
				HAL_Delay(10);
				
				GPS_Finish();
				
				HAL_Delay(100);
			}

			bRes = GPS_XTRA_Time(cTimeStamp);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_OTHER_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA set time maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA set time ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_SET_TIME;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			procXTRA_Download_Step = XTRA_STEP_ACT_DATA;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_ACT_DATA: Activate XTRA data file
		case XTRA_STEP_ACT_DATA:
		{
			HAL_Delay(10);

			bRes = GPS_XTRA_Data((char *)GPS_XTRA_FILE_NAME);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_OTHER_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA data maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA data ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_ACT_DATA;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;

			procXTRA_Download_Step = XTRA_STEP_VERIFY;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_VERIFY: Verify XTRA file is OK 
		case XTRA_STEP_VERIFY:
		{
			uint16_t u16DurTime;
			time_t ttValidTime;
			
			
			HAL_Delay(10);

			bRes = GPS_XTRA_DataQuery(&u16DurTime, &ttValidTime);

			if(!bRes)
			{
				tcpRes = TCP_HTTP_OTHER_ERR;

				u8HTTP_RetryCnt++;

				if(u8HTTP_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
				{
					u8HTTP_RetryCnt = 0;
					
					GSMprintf("\r\n> GSM XTRA data maximum loop retries reached.");
					
				  dlRes = GSMRES_FAIL_REDO_LIMIT;
					
					procXTRA_Download_Step = XTRA_STEP_FINISH;
				}
				else
				{
					GSMprintf("\r\n> GSM XTRA data ERROR. Retry in the next loop.");
					
					procXTRA_Download_Step = XTRA_STEP_VERIFY;
				}

				return tcpRes;
			}

			u8HTTP_RetryCnt = 0;
			
	    if(u16DurTime == 0)
			{
				dlRes = GSMRES_FAIL_NO_REDO;
					
				procXTRA_Download_Step = XTRA_STEP_FINISH;
			}
			
			procXTRA_Download_Step = XTRA_STEP_FINISH;

			tcpRes = TCP_OK;

			break;
		}
		
		// XTRA_STEP_FINISH: Complete the operation 
		case XTRA_STEP_FINISH:
		{
			bool bXTRA_RedoLimit = FALSE, bXTRA_RedoDelay = FALSE, bXTRA_RedoRst = FALSE;
			char cUploadDelayMsg[80] = {0};
			
			
			if(bGPS_Ready == FALSE)
				GPS_TurnOn();
			
			if((dlRes == GSMRES_SUCCESS) || 
				 (dlRes == GSMRES_FAIL_NO_REDO))
			{
			  tcpRes = TCP_OK;
				
				if(dlRes == GSMRES_SUCCESS)
				{
				  GSMprintf("\r\n >>> ### GPS XTRA OK ###");
					
					//GSM_TCP_ReportEvent((char *)EVENT_UPLOAD_CHK_OK, STR_NULL);
				}
				else
				{
					GSMprintf("\r\n ### GPS XTRA ERROR NO REDO ###");
					
					sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d", 
									procXTRA_LastStep, 
									dlRes, 
									u16GSM_ErrCode);
				
					GSM_TCP_ReportEvent((char *)EVENT_GPS_XTRA_FAIL, cUploadDelayMsg);
					
					memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
				}

				procXTRA_Download_Step = XTRA_STEP_NULL;
				
				break;
			}
			
			switch(dlRes)
			{
				// For the cases where the number of redo is unlimited
				case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
					dlRes = GSMRES_FAIL_REDO_LIMIT_DELAY;
				
					break;
				
				case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
					dlRes = GSMRES_FAIL_REDO_LIMIT_DELAY_RST;
				
					break;
					
				default:
					break;
			}
			
			if(dlRes != GSMRES_SUCCESS)
			{
				sprintf(cUploadDelayMsg, "S:%d-E:%d-C:%d", 
				        procXTRA_LastStep, 
				        dlRes, 
				        u16GSM_ErrCode);
				
			  GSM_TCP_ReportEvent((char *)EVENT_GPS_XTRA_FAIL, cUploadDelayMsg);
				
				memset(cUploadDelayMsg, 0x00, sizeof(cUploadDelayMsg));
			}
			
			// Check for number of redo
			switch(dlRes)
			{
				// For these cases the number of redo is limited
			  case GSMRES_NULL:
				case GSMRES_FAIL_REDO_LIMIT:
				case GSMRES_FAIL_REDO_LIMIT_RST:
				case GSMRES_FAIL_REDO_LIMIT_DELAY:
				case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
				  bXTRA_RedoLimit = TRUE;	
				
				  break;
					
				default:
					break;
			}
			
			// Check for delay
			switch(dlRes)
			{
				// For these cases add a delay before redo
				case GSMRES_FAIL_REDO_LIMIT_DELAY:
				case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
				case GSMRES_FAIL_REDO_UNLIMIT_DELAY:
			  case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
				  bXTRA_RedoDelay = TRUE;
				
				  break;
				
				default:
					break;
			}
			
			// Check for reset GSM module
			switch(dlRes)
			{
				// For these case reset the GSM module before redo
			  case GSMRES_FAIL_REDO_LIMIT_RST:
				case GSMRES_FAIL_REDO_LIMIT_DELAY_RST:
				case GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST:
				  bXTRA_RedoRst = TRUE;
				
				  break;
				
				default:
					break;
			}
			
			if(bXTRA_RedoLimit == TRUE)
			{
			  tGPS_Xtra.u8GPS_XTRA_RetryCnt++;

				if(tGPS_Xtra.u8GPS_XTRA_RetryCnt >= 3)
				{
					tGPS_Xtra.u8GPS_XTRA_RetryCnt = 0;
					
					GSMprintf("\r\n> GPS XTRA maximum redo reached. Terminate operation.");
					
					//GSM_TCP_ReportEvent(EVENT_FTP_UPLOAD_FAIL, gsmUploadRequest.cSrcFile);
		
					tcpRes = TCP_OK;
					
					procXTRA_Download_Step = XTRA_STEP_NULL;
					
					break;
				}
				
				System_SaveSetting();
			}
			
			if(bXTRA_RedoDelay)
			{
				uint32_t u32DelayTime = 0;
				
				
				if(dlRes == GSMRES_FAIL_REDO_UNLIMIT_DELAY_RST)
					u32DelayTime = STOP_UPLOAD_VDR_TIMEOUT;
				else
					u32DelayTime = STOP_FTP_ACCESS_TIMEOUT;
				
				sprintf(cUploadDelayMsg, " after %d seconds", u32DelayTime);
			
				GSMprintf("\r\n> GPS XTRA%s request to redo%s%s.", 
									bXTRA_RedoRst ? (char *)" reset GSM module and" : (char *)"",
									bXTRA_RedoDelay ? cUploadDelayMsg : (char *)"",
									bXTRA_RedoLimit ? (char *)"" : (char *)" (Unlimited)");
				
				if(bXTRA_RedoDelay)
					FuncSkip_Enable(FUNC_GSM_FTP, u32DelayTime);
			}
								
			if(bXTRA_RedoRst)
				//GSM_Reset();
			  bGSM_ResetFlag = TRUE;
			
			tGPS_Xtra.bGPS_XTRA_Update = TRUE;
			
			tcpRes = TCP_OK;
			
			procXTRA_Download_Step = XTRA_STEP_NULL;
			
			break;
		}
		
		default:
			break;
	}
	
	return tcpRes;
}
#endif

//============================================================================//
// @brief  Create an XTRA update request.
// @param  pcFileName: XTRA file path.
//         u32Size: XTRA file size.
// @retval None.
//============================================================================//
void GSM_XTRA_RequestUpdate(char *pcFilePath, uint32_t u32Size)
{
    if ((tXTRA_Update.bRequest == TRUE) || (procXTRA_Update_Step != XTRA_UPDATE_STEP_NULL))
    {
        return;
    }

    if (strlen(pcFilePath) > sizeof(tXTRA_Update.cFilePath))
    {
        GSMprintf("\r\n> GPS XTRA request file path ERROR: %s", strlen(pcFilePath));

        return;
    }

    if (u32Size > GPS_XTRA_SIZE_MAX)
    {
        GSMprintf("\r\n> GPS XTRA request file size ERROR: %d", u32Size);

        return;
    }

    memcpy(tXTRA_Update.cFilePath, pcFilePath, strlen(pcFilePath));
    tXTRA_Update.u32FileSize = u32Size;
    tXTRA_Update.bRequest = TRUE;

    GSMprintf("\r\n> Add GPS XTRA update request OK");

    procXTRA_Update_Step = XTRA_UPDATE_STEP_INIT;
}

//============================================================================//
// @brief  Begin XTRA update process.
// @param  None.
// @retval None.
//============================================================================//
void GSM_XTRA_UpdateInit(void)
{
    if ((tXTRA_Update.bRequest == TRUE) && (procXTRA_Update_Step == XTRA_UPDATE_STEP_NULL))
    {
        // tXTRA_Update.bRequest = FALSE;

        procXTRA_Update_Step = XTRA_UPDATE_STEP_INIT;
    }
}

//============================================================================//
// @brief  Handle XTRA file update process.
// @param  None.
// @retval Result in TCP_StatusTypeDef.
//============================================================================//
TCP_StatusTypeDef GSM_XTRA_Update_Proc(void)
{
    TCP_StatusTypeDef tcpRes;
    bool bRes;
    // static XTRA_UpdateStepTypeDef procXTRA_UpdateLastStep;
    static GSM_NonBlockResp_ResTypeDef xtraRes;
    static uint8_t u8XTRA_RetryCnt = 0;
    // static uint32_t u32XTRA_LastTick = 0;

    if (procXTRA_Update_Step == XTRA_UPDATE_STEP_NULL)
        return TCP_OK;

    if ((bQtEC21_State == FALSE) || (bQtEC21_InitFinish == FALSE))
        return TCP_OK;

    // if(procXTRA_Update_Step != XTRA_UPDATE_STEP_FINISH)
    //{
    //	procXTRA_UpdateLastStep = procXTRA_Update_Step;
    // }

    switch ((int)procXTRA_Update_Step)
    {
    // XTRA_UPDATE_STEP_INIT: Init update XTRA
    case XTRA_UPDATE_STEP_INIT:
    {
        u8XTRA_RetryCnt = 0;
        xtraRes = GSMRES_SUCCESS;

        // Disable GNSS function before start
        if (bGPS_Ready)
        {
            HAL_Delay(10);

            GPS_Finish();

            HAL_Delay(100);
        }

        procXTRA_Update_Step = XTRA_UPDATE_STEP_SET_TIME;

        break;
    }

    // XTRA_UPDATE_STEP_SET_TIME: Inject GPS time
    case XTRA_UPDATE_STEP_SET_TIME:
    {
        char cTimeStamp[32] = {0};
        time_t ttCurrTime;
        struct tm tmTime = {0};

        ttCurrTime = System_GetTime() - g_Setting.i32TimeZoneOffset;

        localtime_r(&ttCurrTime, &tmTime);

        strftime(cTimeStamp, sizeof(cTimeStamp), "%Y/%m/%d,%H:%M:%S", &tmTime);

        bRes = GPS_XTRA_Time(cTimeStamp);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_OTHER_ERR;

            u8XTRA_RetryCnt++;

            if (u8XTRA_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8XTRA_RetryCnt = 0;

                GSMprintf("\r\n> GSM XTRA set time maximum loop retries reached.");

                xtraRes = GSMRES_FAIL_REDO_LIMIT;

                procXTRA_Update_Step = XTRA_UPDATE_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM XTRA set time ERROR. Retry in the next loop.");

                procXTRA_Update_Step = XTRA_UPDATE_STEP_SET_TIME;
            }

            return tcpRes;
        }

        u8XTRA_RetryCnt = 0;

        procXTRA_Update_Step = XTRA_UPDATE_STEP_ACT_DATA;

        tcpRes = TCP_OK;

        break;
    }

    // XTRA_UPDATE_STEP_ACT_DATA: Activate XTRA data file
    case XTRA_UPDATE_STEP_ACT_DATA:
    {
        HAL_Delay(10);

        bRes = GPS_XTRA_Data((char *)tXTRA_Update.cFilePath);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_OTHER_ERR;

            u8XTRA_RetryCnt++;

            if (u8XTRA_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8XTRA_RetryCnt = 0;

                GSMprintf("\r\n> GSM XTRA activate maximum loop retries reached.");

                xtraRes = GSMRES_FAIL_REDO_LIMIT;

                procXTRA_Update_Step = XTRA_UPDATE_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM XTRA activate ERROR. Retry in the next loop.");

                procXTRA_Update_Step = XTRA_UPDATE_STEP_ACT_DATA;
            }

            return tcpRes;
        }

        u8XTRA_RetryCnt = 0;

        procXTRA_Update_Step = XTRA_UPDATE_STEP_VERIFY;

        tcpRes = TCP_OK;

        break;
    }

    // XTRA_UPDATE_STEP_VERIFY: Verify XTRA file is OK
    case XTRA_UPDATE_STEP_VERIFY:
    {
        uint16_t u16DurTime;
        time_t ttValidTime;

        HAL_Delay(10);

        bRes = GPS_XTRA_DataQuery(&u16DurTime, &ttValidTime);

        if (!bRes)
        {
            tcpRes = TCP_HTTP_OTHER_ERR;

            u8XTRA_RetryCnt++;

            if (u8XTRA_RetryCnt >= GSM_FUNCTION_RETRY_MAX)
            {
                u8XTRA_RetryCnt = 0;

                GSMprintf("\r\n> GSM XTRA verify maximum loop retries reached.");

                xtraRes = GSMRES_FAIL_REDO_LIMIT;

                procXTRA_Update_Step = XTRA_UPDATE_STEP_FINISH;
            }
            else
            {
                GSMprintf("\r\n> GSM XTRA verify ERROR. Retry in the next loop.");

                procXTRA_Update_Step = XTRA_UPDATE_STEP_VERIFY;
            }

            return tcpRes;
        }

        u8XTRA_RetryCnt = 0;

        if (u16DurTime == 0)
        {
            xtraRes = GSMRES_FAIL_NO_REDO;

            procXTRA_Update_Step = XTRA_UPDATE_STEP_FINISH;
        }

        xtraRes = GSMRES_SUCCESS;

        procXTRA_Update_Step = XTRA_UPDATE_STEP_FINISH;

        tcpRes = TCP_OK;

        break;
    }

    // XTRA_UPDATE_STEP_FINISH: Complete the operation
    case XTRA_UPDATE_STEP_FINISH:
    {
        tXTRA_Update.bRequest = FALSE;

        if (bGPS_Ready == FALSE)
            GPS_TurnOn();

        if (xtraRes != GSMRES_SUCCESS)
        {
            GSMprintf("\r\n> GPS XTRA update FAIL. Terminate operation.");
        }
        else
        {
            GSMprintf("\r\n> GPS XTRA update OK");
        }

        procXTRA_Update_Step = XTRA_UPDATE_STEP_NULL;

        break;
    }

    default:
        break;
    }

    return tcpRes;
}
#endif

//============================================================================//
// @brief  Save a TCP/IP packet into the temporary storage file in SD card.
// @param  pu8Buf: Pointer to a uint8_t buffer storing the packet.
//         u16Len: Length of the packet.
//         u8Dst: ID number of the destination server.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool GSM_SaveTempPkg(uint8_t *pu8Buf, uint16_t u16Len, uint8_t u8Dst)
{
    bool bRes = FALSE;
    char cFileName[64];
    uint32_t u32WritePos;
    uint16_t u16TempIdx;

    if (u8Dst == SOCKET_BANK_DEF)
    {
        if (u16UnsendPkgNum[SOCKET_BANK_DEF] >= GSM_PACKET_NUM_MAX)
            u16UnsendPkgNum[SOCKET_BANK_DEF] = 0;

        u32WritePos = 2 + ((uint32_t)u16UnsendPkgNum[SOCKET_BANK_DEF] * TEMP_PKG_SIZE);

        u16TempIdx = u16UnsendPkgNum[SOCKET_BANK_DEF] + 1;

        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_TEMP_FILE);

        pu8Buf[u16Len] = u8Dst;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16TempIdx, 2, 0);

            if (bRes)
                break;
        }

        if (bRes)
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, pu8Buf, u16Len + 1, u32WritePos);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                GSMprintf("\r\n> GSM save packet %d-%d OK.", u8Dst, u16TempIdx);

                u16UnsendPkgNum[SOCKET_BANK_DEF]++;
            }
            else
            {
                GSMprintf("\r\n> GSM save packet %d-%d ERROR.", u8Dst, u16TempIdx);
            }
        }
        else
        {
            GSMprintf("\r\n> GSM save packet %d-%d ERROR.", u8Dst, u16TempIdx, u8Dst);
        }
    }
#if ENABLE_MULTI_SERVER
    else
    {
        if (u16UnsendPkgNum[SOCKET_BANK_EXT] >= GSM_PACKET_NUM_MAX)
            u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

        u32WritePos = 2 + ((uint32_t)u16UnsendPkgNum[SOCKET_BANK_EXT] * TEMP_PKG_SIZE);

        u16TempIdx = u16UnsendPkgNum[SOCKET_BANK_EXT] + 1;

        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_EXT_TEMP_FILE);

        pu8Buf[u16Len] = u8Dst;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16TempIdx, 2, 0);

            if (bRes)
                break;
        }

        if (bRes)
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, pu8Buf, u16Len + 1, u32WritePos);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                GSMprintf("\r\n> GSM save packet %d-%d OK.", u8Dst, u16TempIdx);

                u16UnsendPkgNum[SOCKET_BANK_EXT]++;
            }
            else
            {
                GSMprintf("\r\n> GSM save packet %d-%d ERROR.", u8Dst, u16TempIdx);
            }
        }
        else
        {
            GSMprintf("\r\n> GSM save packet %d-%d ERROR.", u8Dst, u16TempIdx);
        }
    }
#endif

    return bRes;
}

//============================================================================//
// @brief  Get a TCP/IP packet from the temporary file in SD card.
// @param  pu8Buf: Pointer to a uint8_t buffer to store the packet.
//         u16Len: Length of the packet.
//         pu8Dst: Pointer to a uint8_t variable to store the ID number of the
//                 destination server.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool GSM_GetTempPkg(uint8_t *pu8Buf, uint16_t *pu16ReadLen, uint8_t u8Dst)
{
    bool bRes;
    // osStatus osRes;
    char cFileName[164];
    uint32_t u32ReadPos;
    uint16_t u16TempIdx;

    if (u8Dst == SOCKET_BANK_DEF)
    {
        if (u16UnsendPkgNum[SOCKET_BANK_DEF] == 0)
            return FALSE;

        u32ReadPos = 2 + (((uint32_t)u16UnsendPkgNum[SOCKET_BANK_DEF] - 1) * TEMP_PKG_SIZE);

        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_TEMP_FILE);

        memset(pu8Buf, 0x00, TEMP_PKG_SIZE);

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_ReadAddrToBuff(cFileName, pu8Buf, TEMP_PKG_SIZE, u32ReadPos);

            if (bRes)
                break;
        }

        if (bRes)
        {
            uint16_t u16FindPos;
            u16TempIdx = u16UnsendPkgNum[SOCKET_BANK_DEF];

            u16UnsendPkgNum[SOCKET_BANK_DEF]--;

            SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);

            if (*pu8Buf == (uint8_t)'$')
            {
                bRes = FindSequence((char *)pu8Buf, (char *)"#\r\n", (uint16_t)TEMP_PKG_SIZE, 3, &u16FindPos);

                if (bRes)
                {
                    *pu16ReadLen = u16FindPos + 3;

                    if (pu8Buf[*pu16ReadLen] != u8Dst)
                    {
                        GSMprintf("\r\n> GSM get packet %d-%d ID ERROR", u8Dst, u16TempIdx);
                    }

                    // if((pu8Buf[*pu16ReadLen] == 0) || (pu8Buf[*pu16ReadLen] == 1))
                    //{
                    //	*pu8Dst = pu8Buf[*pu16ReadLen];

                    // GSMprintf("\r\n> GSM get unsent packet %d for server %d OK", u16TempIdx, *pu8Dst);
                    //}
                    // else
                    //{
                    //	GSMprintf("\r\n> Not found packet destination server. Read out value:%d", pu8Buf[*pu16ReadLen]);

                    //	*pu16ReadLen = 0;
                    //}
                }
                else
                {
                    GSMprintf("\r\n> GSM get packet %d-%d not found packet end sequence.", u8Dst, u16TempIdx);

                    *pu16ReadLen = 0;
                }
            }
            else
            {
                GSMprintf("\r\n> GSM get packet %d-%d not found packet begin character.", u8Dst, u16TempIdx);

                *pu16ReadLen = 0;
            }

            bRes = TRUE;
        }
        else
        {
            // #if ENABLE_DEBUG_MSG
            GSMprintf("\r\n> GSM get packet %d-%d ERROR", u8Dst, u16UnsendPkgNum[SOCKET_BANK_DEF]);
            // #endif
        }
    }
#if ENABLE_MULTI_SERVER
    else
    {
        if (u16UnsendPkgNum[SOCKET_BANK_EXT] == 0)
            return FALSE;

        u32ReadPos = 2 + (((uint32_t)u16UnsendPkgNum[SOCKET_BANK_EXT] - 1) * TEMP_PKG_SIZE);

        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_EXT_TEMP_FILE);

        memset(pu8Buf, 0x00, TEMP_PKG_SIZE);

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_ReadAddrToBuff(cFileName, pu8Buf, TEMP_PKG_SIZE, u32ReadPos);

            if (bRes)
                break;
        }

        if (bRes)
        {
            uint16_t u16FindPos;
            u16TempIdx = u16UnsendPkgNum[SOCKET_BANK_EXT];

            u16UnsendPkgNum[SOCKET_BANK_EXT]--;

            SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);

            if (*pu8Buf == (uint8_t)'$')
            {
                bRes = FindSequence((char *)pu8Buf, (char *)"#\r\n", (uint16_t)TEMP_PKG_SIZE, 3, &u16FindPos);

                if (bRes)
                {
                    *pu16ReadLen = u16FindPos + 3;

                    if (pu8Buf[*pu16ReadLen] != u8Dst)
                    {
                        GSMprintf("\r\n> GSM get packet %d-%d ID ERROR", u8Dst, u16TempIdx);
                    }

                    // if((pu8Buf[*pu16ReadLen] == 0) || (pu8Buf[*pu16ReadLen] == 1))
                    //{
                    //	*pu8Dst = pu8Buf[*pu16ReadLen];

                    // GSMprintf("\r\n> GSM get unsent packet %d for server %d OK", u16TempIdx, *pu8Dst);
                    //}
                    // else
                    //{
                    //	GSMprintf("\r\n> Not found packet destination server. Read out value:%d", pu8Buf[*pu16ReadLen]);

                    //	*pu16ReadLen = 0;
                    //}
                }
                else
                {
                    GSMprintf("\r\n> GSM get packet %d-%d not found packet end sequence.", u8Dst, u16TempIdx);

                    *pu16ReadLen = 0;
                }
            }
            else
            {
                GSMprintf("\r\n> GSM get packet %d-%d not found packet begin character.", u8Dst, u16TempIdx);

                *pu16ReadLen = 0;
            }

            bRes = TRUE;
        }
        else
        {
            // #if ENABLE_DEBUG_MSG
            GSMprintf("\r\n> GSM get packet %d-%d ERROR", u8Dst, u16UnsendPkgNum[SOCKET_BANK_EXT]);
            // #endif
        }
    }
#endif

    return bRes;
}

//============================================================================//
// @brief  Send TCP/IP CHK message to server.
// @param  u8Dst: ID of destination server, 0 or 1.
// @retval Result in MsgSendStatusTypedef format.
//============================================================================//
MsgSendStatusTypeDef GSM_TCP_SendCHK(uint8_t u8Dst)
{
    MsgSendStatusTypeDef smsRes;
    uint8_t u8Buf[256] = {0};

    if (strlen(g_Setting.tGSM_SET.tMobileInfo.cIMEI) != 15)
    {
        GSM_GetIMEI(g_Setting.tGSM_SET.tMobileInfo.cIMEI);

        System_SaveSetting();
    }

    if (strlen(g_Setting.tGSM_SET.tMobileInfo.cIMSI) != 15)
    {
        GSM_GetIMSI(g_Setting.tGSM_SET.tMobileInfo.cIMSI);

        System_SaveSetting();
    }

#if ENABLE_MULTI_SERVER
    if ((u8Dst == SOCKET_BANK_EXT) &&
        ((GSM_TCP_CheckServDuplicate() == TRUE) || (g_Setting.tGSM_SET.bDisableSocketEXT == TRUE)))
    {
        return SEND_OK;
    }
#endif

    // HAL_Delay(100);

    sprintf((char *)u8Buf, "%s,%s,%s,%s,%s,FW-%d.%03d%s,%d,%c%d,%d,%s,%d.%d.%d.%d:%d,%s:%d,%d,%d,%d,%f,%d,%d, #\r\n",
            u8Dst == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_CHK : (char *)SERVER_2_HEADER_CHK,
            g_Setting.tGSM_SET.tMobileInfo.cIMEI,
            g_Setting.tBusInfo.cBus_ID,
            g_Setting.tGSM_SET.tMobileInfo.cIMSI,
            g_Setting.cJAS_Ver,
            (VERSION_BUILD / 1000),
            (VERSION_BUILD % 1000),
            FW_VERSION_CHK,
            g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
            (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
            g_SysInfo.tGPS_Data.u8StlNum,
            g_SysInfo.tEXTIO.bits.IN_ACC,
            g_Setting.tGSM_SET.tNetSet.cAPN,
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u32IP_Addr._U8[3],
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u32IP_Addr._U8[2],
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u32IP_Addr._U8[1],
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u32IP_Addr._U8[0],
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u16Port,
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].cDomainName,
            // g_Setting.tGSM_SET.tServerInfo[u8Dst].u16Port,
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0],
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
            g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port,
            u8DATALOG_UPLOAD_MAX[u8Dst],
            g_Setting.tGSM_SET.tNetSet.u16SendStandby,
            g_Setting.tSpeedSet.u16Trig,
            g_Setting.tSpeedSet.fGain,
            g_Setting.tRPM_Set.u16Trig,
            g_Setting.tRPM_Set.u16DIV);

    // GSMprintf("\r\n> GSM CHK:\r\n%s\r\n> GSM CHK end.", (char *)u8Buf);
    GSMprintf("\r\n\r\n> %s", (char *)u8Buf);

    gsmTCP_CHK[u8Dst].SocketNum = u8CurrTCP_Socket[u8Dst];
    gsmTCP_CHK[u8Dst].pcData = (char *)u8Buf;
    gsmTCP_CHK[u8Dst].u16DataLen = strlen((char *)u8Buf);

    smsRes = GSM_TCP_Send(&gsmTCP_CHK[u8Dst]);

    if (smsRes == SEND_OK)
        HAL_Delay(1500);

    return smsRes;
}

//============================================================================//
// @brief  Send TCP/IP DATA message to server.
// @param  pcEventCode: String of event code.
//         pcEventMsg: String of event message.
//         u8SocketBank: ID of the server to which the message is sent.
// @retval None.
//============================================================================//
void GSM_TCP_SendEVENT(char *pcEventCode, char *pcEventMsg)
{
    // uint8_t u8GSM_EVENT_Buf[256] = {0};
    struct tm tmTime = {0};
    time_t rawtime;
    char szDateStr[10];
    char szTimeStr[10];
    char szDriverIN[10] = {0};
    uint8_t u8Time[7];
    bool bRes;
    DatalogEventlogTypeDef stDatalogEvent;

    if ((bSystemTimeReady == FALSE) || (bSD_ReportErrFlag == TRUE))
    {
        return;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            return;
        }
    }

    rawtime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    localtime_r(&rawtime, &tmTime);

    strftime(szDateStr, sizeof(szDateStr), "%d%m%y", &tmTime);

    strftime(szTimeStr, sizeof(szTimeStr), "%H%M%S", &tmTime);

    sprintf(szDriverIN, "%08X", g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum);
    // GSMprintf("\r\n> $DATA Driver ID = %s", szDriverIN);

    memset(&stDatalogEvent, 0x00, sizeof(DatalogEventlogTypeDef));

    stDatalogEvent.CMD = 0xDC;
    stDatalogEvent.UTC = rawtime;
    stDatalogEvent.u16EventCode = atoi(pcEventCode);
    stDatalogEvent.u8Speed = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);
    stDatalogEvent.i32Longitude = g_SysInfo.tGPS_Data.gpsLocation.dLongitude * 1000000;
    stDatalogEvent.i32Latitude = g_SysInfo.tGPS_Data.gpsLocation.dLatitude * 1000000;
    stDatalogEvent.dwDistance = g_SysInfo.tVehicleState.dDistance;

    stDatalogEvent.u32DRV_IN = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum;

    stDatalogEvent.u16RPM = g_SysInfo.tVehicleState.u16RPM;
    stDatalogEvent.u16Course = g_SysInfo.tGPS_Data.u16Course;
    stDatalogEvent.u8Fixed = g_SysInfo.tGPS_Data.bStatus;
    stDatalogEvent.u8STNum = g_SysInfo.tGPS_Data.u8StlNum;
    stDatalogEvent.u8CSQ = g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ;
    stDatalogEvent.EXTIO._U8 = g_SysInfo.tEXTIO.word;

    SD_AddBuffToFile(FILENAME_DATALOG_EVENT_BIN, (uint8_t *)&stDatalogEvent, sizeof(DatalogEventlogTypeDef));

    // Record Table 11 in VDR file
    rawtime += g_Setting.i32TimeZoneOffset;

    SecToBCDTime(rawtime, u8Time);

    V161_WriteDatalogEventToVDR(u8Time, stDatalogEvent.u16EventCode);

    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
#if ENABLE_MULTI_SERVER
        if ((i == SOCKET_BANK_EXT) &&
            ((GSM_TCP_CheckServDuplicate() == TRUE) || (g_Setting.tGSM_SET.bDisableSocketEXT == TRUE)))
        {
            continue;
        }
#endif

        // if(g_Setting.tGSM_SET.tServerInfo[i].bEnableEvent == TRUE)
        {
            memset(u8GSM_EVENT_Buf, 0x00, sizeof(u8GSM_EVENT_Buf));

            sprintf((char *)u8GSM_EVENT_Buf, "%s,%s,%s,%s,%s,%s,%c,%c%03d%02d.%04d,%c%02d%02d.%04d,%03d,%03d,%09d,%04d,%s,%02d,%02d,%d%d%d%d%d%d%d%d,%s,%s#\r\n",
                    i == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_EVENT : (char *)SERVER_2_HEADER_EVENT,
                    g_Setting.tGSM_SET.tMobileInfo.cIMEI,
                    g_Setting.tBusInfo.cBus_ID,
                    g_Setting.tGSM_SET.tMobileInfo.cIMSI,
                    szDateStr,
                    szTimeStr,
                    (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                    'E',
                    g_SysInfo.tGPS_Data.gpsLon.u8ddd,
                    g_SysInfo.tGPS_Data.gpsLon.u8mm,
                    g_SysInfo.tGPS_Data.gpsLon.u16mmmm,
                    'N',
                    g_SysInfo.tGPS_Data.gpsLat.u8dd,
                    g_SysInfo.tGPS_Data.gpsLat.u8mm,
                    g_SysInfo.tGPS_Data.gpsLat.u16mmmm,
                    g_SysInfo.tGPS_Data.u16Course, // Heinz // GPS_AllData.u16Course,
                    (uint8_t)M_2_KM(g_SysInfo.tVehicleState.u32Speed),
                    (uint32_t)g_SysInfo.tVehicleState.dDistance,
                    g_SysInfo.tVehicleState.u16RPM,
                    szDriverIN,
                    g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                    g_SysInfo.tGPS_Data.u8StlNum,
                    g_SysInfo.tEXTIO.bits.IN_ACC,
                    g_SysInfo.tEXTIO.bits.IN_1,
                    g_SysInfo.tEXTIO.bits.IN_2,
                    g_SysInfo.tEXTIO.bits.IN_3,
                    g_SysInfo.tEXTIO.bits.IN_4,
                    g_SysInfo.tEXTIO.bits.IN_5,
                    g_SysInfo.tEXTIO.bits.IN_6,
                    g_SysInfo.tEXTIO.bits.IN_7,
                    pcEventCode,
                    pcEventMsg);

            GSMprintf("\r\n\r\n> %s", (char *)u8GSM_EVENT_Buf);

            for (int j = 0; j < 3; j++)
            {
                bRes = GSM_SaveTempPkg(u8GSM_EVENT_Buf, strlen((char *)u8GSM_EVENT_Buf), i);

                if (bRes)
                {
                    // GSMprintf("\r\n> ### %s: %s, %s, %s Send OK ###",
                    //          g_Setting.tGSM_SET.tServerInfo[i].cHeaderEvent,
                    //          szDateStr,
                    //          szTimeStr,
                    //          pcEventCode);

                    break;
                }
            }
        }
    }

    if (bRTOS_En)
        // Release GSM SD synchronization
        osMutexRelease(mutex_gsm_sd_user);
}

//============================================================================//
// @brief  Send TCP/IP DATA message to server directly.
// @param  pcEventCode: String of event code.
//         pcEventMsg: String of event message.
//         u8SocketBank: ID of the server to which the message is sent.
// @retval None.
//============================================================================//
bool GSM_TCP_SendEVENT_Direct(char *pcEventCode, char *pcEventMsg, uint8_t u8SocketID)
{
    struct tm tmTime = {0};
    time_t rawtime;
    char szDateStr[10];
    char szTimeStr[10];
    char szDriverIN[10] = {0};
    uint8_t u8Time[7];
    uint16_t u16BufLen = 0;
    // bool bRes;
    DatalogEventlogTypeDef stDatalogEvent;
    MsgSendStatusTypeDef tcpRes = SEND_FAIL;

#if ENABLE_MULTI_SERVER
    if ((u8SocketID == SOCKET_BANK_EXT) &&
        ((GSM_TCP_CheckServDuplicate() == TRUE) || (g_Setting.tGSM_SET.bDisableSocketEXT == TRUE)))
    {
        return TRUE;
    }
#endif

    rawtime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    localtime_r(&rawtime, &tmTime);

    strftime(szDateStr, sizeof(szDateStr), "%d%m%y", &tmTime);

    strftime(szTimeStr, sizeof(szTimeStr), "%H%M%S", &tmTime);

    sprintf(szDriverIN, "%08X", g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum);
    // GSMprintf("\r\n> $DATA Driver ID = %s", szDriverIN);

    if (bSD_ReportErrFlag == FALSE)
    {
        memset(&stDatalogEvent, 0x00, sizeof(DatalogEventlogTypeDef));

        stDatalogEvent.CMD = 0xDC;
        stDatalogEvent.UTC = rawtime;
        stDatalogEvent.u16EventCode = atoi(pcEventCode);
        stDatalogEvent.u8Speed = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);
        stDatalogEvent.i32Longitude = g_SysInfo.tGPS_Data.gpsLocation.dLongitude * 1000000;
        stDatalogEvent.i32Latitude = g_SysInfo.tGPS_Data.gpsLocation.dLatitude * 1000000;
        stDatalogEvent.dwDistance = g_SysInfo.tVehicleState.dDistance;

        stDatalogEvent.u32DRV_IN = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum;

        stDatalogEvent.u16RPM = g_SysInfo.tVehicleState.u16RPM;
        stDatalogEvent.u16Course = g_SysInfo.tGPS_Data.u16Course;
        stDatalogEvent.u8Fixed = g_SysInfo.tGPS_Data.bStatus;
        stDatalogEvent.u8STNum = g_SysInfo.tGPS_Data.u8StlNum;
        stDatalogEvent.u8CSQ = g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ;
        stDatalogEvent.EXTIO._U8 = g_SysInfo.tEXTIO.word;

        SD_AddBuffToFile(FILENAME_DATALOG_EVENT_BIN, (uint8_t *)&stDatalogEvent, sizeof(DatalogEventlogTypeDef));

        // Record Table 11 in VDR file
        rawtime += g_Setting.i32TimeZoneOffset;

        SecToBCDTime(rawtime, u8Time);

        V161_WriteDatalogEventToVDR(u8Time, stDatalogEvent.u16EventCode);
    }

    // if(g_Setting.tGSM_SET.tServerInfo[u8SocketID].bEnableEvent == TRUE)
    {
        memset(u8GSM_EVENT_Buf, 0x00, sizeof(u8GSM_EVENT_Buf));

        sprintf((char *)u8GSM_EVENT_Buf, "%s,%s,%s,%s,%s,%s,%c,%c%03d%02d.%04d,%c%02d%02d.%04d,%03d,%03d,%09d,%04d,%s,%02d,%02d,%d%d%d%d%d%d%d%d,%s,%s#\r\n",
                u8SocketID == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_EVENT : (char *)SERVER_2_HEADER_EVENT,
                g_Setting.tGSM_SET.tMobileInfo.cIMEI,
                g_Setting.tBusInfo.cBus_ID,
                g_Setting.tGSM_SET.tMobileInfo.cIMSI,
                szDateStr,
                szTimeStr,
                (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                'E',
                g_SysInfo.tGPS_Data.gpsLon.u8ddd,
                g_SysInfo.tGPS_Data.gpsLon.u8mm,
                g_SysInfo.tGPS_Data.gpsLon.u16mmmm,
                'N',
                g_SysInfo.tGPS_Data.gpsLat.u8dd,
                g_SysInfo.tGPS_Data.gpsLat.u8mm,
                g_SysInfo.tGPS_Data.gpsLat.u16mmmm,
                g_SysInfo.tGPS_Data.u16Course, // Heinz // GPS_AllData.u16Course,
                (uint8_t)M_2_KM(g_SysInfo.tVehicleState.u32Speed),
                (uint32_t)g_SysInfo.tVehicleState.dDistance,
                g_SysInfo.tVehicleState.u16RPM,
                szDriverIN,
                g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                g_SysInfo.tGPS_Data.u8StlNum,
                g_SysInfo.tEXTIO.bits.IN_ACC,
                g_SysInfo.tEXTIO.bits.IN_1,
                g_SysInfo.tEXTIO.bits.IN_2,
                g_SysInfo.tEXTIO.bits.IN_3,
                g_SysInfo.tEXTIO.bits.IN_4,
                g_SysInfo.tEXTIO.bits.IN_5,
                g_SysInfo.tEXTIO.bits.IN_6,
                g_SysInfo.tEXTIO.bits.IN_7,
                pcEventCode,
                pcEventMsg);

        u16BufLen = strlen((char *)u8GSM_EVENT_Buf);

        GSMprintf("\r\n\r\n> %s", (char *)u8GSM_EVENT_Buf);

        if ((bTCPIP_Connect[u8SocketID] == TRUE) && (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ > 8))
        {
            gsmTCP_DATA[u8SocketID].SocketNum = u8CurrTCP_Socket[u8SocketID];
            gsmTCP_DATA[u8SocketID].pcData = (char *)u8GSM_EVENT_Buf;
            gsmTCP_DATA[u8SocketID].u16DataLen = u16BufLen;

            for (int j = 0; j < 3; j++)
            {
                tcpRes = GSM_TCP_Send(&gsmTCP_DATA[u8SocketID]);

                if (tcpRes == SEND_OK)
                    break;
            }

            HAL_Delay(2000);
        }
    }

    if (tcpRes == SEND_OK)
        return TRUE;
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            bool bRes;

            if (bRTOS_En)
            {
                osStatus osRes;

                // Acquire GSM_SD synchronization
                osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

                if (osRes != osOK)
                {
                    return FALSE;
                }
            }

            for (int j = 0; j < 3; j++)
            {
                bRes = GSM_SaveTempPkg(u8GSM_EVENT_Buf, u16BufLen, u8SocketID);

                if (bRes)
                    break;
            }

            if (bRTOS_En)
                // Release GSM SD synchronization
                osMutexRelease(mutex_gsm_sd_user);
        }

        return FALSE;
    }
}

//============================================================================//
// @brief  Send TCP/IP LOG message to server.
// @param  pUPLOAD: DatalogUploadTypeDef variable which stores the upload data.
//         u8SocketBank: ID of the server to which the message is sent.
// @retval None.
//============================================================================//
void GSM_TCP_SendLOG(DatalogUploadTypeDef *pUPLOAD, uint8_t u8SocketBank)
{
    MsgSendStatusTypeDef msgRes;
    struct tm tmTime = {0};
    uint16_t u16BufLen = 0;
    uint8_t *pBufPtr;
    char szDateStr[10];
    char szTimeStr[10];
    uint8_t u8BCDTime2[7] = {0};
    uint8_t *pData;
    uint16_t u16CRC_Val;
    uint8_t u8UploadLen;
    bool bRes;

    if (bSystemTimeReady == FALSE)
        return;

    /*###
    if(bRTOS_En)
    {
        osStatus osRes;


        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if(osRes != osOK)
        {
            return;
        }
    }
    */

    localtime_r(&ttDataLogTime[u8SocketBank], &tmTime);

    strftime(szDateStr, sizeof(szDateStr), "%d%m%y", &tmTime);

    strftime(szTimeStr, sizeof(szTimeStr), "%H%M%S", &tmTime);

    SecToBCDTime(ttDataLogTime[u8SocketBank] + g_Setting.i32TimeZoneOffset, u8BCDTime2);

    u8UploadLen = u8DATALOG_UPLOAD_MAX[u8SocketBank];

    memset(u8GSM_LOG_Buf, 0x00, sizeof(u8GSM_LOG_Buf));

    pBufPtr = (uint8_t *)u8GSM_LOG_Buf;

    u16BufLen = 0;

    // If not send to Government server
    if (strncmp(g_Setting.tGSM_SET.tServerInfo[u8SocketBank].cIP_Addr,
                (char *)SERVER_GOV_IP_ADDR,
                strlen((char *)SERVER_GOV_IP_ADDR)) != 0)
    {
        sprintf((char *)pBufPtr, "%s,%s,%s,%s,%08X,%s,%s,%c,%02d,%02d,%09d,%02d,",
                u8SocketBank == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_LOG : (char *)SERVER_2_HEADER_LOG,
                g_Setting.tGSM_SET.tMobileInfo.cIMEI,
                g_Setting.tBusInfo.cBus_ID,
                g_Setting.tGSM_SET.tMobileInfo.cIMSI,
                g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum,
                szDateStr,
                szTimeStr,
                (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                g_SysInfo.tGPS_Data.u8StlNum,
                // pUPLOAD->HEAD.dwDistance,
                (uint32_t)g_SysInfo.tVehicleState.dDistance,
                u8UploadLen);

        GSMprintf("\r\n\r\n> %s", (char *)u8GSM_LOG_Buf);

        u16BufLen += strlen((char *)pBufPtr);

        pBufPtr += strlen((char *)pBufPtr);

        memcpy(pBufPtr, pUPLOAD->DATA, sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]);

        u16BufLen += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pBufPtr += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pData = (uint8_t *)pUPLOAD->DATA;

        CRC16_Reset(&u16CRC_Val);

        for (int k = 0; k < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; k++)
        {
            u16CRC_Val = CRC16_Calc(u16CRC_Val, *pData++);
        }

        sprintf((char *)pBufPtr, ",%04X#\r\n", u16CRC_Val);

        u16BufLen += strlen((char *)pBufPtr);
    }
    // If send to Government server
    else
    {
        sprintf((char *)pBufPtr, "%s,%s,%s,%s,%c,%02d,%02d,%09d,%02d,",
                u8SocketBank == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_LOG : (char *)SERVER_2_HEADER_LOG,
                g_Setting.tBusInfo.cBus_ID,
                szDateStr,
                szTimeStr,
                (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                g_SysInfo.tGPS_Data.u8StlNum,
                pUPLOAD->HEAD.dwDistance,
                u8UploadLen);

        GSMprintf("\r\n\r\n> %s", (char *)u8GSM_LOG_Buf);

        u16BufLen += strlen((char *)pBufPtr);

        pBufPtr += strlen((char *)pBufPtr);

        memcpy(pBufPtr, pUPLOAD->DATA, sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]);

        // GSMprintf("\r\n> Datalog:");

        // for(int i = 0; i < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; i++)
        //   GSMprintf("%d,", *(pBufPtr + i));

        u16BufLen += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pBufPtr += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pData = (uint8_t *)pUPLOAD->DATA;

        CRC16_Reset(&u16CRC_Val);

        for (int i = 0; i < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; i++)
        {
            u16CRC_Val = CRC16_Calc(u16CRC_Val, *pData++);
        }

        sprintf((char *)pBufPtr, ",%04X#\r\n", u16CRC_Val);

        u16BufLen += strlen((char *)pBufPtr);

        // GSMprintf("\r\n> GSM Datalog:\r\n%s\r\n> GSM Datalog end.", (char *)u8GSM_LOG_Buf);
        // GSMprintf("\r\n\r\n> %s", (char *)u8GSM_LOG_Buf);
    }

    /*
    for(int i = 0; i < 3; i++)
    {
        bRes = GSM_SaveTempPkg(u8GSM_LOG_Buf, u16BufLen, u8SocketBank);

        if(bRes)
        {
            //GSMprintf("\r\n> GSM save temp LOG for server %d OK", u8SocketBank);

            break;
        }
    }
    */

    bRes = TRUE;

    if ((bQtEC21_State == FALSE) ||
        (GSM_IsBusy() == TRUE) ||
        (qtec21_BlockCmdInstance.qtec21Cmd == QTEC21_TCP_CONN_CLOSE) ||
        (bGSM_ResetFlag == TRUE) || (bGSM_ResetCheckFTP_Flag == TRUE) ||
        (gsmCurrNonBlockCmd == QTEC21_SMS_SEND))
    {
        bRes = FALSE;
    }
    else
    {
        if ((bTCPIP_Connect[u8SocketBank] == TRUE) &&
            (g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ > 8))
        {
            gsmTCP_LOG[u8SocketBank].SocketNum = u8CurrTCP_Socket[u8SocketBank];
            gsmTCP_LOG[u8SocketBank].pcData = (char *)u8GSM_LOG_Buf;
            gsmTCP_LOG[u8SocketBank].u16DataLen = u16BufLen;

            msgRes = GSM_TCP_Send(&gsmTCP_LOG[u8SocketBank]);

            if (msgRes != SEND_OK)
            {
                bRes = FALSE;

                u8SendLogFailCnt[u8SocketBank]++;

                if (u8SendLogFailCnt[u8SocketBank] >= GSM_SEND_LOG_RETRY_MAX)
                {
                    u8SendLogFailCnt[u8SocketBank] = 0;

                    char cLog[30] = {0};

                    sprintf(cLog, "Conn %d close flag ON(1)", u8SocketBank);

                    GPRS_DebugLog(cLog, strlen(cLog));

                    bTCP_CloseFlag[u8SocketBank] = TRUE;
                }
            }
            else
            {
                uint16_t u16UnAckBytes;
                uint8_t u8QueryCnt = 0;

                // GSMprintf("\r\n> GSM request to send DATALOG to server %d OK.", u8SocketBank);

                while (u8QueryCnt < 3)
                {
                    HAL_Delay(1000);

                    if (bTCPIP_Connect[u8SocketBank] == FALSE)
                    {
                        bRes = FALSE;

                        break;
                    }

                    bRes = GSM_TCP_SendingStatus(u8CurrTCP_Socket[u8SocketBank], &u16UnAckBytes);

                    if (bRes)
                    {
                        if (u16UnAckBytes == 0)
                        {
                            break;
                        }
                    }

                    u8QueryCnt++;
                }

                // GSMprintf("\r\n> Number of unsent bytes in connection socket %d: %d", u8CurrTCP_Socket[i], u16UnAckBytes);

                if (bRes)
                {
                    if (u16UnAckBytes > 0)
                    {
                        bRes = FALSE;
                    }
                    else
                    {
                        bRes = TRUE;
                    }
                }
            }
        }
        else
            bRes = FALSE;
    }

    if (bRes == FALSE)
    {
        if (bSD_ReportErrFlag == TRUE)
            return;

        if (bRTOS_En)
        {
            osStatus osRes;

            // Acquire GSM_SD synchronization
            osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

            if (osRes != osOK)
            {
                return;
            }
        }

        for (int j = 0; j < 3; j++)
        {
            bRes = GSM_SaveTempPkg(u8GSM_LOG_Buf, u16BufLen, u8SocketBank);

            if (bRes)
                break;
        }

        if (bRTOS_En)
            // Release GSM SD synchronization
            osMutexRelease(mutex_gsm_sd_user);
    }

    /*
    if(bRTOS_En)
        // Release GSM SD synchronization
        osMutexRelease(mutex_gsm_sd_user);
    */
}

//============================================================================//
// @brief  Handle reporting LOG message to server.
// @param  None.
// @retval None.
//============================================================================//
void GSM_LOG_Proc(uint8_t u8Dst)
{
    uint8_t u8Date[7] = {0};

#if ENABLE_MULTI_SERVER
    if (u8Dst == SOCKET_BANK_EXT)
    {
        return;
    }
#endif

    // if(g_Setting.tGSM_SET.tServerInfo[u8Dst].bEnableLog == TRUE)
    if (bSystemMode != SYSTEM_MODE_STEALTH)
    {
        SecToBCDTime(ttDataLogTime[u8Dst] + 1, u8Date);

        GSMprintf("\r\n> ### UTC %02X%02X/%02X/%02X %02X:%02X:%02X, DataLog for Server %d ###",
                  u8Date[0], u8Date[1], u8Date[2],
                  u8Date[3], u8Date[4], u8Date[5],
                  u8Date[6],
                  u8Dst);

        GSM_TCP_SendLOG(&datalogHandleTemp[u8Dst], u8Dst);
    }
}

//============================================================================//
// @brief  Create a request to send an Event message.
// @param  pcEventCode: Pointer to a char array storing the event code.
//         pcEventMsg: Pointer to a char array storing the event message.
// @retval None.
//============================================================================//
void GSM_TCP_ReportEvent(char *pcEventCode, char *pcEventMsg)
{
    if ((bSD_ReportErrFlag == FALSE) &&
        (bSystemTimeReady == TRUE))
    {
        // if(EXTIO_IsMainPowerNormal() || ((EXTIO_IsMainPowerNormal() == FALSE) &&
        //	                              ((strncmp(pcEventCode, (char *)EVENT_EXTERNAL_POWER_ISSUE, 3) == 0) ||
        //	                              (strncmp(pcEventCode, (char *)EVENT_KEY_OFF, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_USB_DISCON, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_USB_CON, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_SYSTEM_OFF, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_SYSTEM_ON, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_RPM_IDLE_END, 3) == 0) ||
        //															  (strncmp(pcEventCode, (char *)EVENT_SPEED_INIT, 3) == 0))))
        if (bSystemMode != SYSTEM_MODE_STEALTH)
        {
            GSMprintf("\r\n> Request to send event %s", pcEventCode);

            GSM_TCP_SendEVENT(pcEventCode, pcEventMsg);
        }
        else
        {
            if ((strncmp(pcEventCode, (char *)EVENT_FOTA_FAIL, 3) == 0) ||
                (strncmp(pcEventCode, (char *)EVENT_FTP_DOWNLOAD_OK, 3) == 0) ||
                (strncmp(pcEventCode, (char *)EVENT_FTP_DOWNLOAD_FAIL, 3) == 0) ||
                (strncmp(pcEventCode, (char *)EVENT_FTP_UPLOAD_OK, 3) == 0) ||
                (strncmp(pcEventCode, (char *)EVENT_FTP_UPLOAD_FAIL, 3) == 0))
            {
                for (int i = 0; i < MAX_SOCKET_BANK; i++)
                {
                    GSM_TCP_SendEVENT_Direct(pcEventCode, pcEventMsg, i);
                }
            }
        }
    }
}

//============================================================================//
// @brief  ReSend TCP/IP LOG message to server.
// @param  pUPLOAD: DatalogUploadTypeDef variable which stores the upload data.
//         u8SocketBank: ID of the server to which the data is sent.
// @retval None.
//============================================================================//
void GSM_TCP_ReSendLOG(DatalogUploadTypeDef *pUPLOAD, uint8_t u8SocketBank)
{
    struct tm tmTime = {0};
    uint16_t u16BufLen = 0;
    uint8_t *pBufPtr;
    char szDateStr[10];
    char szTimeStr[10];
    uint8_t *pData;
    uint16_t u16CRC_Val;
    uint8_t u8UploadLen;
    time_t ttBuf;
    bool bRes;

    if (bSystemTimeReady == FALSE)
        return;

#if ENABLE_MULTI_SERVER
    // if((u8SocketBank	== SOCKET_BANK_EXT) &&
    //	 (strcmp(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr,
    //					 g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr) == 0) &&
    //	 (g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port ==
    //		g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port))
    if (u8SocketBank == SOCKET_BANK_EXT)
    {
        return;
    }
#endif

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    // Acquire GSM_SD synchronization
    //	osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

    //	if(osRes != osOK)
    //	{
    //		return;
    //	}
    //}

    ttBuf = pUPLOAD->HEAD.UTC + g_Setting.i32TimeZoneOffset;

    localtime_r(&ttBuf, &tmTime);

    strftime(szDateStr, sizeof(szDateStr), "%d%m%y", &tmTime);

    strftime(szTimeStr, sizeof(szTimeStr), "%H%M%S", &tmTime);

    u8UploadLen = pUPLOAD->HEAD.u8DataCnt;

    memset(u8GSM_LOG_Buf2, 0x00, sizeof(u8GSM_LOG_Buf2));

    pBufPtr = (uint8_t *)u8GSM_LOG_Buf2;

    u16BufLen = 0;

    // If not send to Government server
    if (strncmp(g_Setting.tGSM_SET.tServerInfo[u8SocketBank].cIP_Addr,
                (char *)SERVER_GOV_IP_ADDR,
                strlen((char *)SERVER_GOV_IP_ADDR)) != 0)
    {
        sprintf((char *)pBufPtr, "%s,%s,%s,%s,%X,%s,%s,%c,%02d,%02d,%09d,%02d,",
                u8SocketBank == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_LOG : (char *)SERVER_2_HEADER_LOG,
                g_Setting.tGSM_SET.tMobileInfo.cIMEI,
                g_Setting.tBusInfo.cBus_ID,
                g_Setting.tGSM_SET.tMobileInfo.cIMSI,
                pUPLOAD->HEAD.u32DRV_IN,
                szDateStr,
                szTimeStr,
                (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                g_SysInfo.tGPS_Data.u8StlNum,
                pUPLOAD->HEAD.dwDistance,
                u8UploadLen);

        GSMprintf("\r\n> GSM Resend LOG:%s.", (char *)u8GSM_LOG_Buf2);

        u16BufLen += strlen((char *)pBufPtr);

        pBufPtr += strlen((char *)pBufPtr);

        memcpy(pBufPtr, pUPLOAD->DATA, sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]);

        // GSMprintf("\r\n> Datalog:");

        // for(int k = 0; k < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; k++)
        //   GSMprintf("%d,", *(pBufPtr + k));

        u16BufLen += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pBufPtr += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pData = (uint8_t *)pUPLOAD->DATA;

        CRC16_Reset(&u16CRC_Val);

        for (int k = 0; k < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; k++)
        {
            u16CRC_Val = CRC16_Calc(u16CRC_Val, *pData++);
        }

        sprintf((char *)pBufPtr, ",%04X#\r\n", u16CRC_Val);

        u16BufLen += strlen((char *)pBufPtr);
    }
    // If send to Goverment server
    else
    {
        sprintf((char *)pBufPtr, "%s,%s,%s,%s,%c,%02d,%02d,%09d,%02d,",
                u8SocketBank == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_LOG : (char *)SERVER_2_HEADER_LOG,
                g_Setting.tBusInfo.cBus_ID,
                szDateStr,
                szTimeStr,
                (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
                g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ,
                g_SysInfo.tGPS_Data.u8StlNum,
                pUPLOAD->HEAD.dwDistance,
                u8UploadLen);

        GSMprintf("\r\n> GSM Resend LOG:%s", (char *)u8GSM_LOG_Buf2);

        u16BufLen += strlen((char *)pBufPtr);

        pBufPtr += strlen((char *)pBufPtr);

        memcpy(pBufPtr, pUPLOAD->DATA, sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]);

        // GSMprintf("\r\n> Datalog:");

        // for(int i = 0; i < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; i++)
        //   GSMprintf("%d,", *(pBufPtr + i));

        u16BufLen += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pBufPtr += sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank];

        pData = (uint8_t *)pUPLOAD->DATA;

        CRC16_Reset(&u16CRC_Val);

        for (int i = 0; i < sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[u8SocketBank]; i++)
        {
            u16CRC_Val = CRC16_Calc(u16CRC_Val, *pData++);
        }

        sprintf((char *)pBufPtr, ",%04X#\r\n", u16CRC_Val);

        u16BufLen += strlen((char *)pBufPtr);

        // GSMprintf("\r\n> GSM Datalog:\r\n%s\r\n> GSM Datalog end.", (char *)u8GSM_LOG_Buf2);
    }

    for (int j = 0; j < 3; j++)
    {
        bRes = GSM_SaveTempPkg(u8GSM_LOG_Buf2, u16BufLen, u8SocketBank);

        if (bRes)
            break;
    }

    // if(bRTOS_En)
    //  Release GSM SD synchronization
    //	osMutexRelease(mutex_gsm_sd_user);
}

//============================================================================//
// @brief  ReSend TCP/IP EVENT message to server.
// @param  stMyDatalogEvent: DatalogEventTypeDef variable which contains the upload data.
//         u8SocketBank: ID of the server to which the data is sent.
// @retval None.
//============================================================================//
void GSM_TCP_ReSendEVENT(DatalogEventlogTypeDef stMyDatalogEvent, uint8_t u8SocketBank)
{
    time_t rawtime;
    struct tm tmTime = {0};
    char szDateStr[10];
    char szTimeStr[10];
    char szDriverIN[10] = {0};
    char szEventCode[10];
    char szLon[15], szLat[15];
    bool bRes;

    if (bSystemTimeReady == FALSE)
        return;

#if ENABLE_MULTI_SERVER
    if ((u8SocketBank == SOCKET_BANK_EXT) &&
        ((GSM_TCP_CheckServDuplicate() == TRUE) || (g_Setting.tGSM_SET.bDisableSocketEXT == TRUE)))
    {
        return;
    }
#endif

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    // Acquire GSM_SD synchronization
    //	osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

    //	if(osRes != osOK)
    //	{
    //		return;
    //	}
    //}

    // rawtime = System_GetTime() - g_Setting.i32TimeZoneOffset;
    rawtime = stMyDatalogEvent.UTC;

    localtime_r(&rawtime, &tmTime);

    strftime(szDateStr, sizeof(szDateStr), "%d%m%y", &tmTime);

    strftime(szTimeStr, sizeof(szTimeStr), "%H%M%S", &tmTime);

    sprintf(szDriverIN, "%08X", stMyDatalogEvent.u32DRV_IN);

    // GSMprintf("\r\n> $DATA Driver ID = %s", szDriverIN);

    sprintf(szEventCode, "%d", stMyDatalogEvent.u16EventCode);

    sprintf(szLon, "%d.%d", (stMyDatalogEvent.i32Longitude / 10000), (stMyDatalogEvent.i32Longitude % 10000));

    sprintf(szLat, "%d.%d", (stMyDatalogEvent.i32Latitude / 10000), (stMyDatalogEvent.i32Latitude % 10000));

    memset(u8GSM_EVENT_Buf, 0x00, sizeof(u8GSM_EVENT_Buf));

    sprintf((char *)u8GSM_EVENT_Buf, "%s,%s,%s,%s,%s,%s,%c,%c%s,%c%s,%03d,%03d,%09d,%04d,%s,%02d,%02d,%d%d%d%d%d%d%d%d,%s,%s#\r\n",
            u8SocketBank == SOCKET_BANK_DEF ? (char *)SERVER_1_HEADER_EVENT : (char *)SERVER_2_HEADER_EVENT,
            g_Setting.tGSM_SET.tMobileInfo.cIMEI,
            g_Setting.tBusInfo.cBus_ID,
            g_Setting.tGSM_SET.tMobileInfo.cIMSI,
            szDateStr,
            szTimeStr,
            (g_SysInfo.tGPS_Data.bStatus == TRUE) ? 'A' : 'V',
            'E',
            szLon,
            'N',
            szLat,
            stMyDatalogEvent.u16Course,
            stMyDatalogEvent.u8Speed,
            stMyDatalogEvent.dwDistance,
            stMyDatalogEvent.u16RPM,
            szDriverIN,
            stMyDatalogEvent.u8CSQ,
            stMyDatalogEvent.u8STNum,
            stMyDatalogEvent.EXTIO.bit._0,
            stMyDatalogEvent.EXTIO.bit._1,
            stMyDatalogEvent.EXTIO.bit._2,
            stMyDatalogEvent.EXTIO.bit._3,
            stMyDatalogEvent.EXTIO.bit._4,
            stMyDatalogEvent.EXTIO.bit._5,
            stMyDatalogEvent.EXTIO.bit._6,
            stMyDatalogEvent.EXTIO.bit._7,
            szEventCode,
            "");

    GSMprintf("\r\n\r\n> GSM Resend EVENT:%s", (char *)u8GSM_EVENT_Buf);

    for (int j = 0; j < 3; j++)
    {
        bRes = GSM_SaveTempPkg(u8GSM_EVENT_Buf, strlen((char *)u8GSM_EVENT_Buf), u8SocketBank);

        if (bRes)
            break;
    }

    // if(bRTOS_En)
    //  Release GSM SD synchronization
    //	osMutexRelease(mutex_gsm_sd_user);
}

//============================================================================//
// @brief  Check temp files that serve GSM functions.
// @param  None.
// @retval None.
//============================================================================//
void GSM_CheckTempFile(void)
{
    char cFileName[64] = {0};
    bool bRes;

    if (SD_CheckDirExist((char *)DIRNAME_TEMP))
    {
        // Check GSM request temp file for file upload
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);

        if (SD_CheckFileExist(cFileName))
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                GSMprintf("\r\n> Number of pending upload file requests: %d", u16UnsendFileNum);

                if (u16UnsendFileNum > 10)
                {
                    u16UnsendFileNum = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);
                }
            }
            else
            {
                u16UnsendFileNum = 0;

                SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);
            }
        }
        else
        {
            u16UnsendFileNum = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
        }

        // Check GSM request temp file for sending packet
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_TEMP_FILE);

        if (SD_CheckFileExist(cFileName))
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                GSMprintf("\r\n> Number of unsent packets: %d", u16UnsendPkgNum[SOCKET_BANK_DEF]);

                if (u16UnsendPkgNum[SOCKET_BANK_DEF] > GSM_PACKET_NUM_MAX)
                {
                    u16UnsendPkgNum[SOCKET_BANK_DEF] = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);
                }
            }
            else
            {
                u16UnsendPkgNum[SOCKET_BANK_DEF] = 0;

                SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);
            }
        }
        else
        {
            u16UnsendPkgNum[SOCKET_BANK_DEF] = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
        }
    }
    else
    {
        SD_MakeDir((char *)DIRNAME_TEMP);

        RTOSprintf("\r\n> Create directory %s OK.", (char *)DIRNAME_TEMP);

        // Create GSM upload file request temp file
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_FILE_TEMP_FILE);

        u16UnsendFileNum = 0;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendFileNum, 2, 0);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK.", cFileName);

                break;
            }
        }

        // Create GSM send packet request temp file
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_TEMP_FILE);

        u16UnsendPkgNum[SOCKET_BANK_DEF] = 0;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendPkgNum, 2, 0);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK.", cFileName);

                break;
            }
        }
    }
}

//============================================================================//
// @brief  Record special GPRS Log.
// @param  pcLog: Log.
//         u16Len: Log length.
// @retval None.
//============================================================================//
void GPRS_DebugLog(char *pcLog, uint16_t u16Len)
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

    SD_AddBuffToFile((char *)FILENAME_GPRS_LOG_FILE, (uint8_t *)cDebugLog, strlen(cDebugLog));
}

#endif
