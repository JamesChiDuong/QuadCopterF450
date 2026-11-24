//============================================================================//
// User Includes
//============================================================================//
#include "USB_Module.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "Buzzer.h"
#include "Default.h"
#include "diskio.h"
#include "EXTIO.h"
#include "fatfs.h"
#include "LCD_Key.h"
#include "MX25L_Driver.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//

bool bUSB_Mounted = FALSE;

DIR dirUSB_Handle;

char cUSB_FilePath[128]; // __attribute__ ((section("ccmram_var")));

uint8_t u8PublicDataBuf[8192]; // 8 kBytes

ProcStepTypeDef procUSB_TransStep = PROC_STEP_NULL;

uint8_t u8USB_FileTransRetry = 0;

// bool bUSB_TransFinish = FALSE;

bool bUSB_PrepData = FALSE;

uint16_t u16USB_DownloadNum = 0;

USB_DownloadTypeDef usbDownload;

bool bUSB_RunOne = FALSE;

APP_CONFIG_t gAppCfg;

uint8_t u8USB_ReportEvent = 0;

static FIL filSD_Handle2;
static char cSD_FilePath2[128];

bool bIsEngModeFlag = FALSE;

bool bIsUSBHoldFlag = FALSE;

bool bUSB_CheckSetupFlag = FALSE;

bool bUSB_UndetDisconnect = FALSE;

//============================================================================//
// User Externs
//============================================================================//
extern uint8_t usbh_pipe_delete(usb_core_driver *udev);

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Mount the USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is mounted successfully.
//                 FALSE: USB drive mounting is failed.
//============================================================================//
bool USB_Mount(void)
{
    FRESULT frRes;

    bUSB_Mounted = FALSE;

    frRes = f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);

    if (frRes != FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> Mount to USB disk fail with error code %d.", frRes);
#endif

        return FALSE;
    }
    else
    {
        bUSB_Mounted = TRUE;

        return TRUE;
    }
}

//============================================================================//
// @brief  Unmount the USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is unmounted successfully.
//                 FALSE: USB drive unmounting is failed.
//============================================================================//
bool USB_UnMount(void)
{
    FRESULT frRes;

    frRes = f_mount(NULL, (TCHAR const *)USBHPath, 1);

    if (frRes != FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> Unmount USB disk fail with error code %d.", frRes);
#endif

        return FALSE;
    }
    else
    {
        bUSB_Mounted = FALSE;

        return TRUE;
    }
}

//============================================================================//
// @brief  Check the status of the USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is mounted and working normally.
//                 FALSE: USB drive is not mounted.
//============================================================================//
bool USB_Check(void)
{
    bool bRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    if (bUSB_Ready)
    {
        if (!bUSB_Mounted)
        {
            if (USB_Mount())
                bRes = TRUE;
            else
                bRes = FALSE;
        }
        else
            bRes = TRUE;
    }
    else
        bRes = FALSE;

    if (bRTOS_En)
        osMutexRelease(mutex_usb_user);

    return bRes;
}

//============================================================================//
// @brief  Initialize USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is initialized successfully.
//                 FALSE: USB drive initialization is failed.
//============================================================================//
bool USB_Init(void)
{
    if (!bUSB_Ready)
        return FALSE;

    if (!bUSB_Mounted)
    {
        if (USB_Mount())
            return TRUE;
        else
            return FALSE;
    }
    else
        return TRUE;
}

//============================================================================//
// @brief  Deinitialize USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is deinitialized successfully.
//                 FALSE: USB drive deinitialization is failed.
//============================================================================//
bool USB_DeInit(void)
{
    if (!bUSB_Ready)
        return FALSE;

    if (bUSB_Mounted)
    {
        if (USB_UnMount())
            return TRUE;
        else
            return FALSE;
    }
    else
        return TRUE;
}

//============================================================================//
// @brief  Check for undetected USB disconnection event.
// @param  None.
// @retval None.
//============================================================================//
void USB_CheckDisconnection(void)
{
    static uint32_t u32TickCnt = 0;
    static uint32_t u32TimerOld = 0, u32TimerCurr = 0;
    static uint8_t u8Cnt = 0;
    usb_core_driver *udev = (usb_core_driver *)usb_host_msc.data;

    if (HAL_GetTick() > (u32TickCnt + S_2_mS(1)))
    {
        u32TickCnt = HAL_GetTick();

        if (udev->host.connect_status == 1U)
        {
            u32TimerCurr = usb_host_msc.control.timer;

            if (u32TimerCurr == u32TimerOld)
            {
                u8Cnt++;

                if (u8Cnt > 3)
                {
                    u8Cnt = 0;

                    if (udev->host.connect_status == 1U)
                    {
                        // RTOSprintf("\r\n> USB found undetected disconnection (Timer: %d)", u32TimerCurr);

                        bUSB_UndetDisconnect = TRUE;

                        udev->host.connect_status = 0U;
                    }
                }
            }
            else
            {
                u8Cnt = 0;
            }

            u32TimerOld = u32TimerCurr;
        }
        else
        {
            u8Cnt = 0;
        }
    }
}

//============================================================================//
// @brief  Check if a file name matches with a name or not.
// @param  pcFileName: Pointer to the file name to be checked.
//         pcFilter: Name to be compared with.
// @retval Result: TRUE: The name is matched.
//                 FALSE: The name is not matched.
//============================================================================//
static bool USB_CheckFileByName(char *pcFileName, char *pcFilter)
{
    uint8_t Buf[128];
    char *pcStr;

    if (strlen(pcFileName) > sizeof(Buf))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file name is too long!");
#endif

        return FALSE;
    }

    pcStr = (char *)Buf;

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
static bool USB_CheckFileByExten(char *pcFileName, char *pcFilter)
{
    uint8_t Buf[128];
    char *pcStr;

    if (strlen(pcFileName) > sizeof(Buf))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file extension is too long!");
#endif

        return FALSE;
    }

    pcStr = (char *)Buf;

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
//                 FALSE: The file not exist/ operation failed.
//============================================================================//
bool USB_CheckFileExist(char *pcFileName)
{
    // Check USB mounting status
    if (!USB_Check())
        return FALSE;

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

    // Try to open file
    if (f_open(&USBHFile, cUSB_FilePath, FA_READ) == FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file %s exists.", pcFileName);
#endif

        f_close(&USBHFile);

        return TRUE;
    }
    else
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file %s does not exist.", pcFileName);
#endif

        return FALSE;
    }
}

//============================================================================//
// @brief  Find a file having a suggested name.
// @param  pcFindName: Pointer to the file name to find.
//         pcReturnName: Pointer to the name of the found file.
// @retval Result: TRUE: A suitable file is found.
//                 FALSE: No file is found.
//============================================================================//
bool USB_FindFileName(char *pcFindName, char *pcReturnName)
{
    bool bRes = FALSE;
    FILINFO finfoHandle;
    char cName[128], cExten[5];
    char *pcStr;

    if (strlen(pcFindName) > sizeof(cUSB_FilePath))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file name to find is too long!");
#endif

        return FALSE;
    }

    strcpy(cUSB_FilePath, pcFindName);

    pcStr = cUSB_FilePath;

    // Split name and extension
    StopChar(pcStr, '.');

    strcpy(cName, pcStr);

    pcStr += strlen(pcStr) + 1;

    if (*pcStr) // If there is extension
        strcpy(cExten, pcStr);
    else // If there is no extension
        sprintf(cExten, "*");

    // Check USB drive status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    // Check files through the USB disk
    while (f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
    {
        // If encounter no name files then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        // If find a random file
        if ((strcmp(cName, "*") == 0) && (strcmp(cExten, "*") == 0))
        {
            sprintf(pcReturnName, "%s", finfoHandle.fname);

            bRes = TRUE;

            break;
        }
        else if (strcmp(cExten, "*") == 0) // If not care about file extension
        {
            if (USB_CheckFileByName(finfoHandle.fname, cName) == TRUE)
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
        else if (strcmp(cName, "*") == 0) // If not care about file name
        {
            if (USB_CheckFileByExten(finfoHandle.fname, cExten) == TRUE)
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
        else // If both name and extension have to be matched
        {
            if ((USB_CheckFileByName(finfoHandle.fname, cName) == TRUE) &&
                (USB_CheckFileByExten(finfoHandle.fname, cExten) == TRUE))
            {
                sprintf(pcReturnName, "%s", finfoHandle.fname);

                bRes = TRUE;

                break;
            }
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Get size of a file.
// @param  pcFileName: Pointer to file name for getting size.
// @retval File size in the uint32_t format.
//============================================================================//
uint32_t USB_GetFileSize(char *pcFileName)
{
    uint32_t u32FileSize;

    // Check USB drive status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return 0;
    }

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

    // Try to open file
    if (f_open(&USBHFile, cUSB_FilePath, FA_READ) != FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n>  SD disk file name %s invalid(get file)).", pcFileName);
#endif

        return 0;
    }

    // Get file size
    u32FileSize = f_size(&USBHFile);

    f_close(&USBHFile);

    return u32FileSize;
}

//============================================================================//
// @brief  Print a list of files in the USB drive.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool USB_FileList(void)
{
    FILINFO finfoHandle;

    // Check SD card status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    sprintf(cUSB_FilePath, "%d:/", DRV_USB);

    RTOSprintf("\r\n> File list:");

    // Check files through the SD card
    while (f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
    {
        // If encounter no name files then stop
        if (finfoHandle.fname[0] == 0)
            break;

        // If file attribute is AM_ARC then continue
        if ((finfoHandle.fattrib & AM_ARC) == 0)
            continue;

        RTOSprintf("\r\n> %s", finfoHandle.fname);
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the last time when a file is modified in local time.
// @param  pcFileName: Pointer to file name to be checked.
// @retval Time in time_t format
//============================================================================//
static time_t USB_GetFileTime(FILINFO *pfinfoHandle)
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
// @brief  Delete a file existing longer than a keeptime period.
// @param  pcFileNames: Pointer to file name for deleting.
//         u8KeepTime: Time period to keep file in days.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_ClearUp(char *pcFileNames, uint8_t u8KeepTime)
{
    FRESULT frRes;
    FILINFO finfoHandle;
    char cFileName[128], cFileExten[5];
    char *pcStr;

    if (strlen(pcFileNames) > sizeof(cUSB_FilePath))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file name to clear up is too long!");
#endif

        return FALSE;
    }

    strcpy(cUSB_FilePath, pcFileNames);

    pcStr = cUSB_FilePath;

    StopChar(pcStr, '.');

    // Get file name
    strcpy(cFileName, pcStr);

    pcStr += strlen(pcStr) + 1;

    // Get file extension
    strcpy(cFileExten, pcStr);

    // Check SD card status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    // Check files through the SD card
    while (f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
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
        if ((ttTimeNow - USB_GetFileTime(&finfoHandle)) < DAY_2_SECOND(u8KeepTime))
            continue;

        // If delete all files
        if (strcmp(cFileName, "*") == 0 &&
            strcmp(cFileExten, "*") == 0)
        {
            sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

            frRes = f_unlink(cUSB_FilePath);

            if (frRes == FR_OK)
            {
#if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n>  USB drive delete file name %s OK.", finfoHandle.fname);
#endif
            }
            else
            {
#if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n>  USB drive delete file name %s error.", finfoHandle.fname);
#endif
            }
        }
        else if (strcmp(cFileName, "*") == 0) // If delete files with matched extension
        {
            if (USB_CheckFileByExten(finfoHandle.fname, cFileExten) == TRUE)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
        else if (strcmp(cFileExten, "*") == 0) // If delete files with matched name
        {
            if (USB_CheckFileByName(finfoHandle.fname, cFileName) == TRUE)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB disk delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB disk delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
        else // If both name and extension have to be matched
        {
            if (strncmp(finfoHandle.fname, pcFileNames, strlen(pcFileNames)) == 0)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Delete a file/ files.
// @param  pcNames: Name or extension of the files to be deleted.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_Delete(char *pcNames)
{
    return USB_ClearUp(pcNames, 0);
}

//============================================================================//
// @brief  Delete all files in USB disk except a file having a chosen name or
//         extension.
// @param  pcNames: Name or extension of the file to be kept.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_DelExcept(char *pcNames)
{
    FRESULT frRes;
    FILINFO finfoHandle;
    char cFileName[128], cFileExten[128];
    char *pcStr;

    if (strlen(pcNames) > sizeof(cUSB_FilePath))
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file name to delete except is to long!");
#endif

        return FALSE;
    }

    strcpy(cUSB_FilePath, pcNames);

    pcStr = cUSB_FilePath;

    StopChar(pcStr, '.');

    // Get file name
    strcpy(cFileName, pcStr);

    pcStr += strlen(pcStr) + 1;

    // Get file extension
    strcpy(cFileExten, pcStr);

    // Check USB disk status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    // Check files through the USB disk
    while (f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
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
            if (USB_CheckFileByExten(finfoHandle.fname, cFileExten) == FALSE)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB disk delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB disk delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
        else if (strcmp(cFileExten, "*") == 0) // If only keep files with same name
        {
            if (USB_CheckFileByName(finfoHandle.fname, cFileExten) == FALSE)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
        else // If only keep a file with exactly matched name and extension
        {
            if (strncmp(finfoHandle.fname, pcNames, strlen(pcNames)) != 0)
            {
                sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, finfoHandle.fname);

                frRes = f_unlink(cUSB_FilePath);

                if (frRes == FR_OK)
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s OK.", finfoHandle.fname);
#endif
                }
                else
                {
#if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>  USB drive delete file name %s error.", finfoHandle.fname);
#endif
                }
            }
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Remove a file in the USB.
// @param  pcFileNames: Pointer to the name of the file to be removed.
// @retval Result: TRUE: The file is removed successfully.
//                 FALSE: Removing the file is failed.
//============================================================================//
bool USB_RemoveFile(char *pcFilenames)
{
    FRESULT frRes;

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFilenames);

    frRes = f_unlink(cUSB_FilePath);

    if (frRes == FR_OK)
    {
        // RTOSprintf("\r\n### USB remove file %s OK ###", filenames);

        return TRUE;
    }
    else
    {
        // RTOSprintf("\r\n### USB remove file %s FAIL ###", filenames);

        return FALSE;
    }
}

//============================================================================//
// @brief  Rename a file in the USB.
// @param  pcOldName: Old name of the file.
//         pcNewName: New name for the file.
// @retval Result: TRUE: The file is removed successfully.
//                 FALSE: Removing the file is failed.
//============================================================================//
bool USB_Rename(char *pcOldName, char *pcNewName)
{
    FRESULT frRes;

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcOldName);

    frRes = f_rename(cUSB_FilePath, pcNewName);

    if (frRes == FR_OK)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Write to the bottom of a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8USB_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of the data for writing.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_AddBuffToFile(char *pcFileName, uint8_t *pu8USB_Buf, uint16_t u16BufLen)
{
    FRESULT frRes;
    UINT uBw;

    // Check USB disk status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

    // Open file
    frRes = f_open(&USBHFile, cUSB_FilePath, FA_WRITE);

    if (frRes == FR_OK)
    {
        f_lseek(&USBHFile, f_size(&USBHFile));
    }
    else // If the specified file is not exist then create a new file
    {
        f_close(&USBHFile);

        f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
    }

    frRes = f_write(&USBHFile, pu8USB_Buf, u16BufLen, &uBw);

    if (frRes != FR_OK)
    {
        RTOSprintf("\r\n> USB add buff to file %s ERROR %d", pcFileName, frRes);
    }

    f_close(&USBHFile);

    if (frRes == FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB write to file %s OK.", pcFileName);
#endif

        return TRUE;
    }
    else
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB write to file %s ERROR %d.", pcFileName, frRes);
#endif

        return FALSE;
    }
}

//============================================================================//
// @brief  Write a string to the bottom of a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pcString: Pointer to string for writing.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_AddStrToFile(char *pcFileName, char *pcString)
{
    return USB_AddBuffToFile(pcFileName, (uint8_t *)pcString, strlen(pcString));
}

//============================================================================//
// @brief  Read from an address in a file.
// @param  pcFileName: Pointer to the file name for reading.
//         pu8USB_Buf: Pointer to the data buffer for storing.
//         u16BufLen: Length of the data buffer for reading.
//         u32Addr: Address to read from in the file.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_ReadAddrToBuff(char *pcFileName, uint8_t *pu8USB_Buf,
                        uint16_t u16BufLen, uint32_t u32Addr)
{
    FRESULT frRes;
    UINT uBr;

    // Check USB disk status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

    // Open file for reading
    if (f_open(&USBHFile, cUSB_FilePath, FA_READ) != FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB file name %s invalid(read).", pcFileName);
#endif

        return FALSE;
    }

    // Put cursor the the start address
    f_lseek(&USBHFile, u32Addr);

    frRes = f_read(&USBHFile, pu8USB_Buf, u16BufLen, &uBr);

    if (frRes != FR_OK)
    {
        RTOSprintf("\r\n> USB read file %s at address %d ERROR %d", pcFileName, u32Addr, frRes);
    }

    f_close(&USBHFile);

    if (frRes == FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB read file %s OK.", pcFileName);
#endif

        return TRUE;
    }
    else
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB write file %s ERROR %d.", pcFileName, frRes);
#endif

        return FALSE;
    }
}

//============================================================================//
// @brief  Read from the beginning of a file.
// @param  pcFileName: Pointer the to file name for reading.
//         pu8USB_Buf: Pointer to the data buffer for storing.
//         u16BufLen: Length of the data for reading.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_ReadFileToBuff(char *pcFileName, uint8_t *pu8USB_Buf, uint16_t u16BufLen)
{
    return USB_ReadAddrToBuff(pcFileName, pu8USB_Buf, u16BufLen, 0);
}

//============================================================================//
// @brief  Write to an address in a file.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8USB_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of the data for writing.
//         u32Addr: Address to start write to.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_WriteBuffToAddr(char *pcFileName, uint8_t *pu8USB_Buf,
                         uint16_t u16BufLen, uint32_t u32Addr)
{
    FRESULT frRes;
    UINT uBw;

    // Check USB drive status
    if (!USB_Check())
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf(MSG_USB_DRIVE_INVALID);
#endif

        return FALSE;
    }

    sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

    // Open file
    if (f_open(&USBHFile, cUSB_FilePath, FA_WRITE) != FR_OK)
    {
        f_close(&USBHFile);

        f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
    }

    // Put cursor to the start address
    f_lseek(&USBHFile, u32Addr);

    frRes = f_write(&USBHFile, pu8USB_Buf, u16BufLen, &uBw);

    if (frRes != FR_OK)
    {
        RTOSprintf("\r\n> USB write to file %s at address ERROR %d", pcFileName, u32Addr, frRes);
    }

    f_close(&USBHFile);

    if (frRes == FR_OK)
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB write file %s OK.", pcFileName);
#endif

        return TRUE;
    }
    else
    {
#if ENABLE_DEBUG_MSG
        RTOSprintf("\r\n> USB write file %s ERROR %d.", pcFileName, frRes);
#endif

        return FALSE;
    }
}

//============================================================================//
// @brief  Write to a file from the beginning.
// @param  pcFileName: Pointer to the file name for writing.
//         pu8USB_Buf: Pointer to the data buffer for writing.
//         u16BufLen: Length of data for writing.
// @retval Result: TRUE: Operation success.
//                 FALSE: Operation failed.
//============================================================================//
bool USB_WriteBuffToFile(char *pcFileName, uint8_t *pu8USB_Buf, uint16_t u16BufLen)
{
    return USB_WriteBuffToAddr(pcFileName, pu8USB_Buf, u16BufLen, 0);
}

//============================================================================//
// @brief  Download to Chinese font file to external flash.
// @param  @fatfsHandle: Pointer to an instance of FATFS.
//         @cDrvPath: Path to drive.
//         @u8Drv: Drive SD or USB.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail.
//============================================================================//
bool SaveChineseFont(void)
{
    bool bRes;
    MX25L_StatusTypeDef mxlRes;
    uint32_t u32StartAddr = (uint32_t)EXFLASH_CHARACTERS_LIB_START_ADDR;
    uint32_t u32FileSize, u32DatCnt = 0, u32DatSize;
    uint16_t u16PageAddr = 0, u16SectEraseNum = 0;

    u32FileSize = USB_GetFileSize((char *)CHINESE_FONT_FILE);

    if (u32FileSize == 0)
    {
        RTOSprintf("\r\n> Size of character font file %s is 0.", CHINESE_FONT_FILE);

        return TRUE;
    }

    RTOSprintf("\r\n> SD size of character font file %s is %d.", CHINESE_FONT_FILE, u32FileSize);

    u16SectEraseNum = (uint16_t)(u32FileSize / MX25L_SECTOR_SIZE);

    if (u32FileSize % MX25L_SECTOR_SIZE)
        u16SectEraseNum++;

    mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);

    if (mxlRes != MX25L_OK)
    {
        RTOSprintf("\r\n> Erase Exflash ERROR %d. Terminate download character font file.", mxlRes);

        return FALSE;
    }

    RTOSprintf("\r\n> Start download character font file from SD to Exflash.");

    u32DatCnt = 0;

    do
    {
        osStatus osRes = osMutexWait(mutex_data_buf2_user, S_2_mS(20));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> Can't acquire DATA BUF2 mutex to transfer file. osRes:%d", osRes);

            break;
        }

        u32DatSize = u32FileSize - u32DatCnt;

        if (u32DatSize > sizeof(u8PublicDataBuf))
            u32DatSize = sizeof(u8PublicDataBuf);

        bRes = USB_ReadAddrToBuff((char *)CHINESE_FONT_FILE, u8PublicDataBuf, u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> Reading file %s at address %d ERROR.", CHINESE_FONT_FILE, u32DatCnt);

            osMutexRelease(mutex_data_buf2_user);

            break;
        }

        u16PageAddr = (uint16_t)((u32StartAddr + u32DatCnt) / MX25L_PAGE_SIZE);

        mxlRes = MX25L_MultiPagesProgram((uint32_t)u16PageAddr, u8PublicDataBuf, u32DatSize);

        osMutexRelease(mutex_data_buf2_user);

        if (mxlRes == MX25L_OK)
        {
            u32DatCnt += u32DatSize;

            if ((u32DatCnt % (sizeof(u8PublicDataBuf) * 4)) == 0)
            {
                RTOSprintf("\r\n> Finish transfer %d bytes.", u32DatCnt);
            }
        }
        else
        {
            RTOSprintf("\r\n> Program to Exflash at page %d fail with ERROR %d.", u16PageAddr, mxlRes);

            break;
        }

        HAL_Delay(2);

    } while (u32DatCnt < u32FileSize);

    if ((bRes != TRUE) || (mxlRes != MX25L_OK))
    {
        mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);

        if (mxlRes != MX25L_OK)
        {
            RTOSprintf("\r\n> Erase Exflash ERROR %d.", mxlRes);
        }

        RTOSprintf("\r\n> Download character font file FAIL.");

        Buzzer_Beep(BEEP_LONG, 1);

        return FALSE;
    }
    else
    {
        RTOSprintf("\r\n> Download character font file OK.");

        Buzzer_Beep(BEEP_SHORT, 3);

        return TRUE;
    }
}

//============================================================================//
// @brief  Handle the USB file download process.
// @param  None.
// @retval None.
//============================================================================//
static void USB_Download_Proc(void)
{
    static ProcStepTypeDef procUSB_LastTransStep;
    static char cUSB_TransFile[128], cSD_TransFile[128];
    FRESULT frRes;
    // FIL filSD_Handle;
    // char cSD_FilePath[100];
    char *pcStr;
    bool bRes;
    static uint32_t u32FilSize, u32DatCnt, u32DatSize, u32GetTick;
    UINT uBw, uBr;
    static uint8_t u8USB_ErrCode;
    static uint32_t u32DlAddr;

    if ((bSD_ReportErrFlag == TRUE) || (procUSB_TransStep == PROC_STEP_NULL))
        return;

    if (procUSB_TransStep != PROC_STEP_7)
    {
        procUSB_LastTransStep = procUSB_TransStep;
    }

    switch (procUSB_TransStep)
    {
    // Step 1: Check designated file existence in SD card and get its size.
    case PROC_STEP_1:
    {
        HAL_Delay(100);

        RTOSprintf("\r\n> Proceed to download file:%s", usbDownload.cSrcFile);

        RTOSprintf("\r\n> Download type: %d", usbDownload.usbDL_Type);

        LCD_SetShowAutoTestPage(26);

        u8USB_ErrCode = 0;

        u32DlAddr = 0;

        memset(cSD_TransFile, 0x00, sizeof(cSD_TransFile));

        memset(cUSB_TransFile, 0x00, sizeof(cUSB_TransFile));

        sprintf(cSD_TransFile, "%s", usbDownload.cSrcFile);

        pcStr = usbDownload.cDstFile;

        if (strncmp(pcStr, "VDR/", 4) == 0)
        {
            StopChar(pcStr, '/');

            pcStr += strlen(pcStr) + 1;

            sprintf(cUSB_TransFile, "%s", pcStr);
        }
        else if (strncmp(pcStr, "DTR/", 4) == 0)
        {
            StopChar(pcStr, '/');

            pcStr += strlen(pcStr) + 1;

            sprintf(cUSB_TransFile, "%s", pcStr);
        }
        else
        {
            sprintf(cUSB_TransFile, "%s", pcStr);
        }

        RTOSprintf("\r\n> USB trans file:%s", cUSB_TransFile);

        bRes = SD_CheckFileExist(cSD_TransFile);

        if (!bRes)
        {
            RTOSprintf("\r\n> Not found designated file %s for SD-USB file transfer.", cSD_TransFile);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                procUSB_TransStep = PROC_STEP_7;

                LCD_SetShowAutoTestPage(43);

                break;
            }

            procUSB_TransStep = PROC_STEP_1;

            break;
        }

        u32FilSize = SD_GetFileSize(cSD_TransFile);

        if (u32FilSize == 0)
        {
            RTOSprintf("\r\n> Get size of file %s in SD FAIL.", cSD_TransFile);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                procUSB_TransStep = PROC_STEP_7;

                break;
            }

            procUSB_TransStep = PROC_STEP_1;

            break;
        }

        u8USB_FileTransRetry = 0;

        LOGprintf("\r\n> Found designated file for SD-USB file transfer. File size: %d Bytes", u32FilSize);

        procUSB_TransStep = PROC_STEP_2;

        break;
    }

    // Step 2: Check if the same file is existing in USB then delete it.
    case PROC_STEP_2:
    {
        if (USB_CheckFileExist(cUSB_TransFile))
        {
            bRes = USB_RemoveFile(cUSB_TransFile);

            if (!bRes)
            {
                RTOSprintf("\r\n> Remove file %s in USB FAIL.", cUSB_TransFile);

                u8USB_FileTransRetry++;

                if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                {
                    u8USB_FileTransRetry = 0;

                    RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                    procUSB_TransStep = PROC_STEP_7;

                    break;
                }

                procUSB_TransStep = PROC_STEP_2;

                break;
            }

            LOGprintf("\r\n> Remove file %s in USB successfully.", cUSB_TransFile);
        }

        memset(cUSB_FilePath, 0x00, sizeof(cUSB_FilePath));

        sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, cUSB_TransFile);

        frRes = f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

        if (frRes != FR_OK)
        {
            RTOSprintf("\r\n> Create designated file %s in USB ERROR %d.", cUSB_TransFile, frRes);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                u8USB_ErrCode = frRes;

                procUSB_TransStep = PROC_STEP_7;

                break;
            }

            procUSB_TransStep = PROC_STEP_2;

            break;
        }

        // Put cursor at the beginning of the file
        f_lseek(&USBHFile, 0);

        u32DatCnt = 0;

        LOGprintf("\r\n> Start transferring file %s from SD to USB.", cUSB_TransFile);

        u32GetTick = HAL_GetTick();

        procUSB_TransStep = PROC_STEP_3;

        break;
    }

    case PROC_STEP_3:
    {
        bRes = TRUE;
        osStatus osRes;

        osRes = osMutexWait(mutex_data_buf2_user, S_2_mS(20));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> Can't acquire DATA BUF2 mutex to transfer file. osRes:%d", osRes);

            bRes = FALSE;
        }

        if (bRes == TRUE)
        {
            // Check SD card status
            if (SD_FS_GetStatus() == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf(MSG_SD_DISK_INVALID);
                // #endif

                bRes = FALSE;
            }
        }

        if (bRes == TRUE)
        {
            if (bRTOS_En)
            {
                osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

                if (osRes != osOK)
                    bRes = FALSE;
            }
        }

        if (bRes == TRUE)
        {
            memset(cSD_FilePath2, 0x00, sizeof(cSD_FilePath2));

            sprintf(cSD_FilePath2, "%d:/%s", DRV_USER, cSD_TransFile);

            frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle2, cSD_FilePath2, FA_READ);

            // Open file for reading
            if (frRes != FR_OK)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD-USB open file %s ERROR %d.", cSD_TransFile, frRes);
                // #endif

                bRes = FALSE;
            }
            else
                // Put cursor to the address
                SD_RequestDo(SD_LSEEK, 2, &filSD_Handle2, u32DatCnt);
        }

        for (int i = 0; i < 10; i++)
        {
            if (bRes == TRUE)
            {
                u32DatSize = u32FilSize - u32DatCnt;

                if (u32DatSize > sizeof(u8PublicDataBuf))
                    u32DatSize = sizeof(u8PublicDataBuf);

                if (!bRTOS_En)
                    frRes = f_read(&filSD_Handle2, u8PublicDataBuf, u32DatSize, &uBr);
                else
                    frRes = SD_RequestDo(SD_READ, 4, &filSD_Handle2, u8PublicDataBuf, u32DatSize, &uBr);

                if (frRes != FR_OK)
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD-USB read file %s ERROR %d.", cUSB_TransFile, (int)frRes);
                    // #endif

                    bRes = FALSE;
                }
            }

            if (bRes == TRUE)
            {
                frRes = f_write(&USBHFile, u8PublicDataBuf, u32DatSize, &uBw);

                if (frRes != FR_OK)
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    f_close(&USBHFile);

                    RTOSprintf("\r\n> SD-USB write fo file %s at %d ERROR %d.",
                               cUSB_TransFile, u32DatCnt, frRes);

                    u8USB_FileTransRetry++;

                    if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                    {
                        u8USB_FileTransRetry = 0;

                        RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                        u8USB_ErrCode = frRes;

                        u32DlAddr = u32DatCnt;

                        procUSB_TransStep = PROC_STEP_7;
                    }
                    else
                        procUSB_TransStep = PROC_STEP_2;

                    if (bRTOS_En)
                        osMutexRelease(mutex_sd_proc_user);

                    if (bRTOS_En)
                        osMutexRelease(mutex_data_buf2_user);

                    HAL_Delay(1);

                    return;
                }

                // RTOSprintf("\r\n> SD-USB read time:%dms, write time:%dms", u32ReadTime, u32WriteTime);

                if ((u32DatCnt % USB_DOWNLOAD_STEP_AMOUNT) == 0)
                {
                    RTOSprintf("\r\n> Finish transferring %d bytes from SD to USB.", u32DatCnt);

                    Buzzer_Beep(BEEP_LONG, 1);
                }

                u32DatCnt += u32DatSize;

                if (u32DatCnt < u32FilSize)
                {
                    procUSB_TransStep = PROC_STEP_3;
                }
                else
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    f_close(&USBHFile);

                    if (bRTOS_En)
                        osMutexRelease(mutex_sd_proc_user);

                    if (bRTOS_En)
                        osMutexRelease(mutex_data_buf2_user);

                    switch ((int)usbDownload.usbDL_Type)
                    {
                    case USB_DL_VDR_HOUR:
                    {
                        uint8_t u8ChkSum = 0xEE, u8ChkSumTmp = 0;
                        uint32_t u32FilSize = 0;
                        bool bRes2 = FALSE;

                        // RTOSprintf("\r\n> Add Technical data to USB Hour VDR file");

                        V161_WriteTechnicalToUSB(cUSB_TransFile);

                        /*
                        //RTOSprintf("\r\n> Add CheckSum to USB Hour VDR file");

                      for(int i = 0; i < 3; i++)
                      {
                          bRes = USB_AddBuffToFile(cUSB_TransFile, &u8ChkSum, SHORTHEAD);

                            if(bRes)
                                break;

                            HAL_Delay(100);
                        }

                        if(bRes == TRUE)
                        {
                        RTOSprintf("\r\n> Add CheckSum to USB Hour VDR file OK");
                        }
                        else
                        {
                            RTOSprintf("\r\n> Add CheckSum to USB Hour VDR file ERROR");

                          #if ENABLE_GSM_NET
                            GSM_TCP_ReportEvent((char *)EVENT_VDR_MAKE_FAIL, (char *)"4");
                            #endif
                        }
                        */

                        // Verify checksum
                        // if(bRes)
                        {
                            u32FilSize = USB_GetFileSize(cUSB_TransFile);

                            if (u32FilSize > 0)
                            {
                                bRes2 = USB_ReadAddrToBuff(cUSB_TransFile, &u8ChkSumTmp, 1, u32FilSize - 1);

                                if (bRes2)
                                {
                                    if (u8ChkSumTmp != u8ChkSum)
                                    {
#if ENABLE_GSM_NET
                                        GSM_TCP_ReportEvent((char *)EVENT_VDR_MAKE_FAIL, (char *)"5");
#endif

                                        for (int i = 0; i < 3; i++)
                                        {
                                            bRes = USB_AddBuffToFile(cUSB_TransFile, &u8ChkSum, SHORTHEAD);

                                            if (bRes)
                                                break;

                                            HAL_Delay(100);
                                        }

                                        if (bRes == FALSE)
                                        {
#if ENABLE_GSM_NET
                                            GSM_TCP_ReportEvent((char *)EVENT_VDR_MAKE_FAIL, (char *)"6");
#endif
                                        }
                                    }
                                    else
                                    {
                                        RTOSprintf("\r\n> Verify USB Hour VDR file checksum OK.");
                                    }
                                }
                                else
                                {
                                    RTOSprintf("\r\n> Verify USB Hour VDR file checksum get checksum ERROR.");
                                }
                            }
                            else
                            {
                                RTOSprintf("\r\n> Verify USB Hour VDR file checksum get file size ERROR.");
                            }
                        }

                        break;
                    }

                    case USB_DL_NONE:
                    default:
                        break;
                    }

                    RTOSprintf("\r\n> ### Downloading file %s from SD to USB complete ###", cUSB_TransFile);

                    RTOSprintf("\r\n> ### Total Used: %d ms ###", (HAL_GetTick() - u32GetTick));

                    // Not get SYSLOG Process
                    LCD_SetShowAutoTestPage(38);

                    Buzzer_Beep(BEEP_SHORT, 3);

                    if (bRes == TRUE)
                    {
#if ENABLE_GSM_NET
                        GSM_TCP_ReportEvent((char *)EVENT_USB_DOWNLOAD_SUCCESS, STR_NULL);
#endif
                    }

                    procUSB_TransStep = PROC_STEP_NULL;

                    // Not get SYSLOG Process
                    // if(bIsDelVDRFileFlag == TRUE)
                    //{
                    // bIsDelVDRFileFlag = FALSE;

                    // RTOSprintf("\r\n> ### USB copy OK, Delete %s file ###", cSD_TransFile);

                    // SD_RemoveFile(cSD_TransFile);
                    //}

                    bSysLogEn = TRUE;

                    HAL_Delay(1);

                    return;
                }
            }
            else
            {
                RTOSprintf("\r\n> SD-USB read file %s at address %d ERROR.", cUSB_TransFile, u32DatCnt);

                u8USB_FileTransRetry++;

                if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                {
                    u8USB_FileTransRetry = 0;

                    RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                    u32DlAddr = u32DatCnt;

                    procUSB_TransStep = PROC_STEP_7;
                }
                else
                    procUSB_TransStep = PROC_STEP_3;

                if (bRTOS_En)
                    osMutexRelease(mutex_sd_proc_user);

                if (bRTOS_En)
                    osMutexRelease(mutex_data_buf2_user);

                HAL_Delay(1);

                return;
            }
        }

        u8USB_FileTransRetry = 0;

        SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        if (bRTOS_En)
            osMutexRelease(mutex_data_buf2_user);

        HAL_Delay(1);

        break;
    }

    // Step 4: Check SYSLOG file existence in SD card and get its size.
    case PROC_STEP_4:
    {
        HAL_Delay(100);

        memset(cSD_TransFile, 0x00, sizeof(cSD_TransFile));

        memset(cUSB_TransFile, 0x00, sizeof(cUSB_TransFile));

        sprintf(cSD_TransFile, "%s", (char *)FILENAME_SYSLOG);

        sprintf(cUSB_TransFile, "%s", (char *)FILENAME_SYSLOG);

        bRes = SD_CheckFileExist(cSD_TransFile);

        if (!bRes)
        {
            RTOSprintf("\r\n> Not found file %s for SD-USB file transfer.", cSD_TransFile);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                procUSB_TransStep = PROC_STEP_7;

                break;
            }

            procUSB_TransStep = PROC_STEP_4;

            break;
        }

        bSysLogEn = FALSE;

        HAL_Delay(20);

        u32FilSize = SD_GetFileSize(cSD_TransFile);

        if (u32FilSize == 0)
        {
            RTOSprintf("\r\n> Get size of file %s in SD FAIL.", cSD_TransFile);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                procUSB_TransStep = PROC_STEP_7;

                bSysLogEn = TRUE;

                break;
            }

            procUSB_TransStep = PROC_STEP_4;

            break;
        }

        u8USB_FileTransRetry = 0;

        LOGprintf("\r\n> Found SYSLOG file for SD-USB file transfer.");

        procUSB_TransStep = PROC_STEP_5;

        break;
    }

    // Step 5: Check if the same file is existing in USB then delete it.
    case PROC_STEP_5:
    {
        if (USB_CheckFileExist(cUSB_TransFile))
        {
            bRes = USB_RemoveFile(cUSB_TransFile);

            if (!bRes)
            {
                RTOSprintf("\r\n> Remove file %s in USB FAIL.", cUSB_TransFile);

                u8USB_FileTransRetry++;

                if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                {
                    u8USB_FileTransRetry = 0;

                    RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                    procUSB_TransStep = PROC_STEP_7;

                    bSysLogEn = TRUE;

                    break;
                }

                procUSB_TransStep = PROC_STEP_5;

                break;
            }

            LOGprintf("\r\n> Remove file %s in USB successfully.", cUSB_TransFile);
        }

        memset(cUSB_FilePath, 0x00, sizeof(cUSB_FilePath));

        sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, cUSB_TransFile);

        frRes = f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

        if (frRes != FR_OK)
        {
            RTOSprintf("\r\n> Create designated file %s in USB ERROR %d.", cUSB_TransFile, frRes);

            u8USB_FileTransRetry++;

            if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
            {
                u8USB_FileTransRetry = 0;

                RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                u8USB_ErrCode = frRes;

                procUSB_TransStep = PROC_STEP_7;

                bSysLogEn = TRUE;

                break;
            }

            procUSB_TransStep = PROC_STEP_5;

            break;
        }

        // Put cursor at the beginning of the file
        f_lseek(&USBHFile, 0);

        u32DatCnt = 0;

        RTOSprintf("\r\n> Start transferring file %s from SD to USB.", cUSB_TransFile);

        u32GetTick = HAL_GetTick();

        procUSB_TransStep = PROC_STEP_6;

        break;
    }

    // Step 6: Transfer SYSLOG file from SD to USB.
    case PROC_STEP_6:
    {
        bRes = TRUE;
        osStatus osRes;
        // uint32_t u32ReadTime, u32WriteTime;

        osRes = osMutexWait(mutex_data_buf2_user, S_2_mS(20));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> Can't acquire DATA BUF2 mutex to transfer file. osRes:%d", osRes);

            bRes = FALSE;
        }

        if (bRes == TRUE)
        {
            // Check SD card status
            if (SD_FS_GetStatus() == FALSE)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf(MSG_SD_DISK_INVALID);
                // #endif

                bRes = FALSE;
            }
        }

        if (bRes == TRUE)
        {
            if (bRTOS_En)
            {
                osRes = osMutexWait(mutex_sd_proc_user, S_2_mS(10));

                if (osRes != osOK)
                    bRes = FALSE;
            }
        }

        if (bRes == TRUE)
        {
            memset(cSD_FilePath2, 0x00, sizeof(cSD_FilePath2));

            sprintf(cSD_FilePath2, "%d:/%s", DRV_USER, cSD_TransFile);

            frRes = SD_RequestDo(SD_OPEN, 3, &filSD_Handle2, cSD_FilePath2, FA_READ);

            // Open file for reading
            if (frRes != FR_OK)
            {
                // #if ENABLE_DEBUG_MSG
                RTOSprintf("\r\n> SD-USB open file %s ERROR %d.", cSD_TransFile, frRes);
                // #endif

                bRes = FALSE;
            }
            else
                // Put cursor to the address
                SD_RequestDo(SD_LSEEK, 2, &filSD_Handle2, u32DatCnt);
        }

        for (int i = 0; i < 10; i++)
        {
            if (bRes == TRUE)
            {
                u32DatSize = u32FilSize - u32DatCnt;

                if (u32DatSize > sizeof(u8PublicDataBuf))
                    u32DatSize = sizeof(u8PublicDataBuf);

                frRes = SD_RequestDo(SD_READ, 4, &filSD_Handle2, u8PublicDataBuf, u32DatSize, &uBr);

                if (frRes != FR_OK)
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n> SD-USB read file %s ERROR %d.", cUSB_TransFile, (int)frRes);
                    // #endif

                    bRes = FALSE;
                }
            }

            if (bRes == TRUE)
            {
                // Put cursor to the write address
                frRes = f_write(&USBHFile, u8PublicDataBuf, u32DatSize, &uBw);

                if (frRes != FR_OK)
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    f_close(&USBHFile);

                    RTOSprintf("\r\n> SD-USB write fo file %s at %d ERROR %d.",
                               cUSB_TransFile, u32DatCnt, frRes);

                    u8USB_FileTransRetry++;

                    if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                    {
                        u8USB_FileTransRetry = 0;

                        RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                        u8USB_ErrCode = frRes;

                        u32DlAddr = u32DatCnt;

                        procUSB_TransStep = PROC_STEP_7;

                        bSysLogEn = TRUE;
                    }
                    else
                        procUSB_TransStep = PROC_STEP_5;

                    if (bRTOS_En)
                        osMutexRelease(mutex_sd_proc_user);

                    if (bRTOS_En)
                        osMutexRelease(mutex_data_buf2_user);

                    HAL_Delay(1);

                    return;
                }

                // RTOSprintf("\r\nRead time:%dms, write time:%dms", u32ReadTime, u32WriteTime);

                if ((u32DatCnt % USB_DOWNLOAD_STEP_AMOUNT) == 0)
                {
                    RTOSprintf("\r\n> Finish transferring %d bytes from SD to USB.", u32DatCnt);

                    Buzzer_Beep(BEEP_LONG, 1);
                }

                u32DatCnt += u32DatSize;

                if (u32DatCnt < u32FilSize)
                {
                    procUSB_TransStep = PROC_STEP_6;
                }
                else
                {
                    SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

                    f_close(&USBHFile);

                    RTOSprintf("\r\n> ### Download file %s from SD to USB complete ###", cUSB_TransFile);

                    RTOSprintf("\r\n> ### Total Used: %d ms ###", (HAL_GetTick() - u32GetTick));

                    // bUSB_TransFinish = TRUE;

                    LCD_SetShowAutoTestPage(38);

                    Buzzer_Beep(BEEP_SHORT, 3);

                    procUSB_TransStep = PROC_STEP_NULL;

                    bSysLogEn = TRUE;

                    if (bRTOS_En)
                        osMutexRelease(mutex_sd_proc_user);

                    if (bRTOS_En)
                        osMutexRelease(mutex_data_buf2_user);

                    HAL_Delay(1);

                    return;
                }
            }
            else
            {
                RTOSprintf("\r\n> SD-USB read file %s at address %d ERROR.", cUSB_TransFile, u32DatCnt);

                u8USB_FileTransRetry++;

                if (u8USB_FileTransRetry == USB_FILE_TRANSFER_MAX_RETRY)
                {
                    u8USB_FileTransRetry = 0;

                    RTOSprintf("\r\n> Maximum SD-USB file transfer retries reached. Terminate operation.");

                    u32DlAddr = u32DatCnt;

                    procUSB_TransStep = PROC_STEP_7;

                    bSysLogEn = TRUE;
                }
                else
                    procUSB_TransStep = PROC_STEP_6;

                if (bRTOS_En)
                    osMutexRelease(mutex_sd_proc_user);

                if (bRTOS_En)
                    osMutexRelease(mutex_data_buf2_user);

                HAL_Delay(1);

                return;
            }
        }

        u8USB_FileTransRetry = 0;

        SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

        if (bRTOS_En)
            osMutexRelease(mutex_sd_proc_user);

        if (bRTOS_En)
            osMutexRelease(mutex_data_buf2_user);

        HAL_Delay(1);

        break;
    }

    case PROC_STEP_7:
    {
        char cEventMsg[20] = {0};

        SD_RequestDo(SD_CLOSE, 1, &filSD_Handle2);

        RTOSprintf("\r\n> Transferring file %s from SD to USB failed.", cUSB_TransFile);

        LCD_SetShowAutoTestPage(43);

        Buzzer_Beep(BEEP_LONG, 1);

        sprintf(cEventMsg, "S:%d-E:%d-A:%d", procUSB_LastTransStep, u8USB_ErrCode, u32DlAddr);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_USB_DOWNLOAD_FAIL, cEventMsg);
#endif

        procUSB_TransStep = PROC_STEP_NULL;

        break;
    }

    default:
        break;
    }
}

//============================================================================//
// @brief  Handle config.sys file content for USB setup
// @param  None.
// @retval None.
//============================================================================//
bool USB_CheckConfigFile(char *CfgPkg)
{
    char *pStr;
    uint8_t u8DatalogCnt;
    uint16_t u16TotalLens, u16Lens, u16Cnt;
    // char cCmd[100];

    u16Lens = 0;

    u16TotalLens = strlen(CfgPkg);

    // RTOSprintf("\r\n> ### u16TotalLens: %d ###", u16TotalLens);

    do
    {
        StopChar(CfgPkg, '\n');
        StopChar(CfgPkg, '\r');

        pStr = CfgPkg;

        u16Cnt = strlen(CfgPkg) + 2;

        // RTOSprintf("\r\n> ### u16Cnt: %d, u16Lens: %d ###", u16Cnt, u16Lens);
        // RTOSprintf("\r\n> ### Data: %s ###", CfgPkg);

        if (strncmp(pStr, STR_CONFIG_AUTH, strlen(STR_CONFIG_AUTH)) == 0)
        {
            StopChar(pStr, '=');

            pStr += strlen(pStr) + 1;

            if (strncmp(pStr, STR_AUTH_ROOT, strlen(STR_AUTH_ROOT)) == 0)
            {
                gAppCfg.AUTH = APP_AUTH_ROOT;

                u8DatalogCnt = GET_DATALOG_ROOT_DEF;
            }

            if (strncmp(pStr, STR_AUTH_FACTORY, strlen(STR_AUTH_FACTORY)) == 0)
            {
                gAppCfg.AUTH = APP_AUTH_FACTORY;

                u8DatalogCnt = GET_DATALOG_ROOT_DEF;
            }

            if (strncmp(pStr, STR_AUTH_ADMIN, strlen(STR_AUTH_ADMIN)) == 0)
            {
                gAppCfg.AUTH = APP_AUTH_ADMIN;

                u8DatalogCnt = GET_DATALOG_ADMIN_DEF;
            }

            if (strncmp(pStr, STR_AUTH_USER, strlen(STR_AUTH_USER)) == 0)
            {
                gAppCfg.AUTH = APP_AUTH_USER;

                u8DatalogCnt = GET_DATALOG_USER_DEF;
            }
        }

        if (strncmp(CfgPkg, STR_CONFIG_GET_LOG, strlen(STR_CONFIG_GET_LOG)) == 0)
        {
            StopChar(pStr, '=');

            pStr += strlen(pStr) + 1;

            u8DatalogCnt = atoi(pStr);

            if ((gAppCfg.AUTH == APP_AUTH_ROOT) && (u8DatalogCnt > GET_DATALOG_ROOT_MAX))
                u8DatalogCnt = GET_DATALOG_ROOT_MAX;

            if ((gAppCfg.AUTH == APP_AUTH_FACTORY) && (u8DatalogCnt > GET_DATALOG_ROOT_MAX))
                u8DatalogCnt = GET_DATALOG_ROOT_MAX;

            if ((gAppCfg.AUTH == APP_AUTH_ADMIN) && (u8DatalogCnt > GET_DATALOG_ADMIN_MAX))
                u8DatalogCnt = GET_DATALOG_ADMIN_MAX;

            if ((gAppCfg.AUTH == APP_AUTH_USER) && (u8DatalogCnt > GET_DATALOG_USER_MAX))
                u8DatalogCnt = GET_DATALOG_USER_MAX;

            gAppCfg.LOG_CNT = u8DatalogCnt;
        }

        if (strncmp(CfgPkg, STR_CONFIG_DRIVER, strlen(STR_CONFIG_DRIVER)) == 0)
        {
            /*
            StopChar(pStr, '=');

            pStr += strlen(pStr) + 1;

            sprintf(cCmd, "%s%s\r\n", STR_AS_DRIVER_IN, pStr);

            RTOSprintf("\r\n> %s ", cCmd);

            Protocl_ChkCmdPkg(cCmd, NULL);
            */
        }

        if (strncmp(CfgPkg, STE_CONFIG_ENG_MODE, strlen(STE_CONFIG_ENG_MODE)) == 0)
        {
            StopChar(pStr, '=');

            pStr += strlen(pStr) + 1;

            if (strncmp(pStr, STR_MSG_ON, strlen(STR_MSG_ON)) == 0)
            {
                bIsEngModeFlag = TRUE;
            }
        }

        u16Lens += u16Cnt;
        CfgPkg += u16Cnt;
    } while (u16TotalLens > u16Lens);

    if (gAppCfg.AUTH == 0)
        RTOSprintf("\r\n> ### Set ID: ROOT, Get Log: %d ###", gAppCfg.LOG_CNT);
    else if (gAppCfg.AUTH == 1)
        RTOSprintf("\r\n> ### Set ID: FACTORY, Get Log: %d ###", gAppCfg.LOG_CNT);
    else if (gAppCfg.AUTH == 2)
        RTOSprintf("\r\n> ### Set ID: ADMIN, Get Log: %d ###", gAppCfg.LOG_CNT);
    else if (gAppCfg.AUTH == 3)
        RTOSprintf("\r\n> ### Set ID: USER, Get Log: %d ###", gAppCfg.LOG_CNT);

    if (bIsEngModeFlag == TRUE)
    {
        RTOSprintf("\r\n> ### Open Eng-Mode ###");
    }
    else
    {
        RTOSprintf("\r\n> ### Not Eng-Mode ###");
    }

    return TRUE;
}

//============================================================================//
// @brief  USB config.sys process
// @param  None.
// @retval None.
//============================================================================//
bool USB_ConfigFileProc(void)
{
    bool bRes = FALSE;
    // char *pStr;
    uint32_t u32FileSize = 0;
    uint8_t *pu8BufPtr;

    u32FileSize = USB_GetFileSize(FILENAME_CONFIG_SYS);

    // RTOSprintf("\r\n> Size of file %s:%d", FILENAME_CONFIG_SYS, u32FileSize);

    if (u32FileSize == 0)
    {
        RTOSprintf("\r\n> File %s size is 0!", FILENAME_CONFIG_SYS);

        return FALSE;
    }

    pu8BufPtr = (uint8_t *)RTOSmalloc(sizeof(uint8_t) * u32FileSize + 100);

    if (pu8BufPtr == NULL)
    {
        RTOSprintf("\r\n> Not enough memory to check file %s.", FILENAME_CONFIG_SYS);

        return FALSE;
    }

    // memset(u8USB_Buf, 0x00, sizeof(u8USB_Buf));

    bRes = USB_ReadAddrToBuff(FILENAME_CONFIG_SYS, pu8BufPtr, u32FileSize, 0);

    if (bRes == TRUE)
    {
        // pStr = (char *)pu8BufPtr;

        USB_CheckConfigFile((char *)pu8BufPtr);

        RTOSprintf("\r\n> File %s Read OK!", FILENAME_CONFIG_SYS);
    }
    else
    {
        RTOSprintf("\r\n> Read file %s ERROR!", FILENAME_CONFIG_SYS);
    }

    RTOSfree(pu8BufPtr);

    return bRes;
}

//============================================================================//
// @brief  USB Setup.txt process
// @param  None.
// @retval None.
//============================================================================//
bool USB_SetupFileProc(void)
{
    bool bRes = FALSE;
    char cCmd[100];
    uint8_t u8Data;
    uint16_t u16Cnt, u16Cnt2;
    uint32_t u32FileSize = 0;
    uint8_t *pu8BufPtr;
    bool bDoCmdFlag;

    u32FileSize = USB_GetFileSize(FILENAME_SETUP_TXT);

    // RTOSprintf("\r\n> Size of file %s:%d", FILENAME_SETUP_TXT, u32FileSize);

    if (u32FileSize == 0)
    {
        RTOSprintf("\r\n> File %s size is 0!", FILENAME_SETUP_TXT);

        return FALSE;
    }

    pu8BufPtr = (uint8_t *)RTOSmalloc(sizeof(uint8_t) * u32FileSize + 100);

    if (pu8BufPtr == NULL)
    {
        RTOSprintf("\r\n> Not enough memory to check file %s.", FILENAME_SETUP_TXT);

        return FALSE;
    }

    bRes = USB_ReadAddrToBuff(FILENAME_SETUP_TXT, pu8BufPtr, u32FileSize, 0);

    if (bRes == TRUE)
    {
        u16Cnt = 0;
        u16Cnt2 = 0;

        do
        {
            memset(cCmd, 0x00, sizeof(cCmd));

            do
            {
                u8Data = pu8BufPtr[u16Cnt];

                cCmd[u16Cnt2] = u8Data;

                u16Cnt++;

                u16Cnt2++;
            } while ((u8Data != '\n') && (u32FileSize > u16Cnt));

            bDoCmdFlag = TRUE;

            if (gAppCfg.AUTH == APP_AUTH_USER)
            {
                if (strncmp(cCmd, STR_AS_DRIVER_IN, strlen(STR_AS_DRIVER_IN)) != 0)
                {
                    RTOSprintf("\r\n> USER can only set DRIVER IN!");

                    u16Cnt2 = 0;

                    bDoCmdFlag = FALSE;
                }
            }

            if (bDoCmdFlag == TRUE)
            {
                RTOSprintf("\r\n> %s ", cCmd);

                bUSB_CheckSetupFlag = TRUE;

                bRes = Protocl_ChkCmdPkg(cCmd, NULL);

                bUSB_CheckSetupFlag = FALSE;

                if ((bRes == FALSE) && (bUSB_RunOne == FALSE))
                {
                    return FALSE;
                }

                u16Cnt2 = 0;
            }
        } while (u32FileSize > u16Cnt);

        RTOSprintf("\r\n> File %s Read OK!", FILENAME_SETUP_TXT);
    }
    else
    {
        RTOSprintf("\r\n> Read file %s ERROR!", FILENAME_SETUP_TXT);
    }

    RTOSfree(pu8BufPtr);

    return bRes;
}

//============================================================================//
// @brief  Increase the index of pending USB download request number by 1 then
//         write the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void USB_IncRequestIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);

    u16USB_DownloadNum++;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);

        if (bRes)
            break;
    }

    if (!bRes)
    {
        LOGprintf("\r\n> USB increase request index FAIL!");
    }
}

//============================================================================//
// @brief  Decrease the index of the pending USB download request number by 1
//         then write the index to the request list.
// @param  None.
// @retval None.
//============================================================================//
void USB_DecRequestIdx(void)
{
    bool bRes;
    char cFileName[64] = {0};

    if (u16USB_DownloadNum == 0)
        return;

    sprintf(cFileName, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);

    u16USB_DownloadNum--;

    for (int i = 0; i < 3; i++)
    {
        bRes = SD_WriteBuffToAddr(cFileName, (uint8_t *)&u16USB_DownloadNum, 2, 0);

        if (bRes)
            break;
    }

    if (!bRes)
    {
        LOGprintf("\r\n> USB decrease request index FAIL!");
    }
}

//============================================================================//
// @brief  Add a USB download request to the pending list.
// @param  pcSrcFile: Pointer to the source file name string.
//         pcDstFile: Pointer to the destination file name string.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool USB_AddDownloadRequest(char *pcSrcFile, char *pcDstFile, va_list arg)
{
    bool bRes;
    osStatus osRes;
    char cFilePath[128];
    USB_DownloadTypeDef usbDownloadRequest;
    USB_DL_TypeDef dlType;

    if ((strlen(pcSrcFile) == 0) ||
        (strlen(pcSrcFile) > sizeof(usbDownloadRequest.cSrcFile)))
    {
        RTOSprintf("\r\n> USB download source file name length invalid: %d", strlen(pcSrcFile));

        return FALSE;
    }

    if ((strlen(pcDstFile) == 0) ||
        (strlen(pcDstFile) > sizeof(usbDownloadRequest.cDstFile)))
    {
        RTOSprintf("\r\n> USB download destination file name length invalid: %d", strlen(pcDstFile));

        return FALSE;
    }

    if (bRTOS_En)
    {
        // Acquire USB_SD mutex
        osRes = osMutexWait(mutex_usb_sd_user, S_2_mS(5));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> Can't acquire USB_SD mutex. osRes:%d", osRes);

            if (u8ShowMenuItem == 26)
                LCD_SetShowAutoTestPage(50);

            return FALSE;
        }
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", USB_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);
#endif

    memset(&usbDownloadRequest, 0x00, sizeof(usbDownloadRequest));

    sprintf(usbDownloadRequest.cSrcFile, "%s", pcSrcFile);

    sprintf(usbDownloadRequest.cDstFile, "%s", pcDstFile);

    usbDownloadRequest.u8ErrCnt = 0;

    dlType = (USB_DL_TypeDef)va_arg(arg, int);

    switch ((int)dlType)
    {
    case USB_DL_VDR_HOUR:
        usbDownloadRequest.usbDL_Type = USB_DL_VDR_HOUR;

        usbDownloadRequest.u32Arg[0] = va_arg(arg, uint32_t);

        break;

    case USB_DL_NONE:
    default:
        usbDownloadRequest.usbDL_Type = USB_DL_NONE;

        break;
    }

    bRes = SD_WriteBuffToAddr(cFilePath,
                              (uint8_t *)&usbDownloadRequest,
                              sizeof(USB_DownloadTypeDef),
                              2 + sizeof(USB_DownloadTypeDef) * (uint32_t)u16USB_DownloadNum);

    if (bRes)
    {
        LOGprintf("\r\n> Add USB download request %d OK.", u16USB_DownloadNum + 1);

        USB_IncRequestIdx();
    }
    else
    {
        RTOSprintf("\r\n> Add USB download request %d ERROR.", u16USB_DownloadNum + 1);

        if (u8ShowMenuItem == 26)
            LCD_SetShowAutoTestPage(50);
    }

    if (bRTOS_En)
        // Release USB_SD mutex
        osMutexRelease(mutex_usb_sd_user);

    return bRes;
}

//============================================================================//
// @brief  Get a USB download request and start the download process.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool USB_InitDownload(void)
{
    bool bRes;
    osStatus osRes;
    char cFilePath[128];

    if (u16USB_DownloadNum == 0)
        return FALSE;

    if (bIsUSBHoldFlag == TRUE)
        return FALSE;

    if (procUSB_TransStep != PROC_STEP_NULL)
        return FALSE;

    if (bRTOS_En)
    {
        // Acquire USB_SD mutex
        osRes = osMutexWait(mutex_usb_sd_user, S_2_mS(5));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> Can't acquire USB_SD mutex. osRes:%d", osRes);

            if (u8ShowMenuItem == 26)
                LCD_SetShowAutoTestPage(50);

            return FALSE;
        }
    }

#ifndef DIRNAME_TEMP
    sprintf(cFilePath, "%s", USB_DOWNLOAD_TEMP_FILE);
#else
    sprintf(cFilePath, "%s/%s", DIRNAME_TEMP, USB_DOWNLOAD_TEMP_FILE);
#endif

    memset(&usbDownload, 0x00, sizeof(USB_DownloadTypeDef));

    bRes = SD_ReadAddrToBuff(cFilePath,
                             (uint8_t *)&usbDownload,
                             sizeof(USB_DownloadTypeDef),
                             2 + sizeof(USB_DownloadTypeDef) * ((uint32_t)u16USB_DownloadNum - 1));

    if (bRes)
    {
        LOGprintf("\r\n> Get USB download request %d OK.", u16USB_DownloadNum);

        USB_DecRequestIdx();
    }
    else
    {
        RTOSprintf("\r\n> Get USB download request %d ERROR.", u16USB_DownloadNum);

        if (u8ShowMenuItem == 26)
            LCD_SetShowAutoTestPage(50);
    }

    if (bRTOS_En)
        // Release USB_SD mutex
        osMutexRelease(mutex_usb_sd_user);

    if (bRes)
    {
        u8USB_FileTransRetry = 0;

        procUSB_TransStep = PROC_STEP_1;
    }

    return bRes;
}

//============================================================================//
// @brief  Download a file from SD card to USB.
// @param  pcSrcFile: Pointer to the source file name string.
//         pcDstFile: Pointer to the destination file name string.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool USB_DownloadFile(char *pcSrcFile, char *pcDstFile, ...)
{
    bool bRes;
    va_list arg;

    va_start(arg, pcDstFile);

    bRes = USB_AddDownloadRequest(pcSrcFile, pcDstFile, arg);

    va_end(arg);

    return bRes;
}

//============================================================================//
// @brief  Check and do the USB automatic download process.
// @param  None.
// @retval None.
//============================================================================//
void USB_CheckAutoDownload(void)
{
    uint32_t u32CurrTime = 0, u32PreTime = 0;
    uint8_t u8BCDTime[7];
    char cFileName[128];

    // if((bSD_ReportErrFlag == TRUE) || (gAppCfg.LOG_CNT == 0) || (gAppCfg.LOG_CNT > 31))
    if ((bSD_ReportErrFlag == TRUE) || (gAppCfg.LOG_CNT == 0) || (gAppCfg.LOG_CNT > (DATALOG_FILE_MAX + 1)))
        return;

    u32CurrTime = g_SysInfo.ttNow;

    for (int i = (gAppCfg.LOG_CNT - 1); i > 0; i--)
    {
        u32PreTime = u32CurrTime + MINUTE_2_SECOND(1) - DAY_2_SECOND(i);

        SecToBCDTime(u32PreTime, u8BCDTime);

        sprintf(cFileName, "%s/D%02X%02X%02X%02X_235959_%s.VDR",
                FOLDERNAME_VDR,
                u8BCDTime[0], u8BCDTime[1], u8BCDTime[2], u8BCDTime[3], g_Setting.tBusInfo.cBus_ID);

        if (SD_CheckFileExist(cFileName))
        {
            USB_DownloadFile(cFileName, cFileName);
        }
    }

    bUSB_PrepData = TRUE;

    bDoVDR_HourFlag = TRUE;

    bUSB_PrepData = FALSE;
}

//============================================================================//
// @brief  Process the USB task.
// @param  None.
// @retval None.
//============================================================================//
void USB_Proc(void)
{
    bool bRes;

    if (u8USB_ReportEvent == 2) // Report event USB disconnected
    {
        // RTOSprintf("\r\n> USB device disconnected.");

        if ((u8ShowMenuItem == 26) || (u8ShowMenuItem == 38) ||
            (u8ShowMenuItem == 43) || (u8ShowMenuItem == 64) ||
            (u8ShowMenuItem == 65) || (u8ShowMenuItem == 66) ||
            (u8ShowMenuItem == 67) || (u8ShowMenuItem == 68) ||
            (u8ShowMenuItem == 69) || (u8ShowMenuItem == 70) ||
            (u8ShowMenuItem == 75))
        {
            LCD_SetShowAutoTestPage(50);
        }

        if (procUSB_TransStep != PROC_STEP_NULL)
        {
            procUSB_TransStep = PROC_STEP_NULL;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_USB_DOWNLOAD_DISCONN, STR_NULL);
#endif
        }

#if ENABLE_GSM_NET
        if (bUSB_UndetDisconnect)
        {
            bUSB_UndetDisconnect = FALSE;

            GSM_TCP_ReportEvent((char *)EVENT_USB_DISCON, (char *)"1");
        }
        else
        {
            GSM_TCP_ReportEvent((char *)EVENT_USB_DISCON, STR_NULL);
        }
#endif

        if (bIsUSBHoldFlag == TRUE)
        {
            bIsUSBHoldFlag = FALSE;

#if ENABLE_GSM_NET
            GSM_TCP_ReportEvent((char *)EVENT_USB_HOLD_OFF, STR_NULL);
#endif
        }

        u8USB_ReportEvent = 0;
    }

    if (!bUSB_Ready)
        return;

    if (u8USB_ReportEvent == 1) // Report event USB device ready
    {
        // RTOSprintf("\r\n> USB device ready.");

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_USB_CON, STR_NULL);
#endif

        u8USB_ReportEvent = 0;
    }

    if (procUSB_TransStep == PROC_STEP_NULL)
    {
        if (bUSB_RunOne == TRUE)
        {
            if ((bSystemTimeReady == TRUE) && (bMX25L_ReportErrFlag == FALSE))
            {
                char cFileName[30];

                sprintf(cFileName, "%s_%02d_%02d_%02d.%s", DEVICE_TYPE, MODEL_NAME,
                        CUSTOMER_TYPE, HW_VER, UPGRADE_FILE_EXT);

                if (USB_CheckFileExist(cFileName))
                {
                    RTOSprintf("\r\n>>> Found upgrade file in USB. Reset to upgrade system.");

                    if (bSD_ReportErrFlag == FALSE)
                        V161_RecordTechnical(2);

#if ENABLE_GSM_NET
                    QtEC21_CheckRedoFTP();
#endif

                    g_Setting.tUpgrade.bUpgradeFlag = TRUE;

                    g_Setting.tUpgrade.ttLastInstall = System_GetTime();

                    // Set the program to check for upgrade file after reset
                    HW_WriteBackupRegister(BKP_DFU_CHK, BKP_RST_VALUE);

                    System_SaveSettingFinal();
                    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

                    // Reset to run the bootloader
                    if (bRTOS_En)
                    {
                        osMutexWait(mutex_setting_user, S_2_mS(5));
                    }

                    System_Reset();
                }

                if (USB_CheckFileExist((char *)CHINESE_FONT_FILE))
                {
                    // #if ENABLE_DEBUG_MSG
                    RTOSprintf("\r\n>>> Character font file exists in USB. Update character font file.");
                    // #endif
                    LCD_SetShowAutoTestPage(90);

                    for (int i = 0; i < 3; i++)
                    {
                        bRes = SaveChineseFont();

                        if (bRes)
                            break;
                    }

                    LCD_SetShowAutoTestPage(91);
                }
            }

            if (USB_CheckFileExist((char *)FILENAME_CONFIG_SYS))
            {
                bRes = USB_ConfigFileProc();

                if (bRes == FALSE)
                {
                    RTOSprintf("\r\n> ### USB Read Config File FAIL ###");
                }
            }
            else
            {
                RTOSprintf("\r\n> ### USB No Config File, Stop ###");

                gAppCfg.LOG_CNT = 0;

                bUSB_RunOne = FALSE;

                return;
            }

            if (USB_CheckFileExist((char *)FILENAME_SETUP_TXT))
            {
                bRes = USB_SetupFileProc();

                if (bRes == FALSE)
                {
                    RTOSprintf("\r\n> ### USB Read Setup File FAIL ###");

                    if (bUSB_RunOne == FALSE)
                    {
                        RTOSprintf("\r\n> ### USB Check Stop ###");

                        return;
                    }
                }
            }

            LOGprintf("\r\n> USB number of file requested to automatically download:%d", gAppCfg.LOG_CNT);

            if ((bIsEngModeFlag == TRUE) || (g_Setting.bVdrNow == TRUE))
            {
                gAppCfg.LOG_CNT = 0;
            }

            if (bSystemTimeReady == TRUE)
            {
                USB_CheckAutoDownload();
            }

            bUSB_RunOne = FALSE;
        }
        else
        {
            if ((bSystemTimeReady == TRUE) && (bSD_ReportErrFlag == FALSE) &&
                (u16USB_DownloadNum > 0) && (procUSB_TransStep == PROC_STEP_NULL))
            {
                USB_InitDownload();
            }
        }
    }
    else
    {
        if ((bSystemTimeReady == TRUE) && (procUSB_TransStep != PROC_STEP_NULL))
        {
            USB_Download_Proc();
        }
    }
}
