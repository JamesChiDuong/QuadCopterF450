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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "DataType.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern SD_HandleTypeDef hsd;

extern SPI_HandleTypeDef hspi1;

extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

typedef void (*pFunction)(void);

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_SCL_Pin GPIO_PIN_2
#define LCD_SCL_GPIO_Port GPIOE
#define EN_MOTOR_PWR_Pin GPIO_PIN_0
#define EN_MOTOR_PWR_GPIO_Port GPIOC
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define N_LCD_CS_Pin GPIO_PIN_11
#define N_LCD_CS_GPIO_Port GPIOE
#define SD_DETECT_Pin GPIO_PIN_12
#define SD_DETECT_GPIO_Port GPIOB
#define LCD_DET_Pin GPIO_PIN_10
#define LCD_DET_GPIO_Port GPIOD
#define N_LCD_BL_Pin GPIO_PIN_9
#define N_LCD_BL_GPIO_Port GPIOA
#define DRIVE_VBUS_Pin GPIO_PIN_10
#define DRIVE_VBUS_GPIO_Port GPIOA
#define WDT_EN_Pin GPIO_PIN_15
#define WDT_EN_GPIO_Port GPIOA
#define N_LCD_RST_Pin GPIO_PIN_3
#define N_LCD_RST_GPIO_Port GPIOD
#define LCD_A0_Pin GPIO_PIN_4
#define LCD_A0_GPIO_Port GPIOD
#define N_ACC_IN_Pin GPIO_PIN_7
#define N_ACC_IN_GPIO_Port GPIOD
#define EN_SD_PWR_Pin GPIO_PIN_3
#define EN_SD_PWR_GPIO_Port GPIOB
#define LCD_SIO_Pin GPIO_PIN_5
#define LCD_SIO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
