/**
  ******************************************************************************
  * @file    usbh_diskio.c (based on usbh_diskio_template.c v2.0.2)
  * @brief   USB Host Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE BEGIN firstSection */
/* can be used to modify / undefine following code or add new definitions */
/* USER CODE END firstSection */

/* Includes ------------------------------------------------------------------*/
#include "usbh_diskio.h"

#include <stdio.h>

#include "ff_gen_drv.h"
#include "USB_Conf.h"
#include "usbh_msc_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define USB_DEFAULT_BLOCK_SIZE 512

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
DSTATUS USBH_initialize (BYTE);
DSTATUS USBH_status (BYTE);
DRESULT USBH_read (BYTE, BYTE*, DWORD, UINT);

#if _USE_WRITE == 1
  DRESULT USBH_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
  DRESULT USBH_ioctl (BYTE, BYTE, void*);
#endif /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  USBH_Driver =
{
  USBH_initialize,
  USBH_status,
  USBH_read,
#if  _USE_WRITE == 1
  USBH_write,
#endif /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USBH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

// User Variables
static volatile DSTATUS state = STA_NOINIT; /* disk status */

// User Externs
extern usbh_host usb_host_msc;

/* USER CODE BEGIN beforeFunctionSection */
/* can be used to modify / undefine following code or add new code */
/* USER CODE END beforeFunctionSection */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_initialize(BYTE drv)
{
  /* CAUTION : USB Host library has to be initialized in the application */
  usb_core_driver *udev = (usb_core_driver *)usb_host_msc.data;
	
	
	if(udev->host.connect_status) 
	{	
		state &= ~STA_NOINIT;
	}

	return state;
}

/**
  * @brief  Gets Disk Status
  * @param  drv: physical drive number
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_status(BYTE drv)
{
  return state;
}

/* USER CODE BEGIN beforeReadSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeReadSection */

/**
  * @brief  Reads Sector(s)
  * @param  drv: physical drive number
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE drv, BYTE *buff, DWORD sector, UINT count)
{
  BYTE status = USBH_OK;
  usb_core_driver *udev = (usb_core_driver *)usb_host_msc.data;

	
	if(!count)
	{
	  return RES_PARERR;
	}

	if(state & STA_NOINIT) {
		return RES_NOTRDY;
	}

	if(udev->host.connect_status) 
	{
		do
		{
			status = usbh_msc_read (&usb_host_msc, drv, sector, buff, count);

			if(!udev->host.connect_status) 
			{
				return RES_ERROR;
			}
			
		}while(status == USBH_BUSY);
	}

	if(status == USBH_OK) 
	{
		return RES_OK;
	}

	return RES_ERROR;
}

/* USER CODE BEGIN beforeWriteSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeWriteSection */

/**
  * @brief  Writes Sector(s)
  * @param  drv: physical drive number
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USBH_write(BYTE drv, const BYTE *buff, DWORD sector, UINT count)
{
  BYTE status = USBH_OK;
	usb_core_driver *udev = (usb_core_driver *)usb_host_msc.data;

	
	if(!count) 
	{
		return RES_PARERR;
	}

	if(state & STA_NOINIT) 
	{
		return RES_NOTRDY;
	}

	if(state & STA_PROTECT) 
	{
		return RES_WRPRT;
	}

	if(udev->host.connect_status)
	{
		do 
		{
			status = usbh_msc_write(&usb_host_msc, drv, sector, (BYTE*)buff, count);

			if(!udev->host.connect_status) 
			{
				return RES_ERROR;
			}
		
		}while(status == USBH_BUSY);
	}

	if(status == USBH_OK) 
	{
		return RES_OK;
	}

	return RES_ERROR;
}
#endif /* _USE_WRITE == 1 */

/* USER CODE BEGIN beforeIoctlSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeIoctlSection */

/**
  * @brief  I/O control operation
  * @param  drv: physical drive number
  * @param  ctrl: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE drv, BYTE ctrl, void *buff)
{
  DRESULT res = RES_OK;
	msc_lun info;


	res = RES_ERROR;

	if(state & STA_NOINIT) 
	{
		return RES_NOTRDY;
	}

	switch(ctrl) 
	{
	  /* make sure that no pending write process */
	  case CTRL_SYNC:
			res = RES_OK;
			
		  break;

	  /* get number of sectors on the disk (dword) */
	  case GET_SECTOR_COUNT:
			if(USBH_OK == usbh_msc_lun_info_get(&usb_host_msc, drv, &info)) 
			{
				*(DWORD*)buff = (DWORD)info.capacity.block_nbr;
					
				res = RES_OK;
			}
			
			break;

	  /* get r/w sector size (word) */
	  case GET_SECTOR_SIZE:
			if(USBH_OK == usbh_msc_lun_info_get(&usb_host_msc, drv, &info)) 
			{
				*(WORD*)buff = (DWORD)info.capacity.block_size;
					
				res = RES_OK;
			}
			
			break;

	  /* get erase block size in unit of sector (dword) */
	  case GET_BLOCK_SIZE:
		  *(DWORD*)buff = 512;
		  //if(USBH_OK == usbh_msc_lun_info_get(&usb_host_msc, drv, &info)) 
			//{
			//	*(WORD*)buff = (DWORD)info.capacity.block_size / USB_DEFAULT_BLOCK_SIZE;
					
			//	res = RES_OK;
			//}
			
	    break;

	  default:
			res = RES_PARERR;
			
		  break;
	}

	return res;
}
#endif /* _USE_IOCTL == 1 */

/* USER CODE BEGIN lastSection */
/* can be used to modify / undefine previous code or add new code */
/* USER CODE END lastSection */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

