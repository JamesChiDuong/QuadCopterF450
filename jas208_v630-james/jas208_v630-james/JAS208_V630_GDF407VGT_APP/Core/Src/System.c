//============================================================================//
// User Includes
//============================================================================//

#include "System.h"

#include <string.h>
#include <stdlib.h>

#include "EXTIO.h"
#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "Config.h"
#include "Datalog.h"
#include "DWT_Delay.h"
#include "EXTIO.h"
#include "GPS_Driver.h"
#include "main.h"
#include "LCD_Key.h"
#include "MG82_Driver.h"
#include "MX25L_Driver.h"
#if ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "Power.h"
#include "PRN_Driver.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "UART_Driver.h"
#include "UART_ExtDriver.h"
#include "USB_Module.h"
#include "Version.h"
#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#endif
//============================================================================//
// User Defines
//============================================================================//

#define FILENAME_SETUP_SYS "SETUP.SYS"

#define FILENAME_SETUP1_SYS "SETUP1.SYS"

//============================================================================//
// User Variables
//============================================================================//
TaskInfoTypeDef taskInfo[TASK_QUANTITY]; // __attribute__ ((section("ccmram_var")));

gSettingTypeDef g_Setting;

gSysInfoTypeDef g_SysInfo; // __attribute__ ((section("ccmram_var")));

bool bSystemMode = SYSTEM_MODE_NORMAL;

bool g_bSystemBootFlag = FALSE;

bool bSettingInitFinish = FALSE;

const ProductInfoTypeDef PRODUCT_INFO = {MODEL_NAME, CUSTOMER_TYPE, HW_VER, VERSION_BUILD};

bool bSysLogEn = TRUE;

bool g_bPTestMode = FALSE;

bool bSystemStop = FALSE;

gSettingTypeDef g_SettingTemp1 = {0}, g_SettingTemp2 = {0};

BackupSettingTypeDef bkpSettingTemp1 = {0}, bkpSettingTemp2 = {0};

bool bSystemTimeReady = FALSE;

RTC_TimeTypeDef tRTC_InitTime;
RTC_DateTypeDef tRTC_InitDate;

uint8_t u8ResetFlagStatus;
bool bHaveResetFlag = FALSE;

uint8_t u8DrvPreStatus[2] = {0};

float fAutoGainValue;

// uint8_t u8PublicDataBuf[8192]; // 8 kBytes

// uint8_t TID_WriteSetupFile = TIMER_INVALID_ID;

// g_u8HwInfo = g_u8HwInfo | 0x80; ///4G
// g_u8HwInfo = g_u8HwInfo | 0x40;
// g_u8HwInfo = g_u8HwInfo | 0x20;
// g_u8HwInfo = g_u8HwInfo | 0x10; ///RTC
////////////////////////////////////////
// g_u8HwInfo = g_u8HwInfo | 0x08; ///SD
// g_u8HwInfo = g_u8HwInfo | 0x04; ///GPS
// g_u8HwInfo = g_u8HwInfo | 0x02; ///Monitor
// g_u8HwInfo = g_u8HwInfo | 0x01; ///Printer
uint8_t g_u8HwInfo = 0;

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
bool bRestoreSettingFlag = FALSE;
#endif

//============================================================================//
// User Functions
//============================================================================//
//============================================================================//
// @brief  Initialize handling information for all tasks.
// @param  None.
// @retval None.
//============================================================================//
void System_TaskInfoInit(void)
{
    memset(taskInfo, 0x00, sizeof(taskInfo));

    taskInfo[TASK_LOG_PROC].taskID = taskLogProcHandle;
    sprintf(taskInfo[TASK_LOG_PROC].cTaskName, "LogProc");
    taskInfo[TASK_LOG_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_EXTIO_PROC].taskID = taskEXTIO_ProcHandle;
    sprintf(taskInfo[TASK_EXTIO_PROC].cTaskName, "EXTIO_Proc");
    taskInfo[TASK_EXTIO_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_GPS_PROC].taskID = taskGPS_ProcHandle;
    sprintf(taskInfo[TASK_GPS_PROC].cTaskName, "GPS_Proc");
    taskInfo[TASK_GPS_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_GSM_PROC].taskID = taskGSM_ProcHandle;
    sprintf(taskInfo[TASK_GSM_PROC].cTaskName, "GSM_Proc");
    taskInfo[TASK_GSM_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_LCD_KEY].taskID = taskLCD_KeyHandle;
    sprintf(taskInfo[TASK_LCD_KEY].cTaskName, "LCD_Key");
    taskInfo[TASK_LCD_KEY].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_RS232_PROC].taskID = taskRS232_ProcHandle;
    sprintf(taskInfo[TASK_RS232_PROC].cTaskName, "RS232_Proc");
    taskInfo[TASK_RS232_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_HIGH_LOAD].taskID = taskHighLoadHandle;
    sprintf(taskInfo[TASK_HIGH_LOAD].cTaskName, "HighLoad");
    taskInfo[TASK_HIGH_LOAD].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_SD_PROC].taskID = taskSD_ProcHandle;
    sprintf(taskInfo[TASK_SD_PROC].cTaskName, "SD_Proc");
    taskInfo[TASK_SD_PROC].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_GSM_RX].taskID = taskGSM_RXHandle;
    sprintf(taskInfo[TASK_GSM_RX].cTaskName, "GSM_RX");
    taskInfo[TASK_GSM_RX].u32TaskTimeout = S_2_mS(240);

    taskInfo[TASK_GSM_TX].taskID = taskGSM_TXHandle;
    sprintf(taskInfo[TASK_GSM_TX].cTaskName, "GSM_TX");
    taskInfo[TASK_GSM_TX].u32TaskTimeout = S_2_mS(240);

    // uint32_t u32StartTick = HAL_GetTick();

    for (int i = 0; i < TASK_QUANTITY; i++)
    {
        taskInfo[i].bTaskState = TRUE;

        taskInfo[i].bTaskDelay = FALSE;

        // taskInfo[i].u32LastTick = u32StartTick;
        taskInfo[i].u32LastTick = HAL_GetTick();
    }
}

//============================================================================//
// @brief  Check the running status of all tasks.
// @param  None.
// @retval None.
//============================================================================//
void System_TaskCheck(void)
{
    static uint32_t u32TickCnt = 0;

    if (sdRequest.sdCmd == SD_MKFS)
    {
        return;
    }

    if (HAL_GetTick() > (u32TickCnt + S_2_mS(1)))
    {
        u32TickCnt = HAL_GetTick();

        for (int i = 0; i < TASK_QUANTITY; i++)
        {
            if (taskInfo[i].bTaskState == TRUE)
            {
                uint32_t u32CurrTick = HAL_GetTick();

                if (((u32CurrTick - taskInfo[i].u32LastTick) >= taskInfo[i].u32TaskTimeout) &&
                    (u32CurrTick >= taskInfo[i].u32LastTick))
                {
                    // if(taskInfo[i].bTaskCheck == FALSE)
                    //{
                    RTOSprintf("\r\n> !!! TASK %s HAS BEEN HANGED AT STEP %d !!!", taskInfo[i].cTaskName, taskInfo[i].u16TaskStep);
                    RTOSprintf("\r\n> TASK %s TIMEOUT:%d ms, LAST TICK:%d, CURRENT TICK: %d",
                               taskInfo[i].cTaskName,
                               taskInfo[i].u32TaskTimeout,
                               taskInfo[i].u32LastTick,
                               u32CurrTick);

                    /*
                    HAL_IWDG_Refresh(&hiwdg);

                    vTaskSuspendAll();

                    printf("\r\n> TERMINATE TASK %s", taskInfo[i].cTaskName);

                    osThreadTerminate(taskInfo[i].taskID);

                    printf("\r\n> RECREATE TASK %s", taskInfo[i].cTaskName);

                    taskInfo[i].taskID = osThreadCreate(&(taskInfo[i].tTaskDef), NULL);

                    printf("\r\n> TASK %s CREATED.", taskInfo[i].cTaskName);

                    if(taskInfo[i].taskID == NULL)
                    {
                        System_SaveSettingFinal();
                        System_SaveBackupSettingFinal();

                        printf("\r\n> CANNOT RESTART TASK %s. RESET SYSTEM TO FIX TASK HANGING ISSUE",
                                             taskInfo[i].cTaskName);

                        HAL_IWDG_Refresh(&hiwdg);

                        if(bRTOS_En)
                        {
                            osMutexWait(mutex_setting_user, S_2_mS(4));
                        }

                        HAL_IWDG_Refresh(&hiwdg);

                        System_Reset();
                    }

                    printf("\r\n> TASK %s RESTARTED SUCCESSFULLY!", taskInfo[i].cTaskName);

                    xTaskResumeAll();

                    RTOSprintf("\r\n> ALL TASK RESUMED");
                    */

                    System_SaveSettingFinal();
                    System_SaveBackupSettingFinal();

                    RTOSprintf("\r\n> RESET SYSTEM TO FIX TASK %s HANGING ISSUE", taskInfo[i].cTaskName);

                    HAL_IWDG_Refresh(&hiwdg);

                    // char cMsg[20] = {0};

                    // sprintf(cMsg, "1-%d-%d-%d", i, taskInfo[i].u16TaskStep, u8USB_TaskCnt);

                    // for(int i = 0; i < MAX_SOCKET_BANK; i++)
                    //{
                    //	GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, cMsg, i);
                    // }

                    HW_WriteBackupRegister(BKP_FAULT_CHK, TASK_HANG_EVENT_CODE);
                    HW_WriteBackupRegister(BKP_FAULT_CODE, (uint8_t)i);
                    HW_WriteBackupRegister(BKP_FAULT_CODE_2, (uint8_t)taskInfo[i].u16TaskStep);

                    if (bRTOS_En)
                    {
                        osMutexWait(mutex_setting_user, S_2_mS(5));
                    }

                    // System_Reset();
                    //}

                    // taskInfo[i].bTaskCheck = FALSE;

                    // taskInfo[i].u32LastTick = HAL_GetTick();
                }
            }
        }
    }
}

//============================================================================//
// @brief  Refresh the timeout check for a task.
// @param  taskTag: Tag of the task as TaskTagTypeDef format.
// @retval None.
//============================================================================//s
void System_TaskWdgRefresh(TaskTagTypeDef taskTag)
{
    if (taskInfo[taskTag].bTaskDelay == TRUE)
    {
        RTOSprintf("\r\n> Test to make task %s hanged.", taskInfo[taskTag].cTaskName);

        taskInfo[taskTag].bTaskDelay = FALSE;

        // taskInfo[taskTag].bTaskCheck = FALSE;

        HAL_Delay(taskInfo[taskTag].u32TaskTimeout + S_2_mS(10));
    }

    // taskInfo[taskTag].bTaskCheck = TRUE;

    taskInfo[taskTag].u32LastTick = HAL_GetTick();

    taskInfo[taskTag].u16TaskStep = 0;
}

//============================================================================//
// @brief  Check tasks' remaining stack availability.
// @param  None.
// @retval None.
//============================================================================//
void System_TaskCheckWaterMark(void)
{
    char cBuf[100] = {0};
    char *pcPtr;

    pcPtr = cBuf;

    sprintf(pcPtr, "\r\n> ### STACK RECORD: ");

    pcPtr += strlen(pcPtr);

    sprintf(pcPtr, "1:%u ", (uint32_t)uxTaskGetStackHighWaterMark(defaultTaskHandle));

    for (int i = 0; i < TASK_QUANTITY; i++)
    {
        pcPtr += strlen(pcPtr);

        sprintf(pcPtr, "%d:%u, ", i + 2, (uint32_t)uxTaskGetStackHighWaterMark(taskInfo[i].taskID));
    }

    RTOSprintf(cBuf);
}

//============================================================================//
// @brief  Enable 3V3 power for peripherals.
// @param  None.
// @retval None.
//============================================================================//
void System_IO_3V3_Enable(void)
{
    HW_PBSet(IO_3V3_EN);

    HAL_Delay(10);
}

//============================================================================//
// @brief  Disable 3V3 power.
// @param  None.
// @retval None.
//============================================================================//
void System_IO_3V3_Disable(void)
{
    HW_PBClr(IO_3V3_EN);

    HAL_Delay(10);
}

//============================================================================//
// @brief  Check the system flags and clear if they are set.
// @param  None.
// @retval None.
//============================================================================//
void System_FlagCheck(void)
{
    // RTOSprintf("\r\n> Reset flag: %d-%d-%d-%d-%d-%d-%d",
    //            __HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST),
    //            __HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST),
    //					 __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST),
    //					 __HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST),
    //					 __HAL_RCC_GET_FLAG(RCC_FLAG_PORRST),
    //					 __HAL_RCC_GET_FLAG(RCC_FLAG_PINRST),
    //					 __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST)
    //					);

    u8ResetFlagStatus = (uint8_t)(RCC->CSR >> 25);

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
    {
        RTOSprintf("\r\n> ### System reset from low-power ###");

        bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
    {
        RTOSprintf("\r\n> ### System reset from WWDG timeout ###");

        bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        RTOSprintf("\r\n> ### System reset from IWDG timeout ###");

        bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        RTOSprintf("\r\n> ### System reset from software-reset ###");

        bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
    {
        // RTOSprintf("\r\n> ### System reset from POR ###");

        // bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        // RTOSprintf("\r\n> ### System reset from Pin reset ###");

        // bHaveResetFlag = TRUE;
    }

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
    {
        RTOSprintf("\r\n> ### System reset from BOR ###");

        bHaveResetFlag = TRUE;
    }

    __HAL_RCC_CLEAR_RESET_FLAGS();

    if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
    {
        // UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)"\r\n> System reset from standby mode.", 35);
        RTOSprintf("\r\n> ### System wakeup from standby mode ###");

        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    }

    if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
    {
        RTOSprintf("\r\n> ### System wakeup from WU pin ###");

        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    }

    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO) != RESET)
    {
        RTOSprintf("\r\n> ### System wakeup from by PVDO ###");

        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVDO);
    }

    if (__HAL_PWR_GET_FLAG(PWR_FLAG_BRR) != RESET)
    {
        RTOSprintf("\r\n> ### System wakeup from by BRR ###");

        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_BRR);
    }

    if (__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) != RESET)
    {
        RTOSprintf("\r\n> ### System wakeup from by VOS ###");

        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_VOSRDY);
    }
    // Clear RTC alarm A flag
    if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_ALRAF))
    {
        RTOSprintf("\r\n> ### System waken up by RTC alarm A ###");

        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

        if ((g_Setting.tExAlarm.bEnableAlm == TRUE) && (g_Setting.tExAlarm.bRepeat == FALSE))
        {
            HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

            g_Setting.tExAlarm.bEnableAlm = FALSE;

            g_Setting.tExAlarm.bRequest = FALSE;

            System_SaveSetting();
        }
    }

    // Clear RTC alarm B flag
    if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_ALRBF))
    {
        RTOSprintf("\r\n> ### System waken up by RTC alarm B ###");

        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRBF);
    }

    // Clear RTC wake up timer flag
    if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF))
    {
        RTOSprintf("\r\n> ### System waken up by RTC wake up timer ###");

        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    }
}

//============================================================================//
// @brief  Check files for temporary storage.
// @param  None.
// @retval None.
//============================================================================//
void System_CheckTempFile(void)
{
    char cFileName[64] = {0};
    bool bRes;

    if (SD_CheckDirExist((char *)DIRNAME_TEMP))
    {
        // Check USB download request temp file
        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);

        if (SD_CheckFileExist(cFileName))
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                LOGprintf("\r\n> Number of pending USB download requests: %d", u16USB_DownloadNum);

                if (u16USB_DownloadNum > 50)
                {
                    u16USB_DownloadNum = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);
                }
            }
            else
            {
                u16USB_DownloadNum = 0;

                SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);
            }
        }
        else
        {
            u16USB_DownloadNum = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
        }

        // Check print request temp file
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, PRINT_DATA_TEMP_FILE);

        if (SD_CheckFileExist(cFileName))
        {
            for (int i = 0; i < 3; i++)
            {
                bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&u16PrintRequestNum, 2, 0);

                if (bRes)
                    break;
            }

            if (bRes)
            {
                EXTprintf("\r\n> Number of pending printing requests: %d", u16PrintRequestNum);

                if (u16PrintRequestNum > 10)
                {
                    u16PrintRequestNum = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16PrintRequestNum, 2, 0);
                }
            }
            else
            {
                u16PrintRequestNum = 0;

                SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16PrintRequestNum, 2, 0);
            }
        }
        else
        {
            u16PrintRequestNum = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16PrintRequestNum, 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
        }

#if ENABLE_GSM_NET
        // If version is JAS208U
        // if(strncmp(g_Setting.cJAS_Ver, (char *)JAS208_GSM, 7) == 0)
        {
            // Check GSM SMS request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);

            if (SD_CheckFileExist(cFileName))
            {
                for (int i = 0; i < 3; i++)
                {
                    memset(&tSMS_Index, 0x00, sizeof(FIFO_IndexTypeDef));

                    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);

                    if (bRes)
                        break;
                }

                if (bRes)
                {
                    GSMprintf("\r\n> Number of pending SMS requests: %d (T:%d, H:%d)",
                              tSMS_Index.u16Size, tSMS_Index.u16Tail, tSMS_Index.u16Head);

                    // If the indices are invalid
                    if ((tSMS_Index.u16Size > (GSM_SMS_NUM_MAX - 1)) ||
                        (((tSMS_Index.u16Tail + GSM_SMS_NUM_MAX - tSMS_Index.u16Head) % GSM_SMS_NUM_MAX) !=
                         tSMS_Index.u16Size))
                    {
                        memset(&tSMS_Index, 0x00, sizeof(FIFO_IndexTypeDef));

                        SD_WriteBuffToAddr(cFileName, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);
                    }
                }
                else
                {
                    memset(&tSMS_Index, 0x00, sizeof(FIFO_IndexTypeDef));

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);
                }
            }
            else
            {
                memset(&tSMS_Index, 0x00, sizeof(FIFO_IndexTypeDef));

                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);

                    if (bRes)
                    {
                        RTOSprintf("\r\n> Create file %s OK.", cFileName);

                        break;
                    }
                }
            }

#if ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
            // Check GSM file download request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);

            if (SD_CheckFileExist(cFileName))
            {
                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);

                    if (bRes)
                        break;
                }

                if (bRes)
                {
                    GSMprintf("\r\n> Number of pending download file requests: %d", u16DownloadFileNum);

                    if (u16DownloadFileNum > 10)
                    {
                        u16DownloadFileNum = 0;

                        SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);
                    }
                }
                else
                {
                    u16DownloadFileNum = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);
                }
            }
            else
            {
                u16DownloadFileNum = 0;

                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);

                    if (bRes)
                    {
                        RTOSprintf("\r\n> Create file %s OK.", cFileName);

                        break;
                    }
                }
            }
#endif

            // Check GSM file upload request temp file
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

            // Check GSM file upload check request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);

            if (SD_CheckFileExist(cFileName))
            {
                for (int i = 0; i < 3; i++)
                {
                    memset(&tUploadChkHandle, 0x00, sizeof(FIFO_IndexTypeDef));

                    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);

                    if (bRes)
                        break;
                }

                if (bRes)
                {
                    GSMprintf("\r\n> Number of pending upload file check requests: %d (T:%d, H:%d)",
                              tUploadChkHandle.u16Size, tUploadChkHandle.u16Tail, tUploadChkHandle.u16Head);

                    // If the indices are invalid
                    if ((tUploadChkHandle.u16Size > (UPLOAD_CHK_NUM_MAX - 1)) ||
                        (((tUploadChkHandle.u16Tail + UPLOAD_CHK_NUM_MAX - tUploadChkHandle.u16Head) % UPLOAD_CHK_NUM_MAX) !=
                         tUploadChkHandle.u16Size))
                    {
                        memset(&tUploadChkHandle, 0x00, sizeof(FIFO_IndexTypeDef));

                        SD_WriteBuffToAddr(cFileName, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);
                    }
                }
                else
                {
                    memset(&tUploadChkHandle, 0x00, sizeof(FIFO_IndexTypeDef));

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);
                }
            }
            else
            {
                memset(&tUploadChkHandle, 0x00, sizeof(FIFO_IndexTypeDef));

                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);

                    if (bRes)
                    {
                        RTOSprintf("\r\n> Create file %s OK.", cFileName);

                        break;
                    }
                }
            }

            // Check GSM send packet request temp file
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
                    GSMprintf("\r\n> GSM server 0 unsent packets: %d", u16UnsendPkgNum[SOCKET_BANK_DEF]);

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

#if ENABLE_MULTI_SERVER
            // Check GSM send packet request temp file for EXT server
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_EXT_TEMP_FILE);

            if (SD_CheckFileExist(cFileName))
            {
                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);

                    if (bRes)
                        break;
                }

                if (bRes)
                {
                    GSMprintf("\r\n> GSM server 1 unsent packets: %d", u16UnsendPkgNum[SOCKET_BANK_EXT]);

                    if (u16UnsendPkgNum[SOCKET_BANK_EXT] > GSM_PACKET_NUM_MAX)
                    {
                        u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

                        SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);
                    }
                }
                else
                {
                    u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

                    SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16UnsendPkgNum[SOCKET_BANK_EXT], 2, 0);
                }
            }
            else
            {
                u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

                for (int i = 0; i < 3; i++)
                {
                    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);

                    if (bRes)
                    {
                        RTOSprintf("\r\n> Create file %s OK.", cFileName);

                        break;
                    }
                }
            }
#endif
        }

        // #if ENABLE_UPLOAD_CHK
        // GSM_FTP_UploadChkHandle();
        // #endif
#endif
    }
    else
    {
        SD_MakeDir((char *)DIRNAME_TEMP);

        RTOSprintf("\r\n> Create directory %s OK.", (char *)DIRNAME_TEMP);

        // Create USB download request temp file
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);

        u16USB_DownloadNum = 0;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK.", cFileName);

                break;
            }
        }

        // Create print request temp file
        memset(cFileName, 0x00, sizeof(cFileName));

        sprintf(cFileName, "%s/%s", DIRNAME_TEMP, PRINT_DATA_TEMP_FILE);

        u16PrintRequestNum = 0;

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16PrintRequestNum, 2, 0);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK.", cFileName);

                break;
            }
        }

#if ENABLE_GSM_NET
        // If version is JAS208U
        // if(strncmp(g_Setting.cJAS_Ver, (char *)JAS208_GSM, 7) == 0)
        {
            // Create SMS request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SMS_PKG_TEMP_FILE);

            memset(&tSMS_Index, 0x00, sizeof(FIFO_IndexTypeDef));

            for (int i = 0; i < 3; i++)
            {
                SD_WriteBuffToAddr(cFileName, (uint8_t *)&tSMS_Index, sizeof(FIFO_IndexTypeDef), 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }

#if ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
            // Create GSM download file request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, FTP_DOWNLOAD_TEMP_FILE);

            u16DownloadFileNum = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16DownloadFileNum, 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
#endif

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

            // Create GSM upload file check request temp file
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, UPLOAD_CHK_TEMP_FILE);

            memset(&tUploadChkHandle, 0x00, sizeof(FIFO_IndexTypeDef));

            for (int i = 0; i < 3; i++)
            {
                SD_WriteBuffToAddr(cFileName, (uint8_t *)&tUploadChkHandle, sizeof(FIFO_IndexTypeDef), 0);

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
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_DEF]), 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }

#if ENABLE_MULTI_SERVER
            // Create GSM send packet request temp file for EXT server
            memset(cFileName, 0x00, sizeof(cFileName));

            sprintf(cFileName, "%s/%s", DIRNAME_TEMP, SEND_PKG_EXT_TEMP_FILE);

            u16UnsendPkgNum[SOCKET_BANK_EXT] = 0;

            for (int i = 0; i < 3; i++)
            {
                bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&(u16UnsendPkgNum[SOCKET_BANK_EXT]), 2, 0);

                if (bRes)
                {
                    RTOSprintf("\r\n> Create file %s OK.", cFileName);

                    break;
                }
            }
#endif
        }
#endif
    }
}

//============================================================================//
// @brief  Check files for temporary storage.
// @param  None.
// @retval None.
//============================================================================//
void System_CheckSettingFile(void)
{
    char cFileName[64] = {0};
    bool bRes;

    // Check SETTING dir
    if (SD_CheckDirExist((char *)DIRNAME_SETTING))
    {
        memset(cFileName, 0x00, sizeof(cFileName));

        // Check file G_SETTING
        sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_G_SETTING);

        if (SD_CheckFileExist(cFileName))
        {
            if (bRTOS_En)
            {
                osStatus osRes;

                osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

                if (osRes != osOK)
                    return;
            }

            memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

            bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&g_SettingTemp1, sizeof(gSettingTypeDef), 0);

            if (bRes)
            {
                // RTOSprintf("\r\n> System file %s OK.", FILENAME_G_SETTING);

                // if((g_SettingTemp1.u8SetVer == SETUP_VERSION) && (g_SettingTemp1.u8SetVerChk == NOT_SETUP_VERSION) &&
                //	 (g_SettingTemp1.ttLastSaveTime != 0))
                //{
                //	RTOSprintf("\r\n> System SD g_Setting data valid.");
                // }
                // else
                //{
                //	RTOSprintf("\r\n> System SD g_Setting data is invalid, SetVer: 0x%02X, SetVer2: 0x%02X, last save time: %d",
                //	           g_SettingTemp1.u8SetVer,
                //	           g_SettingTemp1.u8SetVerChk,
                //	           g_SettingTemp1.ttLastSaveTime);
                // }
            }

            if (bRTOS_En)
            {
                osMutexRelease(mutex_setting_user);
            }
        }
        else
        {
            bRes = SD_AddBuffToFile(cFileName, (uint8_t *)" ", 1);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK", FILENAME_G_SETTING);
            }
        }

        // Check file BACKUP_SETTING
        sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_BACKUP_SETTING);

        if (SD_CheckFileExist(cFileName))
        {
            if (bRTOS_En)
            {
                osStatus osRes;

                osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

                if (osRes != osOK)
                    return;
            }

            memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

            bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&bkpSettingTemp1, sizeof(BackupSettingTypeDef), 0);

            if (bRes)
            {
                // RTOSprintf("\r\n> System file %s OK.", FILENAME_BACKUP_SETTING);

                // if(bkpSettingTemp1.ttLastSaveTime != 0)
                //{
                //   RTOSprintf("\r\n> System SD backup setting last save time is valid.");
                // }
                // else
                //{
                //   RTOSprintf("\r\n> System SD backup setting last save time is invalid.");
                // }
            }

            if (bRTOS_En)
            {
                osMutexRelease(mutex_setting_user);
            }
        }
        else
        {
            bRes = SD_AddBuffToFile(cFileName, (uint8_t *)" ", 1);

            if (bRes)
            {
                RTOSprintf("\r\n> Create file %s OK", FILENAME_BACKUP_SETTING);
            }
        }
    }
    else
    {
        for (int i = 1; i < 3; i++)
        {
            bRes = SD_MakeDir((char *)DIRNAME_SETTING);

            if (bRes)
                break;
        }

        RTOSprintf("\r\n> Create directory %s OK.", (char *)DIRNAME_SETTING);

        sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_G_SETTING);

        bRes = SD_AddBuffToFile(cFileName, (uint8_t *)" ", 1);

        if (bRes)
        {
            RTOSprintf("\r\n> Create file %s OK", FILENAME_G_SETTING);
        }

        sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_BACKUP_SETTING);

        bRes = SD_AddBuffToFile(cFileName, (uint8_t *)" ", 1);

        if (bRes)
        {
            RTOSprintf("\r\n> Create file %s OK", FILENAME_BACKUP_SETTING);
        }
    }
}

//============================================================================//
// @brief  Setting system in default.
// @param  None.
// @retval None.
//============================================================================//
void System_SetupDefault(void)
{
    RTOSprintf("\r\n> ### System_SetupDefault ###");

    memset(&g_Setting, 0x00, sizeof(gSettingTypeDef));

    // SetVer //
    g_Setting.u8SetVer = SETUP_VERSION;

    // SetVerChk //
    g_Setting.u8SetVerChk = ~SETUP_VERSION;

    // SetSize //
    g_Setting.u32SetSize = sizeof(gSettingTypeDef);

// GSM version //
#if ENABLE_GSM_NET
    sprintf(g_Setting.cJAS_Ver, "%s", JAS208_GSM);
#else
    sprintf(g_Setting.cJAS_Ver, "%s", JAS208_NO_GSM);
#endif

    // FW version
    sprintf(g_Setting.cFW_Version, "%s%s", FIRMWARE_VERSION, FW_VERSION_CHK);

    // Time zone offset //
    g_Setting.i32TimeZoneOffset = TIME_ZONE_OFFSET_DEF;

    // Power down setting //
    g_Setting.tPwrDown.tMode = PWR_DOWN_SHUTDOWN;
#if HW_TYPE == HW_JAS208
    g_Setting.tPwrDown.u16Delay = MINUTE_2_SECOND(3);
#elif HW_TYPE == HW_JAS306SD || HW_TYPE == HW_JAS411
    g_Setting.tPwrDown.u16Delay = MINUTE_2_SECOND(3);
#endif

#if ENABLE_LSM6DSL
    g_Setting.tGS_Setting.u32DelayTimeOut = MINUTE_2_SECOND(3);
    g_Setting.tGSM_SET.u32DelayTimeOut = MINUTE_2_SECOND(5);
#endif

    // ACC off time //
    g_Setting.ttAccOffTime = 0;

    // Current driver ID //
    g_Setting.u8CurrDrvID = 0;

    // Debug mode //
    g_Setting.u32DebugMode = DebugModeValue[DBG_OFF];

    // Record mode //
    g_Setting.u32RecordMode = RecordModeValue[REC_SPD_INT] |
                              RecordModeValue[REC_CAN_DBG] |
                              RecordModeValue[REC_GPS_SPD];

    // BUS Info //
    strcpy(g_Setting.tBusInfo.cBus_ID, DEFAULT_STRING);
    strcpy(g_Setting.tBusInfo.cBus_SN, DEFAULT_STRING);
    strcpy(g_Setting.tBusInfo.cVIN, DEFAULT_STRING);

    g_Setting.tBusInfo.tCarType = CARTYPE_END;

    // SD card state //
    g_Setting.tSD_State.bSD_State = TRUE;
    g_Setting.tSD_State.bSD_FormatFlag = FALSE;
    g_Setting.tSD_State.u8SD_ResetCnt = 0;

// Datalog setting //
// g_Setting.tLogSet.stRestartLog.u8RestartCnt = 0;
// g_Setting.tLogSet.stRestartLog.u32SysTickBuf = 0;

// GSM setting //
#if ENABLE_GSM_NET
    // sprintf(g_Setting.tGSM_SET.tPhoneBank[0].cLabel, "Default user");

    // sprintf(g_Setting.tGSM_SET.tPhoneBank[0].cNumber, PHONE_NUMBER_DEFAULT);

    g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ = 0;

    g_Setting.tGSM_SET.tNetSet.u16SendStandby = MINUTE_2_SECOND(1);

    // g_Setting.tGSM_SET.u8LevelMicGain = 10;
    // g_Setting.tGSM_SET.u8LevelSpkGain = 50;
    // g_Setting.tGSM_SET.u8LevelRingSound = 50;	 //Heinz 100

    g_Setting.tGSM_SET.tGSM_Err.bGSM_ModuleErr = FALSE;
    g_Setting.tGSM_SET.tGSM_Err.u8SIM_ErrCnt = 0;
    g_Setting.tGSM_SET.tGSM_Err.u8TCP_ErrCnt = 0;
    g_Setting.tGSM_SET.tGSM_Err.u8FTP_ErrCnt = 0;
    g_Setting.tGSM_SET.tGSM_Err.u8SMS_ErrCnt = 0;
    g_Setting.tGSM_SET.tGSM_Err.u8PkgErrCnt = 0;

    // GSM TCP IP setting
    strcpy(g_Setting.tGSM_SET.tNetSet.cAPN, "");
    strcpy(g_Setting.tGSM_SET.tNetSet.cAPN_UserName, "");
    strcpy(g_Setting.tGSM_SET.tNetSet.cAPN_Password, "");

    sprintf(g_Setting.tGSM_SET.tServerInfo[0].cIP_Addr, "%s", (char *)SERVER_1_IP_ADDR);

    g_Setting.tGSM_SET.tServerInfo[0].u32IP_Addr._U8[3] = (uint8_t)SERVER_1_IP_ADDR_BYTE3;
    g_Setting.tGSM_SET.tServerInfo[0].u32IP_Addr._U8[2] = (uint8_t)SERVER_1_IP_ADDR_BYTE2;
    g_Setting.tGSM_SET.tServerInfo[0].u32IP_Addr._U8[1] = (uint8_t)SERVER_1_IP_ADDR_BYTE1;
    g_Setting.tGSM_SET.tServerInfo[0].u32IP_Addr._U8[0] = (uint8_t)SERVER_1_IP_ADDR_BYTE0;

    sprintf(g_Setting.tGSM_SET.tServerInfo[0].cDomainName, "%s", TCP_DNS_NAME_DEF);

    g_Setting.tGSM_SET.tServerInfo[0].u16Port = (uint16_t)TCP_DNS_PORT_DEF;

    g_Setting.tGSM_SET.tServerInfo[0].bEnableChk = (bool)SERVER_1_ENABLE_CHK;
    g_Setting.tGSM_SET.tServerInfo[0].bEnableLog = (bool)SERVER_1_ENABLE_LOG;
    g_Setting.tGSM_SET.tServerInfo[0].bEnableEvent = (bool)SERVER_1_ENABLE_EVENT;

    sprintf(g_Setting.tGSM_SET.tServerInfo[0].cHeaderChk, "%s", (char *)SERVER_1_HEADER_CHK);
    sprintf(g_Setting.tGSM_SET.tServerInfo[0].cHeaderLog, "%s", (char *)SERVER_1_HEADER_LOG);
    sprintf(g_Setting.tGSM_SET.tServerInfo[0].cHeaderEvent, "%s", (char *)SERVER_1_HEADER_EVENT);

    g_Setting.tGSM_SET.tServerInfo[0].u8LogPeriod = (uint8_t)SERVER_1_SEND_NORMAL;
    // If enable multiple servers
    // #if ENABLE_MULTI_SERVER
    sprintf(g_Setting.tGSM_SET.tServerInfo[1].cIP_Addr, "%s", (char *)SERVER_2_IP_ADDR);

    g_Setting.tGSM_SET.tServerInfo[1].u32IP_Addr._U8[3] = (uint8_t)SERVER_2_IP_ADDR_BYTE3;
    g_Setting.tGSM_SET.tServerInfo[1].u32IP_Addr._U8[2] = (uint8_t)SERVER_2_IP_ADDR_BYTE2;
    g_Setting.tGSM_SET.tServerInfo[1].u32IP_Addr._U8[1] = (uint8_t)SERVER_2_IP_ADDR_BYTE1;
    g_Setting.tGSM_SET.tServerInfo[1].u32IP_Addr._U8[0] = (uint8_t)SERVER_2_IP_ADDR_BYTE0;

    sprintf(g_Setting.tGSM_SET.tServerInfo[1].cDomainName, "%s", TCP_DNS_NAME_DEF);

    g_Setting.tGSM_SET.tServerInfo[1].u16Port = (uint16_t)TCP_DNS_PORT_DEF;

    g_Setting.tGSM_SET.tServerInfo[1].bEnableChk = (bool)SERVER_2_ENABLE_CHK;
    g_Setting.tGSM_SET.tServerInfo[1].bEnableLog = (bool)SERVER_2_ENABLE_LOG;
    g_Setting.tGSM_SET.tServerInfo[1].bEnableEvent = (bool)SERVER_2_ENABLE_EVENT;

    sprintf(g_Setting.tGSM_SET.tServerInfo[1].cHeaderChk, "%s", (char *)SERVER_2_HEADER_CHK);
    sprintf(g_Setting.tGSM_SET.tServerInfo[1].cHeaderLog, "%s", (char *)SERVER_2_HEADER_LOG);
    sprintf(g_Setting.tGSM_SET.tServerInfo[1].cHeaderEvent, "%s", (char *)SERVER_2_HEADER_EVENT);

    g_Setting.tGSM_SET.tServerInfo[1].u8LogPeriod = (uint8_t)SERVER_2_SEND_NORMAL;

    g_Setting.tGSM_SET.bDisableSocketEXT = FALSE;

    g_Setting.tGSM_SET.b4G_WakeUp = TRUE;
    // #endif

    // GSM FTP setting
    sprintf(g_Setting.tGSM_SET.tFTP_Set[0].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME);
    g_Setting.tGSM_SET.tFTP_Set[0].u16Port = (uint16_t)FTP_SERVER_PORT;
    sprintf(g_Setting.tGSM_SET.tFTP_Set[0].cUserName, "%s", (char *)FTP_ACC_USERNAME);
    sprintf(g_Setting.tGSM_SET.tFTP_Set[0].cPassword, "%s", (char *)FTP_ACC_PASSWORD);

    sprintf(g_Setting.tGSM_SET.tFTP_Set[1].cAddr, "%s", (char *)FTP_SERVER_DOMAIN_NAME_EXT);
    g_Setting.tGSM_SET.tFTP_Set[1].u16Port = (uint16_t)FTP_SERVER_PORT_EXT;
    sprintf(g_Setting.tGSM_SET.tFTP_Set[1].cUserName, "%s", (char *)FTP_ACC_USERNAME_EXT);
    sprintf(g_Setting.tGSM_SET.tFTP_Set[1].cPassword, "%s", (char *)FTP_ACC_PASSWORD_EXT);

    g_Setting.bIOT_SIM_En = FALSE;

#if ENABLE_MULTI_FTP_SERVER
    sprintf(g_Setting.cFTP_VDR_Path, "%s", FOLDERNAME_VDR_2);
#else
    sprintf(g_Setting.cFTP_VDR_Path, "%s", FOLDERNAME_VDR);
#endif

    // Optional date classification for VDR file path
    g_Setting.u8FTP_VDR_Mode = 0;
#endif

    // Drive setting //
    g_Setting.tDRV_SET.ttDrvWorkStartTime = 0;

    // g_Setting.tDRV_SET.u16DrvResponse = 0;

    g_Setting.tDRV_SET.u8DriveThreshold[0] = 4;
    g_Setting.tDRV_SET.u8DriveThreshold[1] = 0;

    g_Setting.tDRV_SET.u8RestThreshold[0] = 0;
    g_Setting.tDRV_SET.u8RestThreshold[1] = 30;

    // Driver info //
    g_Setting.tDrvInfo[0].u32DrvIdNum = INVALID_VALUE_DEF;
    g_Setting.tDrvInfo[1].u32DrvIdNum = INVALID_VALUE_DEF;

    g_Setting.tDrvInfo[0].u8DrvStatus = 0;
    g_Setting.tDrvInfo[1].u8DrvStatus = 0;

    g_Setting.tDrvInfo[0].dDriverDistance = 0;
    g_Setting.tDrvInfo[1].dDriverDistance = 0;

    g_Setting.tDrvInfo[0].u32DrWorkTime = 0;
    g_Setting.tDrvInfo[1].u32DrWorkTime = 0;

    g_Setting.tDrvInfo[0].u32DrRestTime = 0;
    g_Setting.tDrvInfo[1].u32DrRestTime = 0;

    for (int i = 0; i < 14; i++)
    {
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32WorkTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32PauseTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32StandByTime = 0;
        g_Setting.tDrvInfo[0].stDrvRecDay[i].u32RestTime = 0;

        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32WorkTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32PauseTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32StandByTime = 0;
        g_Setting.tDrvInfo[1].stDrvRecDay[i].u32RestTime = 0;
    }

    // Power mode //
    g_Setting.tPowerMode = PWR_SAVE;

    // Speed setting
    g_Setting.tSpeedSet.u16Trig = 0;   // Heinz 50
    g_Setting.tSpeedSet.u8Offset = 0;  // 35->3.5KM
    g_Setting.tSpeedSet.u16Star = 250; /// 700->70KM.
    g_Setting.tSpeedSet.fGain = 0;     // Heinz
    g_Setting.tSpeedSet.fAutoGain = 0; // Heinz
    g_Setting.tSpeedSet.u8Auto_Gain_Count = 128;
    g_Setting.tSpeedSet.ttAgain_Time = 1924905600;
    g_Setting.tSpeedSet.ttAutoGainTimeOut = 0;
    g_Setting.tSpeedSet.u8AutoGainInit = 0;
    g_Setting.tSpeedSet.u8AutoGainSrc = AUTOGAIN_SOURCE_GPS;

    g_Setting.tSpeedSet.u16SPEED_LIMIT = 1200;
    g_Setting.tSpeedSet.u16OVER_SPEED_END_CNT_MAX = 5;
    g_Setting.tSpeedSet.u16ACCEL_LIMIT = 30;
    g_Setting.tSpeedSet.u16DECEL_LIMIT = 30;
    g_Setting.tSpeedSet.u8ACCEL_LIM_DURA = 5;

    for (int i = 0; i < SPEED_LIMIT_BANK_MAX; i++)
        g_Setting.tSpeedSet.u32Limit[i] = 0;

    // RPM setting
    g_Setting.tRPM_Set.u16DIV = 1;
    g_Setting.tRPM_Set.u16Trig = 0; // Heinz 50

    g_Setting.tRPM_Set.u16DIV = 10;
    g_Setting.tRPM_Set.u16Limit = 2000;
    g_Setting.tRPM_Set.u16RPM_OVER_ON_CNT_MAX = 60;
    g_Setting.tRPM_Set.u16RPM_OVER_OFF_CNT_MAX = 2;
    g_Setting.tRPM_Set.u16RPM_IDLE_CNT_MAX = 180;

    // CAN Link Type
    g_Setting.tCanLinkType = LINK_BEGIN;

    // CAN Relink mode
    g_Setting.bCAN_RelinkAll = FALSE;

    // Enable setting
    g_Setting.tENABLE_SET.bits.DRV_ID = 1;
    g_Setting.tENABLE_SET.bits.CAN_SPEED = 0;
    g_Setting.tENABLE_SET.bits.CAN_RPM = 0;
    g_Setting.tENABLE_SET.bits.CAN_DIST = 0;

    // Reverse bits setting
    g_Setting.tReverse.word = 0;

    // Upgrade control
    g_Setting.tUpgrade.bFOTA_Flag = FALSE;
    g_Setting.tUpgrade.bUpgradeNow = FALSE;
    g_Setting.tUpgrade.bUpgradeFlag = FALSE;
    g_Setting.tUpgrade.bUpgrade_SMS = FALSE;
    g_Setting.tUpgrade.bUpgrade_TCP = FALSE;
    g_Setting.tUpgrade.ttLastUpgrade = 0;
    g_Setting.tUpgrade.ttLastInstall = 0;
    g_Setting.tUpgrade.u8FOTA_RetryCnt = 0;

    g_Setting.tSpeedSet.u8MergeSpeed = 0;

    // Last making VDR file time
    g_Setting.ttLastVDR_Time = 0;

    // LCD Saver
    g_Setting.bLCDSaver = FALSE;

    // VDR Now
    g_Setting.bVdrNow = FALSE;

    // DB9 Function Select
    g_Setting.u8DB9Func = 0;

    // AutoGain distance limit
    g_Setting.u16AG_Dist_Limit = 3000;
    g_Setting.u16AG_Dist_Limit_Temp = 512;

    // GPS UART Baudrate
    g_Setting.u32GPS_Baudrate = BR_9600bps;
#if ENABLE_LSM6DSL
    // GSensor Wake up
    g_Setting.tGS_Setting.fScale = 2.0;
    g_Setting.tGS_Setting.fThreSold = 1.5;
    g_Setting.tGS_Setting.u8ThresoldAngle = 30;
    g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg);
    memcpy(g_Setting.tGS_Setting.cAxis_Str, "P", strlen("P"));
#endif
#if ENABLE_GSM_NET
    // GPS XTRA file URL
    sprintf(g_Setting.cXTRA_URL, "%s", (char *)GPS_XTRA_FILE_URL);

// GPS EPO file URL
#if GPS_SERVICE_TYPE_DEF == GPS_GLONASS
    sprintf(g_Setting.cEPO_URL, "%s", (char *)HTTP_EPO_GR_FILE_URL);
#else
    sprintf(g_Setting.cEPO_URL, "%s", (char *)HTTP_EPO_GPS_FILE_URL);
#endif
#endif

    g_Setting.tSpeedSet.u32KGAIN_Val = 0;
    g_Setting.tSpeedSet.bGAIN_ChkDisable = FALSE;

    // External alarm setting
    g_Setting.tExAlarm.bEnableAlm = FALSE;
    g_Setting.tExAlarm.bRepeat = FALSE;
    memset(&g_Setting.tExAlarm.tAlmDate, 0x00, sizeof(RTC_DateTypeDef));
    memset(&g_Setting.tExAlarm.tAlmTime, 0x00, sizeof(RTC_TimeTypeDef));

    // External periodic wake up
    g_Setting.tExPedWkup.bEnableWkup = FALSE;
    g_Setting.tExPedWkup.u32Timeout = HOUR_2_SECOND(1);

#if CUSTOMER_TYPE == CUSTOMER_MASTER
    g_Setting.tENABLE_SET.bits.CAN_RPM = 1;
#endif

#if CUSTOMER_TYPE == CUSTOMER_TANGENG
    g_Setting.tENABLE_SET.bits.DRV_ID = 1;

    g_Setting.tENABLE_SET.bits.CAN_SPEED = 1;

    g_Setting.tENABLE_SET.bits.CAN_RPM = 1;
#endif

#if CUSTOMER_TYPE == CUSTOMER_SKYEYE
    g_Setting.tENABLE_SET.bits.DRV_ID = 1;

#if ENABLE_GSM_NET
    // g_Setting.tGSM_SET.tNetSet.u16SendNormal = 30;

    g_Setting.tGSM_SET.tNetSet.u16SendStandby = 30;
#endif
#endif

#if CUSTOMER_TYPE == CUSTOMER_FORMOSA
    g_Setting.tENABLE_SET.bits.DRV_ID = 1;

    g_Setting.tENABLE_SET.bits.ImmoCtrl = 1; // Heinz

#if ENABLE_GSM_NET
    // g_Setting.tGSM_SET.tNetSet.u16SendNormal = 30; //Heinz 30

    g_Setting.tGSM_SET.tNetSet.u16SendStandby = 0; // MINUTE_2_SECOND(10); // Heinz 0
#endif

    g_Setting.tSpeedSet.u16Trig = 100; // 50; // Heinz 30

    g_Setting.tSpeedSet.fGain = 107; // Heinz // 112

    g_Setting.tRPM_Set.u16DIV = 30; // Heinz

// FCFC
#if ENABLE_GSM_NET
    g_Setting.tGSM_SET.tServerInfo[0].u32ADDR._U8[3] = 122;
    g_Setting.tGSM_SET.tServerInfo[0].u32ADDR._U8[2] = 146;
    g_Setting.tGSM_SET.tServerInfo[0].u32ADDR._U8[1] = 31;
    g_Setting.tGSM_SET.tServerInfo[0].u32ADDR._U8[0] = 217;
    g_Setting.tGSM_SET.tServerInfo[0].u16Port = 10224; // 10227

#if ENABLE_MULTI_SERVER
    g_Setting.tGSM_SET.tServerInfo[1].u32ADDR._U8[3] = 223;
    g_Setting.tGSM_SET.tServerInfo[1].u32ADDR._U8[2] = 200;
    g_Setting.tGSM_SET.tServerInfo[1].u32ADDR._U8[1] = 80;
    g_Setting.tGSM_SET.tServerInfo[1].u32ADDR._U8[0] = 54;
    g_Setting.tGSM_SET.tServerInfo[1].u16Port = 9030;
#endif
#endif

#endif

#if CUSTOMER_TYPE == CUSTOMER_FPTC
    g_Setting.tENABLE_SET.bits.DRV_ID = 1;

    g_Setting.tENABLE_SET.bits.ImmoCtrl = 1;

#if ENABLE_GSM_NET
    // g_Setting.tGSM_SET.tNetSet.u16SendNormal = 30;

    g_Setting.tGSM_SET.tNetSet.u16SendStandby = 0;
#endif
#endif
}

//============================================================================//
// @brief  Setup system when initialize.
// @param  None.
// @retval None.
//============================================================================//
static void System_InitSetup(void)
{
    bool bRes;
    uint32_t u32SettingSize;

    memset(&g_SysInfo, 0x00, sizeof(gSysInfoTypeDef));

    g_SysInfo.tVehicleState.bPWR_ON = TRUE;
    g_SysInfo.tSpeedInfo.bIsOverSpeed = FALSE;
    g_SysInfo.tSpeedInfo.bIsRPM_Idle = FALSE;

    bRes = System_GetSetting();

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System get g_Setting ERROR. Setting g_Setting to default.");

        System_SetupDefault();

        System_RestoreBackupSetting(VAR_ALL_ITEMS);

        bSettingInitFinish = TRUE;

        System_SaveSettingAll();

        System_SaveBackupSettingAll(VAR_ALL_ITEMS);
    }

    u32SettingSize = sizeof(gSettingTypeDef);

    if (u32SettingSize != g_Setting.u32SetSize)
    {
        RTOSprintf("\r\n> System g_Setting size changed. Setting g_Setting to default.");

        System_SetupDefault();

        System_RestoreBackupSetting(VAR_ALL_ITEMS);

        bSettingInitFinish = TRUE;

        System_SaveSettingAll();

        System_SaveBackupSettingAll(VAR_ALL_ITEMS);
    }

    bSettingInitFinish = TRUE;

    System_RestoreBackupSetting(VAR_DISTANCE);

    g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;

    u8DrvPreStatus[0] = g_Setting.tDrvInfo[0].u8DrvStatus;
    u8DrvPreStatus[1] = g_Setting.tDrvInfo[1].u8DrvStatus;

#if ENABLE_GSM_NET
    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        if (g_Setting.tGSM_SET.tServerInfo[i].u8LogPeriod > 0)
        {
            u8DATALOG_UPLOAD_MAX[i] = g_Setting.tGSM_SET.tServerInfo[i].u8LogPeriod;
        }
    }
#endif
}

//============================================================================//
// @brief  Initialize system.
// @param  None.
// @retval None.
//============================================================================//
void System_Init(void)
{
    MX25L_StatusTypeDef mxlRes;
    // uint8_t u8MX25L_RestartCnt;
    bool bRes;

    // HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
    // HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);
    // HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

    System_IO_3V3_Disable();

    HAL_Delay(500);

    System_IO_3V3_Enable();

    HAL_Delay(100);

    EXTIO_BatteryEnable();

    UART_SetupAll();

    DWT_Init();

    bPrintfEn = TRUE;

    UART_SetupAll();
#if ENABLE_MG82
    UART_EXT_SetupAll();
#endif
    // HAL_PWR_EnableBkUpAccess();
    uint16_t u16ADC_Val = EXTIO_GetADC1(ADC_MOTOR_PWR_CHANNEL);

    if (u16ADC_Val < 0x200)
    {
        RTOSprintf("\r\n> ### NO MAIN POWER. SYSTEM STOP ###");

        System_IO_3V3_Disable();

        bSystemStop = TRUE;

        MG82_PWR_Mode(MG82_PWR_MODE_DEF);

        System_Reset();

        return;
    }
    RTOSprintf("\r\n> ### SYSTEM INIT ###");

#if ENABLE_MG82
    MG82_Wakeup();
#endif

#if ENABLE_INDEX_BUFFER
    if (bSD_ReportErrFlag == FALSE)
        V161_IndexFileNameInit();
#endif

    HAL_Delay(500);

    mxlRes = MX25L_Init();

    // RTOSprintf("\r\n> MX25L initialization status:%d", mxlRes);

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> MX25L initialization ERROR!");

        RTOSprintf("\r\n> MX25L reset.");

        MX25L_Restart();
    }

    if ((HW_PBGetState(ACC_IN) == 0))
    {
        LCD_Init();

        // Show auto test
        LCD_SetShowAutoTestPage(30);
    }
#if ENABLE_MG82
    // if(HW_PBGetState(ACC_IN) == 1)
    {
        bRes = MG82_Init();

        // RTOSprintf("\r\n> MG82 initialization status: %d", bRes);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> MG82 initialization ERROR!");

            RTOSprintf("\r\n> MG82 reset.");

            MG82_Restart();
        }
    }
#endif
    Power_SourceEnable();

#if ENABLE_MG82_RTC
    // if(HW_PBGetState(ACC_IN) == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            bRes = MG82_RTC_GetDateTime(&tRTC_InitDate, &tRTC_InitTime, 1);

            if (bRes)
                break;
        }

        if (bRes == TRUE)
        {
            if ((tRTC_InitDate.Year > 19) && (tRTC_InitDate.Year < 40))
            {
                bMG82_RTC_EN = TRUE;

                // g_u8HwInfo = (g_u8HwInfo | 0x10);

                // RTOSprintf("\r\n> Check IC RTC OK. (0x%02X)", g_u8HwInfo);

                // bSystemTimeReady = TRUE;
            }
            else
            {
                bMG82_RTC_EN = FALSE;

                // g_u8HwInfo = (g_u8HwInfo & 0xEF);

                // RTOSprintf("\r\n> RTC IC time not stored. (0x%02X)", g_u8HwInfo);
            }
        }
        else
        {
            bMG82_RTC_EN = FALSE;

            RTOSprintf("\r\n> MG82 RTC initialization FAIL!");

            RTOSprintf("\r\n> MG82 reset.");

            MG82_Restart();
        }
    }
#elif ENABLE_RX8130
    bRes = RX8130_Init();

    bRes = TRUE;

    if (bRes)
    {
        for (int i = 0; i < 3; i++)
        {
            bRes = RX8130_GetDateTime(&tRTC_InitDate, &tRTC_InitTime, 1);

            if (bRes)
                break;
        }
    }

    if (bRes == TRUE)
    {
        if ((tRTC_InitDate.Year > 19) && (tRTC_InitDate.Year < 40))
        {
            bRX8130_EN = TRUE;

            // g_u8HwInfo = (g_u8HwInfo | 0x10);

            // RTOSprintf("\r\n> Check RTC IC OK. (0x%02X)", g_u8HwInfo);

            // bSystemTimeReady = TRUE;
        }
        else
        {
            bRX8130_EN = FALSE;

            // g_u8HwInfo = (g_u8HwInfo & 0xEF);

            // RTOSprintf("\r\n> RTC IC time not stored. (0x%02X)", g_u8HwInfo);
        }
    }
    else
    {
        bRX8130_EN = FALSE;

        RTOSprintf("\r\n> RX8130 initialization FAIL!");

        RTOSprintf("\r\n> RX8130 reset.");

        RX8130_Restart();
    }
#else
    // RTOSprintf("\r\n> Check RTC IC time.");

    for (int i = 0; i < 3; i++)
    {
        bRes = PCF_GetDate(&tRTC_InitDate, 1);

        if (bRes)
            break;
    }

    for (int i = 0; i < 3; i++)
    {
        bRes = PCF_GetTime(&tRTC_InitTime, 1);

        if (bRes)
            break;
    }

    if (bRes == TRUE)
    {
        if ((tRTC_InitDate.Year > 19) && (tRTC_InitDate.Year < 40))
        {
            g_bPCF_EN = TRUE;

            // g_u8HwInfo = (g_u8HwInfo | 0x10);

            // RTOSprintf("\r\n> Check RTC IC OK. (0x%02X)", g_u8HwInfo);

            // bSystemTimeReady = TRUE;
        }
        else
        {
            g_bPCF_EN = FALSE;

            // g_u8HwInfo = (g_u8HwInfo & 0xEF);

            // RTOSprintf("\r\n> RTC IC time not stored. (0x%02X)", g_u8HwInfo);
        }
    }
    else
    {
        g_bPCF_EN = FALSE;

        RTOSprintf("\r\n> PCF initialization FAIL!");

        RTOSprintf("\r\n> PCF reset.");

        PCF_Restart();
    }
#endif

    bRes = SD_Init();

    // RTOSprintf("\r\n> SD card initialization status:%d", bRes);

    if (!bRes)
    {
        RTOSprintf("\r\n> SD card initialization ERROR!");

        RTOSprintf("\r\n> SD reset.");

        SD_Reset();
    }

    if (bSD_ReportErrFlag == FALSE)
    {
        // RTOSprintf("\r\n> Check SETTING files");

        System_CheckSettingFile();
    }

    // RTOSprintf("\r\n> System init setup.");

    System_InitSetup();

    if (bSD_ReportErrFlag == FALSE)
    {
        // RTOSprintf("\r\n> Check TEMP files");

        System_CheckTempFile();

        if (SD_CheckDirExist(FOLDERNAME_VDR) == FALSE)
        {
            bRes = SD_MakeDir(FOLDERNAME_VDR);

            if (bRes)
                RTOSprintf("\r\n> Make VDR directory OK");
        }
    }

    System_FlagCheck();

    System_DisableResetAfterStandby();

    if (g_Setting.bVdrNow == FALSE)
    {
        if (SD_CheckFileExist(FILENAME_VDR_NOW_TMP) == TRUE)
        {
            SD_RemoveFile(FILENAME_VDR_NOW_TMP);
        }
    }

    // RTOSprintf("\r\n> LCD init");

#if ENABLE_GSM_NET
    if (strncmp(g_Setting.cJAS_Ver, (char *)JAS208_NO_GSM, 7) == 0)
    {
        RTOSprintf("\r\n> DETECTED GSM FUNCTION ENABLED, CHANGE VERSION TO JAS208U.");

        memset(g_Setting.cJAS_Ver, 0x00, sizeof(g_Setting.cJAS_Ver));

        sprintf(g_Setting.cJAS_Ver, "%s", JAS208_GSM);

        System_SaveSetting();
    }

#if ENABLE_MG82
    QtEC21_PwrDisable();

    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    HAL_Delay(100);

    QtEC21_WakeUp();

    HAL_Delay(100);

    QtEC21_PwrEnable();

    QtEC21_Reset(); // add reset to make sure wakeup GSM
#else
    GSM_BlockCmdInit();

    GSM_NonBlockCmdInit();

    HW_PBSet(GSM_WAKEUP); // Pull N_WAKEUP pin to low to wake up the module

    HAL_Delay(100);

    bQtEC21_State = TRUE;

    QtEC21_TurnOnPwrKey();

    HAL_Delay(100);

    HW_PBClr(GSM_WAKEUP); // Pull N_WAKEUP pin to high
#endif
#else
    if (strncmp(g_Setting.cJAS_Ver, (char *)JAS208_GSM, 7) == 0)
    {
        RTOSprintf("\r\n> DETECTED GSM FUNCTION DISABLED, CHANGE VERSION TO JAS208S.");

        memset(g_Setting.cJAS_Ver, 0x00, sizeof(g_Setting.cJAS_Ver));

        sprintf(g_Setting.cJAS_Ver, "%s", JAS208_NO_GSM);

        System_SaveSetting();
    }
#endif

    // RTOSprintf("\r\n> EXTIO init");

    EXTIO_Init();

    // RTOSprintf("\r\n> GPS Init");

#if ENABLE_GPS_MODULE && GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    GPS_PowerOn();
#endif

    // RTOSprintf("\r\n> PRN init");

    PRN_Init();

    // Close Admin mode
    g_Setting.tENABLE_SET.bits.ENG_MODE = 0;

    // Close Calibration mode
    g_Setting.tENABLE_SET.bits.CORRECT_MODE = 0;

    if (g_Setting.tUpgrade.bUpgradeFlag == TRUE)
    {
        RTOSprintf("\r\n> Have Upgrade, Reflash FW Version");

        if (bSD_ReportErrFlag == FALSE)
            V161_RecordTechnical(3);

        g_Setting.tUpgrade.bUpgradeFlag = FALSE;

        System_SaveSetting();
    }

    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYSTEM_INIT_OK);

    // Set Last GPS fix X & Y
    GPS_LastCoordinateProcess(GPS_POS_LOAD);

    if (HW_PBGetState(ACC_IN) == 1) // ACC off
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_ON, (char *)"0");
#endif
#if ENABLE_LSM6DSL
        if (u8WkupSrc == MG82_WKUP_SRC_G_SEN)
        {
            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_WAKEUP, (char *)"0");
        }
#endif
    }
    else
    {
#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_ON, STR_NULL);
#endif

        HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
        HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);
        HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    }

    if ((EXTIO_IsPowerNormal() == TRUE) && (HW_PBGetState(ACC_IN) == 1))
    {
        bSystemMode = SYSTEM_MODE_STEALTH;
    }
}

//============================================================================//
// @brief  Deinitialize system.
// @param  None.
// @retval None.
//============================================================================//
void System_DeInit(void)
{
    // SD_DeInitComm();

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    GPS_PowerOff();
#endif

    // #if ENABLE_GSM_NET
    // QtEC21_TurnOffPwrKey();
    // #endif

    // HAL_Delay(S_2_mS(1));

#if ENABLE_INDEX_BUFFER
    if ((bSD_ReportErrFlag == FALSE) && (u8SD_RetryCnt == 0) && (u8SD_ErrCnt == 0))
    {
        // printf("\r\n> ### Save file index before enter standby mode ###");

        V161_SaveFileIdxAll();
    }
#endif

    System_IO_3V3_Disable();
}

//============================================================================//
// @brief  System software reset.
// @param  None.
// @retval None.
//============================================================================//
void System_Reset(void)
{
    System_DeInit();

    // Generate system reset
    NVIC_SystemReset();
}

//============================================================================//
// @brief  Get system time.
// @param  None.
// @retval System time in time_t format.
//============================================================================//
time_t System_GetTime(void)
{
    time_t ttTimeNow;
    osStatus osRes;

    if (bSysInitFinish)
    {
        // osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

        // if(osRes != osOK)
        //	return FALSE;

        ttTimeNow = g_SysInfo.ttNow;

        // osMutexRelease(mutex_ttnow_user);
    }
    else
    {
        struct tm tmRTCTimeInfo;
        RTC_TimeTypeDef rtcTime;
        RTC_DateTypeDef rtcDate;

#if ENABLE_MG82_RTC
        if (bMG82_RTC_EN == TRUE)
        {
            // Get current date time in Binary format
            if (!MG82_RTC_GetDateTime(&rtcDate, &rtcTime, 1))
            {
                RTOSprintf("\r\n> Get system date time ERROR.");

                return 0;
            }
        }
#elif ENABLE_RX8130
        // If using IC RX8130 as time source
        if (bRX8130_EN == TRUE)
        {
            // Get current date time in Binary format
            if (!RX8130_GetDateTime(&rtcDate, &rtcTime, 1))
            {
                RTOSprintf("\r\n> Get system date time ERROR.");

                return 0;
            }
        }
#else
        // If using IC PCF as time source
        if (g_bPCF_EN == TRUE)
        {
            // Get current time in Binary format
            if (!PCF_GetTime(&rtcTime, 1))
            {
                RTOSprintf("\r\n> Get system time ERROR.");

                return 0;
            }

            // Get current date in Binary format
            if (!PCF_GetDate(&rtcDate, 1))
            {
                RTOSprintf("\r\n> Get system date ERROR.");

                return 0;
            }
        }
#endif
        // If using RTC as time source
        else
        {
            if (bRTOS_En)
            {
                osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

                if (osRes != osOK)
                    return 0;
            }

            // Get current time in Binary format
            if (HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK)
            {
                RTOSprintf("\r\n> Get system time ERROR.");

                if (bRTOS_En)
                    osMutexRelease(mutex_ttnow_user);

                return 0;
            }

            // Get current date in Binary format
            if (HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK)
            {
                RTOSprintf("\r\n> Get system date ERROR.");

                if (bRTOS_En)
                    osMutexRelease(mutex_ttnow_user);

                return 0;
            }

            if (bRTOS_En)
                osMutexRelease(mutex_ttnow_user);
        }

        // Year in tm struct is counted from 1900
        tmRTCTimeInfo.tm_year = rtcDate.Year + 2000 - 1900U;
        // Month in tm struct is counted from 0 corresponding to Jan
        tmRTCTimeInfo.tm_mon = rtcDate.Month - 1U;
        tmRTCTimeInfo.tm_mday = rtcDate.Date;
        tmRTCTimeInfo.tm_hour = rtcTime.Hours;
        tmRTCTimeInfo.tm_min = rtcTime.Minutes;
        tmRTCTimeInfo.tm_sec = rtcTime.Seconds;
        tmRTCTimeInfo.tm_isdst = -1;

        ttTimeNow = mktime(&tmRTCTimeInfo);
    }

    return ttTimeNow;
}

//============================================================================//
// @brief  Save g_Setting data to ExFlash
// @param  pgSettingBuf: Pointer to a gSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_SaveSettingFlash(gSettingTypeDef *pgSettingBuf)
{
    const uint32_t u32Addr = EXFLASH_G_SETTING_START_ADDR;
    uint16_t u16SectEraseNum = 0;
    MX25L_StatusTypeDef mxlRes;
    uint8_t u8BCDTime[7] = {0};
    time_t ttSaveTime;

    if (bSettingInitFinish == FALSE)
        return TRUE;

    // RTOSprintf("\r\n> ### Sytem_SaveSettingFlash ###");

    if (bSystemTimeReady == TRUE)
        ttSaveTime = System_GetTime();
    else
        ttSaveTime = 1;

    SecToBCDTime(ttSaveTime, u8BCDTime);

    u16SectEraseNum = sizeof(gSettingTypeDef) / MX25L_SECTOR_SIZE;

    if ((sizeof(gSettingTypeDef) % MX25L_SECTOR_SIZE) > 0)
        u16SectEraseNum++;

    // RTOSprintf("\r\n> System save Setting to Flash - number of sectors erased:%d", u16SectEraseNum);

    mxlRes = MX25L_MultiSectorsErase((u32Addr / MX25L_SECTOR_SIZE),
                                     u16SectEraseNum);

    // RTOSprintf("\r\n> System save Setting to Flash - start sector:%d, number of sector erased:%d", u32Addr / MX25L_SECTOR_SIZE, u16SectEraseNum);

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System save setting to Flash - erase ERROR:%d.", mxlRes);

        return FALSE;
    }

    pgSettingBuf->ttLastSaveTime = ttSaveTime;

    mxlRes = MX25L_MultiPagesProgram((u32Addr / MX25L_PAGE_SIZE),
                                     (uint8_t *)pgSettingBuf, sizeof(gSettingTypeDef));

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System save Setting to Flash - write ERROR:%d.", mxlRes);

        return FALSE;
    }

    if (bSystemTimeReady == TRUE)
    {
        RTOSprintf("\r\n> ### %02X%02X/%02X/%02X %02X:%02X:%02X SYSTEM SAVE SETTING TO FLASH OK ###",
                   u8BCDTime[0],
                   u8BCDTime[1],
                   u8BCDTime[2],
                   u8BCDTime[3],
                   u8BCDTime[4],
                   u8BCDTime[5],
                   u8BCDTime[6]);
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM SAVE SETTING TO FLASH OK ###");
    }

    return TRUE;
}

//============================================================================//
// @brief  Save g_Setting data to SD
// @param  pgSettingBuf: Pointer to a gSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_SaveSettingSD(gSettingTypeDef *pgSettingBuf)
{
    uint8_t u8BCDTime[7] = {0};
    time_t ttSaveTime;
    char cFileName[64] = {0};
    bool bRes;

    if (bSettingInitFinish == FALSE)
        return TRUE;

    // RTOSprintf("\r\n> ### Sytem_SaveSettingSD ###");

    if (bSystemTimeReady == TRUE)
        ttSaveTime = System_GetTime();
    else
        ttSaveTime = 1;

    SecToBCDTime(ttSaveTime, u8BCDTime);

    pgSettingBuf->ttLastSaveTime = ttSaveTime;

    sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_G_SETTING);

    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)pgSettingBuf, sizeof(gSettingTypeDef), 0);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System save Setting to SD - write ERROR.");

        return FALSE;
    }

    if (bSystemTimeReady == TRUE)
    {
        RTOSprintf("\r\n> ### %02X%02X/%02X/%02X %02X:%02X:%02X SYSTEM SAVE SETTING TO SD OK ###",
                   u8BCDTime[0],
                   u8BCDTime[1],
                   u8BCDTime[2],
                   u8BCDTime[3],
                   u8BCDTime[4],
                   u8BCDTime[5],
                   u8BCDTime[6]);
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM SAVE SETTING TO SD OK ###");
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the g_Setting data from ExFlash.
// @param  pgSettingBuf: Pointer to a gSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_GetSettingFlash(gSettingTypeDef *pgSettingBuf)
{
    bool bRes = FALSE;
    const uint32_t u32Addr = EXFLASH_G_SETTING_START_ADDR;
    MX25L_StatusTypeDef mxlRes;

    mxlRes = MX25L_MultiBytesRead((uint32_t)u32Addr, (uint8_t *)pgSettingBuf, sizeof(gSettingTypeDef));

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System get Flash Setting ERROR:%d.", mxlRes);

        return FALSE;
    }

    bRes = TRUE;

    if (pgSettingBuf->u8SetVer != SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get Flash Setting - SetVer1 invalid: %d.", pgSettingBuf->u8SetVer);

        bRes = FALSE;
    }

    if (pgSettingBuf->u8SetVerChk != NOT_SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get Flash Setting - SetVer2 invalid: %d.", pgSettingBuf->u8SetVerChk);

        bRes = FALSE;
    }

    if (pgSettingBuf->u32SetSize != sizeof(gSettingTypeDef))
    {
        RTOSprintf("\r\n> System get Flash Setting - size not match: %d -> %d",
                   pgSettingBuf->u32SetSize,
                   sizeof(gSettingTypeDef));

        bRes = FALSE;
    }

    if (pgSettingBuf->ttLastSaveTime == 0)
    {
        RTOSprintf("\r\n> System get Flash Setting - last save time is 0.");

        bRes = FALSE;
    }

    if (bRes == TRUE)
    {
        // RTOSprintf("\r\n> System get Flash Setting OK. Last save time: %d", pgSettingBuf->ttLastSaveTime);
    }

    return bRes;
}

//============================================================================//
// @brief  Get the g_Setting data from SD card.
// @param  pgSettingBuf: Pointer to a gSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_GetSettingSD(gSettingTypeDef *pgSettingBuf)
{
    bool bRes = FALSE;
    char cFileName[64] = {0};

    sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_G_SETTING);

    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)pgSettingBuf, sizeof(gSettingTypeDef), 0);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System get SD Setting ERROR.");

        return FALSE;
    }

    bRes = TRUE;

    if (pgSettingBuf->u8SetVer != SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get SD Setting - SetVer1 invalid: %d.", pgSettingBuf->u8SetVer);

        bRes = FALSE;
    }

    if (pgSettingBuf->u8SetVerChk != NOT_SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get SD Setting - SetVer2 invalid: %d.", pgSettingBuf->u8SetVerChk);

        bRes = FALSE;
    }

    if (pgSettingBuf->u32SetSize != sizeof(gSettingTypeDef))
    {
        RTOSprintf("\r\n> System get SD Setting - size not match: %d -> %d",
                   pgSettingBuf->u32SetSize,
                   sizeof(gSettingTypeDef));

        bRes = FALSE;
    }

    if (pgSettingBuf->ttLastSaveTime == 0)
    {
        RTOSprintf("\r\n> System get SD Setting - last save time is 0.");

        bRes = FALSE;
    }

    if (bRes == TRUE)
    {
        // RTOSprintf("\r\n> System get SD Setting OK. Last save time: %d", pgSettingBuf->ttLastSaveTime);
    }

    return bRes;
}

//============================================================================//
// @brief  Save Backup Setting data to ExFlash
// @param  pbkpSettingBuf: Pointer to a BackupSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_SaveBackupSettingFlash(BackupSettingTypeDef *pbkpSettingBuf)
{
    const uint32_t u32Addr = EXFLASH_BACKUP_SETTING_START_ADDR;
    uint16_t u16SectEraseNum = 0;
    MX25L_StatusTypeDef mxlRes;
    uint8_t u8BCDTime[7] = {0};
    time_t ttSaveTime;

    if (bSettingInitFinish == FALSE)
        return TRUE;

    if (bSystemTimeReady == TRUE)
        ttSaveTime = System_GetTime();
    else
        ttSaveTime = 1;

    SecToBCDTime(ttSaveTime, u8BCDTime);

    u16SectEraseNum = sizeof(BackupSettingTypeDef) / MX25L_SECTOR_SIZE;

    if ((sizeof(BackupSettingTypeDef) % MX25L_SECTOR_SIZE) > 0)
        u16SectEraseNum++;

    // RTOSprintf("\r\n> System save Backup Setting to Flash - number of sectors erased:%d", u16SectEraseNum);

    mxlRes = MX25L_MultiSectorsErase((u32Addr / MX25L_SECTOR_SIZE),
                                     u16SectEraseNum);

    // RTOSprintf("\r\n> System save Backup Setting to Flash - start sector:%d, number of sector erased:%d", u32Addr / MX25L_SECTOR_SIZE, u16SectEraseNum);

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System save Backup Setting to Flash - erase ERROR:%d.", mxlRes);

        return FALSE;
    }

    pbkpSettingBuf->ttLastSaveTime = ttSaveTime;

    mxlRes = MX25L_MultiPagesProgram((u32Addr / MX25L_PAGE_SIZE),
                                     (uint8_t *)pbkpSettingBuf, sizeof(BackupSettingTypeDef));

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System save Backup Setting to Flash - write ERROR:%d.", mxlRes);

        return FALSE;
    }

    if (bSystemTimeReady == TRUE)
    {
        RTOSprintf("\r\n> ### %02X%02X/%02X/%02X %02X:%02X:%02X SYSTEM SAVE BACKUP SETTING TO FLASH OK ###",
                   u8BCDTime[0],
                   u8BCDTime[1],
                   u8BCDTime[2],
                   u8BCDTime[3],
                   u8BCDTime[4],
                   u8BCDTime[5],
                   u8BCDTime[6]);
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM SAVE BACKUP SETTING TO FLASH OK ###");
    }

    return TRUE;
}

//============================================================================//
// @brief  Save Backup Setting data to SD
// @param  pbkpSettingBuf: Pointer to a BackupSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_SaveBackupSettingSD(BackupSettingTypeDef *pbkpSettingBuf)
{
    uint8_t u8BCDTime[7] = {0};
    time_t ttSaveTime;
    char cFileName[64] = {0};
    bool bRes;

    if (bSettingInitFinish == FALSE)
        return TRUE;

    // RTOSprintf("\r\n> ### Sytem_SaveBackupSettingSD ###");

    if (bSystemTimeReady == TRUE)
        ttSaveTime = System_GetTime();
    else
        ttSaveTime = 1;

    SecToBCDTime(ttSaveTime, u8BCDTime);

    pbkpSettingBuf->ttLastSaveTime = ttSaveTime;

    sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_BACKUP_SETTING);

    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)pbkpSettingBuf, sizeof(BackupSettingTypeDef), 0);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System save Backup Setting to SD ERROR.");

        return FALSE;
    }

    if (bSystemTimeReady == TRUE)
    {
        RTOSprintf("\r\n> ### %02X%02X/%02X/%02X %02X:%02X:%02X SYSTEM SAVE BACKUP SETTING TO SD OK ###",
                   u8BCDTime[0],
                   u8BCDTime[1],
                   u8BCDTime[2],
                   u8BCDTime[3],
                   u8BCDTime[4],
                   u8BCDTime[5],
                   u8BCDTime[6]);
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM SAVE BACKUP SETTING TO SD OK ###");
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the Backup Setting data from ExFlash.
// @param  pbkpSettingBuf: Pointer to a BackupSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_GetBackupSettingFlash(BackupSettingTypeDef *pbkpSettingBuf)
{
    bool bRes = FALSE;
    const uint32_t u32Addr = EXFLASH_BACKUP_SETTING_START_ADDR;
    MX25L_StatusTypeDef mxlRes;

    mxlRes = MX25L_MultiBytesRead((uint32_t)u32Addr, (uint8_t *)pbkpSettingBuf, sizeof(BackupSettingTypeDef));

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System get Flash Backup Setting ERROR:%d.", mxlRes);

        return FALSE;
    }

    bRes = TRUE;

    if (pbkpSettingBuf->u8Ver != BACKUP_SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get Flash Backup Setting - Version invalid: %d", pbkpSettingBuf->u8Ver);

        bRes = FALSE;
    }

    if (pbkpSettingBuf->ttLastSaveTime == 0)
    {
        RTOSprintf("\r\n> System get Flash Backup Setting - last save time is 0.");

        bRes = FALSE;
    }

    // if(bRes == TRUE)
    //	RTOSprintf("\r\n> System get Flash Backup Setting OK. Last save time: %d", pbkpSettingBuf->ttLastSaveTime);

    return bRes;
}

//============================================================================//
// @brief  Get the Backup Setting data from SD card.
// @param  pgSettingTemp: Pointer to a BackupSettingTypeDef variable.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
static bool System_GetBackupSettingSD(BackupSettingTypeDef *pbkpSettingBuf)
{
    bool bRes = FALSE;
    char cFileName[64] = {0};

    sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_BACKUP_SETTING);

    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)pbkpSettingBuf, sizeof(BackupSettingTypeDef), 0);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System get SD Backup Setting ERROR.");

        return FALSE;
    }

    bRes = TRUE;

    if (pbkpSettingBuf->u8Ver != BACKUP_SETUP_VERSION)
    {
        RTOSprintf("\r\n> System get SD Backup Setting - Version invalid: %d", pbkpSettingBuf->u8Ver);

        bRes = FALSE;
    }

    if (pbkpSettingBuf->ttLastSaveTime == 0)
    {
        RTOSprintf("\r\n> System get SD Backup Setting - last save time is 0.");

        bRes = FALSE;
    }

    // if(bRes == TRUE)
    //	RTOSprintf("\r\n> System get SD Backup Setting OK. Last save time: %d", pbkpSettingBuf->ttLastSaveTime);

    return bRes;
}

//============================================================================//
// @brief  Copy backup setting data from g_Setting to Backup Setting storage in
//         SD every 1 minute.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveBackupSetting(void)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    bRes = System_GetBackupSettingSD(&bkpSettingTemp1);

    if (bRes == FALSE)
    {
        // RTOSprintf("\r\n> System save Backup Setting - get Backup Setting ERROR.");

        memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));
    }

    memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

    memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

    if (bkpSettingTemp1.u8Ver != BACKUP_SETUP_VERSION)
        bkpSettingTemp1.u8Ver = BACKUP_SETUP_VERSION;

    bkpSettingTemp1.u32Size = sizeof(BackupSettingTypeDef);

    memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));
    memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));
#if ENABLE_GSM_NET
    memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));
    memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));
    memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));
    memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));
    memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));
    memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));
    memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));
    memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));
    memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));
    memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));
#endif

    bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;
    strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);
    strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);
    bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;
    bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;
    bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;
    bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;
    bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;
#if ENABLE_GSM_NET
    bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
    bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
    bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
    bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
    bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
    strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
    strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
    bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
    bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
    bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
    bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
    bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
    strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
    strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
    bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
    strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
    strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
    strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
    bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
    strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
    strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
    strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
    bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif
    bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;
    bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;
    bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];
    bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];
    bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];
    bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;
    bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;
    bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;
    bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;
    memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
    sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);
    memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
    sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);
    bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;
    bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;
    bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;
    bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;
    bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;
    bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;
    bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

    bRes = System_SaveBackupSettingSD(&bkpSettingTemp1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System save Backup Setting ERROR.");
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

//============================================================================//
// @brief  Copy backup setting data from g_Setting to Backup Setting storage in
//         Flash before the system is reset or shutdown.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveBackupSettingFinal(void)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    bRes = System_GetBackupSettingFlash(&bkpSettingTemp1);

    if (bRes == FALSE)
    {
        // RTOSprintf("\r\n> System save Backup Setting Final - get Backup Setting ERROR.");

        memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));
    }

    {
        memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

        memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

        if (bkpSettingTemp1.u8Ver != BACKUP_SETUP_VERSION)
            bkpSettingTemp1.u8Ver = BACKUP_SETUP_VERSION;

        bkpSettingTemp1.u32Size = sizeof(BackupSettingTypeDef);

        memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));
        memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));
#if ENABLE_GSM_NET
        memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));
        memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));
        memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));
        memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));
        memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));
        memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));
        memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));
        memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));
        memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));
        memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));
#endif

        bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;
        strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);
        strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);
        bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;
        bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;
        bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;
        bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;
        bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;
#if ENABLE_GSM_NET
        bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
        bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
        bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
        bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
        bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
        strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
        strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
        bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
        bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
        bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
        bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
        bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
        strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
        strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
        bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
        strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
        strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
        strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
        bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
        strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
        strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
        strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
        bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif
        bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;
        bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;
        bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];
        bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];
        bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];
        bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;
        bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;
        bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;
        bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;
        memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
        sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);
        memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
        sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);
        bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;
        bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;
        bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;
        bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;
        bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;
        bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;
        bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

        bRes = System_SaveBackupSettingFlash(&bkpSettingTemp1);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> System save Backup Setting Final ERROR.");
        }
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

//============================================================================//
// @brief  Copy backup setting data from g_Setting to Backup Setting storage in
//         ExFlash and SD.
// @param  tItem: ID of the setting to save in SettingItemTypeDef format.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveBackupSettingAll(SettingItemTypeDef tItem)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    bRes = System_GetBackupSettingFlash(&bkpSettingTemp1);

    if (bRes == FALSE)
    {
        // RTOSprintf("\r\n> System save Backup Setting item %d to Flash - get Backup Setting ERROR.", tItem);

        memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));
    }

    {
        memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

        memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

        if (bkpSettingTemp1.u8Ver != BACKUP_SETUP_VERSION)
            bkpSettingTemp1.u8Ver = BACKUP_SETUP_VERSION;

        bkpSettingTemp1.u32Size = sizeof(BackupSettingTypeDef);

        switch ((uint8_t)tItem)
        {
        case VAR_DISTANCE:
            bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;

            break;

        case VAR_BUSID:
            memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));

            strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);

            break;

        case VAR_VIN:
            memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));

            strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);

            break;

        case VAR_SPEED_GAIN:
            bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;

            break;

        case VAR_RPM_DIV:
            bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;

            break;

        case VAR_CAN_LINK:
            bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;

            break;

        case VAR_CAN_SPEED:
            bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;

            break;

        case VAR_CAN_RPM:
            bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;

            break;

        case VAR_GPRS_SERVER_IP:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
#endif

            break;

        case VAR_GPRS_SERVER_PORT:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
#endif

            break;

        case VAR_DRIVER_ID1:

            bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;

            break;

        case VAR_DRIVER_ID2:

            bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;

            break;

        case VAR_SPEED_LIMIT1:

            bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];

            break;

        case VAR_SPEED_LIMIT2:

            bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];

            break;

        case VAR_SPEED_LIMIT3:

            bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];

            break;

        case VAR_CAL_TIME:

            bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;

            break;

        case VAR_GPRS_APN:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));

            strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
#endif

            break;

        case VAR_GPRS_DNS:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));

            strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
#endif

            break;

        case VAR_GPRS_SERVER_IP_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
#endif

            break;

        case VAR_GPRS_SERVER_PORT_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
#endif

            break;

        case VAR_GPRS_DNS_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));

            strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
#endif

            break;

        case VAR_GPRS_FTP_ADDR:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));

            strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
#endif

            break;

        case VAR_GPRS_FTP_PORT:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));

            strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));

            strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
#endif

            break;

        case VAR_GPRS_FTP_ADDR_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));

            strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
#endif

            break;

        case VAR_GPRS_FTP_PORT_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));

            strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));

            strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
#endif

            break;

        case VAR_FTP_VDR_PATH:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));

            strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
#endif

            break;

        case VAR_FTP_VDR_DATE_EN:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif

            break;

        case VAR_LCD_SAVER:
            bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;

            break;

        case VAR_VDR_NOW:
            bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;

            break;

        case VAR_DB9_FUNC:
            bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;

            break;

        case VAR_XTRA_URL:
            memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
            sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);

            break;

        case VAR_EPO_URL:
            memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
            sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);

            break;

        case VAR_SOCKET_EXT:
            bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;

            break;

        case VAR_4G_WKUP:
            bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;

            break;

        case VAR_EX_ALM:
            bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;

            break;

        case VAR_EX_PED_WKUP:
            bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;

            break;

        case VAR_PWR_DOWN:
            bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;

            break;

        case VAR_KGAIN:
            bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;

            break;

        case VAR_GAIN_CHK_DIS:
            bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

            break;

        case VAR_ALL_ITEMS:
            memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));
            memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));
            memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));
            memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));
            memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));
            memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));
            memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));
            memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));
            memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));
            memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));
            memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));
#endif

            bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;
            strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);
            strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);
            bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;
            bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;
            bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;
            bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;
            bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
            bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
            strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
            strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
            bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
            bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
            strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
            strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
            bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
            strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
            strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
            strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
            bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
            strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
            strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
            strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
            bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif
            bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;
            bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;
            bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];
            bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];
            bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];
            bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;
            bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;
            bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;
            bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;
            memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
            sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);
            memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
            sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);
            bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;
            bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;
            bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;
            bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;
            bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;
            bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;
            bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

            break;

        default:
            break;
        }

        bRes = System_SaveBackupSettingFlash(&bkpSettingTemp1);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> System save Backup Setting item %d to Flash ERROR.", tItem);
        }
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    bRes = System_GetBackupSettingSD(&bkpSettingTemp1);

    if (bRes == FALSE)
    {
        // RTOSprintf("\r\n> System save Backup Setting item %d to SD - get Backup Setting ERROR.", tItem);

        memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));
    }

    {
        memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

        memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

        if (bkpSettingTemp1.u8Ver != BACKUP_SETUP_VERSION)
            bkpSettingTemp1.u8Ver = BACKUP_SETUP_VERSION;

        bkpSettingTemp1.u32Size = sizeof(BackupSettingTypeDef);

        switch ((uint8_t)tItem)
        {
        case VAR_DISTANCE:
            bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;

            break;

        case VAR_BUSID:
            memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));

            strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);

            break;

        case VAR_VIN:
            memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));

            strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);

            break;

        case VAR_SPEED_GAIN:
            bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;

            break;

        case VAR_RPM_DIV:
            bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;

            break;

        case VAR_CAN_LINK:
            bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;

            break;

        case VAR_CAN_SPEED:
            bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;

            break;

        case VAR_CAN_RPM:
            bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;

            break;

        case VAR_GPRS_SERVER_IP:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
#endif

            break;

        case VAR_GPRS_SERVER_PORT:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
#endif

            break;

        case VAR_DRIVER_ID1:

            bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;

            break;

        case VAR_DRIVER_ID2:

            bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;

            break;

        case VAR_SPEED_LIMIT1:

            bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];

            break;

        case VAR_SPEED_LIMIT2:

            bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];

            break;

        case VAR_SPEED_LIMIT3:

            bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];

            break;

        case VAR_CAL_TIME:

            bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;

            break;

        case VAR_GPRS_APN:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));

            strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
#endif

            break;

        case VAR_GPRS_DNS:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));

            strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
#endif

            break;

        case VAR_GPRS_SERVER_IP_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
#endif

            break;

        case VAR_GPRS_SERVER_PORT_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
#endif

            break;

        case VAR_GPRS_DNS_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));

            strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
#endif

            break;

        case VAR_GPRS_FTP_ADDR:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));

            strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
#endif

            break;

        case VAR_GPRS_FTP_PORT:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));

            strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));

            strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
#endif

            break;

        case VAR_GPRS_FTP_ADDR_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));

            strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
#endif

            break;

        case VAR_GPRS_FTP_PORT_2:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));

            strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD_2:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));

            strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
#endif

            break;

        case VAR_FTP_VDR_PATH:
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));

            strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
#endif

            break;

        case VAR_FTP_VDR_DATE_EN:
#if ENABLE_GSM_NET
            bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif

            break;

        case VAR_LCD_SAVER:
            bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;

            break;

        case VAR_VDR_NOW:
            bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;

            break;

        case VAR_DB9_FUNC:
            bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;

            break;

        case VAR_XTRA_URL:
            memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
            sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);

            break;

        case VAR_EPO_URL:
            memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
            sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);

            break;

        case VAR_SOCKET_EXT:
            bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;

            break;

        case VAR_4G_WKUP:
            bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;

            break;

        case VAR_EX_ALM:
            bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;

            break;

        case VAR_EX_PED_WKUP:
            bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;

            break;

        case VAR_PWR_DOWN:
            bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;

            break;

        case VAR_KGAIN:
            bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;

            break;

        case VAR_GAIN_CHK_DIS:
            bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

            break;

        case VAR_ALL_ITEMS:
            memset(bkpSettingTemp1.cBus_ID, 0x00, sizeof(bkpSettingTemp1.cBus_ID));
            memset(bkpSettingTemp1.cVIN, 0x00, sizeof(bkpSettingTemp1.cVIN));
#if ENABLE_GSM_NET
            memset(bkpSettingTemp1.cAPN, 0x00, sizeof(bkpSettingTemp1.cAPN));
            memset(bkpSettingTemp1.cDomainName, 0x00, sizeof(bkpSettingTemp1.cDomainName));
            memset(bkpSettingTemp1.cDomainName2, 0x00, sizeof(bkpSettingTemp1.cDomainName2));
            memset(bkpSettingTemp1.cFTP_Addr, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr));
            memset(bkpSettingTemp1.cFTP_UserName, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName));
            memset(bkpSettingTemp1.cFTP_Password, 0x00, sizeof(bkpSettingTemp1.cFTP_Password));
            memset(bkpSettingTemp1.cFTP_Addr2, 0x00, sizeof(bkpSettingTemp1.cFTP_Addr2));
            memset(bkpSettingTemp1.cFTP_UserName2, 0x00, sizeof(bkpSettingTemp1.cFTP_UserName2));
            memset(bkpSettingTemp1.cFTP_Password2, 0x00, sizeof(bkpSettingTemp1.cFTP_Password2));
            memset(bkpSettingTemp1.cFTP_VDR_Path, 0x00, sizeof(bkpSettingTemp1.cFTP_VDR_Path));
#endif

            bkpSettingTemp1.dDistance = g_SysInfo.tVehicleState.dDistance;
            strcpy(bkpSettingTemp1.cBus_ID, g_SettingTemp1.tBusInfo.cBus_ID);
            strcpy(bkpSettingTemp1.cVIN, g_SettingTemp1.tBusInfo.cVIN);
            bkpSettingTemp1.fGain = g_SettingTemp1.tSpeedSet.fGain;
            bkpSettingTemp1.u16DIV = g_SettingTemp1.tRPM_Set.u16DIV;
            bkpSettingTemp1.tCanLink = g_SettingTemp1.tCanLinkType;
            bkpSettingTemp1.u8CanSpeed = g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED;
            bkpSettingTemp1.u8CanRpm = g_SettingTemp1.tENABLE_SET.bits.CAN_RPM;
#if ENABLE_GSM_NET
            bkpSettingTemp1.u32IP_Addr._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3];
            bkpSettingTemp1.u16Port = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port;
            strcpy(bkpSettingTemp1.cAPN, g_SettingTemp1.tGSM_SET.tNetSet.cAPN);
            strcpy(bkpSettingTemp1.cDomainName, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName);
            bkpSettingTemp1.u32IP_Addr2._U8[0] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0];
            bkpSettingTemp1.u32IP_Addr2._U8[1] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1];
            bkpSettingTemp1.u32IP_Addr2._U8[2] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2];
            bkpSettingTemp1.u32IP_Addr2._U8[3] = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3];
            bkpSettingTemp1.u16Port2 = g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port;
            strcpy(bkpSettingTemp1.cDomainName2, g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName);
            strcpy(bkpSettingTemp1.cFTP_Addr, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr);
            bkpSettingTemp1.u16FTP_Port = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port;
            strcpy(bkpSettingTemp1.cFTP_UserName, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName);
            strcpy(bkpSettingTemp1.cFTP_Password, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword);
            strcpy(bkpSettingTemp1.cFTP_Addr2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr);
            bkpSettingTemp1.u16FTP_Port2 = g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port;
            strcpy(bkpSettingTemp1.cFTP_UserName2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName);
            strcpy(bkpSettingTemp1.cFTP_Password2, g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword);
            strcpy(bkpSettingTemp1.cFTP_VDR_Path, g_SettingTemp1.cFTP_VDR_Path);
            bkpSettingTemp1.u8FTP_VDR_Mode = g_SettingTemp1.u8FTP_VDR_Mode;
#endif
            bkpSettingTemp1.u32DrvIdNum1 = g_SettingTemp1.tDrvInfo[0].u32DrvIdNum;
            bkpSettingTemp1.u32DrvIdNum2 = g_SettingTemp1.tDrvInfo[1].u32DrvIdNum;
            bkpSettingTemp1.u32Limit1 = g_SettingTemp1.tSpeedSet.u32Limit[0];
            bkpSettingTemp1.u32Limit2 = g_SettingTemp1.tSpeedSet.u32Limit[1];
            bkpSettingTemp1.u32Limit3 = g_SettingTemp1.tSpeedSet.u32Limit[2];
            bkpSettingTemp1.ttCalibrationTime = g_SettingTemp1.tSpeedSet.ttAgain_Time;
            bkpSettingTemp1.bLCDSaver = g_SettingTemp1.bLCDSaver;
            bkpSettingTemp1.bVdrNow = g_SettingTemp1.bVdrNow;
            bkpSettingTemp1.u8DB9Func = g_SettingTemp1.u8DB9Func;
            memset(bkpSettingTemp1.cXTRA_URL, 0x00, sizeof(bkpSettingTemp1.cXTRA_URL));
            sprintf(bkpSettingTemp1.cXTRA_URL, "%s", g_Setting.cXTRA_URL);
            memset(bkpSettingTemp1.cEPO_URL, 0x00, sizeof(bkpSettingTemp1.cEPO_URL));
            sprintf(bkpSettingTemp1.cEPO_URL, "%s", g_Setting.cEPO_URL);
            bkpSettingTemp1.bDisableSocketEXT = g_Setting.tGSM_SET.bDisableSocketEXT;
            bkpSettingTemp1.b4G_WakeUp = g_Setting.tGSM_SET.b4G_WakeUp;
            bkpSettingTemp1.tExAlarm = g_Setting.tExAlarm;
            bkpSettingTemp1.tExPedWkup = g_Setting.tExPedWkup;
            bkpSettingTemp1.tPwrDown = g_Setting.tPwrDown;
            bkpSettingTemp1.u32KGAIN_Val = g_Setting.tSpeedSet.u32KGAIN_Val;
            bkpSettingTemp1.bGAIN_ChkDisable = g_Setting.tSpeedSet.bGAIN_ChkDisable;

            break;

        default:
            break;
        }

        bRes = System_SaveBackupSettingSD(&bkpSettingTemp1);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> System save Backup Setting item %d to SD ERROR.", tItem);
        }
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return TRUE;
}

//============================================================================//
// @brief  Copy backup setting data from Backup storage in ExFlash or SD to
//         g_Setting.
// @param  tItem: ID of the setting to save in SettingItemTypeDef format.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_RestoreBackupSetting(SettingItemTypeDef tItem)
{
    bool bRes, bRes1, bRes2;
    uint32_t u32RestoreSize = 0, u32NewSize = 0, u32DatCnt = 0;
    static bool bDoOnce = FALSE;
    bool bSetMCU_TimeFlag = FALSE;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    memset(&bkpSettingTemp2, 0x00, sizeof(BackupSettingTypeDef));

    bRes1 = System_GetBackupSettingFlash(&bkpSettingTemp1);

    bRes2 = System_GetBackupSettingSD(&bkpSettingTemp2);

    if ((bRes1 == TRUE) && (bRes2 == TRUE))
    {
        if (bkpSettingTemp1.ttLastSaveTime >= bkpSettingTemp2.ttLastSaveTime)
        {
            RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM FLASH OK (1) ###", tItem);
        }
        else
        {
            memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

            memcpy(&bkpSettingTemp1, &bkpSettingTemp2, sizeof(BackupSettingTypeDef));

            RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM SD OK (1) ###", tItem);
        }

        bRes = TRUE;
    }
    else if ((bRes1 == TRUE) && (bRes2 == FALSE))
    {
        RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM FLASH OK (2) ###", tItem);

        bRes = TRUE;
    }
    else if ((bRes1 == FALSE) && (bRes2 == TRUE))
    {
        memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

        memcpy(&bkpSettingTemp1, &bkpSettingTemp2, sizeof(BackupSettingTypeDef));

        RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM SD OK (2) ###", tItem);

        bRes = TRUE;
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM FLASH AND SD BOTH ERROR ###", tItem);

        bRes = FALSE;
    }

    if (bRes == TRUE)
    {
        memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));
        memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

        u32RestoreSize = bkpSettingTemp1.u32Size;
        u32NewSize = sizeof(BackupSettingTypeDef);

        // If the backup setting size is different between previous and current version
        if (u32RestoreSize < u32NewSize)
        {
            RTOSprintf("\r\n> System new Backup Setting struct detected. Old size: %d, New size: %d",
                       u32RestoreSize,
                       u32NewSize);

            memset((uint8_t *)(&bkpSettingTemp1) + u32RestoreSize,
                   0x00,
                   (u32NewSize - u32RestoreSize));
        }

        switch ((uint8_t)tItem)
        {
        case VAR_DISTANCE:
            g_SysInfo.tVehicleState.dDistance = bkpSettingTemp1.dDistance;

            break;

        case VAR_BUSID:
            memset(g_SettingTemp1.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));

            strcpy(g_SettingTemp1.tBusInfo.cBus_ID, bkpSettingTemp1.cBus_ID);

            break;

        case VAR_VIN:
            memset(g_SettingTemp1.tBusInfo.cVIN, 0x00, sizeof(g_Setting.tBusInfo.cVIN));

            strcpy(g_SettingTemp1.tBusInfo.cVIN, bkpSettingTemp1.cVIN);

            break;

        case VAR_SPEED_GAIN:
            g_SettingTemp1.tSpeedSet.fGain = bkpSettingTemp1.fGain;

            break;

        case VAR_RPM_DIV:
            g_SettingTemp1.tRPM_Set.u16DIV = bkpSettingTemp1.u16DIV;

            break;

        case VAR_CAN_LINK:
            g_SettingTemp1.tCanLinkType = bkpSettingTemp1.tCanLink;

            break;

        case VAR_CAN_SPEED:
            g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED = bkpSettingTemp1.u8CanSpeed;

            break;

        case VAR_CAN_RPM:
            g_SettingTemp1.tENABLE_SET.bits.CAN_RPM = bkpSettingTemp1.u8CanRpm;

            break;

        case VAR_GPRS_SERVER_IP:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr._U8[0];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr._U8[1];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr._U8[2];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr._U8[3];

            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, 0x00,
                   sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));

            sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                    bkpSettingTemp1.u32IP_Addr._U8[3],
                    bkpSettingTemp1.u32IP_Addr._U8[2],
                    bkpSettingTemp1.u32IP_Addr._U8[1],
                    bkpSettingTemp1.u32IP_Addr._U8[0]);
#endif

            break;

        case VAR_GPRS_SERVER_PORT:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16Port;
#endif

            break;

        case VAR_DRIVER_ID1:
            g_SettingTemp1.tDrvInfo[0].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum1;

            break;

        case VAR_DRIVER_ID2:
            g_SettingTemp1.tDrvInfo[1].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum2;

            break;

        case VAR_SPEED_LIMIT1:
            g_SettingTemp1.tSpeedSet.u32Limit[0] = bkpSettingTemp1.u32Limit1;

            break;

        case VAR_SPEED_LIMIT2:
            g_SettingTemp1.tSpeedSet.u32Limit[1] = bkpSettingTemp1.u32Limit2;

            break;

        case VAR_SPEED_LIMIT3:
            g_SettingTemp1.tSpeedSet.u32Limit[2] = bkpSettingTemp1.u32Limit3;

            break;

        case VAR_CAL_TIME:
            g_SettingTemp1.tSpeedSet.ttAgain_Time = bkpSettingTemp1.ttCalibrationTime;

            break;

        case VAR_GPRS_APN:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, 0x00, sizeof(g_Setting.tGSM_SET.tNetSet.cAPN));

            strcpy(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, bkpSettingTemp1.cAPN);
#endif

            break;

        case VAR_GPRS_DNS:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                   0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));

            strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, bkpSettingTemp1.cDomainName);
#endif

            break;

        case VAR_GPRS_SERVER_IP_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr2._U8[0];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr2._U8[1];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr2._U8[2];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr2._U8[3];

            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, 0x00,
                   sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr));

            sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, "%d.%d.%d.%d",
                    bkpSettingTemp1.u32IP_Addr2._U8[3],
                    bkpSettingTemp1.u32IP_Addr2._U8[2],
                    bkpSettingTemp1.u32IP_Addr2._U8[1],
                    bkpSettingTemp1.u32IP_Addr2._U8[0]);
#endif

            break;

        case VAR_GPRS_SERVER_PORT_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16Port2;
#endif

            break;

        case VAR_GPRS_DNS_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                   0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName));

            strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, bkpSettingTemp1.cDomainName2);
#endif

            break;

        case VAR_GPRS_FTP_ADDR:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, bkpSettingTemp1.cFTP_Addr);
#endif

            break;

        case VAR_GPRS_FTP_PORT:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16FTP_Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName, bkpSettingTemp1.cFTP_UserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword, bkpSettingTemp1.cFTP_Password);
#endif

            break;

        case VAR_GPRS_FTP_ADDR_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, bkpSettingTemp1.cFTP_Addr2);
#endif

            break;

        case VAR_GPRS_FTP_PORT_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16FTP_Port2;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName, bkpSettingTemp1.cFTP_UserName2);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword, bkpSettingTemp1.cFTP_Password2);
#endif

            break;

        case VAR_FTP_VDR_PATH:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.cFTP_VDR_Path, 0x00, sizeof(g_SettingTemp1.cFTP_VDR_Path));

            strcpy(g_SettingTemp1.cFTP_VDR_Path, bkpSettingTemp1.cFTP_VDR_Path);
#endif

            break;

        case VAR_FTP_VDR_DATE_EN:
#if ENABLE_GSM_NET
            g_SettingTemp1.u8FTP_VDR_Mode = bkpSettingTemp1.u8FTP_VDR_Mode;
#endif

            break;

        case VAR_LCD_SAVER:
            g_SettingTemp1.bLCDSaver = bkpSettingTemp1.bLCDSaver;

            break;

        case VAR_VDR_NOW:
            g_SettingTemp1.bVdrNow = bkpSettingTemp1.bVdrNow;

            break;

        case VAR_DB9_FUNC:
            g_SettingTemp1.u8DB9Func = bkpSettingTemp1.u8DB9Func;

            break;

        case VAR_XTRA_URL:
            memset(g_SettingTemp1.cXTRA_URL, 0x00, sizeof(g_SettingTemp1.cXTRA_URL));
            sprintf(g_SettingTemp1.cXTRA_URL, "%s", bkpSettingTemp1.cXTRA_URL);

            break;

        case VAR_EPO_URL:
            memset(g_SettingTemp1.cEPO_URL, 0x00, sizeof(g_SettingTemp1.cEPO_URL));
            sprintf(g_SettingTemp1.cEPO_URL, "%s", bkpSettingTemp1.cEPO_URL);

            break;

        case VAR_SOCKET_EXT:
            g_SettingTemp1.tGSM_SET.bDisableSocketEXT = bkpSettingTemp1.bDisableSocketEXT;

            break;

        case VAR_4G_WKUP:
            g_SettingTemp1.tGSM_SET.b4G_WakeUp = bkpSettingTemp1.b4G_WakeUp;

            break;

        case VAR_EX_ALM:
            g_SettingTemp1.tExAlarm = bkpSettingTemp1.tExAlarm;

            break;

        case VAR_EX_PED_WKUP:
            g_SettingTemp1.tExPedWkup = bkpSettingTemp1.tExPedWkup;

            break;

        case VAR_PWR_DOWN:
            g_SettingTemp1.tPwrDown = bkpSettingTemp1.tPwrDown;

            break;

        case VAR_KGAIN:
            g_SettingTemp1.tSpeedSet.u32KGAIN_Val = bkpSettingTemp1.u32KGAIN_Val;

            break;

        case VAR_GAIN_CHK_DIS:
            g_SettingTemp1.tSpeedSet.bGAIN_ChkDisable = bkpSettingTemp1.bGAIN_ChkDisable;

            break;

        case VAR_ALL_ITEMS:
        {
            ProcStepTypeDef procRestoreStep;
            const uint8_t u32RestoreMaxStep = 20;

            procRestoreStep = PROC_STEP_1;

            u32DatCnt = 0;

            while ((procRestoreStep <= u32RestoreMaxStep) &&
                   (u32DatCnt < u32RestoreSize))
            {
                RTOSprintf("\r\n> System restore backup setting step %d", procRestoreStep);

                switch ((int)procRestoreStep)
                {
                case PROC_STEP_1:
                    g_SysInfo.tVehicleState.dDistance = bkpSettingTemp1.dDistance;
                    memset(g_SettingTemp1.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));
                    strcpy(g_SettingTemp1.tBusInfo.cBus_ID, bkpSettingTemp1.cBus_ID);
                    memset(g_SettingTemp1.tBusInfo.cVIN, 0x00, sizeof(g_Setting.tBusInfo.cVIN));
                    strcpy(g_SettingTemp1.tBusInfo.cVIN, bkpSettingTemp1.cVIN);
                    g_SettingTemp1.tSpeedSet.fGain = bkpSettingTemp1.fGain;
                    g_SettingTemp1.tRPM_Set.u16DIV = bkpSettingTemp1.u16DIV;
                    g_SettingTemp1.tCanLinkType = bkpSettingTemp1.tCanLink;
                    g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED = bkpSettingTemp1.u8CanSpeed;
                    g_SettingTemp1.tENABLE_SET.bits.CAN_RPM = bkpSettingTemp1.u8CanRpm;
#if ENABLE_GSM_NET
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr._U8[0];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr._U8[1];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr._U8[2];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr._U8[3];
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, 0x00,
                           sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));
                    sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                            bkpSettingTemp1.u32IP_Addr._U8[3],
                            bkpSettingTemp1.u32IP_Addr._U8[2],
                            bkpSettingTemp1.u32IP_Addr._U8[1],
                            bkpSettingTemp1.u32IP_Addr._U8[0]);
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16Port;
#endif
                    g_SettingTemp1.tDrvInfo[0].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum1;
                    g_SettingTemp1.tDrvInfo[1].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum2;
                    g_SettingTemp1.tSpeedSet.u32Limit[0] = bkpSettingTemp1.u32Limit1;
                    g_SettingTemp1.tSpeedSet.u32Limit[1] = bkpSettingTemp1.u32Limit2;
                    g_SettingTemp1.tSpeedSet.u32Limit[2] = bkpSettingTemp1.u32Limit3;
                    g_SettingTemp1.tSpeedSet.ttAgain_Time = bkpSettingTemp1.ttCalibrationTime;
// g_SettingTemp1.bVdrNow = bkpSettingTemp1.u8VdrNow;
#if ENABLE_GSM_NET
                    memset(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, 0x00, sizeof(g_Setting.tGSM_SET.tNetSet.cAPN));
                    strcpy(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, bkpSettingTemp1.cAPN);
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                           0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));
                    strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, bkpSettingTemp1.cDomainName);
#endif

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.cDomainName) + (uint32_t)sizeof(bkpSettingTemp1.cDomainName) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_2:
#if ENABLE_GSM_NET
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr2._U8[0];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr2._U8[1];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr2._U8[2];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr2._U8[3];
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, 0x00,
                           sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr));
                    sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, "%d.%d.%d.%d",
                            bkpSettingTemp1.u32IP_Addr2._U8[3],
                            bkpSettingTemp1.u32IP_Addr2._U8[2],
                            bkpSettingTemp1.u32IP_Addr2._U8[1],
                            bkpSettingTemp1.u32IP_Addr2._U8[0]);
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16Port2;
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                           0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName));
                    strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, bkpSettingTemp1.cDomainName2);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, bkpSettingTemp1.cFTP_Addr);
                    g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16FTP_Port;
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName, bkpSettingTemp1.cFTP_UserName);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword, bkpSettingTemp1.cFTP_Password);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, bkpSettingTemp1.cFTP_Addr2);
                    g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16FTP_Port2;
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName, bkpSettingTemp1.cFTP_UserName2);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword, bkpSettingTemp1.cFTP_Password2);
                    memset(g_SettingTemp1.cFTP_VDR_Path, 0x00, sizeof(g_SettingTemp1.cFTP_VDR_Path));
                    strcpy(g_SettingTemp1.cFTP_VDR_Path, bkpSettingTemp1.cFTP_VDR_Path);
                    g_SettingTemp1.u8FTP_VDR_Mode = bkpSettingTemp1.u8FTP_VDR_Mode;
#endif

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.u8FTP_VDR_Mode) + (uint32_t)sizeof(bkpSettingTemp1.u8FTP_VDR_Mode) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_3:
                    g_SettingTemp1.bLCDSaver = bkpSettingTemp1.bLCDSaver;
                    g_SettingTemp1.bVdrNow = bkpSettingTemp1.bVdrNow;
                    g_SettingTemp1.u8DB9Func = bkpSettingTemp1.u8DB9Func;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.u8DB9Func) + (uint32_t)sizeof(bkpSettingTemp1.u8DB9Func) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_4:
                    memset(g_SettingTemp1.cXTRA_URL, 0x00, sizeof(g_SettingTemp1.cXTRA_URL));
                    sprintf(g_SettingTemp1.cXTRA_URL, "%s", bkpSettingTemp1.cXTRA_URL);

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.cXTRA_URL) + (uint32_t)sizeof(bkpSettingTemp1.cXTRA_URL) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_5:
                    memset(g_SettingTemp1.cEPO_URL, 0x00, sizeof(g_SettingTemp1.cEPO_URL));
                    sprintf(g_SettingTemp1.cEPO_URL, "%s", bkpSettingTemp1.cEPO_URL);
                    g_SettingTemp1.tGSM_SET.bDisableSocketEXT = bkpSettingTemp1.bDisableSocketEXT;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.bDisableSocketEXT) + (uint32_t)sizeof(bkpSettingTemp1.bDisableSocketEXT) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_6:
                    g_SettingTemp1.tGSM_SET.b4G_WakeUp = bkpSettingTemp1.b4G_WakeUp;
                    g_SettingTemp1.tExAlarm = bkpSettingTemp1.tExAlarm;
                    g_SettingTemp1.tExPedWkup = bkpSettingTemp1.tExPedWkup;
                    g_SettingTemp1.tPwrDown = bkpSettingTemp1.tPwrDown;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.tPwrDown) + (uint32_t)sizeof(bkpSettingTemp1.tPwrDown) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_7:
                    g_SettingTemp1.tSpeedSet.u32KGAIN_Val = bkpSettingTemp1.u32KGAIN_Val;
                    g_SettingTemp1.tSpeedSet.bGAIN_ChkDisable = bkpSettingTemp1.bGAIN_ChkDisable;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.bGAIN_ChkDisable) + (uint32_t)sizeof(bkpSettingTemp1.bGAIN_ChkDisable) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                default:
                    break;
                }

                procRestoreStep++;
            }

            break;
        }

        default:
            break;
        }

        memcpy(&g_Setting, &g_SettingTemp1, sizeof(gSettingTypeDef));

        RTOSprintf("\r\n> ### SYSTEM RESTORE BACKUP SETTING OK ###");

#if ENABLE_MG82_RTC
        if (bDoOnce == FALSE && (bMG82_RTC_EN == TRUE))
#elif ENABLE_RX8130
        if (bDoOnce == FALSE && (bRX8130_EN == TRUE))
#else
        if (bDoOnce == FALSE && (g_bPCF_EN == TRUE))
#endif
        {
            struct tm tTimeRTC = {0};
            time_t ttTimeMCU, ttTimeRTC;

            tTimeRTC.tm_year = tRTC_InitDate.Year + 2000 - 1900;
            tTimeRTC.tm_mon = tRTC_InitDate.Month - 1;
            tTimeRTC.tm_mday = tRTC_InitDate.Date;
            tTimeRTC.tm_hour = tRTC_InitTime.Hours;
            tTimeRTC.tm_min = tRTC_InitTime.Minutes;
            tTimeRTC.tm_sec = tRTC_InitTime.Seconds;
            ttTimeRTC = mktime(&tTimeRTC);
            ttTimeMCU = bkpSettingTemp1.ttLastSaveTime;

            if ((ttTimeRTC < ttTimeMCU) || (ttTimeRTC > (ttTimeMCU + DAY_2_SECOND((uint32_t)2 * 365))))
            {
                bSystemTimeReady = FALSE;
#if ENABLE_MG82_RTC
                bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
                bRX8130_EN = FALSE;
#else
                g_bPCF_EN = FALSE;
#endif

                g_u8HwInfo = (g_u8HwInfo & 0xEF);

                RTOSprintf("\r\n> RTC IC time not stored (%d-%d). (0x%02X)", ttTimeRTC, ttTimeMCU, g_u8HwInfo);
            }
            else
            {
                g_u8HwInfo = (g_u8HwInfo | 0x10);

                // bSystemTimeReady = TRUE;
                bSetMCU_TimeFlag = TRUE;

                // RTOSprintf("\r\n> System restore RTC IC time OK.");
            }

            bDoOnce = TRUE;
        }
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    if ((bSetMCU_TimeFlag == TRUE) && (bMCU_RTC_Updated == FALSE))
    {
        RTC_TimeTypeDef rtcTime;
        RTC_DateTypeDef rtcDate;
        HAL_StatusTypeDef halRes;
        osStatus osRes;
        bool bRes2;

        for (int i = 0; i < 4; i++)
        {
#if ENABLE_MG82_RTC
            bRes2 = MG82_RTC_GetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
            bRes2 = RX8130_GetDateTime(&rtcDate, &rtcTime, 1);
#else
            bRes2 = PCF_GetTime(&rtcTime, 1);

            if (bRes2 == FALSE)
                continue;

            bRes2 = PCF_GetDate(&rtcDate, 1);
#endif

            if (bRes2 == TRUE)
                break;
        }

        if (bRes2 == FALSE)
        {
#if ENABLE_MG82_RTC
            bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
            bRX8130_EN = FALSE;
#else
            g_bPCF_EN = FALSE;
#endif

            return bRes;
        }

        if (bRTOS_En)
        {
            osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

            if (osRes != osOK)
            {
#if ENABLE_MG82_RTC
                bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
                bRX8130_EN = FALSE;
#else
                g_bPCF_EN = FALSE;
#endif

                return bRes;
            }
        }

        halRes = HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

        // RTOSprintf("\r\n> Set date result: %d", halRes);

        if (halRes == HAL_OK)
        {
            halRes = HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

            // RTOSprintf("\r\n> Set time result: %d", halRes);
        }

        if (bRTOS_En)
            osMutexRelease(mutex_ttnow_user);

        if (halRes == HAL_OK)
        {
#if ENABLE_MG82_RTC
            bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
            bRX8130_EN = FALSE;
#else
            g_bPCF_EN = FALSE;
#endif

            if (bSystemTimeReady == FALSE)
                bSystemTimeReady = TRUE;

            RTOSprintf("\r\n> Set MCU time from RTC IC OK");
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Copy backup setting data from Backup storage in SD to g_Setting.
// @param  tItem: ID of the setting to save in SettingItemTypeDef format.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_RestoreBackupSettingSD(SettingItemTypeDef tItem)
{
    bool bRes;
    uint32_t u32RestoreSize = 0, u32NewSize = 0, u32DatCnt = 0;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    bRes = System_GetBackupSettingSD(&bkpSettingTemp1);

    if (bRes == TRUE)
    {
        RTOSprintf("\r\n> ### SYSTEM GET BACKUP SETTING ITEM %d FROM SD OK ###", tItem);

        memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));
        memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

        u32RestoreSize = bkpSettingTemp1.u32Size;
        u32NewSize = sizeof(BackupSettingTypeDef);

        // If the backup setting size is different between previous and current version
        if (u32RestoreSize < u32NewSize)
        {
            RTOSprintf("\r\n> System new Backup Setting struct detected. Old size: %d, New size: %d",
                       u32RestoreSize,
                       u32NewSize);

            memset((uint8_t *)(&bkpSettingTemp1) + u32RestoreSize,
                   0x00,
                   (u32NewSize - u32RestoreSize));
        }

        switch ((uint8_t)tItem)
        {
        case VAR_DISTANCE:
            g_SysInfo.tVehicleState.dDistance = bkpSettingTemp1.dDistance;

            break;

        case VAR_BUSID:
            memset(g_SettingTemp1.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));

            strcpy(g_SettingTemp1.tBusInfo.cBus_ID, bkpSettingTemp1.cBus_ID);

            break;

        case VAR_VIN:
            memset(g_SettingTemp1.tBusInfo.cVIN, 0x00, sizeof(g_Setting.tBusInfo.cVIN));

            strcpy(g_SettingTemp1.tBusInfo.cVIN, bkpSettingTemp1.cVIN);

            break;

        case VAR_SPEED_GAIN:
            g_SettingTemp1.tSpeedSet.fGain = bkpSettingTemp1.fGain;

            break;

        case VAR_RPM_DIV:
            g_SettingTemp1.tRPM_Set.u16DIV = bkpSettingTemp1.u16DIV;

            break;

        case VAR_CAN_LINK:
            g_SettingTemp1.tCanLinkType = bkpSettingTemp1.tCanLink;

            break;

        case VAR_CAN_SPEED:
            g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED = bkpSettingTemp1.u8CanSpeed;

            break;

        case VAR_CAN_RPM:
            g_SettingTemp1.tENABLE_SET.bits.CAN_RPM = bkpSettingTemp1.u8CanRpm;

            break;

        case VAR_GPRS_SERVER_IP:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr._U8[0];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr._U8[1];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr._U8[2];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr._U8[3];

            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, 0x00,
                   sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));

            sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                    bkpSettingTemp1.u32IP_Addr._U8[3],
                    bkpSettingTemp1.u32IP_Addr._U8[2],
                    bkpSettingTemp1.u32IP_Addr._U8[1],
                    bkpSettingTemp1.u32IP_Addr._U8[0]);
#endif

            break;

        case VAR_GPRS_SERVER_PORT:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16Port;
#endif

            break;

        case VAR_DRIVER_ID1:
            g_SettingTemp1.tDrvInfo[0].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum1;

            break;

        case VAR_DRIVER_ID2:
            g_SettingTemp1.tDrvInfo[1].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum2;

            break;

        case VAR_SPEED_LIMIT1:
            g_SettingTemp1.tSpeedSet.u32Limit[0] = bkpSettingTemp1.u32Limit1;

            break;

        case VAR_SPEED_LIMIT2:
            g_SettingTemp1.tSpeedSet.u32Limit[1] = bkpSettingTemp1.u32Limit2;

            break;

        case VAR_SPEED_LIMIT3:
            g_SettingTemp1.tSpeedSet.u32Limit[2] = bkpSettingTemp1.u32Limit3;

            break;

        case VAR_CAL_TIME:
            g_SettingTemp1.tSpeedSet.ttAgain_Time = bkpSettingTemp1.ttCalibrationTime;

            break;

        case VAR_GPRS_APN:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, 0x00, sizeof(g_Setting.tGSM_SET.tNetSet.cAPN));

            strcpy(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, bkpSettingTemp1.cAPN);
#endif

            break;

        case VAR_GPRS_DNS:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                   0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));

            strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, bkpSettingTemp1.cDomainName);
#endif

            break;

        case VAR_GPRS_SERVER_IP_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr2._U8[0];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr2._U8[1];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr2._U8[2];
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr2._U8[3];

            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, 0x00,
                   sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr));

            sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, "%d.%d.%d.%d",
                    bkpSettingTemp1.u32IP_Addr2._U8[3],
                    bkpSettingTemp1.u32IP_Addr2._U8[2],
                    bkpSettingTemp1.u32IP_Addr2._U8[1],
                    bkpSettingTemp1.u32IP_Addr2._U8[0]);
#endif

            break;

        case VAR_GPRS_SERVER_PORT_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16Port2;
#endif

            break;

        case VAR_GPRS_DNS_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                   0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName));

            strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, bkpSettingTemp1.cDomainName2);
#endif

            break;

        case VAR_GPRS_FTP_ADDR:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, bkpSettingTemp1.cFTP_Addr);
#endif

            break;

        case VAR_GPRS_FTP_PORT:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16FTP_Port;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName, bkpSettingTemp1.cFTP_UserName);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword, bkpSettingTemp1.cFTP_Password);
#endif

            break;

        case VAR_GPRS_FTP_ADDR_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, bkpSettingTemp1.cFTP_Addr2);
#endif

            break;

        case VAR_GPRS_FTP_PORT_2:
#if ENABLE_GSM_NET
            g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16FTP_Port2;
#endif

            break;

        case VAR_GPRS_FTP_USERNAME_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName, bkpSettingTemp1.cFTP_UserName2);
#endif

            break;

        case VAR_GPRS_FTP_PASSWORD_2:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword,
                   0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword));

            strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword, bkpSettingTemp1.cFTP_Password2);
#endif

            break;

        case VAR_FTP_VDR_PATH:
#if ENABLE_GSM_NET
            memset(g_SettingTemp1.cFTP_VDR_Path, 0x00, sizeof(g_SettingTemp1.cFTP_VDR_Path));

            strcpy(g_SettingTemp1.cFTP_VDR_Path, bkpSettingTemp1.cFTP_VDR_Path);
#endif

            break;

        case VAR_FTP_VDR_DATE_EN:
#if ENABLE_GSM_NET
            g_SettingTemp1.u8FTP_VDR_Mode = bkpSettingTemp1.u8FTP_VDR_Mode;
#endif

            break;

        case VAR_LCD_SAVER:
            g_SettingTemp1.bLCDSaver = bkpSettingTemp1.bLCDSaver;

            break;

        case VAR_VDR_NOW:
            g_SettingTemp1.bVdrNow = bkpSettingTemp1.bVdrNow;

            break;

        case VAR_DB9_FUNC:
            g_SettingTemp1.u8DB9Func = bkpSettingTemp1.u8DB9Func;

            break;

        case VAR_XTRA_URL:
            memset(g_SettingTemp1.cXTRA_URL, 0x00, sizeof(g_SettingTemp1.cXTRA_URL));
            sprintf(g_SettingTemp1.cXTRA_URL, "%s", bkpSettingTemp1.cXTRA_URL);

            break;

        case VAR_EPO_URL:
            memset(g_SettingTemp1.cEPO_URL, 0x00, sizeof(g_SettingTemp1.cEPO_URL));
            sprintf(g_SettingTemp1.cEPO_URL, "%s", bkpSettingTemp1.cEPO_URL);

            break;

        case VAR_SOCKET_EXT:
            g_SettingTemp1.tGSM_SET.bDisableSocketEXT = bkpSettingTemp1.bDisableSocketEXT;

            break;

        case VAR_4G_WKUP:
            g_SettingTemp1.tGSM_SET.b4G_WakeUp = bkpSettingTemp1.b4G_WakeUp;

            break;

        case VAR_EX_ALM:
            g_SettingTemp1.tExAlarm = bkpSettingTemp1.tExAlarm;

            break;

        case VAR_EX_PED_WKUP:
            g_SettingTemp1.tExPedWkup = bkpSettingTemp1.tExPedWkup;

            break;

        case VAR_PWR_DOWN:
            g_SettingTemp1.tPwrDown = bkpSettingTemp1.tPwrDown;

            break;

        case VAR_KGAIN:
            g_SettingTemp1.tSpeedSet.u32KGAIN_Val = bkpSettingTemp1.u32KGAIN_Val;

            break;

        case VAR_GAIN_CHK_DIS:
            g_SettingTemp1.tSpeedSet.bGAIN_ChkDisable = bkpSettingTemp1.bGAIN_ChkDisable;

            break;

        case VAR_ALL_ITEMS:
        {
            ProcStepTypeDef procRestoreStep;
            const uint8_t u32RestoreMaxStep = 20;

            procRestoreStep = PROC_STEP_1;

            u32DatCnt = 0;

            while ((procRestoreStep <= u32RestoreMaxStep) &&
                   (u32DatCnt < u32RestoreSize))
            {
                RTOSprintf("\r\n> System restore backup setting step %d", procRestoreStep);

                switch ((int)procRestoreStep)
                {
                case PROC_STEP_1:
                    g_SysInfo.tVehicleState.dDistance = bkpSettingTemp1.dDistance;
                    memset(g_SettingTemp1.tBusInfo.cBus_ID, 0x00, sizeof(g_Setting.tBusInfo.cBus_ID));
                    strcpy(g_SettingTemp1.tBusInfo.cBus_ID, bkpSettingTemp1.cBus_ID);
                    memset(g_SettingTemp1.tBusInfo.cVIN, 0x00, sizeof(g_Setting.tBusInfo.cVIN));
                    strcpy(g_SettingTemp1.tBusInfo.cVIN, bkpSettingTemp1.cVIN);
                    g_SettingTemp1.tSpeedSet.fGain = bkpSettingTemp1.fGain;
                    g_SettingTemp1.tRPM_Set.u16DIV = bkpSettingTemp1.u16DIV;
                    g_SettingTemp1.tCanLinkType = bkpSettingTemp1.tCanLink;
                    g_SettingTemp1.tENABLE_SET.bits.CAN_SPEED = bkpSettingTemp1.u8CanSpeed;
                    g_SettingTemp1.tENABLE_SET.bits.CAN_RPM = bkpSettingTemp1.u8CanRpm;
#if ENABLE_GSM_NET
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr._U8[0];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr._U8[1];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr._U8[2];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr._U8[3];
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, 0x00,
                           sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr));
                    sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cIP_Addr, "%d.%d.%d.%d",
                            bkpSettingTemp1.u32IP_Addr._U8[3],
                            bkpSettingTemp1.u32IP_Addr._U8[2],
                            bkpSettingTemp1.u32IP_Addr._U8[1],
                            bkpSettingTemp1.u32IP_Addr._U8[0]);
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16Port;
#endif
                    g_SettingTemp1.tDrvInfo[0].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum1;
                    g_SettingTemp1.tDrvInfo[1].u32DrvIdNum = bkpSettingTemp1.u32DrvIdNum2;
                    g_SettingTemp1.tSpeedSet.u32Limit[0] = bkpSettingTemp1.u32Limit1;
                    g_SettingTemp1.tSpeedSet.u32Limit[1] = bkpSettingTemp1.u32Limit2;
                    g_SettingTemp1.tSpeedSet.u32Limit[2] = bkpSettingTemp1.u32Limit3;
                    g_SettingTemp1.tSpeedSet.ttAgain_Time = bkpSettingTemp1.ttCalibrationTime;
// g_SettingTemp1.bVdrNow = bkpSettingTemp1.u8VdrNow;
#if ENABLE_GSM_NET
                    memset(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, 0x00, sizeof(g_Setting.tGSM_SET.tNetSet.cAPN));
                    strcpy(g_SettingTemp1.tGSM_SET.tNetSet.cAPN, bkpSettingTemp1.cAPN);
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName,
                           0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName));
                    strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].cDomainName, bkpSettingTemp1.cDomainName);
#endif

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.cDomainName) + (uint32_t)sizeof(bkpSettingTemp1.cDomainName) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_2:
#if ENABLE_GSM_NET
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[0] = bkpSettingTemp1.u32IP_Addr2._U8[0];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[1] = bkpSettingTemp1.u32IP_Addr2._U8[1];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[2] = bkpSettingTemp1.u32IP_Addr2._U8[2];
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u32IP_Addr._U8[3] = bkpSettingTemp1.u32IP_Addr2._U8[3];
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, 0x00,
                           sizeof(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr));
                    sprintf(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cIP_Addr, "%d.%d.%d.%d",
                            bkpSettingTemp1.u32IP_Addr2._U8[3],
                            bkpSettingTemp1.u32IP_Addr2._U8[2],
                            bkpSettingTemp1.u32IP_Addr2._U8[1],
                            bkpSettingTemp1.u32IP_Addr2._U8[0]);
                    g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16Port2;
                    memset(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName,
                           0x00, sizeof(g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName));
                    strcpy(g_SettingTemp1.tGSM_SET.tServerInfo[SOCKET_BANK_EXT].cDomainName, bkpSettingTemp1.cDomainName2);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cAddr, bkpSettingTemp1.cFTP_Addr);
                    g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].u16Port = bkpSettingTemp1.u16FTP_Port;
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cUserName, bkpSettingTemp1.cFTP_UserName);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_DEF].cPassword, bkpSettingTemp1.cFTP_Password);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cAddr, bkpSettingTemp1.cFTP_Addr2);
                    g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].u16Port = bkpSettingTemp1.u16FTP_Port2;
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cUserName, bkpSettingTemp1.cFTP_UserName2);
                    memset(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword,
                           0x00, sizeof(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword));
                    strcpy(g_SettingTemp1.tGSM_SET.tFTP_Set[SOCKET_BANK_EXT].cPassword, bkpSettingTemp1.cFTP_Password2);
                    memset(g_SettingTemp1.cFTP_VDR_Path, 0x00, sizeof(g_SettingTemp1.cFTP_VDR_Path));
                    strcpy(g_SettingTemp1.cFTP_VDR_Path, bkpSettingTemp1.cFTP_VDR_Path);
                    g_SettingTemp1.u8FTP_VDR_Mode = bkpSettingTemp1.u8FTP_VDR_Mode;
#endif

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.u8FTP_VDR_Mode) + (uint32_t)sizeof(bkpSettingTemp1.u8FTP_VDR_Mode) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_3:
                    g_SettingTemp1.bLCDSaver = bkpSettingTemp1.bLCDSaver;
                    g_SettingTemp1.bVdrNow = bkpSettingTemp1.bVdrNow;
                    g_SettingTemp1.u8DB9Func = bkpSettingTemp1.u8DB9Func;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.u8DB9Func) + (uint32_t)sizeof(bkpSettingTemp1.u8DB9Func) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_4:
                    memset(g_SettingTemp1.cXTRA_URL, 0x00, sizeof(g_SettingTemp1.cXTRA_URL));
                    sprintf(g_SettingTemp1.cXTRA_URL, "%s", bkpSettingTemp1.cXTRA_URL);

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.cXTRA_URL) + (uint32_t)sizeof(bkpSettingTemp1.cXTRA_URL) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_5:
                    memset(g_SettingTemp1.cEPO_URL, 0x00, sizeof(g_SettingTemp1.cEPO_URL));
                    sprintf(g_SettingTemp1.cEPO_URL, "%s", bkpSettingTemp1.cEPO_URL);
                    g_SettingTemp1.tGSM_SET.bDisableSocketEXT = bkpSettingTemp1.bDisableSocketEXT;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.bDisableSocketEXT) + (uint32_t)sizeof(bkpSettingTemp1.bDisableSocketEXT) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_6:
                    g_SettingTemp1.tGSM_SET.b4G_WakeUp = bkpSettingTemp1.b4G_WakeUp;
                    g_SettingTemp1.tExAlarm = bkpSettingTemp1.tExAlarm;
                    g_SettingTemp1.tExPedWkup = bkpSettingTemp1.tExPedWkup;
                    g_SettingTemp1.tPwrDown = bkpSettingTemp1.tPwrDown;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.tPwrDown) + (uint32_t)sizeof(bkpSettingTemp1.tPwrDown) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                case PROC_STEP_7:
                    g_SettingTemp1.tSpeedSet.u32KGAIN_Val = bkpSettingTemp1.u32KGAIN_Val;
                    g_SettingTemp1.tSpeedSet.bGAIN_ChkDisable = bkpSettingTemp1.bGAIN_ChkDisable;

                    u32DatCnt = (uint32_t)(&bkpSettingTemp1.bGAIN_ChkDisable) + (uint32_t)sizeof(bkpSettingTemp1.bGAIN_ChkDisable) - (uint32_t)(&bkpSettingTemp1);

                    // RTOSprintf("\r\n> u32DatCnt: %d", u32DatCnt);

                    break;

                default:
                    break;
                }

                procRestoreStep++;
            }

            break;
        }

        default:
            break;
        }

        memcpy(&g_Setting, &g_SettingTemp1, sizeof(gSettingTypeDef));

        RTOSprintf("\r\n> ### SYSTEM RESTORE BACKUP SETTING OK ###");
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

//============================================================================//
// @brief  Delete the Backup Setting in ExFlash and SD.
// @param  tItem: ID of the setting to save in SettingItemTypeDef format.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
void System_DelBackupSetting(void)
{
    bool bRes;
    char cFileName[64] = {0};
    const uint32_t u32Addr = EXFLASH_BACKUP_SETTING_START_ADDR;
    uint16_t u16SectEraseNum = 0;
    MX25L_StatusTypeDef mxlRes;
    // uint8_t u8BCDTime[7] = {0};
    // time_t ttSaveTime;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return;
    }

    // RTOSprintf("\r\n> ### System delete Backup Setting ###");

    memset(&bkpSettingTemp1, 0x00, sizeof(BackupSettingTypeDef));

    u16SectEraseNum = sizeof(BackupSettingTypeDef) / MX25L_SECTOR_SIZE;

    if ((sizeof(BackupSettingTypeDef) % MX25L_SECTOR_SIZE) > 0)
        u16SectEraseNum++;

    RTOSprintf("\r\n> System delete Flash Backup Setting number of sectors erased:%d", u16SectEraseNum);

    mxlRes = MX25L_MultiSectorsErase((u32Addr / MX25L_SECTOR_SIZE),
                                     u16SectEraseNum);

    RTOSprintf("\r\n> System delete Flash Backup Setting start sector:%d, number of sector erased:%d", u32Addr / MX25L_SECTOR_SIZE, u16SectEraseNum);

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> System delete Backup Setting to Flash erase ERROR:%d.", mxlRes);
    }

    // mxlRes = MX25L_MultiPagesProgram((u32Addr / MX25L_PAGE_SIZE),
    //                                  (uint8_t *)&bkpSettingTemp1, sizeof(BackupSettingTypeDef));

    // if(mxlRes != MX25L_OK)
    //{
    //	RTOSprintf("\r\n> System delete Flash Backup Setting write ERROR:%d.", mxlRes);

    //	return FALSE;
    //}

    sprintf(cFileName, "%s/%s", DIRNAME_SETTING, FILENAME_BACKUP_SETTING);

    bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&bkpSettingTemp1, sizeof(BackupSettingTypeDef), 0);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System delete SD Backup Setting ERROR.");
    }

    if ((mxlRes == MX25L_OK) && (bRes == TRUE))
        RTOSprintf("\r\n> System delete Backup Setting OK.");

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);
}

//============================================================================//
// @brief  Save the g_Setting data every 1 minute.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveSetting(void)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

    memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

    bRes = System_SaveSettingSD(&g_SettingTemp1);

    // if(bRes)
    //{
    //	RTOSprintf("\r\n> System save Setting OK.");
    // }
    // else
    //{
    //   RTOSprintf("\r\n> System save Setting ERROR.");
    // }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

//============================================================================//
// @brief  Save the g_Setting data before the system is reset of shut down.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveSettingFinal(void)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

    memcpy(&g_SettingTemp1, &g_Setting, sizeof(gSettingTypeDef));

    bRes = System_SaveSettingFlash(&g_SettingTemp1);

    // if(bRes)
    //{
    //	RTOSprintf("\r\n> System save Setting Final OK.");
    // }
    // else
    //{
    //   RTOSprintf("\r\n> System save Setting Final ERROR.");
    // }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

//============================================================================//
// @brief  Save the g_Setting data to SD and ExFlash.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_SaveSettingAll(void)
{
    bool bRes1, bRes2;

    bRes1 = System_SaveSetting();

    bRes2 = System_SaveSettingFinal();

    if (bRes1 && bRes2)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Get the Setting data from ExFlash or SDs.
// @param  None.
// @retval TRUE: Operation successful.
//         FALSE: Operation failed.
//============================================================================//
bool System_GetSetting(void)
{
    bool bRes = FALSE, bRes1, bRes2;
    // struct tm tTimeRTC = {0};
    // time_t ttTimeMCU, ttTimeRTC;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
            return FALSE;
    }

    memset(&g_SettingTemp1, 0x00, sizeof(gSettingTypeDef));

    memset(&g_SettingTemp2, 0x00, sizeof(gSettingTypeDef));

    bRes1 = System_GetSettingFlash(&g_SettingTemp1);

    bRes2 = System_GetSettingSD(&g_SettingTemp2);

    if ((bRes1 == TRUE) && (bRes2 == TRUE))
    {
        if (g_SettingTemp1.ttLastSaveTime >= g_SettingTemp2.ttLastSaveTime)
        {
            memcpy(&g_Setting, &g_SettingTemp1, sizeof(gSettingTypeDef));

            RTOSprintf("\r\n> ### SYSTEM GET SETTING FROM FLASH OK (1) ###");
        }
        else
        {
            memcpy(&g_Setting, &g_SettingTemp2, sizeof(gSettingTypeDef));

            RTOSprintf("\r\n> ### SYSTEM GET SETTING FROM SD OK (1) ###");
        }

        bRes = TRUE;
    }
    else if ((bRes1 == TRUE) && (bRes2 == FALSE))
    {
        memcpy(&g_Setting, &g_SettingTemp1, sizeof(gSettingTypeDef));

        RTOSprintf("\r\n> ### SYSTEM GET SETTING FROM FLASH OK (2) ###");

        bRes = TRUE;
    }
    else if ((bRes1 == FALSE) && (bRes2 == TRUE))
    {
        memcpy(&g_Setting, &g_SettingTemp2, sizeof(gSettingTypeDef));

        RTOSprintf("\r\n> ### SYSTEM GET SETTING FROM SD OK (2) ###");

        bRes = TRUE;
    }
    else
    {
        RTOSprintf("\r\n> ### SYSTEM GET SETTING FROM FLASH AND SD BOTH ERROR ###");

        bRes = FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    return bRes;
}

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
//============================================================================//
// @brief  Request to upload backup file to FTP server.
// @param  ttBackupTime: Backup time.
// @retval None.
//============================================================================//
bool System_UploadBackupSettingToFTP(time_t ttBackupTime)
{
    char cUploadFileName[64] = {0};
    uint8_t u8TimeBCD[7] = {0};

    if (strcmp(g_Setting.tBusInfo.cBus_ID, DEFAULT_STRING) == 0)
    {
        RTOSprintf("\r\n> System upload setting to FTP server BUS ID invalid.");

        return FALSE;
    }

    System_SaveBackupSetting();

    SecToBCDTime(ttBackupTime, u8TimeBCD);

    sprintf(cUploadFileName, "%s_%02X%02X%02X%02X.%s",
            g_Setting.tBusInfo.cBus_ID,
            u8TimeBCD[0],
            u8TimeBCD[1],
            u8TimeBCD[2],
            u8TimeBCD[3],
            (char *)BACKUP_SETTING_FTP_EXT);

    if ((u8TimeBCD[1] < 0x22) || (u8TimeBCD[1] > 0x50))
    {
        RTOSprintf("\r\n> System upload setting to FTP server system time invalid.");

        return FALSE;
    }

    GSM_FTP_FileUploadRequest((char *)FILENAME_BACKUP_SETTING,
                              (char *)cUploadFileName,
                              (char *)DIRNAME_SETTING,
                              (char *)DIRNAME_FTP_BACKUP_SETTING,
                              1,
                              0);

    return TRUE;
}

//============================================================================//
// @brief  Restore g_Setting from Backup setting file downloaded from FTP server.
// @param  ttBackupTime: Backup time.
// @retval None.
//============================================================================//
void System_RestoreSettingFromFTP(void)
{
    char cBackupPath[64] = {0},
         cBackupTempPath[64];
    bool bRes;

    sprintf(cBackupPath, "%s/%s",
            (char *)DIRNAME_SETTING,
            (char *)FILENAME_BACKUP_SETTING);

    sprintf(cBackupTempPath, "%s/%s",
            (char *)DIRNAME_SETTING,
            (char *)FILENAME_BACKUP_SETTING_TEMP);

    if (SD_CheckFileExist(cBackupTempPath) == FALSE)
    {
        GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 1");

        return;
    }

    if (SD_GetFileSize(cBackupTempPath) <= 1)
    {
        GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 2");

        return;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_setting_user, S_2_mS(5));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> System restore setting from FTP wait mutex ERROR %d", osRes);

            GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 3");

            return;
        }
    }

    bRes = SD_RemoveFile(cBackupPath);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System restore setting from FTP file remove ERROR.");

        GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 4");

        if (bRTOS_En)
            osMutexRelease(mutex_setting_user);

        return;
    }

    bRes = SD_Rename(cBackupTempPath, cBackupPath);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System restore setting from FTP file rename ERROR.");

        GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 5");

        if (bRTOS_En)
            osMutexRelease(mutex_setting_user);

        return;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_setting_user);

    bRes = System_RestoreBackupSettingSD(VAR_ALL_ITEMS);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> System restore setting from FTP ERROR.");

        GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_FAIL, (char *)"SETTING ERROR 6");

        return;
    }

    System_SaveSettingAll();

    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    RTOSprintf("\r\n> ### SYSTEM RESTORE SETTING FROM FTP SERVER OK ###");

    GSM_TCP_ReportEvent((char *)EVENT_RESTORE_SETTING_OK, (char *)"SETTING OK");
}
#endif

//============================================================================//
// @brief  Put the system into standby mode.
// @param  None.
// @retval None.
//============================================================================//
void System_EnterStandbyMode(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();

    // If Standby flag is set
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
    {
        // Clear Standby flag
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    }

    // If Reset flag is set
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
    {
        // Clear Reset flag
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }

    // Disable all used wakeup sources: Pin1(PA.0)
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

    // Clear all related wakeup flags
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    // Re-enable all used wakeup sources: Pin1(PA.0)
    // HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    for (int i = 0; i < 3; i++)
    {
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

        if (PWR->CSR & PWR_WAKEUP_PIN1)
            break;
    }

    if (bSysInitFinish)
        RTOSprintf("\r\n### ENTER STANDBY MODE ###");

    // Enter standby mode
    HAL_PWR_EnterSTANDBYMode();
}

//============================================================================//
// @brief  Stop all main functions of the system.
// @param  None.
// @retval None.
//============================================================================//
void System_Stop(void)
{
    osTimerStop(timerRTOS_GSM_BlockHandle);
    osTimerStop(timerRTOS_GSM_NonBlockHandle);
    osTimerStop(timerRTOS_EXTIHandle);
    osTimerStop(timerRTOS_PCTransHandle);

    RTOSprintf("\r\n> System STOP!");

    bSystemStop = TRUE;
}

//============================================================================//
// @brief  Erase setup storage section in flash.
// @param  None.
// @retval TRUE: if operation successful.
//         FALSE: if operation fail.
//============================================================================//
bool SystemFLASH_Erase(void)
{
    HAL_StatusTypeDef halRes;
    FLASH_EraseInitTypeDef halFlashEraseInstance;
    uint32_t u32EraseError = 0;

    halFlashEraseInstance.TypeErase = TYPEERASE_SECTORS;
    halFlashEraseInstance.VoltageRange = VOLTAGE_RANGE_3;
    halFlashEraseInstance.Sector = FLASH_SECTOR_11;
    halFlashEraseInstance.NbSectors = 1;

    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
                           FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
                           FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    halRes = HAL_FLASHEx_Erase(&halFlashEraseInstance, &u32EraseError);

    if (halRes != HAL_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> Flash erase error at sector %d with error code %d.", u32EraseError, halRes);
#endif

        RTOSprintf("\r\n> Flash erase error at sector %d with error code %d.", u32EraseError, halRes);

        HAL_FLASH_Lock();

        return FALSE;
    }

    HAL_FLASH_Lock();

    return TRUE;
}

//============================================================================//
// @brief  Check the RDP option.
// @param  None.
// @retval TRUE: RDP is set at level 1.
//         FALSE: RDP is not set (level 0).
//============================================================================//
bool System_CheckRDP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};

    HAL_FLASHEx_OBGetConfig(&obHandle);

    // RTOSprintf("\r\n> RDP value: 0x%X", obHandle.RDPLevel);

    if (obHandle.RDPLevel != OB_RDP_LEVEL_1)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Set the RDP option to 1.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool System_SetRDP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};
    HAL_StatusTypeDef halRes;

    HAL_FLASHEx_OBGetConfig(&obHandle);

    obHandle.OptionType = OPTIONBYTE_RDP;
    obHandle.RDPLevel = OB_RDP_LEVEL_1;

    HAL_FLASH_OB_Unlock();

    halRes = HAL_FLASHEx_OBProgram(&obHandle);

    if (halRes != HAL_OK)
    {
        HAL_FLASH_OB_Lock();

        return FALSE;
    }

    halRes = HAL_FLASH_OB_Launch();

    HAL_FLASH_OB_Lock();

    if (halRes == HAL_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Set the RDP option to 0.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool System_ClrRDP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};
    HAL_StatusTypeDef halRes;

    HAL_FLASHEx_OBGetConfig(&obHandle);

    obHandle.OptionType = OPTIONBYTE_RDP;
    obHandle.RDPLevel = OB_RDP_LEVEL_0;

    HAL_FLASH_OB_Unlock();

    halRes = HAL_FLASHEx_OBProgram(&obHandle);

    if (halRes != HAL_OK)
    {
        HAL_FLASH_OB_Lock();

        return FALSE;
    }

    halRes = HAL_FLASH_OB_Launch();

    HAL_FLASH_OB_Lock();

    if (halRes == HAL_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Check the WRP option.
// @param  None.
// @retval TRUE: WRP is enabled.
//         FALSE: WRP is disabled.
//============================================================================//
bool System_CheckWRP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};

    HAL_FLASHEx_OBGetConfig(&obHandle);

    // RTOSprintf("\r\n> WRP state: %s", (obHandle.WRPSector == OB_WRP_SECTOR_All) ? (char *)"OFF" : (char *)"ON");
    // RTOSprintf("\r\n> WRP value: 0x%X", obHandle.WRPSector);

    if (obHandle.WRPSector == OB_WRP_SECTOR_All)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Enable WRP.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool System_EnableWRP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};
    HAL_StatusTypeDef halRes;
    uint32_t u32WRP_Sectors = 0;

    HAL_FLASHEx_OBGetConfig(&obHandle);

    obHandle.OptionType = OPTIONBYTE_WRP;
    obHandle.WRPState = OB_WRPSTATE_ENABLE;
    // obHandle.WRPSector = OB_WRP_SECTOR_All;
    obHandle.Banks = FLASH_BANK_1;

    for (int i = 0; i < APP_FIRST_UNIT; i++)
    {
        u32WRP_Sectors |= (OB_WRP_SECTOR_0 << i);
    }

    obHandle.WRPSector = u32WRP_Sectors;

    HAL_FLASH_OB_Unlock();

    halRes = HAL_FLASHEx_OBProgram(&obHandle);

    if (halRes != HAL_OK)
    {
        RTOSprintf("\r\n> HAL_FLASHEx_OBProgram ERROR %d", halRes);

        HAL_FLASH_OB_Lock();

        return FALSE;
    }

    halRes = HAL_FLASH_OB_Launch();

    HAL_FLASH_OB_Lock();

    if (halRes == HAL_OK)
    {
        return TRUE;
    }
    else
    {
        RTOSprintf("\r\n> HAL_FLASH_OB_Launch %d", halRes);

        return FALSE;
    }
}

//============================================================================//
// @brief  Disable WRP.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool System_DisableWRP(void)
{
    FLASH_OBProgramInitTypeDef obHandle = {0};
    HAL_StatusTypeDef halRes;

    HAL_FLASHEx_OBGetConfig(&obHandle);

    obHandle.OptionType = OPTIONBYTE_WRP;
    obHandle.WRPState = OB_WRPSTATE_DISABLE;
    obHandle.WRPSector = OB_WRP_SECTOR_All;
    obHandle.Banks = FLASH_BANK_1;

    HAL_FLASH_OB_Unlock();

    halRes = HAL_FLASHEx_OBProgram(&obHandle);

    if (halRes != HAL_OK)
    {
        RTOSprintf("\r\n> HAL_FLASHEx_OBProgram ERROR %d", halRes);

        HAL_FLASH_OB_Lock();

        return FALSE;
    }

    halRes = HAL_FLASH_OB_Launch();

    HAL_FLASH_OB_Lock();

    if (halRes == HAL_OK)
    {
        return TRUE;
    }
    else
    {
        RTOSprintf("\r\n> HAL_FLASH_OB_Launch ERROR %d", halRes);

        return FALSE;
    }
}

//============================================================================//
// @brief  Check for hardfault and report event if a previous hardfault detected.
// @param  None.
// @retval None.
//============================================================================//
void System_ChkHardfault(void)
{
    uint8_t u8Code, u8Code2;
    char cMsg[20] = {0};

    u8Code = HW_ReadBackupRegister(BKP_FAULT_CHK);

    // If hardfault detected

    if (u8Code == HARDFAULT_EVENT_CODE)
    {
        HW_WriteBackupRegister(BKP_FAULT_CHK, BKP_RST_VALUE);

        u8Code = HW_ReadBackupRegister(BKP_FAULT_CODE);

        HW_WriteBackupRegister(BKP_FAULT_CODE, BKP_RST_VALUE);

        sprintf(cMsg, "0-0x%02X", u8Code);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_RESET, cMsg);
#endif
    }
    else if (u8Code == TASK_HANG_EVENT_CODE)
    {
        HW_WriteBackupRegister(BKP_FAULT_CHK, BKP_RST_VALUE);

        u8Code = HW_ReadBackupRegister(BKP_FAULT_CODE);
        u8Code2 = HW_ReadBackupRegister(BKP_FAULT_CODE_2);
        HW_WriteBackupRegister(BKP_FAULT_CODE, BKP_RST_VALUE);
        HW_WriteBackupRegister(BKP_FAULT_CODE_2, BKP_RST_VALUE);

        sprintf(cMsg, "1-0x%02X-0x%02X", u8Code, u8Code2);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_RESET, cMsg);
#endif
    }
    else if (u8Code == SD_ERROR_EVENT_CODE)
    {
        HW_WriteBackupRegister(BKP_FAULT_CHK, BKP_RST_VALUE);

        u8Code = HW_ReadBackupRegister(BKP_FAULT_CODE);

        HW_WriteBackupRegister(BKP_FAULT_CODE, BKP_RST_VALUE);

        sprintf(cMsg, "2-0x%02X", u8Code);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_SYSTEM_RESET, cMsg);
#endif
    }
}

#if RTC_WKUP_SRC == RTC_WKUP_SRC_MCU
//============================================================================//
// @brief  Set RTC Alarm A.
// @param  u8Date: Alarm date in binary format.
//         tTime: Alarm time in RTC_TimeTypeDef format.
//         u32Mask: Masking options:
//                  RTC_ALARMMASK_NONE: Use all components.
//                  RTC_ALARMMASK_DATEWEEKDAY: Don't care about date.
//                  RTC_ALARMMASK_HOURS: Don't care about hour.
//                  RTC_ALARMMASK_MINUTES: Don't care about minute.
//                  RTC_ALARMMASK_SECONDS: Don't care about second.
//                  RTC_ALARMMASK_ALL: Masking all components, generate 1s alarm.
// @retval None.
//============================================================================//
static void System_SetAlarm(uint8_t u8Date, RTC_TimeTypeDef tTime, uint32_t u32Mask)
{
    RTC_AlarmTypeDef sAlarm = {0};

    sAlarm.AlarmTime.Hours = tTime.Hours;
    sAlarm.AlarmTime.Minutes = tTime.Minutes;
    sAlarm.AlarmTime.Seconds = tTime.Seconds;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = u32Mask;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = u8Date;
    sAlarm.Alarm = RTC_ALARM_A;

    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);

    if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_ALRAF))
    {
        RTOSprintf("\r\n> ### System waken up by RTC alarm A ###");

        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
    }

    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        RTOSprintf("\r\n> RTC set alarm (%02d%02d%02d%02d) ERROR.",
                   u8Date, tTime.Hours, tTime.Minutes, tTime.Seconds);

        Error_Handler();
    }
    else
    {
        RTOSprintf("\r\n> RTC set alarm (%02d%02d%02d%02d) OK.",
                   u8Date, tTime.Hours, tTime.Minutes, tTime.Seconds);
    }
}
#endif

//============================================================================//
// @brief  Create alarm enabling request.
// @param  None.
// @retval None.
//============================================================================//
void System_SetExAlmSetting(ExternalAlarmTypeDef *ptSetting)
{
#if RTC_WKUP_SRC == RTC_WKUP_SRC_MCU
    if (ptSetting->bRepeat == FALSE)
    {
        ptSetting->bRequest = TRUE;
    }

    memcpy(&g_Setting.tExAlarm, ptSetting, sizeof(ExternalAlarmTypeDef));

    System_SaveSetting();

// System_SetAlarm(g_Setting.tExAlarm.tAlmDate.Date,
//								g_Setting.tExAlarm.tAlmTime,
//								g_Setting.tExAlarm.u32Mask);
#else
    if (ptSetting->bRepeat == FALSE)
    {
        ptSetting->bRequest = TRUE;
    }

    memcpy(&g_Setting.tExAlarm, ptSetting, sizeof(ExternalAlarmTypeDef));

    System_SaveSetting();
#endif
}

//============================================================================//
// @brief  Check alarm enabling request.
// @param  None.
// @retval None.
//============================================================================//
void System_CheckExAlmSetting(void)
{
    // RTOSprintf("\r\n> RTC Ex Alarm setting: %d-%d-%d-%d",
    //					 g_Setting.tExAlarm.bEnableAlm,
    //					 g_Setting.tExAlarm.bRepeat,
    //					 g_Setting.tExAlarm.bRequest,
    //					 bSystemTimeReady);

    if ((g_Setting.tExAlarm.bEnableAlm == TRUE) && (bSystemTimeReady == TRUE) &&
        ((g_Setting.tExAlarm.bRepeat == TRUE) || ((g_Setting.tExAlarm.bRepeat == FALSE) && (g_Setting.tExAlarm.bRequest == TRUE))))
    {
#if RTC_WKUP_SRC == RTC_WKUP_SRC_MCU
        System_SetAlarm(g_Setting.tExAlarm.tAlmDate.Date,
                        g_Setting.tExAlarm.tAlmTime,
                        g_Setting.tExAlarm.u32Mask);
#elif RTC_WKUP_SRC == RTC_WKUP_SRC_MG82 && ENABLE_MG82
        MG82_RTC_Alarm(System_GetTime(), g_Setting.tExAlarm);
#endif

        if (g_Setting.tExAlarm.bRepeat == FALSE)
        {
            g_Setting.tExAlarm.bEnableAlm = FALSE;

            g_Setting.tExAlarm.bRequest = FALSE;

            System_SaveSetting();
        }
    }
}

//============================================================================//
// @brief  Set RTC wakeup timer.
// @param  u32Timeout: Wakeup period in seconds.
// @retval None.
//============================================================================//
void System_SetWakeupTimer(uint32_t u32Timeout)
{
    if ((u32Timeout == 0) || (u32Timeout > 131072))
        return;

    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);

    if (__HAL_RTC_WAKEUPTIMER_GET_FLAG(&hrtc, RTC_FLAG_WUTF) != RESET)
    {
        RTOSprintf("\r\n> ### RTC wake up flag set. Reset ###");

        __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
    }

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

    if (u32Timeout <= 65536)
    {
        if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, u32Timeout - 1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS))
        {
            RTOSprintf("\r\n> RTC set wake up timer (%d) ERROR.", u32Timeout);

            Error_Handler();
        }
        else
        {
            RTOSprintf("\r\n> RTC set wake up timer (%d) OK.", u32Timeout);
        }
    }
    else
    {
        if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, u32Timeout - 65536 - 1, RTC_WAKEUPCLOCK_CK_SPRE_17BITS))
        {
            RTOSprintf("\r\n> RTC set wake up timer (%d) ERROR.", u32Timeout);

            Error_Handler();
        }
        else
        {
            RTOSprintf("\r\n> RTC set wake up timer (%d) OK.", u32Timeout);
        }
    }
}
//============================================================================//
// @brief  Set Sofware IWDG and No reset after into low-power mode.
// @param  u32Timeout: Wakeup period in seconds.
// @retval None.
//============================================================================//
bool System_DisableResetAfterStandby(void)
{
    bool bCheck = FALSE;
    HAL_StatusTypeDef halRes;
    FLASH_OBProgramInitTypeDef obHandle = {0};
    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();
    // Get current Option Byte configuration
    HAL_FLASHEx_OBGetConfig(&obHandle);

    if (obHandle.USERConfig & OB_IWDG_SW)
    {
        if ((obHandle.USERConfig & OB_STDBY_NO_RST) || (obHandle.USERConfig & OB_STOP_NO_RST))
        {
            bCheck = TRUE;
            RTOSprintf("\r\n> IDWDG SW and NO STDBY RST, NO STOP RST set");
        }
        else
        {
            // Set the IWDG to software mode (not hardware mode)
            obHandle.OptionType = OPTIONBYTE_USER;
            obHandle.USERConfig = OB_IWDG_SW; // Switch IWDG to software control
            obHandle.USERConfig |= OB_STDBY_NO_RST | OB_STOP_NO_RST;
            halRes = HAL_FLASHEx_OBProgram(&obHandle);
            if (halRes != HAL_OK)
            {
                RTOSprintf("\r\n> HAL_FLASHEx_OBProgram ERROR %d", halRes);
                bCheck = FALSE;
            }
            RTOSprintf("\r\n> IDWDG SW and NO STDBY RST, NO STOP RST set");
        }
    }
    else
    {
        // Set the IWDG to software mode (not hardware mode)
        obHandle.OptionType = OPTIONBYTE_USER;
        obHandle.USERConfig = OB_IWDG_SW; // Switch IWDG to software control
        obHandle.USERConfig |= OB_STDBY_NO_RST | OB_STOP_NO_RST;
        halRes = HAL_FLASHEx_OBProgram(&obHandle);
        if (halRes != HAL_OK)
        {
            RTOSprintf("\r\n> HAL_FLASHEx_OBProgram ERROR %d", halRes);
            bCheck = FALSE;
        }
    }
    HAL_FLASH_OB_Launch();
    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();
    return bCheck;
}
