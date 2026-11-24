
//============================================================================//
// Includes
//============================================================================//
#include "GSM_QtEC21.h"

#include <stdlib.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "DataLog.h"
#include "GPS_Driver.h"
#include "HW_Config.h"
#include "main.h"
#include "MG82_Driver.h"
#include "Protocol.h"
#include "SD_Module.h"
#include "System.h"
#include "USB_Module.h"

//============================================================================//
// User Variables
//============================================================================//
#if ENABLE_GSM_QTEC21
GSM_ModuleTypeDef tGSM_ModuleType;

bool bQtEC21_State = TRUE;

bool bQtEC21_Ready = FALSE;

bool bGPS_Ready = FALSE;

bool bQtEC21_InitFinish = FALSE;

bool bGSMTimeOut = FALSE;

uint32_t u32GSM_ResetLastTick = 0;

uint8_t u8GSM_NoRespCnt = 0;

bool bGSM_PwrOnFlag = FALSE;
bool bGSM_PwrOffFlag = FALSE;

bool bGSM_ResetFlag = FALSE;
bool bGSM_ResetCheckFTP_Flag = FALSE;

GSM_SocketConnTypeDef tcpConnParamHandle;

TCP_PDP_ContextStruct tcp_pdpContext[TCP_PDP_NUM_MAX + 1]; // __attribute__ ((section("ccmram_var")));

TCP_SocketStruct tcpSocket[TCP_SOCKET_NUM_MAX + 1]; // __attribute__ ((section("ccmram_var")));

const uint8_t u8CurrPDP_ID = 1;

uint8_t u8CurrTCP_Socket[MAX_SOCKET_BANK] = {TCP_SOCKET_NUM_MAX};

char cGSM_CurrIP[20] = {0};

bool bTCPIP_Connect[MAX_SOCKET_BANK] = {FALSE};

bool bFTP_SetupFinish = FALSE;

bool bFTP_Connect = FALSE;

uint8_t u8TCP_RetryCnt[MAX_SOCKET_BANK] = {0};

uint8_t u8TCP_ConnectRetryCnt[MAX_SOCKET_BANK] = {0};

uint8_t u8FTP_RetryCnt = 0;

uint8_t u8FTP_AccessRetryCnt = 0;

uint8_t u8HTTP_RetryCnt = 0;

uint8_t u8SMS_List[GSM_BUFFER_SIZE] = {0}; // __attribute__ ((section("ccmram_var"))) = {0};

bool bQtEC21_Pwr = FALSE;

bool bQtEC21_Switch = FALSE;

GSM_CharSetTypeDef tQtEC21_CharSet = CHAR_SET_IRA;
#endif

//============================================================================//
// User Functions
//============================================================================//
#if ENABLE_GSM_QTEC21
// Power control functions //

//============================================================================//
// @brief  Enable QtEC21 module power.
// @param  None.
// @retval None.
//============================================================================//
void QtEC21_PwrEnable(void)
{
    GSMprintf("\r\n> ### GSM POWER ENABLE ###");

// HW_PBSet(GSM_PWR_EN);
#if ENABLE_MG82
    MG82_IO_Set(1, 1);
#endif

    HAL_Delay(500);

    QtEC21_TurnOnPwrKey();

    bQtEC21_Pwr = TRUE;

    u32GSM_ResetLastTick = HAL_GetTick();

    bQtEC21_State = TRUE;

    HAL_Delay(10);
}

//============================================================================//
// @brief  Disable QtEC21 module power.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_PwrDisable(void)
{
    bQtEC21_State = FALSE;

    bQtEC21_Ready = FALSE;

    bQtEC21_InitFinish = FALSE;

    if (bRTOS_En)
    {
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

        GSM_BlockCmdInit();

        GSM_NonBlockCmdInit();

        QtEC21_TCP_Init();

        QtEC21_CheckRedoFTP();

        // osMutexRelease(mutex_gsm_rx_user);
    }

    GSMprintf("\r\n> ### GSM POWER DISABLE ###");

// HW_PBClr(GSM_PWR_EN);
#if ENABLE_MG82
    MG82_IO_Set(1, 0);
#endif

    u32GSM_ResetLastTick = HAL_GetTick();

    bQtEC21_Pwr = FALSE;

    HAL_Delay(10);
}

// Software control functions //

//============================================================================//
// @brief  Send data to IC EC21 via UART.
// @param  pu8Buf: Pointer to the data buffer.
//         u8Len: Length of the data buffer.
// @retval True/False.
//============================================================================//
bool QtEC21_SendBuffer(uint8_t *pu8Buf, uint16_t u16Len)
{
    return UART_SendBuffer(GSM_COMPORT_DEF, pu8Buf, u16Len);
}

//============================================================================//
// @brief  Send command to IC EC21 via UART.
// @param  pcCmd: Pointer to the command char array.
// @retval True/False.
//============================================================================//
bool QtEC21_SendCMD(char *pcCmd)
{
    char cCommand[120];

    // Add header and terminate sequences to the message
    sprintf(cCommand, "%s%s%s", CMD_BEGIN, pcCmd, CMD_END);

    GSMprintf("\r\n> %s%s", CMD_BEGIN, pcCmd);

    return QtEC21_SendBuffer((uint8_t *)cCommand, (uint16_t)strlen(cCommand));
}

//============================================================================//
// @brief  Check if the QTEC21 module need to be reset.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_RetryCheck(void)
{
    if ((bQtEC21_Ready == FALSE) || (bGSM_PwrOffFlag == TRUE))
        return;

    u8GSM_NoRespCnt++;

    if (u8GSM_NoRespCnt >= GSM_NO_RESPONSE_CNT_MAX)
    {
        u8GSM_NoRespCnt = 0;

        bGSM_ResetCheckFTP_Flag = TRUE;
    }
}

//============================================================================//
// @brief  Initialize IC EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_Init(void)
{
    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    bQtEC21_State = TRUE;

    QtEC21_TurnOnPwrKey();
}

/*
//============================================================================//
// @brief  Check if the IC EC21 is initialized or not. If not, initialize it.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_InitCheck(void)
{
    uint16_t u16Pos;
    char cContent[1024];


    HAL_Delay(100);

    QtEC21_SendCMD(CMD_GMI);

    HAL_Delay(S_2_mS(1));

    if(!UART_RecvFind(GSM_COMPORT_DEF, (char *)"\r\nOK\r\n", &u16Pos))
    {
        if(UART_RecvFind(GSM_COMPORT_DEF, (char *)"\r\nRDY\r\n", &u16Pos))
        {
            bQtEC21_Ready = TRUE;

            //GSMprintf("\r\n> QT EC21 is ready to receive commands.");

            uint16_t u16Len = UART_RecvDataLen(GSM_COMPORT_DEF);

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16Pos, (uint8_t*)cContent, u16Len);

            cContent[u16Len] = 0;

            //GSMprintf("\r\n> QtEC21 response:\r\n%s\r\nQtEC21 response end.", cContent);

            return;
        }

        UART_ReInit(UART_PORT5, GSM_BAUDRATE_APPLY);

        // Enable only RXNE and ERR interrupt
        __HAL_UART_ENABLE_IT(puartHandle[UART_PORT5], UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(puartHandle[UART_PORT5], UART_IT_ERR);

        HAL_Delay(100);

        QtEC21_SendCMD((char*)"+GMI");

        HAL_Delay(S_2_mS(1));

        if(!UART_RecvFind(GSM_COMPORT_DEF, (char *)"\r\nOK\r\n", &u16Pos))
        {
            if(UART_RecvFind(GSM_COMPORT_DEF, (char *)"\r\nRDY\r\n", &u16Pos))
            {
                bQtEC21_Ready = TRUE;

                //GSMprintf("\r\n> QT EC21 is ready to receive commands.");

                uint16_t u16Len = UART_RecvDataLen(GSM_COMPORT_DEF);

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16Pos, (uint8_t*)cContent, u16Len);

                cContent[u16Len] = 0;

                //GSMprintf("\r\n> QtEC21 response:\r\n%s\r\nQtEC21 response end.", cContent);

                return;
            }

            UART_ReInit(UART_PORT5, GSM_BAUDRATE_DEFAULT);

            // Enable only RXNE and ERR interrupt
            __HAL_UART_ENABLE_IT(puartHandle[UART_PORT5], UART_IT_RXNE);
            __HAL_UART_ENABLE_IT(puartHandle[UART_PORT5], UART_IT_ERR);

            HAL_Delay(100);

            //GSMprintf("\r\n> Initialize QT EC21.");

            QtEC21_Init();
        }
        else
        {
            uint16_t u16Len = UART_RecvDataLen(GSM_COMPORT_DEF);

            UART_GetRecvData(GSM_COMPORT_DEF, (uint8_t*)cContent, u16Len);

            //GSMprintf("\r\n> QtEC21 response:\r\n%s\r\nQtEC21 response end.", cContent);

            bQtEC21_Ready = TRUE;

            //GSMprintf("\r\n> QT EC21 is ready to receive commands.");

            #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
            GPS_EnableRecvMsg();
            #endif
        }
    }
    else
    {
        uint16_t u16Len = UART_RecvDataLen(GSM_COMPORT_DEF);

        UART_GetRecvData(GSM_COMPORT_DEF, (uint8_t*)cContent, u16Len);

        //GSMprintf("\r\n> QtEC21 response:\r\n%s\r\nQtEC21 response end.", cContent);

        bQtEC21_Ready = TRUE;

        //GSMprintf("\r\n> QT EC21 is ready to receive commands.");

        #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
        GPS_EnableRecvMsg();
        #endif
    }
}

//============================================================================//
// @brief  Config the QtEC21 module after it is ready to receive commands.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool QtEC21_InitConfig(void)
{
    bool bRes;


    bRes = QtEC21_ConfigEcho(0);

    if(!bRes)
        return FALSE;

    HAL_Delay(100);

    bRes = QtEC21_SMS_SetFormat(1);

    if(!bRes)
        return FALSE;

    HAL_Delay(100);

    bRes = QtEC21_COM_SetBaudRate(GSM_BAUDRATE_APPLY);

    if(bRes)
    {
        UART_ReInit(GSM_COMPORT_DEF, GSM_BAUDRATE_APPLY);

        // Enable only RXNE and ERR interrupt
        __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_ERR);
    }
    else
        return FALSE;

  #if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    QtEC21_GPS_On();

    HAL_Delay(300);

    QtEC21_GPS_EnableRecvMsg();

    QtEC21_GPS_EnableSendMsg();
    #endif

    return TRUE;
}
*/

//============================================================================//
// @brief  Checking FTP related processes before reset GSM module.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_CheckRedoFTP(void)
{
    FuncSkip_Enable(FUNC_GSM_FTP, 10);

    if (procGSM_FOTA_Step != PROC_STEP_NULL)
    {
        procGSM_FOTA_Step = FOTA_STEP_NULL;

        g_Setting.tUpgrade.bFOTA_Flag = TRUE;

        System_SaveSetting();
    }

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
    if ((procGSM_Download_Step > DOWNLOAD_STEP_NULL) &&
        (procGSM_Download_Step < DOWNLOAD_STEP_REDO))
    {
        procGSM_Download_Step = DOWNLOAD_STEP_NULL;

        GSM_FTP_FileDownloadRequest(&gsmDownloadRequest);
    }
#endif

    if ((procGSM_Upload_Step > UPLOAD_STEP_NULL) &&
        (procGSM_Upload_Step < UPLOAD_STEP_REDO))
    {
        procGSM_Upload_Step = UPLOAD_STEP_NULL;

        GSM_FTP_FileUploadRequest(gsmUploadRequest.cSrcFile,
                                  gsmUploadRequest.cDstFile,
                                  gsmUploadRequest.cSrcDir,
                                  gsmUploadRequest.cDstDir,
                                  gsmUploadRequest.u8ServerID,
                                  gsmUploadRequest.u8ReUploadCnt);
    }

    if (procHTTP_Download_Step != HTTP_DOWNLOAD_STEP_NULL)
    {
        procHTTP_Download_Step = HTTP_DOWNLOAD_STEP_NULL;

        tHTTP_DlRequest.bRequest = TRUE;
    }

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
    if (procXTRA_Update_Step != XTRA_UPDATE_STEP_NULL)
    {
        procXTRA_Update_Step = XTRA_UPDATE_STEP_NULL;
    }
#endif
}

// Hardware control functions //

//============================================================================//
// @brief  Turn on EC21 using PWRKEY line.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_TurnOnPwrKey(void)
{
    GSMprintf("\r\n> ### GSM SWITCH ON ###");

    HAL_GPIO_WritePin(GSM_PWRKEY_GPIO_Port, GSM_PWRKEY_Pin, GPIO_PIN_SET);

    HAL_Delay(2100);

    HAL_GPIO_WritePin(GSM_PWRKEY_GPIO_Port, GSM_PWRKEY_Pin, GPIO_PIN_RESET);

    HAL_Delay(10);

    bQtEC21_Switch = TRUE;

    u32GSM_ResetLastTick = HAL_GetTick();

    bQtEC21_State = TRUE;
}

//============================================================================//
// @brief  Turn off EC21 using PWRKEY line.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_TurnOffPwrKey(void)
{
    bQtEC21_State = FALSE;

    bQtEC21_Ready = FALSE;

    bQtEC21_InitFinish = FALSE;

    if (bRTOS_En)
    {
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

        GSM_BlockCmdInit();

        GSM_NonBlockCmdInit();

        QtEC21_TCP_Init();

        QtEC21_CheckRedoFTP();

        // osMutexRelease(mutex_gsm_rx_user);
    }

    GSMprintf("\r\n> ### GSM SWITCH OFF ###");

    HAL_GPIO_WritePin(GSM_PWRKEY_GPIO_Port, GSM_PWRKEY_Pin, GPIO_PIN_SET);

    HAL_Delay(660);

    HAL_GPIO_WritePin(GSM_PWRKEY_GPIO_Port, GSM_PWRKEY_Pin, GPIO_PIN_RESET);

    HAL_Delay(10);

    u32GSM_ResetLastTick = HAL_GetTick();

    bQtEC21_Switch = FALSE;
}

//============================================================================//
// @brief  Turn off EC21 using command.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_TurnOffCmd(void)
{
    bQtEC21_State = FALSE;

    bQtEC21_Ready = FALSE;

    bQtEC21_InitFinish = FALSE;

    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    QtEC21_TCP_Init();

    u8GSM_NoRespCnt = 0;

    QtEC21_CheckRedoFTP();

    osTimerStop(timerRTOS_GSM_BlockHandle);
    osTimerStop(timerRTOS_GSM_NonBlockHandle);

    bGSMTimeOut = TRUE;

    if (gsmCurrNonBlockCmd != QTEC21_NONBLOCK_NULL)
    {
        if (gsmCurrNonBlockCmd != QTEC21_SMS_SEND)
        {
            // osMessagePut(GSM_NonBlockCpltHandle, GSM_EVENT_NULL, 0);
            osMessagePut(GSM_NonBlockResHandle, ((uint16_t)gsmCurrNonBlockCmd << 8) | GSM_NONBLOCK_RES_NULL, 0);
        }

        gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;
    }

    if (bRTOS_En)
    {
        osMutexRelease(mutex_gsm_rx_user);
    }

    GSMprintf("\r\n> ### GSM TURN OFF CMD ###");

    QtEC21_SendCMD(CMD_QPOWD);

    bQtEC21_Switch = FALSE;

    HAL_Delay(10);
}

//============================================================================//
// @brief  Send QtEC21 to sleep.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_SetSleep(void)
{
#if ENABLE_MG82
    MG82_4G_DTR(0);
#endif
}

//============================================================================//
// @brief  Wake up QtEC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_WakeUp(void)
{
    HW_PBSet(GSM_WAKEUP);

    HAL_Delay(10);

    HW_PBClr(GSM_WAKEUP);

    bQtEC21_State = TRUE;

    QtEC21_Reset();
}

//============================================================================//
// @brief  Reset module EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_Reset(void)
{
    u32GSM_ResetLastTick = HAL_GetTick();

    bQtEC21_State = TRUE;

    bQtEC21_Ready = FALSE;

    bQtEC21_InitFinish = FALSE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    bGPS_Ready = FALSE;
#endif

    if (bRTOS_En)
    {
        osTimerStop(timerRTOS_GSM_BlockHandle);
        osTimerStop(timerRTOS_GSM_NonBlockHandle);

        bGSMTimeOut = TRUE;

        // if(gsmCurrNonBlockCmd != QTEC21_NONBLOCK_NULL)
        //{
        //	if(gsmCurrNonBlockCmd != QTEC21_SMS_SEND)
        //	{
        //		osMessagePut(GSM_NonBlockResHandle, ((uint16_t)gsmCurrNonBlockCmd << 8) | GSM_NONBLOCK_RES_NULL, 0);
        //	}

        //	gsmCurrNonBlockCmd = QTEC21_NONBLOCK_NULL;
        //}

        GSM_BlockCmdInit();

        GSM_NonBlockCmdInit();

        QtEC21_TCP_Init();

        // osMutexRelease(mutex_gsm_rx_user);
    }

    RTOSprintf("\r\n> ### GSM HARDWARE RESET ###");

    HAL_GPIO_WritePin(GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_SET);

    HAL_Delay(500);

    HAL_GPIO_WritePin(GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_RESET);

    HAL_Delay(500);

    __HAL_UART_DISABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_ERR);

    HAL_Delay(1);

    __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(puartHandle[GSM_COMPORT_DEF], UART_IT_ERR);
}

//============================================================================//
// @brief  Reset IC EC21 with checking FTP service in advance.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_ResetWithCheckFTP(void)
{
    QtEC21_CheckRedoFTP();

    QtEC21_Reset();
}

// General functions

//============================================================================//
// @brief  Send AT command to check ME response.
// @param  None
// @retval True: The ME response normally.
//         False: No normal response from ME.
//============================================================================//
bool QtEC21_AT_Test(void)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cSignal3[] = "\r\nAT\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT" command
    QtEC21_SendCMD((char *)"");

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT response:\r\n%s\r\n> QtEC21 response end.", cContent);

            // If find \r\nAT\r\n
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
            {
                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT echo:\r\n%s\r\n> QtEC21 echo end.", cContent);
            }

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT response:\r\n%s\r\n> QtEC21 response end.", cContent);

            // If find \r\nAT\r\n
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
            {
                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT echo:\r\n%s\r\n> QtEC21 echo end.", cContent);
            }

            return FALSE;
        }
        else
        {
            // GSMprintf("\r\n> QtEC21 AT no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        // GSMprintf("\r\n> QtEC21 AT no response.", cContent);

        // QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Read the CGMI.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool QtEC21_ReadCGMI(char *pcCGMI)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGMI" command
    QtEC21_SendCMD(CMD_CGMI);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGMI response:\r\n%s\r\nQtEC21 response end.", cContent);

            memcpy(pcCGMI, cContent, u16DatLen - 6);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGMI response:\r\n%s\r\nQtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CGMI no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGMI no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Read the CGMM.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool QtEC21_ReadCGMM(char *pcCGMM)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGMM" command
    QtEC21_SendCMD(CMD_CGMM);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGMM response:\r\n%s\r\n QtEC21 response end.", cContent);

            memcpy(pcCGMM, cContent, u16DatLen - 6);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGMM response:\r\n%s\r\nQtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CGMM no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGMM no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Read the CGMR.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool QtEC21_ReadCGMR(char *pcCGMR)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGMR" command
    QtEC21_SendCMD(CMD_CGMR);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGMR response:\r\n%s\r\n QtEC21 response end.", cContent);

            memcpy(pcCGMR, cContent, u16DatLen - 6);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGMR response:\r\n%s\r\nQtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CGMR no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGMR no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Read the IMEI number.
// @param  pcIMEI: Pointer to a char array to store the IMEI number.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool QtEC21_GetIMEI(char *pcIMEI)
{
    bool bRes = FALSE;
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGSN" command
    QtEC21_SendCMD(CMD_CGSN);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos - u16StartPos - 2) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            // GSMprintf("\r\n> QtEC21 AT+CGSN response length: %d", u16DatLen);

            if (u16DatLen > 17)
            {
                u16StartPos = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos - 19) %
                              rbBuf[GSM_COMPORT_DEF].u16Size;
            }

            UART_PeekRecvData(GSM_COMPORT_DEF, u16StartPos + 2, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            StopChar(cContent, '\r');

            if (strlen(cContent) == 15)
            {
                strncpy(pcIMEI, cContent, 15);

                // GSMprintf("\r\n+IMEI: %s\r\n", pcIMEI);

                bRes = TRUE;
            }
            else
                bRes = FALSE;

            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGSN response:\r\n%s\r\nQtEC21 response end.", cContent);

            return bRes;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGSN response:\r\n%s\r\nQtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CGSN no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGSN no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_SetFunc(uint8_t u8Func, bool bRst)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d,%d", CMD_CFUN, u8Func, bRst);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CFUN" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CFUN response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CFUN response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CFUN no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CFUN no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Save the current parameters configuration to a profile in the memory.
// @param  u8ConfigID: ID to assign to the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_SaveConfigProf(uint8_t u8ConfigID)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_ATW, u8ConfigID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT&W" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT&W response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT&W response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT&W no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT&W no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set the parameters from a profile as the current parameters.
// @param  u8ConfigID: ID of the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_SetConfigProf(uint8_t u8ConfigID)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_ATZ, u8ConfigID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATZ" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 ATZ response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATZ response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 ATZ no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 ATZ no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set command echo mode.
// @param  bOption: 1: Command echo on.
//                  0: Command echo off.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool QtEC21_ConfigEcho(bool bOption)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_ATE, bOption);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATE" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 ATE response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATE response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 ATE no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 ATE no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_ConnCodeFormat(uint8_t u8Option)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_ATX, u8Option);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATX" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 ATX response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATX response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 ATX no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 ATX no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_TECharSet(char *pcCharSet)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_CSCS, pcCharSet);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CSCS" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CSCS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSCS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CSCS no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CSCS no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Save the current parameters configuration to a profile in the memory.
// @param  u8ConfigID: ID to assign to the profile.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_SetErrFormat(uint8_t u8Option)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_CMEE, u8Option);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMEE" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CMEE response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMEE response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CMEE no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CMEE no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_URC_Outport(char *pcOutPort)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"urcport\",\"%s\"", CMD_QURCCFG, pcOutPort);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QURCCFG" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QURCCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QURCCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QURCCFG no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QURCCFG no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// Serial interface functions

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
bool QtEC21_COM_SetDTR(uint8_t u8Option)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_AT_AND_D, u8Option);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT&D" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT&D response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT&D response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT&D no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT&D no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set the operating baud rate for the QtEC21.
// @param  u32BaudRate: The desired baudrate.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_COM_SetBaudRate(uint32_t u32BaudRate)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_IPR, u32BaudRate);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+IPR" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+IPR response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+IPR response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+IPR no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+IPR no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// Status control functions

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
bool QtEC21_Stat_ConfigRI(char *pcOption)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QCFG:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char cSignal4[] = "\r\n+CME ERROR:";
    char cSignal5[] = "\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"urc/ri/%s", CMD_QCFG, pcOption);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QCFG" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+QCFG:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
        } // If find "\r\nOK\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);
            }

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QCFG no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QCFG no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }

    return TRUE;
}

// SIM functions
//============================================================================//
// @brief  Get the IMSI number.
// @param  pcIMSI: Pointer to a char array to store the IMSI number.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GetIMSI(char *pcIMSI)
{
    bool bRes = FALSE;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CIMI" command
    QtEC21_SendCMD(CMD_SIM_CIMI);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 - u16StartPos - 2) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            // GSMprintf("\r\n> QtEC21 AT+CIMI response length: %d", u16DatLen);

            if (u16DatLen > 17)
            {
                u16StartPos = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 - 19) %
                              rbBuf[GSM_COMPORT_DEF].u16Size;
            }

            UART_PeekRecvData(GSM_COMPORT_DEF, u16StartPos + 2, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            StopChar(cContent, '\r');

            if (strlen(cContent) == 15)
            {
                strncpy(pcIMSI, cContent, 15);

                // GSMprintf("\r\n+IMSI: %s\r\n", pcIMSI);

                bRes = TRUE;
            }
            else
                bRes = FALSE;

            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 + 6 - u16StartPos) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CIMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return bRes;
        }
        // If find "\r\n+CME ERROR: "
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CIMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> QtEC21 AT+CIMI CME Error message incompleted");

                return FALSE;
            }
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CIMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CIMI no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CIMI no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Query SIM status.
// @param  pu8RetCode: Pointer to a unit8_t variable to store the returned code.
// @retval True: SIM is detected.
//         False: SIM is not detected/ locked.
//============================================================================//
bool QtEC21_SIM_QueryUSIM(GSM_SIM_StatTypedef *simRetCode)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CPIN:";
    char cSignal2[] = " READY";
    char cSignal3[] = " SIM PIN";
    char cSignal4[] = " SIM PUK";
    char cSignal5[] = "\r\nOK\r\n";
    char cSignal6[] = "\r\n+CME ERROR:";
    char cSignal7[] = "\r\n";
    char cSignal8[] = "\r\nERROR\r\n";
    GSM_SIM_StatTypedef simCode = GSM_SIM_OK;
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s?", CMD_SIM_CPIN);

    // Send "AT+CPIN" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_USIM_QUERY_TIMEOUT));

    while (!bGSMTimeOut)
    {
        uint16_t u16DatLen;

        // If found "\r\n+CPIN:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            // If found " READY"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                simCode = GSM_SIM_OK;
            }
            // If found " SIM PIN"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2, u16FindPos1))
            {
                simCode = GSM_SIM_PIN_REQ;
            }
            // If found " SIM PUK"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2, u16FindPos1))
            {
                simCode = GSM_SIM_PUK_REQ;
            }
            // If found unfamiliar return code
            else
            {
                simCode = GSM_SIM_UNCLEAR_ERR;
            }

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2, u16FindPos1))
            {
                // GSMprintf("\r\n> QtEC21 SIM card detected.");

                // Read out the message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            else
            {
                // Read out the message
                u16DatLen = 8;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                GSMprintf("\r\n> QtEC21 SIM card detection error.");

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
        }

        // If found "\r\n+CME ERROR: "
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            uint16_t u16PeekPos = (u16FindPos1 + 14) % rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_PeekRecvData(GSM_COMPORT_DEF, u16PeekPos, (uint8_t *)cContent, 3);

            if (simRetCode != NULL)
            {
                if (strncmp(cContent, (char *)"10", 2) == 0)
                {
                    *simRetCode = GSM_SIM_NOT_INSERTED;

                    GSMprintf("\r\n> QtEC21 SIM is not inserted!");
                }
                else
                {
                    *simRetCode = GSM_SIM_UNCLEAR_ERR;
                }
            }

            // If found "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal7, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                // Read out the current message
                u16DatLen = 14;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 CME Error message incompleted");

                return FALSE;
            }
        }
        // If found "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal8, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> QtEC21 response end.", cContent);

            if (simRetCode != NULL)
                *simRetCode = GSM_SIM_UNCLEAR_ERR;

            return FALSE;
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        if (simRetCode != NULL)
            *simRetCode = GSM_SIM_NOT_RESP;

        GSMprintf("\r\n> QtEC21 AT+CPIN no response.");

        return FALSE;
    }

    if (simCode == GSM_SIM_OK)
    {
        if (simRetCode != NULL)
            *simRetCode = GSM_SIM_OK;

        return TRUE;
    }
    else if (simCode == GSM_SIM_UNCLEAR_ERR)
    {
        if (simRetCode != NULL)
            *simRetCode = GSM_SIM_UNCLEAR_ERR;

        return FALSE;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    if (simCode == GSM_SIM_PIN_REQ)
    {
        // Check SIM PIN validity
        if (strlen(g_Setting.tGSM_SET.cSIM_PIN) != 4)
        {
            GSMprintf("\r\n> QtEC21 current SIM PIN is invalid:%s", g_Setting.tGSM_SET.cSIM_PIN);

            if (simRetCode != NULL)
                *simRetCode = GSM_SIM_PIN_REJECTED;

            return FALSE;
        }

        sprintf(cContent, "%s=%s", CMD_SIM_CPIN, g_Setting.tGSM_SET.cSIM_PIN);
    }
    else if (simCode == GSM_SIM_PUK_REQ)
    {
        // Check SIM PIN validity
        if (strlen(g_Setting.tGSM_SET.cSIM_PIN) != 4)
        {
            GSMprintf("\r\n> QtEC21 current SIM PIN is invalid:%s", g_Setting.tGSM_SET.cSIM_PIN);

            if (simRetCode != NULL)
                *simRetCode = GSM_SIM_PIN_REJECTED;

            return FALSE;
        }

        // Check SIM PUK validity
        if (strlen(g_Setting.tGSM_SET.cSIM_PUK) != 8)
        {
            GSMprintf("\r\n> QtEC21 current SIM PUK is invalid:%s", g_Setting.tGSM_SET.cSIM_PUK);

            if (simRetCode != NULL)
                *simRetCode = GSM_SIM_PUK_REJECTED;

            return FALSE;
        }

        sprintf(cContent,
                "%s=\"%s\",\"%s\"",
                CMD_SIM_CPIN,
                g_Setting.tGSM_SET.cSIM_PUK,
                g_Setting.tGSM_SET.cSIM_PIN);
    }
    else
    {
        if (simRetCode != NULL)
            *simRetCode = GSM_SIM_UNCLEAR_ERR;

        return FALSE;
    }

    // Send USIM enter code command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_USIM_QUERY_TIMEOUT));

    while (!bGSMTimeOut)
    {
        uint16_t u16DatLen;

        // If found "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            // If found "\r\n+CPIN:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos2, u16FindPos1))
            {
                uint16_t u16FindPos3;

                HAL_Delay(10);

                // If found " READY"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos3, u16FindPos2))
                {
                    // GSMprintf("\r\n> GSM SIM is ready.");

                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    simCode = GSM_SIM_OK;
                }
                // If found unfamiliar return code
                else
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 8 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    if (simCode == GSM_SIM_PIN_REQ)
                    {
                        GSMprintf("\r\n> QtEC21 SIM PIN is rejected.");

                        simCode = GSM_SIM_PIN_REJECTED;
                    }
                    else if (simCode == GSM_SIM_PUK_REQ)
                    {
                        GSMprintf("\r\n> QtEC21 SIM PUK is rejected.");

                        simCode = GSM_SIM_PUK_REJECTED;
                    }
                    else
                    {
                        simCode = GSM_SIM_UNCLEAR_ERR;
                    }
                }
            }
            else
            {
                GSMprintf("\r\n> QtEC21 SIM enter code no response.");

                u16DatLen = 6;

                simCode = GSM_SIM_NOT_RESP;
            }

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> LE910 response end.", cContent);

            break;
        }
        // If found "\r\n+CME ERROR: "
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            uint16_t u16PeekPos = (u16FindPos1 + 14) % rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_PeekRecvData(GSM_COMPORT_DEF, u16PeekPos, (uint8_t *)cContent, 3);

            if (simRetCode != NULL)
            {
                if (strncmp(cContent, (char *)"10", 2) == 0)
                {
                    *simRetCode = GSM_SIM_NOT_INSERTED;

                    // GSMprintf("\r\n> QtEC21 SIM is not inserted!");
                }
                else
                {
                    *simRetCode = GSM_SIM_UNCLEAR_ERR;
                }
            }

            // If found "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal7, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> LE910 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                // Read out the current message
                u16DatLen = 14;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 CME Error message incompleted");

                return FALSE;
            }
        }
        // If found "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal8, &u16FindPos1, u16StartPos))
        {
            // Stop the timeout timer
            osTimerStop(timerRTOS_GSM_BlockHandle);

            u8GSM_NoRespCnt = 0;

            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPIN response:\r\n%s\r\n> LE910 response end.", cContent);

            if (simRetCode != NULL)
                *simRetCode = GSM_SIM_UNCLEAR_ERR;

            return FALSE;
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        if (simRetCode != NULL)
            *simRetCode = GSM_SIM_NOT_RESP;

        GSMprintf("\r\n> QtEC21 AT+CPIN no response.");

        return FALSE;
    }

    if (simRetCode != NULL)
    {
        *simRetCode = simCode;
    }

    if (simCode == GSM_SIM_OK)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Change SIM pin.
// @param  pcOldPIN: Current PIN.
//         pcNewPIN: New PIN.
// @retval True: The operation is successful.
//         False: The operation is fail.
//============================================================================//
bool QtEC21_SIM_SetPIN(char *pcOldPIN, char *pcNewPIN)
{
    // bool bRes = FALSE;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent,
            "%s=\"SC\",\"%s\",\"%s\"",
            CMD_SIM_CPWD,
            pcOldPIN,
            pcNewPIN);

    // Send "AT+CPWD" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CPWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR: "
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CPWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> CME Error message incompleted");

                return FALSE;
            }
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CPWD no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CPWD no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// Network functions

//============================================================================//
// @brief  Query network registration on CS domain status.
// @param  None
// @retval True: Network registered on CS domain.
//         False: Network not registered on CS domain.
//============================================================================//
bool QtEC21_Net_QueryCS(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CREG:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR: ";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint16_t u16DatLen;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send CS service query command
    QtEC21_SendCMD(CMD_NET_CREG);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // If find "\r\n+CREG:"
    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        HAL_Delay(10);

        uint16_t u16CheckPos = (u16FindPos1 + 11) % rbBuf[GSM_COMPORT_DEF].u16Size;

        // If the response indicating that the network has registered on the CS
        // service domain
        if ((rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '1') ||
            (rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '5'))
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
            {
                GSMprintf("\r\n> QtEC21 network CS registered.");

                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> QtEC21 AT+CREG detection response not completed.");

                // Read out the current message
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
        else // If the return code indicates that network not registered yet.
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
            {
                GSMprintf("\r\n> QtEC21 network CS unregistered.");

                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> QtEC21 AT+CREG response not completed.");

                // Read out the current message
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }
    // If find "\r\n+CME ERROR: "
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        HAL_Delay(10);

        // If find "\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                             (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
        {
            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else // If the found message is not completed
        {
            GSMprintf("\r\n> QtEC21 AT+CREG CME Error message incompleted");

            return FALSE;
        }
    }
    // If find "\r\nERROR\r\n"
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        u16DatLen = 9;

        // Get the current message
        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

        cContent[u16DatLen] = 0;

        GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return FALSE;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CREG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }

    // return TRUE;
}

//============================================================================//
// @brief  Query network registration on CS domain status.
// @param  None
// @retval True: Network registered on CS domain.
//         False: Network not registered on CS domain.
//============================================================================//
bool QtEC21_Net_QueryCS2(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CREG:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR: ";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_CREG_QUERY_TIMEOUT));

    while (!bGSMTimeOut)
    {
        uint16_t u16DatLen;

        u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

        // Send CS service query command
        QtEC21_SendCMD(CMD_NET_CREG);

        HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

        // If find "+CREG:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            uint16_t u16CheckPos = (u16FindPos1 + 11) % rbBuf[GSM_COMPORT_DEF].u16Size;

            // If the response indicating that the network has registered on the CS
            // service domain
            if ((rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '1') ||
                (rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '5'))
            {
                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    GSMprintf("\r\n> QtEC21 network CS registered.");

                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return TRUE;
                }
                else // If the found message is not completed
                {
                    GSMprintf("\r\n> QtEC21 AT+CREG response not completed.");

                    u16DatLen = 0;

                    return FALSE;
                }
            }
            else // If the return code indicates that network not registered yet.
            {
                GSMprintf("\r\n> QtEC21 network CS unregistered.");

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
                    u16DatLen = 6;
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+CREG response not completed.");

                    u16DatLen = 0;
                }
            }

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\n+CME ERROR: "
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);
            }
            else
            {
                GSMprintf("\r\n> QtEC21	AT+CREG CME Error message incompleted");
            }
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CREG response:\r\n%s\r\n> QtEC21 response end.", cContent);
        }

        HAL_Delay(490);
    }

    if (bGSMTimeOut)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Get signal quality report.
// @param  pu8RSSI: Pointer to the variable to store the RSSI value.
//         pu8Ber: Pointer to the variable to store the BER value.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_Net_GetCSQ(uint8_t *pu8RSSI, uint8_t *pu8Ber)
{
    bool bRes;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CSQ: ";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    char *pcPtr;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CSQ" command
    QtEC21_SendCMD(CMD_NET_CSQ);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CSQ: "
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CSQ response:\r\n%s\r\n> QtEC21 response end.", cContent);

                pcPtr = cContent;

                StopChar(pcPtr, ' ');

                pcPtr += strlen(pcPtr) + 1;

                StopChar(pcPtr, ',');

                // Get RSSI
                if (strlen(pcPtr) > 0)
                    *pu8RSSI = atoi(pcPtr);
                else
                {
                    GSMprintf("\r\n> GSM get RSSI FAIL");

                    GSMprintf("\r\n> QtEC21 AT+CSQ response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }

                pcPtr += strlen(pcPtr) + 1;

                StopChar(pcPtr, ' ');

                // Get Ber
                if (strlen(pcPtr) > 0)
                    *pu8Ber = atoi(pcPtr);
                else
                {
                    GSMprintf("\r\n> GSM get BER FAIL");

                    GSMprintf("\r\n> QtEC21 AT+CSQ response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }

                return TRUE;
            }
            else
            {
                u16DatLen = 0;

                bRes = FALSE;
            }
        }
        else
        {
            // If find "\r\n+CME ERROR:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 0;
            }
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
                u16DatLen = 9;
            else // If find no terminate sequence
            {
                u16FindPos1 = 0;

                u16DatLen = 0;
            }

            bRes = FALSE;
        }

        // Read out the current message
        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

        cContent[u16DatLen] = 0;

        GSMprintf("\r\n> QtEC21 AT+CSQ response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return bRes;
    }
    else
    {
        GSMprintf("\r\n> Not found CSQ response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_Net_GetTime(uint8_t u8Option, char *pcBuf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QLTS: ";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[150] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d", CMD_NET_QLTS, u8Option);

    // Send "AT+QLTS" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\+QLTS: "
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QLTS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                // Copy the positioning information to buffer
                memcpy(pcBuf, &(cContent[9]), u16DatLen - 9 - 6);

                // GSMprintf("\r\n> QLTS time: %s", pcBuf);

                return TRUE;
            }
            else
            {
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QLTS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QLTS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QLTS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QLTS no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QLTS no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// Data mode to command mode fucntion

//============================================================================//
// @brief  Change the operation mode from data to command.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_Dat_2_Cmd(void)
{
    HAL_StatusTypeDef halStat;
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    if (bRTOS_En)
    {
        osStatus osRes;

        // Acquire uart5 synchronization
        osRes = osMutexWait(mutex_uart_user[GSM_COMPORT_DEF], S_2_mS(1));

        if (osRes != osOK)
            return FALSE;
    }

    HAL_Delay(1100);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "+++" command
    halStat = HAL_UART_Transmit(puartHandle[GSM_COMPORT_DEF], (uint8_t *)CMD_DAT_2_CMD,
                                3, 0xFFFF);

    HAL_Delay(1100);

    if (bRTOS_En)
        // Release uart5 synchronization
        osMutexRelease(mutex_uart_user[GSM_COMPORT_DEF]);

    if (halStat != HAL_OK)
        return FALSE;

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 +++ response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 +++ response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
            return FALSE;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 +++ no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
// GPS functions //

//============================================================================//
// @brief  Set the outport for the NMEA message to UART port.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigOutport(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    if (tGSM_ModuleType == GSM_MODULE_EC200)
        sprintf(cContent, "%s,\"none\"", CMD_GNSS_CONFIG_OUTPORT);
    else
        sprintf(cContent, "%s,\"uartdebug\"", CMD_GNSS_CONFIG_OUTPORT);

    // Send "AT+QGPSCFG="outport"" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure enable to get the NMEA message through command or not.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigNMEA_Src(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="nmeasrc",1" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_NMEASRC);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the NMEA type for GPS.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigGPS_NMEA_Type(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="gpsnmeatype",11" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_GPSNMEATYPE);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // if find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the NMEA type for GLONASS.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigGLONASS_NMEA_Type(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    if (tGSM_ModuleType == GSM_MODULE_EC200)
        sprintf(cContent, "%s,1", CMD_GNSS_CONFIG_GLONASSNMEATYPE); // GSV
    else
        sprintf(cContent, "%s,3", CMD_GNSS_CONFIG_GLONASSNMEATYPE); // GSA, GSV

    // Send "AT+QGPSCFG="glonassnmeatype",0" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // if find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the NMEA type for Galileo.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigGalileo_NMEA_Type(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="galileonmeatype",0" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_GALILEONMEATYPE);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the NMEA type for Beidou.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigBeidou_NMEA_Type(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="beidounmeatype",0" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_BEIDOUNMEATYPE);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the GSV message extension.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigGSV_Ext(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="gsvextnmeatype",0" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_GSVEXTNMEATYPE);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the GNSS message types.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigGNSS(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    if (tGSM_ModuleType == GSM_MODULE_EC200)
        sprintf(cContent, "%s,4", CMD_GNSS_CONFIG_GNSSCONFIG); // GPS + GLONASS
    else
        sprintf(cContent, "%s,2", CMD_GNSS_CONFIG_GNSSCONFIG); // GPS + GLONASS + Beidou

    // Send "AT+QGPSCFG="gnssconfig",4" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the GPS automatic mode.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigAutoGPS(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSCFG="autogps",0" command
    QtEC21_SendCMD(CMD_GNSS_CONFIG_AUTOGPS);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSCFG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Delete GPS messages in storage.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_DelMsg(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSDEL=0" command
    QtEC21_SendCMD(CMD_GNSS_QGPSDEL);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSDEL no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSDEL no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Turn on GPS module.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_TurnOn(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPS=1,30,50,0,1" command
    QtEC21_SendCMD(CMD_GNSS_QGPS);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPS no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPS no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Turn off GPS module.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_TurnOff(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QGPSEND" command
    QtEC21_SendCMD(CMD_GNSS_QGPSEND);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSEND no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSEND no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Get the GPS positioning information.
// @param  pcBuf: Pointer to a char array to store the positioning information.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_GetLoc(char *pcBuf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QGPSLOC: ";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[150] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s%s%s", CMD_BEGIN, CMD_GNSS_QGPSLOC, CMD_END);

    // Send "AT+QGPSLOC" command
    // QtEC21_SendCMD(CMD_GNSS_QGPSLOC);
    QtEC21_SendBuffer((uint8_t *)cContent, (uint16_t)strlen(cContent));

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\+QGPSLOC: "
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QGPSLOC response:\r\n%s\r\n> QtEC21 response end.", cContent);

                // Copy the positioning information to buffer
                memcpy(pcBuf, &(cContent[12]), u16DatLen - 12 - 6);

                // GSMprintf("\r\n> GNSS loc: %s", pcBuf);

                return TRUE;
            }
            else
            {
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QGPSLOC response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                if (strncmp(cContent, (char *)"\r\n+CME ERROR: 516", 17) == 0)
                {
                    // GSMprintf("\r\n> QtEC21 AT+QGPSLOC GNSS invalid");
                }
                else
                    GSMprintf("\r\n> QtEC21 AT+QGPSLOC response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSLOC response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSLOC no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSLOC no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Acquire NMEA message.
// @param  pcBuf: Pointer to a char array to store the NMEA message.
//         pcType: NMEA message type string (GGA, GSA, RMC, GSV, VTG, ...)
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_GetNMEA(char *pcBuf, char *pcType)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QGPSGNMEA: ";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[150] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s%s\"%s\"%s", CMD_BEGIN, CMD_GNSS_QGPSGNMEA, pcType, CMD_END);

    // Send "AT+QGPSGNMEA=" command
    QtEC21_SendBuffer((uint8_t *)cContent, (uint16_t)strlen(cContent));

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\+QGPSGNMEA: "
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                // Copy the positioning information to buffer
                memcpy(pcBuf, &(cContent[14]), u16DatLen - 14 - 6);

                // GSMprintf("\r\n> GNSS NMEA: %s", pcBuf);

                return TRUE;
            }
            else
            {
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSGNMEA no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the XTRA service.
// @param  bEnable: 0: Disable XTRA service.
//                  1: Enable XTRA service.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_ConfigXTRA(bool bEnable)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s%d", CMD_GNSS_QGPSXTRA, bEnable);

    // Send "AT+QGPSXTRA" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSXTRA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSXTRA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSXTRA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSXTRA no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSXTRA no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set GPS XTRA time.
// @param  pcTimeStamp: GPS time stamp in UTC.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_XTRA_Time(char *pcTimeStamp)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s0,\"%s\",1,1,3500", CMD_GNSS_QGPSXTRATIME, pcTimeStamp);

    // Send "AT+QGPSXTRATIME=" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSXTRATIME response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSXTRATIME response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSXTRATIME response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSXTRATIME no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSXTRATIME no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Active GPS XTRA function.
// @param  pcFileName: File name to inject GPS XTRA data file.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_XTRA_Data(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s\"%s\"", CMD_GNSS_QGPSXTRADATA, pcFileName);

    // Send "AT+QGPSXTRADATA=" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(2000);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Query GPS XTRA function status.
// @param  pu16DurTime: Pointer to a variable to store the returned XTRA file
//                      duration time.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_GPS_XTRA_DataQuery(uint16_t *pu16DurTime, time_t *pttValidTime)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QGPSXTRADATA:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR: ";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint16_t u16DatLen;
    struct tm tmDateTime = {0};
    uint32_t u32Year, u32Month, u32Day, u32Hour, u32Minute, u32Second;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send AT+QGPSXTRADATA? command
    QtEC21_SendCMD(CMD_GNSS_QGPSXTRADATA_QUERY);

    HAL_Delay(1200);

    // If find "\r\n+QGPSXTRADATA:"
    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        HAL_Delay(10);

        // If the response indicating that the network has registered on the CS
        // service domain
        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
        {
            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            char *pcPtr = cContent + 17;

            StopChar(pcPtr, ',');

            *pu16DurTime = atoi(pcPtr);

            pcPtr += strlen(pcPtr) + 2;

            StopChar(pcPtr, '\"');

            if (sscanf(pcPtr, "%d/%d/%d,%d:%d:%d", &u32Year, &u32Month, &u32Day, &u32Hour, &u32Minute, &u32Second) != 6)
            {
                return FALSE;
            }

            tmDateTime.tm_year = u32Year - 1900;
            tmDateTime.tm_mon = u32Month - 1;
            tmDateTime.tm_mday = u32Day;
            tmDateTime.tm_hour = u32Hour;
            tmDateTime.tm_min = u32Minute;
            tmDateTime.tm_sec = u32Second;

            *pttValidTime = mktime(&tmDateTime);

            return TRUE;
        }
        else // If the found message is not completed
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA detection response not completed.");

            // Read out the current message
            u16DatLen = 12;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
    }
    // If find "\r\nOK\r\n"
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        u16DatLen = 6;

        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

        cContent[u16DatLen] = 0;

        GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return FALSE;
    }
    // If find "\r\n+CME ERROR: "
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        HAL_Delay(10);

        // If find "\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                             (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
        {
            // Read out the current message
            u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                        rbBuf[GSM_COMPORT_DEF].u16Size;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else // If the found message is not completed
        {
            GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA CME Error message incompleted");

            return FALSE;
        }
    }
    // If find "\r\nERROR\r\n"
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        u16DatLen = 9;

        // Get the current message
        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

        cContent[u16DatLen] = 0;

        GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return FALSE;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QGPSXTRADATA no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Configure the GPS module.
// @param  None.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
void QtEC21_GPS_Config(void)
{
    // Turn off QGPSXTRA service
    QtEC21_SendCMD(CMD_GNSS_QGPSXTRA);

    HAL_Delay(310);

    // Config NMEA messages output port to UART debug port
    // QtEC21_SendCMD(CMD_GNSS_CONFIG_OUTPORT);

    // HAL_Delay(310);

    QtEC21_SendCMD(CMD_GNSS_CONFIG_NMEASRC);

    HAL_Delay(310);

    // Config to send type of NMEA messages
    QtEC21_SendCMD(CMD_GNSS_CONFIG_GPSNMEATYPE);

    HAL_Delay(310);

    QtEC21_SendCMD(CMD_GNSS_CONFIG_GLONASSNMEATYPE);

    HAL_Delay(310);

    // Config to turn off GLONASS, Beidou, Galileo
    QtEC21_SendCMD(CMD_GNSS_CONFIG_GNSSCONFIG);

    HAL_Delay(310);

    // Config to disable automatic GNSS
    QtEC21_SendCMD(CMD_GNSS_CONFIG_AUTOGPS);

    HAL_Delay(310);
}

//============================================================================//
// @brief  Enable sending messages from GPS module via UART debug of the IC EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_GPS_EnableSendMsg(void)
{
    // Config NMEA messages output port to UART debug port
    QtEC21_SendCMD(CMD_GNSS_CONFIG_OUTPORT);

    // u32GPS_IntTick = HAL_GetTick();

    // bGPS_Ready = TRUE;
}

//============================================================================//
// @brief  Enable receive messages from GPS module via UART debug of the IC EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_GPS_EnableRecvMsg(void)
{
#if GNSS_ENABLE_GET_LOC == 0
    bool bRXNE_En;

    // Enable RXNE interrupt
    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);

    // Enable ERR interrupt
    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

    for (int i = 0; i < 3; i++)
    {
        bRXNE_En = __HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);

        // RTOSprintf("\r\n> UART%d RXNE En: %d", GPS_COMPORT_DEF + 1, bRXNE_En);

        if (bRXNE_En == 0)
        {
            // RTOSprintf("\r\n> Restart UART%d RXNE EN", uartPort + 1);

            __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
        }
        else
            break;
    }

    if (bRXNE_En == 0)
    {
        // RTOSprintf("\r\n> SET GPS COMPORT RXNE INTERRUPT ERROR!");
    }
    else
    {
        // RTOSprintf("\r\n> SET GPS COMPORT RXNE INTERRUPT OK!");
    }
#endif
}

//============================================================================//
// @brief  Enable GPS module of the IC EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_GPS_On(void)
{
    // Config the GNSS module
    QtEC21_GPS_Config();

    // Turn on GNSS module
    QtEC21_SendCMD(CMD_GNSS_QGPS);
}

//============================================================================//
// @brief  Disable GPS module of the IC EC21.
// @param  None
// @retval None.
//============================================================================//
void QtEC21_GPS_Off(void)
{
    // Turn off GNSS module
    QtEC21_SendCMD(CMD_GNSS_QGPSEND);

    bGPS_Ready = FALSE;
}
#endif

// SMS Functions

//============================================================================//
// @brief  Select message service.
// @param  u8Type: Type of message service. Value: 0/1.
//                 Refer to the AT commands datasheet for the meaning of the
//                 options.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_SelectServ(uint8_t u8Type)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CSMS:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CMS ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_SMS_CSMS, u8Type);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CSMS" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CSMS:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            }
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 8;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CSMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CMS ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 13;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CSMS no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CSMS no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set the input and output format of the short messages.
// @param  u8Type: Type of format.
//                 0: PDU mode.
//                 1: Text mode.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_SetFormat(uint8_t u8Type)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_SMS_CMGF, u8Type);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGF" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CMGF response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMGF response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CMGF no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CMGF no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set the service center address.
// @param  pcSCA: Pointer to the service center address as a string.
//         u16Type: Type of service center address.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_SCA(char *pcSCA, uint16_t u16Type)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"+%s\"%d", CMD_SMS_CSCA, pcSCA, u16Type);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CSCA" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CSCA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 13;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSCA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSCA response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CSCA no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CSCA no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_SMS_SetStorage(char *pcMem1, char *pcMem2, char *pcMem3)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CPMS:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CMS ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\",\"%s\",\"%s\"", CMD_SMS_CPMS, pcMem1, pcMem2, pcMem3);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CPMS" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CPMS:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            }
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 8;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CPMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CMS ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 13;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CPMS response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CPMS no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CPMS no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
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
bool QtEC21_SMS_Del(uint16_t u16Idx, uint8_t u8DelFlag)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CMS ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d,%d", CMD_SMS_CMGD, u16Idx, u8DelFlag);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGD" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(1000);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CMGD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 13;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMGD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMGD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CMGD no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CMGD no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Get the list of the messages stored in the preferred message storage.
// @param  pcStat: Pointer to the option for the type of messages listed.
//         pu8Buf: Pointer to the buffer storing message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_GetList(char *pcStat, uint8_t *pu8Buf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CMGL:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CMS ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint32_t u32StartTime;
    uint16_t u32TimeOut = 2000;

    sprintf(cContent, "%s=\"%s\"", CMD_SMS_CMGL, pcStat);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGL" command
    QtEC21_SendCMD(cContent);

    u32StartTime = HAL_GetTick();

    while (HAL_GetTick() < (u32StartTime + u32TimeOut))
    {
        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // GSMprintf("\r\n> Receive %d bytes from GSM port", u16DatLen);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\n+CMGL:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(100);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Buf, u16DatLen);

                    pu8Buf[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGL message list:\r\n%s\r\n> Message list end.", (char *)pu8Buf);

                    return TRUE;
                }
                else
                {
                    u16DatLen = 8;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Buf, u16DatLen);

                    pu8Buf[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+CMGL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
            // If find "\r\nOK\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 6;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CMGL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CMS ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGL response:\r\n%s\r\n> QtEC21 response end.", cContent);
                }
                else
                {
                    u16DatLen = 13;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGL response:\r\n%s\r\n> QtEC21 response end.", cContent);
                }

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 9;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CMGL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }

        HAL_Delay(1);
    }

    GSMprintf("\r\n> QtEC21 AT+CMGL no response.");

    QtEC21_RetryCheck();

    return FALSE;
}

//============================================================================//
// @brief  Read a message from the preferred message storage.
// @param  u8Idx: Index of the message to read.
//         pu8Buf: Pointer to the buffer to store the read message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_Read(uint16_t u16Idx, uint8_t *pu8Buf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CMGR:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CMS ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_SMS_CMGR, u16Idx);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGR" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // GSMprintf("Receive %d bytes from GSM port", u16DatLen);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CMGR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Buf, u16DatLen);

                pu8Buf[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CMGR message content:\r\n%s\r\n> Message content end.", (char *)pu8Buf);

                return TRUE;
            }
            else
                return FALSE;

        } // If find "\r\nOK\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            uint16_t u16StrLen = strlen((char *)pu8Buf);

            if (u16DatLen > u16StrLen)
                u16DatLen = u16StrLen;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMGR response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\n+CMS ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CMGR response:\r\n%s\r\n> QtEC21 response end.", cContent);
            }

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CMGR response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CMGR no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CMGR no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }

    // return TRUE;
}

//============================================================================//
// @brief  Send a short message.
// @param  pcDstNbr: Pointer to ther destination phone number as a string.
//         u16Type: Type of service center address.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_Send2(GSM_MsgDataTypeDef *pgsmMsgData)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n> ";
    // char cSignal2[] = "\r\n+CMGS:";
    // char cSignal3[] = "\r\nOK\r\n";
    char cSignal4[] = "\r\n+CMS ERROR:";
    char cSignal5[] = "\r\n";
    char cSignal6[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint16_t u16DatLen;

    sprintf(cContent, "%s=\"%s\"", CMD_SMS_CMGS, pgsmMsgData->pcPhoneNum);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGS" command
    QtEC21_SendCMD(cContent);

    uint16_t u16Cnt = 0;

    // Check in 5 seconds
    while (u16Cnt < 500)
    {
        // Check response
        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\n> " then the module is ready to receive the message content
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 4;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CMS ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Read out the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);
                }

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }

        u16Cnt++;

        HAL_Delay(10);
    }

    if (u16Cnt >= 500)
    {
        GSMprintf("\r\n> QtEC21 AT+CMGS no response.");

        return FALSE;
    }

    // u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send the message content
    QtEC21_SendBuffer((uint8_t *)pgsmMsgData->pcData, pgsmMsgData->u16DataLen);

    GSMprintf("\r\n> QtEC21 AT+CMGS send SMS:\r\n%s\r\n> GSM message end.", (char *)(pgsmMsgData->pcData));

    // uint8_t u8Cmd = SEND_MSG_CMD;

    // Send "Ctrl+Z" signal to indicate the end of the message content
    QtEC21_SendBuffer((uint8_t *)SEND_MSG_CMD, strlen(SEND_MSG_CMD));

    // GSMprintf("\r\n> %x", u8Cmd);

    return TRUE;
}

//============================================================================//
// @brief  Send a short message.
// @param  pcDstNbr: Pointer to ther destination phone number as a string.
//         u16Type: Type of service center address.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_Send(GSM_MsgDataTypeDef *pgsmMsgData)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n> ";
    char cSignal2[] = "\r\n+CMGS:";
    char cSignal3[] = "\r\nOK\r\n";
    char cSignal4[] = "\r\n+CMS ERROR:";
    char cSignal5[] = "\r\n";
    char cSignal6[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint16_t u16DatLen;

    sprintf(cContent, "%s=\"%s\"", CMD_SMS_CMGS, pgsmMsgData->pcPhoneNum);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CMGS" command
    QtEC21_SendCMD(cContent);

    uint16_t u16Cnt = 0;

    // Check in 5 seconds
    while (u16Cnt < 500)
    {
        // Check response
        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\n> " then the module is ready to receive the message content
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 4;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CMS ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Read out the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);
                }

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }

        u16Cnt++;

        HAL_Delay(10);
    }

    if (u16Cnt >= 500)
    {
        GSMprintf("\r\n> QtEC21 AT+CMGS no response.");

        return FALSE;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send the message content
    QtEC21_SendBuffer((uint8_t *)pgsmMsgData->pcData, pgsmMsgData->u16DataLen);

    GSMprintf("\r\n> QtEC21 AT+CMGS send SMS:\r\n%s\r\n> GSM message end.", (char *)(pgsmMsgData->pcData));

    // uint8_t u8Cmd = SEND_MSG_CMD;

    // Send "Ctrl+Z" signal to indicate the end of the message content
    QtEC21_SendBuffer((uint8_t *)SEND_MSG_CMD, strlen(SEND_MSG_CMD));

    // GSMprintf("\r\n> %x", u8Cmd);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    // osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_SMS_SEND_TIMEOUT));
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        // Check response
        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "+CMGS:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(10);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2, u16FindPos1))
                {
                    // GSMprintf("\r\n> SMS sent successfullly.");

                    // Read out the message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);
                }

                return TRUE;
            }
            else
            {
                // If find "\r\n+CMS ERROR:"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    HAL_Delay(20);

                    // If find "\r\n"
                    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                         (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Read out the current message
                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);
                    }

                    return FALSE;
                }
                // If find "\r\nERROR\r\n"
                else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    u16DatLen = 9;

                    // Read out the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+CMGS response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
        }

        HAL_Delay(10);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+CMGS no response.");

        return FALSE;
    }

    return TRUE;
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
bool QtEC21_SMS_ReportConfig(uint8_t u8Mode, uint8_t u8Mt, uint8_t u8Bm,
                             uint8_t u8Ds, uint8_t u8Bfr)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CMS ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d,%d,%d,%d,%d", CMD_SMS_CNMI, u8Mode, u8Mt, u8Bm, u8Ds, u8Bfr);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CNMI" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CNMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CMS ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            else
            {
                u16FindPos1 = 0;

                u16DatLen = 13;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CNMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CNMI response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CNMI no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CNMI no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set including option for SMS text mode parameter.
// @param  u8Mode: Text mode parameter option.
//                 0: Not show additional parameters in result code.
//                 1: Show additional parameters in result code.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_SetParam(uint8_t u8Mode)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_SMS_CSDH, u8Mode);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CSDH" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CSDH response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CSDH response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CSDH no response.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CSDH no response.", cContent);

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Parse a formatted received message.
// @param  pcMsg: pcMsg: Pointer to the message for parsing.
//         pcPhoneNbr: Pointer to the variable to store the originating phone
//         number.
//         pcTime: Pointer to the variable to store the receiving time stamp.
//         pcContent: Pointer to the variable to store the content of the message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_Parse(char *pcMsg, char *pcPhoneNbr, char *pcTime, char *pcContent)
{
    char cSignal1[] = "REC READ";
    char cSignal2[] = "REC UNREAD";
    char cBuf[500] = {0};
    char *pcPtr;

    strncpy(cBuf, pcMsg, strlen(pcMsg));

    // GSMprintf("\r\n> Parse SMS content:%s", cBuf);

    pcPtr = (char *)cBuf;

    StopChar(pcPtr, '\"');

    // Get Stat
    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, '\"');

    // Check Stat
    if (strcmp(pcPtr, cSignal1) && strcmp(pcPtr, cSignal2))
    {
        GSMprintf("\r\n> QtEC21 SMS status incorrect.");

        return FALSE;
    }

    // Get phone number
    pcPtr += strlen(pcPtr) + 3;

    StopChar(pcPtr, '\"');

    if (strlen(pcPtr) >= 9)
        strcpy(pcPhoneNbr, pcPtr);
    else
    {
        GSMprintf("\r\n> QtEC21 SMS sender phone number wrong.");

        return FALSE;
    }

    // Get receiving time
    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, '\"');

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, '\"');

    // Store receiving time
    if (strlen(pcPtr) >= 19)
    {
        strncpy(pcTime, pcPtr, 17);
    }
    else
    {
        GSMprintf("\r\n> QtEC21 SMS receiving time wrong.");

        return FALSE;
    }

    // Get message length
    pcPtr += strlen(pcPtr) + 2;

    for (int i = 0; i < 6; i++)
    {
        StopChar(pcPtr, ',');

        pcPtr += strlen(pcPtr) + 1;
    }

    StopChar(pcPtr, '\r');

    uint16_t u16MsgLen;

    // Save message length
    if (strlen(pcPtr) > 0)
    {
        u16MsgLen = atoi(pcPtr);

        // GSMprintf("\r\n> QtEC21 SMS length:%d", u16MsgLen);
    }
    else
    {
        // GSMprintf("\r\n QtEC21 SMS length wrong.");

        return FALSE;
    }

    // Get message content
    pcPtr += strlen(pcPtr) + 2;

    if (strncmp(pcPtr, (char *)"1111", 4) != 0)
    {
        GSMprintf("\r\n> QtEC21 SMS command incorrect format: %s", pcPtr);

        return FALSE;
    }

    // Save message content
    if (strlen(pcPtr) >= (u16MsgLen + 6))
    {
        strncpy(pcContent, pcPtr, u16MsgLen);

        pcContent[u16MsgLen] = 0;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 SMS content length wrong.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the first unread message from the memory if there is at least
//         one unread message in the memory.
// @param  pcPhoneNbr: Pointer to the variable to store the originating phone
//         number.
//         pcTime: Pointer to the variable to store the receiving time stamp.
//         pcContent: Pointer to the variable to store the content of the message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_GetUnread(uint16_t *pu16MsgIdx, char *pcPhoneNbr,
                          char *pcTime, char *pcContent)
{
    char cSignal1[] = "+CMGL:";
    char cSignal2[] = "REC UNREAD";
    char *pcPtr;
    uint16_t u16FindPos;
    bool bRes;

    memset(u8SMS_List, 0x00, sizeof(u8SMS_List));

    // pcPtr = RTOSmalloc(2000);
    pcPtr = (char *)u8SMS_List;

    // if(pcPtr == NULL)
    //	return FALSE;

    bRes = QtEC21_SMS_GetList(cSignal2, (uint8_t *)pcPtr);

    if (!bRes)
    {
        // RTOSfree(pcPtr);

        // pcPtr = NULL;

        return FALSE;
    }

    // If find "+CMGL:"
    if (FindSequence(pcPtr, cSignal1, sizeof(u8SMS_List), 6, &u16FindPos))
    {
        pcPtr += u16FindPos + 7;

        StopChar(pcPtr, ',');

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) > 0)
        {
            *pu16MsgIdx = atoi(pcPtr);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, '\"');

        // Check stat
        if (strcmp(pcPtr, cSignal2) != 0)
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get phone number
        pcPtr += strlen(pcPtr) + 3;

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) >= 9)
            strcpy(pcPhoneNbr, pcPtr);
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get receiving time
        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        // Store receiving time
        if (strlen(pcPtr) >= 19)
        {
            strncpy(pcTime, pcPtr, 17);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get message length
        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, ',');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\r');

        uint16_t u16MsgLen;

        // Save message length
        if (strlen(pcPtr) > 0)
        {
            u16MsgLen = atoi(pcPtr);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get message content
        pcPtr += strlen(pcPtr) + 2;

        // Save message content
        strncpy(pcContent, pcPtr, u16MsgLen);

        pcContent[u16MsgLen] = 0;
    }
    else
    {
        // RTOSfree(pcPtr);

        // pcPtr = NULL;

        return FALSE;
    }

    // RTOSfree(pcPtr);

    // pcPtr = NULL;

    return TRUE;
}

//============================================================================//
// @brief  Get the first read message from the memory if there is at least one
//         read message in the memory.
// @param  pcPhoneNbr: Pointer to the variable to store the originating phone
//         number.
//         pcTime: Pointer to the variable to store the receiving time stamp.
//         pcContent: Pointer to the variable to store the content of the message.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_SMS_GetRead(uint16_t *pu16MsgIdx, char *pcPhoneNbr,
                        char *pcTime, char *pcContent)
{
    char cSignal1[] = "+CMGL:";
    char cSignal2[] = "REC READ";
    char *pcPtr;
    uint16_t u16FindPos;
    bool bRes;

    memset(u8SMS_List, 0x00, sizeof(u8SMS_List));

    // pcPtr = RTOSmalloc(2000);
    pcPtr = (char *)u8SMS_List;

    // if(pcPtr == NULL)
    //	return FALSE;

    bRes = QtEC21_SMS_GetList(cSignal2, (uint8_t *)pcPtr);

    if (!bRes)
    {
        // RTOSfree(pcPtr);

        // pcPtr = NULL;

        return FALSE;
    }

    // If find "+CMGL:"
    if (FindSequence(pcPtr, cSignal1, sizeof(u8SMS_List), 6, &u16FindPos))
    {
        pcPtr += u16FindPos + 7;

        StopChar(pcPtr, ',');

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) > 0)
        {
            *pu16MsgIdx = atoi(pcPtr);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, '\"');

        // Check stat
        if (strcmp(pcPtr, cSignal2) != 0)
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get phone number
        pcPtr += strlen(pcPtr) + 3;

        StopChar(pcPtr, '\"');

        if (strlen(pcPtr) >= 9)
            strcpy(pcPhoneNbr, pcPtr);
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // GSMprintf("\r\n> QtEC21 SMS sender phone number:%s", pcPhoneNbr);

        // Get receiving time
        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\"');

        // Store receiving time
        if (strlen(pcPtr) >= 19)
        {
            strncpy(pcTime, pcPtr, 17);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // GSMprintf("\r\n> QtEC21 SMS receiving time:%s", pcTime);

        // Get message length
        pcPtr += strlen(pcPtr) + 2;

        StopChar(pcPtr, ',');

        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, '\r');

        uint16_t u16MsgLen;

        // Save message length
        if (strlen(pcPtr) > 0)
        {
            u16MsgLen = atoi(pcPtr);
        }
        else
        {
            // RTOSfree(pcPtr);

            // pcPtr = NULL;

            return FALSE;
        }

        // Get message content
        pcPtr += strlen(pcPtr) + 2;

        // Save message content
        strncpy(pcContent, pcPtr, u16MsgLen);

        pcContent[u16MsgLen] = 0;
    }
    else
    {
        // RTOSfree(pcPtr);

        // pcPtr = NULL;

        return FALSE;
    }

    // RTOSfree(pcPtr);

    // pcPtr = NULL;

    return TRUE;
}

// Phone call functions //

//============================================================================//
// @brief  Connect to answer a phone call.
// @param  None
// @retval True: Connection is established successfully.
//         False: Connection is not established.
//============================================================================//
bool QtEC21_Call_Ans(void)
{
    // Send "ATA" command
    QtEC21_SendCMD(CMD_CALL_ATA);

    return TRUE;
}

bool QtEC21_Call_Ans2(void)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nCONNECT";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nNO CARRIER\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATA" command
    QtEC21_SendCMD(CMD_CALL_ATA);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_CALL_ANS_TIMEOUT));

    while (!bGSMTimeOut)
    {
        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\nCONNECT"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nOK\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the message
                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 ATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nNO CARRIER\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 14;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATA response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 ATA no response.", cContent);

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Dial and connect for a call.
// @param  pcPhoneNbr: Pointer to the destination phone number as a string.
// @retval Operation status in GSM_StatusTypeDef format.
//============================================================================//
GSM_StatusTypeDef QtEC21_Call_Dial(char *pcPhoneNbr)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nCONNECT";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nNO DIALTONE\r\n";
    char cSignal4[] = "\r\nBUSY\r\n";
    char cSignal5[] = "\r\nNO CARRIER\r\n";
    char cSignal6[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%s;", CMD_CALL_ATD, pcPhoneNbr);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATD" command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_CALL_DIAL_TIMEOUT));

    while (!bGSMTimeOut)
    {
        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\nCONNECT"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_OK;
            }
            // If find "\r\nOK\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the message
                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_OK;
            }
            // If find "\r\nNO DIALTONE\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 15;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_NO_DIALTONE;
            }
            // If find "\r\nBUSY\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 8;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_BUSY;
            }
            // If find "\r\nNO CARRIER\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 14;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_NO_CARRIER;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 ATD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return GSM_ERROR;
            }
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n QtEC21 ATD no response.");

        return GSM_ERROR;
    }

    return GSM_OK;
}

//============================================================================//
// @brief  Disconnect a call.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_Call_Disconn(void)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    ;
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATH" command
    // QtEC21_SendCMD(CMD_CALL_ATH);

    // Send "AT+CHUP" command
    QtEC21_SendCMD(CMD_CALL_CHUP);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_CALL_DISCONN_TIMEOUT));

    while (!bGSMTimeOut)
    {
        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the message
                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CHUP response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CHUP response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n QtEC21 AT+CHUP no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Change the operation mode from command to data.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_Call_Cmd_2_Dat(void)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nCONNECT";
    char cSignal2[] = "\r\nNO CARRIER\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATO" command
    QtEC21_SendCMD(CMD_CALL_CMD_2_DAT);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nCONNECT"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATO response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nNO CARRIER\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 14;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATO response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATO response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n QtEC21 ATO no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n QtEC21 ATO no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Set the waiting time for a connection to be established.
// @param  u8Time: The amount of waiting time in seconds.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_Call_ConnWaitTime(uint8_t u8Time)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_CALL_ATS7, u8Time);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "ATS7" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 ATS7 response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 ATS7 response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 ATS7 no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 ATS7 no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Get the list of the current calls.
// @param  pu8Buf: Pointer to the buffer to store the call list.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_Call_GetList(uint8_t *pu8Buf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n[+CLCC:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CMS ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CLCC" command
    QtEC21_SendCMD(CMD_CALL_CLCC);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // GSMprintf("\r\n> Receive %d bytes from GSM port", u16DatLen);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CLCC:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Buf, u16DatLen);

                pu8Buf[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CLCC call list:\r\n%s\r\n> QtEC21 response end.", (char *)pu8Buf);

                return TRUE;
            }
        }
        // If find "\r\nOK\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Buf, u16DatLen);

            pu8Buf[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CLCC response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CMS ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;
            }
            else
                u16DatLen = 13;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CLCC response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CLCC response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CLCC no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CLCC no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Check if there is an incoming call by find the "RING" message.
// @param  None.
// @retval True: There is an incoming call.
//         False: There is no incoming call.
//============================================================================//
bool QtEC21_Call_CheckRing(void)
{
    uint16_t u16FindPos;
    char cSignal[] = "\r\nRING\r\n";
    char cContent[100];

    if (UART_RecvFind(GSM_COMPORT_DEF, cSignal, &u16FindPos))
    {
        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, 8);

        cContent[6] = 0;

        GSMprintf("\r\n> QtEC21 check RING response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  Check if there is an incoming call by check the call list.
// @param  None.
// @retval True: There is an incoming call.
//         False: There is no incoming call.
//============================================================================//
bool QtEC21_Call_CheckIncoming(void)
{
    uint16_t u16FindPos;
    char cSignal[] = "\r\n+CLCC:";
    char cContent[200];
    uint8_t u8Stat;
    char *pcPtr;
    bool bRes;

    bRes = QtEC21_Call_GetList((uint8_t *)cContent);

    if (!bRes)
        return FALSE;

    while (FindSequence(cContent, cSignal, 200, 8, &u16FindPos))
    {
        pcPtr = &(cContent[u16FindPos]);

        *pcPtr = 'X';

        for (int i = 0; i < 2; i++)
        {
            StopChar(pcPtr, ',');

            pcPtr += strlen(pcPtr) + 1;
        }

        StopChar(pcPtr, ',');

        if (strlen(pcPtr) == 1)
        {
            u8Stat = atoi(pcPtr);

            if (u8Stat == 4)
                return TRUE;
        }
        else
            return FALSE;
    }

    return FALSE;
}

// Packet domain functions

//============================================================================//
// @brief  Attach or detach the ME from the domain service.
// @param  bOption: 1: Attach the ME to the domain service.
//                  0: Detach the ME from the domain service.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_Attach(bool bOption)
{
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_PD_CGATT, bOption);

    // Send "AT+CGATT" command
    QtEC21_SendCMD(cContent);

    return TRUE;
}

//============================================================================//
// @brief  Define parameter values for a PDP context.
// @param  u8ID: ID of the PDP context.
//         pcAPN: Pointer to the access point name.
//         pcAddr: Address to identify the MT, can be NULL.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_ConfigPDP(uint8_t u8ID, char *pcAPN, char *pcAddr)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    if ((pcAddr == NULL) || (strlen(pcAddr) == 0))
        sprintf(cContent, "%s=%d,\"IP\",\"%s\"", CMD_PD_CGDCONT, u8ID, pcAPN);
    else
        sprintf(cContent,
                "%s=%d,\"IP\",\"%s\",\"%s\"",
                CMD_PD_CGDCONT,
                u8ID,
                pcAPN,
                pcAddr);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGDCONT" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGDCONT response:\r\n%s\r\n> LE910 response end.", cContent);

            return TRUE;
        }
        else
        {
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
                u16DatLen = 9;
            else // If find no terminate sequence
            {
                u16FindPos = 0;

                u16DatLen = 0;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGDCONT response:\r\n%s\r\n> LE910 response end.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGDCONT no response");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Specify the quality of service profile when a PDP context is activated.
// @param  u8ID: ID of the PDP context.
//         pdpInstance: Pointer to a TCP_PDP_ContextStruct instance.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_QualityReq(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent,
            "%s=%d,%d,%d,%d,%d,%d",
            CMD_PD_CGQREQ,
            u8ID,
            pdpInstance->u8Precedence,
            pdpInstance->u8Delay,
            pdpInstance->u8Reliability,
            pdpInstance->u8Peak,
            pdpInstance->u8Mean);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGQREQ" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGQREQ response:\r\n%s\r\n> LE910 response end.", cContent);

            return TRUE;
        }
        else
        {
            // If find "\r\n+CME ERROR:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(20);

                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                {
                    u16FindPos1 = 0;

                    u16DatLen = 0;
                }
            }
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
                u16DatLen = 9;
            else // If find no terminate sequence
            {
                u16FindPos1 = 0;

                u16DatLen = 0;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGQREQ response:\r\n%s\r\n> LE910 response end.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGQREQ no response");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Allow specify Quality of Service Profile that used when the terminal
//         send an Activate PDP Context request message to the network.
// @param  u8ID: ID of the PDP context.
//         pdpInstance: Pointer to a TCP_PDP_ContextStruct instance.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool LE910_PD_QualityMin(uint8_t u8ID, TCP_PDP_ContextStruct *pdpInstance)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent,
            "%s=%d,%d,%d,%d,%d,%d",
            CMD_PD_CGQMIN,
            u8ID,
            pdpInstance->u8Precedence,
            pdpInstance->u8Delay,
            pdpInstance->u8Reliability,
            pdpInstance->u8Peak,
            pdpInstance->u8Mean);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+CGQMIN" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGQMIN response:\r\n%s\r\n> LE910 response end.", cContent);

            return TRUE;
        }
        else
        {
            // If find "\r\n+CME ERROR:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                HAL_Delay(20);

                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                {
                    u16FindPos1 = 0;

                    u16DatLen = 0;
                }
            }
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
                u16DatLen = 9;
            else // If find no terminate sequence
            {
                u16FindPos1 = 0;

                u16DatLen = 0;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGQMIN response:\r\n%s\r\n> LE910 response end.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGQMIN no response");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Activate a PDP context.
// @param  u8ID: ID of the PDP context.
//         bOption: 1: Activate PDP context, 0: Deactivate PDP context.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_Activ(uint8_t u8ID, bool bOption)
{
    char cContent[100] = {0};

    sprintf(cContent,
            "%s=%d,%d",
            CMD_PD_CGACT,
            bOption,
            u8ID);

    // Send "AT+CGACT" command
    QtEC21_SendCMD(cContent);

    return TRUE;
}

//============================================================================//
// @brief  Get IP address of a PDP context.
// @param  u8ID: ID of the PDP context.
//         pcAddr: Pointer to a char array to store the returned address.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_GetAddr(uint8_t u8ID, char *pcAddr)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CGPADDR:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    char *pcPtr;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d", CMD_PD_CGPADDR, u8ID);

    // Send "AT+CGPADDR" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // GSMprintf("\r\n> Receive %d bytes from GSM port", u16DatLen);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\n+CGPADDR:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CGPADDR response:\r\n%s\r\n> LE910 response end.", cContent);

                pcPtr = cContent;

                StopChar(pcPtr, '\"');

                pcPtr += strlen(pcPtr) + 1;

                StopChar(pcPtr, '\"');

                if ((strlen(pcPtr) > 6) && (strlen(pcPtr) < 16))
                {
                    strcpy(pcAddr, pcPtr);

                    return TRUE;
                }
                else
                {
                    // GSMprintf("\r\n> QtEC21 CGPADDR retured address invalid.");

                    return FALSE;
                }
            }
        }
        // If find "\r\nOK\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 6;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+CGPADDR response:\r\n%s\r\n> LE910 response end.", cContent);

            return TRUE;
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CGPADDR response:\r\n%s\r\n> LE910 response end.", cContent);

                return FALSE;
            }
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGPADDR response:\r\n%s\r\n> LE910 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CGPADDR no response");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGPADDR no response");

        QtEC21_RetryCheck();

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Query network registration on PS domain status.
// @param  None.
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_QueryPS(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CGREG:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint16_t u16DatLen;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send PS service query command
    QtEC21_SendCMD(CMD_PD_CGREG);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    // If find "\r\n+CGREG:"
    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        HAL_Delay(10);

        uint16_t u16CheckPos = (u16FindPos1 + 12) % rbBuf[GSM_COMPORT_DEF].u16Size;

        // If the response indicating that the network has registered on the PS
        // service domain
        if ((rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '1') ||
            (rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '5'))
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                GSMprintf("\r\n> QtEC21 network PS registered.");

                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> QtEC21 AT+CGREG response not completed.");

                // Read out the current message
                u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
        }
        else // If the return code indicates that network not registered yet.
        {
            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
            {
                GSMprintf("\r\n> QtEC21 network PS unregistered.");

                // Read out the current message
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else // If the found message is not completed
            {
                GSMprintf("\r\n> QtEC21 AT+CGREG response not completed.");

                // Read out the current message
                u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }
    // If find "\r\nERROR\r\n"
    else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
    {
        u8GSM_NoRespCnt = 0;

        u16DatLen = 9;

        // Get the current message
        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

        cContent[u16DatLen] = 0;

        GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

        return FALSE;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+CGREG no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }

    // return TRUE;
}

//============================================================================//
// @brief  Query network registration on PS domain status.
// @param  None
// @retval True: Network registered on PS domain.
//         False: Network not registered on PS domain.
//============================================================================//
bool QtEC21_PD_QueryPS2(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CGREG:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(GSM_CGREG_QUERY_TIMEOUT));

    while (!bGSMTimeOut)
    {
        uint16_t u16DatLen;

        u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

        // Send PS service query command
        QtEC21_SendCMD(CMD_PD_CGREG);

        HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

        // If find "\r\n+CGREG:"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(10);

            uint16_t u16CheckPos = (u16FindPos1 + 12) % rbBuf[GSM_COMPORT_DEF].u16Size;

            // If the response indicating that the network has registered on the PS
            // service domain
            if ((rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '1') ||
                (rbBuf[GSM_COMPORT_DEF].pu8Buf[u16CheckPos] == '5'))
            {
                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    GSMprintf("\r\n> QtEC21 network PS registered.");

                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return TRUE;
                }
                else // If the found message is not completed
                {
                    GSMprintf("\r\n> QtEC21 network PS detection error.");

                    u16DatLen = 0;
                }
            }
            else // If the return code indicates that network not registered yet.
            {
                GSMprintf("\r\n> QtEC21 network PS unregistered.");

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
                    u16DatLen = 6;
                else
                {
                    GSMprintf("\r\n> QtEC21 network PS detection error.");

                    u16DatLen = 0;
                }
            }

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            u8GSM_NoRespCnt = 0;

            u16DatLen = 9;

            // Get the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CGREG response:\r\n%s\r\n> QtEC21 response end.", cContent);
        }

        HAL_Delay(490);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+CGREG no response.");

        return FALSE;
    }

    return TRUE;
}

// Hardware functions

//============================================================================//
// @brief  Obtain the 4G module's RTC time.
// @param  pcBuf: Pointer to a char array to store the returned time.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_HW_GetRTC(char *pcBuf)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+CCLK: ";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[150] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s", CMD_HW_CCLK);

    // Send "AT+CCLK" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_GPS_RESPONSE_TIMEOUT);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\+CCLK: "
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(10);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CCLK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                // Copy the positioning information to buffer
                memcpy(pcBuf, &(cContent[9]), u16DatLen - 9 - 6);

                // GSMprintf("\r\n> CCLK time: %s", pcBuf);

                return TRUE;
            }
            else
            {
                u16DatLen = 12;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+CCLK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
        // If find "\r\n+CME ERROR:"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
        {
            HAL_Delay(20);

            // If find "\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                 (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+CCLK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+CCLK response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
        {
            GSMprintf("\r\n> QtEC21 AT+CCLK no response.");

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QLTS no response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// TCP/IP functions //

//============================================================================//
// @brief  Init for TCP/IP function.
// @param  None.
// @retval None.
//============================================================================//
void QtEC21_TCP_Init(void)
{
    for (int i = 1; i <= TCP_PDP_NUM_MAX; i++)
    {
        tcp_pdpContext[i].bActiv = FALSE;

        tcp_pdpContext[i].bInUse = FALSE;
    }

    u8PDP_ActRetryCnt = 0;

    bPDP_DeactFlag = FALSE;

    for (int i = 0; i < TCP_SOCKET_NUM_MAX; i++)
    {
        tcpSocket[i].bConnect = FALSE;
    }

    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        u8CurrTCP_Socket[i] = TCP_SOCKET_NUM_MAX;

        bTCPIP_Connect[i] = FALSE;

        bTCP_CloseFlag[i] = FALSE;

        u8TCP_RetryCnt[i] = 0;

        u8TCP_ConnectRetryCnt[i] = 0;

        procGSM_TCP_Step[i] = CONN_STEP_NULL;
    }

    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        gsmTCP_DATA[i].u16DataLen = 0;

        gsmTCP_LOG[i].u16DataLen = 0;
    }

    bFTP_SetupFinish = FALSE;

    bFTP_Connect = FALSE;

    u8FTP_RetryCnt = 0;

    u8FTP_AccessRetryCnt = 0;

    // char cLog[20] = {0};

    // sprintf(cLog, "QtEC21 TCP Init");

    // GPRS_DebugLog(cLog, strlen(cLog));
}

//============================================================================//
// @brief  Config PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
//         pcAPN: Pointer to the APN name.
//         pcUser_1_Name: Pointer to the user name.
//         pcPwd: Pointer to the password.
// @retval True: The PDP context configured successfully.
//         False: The PDP context configuration fail.
//============================================================================//
bool QtEC21_TCP_ConfigPDP(uint8_t u8PDP_ID, char *pcAPN, char *pcUser_1_Name, char *pcPwd)
{
    uint16_t u16StartPos, u16FindPos;
    char cContent[50];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";

    // Create PDP context config command
    sprintf(cContent, "%s=%d,%d,\"%s\",\"%s\",\"%s\",%d", CMD_TCP_QICSGP, u8PDP_ID,
            TCP_PDP_TYPE, pcAPN, pcUser_1_Name, pcPwd, TCP_PDP_AUTH);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send AT+QICSGP command
    QtEC21_SendCMD(cContent);

    HAL_Delay(QTEC21_COMM_RESPONSE_TIMEOUT);

    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QICSGP response:\r\n%s\r\n> LE910 response end.", cContent);

            return TRUE;
        }
        else
        {
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
                u16DatLen = 9;
            else // If find no terminate sequence
            {
                u16FindPos = 0;

                u16DatLen = 0;
            }

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QICSGP response:\r\n%s\r\n> LE910 response end.", cContent);

            return FALSE;
        }
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QICSGP no response");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

//============================================================================//
// @brief  Config QoS.
// @param  None.
// @retval True: The QoS configured successfully.
//         False: The QoS configuration fail.
//============================================================================//
bool QtEC21_TCP_ConfigQoS(void)
{
    return TRUE;
}

//============================================================================//
// @brief  Activate a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
// @retval True: The PDP context is activated successfully.
//         False: The PDP context activation fail.
//============================================================================//
bool QtEC21_TCP_ActivPDP(uint8_t u8PDP_ID)
{
    char cContent[50];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    uint16_t u16StartPos, u16FindPos;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d", CMD_TCP_QIACT, u8PDP_ID);

    // Send AT+QIACT command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(TCP_PDP_ACTIV_TIMEOUT));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QIACT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QIACT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QIACT no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Get IP of a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
//         pcIP_Addr: Pointer to a char array to store the acquired IP.
// @retval True: PDP context IP is retrieved successfully.
//         False: PDP context IP retrieval fail.
//============================================================================//
bool QtEC21_TCP_GetIP(uint8_t u8PDP_ID, char *pcIP_Addr)
{
    bool bRes;
    char cContent[50];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2, u16FindPos3;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s?", CMD_TCP_QIACT);

    // Send AT+QIACT? command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(TCP_PDP_ACTIV_TIMEOUT));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            uint8_t u8TimeOut = 100;

            u8GSM_NoRespCnt = 0;

            while (u8TimeOut--)
            {
                bRes = UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos);

                if (bRes)
                    break;

                HAL_Delay(1);
            }

            // If find "\r\nOK\r\n"
            if (bRes)
            {
                sprintf(cContent, "%s %d", STR_RESPONSE_QIACT, u8PDP_ID);

                // If find the header sequence for the PDP context of the current PDP ID
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cContent, &u16FindPos2, u16StartPos))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 + 6 - u16FindPos2) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos2, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QIACT? response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent;

                    StopChar(pcPtr, ',');

                    // Get into context state field
                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, ',');

                    if (strlen(pcPtr) > 0)
                    {
                        uint8_t u8ActivState = atoi(pcPtr);

                        if (u8ActivState == 1)
                        {
                            pcPtr += strlen(pcPtr) + 1;

                            StopChar(pcPtr, ',');

                            // Get into IP address field
                            pcPtr += strlen(pcPtr) + 1;

                            // Get the assigned IP address
                            if (*pcPtr == '"')
                            {
                                pcPtr++;

                                StopChar(pcPtr, '"');

                                if (strlen(pcPtr) >= 7)
                                {
                                    strcpy(pcIP_Addr, pcPtr);

                                    // GSMprintf("\r\n> Assigned IP:%s", pcIP_Addr);

                                    bRes = TRUE;
                                }
                                else
                                    bRes = FALSE;
                            }
                            else
                                bRes = FALSE;
                        }
                        else
                            bRes = FALSE;
                    }
                    else
                        bRes = FALSE;

                    if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)STR_RESPONSE_QIACT, &u16FindPos3, u16StartPos))
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 + 6 - u16FindPos3) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Get the current message
                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos3, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n> QtEC21 AT+QIACT? response:\r\n%s\r\n> QtEC21 response end.", cContent);
                    }

                    return bRes;
                }
                else
                {
                    if (UART_RecvFindPos(GSM_COMPORT_DEF, (char *)CMD_TCP_QIACT, &u16FindPos3, u16StartPos))
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos1 + 6 - u16FindPos3) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Get the current message
                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos3, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n> QtEC21 AT+QIACT? response:\r\n%s\r\n> QtEC21 response end.", cContent);
                    }
                    else
                    {
                        u16DatLen = 6;

                        // Get the current message
                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n> QtEC21 AT+QIACT? response:\r\n%s\r\n> QtEC21 response end.", cContent);
                    }

                    return FALSE;
                }
            }
            // If find "\r\nERROR\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QIACT? response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QIACT? no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Deactivate a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~16).
// @retval True: The PDP context is deactivated successfully.
//         False: The PDP context deactivation fail.
//============================================================================//
bool QtEC21_TCP_DeactivPDP(uint8_t u8PDP_ID)
{
    char cContent[50];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    uint16_t u16StartPos, u16FindPos;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d", CMD_TCP_QIDEACT, u8PDP_ID);

    // Send AT+QIDEACT command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(TCP_PDP_DEACTIV_TIMEOUT));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the rinf buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QIDEACT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QIDEACT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QIDEACT no response.");

        return FALSE;
    }

    return TRUE;
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
bool QtEC21_TCP_OpenConn(uint8_t u8PDP_ID, uint8_t u8ConnID,
                         GSM_SocketConnTypeDef *pgsmConnHandle)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100];

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Create the connection open command
    sprintf(cContent, "%s=%d,%d,\"%s\",\"%s\",%d,%d,%d",
            CMD_TCP_QIOPEN, u8PDP_ID, u8ConnID, pgsmConnHandle->cServType, pgsmConnHandle->cRemoteAddr,
            pgsmConnHandle->u16RemotePort, pgsmConnHandle->u16LocalPort, pgsmConnHandle->u8ConnMode);

    // Send the connection open command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(10);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> AT+QIOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> AT+QIOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> AT+QIOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    // if(bGSMTimeOut)
    //	return TRUE;

    return TRUE;
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
bool QtEC21_TCP_OpenConn2(uint8_t u8PDP_ID, uint8_t u8ConnID,
                          GSM_SocketConnTypeDef gsmConnHandle)
{
    bool bRes;
    char cContent[100];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n";
    char cSignal3[] = "\r\nCONNECT\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2, u16FindPos3;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Create the connection open command
    sprintf(cContent, "%s=%d,%d,\"%s\",\"%s\",%d,%d,%d",
            CMD_TCP_QIOPEN, u8PDP_ID, u8ConnID, gsmConnHandle.cServType, gsmConnHandle.cRemoteAddr,
            gsmConnHandle.u16RemotePort, gsmConnHandle.u16LocalPort, gsmConnHandle.u8ConnMode);

    // Send the connection open command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(TCP_OPEN_CONN_TIMEOUT));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                sprintf(cContent, "%s:", CMD_TCP_QIOPEN);

                // If find the header sequence for result indicating
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cContent, &u16FindPos2, u16StartPos))
                {
                    // Stop the timeout timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos2);

                    // Get the preview data
                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos2, (uint8_t *)cContent, u16DatLen);

                    pcPtr = (char *)cContent;

                    StopChar(pcPtr, ',');

                    pcPtr += strlen(pcPtr) + 1;

                    // If the result code indicates the operation is successful
                    if (*pcPtr == '0')
                        bRes = TRUE;
                    else
                        bRes = FALSE;

                    // If find "\r\n"
                    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos3,
                                         (u16FindPos2 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        // Read out the current message
                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n%s", cContent);
                    }

                    return bRes;
                }
            }

            // If find "\r\nCONNECT\r\n" for transparent access mode
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n%s", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n%s", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Close a connection of a PDP context.
// @param  u8PDP_ID: ID of the PDP context (1~11).
// @retval True: Connection is closed successfully.
//         False: Connection close fail.
//============================================================================//
void QtEC21_TCP_CloseConn2(uint8_t u8SocketID, uint16_t u16TimeOut)
{
    char cContent[50];

    sprintf(cContent, "%s=%d,%d", CMD_TCP_QICLOSE, u8SocketID, u16TimeOut);

    // Send connection close command
    QtEC21_SendCMD(cContent);
}

//============================================================================//
// @brief  Close a connection of a PDP context.
// @param  u8SocketID: ID of the socket (0~11).
//         u16TimeOut: Timeout value (Second).
// @retval True: Connection is closed successfully.
//         False: Connection close fail.
//============================================================================//
bool QtEC21_TCP_CloseConn(uint8_t u8SocketID, uint16_t u16TimeOut)
{
    char cContent[50];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+QIOPEN: ";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    uint16_t u16StartPos, u16FindPos1, u16FindPos2, u16FindPos3;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,%d", CMD_TCP_QICLOSE, u8SocketID, u16TimeOut);

    // Send connection close command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(u16TimeOut + 2));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(50);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QICLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                HAL_Delay(100);

                // If find "\r\n+QIOPEN: "
                if (UART_RecvFind(GSM_COMPORT_DEF, cSignal2, &u16FindPos2))
                {
                    HAL_Delay(50);

                    // If find "\r\n"
                    if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos3, u16FindPos2))
                    {
                        uint16_t u16RetCode;
                        char *pcPtr;

                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 2 - u16FindPos2) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos2, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        // GSMprintf("\r\n> QtEC21 AT+QICLOSE response 2:\r\n%s\r\n> QtEC21 response end.", cContent);

                        pcPtr = cContent;

                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr);

                        StopChar(pcPtr, '\r');

                        u16RetCode = atoi(pcPtr);

                        if ((u16RetCode == 0) || (u16RetCode == 567))
                        {
                            // GSMprintf("\r\n> QtEC21 AT+QICLOSE close connection socket %d OK", u8SocketID);

                            return TRUE;
                        }
                        else
                        {
                            GSMprintf("\r\n> QtEC21 AT+QICLOSE close connection socket %d ERROR %d", u8SocketID, u16RetCode);

                            return TRUE;
                        }
                    }
                    else
                    {
                        u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 11 - u16FindPos1) %
                                    rbBuf[GSM_COMPORT_DEF].u16Size;

                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                        cContent[u16DatLen] = 0;

                        GSMprintf("\r\n> QtEC21 AT+QICLOSE response incomplete:\r\n%s\r\n> QtEC21 response end.", cContent);

                        return TRUE;
                    }
                }
                else
                {
                    u16DatLen = 6;

                    // Read out the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QICLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return TRUE;
                }
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QICLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QICLOSE no response.");

        return FALSE;
    }

    return TRUE;
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
bool QtEC21_TCP_IsConnect(bool bQueryType, uint8_t u8ID, bool *pbIsConnect)
{
    char cContent[100];
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,%d", CMD_TCP_QISTATE, bQueryType, u8ID);

    // Send AT+QISTATE command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            sprintf(cContent, "\r\n%s: %d", CMD_TCP_QISTATE, u8ID);

            // If find the result indicating sequence for the current socket ID
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cContent, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos2, u16StartPos))
                {
                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QISTATE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent;

                    // Move to the socket state field
                    for (int i = 0; i < 5; i++)
                    {
                        StopChar(pcPtr, ',');

                        pcPtr += strlen(pcPtr) + 1;
                    }

                    // GSMprintf("\r\n> Connection status:%c", *pcPtr);

                    // If the socket state indicates that the connection is normal
                    if ((*pcPtr == '1') || (*pcPtr == '2') || (*pcPtr == '3'))
                        *pbIsConnect = TRUE;
                    else
                        *pbIsConnect = FALSE;

                    return TRUE;
                }
                else
                    return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QISTATE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QISTATE no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Send data through a socket.
// @param  u8SocketID: Socket ID.
//         pu8Dat: Pointer to the data array.
//         u16Len: Length of the data to send.
// @retval True: Send successfully.
//         False: Send failed.
//============================================================================//
MsgSendStatusTypeDef QtEC21_TCP_Send(GSM_SocketDataTypeDef *pgsmSocketData)
{
    char cContent[1500];
    char cSignal1[] = "\r\n> ";
    char cSignal2[] = "\r\nSEND OK\r\n";
    char cSignal3[] = "\r\nSEND FAIL\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    uint16_t u16StartPos, u16FindPos1;
    uint16_t u16DatLen;

    if ((pgsmSocketData->u16DataLen == 0) || (pgsmSocketData->u16DataLen > 1460))
        return SEND_ERROR;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,%d", CMD_TCP_QISEND, pgsmSocketData->SocketNum, pgsmSocketData->u16DataLen);

    // Send AT+QISEND command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n> "
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the received message
                u16DatLen = 4;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the current message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return SEND_ERROR;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QISEND step 1 no response.");

        return SEND_ERROR;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send the data for transmitting
    UART_SendBuffer(GSM_COMPORT_DEF, (uint8_t *)(pgsmSocketData->pcData), pgsmSocketData->u16DataLen);

    // GSMprintf("\r\n> QtEC21 AT+QISEND message:\r\n");

    // GSMprintf("%s", (uint8_t*)(pgsmSocketData->cData));

    // GSMprintf("\r\n> QtEC21 AT+QISEND message end.");

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nSEND OK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                // Stop the timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return SEND_OK;
            }
            // If find "\r\nSEND FAIL\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // Stop the timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 13;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return SEND_FAIL;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                // Stop the timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Read out the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return SEND_ERROR;
            }
        }

        HAL_Delay(100);
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QISEND step 2 no response.");

        return SEND_ERROR;
    }

    return SEND_OK;
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
bool QtEC21_TCP_SendingStatus(uint8_t u8SocketID, uint16_t *pu16NACK_Bytes)
{
    char cContent[100];
    char cSignal1[] = "\r\n+QISEND:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,0", CMD_TCP_QISEND, u8SocketID);

    // Send AT+QISEND command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(2));

    while (!bGSMTimeOut)
    {
        HAL_Delay(30);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n+QISEND:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(10);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent;

                    // Go to the total number of bytes sent field
                    StopChar(pcPtr, ' ');

                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, ',');

                    if (strlen(pcPtr) > 0)
                    {
                        uint32_t u32SendBytes = atoi(pcPtr);

                        // GSMprintf("\r\n> QtEC21 AT+QISEND total number of bytes for sending:%d", u32SendBytes);
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QISEND not found total number of bytes for sending.");

                        return FALSE;
                    }

                    // Go to the number of acknowledged bytes field
                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, ',');

                    if (strlen(pcPtr) > 0)
                    {
                        // uint32_t u32ACK_Bytes = atoi(pcPtr);

                        // GSMprintf("\r\n> ACK bytes:%d", u32ACK_Bytes);
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QISEND not found number of bytes ACK.");

                        return FALSE;
                    }

                    // Go to the number of unacknowledged bytes field
                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        *pu16NACK_Bytes = atoi(pcPtr);

                        if (*pu16NACK_Bytes > 0)
                            GSMprintf("\r\n> QtEC21 AT+QISEND NACK bytes:%d\r\n", *pu16NACK_Bytes);
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QISEND not found number of NACK bytes .");

                        return FALSE;
                    }

                    return TRUE;
                }
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+QISEND sending status not found OK sequence.");

                    return FALSE;
                }
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the current message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QISEND response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QISEND sending status no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Check the number of TCP bytes received.
// @param  u8SocketID: Socket ID.
//         pu16RecvBytesNum: Pointer to the variable indicating the number of
//                           bytes received.
// @retval True: Operation success.
//         False: Operation failed.
//============================================================================//
bool QtEC21_TCP_RecvDataLen(uint8_t u8SocketID, uint16_t *pu16RecvBytesNum)
{
    char cContent[100];
    char cSignal1[] = "\r\n+QIRD:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    // uint16_t u16TotalBytes, u16ReadBytes,
    uint16_t u16UnreadBytes;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,0", CMD_TCP_QIRD, u8SocketID);

    // Send AT+QIRD command
    QtEC21_SendCMD(cContent);

    HAL_Delay(TCP_SOCKET_NUM_MAX);

    bGSMTimeOut = FALSE;

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nQIRD:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QIRD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent;

                    // Go to the total number of bytes received field
                    StopChar(pcPtr, ':');

                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, ',');

                    if (strlen(pcPtr) == 0)
                        return FALSE;

                    // Go to the number of bytes read field
                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, ',');

                    if (strlen(pcPtr) == 0)
                        return FALSE;

                    // Go to the number of bytes unread field
                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        u16UnreadBytes = atoi(pcPtr);

                        // GSMprintf("\r\n> Number of unread bytes:%d", u16UnreadBytes);

                        *pu16RecvBytesNum = u16UnreadBytes;
                    }
                    else
                        return FALSE;

                    return TRUE;
                }
                else
                    return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the current message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QIRD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QIRD no response.");

        return FALSE;
    }

    return TRUE;
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
bool QtEC21_TCP_GetRecvData(uint8_t u8SocketID, uint8_t *pu8Buf, uint16_t u16Len,
                            uint16_t *u16ReadLen)
{
    char cContent[1024];
    char cSignal1[] = "\r\n+QIRD:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char *pcPtr;
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    uint16_t u16TotalBytes;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    sprintf(cContent, "%s=%d,%d", CMD_TCP_QIRD, u8SocketID, u16Len);

    // Send AT+QIRD command
    QtEC21_SendCMD(cContent);

    bGSMTimeOut = FALSE;

    // Stop the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\n+QIRD:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16StartPos))
                {
                    // Read out the current message
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QIRD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent;

                    // Go to the number of actual bytes read field
                    StopChar(pcPtr, ' ');

                    pcPtr += strlen(pcPtr) + 1;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        u16TotalBytes = atoi(pcPtr);
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QIRD cannot get the number of bytes read field.");

                        return FALSE;
                    }

                    // If there is data read
                    if (u16TotalBytes > 0)
                    {
                        // Go to the data field
                        pcPtr += strlen(pcPtr) + 2;

                        StopChar(pcPtr, '\r');

                        // GSMprintf("\r\n> Data:%s", pcPtr);

                        // Check if the number of bytes read field is matched with the length of the data field
                        if (strlen(pcPtr) != u16TotalBytes)
                        {
                            GSMprintf("\r\n> QtEC21 AT+QIRD the number of bytes read and the received data length are not match.");

                            GSMprintf("\r\n> QtEC21 AT+QIRD number of bytes read:%d", u16TotalBytes);

                            GSMprintf("\r\n> QtEC21 AT+QIRD received data length:%d", strlen(pcPtr));

                            return FALSE;
                        }
                        else
                        {
                            // Get the received data
                            strncpy((char *)pu8Buf, pcPtr, u16TotalBytes);

                            *u16ReadLen = u16TotalBytes;

                            //*u16ReadLen = 0;

                            // while((*u16ReadLen < u16Len) && (*u16ReadLen < u16TotalBytes))
                            //{

                            //*pu8Buf = (uint8_t)(*pcPtr);

                            // pu8Buf++;
                            // pcPtr++;
                            //(*u16ReadLen)++;
                            // }

                            return TRUE;
                        }
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QIRD the number of bytes read is 0.");

                        return FALSE;
                    }
                }
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+QIRD cannot find OK message.");

                    return FALSE;
                }
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the current message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QIRD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QIRD no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Change the operation mode from command to data for TCP/IP functions.
// @param  None
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_TCP_Cmd_2_Dat(uint8_t u8SocketID)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nCONNECT\r\n";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d,2", CMD_TCP_QISWTMD, u8SocketID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QISWTMD" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(TCP_SOCKET_NUM_MAX);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nCONNECT\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the message
            u16DatLen = 11;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QISWTMD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nOK\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            u16DatLen = 6;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 AT+QISWTMD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos, u16StartPos))
        {
            u16DatLen = 9;

            // Read out the current message
            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 AT+QISWTMD response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
            return FALSE;
    }
    else
    {
        GSMprintf("\r\n> QtEC21 AT+QISWTMD response.");

        QtEC21_RetryCheck();

        return FALSE;
    }
}

// FTP funtions //

//============================================================================//
// @brief  Configure the PDP context for FTP.
// @param  u8PDP_ID: ID of the selected context.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgPDP(uint8_t u8PDP_ID)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_FTP_QFTPCFG_PDP_ID, u8PDP_ID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG = "contextid"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure the account for FTP.
// @param  pcUser_1_Name: Pointer to a char array storing the username.
//         pcPwd: Pointer to a char array storing the password.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgAcc(char *pcUserName, char *pcPwd)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,\"%s\",\"%s\"", CMD_FTP_QFTPCFG_ACC, pcUserName, pcPwd);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "account"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(FTP_ACC_CONFIG_TIMEOUT));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure the file to to binary.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgFileType(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "filetype"" command
    QtEC21_SendCMD(CMD_FTP_QFTPCFG_FILE_TYPE);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure the transfer mode.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgTransMode(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "transmode"" command
    QtEC21_SendCMD(CMD_FTP_QFTPCFG_TRANS_MODE);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure timeout value.
// @param  u16TimeOut: Timeout value.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgTimeout(uint16_t u16TimeOut)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_FTP_QFTPCFG_TIMEOUT, u16TimeOut);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "rsptimeout"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL mode.
// @param  bSSL_Type: 0: Disable SSL mode.
//                    1: Enable SSL mode.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgSSL_Type(bool bSSL_Type)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_FTP_QFTPCFG_SSL_TYPE, bSSL_Type);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "ssltype"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL context ID.
// @param  u8SSL_ID: Context ID for SSL, range of 0-5
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_CfgSSL_ID(uint8_t u8SSL_ID)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_FTP_QFTPCFG_SSL_ID, u8SSL_ID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCFG= "sslctxid"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL version.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_Ver: SSL version, range of 0-4.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_SSL_Ver(uint8_t u8SSL_ID, uint8_t u8SSL_Ver)
{
    uint16_t u16StartPos, u16FindPos1;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d,%d", CMD_FTP_QSSLCFG_VER, u8SSL_ID, u8SSL_Ver);
    // sprintf(cContent, "+QSSLCFG= \"negotiatetime\",%d,120", FTP_SSL_ID_DEF);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QSSLCFG= "sslversion"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QSSLCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL cipher suit.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_Cipher: Code for SSL cipher suit.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_SSL_Cipher(uint8_t u8SSL_ID, char *pcCipher)
{
    uint16_t u16StartPos, u16FindPos1;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d,%s", CMD_FTP_QSSLCFG_CIPHER, u8SSL_ID, pcCipher);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QSSLCFG = "sslciphersuite"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QSSLCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL path for CA certificate.
// @param  u8SSL_ID: Selected SSL context ID.
//         pcSSL_CA_Path: Pointer to the path for SSL CA.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_SSL_CA(uint8_t u8SSL_ID, char *pcSSL_CA_Path)
{
    uint16_t u16StartPos, u16FindPos1;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d,\"%s\"", CMD_FTP_QSSLCFG_CACERT, u8SSL_ID, pcSSL_CA_Path);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QSSLCFG= "cacert"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QSSLCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure SSL security level.
// @param  u8SSL_ID: Selected SSL context ID.
//         u8SSL_SecLV: SSL security level, range of 0-2.
//         See Quectel's EC2x SSL AT commands document.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_SSL_SecLV(uint8_t u8SSL_ID, uint8_t u8SSL_SecLV)
{
    uint16_t u16StartPos, u16FindPos1;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d,%d", CMD_FTP_QSSLCFG_SEC_LV, u8SSL_ID, u8SSL_SecLV);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QSSLCFG: "seclevel"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QSSLCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QSSLCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Login the FTP server.
// @param  pcHostName: Domain name of the server.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_Login(char *pcHostName, uint16_t u16Port)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\",%d", CMD_FTP_QFTPOPEN, pcHostName, u16Port);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPOPEN" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPOPEN no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Set the working directory on FTP server.
// @param  pcDirPath: Pointer to a char array storing the directory path.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_SetDir(char *pcDirPath)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_FTP_QFTPCWD, pcDirPath);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCWD" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCWD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCWD no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Make 1 or multiple directories on the FTP server.
// @param  pcDirPath: Pointer to a char array storing the directory path.
//         Any directory and sub-directory in the path which does not exist on
//         FTP server will be created.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_MakeDir(char *pcDirPath)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_FTP_QFTPMKDIR, pcDirPath);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPMKDIR" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPMKDIR response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPMKDIR response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPMKDIR response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPMKDIR no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Upload a file from the module local storage to the FTP server.
// @param  pcLocalName: Pointer to a char array to store the local name.
//         pcFileName: Pointer to a char array to store the name of the file
//         on server.
// @retval True: The operation is done successfully.
//         False: The operation is failed.
//============================================================================//
bool QtEC21_FTP_UploadFile(char *pcLocalName, char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\",\"%s%s\",0", CMD_FTP_QFTPPUT, pcFileName, FTP_LOCAL_STORAGE, pcLocalName);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPPUT" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPPUT no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Upload a file to the FTP server directly.
// @param  pu8DatBuf: Pointer to the data buffer.
//         pcUploadFile: Pointer to a char array storing the name of the file on
//         server.
//         u32UploadPos: Position to store the data from in the file on the server.
//         u16UploadLen: Size of data to upload to the server.
//         bUploadFinish: 0: This amount of data is not the last.
//                        1: This amount of data is the last.
// @retval True: The operation is done successfully.
//         False: The operation is failed.
//============================================================================//
bool QtEC21_FTP_UploadDirect(uint8_t *pu8DatBuf, char *pcUploadFile, uint32_t u32UploadPos,
                             uint16_t u16UploadLen, bool bUploadFinish)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nCONNECT\r\n";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    if (pu8DatBuf == NULL)
        return FALSE;

    sprintf(cContent, "%s=\"%s\",\"COM:\",%d,%d,%d",
            CMD_FTP_QFTPPUT,
            pcUploadFile,
            u32UploadPos,
            u16UploadLen,
            bUploadFinish);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPPUT" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(5);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(5);

            // If find "\r\nCONNECT\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPPUT no response.");

        return FALSE;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    QtEC21_SendBuffer(pu8DatBuf, u16UploadLen);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(5);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(5);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPPUT response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPPUT no response 2.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Download a file from the FTP server.
// @param  pcFileName: Pointer to a char array storing the name of the file to download.
//         pcLocalDest: Pointer to a char array storing the destination storage.
//         u32FilePtr: Start address of the content of the file to start download from.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_DownloadFile(char *pcFileName, char *pcLocalDest, uint32_t u32FilePtr)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\",\"%s\",%d", CMD_FTP_QFTPGET, pcFileName, pcLocalDest, u32FilePtr);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPGET" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPGET no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Download a file from the FTP server in transparent mode.
// @param  pcFileName: Pointer to a char array storing the file name.
//         u32FilePtr: Start address of the content of the file to start download from.
//         u32DownloadLen: The length of data to download.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_DownloadDirect(char *pcFileName, uint32_t u32FilePtr, uint32_t u32DownloadLen)
{
    return TRUE;
}

//============================================================================//
// @brief  Get the size of a file on the FTP server.
// @param  pcFileName: Pointer to a char array storing the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_FileSize(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_FTP_QFTPSIZE, pcFileName);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPSIZE" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPSIZE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPSIZE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPSIZE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPSIZE no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the latest modifying date of a file on server.
// @param  pcFileName: Pointer to a char array storing the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_FileDate(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_FTP_QFTPMDTM, pcFileName);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPMDTM" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPMDTM response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPMDTM response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPMDTM response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPMDTM no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the number of bytes have been transferred from server to module.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_TransLen(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPLEN" command
    QtEC21_SendCMD(CMD_FTP_QFTPLEN);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPLEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPLEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPLEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPLEN no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Delete a file on the FTP server.
// @param  pcFileName: Pointer to a char array to store the name of the file
//         on server.
// @retval True: The operation is done successfully.
//         False: The operation is failed.
//============================================================================//
bool QtEC21_FTP_FileDel(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    sprintf(cContent, "%s=\"%s\"", CMD_FTP_QFTPDEL, pcFileName);

    // Send "AT+QFTPDEL" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPDEL no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Logout FTP server.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FTP_Logout(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFTPCLOSE" command
    QtEC21_SendCMD(CMD_FTP_QFTPCLOSE);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFTPCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFTPCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFTPCLOSE no response.");

        return FALSE;
    }

    return TRUE;
}

// HTTP functions

//============================================================================//
// @brief  Configure the PDP context for HTTP.
// @param  u8PDP_ID: ID of the selected context.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_CfgPDP(uint8_t u8PDP_ID)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_HTTP_CFG_PDP_ID, u8PDP_ID);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPCFG = "contextid"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Configure the response header for HTTP.
// @param  bEnable: 0: Disable response header.
//                  1: Enable response header.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_RespHeader(bool bEnable)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s,%d", CMD_HTTP_CFG_RESP_HEADER, bEnable);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPCFG = "responseheader"" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPCFG response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPCFG no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Set the HTTP URL.
// @param  pcURL: URL to set.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_SetURL(char *pcURL)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nCONNECT\r\n";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d,%d",
            CMD_HTTP_URL,
            strlen(pcURL),
            5);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPURL" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(1);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(2);

            // If find "\r\nCONNECT\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPURL no response.");

        return FALSE;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    QtEC21_SendBuffer((uint8_t *)pcURL, strlen(pcURL));

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(1);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(2);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPURL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Send a GET request to an HTTP server.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_Get(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s%d", CMD_HTTP_GET, HTTP_FUNCTION_TIMEOUT);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPGET" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPGET response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPGET no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Read the data as the GET request response and store it into a file.
// @param  pcFileName: Path of the file to store the response data.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_ReadFile(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s\"%s\",%d",
            CMD_HTTP_READ_FILE,
            pcFileName,
            HTTP_FUNCTION_TIMEOUT);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPREADFILE" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPREADFILE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPREADFILE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPREADFILE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPREADFILE no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  End an HTTP request.
// @param  None.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_HTTP_Cancel(void)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QHTTPSTOP" command
    QtEC21_SendCMD(CMD_HTTP_STOP);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(12));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QHTTPSTOP response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPSTOP response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QHTTPSTOP response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QHTTPSTOP no response.");

        return FALSE;
    }

    return TRUE;
}

// File operation functions //

//============================================================================//
// @brief  List the information of a file/ files in a storage.
// @param  pcPattern: Pointer to a char array storing the file path pattern.
//         pu8Resp: Pointer to the buffer storing the returned response.
//         pu16RetLen: Pointer to a variable storing the length of the response.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FileList(char *pcPattern, uint8_t *pu8Resp, uint16_t *pu16RetLen)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QFLST:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    char *pcPtr;

    sprintf(cContent, "%s=\"%s\"", CMD_FILE_LIST, pcPattern);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFLST" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // GSMprintf("\r\n> Receive %d bytes from GSM port", u16DatLen);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\n+QFLST:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\nOK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    if (u16DatLen > 128)
                    {
                        GSMprintf("\r\n> QtEC21 AT+QFLST response length ERROR: %d", u16DatLen);

                        return FALSE;
                    }

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, pu8Resp, u16DatLen);

                    pu8Resp[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFLST response:\r\n%s\r\n> QtEC21 response end.", (char *)pu8Resp);

                    *pu16RetLen = u16DatLen;

                    return TRUE;
                }
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFLST response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 14;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        uint16_t u16ErrCode = atoi(pcPtr);

                        if (u16ErrCode == 426)
                        {
                            bGSM_ResetCheckFTP_Flag = TRUE;

                            // return TRUE;
                            return FALSE;
                        }
                        else
                            return FALSE;
                    }
                    else
                        return FALSE;
                }
                else
                {
                    u16DatLen = 13;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFLST response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFLST response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFLST no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Upload a file from SD card to the module local storage.
// @param  pcFileName: Pointer to a char array to store the name of the file
//         in the SD card.
//         pcLocalName: Name of the file in the local storage.
// @retval True: The operation is done successfully.
//         False: The operation is failed.
//============================================================================//
bool QtEC21_FileUpload(char *pcFileName, char *pcLocalName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nCONNECT\r\n";
    char cSignal2[] = "+QFUPL:";
    char cSignal3[] = "\r\nOK\r\n";
    char cSignal4[] = "\r\n+CME ERROR:";
    char cSignal5[] = "\r\n";
    char cSignal6[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    uint32_t u32FileSize, u32DatCnt, u32DatSize, u32UploadLen;
    char *pcPtr;
    bool bRes;

    if (pcFileName == NULL)
        return FALSE;

    if (strlen(pcFileName) > 80)
        return FALSE;

    u32FileSize = SD_GetFileSize(pcFileName);

    if ((u32FileSize == 0) || (u32FileSize > FTP_FILE_UPLOAD_SIZE_MAX))
    {
        GSMprintf("\r\n> GSM not suitable file size of %d bytes.", u32FileSize);

        return FALSE;
    }

    sprintf(cContent, "%s=\"%s\",%d,%d,%d",
            CMD_FILE_UPLOAD,
            pcLocalName,
            u32FileSize,
            10,
            0);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFUPL" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(5);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(5);

            // If find "\r\nCONNECT\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
        return FALSE;

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    u32DatCnt = 0;

    do
    {
        osStatus osRes = osMutexWait(mutex_data_buf2_user, S_2_mS(20));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> QtEC21 AT+QFUPL can't acquire DATA BUF2 mutex to upload file. osRes:%d", osRes);

            break;
        }

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > sizeof(u8PublicDataBuf))
            u32DatSize = sizeof(u8PublicDataBuf);

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_ReadAddrToBuff(pcFileName, u8PublicDataBuf, u32DatSize, u32DatCnt);

            if (bRes)
            {
                osMutexRelease(mutex_data_buf2_user);

                break;
            }
        }

        if (bRes == FALSE)
        {
            HAL_Delay(1100);

            HAL_UART_Transmit(puartHandle[GSM_COMPORT_DEF], (uint8_t *)CMD_DAT_2_CMD,
                              3, 0xFFFF);

            HAL_Delay(1100);

            osMutexRelease(mutex_data_buf2_user);

            break;
        }

        QtEC21_SendBuffer(u8PublicDataBuf, u32DatSize);

        osMutexRelease(mutex_data_buf2_user);

        u32DatCnt += u32DatSize;

        // if((u32DatCnt % (sizeof(u8PublicDataBuf) * 10)) == 0)
        GSMprintf("\r\n> QtEC21 AT+QFUPL sent %d bytes to storage.", u32DatCnt);

        HAL_Delay(3);

    } while (u32DatCnt < u32FileSize);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(12));

    while (!bGSMTimeOut)
    {
        HAL_Delay(5);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(5);

            // If find "+QFUPL:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                uint8_t u8Cnt = 0;
                bool bRes;

                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Find terminate sequence "\r\nOK\r\n"
                while (u8Cnt < 50)
                {
                    HAL_Delay(1);

                    bRes = UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                            (u16FindPos1 + 10) % rbBuf[GSM_COMPORT_DEF].u16Size);

                    if (bRes)
                        break;

                    u8Cnt++;
                }

                // If find "\r\nOK\r\n"
                if (bRes)
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 8;

                    StopChar(pcPtr, ',');

                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) <= 10))
                    {
                        u32UploadLen = atoi(pcPtr);

                        // GSMprintf("\r\n> Upload length:%d", u32UploadLen);

                        if (u32UploadLen != u32FileSize)
                        {
                            GSMprintf("\r\n> QtEC21 AT+QFUPL number of bytes uploaded is not correct of %d.", u32UploadLen);

                            // GSMprintf("\r\n> QtEC21 response:\r\n%s\r\n> QtEC21 response end.", cContent);

                            return FALSE;
                        }

                        // GSMprintf("\r\n> Find signal 1 at position %d", u16FindPos1);

                        return TRUE;
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QFUPL fail to get uploaded length.");

                        GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                        return FALSE;
                    }
                }
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+QFUPL found message incompleted.");

                    u16DatLen = 7;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFUPL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFUPL no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Open a file in the module storage.
// @param  pcFileName: Pointer to a char array storing the file name.
//         pu32FileHandle: Pointer to a uint32_t variable to store the assigned file ID.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FileOpen(char *pcFileName, uint32_t *pu32FileHandle, uint8_t u8Mode)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\n+QFOPEN:";
    char cSignal2[] = "\r\nOK\r\n";
    char cSignal3[] = "\r\n+CME ERROR:";
    char cSignal4[] = "\r\n";
    char cSignal5[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    char *pcPtr;

    sprintf(cContent, "%s=\"%s\",%d", CMD_FILE_OPEN, pcFileName, u8Mode);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFOPEN" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (!bGSMTimeOut)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // GSMprintf("\r\n> Receive %d bytes from GSM port", u16DatLen);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\n+QFOPEN:"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "OK\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2, u16FindPos1))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 11;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        *pu32FileHandle = atoi(pcPtr);

                        return TRUE;
                    }
                    else
                        return FALSE;
                }
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 14;

                    StopChar(pcPtr, '\r');

                    if (strlen(pcPtr) > 0)
                    {
                        uint16_t u16ErrCode = atoi(pcPtr);

                        if (u16ErrCode == 426)
                        {
                            bGSM_ResetCheckFTP_Flag = TRUE;

                            // return TRUE;
                            return FALSE;
                        }
                        else
                            return FALSE;
                    }
                    else
                        return FALSE;
                }
                else
                {
                    u16DatLen = 13;

                    // Get the current message
                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                // Get the current message
                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFOPEN response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            else
                return FALSE;
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFOPEN no response.");

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Set the file pointer to an address in a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
//         u32FilePtr: Address of the file to set the file pointer to.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FileSeek(uint32_t u32FileHandle, uint32_t u32FilePtr)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d,%d,0", CMD_FILE_SEEK, u32FileHandle, u32FilePtr);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFSEEK" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(10);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFSEEK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFSEEK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFSEEK response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFSEEK no response.");

        return FALSE;
    }

    return TRUE;
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
bool QtEC21_FileRead(uint32_t u32FileHandle, uint32_t u32ReadLen, uint32_t *pu32ActualLen, char *pcReadContent)
{
    char cContent[300];
    char cSignal1[] = "\r\nCONNECT";
    char cSignal2[] = "\r\n";
    char cSignal3[] = "\r\nOK\r\n";
    char cSignal4[] = "\r\n+CME ERROR:";
    char cSignal5[] = "\r\nERROR\r\n";
    uint16_t u16StartPos, u16FindPos1, u16FindPos2, u16FindPos3;
    uint16_t u16DatLen; //, u16MsgLen;
    char *pcPtr;
    uint16_t u16DatSize, u16DatCnt;

    if (u32ReadLen > 1024)
    {
        GSMprintf("\r\n> Can't read more than 1024 bytes.");

        return FALSE;
    }

    // memset(cContent, 0x00, 600);

    sprintf(cContent, "%s=%d,%d", CMD_FILE_READ, u32FileHandle, u32ReadLen);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFREAD" command
    QtEC21_SendCMD(cContent);

    // memset(cContent, 0x00, 600);

    // sprintf(cContent, "%s %d%s", cSignal1, u32ReadLen, cSignal2);

    // u16MsgLen = strlen(cContent);

    // Start the timeout timer
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(1);

        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // HAL_Delay(20);

            // If find "\r\nCONNECT"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            // if(UART_RecvFindPos(GSM_COMPORT_DEF, (char *)cContent, &u16FindPos1, u16StartPos))
            {
                uint8_t u8Cnt = 0;
                bool bRes;

                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Find terminate sequence "\r\n"
                while (u8Cnt < 50)
                {
                    HAL_Delay(1);

                    bRes = UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2,
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

                    UART_PeekRecvData(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 10;

                    StopChar(pcPtr, '\r');

                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) <= 4))
                    {
                        *pu32ActualLen = atoi(pcPtr);

                        // GSMprintf("\r\n> Read actual length:%d", *pu32ActualLen);

                        if (*pu32ActualLen != u32ReadLen)
                        {
                            GSMprintf("\r\n> QtEC21 AT+QFREAD number of bytes read is not correct of %d.", *pu32ActualLen);

                            return FALSE;
                        }

                        // GSMprintf("\r\n> Find signal 1 at position %d", u16FindPos1);

                        break;
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QFREAD failed to get transferred data length.");

                        u16DatCnt = 0;

                        // GSMprintf("\r\n> QtEC21 response:\r\n");

                        while (u16DatCnt < u16DatLen)
                        {
                            u16DatSize = u16DatLen - u16DatCnt;

                            if (u16DatSize > (sizeof(cContent) - 5))
                                u16DatSize = sizeof(cContent) - 5;

                            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatSize);

                            cContent[u16DatSize] = 0;

                            // GSMprintf("%s", cContent);

                            u16DatCnt += u16DatSize;

                            u16FindPos1 = (u16FindPos1 + u16DatSize) % rbBuf[GSM_COMPORT_DEF].u16Size;
                        }

                        // GSMprintf("\r\n> QtEC21 AT+QFREAD response end.");

                        return FALSE;
                    }
                }
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+QFREAD found message incompleted.");

                    u16DatLen = UART_RecvDataLen(GSM_COMPORT_DEF);

                    u16DatCnt = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD response %d bytes:\r\n", u16DatLen);

                    while (u16DatCnt < u16DatLen)
                    {
                        u16DatSize = u16DatLen - u16DatCnt;

                        if (u16DatSize > (sizeof(cContent) - 5))
                            u16DatSize = sizeof(cContent) - 5;

                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatSize);

                        cContent[u16DatSize] = 0;

                        // GSMprintf("%s", cContent);

                        u16DatCnt += u16DatSize;

                        u16FindPos1 = (u16FindPos1 + u16DatSize) % rbBuf[GSM_COMPORT_DEF].u16Size;
                    }

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD response end.");

                    return FALSE;
                }
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                }
                else
                {
                    u16DatLen = 13;
                }

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFREAD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos1, u16StartPos))
            {
                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Read out the current message
                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFREAD response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFREAD couldn't find response CONNECT.");

        *pu32ActualLen = 0;

        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        u16DatCnt = 0;

        // GSMprintf("\r\n> QtEC21 AT+QFREAD response %d bytes:\r\n", u16DatLen);

        while (u16DatCnt < u16DatLen)
        {
            u16DatSize = u16DatLen - u16DatCnt;

            if (u16DatSize > (sizeof(cContent) - 5))
                u16DatSize = sizeof(cContent) - 5;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16StartPos + u16DatCnt, (uint8_t *)cContent, u16DatSize);

            cContent[u16DatSize] = 0;

            // GSMprintf("%s", cContent);

            u16DatCnt += u16DatSize;
        }

        // GSMprintf("\r\n> QtEC21 AT+QFREAD response end.");

        return FALSE;
    }

    // GSMprintf("\r\n> QtEC21 AT+QFREAD wait for OK.");

    // HAL_Delay(100);

    bGSMTimeOut = FALSE;

    u16StartPos = (u16FindPos2 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size;

    // Start the timeout timer
    // osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(FILE_READ_TIMEOUT));
    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        // HAL_Delay(10);

        HAL_Delay(1);

        // u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1 + u16MsgLen);
        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message in the ring buffer
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos3, u16StartPos))
            {
                u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 - u16FindPos2 - 2) %
                            rbBuf[GSM_COMPORT_DEF].u16Size;

                if (u16DatLen == *pu32ActualLen)
                {
                    // Stop the timer
                    osTimerStop(timerRTOS_GSM_BlockHandle);

                    UART_PeekRecvData(GSM_COMPORT_DEF, (u16FindPos2 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size,
                                      (uint8_t *)pcReadContent, u16DatLen);

                    pcReadContent[u16DatLen] = 0;

                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos3 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    u16DatCnt = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD read data from file successfully. Total data size:%d", u16DatLen);

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD found signal 1 at position %d", u16FindPos1);

                    // GSMprintf("\r\n> QtEC21 AT+QFREAD found signal 3 at position %d", u16FindPos3);

                    while (u16DatCnt < u16DatLen)
                    {
                        u16DatSize = u16DatLen - u16DatCnt;

                        if (u16DatSize > (sizeof(cContent) - 5))
                            u16DatSize = sizeof(cContent) - 5;

                        UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatSize);

                        cContent[u16DatSize] = 0;

                        u16DatCnt += u16DatSize;

                        u16FindPos1 = (u16FindPos1 + u16DatSize) % rbBuf[GSM_COMPORT_DEF].u16Size;
                    }

                    return TRUE;
                }
                else
                {
                    u16StartPos = (u16FindPos3 + 4) % rbBuf[GSM_COMPORT_DEF].u16Size;
                }
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFREAD couldn't find response OK. Found signal 1 at position %d", u16FindPos1);

        u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16FindPos1);

        u16DatCnt = 0;

        // GSMprintf("\r\n> QtEC21 response %d bytes:\r\n", u16DatLen);

        while (u16DatCnt < u16DatLen)
        {
            u16DatSize = u16DatLen - u16DatCnt;

            if (u16DatSize > (sizeof(cContent) - 5))
                u16DatSize = sizeof(cContent) - 5;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1 + u16DatCnt, (uint8_t *)cContent, u16DatSize);

            cContent[u16DatSize] = 0;

            // GSMprintf("%s", cContent);

            u16DatCnt += u16DatSize;
        }

        // GSMprintf("\r\n> QtEC21 response end.");

        return FALSE;
    }

    return FALSE;
}

//============================================================================//
// @brief  Write to a file in the module local storage.
// @param  u32FileHandle: ID of the file to write to.
//         pu8DatBuf: Pointer to the data buffer.
//         u16WriteLen: Size of data to write.
// @retval True: The operation is done successfully.
//         False: The operation is failed.
//============================================================================//
bool QtEC21_FileWrite(uint32_t u32FileHandle, uint8_t *pu8DatBuf, uint16_t u16WriteLen)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nCONNECT\r\n";
    char cSignal2[] = "+QFWRITE:";
    char cSignal3[] = "\r\nOK\r\n";
    char cSignal4[] = "\r\n+CME ERROR:";
    char cSignal5[] = "\r\n";
    char cSignal6[] = "\r\nERROR\r\n";
    char cContent[100] = {0};
    char *pcPtr;
    uint32_t u32WrittenLen;

    if (u16WriteLen > sizeof(u8PublicDataBuf))
        return FALSE;

    sprintf(cContent, "%s=%d,%d,%d",
            CMD_FILE_WRITE,
            u32FileHandle,
            u16WriteLen,
            10);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFWRITE" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(5));

    while (!bGSMTimeOut)
    {
        HAL_Delay(1);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(2);

            // If find "\r\nCONNECT\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 11;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                break;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFWRITE no response.");

        return FALSE;
    }

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    QtEC21_SendBuffer(pu8DatBuf, u16WriteLen);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(12));

    while (!bGSMTimeOut)
    {
        HAL_Delay(1);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(2);

            // If find "+QFWRITE:\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                uint8_t u8Cnt = 0;
                bool bRes;

                // Stop the timeout timer
                osTimerStop(timerRTOS_GSM_BlockHandle);

                // Find terminate sequence "\r\nOK\r\n"
                while (u8Cnt < 50)
                {
                    HAL_Delay(1);

                    bRes = UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                            (u16FindPos1 + 9) % rbBuf[GSM_COMPORT_DEF].u16Size);

                    if (bRes)
                        break;

                    u8Cnt++;
                }

                // If find "\r\nOK\r\n"
                if (bRes)
                {
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 6 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    // GSMprintf("\r\n> QtEC21 AT+QFWRITE no response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    pcPtr = (char *)cContent + 10;

                    StopChar(pcPtr, ',');

                    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) <= 10))
                    {
                        u32WrittenLen = atoi(pcPtr);

                        // GSMprintf("\r\n> Upload length:%d", u32UploadLen);

                        if (u32WrittenLen != u16WriteLen)
                        {
                            GSMprintf("\r\n> QtEC21 AT+QFWRITE number of bytes written is not correct of %d.", u32WrittenLen);

                            GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                            return FALSE;
                        }

                        // GSMprintf("\r\n> Find signal 1 at position %d", u16FindPos1);

                        return TRUE;
                    }
                    else
                    {
                        GSMprintf("\r\n> QtEC21 AT+QFWRITE fail to get uploaded length.");

                        GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                        return FALSE;
                    }
                }
                else
                {
                    GSMprintf("\r\n> QtEC21 AT+QFWRITE found message incompleted.");

                    u16DatLen = 9;

                    UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                    cContent[u16DatLen] = 0;

                    GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                    return FALSE;
                }
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal5, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal6, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFWRITE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Close a file in the module storage.
// @param  u32FileHandle: The file's assigned ID.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FileClose(uint32_t u32FileHandle)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_FILE_CLOSE, u32FileHandle);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFCLOSE" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(1));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> AT+QFCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> AT+QFCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> AT+QFCLOSE response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Delete a file in the module storage.
// @param  pcFileName: Pointer to a char array store the file name.
// @retval True: Operation is done successfully.
//         False: Operation fail.
//============================================================================//
bool QtEC21_FileDelete(char *pcFileName)
{
    uint16_t u16StartPos, u16FindPos1, u16FindPos2;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\n+CME ERROR:";
    char cSignal3[] = "\r\n";
    char cSignal4[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=\"%s\"", CMD_FILE_DELETE, pcFileName);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    bGSMTimeOut = FALSE;

    // Send "AT+QFDEL" command
    QtEC21_SendCMD(cContent);

    osTimerStart(timerRTOS_GSM_BlockHandle, S_2_mS(10));

    while (bGSMTimeOut == FALSE)
    {
        HAL_Delay(100);

        // Check response
        uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

        // If find a message
        if (u16DatLen)
        {
            u8GSM_NoRespCnt = 0;

            HAL_Delay(20);

            // If find "\r\nOK\r\n"
            if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 6;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                // GSMprintf("\r\n> QtEC21 AT+QFDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return TRUE;
            }
            // If find "\r\n+CME ERROR:"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                HAL_Delay(20);

                // If find "\r\n"
                if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal3, &u16FindPos2,
                                     (u16FindPos1 + 2) % rbBuf[GSM_COMPORT_DEF].u16Size))
                    u16DatLen = (rbBuf[GSM_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                                rbBuf[GSM_COMPORT_DEF].u16Size;
                else
                    u16DatLen = 13;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
            // If find "\r\nERROR\r\n"
            else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal4, &u16FindPos1, u16StartPos))
            {
                osTimerStop(timerRTOS_GSM_BlockHandle);

                u16DatLen = 9;

                UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos1, (uint8_t *)cContent, u16DatLen);

                cContent[u16DatLen] = 0;

                GSMprintf("\r\n> QtEC21 AT+QFDEL response:\r\n%s\r\n> QtEC21 response end.", cContent);

                return FALSE;
            }
        }
    }

    if (bGSMTimeOut)
    {
        GSMprintf("\r\n> QtEC21 AT+QFDEL no response.");

        return FALSE;
    }

    return TRUE;
}

// Hardware related functions

//============================================================================//
// @brief  Configure the sleep mode enable.
// @param  bOption: Sleep mode enable, 1 Enable, 0: Disable.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool QtEC21_HW_SleepEn(bool bOption)
{
    uint16_t u16StartPos, u16FindPos;
    char cSignal1[] = "\r\nOK\r\n";
    char cSignal2[] = "\r\nERROR\r\n";
    char cContent[100] = {0};

    sprintf(cContent, "%s=%d", CMD_HW_SLEEP_EN, bOption);

    u16StartPos = rbBuf[GSM_COMPORT_DEF].u16Head;

    // Send "AT+QSCLK" command
    QtEC21_SendCMD(cContent);

    HAL_Delay(400);

    // Check response
    uint16_t u16DatLen = UART_RecvDataLenPos(GSM_COMPORT_DEF, u16StartPos);

    // If find a message
    if (u16DatLen)
    {
        u8GSM_NoRespCnt = 0;

        // If find "\r\nOK\r\n"
        if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal1, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 6;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            // GSMprintf("\r\n> QtEC21 response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return TRUE;
        }
        // If find "\r\nERROR\r\n"
        else if (UART_RecvFindPos(GSM_COMPORT_DEF, cSignal2, &u16FindPos, u16StartPos))
        {
            // Read out the current message
            u16DatLen = 9;

            UART_GetRecvDataPos(GSM_COMPORT_DEF, u16FindPos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GSMprintf("\r\n> QtEC21 response:\r\n%s\r\n> QtEC21 response end.", cContent);

            return FALSE;
        }
        else
            return FALSE;
    }
    else
    {
        QtEC21_RetryCheck();

        return FALSE;
    }
}

#endif // #if ENABLE_GSM_QTEC21
