//============================================================================//
// User Includes
//============================================================================//
#include "SD_Module.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "Default.h"
#include "diskio.h"
#include "EXTIO.h"
#include "fatfs.h"
#include "LCD_Key.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//
SD_RequestTypeDef sdRequest;

static DIR dirSD_Handle;
static FIL filSD_Handle;
static FILINFO finfoHandle;
static char cSD_FilePath[128];

bool bSD_ReInit = TRUE;

uint8_t u8SD_RetryCnt = 0, u8SD_ErrCnt = 0; //, u8SD_ResetCnt = 0;

bool bSD_FormatFlag = FALSE;

bool bSD_ReportErrFlag = FALSE;

uint32_t u32SD_ReportErrLastTick = 0;

uint8_t u8SD_LastErrCode = 0;

static uint8_t u8SD_Buf[10240];

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Enable SD card power source.
// @param  None.
// @retval None
//============================================================================//
void SD_PWR_Enable(void)
{
    HW_PBSet(SD_PWR_EN);
}

//============================================================================//
// @brief  Disable SD card power source.
// @param  None.
// @retval None
//============================================================================//
void SD_PWR_Disable(void)
{
    HW_PBClr(SD_PWR_EN);
}

//============================================================================//
// @brief  Get SD card information.
// @param  pCardInfo: Pointer to a HAL_SD_CardInfoTypeDef variable.
//         pCID: Pointer to a HAL_SD_CardCIDTypeDef variable.
//         pCSD: Pointer to a HAL_SD_CardCSDTypeDef variable.
// @retval Function result in HAL_StatusTypeDef format.
//============================================================================//
static HAL_StatusTypeDef SD_GetCardInfo(HAL_SD_CardInfoTypeDef *pCardInfo,
                                        HAL_SD_CardCIDTypeDef *pCID,
                                        HAL_SD_CardCSDTypeDef *pCSD)
{
    HAL_StatusTypeDef halRes;

    halRes = HAL_SD_GetCardCID(&hsd, pCID);

    if (halRes != HAL_OK)
        return halRes;

    halRes = HAL_SD_GetCardCSD(&hsd, pCSD);

    if (halRes != HAL_OK)
        return halRes;

    halRes = HAL_SD_GetCardInfo(&hsd, pCardInfo);

    return halRes;
}

//============================================================================//
// @brief  Display SD card information.
// @param  None.
// @retval None
//============================================================================//
void SD_ShowInfo(void)
{
    HAL_StatusTypeDef halRes;
    HAL_SD_CardInfoTypeDef sdCardInfo;
    HAL_SD_CardCIDTypeDef sdCID;
    HAL_SD_CardCSDTypeDef sdCSD;

    if (!bRTOS_En)
        halRes = SD_GetCardInfo(&sdCardInfo, &sdCID, &sdCSD);
    else
        halRes = (HAL_StatusTypeDef)SD_RequestDo(SD_GET_INFO, 3, &sdCardInfo, &sdCID, &sdCSD);

    if (halRes == HAL_OK)
    {
        // #if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD disk Product name: %c%c%c%c%c!",
                   (sdCID.ProdName1 >> 24) & 0xFF,
                   (sdCID.ProdName1 >> 16) & 0xFF,
                   (sdCID.ProdName1 >> 8) & 0xFF,
                   sdCID.ProdName1 & 0xFF,
                   sdCID.ProdName2);

        RTOSprintf("\r\n> SD disk ManufactDate: 20%02d.%d!",
                   (sdCID.ManufactDate & 0x0ff0) >> 4,
                   (sdCID.ManufactDate & 0x0f));

        RTOSprintf("\r\n> SD disk capacity : %d MB\r\n",
                   (uint32_t)((long long)sdCardInfo.BlockNbr * sdCardInfo.BlockSize / 1024768));
        // #endif
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD get information ERROR %d.", halRes);
            // #endif
        }
    }

    HAL_Delay(1);
}

//============================================================================//
// @brief  Check SD card status.
// @param  bInit: Initialize SD card or not.
// @retval Result: TRUE: SD card works normally.
//                 FALSE: SD card not present/ work normally.
//============================================================================//
bool SD_FS_Check(bool bInit)
{
    bool bRes;
    DSTATUS dsStat;

    if (bInit == TRUE)
    {
        if (!bRTOS_En)
            dsStat = disk_initialize(DRV_USER);
        else
            dsStat = (DSTATUS)SD_RequestDo(SD_DISK_INITIALIZE, 1, DRV_USER);
    }
    else
    {
        if (bSD_ReInit == TRUE)
        {
            HAL_SD_CardStateTypeDef sdState;

            if (!bRTOS_En)
                sdState = HAL_SD_GetCardState(&hsd);
            else
                sdState = (HAL_SD_CardStateTypeDef)SD_RequestDo(SD_GET_STATUS, 1, &hsd);

            if ((sdState == 0) || (sdState == HAL_SD_CARD_READY) || (sdState == HAL_SD_CARD_TRANSFER))
            {
                dsStat = STA_READY;
            }
            else
            {
                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD card state:%d", sdState);
                    // #endif
                }

                if (!bRTOS_En)
                    dsStat = disk_initialize(DRV_USER);
                else
                    dsStat = SD_RequestDo(SD_DISK_INITIALIZE, 1, DRV_USER);
            }
        }
        else
        {
            HAL_SD_CardStateTypeDef sdState;

            if (!bRTOS_En)
                sdState = HAL_SD_GetCardState(&hsd);
            else
                sdState = (HAL_SD_CardStateTypeDef)SD_RequestDo(SD_GET_STATUS, 1, &hsd);

            if ((sdState == 0) || (sdState == HAL_SD_CARD_READY) || (sdState == HAL_SD_CARD_TRANSFER))
            {
                dsStat = STA_READY;
            }
            else
            {
                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD card state:%d", sdState);
                    // #endif
                }

                dsStat = STA_NOINIT;
            }
        }
    }

    if (bSD_ReportErrFlag == FALSE)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD stat:%d", dsStat);
#endif
    }

    switch (dsStat)
    {
    case STA_OK:
    {
        FRESULT frRes;

        SD_ShowInfo();

        if (!bRTOS_En)
            frRes = f_mount(&USERFatFS, (TCHAR const *)USERPath, 1);
        else
            frRes = SD_RequestDo(SD_MOUNT, 3, &USERFatFS, (TCHAR const *)USERPath, 1);

        if (frRes == FR_OK)
        {
            g_u8HwInfo = (g_u8HwInfo | 0x08);

#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD mounting OK. (0x%02X)", g_u8HwInfo);
#endif

            u8SD_RetryCnt = 0;

            bRes = TRUE;
        }
        else
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD mounting ERROR %d. (0x%02X)", frRes, g_u8HwInfo);
                // #endif
            }

            bRes = FALSE;
        }
    }

    break;

    case STA_READY:
        u8SD_RetryCnt = 0;

        bRes = TRUE;

        break;

    case STA_NOINIT:
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD get NOINIT ERROR.");
            // #endif
        }

        bRes = FALSE;

        break;

    case STA_NODISK:
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD get NODISK ERROR.");
            // #endif
        }

        bRes = FALSE;

        break;

    default:
        bRes = FALSE;

        break;
    }

    if (bInit == TRUE)
    {
        bSD_ReInit = FALSE;

        u8SD_RetryCnt = 0;

        return bRes;
    }

    if (bRes == TRUE)
    {
        bSD_ReInit = FALSE;

        u8SD_RetryCnt = 0;
    }
    else
    {
        // #if ENABLE_DEBUG_MSG
        RTOSprintf("%s", MSG_SD_DISK_INVALID);
        // #endif

        bSD_ReInit = TRUE;

        if (u8SD_RetryCnt < SD_ERROR_CNT_MAX)
        {
            u8SD_RetryCnt++;
        }
        else
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n>>> SD reset due to SD ISSUE...");
                // #endif
            }

            SD_Reset();
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Get SD card status with reinitialization.
// @param  None.
// @retval Result: TRUE: SD card works normally.
//                 FALSE: SD card not present/ work normally.
//============================================================================//
bool SD_FS_GetStatus(void)
{
    bool bResult;

    bSD_ReInit = TRUE;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD FS Get Status Wait Mutex ERROR %d", osRes);

            return FALSE;
        }
    }

    bResult = SD_FS_Check(FALSE);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return bResult;
}

//============================================================================//
// @brief  Initialize SD card.
// @param  None.
// @retval Result: TRUE: SD card is initialized successfully.
//                 FALSE: SD card initialization is fail.
//============================================================================//
bool SD_Init(void)
{
    bSD_ReportErrFlag = FALSE;

    SD_PWR_Disable();

    HAL_Delay(100);

    SD_PWR_Enable();

    HAL_Delay(100);

    SD_RequestInit();

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Init Wait Mutex Error %d", osRes);

            return FALSE;
        }
    }

    if (SD_FS_Check(TRUE) == TRUE)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Deinitialize SD card.
// @param  None.
// @retval Result: TRUE: SD card deinitialization success.
//                 FALSE: SD card deinitialization is fail.
//============================================================================//
bool SD_Deinit(void)
{
    FRESULT frRes;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD DeInit FS Get Status ERROR");
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD DeInit Wait Mutex Error %d", osRes);

            return FALSE;
        }
    }

    if (!bRTOS_En)
        frRes = f_mount(NULL, (TCHAR const *)USERPath, 0);
    else
        frRes = SD_RequestDo(SD_MOUNT, 3, NULL, (TCHAR const *)USERPath, 0);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    if (frRes == FR_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//============================================================================//
// @brief  Reset the SD card power source.
//         Update: Reset the system to reset the FATFS and SD card power source.
// @param  None.
// @retval None.
//============================================================================//
void SD_Reset(void)
{
    uint8_t u8SD_RestartCnt;

    if (g_SysInfo.tVehicleState.bPWR_ON == FALSE)
        return;

    u8SD_RestartCnt = HW_ReadBackupRegister(SD_ERROR_CHK);

    if (bSD_ReportErrFlag == TRUE)
        return;

    u8SD_RestartCnt++;

    HW_WriteBackupRegister(SD_ERROR_CHK, u8SD_RestartCnt);

    if (u8SD_RestartCnt >= SD_RESET_CNT_MAX)
    {
        RTOSprintf("\r\n> SD maximum reset reached. Report SD ERROR!");

        u8SD_RestartCnt = 0;

        HW_WriteBackupRegister(SD_ERROR_CHK, u8SD_RestartCnt);

        u32SD_ReportErrLastTick = HAL_GetTick();

        bSD_ReportErrFlag = TRUE;

        LCD_SetShowAutoTestPage(53);

        return;
    }

    // for(int i = 0; i < MAX_SOCKET_BANK; i++)
    //{
    //	if(bTCPIP_Connect[i] == TRUE)
    //		GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, (char *)"2", i);
    // }

#if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
#endif

    System_SaveSettingFinal();
    System_SaveBackupSettingFinal();

    HW_WriteBackupRegister(BKP_FAULT_CHK, SD_ERROR_EVENT_CODE);
    HW_WriteBackupRegister(BKP_FAULT_CODE, u8SD_LastErrCode);

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    // RTOSprintf("\r\n> SD reset.");

    System_Reset();
}

//============================================================================//
// @brief  Check if a directory exists.
// @param  pcDirName: Pointer to a char array storing the directory name.
// @retval Result: TRUE: The directory exists.
//                 FALSE: The directory does not exist/ the operation is fail.
//============================================================================//
bool SD_CheckDirExist(char *pcDirName)
{
    FRESULT frRes;
    uint8_t u8TryCnt = 0;

    if (strlen(pcDirName) == 0)
        return FALSE;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Check Dir %s Exist FS Get Status ERROR", pcDirName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Check Dir %s Exist Wait Mutex Error %d", pcDirName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

    do
    {
        // Try to open directory
        if (!bRTOS_En)
            frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
        else
            frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

        u8TryCnt++;

    } while ((frRes != FR_OK) && (u8TryCnt < 2));

    if (frRes == FR_OK)
    {
        if (!bRTOS_En)
            f_closedir(&dirSD_Handle);
        else
            SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD Check Dir %s Exist OK.", pcDirName);
#endif

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (frRes != 5)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD Check Dir %s Exist ERROR %d.", pcDirName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }
        }
        else
        {
            u8SD_ErrCnt = 0;
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Create a new directory.
// @param  pcDirName: Pointer to a char array storing the directory name.
// @retval Result: TRUE: The operation is successful.
//                 FALSE: The operation is fail.
//============================================================================//
bool SD_MakeDir(char *pcDirName)
{
    FRESULT frRes;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Make Dir %s FS Get Status ERROR", pcDirName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Make Dir %s Wait Mutex Error %d", pcDirName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

    if (!bRTOS_En)
        frRes = f_mkdir(cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_MKDIR, 1, cSD_FilePath);

    if (frRes == FR_OK)
    {
        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD Make Dir %s OK.", pcDirName);
#endif

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD make new directory %s ERROR %d.", pcDirName, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Remove a directory if it exists.
// @param  pcDirName: Pointer to a char array storing the directory name.
// @retval Result: TRUE: The operation is successful.
//                 FALSE: The operation is fail.
//============================================================================//
bool SD_RemoveDir(char *pcDirName)
{
    FRESULT frRes;

    if (SD_CheckDirExist(pcDirName) == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file %s does not exist anymore.", pcDirName);
#endif
        }

        return TRUE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Remove Dir %s Wait Mutex Error %d", pcDirName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

    if (!bRTOS_En)
        frRes = f_unlink(cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

    if (frRes == FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD Remove Dir %s OK.", pcDirName);
#endif

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD remove directory %s ERROR %d.", pcDirName, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Check if a file name matches with a name or not.
// @param  pcFileName: Pointer to the file name to be checked.
//         pcFilter: Name to be compared with.
// @retval Result: TRUE: The name is matched.
//                 FALSE: The name is not matched.
//============================================================================//
static bool SD_CheckFileByName(char *pcFileName, char *pcFilter)
{
    char *pcStr;
    uint8_t u8DataBuf[128] = {0};

    if (strlen(pcFileName) > sizeof(u8DataBuf))
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file name to check is too long!");
#endif
        }

        return FALSE;
    }

    pcStr = (char *)u8DataBuf;

    strcpy(pcStr, pcFileName);

    // Get file name only
    StopChar(pcStr, '.');

    if (strncmp(pcStr, pcFilter, strlen(pcFilter)) != 0)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Check if a file name matches with an extension or not.
// @param  pcFileName: Pointer to the file name to be checked.
//         pcFilter: Extension to be compared with.
// @retval Result: TRUE: The extension is matched.
//                 FALSE: The extension is not matched.
//============================================================================//
static bool SD_CheckFileByExten(char *pcFileName, char *pcFilter)
{
    uint8_t u8DataBuf[128] = {0};
    char *pcStr;

    if (strlen(pcFileName) > sizeof(u8DataBuf))
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file extension to check is too long!");
#endif
        }

        return FALSE;
    }

    pcStr = (char *)u8DataBuf;

    strcpy(pcStr, pcFileName);

    // Get file extension only
    StopChar(pcStr, '.');

    pcStr += strlen(pcStr) + 1;

    if (strncmp(pcStr, pcFilter, strlen(pcFilter)) != 0)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Check if a file is existing.
// @param  pcFileName: Pointer to the file name to be checked.
// @retval Result: TRUE: The file exists.
//                 FALSE: The file not exist/ operation ERROR.
//============================================================================//
bool SD_CheckFileExist(char *pcFileName)
{
    FRESULT frRes;
    // uint8_t u8TryCnt = 0;

    if (strlen(pcFileName) == 0)
    {
        return FALSE;
    }

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Check File %s Exist FS Get Status ERROR", pcFileName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Check File %s Exist Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    // do
    //{
    //  Try to open file
    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_READ);

    // u8TryCnt++;
    //}
    // while((frRes != FR_OK) && (u8TryCnt < 1));

    if (frRes == FR_OK)
    {
        for (int i = 0; i < 1; i++)
        {
            if (!bRTOS_En)
                frRes = f_close(&filSD_Handle);
            else
                frRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

            if (frRes == FR_OK)
                break;
        }

        if (frRes != FR_OK)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, frRes);
                // #endif
            }
        }

        u8SD_ErrCnt = 0;

#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD file %s exists.", pcFileName);
#endif

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (frRes != 4)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }
        }
        else
        {
            if (bSD_ReportErrFlag == FALSE)
            {
#if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD file %s does not exist.", pcFileName);
#endif
            }

            u8SD_ErrCnt = 0;
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Find a file with a given pattern.
// @param  pcDir: Path of the directory to find the find.
//         pcPattern: Pattern to find.
//         pcReturnName: If there is at least file found return the name of the
//                       first file.
// @retval Result: TRUE: The file exists.
//                 FALSE: The file not exist/ operation ERROR.
//============================================================================//
bool SD_FindFilePattern(char *pcDir, char *pcPattern, char *pcReturnName)
{
    FRESULT frRes;

    if (strlen(pcPattern) == 0)
    {
        return FALSE;
    }

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Find File with pattern %s FS Get Status ERROR", pcPattern);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Find File with pattern %s Wait Mutex Error %d", pcPattern, osRes);

            return FALSE;
        }
    }

    if (strlen(pcDir))
        sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDir);
    else
        sprintf(cSD_FilePath, "%d:/", DRV_USER);

#if ENABLE_DEBUG_MSG
    RTOSprintf("\r\n> SD Find File with pattern %s in dir: %s", pcPattern, cSD_FilePath);
#endif

    if (!bRTOS_En)
        frRes = f_findfirst(&dirSD_Handle, &finfoHandle, cSD_FilePath, pcPattern);
    else
        frRes = SD_RequestDo(SD_FIND_FIRST, 4, &dirSD_Handle, &finfoHandle, cSD_FilePath, pcPattern);

    if (frRes != FR_OK)
    {
        if (frRes != 4)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD Find File with pattern %s find file ERROR %d.", pcPattern, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }
        }
        else
        {
            u8SD_ErrCnt = 0;
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }

#if ENABLE_DEBUG_MSG
    RTOSprintf("\r\n> SD Find File with pattern %s found file %s.", pcPattern, finfoHandle.fname);
#endif

    if ((finfoHandle.fname[0] != NULL) && (strlen(finfoHandle.fname) < 128))
    {
        strcpy(pcReturnName, finfoHandle.fname);

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return FALSE;
}

//============================================================================//
// @brief  Find a file having a suggested name.
// @param  pcFindName: Pointer to the file name to find.
//         pcReturnName: Pointer to the name of the found file.
// @retval Result: TRUE: A suitable file is found.
//                 FALSE: No file is found.
//============================================================================//
bool SD_FindFileName(char *pcDirName, char *pcFindName, char *pcReturnName)
{
    bool bRes = FALSE;
    FRESULT frRes;
    char cName[50] = {0}, cExten[5] = {0};
    char *pcStr;

    if (strlen(pcFindName) == 0)
        return FALSE;

    if (strlen(pcFindName) > sizeof(cSD_FilePath))
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file name to find is too long!");
#endif
        }

        return FALSE;
    }

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Find File Name ERROR: %s", MSG_SD_DISK_INVALID);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Find File Name %s Wait Mutex Error %d", pcFindName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));

    strcpy(cSD_FilePath, pcFindName);

    pcStr = cSD_FilePath;

    // Split name and extension
    StopChar(pcStr, '.');

    strcpy(cName, pcStr);

    if (bSD_ReportErrFlag == FALSE)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD find file with name:\"%s\".", cName);
#endif
    }

    if (strlen(pcStr) < strlen(pcFindName)) // If there is extension
    {
        pcStr += strlen(pcStr) + 1;

        strcpy(cExten, pcStr);
    }
    else // If there is no extension
        sprintf(cExten, "*");

    if (bSD_ReportErrFlag == FALSE)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD find file with extension:\"%s\".", cExten);
#endif
    }

    if (strlen(pcDirName))
        sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);
    else
        sprintf(cSD_FilePath, "%d:/", DRV_USER);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        // If encounter no name files then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // #if _USE_LFN
        // pcFileName = *finfoHandle.lfname ? finfoHandle.lfname : finfoHandle.fname;
        // #else
        // pcFileName = finfoHandle.fname;
        // #endif

        // #if ENABLE_DEBUG_MSG
        // RTOSprintf("\r\n> SD check file name:%s.", pcFileName);
        // #endif

        // If find a random file
        if ((strcmp(cName, (char *)"*") == 0) && (strcmp(cExten, (char *)"*") == 0))
        {
            sprintf(pcReturnName, "%s", finfoHandle.fname);

            bRes = TRUE;

            break;
        }
        else if (strcmp(cExten, (char *)"*") == 0) // If not care about file extension
        {
            if (SD_CheckFileByName(finfoHandle.fname, cName) == TRUE)
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
        else if (strcmp(cName, (char *)"*") == 0) // If not care about file name
        {
            if (SD_CheckFileByExten(finfoHandle.fname, cExten) == TRUE)
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
        else // If both name and extension have to be matched
        {
            if ((SD_CheckFileByName(finfoHandle.fname, cName) == TRUE) &&
                (SD_CheckFileByExten(finfoHandle.fname, cExten) == TRUE))
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return bRes;
}

//============================================================================//
// @brief  Get size of a file.
// @param  pcFileName: Pointer to file name for getting size.
// @retval File size in the uint32_t format.
//============================================================================//
uint32_t SD_GetFileSize(char *pcFileName)
{
    FRESULT frRes;
    uint32_t u32FileSize = 0;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Get File Size %s FS Get Status ERROR", pcFileName);
            // #endif
        }

        return 0;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Get File Size %s Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_READ);

    // Try to open file
    if (frRes != FR_OK)
    {
        if (frRes != 4)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD open file %s ERROR %d. Fail to get file size.", pcFileName, frRes);
                // #endif
            }
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
        u8SD_ErrCnt = 0;

    // Get file size
    u32FileSize = f_size(&filSD_Handle);

    if (!bRTOS_En)
        frRes = f_close(&filSD_Handle);
    else
        frRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, frRes);
            // #endif
        }
    }

#if ENABLE_DEBUG_MSG
    RTOSprintf("\r\n> SD Get File Size %s OK.", pcFileName);
#endif

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return u32FileSize;
}

//============================================================================//
// @brief  Print a list of files in the SD card.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation ERRORed.
//============================================================================//
bool SD_FileList(void)
{
    FRESULT frRes;
    struct tm tmTimeInfo;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD File List FS Get Status ERROR");
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD File List Wait Mutex Error %d", osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&finfoHandle, 0x00, sizeof(finfoHandle));

    sprintf(cSD_FilePath, "%d:/", DRV_USER);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    uint16_t u16Cnt = 1;

    RTOSprintf("\r\n@ SD file list:");

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        // RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);
        // if((finfoHandle.fattrib & AM_DIR) == 0)
        //   continue;

        // If all files are already listed then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        // if((((finfoHandle.fattrib & AM_ARC) == 0) || (finfoHandle.fattrib & AM_DIR) == 0))
        if ((finfoHandle.fattrib & AM_ARC) == 0)
        {
            if (strncmp(finfoHandle.fname, (char *)"SYSTEM~1", 8) != 0)
                RTOSprintf("\r\n> #%d #ARCHIEVE NAME:\"%s\"", u16Cnt++, finfoHandle.fname);

            continue;
        }

        // RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);

        tmTimeInfo.tm_year = ((finfoHandle.fdate >> 9) & 0x7F) + 1980;
        tmTimeInfo.tm_mon = ((finfoHandle.fdate >> 5) & 0x0F);
        tmTimeInfo.tm_mday = finfoHandle.fdate & 0x1F;
        tmTimeInfo.tm_hour = (finfoHandle.ftime >> 11) & 0x1F;
        tmTimeInfo.tm_min = (finfoHandle.ftime >> 5) & 0x3F;
        tmTimeInfo.tm_sec = (finfoHandle.ftime & 0x1F) * 2;

        if (tmTimeInfo.tm_year > 2100)
            continue;

        RTOSprintf("\r\n> #%d #FILE NAME:\"%s\",     #SIZE: %d,     #LAST MODIFIED DATE: %04d/%02d/%02d - %02d:%02d:%02d.",
                   u16Cnt++,
                   finfoHandle.fname, finfoHandle.fsize,
                   tmTimeInfo.tm_year, tmTimeInfo.tm_mon, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return TRUE;
}

//============================================================================//
// @brief  Print a list of files in the a directory in SD card.
// @param  pcDir.
// @retval TRUE: Operation success.
//         FALSE: Operation ERRORed.
//============================================================================//
bool SD_FileListDir(char *pcDirName)
{
    FRESULT frRes;
    struct tm tmTimeInfo;

    if (pcDirName == NULL)
        return FALSE;

    if (strlen(pcDirName) > (sizeof(cSD_FilePath) - 5))
        return FALSE;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD File List Dir %s FS Get Status ERROR", pcDirName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD File List Dir %s Wait Mutex Error %d", pcDirName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&finfoHandle, 0x00, sizeof(finfoHandle));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    uint16_t u16Cnt = 1;

    RTOSprintf("\r\n@ SD directory %s file list:", pcDirName);

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        // RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);
        // if((finfoHandle.fattrib & AM_DIR) == 0)
        //   continue;

        // If all files are already listed then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        // if((((finfoHandle.fattrib & AM_ARC) == 0) || (finfoHandle.fattrib & AM_DIR) == 0))
        if ((finfoHandle.fattrib & AM_ARC) == 0)
        {
            RTOSprintf("\r\n> #%d #ARCHIEVE NAME:\"%s\"", u16Cnt++, finfoHandle.fname);

            continue;
        }

        // RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);

        tmTimeInfo.tm_year = ((finfoHandle.fdate >> 9) & 0x7F) + 1980;
        tmTimeInfo.tm_mon = ((finfoHandle.fdate >> 5) & 0x0F);
        tmTimeInfo.tm_mday = finfoHandle.fdate & 0x1F;
        tmTimeInfo.tm_hour = (finfoHandle.ftime >> 11) & 0x1F;
        tmTimeInfo.tm_min = (finfoHandle.ftime >> 5) & 0x3F;
        tmTimeInfo.tm_sec = (finfoHandle.ftime & 0x1F) * 2;

        if (tmTimeInfo.tm_year > 2100)
            continue;

        RTOSprintf("\r\n> #%d #FILE NAME:\"%s\",     #SIZE: %d,     #LAST MODIFIED DATE: %04d/%02d/%02d - %02d:%02d:%02d.",
                   u16Cnt++,
                   finfoHandle.fname, finfoHandle.fsize,
                   tmTimeInfo.tm_year, tmTimeInfo.tm_mon, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return TRUE;
}

//============================================================================//
// @brief  Get the last time when a file is modified in local time.
// @param  pcFileName: Pointer to file name to be checked.
// @retval Time in time_t format
//============================================================================//
static time_t GetFileTime(FILINFO *pfinfoHandle)
{
    struct tm tmTimeInfo;

    tmTimeInfo.tm_year = ((pfinfoHandle->fdate >> 9) & 0x7F) + 1980 - 1900;
    tmTimeInfo.tm_mon = ((pfinfoHandle->fdate >> 5) & 0x0F) - 1;
    tmTimeInfo.tm_mday = pfinfoHandle->fdate & 0x1F;
    tmTimeInfo.tm_hour = (pfinfoHandle->ftime >> 11) & 0x1F;
    tmTimeInfo.tm_min = (pfinfoHandle->ftime >> 5) & 0x3F;
    tmTimeInfo.tm_sec = (pfinfoHandle->ftime & 0x1F) * 2;
    tmTimeInfo.tm_isdst = -1;

    return mktime(&tmTimeInfo);
}

//============================================================================//
// @brief  Delete file which exist longer than a keeptime period.
// @param  pcFileNames: Pointer to file name for deleting.
//         u8KeepTime: Time period to keep file in days.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_ClearUp(char *pcFileNames, uint8_t u8KeepTime)
{
    FRESULT frRes;
    char cFileName[50], cFileExten[5];
    char *pcStr;

    if (strlen(pcFileNames) > sizeof(cSD_FilePath))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD file name to clear up is too long!");
#endif

        return FALSE;
    }

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Clear Up %s FS Get Status ERROR", pcFileNames);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Clear Up %s Wait Mutex Error %d", pcFileNames, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&finfoHandle, 0x00, sizeof(finfoHandle));

    strcpy(cSD_FilePath, pcFileNames);

    pcStr = cSD_FilePath;

    StopChar(pcStr, '.');

    // Get file name
    strcpy(cFileName, pcStr);

    pcStr += strlen(pcStr) + 1;

    // Get file extension
    strcpy(cFileExten, pcStr);

    sprintf(cSD_FilePath, "%d:/", DRV_USER);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        time_t ttTimeNow;

        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        ttTimeNow = System_GetTime();

        // If file date is within KeepTime day, don't delete
        if ((ttTimeNow - GetFileTime(&finfoHandle)) < DAY_2_SECOND(u8KeepTime))
            continue;

        // If delete all files
        if (strcmp(cFileName, "*") == 0 &&
            strcmp(cFileExten, "*") == 0)
        {
            sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

            if (!bRTOS_En)
                frRes = f_unlink(cSD_FilePath);
            else
                frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

            if (frRes == FR_OK)
            {
                if (bSD_ReportErrFlag == FALSE)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                }

                u8SD_ErrCnt = 0;
            }
            else
            {
                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                    // #endif
                }

                u8SD_ErrCnt++;

                if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                {
                    u8SD_ErrCnt = 0;

                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                        // #endif
                    }

                    SD_Reset();
                }
            }
        }
        else if (strcmp(cFileName, "*") == 0) // If delete files with matched extension
        {
            if (SD_CheckFileByExten(finfoHandle.fname, cFileExten) == TRUE)
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else if (strcmp(cFileExten, "*") == 0) // If delete files with matched name
        {
            if (SD_CheckFileByName(finfoHandle.fname, cFileName) == TRUE)
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else // If both name and extension have to be matched
        {
            // if(strncmp(pcFileName, pcFileNames, strlen(pcFileNames)) == 0)
            if ((SD_CheckFileByName(finfoHandle.fname, cFileName) == TRUE) &&
                (SD_CheckFileByExten(finfoHandle.fname, cFileExten) == TRUE))
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return TRUE;
}

//============================================================================//
// @brief  Delete files in a directory which exist longer than a keeptime period.
// @param  pcDir: Pointer to the directory name.
//         pcFileNames: Pointer to file name for deleting.
//         u8KeepTime: Time period to keep file in days.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_ClearUpDir(char *pcDir, char *pcFileNames, uint8_t u8KeepTime)
{
    FRESULT frRes;
    char cFileName[50], cFileExten[5];
    char *pcStr;

    if (strlen(pcFileNames) > sizeof(cSD_FilePath))
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file name to clear up is too long!");
#endif
        }

        return FALSE;
    }

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Clear Up Dir %s for File %s FS Get Status ERROR", pcDir, pcFileNames);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Clear Up Dir %s for File %s Wait Mutex Error %d", pcDir, pcFileNames, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&finfoHandle, 0x00, sizeof(finfoHandle));

    strcpy(cSD_FilePath, pcFileNames);

    pcStr = cSD_FilePath;

    StopChar(pcStr, '.');

    // Get file name
    strcpy(cFileName, pcStr);

    pcStr += strlen(pcStr) + 1;

    // Get file extension
    strcpy(cFileExten, pcStr);

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDir);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (bRTOS_En)
        {
            osMutexRelease(mutex_sd_proc_user);
        }

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        time_t ttTimeNow;

        // If encounter no name files then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        ttTimeNow = System_GetTime();

        // If file date is within KeepTime day, don't delete
        if ((ttTimeNow - GetFileTime(&finfoHandle)) < DAY_2_SECOND(u8KeepTime))
            continue;

        // If delete all files
        if (strcmp(cFileName, "*") == 0 &&
            strcmp(cFileExten, "*") == 0)
        {
            sprintf(cSD_FilePath, "%d:/%s/%s", DRV_USER, pcDir, finfoHandle.fname);

            if (!bRTOS_En)
                frRes = f_unlink(cSD_FilePath);
            else
                frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

            if (frRes == FR_OK)
            {
                if (bSD_ReportErrFlag == FALSE)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                }

                u8SD_ErrCnt = 0;
            }
            else
            {
                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                    // #endif
                }

                u8SD_ErrCnt++;

                if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                {
                    u8SD_ErrCnt = 0;

                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                        // #endif
                    }

                    SD_Reset();
                }
            }
        }
        else if (strcmp(cFileName, "*") == 0) // If delete files with matched extension
        {
            if (SD_CheckFileByExten(finfoHandle.fname, cFileExten) == TRUE)
            {
                sprintf(cSD_FilePath, "%d:/%s/%s", DRV_USER, pcDir, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else if (strcmp(cFileExten, "*") == 0) // If delete files with matched name
        {
            if (SD_CheckFileByName(finfoHandle.fname, cFileName) == TRUE)
            {
                sprintf(cSD_FilePath, "%d:/%s/%s", DRV_USER, pcDir, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else // If both name and extension have to be matched
        {
            // if(strncmp(pcFileName, pcFileNames, strlen(pcFileNames)) == 0)
            if ((SD_CheckFileByName(finfoHandle.fname, cFileName) == TRUE) &&
                (SD_CheckFileByExten(finfoHandle.fname, cFileExten) == TRUE))
            {
                sprintf(cSD_FilePath, "%d:/%s/%s", DRV_USER, pcDir, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return TRUE;
}

//============================================================================//
// @brief  Delete a file/ files.
// @param  pcNames: Name or extension of the files to be deleted.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_Delete(char *pcNames)
{
    return SD_ClearUp(pcNames, 0);
}

//============================================================================//
// @brief  Delete all files in SD card except a file having a chosen name or
//         extension.
// @param  pcNames: Name or extension of the file to be kept.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_DelExcept(char *pcNames)
{
    FRESULT frRes;
    char cFileName[50], cFileExten[10];
    char *pcStr;

    if (strlen(pcNames) > sizeof(cSD_FilePath))
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file name to delete except is too long!");
#endif
        }

        return FALSE;
    }

    // Check USB disk status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Del Except FS Get Status ERROR");
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Del Except Wait Mutex Error %d", osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&dirSD_Handle, 0x00, sizeof(dirSD_Handle));
    memset(&finfoHandle, 0x00, sizeof(finfoHandle));

    strcpy(cSD_FilePath, pcNames);

    pcStr = cSD_FilePath;

    StopChar(pcStr, '.');

    // Get file name
    strcpy(cFileName, pcStr);

    pcStr += strlen(pcStr) + 1;

    // Get file extension
    strcpy(cFileExten, pcStr);

    sprintf(cSD_FilePath, "%d:/", DRV_USER);

    if (!bRTOS_En)
        frRes = f_opendir(&dirSD_Handle, cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_OPENDIR, 2, &dirSD_Handle, cSD_FilePath);

    // Open directory
    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (bRTOS_En)
        {
            osMutexRelease(mutex_sd_proc_user);
        }

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        return FALSE;
    }
    else
    {
        u8SD_ErrCnt = 0;
    }

    // Check files through the SD card
    // while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
    while (SD_RequestDo(SD_READDIR, 2, &dirSD_Handle, &finfoHandle) == FR_OK)
    {
        // If encounter no name files then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        // If keep all files
        if (strcmp(cFileName, "*") == 0 &&
            strcmp(cFileExten, "*") == 0)
        {
            break;
        }
        else if (strcmp(cFileName, "*") == 0) // If only keep file with same extension
        {
            if (SD_CheckFileByExten(finfoHandle.fname, cFileExten) == FALSE)
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file%s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else if (strcmp(cFileExten, "*") == 0) // If only keep files with same name
        {
            if (SD_CheckFileByName(finfoHandle.fname, cFileExten) == FALSE)
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname, frRes);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
        else // If only keep a file with exactly matched name and extension
        {
            if (strncmp(finfoHandle.fname, pcNames, strlen(pcNames)) != 0)
            {
                sprintf(cSD_FilePath, "%d:/%s", DRV_USER, finfoHandle.fname);

                if (!bRTOS_En)
                    frRes = f_unlink(cSD_FilePath);
                else
                    frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

                if (frRes == FR_OK)
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
#if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s OK.", finfoHandle.fname);
#endif
                    }

                    u8SD_ErrCnt = 0;
                }
                else
                {
                    if (bSD_ReportErrFlag == FALSE)
                    {
                        // #if ENABLE_DEBUG_MSG
                        RTOSprintf("\r\n> SD delete file %s ERROR %d", finfoHandle.fname);
                        // #endif
                    }

                    u8SD_ErrCnt++;

                    if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
                    {
                        u8SD_ErrCnt = 0;

                        if (bSD_ReportErrFlag == FALSE)
                        {
                            // #if ENABLE_DEBUG_MSG
                            RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                            // #endif
                        }

                        SD_Reset();
                    }
                }
            }
        }
    }

    if (!bRTOS_En)
        f_closedir(&dirSD_Handle);
    else
        SD_RequestDo(SD_CLOSEDIR, 1, &dirSD_Handle);

    if (bRTOS_En)
        osMutexRelease(mutex_sd_proc_user);

    HAL_Delay(1);

    return TRUE;
}

//============================================================================//
// @brief  Format SD card.
// @param  None.
// @retval Result: TRUE: SD card is formatted successsfully.
//                 FALSE: SD card is not formatted successfully.
//============================================================================//
bool SD_Format(void)
{
    FRESULT frRes;
    uint8_t u8FormatCnt = 0;

    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Format %s FS Get Status ERROR");
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            u8FormatCnt++;

            RTOSprintf("\r\n> SD Format Wait Mutex ERROR %d", osRes);

            return FALSE;
        }
    }

    do
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> Format %d times.", (u8FormatCnt + 1));
        }

        memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

        if (!bRTOS_En)
            frRes = f_mkfs((const TCHAR *)USERPath, FM_FAT32, 0, u8SD_Buf, sizeof(u8SD_Buf));
        else
            frRes = SD_RequestDo(SD_MKFS, 5, (const TCHAR *)USERPath, FM_FAT32, 0, u8SD_Buf, sizeof(u8SD_Buf));

        u8FormatCnt++;

        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> Format %d times result: %d", u8FormatCnt, frRes);
#endif
        }

        HAL_Delay(500);

        if (u8FormatCnt >= 15)
        {
            break;
        }
    } while (frRes != FR_OK);

    if (u8FormatCnt >= 15)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Format ERROR %d", frRes);
            // #endif
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
    {
        g_Setting.tSD_State.bSD_FormatFlag = TRUE;

#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD Format OK.");
#endif

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
}

//============================================================================//
// @brief  Remove a file in the SD card.
// @param  pcFileName: Pointer to an array storing the file name.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool SD_RemoveFile(char *pcFilename)
{
    FRESULT frRes;

    if (SD_CheckFileExist(pcFilename) == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file %s does not exist anymore.", pcFilename);
#endif
        }

        return TRUE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Remove File Wait Mutex Error %d", osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFilename);

    if (!bRTOS_En)
        frRes = f_unlink(cSD_FilePath);
    else
        frRes = SD_RequestDo(SD_UNLINK, 1, cSD_FilePath);

    if (frRes == FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Remove File %s OK.", pcFilename);
#endif
        }

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Remove File %s ERROR %d.", pcFilename, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Remove a file in the SD card.
// @param  pcFileName: Pointer to an array storing the file name.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool SD_Rename(char *pcOldName, char *pcNewName)
{
    FRESULT frRes;

    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Rename File %s FS Get Status ERROR", pcOldName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Rename File %s Wait Mutex Error %d", pcOldName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcOldName);

    if (!bRTOS_En)
        frRes = f_rename(cSD_FilePath, pcNewName);
    else
        frRes = SD_RequestDo(SD_RENAME, 2, cSD_FilePath, pcNewName);

    if (frRes == FR_OK)
    {
        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> SD Rename File %s OK.", pcOldName);
#endif

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Rename File %s ERROR %d", pcOldName, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();

            if (bRTOS_En)
                osMutexRelease(mutex_sd_proc_user);

            HAL_Delay(1);

            return FALSE;
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Write to the bottom of a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8SD_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of the data for writing.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_AddBuffToFile(char *pcFileName, uint8_t *pu8SD_Buf, uint16_t u16BufLen)
{
    FRESULT frRes;
    UINT uBw;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Add Buff To File %s FS Get Status ERROR", pcFileName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)

        {
            RTOSprintf("\r\n> SD Add Buff To File %s Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));
    memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    memcpy(u8SD_Buf, pu8SD_Buf, u16BufLen);

    // Open file
    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_WRITE);

    if (frRes != FR_OK) // If the specified file is not exist then create a new file
    {
        if (!bRTOS_En)
            frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
        else
            frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

        if (frRes != FR_OK)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD open file %s ERROR %d", pcFileName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }

            if (bRTOS_En)
                osMutexRelease(mutex_sd_proc_user);

            HAL_Delay(1);

            return FALSE;
        }
        else
            u8SD_ErrCnt = 0;
    }

    if (!bRTOS_En)
        frRes = f_lseek(&filSD_Handle, f_size(&filSD_Handle));
    else
        frRes = SD_RequestDo(SD_LSEEK, 2, &filSD_Handle, f_size(&filSD_Handle));

    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Add Buff To File of file %s set cursor ERROR %d.", pcFileName, frRes);
            // #endif
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }

    if (!bRTOS_En)
        frRes = f_write(&filSD_Handle, u8SD_Buf, u16BufLen, &uBw);
    else
        frRes = SD_RequestDo(SD_WRITE, 4, &filSD_Handle, u8SD_Buf, u16BufLen, &uBw);

    FRESULT fRes;

    for (int i = 0; i < 3; i++)
    {
        if (!bRTOS_En)
            fRes = f_close(&filSD_Handle);
        else
            fRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

        if (fRes == FR_OK)
            break;
    }

    if (fRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
        }
    }

    if (frRes == FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD write to file %s OK.", pcFileName);
#endif
        }

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD write to file %s at address %d ERROR %d.",
                       pcFileName, f_size(&filSD_Handle), frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Write a string to the bottom of a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pcString: Pointer to string for writing.
// @retval Result: TRUE:.
//============================================================================//
bool SD_AddStrToFile(char *pcFileName, char *pcString)
{
    return SD_AddBuffToFile(pcFileName, (uint8_t *)pcString, strlen(pcString));
}

//============================================================================//
// @brief  Read from an address in a file.
// @param  pcFileName: Pointer to the file name for reading.
//         pu8SD_Buf: Pointer to the data buffer for storing.
//         u16BufLen: Length of the data buffer for reading.
//         u32Addr: Address to read from in the file.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_ReadAddrToBuff(char *pcFileName, uint8_t *pu8SD_Buf,
                       uint16_t u16BufLen, uint32_t u32Addr)
{
    FRESULT frRes;
    UINT uBr;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Read Addr To Buff %s FS Get Status ERROR", pcFileName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Read Add To Buff %s Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));
    memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_READ);

    // Open file for reading
    if (frRes != FR_OK)
    {
        if (frRes == 4)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD file %s does not exist for reading.", pcFileName);
#endif
        }
        else
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
    else
        u8SD_ErrCnt = 0;

    // Put cursor to the start address
    if (!bRTOS_En)
        frRes = f_lseek(&filSD_Handle, u32Addr);
    else
        frRes = SD_RequestDo(SD_LSEEK, 2, &filSD_Handle, u32Addr);

    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Read Addr To Buff of file %s set cursor ERROR %d.", pcFileName, frRes);
            // #endif
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }

    if (!bRTOS_En)
        frRes = f_read(&filSD_Handle, u8SD_Buf, u16BufLen, &uBr);
    else
        frRes = SD_RequestDo(SD_READ, 4, &filSD_Handle, u8SD_Buf, u16BufLen, &uBr);

    FRESULT fRes;

    for (int i = 0; i < 3; i++)
    {
        if (!bRTOS_En)
            fRes = f_close(&filSD_Handle);
        else
            fRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

        if (fRes == FR_OK)
            break;
    }

    if (fRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
        }
    }

    if (frRes == FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD read file %s OK.", pcFileName);
#endif
        }

        u8SD_ErrCnt = 0;

        memcpy(pu8SD_Buf, u8SD_Buf, u16BufLen);

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD read file %s at address %d ERROR %d.",
                       pcFileName, u32Addr, frRes);

            RTOSprintf("\r\n> SD number of bytes read:%d.", uBr);
        }
        // #endif

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Read from the beginning of a file.
// @param  pcFileName: Pointer the to file name for reading.
//         pu8SD_Buf: Pointer to the data buffer for storing.
//         u16BufLen: Length of the data for reading.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_ReadFileToBuff(char *pcFileName, uint8_t *pu8SD_Buf, uint16_t u16BufLen)
{
    return SD_ReadAddrToBuff(pcFileName, pu8SD_Buf, u16BufLen, 0);
}

//============================================================================//
// @brief  Write to an address in a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8SD_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of the data for writing.
//         u32Addr: Address to start write to.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_WriteBuffToAddr(char *pcFileName, uint8_t *pu8SD_Buf,
                        uint16_t u16BufLen, uint32_t u32Addr)
{
    FRESULT frRes;
    UINT uBw;

    // Check SD card status
    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Write Buff To Addr %s FS Get Status ERROR", pcFileName);
            // #endif
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Write Buff To Addr %s Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));
    memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    memcpy(u8SD_Buf, pu8SD_Buf, u16BufLen);

    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_WRITE);

    // Open file
    if (frRes != FR_OK)
    {
        if (!bRTOS_En)
            frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
        else
            frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

        // HAL_Delay(SD_CMD_DELAY);

        if (frRes != FR_OK)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }

            if (bRTOS_En)
                osMutexRelease(mutex_sd_proc_user);

            HAL_Delay(1);

            return FALSE;
        }
        else
            u8SD_ErrCnt = 0;
    }

    // Put cursor to the start address
    if (!bRTOS_En)
        frRes = f_lseek(&filSD_Handle, u32Addr);
    else
        frRes = SD_RequestDo(SD_LSEEK, 2, &filSD_Handle, u32Addr);

    if (frRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD Write Buff To Addr of file %s set cursor ERROR %d.", pcFileName, frRes);
            // #endif
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }

    if (!bRTOS_En)
        frRes = f_write(&filSD_Handle, u8SD_Buf, u16BufLen, &uBw);
    else
        frRes = SD_RequestDo(SD_WRITE, 4, &filSD_Handle, u8SD_Buf, u16BufLen, &uBw);

    FRESULT fRes;

    for (int i = 0; i < 3; i++)
    {
        if (!bRTOS_En)
            fRes = f_close(&filSD_Handle);
        else
            fRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

        if (fRes == FR_OK)
            break;
    }

    if (fRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
        }
    }

    if (frRes == FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD write to file %s OK.", pcFileName);
#endif
        }

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD write to file %s at address %d ERROR %d.",
                       pcFileName, u32Addr, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Write to a file from the beginning.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8SD_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of data for writing.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_WriteBuffToFile(char *pcFileName, uint8_t *pu8SD_Buf, uint16_t u16BufLen)
{
    return SD_WriteBuffToAddr(pcFileName, pu8SD_Buf, u16BufLen, 0);
}

//============================================================================//
// @brief  Create a file having empty space for writing.
// @param  pcFileName: Pointer to the file name for creating.
//         u32Addr: Start address to write the content.
//         bClearFlag: Where to put the cursor after creating.
//                     TRUE: Put the cursor after the empty area.
//                     FALSE: Put the cursor at the beginning address.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation ERRORed.
//============================================================================//
bool SD_CreateFile(char *pcFileName, uint32_t u32Addr, bool bClearFlag)
{
    FRESULT frRes, fRes;
    UINT uBw;
    uint16_t u16BufSize, u16DataSize;

    if (SD_FS_GetStatus() == FALSE)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> SD Create File %s FS Get Status ERROR", pcFileName);
        }

        return FALSE;
    }

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> SD Create File %s Wait Mutex Error %d", pcFileName, osRes);

            return FALSE;
        }
    }

    memset(cSD_FilePath, 0x00, sizeof(cSD_FilePath));
    memset(&filSD_Handle, 0x00, sizeof(filSD_Handle));
    memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

    sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

    if (!bRTOS_En)
        frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);
    else
        frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_WRITE);

    if (frRes != FR_OK)
    {
        if (!bRTOS_En)
            frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
        else
            frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

        if (frRes != FR_OK)
        {
            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD create file %s ERROR %d.", pcFileName, frRes);
                // #endif
            }

            u8SD_ErrCnt++;

            if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
            {
                u8SD_ErrCnt = 0;

                if (bSD_ReportErrFlag == FALSE)
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                    // #endif
                }

                SD_Reset();
            }

            if (bRTOS_En)
                osMutexRelease(mutex_sd_proc_user);

            HAL_Delay(1);

            return FALSE;
        }
        else
            u8SD_ErrCnt = 0;
    }

    if (!bRTOS_En)
        f_lseek(&filSD_Handle, 0);
    else
        SD_RequestDo(SD_LSEEK, 2, &filSD_Handle, 0);

    u16BufSize = u16DataSize = 0;

    memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

    if (bClearFlag == TRUE)
    {
        do
        {
            if ((u32Addr - u16BufSize) > sizeof(u8SD_Buf))
            {
                u16DataSize = sizeof(u8SD_Buf);
            }
            else
            {
                u16DataSize = u32Addr - u16BufSize;
            }

            if (!bRTOS_En)
                frRes = f_write(&filSD_Handle, &u8SD_Buf, u16DataSize, &uBw);
            else
                frRes = SD_RequestDo(SD_WRITE, 4, &filSD_Handle, &u8SD_Buf, u16DataSize, &uBw);

            if (frRes != FR_OK)
                break;

            u16BufSize += u16DataSize;
        }

        while (u16BufSize < u32Addr);
    }
    else
    {
        if (u32Addr < sizeof(u8SD_Buf))
        {
            if (!bRTOS_En)
                frRes = f_write(&filSD_Handle, &u8SD_Buf, u32Addr, &uBw);
            else
                frRes = SD_RequestDo(SD_WRITE, 4, &filSD_Handle, &u8SD_Buf, u32Addr, &uBw);
        }
        else
        {
            if (!bRTOS_En)
                frRes = f_write(&filSD_Handle, &u8SD_Buf, sizeof(u8SD_Buf), &uBw);
            else
                frRes = SD_RequestDo(SD_WRITE, 4, &filSD_Handle, &u8SD_Buf, sizeof(u8SD_Buf), &uBw);
        }
    }

    if (!bRTOS_En)
        fRes = f_close(&filSD_Handle);
    else
        fRes = SD_RequestDo(SD_CLOSE, 1, &filSD_Handle);

    if (fRes != FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
            // #endif
        }
    }

    if (frRes == FR_OK)
    {
        if (bSD_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD create file %s OK.", pcFileName);
#endif
        }

        u8SD_ErrCnt = 0;

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return TRUE;
    }
    else
    {
        if (bSD_ReportErrFlag == FALSE)
        {
            // #if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> SD create and write to file %s ERROR %d.", pcFileName, frRes);
            // #endif
        }

        u8SD_ErrCnt++;

        if (u8SD_ErrCnt >= SD_ERROR_CNT_MAX)
        {
            u8SD_ErrCnt = 0;

            if (bSD_ReportErrFlag == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD max error reached. Reset SD card.");
                // #endif
            }

            SD_Reset();
        }

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        HAL_Delay(1);

        return FALSE;
    }
}

//============================================================================//
// @brief  Initialize SD function processing mechanism.
// @param  None.
// @retval None.
//============================================================================//
void SD_RequestInit(void)
{
    sdRequest.bInUse = FALSE;
    sdRequest.sdCmd = SD_NULL;

    for (int i = 0; i < 5; i++)
        sdRequest.u32Arg[i] = 0;

    sdRequest.pu32RetVal = NULL;
}

//============================================================================//
// @brief  Make a request to do an SD function.
// @param  sdCmd: SD function ID number based on SD_FunctionTypeDef.
//         u8ArgNum: Number of argument.
// @retval SD function result in FRESULT format.
//============================================================================//
FRESULT SD_RequestDo(SD_FunctionTypeDef sdCmd, uint8_t u8ArgNum, ...)
{
    va_list valist;
    uint32_t u32RetVal;
    // uint32_t u32SD_TimeOut = S_2_mS(5);

    if (sdCmd == SD_NULL)
        return FR_OK;

    sdRequest.pu32RetVal = &u32RetVal;

    va_start(valist, u8ArgNum);

    for (int i = 0; i < u8ArgNum; i++)
    {
        sdRequest.u32Arg[i] = va_arg(valist, uint32_t);
    }

    va_end(valist);

    sdRequest.sdCmd = sdCmd;

    while (sdRequest.sdCmd != SD_NULL)
    {
        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (FRESULT)u32RetVal;
}

//============================================================================//
// @brief  Handle SD requests.
// @param  None.
// @retval None
//============================================================================//
void SD_RequestHandle(void)
{
    uint32_t u32Arg[5];
    uint32_t u32RetVal;

    if (sdRequest.sdCmd == SD_NULL)
        return;

    if ((bSystemStop == TRUE) || (bSD_ReportErrFlag == TRUE))
    {
        sdRequest.sdCmd = SD_NULL;

        return;
    }

    u32Arg[0] = sdRequest.u32Arg[0];
    u32Arg[1] = sdRequest.u32Arg[1];
    u32Arg[2] = sdRequest.u32Arg[2];
    u32Arg[3] = sdRequest.u32Arg[3];
    u32Arg[4] = sdRequest.u32Arg[4];

    taskInfo[TASK_SD_PROC].u16TaskStep = (uint8_t)sdRequest.sdCmd;

    u8SD_LastErrCode = (uint8_t)sdRequest.sdCmd;

    switch ((uint8_t)sdRequest.sdCmd)
    {
    case SD_OPEN:
    {
        u32RetVal = (uint32_t)f_open((FIL *)u32Arg[0], (const TCHAR *)u32Arg[1], (BYTE)u32Arg[2]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_CLOSE:
    {
        u32RetVal = (uint32_t)f_close((FIL *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_READ:
    {
        u32RetVal = (uint32_t)f_read((FIL *)u32Arg[0], (void *)u32Arg[1], (UINT)u32Arg[2], (UINT *)u32Arg[3]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_WRITE:
    {
        u32RetVal = (uint32_t)f_write((FIL *)u32Arg[0], (const void *)u32Arg[1], (UINT)u32Arg[2], (UINT *)u32Arg[3]);

        if (g_Setting.tSD_State.u8SD_ResetCnt > 0)
        {
            if (u32RetVal == FR_OK)
            {
                g_Setting.tSD_State.u8SD_ResetCnt = 0;

                System_SaveSetting();
            }
        }

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_LSEEK:
    {
        u32RetVal = (uint32_t)f_lseek((FIL *)u32Arg[0], (DWORD)u32Arg[1]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_OPENDIR:
    {
        u32RetVal = (uint32_t)f_opendir((DIR *)u32Arg[0], (const TCHAR *)u32Arg[1]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_CLOSEDIR:
    {
        u32RetVal = (uint32_t)f_closedir((DIR *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_READDIR:
    {
        u32RetVal = (uint32_t)f_readdir((DIR *)u32Arg[0], (FILINFO *)u32Arg[1]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_MKDIR:
    {
        u32RetVal = (uint32_t)f_mkdir((const TCHAR *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_UNLINK:
    {
        u32RetVal = (uint32_t)f_unlink((const TCHAR *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_RENAME:
    {
        u32RetVal = (uint32_t)f_rename((const TCHAR *)u32Arg[0], (const TCHAR *)u32Arg[1]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_MOUNT:
    {
        u32RetVal = (uint32_t)f_mount((FATFS *)u32Arg[0], (const TCHAR *)u32Arg[1], (BYTE)u32Arg[2]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_MKFS:
    {
        u32RetVal = (uint32_t)f_mkfs((const TCHAR *)u32Arg[0], (BYTE)u32Arg[1], (DWORD)u32Arg[2], (void *)u32Arg[3], (UINT)u32Arg[4]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        for (int i = 0; i < TASK_QUANTITY; i++)
        {
            taskInfo[i].u32LastTick = HAL_GetTick();
        }

        break;
    }

    case SD_TELL:
    {
        u32RetVal = (uint32_t)f_tell((FIL *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_SIZE:
    {
        u32RetVal = (uint32_t)f_size((FIL *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_FIND_FIRST:
    {
        u32RetVal = (uint32_t)f_findfirst((DIR *)u32Arg[0],
                                          (FILINFO *)u32Arg[1],
                                          (const TCHAR *)u32Arg[2],
                                          (const TCHAR *)u32Arg[3]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_DISK_INITIALIZE:
    {
        u32RetVal = (uint32_t)disk_initialize((BYTE)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_DISK_STATUS:
    {
        u32RetVal = (uint32_t)disk_status((BYTE)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_GET_INFO:
    {
        u32RetVal = (uint32_t)SD_GetCardInfo((HAL_SD_CardInfoTypeDef *)u32Arg[0],
                                             (HAL_SD_CardCIDTypeDef *)u32Arg[1],
                                             (HAL_SD_CardCSDTypeDef *)u32Arg[2]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_GET_STATUS:
    {
        u32RetVal = (uint32_t)HAL_SD_GetCardState((SD_HandleTypeDef *)u32Arg[0]);

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = u32RetVal;

        break;
    }

    case SD_RESET:
    {
        SD_PWR_Disable();

        HAL_Delay(300);

        SD_PWR_Enable();

        HAL_Delay(S_2_mS(1));

        if (sdRequest.pu32RetVal != NULL)
            *(sdRequest.pu32RetVal) = 0;

        break;
    }

    default:
        break;
    }

    sdRequest.sdCmd = SD_NULL;
}
