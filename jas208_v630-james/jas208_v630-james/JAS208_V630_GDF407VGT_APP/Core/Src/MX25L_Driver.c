//============================================================================//
// User Includes
//============================================================================//

#include "MX25L_Driver.h"
#include "stm32fxxx_hal_user.h"
#include "Config.h"
#include "HW_Config.h"
#include "LCD_Key.h"
#include "main.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//

// MX25L handling struct instance
MX25L_TypeDef mx25lInstance, *mx25lHandle;

bool bMX25L_ReportErrFlag = FALSE;

uint8_t u8MX25L_ErrCnt = 0;

uint32_t u32MX25L_ReportErrLastTick = 0;

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Select IC MX25L.
// @param  None.
// @retval None.
//============================================================================//
static void MX25L_Select(void)
{
    // CS = 0
    HW_PBClr(MX25L_CS);
}

//============================================================================//
// @brief  Deselect IC MX25L.
// @param  None.
// @retval None.
//============================================================================//
static void MX25L_Deselect(void)
{
    // CS = 1
    HW_PBSet(MX25L_CS);
}

//============================================================================//
// @brief  Restart IC MX25L.
// @param  None.
// @retval None.
//============================================================================//
void MX25L_Restart(void)
{
    uint8_t u8MX25L_RestartCnt;

    if (g_SysInfo.tVehicleState.bPWR_ON == FALSE)
        return;

    u8MX25L_RestartCnt = HW_ReadBackupRegister(MX25L_ERROR_CHK);

    if (bMX25L_ReportErrFlag == TRUE)
        return;

    u8MX25L_RestartCnt++;

    HW_WriteBackupRegister(MX25L_ERROR_CHK, u8MX25L_RestartCnt);

    if (u8MX25L_RestartCnt >= MX25L_RESTART_CNT_MAX)
    {
        RTOSprintf("\r\n> Maximum MX25L restart reached. Report MX25L ERROR!");

        u8MX25L_RestartCnt = 0;

        HW_WriteBackupRegister(MX25L_ERROR_CHK, u8MX25L_RestartCnt);

        u32MX25L_ReportErrLastTick = HAL_GetTick();

        bMX25L_ReportErrFlag = TRUE;

        LCD_SetShowAutoTestPage(54);

        return;
    }

#if ENABLE_GSM_NET
    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, (char *)"4", i);
    }

// QtEC21_CheckRedoFTP();
#endif

    // System_SaveSetting();
    // System_SaveBackupSetting();

    if (bRTOS_En)
    {
        osMutexWait(mutex_setting_user, S_2_mS(5));
    }

    System_Reset();
}

/*
//============================================================================//
// @brief  Convert page's offset address to physical address.
// @param  u16PageOffset: Page's offset address.
// @retval Physical address in uint32_t format.
//============================================================================//
uint32_t MX25L_PageAddr(uint16_t u16PageOffset)
{
    return (MX25L_MEM_START_ADDR + MX25L_PAGE_SIZE * u16PageOffset);
}

//============================================================================//
// @brief  Convert sector's offset address to physical address.
// @param  u16SectOffset: Sector's offset address.
// @retval Physical address in uint32_t format.
//============================================================================//
uint32_t MX25L_SectAddr(uint16_t u16SectOffset)
{
    return (MX25L_MEM_START_ADDR + MX25L_SECTOR_SIZE * u16SectOffset);
}

//============================================================================//
// @brief  Convert 32K block's offset address to physical address.
// @param  u16Block32Offset: 32K block's offset address.
// @retval Physical address in uint32_t format.
//============================================================================//
uint32_t MX25L_B32Addr(uint16_t u16Block32Offset)
{
    return (MX25L_MEM_START_ADDR + MX25L_BLOCK32_SIZE * u16Block32Offset);
}

//============================================================================//
// @brief  Convert 64K block's offset address to physical address.
// @param  u16Block64Offset: 64K block's offset address.
// @retval Physical address in uint32_t format.
//============================================================================//
uint32_t MX25L_B64Addr(uint16_t u16Block64Offset)
{
    return (MX25L_MEM_START_ADDR + MX25L_BLOCK64_SIZE * u16Block64Offset);
}
*/

//============================================================================//
// @brief  Write a byte to IC MX25L.
// @param  u8DataSend: Byte to write.
// @retval HAL status in HAL_StatusTypeDef format.
//============================================================================//
static HAL_StatusTypeDef MX25L_WriteByte(uint8_t u8DataSend)
{
    uint8_t u8DataRecv;

    return HAL_SPI_TransmitReceive(&hspi1, &u8DataSend, &u8DataRecv, 1, 100);
}

//============================================================================//
// @brief  Read a byte to IC MX25L.
// @param  pu8RecvData: Pointer to variable for storing read byte.
// @retval HAL status in HAL_StatusTypeDef format.
//============================================================================//
static HAL_StatusTypeDef MX25L_ReadByte(uint8_t *pu8RecvData)
{
    uint8_t u8DataSend = MX25L_DUMMY;

    return HAL_SPI_TransmitReceive(&hspi1, &u8DataSend, pu8RecvData, 1, 100);
}

//============================================================================//
// @brief  Initialize an instance of MX25L handling struct.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_Init(void)
{
    uint8_t u8Add = 0x00;

    mx25lHandle = &mx25lInstance;

    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Read manufacture and ID command
    if (MX25L_WriteByte((uint8_t)MX25L_REMS) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Send 1 dummy byte
    if (MX25L_WriteByte((uint8_t)MX25L_DUMMY) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Send 1 dummy byte
    if (MX25L_WriteByte((uint8_t)MX25L_DUMMY) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Send ADD byte
    if (MX25L_WriteByte(u8Add) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Read manufacturer code
    if (MX25L_ReadByte(&mx25lHandle->u8MX25L_Manufacturer) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (bMX25L_ReportErrFlag == FALSE)
    {
        // RTOSprintf("\r\n> MX25L manufacturer code:0x%02x", mx25lHandle->u8MX25L_Manufacturer);
    }

    // Read device ID
    if (MX25L_ReadByte(&mx25lHandle->u8MX25L_DeviceID) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (bMX25L_ReportErrFlag == FALSE)
    {
        // RTOSprintf("\r\n> MX25L manufacturer device ID:0x%02x", mx25lHandle->u8MX25L_DeviceID);
    }

    MX25L_Deselect();

    // Read manufacturer ID must match with default manufacturer ID
    if (mx25lHandle->u8MX25L_Manufacturer != MX25L_MANUFACTURER_ID)
    {
        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_MATCH_ERROR;
    }

    // Set parameters for struct instance
    // Device is MX25L3233F
    if (mx25lHandle->u8MX25L_DeviceID == MX25L3233F_ID)
    {
        mx25lHandle->u8MX25L_Type = MX25L3233F;

        mx25lHandle->u8MX25L_Memory = MX25L_MEMORY_TYPE;

        mx25lHandle->u8MX25L_Capacity = MX25L3233F_CAPACITY;

        mx25lHandle->u32MX25L_MemSize = MX25L3233F_MEM_SIZE;

        mx25lHandle->u16MX25L_PageNum = MX25L3233F_PAGE_NUM;

        mx25lHandle->u16MX25L_SectorNum = MX25L3233F_SECTOR_NUM;

        mx25lHandle->u16MX25L_B32Num = MX25L3233F_BLOCK32_NUM;

        mx25lHandle->u8MX25L_B64Num = MX25L3233F_BLOCK64_NUM;
    }
    // Device is MX25L6433F
    else if (mx25lHandle->u8MX25L_DeviceID == MX25L6433F_ID)
    {
        mx25lHandle->u8MX25L_Type = MX25L6433F;

        mx25lHandle->u8MX25L_Memory = MX25L_MEMORY_TYPE;

        mx25lHandle->u8MX25L_Capacity = MX25L6433F_CAPACITY;

        mx25lHandle->u32MX25L_MemSize = MX25L6433F_MEM_SIZE;

        mx25lHandle->u16MX25L_PageNum = MX25L6433F_PAGE_NUM;

        mx25lHandle->u16MX25L_SectorNum = MX25L6433F_SECTOR_NUM;

        mx25lHandle->u16MX25L_B32Num = MX25L6433F_BLOCK32_NUM;

        mx25lHandle->u8MX25L_B64Num = MX25L6433F_BLOCK64_NUM;
    }
    else
    {
        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_MATCH_ERROR;
    }

    mx25lHandle->MX25L_LockState = MX25L_BLOCK_UNLOCK;

    mx25lHandle->MX25L_PowerState = MX25L_POWER_NORMAL;

    return MX25L_OK;
}

//============================================================================//
// @brief  Read status register.
// @param  pu8Stat: Pointer to a uint8_t variable to store the Status register value.
//         pu8Conf: Pointer to a uint8_t variable to store the Configuration
//                  register value.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_ReadStatConf(uint8_t *pu8Stat, uint8_t *pu8Conf)
{
    uint8_t u8StatRegVal1, u8StatRegVal2,
        u8ConfRegVal1, u8ConfRegVal2;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Read status register command
    if (MX25L_WriteByte((uint8_t)MX25L_RDSR) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Read status register 2 times
    if (MX25L_ReadByte(&u8StatRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (MX25L_ReadByte(&u8StatRegVal2) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (u8StatRegVal1 != u8StatRegVal2)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_MATCH_ERROR;
    }

    // Read config register command
    if (MX25L_WriteByte((uint8_t)MX25L_RDCR) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Read config register 2 times
    if (MX25L_ReadByte(&u8ConfRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (MX25L_ReadByte(&u8ConfRegVal2) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_READ_ERROR;
    }

    if (u8ConfRegVal1 != u8ConfRegVal2)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_MATCH_ERROR;
    }

    *pu8Stat = u8StatRegVal1;

    *pu8Conf = u8ConfRegVal1;

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Enable write to flash.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
static MX25L_StatusTypeDef MX25L_WriteEnable(void)
{
    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Write enable command
    if (MX25L_WriteByte((uint8_t)MX25L_WREN) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    return MX25L_OK;
}

//============================================================================//
// @brief  Disable write to flash.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
static MX25L_StatusTypeDef MX25L_WriteDisable(void)
{
    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Write disable command
    if (MX25L_WriteByte((uint8_t)MX25L_WRDI) != HAL_OK)
    {
        MX25L_Deselect();

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    return MX25L_OK;
}

//============================================================================//
// @brief  Wait until a write or erase operation complete.
// @param  u32Timeout: Maximum waiting time in ms.
// @retval TRUE: Operation completed before timeout.
//         FALSE: Function returns when timeout.
//============================================================================//
static bool MX25L_WaitOperation(uint32_t u32Timeout)
{
    uint8_t u8StatRegVal1, u8StatRegVal2;
    uint8_t u8WriteStat = MX25L_SR_WIP;

    MX25L_Deselect();

    HAL_Delay(1);

    do
    {
        MX25L_Select();

        // Read status register command
        if (MX25L_WriteByte((uint8_t)MX25L_RDSR) != HAL_OK)
        {
            MX25L_Deselect();

            HAL_Delay(1);

            u32Timeout--;

            continue;
        }

        // Read status register 2 times
        if (MX25L_ReadByte(&u8StatRegVal1) != HAL_OK)
        {
            MX25L_Deselect();

            HAL_Delay(1);

            u32Timeout--;

            continue;
        }

        if (MX25L_ReadByte(&u8StatRegVal2) != HAL_OK)
        {
            MX25L_Deselect();

            HAL_Delay(1);

            u32Timeout--;

            continue;
        }

        if (u8StatRegVal1 != u8StatRegVal2)
        {
            MX25L_Deselect();

            HAL_Delay(1);

            u32Timeout--;

            continue;
        }

        MX25L_Deselect();

        // Get the WIP bit value
        u8WriteStat = u8StatRegVal1 & MX25L_SR_WIP;

        HAL_Delay(1);

        u32Timeout--;
    } while (u8WriteStat && u32Timeout); // Do while WIP bit is still set
    // or timeout is not reached

    // If timeout is reached
    if (u32Timeout == 0)
    {
        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return FALSE;
    }

    else
        return TRUE;
}

//============================================================================//
// @brief  Lock the flash in unit of blocks.
// @param  mx25lLockLevel: Level of lock.
//         bLockBot: 1: Lock from bottom, 0: Lock from top.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_BlockLock(MX25L_BlockLevelTypeDef mx25lLockLevel, bool bLockBot)
{
    uint8_t u8StatRegVal1, u8ConfRegVal1;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    u8StatRegVal1 = 0x02;

    // Set the corresponding BP bits
    u8StatRegVal1 |= (mx25lLockLevel << MX25L_SR_BP_SHIFT);

    // If lock from bottom
    if (bLockBot)
        // Set TB bit
        u8ConfRegVal1 = MX25L_CR_TB;
    else // Lock from top
        // Reset TB bit
        u8ConfRegVal1 = 0x00;

    // Write enable
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    MX25L_Select();

    // Write stat/conf register command
    if (MX25L_WriteByte((uint8_t)MX25L_WRSR) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write status register value
    if (MX25L_WriteByte((uint8_t)u8StatRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write config register value
    if (MX25L_WriteByte((uint8_t)u8ConfRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until operation completes
    if (!MX25L_WaitOperation(MX25L_WRITE_STATUS_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (MX25L_WriteDisable() != MX25L_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_DIS_ERROR;
    }

    mx25lHandle->MX25L_LockState = mx25lLockLevel;

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Unlock the flash.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_BlockUnlock(void)
{
    uint8_t u8StatRegVal1, u8ConfRegVal1;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    u8StatRegVal1 = 0x02;

    u8ConfRegVal1 = 0x00;

    // Write enable
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    MX25L_Select();

    // Write stat/conf register command
    if (MX25L_WriteByte((uint8_t)MX25L_WRSR) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write status register value
    if (MX25L_WriteByte((uint8_t)u8StatRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write config register value
    if (MX25L_WriteByte((uint8_t)u8ConfRegVal1) != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until operation completes
    if (!MX25L_WaitOperation(MX25L_WRITE_STATUS_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (MX25L_WriteDisable() != MX25L_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_DIS_ERROR;
    }

    mx25lHandle->MX25L_LockState = MX25L_BLOCK_UNLOCK;

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Write within a page.
// @param  u32Addr: Physical address within the page to start program.
//         pu8Dat: Pointer to data buffer for writing.
//         u8DatLen: Length of data for writing.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_PageProgram(uint32_t u32Addr, uint8_t *pu8Dat,
                                      uint8_t u8DatLen)
{
    HAL_StatusTypeDef halRes;

    // Address must be within memory size
    if (u32Addr >= mx25lHandle->u32MX25L_MemSize)
        return MX25L_ADDR_INVALID;

    // Data written from the address must not exceed a page boundary
    if ((u8DatLen + u32Addr % MX25L_PAGE_SIZE) > MX25L_PAGE_SIZE)
        return MX25L_LEN_INVALID;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Write enable
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    HAL_Delay(1);

    MX25L_Select();

    // Page program command
    if (MX25L_WriteByte((uint8_t)MX25L_PP) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Send 24 bits address
    if (MX25L_WriteByte((uint8_t)(u32Addr >> 16)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)(u32Addr >> 8)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)u32Addr) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write data to flash
    // halRes = HAL_SPI_Transmit(&hspi1, pu8Dat, u8DatLen, 0xFF);

    do
    {
        halRes = MX25L_WriteByte(*pu8Dat);

        pu8Dat++;

        u8DatLen--;
    } while ((halRes == HAL_OK) && u8DatLen);

    if (halRes != HAL_OK)
    {
        if (bMX25L_ReportErrFlag == FALSE)
        {
#if ENABLE_DEBUG_MSG
            RTOSprintf("\r\n> HAL SPI Transmit error:%d.", halRes);
#endif
        }

        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_PROGRAM_ERROR;
    }

    MX25L_Deselect();

    // Wait until operation completes
    if (!MX25L_WaitOperation(MX25L_PAGE_PROG_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Write to multiple pages.
// @param  u32Addr: Physical address of the page to start program, must be the
//                  beginning address of a page.
//         pu8Dat: Pointer to data buffer for writing.
//         u32DatLen: Length of data for writing.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_MultiPagesProgram(uint32_t u32PageOffset, uint8_t *pu8Dat,
                                            uint32_t u32DatLen)
{
    MX25L_StatusTypeDef mx25lRes;
    uint32_t u32Addr, u32DatCnt = 0, u32BufSize = 0;

    // Address must be at the beginning of a page and within memory size
    if (u32PageOffset >= mx25lHandle->u16MX25L_PageNum)
        return MX25L_ADDR_INVALID;

    u32Addr = __MX25L_PAGE_ADDR(u32PageOffset);

    // Amount of data for writing must not exceed memory size
    if ((u32Addr + u32DatLen) > mx25lHandle->u32MX25L_MemSize)
        return MX25L_LEN_INVALID;

    do
    {
        // Get the amount of data corresponding to a page
        u32BufSize = u32DatLen - u32DatCnt;

        if (u32BufSize > MX25L_PAGE_SIZE)
            u32BufSize = MX25L_PAGE_SIZE;

        // Program the current page
        mx25lRes = MX25L_PageProgram(u32Addr, &pu8Dat[u32DatCnt], u32BufSize);

        u32Addr += u32BufSize;

        u32DatCnt += u32BufSize;
    } while ((u32DatCnt < u32DatLen) && (mx25lRes == MX25L_OK));

    if (mx25lRes != MX25L_OK)
    {
        return MX25L_PROGRAM_ERROR;
    }

    return MX25L_OK;
}

//============================================================================//
// @brief  Read from a physical address.
// @param  u32Addr: Physical address to start read from.
//         pu8Dat: Pointer to data buffer for storing read bytes.
//         u32DatLen: Length of data needed to be read.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_MultiBytesRead(uint32_t u32Addr, uint8_t *pu8Dat,
                                         uint32_t u32DatLen)
{
    HAL_StatusTypeDef halRes;

    // Address must be within memory size
    if (u32Addr >= mx25lHandle->u32MX25L_MemSize)
        return MX25L_ADDR_INVALID;

    // The amount of data to read must not exceed memory size
    if ((u32Addr + u32DatLen) > mx25lHandle->u32MX25L_MemSize)
        return MX25L_LEN_INVALID;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Wait for any preceding write or erase operation to complete
    if (!MX25L_WaitOperation(MX25L_CHIP_ERASE_WAIT))
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_PROGRAM_ERROR;
    }

    // If by any change write function is being enabled then disable it
    MX25L_WriteDisable();

    HAL_Delay(1);

    MX25L_Select();

    // Read data bytes command
    if (MX25L_WriteByte((uint8_t)MX25L_READ) != HAL_OK)
    {
        MX25L_Deselect();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write 24 bits address
    if (MX25L_WriteByte((uint8_t)(u32Addr >> 16)) != HAL_OK)
    {
        MX25L_Deselect();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)(u32Addr >> 8)) != HAL_OK)
    {
        MX25L_Deselect();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)u32Addr) != HAL_OK)
    {
        MX25L_Deselect();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // uint32_t u32Cnt = 0;

    do
    {
        // u32Cnt++;

        // Read bytes
        halRes = MX25L_ReadByte(pu8Dat++);

        u32DatLen--;
    } while (u32DatLen && (halRes == HAL_OK));

    MX25L_Deselect();

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        if (bMX25L_ReportErrFlag == FALSE)
        {
            // RTOSprintf("\r\n> Reading %d byte fail with HAL error code:%d.", halRes);
        }

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_GET_ERROR;
    }
    else
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        return MX25L_OK;
    }
}

//============================================================================//
// @brief  Erase a sector.
// @param  u16SectorOffset: Offset address of the sector to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_SectorErase(uint16_t u16SectorOffset)
{
    uint32_t u32Addr;

    // Offset address must be within the number of sectors
    if (u16SectorOffset >= mx25lHandle->u16MX25L_SectorNum)
        return MX25L_ADDR_INVALID;

    // Convert to physical address
    u32Addr = __MX25L_SECT_ADDR(u16SectorOffset);

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Enable write
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    HAL_Delay(1);

    MX25L_Select();

    // Sector erase command
    if ((MX25L_WriteByte((uint8_t)MX25L_SE)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bMX25L_ReportErrFlag == FALSE)
        {
            // RTOSprintf("\r\n> Write sector erase command error with error code %d.", halRes);
        }

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write 24 bits address
    if ((MX25L_WriteByte((uint8_t)(u32Addr >> 16))) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bMX25L_ReportErrFlag == FALSE)
        {
            // RTOSprintf("\r\n> Write first sector address error with error code %d.", halRes);
        }

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if ((MX25L_WriteByte((uint8_t)(u32Addr >> 8))) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bMX25L_ReportErrFlag == FALSE)
        {
            // RTOSprintf("\r\n> Write second sector address error with error code %d.", halRes);
        }

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if ((MX25L_WriteByte((uint8_t)u32Addr)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bMX25L_ReportErrFlag == FALSE)
        {
            // RTOSprintf("\r\n> Write third sector address error with error code %d.", halRes);
        }

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the operation completes
    if (!MX25L_WaitOperation(MX25L_SECT_ERASE_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Erase multiple sectors.
// @param  u16SectorOffset: Offset address of the sector to start erase.
//         u16SectorNum: Number of sectors to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_MultiSectorsErase(uint16_t u16SectorOffset,
                                            uint16_t u16SectorNum)
{
    MX25L_StatusTypeDef mx25lRes;

    // The offset address must be within the number of sectors
    if (u16SectorOffset >= mx25lHandle->u16MX25L_SectorNum)
        return MX25L_ADDR_INVALID;

    // The number of sector to erase must not exceed the sectors number boundary
    if ((u16SectorOffset + u16SectorNum) >= mx25lHandle->u16MX25L_SectorNum)
        return MX25L_LEN_INVALID;

    do
    {
        // Erase current sector and increase offset address by 1
        mx25lRes = MX25L_SectorErase(u16SectorOffset++);

        u16SectorNum--;
    } while ((mx25lRes == MX25L_OK) && u16SectorNum);

    if (mx25lRes == MX25L_OK)
        return MX25L_OK;
    else
    {
        if (bMX25L_ReportErrFlag == FALSE)
        {
            RTOSprintf("\r\n> Erase error code:%d", mx25lRes);
        }

        return MX25L_ERASE_ERROR;
    }
}

//============================================================================//
// @brief  Erase a 32K block.
// @param  u16B32Offset: Offset address of the 32K block to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_Block32Erase(uint16_t u16B32Offset)
{
    uint32_t u32Addr;

    // The offset address must be within the number of 32K blocks
    if (u16B32Offset >= mx25lHandle->u16MX25L_B32Num)
        return MX25L_ADDR_INVALID;

    // Convert to physical address
    u32Addr = __MX25L_B32_ADDR(u16B32Offset);

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Enable write
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    HAL_Delay(1);

    MX25L_Select();

    // 32K block erase command
    if (MX25L_WriteByte((uint8_t)MX25L_BE32) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write 24 bits address
    if (MX25L_WriteByte((uint8_t)(u32Addr >> 16)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)(u32Addr >> 8)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)u32Addr) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the operation completes
    if (!MX25L_WaitOperation(MX25L_B32_ERASE_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Erase multiple 32K blocks.
// @param  u16B32Offset: Offset address of the 32K block to start erase.
//         u16B32Num: Number of 32K blocks to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_MultiBlock32Erase(uint16_t u16B32Offset,
                                            uint16_t u16B32Num)
{
    MX25L_StatusTypeDef mx25lRes;

    // The offset address must be within the number of 32K blocks
    if (u16B32Offset >= mx25lHandle->u16MX25L_B32Num)
        return MX25L_ADDR_INVALID;

    // The number of 32K blocks to erase must not exceed the 32K block number boundary
    if ((u16B32Offset + u16B32Num) >= mx25lHandle->u16MX25L_B32Num)
        return MX25L_LEN_INVALID;

    do
    {
        // Erase the current 32K block and increase the offset address by 1
        mx25lRes = MX25L_Block32Erase(u16B32Offset++);

        u16B32Num--;
    } while ((mx25lRes == MX25L_OK) && u16B32Num);

    if (mx25lRes == MX25L_OK)
        return MX25L_OK;
    else
        return MX25L_ERASE_ERROR;
}

//============================================================================//
// @brief  Erase a 64K block.
// @param  u8B64Offset: Offset address of the 64K block to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_Block64Erase(uint8_t u8B64Offset)
{
    uint32_t u32Addr;

    // The offset address must be within the number of 64K blocks
    if (u8B64Offset >= mx25lHandle->u8MX25L_B64Num)
        return MX25L_ADDR_INVALID;

    // Convert to offset address
    u32Addr = __MX25L_B64_ADDR(u8B64Offset);

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Write enable
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    HAL_Delay(1);

    MX25L_Select();

    // 64K block erase command
    if (MX25L_WriteByte((uint8_t)MX25L_BE64) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Write 24 bits address
    if (MX25L_WriteByte((uint8_t)(u32Addr >> 16)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)(u32Addr >> 8)) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    if (MX25L_WriteByte((uint8_t)u32Addr) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the operation completes
    if (!MX25L_WaitOperation(MX25L_B64_ERASE_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Erase multiple 64K blocks.
// @param  u8B64Offset: Offset address of the 64K block to start erase.
//         u8B64Num: Number of 64K blocks to erase.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_MultiBlock64Erase(uint8_t u8B64Offset,
                                            uint8_t u8B64Num)
{
    MX25L_StatusTypeDef mx25lRes;

    // The offset address must be within the number of 64K blocks
    if (u8B64Offset >= mx25lHandle->u8MX25L_B64Num)
        return MX25L_ADDR_INVALID;

    // The number of 32K blocks to erase must not exceed the 32K block number boundary
    if ((u8B64Offset + u8B64Num) >= mx25lHandle->u8MX25L_B64Num)
        return MX25L_LEN_INVALID;

    do
    {
        // Erase the current 64K block and increase the offset address by 1
        mx25lRes = MX25L_Block64Erase(u8B64Offset++);

        u8B64Num--;
    } while ((mx25lRes == MX25L_OK) && u8B64Num);

    if (mx25lRes == MX25L_OK)
        return MX25L_OK;
    else
        return MX25L_ERASE_ERROR;
}

//============================================================================//
// @brief  Erase the whole chip.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_ChipErase(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Write enable
    if (MX25L_WriteEnable() != MX25L_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WR_EN_ERROR;
    }

    HAL_Delay(1);

    MX25L_Select();

    // Chip erase command
    if (MX25L_WriteByte((uint8_t)MX25L_CE) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the operation completes
    if (!MX25L_WaitOperation(MX25L_CHIP_ERASE_WAIT))
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WAIT_ERROR;
    }

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Enter deep power down mode.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_PwrDownEnter(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Enter deep power down command
    if (MX25L_WriteByte((uint8_t)MX25L_DP) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the state transition completes
    HAL_Delay(MX25L_ENT_PWR_DOWN_WAIT);

    mx25lHandle->MX25L_PowerState = MX25L_DEEP_POWER_DOWN;

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Release from deep power down mode.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_PwrDownRelease(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    MX25L_Deselect();

    HAL_Delay(1);

    MX25L_Select();

    // Release from deep power down command
    if (MX25L_WriteByte((uint8_t)MX25L_RDP) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the state transtion completes
    HAL_Delay(MX25L_RLS_PWR_DOWN_WAIT);

    mx25lHandle->MX25L_PowerState = MX25L_POWER_NORMAL;

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}

//============================================================================//
// @brief  Software reset.
// @param  None.
// @retval Operation status in MX25L_StatusTypeDef format.
//============================================================================//
MX25L_StatusTypeDef MX25L_Reset(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_spi1_user, S_2_mS(10));

        if (osRes != osOK)
            return MX25L_TIMEOUT;
    }

    // Wait for any preceding write or erase operation to complete
    if (!MX25L_WaitOperation(MX25L_CHIP_ERASE_WAIT))
    {
        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_PROGRAM_ERROR;
    }

    // If by any change write function is being enabled then disable it
    MX25L_WriteDisable();

    HAL_Delay(1);

    MX25L_Select();

    // Reset enable command
    if (MX25L_WriteByte((uint8_t)MX25L_RSTEN) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    // Software reset command
    if (MX25L_WriteByte((uint8_t)MX25L_RST) != HAL_OK)
    {
        MX25L_WriteDisable();

        if (bRTOS_En)
            osMutexRelease(mutex_spi1_user);

        u8MX25L_ErrCnt++;

        if (u8MX25L_ErrCnt >= MX25L_ERR_CNT_MAX)
        {
            u8MX25L_ErrCnt = 0;

            if (bMX25L_ReportErrFlag == FALSE)
            {
                RTOSprintf("\r\n> Maximum MX25L errors reached. Restart IC MX25L.");

                MX25L_Restart();
            }
        }

        return MX25L_WRITE_ERROR;
    }

    MX25L_Deselect();

    // Wait until the operation completes
    HAL_Delay(MX25L_RST_WAIT);

    if (bRTOS_En)
        osMutexRelease(mutex_spi1_user);

    return MX25L_OK;
}
