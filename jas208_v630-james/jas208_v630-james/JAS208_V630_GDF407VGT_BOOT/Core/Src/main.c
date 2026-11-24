/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "Config.h"
#include "Default.h"
#include "IAP_Func.h"
#include "LCD_Key.h"
#include "SD_Module.h"
#include "System.h"
#include "USB_Module.h"
#include "Version.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

pFunction Jump_To_Application;

uint32_t JumpAddress;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

#if ENABLE_DEBUG_MSG
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif

static void JumpToApp(void);

static void DFUCheck(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SDIO_SD_Init();
    MX_SPI1_Init();
    MX_TIM3_Init();
    MX_UART4_Init();
    MX_USART3_UART_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    char cUpgradeFileName[30];

    HAL_PWR_EnableBkUpAccess();

    DFUCheck();

    System_Init();

    HAL_TIM_Base_Start_IT(&htim3);

    HAL_Delay(10);

    uint32_t u32BKP_DFU_CHK = HW_ReadBackupRegister(BKP_DFU_CHK);

    uint16_t u16TimeOut = 500;

    while (u16TimeOut-- && (bUSB_Ready == FALSE))
    {
        USBH_Process(&usb_host_msc);

        HAL_Delay(1);
    }

    // #if ENABLE_DEBUG_MSG
    // printf("\r\n> Check for Chinese font file...");
    // #endif

    if (bUSB_Ready)
        if (USB_CheckFileExist((char *)CHINESE_FONT_FILE))
        {
#if ENABLE_DEBUG_MSG
            printf("\r\n> Character font file exists in USB.");
#endif

            for (int i = 0; i < 3; i++)
            {
                bool bRes = IAP_SaveChineseFont(&USBHFatFS, USBHPath, DRV_USB);

                if (bRes)
                    break;
            }
        }

    if (SD_FS_GetStatus())
        if (SD_CheckFileExist((char *)CHINESE_FONT_FILE))
        {
#if ENABLE_DEBUG_MSG
            printf("\r\n> Character font file exists in SD.");
#endif

            for (int i = 0; i < 3; i++)
            {
                bool bRes = IAP_SaveChineseFont(&USERFatFS, USERPath, DRV_USER);

                if (bRes)
                    break;
            }
        }

    if (u32BKP_DFU_CHK == BKP_LOAD_VALUE)
    {
#if ENABLE_DEBUG_MSG
        printf("\r\n> Load firmware image from ExFlash.");
#endif

        HW_WriteBackupRegister(BKP_DFU_CHK, BKP_RST_VALUE);

        IAP_LoadFWImage();
    }

    sprintf(cUpgradeFileName, "%s_%02d_%02d_%02d.%s", DEVICE_TYPE,
            MODEL_NAME, CUSTOMER_TYPE, HW_VER, UPGRADE_FILE_EXT);

    // #if ENABLE_DEBUG_MSG
    // printf("\r\n> Check for upgrade file:%s...", cUpgradeFileName);
    // #endif

    for (int i = 0; i < 3; i++)
    {
        if (bUSB_Ready)
        {
            if (USB_CheckFileExist(cUpgradeFileName))
            {
#if ENABLE_DEBUG_MSG
                printf("\r\n> Upgrade file exists in USB.");
#endif

                IAP_Process(&USBHFatFS, USBHPath, DRV_USB);
            }
        }

        if (SD_FS_GetStatus())
            if (SD_CheckFileExist(cUpgradeFileName))
            {
#if ENABLE_DEBUG_MSG
                printf("\r\n> Upgrade file exists in SD.");
#endif

                IAP_Process(&USERFatFS, USERPath, DRV_USER);
            }

        HAL_Delay(1);
    }

    // Test if user code is programmed starting from user start code address
    if (((*(__IO uint32_t *)APPLICATION_ADDR) & 0x2FFE0000) == 0x20000000)
    {
        HW_WriteBackupRegister(BKP_DFU_CHK, BKP_JUMP_VALUE);

        // #if ENABLE_DEBUG_MSG
        // printf("\r\n>>> GO TO APPLICATION...\r\n");
        // #endif

        System_Reset();
    }
    else
    {
        const uint32_t u32ExFlashTempAddr = EXFLASH_TEMP_START_ADDR;
        uint32_t u32AddrChk = 0;
        uint32_t u32ProgSize = 0;
        MX25L_StatusTypeDef mxlRes;

#if ENABLE_DEBUG_MSG
        printf("\r\n>>> FOUND NO PROGRAM FOR APPLICATION!!! TRY TO REINSTALL BACKUP FIRMWARE...");
#endif

        mxlRes = MX25L_MultiBytesRead(u32ExFlashTempAddr, (uint8_t *)&u32AddrChk, 4);

        if (mxlRes == MX25L_OK)
        {
            if ((u32AddrChk & 0x2FFE0000) == 0x20000000)
            {
                // mxlRes = MX25L_MultiBytesRead(EXFLASH_TEMP_SIZE_START_ADDR, (uint8_t *)&u32ProgSize, 4);

                // if((mxlRes != MX25L_OK) || (u32ProgSize > (PROGRAM_SIZE - sizeof(mem32DatBuf))))
                //{
                //	u32ProgSize = PROGRAM_SIZE - sizeof(mem32DatBuf);
                // }

                u32ProgSize = PROGRAM_SIZE - sizeof(mem32DatBuf);

                HW_WriteBackupRegister(BKP_FW_SIZE, u32ProgSize);

                IAP_LoadFWImage();
            }
            else
            {
#if ENABLE_DEBUG_MSG
                printf("\r\n>>> NO VALID BACKUP FIRMWARE. PLEASE REINSTALL FIRMWARE");
#endif
            }
        }
        else
        {
#if ENABLE_DEBUG_MSG
            printf("\r\n>>> NO VALID BACKUP FIRMWARE. PLEASE REINSTALL FIRMWARE");
#endif
        }
    }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
#if HW_TYPE == HW_JAS106
    uint16_t u16Cnt = 0;
#elif HW_TYPE == HW_JAS208
    const char cMsg1[] = "No firmware";

    System_IO_3V3_Enable();

    HAL_Delay(100);

    // Only show screen if ACC is ON
    if (HW_PBGetState(N_ACC_IN) == 0)
        LCD_Init();

    LCD_ScreenClear();

    LCD_DisplayMsg((char *)cMsg1, 1, 1);
#endif

    while (1)
    {
        USBH_Process(&usb_host_msc);
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (bUSB_Ready)
            if (USB_CheckFileExist(cUpgradeFileName))
            {
#if ENABLE_DEBUG_MSG
                printf("\r\n> Upgrade file exists in USB.");
#endif

                LCD_DisplayMsg((char *)cMsg1, 1, 1);

                IAP_Process(&USBHFatFS, USBHPath, DRV_USB);
            }

        if (SD_FS_GetStatus())
            if (SD_CheckFileExist(cUpgradeFileName))
            {
#if ENABLE_DEBUG_MSG
                printf("\r\n> Upgrade file exists in SD.");
#endif

                IAP_Process(&USERFatFS, USERPath, DRV_USER);
            }

#if HW_TYPE == HW_JAS106
        u16Cnt++;

        if (u16Cnt == 30)
        {
            HW_PBClr(LED7);
        }
        else if (u16Cnt == 60)
        {
            HW_PBSet(LED7);

            u16Cnt = 0;
        }
#endif

        // HAL_Delay(1);
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 240;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDIO_SD_Init(void)
{

    /* USER CODE BEGIN SDIO_Init 0 */

    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 1;
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 11999;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 9999;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART4_Init(void)
{

    /* USER CODE BEGIN UART4_Init 0 */

    /* USER CODE END UART4_Init 0 */

    /* USER CODE BEGIN UART4_Init 1 */

    /* USER CODE END UART4_Init 1 */
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 115200;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART4_Init 2 */

    /* USER CODE END UART4_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(EN_MOTOR_PWR_GPIO_Port, EN_MOTOR_PWR_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, SPI1_NSS_Pin | N_LCD_BL_Pin | WDT_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(N_LCD_CS_GPIO_Port, N_LCD_CS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(DRIVE_VBUS_GPIO_Port, DRIVE_VBUS_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(N_LCD_RST_GPIO_Port, N_LCD_RST_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, EN_SD_PWR_Pin | LCD_SIO_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : LCD_SCL_Pin N_LCD_CS_Pin */
    GPIO_InitStruct.Pin = LCD_SCL_Pin | N_LCD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : EN_MOTOR_PWR_Pin */
    GPIO_InitStruct.Pin = EN_MOTOR_PWR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(EN_MOTOR_PWR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : SPI1_NSS_Pin N_LCD_BL_Pin DRIVE_VBUS_Pin WDT_EN_Pin */
    GPIO_InitStruct.Pin = SPI1_NSS_Pin | N_LCD_BL_Pin | DRIVE_VBUS_Pin | WDT_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : SD_DETECT_Pin */
    GPIO_InitStruct.Pin = SD_DETECT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SD_DETECT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LCD_DET_Pin N_ACC_IN_Pin */
    GPIO_InitStruct.Pin = LCD_DET_Pin | N_ACC_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : N_LCD_RST_Pin LCD_A0_Pin */
    GPIO_InitStruct.Pin = N_LCD_RST_Pin | LCD_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : EN_SD_PWR_Pin */
    GPIO_InitStruct.Pin = EN_SD_PWR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(EN_SD_PWR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LCD_SIO_Pin */
    GPIO_InitStruct.Pin = LCD_SIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_SIO_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

#if ENABLE_DEBUG_MSG
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
    if (IsMainRS232_SET())
        HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
    else
        HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}
#endif

//============================================================================//
// @brief  Go to the application program.
// @param  None.
// @retval None.
//============================================================================//
static void JumpToApp(void)
{
    // Jump to user application
    JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDR + 4);

    Jump_To_Application = (pFunction)JumpAddress;

    HAL_DeInit();

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    // Initialize user application's Stack Pointer
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDR);

    Jump_To_Application();
}

//============================================================================//
// @brief  Check if the system needs to go to the application program.
// @param  None.
// @retval None.
//============================================================================//
static void DFUCheck(void)
{
    uint32_t u32ChkVal;

    u32ChkVal = HW_ReadBackupRegister(BKP_SLEEP_CHK);

    if (u32ChkVal == BKP_SLEEP_VALUE)
    {
        // Test if user code is programmed starting from user start code address
        if (((*(__IO uint32_t *)APPLICATION_ADDR) & 0x2FFE0000) == 0x20000000)
        {
            // #if ENABLE_DEBUG_MSG
            // printf("\r\n> BOOT JUMP TO SLEEP.");
            // #endif

            // If ACC is ON
            if (HW_PBGetState(N_ACC_IN) == 0)
            {
                HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_RST_VALUE);
            }

            JumpToApp();
        }
    }

    u32ChkVal = HW_ReadBackupRegister(BKP_DFU_CHK);

    if (u32ChkVal == BKP_JUMP_VALUE)
    {
        HW_WriteBackupRegister(BKP_DFU_CHK, BKP_RST_VALUE);

        // Test if user code is programmed starting from user start code address
        if (((*(__IO uint32_t *)APPLICATION_ADDR) & 0x2FFE0000) == 0x20000000)
        {
            // #if ENABLE_DEBUG_MSG
            // printf("\r\n> BOOT JUMP TO APP.");
            // #endif

            JumpToApp();
        }
    }
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM2)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
