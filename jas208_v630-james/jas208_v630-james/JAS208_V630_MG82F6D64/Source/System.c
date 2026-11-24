//============================================================================//
// Include
//============================================================================//
#include "System.h"

#include <Intrins.h>
#include <stdio.h> // for printf
#include <string.h>

#include "ADC_Driver.h"
#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "IAP_Driver.h"
#include "I2C_Driver.h"
#include "IO_Driver.h"
#include "main.h"
#include "REG_MG82F6D64.H"
#include "RTC_Driver.h"
#include "Timer_Driver.h"
#include "UART_Driver.h"
#include "Utility.h"
#include "MG82F6D64_PW_DRV.h"
#include "Power.h"
//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
xdata SYS_PARAM SysParam;
xdata u8 u8MCU_WDT_Tick = 0;
bit bExitSlowMode = FALSE;
xdata u8 u8CheckInterrupt = 0;
//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  System clock init.
// @param  None.
// @retval None.
//============================================================================//
void InitClock(void)
{
    CLK_SetCKCON3(WDTCS_ILRCO | FWKP_NORMAL_120uS | MCKDO_MCK_DIV_1);
#if (MCU_SYSCLK == 11059200)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // SysClk=11.0592MHz CpuClk=11.0592MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1);

#else
    // SysClk=11.0592MHz CpuClk=5.5296MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1);
#endif
#endif

#if (MCU_SYSCLK == 12000000)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // SysClk=12MHz CpuClk=12MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1);

#else
    // SysClk=12MHz CpuClk=6MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1);
#endif
#endif

#if (MCU_SYSCLK == 22118400)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // SysClk=22.1184MHz CpuClk=22.1184MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx4, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4 | OSCIn_IHRCO);
#else
    // SysClk=22.1184MHz CpuClk=11.0592MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx4, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4 | OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK == 24000000)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // SysClk=24MHz CpuClk=24MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx4, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4 | OSCIn_IHRCO);
#else
    // SysClk=24MHz CpuClk=12MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx4, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4 | OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK == 29491200)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // Cpuclk high speed
    CLK_SetCpuCLK_HighSpeed();
    // SysClk=29.491200MHz CpuClk=29.491200MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X533 | OSCIn_IHRCO);
#else
    // SysClk=29.491200MHz CpuClk=14.7456MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X533 | OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK == 32000000)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // Cpuclk high speed
    CLK_SetCpuCLK_HighSpeed();
    // SysClk=32MHz CpuClk=32MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X533 | OSCIn_IHRCO);
#else
    // SysClk=32MHz CpuClk=16MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx5.33, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X533 | OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK == 36000000)
#if (MCU_CPUCLK == MCU_SYSCLK)
    // Cpuclk high speed
    CLK_SetCpuCLK_HighSpeed();
    // CKMIx6,x8,x12
    CLK_SetCKM_x6x8x12();
    // SysClk=36MHz CpuClk=36MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx6, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4_X6 | OSCIn_IHRCO);
#else
    // CKMIx6,x8,x12
    CLK_SetCKM_x6x8x12();
    // SysClk=36MHz CpuClk=18MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx6, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X4_X6 | OSCIn_IHRCO);
#endif
#endif

#if (MCU_SYSCLK == 44236800)
    // SysClk=44.2368MHz CpuClk=22.1184MHz
    CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx8, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X8 | OSCIn_IHRCO);
#endif

#if (MCU_SYSCLK == 48000000)
    // SysClk=48MHz CpuClk=24MHz
    CLK_SetCKCON0(IHRCO_12MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_1 | ENABLE_CKM | CKM_OSCIN_DIV_2);
    DelayXus(100);
    // IHRCO, MCK=CKMIx8, OSCin=IHRCO
    CLK_SetCKCON2(ENABLE_IHRCO | MCK_CKMI_X8 | OSCIn_IHRCO);
#endif

    // P60 output MCK/4
    // CLK_P60OC_MCKDiv4();
}

//============================================================================//
// @brief  Change system clock to slower speed.
// @param  tMode: PWR_MODE_SLOW_CLK: .
//                PWR_MODE_SUB_CLK: 32kHz.
// @retval None.
//============================================================================//
bit System_SetSlowMode(PWR_ModeTypeDef tMode)
{
    bit bRes = FALSE;

    switch (tMode)
    {
    case PWR_MODE_SLOW_CLK:
        CLK_SetCKCON2(ENABLE_IHRCO | MCK_OSCin | OSCIn_IHRCO);
        CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_2 | SYSCLK_MCKDO_DIV_128 | DISABLE_CKM); // 86.4kHz
        // CLK_SetCKCON3(WDTCS_ILRCO|FWKP_NORMAL_120uS|MCKDO_MCK_DIV_1);
        bRes = TRUE;
        break;
    case PWR_MODE_SUB_CLK:
        CLK_SetCKCON2(ENABLE_IHRCO | MCK_OSCin | OSCIn_ILRCO);
        CLK_SetCKCON0(IHRCO_110592MHz | CPUCLK_SYSCLK_DIV_1 | SYSCLK_MCKDO_DIV_1 | DISABLE_CKM); // 32kHz
        bRes = TRUE;
        break;
    default:
        break;
    }

    return bRes;
}

//============================================================================//
// @brief  System init.
// @param  None.
// @retval None.
//============================================================================//
void System_Init(void)
{
    InitClock();
    InitPort();
    TWI0_Init(1); // i2c slave
#if ENABLE_TWI1 == 1
    TWI1_Init(1); // i2c master
#endif
    Timer0_Init();
    UART_BufInit();
    UART0_Init(1);
#if ENABLE_UART1 == 1 && UART_NUM_MAX > 1
    UART1_Init(1);
#endif
#if ENABLE_UART2 == 1 && UART_NUM_MAX > 2
    UART2_Init(1);
#endif
#if ENABLE_UART3 == 1 && UART_NUM_MAX > 3
    UART3_Init(1);
#endif
    // #if ENABLE_RTC == 1
    RTC_Init();
// #endif
#if ENABLE_ADC0
    ADC_Init(0);
    // DMA_ConfigADC0_2_XRAM();
#endif
#if ENABLE_INT0
    INT0_Init();
    // INT_EnINT0();
#endif
#if ENABLE_INT1
    INT1_Init();
    // INT_EnINT1();
#endif
#if ENABLE_INT2
    INT2_Init();
    // INT_EnINT2();
#endif
#if ENABLE_INT3
    INT3_Init();
    // INT_EnINT3();
#endif
    INT_EnAll(); // Enable global interrupt

    Power_SettingDefault();
}

//============================================================================//
// @brief  Deinit system befor entering a non-freezing low-power mode.
// @param  None.
// @retval None.
//============================================================================//
void System_DeInit(void)
{
    // INT_DisAll();
    //  TWI0
    TWI0_DeInit();
#if ENABLE_TWI1 == 1
    TWI1_DeInit();
#endif
    // T0
    Timer0_DeInit();
// RTC
#if !EN_RTC_WKUP
    INT_DisSF();
    RTC_DeInit();
#endif
    // UART0
    UART0_DeInit();
// UART1
#if ENABLE_UART1 == 1 && UART_NUM_MAX > 1
    UART1_DeInit();
#endif
// UART2
#if ENABLE_UART2 == 1 && UART_NUM_MAX > 2
    UART2_DeInit();
#endif
// UART3
#if ENABLE_UART3 == 1 && UART_NUM_MAX > 3
    UART3_DeInit();
#endif
// ADC
#if ENABLE_ADC0
    ADC_DeInit();
    DMA_Suspend();
#endif
}

//============================================================================//
// @brief  Set system power mode to low power mode.
// @param  u8Mode: 1: Idle mode, 2: Power down mode.
// @retval None.
//============================================================================//
void SetPowerMode(u8 u8Mode)
{
    bit bRes;

    switch (u8Mode)
    {
    case PWR_MODE_IDLE:
        ResetWDT();
#if EN_RTC_WKUP
        NV_Set.tPedWkup.u32Counter = 0;
#endif
        SysParam.u8PwrMode = PWR_MODE_IDLE;
        UART_SendStr(0, "Go to Idle\r\n");
        SaveNV();
        INT_DisAll();
        TM_DisableT0();
        RTC_Disable();
        DeInitWDT();
#if ENABLE_INT0 == 1
        INT_EnINT0();
#endif
#if ENABLE_INT1 == 1
        INT_EnINT1();
#endif
#if ENABLE_INT2 == 1
        INT_EnINT2();
#endif
#if ENABLE_INT3 == 1
        INT_EnINT3();
#endif
        INT_EnAll(); // Enable global interrupt
        POW_SetMode_Idle();
        TM_EnableT0();
        RTC_Enable();
        InitWDT();
        break;
    case PWR_MODE_PD:
        ResetWDT();
#if EN_RTC_WKUP
        NV_Set.tPedWkup.u32Counter = 0;
#endif
        SysParam.u8PwrMode = PWR_MODE_PD;
        UART_SendStr(0, "Go to Power Down\r\n");
        SaveNV();
        INT_DisAll();
#if ENABLE_INT0 == 1
        INT_EnINT0();
#endif
#if ENABLE_INT1 == 1
        INT_EnINT1();
#endif
#if ENABLE_INT2 == 1
        INT_EnINT2();
#endif
#if ENABLE_INT3 == 1
        INT_EnINT3();
#endif
        INT_EnAll(); // Enable global interrupt
        POW_SetMode_PD();
        break;
    case PWR_MODE_SLOW_CLK:
    case PWR_MODE_SUB_CLK:
        ResetWDT();
        // if(N_ACC_IN == 1)
        {
#if ENABLE_INT0 == 1
            INT_EnINT0();
#endif
#if ENABLE_INT1 == 1
            INT_EnINT1();
#endif
#if ENABLE_INT2 == 1
            INT_EnINT2();
#endif
#if ENABLE_INT3 == 1
            INT_EnINT3();
#endif
            if (u8Mode == PWR_MODE_SLOW_CLK)
                UART_SendStr(0, "Go to Power Mode Slow \r\n");
            else
                UART_SendStr(0, "Go to Power Mode Sub-Clock \r\n");
            u8ACC_ON_Cnt = 0;
#if EN_RTC_WKUP
            NV_Set.tPedWkup.u32Counter = 0;
#endif
            SysParam.u8PwrMode = u8Mode;
            SaveNV();
            System_DeInit();
            bRes = System_SetSlowMode((PWR_ModeTypeDef)u8Mode);
            if (bRes == FALSE)
            {
                bExitSlowMode = TRUE;
            }
        }
        break;
    default:
        break;
    }
}
