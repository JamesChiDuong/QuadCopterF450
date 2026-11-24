//============================================================================//
// User Includes
//============================================================================//

#include "HW_Config.h"
#include "main.h"

//============================================================================//
// User Typedef Variables
//============================================================================//

typedef struct
{
    GPIO_TypeDef *gpioPORT;

    uint16_t u16PIN;

    uint32_t u32SPEED;

    uint32_t u32MODE;

    uint32_t u32PULL;

} GPIO_HandleTypeDef;

//============================================================================//
// User Variables
//============================================================================//

#if HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SPI
static const GPIO_HandleTypeDef gpioHandleSet[] =
    {
        // MOTOR_PWR_EN
        {EN_MOTOR_PWR_GPIO_Port, EN_MOTOR_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // MOTOR_PWR_EN

        // 3V3 IO ENABLE
        {GPIOC, GPIO_PIN_6, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // IO_3V3_EN

        // Buzzer
        {BUZZER_GPIO_Port, BUZZER_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // BUZZER

        // MX25L SPI1
        {SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // MX25L_CS

        // SD_SPI2
        {EN_SD_PWR_GPIO_Port, EN_SD_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // SD_PWR_EN
        {SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_PULLDOWN},     // SD_DETECT
        {SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // SD_SPI_CS

        // PCF I2C2
        {I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // I2C2_SCL
        {I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},     // I2C2_SDA

        // Speed count
        {SPEED_IN_GPIO_Port, SPEED_IN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // SPEED_IN

        // RPM count
        {RPM_IN_GPIO_Port, RPM_IN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // RPM_IN

        // EXTIO
        {GPIOA, GPIO_PIN_0, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},          // ACC
        {N_IO1_GPIO_Port, N_IO1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO1
        {N_IO2_GPIO_Port, N_IO2_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO2
        {N_IO3_GPIO_Port, N_IO3_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO3
        {N_IO4_GPIO_Port, N_IO4_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO4
        {N_IO5_GPIO_Port, N_IO5_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO5
        {N_IO6_GPIO_Port, N_IO6_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO6
        {N_IO7_GPIO_Port, N_IO7_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO7
        {N_IO8_GPIO_Port, N_IO8_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // IO8

        // BAT Detect
        {BAT_DETECT_GPIO_Port, BAT_DETECT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_PULLUP}, // BAT_DETECT

        // Printer
        {EN_PRN_PWR_GPIO_Port, EN_PRN_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // PRN_PWR_EN
        {P_EMPTY_GPIO_Port, P_EMPTY_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},           // PRN_PA_EXIST
        {P_STB1_GPIO_Port, P_STB1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},         // P_STB1
        {P_STB6_GPIO_Port, P_STB6_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},         // P_STB6
        {P_MTA_P_GPIO_Port, P_MTA_P_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTA2
        {P_MTA_M_GPIO_Port, P_MTA_M_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTA1
        {P_MTB_P_GPIO_Port, P_MTB_P_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTB2
        {P_MTB_M_GPIO_Port, P_MTB_M_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTB1
        {P_DI_GPIO_Port, P_DI_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},             // P_DI
        {P_CLK_GPIO_Port, P_CLK_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},           // P_CLK
        {N_P_LAT_GPIO_Port, N_P_LAT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_LAT

        // LCD
        {N_LCD_CS_GPIO_Port, N_LCD_CS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // LCD_CS
        {N_LCD_RST_GPIO_Port, N_LCD_RST_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // LCD_RST
        {LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // LCD_A0
        {LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},     // LCD_SCL
        {LCD_SIO_GPIO_Port, LCD_SIO_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},     // LCD_SIO
        {N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // LCD_BL

        // IO INPUT
        {KEY1_GPIO_Port, KEY1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY1
        {KEY2_GPIO_Port, KEY2_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY2
        {KEY3_GPIO_Port, KEY3_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY3
        {KEY4_GPIO_Port, KEY4_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY4
        {KEY5_GPIO_Port, KEY5_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}  // KEY5
};
#elif HW_TYPE == HW_JAS208 && SD_INTERFACE == SD_SDIO
static const GPIO_HandleTypeDef gpioHandleSet[] =
    {
        // MOTOR_PWR_EN
        {EN_MOTOR_PWR_GPIO_Port, EN_MOTOR_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // MOTOR_PWR_EN

        // 8051 Int
        {N_8051_INT_GPIO_Port, N_8051_INT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // N_8051_INT

        // 3V3 IO ENABLE
        {GPIOC, GPIO_PIN_6, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // IO_3V3_EN

        // Buzzer
        {BUZZER_GPIO_Port, BUZZER_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // BUZZER

        // 4G
        {GSM_WAKEUP_GPIO_Port, GSM_WAKEUP_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // GSM_WAKEUP

        // CAN1
        {CAN1_TX_GPIO_Port, CAN1_TX_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // CAN1_TX
        {CAN1_RX_GPIO_Port, CAN1_RX_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // CAN1_RX
        {CAN_DETECT_GPIO_Port, CAN_DETECT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // CAN_DETECT

        // MX25L SPI1
        {SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // MX25L_CS

        // SD_SPI2
        {EN_SD_PWR_GPIO_Port, EN_SD_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // SD_PWR_EN
        {SD_DETECT_GPIO_Port, SD_DETECT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_PULLDOWN},   // SD_DETECT

        // I2C2
        {I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // I2C2_SCL
        {I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},     // I2C2_SDA

        // I2C1
        {I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL}, // I2C1_SCL
        {I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},     // I2C1_SDA

        // Speed count
        {SPEED_IN_GPIO_Port, SPEED_IN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // SPEED_IN

        // RPM count
        {RPM_IN_GPIO_Port, RPM_IN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // RPM_IN

        // EXTIO
        {N_ACC_IN_GPIO_Port, N_ACC_IN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // N_ACC
        {N_IO1_GPIO_Port, N_IO1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO1
        {N_IO2_GPIO_Port, N_IO2_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO2
        {N_IO3_GPIO_Port, N_IO3_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO3
        {N_IO4_GPIO_Port, N_IO4_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO4
        {N_IO5_GPIO_Port, N_IO5_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO5
        {N_IO6_GPIO_Port, N_IO6_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO6
        {N_IO7_GPIO_Port, N_IO7_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO7
        {N_IO8_GPIO_Port, N_IO8_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},       // IO8

        // BAT Detect
        {CHG_STATE_GPIO_Port, CHG_STATE_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},   // CHG_STATE
        {BAT_DETECT_GPIO_Port, BAT_DETECT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_PULLUP}, // BAT_DETECT

        // Watchdog Enable
        {WDT_EN_GPIO_Port, WDT_EN_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // WDT_EN

        // Printer
        {EN_PRN_PWR_GPIO_Port, EN_PRN_PWR_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // PRN_PWR_EN
        {P_EMPTY_GPIO_Port, P_EMPTY_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL},           // PRN_PA_EXIST
        {P_STB1_GPIO_Port, P_STB1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},         // P_STB1
        {P_STB6_GPIO_Port, P_STB6_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},         // P_STB6
        {P_MTA_P_GPIO_Port, P_MTA_P_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTA2
        {P_MTA_M_GPIO_Port, P_MTA_M_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTA1
        {P_MTB_P_GPIO_Port, P_MTB_P_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTB2
        {P_MTB_M_GPIO_Port, P_MTB_M_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_MTB1
        {P_DI_GPIO_Port, P_DI_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},             // P_DI
        {P_CLK_GPIO_Port, P_CLK_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},           // P_CLK
        {N_P_LAT_GPIO_Port, N_P_LAT_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // P_LAT

        // LCD
        {N_LCD_CS_GPIO_Port, N_LCD_CS_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // LCD_CS
        {N_LCD_RST_GPIO_Port, N_LCD_RST_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL}, // LCD_RST
        {LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},       // LCD_A0
        {LCD_SCL_GPIO_Port, LCD_SCL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},     // LCD_SCL
        {LCD_SIO_GPIO_Port, LCD_SIO_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},     // LCD_SIO
        {N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL},   // LCD_BL

        // IO INPUT
        {KEY1_GPIO_Port, KEY1_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY1
        {KEY2_GPIO_Port, KEY2_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY2
        {KEY3_GPIO_Port, KEY3_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY3
        {KEY4_GPIO_Port, KEY4_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}, // KEY4
        {KEY5_GPIO_Port, KEY5_Pin, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT, GPIO_NOPULL}  // KEY5
};
#endif // HW_TYPE

bool bPrintfEn = FALSE;

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Configure 3V3_EN GPIO for the MCU.
// @param  None.
// @retval None.
//============================================================================//
void HW_3V3EN_GPIO_Init(void)
{
    HW_PBInit(IO_3V3_EN, FALSE);

    HW_PBClr(IO_3V3_EN);
}

//============================================================================//
// @brief  Initialize a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
//         bEnEXTI: Enable corresponding interrupt or not.
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
    if (bEnEXTI == true)
    {
    }
}

//============================================================================//
// @brief  Deinitialize a GPIO pin.
// @param  gpioDef: GPIO pin based on GPIO_PosTypeDef format.
//         bEnEXTI: Enable corresponding interrupt or not.
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
    if (bEnEXTI == true)
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

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    u32Tmp = (uint32_t)&(RTC->BKP0R);
    u32Tmp += (u32BKP_Reg * 4U);

    *(__IO uint32_t *)u32Tmp = (uint32_t)u32Dat;

    HAL_PWR_DisableBkUpAccess();
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

    u32Tmp = (uint32_t)&(RTC->BKP0R);
    u32Tmp += (u32BKP_Reg * 4U);

    return (*(__IO uint32_t *)u32Tmp);
}
