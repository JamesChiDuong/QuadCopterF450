//============================================================================//
// Include
//============================================================================//
#include "interrupt.h"

#include <Intrins.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ADC_Driver.h"
#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "main.h"
#include "I2C_Driver.h"
#include "IAP_Driver.h"
#include "IO_Driver.h"
#include "REG_MG82F6D64.H"
#include "RTC_Driver.h"
#include "System.h"
#include "Timer_Driver.h"
#include "UART_Driver.h"
#include "Utility.h"

//============================================================================//
// Variable
//============================================================================//
xdata u8 u8SecCnt = 0;
xdata u8 u8SecCompCnt = 0;

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  DMA interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_ADC0 == 1
void DMA_IntHandler(void) interrupt INT_VECTOR_DMA
{
    u8 u8i;

    _push_(SFRPI);
    SFRPI = 0;
    for (u8i = 0; u8i < ADC_DATA_CNT; u8i++)
    {
        tADC_Val[u8i] = tADC_Buf[u8i];
    }
    bADC_CpltFlag = TRUE;
    DMACR0 = DMACR0 & (~DCF0); // Clear DCF0 FLAG
    _pop_(SFRPI);
}
#endif

//============================================================================//
// @brief  SF interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
void SF_IntHandler(void) interrupt INT_VECTOR_SF
{
    if ((PCON1 & WDTF) != 0) // watchdog timer
    {
        PCON1 = WDTF; // Clear WDT Interrupt flag
    }
    else if ((PCON1 & BOF0) != 0)
    {
        PCON1 = BOF0; // Clear BOF0 Interrupt flag
    }
    else if ((PCON1 & BOF1) != 0)
    {
        PCON1 = BOF1; // Clear BOF1 Interrupt flag
    }
    else if ((PCON1 & RTCF) != 0) // RTC
    {
        PCON1 = RTCF; // Clear RTC Interrupt flag

        if (bRTC_SetTimeFlag == TRUE)
        {
            bRTC_SetTimeFlag = FALSE;

            memcpy(&DateTime, &DateTimeTemp, sizeof(DATE_TIME));
        }

        IncDateTime();

#if EN_RTC_WKUP
        // if((NV_Set.tPedWkup.u8Enable > 0) &&
        //	 ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
        //		(SysParam.u8PwrMode == PWR_MODE_SUB_CLK)))
        if (NV_Set.tPedWkup.u8Enable > 0)
        {
            NV_Set.tPedWkup.u32Counter++;
        }
#endif

// If the clock source is 32kHz the counter frequency will be 3/128 slower than real time's frequency
// So every 125 counts the counter must be compensated with 3 counts to match the real time
#if EN_RTC_SRC_32000
        u8SecCnt++;

        // Increase 1 count every 40, 80, and 120 counts to compensate frequency difference
        if ((u8SecCnt == 40) || (u8SecCnt == 80) || (u8SecCnt == 120))
        {
            // u8SecCompCnt++;
            // if(u8SecCompCnt == 100) // Skip the 100th compensation every 100 compensations due to ILRCO inaccuracy
            //	u8SecCompCnt = 0;
            // else
            IncDateTime();
        }
        if (u8SecCnt >= 125)
        {
            u8SecCnt = 0;
        }
#endif

        if (SysParam.u8PwrMode == PWR_MODE_PD)
        {
            DelayXus(5); // Delay at least 4 uS before go back to power down mode

            // #if ENABLE_INT0 == 1
            // INT_EnINT0();
            // #endif
            // #if ENABLE_INT1 == 1
            // INT_EnINT1();
            // #endif
            // #if ENABLE_INT2 == 1
            // INT_EnINT2();
            // #endif
            // #if ENABLE_INT3 == 1
            // INT_EnINT3();
            // #endif
            POW_SetMode_PD();
        }
    }
}

//============================================================================//
// @brief  INT0 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT0
void INT0_IntHandler(void) interrupt INT_VECTOR_INT0
{
    SysParam.u8WakeupSource = WKUP_SRC_INT0;

    // Set power mode back to normal
    if ((SysParam.u8PwrMode == PWR_MODE_PD) ||
        (SysParam.u8PwrMode == PWR_MODE_IDLE))
    {
        INT_DisINT0();
        SysParam.u8PwrMode = PWR_MODE_NORMAL;
    }
    else if ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
             (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
    {
        INT_DisINT0();
        bExitSlowMode = TRUE;
    }
}
#endif

//============================================================================//
// @brief  INT1 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT1
void INT1_IntHandler(void) interrupt INT_VECTOR_INT1
{
    SysParam.u8WakeupSource = WKUP_SRC_INT1;

    // Set power mode back to normal
    if ((SysParam.u8PwrMode == PWR_MODE_PD) ||
        (SysParam.u8PwrMode == PWR_MODE_IDLE))
    {
        INT_DisINT1();
        SysParam.u8PwrMode = PWR_MODE_NORMAL;
    }
    else if ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
             (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
    {
        INT_DisINT1();
        bExitSlowMode = TRUE;
    }
}
#endif

//============================================================================//
// @brief  INT2 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT2
void INT2_IntHandler(void) interrupt INT_VECTOR_INT2
{
    SysParam.u8WakeupSource = WKUP_SRC_INT2;

    // Set power mode back to normal
    if ((SysParam.u8PwrMode == PWR_MODE_PD) ||
        (SysParam.u8PwrMode == PWR_MODE_IDLE))
    {
        INT_DisINT2();
        SysParam.u8PwrMode = PWR_MODE_NORMAL;
    }
    else if ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
             (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
    {
        INT_DisINT2();
        bExitSlowMode = TRUE;
    }
}
#endif

//============================================================================//
// @brief  INT3 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT3
void INT3_IntHandler(void) interrupt INT_VECTOR_INT3
{
    // SysParam.u8WakeupSource = WKUP_SRC_INT3;

    // Set power mode back to normal
    if ((SysParam.u8PwrMode == PWR_MODE_PD) ||
        (SysParam.u8PwrMode == PWR_MODE_IDLE))
    {
        INT_DisINT3();
        SysParam.u8PwrMode = PWR_MODE_NORMAL;
    }
    else if ((SysParam.u8PwrMode == PWR_MODE_SLOW_CLK) ||
             (SysParam.u8PwrMode == PWR_MODE_SUB_CLK))
    {
        INT_DisINT3();
        bExitSlowMode = TRUE;
    }
}
#endif

//============================================================================//
// @brief  Timer0 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
void T0_IntHandler(void) interrupt INT_VECTOR_T0
{
    TH0 = TIMER_12T_1ms_TH;
    TL0 = TIMER_12T_1ms_TL;
    if (u8TWI0OvTime != 0)
        u8TWI0OvTime--;
    if (u8TWI1OvTime != 0)
        u8TWI1OvTime--;
    u16TickTime = (u16TickTime + 1) % 1000;
    if (u16TickTime == 0) // 0-999
        u8Tick1Hz = 1;
    if (u16TickTime % 500 == 0)
        u8Tick2Hz = 1;
    if (u16TickTime % 200 == 0)
        u8Tick5Hz = 1;
    if (u16TickTime % 100 == 0)
        u8Tick10Hz = 1;
    if (u16TickTime % 50 == 0)
        u8Tick20Hz = 1;
}

//============================================================================//
// @brief  TWI0 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
void TWI0_IntHandler(void) interrupt INT_VECTOR_TWI0
{
    xdata u8 u8Ch;
    xdata u8 u8i, u8Num;
    xdata u16 u16Idx;
    xdata int nNum;
    static xdata u16 u16RxLen = I2C_PKT_SIZE;
    // xdata char cBuf[30] = {0};

    _push_(SFRPI);
    SFR_SetPage(0);
    // I2C block write
    // S -> address+W(S_A) -> register(S_A) -> data[0](S_A) -> ... data[N-1](S_A) -> P
    // I2C block read
    // S -> address+W(S_A) -> register(S_A) -> SR > address+R -> data[0](M_A) -> ... data[N-1](M_NA) -> P
    if ((SISTA == 0x80) || (SISTA == 0x88)) // slave receiver
    {                                       // 0x80: DATA has been received. ACK has been returned.
        // 0x88: DATA has been received. NAK has been returned.
        u8TWI0OvTime = TWI_OV_TIME_MAX; // restore TWI0 overtime
        if (bTWI0ADFlag == 0)
        { // device address
            u8TWI0DevAddr = SIDAT;
            // memset(cBuf, 0x00, sizeof(cBuf));
            // sprintf(cBuf, "TWI0 RX:%d\r\n", (int)u8TWI0DevAddr);
            // UART_SendStr(0, cBuf);
            if (u8TWI0DevAddr == TWI0_SLAVE_ADDRESS)
            {
                bTWI0ADFlag = 1; // The next byte is Data, AD=addressed
                u8TWI0RxInx = 0;
                u16RxLen = I2C_PKT_SIZE;
                u8TWI0RegAddr = INVALID_REG_ADDR;
                // UART_SendStr(0, "TWI0 RX Init\r\n");
            }
        }
        else // bTWI0ADFlag=1, addressed=1
        {    // RegAddr = bCommand
            if (u8TWI0RegAddr == INVALID_REG_ADDR)
            {                          //
                u8TWI0RegAddr = SIDAT; // u8TWI0RegAddr can determine block read or block write
                u8TWI0BlockRead = (u8TWI0RegAddr & 0x80) ? 1 : 0;
                u8TWI0BlockWrite = (u8TWI0RegAddr & 0x80) ? 0 : 1;
            }
            else
            {                                   // block write
                TWI0RxBuf[u8TWI0RxInx] = SIDAT; // Slave received data into buffer
                if (u8TWI0RxInx == 1)           // Check packet data num
                {
                    if (TWI0RxBuf[u8TWI0RxInx] <= I2C_DATA_SIZE)
                    {
                        // Calculate packet length
                        u16RxLen = I2C_PKT_HEADER_SIZE + TWI0RxBuf[u8TWI0RxInx];
                        // memset(cBuf, 0x00, sizeof(cBuf));
                        // sprintf(cBuf, "TWI0 RX LEN:%d\r\n", (int)u16RxLen);
                        // UART_SendStr(0, cBuf);
                    }
                }
                // memset(cBuf, 0x00, sizeof(cBuf));
                // sprintf(cBuf, "TWI0 RX%d:%d\r\n", (int)u8TWI0RxInx, (int)TWI0RxBuf[u8TWI0RxInx]);
                // UART_SendStr(0, cBuf);
                u8TWI0RxInx++;
            }
            if (u8TWI0BlockRead == 1)
            {
                switch (u8TWI0RegAddr)
                { // master block read
                case I2C_CMD_UART_CHK:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_UART_CHK;
                    TWI0TxBuf[1] = UART_NUM_MAX * 2; // bDataNum
                    for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
                    {
                        nNum = __UART_GET_DATA_SIZE(u8i);
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE + u8i * 2] = (u8)(nNum % 256);
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE + u8i * 2 + 1] = (u8)(nNum / 256);
                    }
                    break;
                case I2C_CMD_UART0_RX:
                case I2C_CMD_UART1_RX:
                    // case I2C_CMD_UART2_RX:
                    // case I2C_CMD_UART3_RX:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = u8TWI0RegAddr;            // bCommand
                    u8Ch = u8TWI0RegAddr - I2C_CMD_UART0_RX; // 0-3
                    nNum = (u16UartRxIn[u8Ch] + u16UART_BufSize[u8Ch] - u16UartRxOut[u8Ch]) % u16UART_BufSize[u8Ch];
                    if (nNum > I2C_DATA_SIZE)
                        nNum = I2C_DATA_SIZE;
                    u8Num = (u8)nNum;
                    TWI0TxBuf[1] = u8Num; // bDataNum
                    TWI0TxBuf[2] = u8Ch;  // bChannel
                    u16Idx = u16UartRxOut[u8Ch];
                    for (u8i = I2C_PKT_HEADER_SIZE; u8i < I2C_PKT_HEADER_SIZE + u8Num; u8i++)
                    {
                        TWI0TxBuf[u8i] = pu8UART_Buf[u8Ch][u16Idx];
                        u16Idx = (u16Idx + 1) % u16UART_BufSize[u8Ch];
                    }
                    break;
                case I2C_CMD_UART_BR:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = u8TWI0RegAddr; // bCommand
                    TWI0TxBuf[1] = UART_NUM_MAX;  // Data num
                    for (u8i = 0; u8i < UART_NUM_MAX; u8i++)
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE + u8i] = NV_Set.u8Baud[u8i]; // Baudrate
                    break;
                case I2C_CMD_UART0_BR:
                case I2C_CMD_UART1_BR:
                    // case I2C_CMD_UART2_BR:
                    // case I2C_CMD_UART3_BR:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = u8TWI0RegAddr;            // bCommand
                    TWI0TxBuf[1] = 1;                        // Data num
                    u8Ch = u8TWI0RegAddr - I2C_CMD_UART0_BR; // 0-3
                    if (u8Ch >= UART_NUM_MAX)
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE] = 0;
                    else
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE] = NV_Set.u8Baud[u8Ch]; // Baudrate
                    break;
                case I2C_CMD_DT_GET:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_DT_GET; // bCommand
                    TWI0TxBuf[1] = 6;              // bDataNum
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = DateTime.u8Year;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 1] = DateTime.u8Mon;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 2] = DateTime.u8Day;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 3] = DateTime.u8Hour;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 4] = DateTime.u8Min;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 5] = DateTime.u8Sec;
                    break;
                case I2C_CMD_IO_GET:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_IO_GET; // bCommand
                    TWI0TxBuf[1] = 1;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = P2;
                    break;
                case I2C_CMD_MCU_WDT_GET:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_MCU_WDT_GET; // bCommand
                    TWI0TxBuf[1] = 2;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = NV_Set.u8EnableMCU_WDT;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 1] = NV_Set.u8MCU_WDT_Reload;
                    break;
                case I2C_CMD_ADC_MODE:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_ADC_MODE; // bCommand
                    TWI0TxBuf[1] = 1;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = SysParam.u8EnableADC;
                    break;
                case I2C_CMD_ADC_GET:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_ADC_GET; // bCommand
                    TWI0TxBuf[1] = 2 * ADC_CHANNEL_NUM;
                    for (u8i = 0; u8i < ADC_CHANNEL_NUM; u8i++)
                    {
                        if (u8ADC_Busy[u8i] == 0)
                            tADC_Val_Temp[u8i] = tADC_Val[u8i];
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE + u8i * 2] = tADC_Val_Temp[u8i].BUF[0];     // Byte low
                        TWI0TxBuf[I2C_PKT_HEADER_SIZE + u8i * 2 + 1] = tADC_Val_Temp[u8i].BUF[1]; // Byte high
                    }
                    break;
                case I2C_CMD_GET_VER:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_GET_VER; // bCommand
                    TWI0TxBuf[1] = strlen(cFW_Ver);
                    memcpy(&TWI0TxBuf[I2C_PKT_HEADER_SIZE], cFW_Ver, strlen(cFW_Ver));
                    break;
                case I2C_CMD_GET_WKUP_SRC:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_GET_WKUP_SRC; // bCommand
                    TWI0TxBuf[1] = 1;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = tWkupSrc;
                    tWkupSrc = 0;
                    break;
                case I2C_CMD_GET_KEY_RST:
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_GET_KEY_RST; // bCommand
                    TWI0TxBuf[1] = 1;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = NV_Set.u8KeyResetEn;
                    break;
                case I2C_CMD_RTC_WKUP_GET:
#if EN_RTC_WKUP
                    memset(TWI0TxBuf, 0x00, sizeof(TWI0TxBuf));
                    TWI0TxBuf[0] = I2C_CMD_RTC_WKUP_GET; // bCommand
                    TWI0TxBuf[1] = 13;                   // bDataNum
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE] = NV_Set.tAlarm.u8Enable;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 1] = NV_Set.tAlarm.u8Repeat;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 2] = NV_Set.tAlarm.tAlarmTime.u8Year;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 3] = NV_Set.tAlarm.tAlarmTime.u8Mon;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 4] = NV_Set.tAlarm.tAlarmTime.u8Day;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 5] = NV_Set.tAlarm.tAlarmTime.u8Hour;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 6] = NV_Set.tAlarm.tAlarmTime.u8Min;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 7] = NV_Set.tAlarm.tAlarmTime.u8Sec;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 8] = NV_Set.tPedWkup.u8Enable;
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 9] = (u8)(NV_Set.tPedWkup.u32Timeout);
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 10] = (u8)(NV_Set.tPedWkup.u32Timeout >> 8);
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 11] = (u8)(NV_Set.tPedWkup.u32Timeout >> 16);
                    TWI0TxBuf[I2C_PKT_HEADER_SIZE + 12] = (u8)(NV_Set.tPedWkup.u32Timeout >> 24);
#endif
                    break;
                default:
                    break;
                } // end of switch
            }
            else
            { // u8TWI0BlockWrite
                if (u8TWI0RxInx >= u16RxLen)
                {
                    bTWI0RxFlag = TRUE; // I2C packet is received, I2C block write
                    u8TWI0RxInx = 0;
                    u16RxLen = I2C_PKT_SIZE;
                    u8TWI0OvTime = 0; // buffer is full,set overtime to zero
                    bTWI0ADFlag = 0;
                }
            }
        }
    }
    else if ((SISTA == 0x60) || (SISTA == 0x68)) // slave receiver
    {                                            // 0x60,0x68: Own SLA+W(master->slave) has been received. ACK has been returned
        u8TWI0OvTime = TWI_OV_TIME_MAX;          // restore TWI0 overtime
        bTWI0ADFlag = 1;                         // The next byte is Data, AD=addressed
        u8TWI0RxInx = 0;
        u16RxLen = I2C_PKT_SIZE;
        u8TWI0RegAddr = INVALID_REG_ADDR;
        // UART_SendStr(0, "TWI0 RX Start\r\n");
    }
    //---------------------------------------
    // slave transmitter, master block read
    //---------------------------------------
    else if ((SISTA == 0xA8) || (SISTA == 0xB0))
    { // 0xB0,0xA8: Own SLA+R(master<-slave) has been received. ACK has been returned
        // slave Tx, block read
        u8TWI0TxInx = 0; // Set buffer index to zero
        goto _I2C_SET_SIDAT;
    }
    else if ((SISTA == 0xB8) || (SISTA == 0xC0) || (SISTA == 0xC8)) // slave transmitter
    {
        // 0xB8:  Data byte in SIDAT has been transmitted ACK has been received
        // 0xC0:  Data byte or Last data byte in SIDAT has been transmitted Not ACK has been received
        // 0xC8:  Last Data byte in SIDAT has been transmitted ACK has been received
    _I2C_SET_SIDAT:
        u8TWI0OvTime = TWI_OV_TIME_MAX; // restore TWI0 overtime
        SIDAT = TWI0TxBuf[u8TWI0TxInx]; // Load data to send
        if ((TWI0TxBuf[0] >= I2C_CMD_UART0_RX) &&
            (TWI0TxBuf[0] <= I2C_CMD_UART3_RX)) // If the command is reading UART RX buffer
        {
            // If the current byte is data byte
            // increase UART buffer RX out index by 1
            if ((u8TWI0TxInx >= I2C_PKT_HEADER_SIZE) &&
                (SISTA != 0xC0))
            {
                u8Ch = TWI0TxBuf[0] - I2C_CMD_UART0_RX;
                if (__UART_GET_DATA_SIZE(u8Ch) > 0)
                    u16UartRxOut[u8Ch] = (u16UartRxOut[u8Ch] + 1) % u16UART_BufSize[u8Ch];
            }
        }
        u8TWI0TxInx++;
        if (u8TWI0TxInx >= sizeof(TWI0TxBuf)) // I2C data is transmitted
        {
            u8TWI0TxInx = 0;
            u8TWI0OvTime = 0; // buffer is full,set overtime to zero
        }
    }
    SICON = SICON | (AA);  // Set AA
    SICON = SICON & (~SI); // clear TWI0 Interrupt flag

    _pop_(SFRPI);
}

//============================================================================//
// @brief  UART0 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
void UART0_IntHandler(void) interrupt INT_VECTOR_UART0
{
    xdata char c;
    xdata u8 u8Ch = 0;

    _push_(SFRPI);
    SFR_SetPage(0);
    if (TI0)
    {
        // TI0 = 0;
    }
    if (RI0)
    {
        RI0 = 0;
        c = S0BUF;
        if (c == '$')
        {
            u8CmdNdx[u8Ch] = 0;
        }
        u8Command[u8Ch][u8CmdNdx[u8Ch]] = c;
        u8CmdNdx[u8Ch] = (u8CmdNdx[u8Ch] + 1) % CMD_SIZE_M1;
        if (c == LF)
        {
            if (u8Command[u8Ch][0] == '$')
            { // valid command start by $
                u8Command[u8Ch][u8CmdNdx[u8Ch]] = 0;
                u8ConnType = u8Ch;
                u8CmdReceived[u8Ch] = 1;
            }
        }
        if (((u16UartRxIn[u8Ch] + u16UART_BufSize[u8Ch] - u16UartRxOut[u8Ch]) % u16UART_BufSize[u8Ch]) < (u16UART_BufSize[u8Ch] - 1))
        {
            pu8UART_Buf[u8Ch][u16UartRxIn[u8Ch]] = c; // S0BUF;
            u16UartRxIn[u8Ch] = (u16UartRxIn[u8Ch] + 1) % u16UART_BufSize[u8Ch];
        }
    }
    _pop_(SFRPI);
}

//============================================================================//
// @brief  UART1 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_UART1
void UART1_IntHandler(void) interrupt INT_VECTOR_UART1
{
    xdata char c;
    xdata u8 u8Ch = 1;

    _push_(SFRPI);
    SFR_SetPage(1);
    if (TI1)
    {
        // TI1 = 0;
    }
    if (RI1)
    {
        RI1 = 0;
        c = S1BUF;
        if (c == '$')
        {
            u8CmdNdx[u8Ch] = 0;
        }
        u8Command[u8Ch][u8CmdNdx[u8Ch]] = c;
        u8CmdNdx[u8Ch] = (u8CmdNdx[u8Ch] + 1) % CMD_SIZE_M1;
        if (c == LF)
        {
            if (u8Command[u8Ch][0] == '$')
            { // valid command start by $
                u8Command[u8Ch][u8CmdNdx[u8Ch]] = 0;
                u8ConnType = u8Ch;
                u8CmdReceived[u8Ch] = 1;
            }
        }
        if (((u16UartRxIn[u8Ch] + u16UART_BufSize[u8Ch] - u16UartRxOut[u8Ch]) % u16UART_BufSize[u8Ch]) < (u16UART_BufSize[u8Ch] - 1))
        {
            pu8UART_Buf[u8Ch][u16UartRxIn[u8Ch]] = c;
            u16UartRxIn[u8Ch] = (u16UartRxIn[u8Ch] + 1) % u16UART_BufSize[u8Ch];
        }
    }
    _pop_(SFRPI);
}
#endif

//============================================================================//
// @brief  UART2 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_UART2
void UART2_IntHandler(void) interrupt INT_VECTOR_UART2
{
    xdata char c;
    xdata u8 u8Ch = 2;

    _push_(SFRPI);
    SFR_SetPage(3);
    if (TI2)
    {
        // TI2 = 0;
    }
    if (RI2)
    {
        RI2 = 0;
        c = S2BUF;
        if (c == '$')
        {
            u8CmdNdx[u8Ch] = 0;
        }
        if (c == '$')
            u8CmdNdx[u8Ch] = 0;
        u8Command[u8Ch][u8CmdNdx[u8Ch]] = c;
        u8CmdNdx[u8Ch] = (u8CmdNdx[u8Ch] + 1) % CMD_SIZE_M1;
        if (c == LF)
        {
            if (u8Command[u8Ch][0] == '$')
            { // valid command start by $
                u8Command[u8Ch][u8CmdNdx[u8Ch]] = 0;
                u8ConnType = u8Ch;
                u8CmdReceived[u8Ch] = 1;
            }
        }
        if (((u16UartRxIn[u8Ch] + u16UART_BufSize[u8Ch] - u16UartRxOut[u8Ch]) % u16UART_BufSize[u8Ch]) < (u16UART_BufSize[u8Ch] - 1))
        {
            pu8UART_Buf[u8Ch][u16UartRxIn[u8Ch]] = c;
            u16UartRxIn[u8Ch] = (u16UartRxIn[u8Ch] + 1) % u16UART_BufSize[u8Ch];
        }
    }
    _pop_(SFRPI);
}
#endif

//============================================================================//
// @brief  UART3 interrupt handler.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_UART3
void UART3_IntHandler(void) interrupt INT_VECTOR_UART3
{
    xdata char c;
    xdata u8 u8Ch = 3;
    _push_(SFRPI);

    SFR_SetPage(4);
    if (TI3)
    {
        // TI3 = 0;
    }
    if (RI3)
    {
        RI3 = 0;
        c = S3BUF;
        if (c == '$')
        {
            u8CmdNdx[u8Ch] = 0;
        }
        if (c == '$')
            u8CmdNdx[u8Ch] = 0;
        u8Command[u8Ch][u8CmdNdx[u8Ch]] = c;
        u8CmdNdx[u8Ch] = (u8CmdNdx[u8Ch] + 1) % CMD_SIZE_M1;
        if (c == LF)
        {
            if (u8Command[u8Ch][0] == '$')
            { // valid command start by $
                u8Command[u8Ch][u8CmdNdx[u8Ch]] = 0;
                u8ConnType = u8Ch;
                u8CmdReceived[u8Ch] = 1;
            }
        }
        if (((u16UartRxIn[u8Ch] + u16UART_BufSize[u8Ch] - u16UartRxOut[u8Ch]) % u16UART_BufSize[u8Ch]) < (u16UART_BufSize[u8Ch] - 1))
        {
            pu8UART_Buf[u8Ch][u16UartRxIn[u8Ch]] = c;
            u16UartRxIn[u8Ch] = (u16UartRxIn[u8Ch] + 1) % u16UART_BufSize[u8Ch];
        }
    }
    _pop_(SFRPI);
}
#endif
