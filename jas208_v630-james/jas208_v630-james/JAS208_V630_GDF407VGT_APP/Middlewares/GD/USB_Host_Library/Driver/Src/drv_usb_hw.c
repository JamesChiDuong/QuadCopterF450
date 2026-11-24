/*!
    \file    gd32f4xx_usb_hw.c
    \brief   this file implements the board support package for the USB host library

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

#include "stm32fxxx_hal_user.h"
#include "drv_usb_hw.h"
#include "main.h"
#include "USB_Conf.h"


/*!
    \brief      configure USB GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_gpio_config (void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		
	  __HAL_RCC_SYSCFG_CLK_ENABLE();
	
#ifdef USE_USB_FS

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_FS GPIO Configuration
    PA11     ------> USB_OTG_FS_DM
    PA12     ------> USB_OTG_FS_DP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#elif defined(USE_USB_HS)

    #ifdef USE_ULPI_PHY
        rcu_periph_clock_enable(RCU_GPIOA);
        rcu_periph_clock_enable(RCU_GPIOB);
        rcu_periph_clock_enable(RCU_GPIOC);
        rcu_periph_clock_enable(RCU_GPIOH);
        rcu_periph_clock_enable(RCU_GPIOI);

        /* ULPI_STP(PC0) GPIO pin configuration */
        gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
        gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_0);

        /* ULPI_CK(PA5) GPIO pin configuration */
        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_5);

        /* ULPI_NXT(PH4) GPIO pin configuration */
        gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
        gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_4);

        /* ULPI_DIR(PI11) GPIO pin configuration */
        gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
        gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_11);

        /* ULPI_D1(PB0), ULPI_D2(PB1), ULPI_D3(PB10), ULPI_D4(PB11) \
           ULPI_D5(PB12), ULPI_D6(PB13) and ULPI_D7(PB5) GPIO pin configuration */
        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, \
                        GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_12 |\
                        GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_0);
        gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, \
                        GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_12 |\
                        GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_0);

        /* ULPI_D0(PA3) GPIO pin configuration */
        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, GPIO_PIN_3);

        gpio_af_set(GPIOC, GPIO_AF_10, GPIO_PIN_0);
        gpio_af_set(GPIOH, GPIO_AF_10, GPIO_PIN_4);
        gpio_af_set(GPIOI, GPIO_AF_10, GPIO_PIN_11);
        gpio_af_set(GPIOA, GPIO_AF_10, GPIO_PIN_5 | GPIO_PIN_3);
        gpio_af_set(GPIOB, GPIO_AF_10, GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_12 |\
                                       GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_0);
    #endif /* USE_ULPI_PHY */

#endif /* USE_USBFS */
}

/*!
    \brief      configure USB clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_rcu_config(void)
{
  #ifdef USE_USB_FS

	/* Peripheral clock enable */
	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();

#elif defined(USE_USB_HS)

    #ifdef USE_EMBEDDED_PHY

        #ifndef USE_IRC48M
            rcu_pll48m_clock_config(RCU_PLL48MSRC_PLLQ);

            rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);
        #else
            /* enable IRC48M clock */
            rcu_osci_on(RCU_IRC48M);

            /* wait till IRC48M is ready */
            while (SUCCESS != rcu_osci_stab_wait(RCU_IRC48M)) {
            }

            rcu_ck48m_clock_config(RCU_CK48MSRC_IRC48M);
        #endif /* USE_IRC48M */

    #elif defined(USE_ULPI_PHY)
        rcu_periph_clock_enable(RCU_USBHSULPI);
    #endif /* USE_EMBEDDED_PHY */

    rcu_periph_clock_enable(RCU_USBHS);
#endif /* USB_USBFS */
}

/*!
    \brief      configure USB global interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_intr_config (void)
{
     /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 10, 0);

#ifdef USE_USB_FS
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
#elif defined(USE_USB_HS)
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
#endif /* USE_USBFS */
}

/*!
    \brief      drives the VBUS signal through GPIO
    \param[in]  state: VBUS states
    \param[out] none
    \retval     none
*/
void usb_vbus_drive (uint8_t state)
{
	  if(state == 0)
        HAL_GPIO_WritePin(DRIVE_VBUS_GPIO_Port, DRIVE_VBUS_Pin, GPIO_PIN_RESET);
		else
			  HAL_GPIO_WritePin(DRIVE_VBUS_GPIO_Port, DRIVE_VBUS_Pin, GPIO_PIN_SET);
}

/*!
    \brief      configures the GPIO for the VBUS
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_vbus_config (void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(DRIVE_VBUS_GPIO_Port, DRIVE_VBUS_Pin, GPIO_PIN_RESET);
	
	  GPIO_InitStruct.Pin = DRIVE_VBUS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(DRIVE_VBUS_GPIO_Port, &GPIO_InitStruct);

    HAL_Delay(200);
}
