/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f2xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "CAN_Driver.h"
#include "drv_usbh_int.h"
#include "EXTIO.h"
#include "HW_Config.h"
#include "System.h"
#include "UART_Driver.h"
#include "user_diskio.h"
#include "USBH_Usr.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern SD_HandleTypeDef hsd;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */
    // register int RV __asm("r1");

    // RTOSprintf("\r\n>>> Hardfault!!! R1: 0x%X", RV);
    TaskHandle_t myTaskHandle = xTaskGetCurrentTaskHandle();
    printf("\r\n> HFault: %d", (int)myTaskHandle);
    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    /* USER CODE BEGIN BusFault_IRQn 0 */
    RTOSprintf("\r\n>>> Busfault!!!");

    /* USER CODE END BusFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles CAN1 RX0 interrupts.
 */
void CAN1_RX0_IRQHandler(void)
{
    /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

    /* USER CODE END CAN1_RX0_IRQn 0 */
    HAL_CAN_IRQHandler(&hcan1);
    /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

    if (__HAL_CAN_GET_FLAG(&hcan1, CAN_FLAG_FOV0))
    {
        __HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_FOV0);
    }
    // else if(__HAL_CAN_GET_FLAG(&hcan1, CAN_FLAG_FF0))
    //{
    //	__HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_FF0);
    // }
    else
    {
        HAL_StatusTypeDef halRes;

        halRes = HAL_CAN_GetRxMessage(&hcan1, 0, &CAN_RxHeaderHandle, u8CAN_RxMsg);

        if (halRes == HAL_OK)
        {
            bCAN_GetRX = TRUE;

            CAN_ParseMsg();
        }
        else
        {
            // printf("\r\n> Get CAN HAL ERROR %d", halRes);
        }

        bSpeedIntFlag = TRUE;
    }
    /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    // Handling RPM_IN interrupt)
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_7))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);

        if (HW_PBGetState(ACC_IN) == 0)
        {
            uiRPM_TickIdx++;
        }
    }
    // Handling SPEED_IN interrupt
    else if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_8))
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);

        if (HW_PBGetState(ACC_IN) == 0)
        {
            uiSPD_TickIdx++;

            g_SysInfo.tVehicleState.u32DistCount++;

            if ((g_Setting.tSpeedSet.u8Auto_Gain_Count < 128) &&
                (((g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS) &&
                  ((g_SysInfo.tVehicleState.u32Speed_GPS >= (30 * 1000)) &&
                   (g_SysInfo.tGPS_Data.bStatus == TRUE))) ||
                 (g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_CAN)))
            {
                g_SysInfo.tVehicleState.u32DistCount_AGain++; // Heinz
            }
        }

        bSpeedIntFlag = TRUE;
    }

    /* USER CODE END EXTI9_5_IRQn 0 */
    /* USER CODE BEGIN EXTI9_5_IRQn 1 */

    /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    uint8_t u8RecvDat;

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    // If the interrupt source is ORE
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
    }
    // If the interrupt source is RXNE
    else if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
    {
        if (HAL_UART_Receive(&huart1, &u8RecvDat, 1, 10) == HAL_OK)
            UART_SaveRecvData(UART_PORT1, &u8RecvDat, 1);
    }

    /* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles USART2 global interrupt.
 */
void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART2_IRQn 0 */
    uint8_t u8RecvDat;

    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);
    /* USER CODE BEGIN USART2_IRQn 1 */
    // If the interrupt source is ORE
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(&huart2);
    }
    // If the interrupt source is RXNE
    else if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
    {
        if (HAL_UART_Receive(&huart2, &u8RecvDat, 1, 10) == HAL_OK)
            UART_SaveRecvData(UART_PORT2, &u8RecvDat, 1);
    }

#if ENABLE_GPS_CHECK_INT
    bGPS_GetInt = TRUE;
#endif

    /* USER CODE END USART2_IRQn 1 */
}

/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void)
{
    /* USER CODE BEGIN USART3_IRQn 0 */
    uint8_t u8RecvDat;

    /* USER CODE END USART3_IRQn 0 */
    HAL_UART_IRQHandler(&huart3);
    /* USER CODE BEGIN USART3_IRQn 1 */
    // If the interrupt source is ORE
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(&huart3);
    }
    // If the interrupt source is RXNE
    else if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE))
    {
        if (HAL_UART_Receive(&huart3, &u8RecvDat, 1, 10) == HAL_OK)
        {
            UART_SaveRecvData(UART_PORT3, &u8RecvDat, 1);

            // printf("%c", u8RecvDat);
        }
    }

    /* USER CODE END USART3_IRQn 1 */
}

/**
 * @brief This function handles SDIO global interrupt.
 */
void SDIO_IRQHandler(void)
{
    /* USER CODE BEGIN SDIO_IRQn 0 */

    /* USER CODE END SDIO_IRQn 0 */
    HAL_SD_IRQHandler(&hsd);
    /* USER CODE BEGIN SDIO_IRQn 1 */

    /* USER CODE END SDIO_IRQn 1 */
}

/**
 * @brief This function handles UART4 global interrupt.
 */
void UART4_IRQHandler(void)
{
    /* USER CODE BEGIN UART4_IRQn 0 */
    uint8_t u8RecvDat;

    /* USER CODE END UART4_IRQn 0 */
    HAL_UART_IRQHandler(&huart4);
    /* USER CODE BEGIN UART4_IRQn 1 */
    // If the interrupt source is ORE
    if (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(&huart4);
    }
    // If the interrupt source is RXNE
    else if (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_RXNE))
    {
        if (HAL_UART_Receive(&huart4, &u8RecvDat, 1, 10) == HAL_OK)
        {
            UART_SaveRecvData(UART_PORT4, &u8RecvDat, 1);

            // printf("%c", u8RecvDat);
        }
    }

    /* USER CODE END UART4_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream3 global interrupt.
 */
void DMA2_Stream3_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */

    /* USER CODE END DMA2_Stream3_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_sdio_rx);
    /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

    /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream6 global interrupt.
 */
void DMA2_Stream6_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

    /* USER CODE END DMA2_Stream6_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_sdio_tx);
    /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

    /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
 * @brief This function handles USB OTG FS interrupt.
 */
void OTG_FS_IRQHandler(void)
{
    usbh_isr(&usbh_core);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    switch ((int)huart->Instance)
    {
    case (int)USART2:
        bUART_DMA_Wait[GPS_COMPORT_DEF] = FALSE;

        break;

    default:
        break;
    }
}
/* USER CODE END 1 */
