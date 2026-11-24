//============================================================================//
// User Includes
//============================================================================//

#include "RX8130_Driver.h"

#include <string.h>

#include "CommFunc.h"
#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
#include "LCD_Key.h"
#endif
#include "main.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//
uint8_t const RX8130_WdayVal[] =
    {
        0x00, // Null
        0x02, // Monday
        0x04, // Tuesday
        0x08, // Wednesday
        0x10, // Thursday
        0x20, // Friday
        0x40, // Saturday
        0x01  // Sunday
};

uint8_t RX8130_Mask[7] =
    {
        RX8130_SEC_MASK,
        RX8130_MIN_MASK,
        RX8130_HOUR_MASK,
        RX8130_WDAY_MASK,
        RX8130_DAY_MASK,
        RX8130_MON_MASK,
        RX8130_YEAR_MASK};

bool bRX8130_EN = FALSE;

uint8_t u8RX8130_ErrCnt = 0;

uint32_t u32RX8130_ReportErrLastTick = 0;

bool bRX8130_ReportErrFlag = FALSE;

bool bRX8130_Stop = FALSE;

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Convert the weekday value from RX8130 format to MCU RTC format.
// @param  u8Wday: Weekday value in RX8130 format.
// @retval Weekday value in MCU RTC format.
//============================================================================//
static uint8_t RX8130_ConvertWday(uint8_t u8Wday)
{
    uint8_t u8RetWday = RTC_WEEKDAY_MONDAY;

    for (int i = 0; i < 8; i++)
    {
        if ((u8Wday & RX8130_WdayVal[i]) != 0)
        {
            u8RetWday = i;

            break;
        }
    }

    return u8RetWday;
}

//============================================================================//
// @brief  Scan the I2C Bus for I2C device address.
// @param  None.
// @retval None.
//============================================================================//
void RX8130_ScanBus(void)
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
// @brief  Make the RX8130 IC ready to communicate again.
// @param  None.
// @retval None.
//============================================================================//
void RX8130_ReInit(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return;
    }

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

    if (bRTOS_En)
    {
        osMutexRelease(mutex_i2c2_user);
    }
}

//============================================================================//
// @brief  Deinit the RX8130 communication interface.
// @param  None.
// @retval None.
//============================================================================//
void RX8130_DeInit(void)
{
    HAL_I2C_DeInit(&hi2c2);
}

//============================================================================//
// @brief  Restart IC RX8130.
// @param  None.
// @retval None.
//============================================================================//
void RX8130_Restart(void)
{
    uint8_t u8RX8130_RestartCnt = 0;

    if (g_SysInfo.tVehicleState.bPWR_ON == FALSE)
        return;

    u8RX8130_RestartCnt = HW_ReadBackupRegister(I2C2_ERROR_CHK);

    if (bRX8130_Stop == TRUE)
        return;

    u8RX8130_RestartCnt++;

    HW_WriteBackupRegister(I2C2_ERROR_CHK, u8RX8130_RestartCnt);

    if (u8RX8130_RestartCnt >= RX8130_RESTART_CNT_MAX)
    {
        RTOSprintf("\r\n> Maximum RX8130 restart reached. Report IC RX8130 ERROR!");

        u8RX8130_RestartCnt = 0;

        HW_WriteBackupRegister(I2C2_ERROR_CHK, u8RX8130_RestartCnt);

        u32RX8130_ReportErrLastTick = HAL_GetTick();

        bRX8130_ReportErrFlag = TRUE;

        bRX8130_Stop = TRUE;

        // #if ENABLE_GSM_NET
        // GSM_TCP_ReportEvent(EVENT_EX_RTC_FAIL, STR_NULL);
        // #endif

#if HW_TYPE == HW_JAS208 || HW_TYPE == HW_JAS306
        LCD_SetShowAutoTestPage(55);
#endif

        return;
    }

#if ENABLE_GSM_NET
    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, (char *)"6", i);
    }

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
// @brief  Check if the RX8130 IC is ready or not.
// @param  None.
// @retval TRUE: IC RX8130 is ready to communicate.
//         FALSE: IC RX8130 is not ready.
//============================================================================//
bool RX8130_IsReady(void)
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

    halRes = HAL_I2C_IsDeviceReady(&hi2c2, RX8130_ADDR_I2C,
                                   RX8130_MAX_TRIALS, RX8130_MAX_TIMEOUT);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    if (halRes == HAL_OK)
        return TRUE;
    else
    {
        u8RX8130_ErrCnt++;

        if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
        {
            u8RX8130_ErrCnt = 0;

            if (bRX8130_Stop == FALSE)
            {
                RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                RX8130_Restart();
            }
        }

        return FALSE;
    }
}

//============================================================================//
// @brief  Read from 1 register of IC RX8130.
// @param  u8Addr: Address of the register to read from.
//         pu8Dat: Pointer to the data buffer to store read data.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_ReadReg(uint8_t u8Addr, uint8_t *pu8Dat)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    halRes = HAL_I2C_Master_Transmit(&hi2c2, RX8130_ADDR_I2C, &u8Addr,
                                     1, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 read from register 0x%0X send address ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)RX8130_ADDR_I2C, pu8Dat,
                                    1, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 read from register 0x%0X read data ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    return TRUE;
}

//============================================================================//
// @brief  Read from registers of IC RX8130.
// @param  u8Addr: Address of the start register to read from.
//         pu8Dat: Pointer to the data buffer to store read data.
//         u8DatLen: Number of bytes to read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_ReadRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    halRes = HAL_I2C_Master_Transmit(&hi2c2, RX8130_ADDR_I2C, &u8Addr,
                                     1, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 read from register 0x%0X send address ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)RX8130_ADDR_I2C, pu8Dat,
                                    u8DatLen, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 read from register 0x%0X read data ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    return TRUE;
}

//============================================================================//
// @brief  Write to a register of IC RX8130.
// @param  u8Addr: Address of start register to write to.
//         pu8Dat: Pointer to data buffer.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_WriteReg(uint8_t u8Addr, uint8_t u8Dat)
{
    HAL_StatusTypeDef halRes;
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

    halRes = HAL_I2C_Master_Transmit(&hi2c2, RX8130_ADDR_I2C, u8DatBuf,
                                     2, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 write to register 0x%0X ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    return TRUE;
}

//============================================================================//
// @brief  Write to registers of IC RX8130.
// @param  u8Addr: Address of the start register to write to.
//         pu8Dat: Pointer to data buffer.
//         u8DatLen: Number of bytes to write.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_WriteRegs(uint8_t u8Addr, uint8_t *pu8Dat, uint8_t u8DatLen)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8DatBuf[20] = {0};

    if (u8DatLen > 18)
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

    halRes = HAL_I2C_Master_Transmit(&hi2c2, RX8130_ADDR_I2C, u8DatBuf,
                                     1 + u8DatLen, RX8130_MAX_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
            RX8130_ReInit();
        }
        else
        {
            u8RX8130_ErrCnt++;

            if (u8RX8130_ErrCnt >= RX8130_ERR_CNT_MAX)
            {
                u8RX8130_ErrCnt = 0;

                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum RX8130 errors reached. Restart IC RX8130.");

                    RX8130_Restart();
                }
            }
        }

        RTOSprintf("\r\n> RX8130 write to registers from 0x%0X ERROR %d", u8Addr, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    return TRUE;
}

//============================================================================//
// @brief  Check VBFF bit.
// @param  None.
// @retval TRUE: VBFF bit is TRUE.
//         FALSE: VBFF bit is FALSE.
//============================================================================//
bool RX8130_CheckVBFF(void)
{
    uint8_t u8RegVal = 0;
    bool bRes;

    bRes = RX8130_ReadReg(RX8130_REG_FLAG, &u8RegVal);

    if (!bRes)
        return FALSE;

    if (u8RegVal & RX8130_BIT_FLAG_VBFF)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Check VLF bit.
// @param  None.
// @retval TRUE: VLF bit is TRUE.
//         FALSE: VLF bit is FALSE.
//============================================================================//
bool RX8130_CheckVLF(void)
{
    uint8_t u8RegVal = 0;
    bool bRes;

    bRes = RX8130_ReadReg(RX8130_REG_FLAG, &u8RegVal);

    if (!bRes)
        return FALSE;

    if (u8RegVal & RX8130_BIT_FLAG_VLF)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Clear VLF bit.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_ClrVLF(void)
{
    bool bRes;
    uint8_t u8RegVal = 0;

    bRes = RX8130_ReadReg(RX8130_REG_FLAG, &u8RegVal);

    if (!bRes)
        return FALSE;

    // Clear VLF bit
    u8RegVal &= ~RX8130_BIT_FLAG_VLF;

    bRes = RX8130_WriteReg(RX8130_REG_FLAG, u8RegVal);

    if (!bRes)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Check VBLF bit.
// @param  None.
// @retval TRUE: VBLF bit is TRUE.
//         FALSE: VBLF bit is FALSE.
//============================================================================//
bool RX8130_CheckVBLF(void)
{
    uint8_t u8RegVal = 0;
    bool bRes;

    bRes = RX8130_ReadReg(RX8130_REG_FLAG, &u8RegVal);

    if (!bRes)
        return FALSE;

    if (u8RegVal & RX8130_BIT_FLAG_VBLF)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Check STOP bit.
// @param  None.
// @retval TRUE: STOP bit is TRUE.
//         FALSE: STOP bit is FALSE.
//============================================================================//
bool RX8130_CheckStop(void)
{
    uint8_t u8RegVal = 0;
    bool bRes;

    bRes = RX8130_ReadReg(RX8130_REG_CTRL0, &u8RegVal);

    if (!bRes)
        return FALSE;

    if (u8RegVal & RX8130_BIT_CTRL_STOP)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Enable Stopping the timekeeping operation.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_StopEnable(void)
{
    bool bRes;
    uint8_t u8RegVal = 0;

    bRes = RX8130_ReadReg(RX8130_REG_CTRL0, &u8RegVal);

    if (!bRes)
        return FALSE;

    // Set stop bit
    u8RegVal |= RX8130_BIT_CTRL_STOP;

    bRes = RX8130_WriteReg(RX8130_REG_CTRL0, u8RegVal);

    if (!bRes)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Disable Stopping the timekeeping operation.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool RX8130_StopDisable(void)
{
    bool bRes;
    uint8_t u8RegVal = 0;

    bRes = RX8130_ReadReg(RX8130_REG_CTRL0, &u8RegVal);

    if (!bRes)
        return FALSE;

    // Clear stop bit
    u8RegVal &= ~RX8130_BIT_CTRL_STOP;

    bRes = RX8130_WriteReg(RX8130_REG_CTRL0, u8RegVal);

    if (!bRes)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Perform software reset on IC RX8130.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool RX8130_SoftwareReset(void)
{
    RX8130_WriteReg(RX8130_REG_CTRL0, 0x00);
    RX8130_WriteReg(RX8130_REG_CTRL0, 0x80);
    RX8130_WriteReg(0x50, 0x6C);
    RX8130_WriteReg(0x53, 0x01);
    RX8130_WriteReg(0x66, 0x03);
    RX8130_WriteReg(0x6B, 0x02);
    RX8130_WriteReg(0x6B, 0x01);

    HAL_Delay(150);

    return TRUE;
}

//============================================================================//
// @brief  Init registers on RX8130.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
static bool RX8130_InitRegs(void)
{
    RX8130_WriteReg(RX8130_REG_DT_OFST, 0x00);
    RX8130_WriteReg(RX8130_REG_EXT, 0x00);
    RX8130_WriteReg(RX8130_REG_FLAG, 0x00);
    RX8130_WriteReg(RX8130_REG_CTRL0, 0x00);
    // RX8130_WriteReg(RX8130_REG_CTRL0, 0x40);
    RX8130_WriteReg(RX8130_REG_CTRL1, 0x33);
    // RX8130_WriteReg(RX8130_REG_CTRL1, 0x00);

    return TRUE;
}

//============================================================================//
// @brief  Init IC RX8130.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool RX8130_Init(void)
{
    uint8_t u8RegVal;
    uint32_t u32WaitTick = 0;
    const uint16_t u32Timeout = S_2_mS(3);

    // Wait > 30ms
    HAL_Delay(50);

    // Dummy read
    RX8130_ReadReg(RX8130_REG_SEC, &u8RegVal);

    // Check VLF, if VLF is 0 the initialization is complete
    if (RX8130_CheckVLF() == 0)
    {
        RTOSprintf("\r\n> RX8130 skip software reset.");

        // return TRUE;
    }
    else
    {
        RTOSprintf("\r\n> RX8130 software reset.");

        // Perform software reset
        RX8130_SoftwareReset();

        u32WaitTick = HAL_GetTick();

        // Try to clear VLF bit
        while (HAL_GetTick() < (u32WaitTick + u32Timeout))
        {
            RX8130_ClrVLF();

            HAL_Delay(10);

            if (RX8130_CheckVLF() == 0)
                break;
        }

        // If clear VLF timeout
        if (HAL_GetTick() >= (u32WaitTick + u32Timeout))
        {
            RTOSprintf("\r\n> RX8130 init clear VLF timeout.");

            return FALSE;
        }

        RTOSprintf("\r\n> RX8130 init registers.");
    }

    // Initialize registers
    RX8130_InitRegs();

    return TRUE;
}

//============================================================================//
// @brief  Init IC RX8130 with date time setting.
// @param  None.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool RX8130_InitTime(RTC_DateTypeDef *prtcInitDate, RTC_TimeTypeDef *prtcInitTime,
                     bool bIsBin)
{
    uint8_t u8RegVal;
    uint32_t u32WaitTick = 0;
    const uint16_t u32Timeout = S_2_mS(3);

    // Wait > 30ms
    HAL_Delay(50);

    // Dummy read
    RX8130_ReadReg(RX8130_REG_SEC, &u8RegVal);

    // Check VLF, if VLF is 0 the initialization is complete
    if (RX8130_CheckVLF() == 0)
    {
        RTOSprintf("\r\n> R8130 skip initialization.");

        return TRUE;
    }

    // Perform software reset
    RX8130_SoftwareReset();

    u32WaitTick = HAL_GetTick();

    // Try to clear VLF bit
    while (HAL_GetTick() < (u32WaitTick + u32Timeout))
    {
        RX8130_ClrVLF();

        HAL_Delay(10);

        if (RX8130_CheckVLF() == 0)
            break;
    }

    // If clear VLF timeout
    if (HAL_GetTick() >= (u32WaitTick + u32Timeout))
    {
        RTOSprintf("\r\n> RX8130 init clear VLF timeout.");

        return FALSE;
    }

    // Initialize registers
    RX8130_InitRegs();

    RX8130_SetDateTime(prtcInitDate, prtcInitTime, bIsBin);

    return TRUE;
}

//============================================================================//
// @brief  Set date time for the IC RX8130.
// @param  prtcInitDate: Pointer to RTC_DateTypeDef struct for input date.
//         prtcInitTime: Pointer to RTC_TimeTypeDef struct for input time.
//         bIsBin: TRUE: The input values are in Binary format.
//                 FALSE: The input values are in BCD format.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool RX8130_SetDateTime(RTC_DateTypeDef *prtcInitDate, RTC_TimeTypeDef *prtcInitTime,
                        bool bIsBin)
{
    uint8_t u8StartAddr = RX8130_REG_SEC;
    uint8_t u8Buf[7] = {0};
    bool bRes, bRes2;

    for (int i = 0; i < 3; i++)
    {
        bRes = RX8130_StopEnable();

        if (bRes)
            break;

        HAL_Delay(100);
    }

    if (!bRes)
        return FALSE;

    u8Buf[0] = prtcInitTime->Seconds;
    u8Buf[1] = prtcInitTime->Minutes;
    u8Buf[2] = prtcInitTime->Hours;
    u8Buf[4] = prtcInitDate->Date;
    u8Buf[5] = prtcInitDate->Month;
    u8Buf[6] = prtcInitDate->Year;

    if (bIsBin)
    {
        for (int i = 0; i < 7; i++)
            u8Buf[i] = Bin_2_BCD(u8Buf[i]);
    }

    u8Buf[3] = RX8130_WdayVal[prtcInitDate->WeekDay];

    for (int i = 0; i < 7; i++)
    {
        u8Buf[i] &= RX8130_Mask[i];
    }

    // RTOSprintf("\r\n> RX8130 write date time: ");

    // for(int i = 0; i < 7; i++)
    //{
    //   RTOSprintf("0x%02X ", u8Buf[6 - i]);
    // }

    bRes = RX8130_WriteRegs(u8StartAddr, u8Buf, 7);

    for (int i = 0; i < 3; i++)
    {
        bRes2 = RX8130_StopDisable();

        if (bRes2)
            break;

        HAL_Delay(100);
    }

    return bRes;
}

//============================================================================//
// @brief  Get date time from IC RX8130.
// @param  prtcRetDate: Pointer to RTC_DateTypeDef struct to store returned date.
//         prtcRetTime: Pointer to RTC_TimeTypeDef struct to store returned time.
//         bIsBin: TRUE: Returned values are in binary format.
//                 FALSE: Returned values are in BCD format.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool RX8130_GetDateTime(RTC_DateTypeDef *prtcRetDate, RTC_TimeTypeDef *prtcRetTime,
                        bool bIsBin)
{
    uint8_t u8StartAddr = RX8130_REG_SEC;
    uint8_t u8Buf[7] = {0};
    bool bRes;

    bRes = RX8130_ReadRegs(u8StartAddr, u8Buf, 7);

    if (!bRes)
        return FALSE;

    for (int i = 0; i < 7; i++)
    {
        u8Buf[i] &= RX8130_Mask[i];
    }

    prtcRetDate->WeekDay = RX8130_ConvertWday(u8Buf[3]); // Week Day

    if (bIsBin)
    {
        for (int i = 0; i < 7; i++)
            u8Buf[i] = BCD_2_Bin(u8Buf[i]);
    }

    prtcRetTime->Seconds = u8Buf[0]; // Second
    prtcRetTime->Minutes = u8Buf[1]; // Minute
    prtcRetTime->Hours = u8Buf[2];   // Hour
    prtcRetDate->Date = u8Buf[4];    // Day
    prtcRetDate->Month = u8Buf[5];   // Month
    prtcRetDate->Year = u8Buf[6];    // Year

    return TRUE;
}
