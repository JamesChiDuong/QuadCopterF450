//============================================================================//
// User Includes
//============================================================================//
#include "System.h"

#include <string.h>
#include <stdlib.h>

#include "stm32fxxx_hal_user.h"
#include "DWT_Delay.h"
#include "HW_Config.h"
#include "main.h"
#include "LCD_Key.h"
#include "MX25L_Driver.h"
#include "SD_Module.h"
#include "USB_Module.h"


//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Enable 3V3 power for peripherals.
// @param  None.
// @retval None.
//============================================================================//
void System_IO_3V3_Enable(void)
{
  HW_PBSet(EN_3V3);
	
	HAL_Delay(10);
}

//============================================================================//
// @brief  Disable 3V3 power.
// @param  None.
// @retval None.
//============================================================================//
void System_IO_3V3_Disable(void)
{
	HW_PBClr(EN_3V3);
	
	HAL_Delay(10);
}

//============================================================================//
// @brief  Initialize system.
// @param  None.
// @retval None.
//============================================================================//
void System_Init(void)
{
	HW_3V3EN_GPIO_Init();
	
	HW_PBSet(EN_MOTOR_PWR);
	
	BackLightDisable;
	
	#if ENABLE_DEBUG_MSG
	System_IO_3V3_Enable();
	#else
	HAL_UART_DeInit(&huart3);
	HAL_UART_DeInit(&huart4);
	#endif
	
	HAL_Delay(100);
	
	//#if ENABLE_DEBUG_MSG
	//printf("\r\n>>> DEVICE IN BOOTLOADER MODE...");
	//#endif
	
	USB_HostInit();
	
	SD_Init();
	
	MX25L_Init();
	
	bLCD_Running = FALSE;
}

//============================================================================//
// @brief  Deinitialize system.
// @param  None.
// @retval None.
//============================================================================//
void System_DeInit(void)
{
	HAL_Delay(S_2_mS(1)); 
}

//============================================================================//
// @brief  System software reset.
// @param  None.
// @retval None.
//============================================================================//
void System_Reset(void)
{
	System_DeInit();

	// Generate system reset to allow jumping to the user code
	NVIC_SystemReset();
}

//============================================================================//
// @brief  Check if the main port for terminal communication is SET or not.
// @param  None.
// @retval TRUE: The main port for terminal communication is SET.
//         FALSE: The main port for terminal communication is DB9.
//============================================================================//
bool IsMainRS232_SET(void)
{
  bool bRes;
	
	
	//#if CUSTOMER_TYPE == CUSTOMER_NORMAL || CUSTOMERTYPE == CUSTOMER_ARTC
	#if CUSTOMER_TYPE == CUSTOMER_ARTC
	bRes = 1;
	#else
	bRes = 0;
	#endif
	
	return bRes;
}
