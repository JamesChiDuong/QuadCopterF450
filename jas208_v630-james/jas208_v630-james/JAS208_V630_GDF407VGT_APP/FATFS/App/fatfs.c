/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */

FIL USERSrcFile;       /* File object for USER */
FIL USERDstFile;       /* File object for USER */
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
	/* additional user code for init */
	retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	struct tm tmLocalTimeInfo = {0};
	//RTC_TimeTypeDef rtcTime;
	//RTC_DateTypeDef rtcDate;
	time_t ttTimeNow;
	DWORD dwFATTime = 0, dwTemp;


	ttTimeNow = System_GetTime();

	//ttTimeNow += g_Setting.i32TimeZoneOffset;

	// Convert local time in second to local time in calendar format
	localtime_r(&ttTimeNow, &tmLocalTimeInfo);

	// Copy date time values to fattime
	// bit31:25, Year from 1980 (0..127)
	dwTemp = tmLocalTimeInfo.tm_year + 1900U - 1980U;
	dwFATTime |= dwTemp << 25;

	// bit24:21, Month (1..12)
	dwTemp = tmLocalTimeInfo.tm_mon + 1U;
	dwFATTime |= dwTemp << 21;

	// bit20:16, Day in month(1..31)
	dwTemp = tmLocalTimeInfo.tm_mday;
	dwFATTime |= dwTemp << 16;

	// bit15:11, Hour (0..23)
	dwTemp = tmLocalTimeInfo.tm_hour;
	dwFATTime |= dwTemp << 11;

	// bit10:5, Minute (0..59)
	dwTemp = tmLocalTimeInfo.tm_min;
	dwFATTime |= dwTemp << 5;

	// bit4:0, Second / 2 (0..29)
	dwTemp = tmLocalTimeInfo.tm_sec / 2;
	dwFATTime |= dwTemp;

	return dwFATTime;

  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
