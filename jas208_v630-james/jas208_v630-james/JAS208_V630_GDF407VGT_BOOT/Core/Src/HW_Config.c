//============================================================================//
// User Includes
//============================================================================//

#include "HW_Config.h"
#include "main.h"
//#include "stm32f2xx_hal_rtc.h"
//#include "stm32f2xx_hal_rtc_ex.h"


//============================================================================//
// User Typedef Variables
//============================================================================//

typedef struct
{
	GPIO_TypeDef*  gpioPORT;
	uint16_t	u16PIN;
	uint32_t	u32SPEED;
	uint32_t	u32MODE;
	uint32_t  u32PULL;
}GPIO_HandleTypeDef;


//============================================================================//
// User Variables
//============================================================================//

#if HW_TYPE == HW_JAS106
static const GPIO_HandleTypeDef gpioHandleSet[] = 
{
	// 3V3 IO ENABLE
	{GPIOC,                    GPIO_PIN_6,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // EN_3V3
	
	// MX25L SPI1
	{SPI1_NSS_GPIO_Port,       SPI1_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // MX25L_SPI_CS
	
	// SD SPI2
  {SPI2_NSS_GPIO_Port,       SPI2_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_INPUT, GPIO_ NOPULL}, // SD_SPI_DETECT
	{SPI2_NSS_GPIO_Port,       SPI2_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // SD_SPI_CS
	
	// LED
	{GPIOD,                    GPIO_PIN_12,       GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LED5
	{GPIOD,                    GPIO_PIN_11,       GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LED6
	{GPIOD,                    GPIO_PIN_10,       GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}  // LED5
};
#elif HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SPI
static const GPIO_HandleTypeDef gpioHandleSet[] = 
{
	// EN_MOTOR_PWR
	{EN_MOTOR_PWR_GPIO_Port,   EN_MOTOR_PWR_Pin,  GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // EN_MOTOR_PWR
	
	// 3V3 IO ENABLE
	{GPIOC,                    GPIO_PIN_6,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // EN_3V3
	
	// ACC IN
	{GPIOA,                    GPIO_PIN_0,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_INPUT,       GPIO_NOPULL}, // ACC_IN
	
	// MX25L SPI1
	{SPI1_NSS_GPIO_Port,       SPI1_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // MX25L_SPI_CS
	
	// SD SPI2
  {EN_SD_PWR_GPIO_Port,      EN_SD_PWR_Pin,     GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // SD_PWR_ENABLE
	{SPI2_NSS_GPIO_Port,       SPI2_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_INPUT,       GPIO_NOPULL}, // SD_SPI_DETECT
	{SPI2_NSS_GPIO_Port,       SPI2_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // SD_SPI_CS
	
	// LCD
	{GPIOE, 	                 GPIO_PIN_11, 	    GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_CS
	{GPIOD,                    GPIO_PIN_3,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_RST 
	{GPIOD, 		               GPIO_PIN_4, 		    GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_A0
	{GPIOE, 	                 GPIO_PIN_2,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_SCL
	{GPIOB, 	                 GPIO_PIN_5, 	      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_SIO
	{GPIOA, 	                 GPIO_PIN_9, 	      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL} // LCD_BL
};
#elif HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SDIO
static const GPIO_HandleTypeDef gpioHandleSet[] = 
{
	// EN_MOTOR_PWR
	{EN_MOTOR_PWR_GPIO_Port,   EN_MOTOR_PWR_Pin,  GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // EN_MOTOR_PWR
	
	// 3V3 IO ENABLE
	{GPIOC,                    GPIO_PIN_6,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // EN_3V3
	
	// ACC IN
	{N_ACC_IN_GPIO_Port,       N_ACC_IN_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_INPUT,       GPIO_NOPULL}, // N_ACC_IN
	
	// MX25L SPI1
	{SPI1_NSS_GPIO_Port,       SPI1_NSS_Pin,      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // MX25L_SPI_CS
	
	// SD SPI2
  {EN_SD_PWR_GPIO_Port,      EN_SD_PWR_Pin,     GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_OD,   GPIO_NOPULL}, // SD_PWR_ENABLE
	{SD_DETECT_GPIO_Port,      SD_DETECT_Pin,     GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_INPUT,       GPIO_PULLDOWN}, // SD_SPI_DETECT
	
	// Watchdog Enable
	{WDT_EN_GPIO_Port,         WDT_EN_Pin,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // WDT_EN
	
	// LCD
	{GPIOE, 	                 GPIO_PIN_11, 	    GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_CS
	{GPIOD,                    GPIO_PIN_3,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_RST 
	{GPIOD, 		               GPIO_PIN_4, 		    GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_A0
	{GPIOE, 	                 GPIO_PIN_2,        GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_SCL
	{GPIOB, 	                 GPIO_PIN_5, 	      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL}, // LCD_SIO
	{GPIOA, 	                 GPIO_PIN_9, 	      GPIO_SPEED_FREQ_HIGH,   GPIO_MODE_OUTPUT_PP,   GPIO_NOPULL} // LCD_BL
};
#endif // HW_TYPE


//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Configure GPIO for the MCU.
// @param  None.
// @retval None.
//============================================================================//
void HW_3V3EN_GPIO_Init(void)
{
  HW_PBClr(EN_3V3);
	HW_PBInit(EN_3V3, FALSE);
	
	#if HW_TYPE == HW_JAS106
	HW_PBSet(LED5);
	HW_PBSet(LED6);
	HW_PBSet(LED7);
	
	HW_PBInit(LED5, FALSE);
	HW_PBInit(LED6, FALSE);
	HW_PBInit(LED7, FALSE);
	#elif HW_TYPE == HW_JAS208
	//HW_PBSet(LCD_CS);
	//HW_PBSet(LCD_RST);
	//HW_PBClr(LCD_A0);
	//HW_PBClr(LCD_SCL);
	//HW_PBClr(LCD_SIO);
	//HW_PBSet(LCD_BL);
	
	//HW_PBInit(LCD_CS, FALSE);
	//HW_PBInit(LCD_RST, FALSE);
	//HW_PBInit(LCD_A0, FALSE);
	//HW_PBInit(LCD_SCL, FALSE);
	//HW_PBInit(LCD_SIO, FALSE);
	//HW_PBInit(LCD_BL, FALSE);
	#endif
}

//============================================================================//
// @brief  Initialize a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
// @param  bEnEXTI: Enable corresponding interrupt or not.
// @retval None.
//============================================================================//
void HW_PBInit(GPIO_PosTypeDef gpioDef, bool bEnEXTI)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO pin parameters
	GPIO_InitStructure.Pin = gpioHandleSet[gpioDef].u16PIN;
	GPIO_InitStructure.Mode = gpioHandleSet[gpioDef].u32MODE;
	GPIO_InitStructure.Speed = gpioHandleSet[gpioDef].u32SPEED;
	GPIO_InitStructure.Pull = gpioHandleSet[gpioDef].u32PULL;
	HAL_GPIO_Init(gpioHandleSet[gpioDef].gpioPORT, &GPIO_InitStructure);

	// If enable external interrupt
	if(bEnEXTI == true)
	{	
	}
}

//============================================================================//
// @brief  Deinitialize a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
// @param  bEnEXTI: Enable corresponding interrupt or not.
// @retval None.
//============================================================================//
void HW_PBDeInit(GPIO_PosTypeDef gpioDef, bool bEnEXTI)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = gpioHandleSet[gpioDef].u16PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(gpioHandleSet[gpioDef].gpioPORT, &GPIO_InitStructure);
	
	// If enable external interrupt
	if(bEnEXTI == true)
	{	
	}
}

//============================================================================//
// @brief  Return logical value of an input pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
// @retval Logical value of the pin in uint8_t format.
//============================================================================//
uint8_t HW_PBGetState(GPIO_PosTypeDef gpioDef)
{
	return HAL_GPIO_ReadPin(gpioHandleSet[gpioDef].gpioPORT, 
		                      gpioHandleSet[gpioDef].u16PIN);
}

//============================================================================//
// @brief  Set a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
// @retval None.
//============================================================================//
void HW_PBSet(GPIO_PosTypeDef gpioDef)
{
	HAL_GPIO_WritePin(gpioHandleSet[gpioDef].gpioPORT, gpioHandleSet[gpioDef].u16PIN, 
	                  GPIO_PIN_SET);     
}

//============================================================================//
// @brief  Reset a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
// @retval None.
//============================================================================//
void HW_PBClr(GPIO_PosTypeDef gpioDef)
{
	HAL_GPIO_WritePin(gpioHandleSet[gpioDef].gpioPORT, gpioHandleSet[gpioDef].u16PIN, 
	                  GPIO_PIN_RESET);     
}

//============================================================================//
// @brief  Write to a backup register.
// @param  @u32BKP_Reg: Backup register to write.
//         @u32Dat: Data to write
// @retval None.
//============================================================================//
void HW_WriteBackupRegister(uint32_t u32BKP_Reg, uint32_t u32Dat)
{
	uint32_t u32Tmp = 0U;
	
	
	assert_param(IS_RTC_BKP(u32BKP_Reg));
	
	HAL_PWR_EnableBkUpAccess();
	
	u32Tmp = (uint32_t) & (RTC->BKP0R);
  u32Tmp += (u32BKP_Reg * 4U);
	
	*(__IO uint32_t *)u32Tmp = (uint32_t)u32Dat;
	
	//HAL_PWR_DisableBkUpAccess();
}

//============================================================================//
// @brief  Read from a backup register.
// @param  @u32BKP_Reg: Backup register to read.
// @retval Backup register value in uint32_t format.
//============================================================================//
uint32_t HW_ReadBackupRegister(uint32_t u32BKP_Reg)
{
	uint32_t u32Tmp = 0U;
	
	
	assert_param(IS_RTC_BKP(u32BKP_Reg));
	
	u32Tmp = (uint32_t) & (RTC->BKP0R);
  u32Tmp += (u32BKP_Reg * 4U);
	
	return (*(__IO uint32_t *)u32Tmp);
}
