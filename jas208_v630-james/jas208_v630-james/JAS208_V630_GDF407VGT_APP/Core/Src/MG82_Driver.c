//============================================================================//
// User Includes
//============================================================================//

#include "MG82_Driver.h"

#include <string.h>

#include "CommFunc.h"
#include "DWT_Delay.h"
#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
#include "LCD_Key.h"
#endif
#include "main.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//
#if ENABLE_MG82
const UART_BaudRateDef_t tMG82_BR_Def[] =
    {
        MG82_EXT_UART0_BR,
        MG82_EXT_UART1_BR,
        MG82_EXT_UART2_BR,
        MG82_EXT_UART3_BR};

uint8_t u8MG82_ErrCnt = 0;

uint32_t u32MG82_ReportErrLastTick = 0;

bool bMG82_ReportErrFlag = FALSE;

bool bMG82_Stop = TRUE;

bool bMG82_RTC_EN = FALSE;

MG82_WkupSrcTypeDef u8WkupSrc = MG82_WKUP_SRC_NONE;
#endif

//============================================================================//
// User Functions
//============================================================================//
#if ENABLE_MG82
//============================================================================//
// @brief  Scan the I2C Bus for I2C device address.
// @param  None.
// @retval None.
//============================================================================//
void MG82_ScanBus(void)
{
    bool bRes;

    RTOSprintf("\r\n> Scan I2C Bus Start:\r\n");

    for (int i = 1; i < 128; i++)
    {
        bRes = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i << 1), 3, 5);

        if (bRes != HAL_OK)
        {
            RTOSprintf("_ ");
        }
        else if (bRes == HAL_OK)
        {
            RTOSprintf("0x%02X ", i);
        }

        if ((i % 20) == 0)
            RTOSprintf("\r\n");
    }

    RTOSprintf("\r\n> Scan I2C Bus End\r\n");
}

//============================================================================//
// @brief  Wakeup 8051 MCU.
// @param  None.
// @retval None.
//============================================================================//
void MG82_Wakeup(void)
{
    HW_PBClr(N_8051_INT);

    HAL_Delay(1);

    HW_PBSet(N_8051_INT);
}

//============================================================================//
// @brief  Init IC MG82
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_Init(void)
{
    bool bRes = FALSE;

    UART_BaudRateDef_t tUART_BR[MG82_UART_CHANNEL_MAX] = {(UART_BaudRateDef_t)0};
    uint8_t u8WDT_En, u8WDT_TimeOut;

    // if(HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)MG82_ADDR_I2C, 2, 5) == FALSE)
    //	return FALSE;

    bMG82_Stop = FALSE;

    for (int i = 0; i < 2; i++)
    {
        bRes = MG82_WKUP_Src(&u8WkupSrc);

        if (bRes)
            break;
    }

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 Init get wakeup source ERROR.");

        // return FALSE;
    }
    else
    {
        RTOSprintf("\r\n> MG82 wake up source: %d", u8WkupSrc);
    }

    // switch (u8WkupSrc)
    // {
    // case MG82_WKUP_SRC_4G_RI:
    // 	g_SysInfo.tVehicleState.b4GWkup = 1;
    // 	break;
    // case MG82_WKUP_SRC_G_SEN:

    // 	g_SysInfo.tVehicleState.bGSensorWkup = 1;
    // 	break;
    // default:
    // 	break;
    // }
    MG82_USBPROTECTION_Set(1);

    for (int i = 0; i < 2; i++)
    {
        bRes = MG82_WDG_GetCfg(&u8WDT_En, &u8WDT_TimeOut);

        if (bRes)
            break;
    }

    if (bRes == TRUE)
    {
        if (u8WDT_En == 0)
        {
            MG82_WDG_Config(1, 10);

            RTOSprintf("\r\n> MG82 WDT enabled");
        }
    }
    else
    {
        MG82_WDG_Config(1, 10);

        RTOSprintf("\r\n> MG82 WDT enabled");
    }

    for (int i = 0; i < 2; i++)
    {
        bRes = MG82_UART_GetBR_All(tUART_BR);

        if (bRes)
            break;
    }

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 Init get UART BR ERROR.");

        // return FALSE;
    }
    else
    {
        for (int i = 0; i < MG82_UART_CHANNEL_MAX; i++)
        {
            if (tUART_BR[i] != tMG82_BR_Def[i])
            {
                RTOSprintf("\r\n> MG82 UART%d change BR (%d->%d)", i, tUART_BR[i], tMG82_BR_Def[i]);

                for (int j = 0; j < 2; j++)
                {
                    bRes = MG82_UART_SetBR(i, tMG82_BR_Def[i]);

                    if (bRes)
                        break;
                }

                if (bRes == FALSE)
                    RTOSprintf("\r\n> MG82 UART%d change BR ERROR", tUART_BR[i]);
            }
        }
    }

    char cVerTemp[64] = {0};
    uint8_t u8Len;

    bRes = MG82_GetVer(cVerTemp, sizeof(cVerTemp) - 1, &u8Len);

    if (bRes)
    {
        RTOSprintf("\r\n> MG82 Version: %s", cVerTemp);
    }

    RTOSprintf("\r\n> MG82 Init OK.");

    return TRUE;
}

//============================================================================//
// @brief  Make the MG82 IC ready to communicate again.
// @param  None.
// @retval None.
//============================================================================//
void MG82_ReInit(void)
{
    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return;
    //}

    HAL_I2C_DeInit(&hi2c2);

    // Reconfig SCL and SDA
    HW_PBClr(I2C2_SCL);
    HW_PBInit(I2C2_SCL, FALSE);
    HW_PBInit(I2C2_SDA, FALSE);

    // Send restart pulses to the IC
    for (int i = 0; i < 9; i++)
    {
        HW_PBSet(I2C2_SCL);

        Delay_uS(10);

        HW_PBClr(I2C2_SCL);

        Delay_uS(10);
    }

    // Put SCL and SDA back to normal state
    HW_PBDeInit(I2C2_SCL, FALSE);
    HW_PBDeInit(I2C2_SDA, FALSE);

    USER_I2C2_Init();

    // if(bRTOS_En)
    //{
    //	osMutexRelease(mutex_i2c2_user);
    // }
}

//============================================================================//
// @brief  Deinit the MG82 communication interface.
// @param  None.
// @retval None.
//============================================================================//
void MG82_DeInit(void)
{
    HAL_I2C_DeInit(&hi2c2);
}

//============================================================================//
// @brief  If there is an external alarm setting request handle it.
// @param  None.
// @retval None.
//============================================================================//
void MG82_CheckExAlmSetting(void)
{
#if ENABLE_EX_WKUP && RTC_WKUP_SRC == RTC_WKUP_SRC_MG82
    // If ACC is ON disable alarm and periodic wakeup
    if ((g_Setting.tExAlarm.bEnableAlm == TRUE) && (g_SysInfo.tVehicleState.bACC_ON == TRUE))
    {
        ExternalAlarmTypeDef tSetting = {0};

        tSetting.bEnableAlm = FALSE;

        MG82_RTC_Alarm(System_GetTime(), tSetting);
    }
    if ((g_Setting.tExPedWkup.bEnableWkup == TRUE) && (g_SysInfo.tVehicleState.bACC_ON == TRUE))
    {
        ExternalPedWkupTypeDef tSetting = {0};

        tSetting.bEnableWkup = FALSE;

        MG82_RTC_PedWkup(tSetting);
    }
#endif
}

//============================================================================//
// @brief  Restart IC MG82.
// @param  None.
// @retval None.
//============================================================================//
void MG82_Restart(void)
{
    uint8_t u8MG82_RestartCnt = 0;

    if (g_SysInfo.tVehicleState.bPWR_ON == FALSE)
        return;

    u8MG82_RestartCnt = HW_ReadBackupRegister(I2C2_ERROR_CHK);

    if (bMG82_Stop == TRUE)
        return;

    u8MG82_RestartCnt++;

    HW_WriteBackupRegister(I2C2_ERROR_CHK, u8MG82_RestartCnt);

    if (u8MG82_RestartCnt >= MG82_RESTART_CNT_MAX)
    {
        RTOSprintf("\r\n> Maximum MG82 restart reached. Report IC MG82 ERROR!");

        u8MG82_RestartCnt = 0;

        HW_WriteBackupRegister(I2C2_ERROR_CHK, u8MG82_RestartCnt);

        u32MG82_ReportErrLastTick = HAL_GetTick();

        bMG82_ReportErrFlag = TRUE;

        bMG82_Stop = TRUE;

        // #if ENABLE_GSM_NET
        // GSM_TCP_ReportEvent(EVENT_EX_RTC_FAIL, STR_NULL);
        // #endif

#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
        LCD_SetShowAutoTestPage(55);
#endif

        return;
    }

    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, (char *)"6", i);
    }

#if ENABLE_GSM_NET
    QtEC21_CheckRedoFTP();
#endif

    System_SaveSettingFinal();
    System_SaveBackupSettingAll(VAR_ALL_ITEMS);

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();
}

//============================================================================//
// @brief  Check if the MG82 IC is ready or not.
// @param  None.
// @retval TRUE: IC MG82 is ready to communicate.
//         FALSE: IC MG82 is not ready.
//============================================================================//
bool MG82_IsReady(void)
{
    HAL_StatusTypeDef halRes;

    // PCF_ReInit();

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    halRes = HAL_I2C_IsDeviceReady(&hi2c2, MG82_ADDR_I2C,
                                   MG82_MAX_TRIALS, MG82_MAX_TIMEOUT);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
        return TRUE;
    else
    {
        u8MG82_ErrCnt++;

        if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
        {
            u8MG82_ErrCnt = 0;

            if (bMG82_Stop == FALSE)
            {
                RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                MG82_Restart();
            }
        }

        return FALSE;
    }
}

//============================================================================//
// @brief  Read from 1 register of IC MG82.
// @param  u8Addr: Address of the register to read from.
//         pu8Dat: Pointer to the data buffer to store read data.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_ReadReg(uint8_t u8Addr, uint8_t *pu8Dat)
{
    HAL_StatusTypeDef halRes = HAL_ERROR;
    uint8_t u8RetryCnt = 3;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    while ((halRes != HAL_OK) && (u8RetryCnt--))
    {
        halRes = HAL_I2C_Master_Transmit(&hi2c2, MG82_ADDR_I2C, &u8Addr,
                                         1, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 read from register 0x%0X send address ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }

        int j = 0xFFF;
        while (j--)
            ;

        // Read data
        halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)MG82_ADDR_I2C, pu8Dat,
                                        1, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 read from register 0x%0X read data ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }
    }

    HAL_Delay(1);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
    {
        u8MG82_ErrCnt = 0;

        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  Read from registers of IC MG82.
// @param  u8Addr: Address of the start register to read from.
//         pu8Dat: Pointer to the data buffer to store read data.
//         u8DatLen: Number of bytes to read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_ReadRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen)
{
    HAL_StatusTypeDef halRes = HAL_ERROR;
    uint8_t u8RetryCnt = 3;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    while ((halRes != HAL_OK) && (u8RetryCnt--))
    {
        halRes = HAL_I2C_Master_Transmit(&hi2c2, MG82_ADDR_I2C, &u8Addr,
                                         1, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 read from register 0x%0X send address ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }

        int j = 0xFFF;
        while (j--)
            ;

        // Read data
        halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)MG82_ADDR_I2C, pu8Dat,
                                        u8DatLen, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 read from register 0x%0X read data ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }
    }

    HAL_Delay(1);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
    {
        u8MG82_ErrCnt = 0;

        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  Write to a register of IC MG82.
// @param  u8Addr: Address of start register to write to.
//         pu8Dat: Pointer to data buffer.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_WriteReg(uint8_t u8Addr, uint8_t u8Dat)
{
    HAL_StatusTypeDef halRes = HAL_ERROR;
    uint8_t u8RetryCnt = 3;
    uint8_t u8DatBuf[2] = {0};

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    u8DatBuf[0] = u8Addr;
    u8DatBuf[1] = u8Dat;

    while ((halRes != HAL_OK) && (u8RetryCnt--))
    {
        halRes = HAL_I2C_Master_Transmit(&hi2c2, MG82_ADDR_I2C, u8DatBuf,
                                         2, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 write to register 0x%0X ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }
    }

    HAL_Delay(1);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
    {
        u8MG82_ErrCnt = 0;

        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  Write to registers of IC MG82.
// @param  u8Addr: Address of the start register to write to.
//         pu8Dat: Pointer to data buffer.
//         u8DatLen: Number of bytes to write.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_WriteRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen)
{
    HAL_StatusTypeDef halRes = HAL_ERROR;
    uint8_t u8RetryCnt = 3;
    uint8_t u8DatBuf[MG82_I2C_DATA_SIZE + 2] = {0};

    if (u8DatLen > MG82_I2C_DATA_SIZE)
        return FALSE;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    u8DatBuf[0] = u8Addr;
    memcpy(&u8DatBuf[1], pu8Dat, u8DatLen);

    while ((halRes != HAL_OK) && (u8RetryCnt--))
    {
        halRes = HAL_I2C_Master_Transmit(&hi2c2, MG82_ADDR_I2C, u8DatBuf,
                                         1 + u8DatLen, MG82_MAX_TIMEOUT);

        if (halRes != HAL_OK)
        {
            if (halRes == HAL_BUSY)
            {
                MG82_ReInit();
            }
            else
            {
                u8MG82_ErrCnt++;

                if (u8MG82_ErrCnt >= MG82_ERR_CNT_MAX)
                {
                    u8MG82_ErrCnt = 0;

                    if (bMG82_Stop == FALSE)
                    {
                        RTOSprintf("\r\n> Maximum MG82 errors reached. Restart IC MG82.");

                        MG82_Restart();

                        break;
                    }
                }
            }

            RTOSprintf("\r\n> MG82 write to registers from 0x%0X ERROR %d", u8Addr, halRes);

            HAL_Delay(10);

            continue;
        }
    }

    HAL_Delay(1);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
    {
        u8MG82_ErrCnt = 0;

        return TRUE;
    }
    else
        return FALSE;
}

// Master write commands

//============================================================================//
// @brief  Send data through an UART port.
// @param  u8Port: UART port, 0-3.
//         pu8Dat: Pointer to data buffer.
//         u16DatLen: Number of bytes to write.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_Send(uint8_t u8Port, uint8_t *pu8Dat, uint8_t u8DatLen)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8Port >= MG82_UART_CHANNEL_MAX)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid UART port (%d)", MG82_CMD_UART_TX, u8Port);

        return FALSE;
    }

    if (u8DatLen > MG82_I2C_DATA_SIZE)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid data length (%d)", MG82_CMD_UART_TX, u8DatLen);

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_UART_TX;
    tCmd.u8DataNum = u8DatLen;
    tCmd.u8Param1 = u8Port;
    memcpy(tCmd.u8Data, pu8Dat, u8DatLen);

    bRes = MG82_WriteRegs(MG82_CMD_UART_TX, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + u8DatLen);

    return bRes;
}

//============================================================================//
// @brief  Set the Baudrate for a UART port.
// @param  u8Port: UART port, 0-3.
//         tBR: Baudrate in UART_BaudRateDef_t format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_SetBR(uint8_t u8Port, UART_BaudRateDef_t tBR)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8Port >= MG82_UART_CHANNEL_MAX)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid UART port (%d)", MG82_CMD_UART_BR_SET, u8Port);

        return FALSE;
    }

    switch ((int)tBR)
    {
    case BR_9600bps:
        tCmd.u8Data[0] = 2;
        break;

    case BR_19200bps:
        tCmd.u8Data[0] = 4;
        break;

    case BR_57600bps:
        tCmd.u8Data[0] = 12;
        break;

    default:
        tCmd.u8Data[0] = 16; // 115200
        break;
    }

    tCmd.u8Command = MG82_CMD_UART_BR_SET;
    tCmd.u8DataNum = 1;
    tCmd.u8Param1 = u8Port;

    bRes = MG82_WriteRegs(MG82_CMD_UART_BR_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    return bRes;
}

//============================================================================//
// @brief  Reset the RX buffer a UART port.
// @param  u8Port: UART port, 0-3.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_Reset(uint8_t u8Port)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8Port >= MG82_UART_CHANNEL_MAX)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid UART port (%d)", MG82_CMD_UART_RST, u8Port);

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_UART_RST;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = u8Port;

    bRes = MG82_WriteRegs(MG82_CMD_UART_RST, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Reset the RX buffer all UART ports.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_ResetAll(void)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_UART_RST_ALL;
    tCmd.u8DataNum = 0;

    bRes = MG82_WriteRegs(MG82_CMD_UART_RST_ALL, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Set the output value for a pin.
// @param  u8IO: IO out pin, 0-2.
//         bVal: IO output value, 0 or 1.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_IO_Set(uint8_t u8IO, bool bVal)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8IO >= MG82_IO_OUT_MAX)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid IO out pin (%d)", MG82_CMD_IO_SET, u8IO);

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_IO_SET;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = u8IO;
    tCmd.u8Param2 = bVal;

    bRes = MG82_WriteRegs(MG82_CMD_IO_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    // RTOSprintf("\r\n> MG82 IO Set result: %d", bRes);

    return bRes;
}
//============================================================================//
// @brief  Set the output value for a pin.
// @param  bVal: IO output value, 0 or 1.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_USBPROTECTION_Set(bool bVal)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_USBPROTECT_SET;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bVal;

    bRes = MG82_WriteRegs(MG82_CMD_USBPROTECT_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    // RTOSprintf("\r\n> MG82 IO Set result: %d", bRes);

    return bRes;
}
//============================================================================//
// @brief  Set the date time.
// @param  ptDate: Pointer to date structure RTC_DateTypeDef.
//         ptTime: Pointer to time structure RTC_TimeTypeDef.
//         bIsBin: TRUE: Data is in binary format, FALSE: Data is in BCD format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RTC_SetDateTime(RTC_DateTypeDef *ptDate, RTC_TimeTypeDef *ptTime,
                          bool bIsBin)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_DT_SET;
    tCmd.u8DataNum = 6;

    tCmd.u8Data[0] = ptDate->Year;
    tCmd.u8Data[1] = ptDate->Month;
    tCmd.u8Data[2] = ptDate->Date;
    tCmd.u8Data[3] = ptTime->Hours;
    tCmd.u8Data[4] = ptTime->Minutes;
    tCmd.u8Data[5] = ptTime->Seconds;

    // If the input is BCD format convert to binary format
    if (bIsBin == FALSE)
    {
        for (int i = 0; i < 6; i++)
            tCmd.u8Data[i] = BCD_2_Bin(tCmd.u8Data[i]);
    }

    bRes = MG82_WriteRegs(MG82_CMD_DT_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 6);

    return bRes;
}

//============================================================================//
// @brief  Config Watchdog function.
// @param  bEnable: 0: Disable WDG function, 1: Enable WDG function.
//         u16TimeOut: WDG timeout in seconds.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_WDG_Config(bool bEnable, uint16_t u16TimeOut)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_MCU_WDT_SET;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bEnable;
    tCmd.u8Param2 = u16TimeOut;

    bRes = MG82_WriteRegs(MG82_CMD_MCU_WDT_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Config ADC function.
// @param  bEnable: 0: Disable ADC function, 1: Enable ADC function.
//         u16TimeOut: WDG timeout in seconds.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_ADC_Config(bool bEnable, uint16_t u16TimeOut)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_MCU_ADC_SET;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bEnable;

    bRes = MG82_WriteRegs(MG82_CMD_MCU_ADC_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Reset MG82 IC.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_SW_Reset(void)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_SW_RESET;
    tCmd.u8DataNum = 0;

    bRes = MG82_WriteRegs(MG82_CMD_SW_RESET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Set MG82 power mode.
// @param  tPwrMode: Power mode in MG82_PwrTypeDef format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_PWR_Mode(MG82_PwrTypeDef tPwrMode)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if ((tPwrMode == MG82_PWR_MODE_NORMAL) || (tPwrMode > MG82_PWR_MODE_SUB_CLK))
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid power mode (%d)", MG82_CMD_PWR_MODE, tPwrMode);

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_PWR_MODE;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = tPwrMode;

    bRes = MG82_WriteRegs(MG82_CMD_PWR_MODE, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    // RTOSprintf("\r\n> MG82 PWR MODE result: %d", bRes);

    if (bRes)
        bMG82_Stop = TRUE;

    return bRes;
}

//============================================================================//
// @brief  Enable/ Disable key reset funtion.
// @param  bOption: TRUE: Enable, FALSE: Disable.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_KetRstEn(bool bOption)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_KEY_RST;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bOption;

    bRes = MG82_WriteRegs(MG82_CMD_KEY_RST, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    // RTOSprintf("\r\n> MG82 KEY RST EN result: %d", bRes);

    return bRes;
}

//============================================================================//
// @brief  Change 4G DTR pin signal.
// @param  bVal: 4G DTR signal.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_4G_DTR(bool bVal)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_4G_DTR;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bVal;

    bRes = MG82_WriteRegs(MG82_CMD_4G_DTR, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Turn off MAIN_PWR and send MG82 to pwr down mode.
// @param  tPwrMode: Power mode in MG82_PwrTypeDef format.
//                   If this parameter is MG82_PWR_MODE_NORMAL stop this function
//                   on MG82. Otherwise it is started.
//         u16TimeOut: Delay time before the function is activated in seconds.
//                     Range: 0-120.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_Shutdown(MG82_PwrTypeDef tPwrMode, uint16_t u16Timeout)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (tPwrMode > MG82_PWR_MODE_SUB_CLK)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid power mode (%d)", MG82_CMD_SHUTDOWN, tPwrMode);

        return FALSE;
    }

    if (u16Timeout > MG82_SHUTDOWN_DELAY_MAX)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid delay time (%d)", MG82_CMD_SHUTDOWN, u16Timeout);

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_SHUTDOWN;
    tCmd.u8DataNum = 2;
    tCmd.u8Param1 = tPwrMode;
    tCmd.u8Data[1] = u16Timeout / 256;
    tCmd.u8Data[0] = u16Timeout % 256;

    bRes = MG82_WriteRegs(MG82_CMD_SHUTDOWN, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 2);

    // RTOSprintf("\r\n> MG82 SHUTDOWN result: %d", bRes);

    if (bRes)
        bMG82_Stop = TRUE;

    return bRes;
}

//============================================================================//
// @brief  Config RTC alarm setting.
// @param  ttCurrTime: Current time in time_t format to update MG82 RTC
//         tAlarm: Alarm setting in ExternalAlarmTypeDef format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RTC_Alarm(time_t ttCurrTime, ExternalAlarmTypeDef tAlarm)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};
    struct tm tmRTCTimeInfo = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    localtime_r(&ttCurrTime, &tmRTCTimeInfo);

    tCmd.u8Command = MG82_CMD_RTC_WKUP;
    tCmd.u8DataNum = 13;
    tCmd.u8Param1 = MG82_RTC_WKUP_ALM;
    tCmd.u8Param2 = (uint8_t)tAlarm.bEnableAlm;
    tCmd.u8Data[0] = tmRTCTimeInfo.tm_year + 1900 - 2000;
    tCmd.u8Data[1] = tmRTCTimeInfo.tm_mon + 1;
    tCmd.u8Data[2] = tmRTCTimeInfo.tm_mday;
    tCmd.u8Data[3] = tmRTCTimeInfo.tm_hour;
    tCmd.u8Data[4] = tmRTCTimeInfo.tm_min;
    tCmd.u8Data[5] = tmRTCTimeInfo.tm_sec;
    tCmd.u8Data[6] = tAlarm.tAlmDate.Year;
    tCmd.u8Data[7] = tAlarm.tAlmDate.Month;
    tCmd.u8Data[8] = tAlarm.tAlmDate.Date;
    tCmd.u8Data[9] = tAlarm.tAlmTime.Hours;
    tCmd.u8Data[10] = tAlarm.tAlmTime.Minutes;
    tCmd.u8Data[11] = tAlarm.tAlmTime.Seconds;
    tCmd.u8Data[12] = tAlarm.bRepeat;

    bRes = MG82_WriteRegs(MG82_CMD_RTC_WKUP, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 13);

    // RTOSprintf("\r\n> MG82 RTC WKUP ALM result: %d", bRes);

    return bRes;
}

//============================================================================//
// @brief  Config RTC periodic wakeup setting.
// @param  tSetting: Periodic wakeup setting in ExternalPedWkupTypeDef format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RTC_PedWkup(ExternalPedWkupTypeDef tSetting)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if ((tSetting.bEnableWkup == TRUE) && (tSetting.u32Timeout == 0))
    {
        RTOSprintf("\r\n> MG82 RTC WKUP PERIODIC timeout cannot be 0!");

        return FALSE;
    }

    tCmd.u8Command = MG82_CMD_RTC_WKUP;
    tCmd.u8DataNum = 4;
    tCmd.u8Param1 = MG82_RTC_WKUP_PERIODIC;
    tCmd.u8Param2 = (uint8_t)tSetting.bEnableWkup;
    tCmd.u8Data[0] = (uint8_t)tSetting.u32Timeout;
    tCmd.u8Data[1] = (uint8_t)(tSetting.u32Timeout >> 8);
    tCmd.u8Data[2] = (uint8_t)(tSetting.u32Timeout >> 16);
    tCmd.u8Data[3] = (uint8_t)(tSetting.u32Timeout >> 24);

    bRes = MG82_WriteRegs(MG82_CMD_RTC_WKUP, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 4);

    // RTOSprintf("\r\n> MG82 RTC WKUP PERIODIC result: %d", bRes);

    return bRes;
}

#if HW_TYPE == HW_JAS306SD
//============================================================================//
// @brief  Config RS232/RS485 for UART port 5.
// @param  bEnable: 0: Select RS232, 1: Select RS485.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RS485_Config(bool bEnable)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    tCmd.u8Command = MG82_CMD_RS485_SET;
    tCmd.u8DataNum = 0;
    tCmd.u8Param1 = bEnable;

    bRes = MG82_WriteRegs(MG82_CMD_RS485_SET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}

//============================================================================//
// @brief  Perform an 1-Wire operation.
// @param  ptOW_Data: Pointer to a OW_DataTypeDef struct.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_OW_Cmd(OW_DataTypeDef *ptOW_Data)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (ptOW_Data->u8DataLen)
    {
        if (ptOW_Data->u8DataLen > MG82_I2C_DATA_SIZE)
        {
            {
                RTOSprintf("\r\n> MG82 0x%02X invalid data length (%d)", MG82_CMD_OW_CMD, ptOW_Data->u8DataLen);

                return FALSE;
            }
        }

        tCmd.u8DataNum = ptOW_Data->u8DataLen;
        memcpy(tCmd.u8Data, ptOW_Data->u8Data, tCmd.u8DataNum);
    }
    else
        tCmd.u8DataNum = 0;

    tCmd.u8Command = MG82_CMD_OW_CMD;
    tCmd.u8Param1 = ptOW_Data->u8Cmd;

    bRes = MG82_WriteRegs(MG82_CMD_OW_CMD, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE);

    return bRes;
}
#endif

// Master read commands

//============================================================================//
// @brief  Get the current data length of all UART RX buffers.
// @param  pu16RX_Len: Buffer pointer to store the data length of all UART buffers.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_GetLen(uint16_t *pu16RX_Len)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_UART_CHK, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + MG82_UART_CHANNEL_MAX * 2);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_UART_CHK);

        return FALSE;
    }

    for (int i = 0; i < MG82_UART_CHANNEL_MAX; i++)
    {
        pu16RX_Len[i] = (((uint16_t)tCmd.u8Data[i * 2 + 1] << 8) & 0xFF00) | tCmd.u8Data[i * 2];
    }

    return TRUE;
}

//============================================================================//
// @brief  Read RX data from a UART port.
// @param  u8Port: UART port ID, 0-3.
//         pu8Buf: Pointer to data buffer.
//         u8Len: Read length.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_Read(uint8_t u8Port, uint8_t *pu8Buf, uint8_t u8Len)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8Port >= MG82_UART_CHANNEL_MAX)
    {
        RTOSprintf("\r\n> MG82 UART read invalid UART port (%d)", u8Port);

        return FALSE;
    }

    if (u8Len > MG82_I2C_DATA_SIZE)
    {
        RTOSprintf("\r\n> MG82 UART read invalid data length (%d)", MG82_CMD_UART_TX, u8Len);

        return FALSE;
    }

    bRes = MG82_ReadRegs(MG82_CMD_UART0_RX + u8Port, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + u8Len);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 UART read port %d ERROR", u8Port);

        return FALSE;
    }

    if (tCmd.u8DataNum > u8Len)
    {
        if (tCmd.u8DataNum > MG82_I2C_DATA_SIZE)
        {
            memcpy(pu8Buf, tCmd.u8Data, tCmd.u8DataNum);
        }
        else
        {
            memcpy(pu8Buf, tCmd.u8Data, tCmd.u8DataNum);
        }
    }
    else
    {
        memcpy(pu8Buf, tCmd.u8Data, tCmd.u8DataNum);
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the current Baudrate of all UART ports.
// @param  ptBR: Buffer to store the baudrate values of all UART ports.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_GetBR_All(UART_BaudRateDef_t *ptBR)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_UART_BR, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + MG82_UART_CHANNEL_MAX);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_UART_BR);

        return FALSE;
    }

    for (int i = 0; i < MG82_UART_CHANNEL_MAX; i++)
    {
        switch (tCmd.u8Data[i])
        {
        case 2:
            ptBR[i] = BR_9600bps;
            break;

        case 4:
            ptBR[i] = BR_19200bps;
            break;

        case 12:
            ptBR[i] = BR_57600bps;
            break;

        default:
            ptBR[i] = BR_115200bps;
            break;
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the current Baudrate of a UART port.
// @param  u8Port: UART port ID, 0-3.
//         ptBR: Buffer to store the baudrate value.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_UART_GetBR(uint8_t u8Port, UART_BaudRateDef_t *ptBR)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8Port >= MG82_UART_CHANNEL_MAX)
    {
        RTOSprintf("\r\n> MG82 UART read invalid UART port (%d)", u8Port);

        return FALSE;
    }

    bRes = MG82_ReadRegs(MG82_CMD_UART0_BR + u8Port, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 UART get port % Baudrate ERROR", u8Port);

        return FALSE;
    }

    switch (tCmd.u8Data[0])
    {
    case 2:
        *ptBR = BR_9600bps;
        break;

    case 4:
        *ptBR = BR_19200bps;
        break;

    case 12:
        *ptBR = BR_57600bps;
        break;

    default:
        *ptBR = BR_115200bps;
        break;
    }

    return TRUE;
}

//============================================================================//
// @brief  Get the date time stamp.
// @param  ptDate: Pointer to date structure RTC_DateTypeDef.
//         ptTime: Pointer to time structure RTC_TimeTypeDef.
//         bIsBin: TRUE: Data is in binary format, FALSE: Data is in BCD format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RTC_GetDateTime(RTC_DateTypeDef *ptDate, RTC_TimeTypeDef *ptTime,
                          bool bIsBin)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_DT_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 6);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_DT_GET);

        return FALSE;
    }

    // If require BCD format convert all data bytes to BCD
    if (bIsBin == FALSE)
    {
        for (int i = 0; i < 6; i++)
        {
            tCmd.u8Data[i] = Bin_2_BCD(tCmd.u8Data[i]);
        }
    }

    ptDate->Year = tCmd.u8Data[0];
    ptDate->Month = tCmd.u8Data[1];
    ptDate->Date = tCmd.u8Data[2];
    ptTime->Hours = tCmd.u8Data[3];
    ptTime->Minutes = tCmd.u8Data[4];
    ptTime->Seconds = tCmd.u8Data[5];

    return TRUE;
}

//============================================================================//
// @brief  Get the value of the IO input port.
// @param  pbIO_Val: Pointer bool buffer to store the port value.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_IO_Get(bool *pbIO_Val)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_IO_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_IO_GET);

        return FALSE;
    }

    pbIO_Val[0] = ((tCmd.u8Data[0] >> 3) & 0x01) ? 1 : 0;
    pbIO_Val[1] = ((tCmd.u8Data[0] >> 4) & 0x01) ? 1 : 0;
    pbIO_Val[2] = ((tCmd.u8Data[0] >> 5) & 0x01) ? 1 : 0;

    return TRUE;
}

//============================================================================//
// @brief  Get the configuration of the Watchdog function.
// @param  pu8En: Returned value: 0: WDG disabled, 1: WDG enabled.
//         pu8TimeOut: Returned value: WDG timeout in seconds.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_WDG_GetCfg(uint8_t *pu8En, uint8_t *pu8TimeOut)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_MCU_WDT_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 2);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_MCU_WDT_GET);

        return FALSE;
    }

    *pu8En = tCmd.u8Data[0];
    *pu8TimeOut = tCmd.u8Data[1];

    return TRUE;
}

//============================================================================//
// @brief  Get the configuration of the ADC function.
// @param  pu8En: Return value: 0: ADC disabled, 1: ADC enabled.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_ADC_GetCfg(uint8_t *pu8En)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_ADC_MODE, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_ADC_MODE);

        return FALSE;
    }

    *pu8En = tCmd.u8Data[0];

    return TRUE;
}

//============================================================================//
// @brief  Get the ADC conversion value.
// @param  pu16Val: Returned ADC value.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_ADC_Read(uint8_t *pu16Val)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_ADC_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 2);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_ADC_GET);

        return FALSE;
    }

    *pu16Val = (((uint16_t)tCmd.u8Data[1] << 8) & 0xFF00) | tCmd.u8Data[0];

    return TRUE;
}

//============================================================================//
// @brief  Get the FW version of MG82.
// @param  pcVer: Buffer to store returned data.
//         u8LenMax: Maximum data buffer length.
//         pu8Len: Returned data length.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_GetVer(char *pcVer, uint8_t u8LenMax, uint8_t *pu8Len)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_GET_VER, (uint8_t *)&tCmd, MG82_I2C_PACKET_SIZE);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_GET_VER);

        return FALSE;
    }

    if (tCmd.u8DataNum > u8LenMax)
        *pu8Len = u8LenMax;
    else
        *pu8Len = tCmd.u8DataNum;

    memcpy(pcVer, tCmd.u8Data, *pu8Len);

    return TRUE;
}

#if HW_TYPE == HW_JAS306SD
//============================================================================//
// @brief  Get the RS232/RS485 configuration of UART5.
// @param  pbEnable: Returned value: 0: RS232 selected, 1: RS485 selected.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RS485_GetConfig(bool *pbEnable)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_RS485_MODE, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_RS485_MODE);

        return FALSE;
    }

    *pbEnable = tCmd.u8Data[0] ? 1 : 0;

    return TRUE;
}

//============================================================================//
// @brief  Check the 1-Wire operation status.
// @param  pu8Cmd: Returned value: Current 1-Wire command code.
//         pu8Cplt: Returned value: 0: Operation is not completed, 1: Completed.
//         pu8Res: Returned value: Operation result: 0 or 1.
//         pu8RetLen: Returned value: Length of the returned data in case read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_OW_CheckStatus(uint8_t *pu8Cmd, uint8_t *pu8Cplt,
                         uint8_t *pu8Res, uint8_t *pu8RetLen)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_OW_CHK_STAT, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 4);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_OW_CHK_STAT);

        return FALSE;
    }

    *pu8Cmd = tCmd.u8Data[0];
    *pu8Cplt = tCmd.u8Data[1];
    *pu8Res = tCmd.u8Data[2];
    *pu8RetLen = tCmd.u8Data[3];

    return TRUE;
}

//============================================================================//
// @brief  Read the data returned from the 1-Wire read operation.
// @param  pu8DatBuf: Buffer to store the read data.
//         u8ReadLen: Number of bytes to read.
//         pu8ActualLen: Return the actual number of bytes read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_OW_Read(uint8_t *pu8DatBuf, uint8_t u8ReadLen, uint8_t *pu8ActualLen)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8ReadLen >= MG82_I2C_DATA_SIZE)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid read length (%d)", MG82_CMD_OW_GET_DATA, u8ReadLen);

        return FALSE;
    }

    bRes = MG82_ReadRegs(MG82_CMD_OW_GET_DATA, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + u8ReadLen);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_OW_GET_DATA);

        return FALSE;
    }

    *pu8ActualLen = tCmd.u8DataNum;

    if (tCmd.u8DataNum)
    {
        memcpy(pu8DatBuf, tCmd.u8Data, tCmd.u8DataNum);
    }

    return TRUE;
}

//============================================================================//
// @brief  Check the 1-Wire operation status and read returned data at the same time.
// @param  pu8DatBuf: Buffer to store the read data.
//         u8ReadLen: Number of bytes to read.
//         pu8ActualLen: Return the actual number of bytes read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_OW_Check_Read(uint8_t *pu8Cmd, uint8_t *pu8Cplt, uint8_t *pu8Res,
                        uint8_t *pu8DatBuf, uint8_t u8ReadLen, uint8_t *pu8ActualLen)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    if (u8ReadLen >= MG82_I2C_DATA_SIZE)
    {
        RTOSprintf("\r\n> MG82 0x%02X invalid read length (%d)", MG82_CMD_OW_GET, u8ReadLen);

        return FALSE;
    }

    bRes = MG82_ReadRegs(MG82_CMD_OW_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 4 + u8ReadLen);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_OW_GET);

        return FALSE;
    }

    *pu8Cmd = tCmd.u8Data[0];
    *pu8Cplt = tCmd.u8Data[1];
    *pu8Res = tCmd.u8Data[2];
    *pu8ActualLen = tCmd.u8Data[3];

    if (*pu8ActualLen)
    {
        memcpy(pu8DatBuf, &tCmd.u8Data[4], *pu8ActualLen);
    }

    return TRUE;
}
#endif

//============================================================================//
// @brief  Get the MCU wake up source.
// @param  ptSrc: Returned wake up source in MG82_WkupSrcTypeDef format.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_WKUP_Src(MG82_WkupSrcTypeDef *ptSrc)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_GET_WKUP_SRC, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_GET_WKUP_SRC);

        return FALSE;
    }

    *ptSrc = (MG82_WkupSrcTypeDef)tCmd.u8Data[0];

    return TRUE;
}

//============================================================================//
// @brief  Get key reset enable option.
// @param  pbOption: Return Enable/Disable option.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_KeyRstGet(bool *pbOption)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_GET_KEYRST, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 1);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_GET_KEYRST);

        return FALSE;
    }

    *pbOption = (MG82_WkupSrcTypeDef)tCmd.u8Data[0];

    return TRUE;
}

//============================================================================//
// @brief  Get the RTC Wakeup setting.
// @param  ptAlarm: Pointer to the variable containing returned alarm setting.
//         ptSetting: Pointer to the variable containing returned wakeup setting.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool MG82_RTC_GetWkup(ExternalAlarmTypeDef *ptAlarm,
                      ExternalPedWkupTypeDef *ptSetting)
{
    bool bRes = FALSE;
    MG82_CmdTypeDef tCmd = {0};

    if (bMG82_Stop == TRUE)
        return FALSE;

    bRes = MG82_ReadRegs(MG82_CMD_RTC_WKUP_GET, (uint8_t *)&tCmd, MG82_I2C_HEADER_SIZE + 13);

    if (bRes == FALSE)
    {
        RTOSprintf("\r\n> MG82 0x%02X ERROR", MG82_CMD_RTC_WKUP_GET);

        return FALSE;
    }

    ptAlarm->bEnableAlm = tCmd.u8Data[0];
    ptAlarm->bRepeat = tCmd.u8Data[1];
    ptAlarm->tAlmDate.Year = tCmd.u8Data[2];
    ptAlarm->tAlmDate.Month = tCmd.u8Data[3];
    ptAlarm->tAlmDate.Date = tCmd.u8Data[4];
    ptAlarm->tAlmTime.Hours = tCmd.u8Data[5];
    ptAlarm->tAlmTime.Minutes = tCmd.u8Data[6];
    ptAlarm->tAlmTime.Seconds = tCmd.u8Data[7];
    ptSetting->bEnableWkup = tCmd.u8Data[8];
    ptSetting->u32Timeout = (uint32_t)((((uint32_t)tCmd.u8Data[12] << 24) & 0xFF000000) |
                                       (((uint32_t)tCmd.u8Data[11] << 16) & 0x00FF0000) |
                                       (((uint32_t)tCmd.u8Data[10] << 8) & 0x0000FF00) |
                                       ((uint32_t)tCmd.u8Data[9] & 0x000000FF));

    return TRUE;
}
#endif
