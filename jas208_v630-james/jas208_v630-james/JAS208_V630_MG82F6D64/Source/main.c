/*********************************************************************
    Project: MG82F6D64-DEMO
            MG82F6D64 LQFP64_V10 EV Board (TH210A)
            CpuCLK=12MHz, SysCLK=12MHz
    Description:
            TWSI0 (SCL/P25, SDA/P23) Master
            TWSI1 (SCL/P42, SDA/P43) Master
    Note:

    Creat time:
    Modify:

*********************************************************************/
#define _MAIN_C

//============================================================================//
// Include
//============================================================================//
#include "main.h"

#include <Absacc.h>
#include <Intrins.h>
#include <stdio.h> // for printf
#include <string.h>

#include "ADC_Driver.h"
#include "API_Macro_MG82F6D64.H"
#include "API_Uart_BRGRL_MG82F6D64.H"
#include "Config.h"
#include "I2C_Driver.h"
#include "IAP_Driver.h"
#include "IO_Driver.h"
#include "MG82F6D64_INT_DRV.h"
#include "REG_MG82F6D64.H"
#include "RTC_Driver.h"
#include "System.h"
#include "Timer_Driver.h"
#include "Type.h"
#include "UART_Driver.h"
#include "Utility.h"
#include "Version.h"

//============================================================================//
// Define
//============================================================================//
#define SHUTDOWN_TICK_MAX 3600U

//============================================================================//
// Variable
//============================================================================//
char cFW_Ver[64] = {0};
xdata u8 u8ACC_ON_Cnt = 0;
xdata WkupSourceTypeDef tWkupSrc = 0;
xdata ShutdownTypeDef tShutdown = {0};

//============================================================================//
// Function Prototype
//============================================================================//
void Slave_CmdProc(void);

//============================================================================//
// Function
//============================================================================//
void main()
{
    xdata u8 u8i;
    xdata u8 u8UartDataNum = 0;
    xdata char cBuf[64] = {0};
    // xdata char cTestData[256] = "The Megawin MG82FG5C64 is an 8051 based Full Static CMOS controller max XTAL 25MHz 256 Bytes On-chip RAM (DATA)"
    //														" 3840 Bytes On-chip XRAM (XDATA) ROM Lock, 32 KB Flash Memory, 44/59 I/O lines, 24 Interrupts sources with 4 levels, 5 Timers/Counters";
    xdata u8 u8SecCnt = 0;
    xdata char u8Message[100] = {0};
    static bit bShutdownFlag = FALSE;
    static xdata u16 u16RstDelay = 0;
    static bit bWDT_CurrState, bWDT_LastState = 0;

#if EN_110592 == 1 || EN_221184 == 1 || EN_294912 == 1 || EN_442368 == 1
    CLK_SelIHRCO110592MHz();
#endif
    IAP_LB();
    LoadNV();
    System_Init();
    InitDateTime();
    InitWDT();
    //
    // for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
    //	UART_SetBaudRate(u8i);
    sprintf(cFW_Ver, "%s-%d.%d.%d-%04d/%02d/%02d",
            (char *)FW_NAME,
            (int)VER_MAJOR,
            (int)VER_MINOR,
            (int)VER_BUILD,
            (int)FW_DATE_Y,
            (int)FW_DATE_M,
            (int)FW_DATE_D);
    if (PCON1 & SWRF)
    {
        PCON1 |= SWRF;
        sprintf(u8Message, "%s START FROM SW RESET\r\n", FW_NAME);
    }
    else
        sprintf(u8Message, "%s START\r\n", FW_NAME);
    UART_SendStr(0, u8Message);
    memset(&SysParam, 0, sizeof(SYS_PARAM));
    SysParam.u8EnableWDT = 1;
    SysParam.u8WakeupSource = 0;
    SysParam.u8PwrMode = PWR_MODE_NORMAL;
    SysParam.u16Tick1S = 0;
    u8TWI1RegAddr = I2C_CMD_UART_TX; // 0x01;
    if (NV_Set.u8MCU_WDT_Reload > 3)
        u8MCU_WDT_Tick = NV_Set.u8MCU_WDT_Reload;
    else
        u8MCU_WDT_Tick = 10;

#if ENABLE_ADC0
    if (SysParam.u8EnableADC)
    {
        DMA_Start();
        ADC_SoftStart();
    }
#endif
    USB_PROTECT_SET = 1;
    SendWkupPulse(MCU_WKUP_PIN, 1);
    // SendRstPulse(N_RESET_MCU_PIN, 1);

    while (1)
    {
        ResetWDT();

        if ((bExitSlowMode == TRUE) &&
            ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
             (SysParam.u8PwrMode == PWR_MODE_SUB_CLK)))
        // if(bExitSlowMode == TRUE)
        {
#if EN_110592 == 1 || EN_221184 == 1 || EN_294912 == 1 || EN_442368 == 1
            CLK_SelIHRCO110592MHz();
#else
            CLK_SelIHRCO12MHz();
#endif
            System_Init();
#if ENABLE_ADC0
            if (SysParam.u8EnableADC)
            {
                DMA_Start();
                ADC_SoftStart();
            }
#endif
            SysParam.u8PwrMode = PWR_MODE_NORMAL;
            UART_SendStr(0, (char *)"EXIT LOW POWER MODE\r\n");
            bExitSlowMode = FALSE;
        }

#if EN_RTC_WKUP
        RTC_Wkup_Handle();
#endif

        if ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
            (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
        {
#if !ENABLE_INT0
            if (N_ACC_IN == 0)
            {
                u8ACC_ON_Cnt++;
                if (u8ACC_ON_Cnt >= 5)
                {
                    u8ACC_ON_Cnt = 0;
                    SysParam.u8WakeupSource = WKUP_SRC_INT0;
                    bExitSlowMode = TRUE;
                }
            }
            else
            {
                u8ACC_ON_Cnt = 0;
            }
#endif
            if ((KEY_1 == 0) && (KEY_5 == 0))
            {
                if (NV_Set.u8KeyResetEn)
                {
                    SysParam.u8WakeupSource = WKUP_SRC_HW_RST;
                    bExitSlowMode = TRUE;
                    u16RstDelay = 20; // Delay this function to avoid multiple reset
                }
            }
            DelayXus(500);
            continue;
        }

        if (u8Tick20Hz == 1)
        {
            u8Tick20Hz = 0;

            if (u16RstDelay == 0)
            {
                if ((KEY_1 == 0) && (KEY_5 == 0))
                {
                    if (NV_Set.u8KeyResetEn)
                    {
                        SendRstPulse(N_RESET_MCU_PIN, 1);
                    }
                    u16RstDelay = 20; // Delay this function for 1 second to avoid multiple reset
                }
            }
            else
            {
                u16RstDelay--;
            }
            if (WDT_EN_FUC == FALSE)
            {
                NV_Set.u8EnableMCU_WDT = FALSE;
                u8MCU_WDT_Tick = NV_Set.u8MCU_WDT_Reload;
            }
            else
            {
                NV_Set.u8EnableMCU_WDT = TRUE;
            }
            if ((NV_Set.u8EnableMCU_WDT == TRUE))
            {
                bWDT_CurrState = WDT_EN;

                if (bWDT_CurrState != bWDT_LastState)
                {
                    u8MCU_WDT_Tick = NV_Set.u8MCU_WDT_Reload;
                    // UART_SendStr(0, (char *)"u8MCU_WDT_Reload: Set reload here\r\n");
                    bWDT_LastState = bWDT_CurrState;
                }
            }
        }
        if (u8Tick10Hz == 1)
        {
            u8Tick10Hz = 0;
        }
        if (u8Tick5Hz == 1)
        {
            u8Tick5Hz = 0;
        }
        if (u8Tick2Hz == 1)
        {
            u8Tick2Hz = 0;
#if ENABLE_ADC0 == 1
            GetADC(0); // AIN0
            DelayXms(2);
#endif
        }
        if (u8Tick1Hz == 1)
        {
            SysParam.u16Tick1S++;
            // IncDateTime();
            u8Tick1Hz = 0;
            if (SysParam.u8Debug == 1)
                PrintDateTime();
            if (SysParam.u8Debug == 2)
                PrintSysInfo();
            if (NV_Set.u8EnableMCU_WDT)
            {
                u8MCU_WDT_Tick--;
                sprintf(u8Message, "u8MCU_WDT_Tick=%u\r\n", u8MCU_WDT_Tick);
                UART_SendStr(0, u8Message);

                if (u8MCU_WDT_Tick == 0)
                {
                    UART_SendStr(0, "WDT TIMEOUT! RESET MCU\r\n");
                    // SendRstPulse(N_RESET_MCU_PIN, 1);
                    u8MCU_WDT_Tick = NV_Set.u8MCU_WDT_Reload;
                }
            }
            if ((tShutdown.u8Request == TRUE) && (SysParam.u8PwrMode == PWR_MODE_NORMAL))
            {
                tShutdown.u16Tick++;
                if (tShutdown.u16Tick >= tShutdown.u16Timeout)
                {
                    tShutdown.u8Request = FALSE;
                    tShutdown.u16Tick = 0;
                    bShutdownFlag = TRUE;
                }
                if (N_ACC_IN == 0)
                {
                    u8ACC_ON_Cnt++;
                    if (u8ACC_ON_Cnt >= 2)
                    {
                        u8ACC_ON_Cnt = 0;
                        tShutdown.u8Request = FALSE;
                        SendWkupPulse(MCU_WKUP_PIN, 1);
                        UART_SendStr(0, (char *)"STOP SHUTDOWN\r\n");
                    }
                }
            }
            else if ((tShutdown.u8Request == TRUE) && (SysParam.u8PwrMode != PWR_MODE_NORMAL))
            {
                tShutdown.u8Request = FALSE;
            }
        }
        if (SysParam.u8WakeupSource != WKUP_SRC_NONE)
        {
            DelayXms(100); // Avoid multiple interrupts
#if ENABLE_INT0 == 1
            INT_DisINT0();
#endif
#if ENABLE_INT1 == 1
            INT_DisINT1();
#endif
#if ENABLE_INT2 == 1
            INT_DisINT2();
#endif
#if ENABLE_INT3 == 1
            INT_DisINT3();
#endif
            // Send wakeup pulse to host, 1ms duration
            SendWkupPulse(MCU_WKUP_PIN, 1);
            memset(u8Message, 0x00, sizeof(u8Message));
            sprintf(u8Message, "WAKEUP=%d\r\n", (int)SysParam.u8WakeupSource);
            UART_SendStr(0, u8Message);
            tWkupSrc = SysParam.u8WakeupSource;
            SysParam.u8WakeupSource = WKUP_SRC_NONE;
        }
        for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
        {
            if (u8CmdReceived[u8i] == 1)
            { // u8Command
                // memset(u8Message, 0x00, sizeof(u8Message));
                // sprintf(u8Message, "CMD: %s\r\n", u8Command[u8i]);
                // UART_SendStr(0, u8Message);
                DecipherCommand(&u8Command[u8i][1]); // skip the first character '$'
                u8CmdReceived[u8i] = 0;
            }
        }
        if (bTWI0RxFlag == TRUE) // i2c packet is received, i2c master block write
        {
            bTWI0RxFlag = FALSE;

            Slave_CmdProc();
        }
        if (bShutdownFlag == TRUE)
        {
            bShutdownFlag = FALSE;
            SaveNV();
            SetPowerMode(tShutdown.u8PwrMode); // Go to pwr down mode
#if ENABLE_INT1
            INT_DisINT1();
#endif
#if ENABLE_INT2
            INT_DisINT2();
#endif
            PWR_EN = 0; // Disable main power
        }
    }
}

//============================================================================//
// @brief  Process write TWI0 command from MCU.
// @param  None.
// @retval None.
//============================================================================//
void Slave_CmdProc(void)
{
    xdata I2C_PKT *pI2CPkt;
    xdata u8 u8i;
    xdata char cMsg[80] = {0};

    memmove(u8TWI0UartBuf, TWI0RxBuf, I2C_PKT_SIZE); // echo, debug test
    memset(cMsg, 0x00, sizeof(cMsg));
    sprintf(cMsg, "TWI0 CMD: %d %d %d %d ",
            (int)u8TWI0UartBuf[0],
            (int)u8TWI0UartBuf[1],
            (int)u8TWI0UartBuf[2],
            (int)u8TWI0UartBuf[3]);
    UART_SendStr(0, cMsg);
    // if((u8TWI0UartBuf[1] > 0) && (u8TWI0UartBuf[1] < I2C_DATA_SIZE))
    // if((u8TWI0UartBuf[1] > 0) && (u8TWI0UartBuf[1] < 30))
    //{
    //	for(u8i = 0; u8i < u8TWI0UartBuf[1]; u8i++)
    //	{
    //		memset(cMsg, 0x00, sizeof(cMsg));
    //		sprintf(cMsg, "%d ", (int)u8TWI0UartBuf[I2C_PKT_HEADER_SIZE + u8i]);
    //		UART_SendStr(0, cMsg);
    //	}
    // }
    UART_SendStr(0, (char *)"\r\n");
    pI2CPkt = (I2C_PKT *)u8TWI0UartBuf;
    switch (pI2CPkt->u8Command)
    {
    case I2C_CMD_UART_TX: // I2C to UART,pI2CPkt->u8Channel=0-3
        UART_SendData(pI2CPkt->u8Param1, pI2CPkt->u8Data, pI2CPkt->u8DataNum);
        // UART_SendStr(0, (char *)"TWI0 UART TX OK\r\n");
        break;
    case I2C_CMD_UART_BR_SET:
    {
        xdata u8Port, u8BR;

        if (pI2CPkt->u8DataNum != 1)
            break;
        u8Port = pI2CPkt->u8Param1;
        u8BR = pI2CPkt->u8Data[0];
        if (u8Port >= UART_NUM_MAX)
            break;
        NV_Set.u8Baud[u8Port] = u8BR;
        SaveNV();
        UART_SetBaudRate(u8Port);
        // UART_SendStr(0, (char *)"TWI0 BR SET OK\r\n");
        break;
    }
    case I2C_CMD_USBPROTECT_SET:
    {
        u8 u8Val;
        u8Val = pI2CPkt->u8Param1;
        if (u8Val > 1)
            break;

        USB_PROTECT_SET = u8Val;
        break;
    }
    case I2C_CMD_UART_RST:
    {
        xdata u8Port;

        u8Port = pI2CPkt->u8Param1;
        if (u8Port >= UART_NUM_MAX)
            break;
        UART_ClearData(u8Port);
        // UART_SendStr(0, (char *)"TWI0 UART RST OK\r\n");
        break;
    }
    case I2C_CMD_UART_RST_ALL:
    {
        for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
            UART_ClearData(u8i);
        // UART_SendStr(0, (char *)"TWI0 UART RST ALL OK\r\n");
        break;
    }
    case I2C_CMD_DT_SET:
        DateTimeTemp.u8Year = pI2CPkt->u8Data[0];
        DateTimeTemp.u8Mon = pI2CPkt->u8Data[1];
        DateTimeTemp.u8Day = pI2CPkt->u8Data[2];
        DateTimeTemp.u8Hour = pI2CPkt->u8Data[3];
        DateTimeTemp.u8Min = pI2CPkt->u8Data[4];
        DateTimeTemp.u8Sec = pI2CPkt->u8Data[5];
        // UART_SendStr(0, (char *)"TWI0 DATETIME OK\r\n");
        bRTC_SetTimeFlag = TRUE;
        break;
    case I2C_CMD_IO_SET: // SET IO,
    {
        u8 u8Pin, u8Val;

        u8Pin = pI2CPkt->u8Param1;
        if (u8Pin > 3)
            break;
        u8Val = pI2CPkt->u8Param2;
        if (u8Val > 1)
            break;
        switch (u8Pin)
        {
        case 0:
            SysParam.u8IO_Out[0] = u8Val;
            IO_EN0 = u8Val;
            break;
        case 1:
            SysParam.u8IO_Out[1] = u8Val;
            IO_EN1 = u8Val;
            break;
        case 2:
            SysParam.u8IO_Out[2] = u8Val;
            IO_EN2 = u8Val;
            break;
        case 3:
            SysParam.u8IO_Out[3] = u8Val;
            IO_EN3 = u8Val;
            break;
        default:
            break;
        }
        // UART_SendStr(0, (char *)"TWI0 IO OK\r\n");
        break;
    }
    case I2C_CMD_MCU_WDT_SET:
        if (pI2CPkt->u8Param1 > 0)
        {
            if (pI2CPkt->u8Param2 > 0)
                NV_Set.u8MCU_WDT_Reload = pI2CPkt->u8Param2;
            u8MCU_WDT_Tick = NV_Set.u8MCU_WDT_Reload;
            NV_Set.u8EnableMCU_WDT = 1;
            UART_SendStr(0, (char *)"TWI0 MCU WDT OK\r\n");
        }
        else
        {
            NV_Set.u8EnableMCU_WDT = 0;
        }
        SaveNV();
        // UART_SendStr(0, (char *)"TWI0 MCU WDT OK\r\n");
        break;
    case I2C_CMD_MCU_ADC_SET:
        if (pI2CPkt->u8Param1 == 1)
        {
#if ENABLE_ADC0
            if (SysParam.u8EnableADC == 0)
            {
                SysParam.u8EnableADC = 1;
                DMA_Start();
                ADC_SoftStart();
            }
#endif
        }
        else if (pI2CPkt->u8Param1 == 0)
            SysParam.u8EnableADC = 0;
        // UART_SendStr(0, (char *)"TWI0 ADC OK\r\n");
        break;
    case I2C_CMD_SW_RESET:
        // UART_SendStr(0, (char *)"TWI0 MCU RST OK\r\n");
        SaveNV();
        POW_ResetToAP();
        break;
    case I2C_CMD_PWR_MODE:
        if ((pI2CPkt->u8Param1 == 0) || (pI2CPkt->u8Param1 > PWR_MODE_SUB_CLK))
            break;
        UART_SendStr(0, (char *)"TWI0 MCU PWR MODE OK\r\n");
        SaveNV();
        SetPowerMode(pI2CPkt->u8Param1); // 1:IDLE, 2:DOWN, 3: SLOW, 4: SUB-CLOCK
        break;
    case I2C_CMD_KEY_RST:
        if (pI2CPkt->u8Param1 == 0)
            NV_Set.u8KeyResetEn = 0;
        else
            NV_Set.u8KeyResetEn = 1;
        break;
    case I2C_CMD_SHUTDOWN:
        if (pI2CPkt->u8DataNum != 2)
            break;
        if (pI2CPkt->u8Param1 > PWR_MODE_SUB_CLK)
            break;
        UART_SendStr(0, (char *)"TWI0 MCU SHUTDOWN OK\r\n");
        tShutdown.u8PwrMode = pI2CPkt->u8Param1;
        tShutdown.u16Timeout = pI2CPkt->u8Data[1] * 256 + pI2CPkt->u8Data[0];
        tShutdown.u16Tick = 0;
        // memset(cMsg, 0x00, sizeof(cMsg));
        // sprintf(cMsg, "SHUTDOWN DELAY: %d\r\n", (int)tShutdown.u16Timeout);
        // UART_SendStr(0, cMsg);
        if (tShutdown.u16Timeout > SHUTDOWN_TICK_MAX)
            break;
        if (tShutdown.u8PwrMode == PWR_MODE_NORMAL)
            tShutdown.u8Request = FALSE;
        else
        {
            if (SysParam.u8PwrMode == PWR_MODE_NORMAL)
                tShutdown.u8Request = TRUE;
        }
        break;
    case I2C_CMD_RTC_WKUP:
    {
#if EN_RTC_WKUP
        xdata u8 u8Mode;
        xdata u32 u32Timeout;
#if EN_RTC_SRC_32000
        double dTimeout;
#endif

        u8Mode = pI2CPkt->u8Param1;
        if (u8Mode > (u8)RTC_WKUP_PERIODIC)
            break;
        if (u8Mode == RTC_WKUP_ALM)
        {
            if (pI2CPkt->u8DataNum != 13)
                break;
            if ((pI2CPkt->u8Data[1] > 12) || (pI2CPkt->u8Data[2] > 31) ||
                (pI2CPkt->u8Data[7] > 12) || (pI2CPkt->u8Data[8] > 31))
                break;
            DateTimeTemp.u8Year = pI2CPkt->u8Data[0];
            DateTimeTemp.u8Mon = pI2CPkt->u8Data[1];
            DateTimeTemp.u8Day = pI2CPkt->u8Data[2];
            DateTimeTemp.u8Hour = pI2CPkt->u8Data[3];
            DateTimeTemp.u8Min = pI2CPkt->u8Data[4];
            DateTimeTemp.u8Sec = pI2CPkt->u8Data[5];
            NV_Set.tAlarm.tAlarmTime.u8Year = pI2CPkt->u8Data[6];
            NV_Set.tAlarm.tAlarmTime.u8Mon = pI2CPkt->u8Data[7];
            NV_Set.tAlarm.tAlarmTime.u8Day = pI2CPkt->u8Data[8];
            NV_Set.tAlarm.tAlarmTime.u8Hour = pI2CPkt->u8Data[9];
            NV_Set.tAlarm.tAlarmTime.u8Min = pI2CPkt->u8Data[10];
            NV_Set.tAlarm.tAlarmTime.u8Sec = pI2CPkt->u8Data[11];
            NV_Set.tAlarm.u8Repeat = pI2CPkt->u8Data[12];
            NV_Set.tAlarm.u8Enable = pI2CPkt->u8Param2;
            // memset(cMsg, 0x00, sizeof(cMsg));
            // sprintf(cMsg, "TIME: %02d%02d%02d%02d%02d%02d\r\n",
            //         (int)DateTimeTemp.u8Year,
            //				(int)DateTimeTemp.u8Mon,
            //				(int)DateTimeTemp.u8Day,
            //				(int)DateTimeTemp.u8Hour,
            //				(int)DateTimeTemp.u8Min,
            //				(int)DateTimeTemp.u8Sec);
            // UART_SendStr(0, cMsg);
            // memset(cMsg, 0x00, sizeof(cMsg));
            // sprintf(cMsg, "ALARM: %02d%02d%02d%02d%02d%02d\r\n",
            //         (int)NV_Set.tAlarm.tAlarmTime.u8Year,
            //				(int)NV_Set.tAlarm.tAlarmTime.u8Mon,
            //				(int)NV_Set.tAlarm.tAlarmTime.u8Day,
            //				(int)NV_Set.tAlarm.tAlarmTime.u8Hour,
            //				(int)NV_Set.tAlarm.tAlarmTime.u8Min,
            //				(int)NV_Set.tAlarm.tAlarmTime.u8Sec);
            // UART_SendStr(0, cMsg);
            bRTC_SetTimeFlag = TRUE;
            SaveNV();
            PWR_MAIN_EN = 1;
            // UART_SendStr(0, (char *)"TWI0 RTC ALM OK\r\n");
        }
        else if (u8Mode == RTC_WKUP_PERIODIC)
        {
            if (pI2CPkt->u8DataNum != 4)
                break;
            u32Timeout = ((u32)pI2CPkt->u8Data[3] << 24) |
                         ((u32)pI2CPkt->u8Data[2] << 16) |
                         ((u32)pI2CPkt->u8Data[1] << 8) |
                         (u32)pI2CPkt->u8Data[0];
            if (u32Timeout > RTC_WKUP_COUNTER_MAX)
                break;
            if ((pI2CPkt->u8Param2 > 0) && (u32Timeout == 0))
                break;
#if EN_RTC_SRC_32000
            dTimeout = (double)u32Timeout;
            NV_Set.tPedWkup.u32Timeout = (u32)(dTimeout * 32768 / 32000);
#else
            NV_Set.tPedWkup.u32Timeout = u32Timeout;
#endif
            // NV_Set.tPedWkup.u32Counter = 0;
            NV_Set.tPedWkup.u8Enable = pI2CPkt->u8Param2;
            // memset(cMsg, 0x00, sizeof(cMsg));
            // sprintf(cMsg, "WKUP PED: %d\r\n", (int)NV_Set.tPedWkup.u32Timeout);
            // UART_SendStr(0, cMsg);
            SaveNV();
            PWR_MAIN_EN = 1;
            // UART_SendStr(0, (char *)"TWI0 RTC WKUP OK\r\n");
        }
#endif
        break;
    }
    default:
        break;
    }
    // UART_SendStr(0, "\r\n");
}
