//============================================================================//
// User Includes
//============================================================================//
#include "LSM6DSL.h"

#include <string.h>

#include "System.h"

#include "EXTIO.h"

#include "math.h"

#include "MG82_Driver.h"
//============================================================================//
// User Variables
//============================================================================//
ACCELERO_ConfigTypeDef tAclrConfig = {0};
GYRO_ConfigTypeDef tGyroConfig = {0};
ACCELERO_InterruptConfigTypeDef tAlrIntConfig = {0};
LSM6DSL_AccDataTypeDef tAccData = {0};
LSM6DSL_GyroDataTypeDef tGyroData = {0};
ACCELERO_FilterConfigTypeDef tAccFilter = {0};
GYRO_FilterConfigTypeDef tGyroFilterl = {0};
LSM6DSL_IntConfigTypeDef tLSM6DSL_IntConfig = {0};
LSM6DSL_InfoTypeDef tG_SenData = {0};

uint8_t u8LSM6DSL_ErrCnt = 0;
uint32_t u32LSM6DSL_ReportErrLastTick = 0;
bool bLSM6DSL_ReportErrFlag = FALSE;
bool bLSM6DSL_Stop = FALSE;
uint8_t u8Pin = 0;
bool bSetCurrOrien = FALSE;
//============================================================================//
// User Function Prototypes
//============================================================================//
static void
LSM6DSL_I2C_ReInit(void);
static void LSM6DSL_Restart(void);
//============================================================================//
// User Functions
//============================================================================//
//============================================================================//
/*******************************************************************************
 * Function Name  : Addr_Incr
 * Description    : Write IF_INC
 * Input          : Data of LSM6DSL_ACC_GYRO_IF_INC_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_IF_Addr_Incr(LSM6DSL_ACC_GYRO_IF_INC_t tData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_IF_INC_MASK;
    u8Tmp |= tData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_BDU
 * Description    : Write BDU
 * Input          : Data of BDU
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_BDU(LSM6DSL_ACC_GYRO_BDU_t tBDUData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_BDU_MASK;
    u8Tmp |= tBDUData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}

//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_BDU
 * Description    : Write BDU
 * Input          : Data of BDU
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_HACTIVE(LSM6DSL_ACC_GYRO_H_ACTIVE_t tHACtiveData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_H_ACTIVE_MASK;
    u8Tmp |= tHACtiveData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL3_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_FIFO_MODE
 * Description    : Write FIFO
 * Input          : Data of fifo
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//

bool LSM6DSL_ACC_GYRO_FIFO_MODE(LSM6DSL_ACC_GYRO_FIFO_MODE_t tfifodata)
{

    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_FIFO_CTRL5, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_FIFO_MODE_MASK;
    u8Tmp |= tfifodata;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_FIFO_CTRL5, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_ODR_XL
 * Description    : Write ODR
 * Input          : Data of ODR
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_t tODRData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL1_XL, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_ODR_XL_MASK;
    u8Tmp |= tODRData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL1_XL, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_FS_XL
 * Description    : Write fsXL for acceleration
 * Input          : Data of FsXLData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_FS_XL(LSM6DSL_ACC_GYRO_FS_XL_t tFsXLData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL1_XL, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_FS_XL_MASK;
    u8Tmp |= tFsXLData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL1_XL, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_ODR_G
 * Description    : Write ODRData
 * Input          : Data of tODRData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_ODR_G(LSM6DSL_ACC_GYRO_ODR_G_t tODRData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_ODR_G_MASK;
    u8Tmp |= tODRData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}

//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_FS_125
 * Description    : Write tFs125Data
 * Input          : Data of tFs125Data
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_FS_125(LSM6DSL_ACC_GYRO_FS_125_t tFs125Data)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_FS_125_MASK;
    u8Tmp |= tFs125Data;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_FS_G
 * Description    : Write tFsGData
 * Input          : Data of tFsGData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_FS_G(LSM6DSL_ACC_GYRO_FS_G_t tFsGData)
{
    uint8_t u8Tmp = 0;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_FS_G_MASK;
    u8Tmp |= tFsGData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_SetFullScaleXL
 * Description    : Write fullScale
 * Input          : Data of tFsGData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_SetFullScaleXL(float ffullScale)
{
    LSM6DSL_ACC_GYRO_FS_XL_t tTemp;

    tTemp = (ffullScale <= 2.0f)   ? LSM6DSL_ACC_GYRO_FS_XL_2g
            : (ffullScale <= 4.0f) ? LSM6DSL_ACC_GYRO_FS_XL_4g
            : (ffullScale <= 8.0f) ? LSM6DSL_ACC_GYRO_FS_XL_8g
                                   : LSM6DSL_ACC_GYRO_FS_XL_16g;
    if (LSM6DSL_ACC_GYRO_FS_XL(tTemp) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_SetFullScale_G
 * Description    : Write fullScale
 * Input          : Data of tFsGData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_SetFullScale_G(float ffullScale)
{
    LSM6DSL_ACC_GYRO_FS_G_t tempData;
    if (ffullScale <= 125.0f)
    {
        if (LSM6DSL_ACC_GYRO_FS_125(LSM6DSL_ACC_GYRO_FS_125_ENABLED) == FALSE)
        {
            return FALSE;
        }
    }
    else
    {
        tempData = (ffullScale <= 250.0f)    ? LSM6DSL_ACC_GYRO_FS_G_250dps
                   : (ffullScale <= 500.0f)  ? LSM6DSL_ACC_GYRO_FS_G_500dps
                   : (ffullScale <= 1000.0f) ? LSM6DSL_ACC_GYRO_FS_G_1000dps
                                             : LSM6DSL_ACC_GYRO_FS_G_2000dps;

        if (LSM6DSL_ACC_GYRO_FS_125(LSM6DSL_ACC_GYRO_FS_125_DISABLED) == FALSE)
        {
            return FALSE;
        }
        if (LSM6DSL_ACC_GYRO_FS_G(tempData) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}
//============================================================================//
// @brief  LSM6DSL Accelerometer Initialization.
// @param  tAccInit: Initialization data.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_AccInit(ACCELERO_ConfigTypeDef *pAccInit)
{
    uint8_t u8Tmp = 0x00;
    bool bRes = FALSE;

    LSM6DSL_ACC_GYRO_ODR_XL_t todrData;

    // Read CTRL6_C
    bRes = LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL6_C, &u8Tmp);

    if (bRes == FALSE)
        return FALSE;

    // Set high-performance mode selection
    if (pAccInit->bHighPerf)
        u8Tmp &= ~LSM6DSL_CTRL6_XL_HM_BIT; // Enable
    else
        u8Tmp |= LSM6DSL_CTRL6_XL_HM_BIT; // Disable

    // Write back control register
    bRes = LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL6_C, u8Tmp);

    if (bRes == FALSE)
        return FALSE;

    if (pAccInit->bEnable)
    {
        todrData = (pAccInit->u8OutputDataRate <= 13.0f)     ? LSM6DSL_ACC_GYRO_ODR_XL_13Hz
                   : (pAccInit->u8OutputDataRate <= 26.0f)   ? LSM6DSL_ACC_GYRO_ODR_XL_26Hz
                   : (pAccInit->u8OutputDataRate <= 52.0f)   ? LSM6DSL_ACC_GYRO_ODR_XL_52Hz
                   : (pAccInit->u8OutputDataRate <= 104.0f)  ? LSM6DSL_ACC_GYRO_ODR_XL_104Hz
                   : (pAccInit->u8OutputDataRate <= 208.0f)  ? LSM6DSL_ACC_GYRO_ODR_XL_208Hz
                   : (pAccInit->u8OutputDataRate <= 416.0f)  ? LSM6DSL_ACC_GYRO_ODR_XL_416Hz
                   : (pAccInit->u8OutputDataRate <= 833.0f)  ? LSM6DSL_ACC_GYRO_ODR_XL_833Hz
                   : (pAccInit->u8OutputDataRate <= 1660.0f) ? LSM6DSL_ACC_GYRO_ODR_XL_1660Hz
                   : (pAccInit->u8OutputDataRate <= 3330.0f) ? LSM6DSL_ACC_GYRO_ODR_XL_3330Hz
                                                             : LSM6DSL_ACC_GYRO_ODR_XL_6660Hz;
        if (LSM6DSL_ACC_GYRO_ODR_XL(todrData) == FALSE)
        {
            return FALSE;
        }
    }
    return TRUE;
}
//============================================================================//
// @brief  LSM6DSL Gyro Initialization.
// @param  tGyroInit: Initialization data.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_GyroInit(GYRO_ConfigTypeDef *pGyroInit)
{
    uint8_t u8Tmp = 0x00;
    bool bRes = FALSE;
    LSM6DSL_ACC_GYRO_ODR_G_t todrData;
    // // Read CTRL6_C
    bRes = LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL7_G, &u8Tmp);

    if (bRes == FALSE)
        return FALSE;

    // Set high-performance mode selection
    if (pGyroInit->bHighPerf)
        u8Tmp &= ~LSM6DSL_CTRL7_GYRO_HM_BIT; // Enable
    else
        u8Tmp |= LSM6DSL_CTRL7_GYRO_HM_BIT; // Disable

    // Write back control register
    bRes = LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL7_G, u8Tmp);

    if (bRes == FALSE)
        return FALSE;

    if (pGyroInit->bEnable)
    {
        todrData = (pGyroInit->u8OutputDataRate <= 13.0f)     ? LSM6DSL_ACC_GYRO_ODR_G_13Hz
                   : (pGyroInit->u8OutputDataRate <= 26.0f)   ? LSM6DSL_ACC_GYRO_ODR_G_26Hz
                   : (pGyroInit->u8OutputDataRate <= 52.0f)   ? LSM6DSL_ACC_GYRO_ODR_G_52Hz
                   : (pGyroInit->u8OutputDataRate <= 104.0f)  ? LSM6DSL_ACC_GYRO_ODR_G_104Hz
                   : (pGyroInit->u8OutputDataRate <= 208.0f)  ? LSM6DSL_ACC_GYRO_ODR_G_208Hz
                   : (pGyroInit->u8OutputDataRate <= 416.0f)  ? LSM6DSL_ACC_GYRO_ODR_G_416Hz
                   : (pGyroInit->u8OutputDataRate <= 833.0f)  ? LSM6DSL_ACC_GYRO_ODR_G_833Hz
                   : (pGyroInit->u8OutputDataRate <= 1660.0f) ? LSM6DSL_ACC_GYRO_ODR_G_1660Hz
                   : (pGyroInit->u8OutputDataRate <= 3330.0f) ? LSM6DSL_ACC_GYRO_ODR_G_3330Hz
                                                              : LSM6DSL_ACC_GYRO_ODR_G_6660Hz;
    }
    u8Tmp = (uint8_t)todrData;
    // // Write back control register
    bRes = LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, u8Tmp);

    if (bRes == FALSE)
        return FALSE;

    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_FUNC_EN
 * Description    : Write tEnable
 * Input          : Data of tEnable
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_t tEnable)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_FUNC_EN_MASK;
    u8Tmp |= tEnable;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_WRIST_TILT
 * Description    : Write tRegData
 * Input          : Data of tRegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_WRIST_TILT(LSM6DSL_ACC_GYRO_WRIST_TILT_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_WRIST_MASK;
    u8Tmp |= tRegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_EN_TILT_CAL
 * Description    : Write tEnable
 * Input          : Data of tEnable
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TILT_CAL(LSM6DSL_ACC_GYRO_TILT_G_t tEnable)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_TILT_MASK;
    u8Tmp |= tEnable;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TiltEvOnInt1
 * Description    : Write tEnable
 * Input          : Data of tEnable
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TiltEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TILT_t tEnable)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT1_TILT_MASK;
    u8Tmp |= tEnable;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TiltEvOnInt2
 * Description    : Write tEnable
 * Input          : Data of tEnable
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TiltEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TILT_t tEnable)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT2_TILT_MASK;
    u8Tmp |= tEnable;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_WAKE_DUR
 * Description    : Write tEnable
 * Input          : Data of tEnable
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_WAKE_DUR(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_WAKE_DUR_POSITION;
    u8RegData &= LSM6DSL_ACC_GYRO_WAKE_DUR_MASK;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_DUR, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_WAKE_DUR_MASK;
    u8Tmp |= u8RegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_DUR, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_WK_THS
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_WK_THS(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_WK_THS_POSITION;
    u8RegData &= LSM6DSL_ACC_GYRO_WK_THS_MASK;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_THS, &u8Tmp))
    {
        return FALSE;
    }

    u8Tmp &= ~LSM6DSL_ACC_GYRO_WK_THS_MASK;
    u8Tmp |= u8RegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_THS, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_BASIC_INT
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_INT_EN_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT_EN_MASK;
    u8Tmp |= tRegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_WUEvOnInt1
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_WUEvOnInt1(LSM6DSL_ACC_GYRO_INT1_WU_t u8RegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT1_WU_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_WUEvOnInt2
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_WUEvOnInt2(LSM6DSL_ACC_GYRO_INT2_WU_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT2_WU_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_EmbeddedAccess
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMB_ACC_t u8RegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_FUNC_CFG_ACCESS, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_EMB_BANKB_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_FUNC_CFG_ACCESS, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_PedoThreshold
 * Description    : Write u8ThresholdValue
 * Input          : Data of u8ThresholdValue
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_PedoThreshold(uint8_t u8ThresholdValue)
{
    uint8_t u8Tmp;
    /* Open Embedded Function Register page*/
    LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_BANKB_ENABLED);

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CONFIG_PEDO_THS_MIN, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~0x1F;
    u8Tmp |= (u8ThresholdValue & 0x1F);

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CONFIG_PEDO_THS_MIN, u8Tmp))
    {
        return FALSE;
    }
    LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_DISABLED);
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_PEDO // Pedometer algorithm
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_PEDO(LSM6DSL_ACC_GYRO_PEDO_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_PEDO_MASK;
    u8Tmp |= tRegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL10_C, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_STEP_DET_on_INT1 // Driven to INT1 pin
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_STEP_DET_on_INT1(LSM6DSL_ACC_GYRO_INT1_PEDO_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT1_CTRL, &u8Tmp))
    {
        return FALSE;
    }
    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT1_PEDO_MASK;
    u8Tmp |= tRegData;
    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT1_CTRL, u8Tmp))
    {
        return FALSE;
    }
    return TRUE;
}
//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TAP_X_EN // Driven to INT1 pin
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_t u8RegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_TAP_X_EN_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, u8Tmp))
        return FALSE;

    return TRUE;
}

//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TAP_Y_EN // Driven to INT1 pin
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_t u8RegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_TAP_Y_EN_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, u8Tmp))
        return FALSE;

    return TRUE;
}

//============================================================================//
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TAP_Z_EN // Driven to INT1 pin
 * Description    : Write u8RegData
 * Input          : Data of u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
//============================================================================//
bool LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_t u8RegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_TAP_Z_EN_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_CFG1, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TAP_THS
 * Description    : Write TAP_THS
 * Input          : u8_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_TAP_THS(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_TAP_THS_POSITION; // mask
    u8RegData &= LSM6DSL_ACC_GYRO_TAP_THS_MASK;                 // coerce

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_THS_6D, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_TAP_THS_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_THS_6D, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_SHOCK_Duration
 * Description    : Write TAP_THS
 * Input          : u8_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_SHOCK_Duration(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_SHOCK_POSITION; // mask
    u8RegData &= LSM6DSL_ACC_GYRO_SHOCK_MASK;                 // coerce

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_SHOCK_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, u8Tmp))
        return FALSE;

    return TRUE;
}
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_QUIET_Duration
 * Description    : Write TAP_THS
 * Input          : u8_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_QUIET_Duration(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_QUIET_POSITION; // mask
    u8RegData &= LSM6DSL_ACC_GYRO_QUIET_MASK;                 // coerce

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_QUIET_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_SingleTapOnInt1
 * Description    : Write INT1_SINGLE_TAP
 * Input          : LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_SingleTapOnInt1(LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_t tRegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, u8Tmp))
        return FALSE;

    return TRUE;
}
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_SingleTapOnInt2
 * Description    : Write INT1_SINGLE_TAP
 * Input          : LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_SingleTapOnInt2(LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_t tRegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_DUR
 * Description    : Write LSM6DSL_ACC_GYRO_DUR
 * Input          : u8RegData
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_DUR(uint8_t u8RegData)
{
    uint8_t u8Tmp;

    u8RegData = u8RegData << LSM6DSL_ACC_GYRO_DUR_POSITION; // mask
    u8RegData &= LSM6DSL_ACC_GYRO_DUR_MASK;                 // coerce

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_DUR_MASK;
    u8Tmp |= u8RegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT_DUR2, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_EV
 * Description    : Write SINGLE_DOUBLE_TAP
 * Input          : LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_EV(LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_t tRegData)
{
    uint8_t u8Tmp;

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_THS, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_THS, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TapEvOnInt1
 * Description    : Write LSM6DSL_ACC_GYRO_TapEvOnInt1
 * Input          : LSM6DSL_ACC_GYRO_INT1_TAP_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_TapEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TAP_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT1_TAP_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_TapEvOnInt1
 * Description    : Write LSM6DSL_ACC_GYRO_TapEvOnInt1
 * Input          : LSM6DSL_ACC_GYRO_INT1_TAP_t
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_TapEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TAP_t tRegData)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT2_TAP_MASK;
    u8Tmp |= tRegData;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_AWRIST_TILT_LAT
 * Description    : Write u8WRISTLatch
 * Input          : u8WRISTLatch
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_LAT(uint8_t u8WRISTLatch)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_LAT, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_A_WRIST_TILT_LAT_MASK;
    u8Tmp |= u8WRISTLatch;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_LAT, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS
 * Description    : Write u8WRISTTHS
 * Input          : u8WRISTTHS
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS(uint8_t u8WRISTTHS)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_THS, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS_MASK;
    u8Tmp |= u8WRISTTHS;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_THS, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK
 * Description    : Write u8WRISTMask
 * Input          : u8WRISTMask
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK(LSM6DSL_ACC_GYRO_A_WRIST_MASK_t tWRISTMask)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_MASK, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_MASK;
    u8Tmp |= tWRISTMask;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_MASK, u8Tmp))
        return FALSE;

    return TRUE;
}

/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG
 * Description    : Write tReg
 * Input          : tReg
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG(LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_t tReg)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_G, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_MASK_t;
    u8Tmp |= tReg;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_G, u8Tmp))
        return FALSE;

    return TRUE;
}
/*******************************************************************************
 * Function Name  : LSM6DSL_ACC_GYRO_INT2_on_INT1
 * Description    : Write tReg
 * Input          : tReg
 * Output         : None
 * Return         : Status [TRUE, FALSE]
 *******************************************************************************/
bool LSM6DSL_ACC_GYRO_INT2_on_INT1(LSM6DSL_ACC_GYRO_INT2_on_INT1_t tReg)
{
    uint8_t u8Tmp;
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL4_C, &u8Tmp))
        return FALSE;

    u8Tmp &= ~LSM6DSL_ACC_GYRO_INT2_on_INT1_MASK_t;
    u8Tmp |= tReg;

    if (!LSM6DSL_WriteReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL4_C, u8Tmp))
        return FALSE;

    return TRUE;
}

//============================================================================//
/**
 * @brief Enable the tilt detection for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval LSM6DSL_STATUS_OK in case of success, an error code otherwise
 */
//============================================================================//

bool LSM6DSL_EnableTiltDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Output Data Rate selection
     * tilt operation at 26Hz
     */
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_26Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
    {
        return FALSE;
    }
    /* Enable embedded functionalities */
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable tilt calculation. */
    if (LSM6DSL_ACC_GYRO_TILT_CAL(LSM6DSL_ACC_GYRO_TILT_ENABLED) == FALSE)
    {
        return FALSE;
    }
    // // /* Enable latched */
    // if (LSM6DSL_ACC_GYRO_BASIC_INT((LSM6DSL_ACC_GYRO_INT_EN_t)0x01) == FALSE)
    // {
    // 	return FALSE;
    // }

    // /* Enable basic Interrupts */
    // if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED) == FALSE)
    // {
    // 	return FALSE;
    // }

    /* Enable tilt detection on either INT1 or INT2 pin */
    switch (tIntPin)
    {
    case LSM6DSL_INT1_PIN:
        if (LSM6DSL_ACC_GYRO_TiltEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TILT_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    case LSM6DSL_INT2_PIN:
        if (LSM6DSL_ACC_GYRO_TiltEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TILT_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}
//============================================================================//
/**
 * @brief Enable the AW tilt detection for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval LSM6DSL_STATUS_OK in case of success, an error code otherwise
 */
//============================================================================//
bool LSM6DSL_EnableAWTiltDetection(void)
{
    // Write ox20 to CTRL_XL
    /*tilt operation at 26Hz */
    float fThreSoldAngle = 0.0f;
    uint8_t u8Data = 0;
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_26Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
    {
        return FALSE;
    }

    /*Enable embedded function by write 0x04 to CTRL10_C*/
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }

    HAL_Delay(50);

    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }
    // Write A0 to enable bank B
    if (LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_BANKB_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /*Set new latency*/
    if (LSM6DSL_ACC_GYRO_A_WRIST_TILT_LAT(0x0F) == FALSE)
    {
        return FALSE;
    }

    fThreSoldAngle = (float)(g_Setting.tGS_Setting.u8ThresoldAngle * 3.14) / 180;

    u8Data = (uint8_t)(sin(fThreSoldAngle) * 64);

    if (LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS(u8Data) == FALSE)
    {
        return FALSE;
    }

    if (LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK((LSM6DSL_ACC_GYRO_A_WRIST_MASK_t)(g_Setting.tGS_Setting.u8Axis)) == FALSE)
    {
        return FALSE;
    }

    // Write disable bank B
    if (LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_DISABLED) == FALSE)
    {
        return FALSE;
    }

    // Write 0x84 to CTRL10_C

    // Write 0x80 to CTRL10_C
    if (LSM6DSL_ACC_GYRO_WRIST_TILT(LSM6DSL_ACC_GYRO_WRIST_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /*Enable embedded function by write 0x04 to CTRL10_C*/
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }

    // AWT interrupt to INT2 pin
    if (LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG((LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_t)0x01) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Disable the tilt detection feature for LSM6DSL accelerometer sensor
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisableAWTiltDetection(void)
{

    /* Disable embedded functionalities */
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /*Enable WRITST_TILT_EN*/
    if (LSM6DSL_ACC_GYRO_WRIST_TILT(LSM6DSL_ACC_GYRO_WRIST_DISABLED) == FALSE)
    {
        return FALSE;
    }
    // AWT interrupt to INT2 pin
    if (LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG(LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_DISABLE) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}
/**
 * @brief Disable the tilt detection feature for LSM6DSL accelerometer sensor
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisableTiltDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Disable tilt event on INT1. */
    if (LSM6DSL_ACC_GYRO_TiltEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TILT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable tilt event on INT2. */
    if (LSM6DSL_ACC_GYRO_TiltEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TILT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable tilt calculation. */
    if (LSM6DSL_ACC_GYRO_TILT_CAL(LSM6DSL_ACC_GYRO_TILT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable embedded functionalities */
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Enable the wake up detection for LSM6DSL accelerometer sensor
 * @param int_pin the interrupt pin to be used
 * @note  This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval LSM6DSL_STATUS_OK in case of success, an error code otherwise
 */
bool LSM6DSL_EnableWakeUpDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Output Data Rate selection*/
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_416Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(g_Setting.tGS_Setting.fScale) == FALSE)
    {
        return FALSE;
    }
    /* WAKE_DUR setting */
    if (LSM6DSL_ACC_GYRO_WAKE_DUR(0x00) == FALSE)
    {
        return FALSE;
    }

    uint8_t u8Data = (uint8_t)((g_Setting.tGS_Setting.fThreSold * 64) / (g_Setting.tGS_Setting.fScale));
    /*Set wake up threshold*/
    if (LSM6DSL_ACC_GYRO_WK_THS(u8Data) == FALSE)
    {
        return FALSE;
    }

    /* Enable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable wake up detection on either INT1 or INT2 pin */
    switch (tIntPin)
    {
    case LSM6DSL_INT1_PIN:
        if (LSM6DSL_ACC_GYRO_WUEvOnInt1(LSM6DSL_ACC_GYRO_INT1_WU_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    case LSM6DSL_INT2_PIN:
        if (LSM6DSL_ACC_GYRO_WUEvOnInt2(LSM6DSL_ACC_GYRO_INT2_WU_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Disable the wake up feature for LSM6DSL accelerometer sensor
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisableWakeUpDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Disable wake up event on INT1 */
    if (LSM6DSL_ACC_GYRO_WUEvOnInt1(LSM6DSL_ACC_GYRO_INT1_WU_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable wake up event on INT2 */
    if (LSM6DSL_ACC_GYRO_WUEvOnInt2((LSM6DSL_ACC_GYRO_INT2_WU_DISABLED)) == FALSE)
    {
        return FALSE;
    }

    /* Disable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* WU_DUR setting */
    if (LSM6DSL_ACC_GYRO_WAKE_DUR(0x00) == FALSE)
    {
        return FALSE;
    }

    /* WU_THS setting */
    if (LSM6DSL_ACC_GYRO_WK_THS(0x00) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Enable the pedometer feature for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval LSM6DSL_STATUS_OK in case of success, an error code otherwise
 */
bool LSM6DSL_EnablePedometer(void)
{
    /* Output Data Rate selection*/
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_26Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
    {
        return FALSE;
    }
    /* Set pedometer threshold. */
    if (LSM6DSL_ACC_GYRO_PedoThreshold(LSM6DSL_PEDOMETER_THRESHOLD_MID_HIGH))
    {
        return FALSE;
    }
    /* Enable embedded functionalities. */
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable pedometer algorithm. */
    if (LSM6DSL_ACC_GYRO_PEDO(LSM6DSL_ACC_GYRO_PEDO_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable pedometer on INT1. */
    if (LSM6DSL_ACC_GYRO_STEP_DET_on_INT1(LSM6DSL_ACC_GYRO_INT1_PEDO_ENABLED) == FALSE)
    {
        return FALSE;
    }
    return TRUE;
}
/**
 * @brief Disable the pedometer feature for LSM6DSL accelerometer sensor
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisablePedometer(void)
{
    /* Disable pedometer on INT1. */
    if (LSM6DSL_ACC_GYRO_STEP_DET_on_INT1(LSM6DSL_ACC_GYRO_INT1_PEDO_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable pedometer algorithm. */
    if (LSM6DSL_ACC_GYRO_PEDO(LSM6DSL_ACC_GYRO_PEDO_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable embedded functionalities. */
    if (LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Set pedometer threshold. */
    if (LSM6DSL_ACC_GYRO_PedoThreshold(0x00))
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Enable the single tap feature for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_EnableSingleTapDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Output Data Rate selection*/
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_416Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
    {
        return FALSE;
    }
    /* Enable X direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Enable Y direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Enable Z direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Set tap threshold. */
    if (LSM6DSL_ACC_GYRO_TAP_THS(LSM6DSL_TAP_THRESHOLD_MID_LOW) == FALSE)
    {
        return FALSE;
    }
    /* Set tap shock time window. */
    if (LSM6DSL_ACC_GYRO_SHOCK_Duration(LSM6DSL_TAP_SHOCK_TIME_MID_HIGH) == FALSE)
    {
        return FALSE;
    }
    /* Set tap quiet time window. */
    if (LSM6DSL_ACC_GYRO_QUIET_Duration(LSM6DSL_TAP_QUIET_TIME_MID_LOW) == FALSE)
    {
        return FALSE;
    }
    /* Enable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable wake up detection on either INT1 or INT2 pin */
    switch (tIntPin)
    {
    case LSM6DSL_INT1_PIN:
        if (LSM6DSL_ACC_GYRO_SingleTapOnInt1(LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    case LSM6DSL_INT2_PIN:
        if (LSM6DSL_ACC_GYRO_SingleTapOnInt2(LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    default:
        return FALSE;
    }
    return TRUE;
}
/**
 * @brief Disable the Single feature for LSM6DSL accelerometer sensor
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisableSingleTapDetection(void)
{
    /* Disable single tap interrupt on INT1 pin. */
    if (LSM6DSL_ACC_GYRO_SingleTapOnInt1(LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable single tap interrupt on INT2 pin. */
    if (LSM6DSL_ACC_GYRO_SingleTapOnInt2(LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Reset tap threshold. */
    if (LSM6DSL_ACC_GYRO_TAP_THS(0x0) == FALSE)
    {
        return FALSE;
    }

    /* Reset tap shock time window. */
    if (LSM6DSL_ACC_GYRO_SHOCK_Duration(0x0) == FALSE)
    {
        return FALSE;
    }

    /* Reset tap quiet time window. */
    if (LSM6DSL_ACC_GYRO_QUIET_Duration(0x0) == FALSE)
    {
        return FALSE;
    }

    /* _NOTE_: Tap duration time window - don't care for single tap. */

    /* _NOTE_: Single/Double Tap event - don't care of this flag for single tap. */

    /* Disable Z direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable Y direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable X direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief Enable the double tap feature for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_EnableDoubleTapDetection(LSM6DSL_Interrupt_Pin_t tIntPin)
{
    /* Output Data Rate selection*/
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_416Hz) == FALSE)
    {
        return FALSE;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
    {
        return FALSE;
    }
    /* Enable X direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Enable Y direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Enable Z direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_ENABLED) == FALSE)
    {
        return FALSE;
    }
    /* Set tap threshold. */
    if (LSM6DSL_ACC_GYRO_TAP_THS(LSM6DSL_TAP_THRESHOLD_MID_LOW) == FALSE)
    {
        return FALSE;
    }
    /* Set tap shock time window. */
    if (LSM6DSL_ACC_GYRO_SHOCK_Duration(LSM6DSL_TAP_SHOCK_TIME_MID_HIGH) == FALSE)
    {
        return FALSE;
    }
    /* Set tap quiet time window. */
    if (LSM6DSL_ACC_GYRO_QUIET_Duration(LSM6DSL_TAP_QUIET_TIME_HIGH) == FALSE)
    {
        return FALSE;
    }
    /* Set tap duration time window. */
    if (LSM6DSL_ACC_GYRO_DUR(LSM6DSL_TAP_DURATION_TIME_MID) == FALSE)
    {
        return FALSE;
    }

    /* Single and double tap enabled. */
    if (LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_EV(LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_DOUBLE_TAP) == FALSE)
    {
        return FALSE;
    }
    /* Enable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED) == FALSE)
    {
        return FALSE;
    }

    /* Enable double tap on either INT1 or INT2 pin */
    switch (tIntPin)
    {
    case LSM6DSL_INT1_PIN:
        if (LSM6DSL_ACC_GYRO_TapEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TAP_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    case LSM6DSL_INT2_PIN:
        if (LSM6DSL_ACC_GYRO_TapEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TAP_ENABLED) == FALSE)
        {
            return FALSE;
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Enable the double tap feature for LSM6DSL accelerometer sensor
 * @note This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval TRUE in case of success, an error code otherwise
 */
bool LSM6DSL_DisableDoubleTapDetection(void)
{
    /* Disable double tap interrupt on INT1 pin. */
    if (LSM6DSL_ACC_GYRO_TapEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TAP_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable double tap interrupt on INT2 pin. */
    if (LSM6DSL_ACC_GYRO_TapEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TAP_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable basic Interrupts */
    if (LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_BASIC_INT_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Set tap threshold. */
    if (LSM6DSL_ACC_GYRO_TAP_THS(0x00) == FALSE)
    {
        return FALSE;
    }

    /* Set tap shock time window. */
    if (LSM6DSL_ACC_GYRO_SHOCK_Duration(0x00) == FALSE)
    {
        return FALSE;
    }

    /* Set tap quiet time window. */
    if (LSM6DSL_ACC_GYRO_QUIET_Duration(0x00) == FALSE)
    {
        return FALSE;
    }

    /* Reset tap duration time window. */
    if (LSM6DSL_ACC_GYRO_DUR(0x00) == FALSE)
    {
        return FALSE;
    }

    /* Only single tap enabled. */
    if (LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_EV(LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_SINGLE_TAP) == FALSE)
    {
        return FALSE;
    }

    /* Disable Z direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable Y direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    /* Disable X direction in tap recognition. */
    if (LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_DISABLED) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief This mode set GS to normal mode with both acclerometer and gyroscope active
 * @retval TRUE in case of success, an error code otherwise
 */
void LSM6DSL_SetStandbyMode(void)
{
    tAclrConfig.bHighPerf = FALSE;
    tAclrConfig.u8OutputDataRate = 208.0f;
    tAclrConfig.bEnable = TRUE;
    if (LSM6DSL_AccInit(&tAclrConfig) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: AccInit ERROR");
        return;
    }

    /* Output data rate selection - power down */
    if (LSM6DSL_ACC_GYRO_ODR_G(LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN ERROR");
        return;
    }
    RTOSprintf("\r\n>LSM6DSL: Set to standby mode OK");
}
// bool LSM6DSL_6DOrientation(LSM6DSL_Interrupt_Pin_t tIntPin)
// {
// 	/* Output Data Rate selection*/
// 	if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_416Hz) == FALSE)
// 	{
// 		return FALSE;
// 	}
// 	/* Full scale selection. */
// 	if (LSM6DSL_SetFullScaleXL(2.0f) == FALSE)
// 	{
// 		return FALSE;
// 	}
// }
void LSM6DSL_Init(void)
{
#if HW_TYPE == HW_JAS306TV
    hi2c = &hi2c2;
    mutex_i2c_user = &mutex_i2c2_user;
#elif HW_TYPE == HW_JAS208
    hi2c = &hi2c1;
    mutex_i2c_user = &mutex_i2c1_user;
#endif
    /* Enable register address automatically incremented during a multiple byte
       access with a serial interface. */
    if (LSM6DSL_ACC_GYRO_IF_Addr_Incr(LSM6DSL_ACC_GYRO_IF_INC_ENABLED) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_IF_Addr_Incr ERROR");
        return;
    }
    /* Enable BDU */
    if (LSM6DSL_ACC_GYRO_BDU(LSM6DSL_ACC_GYRO_BDU_BLOCK_UPDATE) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_BDU_BLOCK_UPDATE ERROR");
        return;
    }
    /* FIFO mode selection */
    if (LSM6DSL_ACC_GYRO_FIFO_MODE(LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS ERROR");
        return;
    }
    /* Output data rate selection - power down. */
    if (LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_POWER_DOWN) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_ODR_XL_POWER_DOWN ERROR");
        return;
    }
    /* Full scale selection. */
    if (LSM6DSL_SetFullScaleXL(8.0f) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: SetFullScaleXL ERROR");
        return;
    }
    /* Output data rate selection - power down */
    if (LSM6DSL_ACC_GYRO_ODR_G(LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN ERROR");
        return;
    }
    /* Full scale selection for G. */
    if (LSM6DSL_SetFullScale_G(250.0f) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: SetFullScaleXL ERROR");
        return;
    }

    if (LSM6DSL_ACC_GYRO_HACTIVE(LSM6DSL_ACC_GYRO_H_ACTIVE_DISABLED) == FALSE)
    {
        return;
    }

    tAclrConfig.bHighPerf = FALSE;
    tAclrConfig.u8OutputDataRate = 208.0f;
    tAclrConfig.bEnable = TRUE;
    if (LSM6DSL_AccInit(&tAclrConfig) == FALSE)
    {
        RTOSprintf("\r\n>LSM6DSL: AccInit ERROR");
        return;
    }

    tGyroConfig.bHighPerf = FALSE;
    tGyroConfig.bEnable = TRUE;
    tGyroConfig.u8OutputDataRate = 208.0f;

    if (LSM6DSL_GyroInit(&tGyroConfig) == FALSE)
    {
        RTOSprintf("\r\n> LSM6DSL config Gyro ERROR");

        return;
    }

    // Enable Wake up Detection
    LSM6DSL_EnableWakeUpDetection(LSM6DSL_INT1_PIN);

    LSM6DSL_EnableAWTiltDetection();

    RTOSprintf("\r\n>LSM6DSL: Init OK");
}

/**
 * @brief This mode deinit Gsensor
 * @retval None
 */
void LSM6DSL_DeInit(void)
{
    LSM6DSL_DisableDoubleTapDetection();
    LSM6DSL_DisableAWTiltDetection();
    LSM6DSL_DisablePedometer();
    LSM6DSL_DisableTiltDetection(LSM6DSL_INT1_PIN);
    LSM6DSL_DisableWakeUpDetection(LSM6DSL_INT1_PIN);
    LSM6DSL_DisableSingleTapDetection();

    RTOSprintf("\r\n>LSM6DSL: DeInit OK");
}
//============================================================================//
// @brief  Write to a register of IC LSM6DSL.
// @param  u8DevAddr: IC's selected slave address.
//         u8Reg: Address of start register to write to.
//         u8Dat: Data byte to write.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_WriteReg(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t u8Dat)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8DatBuf[2] = {0};

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(*mutex_i2c_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    u8DatBuf[0] = u8Reg;
    u8DatBuf[1] = u8Dat;

    halRes = HAL_I2C_Master_Transmit(hi2c, u8DevAddr, u8DatBuf,
                                     2, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL write to register 0x%0X ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(*mutex_i2c_user);

    return TRUE;
}

//============================================================================//
// @brief  Write to registers of IC RX8130.
// @param  u8DevAddr: IC's slave address.
//         u8Red: Address of the start register to write to.
//         pu8Buf: Pointer to data buffer.
//         u16Len: Number of bytes to write.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_WriteRegs(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Buf, uint16_t u16Len)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8DatBuf[64] = {0};

    if (u16Len > 60)
        return FALSE;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(*mutex_i2c_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    u8DatBuf[0] = u8Reg;
    memcpy(&u8DatBuf[1], pu8Buf, u16Len);

    halRes = HAL_I2C_Master_Transmit(hi2c, u8DevAddr, u8DatBuf,
                                     1 + u16Len, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL write to registers from 0x%0X ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(*mutex_i2c_user);

    return TRUE;
}

//============================================================================//
// @brief  Read from 1 register of IC LSM6DSL.
// @param  u8DevAddr: IC's slave address.
//         u8Reg: Address of the register to read from.
//         pu8Dat: Pointer to the data buffer to store read data.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_ReadReg(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Dat)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(*mutex_i2c_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    halRes = HAL_I2C_Master_Transmit(hi2c, u8DevAddr, &u8Reg,
                                     1, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL read from register 0x%0X send address ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(hi2c, (uint16_t)u8DevAddr, pu8Dat,
                                    1, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL read from register 0x%0X read data ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(*mutex_i2c_user);

    return TRUE;
}
//============================================================================//
// @brief  Read from registers of IC LSM6DSL.
// @param  u8DevAddr: IC's slave address.
//         u8Reg: Address of the start register to read from.
//         pu8Buf: Pointer to the data buffer to store read data.
//         u16Len: Number of bytes to read.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool LSM6DSL_ReadRegs(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Buf, uint16_t u16Len)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(*mutex_i2c_user, S_2_mS(10));

        if (osRes != osOK)
            return FALSE;
    }

    halRes = HAL_I2C_Master_Transmit(hi2c, u8DevAddr, &u8Reg,
                                     1, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL read from register 0x%0X send address ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(hi2c, (uint16_t)u8DevAddr, pu8Buf,
                                    u16Len, LSM6DSL_I2C_TIMEOUT);

    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(*mutex_i2c_user);

        if (halRes == HAL_BUSY)
        {
            LSM6DSL_I2C_ReInit();
        }
        else
        {
            u8LSM6DSL_ErrCnt++;

            if (u8LSM6DSL_ErrCnt >= LSM6DSL_ERR_CNT_MAX)
            {
                u8LSM6DSL_ErrCnt = 0;

                if (bLSM6DSL_Stop == FALSE)
                {
                    RTOSprintf("\r\n> Maximum LSM6DSL errors reached. Restart IC LSM6DSL.");

                    LSM6DSL_Restart();
                }
            }
        }

        RTOSprintf("\r\n> LSM6DSL read from register 0x%0X read data ERROR %d", u8Reg, halRes);

        return FALSE;
    }

    if (bRTOS_En)
        osMutexRelease(*mutex_i2c_user);

    return TRUE;
}

//============================================================================//
// @brief  Make the LSM6DSL IC ready to communicate again.
// @param  None.
// @retval None.
//============================================================================//
static void LSM6DSL_I2C_ReInit(void)
{
    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(*mutex_i2c_user, S_2_mS(10));

        if (osRes != osOK)
            return;
    }

    HAL_I2C_DeInit(hi2c);

#if HW_TYPE == HW_JAS306TV
    // Reconfig SCL and SDA
    HW_PBClr(I2C2_SCL);
    HW_PBInit(I2C2_SCL, FALSE);
    HW_PBInit(I2C2_SDA, FALSE);

#elif HW_TYPE == HW_JAS208
    // Reconfig SCL and SDA
    HW_PBClr(I2C1_SCL);
    HW_PBInit(I2C1_SCL, FALSE);
    HW_PBInit(I2C1_SDA, FALSE);
#endif

    // Send restart pulses to the IC
    for (int i = 0; i < 9; i++)
    {
#if HW_TYPE == HW_JAS306TV
        HW_PBSet(I2C2_SCL);

        Delay_uS(10);

        HW_PBClr(I2C2_SCL);

        Delay_uS(10);

#elif HW_TYPE == HW_JAS208
        HW_PBSet(I2C1_SCL);

        Delay_uS(10);

        HW_PBClr(I2C1_SCL);

        Delay_uS(10);
#endif
    }

#if HW_TYPE == HW_JAS306TV
    // Put SCL and SDA back to normal state
    HW_PBDeInit(I2C2_SCL, FALSE);
    HW_PBDeInit(I2C2_SDA, FALSE);

    USER_I2C2_Init();

#elif HW_TYPE == HW_JAS208
    // Put SCL and SDA back to normal state
    HW_PBDeInit(I2C1_SCL, FALSE);
    HW_PBDeInit(I2C1_SDA, FALSE);

    USER_I2C1_Init();
#endif

    if (bRTOS_En)
    {
        osMutexRelease(*mutex_i2c_user);
    }
}

//============================================================================//
// @brief  Deinit the LSM6DSL communication interface.
// @param  None.
// @retval None.
//============================================================================//
void LSM6DSL_I2C_DeInit(void)
{
    HAL_I2C_DeInit(hi2c);
}

//============================================================================//
// @brief  Restart IC LSM6DSL.
// @param  None.
// @retval None.
//============================================================================//
static void LSM6DSL_Restart(void)
{
    uint8_t u8LSM6DSL_RestartCnt = 0;
#if HW_TYPE == HW_JAS306TV
    u8LSM6DSL_RestartCnt = HW_ReadBackupRegister(I2C2_ERROR_CHK);

#elif HW_TYPE == HW_JAS208
    u8LSM6DSL_RestartCnt = HW_ReadBackupRegister(I2C1_ERROR_CHK);
#endif

    if (bLSM6DSL_Stop == TRUE)
        return;

    u8LSM6DSL_RestartCnt++;

#if HW_TYPE == HW_JAS306TV
    HW_WriteBackupRegister(I2C2_ERROR_CHK, u8LSM6DSL_RestartCnt);

#elif HW_TYPE == HW_JAS208
    HW_WriteBackupRegister(I2C1_ERROR_CHK, u8LSM6DSL_RestartCnt);
#endif

    if (u8LSM6DSL_RestartCnt >= LSM6DSL_RESTART_CNT_MAX)
    {
        RTOSprintf("\r\n> Maximum LSM6DSL restart reached. Report IC LSM6DSL ERROR!");

        u8LSM6DSL_RestartCnt = 0;
#if HW_TYPE == HW_JAS306TV
        HW_WriteBackupRegister(I2C2_ERROR_CHK, u8LSM6DSL_RestartCnt);

#elif HW_TYPE == HW_JAS208
        HW_WriteBackupRegister(I2C1_ERROR_CHK, u8LSM6DSL_RestartCnt);
#endif

        u32LSM6DSL_ReportErrLastTick = HAL_GetTick();

        bLSM6DSL_ReportErrFlag = TRUE;

        bLSM6DSL_Stop = TRUE;

        // #if ENABLE_GSM_NET
        // GSM_TCP_ReportEvent(EVENT_EX_RTC_FAIL, STR_NULL);
        // #endif

        return;
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
// @brief  Read X, Y & Z Acceleration values
// @param  pConfig: Accelerator config pointer
//         pData: Data out pointer
// @retval None.
//============================================================================//
bool LSM6DSL_AccReadXYZ(ACCELERO_ConfigTypeDef *pConfig, LSM6DSL_AccDataTypeDef *pData)
{
    int16_t iRawData[3] = {0};
    uint8_t u8Buf[6] = {0};
    uint8_t u8Temp = 0;
    uint8_t u8BaseAddress = 0;
    float fSensitivity = 0;

    if (bLSM6DSL_Stop == TRUE)
        return FALSE;

    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 2; j++)
        {
            if (LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_OUTX_L_XL + u8BaseAddress, &u8Buf[u8BaseAddress]) == FALSE)
                return FALSE;
            u8BaseAddress++;
        }
    }

    // format data
    for (int i = 0; i < 3; i++)
    {
        iRawData[i] = ((((int16_t)u8Buf[2 * i + 1]) << 8) + (int16_t)u8Buf[2 * i]);
    }

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL1_XL, &u8Temp))
    {
        return FALSE;
    }
    u8Temp &= LSM6DSL_ACC_GYRO_FS_XL_MASK;
    // Normal mode
    // Switch the sensitivity value set in the CRTL1_XL
    switch (u8Temp)
    {
    case LSM6DSL_ACC_GYRO_FS_XL_2g:
        fSensitivity = LSM6DSL_ACC_SENSITIVITY_2G;

        break;

    case LSM6DSL_ACC_GYRO_FS_XL_4g:
        fSensitivity = LSM6DSL_ACC_SENSITIVITY_4G;

        break;

    case LSM6DSL_ACC_GYRO_FS_XL_8g:
        fSensitivity = LSM6DSL_ACC_SENSITIVITY_8G;

        break;

    case LSM6DSL_ACC_GYRO_FS_XL_16g:
        fSensitivity = LSM6DSL_ACC_SENSITIVITY_16G;

        break;
    }

    // Obtain the mg value for the three axis
    // pData->i32AccXRaw = (int32_t)(iRawData[0]);
    // pData->i32AccYRaw = (int32_t)(iRawData[1]);
    // pData->i32AccZRaw = (int32_t)(iRawData[2]);
    pData->i32AccX = (int32_t)(iRawData[0] * fSensitivity);
    pData->i32AccY = (int32_t)(iRawData[1] * fSensitivity);
    pData->i32AccZ = (int32_t)(iRawData[2] * fSensitivity);
    pData->fSensitive = fSensitivity;

    return TRUE;
}
//============================================================================//
// @brief  Calculate the LSM6DSL angular data.
// @param  pConfig: Gyro config pointer
//         pData: Data out pointer
// @retval None.
//============================================================================//
bool LSM6DSL_GyroReadXYZAngRate(GYRO_ConfigTypeDef *pConfig, LSM6DSL_GyroDataTypeDef *pData)
{
    int16_t iRawData[3] = {0};
    uint8_t u8Buf[6] = {0};
    uint8_t u8Temp = 0;
    float fSensitivity = 0;
    uint8_t u8BaseAddress = 0;

    if (bLSM6DSL_Stop == TRUE)
        return FALSE;

    // Read full scale 125 selection from sensor//
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, &u8Temp))
    {
        return FALSE;
    }
    u8Temp &= LSM6DSL_ACC_GYRO_FS_125_MASK;

    if (u8Temp == LSM6DSL_ACC_GYRO_FS_125_ENABLED)
    {
        fSensitivity = LSM6DSL_GYRO_SENSITIVITY_FOR_FS_125DPS;
    }

    else
    {
        if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_CTRL2_G, &u8Temp))
        {
            return FALSE;
        }
        else
        {
            u8Temp &= LSM6DSL_ACC_GYRO_FS_G_MASK; // mask
        }
    }

    // Normal mode
    // Switch the sensitivity value set in the CRTL2_G
    switch (u8Temp)
    {
    case LSM6DSL_ACC_GYRO_FS_G_250dps:
        fSensitivity = LSM6DSL_GYRO_SENSITIVITY_250DPS;

        break;

    case LSM6DSL_ACC_GYRO_FS_G_500dps:
        fSensitivity = LSM6DSL_GYRO_SENSITIVITY_500DPS;

        break;

    case LSM6DSL_ACC_GYRO_FS_G_1000dps:
        fSensitivity = LSM6DSL_GYRO_SENSITIVITY_1000DPS;

        break;

    case LSM6DSL_ACC_GYRO_FS_G_2000dps:
        fSensitivity = LSM6DSL_GYRO_SENSITIVITY_2000DPS;

    default:
        fSensitivity = -1.0f;
        break;
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 2; j++)
        {
            if (LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_OUTX_L_G + u8BaseAddress, &u8Buf[u8BaseAddress]) == FALSE)
                return FALSE;
            u8BaseAddress++;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        iRawData[i] = ((((int16_t)u8Buf[2 * i + 1]) << 8) + (int16_t)u8Buf[2 * i]);
    }

    // Obtain the mg value for the three axis
    // pData->i32AngXRaw = (int32_t)(iRawData[0]);
    // pData->i32AngYRaw = (int32_t)(iRawData[1]);
    // pData->i32AngZRaw = (int32_t)(iRawData[2]);
    pData->i32AngP = (int32_t)(iRawData[0] * fSensitivity);
    pData->i32AngR = (int32_t)(iRawData[1] * fSensitivity);
    pData->i32AngY = (int32_t)(iRawData[2] * fSensitivity);
    pData->fSensitive = fSensitivity;
    return TRUE;
}
//============================================================================//
// @brief  Get event status
// @param  pStatus: Status data pointer
// @retval TRUE: if successfully.
//			FALSE: if FALSE
//============================================================================//
bool LSM6DSL_GetEventStatus(LSM6DSL_Event_Status_t *pStatus)
{
    uint8_t u8WakeUpSrc = 0;
    uint8_t u8TapSrc = 0;
    uint8_t u8D6DSrc = 0;
    uint8_t u8FuncSrc = 0;
    uint8_t u8MD1Src = 0;
    uint8_t u8MD2Src = 0;
    uint8_t u8Int1CtrlSrc = 0;
    uint8_t u8FuncSrc2 = 0;
    uint8_t u8Int2WristSrc = 0;
    // uint8_t u8Pin = 0;
    //  Read data of all the Wakeup source
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_WAKE_UP_SRC, &u8WakeUpSrc))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_TAP_SRC, &u8TapSrc))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_D6D_SRC, &u8D6DSrc))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_FUNC_SRC, &u8FuncSrc))
    {
        return FALSE;
    }
    // FUNC_SRC2
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSMDSL_ACC_GYRO_A_WRIST_TILT_THS, &u8FuncSrc2))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD1_CFG, &u8MD1Src))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_MD2_CFG, &u8MD2Src))
    {
        return FALSE;
    }
    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_INT1_CTRL, &u8Int1CtrlSrc))
    {
        return FALSE;
    }

    if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_G, &u8Int2WristSrc))
    {
        return FALSE;
    }

    // Check interrupt flag is set or not
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_FF_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_FF_MASK))
    {
        if (u8WakeUpSrc & 0x20)
        {
            pStatus->u8FreeFallStatus = 1;
        }
    }
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_WU_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_WU_MASK))
    {
        if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_WU_MASK))
        {
            u8Pin = 1;
            // pStatus->u8PinInterrupt = 1;
            //*u8PinInt = 1;
        }
        if ((u8MD2Src & LSM6DSL_ACC_GYRO_INT2_WU_MASK))
        {
            u8Pin = 2;
            //*u8PinInt = 2;
            // pStatus->u8PinInterrupt = 2;
        }
        if ((u8WakeUpSrc & LSM6DSL_ACC_GYRO_WU_EV_STATUS_MASK))
        {
            pStatus->u8WakeUpStatus = 1;
        }
        if ((u8WakeUpSrc & LSM6DSL_ACC_GYRO_X_WU_EV_STATUS_MASK))
        {
            pStatus->u8Axes = 1;
        }
        else if ((u8WakeUpSrc & LSM6DSL_ACC_GYRO_Y_WU_EV_STATUS_MASK))
        {
            pStatus->u8Axes = 2;
        }
        else if ((u8WakeUpSrc & LSM6DSL_ACC_GYRO_Z_WU_EV_STATUS_MASK))
        {
            pStatus->u8Axes = 3;
        }
    }
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_MASK))
    {
        if ((u8TapSrc & LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_MASK))
        {
            pStatus->u8TapStatus = 1;
        }
    }
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_TAP_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_TAP_MASK))
    {
        if ((u8TapSrc & LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_MASK))
        {
            pStatus->u8DoubleTapStatus = 1;
        }
    }
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_6D_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_6D_MASK))
    {
        if ((u8D6DSrc & LSM6DSL_ACC_GYRO_D6D_EV_STATUS_MASK))
        {
            pStatus->u8D6DOrientationStatus = 1;
        }
    }
    if ((u8Int1CtrlSrc & LSM6DSL_ACC_GYRO_INT1_PEDO_MASK))
    {
        if ((u8FuncSrc & LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_MASK))
        {
            pStatus->u8StepStatus = 1;
        }
    }
    if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_TILT_MASK) || (u8MD2Src & LSM6DSL_ACC_GYRO_INT2_TILT_MASK))
    {
        if ((u8MD1Src & LSM6DSL_ACC_GYRO_INT1_TILT_MASK))
        {
            u8Pin = 1;
            // pStatus->u8PinInterrupt = 1;
            //*u8PinInt = 1;
        }
        if ((u8MD2Src & LSM6DSL_ACC_GYRO_INT2_TILT_MASK))
        {
            u8Pin = 2;
            //*u8PinInt = 2;
            // pStatus->u8PinInterrupt = 2;
        }
        if ((u8FuncSrc & LSM6DSL_ACC_GYRO_TILT_EV_STATUS_MASK))
        {
            pStatus->u8TiltStatus = 1;
        }
    }
    if (u8Int2WristSrc & LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_MASK_t)
    {
        if ((u8FuncSrc2 & 0x01))
        {
            pStatus->u8WristTiltStatus = 1;
        }
    }
    return TRUE;
}
//============================================================================//
// @brief  Get the step counter for LSM6DSL accelerometer sensor
// @param  step_count the pointer to the step counter
// @retval TRUE: if successfully.
//			FALSE: if FALSE
//============================================================================//

bool LSM6DSL_GetStepCounter(uint16_t *pStepCount)
{
    uint8_t u8NumberByteForDimension = 2;
    uint8_t u8AdrrInc;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < u8NumberByteForDimension; j++)
        {
            // Read 2 register Step Counter L and Step counter H
            if (!LSM6DSL_ReadReg(LSM6DSL_I2C_ADDR, (LSM6DSL_ACC_GYRO_STEP_COUNTER_L + u8AdrrInc), (uint8_t *)&pStepCount[u8AdrrInc]))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
//============================================================================//
// @brief  LSM6DSL_Proc. Check event status and print it
// @param  NONE
// @retval NONE
//			NONE
//============================================================================//
void LSM6DSL_Proc(void)
{
    LSM6DSL_Event_Status_t tEventStatus;
    static LSM6DSL_AccDataTypeDef tLastData;
    //	MG82_WkupSrcTypeDef u8WkupSrc = MG82_WKUP_SRC_NONE;
    //	uint16_t u16Stepcount = 0;
    if (!LSM6DSL_AccReadXYZ(&tAclrConfig, &tAccData))
    {
        RTOSprintf("\r\n> LSM6DSL: AccReadXYZ: ERROR");
    }
    if (!LSM6DSL_GyroReadXYZAngRate(&tGyroConfig, &tGyroData))
    {
        RTOSprintf("\r\n> LSM6DSL: GyroReadXYZ: ERROR");
    }
    if (bSetCurrOrien == TRUE)
    {
        if (LSM6DSL_SetOrienTation(tAccData, tLastData))
        {
            bSetCurrOrien = FALSE;
        }
    }

    if (!LSM6DSL_GetEventStatus(&tEventStatus))
    {
        RTOSprintf("\r\n> LSM6DSL: Get Event Status: ERROR");
    }

    if (tEventStatus.u8WakeUpStatus == 1)
    {
        GSprintf("\r\n> LSM6DSL wake up on INT pin %d", u8Pin);
        // GSprintf("\r\n> LSM6DSL wake up on INT pin %d and trigger by Axes: %d with value XYZ: X = %.4d, Y = %.4d, Z = %.4d, and last value : XYZ: X = %.4d, Y = %.4d, Z = %.4d", u8Pin, tEventStatus.u8Axes, tAccData.i32AccX, tAccData.i32AccY, tAccData.i32AccZ, tLastData.iLastX, tLastData.iLastY, tLastData.iLastZ);
        tEventStatus.u8WakeUpStatus = 0;
        GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_WAKEUP, (char *)"0");
    }
    if (tEventStatus.u8WristTiltStatus == 1)
    {

        GSprintf("\r\n> LSM6DSL: Wrist Tilt Detected");

        tEventStatus.u8WristTiltStatus = 0;

        GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_TILT_CHANGE, (char *)"0");
    }
    // if(tEventStatus.u8TiltStatus == 1)
    // {
    // 	GSprintf("\r\n> LSM6DSL: Tilt Detected on INT pin %d", u8Pin);

    // 	tEventStatus.u8WristTiltStatus = 0;
    // 	GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_TILT_CHANGE, (char *)"0");
    // }
    if (tEventStatus.u8TapStatus == 1)
    {
        GSprintf("\r\n> LSM6DSL: Single Tap Detected on INT pin %d", u8Pin);

        tEventStatus.u8TapStatus = 0;
    }
    if (tEventStatus.u8StepStatus == 1)
    {
        GSprintf("\r\n> LSM6DSL: Pedodimet Detected on INT pin %d", u8Pin);

        tEventStatus.u8TapStatus = 0;
    }
    tLastData.i32AccX = tAccData.i32AccX;
    tLastData.i32AccY = tAccData.i32AccY;
    tLastData.i32AccZ = tAccData.i32AccZ;
}

//============================================================================//
// @brief  Set the orientation of Gsensor
// @param  CurrentData: The current data of accelemeter
//         LastData: The last data of accelemeter
// @retval TRUE: if successfully.
//			FALSE: if FALSE
//============================================================================//
bool LSM6DSL_SetOrienTation(LSM6DSL_AccDataTypeDef CurrentData, LSM6DSL_AccDataTypeDef LastData)
{
    bool bRes = FALSE;
    static uint8_t u8CntTime;
    uint8_t u8AxisFlag;
    // static uint8_t u8Flag;
    // uint8_t u8Data;
    if ((abs(CurrentData.i32AccX - LastData.i32AccX) > LSM6DSL_G_SENSOR_THRESHOLD) || (abs(CurrentData.i32AccZ) > LSM6DSL_G_SENSOR_THRESHOLD)) // Car moving following X
    {
        g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg);
        u8CntTime++;
        u8AxisFlag = 1;
    }
    else if ((abs(CurrentData.i32AccZ - LastData.i32AccZ) > LSM6DSL_G_SENSOR_DISTANCE) || (abs(CurrentData.i32AccX) > LSM6DSL_G_SENSOR_THRESHOLD)) // Car moving following Z
    {
        g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg);
        u8CntTime++;
        u8AxisFlag = 2;
    }
    else if ((abs(CurrentData.i32AccY - LastData.i32AccY) > LSM6DSL_G_SENSOR_THRESHOLD) || (abs(CurrentData.i32AccY) > LSM6DSL_G_SENSOR_THRESHOLD)) // Car moving following Y
    {
        g_Setting.tGS_Setting.u8Axis = (uint8_t)(LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos | LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg);
        u8CntTime++;
        u8AxisFlag = 3;
    }
    else
    {
        bRes = FALSE;
    }
    if (u8CntTime >= LSM6DSL_TIMECHECK)
    {
        u8CntTime = 0;
        RTOSprintf("\r\n> LSM6DSL: Change orientation on axes: %d", u8AxisFlag);
        if (LSM6DSL_EnableAWTiltDetection() == FALSE)
        {
            RTOSprintf("\r\n> LSM6DSL: Change orientation on axes: %d FAILED!", u8AxisFlag);
        }
#if HW_TYPE == HW_JAS306TV
        LSM6DSL_Init();
#endif
        // if (LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS(u8Data) == FALSE)
        // {
        //     return FALSE;
        // }
        // if (LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK((LSM6DSL_ACC_GYRO_A_WRIST_MASK_t)(g_Setting.tGS_Setting.u8Axis)) == FALSE)
        // {
        //     bRes = FALSE;
        // }
        // u8AxisFlag = 0;
        bRes = TRUE;
    }
    // u8Flag = u8AxisFlag;
    return bRes;
}
//============================================================================//
// @brief  Scan the I2C Bus for I2C device address.
// @param  None.
// @retval None.
//============================================================================//
void LSM6DSL_ScanBus(void)
{
    bool bRes;

    RTOSprintf("\r\n> Scan I2C Bus Start:\r\n");

    for (int i = 1; i < 256; i++)
    {
        bRes = HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(i << 1), 3, 5);

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
