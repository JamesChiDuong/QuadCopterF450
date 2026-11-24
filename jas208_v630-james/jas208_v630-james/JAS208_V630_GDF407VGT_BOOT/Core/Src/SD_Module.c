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
#include "fatfs.h"
//#include "PCF_Driver.h"
#include "System.h"


//============================================================================//
// User Variables
//============================================================================//
bool bSD_ReInit = TRUE;

uint8_t u8SD_RetryCnt = 0, u8SD_ErrCnt = 0, u8SD_ResetCnt = 0;

char u8SD_Buf[2048];


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
	
	if(halRes != HAL_OK)
		return halRes;
	
	halRes = HAL_SD_GetCardCSD(&hsd, pCSD);
	
	if(halRes != HAL_OK)
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


	halRes = SD_GetCardInfo(&sdCardInfo, &sdCID, &sdCSD);
	
	if(halRes == HAL_OK)
	{
		//#if ENABLE_DEBUG_MSG
		//printf("\r\n> SD disk Product name: %c%c%c%c%c!",
		//			 (sdCID.ProdName1 >> 24) & 0xFF,
		//			 (sdCID.ProdName1 >> 16) & 0xFF,
		//			 (sdCID.ProdName1 >> 8) & 0xFF,
		//			 sdCID.ProdName1 & 0xFF,
		//			 sdCID.ProdName2);

		//printf("\r\n> SD disk ManufactDate: 20%02d.%d!",
		//			 (sdCID.ManufactDate & 0x0ff0) >> 4,
		//			 (sdCID.ManufactDate & 0x0f));

		//printf("\r\n> SD disk capacity : %d MB\r\n",
		//			 (uint32_t)((long long) sdCardInfo.BlockNbr * sdCardInfo.BlockSize / 1024768));
		//#endif
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD get information ERROR %d.", halRes);
		#endif
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


	if(bInit == TRUE)
	{
		dsStat = disk_initialize(DRV_USER);
	}
	else
	{
		if(bSD_ReInit == TRUE)
		{
			HAL_SD_CardStateTypeDef sdState;
			
			
			sdState = HAL_SD_GetCardState(&hsd);
			
			if((sdState == 0) || (sdState == HAL_SD_CARD_READY) || (sdState == HAL_SD_CARD_TRANSFER))
			{
				dsStat = STA_READY;
			}
			else
			{
			  #if ENABLE_DEBUG_MSG
				printf("\r\n> SD card state:%d", sdState);
				#endif
				
				dsStat = disk_initialize(DRV_USER);
			}
		}
		else
		{
			HAL_SD_CardStateTypeDef sdState;
			
			
			sdState = HAL_SD_GetCardState(&hsd);
			
			if((sdState == 0) || (sdState == HAL_SD_CARD_READY) || (sdState == HAL_SD_CARD_TRANSFER))
			{
				dsStat = STA_READY;
			}
			else
			{
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD card state:%d", sdState);
				#endif
				
				dsStat = STA_NOINIT;
			}
		}
  }
	
	//#if ENABLE_DEBUG_MSG
	//printf("\r\n> SD stat:%d", dsStat);
	//#endif

	switch(dsStat)
	{
		case STA_OK:
			{
				FRESULT frRes;


				SD_ShowInfo();

				frRes = f_mount(&USERFatFS, (TCHAR const *)USERPath, 1);

				if(frRes == FR_OK)
				{
					//#if ENABLE_DEBUG_MSG
					//printf("\r\n> SD mounting OK.");
					//#endif
					
					u8SD_RetryCnt = 0;
					
					u8SD_ResetCnt = 0;

					bRes = TRUE;
				}
				else
				{
					#if ENABLE_DEBUG_MSG
					printf("\r\n> SD mounting ERROR %d.", frRes);
					#endif
					
					bRes = FALSE;
				}
			}
			
			break;

		case STA_READY:
			u8SD_RetryCnt = 0;
		
		  u8SD_ResetCnt = 0;  

			bRes = TRUE;
		
			break;

		case STA_NOINIT:
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD get NOINIT ERROR.");
			#endif

			bRes = FALSE;
			
			break;

		case STA_NODISK:

			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD get NODISK ERROR.");
			#endif

			bRes = FALSE;
		
			break;

		default:
			bRes = FALSE;
		
			break;
	}

	if(bInit == TRUE)
	{
		bSD_ReInit = FALSE;

		u8SD_RetryCnt = 0;
		
		return bRes;
	}	

	if(bRes == TRUE)
	{
		bSD_ReInit = FALSE;

		u8SD_RetryCnt = 0;
	}
	else
	{
		bSD_ReInit = TRUE;

		if(u8SD_RetryCnt < SD_ERROR_MAX_CNT)
		{
			u8SD_RetryCnt++;
		}
		else
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n>>> SD reset due to SD ISSUE...");
			#endif

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

	bResult = SD_FS_Check(FALSE);

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
	SD_PWR_Disable();
	
	HAL_Delay(100);
	
	SD_PWR_Enable();
	
	HAL_Delay(100);

	if(SD_FS_Check(TRUE) == TRUE)
	{
		/*
		if(SD_CheckFileExist((char *)FILENAME_SYSLOG))
		{
			bool bRes = SD_RemoveFile((char *)FILENAME_SYSLOG);

		  #if ENABLE_DEBUG_MSG
			if(bRes)
			  printf("\r\n> Remove file SYSLOG OK.");
			else
				printf("\r\n> Remove file SYSLOG ERROR.");
		  #endif
		}
		*/

		return TRUE;
	}
	else
		return FALSE;
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
	
  	
	if(SD_FS_Check(FALSE) == TRUE)
	{
		frRes = f_mount(NULL, (TCHAR const*)USERPath, 0);

		if(frRes == FR_OK)
		{
			HAL_Delay(1);

			return TRUE;
		}
		else
		{
			HAL_Delay(1);

			return FALSE;
		}
	}
	else
	{
		return TRUE;
	}
}

void SD_Reset(void)
{
	//System_Reset();
	/*
	DSTATUS dsStat;
	
	
	u8SD_RetryCnt = 0;
			
	u8SD_ErrCnt = 0;
	
	u8SD_ResetCnt++;
	
	f_mount(NULL, (TCHAR const*)USERPath, 0);
	
	if(u8SD_ResetCnt < SD_RESET_MAX_CNT)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Reset SD card.");
		#endif
		
		SD_PWR_Disable();
		
		HAL_Delay(100);
		
		SD_PWR_Enable();
		
		HAL_Delay(100);
  }
	else
	{
	  u8SD_ResetCnt = 0;
		
		SD_Format();
		
		HAL_Delay(S_2_mS(1));
	}
	
	dsStat = disk_initialize(DRV_USER);
	
	if(dsStat == STA_OK)
	{
	  FRESULT frRes;


		SD_ShowInfo();

		frRes = f_mount(&USERFatFS, (TCHAR const *)USERPath, 1);

		if(frRes == FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD mounting OK.");
			#endif
			
			u8SD_ResetCnt = 0;
		}
		else
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD mounting ERROR %d.", frRes);
			#endif
			
			u8SD_RetryCnt++;
			
			bSD_ReInit = TRUE;
		}
	}
	else
	{
	  u8SD_RetryCnt++;
		
		bSD_ReInit = TRUE;
	}
	*/
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
	DIR dirSD_Handle;
	char cSD_FilePath[128];
	uint8_t u8TryCnt = 0;


	if(strlen(pcDirName) == 0)
		return FALSE;

	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
		return FALSE;

	sprintf(cSD_FilePath, "%d:/", DRV_USER);

	do
	{
		// Try to open directory
		frRes = f_opendir(&dirSD_Handle, cSD_FilePath);

		u8TryCnt++;

	}
	while((frRes != FR_OK) && (u8TryCnt < 2));

	if(frRes == FR_OK)
	{
		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
		if(frRes != 5)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD open directory %s ERROR %d.", pcDirName, frRes);
			#endif
			
			u8SD_ErrCnt++;
			
			if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
			{
				u8SD_ErrCnt = 0;
				
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD max error reached. Reset SD card.");
				#endif
				
				SD_Reset();
			}
		}
		else
		{
		  u8SD_ErrCnt = 0;
		}

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
	char cSD_FilePath[128];
	
	
	if(SD_FS_Check(FALSE) == FALSE)
		return FALSE;

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

	frRes = f_mkdir(cSD_FilePath);

	if(frRes == FR_OK)
	{	
		u8SD_ErrCnt = 0;
		
		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD make new directory %s ERROR %d.", pcDirName, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	char cSD_FilePath[128];
	
	
	if(SD_CheckDirExist(pcDirName) == FALSE)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD file %s does not exist anymore.", pcDirName);
    #endif

		return TRUE;
	}

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcDirName);

	frRes = f_unlink(cSD_FilePath);

	//HAL_Delay(SD_CMD_DELAY);

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD remove directory %s OK.", pcDirName);
    #endif
		
		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD remove directory %s ERROR %d.", pcDirName, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	uint8_t u8DataBuf[128];


	pcStr = (char *)u8DataBuf;

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
static bool SD_CheckFileByExten(char *pcFileName, char *pcFilter)
{
	uint8_t u8DataBuf[128];
	char *pcStr;


	pcStr = (char *)u8DataBuf;

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
//                 FALSE: The file not exist/ operation ERRORed.
//============================================================================//
bool SD_CheckFileExist(char *pcFileName)
{
	FRESULT frRes;
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	//uint8_t u8TryCnt = 0;


	if(strlen(pcFileName) == 0)
	{
		return FALSE;
	}

	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		return FALSE;
	}

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);


	// Try to open file
	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);

	if(frRes == FR_OK)
	{
		for(int i = 0; i < 1; i++)
		{
			frRes = f_close(&filSD_Handle);

			if(frRes == FR_OK)
				break;
		}
		
		if(frRes != FR_OK)
		{
		  #if ENABLE_DEBUG_MSG
			printf("\r\n> SD close file %s ERROR %d.", pcFileName, frRes);
			#endif
		}
		
		u8SD_ErrCnt = 0;
		
		//#if ENABLE_DEBUG_MSG
		//printf("\r\n> SD file %s exists.", pcFileName);
		//#endif

		return TRUE;
	}
	else
	{
		if(frRes != 4)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
			#endif
			
			u8SD_ErrCnt++;
			
			if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
			{
				u8SD_ErrCnt = 0;
				
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD max error reached. Reset SD card.");
				#endif
				
				SD_Reset();
			}
		}
		else
		{
			//#if ENABLE_DEBUG_MSG
			//printf("\r\n> SD file %s does not exist.", pcFileName);
			//#endif
			
			u8SD_ErrCnt = 0;
		}

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
bool SD_FindFileName(char *pcFindName, char *pcReturnName)
{
	bool bRes = FALSE;
	FRESULT frRes;
	DIR dirSD_Handle;
	FILINFO finfoHandle;
	char cSD_FilePath[128];
	char cName[50] = {0}, cExten[5] = {0};
	char *pcStr;


	if(strlen(pcFindName) == 0)
		return FALSE;

	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return FALSE;
	}

	strcpy(cSD_FilePath, pcFindName);

	pcStr = cSD_FilePath;

	// Split name and extension
	StopChar(pcStr, '.');

	strcpy(cName, pcStr);

  #if ENABLE_DEBUG_MSG
	printf("\r\n> SD find file with name:\"%s\".", cName);
  #endif

	if(strlen(pcStr) < strlen(pcFindName)) // If there is extension
	{
		pcStr += strlen(pcStr) + 1;

		strcpy(cExten, pcStr);
	}
	else // If there is no extension
		sprintf(cExten, "*");

  #if ENABLE_DEBUG_MSG
	printf("\r\n> SD find file with extension:\"%s\".", cExten);
  #endif

	sprintf(cSD_FilePath, "%d:/", DRV_USER);

	frRes = f_opendir(&dirSD_Handle, cSD_FilePath);

	// Open directory
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("%s ERROR CODE %d", MSG_SD_OPEN_DIR_ERROR, frRes);
		#endif

		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

		return FALSE;
	}
	else
	{
		u8SD_ErrCnt = 0;
	}

	// If use long file name format
  //#if _USE_LFN
	//char cLFN[_MAX_LFN + 1];
	//finfoHandle.lfname = cLFN;
	//finfoHandle.lfsize = sizeof(cLFN);
  //#endif

	// Check files through the SD card
	while(f_readdir(&dirSD_Handle, &finfoHandle) == FR_OK)
	{
		// If file attribute is AM_ARC then continue
		if((finfoHandle.fattrib & AM_ARC) == 0)
			continue;

		// If encounter no name files then stop
		if(finfoHandle.fname[0] == 0)
			break;

    //#if _USE_LFN
		//pcFileName = *finfoHandle.lfname ? finfoHandle.lfname : finfoHandle.fname;
    //#else
		//pcFileName = finfoHandle.fname;
    //#endif

		//#if ENABLE_DEBUG_MSG
		//printf("\r\n> SD check file name:%s.", pcFileName);
		//#endif

		// If find a random file
		if((strcmp(cName, (char *)"*") == 0) && (strcmp(cExten, (char *)"*") == 0))
		{
			sprintf(pcReturnName, "%s", finfoHandle.fname);

			bRes = TRUE;

			break;
		}
		else if(strcmp(cExten, (char *)"*") == 0) // If not care about file extension
		{
			if(SD_CheckFileByName(finfoHandle.fname, cName) == TRUE)
			{
				sprintf(pcReturnName, "%s", finfoHandle.fname);

				bRes = TRUE;

				break;
			}
		}
		else if(strcmp(cName, (char *)"*") == 0) // If not care about file name
		{
			if(SD_CheckFileByExten(finfoHandle.fname, cExten) == TRUE)
			{
				sprintf(pcReturnName, "%s", finfoHandle.fname);

				bRes = TRUE;

				break;
			}
		}
		else // If both name and extension have to be matched
		{
			if((SD_CheckFileByName(finfoHandle.fname, cName) == TRUE) &&
			   (SD_CheckFileByExten(finfoHandle.fname, cExten) == TRUE))
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
uint32_t SD_GetFileSize(char *pcFileName)
{
	FRESULT frRes;
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	uint32_t u32FileSize = 0;


	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return 0;
	}
	
	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);

	// Try to open file
	if(frRes != FR_OK)
	{
		if(frRes != 4)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD open file %s ERROR %d. Fail to get file size.", pcFileName, frRes);
			#endif
		}
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

		return 0;
	}
	else
		u8SD_ErrCnt = 0;

	// Get file size
	u32FileSize = f_size(&filSD_Handle);

	frRes = f_close(&filSD_Handle);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD close file %s ERROR %d.", pcFileName, frRes);
		#endif
	}

	return u32FileSize;
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


	do
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Format %d times.", (u8FormatCnt + 1));
		#endif
		
		memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

		frRes = f_mkfs((TCHAR const *)SDPath, 0, 0, u8SD_Buf, sizeof(u8SD_Buf));

		u8FormatCnt++;

		#if ENABLE_DEBUG_MSG
		printf("\r\n> Format %d times result: %d", u8FormatCnt, frRes);
		#endif

		HAL_Delay(500);

		if(u8FormatCnt >= 15)
		{
			break;
		}
	}
	while(frRes != FR_OK);

	if(u8FormatCnt >= 15)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD format ERROR %d", frRes);
		#endif

		return FALSE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD format OK.");
		#endif

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
	char cSD_FilePath[128];
	
	
	if(SD_CheckFileExist(pcFilename) == FALSE)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD file %s does not exist anymore.", pcFilename);
    #endif

		return TRUE;
	}


	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFilename);

	frRes = f_unlink(cSD_FilePath);

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD remove file %s OK.", pcFilename);
    #endif
		
		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD remove file %s ERROR %d.", pcFilename, frRes);
    #endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	char cSD_FilePath[128];
	
	
	if(SD_FS_Check(FALSE) == FALSE)
		return FALSE;

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcOldName);

	frRes = f_rename(cSD_FilePath, pcNewName);

	if(frRes == FR_OK)
	{
		u8SD_ErrCnt = 0;
		
		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD rename file %s to %s ERROR %d", pcOldName, pcNewName, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	UINT uBw;
	//uint16_t u16BufSize, u16DataSize;
	//uint32_t u32FileSize;


	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return FALSE;
	}
	
	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

	// Open file
	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);

	if(frRes != FR_OK) // If the specified file is not exist then create a new file
	{
		frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD open file %s ERROR %d", pcFileName, frRes);
			#endif
			
			u8SD_ErrCnt++;
			
			if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
			{
				u8SD_ErrCnt = 0;
				
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD max error reached. Reset SD card.");
				#endif
				
				SD_Reset();
			}

			return FALSE;
		}
		else
			u8SD_ErrCnt = 0;
	}
	
	f_lseek(&filSD_Handle, f_size(&filSD_Handle));

  frRes = f_write(&filSD_Handle, pu8SD_Buf, u16BufLen, &uBw);

	FRESULT fRes;

	for(int i = 0; i < 1; i++)
	{
		fRes = f_close(&filSD_Handle);

		if(fRes == FR_OK)
			break;
	}

	if(fRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
		#endif
	}

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD write to file %s OK.", pcFileName);
    #endif

		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD write to file %s at address %d ERROR %d.",
		       pcFileName, (int)f_size(&filSD_Handle), frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	//uint16_t BufSize, DataSize;
	//uint16_t u16DatCnt = 0, u16DatSize;
	UINT uBr;


	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return FALSE;
	}
	
	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);

	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_READ);

	// Open file for reading
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

		return FALSE;
	}
	else
		u8SD_ErrCnt = 0;

	// Put cursor to the start address
	frRes = f_lseek(&filSD_Handle, u32Addr);
	
	if(frRes != FR_OK)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> SD set cursor for file %s ERROR %d.", pcFileName, frRes);
    #endif
	}

	frRes = f_read(&filSD_Handle, pu8SD_Buf, u16BufLen, &uBr);
	
	FRESULT fRes;

	for(int i = 0; i < 1; i++)
	{
		fRes = f_close(&filSD_Handle);

		if(fRes == FR_OK)
			break;
	}

	if(fRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
		#endif
	}

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD read file %s OK.", pcFileName);
    #endif
		
		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD read file %s at address %d ERROR %d.",
		           pcFileName, u32Addr, frRes);

		printf("\r\n> SD number of bytes read:%d.", uBr);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	//uint16_t u16BufSize, u16DataSize;
	//uint16_t u16DatCnt = 0, u16DatSize;
	UINT uBw;


	// Check SD card status
	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return FALSE;
	}

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);
	
	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);

	// Open file
	if(frRes != FR_OK)
	{
		frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD open file %s ERROR %d.", pcFileName, frRes);
			#endif
			
			u8SD_ErrCnt++;
			
			if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
			{
				u8SD_ErrCnt = 0;
				
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD max error reached. Reset SD card.");
				#endif
				
				SD_Reset();
			}

			return FALSE;
		}
		else
			u8SD_ErrCnt = 0;
	}

	// Put cursor to the start address
	frRes = f_lseek(&filSD_Handle, u32Addr);
	
	if(frRes != FR_OK)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> SD set cursor for file %s ERROR %d.", pcFileName, frRes);
    #endif
	}

  frRes = f_write(&filSD_Handle, pu8SD_Buf, u16BufLen, &uBw);

	FRESULT fRes;

	for(int i = 0; i < 1; i++)
	{
		fRes = f_close(&filSD_Handle);

		if(fRes == FR_OK)
			break;
	}

	if(fRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
		#endif
	}

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD write to file %s OK.", pcFileName);
    #endif
		
		u8SD_ErrCnt = 0;

		return TRUE;
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD write to file %s at address %d ERROR %d.",
		           pcFileName, u32Addr, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

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
	//DIR dirSD_Handle;
	FIL filSD_Handle;
	char cSD_FilePath[128];
	uint8_t u8SD_Buf[256];
	UINT uBw;
	uint16_t u16BufSize, u16DataSize;


	if(SD_FS_Check(FALSE) == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf(MSG_SD_DISK_INVALID);
		#endif

		return FALSE;
	}

	sprintf(cSD_FilePath, "%d:/%s", DRV_USER, pcFileName);
	
	frRes = f_open(&filSD_Handle, cSD_FilePath, FA_WRITE);

	if(frRes != FR_OK)
	{
		frRes = f_open(&filSD_Handle, cSD_FilePath, FA_CREATE_ALWAYS | FA_WRITE);

		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD create file %s ERROR %d.", pcFileName, frRes);
			#endif
			
			u8SD_ErrCnt++;
			
			if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
			{
				u8SD_ErrCnt = 0;
				
				#if ENABLE_DEBUG_MSG
				printf("\r\n> SD max error reached. Reset SD card.");
				#endif
				
				SD_Reset();
			}

			return FALSE;
		}
		else
			u8SD_ErrCnt = 0;
	}

	f_lseek(&filSD_Handle, 0);

	u16BufSize = u16DataSize = 0;

	memset(u8SD_Buf, 0x00, sizeof(u8SD_Buf));

	if(bClearFlag == TRUE)
	{
		do
		{
			if((u32Addr - u16BufSize) > sizeof(u8SD_Buf))
			{
				u16DataSize = sizeof(u8SD_Buf);
			}
			else
			{
				u16DataSize = u32Addr - u16BufSize;
			}

			frRes = f_write(&filSD_Handle, &u8SD_Buf, u16DataSize, &uBw);

			if(frRes != FR_OK)
				break;

			u16BufSize += u16DataSize;
		}

		while(u16BufSize < u32Addr);
	}
	else
	{
		if(u32Addr < sizeof(u8SD_Buf))
		{
			frRes = f_write(&filSD_Handle, &u8SD_Buf, u32Addr, &uBw);
		}
		else
		{
			frRes = f_write(&filSD_Handle, &u8SD_Buf, sizeof(u8SD_Buf), &uBw);
		}
	}

	fRes = f_close(&filSD_Handle);
	
	if(fRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD close file %s ERROR %d.", pcFileName, fRes);
		#endif
	}

	if(frRes == FR_OK)
	{
    #if ENABLE_DEBUG_MSG
		printf("\r\n> SD create file %s OK.", pcFileName);
    #endif
		
		u8SD_ErrCnt = 0;

		return TRUE;

	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> SD create and write to file %s ERROR %d.", pcFileName, frRes);
		#endif
		
		u8SD_ErrCnt++;
			
		if(u8SD_ErrCnt >= SD_ERROR_MAX_CNT)
		{
			u8SD_ErrCnt = 0;
			
			#if ENABLE_DEBUG_MSG
			printf("\r\n> SD max error reached. Reset SD card.");
			#endif
			
			SD_Reset();
		}

		return FALSE;
	}
}
