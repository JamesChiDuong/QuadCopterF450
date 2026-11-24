//============================================================================//
// User Includes
//============================================================================//

#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "IAP_Func.h"
#include "CommFunc.h"
#include "Config.h"
#include "fatfs.h"
#include "HW_Config.h"
#include "LCD_Key.h"
#include "SD_Module.h"
#include "USB_Module.h"
#include "user_diskio.h"
#include "Version.h"


//============================================================================//
// User Variables
//============================================================================//

bool bInDownload = TRUE;

MEM_U32_t mem32DatBuf[8192]; // 32kB data buffer
//MEM_U32_t mem32DatBuf[2048]; // 32kB data buffer

FIL filHandle2;
DIR dirHandle2;

#if HW_TYPE == HW_JAS106
GPIO_PosTypeDef gpioLED_Curr;
#elif HW_TYPE == HW_JAS208
int iLoadingPercent = 0;
#endif

//HeaderTypeDef headerInstance;


//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Download firmware from SD/USB drive to flash.
// @param  @fatfsHandle: Pointer to an instance of FATFS.
//         @cDrvPath: Path to drive.
//         @u8Drv: Drive SD or USB.
// @retval TRUE: If process successful.
//         FALSE: If process fail. 
//============================================================================//
bool IAP_Process(FATFS* fatfsHandle, char* cDrvPath, uint8_t u8Drv)
{
	const uint32_t u32ExFlashTempAddr = EXFLASH_TEMP_START_ADDR, 
	               u32FlashAddr = APPLICATION_ADDR;
	FRESULT frRes;
	HAL_StatusTypeDef halRes;
	MX25L_StatusTypeDef mxlRes, mxlRes2;
	bool bCRC_Check = TRUE;
	char cPath[128];
	uint16_t u16CRC16Chk, u16CRC16Res;
	MEM_U32_t mem32CodeSize;
	uint32_t u32FileSize, u32ProgSize,
	         u32DatCnt = 0, u32DatSize = 0;
  uint16_t u16SectEraseNum;
	uint32_t u32PageAddr;
	uint32_t u32FirstByteBuf;
	UINT uBr;
	#if HW_TYPE == HW_JAS208
	char cPercent[5];
	#endif
	bool bRes;
	
	
	// If ACC is OFF
	if(HW_PBGetState(N_ACC_IN) == 1)
	{
		#if 0
		const char cMsg1[] = "ACC OFF";
		const char cMsg2[] = "Stop upgrade";
		const char cMsg3[] = "Go to sleep...";
	
	
		System_IO_3V3_Enable();
		
		HAL_Delay(100);
		
		LCD_Init();
		
		LCD_ScreenClear();
		
		LCD_DisplayMsg((char *)cMsg1, 1, 1);
		LCD_DisplayMsg((char *)cMsg2, 3, 1);
		LCD_DisplayMsg((char *)cMsg3, 5, 1);
		
		HAL_Delay(3000);
		
		// If ACC is still OFF
		if(HW_PBGetState(N_ACC_IN) == 1)
		{
			LCD_ScreenClear();
			BackLightDisable;
			
		  HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);
			
			System_Reset();
		}
		#endif
	}
	
	#if HW_TYPE == HW_JAS106
	gpioLED_Curr = LED5;
  #endif
	
	IAP_StartNotify();
	
	frRes = f_mount(fatfsHandle, cDrvPath, 0);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Mount to drive fail with ERROR %d", frRes);
		#endif
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	sprintf(cPath, "%d:/", u8Drv);
	
	frRes = f_opendir(&dirHandle2, cPath);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Open drive %s fail with ERROR %d", cPath, frRes);
		#endif
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}

	sprintf(cPath, "%d:/%s_%02d_%02d_%02d.%s", u8Drv, DEVICE_TYPE,
	        MODEL_NAME, CUSTOMER_TYPE, HW_VER, UPGRADE_FILE_EXT);
	
	frRes = f_open(&filHandle2, cPath, FA_READ);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		//printf("\r\n> Open upgrade file in drive fail!");
		printf("\r\n> Open upgrade file %s fail with ERROR %d", cPath, frRes);
		#endif 
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	u32FileSize = f_size(&filHandle2);
	
  #if ENABLE_HEADER_CHECK
	u32ProgSize = u32FileSize - (uint32_t)FLASH_HEADER_SIZE;
	
	if(u32ProgSize == 0)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Upgrade file has wrong content!");
		#endif
	
		IAP_StopNotify(FALSE);
	  
		return FALSE;
	}
	
	// Read header at the beginning of the file
	f_lseek(&filHandle2, 0);
	
	memset(mem32DatBuf, 0x00, (uint32_t)FLASH_HEADER_SIZE);
	
	frRes = f_read(&filHandle2, mem32DatBuf, (uint32_t)FLASH_HEADER_SIZE, &uBr);
		
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Get firmware header fail!");
		#endif
		
		IAP_StopNotify(FALSE);
	  
		return FALSE;
	}
	
	// Get firmware CRC checksum
	u16CRC16Chk = ((uint16_t)(mem32DatBuf[0]._U8[3]) << 8) | (uint16_t)mem32DatBuf[0]._U8[2];

	// Get firmware size
	mem32CodeSize._U8[3] = mem32DatBuf[1]._U8[3];
	mem32CodeSize._U8[2] = mem32DatBuf[1]._U8[2];
	mem32CodeSize._U8[1] = mem32DatBuf[1]._U8[1];
	mem32CodeSize._U8[0] = mem32DatBuf[1]._U8[0];
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Firmware CRC Checksum:%X", u16CRC16Chk);
	
	printf("\r\n> Firmware size:%d", (uint32_t)(mem32CodeSize._U32));
	#endif
	
	if(u32ProgSize != mem32CodeSize._U32)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> Firmware size unmatch!");
		#endif
		
		IAP_StopNotify(FALSE);
	  
		return FALSE;
	}
	
	CRC16_Reset(&u16CRC16Res);
  #else
	u32ProgSize = u32FileSize;
  #endif 
	
	// Make sure the file has content and firmware size is not larger than 
	// application flash size
	if((u32ProgSize == 0) || (u32ProgSize > (FLASH_END_ADDR - APPLICATION_ADDR + 1)))
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Insufficient size of firmware!");
		#endif 
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	u16SectEraseNum = (uint16_t)(u32ProgSize / MX25L_SECTOR_SIZE);
	
	if(u32ProgSize % MX25L_SECTOR_SIZE)
		u16SectEraseNum++;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Erase external flash...");
	#endif
	
	//MX25L_BlockUnlock();
	
	mxlRes = MX25L_MultiSectorsErase((u32ExFlashTempAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);
	
	if(mxlRes != MX25L_OK)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> Erase external flash fail with error code %d.!", mxlRes);
		#endif 
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Erase external flash done. Download firmware from disk to external flash.");
	#endif
	
	u32DatCnt = 0;
	
	// Read upgrade file and program into external flash
	do
	{
	  u32DatSize = u32ProgSize - u32DatCnt;
					
		if(u32DatSize > sizeof(mem32DatBuf))
			 u32DatSize = sizeof(mem32DatBuf); 
		
		f_lseek(&filHandle2, (uint32_t)FLASH_HEADER_SIZE + u32DatCnt);
		
		frRes = f_read(&filHandle2, mem32DatBuf, u32DatSize, &uBr);
		
		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Reading file %s at address %d fail with ERROR %d.", 
		         FILENAME_UPGRADE_BIN, u32DatCnt, frRes);
			#endif 
			
			break;
		}
		
		// Save the first 4 bytes to write at the end
		if(u32DatCnt == 0)
		{
		  u32FirstByteBuf = mem32DatBuf[0]._U32;
			
			mem32DatBuf[0]._U32 = 0xFFFFFFFF;
		}
		
		u32PageAddr = (u32ExFlashTempAddr + u32DatCnt) / MX25L_PAGE_SIZE;
			
		mxlRes = MX25L_MultiPagesProgram(u32PageAddr, (uint8_t *)mem32DatBuf, u32DatSize);
		
		if(u32DatCnt == 0)
		{
			mem32DatBuf[0]._U32 = u32FirstByteBuf;
		}
			
	  if(mxlRes != MX25L_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Program to external flash at address %d fail with ERROR %d.", 
			       u32ExFlashTempAddr + u32DatCnt, mxlRes);
			#endif 
			
			break;
		}
				
	  u32DatCnt += u32DatSize;
		
    #if ENABLE_HEADER_CHECK	
		for(int i = 0; i < (u32DatSize / sizeof(uint32_t)); i++)
		{
			u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[0]);
			u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[1]);
			u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[2]);
			u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[3]);
		}
		
		for(int i = 0; i < (u32DatSize % sizeof(uint32_t)); i++)
		  u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[u32DatSize / sizeof(uint32_t)]._U8[i]);
    #endif
		
		if(u32DatCnt % 2048 == 0)
		{
		  #if ENABLE_DEBUG_MSG
		  printf("\r\n> Finish download %d bytes from disk to external flash.", u32DatCnt);
	    #endif
			
			#if HW_TYPE == HW_JAS208
			iLoadingPercent = (int)((float)u32DatCnt / (u32ProgSize * 2) * 100);
			
			sprintf(cPercent, "%d%%  ", iLoadingPercent);
			
			LCD_DisplayMsg(cPercent, 1, 9);
			#endif
		}
		
	}while((frRes == FR_OK) && (mxlRes == MX25L_OK) && 
	       (u32DatCnt < u32ProgSize));
	
	f_close(&filHandle2);

  #if ENABLE_HEADER_CHECK	
	if(u16CRC16Res != u16CRC16Chk)
	{
		bCRC_Check = FALSE;
	
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download from disk to external flash CRC unmatched.");
		printf(" Calculated CRC:0x%04X", u16CRC16Res);
		printf(" Original CRC:0x%04X", u16CRC16Chk);
		#endif
  }
  #endif
	
	for(int i = 0; i < 3; i++)
	{
	  mxlRes2 = MX25L_MultiSectorsErase((EXFLASH_TEMP_SIZE_START_ADDR / MX25L_SECTOR_SIZE), 1);
		
		if(mxlRes2 != MX25L_OK)
			continue;
				
		u32PageAddr = EXFLASH_TEMP_SIZE_START_ADDR / (uint32_t)MX25L_PAGE_SIZE;
	
	  mxlRes2 = MX25L_MultiPagesProgram(u32PageAddr, (uint8_t *)&u32ProgSize, 4);
		
		if(mxlRes2 == MX25L_OK)
			break;
	}
	
	if(mxlRes2 != MX25L_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Write temp firmware size ERROR: %d", mxlRes2);
		#endif
	}
	
	// Write back the first 4 bytes
	if((u32FirstByteBuf & 0x2FFE0000) == 0x20000000)
	{
		if((frRes == FR_OK) && (mxlRes == MX25L_OK) && (bCRC_Check == TRUE))
		{
			u32PageAddr = u32ExFlashTempAddr / MX25L_PAGE_SIZE;
			
			for(int i = 0; i < 3; i++)
			{
				mxlRes = MX25L_MultiPagesProgram(u32PageAddr, (uint8_t *)&u32FirstByteBuf, 4);
				
				if(mxlRes == MX25L_OK)
					break;
			}
		}
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Invalid first byte buf: 0x%08X", u32FirstByteBuf);
		#endif
		
		mxlRes = MX25L_WRITE_ERROR;
	}
	
	// If read upgrade file fail or program external flash fail 
	// or CRC checksum is not match then terminate the operation
	if((frRes != FR_OK) || (mxlRes != MX25L_OK) || (!bCRC_Check))
	{
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	// If ACC is OFF
	if(HW_PBGetState(N_ACC_IN) == 1)
	{
		#if 0
		const char cMsg1[] = "ACC OFF";
		const char cMsg2[] = "Stop upgrade";
		const char cMsg3[] = "Go to sleep...";
	
	
		System_IO_3V3_Enable();
		
		HAL_Delay(100);
		
		LCD_Init();
		
		LCD_ScreenClear();
		
		LCD_DisplayMsg((char *)cMsg1, 1, 1);
		LCD_DisplayMsg((char *)cMsg2, 3, 1);
		LCD_DisplayMsg((char *)cMsg3, 5, 1);
		
		HAL_Delay(3000);
		
		// If ACC is still OFF
		if(HW_PBGetState(N_ACC_IN) == 1)
		{
			LCD_ScreenClear();
			BackLightDisable;
			
		  HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);
			
			System_Reset();
		}
		#endif
	}

  #if ENABLE_HEADER_CHECK
	CRC16_Reset(&u16CRC16Res);
  #endif
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Erase flash...");
	#endif
	
	// Erase current flash content to program new content
	for(int i = 0; i < 5; i++)
	{
	  bRes = IAP_FLASH_Erase();
		
		if(bRes)
		  break;
		
		HAL_Delay(100);
	}
	
	if(bRes == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Flash erase error, terminate current operation .");
		#endif 
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Erase flash done.");
	#endif
	
	u32DatCnt = 0;
	
	// Unlock flash
	HAL_FLASH_Unlock();
	
	// Read from external flash and program into chip flash
  do
	{
		memset(mem32DatBuf, 0xFF, sizeof(mem32DatBuf));
		
		u32DatSize = u32ProgSize - u32DatCnt;
		
		if(u32DatSize > sizeof(mem32DatBuf))
			 u32DatSize = sizeof(mem32DatBuf);
		
		for(int i = 0; i < 5; i++)
		{
			mxlRes = MX25L_MultiBytesRead(u32ExFlashTempAddr + u32DatCnt, (uint8_t *)mem32DatBuf, u32DatSize);
		
			if(mxlRes == MX25L_OK)
				break;
			
			HAL_Delay(100);
		}
		
		if(mxlRes != MX25L_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Read from external flash at address %d fail with ERROR %d.", 
			       u32ExFlashTempAddr + u32DatCnt, mxlRes);
			#endif 
			
			break;
		}
		
		for(int i = 0; i < (sizeof(mem32DatBuf) / sizeof(uint32_t)); i++)
		{
			// Save the first 4 bytes to write at last
			if((u32DatCnt == 0) && (i == 0))
			{
			  u32FirstByteBuf = mem32DatBuf[0]._U32;
				
				halRes = HAL_OK;
				
				continue;
			}
			
			for(int j = 0; j < 5; j++)
			{
				// Write word into flash memory
				halRes = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, u32FlashAddr + 
																	 u32DatCnt + i * sizeof(uint32_t), mem32DatBuf[i]._U32);
			
				if(halRes == HAL_OK)
					break;
				
				HAL_Delay(100);
			}
			
			if(halRes != HAL_OK)
			{
				#if ENABLE_DEBUG_MSG
			  printf("\r\n> Program to flash at address %d fail with ERROR %d.", 
			         APPLICATION_ADDR + u32DatCnt + i * sizeof(uint32_t), halRes);
			  #endif
				
				break;
			}	
		}
		
		if(halRes == HAL_OK)
		{
			u32DatCnt += u32DatSize;
			
			#if ENABLE_HEADER_CHECK
			for(int i = 0; i < (u32DatSize / sizeof(uint32_t)); i++)
			{
				u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[0]);
				u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[1]);
				u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[2]);
				u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[i]._U8[3]);
			}
			
			for(int i = 0; i < (u32DatSize % sizeof(uint32_t)); i++)
				u16CRC16Res = CRC16_Calc(u16CRC16Res, mem32DatBuf[u32DatSize / sizeof(uint32_t)]._U8[i]);
			#endif
			
			if(u32DatCnt % 2048 == 0)
			{
				#if ENABLE_DEBUG_MSG
					printf("\r\n> Finish download %d bytes from external flash to flash.", u32DatCnt);
				#endif
				
				#if HW_TYPE == HW_JAS208
				iLoadingPercent = (int)((float)(u32DatCnt + u32ProgSize) / (u32ProgSize * 2) * 100);
				
				sprintf(cPercent, "%d%%  ", iLoadingPercent);
				
				LCD_DisplayMsg(cPercent, 1, 9);
				#endif
			}
		}
		else
		{
		  break;
		}
		
	}while((mxlRes == MX25L_OK) && (halRes == HAL_OK) &&
	       (u32DatCnt < u32ProgSize));
	
  #if ENABLE_HEADER_CHECK
	if(u16CRC16Res != u16CRC16Chk)
	{
		bCRC_Check = FALSE;
	
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download from external flash to flash CRC unmatched.");
		printf(" Calculated:0x%04X", u16CRC16Res);
		printf(" Original CRC:0x%04X", u16CRC16Chk);
		#endif
	}
  #endif
	
	// Write back the first 4 bytes
	if((u32FirstByteBuf & 0x2FFE0000) == 0x20000000)
	{
		if((mxlRes == MX25L_OK) && (halRes == HAL_OK) && (bCRC_Check == TRUE))
		{
			for(int i = 0; i < 3; i++)
			{
				halRes = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, u32FlashAddr, u32FirstByteBuf);
			
				if(halRes == HAL_OK)
					break;
			}
		}
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Invalid first byte buf: 0x%08X", u32FirstByteBuf);
		#endif
		
		halRes = HAL_ERROR;
	}
	
	// Lock flash
	HAL_FLASH_Lock();
	
	// If read external flash fail or program flash fail
	// or CRC checksum is not match then 
	// erase flash and terminate the current operation
	if((mxlRes != MX25L_OK) || (halRes != HAL_OK) || (!bCRC_Check))
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download from external flash to flash fail, terminate current operation.");
	  #endif
		
		// Erase flash
		if(!IAP_FLASH_Erase())
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Flash erase error, terminate current operation.");
			#endif 
		}
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	sprintf(cPath, "%d:/%s", u8Drv, FILENAME_UPGRADE_BIN);
	
	if(u8Drv == DRV_USER)
	  f_unlink(cPath);
	
	f_closedir(&dirHandle2);
	
	f_mount(NULL, cDrvPath, 0);
	
	IAP_StopNotify(TRUE);
	
	IAP_CompleteNotify(1);
	
	if(u8Drv == DRV_USB)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Finish upgrade firmware. Take out USB to run the application.");
		#endif
		
	  while(bUSB_Ready == TRUE)
		{
			USBH_Process(&usb_host_msc);
			
		  HAL_Delay(1);
		}
	}
	
	#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306SD || HW_TYPE == HW_JAS411
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Finish upgrade firmware. Reset to run the application.");
	#endif
	
	HW_WriteBackupRegister(BKP_DFU_CHK, BKP_JUMP_VALUE);
	#endif
	
	#if HW_TYPE == HW_JAS306
	// If ACC is ON set the program to jump directly to application after reset
	// otherwise go to sleep
	if(HW_PBGetState(N_ACC_IN) == 0)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Finish upgrade firmware. Reset to run the application.");
		#endif
		
		HW_WriteBackupRegister(BKP_DFU_CHK, BKP_JUMP_VALUE);
	}
	else
	{
		LCD_ScreenClear();
		BackLightDisable;
		
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Finish upgrade firmware. Go to sleep.");
		#endif
		
		HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);
	}
	#endif
	
	// Reset to run the application
	System_Reset();
	
  return TRUE;	
}

//============================================================================//
// @brief  Download firmware image from the external exflash to the chip flash.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool IAP_LoadFWImage(void)
{
	const uint32_t u32ExFlashTempAddr = EXFLASH_TEMP_START_ADDR, 
	               u32FlashAddr = APPLICATION_ADDR;
	HAL_StatusTypeDef halRes;
	MX25L_StatusTypeDef mxlRes;
	uint32_t u32ProgSize, u32DatCnt = 0, u32DatSize = 0;
	uint32_t u32FirstByteBuf;
	#if HW_TYPE == HW_JAS208
	char cPercent[5];
	#endif
	bool bRes;
	
	
	// If ACC is OFF
	if(HW_PBGetState(N_ACC_IN) == 1)
	{
		#if 0
		const char cMsg1[] = "ACC OFF";
		const char cMsg2[] = "Stop upgrade";
		const char cMsg3[] = "Go to sleep...";
	
	
		System_IO_3V3_Enable();
		
		HAL_Delay(100);
		
		LCD_Init();
		
		LCD_ScreenClear();
		
		LCD_DisplayMsg((char *)cMsg1, 1, 1);
		LCD_DisplayMsg((char *)cMsg2, 3, 1);
		LCD_DisplayMsg((char *)cMsg3, 5, 1);
		
		HAL_Delay(3000);
		
		// If ACC is still OFF
		if(HW_PBGetState(N_ACC_IN) == 1)
		{
			LCD_ScreenClear();
			BackLightDisable;
			
		  HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);
			
			System_Reset();
		}
		#endif
	}
	
	#if HW_TYPE == HW_JAS106
	gpioLED_Curr = LED5;
  #endif
	
	//#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306 || HW_TYPE == HW_JAS306SD
	//LCD_Init();
	//#endif
	
	IAP_StartNotify();
	
	u32ProgSize = HW_ReadBackupRegister(BKP_FW_SIZE);
	
	if(u32ProgSize == 0)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Firmware file not found!");
		#endif
	
		IAP_StopNotify(FALSE);
	  
		return FALSE;
	}
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Firmware size:%d", u32ProgSize);
	#endif	
	
	for(int i = 0; i < 5; i++)
	{
	  bRes = IAP_FLASH_Erase();
		
		if(bRes)
		  break;
		
		HAL_Delay(100);
	}
	
	if(bRes == FALSE)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Flash erase error, terminate current operation .");
		#endif 
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	u32DatCnt = 0;
	
	// Unlock flash
	HAL_FLASH_Unlock();
	
	// Read from external flash and program into chip flash
  do
	{
		memset(mem32DatBuf, 0xFF, sizeof(mem32DatBuf));
		
		u32DatSize = u32ProgSize - u32DatCnt;
		
		if(u32DatSize > sizeof(mem32DatBuf))
			 u32DatSize = sizeof(mem32DatBuf);
		
		for(int i = 0; i < 5; i++)
		{
			mxlRes = MX25L_MultiBytesRead(u32ExFlashTempAddr + u32DatCnt, (uint8_t *)mem32DatBuf, u32DatSize);
		
		  if(mxlRes == MX25L_OK)
				break;
			
			HAL_Delay(100);
		}
		
		if(mxlRes != MX25L_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Read from external flash at address %d fail with error code %d.", 
			       u32ExFlashTempAddr + u32DatCnt, mxlRes);
			#endif 
			
			break;
		}
		
		for(int i = 0; i < (sizeof(mem32DatBuf) / sizeof(uint32_t)); i++)
		{
			// Save the first 4 bytes to write at last
			if((u32DatCnt == 0) && (i == 0))
			{
			  u32FirstByteBuf = mem32DatBuf[0]._U32;
				
				halRes = HAL_OK;
				
				continue;
			}
			
			for(int j = 0; j < 5; j++)
			{
				// Write word into flash memory
				halRes = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, u32FlashAddr + 
																	 u32DatCnt + i * sizeof(uint32_t), mem32DatBuf[i]._U32);
				
				if(halRes == HAL_OK)
					break;
				
				HAL_Delay(100);
			}
			
			if(halRes != HAL_OK)
			{
				#if ENABLE_DEBUG_MSG
			  printf("\r\n> Program byte %d to Flash fail with error code %d.", 
			         u32DatCnt + i * sizeof(uint32_t), halRes);
			  #endif
				
				break;
			}	
		}
		
		if(halRes == HAL_OK)
		{
			u32DatCnt += u32DatSize;
			
			if(u32DatCnt % 2048 == 0)
			{
				#if ENABLE_DEBUG_MSG
				printf("\r\n> Finish download %d bytes from external flash to flash.", u32DatCnt);
				#endif
				
				#if HW_TYPE == HW_JAS208
				iLoadingPercent = (int)((float)u32DatCnt / u32ProgSize * 100);
				
				sprintf(cPercent, "%d%%  ", iLoadingPercent);
				
				LCD_DisplayMsg(cPercent, 1, 9);
				#endif
			}
		}
		else
		{
		  break;
		}
		
	}while((mxlRes == MX25L_OK) && (halRes == HAL_OK) && (u32DatCnt < u32ProgSize));
	
	// Write back the first 4 bytes
	if((u32FirstByteBuf & 0x2FFE0000) == 0x20000000)
	{
		if((mxlRes == MX25L_OK) && (halRes == HAL_OK))
		{
			for(int i = 0; i < 3; i++)
			{
				halRes = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, u32FlashAddr, u32FirstByteBuf);
				
				if(halRes == HAL_OK)
					break;
			}
		}
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Invalid first byte buf: 0x%08X", u32FirstByteBuf);
		#endif
		
		halRes = HAL_ERROR;
	}
	
	// Lock flash
	HAL_FLASH_Lock();
	
	// If read external flash fail or program flash fail
	// or CRC checksum is not match then 
	// erase flash and terminate the current operation
	if((mxlRes != MX25L_OK) || (halRes != HAL_OK))
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download from external flash to flash fail, terminate current operation.");
	  #endif
		
		// Erase flash
		if(!IAP_FLASH_Erase())
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Flash erase error, terminate current operation.");
			#endif 
		}
		
		IAP_StopNotify(FALSE);
		
		return FALSE;
	}
	
	IAP_StopNotify(TRUE);
	
	IAP_CompleteNotify(0);
	
	#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306SD || HW_TYPE == HW_JAS411
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Finish upgrade firmware. Reset to run the application.");
	#endif
	
	HW_WriteBackupRegister(BKP_DFU_CHK, BKP_JUMP_VALUE);
	#endif
	
	#if HW_TYPE == HW_JAS306 
	// If ACC is ON set the program to jump directly to application after reset
	// otherwise go to sleep
	if(HW_PBGetState(N_ACC_IN) == 0)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Finish upgrade firmware. Reset to run the application.");
		#endif
		
		HW_WriteBackupRegister(BKP_DFU_CHK, BKP_JUMP_VALUE);
	}
	else
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Finish upgrade firmware. Go to sleep.");
		#endif
		
		LCD_ScreenClear();
		BackLightDisable;
		
		HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);
	}
	#endif
	
	// Reset to run the application
	System_Reset();
	
  return TRUE;	
}

//============================================================================//
// @brief  Start to notify that the current operation is IAP.
// @param  None.
// @retval None.
//============================================================================//
void IAP_StartNotify(void)
{
	#if HW_TYPE == HW_JAS208
	const char cMsg1[] = "Loading:0%";
	
	
	System_IO_3V3_Enable();
	
	HAL_Delay(100);
	
	// Only show screen if ACC is ON
	if(HW_PBGetState(N_ACC_IN) == 0)
		LCD_Init();
	
	LCD_ScreenClear();
	
	LCD_DisplayMsg((char *)cMsg1, 1, 1);
	
	#elif HW_TYPE == HW_JAS106
	HAL_TIM_Base_Start_IT(&htim3);
	#endif
	
	bInDownload = TRUE;
}

//============================================================================//
// @brief  Stop to notify that the current operation is IAP.
// @param  None.
// @retval None.
//============================================================================//
#if HW_TYPE == HW_JAS106
void IAP_StopNotify(void)
#elif HW_TYPE == HW_JAS208
void IAP_StopNotify(bool bRes)
#endif
{
	bInDownload = FALSE;
	
	HAL_Delay(1);
	
	#if HW_TYPE == HW_JAS106
	HAL_TIM_Base_Stop_IT(&htim3);
	
	HW_PBSet(LED5);
	HW_PBSet(LED6);
	HW_PBSet(LED7);
	#elif HW_TYPE == HW_JAS208
	if(!bRes)
	{
	  const char cMsg1[] = "Loading";
		const char cMsg2[] = "failed";
		const char cMsg3[] = "No firmware";
		
		
		LCD_ScreenClear();
		
		LCD_DisplayMsg((char *)cMsg1, 1, 1);
		
		LCD_DisplayMsg((char *)cMsg2, 3, 1);
		
		HAL_Delay(2000);
		
		LCD_ScreenClear();
		
		LCD_DisplayMsg((char *)cMsg3, 1, 1);
	}
	#endif
}

//============================================================================//
// @brief  Notify that IAP is completed.
// @param  None.
// @retval None.
//============================================================================//
void IAP_CompleteNotify(bool bUseUSB)
{
	#if HW_TYPE == HW_JAS106
  HAL_Delay(500);
	
	HW_PBClr(LED5);
	HW_PBClr(LED6);
	HW_PBClr(LED7);
	
	HAL_Delay(S_2_mS(2));
	
	HW_PBSet(LED5);
	HW_PBSet(LED6);
	HW_PBSet(LED7);
	#elif HW_TYPE == HW_JAS208
	const char cMsg1[] = "FW downloaded";
	const char cMsg2[] = "successfully";
	const char cMsg3[] = "Take out USB";
	
	
	LCD_ScreenClear();
	
	LCD_DisplayMsg((char *)cMsg1, 1, 1);
	
	LCD_DisplayMsg((char *)cMsg2, 3, 1);
	
	if(bUseUSB)
	  LCD_DisplayMsg((char *)cMsg3, 5, 1);
	#endif
	
	HAL_Delay(S_2_mS(2));
}

//============================================================================//
// @brief  Erase application section in flash.
// @param  None.
// @retval TRUE: if operation successful.
//         FALSE: if operation fail.
//============================================================================//
bool IAP_FLASH_Erase(void)
{
	HAL_StatusTypeDef halRes;
	FLASH_EraseInitTypeDef halFlashEraseInstance;
	uint32_t u32EraseError = 0;
	
	
	halFlashEraseInstance.TypeErase = TYPEERASE_SECTORS;
  halFlashEraseInstance.VoltageRange = VOLTAGE_RANGE_3;
  halFlashEraseInstance.Sector = APP_FIRST_UNIT;
  halFlashEraseInstance.NbSectors = APP_UNIT_NUMBER;
	
	HAL_FLASH_Unlock();
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                         FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	halRes = HAL_FLASHEx_Erase(&halFlashEraseInstance, &u32EraseError);
	
	if(halRes != HAL_OK)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> Flash erase error at sector %d with error code %d.", u32EraseError, halRes);
		#endif 
		
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
bool IAP_CheckRDP(void)
{
  FLASH_OBProgramInitTypeDef obHandle = {0};
	
	
	HAL_FLASHEx_OBGetConfig(&obHandle);
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> RDP level: 0x%X", obHandle.RDPLevel);
	#endif
	
	if(obHandle.RDPLevel != OB_RDP_LEVEL_1)
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
bool IAP_SetRDP(void)
{
  FLASH_OBProgramInitTypeDef obHandle = {0};
	HAL_StatusTypeDef halRes;
	
	
	HAL_FLASHEx_OBGetConfig(&obHandle);
	
	obHandle.OptionType = OPTIONBYTE_RDP;
	obHandle.RDPLevel = OB_RDP_LEVEL_1;
	
	HAL_FLASH_OB_Unlock();
	
	halRes = HAL_FLASHEx_OBProgram(&obHandle);
	
	if(halRes != HAL_OK)
	{
		HAL_FLASH_OB_Lock();
	
	  //HAL_FLASH_Lock();
	 	
	  return FALSE;
	}
	
	halRes = HAL_FLASH_OB_Launch();
	
	HAL_FLASH_OB_Lock();
	
	if(halRes == HAL_OK)
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
bool IAP_CheckWRP(void)
{
  FLASH_OBProgramInitTypeDef obHandle = {0};
	
	
	HAL_FLASHEx_OBGetConfig(&obHandle);
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> WRP state: %s", (obHandle.WRPSector == OB_WRP_SECTOR_All) ? (char *)"OFF" : (char *)"ON");
	#endif
	
	if(obHandle.WRPSector == OB_WRP_SECTOR_All)
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
bool IAP_EnableWRP(void)
{
  FLASH_OBProgramInitTypeDef obHandle = {0};
	HAL_StatusTypeDef halRes;
	uint32_t u32WRP_Sectors = 0;
	
	
	HAL_FLASHEx_OBGetConfig(&obHandle);
	
	obHandle.OptionType = OPTIONBYTE_WRP;
	obHandle.WRPState = OB_WRPSTATE_ENABLE;
	//obHandle.WRPSector = OB_WRP_SECTOR_All;
	obHandle.Banks = FLASH_BANK_1;
	
	for(int i = 0; i < APP_FIRST_UNIT; i++)
	{
	  u32WRP_Sectors |= (OB_WRP_SECTOR_0 << i);
	}
	
	obHandle.WRPSector = u32WRP_Sectors;
	
	HAL_FLASH_OB_Unlock();
	
	halRes = HAL_FLASHEx_OBProgram(&obHandle);
	
	if(halRes != HAL_OK)
	{
		HAL_FLASH_OB_Lock();
	 	
	  return FALSE;
	}
	
	halRes = HAL_FLASH_OB_Launch();
	
	HAL_FLASH_OB_Lock();
	
	if(halRes == HAL_OK)
	{
		return TRUE;
	}
	else
	{
	  return FALSE;
	}
}

//============================================================================//
// @brief  Disable WRP.
// @param  None.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.  
//============================================================================//
bool IAP_DisableWRP(void)
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
	
	if(halRes != HAL_OK)
	{
		HAL_FLASH_OB_Lock();
	 	
	  return FALSE;
	}
	
	halRes = HAL_FLASH_OB_Launch();
	
	HAL_FLASH_OB_Lock();
	
	if(halRes == HAL_OK)
	{
		return TRUE;
	}
	else
	{
	  return FALSE;
	}
}

//============================================================================//
// @brief  Download to Chinese font file to external flash.
// @param  @fatfsHandle: Pointer to an instance of FATFS.
//         @cDrvPath: Path to drive.
//         @u8Drv: Drive SD or USB.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is fail. 
//============================================================================//
bool IAP_SaveChineseFont(FATFS* fatfsHandle, char* cDrvPath, uint8_t u8Drv)
{
	//bool bRes;
	FRESULT frRes;
	MX25L_StatusTypeDef mxlRes;
	char cPath[128];
	uint32_t u32StartAddr = (uint32_t)EXFLASH_CHARACTERS_LIB_START_ADDR;
  uint32_t u32FileSize, u32DatCnt = 0, u32DatSize;
	uint16_t u16PageAddr = 0, u16SectEraseNum = 0;
	int iLoadingPercent;
	char cPercent[5];
	UINT uBr;
	  
	
	#if HW_TYPE == HW_JAS208
	System_IO_3V3_Enable();
	
	HAL_Delay(100);
	
	// Only show screen if ACC is ON
	if(HW_PBGetState(N_ACC_IN) == 0)
		LCD_Init();
	
	LCD_ScreenClear();
	
	LCD_DisplayMsg((char *)"Download file", 1, 1);
	#endif
	
	frRes = f_mount(fatfsHandle, cDrvPath, 0);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Mount to drive ERROR %d.", frRes);
		#endif
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Download fail", 1, 1);
		
		HAL_Delay(2000);
		#endif 
		
		return FALSE;
	}
	
	sprintf(cPath, "%d:/", u8Drv);
	
	frRes = f_opendir(&dirHandle2, cPath);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Open drive directory ERROR %d", frRes);
		#endif
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Download fail", 1, 1);
		
		HAL_Delay(2000);
		#endif 
		
		return FALSE;
	}

	sprintf(cPath, "%d:/%s", u8Drv, CHINESE_FONT_FILE);
	
	frRes = f_open(&filHandle2, cPath, FA_READ);
	
	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Open character font file in drive ERROR %d", frRes);
		#endif 
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Download fail", 1, 1);
		
		HAL_Delay(2000);
		#endif
		
		return FALSE;
	}
	
	u32FileSize = f_size(&filHandle2);
	
	if(u32FileSize == 0)
	{
		#if ENABLE_DEBUG_MSG
	  printf("\r\n> Size of character font file %s is 0.", CHINESE_FONT_FILE);
		#endif
		
		return TRUE;
	}
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> SD size of character font file %s is %d.", CHINESE_FONT_FILE, u32FileSize);
  #endif
				
	u16SectEraseNum = (uint16_t)(u32FileSize / MX25L_SECTOR_SIZE);

	if(u32FileSize % MX25L_SECTOR_SIZE)
		u16SectEraseNum++;
	
	mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);
	
	if(mxlRes != MX25L_OK)
	{
	  #if ENABLE_DEBUG_MSG
		printf("\r\n> Erase Exflash ERROR %d. Terminate download character font file.", mxlRes);
		#endif
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Erase fail", 1, 1);
		
		HAL_Delay(2000);
		#endif
		
		return FALSE;
	}
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Start download character font file from SD to Exflash.");
	#endif
	
	#if HW_TYPE == HW_JAS208
	LCD_ScreenClear();
	
	LCD_DisplayMsg((char *)"Loading:0%", 1, 1);
	#endif
	
	//MX25L_ChipErase();
	
	u32DatCnt = 0;
	
	do
	{
		u32DatSize = u32FileSize - u32DatCnt;
		
		if(u32DatSize > sizeof(mem32DatBuf))
			u32DatSize = sizeof(mem32DatBuf); 
		
		frRes = f_lseek(&filHandle2, u32DatCnt);
	
		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Set font file pointer ERROR %d", frRes);
			#endif 
			
			break;
		}
		
		frRes = f_read(&filHandle2, (uint8_t *)mem32DatBuf, u32DatSize, &uBr);
		
		if(frRes != FR_OK)
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Reading source file %s at address %d FAIL.", CHINESE_FONT_FILE, u32DatCnt);
			#endif
			
			break;
		}
		
		u16PageAddr = (uint16_t)((u32StartAddr + u32DatCnt) / MX25L_PAGE_SIZE);
		
		mxlRes = MX25L_MultiPagesProgram((uint32_t)u16PageAddr, (uint8_t *)mem32DatBuf, u32DatSize);
		
		if(mxlRes == MX25L_OK)
		{
			u32DatCnt += u32DatSize;
			
			if((u32DatCnt % (sizeof(mem32DatBuf) * 1)) == 0)
			{
				#if ENABLE_DEBUG_MSG
				printf("\r\n> Finish transfer %d bytes.", u32DatCnt);			
				#endif					
				
				#if HW_TYPE == HW_JAS208
				iLoadingPercent = (int)((float)u32DatCnt / u32FileSize * 100);
		
				sprintf(cPercent, "%d%%  ", iLoadingPercent);
				
				LCD_DisplayMsg(cPercent, 1, 9);
				#endif
			}					
			
			//for(int i = 0; i < u32DatSize; i++)
			//{
			//	u32CRC_Val1 = CRC32_Calc(u8Buf[i]);
			//}
		}
		else
		{
			#if ENABLE_DEBUG_MSG
			printf("\r\n> Program to Exflash at page %d fail with ERROR %d.", u16PageAddr, mxlRes);
			#endif
			
			break;
		}	
	} 
	while(u32DatCnt < u32FileSize);
	
	f_close(&filHandle2);
	
	if((frRes != FR_OK) || (mxlRes != MX25L_OK))
	{
		f_closedir(&dirHandle2);
		
		mxlRes = MX25L_MultiSectorsErase((uint16_t)(u32StartAddr / MX25L_SECTOR_SIZE), u16SectEraseNum);
		
		if(mxlRes != MX25L_OK)
		{
		  #if ENABLE_DEBUG_MSG
			printf("\r\n> Erase Exflash ERROR %d.", mxlRes);
			#endif
		}
		
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download character font file FAIL.");
		#endif
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Download fail", 1, 1);
		
		if(u8Drv == DRV_USB)
		{
		  LCD_DisplayMsg((char *)"Take out USB", 3, 1);
			
			while(bUSB_Ready == TRUE)
			{
				USBH_Process(&usb_host_msc);
				
				HAL_Delay(1);
			}
		}
		else if(u8Drv == DRV_USER)
		{
		  HAL_Delay(2000);
		}
		#endif
		
		return FALSE;
	}
	else
	{
		//if(u8Drv == DRV_USER)
		//  f_unlink(cPath);
		
		f_closedir(&dirHandle2);
		
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Download character font file OK.");
		#endif
		
		#if HW_TYPE == HW_JAS208
		LCD_ScreenClear();
	
	  LCD_DisplayMsg((char *)"Download OK", 1, 1);
		
		if(u8Drv == DRV_USB)
		{
		  LCD_DisplayMsg((char *)"Take out USB", 3, 1);
			
			while(bUSB_Ready == TRUE)
			{
				USBH_Process(&usb_host_msc);
				
				HAL_Delay(1);
			}
		}
		else if(u8Drv == DRV_USER)
		{
			SD_RemoveFile((char *)CHINESE_FONT_FILE);
			
		  HAL_Delay(2000);
		}
		#endif
		
	  return TRUE;
	}
}

