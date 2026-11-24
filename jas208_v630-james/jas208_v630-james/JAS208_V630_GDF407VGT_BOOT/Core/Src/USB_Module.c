//============================================================================//
// User Includes
//============================================================================//
#include "USB_Module.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "Default.h"
#include "diskio.h"
#include "fatfs.h"
#include "LCD_Key.h"
#include "MX25L_Driver.h"
#include "SD_Module.h"
#include "System.h"


//============================================================================//
// User Variables
//============================================================================//

bool bUSB_Mounted = FALSE;

DIR dirUSB_Handle;

char cUSB_FilePath[128];

uint8_t u8USB_Buf[32768];
//uint8_t u8USB_Buf[8192];

//uint8_t u8USB_Buf[8192]; //__attribute__ ((section("ccm_var"))); // 8 kBytes

ProcStepTypeDef procUSB_TransStep = PROC_STEP_NULL;

uint8_t u8USB_FileTransRetry = 0;

//bool bUSB_TransFinish = FALSE;

bool bUSB_PrepData = FALSE;

uint16_t u16USB_DownloadNum = 0;

DIR dirHandle;

FIL filHandle;

bool bUSB_RunOne = FALSE;

APP_CONFIG_t gAppCfg;

uint8_t u8USB_ReportEvent = 0;

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

	frRes = f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);

	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Mount to USB disk fail with error code %d.", frRes);
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


	frRes = f_mount(NULL, (TCHAR const*)USBHPath, 1);

	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Unmount USB disk fail with error code %d.", frRes);
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
	
	
	if(bUSB_Ready)
	{
		if(!bUSB_Mounted)
		{
			if(USB_Mount())
				bRes = TRUE;
			else
				bRes = FALSE;
		}
		else
			bRes = TRUE;
	}
	else
		bRes = FALSE;
	
	return bRes;
}

//============================================================================//
// @brief  Initialize USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is initialized successfully.
//                 FALSE: USB drive initialization is failed.
//============================================================================//
bool USB_FS_Init(void)
{
	if(!bUSB_Ready)
		return FALSE;

	if(!bUSB_Mounted)
	{
		if(USB_Mount())
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
bool USB_FS_DeInit(void)
{
	if(!bUSB_Ready)
		return FALSE;

	if(bUSB_Mounted)
	{
		if(USB_UnMount())
			return TRUE;
		else
			return FALSE;
	}
	else
		return TRUE;
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


	if(strlen(pcFileName) > sizeof(Buf))
	{
		#if ENABLE_DEBUG_MSG
	  printf("\r\n> USB file name is too long!");
		#endif
		
		return FALSE;
	}
	
	pcStr = (char *)Buf;

	strcpy(pcStr, pcFileName);

	// Get file name only
	StopChar(pcStr, '.');

	if(strncmp(pcStr, pcFilter, strlen(pcFilter)) != 0)
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


	if(strlen(pcFileName) > sizeof(Buf))
	{
		#if ENABLE_DEBUG_MSG
	  printf("\r\n> USB file extension is too long!");
		#endif
		
		return FALSE;
	}
	
	pcStr = (char *)Buf;

	strcpy(pcStr, pcFileName);

	// Get file extension only
	StopChar(pcStr, '.');

	pcStr += strlen(pcStr) + 1;

	if(strncmp(pcStr, pcFilter, strlen(pcFilter)) != 0)
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
	FRESULT frRes;
	//DIR dirUSB_Handle;
	//char cUSB_FilePath[128];


	// Check USB mounting status
	if(!USB_Check())
		return FALSE;
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);
	
	//frRes = f_opendir(&dirUSB_Handle, cUSB_FilePath);

	// Open directory
	//if(frRes != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf("\r\n> Check file exist open dir ERROR %d", frRes);
	//	#endif
		
	//	return FALSE;
	//}

	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);
	
	frRes = f_open(&USBHFile, cUSB_FilePath, FA_READ);

	// Try to open file
	if(frRes == FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		//printf("\r\n> USB file %s exists.", pcFileName);
		#endif
		
		f_close(&USBHFile);
		
		//if(bRTOS_En)
	  //  osMutexRelease(mutex_usb_user);

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		if(frRes != 4)
		  printf("\r\n> USB trying open file %s ERROR %d.", pcFileName, frRes);
		#endif
		
		//if(bRTOS_En)
	  //  osMutexRelease(mutex_usb_user);
		
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
	//DIR dirUSB_Handle;
	FILINFO finfoHandle;
	//char cUSB_FilePath[128],
	char cName[128], cExten[5];
	char *pcStr;


	if(strlen(pcFindName) > sizeof(cUSB_FilePath))
	{
		#if ENABLE_DEBUG_MSG
	  printf("\r\n> USB file name to find is too long!");
		#endif
		
		return FALSE;
	}
	
	strcpy(cUSB_FilePath, pcFindName);

	pcStr = cUSB_FilePath;

	// Split name and extension
	StopChar(pcStr, '.');

	strcpy(cName, pcStr);

	pcStr += strlen(pcStr) + 1;

	if(*pcStr) // If there is extension
		strcpy(cExten, pcStr);
	else // If there is no extension
		sprintf(cExten, "*");

	// Check USB drive status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return FALSE;
	}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	//if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf(MSG_USB_OPEN_DIR_ERROR);
	//	#endif

	//	return FALSE;
	//}

	// If use long file name format
	//#if _USE_LFN
	//char cLFN[_MAX_LFN + 1];
	//finfoHandle.lfname = cLFN;
	//finfoHandle.lfsize = sizeof(cLFN);
	//#endif
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	// Check files through the USB disk
	while(f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
	{
	    // If encounter no name files then stop
		if(finfoHandle.fname[0] == 0)
			break;
	
		// If file attribute is AM_ARC then continue
		if((finfoHandle.fattrib & AM_ARC) == 0)
			continue;		

		//#if _USE_LFN
		//pcFileName = *finfoHandle.lfname ? finfoHandle.lfname : finfoHandle.fname;
		//#else
		//pcFileName = finfoHandle.fname;
		//#endif

		// If find a random file
		if((strcmp(cName, "*") == 0) && (strcmp(cExten, "*") == 0))
		{
			sprintf(pcReturnName, "%s", finfoHandle.fname);

			bRes = TRUE;

			break;
		}
		else if(strcmp(cExten, "*") == 0) // If not care about file extension
		{
			if(USB_CheckFileByName(finfoHandle.fname, cName) == TRUE)
			{
				sprintf(pcReturnName, "%s", finfoHandle.fname);

				bRes = TRUE;

				break;
			}
		}
		else if(strcmp(cName, "*") == 0) // If not care about file name
		{
			if(USB_CheckFileByExten(finfoHandle.fname, cExten) == TRUE)
			{
				sprintf(pcReturnName, "%s", finfoHandle.fname);

				bRes = TRUE;

				break;
			}
		}
		else // If both name and extension have to be matched
		{
			if((USB_CheckFileByName(finfoHandle.fname, cName) == TRUE) &&
			        (USB_CheckFileByExten(finfoHandle.fname, cExten) == TRUE))
			{
				sprintf(pcReturnName, "%s", finfoHandle.fname);

				bRes = TRUE;

				break;
			}
		}
	}
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	return bRes;
}

//============================================================================//
// @brief  Get size of a file.
// @param  pcFileName: Pointer to file name for getting size.
// @retval File size in the uint32_t format.
//============================================================================//
uint32_t USB_GetFileSize(char *pcFileName)
{
	//DIR dirUSB_Handle;
	//char cUSB_FilePath[128];
	uint32_t u32FileSize;


	// Check USB drive status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return 0;
	}
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	//if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf(MSG_USB_OPEN_DIR_ERROR);
	//	#endif

	//	return 0;
	//}

	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

	// Try to open file
	if(f_open(&USBHFile, cUSB_FilePath, FA_READ) != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n>  SD disk file name %s invalid(get file)).", pcFileName);
		#endif
		
		//if(bRTOS_En)
	  //  osMutexRelease(mutex_usb_user);

		return 0;
	}

	// Get file size
	u32FileSize = f_size(&USBHFile);

	f_close(&USBHFile);
	
	//f_closedir(&dirUSB_Handle);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

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
	//DIR dirUSB_Handle;
	FILINFO finfoHandle;
	struct tm tmTimeInfo;


	// Check SD card status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return FALSE;
	}

	sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_OPEN_DIR_ERROR);
		#endif

		return FALSE;
	}

	// If use long name format
	//#if _USE_LFN
	//char cLFN[_MAX_LFN + 1];
	//finfoHandle.lfname = cLFN;
	//finfoHandle.lfsize = sizeof(cLFN);
	//#endif
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}
	
	uint16_t u16Cnt = 1;

	printf("\r\n> USB file list:");

	// Check files through the SD card
	while(f_readdir(&dirUSB_Handle, &finfoHandle) == FR_OK)
	{
		/*
	    // If encounter no name files then stop
		if(finfoHandle.fname[0] == 0)
			break;
			
		// If file attribute is AM_ARC then continue
		if((finfoHandle.fattrib & AM_ARC) == 0)
			continue;

		// Check file name
		//#if _USE_LFN
		//pcFileName = *finfoHandle.lfname ? finfoHandle.lfname : finfoHandle.fname;
		//#else
		//pcFileName = finfoHandle.fname;
		//#endif

		printf("\r\n> %s", finfoHandle.fname);
		*/
		//RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);
		//if((finfoHandle.fattrib & AM_DIR) == 0)
		//  continue;	
		
		// If all files are already listed then stop
		if(finfoHandle.fname[0] == 0)
			break;
		
		// If file attribute is AM_ARC then continue
		//if((((finfoHandle.fattrib & AM_ARC) == 0) || (finfoHandle.fattrib & AM_DIR) == 0))
		if((finfoHandle.fattrib & AM_ARC) == 0)
		{
			if(strncmp(finfoHandle.fname, (char *)"SYSTEM~1", 8) != 0)
			  printf("\r\n> #%d #ARCHIEVE NAME:\"%s\"", u16Cnt++, finfoHandle.fname);
			
			continue;
		}
		
		//RTOSprintf("\r\n> File attribute:%s, file name:%s", finfoHandle.fattrib, finfoHandle.fname);

		// Check file name
    //#if _USE_LFN
		//pcFileName = *finfoHandle.lfname ? finfoHandle.lfname : finfoHandle.fname;
    //#else
		//pcFileName = finfoHandle.fname;
    //#endif

		tmTimeInfo.tm_year = ((finfoHandle.fdate >> 9) & 0x7F) + 1980;
		tmTimeInfo.tm_mon	= ((finfoHandle.fdate >> 5) & 0x0F);
		tmTimeInfo.tm_mday = finfoHandle.fdate & 0x1F;
		tmTimeInfo.tm_hour = (finfoHandle.ftime >> 11) & 0x1F;
		tmTimeInfo.tm_min = (finfoHandle.ftime >> 5) & 0x3F;
		tmTimeInfo.tm_sec = (finfoHandle.ftime & 0x1F) * 2;

		if(tmTimeInfo.tm_year > 2100)
			continue;

		printf("\r\n> #%d #FILE NAME:\"%s\",     #SIZE: %d,     #LAST MODIFIED DATE: %04d/%02d/%02d - %02d:%02d:%02d.",
		           u16Cnt++,
		           finfoHandle.fname, (uint32_t)finfoHandle.fsize,
		           tmTimeInfo.tm_year, tmTimeInfo.tm_mon, tmTimeInfo.tm_mday,
		           tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);
	}
	
	//f_closedir(&dirUSB_Handle);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	return TRUE;
}

/*
//============================================================================//
// @brief  Format USB drive.
// @param  None.
// @retval Result: TRUE: USB drive is formatted successsfully.
//                 FALSE: USB drive is not formatted successfully.
//============================================================================//
bool USB_Format(void)
{
	FRESULT frRes;
	uint8_t u8FormatCnt = 0;


	if(!USB_Check())
		return FALSE;
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	do
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> Format %d times.", (u8FormatCnt + 1));
    #endif

		frRes = f_mkfs((TCHAR const*)USBHPath, 0, 0);

		u8FormatCnt++;

    #if ENABLE_DEBUG_MSG
		printf("\r\n> Format %d times result:%d.", u8FormatCnt, frRes);
    #endif

		HAL_Delay(500);

		if(u8FormatCnt >= 15)
		{
      #if ENABLE_DEBUG_MSG
			printf("\r\n> USB drive format failure.");
      #endif

			break;
		}
	}
	while(frRes != FR_OK);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	if(u8FormatCnt >= 15)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB drive format failure.");
		#endif

		return FALSE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB drive format success %d.", u8FormatCnt);
		#endif

		return TRUE;
	}
}
*/

//============================================================================//
// @brief  Remove a file in the USB.
// @param  pcFileNames: Pointer to the name of the file to be removed.
// @retval Result: TRUE: The file is removed successfully.
//                 FALSE: Removing the file is failed.
//============================================================================//
bool USB_RemoveFile(char *pcFilenames)
{
	FRESULT frRes;
	//char szPath[128];


	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}
	
	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFilenames);

	frRes = f_unlink(cUSB_FilePath);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	if(frRes == FR_OK)
	{
		//printf("\r\n### USB remove file %s OK ###", filenames);

		return TRUE;
	}
	else
	{
		//printf("\r\n### USB remove file %s FAIL ###", filenames);

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
	//char cOldPath[50];


	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}
	
	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcOldName);

	frRes = f_rename(cUSB_FilePath, pcNewName);
	
	//if(bRTOS_En)
	// osMutexRelease(mutex_usb_user);

	if(frRes == FR_OK)
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
	//DIR dirUSB_Handle;
	//char cUSB_FilePath[128];
	UINT uBw;
	//uint8_t u8DataBuf[128];
	//uint16_t u16BufSize, u16DataSize;


	// Check USB disk status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return FALSE;
	}
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	//if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf(MSG_USB_OPEN_DIR_ERROR);
	//	#endif

	//	return FALSE;
	//}

	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

	// Open file
	frRes = f_open(&USBHFile, cUSB_FilePath, FA_WRITE);
	
	if(frRes == FR_OK)
	{
		f_lseek(&USBHFile, f_size(&USBHFile));
	}
	else // If the specified file is not exist then create a new file
	{
		f_close(&USBHFile);

		f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
	}

	/*
	u16BufSize = u16DataSize = 0;


	// Write data from the buffer to the file
	do
	{
		u8USB_Buf[u16DataSize++] = pu8USB_Buf[u16BufSize++];

		if((u16DataSize < sizeof(u8USB_Buf)) && (u16BufSize < u16BufLen))
			continue;

		frRes = f_write(&USBHFile, &u8USB_Buf, u16DataSize, &uBw);

		if(frRes != FR_OK)
			break;

		u16DataSize = 0;
	}while(u16BufSize < u16BufLen);
	*/

	frRes = f_write(&USBHFile, pu8USB_Buf, u16BufLen, &uBw);

	f_close(&USBHFile);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	if(frRes == FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB write to file %s OK.", pcFileName);
		#endif

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB write to file %s ERROR %d.", pcFileName, frRes);
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
	//DIR dirUSB_Handle;
	//char cUSB_FilePath[128];
	//uint8_t DataBuf[128];
	//uint16_t BufSize, DataSize;
	UINT uBr;


	// Check USB disk status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return FALSE;
	}
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	//if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf(MSG_USB_OPEN_DIR_ERROR);
	//	#endif

	//	return FALSE;
	//}

	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);
	
	frRes = f_open(&USBHFile, cUSB_FilePath, FA_READ);

	// Open file for reading
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB open file %s ERROR %d.", pcFileName, frRes);
		#endif
		
		//if(bRTOS_En)
	  //  osMutexRelease(mutex_usb_user);

		return FALSE;
	}
	
	//f_sync(&USBHFile);

	// Put cursor the the start address
	f_lseek(&USBHFile, u32Addr);

	/*
	BufSize = DataSize = 0;


	// Read data from file to the buffer
	do
	{
		if(DataSize == 0)
		{
			frRes = f_read(&USBHFile, &u8USB_Buf, sizeof(u8USB_Buf), &uBr);

			if(frRes != FR_OK)
				break;
		}

		pu8USB_Buf[BufSize++] = u8USB_Buf[DataSize++];

		if((DataSize < sizeof(u8USB_Buf)) && (BufSize < u16BufLen))
		{
			continue;
		}

		DataSize = 0;

	}
	while(BufSize < u16BufLen);
	*/

	frRes = f_read(&USBHFile, pu8USB_Buf, u16BufLen, &uBr);
	
	//f_sync(&USBHFile);

	f_close(&USBHFile);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	if(frRes == FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB read file %s OK.", pcFileName);
		#endif

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB read file %s ERROR %d.", pcFileName, frRes);
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
	//DIR dirUSB_Handle;
	//char cUSB_FilePath[128];
	//uint8_t u8DataBuf[128];
	//uint16_t u16BufSize, u16DataSize;
	UINT uBw;


	// Check USB drive status
	if(!USB_Check())
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_USB_DRIVE_INVALID);
		#endif

		return FALSE;
	}
	
	//if(bRTOS_En)
	//{
	//	osStatus osRes;


	//	osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//	if(osRes != osOK)
	//		return FALSE;
	//}

	//sprintf(cUSB_FilePath, "%d:/", DRV_USB);

	// Open directory
	//if(f_opendir(&dirUSB_Handle, cUSB_FilePath) != FR_OK)
	//{
	//	#if ENABLE_DEBUG_MSG
	//	printf(MSG_USB_OPEN_DIR_ERROR);
	//	#endif

	//	return FALSE;
	//}

	sprintf(cUSB_FilePath, "%d:/%s", DRV_USB, pcFileName);

	// Open file
	if(f_open(&USBHFile, cUSB_FilePath, FA_WRITE) != FR_OK)
	{
		//f_close(&USBHFile);

		frRes = f_open(&USBHFile, cUSB_FilePath, FA_CREATE_ALWAYS | FA_WRITE);
		{
		  if(frRes != FR_OK)
			{
			  printf("\r\n> USB create file %s ERROR %d", pcFileName, frRes);
			}
		}
	}
	
	//f_sync(&USBHFile);

	// Put cursor to the start address
	f_lseek(&USBHFile, u32Addr);

	frRes = f_write(&USBHFile, pu8USB_Buf, u16BufLen, &uBw);
	
	//f_sync(&USBHFile);

	f_close(&USBHFile);
	
	//if(bRTOS_En)
	//  osMutexRelease(mutex_usb_user);

	if(frRes == FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB write file %s OK.", pcFileName);
		#endif

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> USB write file %s ERROR %d.", pcFileName, frRes);
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
	//FRESULT frRes;
	MX25L_StatusTypeDef mxlRes;
	//char cPath[128];
	uint32_t u32StartAddr = (uint32_t)EXFLASH_CHARACTERS_LIB_START_ADDR;
	uint32_t u32FileSize, u32DatCnt = 0, u32DatSize;
	uint16_t u16PageAddr = 0, u16SectEraseNum = 0;
	//UINT uBr;


	//frRes = f_mount(fatfsHandle, cDrvPath, 0);

	//if(frRes != FR_OK)
	//{
	//#if ENABLE_DEBUG_MSG
	//printf("\r\n> Mount to drive ERROR %d.", frRes);
	//#endif

	//return FALSE;
	//}

	//sprintf(cPath, "%d:/", u8Drv);

  //if(u8Drv == DRV_USB)
	//  frRes = f_opendir(&dirHandle, cPath);
	//else
	//	frRes = SD_RequestDo(SD_OPENDIR, 2, &dirHandle, cPath);

	//if(frRes != FR_OK)
	//{
		//#if ENABLE_DEBUG_MSG
		//printf("\r\n> Open drive directory ERROR %d", frRes);
		//#endif

		//return FALSE;
	//}
	
	//if(u8Drv == DRV_USB)
	//{
	//  if(bRTOS_En)
	//	{
	//		osStatus osRes;


	//		osRes = osMutexWait(mutex_usb_user, S_2_mS(10));

	//		if(osRes != osOK)
	//			return FALSE;
	//	}
	//}

	//sprintf(cPath, "%d:/%s", u8Drv, CHINESE_FONT_FILE);

	//if(u8Drv == DRV_USB)
	//  frRes = f_open(&filHandle, cPath, FA_READ);
	//else
	//	frRes = SD_RequestDo(SD_OPEN, 3, &filHandle, cPath, FA_READ);

	//if(frRes != FR_OK)
	//{
		//#if ENABLE_DEBUG_MSG
	//	printf("\r\n> Open character font file in drive ERROR %d", frRes);
		//#endif
		
	//	if(u8Drv == DRV_USB)
	//	{
	//	  if(bRTOS_En)
	//      osMutexRelease(mutex_usb_user);
	//	}	

	//	return FALSE;
	//}

	//u32FileSize = f_size(&filHandle);
	u32FileSize = USB_GetFileSize((char *)CHINESE_FONT_FILE);

	if(u32FileSize == 0)
	{
		//#if ENABLE_DEBUG_MSG
		printf("\r\n> Size of character font file %s is 0.", CHINESE_FONT_FILE);
		//#endif
		
		//if(u8Drv == DRV_USB)
		//{
		//  if(bRTOS_En)
	  //    osMutexRelease(mutex_usb_user);
		//}

		return TRUE;
	}

	//#if ENABLE_DEBUG_MSG
	printf("\r\n> SD size of character font file %s is %d.", CHINESE_FONT_FILE, u32FileSize);
	//#endif

	u16SectEraseNum = (uint16_t)(u32FileSize / MX25L_SECTOR_SIZE);

	if(u32FileSize % MX25L_SECTOR_SIZE)
		u16SectEraseNum++;

	mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);

	if(mxlRes != MX25L_OK)
	{
		//#if ENABLE_DEBUG_MSG
		printf("\r\n> Erase Exflash ERROR %d. Terminate download character font file.", mxlRes);
		//#endif
		
		//if(u8Drv == DRV_USB)
		//{
		//  if(bRTOS_En)
	  //    osMutexRelease(mutex_usb_user);
		//}

		return FALSE;
	}

	//#if ENABLE_DEBUG_MSG
	printf("\r\n> Start download character font file from SD to Exflash.");
	//#endif

	//MX25L_ChipErase();

	u32DatCnt = 0;

	do
	{
		u32DatSize = u32FileSize - u32DatCnt;

		if(u32DatSize > sizeof(u8USB_Buf))
			u32DatSize = sizeof(u8USB_Buf);

		//if(u8Drv == DRV_USB)
		//  frRes = f_lseek(&filHandle, u32DatCnt);
		//else
		//	frRes = SD_RequestDo(SD_LSEEK, 2, &filHandle, u32DatCnt);

		//if(frRes != FR_OK)
		//{
			//#if ENABLE_DEBUG_MSG
		//	printf("\r\n> Set file %s pointer ERROR %d", CHINESE_FONT_FILE, frRes);
			//#endif
			
		//	osMutexRelease(mutex_data_buf2_user);

		//	break;
		//}

		//if(u8Drv == DRV_USB)
		//  frRes = f_read(&filHandle, u8USB_Buf, u32DatSize, &uBr);
		//else
		//	frRes = SD_RequestDo(SD_READ, 4, &filHandle, u8USB_Buf, u32DatSize, &uBr);

    bRes = USB_ReadAddrToBuff((char *)CHINESE_FONT_FILE, u8USB_Buf, u32DatSize, u32DatCnt);

    //if(frRes != FR_OK)
		if(bRes == FALSE)
		{
			//#if ENABLE_DEBUG_MSG
			//printf("\r\n> Reading file %s at address %d ERROR %d.", CHINESE_FONT_FILE, u32DatCnt, frRes);
			printf("\r\n> Reading file %s at address %d ERROR.", CHINESE_FONT_FILE, u32DatCnt);
			//#endif

			break;
		}

		u16PageAddr = (uint16_t)((u32StartAddr + u32DatCnt) / MX25L_PAGE_SIZE);

		mxlRes = MX25L_MultiPagesProgram((uint32_t)u16PageAddr, u8USB_Buf, u32DatSize);

		if(mxlRes == MX25L_OK)
		{
			u32DatCnt += u32DatSize;

			if((u32DatCnt % (sizeof(u8USB_Buf) * 4)) == 0)
			{
				//#if ENABLE_DEBUG_MSG
				printf("\r\n> Finish transfer %d bytes.", u32DatCnt);
				//#endif
			}

			//for(int i = 0; i < u32DatSize; i++)
			//{
			//	u32CRC_Val1 = CRC32_Calc(u8Buf[i]);
			//}
		}
		else
		{
			//#if ENABLE_DEBUG_MSG
			printf("\r\n> Program to Exflash at page %d fail with ERROR %d.", u16PageAddr, mxlRes);
			//#endif

			break;
		}
		
		HAL_Delay(2);
	}
	while(u32DatCnt < u32FileSize);

	//if(u8Drv == DRV_USB)
	//  f_close(&filHandle);
	//else
	//	SD_RequestDo(SD_CLOSE, 1, &filHandle);

	if((bRes != TRUE) || (mxlRes != MX25L_OK))
	//if((frRes != FR_OK) || (mxlRes != MX25L_OK))
	{
		//if(u8Drv == DRV_USB)
		//  f_closedir(&dirHandle);
		//else
		//	SD_RequestDo(SD_CLOSEDIR, 1, &dirHandle);

		mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);

		if(mxlRes != MX25L_OK)
		{
			//#if ENABLE_DEBUG_MSG
			printf("\r\n> Erase Exflash ERROR %d.", mxlRes);
			//#endif
		}

		//#if ENABLE_DEBUG_MSG
		printf("\r\n> Download character font file FAIL.");
		//#endif
		
		//if(u8Drv == DRV_USB)
		//{
		//  if(bRTOS_En)
	  //    osMutexRelease(mutex_usb_user);
		//}

		return FALSE;
	}
	else
	{
		//if(u8Drv == DRV_USB)
		//{
		  //f_unlink(cPath);

		//  f_closedir(&dirHandle);
		//}
		//else
		//{
		//	if(!bRTOS_En)
		//	{
		//		f_unlink(cPath);

		//    f_closedir(&dirHandle);
		//	}
		//	else
		//	{
		//		SD_RequestDo(SD_UNLINK, 1, cPath);
				
		//		SD_RequestDo(SD_CLOSEDIR, 1, &dirHandle);
		//	}
		//}

		//#if ENABLE_DEBUG_MSG
		printf("\r\n> Download character font file OK.");
		//#endif
		
		//if(u8Drv == DRV_USB)
		//{
		//  if(bRTOS_En)
	  //    osMutexRelease(mutex_usb_user);
		//}

		return TRUE;
	}
}

/*
void USB_Demo(void)
{
	char cTestFile[30] = "GD32.txt";
	char cCopyFile[30] = "GD32_Copy.txt";
	uint32_t u32FileSize, u32DatCnt, u32DatSize;
	bool bRes;
	
	
  if(USB_CheckFileExist(cTestFile) == FALSE)
	{
	  printf("\r\n> USB test file does not exist!");
		
		USB_FileList();
		
		return;
	}
	
	u32FileSize = USB_GetFileSize(cTestFile);
	
	if(u32FileSize == 0)
	{
	  printf("\r\n> USB test file size is 0!");
		
		return;
	}
	
	if(USB_CheckFileExist(cCopyFile))
	{
	  USB_RemoveFile(cCopyFile);
		
		printf("\r\n> USB remove copy file successfully!");
	}
	
	u32DatCnt = 0;
	
	while(u32DatCnt < u32FileSize)
	{
	  u32DatSize = u32FileSize - u32DatCnt;
		
		if(u32DatSize > sizeof(u8USB_Buf))
		{
		  u32DatSize = sizeof(u8USB_Buf);
		}
		
		bRes = USB_ReadAddrToBuff(cTestFile, u8USB_Buf, u32DatSize, u32DatCnt);
		
		if(bRes == FALSE)
		{
		  printf("\r\n> USB read test file at address %d ERROR!", u32DatCnt);
			
			return;
		}
		
		bRes = USB_WriteBuffToAddr(cCopyFile, u8USB_Buf, u32DatSize, u32DatCnt);
		
		if(bRes == FALSE)
		{
		  printf("\r\n> USB write copy file at address %d ERROR!", u32DatCnt);
			
			return;
		}
	
		u32DatCnt += u32DatSize;
		
		if((u32DatCnt % (1024 * 32)) == 0)
		{
		  printf("\r\n> USB finish transfer %d bytes.", u32DatCnt);
		}
	}
	
	printf("\r\n> USB file copy finish.");
	
	USB_FileList();
	
	printf("\r\n> USB Demo complete.");
}
*/
