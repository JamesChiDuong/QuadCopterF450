/*!
    \file    usbh_usr.c
    \brief   user application layer for USBFS host-mode MSC class operation

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/


//============================================================================//
// User Includes
//============================================================================//
#include "USBH_Usr.h"

#include <stdio.h>
#include <string.h>

#include "Config.h"
#if USBH_USE_OS == 1
#include "cmsis_os.h"
#endif
#include "drv_usb_hw.h"
#include "fatfs.h"
#include "ff.h"
#include "usbh_msc_core.h"
#include "usbh_msc_bbb.h"
#include "usbh_msc_scsi.h"


//============================================================================//
// User Defines
//============================================================================//
#if (USBH_USE_OS == 1U)
#define USBH_PROCESS_PRIO            osPriorityNormal
#define USBH_PROCESS_STACK_SIZE      ((uint16_t)512)
#endif


//============================================================================//
// User Variables
//============================================================================//
bool bUSB_Ready = FALSE;
#if (USBH_USE_OS == 1U)
osThreadId taskUSBH_ThreadHandle;
#endif
usbh_host usb_host_msc;
usb_core_driver usbh_core;

uint8_t line_idx;
uint8_t usbh_usr_application_state = USBH_USR_FS_INIT;

/*  points to the DEVICE_PROP structure of current device */
usbh_user_cb usr_cb =
{
    .dev_init = usbh_user_init,
    .dev_deinit = usbh_user_deinit,
    .dev_attach = usbh_user_device_connected,
    .dev_reset = usbh_user_device_reset,
    .dev_detach = usbh_user_device_disconnected,
    .dev_over_currented = usbh_user_over_current_detected,
    .dev_speed_detected = usbh_user_device_speed_detected,
    .dev_devdesc_assigned = usbh_user_device_desc_available,
    .dev_address_set = usbh_user_device_address_assigned,
    .dev_cfgdesc_assigned = usbh_user_configuration_descavailable,
    .dev_mfc_str = usbh_user_manufacturer_string,
    .dev_prod_str = usbh_user_product_string,
    .dev_seral_str = usbh_user_serialnum_string,
    .dev_enumerated = usbh_user_enumeration_finish,
    .dev_user_input = usbh_user_userinput,
	  .dev_msc_ready = usbh_user_msc_ready,
    .dev_user_app = usbh_usr_msc_application,
    .dev_not_supported = usbh_user_device_not_supported,
    .dev_error = usbh_user_unrecovered_error
};

const uint8_t MSG_HOST_HEADER[] = "USBFS & USBHS MSC Host";
const uint8_t MSG_HOST_FOOTER[] = "USB Host Library v2.0.0";


//============================================================================//
// User Externs
//============================================================================//
extern bool bUSB_Mounted;


//============================================================================//
// User Function Declarations
//============================================================================//
#if USBH_USE_OS == 1U
static void USBH_ThreadInit(void);
#endif


//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Initialize USB MSC application.
// @param  None.
// @retval None.
//============================================================================//
void USB_HostInit(void)
{
	usb_gpio_config();
	usb_rcu_config();

	usbh_init(&usb_host_msc,
						&usbh_core,
	#ifdef USE_USB_FS
						USB_CORE_ENUM_FS,
	#elif defined(USE_USB_HS)
						USB_CORE_ENUM_HS,
	#endif
						&usr_cb);
	
	#if USBH_USE_OS == 1U
	USBH_ThreadInit();
	#endif
	
	/* register device class */
	usbh_class_register(&usb_host_msc, &usbh_msc);

	/* enable interrupts */
	usb_intr_config ();
}

//============================================================================//
// @brief  USB Host process.
// @param  argument: Input argument for the process.
// @retval None.
//============================================================================//
void USBH_Process(void const *argument)
{
  usbh_core_task((usbh_host *)argument);
}

#if USBH_USE_OS == 1U
//============================================================================//
// @brief  USB Host Thread task.
// @param  argument: Input argument for the process.
// @retval None.
//============================================================================//
#if osCMSIS < 0x20000U // CMSIS V1
static void USBH_Process_OS(void const *argument)
{
  for(;;)
  {
    USBH_Process(argument);
		
		osDelay(1);
  }
}
#else // CMSIS V2
static void USBH_Process(void *argument)
{
  for(;;)
  {
    USBH_Process(argument);
		
		osDelay(1);
  }
}
#endif /* (osCMSIS < 0x20000U) */

//============================================================================//
// @brief  Initialize USB Host Thread.
// @param  None.
// @retval None.
//============================================================================//
static void USBH_ThreadInit(void)
{
#if osCMSIS < 0x20000U // CMSIS V1
	
  /* Create USB Host Task */
  #if defined (USBH_PROCESS_STACK_SIZE)
  osThreadDef(USBH_Thread, USBH_Process_OS, USBH_PROCESS_PRIO, 0U, USBH_PROCESS_STACK_SIZE);
  #else
  osThreadDef(USBH_Thread, USBH_Process_OS, USBH_PROCESS_PRIO, 0U, 8U * configMINIMAL_STACK_SIZE);
  #endif /* defined (USBH_PROCESS_STACK_SIZE) */

  phost->thread = osThreadCreate(osThread(USBH_Thread), phost);

#else // CMSIS V2

  /* Create USB Host Task */
  USBH_Thread_Atrr.name = "USBH_Queue";

  #if defined (USBH_PROCESS_STACK_SIZE)
  USBH_Thread_Atrr.stack_size = USBH_PROCESS_STACK_SIZE;
  #else
  USBH_Thread_Atrr.stack_size = (8U * configMINIMAL_STACK_SIZE);
  #endif /* defined (USBH_PROCESS_STACK_SIZE) */

  USBH_Thread_Atrr.priority = USBH_PROCESS_PRIO;
  phost->thread = osThreadNew(USBH_Process_OS, phost, &USBH_Thread_Atrr);

#endif /* (osCMSIS < 0x20000U) */
}
#endif

//============================================================================//
// @brief  User operation for host-mode initialization.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_init(void)
{
  static uint8_t startup = 0U;

	
	if(0U == startup) 
	{
	  startup = 1U;		
	
	  #if ENABLE_DEBUG_MSG
	  //printf("\r\n> USB host library started.");
	  #endif
	}
}

//============================================================================//
// @brief  User operation for device attached.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_device_connected(void)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
  printf("\r\n> USB Device Connected.");
	#endif
}

//============================================================================//
// @brief  User operation when unrecovered error happens.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_unrecovered_error(void)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB UNRECOVERED ERROR STATE.");
	#endif
}

//============================================================================//
// @brief  User operation for device disconnect event.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_device_disconnected(void)
{
	FRESULT frRes;


	bUSB_Ready = FALSE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Device Disconnected.");
  #endif

	frRes = f_mount(NULL, (TCHAR const*)USBHPath, 1);

	if(frRes != FR_OK)
	{
		#if ENABLE_DEBUG_MSG
		printf("\r\n> Unmount USB disk fail with error code %d.", frRes);
		#endif
	
		bUSB_Mounted = FALSE;
	}
	else
	{
		bUSB_Mounted = FALSE;
	}
}

//============================================================================//
// @brief  User operation for reset USB Device.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_device_reset(void)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> Reset the USB device.");
	#endif
}

//============================================================================//
// @brief  User operation for detecting device speed.
// @param  device_speed: USB device speed.
// @retval None.
//============================================================================//
void usbh_user_device_speed_detected(uint32_t device_speed)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	if(PORT_SPEED_HIGH == device_speed)
	{
		printf("\r\n> High speed USB device detected.");
	}
	else if(PORT_SPEED_FULL == device_speed)
	{
		printf("\r\n> Full speed USB device detected.");
	}
	else if(PORT_SPEED_LOW == device_speed)
	{
		printf("\r\n> Low speed USB device detected.");
	}
	else
	{
		printf("\r\n> USB Device Fault.");
	}
	#endif
}

//============================================================================//
// @brief  User operation when device descriptor is available.
// @param  device_desc: USB device descriptor.
// @retval None.
//============================================================================//
void usbh_user_device_desc_available(void *device_desc)
{
	#if ENABLE_DEBUG_MSG
	usb_desc_dev *pDevStr = device_desc;
	#endif

	
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Device VID: %04Xh", (uint32_t)pDevStr->idVendor);
	printf("\r\n> USB Device PID: %04Xh", (uint32_t)pDevStr->idProduct);
	#endif
}

//============================================================================//
// @brief  USB device is successfully assigned the Address.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_device_address_assigned(void)
{
}

//============================================================================//
// @brief  User operation when configuration descriptor is available.
// @param  cfg_desc: Pointer to configuration descriptor.
//         itf_desc: Pointer to interface descriptor.
//         ep_desc: Pointer to endpoint descriptor.
// @retval None.
//============================================================================//
void usbh_user_configuration_descavailable(usb_desc_config *cfg_desc,
                                           usb_desc_itf *itf_desc,
                                           usb_desc_ep *ep_desc)
{
	#if ENABLE_DEBUG_MSG
  usb_desc_itf *id = itf_desc;
	#endif
	

	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	if(0x08U  == (*id).bInterfaceClass)
	{
		printf("\r\n> USB Mass storage device connected.");
	}
	else if(0x03U  == (*id).bInterfaceClass)
	{
		printf("\r\n> USB HID device connected.");
	}
	#endif
}

//============================================================================//
// @brief  User operation when manufacturer string exists.
// @param  manufacturer_string: Manufacturer string of usb device.
// @retval None.
//============================================================================//
void usbh_user_manufacturer_string(void *manufacturer_string)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Manufacturer: %s", (char *)manufacturer_string);
	#endif
}

//============================================================================//
// @brief  User operation when product string exists.
// @param  Product_string: Product string of USB device.
// @retval None.
//============================================================================//
void usbh_user_product_string(void *product_string)
{
	bUSB_Ready = FALSE;
	
	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Product: %s", (char *)product_string);
	#endif
}

//============================================================================//
// @brief  User operation when serialNum string exists.
// @param  serial_num_string: SerialNum string of USB device.
// @retval None.
//============================================================================//
void usbh_user_serialnum_string(void *serial_num_string)
{
	bUSB_Ready = FALSE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Serial Number: %s", (char *)serial_num_string);
	#endif
}

//============================================================================//
// @brief  User response request is displayed to ask for application jump to class.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_enumeration_finish(void)
{
	bUSB_Ready = FALSE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Enumeration completed.");
	printf("\r\n---------------------------------------");
	#endif
}

//============================================================================//
// @brief  User operation when device is not supported.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_device_not_supported(void)
{
	bUSB_Ready = FALSE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Device not supported.");
	#endif
}

//============================================================================//
// @brief  User action for application state entry.
// @param  None.
// @retval None.
//============================================================================//
usbh_user_status usbh_user_userinput(void)
{
	usbh_user_status usbh_usr_status = USBH_USER_NO_RESP;

	/*key USER is in polling mode to detect user action */
  //Key1 is pressed then goto next step
	//if (RESET == READ_KEY1()) {
		usbh_usr_status = USBH_USER_RESP_OK;
	//}

	return usbh_usr_status;
}

//============================================================================//
// @brief  User action when USB device is ready to use for MSC.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_msc_ready(void)
{
	bUSB_Ready = TRUE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Device Ready.");
	#endif
}

//============================================================================//
// @brief  User operation for device over current detection event.
// @param  None.
// @retval None.
//============================================================================//
void usbh_user_over_current_detected(void)
{
	bUSB_Ready = FALSE;

	#if ENABLE_DEBUG_MSG
	printf("\r\n> USB Power Supply Overcurrent detected.");
	#endif
}

//============================================================================//
// @brief  Demo application for mass storage.
// @param  None        
// @retval Demo result in format int.
//============================================================================//
int usbh_usr_msc_application(void)
{
  return(0);
}

//============================================================================//
// @brief  Displays disk content.
// @param  path: Pointer to root path.    
//         recu_level: recursive level
// @retval Operation result in format uint8_t.
//============================================================================//
static uint8_t USB_ExploreDisk(char *path, uint8_t recu_level)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	char *fn;

   
	res = f_opendir(&dir, path);

	if(res == FR_OK) 
	{
		while((usbh_core.host.connect_status)) 
		{
      res = f_readdir(&dir, &fno);
            
			if(FR_OK != res || 0U == fno.fname[0]) 
			{
				break;
			}

			if('.' == fno.fname[0]) 
			{
				continue;
			}

			fn = fno.fname;

			line_idx++;

			if(line_idx > 4) 
			{
				line_idx = 0;
			}

			if(1U == recu_level)
			{
				printf("\r\n>   |__");
			}
			else if(2U == recu_level)
			{
				printf("\r\n>   |   |__");
			}

			if(AM_DIR == fno.fattrib)
			{
				printf("%s", fno.fname);
			}
			else
			{
				printf("%s", fno.fname);
			}

			if((AM_DIR == fno.fattrib) && (1U == recu_level))
			{
				USB_ExploreDisk(fn, 2);
			}
    }
  }

	return res;
}

//============================================================================//
// @brief  Deinitialize user state and associated variables.
// @param  None
// @retval None.
//============================================================================//
void usbh_user_deinit(void)
{
  usbh_usr_application_state = USBH_USR_FS_INIT;
}
