/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>

#include "cmsis_os.h"
#include "DataType.h"
#include "GSM_Driver.h"
#include "System.h"
#include <stdio.h>
#include <stdlib.h>
    /* USER CODE END Includes */

    /* Exported types ------------------------------------------------------------*/
    /* USER CODE BEGIN ET */

    extern ADC_HandleTypeDef hadc1;

    extern CAN_HandleTypeDef hcan1;

    extern I2C_HandleTypeDef hi2c1;

    extern I2C_HandleTypeDef hi2c2;

    extern IWDG_HandleTypeDef hiwdg;

    extern RTC_HandleTypeDef hrtc;

    extern SD_HandleTypeDef hsd;

    extern SPI_HandleTypeDef hspi1;

    extern UART_HandleTypeDef *puartHandle[5];

    extern osThreadId defaultTaskHandle;
    extern osThreadId taskLogProcHandle;
    extern osThreadId taskEXTIO_ProcHandle;
    extern osThreadId taskGPS_ProcHandle;
    extern osThreadId taskGSM_ProcHandle;
    extern osThreadId taskLCD_KeyHandle;
    extern osThreadId taskRS232_ProcHandle;
    extern osThreadId taskHighLoadHandle;
    extern osThreadId taskSD_ProcHandle;
    extern osThreadId taskGSM_RXHandle;
    extern osThreadId taskGSM_TXHandle;

    extern osMessageQId GSM_BlockCpltHandle;       // Notice GSM block command completion, 1 byte command type value of LE910_BlockCMD_TypeDef
    extern osMessageQId GSM_NonBlockCpltHandle;    // Notice GSM non-block command completion, 1 byte command type value of LE910_NonBlockCMD_TypeDef
    extern osMessageQId GSM_NonBlockResHandle;     // Return GSM non-block command result,
                                                   // 2 bytes: MSB: Command type value of LE910_NonBlockCMD_TypeDef
                                                   //          LSB: Result value of GSM_NonBlockCMD_Res_Typedef
    extern osMessageQId GSM_NonBlockResCodeHandle; // Return GSM error code,
                                                   // 2 bytes: MSB: Primary error code/ CME/ CMS error code
                                                   //          LSB: Protocol error code (if have)

    extern osTimerId timerRTOS_GSM_BlockHandle;
    extern osTimerId timerRTOS_GSM_NonBlockHandle;
    extern osTimerId timerRTOS_EXTIHandle;
    extern osTimerId timerRTOS_PCTransHandle;
    extern osTimerId timerACC_OffHandle;

    extern osMutexId mutex_i2c1_user;
    extern osMutexId mutex_i2c2_user;
    extern osMutexId mutex_spi1_user;
    extern osMutexId mutex_spi2_user;
    extern osMutexId mutex_uart_user[5];
    extern osMutexId mutex_lcd_user;
    extern osMutexId mutex_prn_user;
    extern osMutexId mutex_ttnow_user;
    extern osMutexId mutex_setting_user;
    extern osMutexId mutex_gsm_block_user;
    extern osMutexId mutex_gsm_nonblock_user;
    extern osMutexId mutex_gsm_rx_user;
    extern osMutexId mutex_gsm_sd_user;
    extern osMutexId mutex_usb_user;
    extern osMutexId mutex_usb_sd_user;
    extern osMutexId mutex_sd_proc_user;
    extern osMutexId mutex_data_buf1_user;
    extern osMutexId mutex_data_buf2_user;
    extern osMutexId mutex_data_buf3_user;
    extern osMutexId mutex_malloc_user;
    extern osMutexId mutex_upload_chk_user;
    extern osMutexId mutex_uartext_user[4];

    extern bool bRTOS_En;
    extern bool bSysInitFinish;
    extern bool bOneMinFlag;
    extern bool bSecProcFlag;
    extern bool bIsCheckVDRProcFlag;

    extern uint8_t u8SysOldMin;

#if ENABLE_GSM_NET
    extern uint8_t u8DatalogIdx;
    extern bool bDataLogFlag[MAX_SOCKET_BANK];
    extern time_t ttDataLogTime[MAX_SOCKET_BANK];
    extern bool bFirstACC;
#endif

#if ENABLE_SPEED_INT_REC
    extern bool bSpeedIntFlag;
#endif

#if ENABLE_PULSE_COUNT_REC
    extern uint32_t u32TIM2_Cnt;
#endif

    extern time_t ttSysLogTime;

    extern I2C_HandleTypeDef *hi2c;

    extern osMutexId *mutex_i2c_user;
    // extern bool bMidNightFlag;

    /* USER CODE END ET */

    /* Exported constants --------------------------------------------------------*/
    /* USER CODE BEGIN EC */

    /* USER CODE END EC */

    /* Exported macro ------------------------------------------------------------*/
    /* USER CODE BEGIN EM */

    /* USER CODE END EM */

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

    /* USER CODE BEGIN EFP */

    void SystemClock_Config(void);

    void USER_PeriphInit(void);

    void USER_I2C2_Init(void);

    void USER_I2C1_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_SCL_Pin GPIO_PIN_2
#define LCD_SCL_GPIO_Port GPIOE
#define P_MTA_M_Pin GPIO_PIN_3
#define P_MTA_M_GPIO_Port GPIOE
#define P_MTA_P_Pin GPIO_PIN_4
#define P_MTA_P_GPIO_Port GPIOE
#define P_MTB_M_Pin GPIO_PIN_5
#define P_MTB_M_GPIO_Port GPIOE
#define P_MTB_P_Pin GPIO_PIN_6
#define P_MTB_P_GPIO_Port GPIOE
#define N_IO6_Pin GPIO_PIN_13
#define N_IO6_GPIO_Port GPIOC
#define EN_MOTOR_PWR_Pin GPIO_PIN_0
#define EN_MOTOR_PWR_GPIO_Port GPIOC
#define N_IO8_Pin GPIO_PIN_1
#define N_IO8_GPIO_Port GPIOC
#define N_IO7_Pin GPIO_PIN_2
#define N_IO7_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_3
#define BUZZER_GPIO_Port GPIOC
#define P_STB1_Pin GPIO_PIN_1
#define P_STB1_GPIO_Port GPIOA
#define P_STB6_Pin GPIO_PIN_2
#define P_STB6_GPIO_Port GPIOA
#define N_P_LAT_Pin GPIO_PIN_3
#define N_P_LAT_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define BAT_DETECT_Pin GPIO_PIN_4
#define BAT_DETECT_GPIO_Port GPIOC
#define GPS_PWR_EN_Pin GPIO_PIN_5
#define GPS_PWR_EN_GPIO_Port GPIOC
#define P_DI_Pin GPIO_PIN_0
#define P_DI_GPIO_Port GPIOB
#define MOTOR_PWR_ADC_Pin GPIO_PIN_1
#define MOTOR_PWR_ADC_GPIO_Port GPIOB
#define P_CLK_Pin GPIO_PIN_2
#define P_CLK_GPIO_Port GPIOB
#define RPM_IN_Pin GPIO_PIN_7
#define RPM_IN_GPIO_Port GPIOE
#define RPM_IN_EXTI_IRQn EXTI9_5_IRQn
#define SPEED_IN_Pin GPIO_PIN_8
#define SPEED_IN_GPIO_Port GPIOE
#define SPEED_IN_EXTI_IRQn EXTI9_5_IRQn
#define GSM_PWRKEY_Pin GPIO_PIN_9
#define GSM_PWRKEY_GPIO_Port GPIOE
#define GSM_WAKEUP_Pin GPIO_PIN_10
#define GSM_WAKEUP_GPIO_Port GPIOE
#define N_LCD_CS_Pin GPIO_PIN_11
#define N_LCD_CS_GPIO_Port GPIOE
#define N_IO3_Pin GPIO_PIN_12
#define N_IO3_GPIO_Port GPIOE
#define N_IO1_Pin GPIO_PIN_13
#define N_IO1_GPIO_Port GPIOE
#define N_IO4_Pin GPIO_PIN_14
#define N_IO4_GPIO_Port GPIOE
#define N_IO2_Pin GPIO_PIN_15
#define N_IO2_GPIO_Port GPIOE
#define I2C2_SCL_Pin GPIO_PIN_10
#define I2C2_SCL_GPIO_Port GPIOB
#define I2C2_SDA_Pin GPIO_PIN_11
#define I2C2_SDA_GPIO_Port GPIOB
#define SD_DETECT_Pin GPIO_PIN_12
#define SD_DETECT_GPIO_Port GPIOB
#define CAN_DETECT_Pin GPIO_PIN_13
#define CAN_DETECT_GPIO_Port GPIOB
#define N_8051_INT_Pin GPIO_PIN_15
#define N_8051_INT_GPIO_Port GPIOB
#define LCD_DET_Pin GPIO_PIN_10
#define LCD_DET_GPIO_Port GPIOD
#define P_EMPTY_Pin GPIO_PIN_11
#define P_EMPTY_GPIO_Port GPIOD
#define KEY5_Pin GPIO_PIN_12
#define KEY5_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_13
#define KEY1_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_14
#define KEY2_GPIO_Port GPIOD
#define KEY3_Pin GPIO_PIN_15
#define KEY3_GPIO_Port GPIOD
#define EN_3V3_IO_Pin GPIO_PIN_6
#define EN_3V3_IO_GPIO_Port GPIOC
#define KEY4_Pin GPIO_PIN_7
#define KEY4_GPIO_Port GPIOC
#define EN_PRN_PWR_Pin GPIO_PIN_9
#define EN_PRN_PWR_GPIO_Port GPIOC
#define CHG_STATE_Pin GPIO_PIN_8
#define CHG_STATE_GPIO_Port GPIOA
#define N_LCD_BL_Pin GPIO_PIN_9
#define N_LCD_BL_GPIO_Port GPIOA
#define DRIVE_VBUS_Pin GPIO_PIN_10
#define DRIVE_VBUS_GPIO_Port GPIOA
#define WDT_EN_Pin GPIO_PIN_15
#define WDT_EN_GPIO_Port GPIOA
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define N_LCD_RST_Pin GPIO_PIN_3
#define N_LCD_RST_GPIO_Port GPIOD
#define LCD_A0_Pin GPIO_PIN_4
#define LCD_A0_GPIO_Port GPIOD
#define N_ACC_IN_Pin GPIO_PIN_7
#define N_ACC_IN_GPIO_Port GPIOD
#define EN_SD_PWR_Pin GPIO_PIN_3
#define EN_SD_PWR_GPIO_Port GPIOB
#define G_SEN_INT_Pin GPIO_PIN_4
#define G_SEN_INT_GPIO_Port GPIOB
#define LCD_SIO_Pin GPIO_PIN_5
#define LCD_SIO_GPIO_Port GPIOB
#define GSM_RESET_Pin GPIO_PIN_0
#define GSM_RESET_GPIO_Port GPIOE
#define N_IO5_Pin GPIO_PIN_1
#define N_IO5_GPIO_Port GPIOE

#define I2C1_SCL_Pin GPIO_PIN_8
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
