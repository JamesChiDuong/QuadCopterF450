#ifndef __LSM6DSL__H
#define __LSM6DSL__H

#ifdef __cplusplus
extern "C"
{
#endif

//============================================================================//
// User Includes
//============================================================================//
#include "stm32fxxx_hal_user.h"
#include "DataType.h"
#include "main.h"

//============================================================================//
// User Defines
//============================================================================//
#define LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW 0xD4  // SAD[0] = 0
#define LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH 0xD6 // SAD[0] = 1

#define LSM6DSL_I2C_ADDR LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW // Select low address

    // Who am I

#define LSM6DSL_ACC_GYRO_WHO_AM_I 0x6A

    // Device Register

#define LSM6DSL_ACC_GYRO_FUNC_CFG_ACCESS 0x01

#define LSM6DSL_ACC_GYRO_SENSOR_SYNC_TIME 0x04
#define LSM6DSL_ACC_GYRO_SENSOR_RES_RATIO 0x05

#define LSM6DSL_ACC_GYRO_FIFO_CTRL1 0x06
#define LSM6DSL_ACC_GYRO_FIFO_CTRL2 0x07
#define LSM6DSL_ACC_GYRO_FIFO_CTRL3 0x08
#define LSM6DSL_ACC_GYRO_FIFO_CTRL4 0x09
#define LSM6DSL_ACC_GYRO_FIFO_CTRL5 0x0A

#define LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_G 0x0B
#define LSM6DSL_ACC_GYRO_INT1_CTRL 0x0D
#define LSM6DSL_ACC_GYRO_INT2_CTRL 0x0E
#define LSM6DSL_ACC_GYRO_WHO_AM_I_REG 0x0F
#define LSM6DSL_ACC_GYRO_CTRL1_XL 0x10
#define LSM6DSL_ACC_GYRO_CTRL2_G 0x11
#define LSM6DSL_ACC_GYRO_CTRL3_C 0x12
#define LSM6DSL_ACC_GYRO_CTRL4_C 0x13
#define LSM6DSL_ACC_GYRO_CTRL5_C 0x14
#define LSM6DSL_ACC_GYRO_CTRL6_C 0x15
#define LSM6DSL_ACC_GYRO_CTRL7_G 0x16
#define LSM6DSL_ACC_GYRO_CTRL8_XL 0x17
#define LSM6DSL_ACC_GYRO_CTRL9_XL 0x18
#define LSM6DSL_ACC_GYRO_CTRL10_C 0x19

#define LSM6DSL_ACC_GYRO_MASTER_CONFIG 0x1A
#define LSM6DSL_ACC_GYRO_WAKE_UP_SRC 0x1B
#define LSM6DSL_ACC_GYRO_TAP_SRC 0x1C
#define LSM6DSL_ACC_GYRO_D6D_SRC 0x1D
#define LSM6DSL_ACC_GYRO_STATUS_REG 0x1E

#define LSM6DSL_ACC_GYRO_OUT_TEMP_L 0x20
#define LSM6DSL_ACC_GYRO_OUT_TEMP_H 0x21
#define LSM6DSL_ACC_GYRO_OUTX_L_G 0x22
#define LSM6DSL_ACC_GYRO_OUTX_H_G 0x23
#define LSM6DSL_ACC_GYRO_OUTY_L_G 0x24
#define LSM6DSL_ACC_GYRO_OUTY_H_G 0x25
#define LSM6DSL_ACC_GYRO_OUTZ_L_G 0x26
#define LSM6DSL_ACC_GYRO_OUTZ_H_G 0x27
#define LSM6DSL_ACC_GYRO_OUTX_L_XL 0x28
#define LSM6DSL_ACC_GYRO_OUTX_H_XL 0x29
#define LSM6DSL_ACC_GYRO_OUTY_L_XL 0x2A
#define LSM6DSL_ACC_GYRO_OUTY_H_XL 0x2B
#define LSM6DSL_ACC_GYRO_OUTZ_L_XL 0x2C
#define LSM6DSL_ACC_GYRO_OUTZ_H_XL 0x2D
#define LSM6DSL_ACC_GYRO_SENSORHUB1_REG 0x2E
#define LSM6DSL_ACC_GYRO_SENSORHUB2_REG 0x2F
#define LSM6DSL_ACC_GYRO_SENSORHUB3_REG 0x30
#define LSM6DSL_ACC_GYRO_SENSORHUB4_REG 0x31
#define LSM6DSL_ACC_GYRO_SENSORHUB5_REG 0x32
#define LSM6DSL_ACC_GYRO_SENSORHUB6_REG 0x33
#define LSM6DSL_ACC_GYRO_SENSORHUB7_REG 0x34
#define LSM6DSL_ACC_GYRO_SENSORHUB8_REG 0x35
#define LSM6DSL_ACC_GYRO_SENSORHUB9_REG 0x36
#define LSM6DSL_ACC_GYRO_SENSORHUB10_REG 0x37
#define LSM6DSL_ACC_GYRO_SENSORHUB11_REG 0x38
#define LSM6DSL_ACC_GYRO_SENSORHUB12_REG 0x39
#define LSM6DSL_ACC_GYRO_FIFO_STATUS1 0x3A
#define LSM6DSL_ACC_GYRO_FIFO_STATUS2 0x3B
#define LSM6DSL_ACC_GYRO_FIFO_STATUS3 0x3C
#define LSM6DSL_ACC_GYRO_FIFO_STATUS4 0x3D
#define LSM6DSL_ACC_GYRO_FIFO_DATA_OUT_L 0x3E
#define LSM6DSL_ACC_GYRO_FIFO_DATA_OUT_H 0x3F
#define LSM6DSL_ACC_GYRO_FIFO_MODE_MASK 0x07
#define LSM6DSL_ACC_GYRO_TIMESTAMP0_REG 0x40
#define LSM6DSL_ACC_GYRO_TIMESTAMP1_REG 0x41
#define LSM6DSL_ACC_GYRO_TIMESTAMP2_REG 0x42

#define LSM6DSL_ACC_GYRO_TIMESTAMP_L 0x49
#define LSM6DSL_ACC_GYRO_TIMESTAMP_H 0x4A

#define LSM6DSL_ACC_GYRO_STEP_COUNTER_L 0x4B
#define LSM6DSL_ACC_GYRO_STEP_COUNTER_H 0x4C

#define LSM6DSL_ACC_GYRO_SENSORHUB13_REG 0x4D
#define LSM6DSL_ACC_GYRO_SENSORHUB14_REG 0x4E
#define LSM6DSL_ACC_GYRO_SENSORHUB15_REG 0x4F
#define LSM6DSL_ACC_GYRO_SENSORHUB16_REG 0x50
#define LSM6DSL_ACC_GYRO_SENSORHUB17_REG 0x51
#define LSM6DSL_ACC_GYRO_SENSORHUB18_REG 0x52

#define LSMDSL_ACC_GYRO_A_WRIST_TILT_LAT 0x50
#define LSMDSL_ACC_GYRO_A_WRIST_TILT_THS 0x54
#define LSMDSL_ACC_GYRO_A_WRIST_TILT_MASK 0x59

#define LSM6DSL_ACC_GYRO_FUNC_SRC 0x53
#define LSM6DSL_ACC_GYRO_TAP_CFG1 0x58
#define LSM6DSL_ACC_GYRO_TAP_THS_6D 0x59
#define LSM6DSL_ACC_GYRO_INT_DUR2 0x5A
#define LSM6DSL_ACC_GYRO_WAKE_UP_THS 0x5B
#define LSM6DSL_ACC_GYRO_WAKE_UP_DUR 0x5C
#define LSM6DSL_ACC_GYRO_FREE_FALL 0x5D
#define LSM6DSL_ACC_GYRO_MD1_CFG 0x5E
#define LSM6DSL_ACC_GYRO_MD2_CFG 0x5F

#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_X_L 0x66
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_X_H 0x67
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Y_L 0x68
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Y_H 0x69
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Z_L 0x6A
#define LSM6DSL_ACC_GYRO_OUT_MAG_RAW_Z_H 0x6B

#define LSM6DSL_ACC_GYRO_X_OFS_USR 0x73
#define LSM6DSL_ACC_GYRO_Y_OFS_USR 0x74
#define LSM6DSL_ACC_GYRO_Z_OFS_USR 0x75

// Embedded functions register mapping
#define LSM6DSL_ACC_GYRO_SLV0_ADD 0x02
#define LSM6DSL_ACC_GYRO_SLV0_SUBADD 0x03
#define LSM6DSL_ACC_GYRO_SLAVE0_CONFIG 0x04
#define LSM6DSL_ACC_GYRO_SLV1_ADD 0x05
#define LSM6DSL_ACC_GYRO_SLV1_SUBADD 0x06
#define LSM6DSL_ACC_GYRO_SLAVE1_CONFIG 0x07
#define LSM6DSL_ACC_GYRO_SLV2_ADD 0x08
#define LSM6DSL_ACC_GYRO_SLV2_SUBADD 0x09
#define LSM6DSL_ACC_GYRO_SLAVE2_CONFIG 0x0A
#define LSM6DSL_ACC_GYRO_SLV3_ADD 0x0B
#define LSM6DSL_ACC_GYRO_SLV3_SUBADD 0x0C
#define LSM6DSL_ACC_GYRO_SLAVE3_CONFIG 0x0D
#define LSM6DSL_ACC_GYRO_DATAWRITE_SRC_MODE_SUB_SLV0 0x0E
#define LSM6DSL_ACC_GYRO_CONFIG_PEDO_THS_MIN 0x0F

#define LSM6DSL_ACC_GYRO_SM_STEP_THS 0x13
#define LSM6DSL_ACC_GYRO_PEDO_DEB_REG 0x14
#define LSM6DSL_ACC_GYRO_STEP_COUNT_DELTA 0x15

#define LSM6DSL_ACC_GYRO_MAG_SI_XX 0x24
#define LSM6DSL_ACC_GYRO_MAG_SI_XY 0x25
#define LSM6DSL_ACC_GYRO_MAG_SI_XZ 0x26
#define LSM6DSL_ACC_GYRO_MAG_SI_YX 0x27
#define LSM6DSL_ACC_GYRO_MAG_SI_YY 0x28
#define LSM6DSL_ACC_GYRO_MAG_SI_YZ 0x29
#define LSM6DSL_ACC_GYRO_MAG_SI_ZX 0x2A
#define LSM6DSL_ACC_GYRO_MAG_SI_ZY 0x2B
#define LSM6DSL_ACC_GYRO_MAG_SI_ZZ 0x2C
#define LSM6DSL_ACC_GYRO_MAG_OFFX_L 0x2D
#define LSM6DSL_ACC_GYRO_MAG_OFFX_H 0x2E
#define LSM6DSL_ACC_GYRO_MAG_OFFY_L 0x2F
#define LSM6DSL_ACC_GYRO_MAG_OFFY_H 0x30
#define LSM6DSL_ACC_GYRO_MAG_OFFZ_L 0x31
#define LSM6DSL_ACC_GYRO_MAG_OFFZ_H 0x32

#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_125DPS 04.375  /**< Sensitivity value for 125 dps full scale [mdps/LSB] */
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_245DPS 08.750  /**< Sensitivity value for 245 dps full scale [mdps/LSB] */
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_500DPS 17.500  /**< Sensitivity value for 500 dps full scale [mdps/LSB] */
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_1000DPS 35.000 /**< Sensitivity value for 1000 dps full scale [mdps/LSB] */
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_2000DPS 70.000 /**< Sensitivity value for 2000 dps full scale [mdps/LSB] */
#define LSM6DSL_GYRO_SENSITIVITY_FOR_WRIST 15.625
    // Output data rate selection

    /**************** Specific Function  *******************/

    /*******************************************************************************
     * Register      : CTRL3_C
     * Address       : 0X12
     * Bit Group Name: BDU
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_BDU_CONTINUOS = 0x00,
        LSM6DSL_ACC_GYRO_BDU_BLOCK_UPDATE = 0x40,
    } LSM6DSL_ACC_GYRO_BDU_t;
#define LSM6DSL_ACC_GYRO_BDU_MASK 0x40
    bool LSM6DSL_ACC_GYRO_BDU(LSM6DSL_ACC_GYRO_BDU_t tBDUData);

    /*******************************************************************************
     * Register      : FIFO_CTRL5
     * Address       : 0X0A
     * Bit Group Name: FIFO_MODE
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS = 0x00,
        LSM6DSL_ACC_GYRO_FIFO_MODE_FIFO = 0x01,
        LSM6DSL_ACC_GYRO_FIFO_MODE_STREAM = 0x02,
        LSM6DSL_ACC_GYRO_FIFO_MODE_STF = 0x03,
        LSM6DSL_ACC_GYRO_FIFO_MODE_BTS = 0x04,
        LSM6DSL_ACC_GYRO_FIFO_MODE_DYN_STREAM = 0x05,
        LSM6DSL_ACC_GYRO_FIFO_MODE_DYN_STREAM_2 = 0x06,
        LSM6DSL_ACC_GYRO_FIFO_MODE_BTF = 0x07,
    } LSM6DSL_ACC_GYRO_FIFO_MODE_t;

#define LSM6DSL_ACC_GYRO_FIFO_MODE_MASK 0x07
    bool LSM6DSL_ACC_GYRO_FIFO_MODE(LSM6DSL_ACC_GYRO_FIFO_MODE_t tfifodata);

    /*******************************************************************************
     * Register      : CTRL1_XL
     * Address       : 0X10
     * Bit Group Name: ODR_XL
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_ODR_XL_POWER_DOWN = 0x00,
        LSM6DSL_ACC_GYRO_ODR_XL_13Hz = 0x10,
        LSM6DSL_ACC_GYRO_ODR_XL_26Hz = 0x20,
        LSM6DSL_ACC_GYRO_ODR_XL_52Hz = 0x30,
        LSM6DSL_ACC_GYRO_ODR_XL_104Hz = 0x40,
        LSM6DSL_ACC_GYRO_ODR_XL_208Hz = 0x50,
        LSM6DSL_ACC_GYRO_ODR_XL_416Hz = 0x60,
        LSM6DSL_ACC_GYRO_ODR_XL_833Hz = 0x70,
        LSM6DSL_ACC_GYRO_ODR_XL_1660Hz = 0x80,
        LSM6DSL_ACC_GYRO_ODR_XL_3330Hz = 0x90,
        LSM6DSL_ACC_GYRO_ODR_XL_6660Hz = 0xA0,
    } LSM6DSL_ACC_GYRO_ODR_XL_t;

#define LSM6DSL_ACC_GYRO_ODR_XL_MASK 0xF0
    bool LSM6DSL_ACC_GYRO_ODR_XL(LSM6DSL_ACC_GYRO_ODR_XL_t tODRData);

    /*******************************************************************************
     * Register      : CTRL1_XL
     * Address       : 0X10
     * Bit Group Name: FS_XL
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_FS_XL_2g = 0x00,
        LSM6DSL_ACC_GYRO_FS_XL_16g = 0x04,
        LSM6DSL_ACC_GYRO_FS_XL_4g = 0x08,
        LSM6DSL_ACC_GYRO_FS_XL_8g = 0x0C,
    } LSM6DSL_ACC_GYRO_FS_XL_t;

#define LSM6DSL_ACC_GYRO_FS_XL_MASK 0x0C
    bool LSM6DSL_ACC_GYRO_FS_XL(LSM6DSL_ACC_GYRO_FS_XL_t tFsXLData);

    /*******************************************************************************
     * Register      : CTRL2_G
     * Address       : 0X11
     * Bit Group Name: ODR_G
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN = 0x00,
        LSM6DSL_ACC_GYRO_ODR_G_13Hz = 0x10,
        LSM6DSL_ACC_GYRO_ODR_G_26Hz = 0x20,
        LSM6DSL_ACC_GYRO_ODR_G_52Hz = 0x30,
        LSM6DSL_ACC_GYRO_ODR_G_104Hz = 0x40,
        LSM6DSL_ACC_GYRO_ODR_G_208Hz = 0x50,
        LSM6DSL_ACC_GYRO_ODR_G_416Hz = 0x60,
        LSM6DSL_ACC_GYRO_ODR_G_833Hz = 0x70,
        LSM6DSL_ACC_GYRO_ODR_G_1660Hz = 0x80,
        LSM6DSL_ACC_GYRO_ODR_G_3330Hz = 0x90,
        LSM6DSL_ACC_GYRO_ODR_G_6660Hz = 0xA0,
    } LSM6DSL_ACC_GYRO_ODR_G_t;

#define LSM6DSL_ACC_GYRO_ODR_G_MASK 0xF0
    bool LSM6DSL_ACC_GYRO_ODR_G(LSM6DSL_ACC_GYRO_ODR_G_t tODRData);

    /*******************************************************************************
     * Register      : CTRL2_G
     * Address       : 0X11
     * Bit Group Name: FS_125
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_FS_125_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_FS_125_ENABLED = 0x02,
    } LSM6DSL_ACC_GYRO_FS_125_t;

#define LSM6DSL_ACC_GYRO_FS_125_MASK 0x02
    bool LSM6DSL_ACC_GYRO_FS_125(LSM6DSL_ACC_GYRO_FS_125_t tFs125Data);
    /*******************************************************************************
     * Register      : CTRL2_G
     * Address       : 0X11
     * Bit Group Name: FS_G
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_FS_G_250dps = 0x00,
        LSM6DSL_ACC_GYRO_FS_G_500dps = 0x04,
        LSM6DSL_ACC_GYRO_FS_G_1000dps = 0x08,
        LSM6DSL_ACC_GYRO_FS_G_2000dps = 0x0C,
    } LSM6DSL_ACC_GYRO_FS_G_t;

#define LSM6DSL_ACC_GYRO_FS_G_MASK 0x0C
    bool LSM6DSL_ACC_GYRO_FS_G(LSM6DSL_ACC_GYRO_FS_G_t tFsGData);

    /*******************************************************************************
     * Register      : CTRL3_C
     * Address       : 0X12
     * Bit Group Name: IF_INC
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_IF_INC_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_IF_INC_ENABLED = 0x04,
    } LSM6DSL_ACC_GYRO_IF_INC_t;

#define LSM6DSL_ACC_GYRO_IF_INC_MASK 0x04
    bool LSM6DSL_ACC_GYRO_IF_Addr_Incr(LSM6DSL_ACC_GYRO_IF_INC_t tData);

    /*******************************************************************************
     * Register      : CTRL3_C
     * Address       : 0X12
     * Bit Group Name: H_ACTIVE
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_H_ACTIVE_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_H_ACTIVE_ENABLED = 0x20,
    } LSM6DSL_ACC_GYRO_H_ACTIVE_t;
#define LSM6DSL_ACC_GYRO_H_ACTIVE_MASK 0x20
    bool LSM6DSL_ACC_GYRO_HACTIVE(LSM6DSL_ACC_GYRO_H_ACTIVE_t tHACtiveData);
    /*******************************************************************************
     * Register      : CTRL10_C
     * Address       : 0X19
     * Bit Group Name: FUNC_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_FUNC_EN_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_FUNC_EN_ENABLED = 0x04,
    } LSM6DSL_ACC_GYRO_FUNC_EN_t;

#define LSM6DSL_ACC_GYRO_FUNC_EN_MASK 0x04
    bool LSM6DSL_ACC_GYRO_FUNC_EN(LSM6DSL_ACC_GYRO_FUNC_EN_t tEnable);

    typedef enum
    {
        LSM6DSL_INT1_PIN,
        LSM6DSL_INT2_PIN
    } LSM6DSL_Interrupt_Pin_t;

    /*******************************************************************************
     * Register      : CTRL10_C
     * Address       : 0X19
     * Bit Group Name: TILT_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_TILT_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_TILT_ENABLED = 0x08,
    } LSM6DSL_ACC_GYRO_TILT_G_t;

#define LSM6DSL_ACC_GYRO_TILT_MASK 0x08
    bool LSM6DSL_ACC_GYRO_TILT_CAL(LSM6DSL_ACC_GYRO_TILT_G_t tEnable);

    /*******************************************************************************
     * Register      : CTRL10_C
     * Address       : 0X19
     * Bit Group Name: PEDO_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_PEDO_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_PEDO_ENABLED = 0x10,
    } LSM6DSL_ACC_GYRO_PEDO_t;

#define LSM6DSL_ACC_GYRO_PEDO_MASK 0x10
    bool LSM6DSL_ACC_GYRO_PEDO(LSM6DSL_ACC_GYRO_PEDO_t tRegData);

    /*******************************************************************************
     * Register      : CTRL10_C
     * Address       : 0X19
     * Bit Group Name: PEDO_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_WRIST_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_WRIST_ENABLED = 0x80,
    } LSM6DSL_ACC_GYRO_WRIST_TILT_t;

#define LSM6DSL_ACC_GYRO_WRIST_MASK 0x80
    bool LSM6DSL_ACC_GYRO_WRIST_TILT(LSM6DSL_ACC_GYRO_WRIST_TILT_t tRegData);

    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_TILT
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_TILT_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_TILT_ENABLED = 0x02,
    } LSM6DSL_ACC_GYRO_INT1_TILT_t;

#define LSM6DSL_ACC_GYRO_INT1_TILT_MASK 0x02
    bool LSM6DSL_ACC_GYRO_TiltEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TILT_t tEnable);

    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_SINGLE_TAP
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_ENABLED = 0x40,
    } LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_t;

#define LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_MASK 0x40
    bool LSM6DSL_ACC_GYRO_SingleTapOnInt1(LSM6DSL_ACC_GYRO_INT1_SINGLE_TAP_t tRegData);

    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_FF
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_FF_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_FF_ENABLED = 0x10,
    } LSM6DSL_ACC_GYRO_INT1_FF_t;

#define LSM6DSL_ACC_GYRO_INT1_FF_MASK 0x10
    // mems_status_t LSM6DSL_ACC_GYRO_W_FFEvOnInt1(void *handle, LSM6DSL_ACC_GYRO_INT1_FF_t newValue);
    // mems_status_t LSM6DSL_ACC_GYRO_R_FFEvOnInt1(void *handle, LSM6DSL_ACC_GYRO_INT1_FF_t *value);

    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_TILT
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_TILT_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_TILT_ENABLED = 0x02,
    } LSM6DSL_ACC_GYRO_INT2_TILT_t;

#define LSM6DSL_ACC_GYRO_INT2_TILT_MASK 0x02
    bool LSM6DSL_ACC_GYRO_TiltEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TILT_t tEnable);

    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_SINGLE_TAP
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_ENABLED = 0x40,
    } LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_t;

#define LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_MASK 0x40
    bool LSM6DSL_ACC_GYRO_SingleTapOnInt2(LSM6DSL_ACC_GYRO_INT2_SINGLE_TAP_t u8RegData);

    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_FF
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_FF_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_FF_ENABLED = 0x10,
    } LSM6DSL_ACC_GYRO_INT2_FF_t;

#define LSM6DSL_ACC_GYRO_INT2_FF_MASK 0x10
    // mems_status_t LSM6DSL_ACC_GYRO_W_FFEvOnInt2(void *handle, LSM6DSL_ACC_GYRO_INT2_FF_t newValue);
    // mems_status_t LSM6DSL_ACC_GYRO_R_FFEvOnInt2(void *handle, LSM6DSL_ACC_GYRO_INT2_FF_t *value);

    /*******************************************************************************
     * Register      : WAKE_UP_DUR
     * Address       : 0X5C
     * Bit Group Name: WAKE_DUR
     * Permission    : RW
     *******************************************************************************/
#define LSM6DSL_ACC_GYRO_WAKE_DUR_MASK 0x60
#define LSM6DSL_ACC_GYRO_WAKE_DUR_POSITION 5
    bool LSM6DSL_ACC_GYRO_WAKE_DUR(uint8_t u8RegData);
    /*******************************************************************************
     * Register      : WAKE_UP_THS
     * Address       : 0X5B
     * Bit Group Name: WK_THS
     * Permission    : RW
     *******************************************************************************/
#define LSM6DSL_ACC_GYRO_WK_THS_MASK 0x3F
#define LSM6DSL_ACC_GYRO_WK_THS_POSITION 0

    bool LSM6DSL_ACC_GYRO_WK_THS(uint8_t u8RegData);

    /*******************************************************************************
     * Register      : WAKE_UP_THS
     * Address       : 0X5B
     * Bit Group Name: SINGLE_DOUBLE_TAP
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_SINGLE_TAP = 0x00,
        LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_DOUBLE_TAP = 0x80,
    } LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_t;

#define LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_MASK 0x80
    // bool LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_EV(LSM6DSL_ACC_GYRO_SINGLE_DOUBLE_TAP_t tRegData);

    /*******************************************************************************
     * Register      : WAKE_UP_SRC
     * Address       : 0X1B
     * Bit Group Name: WU_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_WU_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_WU_EV_STATUS_DETECTED = 0x08,
    } LSM6DSL_ACC_GYRO_WU_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_WU_EV_STATUS_MASK 0x08
#define LSM6DSL_ACC_GYRO_X_WU_EV_STATUS_MASK 0x04
#define LSM6DSL_ACC_GYRO_Y_WU_EV_STATUS_MASK 0x02
#define LSM6DSL_ACC_GYRO_Z_WU_EV_STATUS_MASK 0x01
    //	mems_status_t LSM6DSL_ACC_GYRO_R_WU_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_WU_EV_STATUS_t *value);
    /*******************************************************************************
     * Register      : TAP_CFG1
     * Address       : 0X58
     * Bit Group Name: INTERRUPTS_ENABLE
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_BASIC_INT_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_BASIC_INT_ENABLED = 0x80,
    } LSM6DSL_ACC_GYRO_INT_EN_t;

#define LSM6DSL_ACC_GYRO_INT_EN_MASK 0x80
    bool LSM6DSL_ACC_GYRO_BASIC_INT(LSM6DSL_ACC_GYRO_INT_EN_t tRegData);
    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_WU
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_WU_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_WU_ENABLED = 0x20,
    } LSM6DSL_ACC_GYRO_INT1_WU_t;

#define LSM6DSL_ACC_GYRO_INT1_WU_MASK 0x20
    bool LSM6DSL_ACC_GYRO_WUEvOnInt1(LSM6DSL_ACC_GYRO_INT1_WU_t tRegData);

    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_TAP
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_TAP_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_TAP_ENABLED = 0x08,
    } LSM6DSL_ACC_GYRO_INT1_TAP_t;

#define LSM6DSL_ACC_GYRO_INT1_TAP_MASK 0x08
    bool LSM6DSL_ACC_GYRO_TapEvOnInt1(LSM6DSL_ACC_GYRO_INT1_TAP_t tRegData);

    /*******************************************************************************
     * Register      : MD1_CFG
     * Address       : 0X5E
     * Bit Group Name: INT1_6D
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_6D_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_6D_ENABLED = 0x04,
    } LSM6DSL_ACC_GYRO_INT1_6D_t;

#define LSM6DSL_ACC_GYRO_INT1_6D_MASK 0x04
    // mems_status_t LSM6DSL_ACC_GYRO_W_6DEvOnInt1(void *handle, LSM6DSL_ACC_GYRO_INT1_6D_t newValue);
    // mems_status_t LSM6DSL_ACC_GYRO_R_6DEvOnInt1(void *handle, LSM6DSL_ACC_GYRO_INT1_6D_t *value);
    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_WU
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_WU_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_WU_ENABLED = 0x20,
    } LSM6DSL_ACC_GYRO_INT2_WU_t;

#define LSM6DSL_ACC_GYRO_INT2_WU_MASK 0x20
    bool LSM6DSL_ACC_GYRO_WUEvOnInt2(LSM6DSL_ACC_GYRO_INT2_WU_t tRegData);

    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_TAP
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_TAP_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_TAP_ENABLED = 0x08,
    } LSM6DSL_ACC_GYRO_INT2_TAP_t;

#define LSM6DSL_ACC_GYRO_INT2_TAP_MASK 0x08
    bool LSM6DSL_ACC_GYRO_TapEvOnInt2(LSM6DSL_ACC_GYRO_INT2_TAP_t tRegData);

    /*******************************************************************************
     * Register      : MD2_CFG
     * Address       : 0X5F
     * Bit Group Name: INT2_6D
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_6D_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT2_6D_ENABLED = 0x04,
    } LSM6DSL_ACC_GYRO_INT2_6D_t;

#define LSM6DSL_ACC_GYRO_INT2_6D_MASK 0x04
    // mems_status_t LSM6DSL_ACC_GYRO_W_6DEvOnInt2(void *handle, LSM6DSL_ACC_GYRO_INT2_6D_t newValue);
    // mems_status_t LSM6DSL_ACC_GYRO_R_6DEvOnInt2(void *handle, LSM6DSL_ACC_GYRO_INT2_6D_t *value);
    /*******************************************************************************
     * Register      : FUNC_CFG_ACCESS
     * Address       : 0X01
     * Bit Group Name: EMB_ACC
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_BANKA_ENABLED = 0x80, // bank A
        LSM6DSL_ACC_GYRO_EMBEDDED_ACCESS_BANKB_ENABLED = 0xA0  // bank B
    } LSM6DSL_ACC_GYRO_EMB_ACC_t;

#define LSM6DSL_ACC_GYRO_EMB_BANKA_MASK 0x80
#define LSM6DSL_ACC_GYRO_EMB_BANKB_MASK 0xA0
    bool LSM6DSL_ACC_GYRO_EmbeddedAccess(LSM6DSL_ACC_GYRO_EMB_ACC_t u8RegData);

    /*******************************************************************************
     * Register      : INT1_CTRL
     * Address       : 0X0D
     * Bit Group Name: INT1_STEP_DETECTOR
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT1_PEDO_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_INT1_PEDO_ENABLED = 0x80,
    } LSM6DSL_ACC_GYRO_INT1_PEDO_t;

#define LSM6DSL_ACC_GYRO_INT1_PEDO_MASK 0x80
    bool LSM6DSL_ACC_GYRO_STEP_DET_on_INT1(LSM6DSL_ACC_GYRO_INT1_PEDO_t tRegData);

    /*******************************************************************************
     * Register      : TAP_CFG1
     * Address       : 0X58
     * Bit Group Name: TAP_X_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_TAP_X_EN_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_TAP_X_EN_ENABLED = 0x08,
    } LSM6DSL_ACC_GYRO_TAP_X_EN_t;

#define LSM6DSL_ACC_GYRO_TAP_X_EN_MASK 0x08
    bool LSM6DSL_ACC_GYRO_TAP_X_EN(LSM6DSL_ACC_GYRO_TAP_X_EN_t u8RegData);

    /*******************************************************************************
     * Register      : TAP_CFG1
     * Address       : 0X58
     * Bit Group Name: TAP_Y_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_TAP_Y_EN_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_TAP_Y_EN_ENABLED = 0x04,
    } LSM6DSL_ACC_GYRO_TAP_Y_EN_t;

#define LSM6DSL_ACC_GYRO_TAP_Y_EN_MASK 0x04
    bool LSM6DSL_ACC_GYRO_TAP_Y_EN(LSM6DSL_ACC_GYRO_TAP_Y_EN_t u8RegData);

    /*******************************************************************************
     * Register      : TAP_CFG1
     * Address       : 0X58
     * Bit Group Name: TAP_Z_EN
     * Permission    : RW
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_TAP_Z_EN_DISABLED = 0x00,
        LSM6DSL_ACC_GYRO_TAP_Z_EN_ENABLED = 0x02,
    } LSM6DSL_ACC_GYRO_TAP_Z_EN_t;

#define LSM6DSL_ACC_GYRO_TAP_Z_EN_MASK 0x02
    bool LSM6DSL_ACC_GYRO_TAP_Z_EN(LSM6DSL_ACC_GYRO_TAP_Z_EN_t u8RegData);

    /*******************************************************************************
     * Register      : TAP_SRC
     * Address       : 0X1C
     * Bit Group Name: SINGLE_TAP_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_DETECTED = 0x20,
    } LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_MASK 0x20
    // mems_status_t LSM6DSL_ACC_GYRO_R_SINGLE_TAP_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_SINGLE_TAP_EV_STATUS_t *value);
    /*******************************************************************************
     * Register      : TAP_SRC
     * Address       : 0X1C
     * Bit Group Name: DOUBLE_TAP_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_DETECTED = 0x10,
    } LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_MASK 0x10
    // mems_status_t LSM6DSL_ACC_GYRO_R_DOUBLE_TAP_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_t *value);

    // mems_status_t LSM6DSL_ACC_GYRO_R_DOUBLE_TAP_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_DOUBLE_TAP_EV_STATUS_t *value);
/*******************************************************************************
 * Register      : TAP_THS_6D
 * Address       : 0X59
 * Bit Group Name: TAP_THS
 * Permission    : RW
 *******************************************************************************/
#define LSM6DSL_ACC_GYRO_TAP_THS_MASK 0x1F
#define LSM6DSL_ACC_GYRO_TAP_THS_POSITION 0
    bool LSM6DSL_ACC_GYRO_TAP_THS(uint8_t u8RegData);

/*******************************************************************************
 * Register      : INT_DUR2
 * Address       : 0X5A
 * Bit Group Name: SHOCK
 * Permission    : RW
 *******************************************************************************/
#define LSM6DSL_ACC_GYRO_SHOCK_MASK 0x03
#define LSM6DSL_ACC_GYRO_SHOCK_POSITION 0
    bool LSM6DSL_ACC_GYRO_SHOCK_Duration(uint8_t u8RegData);

/*******************************************************************************
 * Register      : INT_DUR2
 * Address       : 0X5A
 * Bit Group Name: QUIET
 * Permission    : RW
 *******************************************************************************/
#define LSM6DSL_ACC_GYRO_QUIET_MASK 0x0C
#define LSM6DSL_ACC_GYRO_QUIET_POSITION 2
    bool LSM6DSL_ACC_GYRO_QUIET_Duration(uint8_t u8RegData);

/*******************************************************************************
 * Register      : INT_DUR2
 * Address       : 0X5A
 * Bit Group Name: DUR
 * Permission    : RW
 *******************************************************************************/
#define LSM6DSL_ACC_GYRO_DUR_MASK 0xF0
#define LSM6DSL_ACC_GYRO_DUR_POSITION 4
    bool LSM6DSL_ACC_GYRO_DUR(uint8_t u8RegData);

    /*******************************************************************************
     * Register      : D6D_SRC
     * Address       : 0X1D
     * Bit Group Name: D6D_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_D6D_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_D6D_EV_STATUS_DETECTED = 0x40,
    } LSM6DSL_ACC_GYRO_D6D_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_D6D_EV_STATUS_MASK 0x40
    // mems_status_t LSM6DSL_ACC_GYRO_R_D6D_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_D6D_EV_STATUS_t *value);

    /*******************************************************************************
     * Register      : FUNC_SRC
     * Address       : 0X53
     * Bit Group Name: PEDO_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_DETECTED = 0x10,
    } LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_MASK 0x10
    // mems_status_t LSM6DSL_ACC_GYRO_R_PEDO_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_PEDO_EV_STATUS_t *value);
    /*******************************************************************************
     * Register      : FUNC_SRC
     * Address       : 0X53
     * Bit Group Name: TILT_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_TILT_EV_STATUS_NOT_DETECTED = 0x00,
        LSM6DSL_ACC_GYRO_TILT_EV_STATUS_DETECTED = 0x20,
    } LSM6DSL_ACC_GYRO_TILT_EV_STATUS_t;

#define LSM6DSL_ACC_GYRO_TILT_EV_STATUS_MASK 0x20
    // mems_status_t LSM6DSL_ACC_GYRO_R_TILT_EV_STATUS(void *handle, LSM6DSL_ACC_GYRO_TILT_EV_STATUS_t *value);

    /*******************************************************************************
     * Register      : A_WRIST_TILT_LAT
     * Address       : 0X50
     * Permission    : RO
     *******************************************************************************/
    bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_LAT(uint8_t u8WRISTLatch);
#define LSM6DSL_ACC_GYRO_A_WRIST_TILT_LAT_MASK 0xFF

    /*******************************************************************************
     * Register      : A_WRIST_TILT_THS
     * Address       : 0X54
     * Permission    : RO
     *******************************************************************************/
    bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS(uint8_t u8WRISTTHS);
#define LSM6DSL_ACC_GYRO_A_WRIST_TILT_THS_MASK 0xFF

    /*******************************************************************************
     * Register      : FUNC_SRC
     * Address       : 0X53
     * Bit Group Name: TILT_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XPos = 0x80,

        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_XNeg = 0x40,

        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YPos = 0x20,

        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_YNeg = 0x10,

        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZPos = 0x08,

        LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_ZNeg = 0x04
    } LSM6DSL_ACC_GYRO_A_WRIST_MASK_t;

    bool LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK(LSM6DSL_ACC_GYRO_A_WRIST_MASK_t tWRISTMask);
#define LSM6DSL_ACC_GYRO_A_WRIST_TILT_MASK_MASK 0xFA

    /*******************************************************************************
     * Register      : FUNC_SRC
     * Address       : 0X53
     * Bit Group Name: TILT_EV_STATUS
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_DISABLE = 0x00,

        LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_ENABLE = 0x01
    } LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_t;
#define LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_MASK_t 0x01

    bool LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG(LSM6DSL_ACC_GYRO_DRDY_PULSE_CFG_t tReg);

    /*******************************************************************************
     * Register      : CTRL4_C
     * Address       : 0X13
     * Bit Group Name: INT2_on_INT1
     * Permission    : RO
     *******************************************************************************/
    typedef enum
    {
        LSM6DSL_ACC_GYRO_INT2_on_INT1_DISABLE = 0x00,

        LSM6DSL_ACC_GYRO_INT2_on_INT1_ENABLE = 0x20
    } LSM6DSL_ACC_GYRO_INT2_on_INT1_t;
#define LSM6DSL_ACC_GYRO_INT2_on_INT1_MASK_t 0x20

    bool LSM6DSL_ACC_GYRO_INT2_on_INT1(LSM6DSL_ACC_GYRO_INT2_on_INT1_t tReg);

/*DEFINE PARAMETER THRESOLD*/
/***PEDOMETER */
#define LSM6DSL_PEDOMETER_THRESHOLD_LOW 0x00 /**< Lowest  value of pedometer threshold */
#define LSM6DSL_PEDOMETER_THRESHOLD_MID_LOW 0x07
#define LSM6DSL_PEDOMETER_THRESHOLD_MID 0x0F
#define LSM6DSL_PEDOMETER_THRESHOLD_MID_HIGH 0x17
#define LSM6DSL_PEDOMETER_THRESHOLD_HIGH 0x1F /**< Highest value of pedometer threshold */
    bool LSM6DSL_ACC_GYRO_PedoThreshold(uint8_t u8ThresholdValue);

/***SINGLE TAP  */
#define LSM6DSL_TAP_THRESHOLD_LOW 0x01 /**< Lowest  value of wake up threshold */
#define LSM6DSL_TAP_THRESHOLD_MID_LOW 0x08
#define LSM6DSL_TAP_THRESHOLD_MID 0x10
#define LSM6DSL_TAP_THRESHOLD_MID_HIGH 0x18
#define LSM6DSL_TAP_THRESHOLD_HIGH 0x1F /**< Highest value of wake up threshold */

#define LSM6DSL_TAP_SHOCK_TIME_LOW 0x00 /**< Lowest  value of wake up threshold */
#define LSM6DSL_TAP_SHOCK_TIME_MID_LOW 0x01
#define LSM6DSL_TAP_SHOCK_TIME_MID_HIGH 0x02
#define LSM6DSL_TAP_SHOCK_TIME_HIGH 0x03 /**< Highest value of wake up threshold */

#define LSM6DSL_TAP_QUIET_TIME_LOW 0x00 /**< Lowest  value of wake up threshold */
#define LSM6DSL_TAP_QUIET_TIME_MID_LOW 0x01
#define LSM6DSL_TAP_QUIET_TIME_MID_HIGH 0x02
#define LSM6DSL_TAP_QUIET_TIME_HIGH 0x03 /**< Highest value of wake up threshold */

/**Double TAP */
#define LSM6DSL_TAP_DURATION_TIME_LOW 0x00 /**< Lowest  value of wake up threshold */
#define LSM6DSL_TAP_DURATION_TIME_MID_LOW 0x04
#define LSM6DSL_TAP_DURATION_TIME_MID 0x08
#define LSM6DSL_TAP_DURATION_TIME_MID_HIGH 0x0C
#define LSM6DSL_TAP_DURATION_TIME_HIGH 0x0F /**< Highest value of wake up threshold */

// Control register 2 bit position
#define LSM6DSL_GYRO_FULLSCALE_BITPOSITION ((uint8_t)0x0C) // Full-scale bit position
#define LSM6DSL_GYRO_FS_245 ((uint8_t)0x00)
#define LSM6DSL_GYRO_FS_500 ((uint8_t)0x04)
#define LSM6DSL_GYRO_FS_1000 ((uint8_t)0x08)
#define LSM6DSL_GYRO_FS_2000 ((uint8_t)0x0C)
#define LSM6DSL_GYRO_FS_125_BITPOSITION ((uint8_t)0x02) // 125 dps bit position

// Control register 6 bit position
#define LSM6DSL_CTRL6_GYRO_TRIG_BIT ((uint8_t)0x80) // Gyro data edge-sensitive trigger enable
#define LSM6DSL_CTRL6_GYRO_LVL_BIT ((uint8_t)0x40)  // Gyro data leve-sensitive trigger enable
#define LSM6DSL_CTRL6_GYRO_LVL2_BIT ((uint8_t)0x20) // Gyro level-sensitive latched enable
#define LSM6DSL_CTRL6_XL_HM_BIT ((uint8_t)0x10)     // Accelero High-performance mode
#define LSM6DSL_CTRL6_XL_USROFF_BIT ((uint8_t)0x08) // XL user offset bits weight
#define LSM6DSL_GYRO_BW_BITPOSITION ((uint8_t)0x03) // Gyro's LPF1 bandwidth selection
#define LSM6DSL_GYRO_BW_0 ((uint8_t)0x00)           // 245Hz, 315Hz, 343Hz, 351Hz
#define LSM6DSL_GYRO_BW_1 ((uint8_t)0x01)           // 195Hz, 224Hz, 234Hz, 237Hz
#define LSM6DSL_GYRO_BW_2 ((uint8_t)0x02)           // 155Hz, 168Hz, 172Hz, 173Hz
#define LSM6DSL_GYRO_BW_3 ((uint8_t)0x03)           // 293Hz, 505Hz, 925Hz, 937Hz

// Control register 7 bit position
#define LSM6DSL_CTRL7_GYRO_HM_BIT ((uint8_t)0x80)         // Gyro High-performance mode
#define LSM6DSL_CTRL7_GYRO_HP_EN_BIT ((uint8_t)0x40)      // Gyro HPF enable
#define LSM6DSL_CTRL7_GYRO_HPM_BITPOSITON ((uint8_t)0x30) // Gyro HPF cutoff bandwidth selection bits
#define LSM6DSL_GYRO_HPM_16 ((uint8_t)0x00)               // Gyro HPF cutoff bandwidth 16mHz
#define LSM6DSL_GYRO_HPM_65 ((uint8_t)0x01)               // Gyro HPF cutoff bandwidth 65mHz
#define LSM6DSL_GYRO_HPM_260 ((uint8_t)0x02)              // Gyro HPF cutoff bandwidth 260mHz
#define LSM6DSL_GYRO_HPM_1040 ((uint8_t)0x03)             // Gyro HPF cutoff bandwidth 1040mHz
#define LSM6DSL_CTRL7_RD_BIT ((uint8_t)0x04)              // Source register rounding function enable

// Accelero Full Scale Sensitivity
#define LSM6DSL_ACC_SENSITIVITY_2G ((float)0.061f)  // Accelerometer sensitivity with 2 g full scale  [mgauss/LSB]
#define LSM6DSL_ACC_SENSITIVITY_4G ((float)0.122f)  // Accelerometer sensitivity with 4 g full scale  [mgauss/LSB]
#define LSM6DSL_ACC_SENSITIVITY_8G ((float)0.244f)  // Accelerometer sensitivity with 8 g full scale  [mgauss/LSB]
#define LSM6DSL_ACC_SENSITIVITY_16G ((float)0.488f) // Accelerometer sensitivity with 16 g full scale [mgauss/LSB]

// Gyro Full Scale Sensitivity
#define LSM6DSL_GYRO_SENSITIVITY_250DPS ((float)8.750f)  // Sensitivity value for 250 dps full scale  [mdps/LSB]
#define LSM6DSL_GYRO_SENSITIVITY_500DPS ((float)17.50f)  // Sensitivity value for 500 dps full scale  [mdps/LSB]
#define LSM6DSL_GYRO_SENSITIVITY_1000DPS ((float)35.00f) // Sensitivity value for 1000 dps full scale [mdps/LSB]
#define LSM6DSL_GYRO_SENSITIVITY_2000DPS ((float)70.00f) // Sensitivity value for 2000 dps full scale [mdps/LSB]

// Output data rate selection
#define LSM6DSL_CTRL1_ODR_BITPOSITION ((uint8_t)0xF0) // Output Data Rate bit position
#define LSM6DSL_ODR_POWER_DOWN ((uint8_t)0x00)        // Power Down mode
#define LSM6DSL_ODR_13Hz ((uint8_t)0x10)              // Low Power mode
#define LSM6DSL_ODR_26Hz ((uint8_t)0x20)              // Low Power mode
#define LSM6DSL_ODR_52Hz ((uint8_t)0x30)              // Low Power mode
#define LSM6DSL_ODR_104Hz ((uint8_t)0x40)             // Normal mode
#define LSM6DSL_ODR_208Hz ((uint8_t)0x50)             // Normal mode
#define LSM6DSL_ODR_416Hz ((uint8_t)0x60)             // High Performance mode
#define LSM6DSL_ODR_833Hz ((uint8_t)0x70)             // High Performance mode
#define LSM6DSL_ODR_1660Hz ((uint8_t)0x80)            // High Performance mode
#define LSM6DSL_ODR_3330Hz ((uint8_t)0x90)            // High Performance mode
#define LSM6DSL_ODR_6660Hz ((uint8_t)0xA0)            // High Performance mode
// Sensor reading interval
#define LSM6DSL_READ_INTERVAL 100U // Sensor reading interval in ms

// I2C timeout
#define LSM6DSL_I2C_TIMEOUT 100U // I2C max timeout

#define LSM6DSL_MAX_TRIALS 5U // I2C max retry

#define LSM6DSL_ERR_CNT_MAX 5U // IC error max

#define LSM6DSL_RESTART_CNT_MAX 2U // IC restart max

#define LSM6DSL_REPORT_ERR_TIMEOUT S_2_mS(120) // Report IC error timeout

//============================================================================//
// User TypeDefs
#define LSM6DSL_G_SENSOR_THRESHOLD 900 // mg
#define LSM6DSL_G_SENSOR_DISTANCE 500
#define LSM6DSL_TIMECHECK 15
//============================================================================//
#pragma pack(1)
    typedef enum
    {
        INT_STAT_NULL = 0x00U,
        INT_STAT_XLDA = 0x01U,
        INT_STAT_GDA = 0x02U,
        INT_STAT_TDA = 0x04U

    } LSM6DSL_IntStatTypeDef;

    // ACCELERO config struct
    typedef struct
    {
        bool bHighPerf;           // High Resolution enabling/disabling
        uint8_t u8OutputDataRate; // OUT data rate
        bool bEnable;             // Enable mode

    } ACCELERO_ConfigTypeDef;

    // ACCELERO filter struct
    typedef struct
    {
        bool bFilterMode;    // Internal filter mode: LPF or HPF
        bool bLPF2_En;       // LPF2 enable
        bool bLPF1_BW;       // LPF1 bandwidth selection
        bool bHPF_RefEn;     // HPF Reference mode selection
        bool bInputComp;     // LPF2 input composite
        uint8_t u8HPCF_Freg; // Frequency selection for LPF2 or HPF

    } ACCELERO_FilterConfigTypeDef;

    // GYRO config struct
    typedef struct
    {
        bool bHighPerf;           // High Resolution enabling/disabling
        uint8_t u8OutputDataRate; // OUT data rate
        bool bEnable;             // Enable full-scale 125 dps

    } GYRO_ConfigTypeDef;

    // GYRO filter struct
    typedef struct
    {
        bool bHPF_En;       // HPF enable
        bool bLPF1_En;      // LPF1 enable
        uint8_t u8LPF1_BW;  // LPF1 bandwidth
        uint8_t u8HPF_Freq; // HPF cutoff frequency

    } GYRO_FilterConfigTypeDef;

    // GYRO interrupt trigger struct
    typedef struct
    {
        bool bTrigEn;
        bool bLVL_En;
        bool bLVL2_En;

    } GYRO_InterruptConfigTypeDef;

    // Accelation interrupt trigger struct
    typedef struct
    {
        bool bIntEnable;
        uint8_t u8PowerEnable;
        uint8_t u8Type;
        uint8_t u8Data;
        bool bFilterSlec;
        bool bTapXEnable;
        bool bTapYEnable;
        bool bTapZEnable;
        bool bLir;
    } ACCELERO_InterruptConfigTypeDef;
    // Interrupt general config struct
    typedef struct
    {
        bool bActiveLevel;
        bool bPP_OD;
        bool bIntChannel;

    } LSM6DSL_IntConfigTypeDef;

    // ACCELERO sensor data
    typedef struct
    {
        // int32_t i32AccXRaw, i32AccYRaw, i32AccZRaw;
        int32_t i32AccX, i32AccY, i32AccZ;
        float fSensitive;
    } LSM6DSL_AccDataTypeDef;

    // GYRO sensor data
    typedef struct
    {
        // int32_t i32AngXRaw, i32AngYRaw, i32AngZRaw;
        int32_t i32AngP, i32AngR, i32AngY;
        float fSensitive;
    } LSM6DSL_GyroDataTypeDef;

    // LSM6DSL sensor data struct
    typedef struct
    {
        LSM6DSL_AccDataTypeDef tAccVal;

        LSM6DSL_GyroDataTypeDef tGyroVal;

    } LSM6DSL_InfoTypeDef;

    typedef enum
    {
        FREE_FALL = 0x00U,
        WAKE_UP = 0x01U,
        ORIENTATION_6D = 0x02U,
        SINGE_TAP = 0x03U,
        DOUBLE_TAP = 0x04U,
        ACTIVE_INACTIVE_RECOG = 0x05U
    } LSM6DSL_IntInterruptTypeDef;

    typedef struct
    {
        uint8_t u8FreeFallStatus;
        uint8_t u8TapStatus;
        uint8_t u8DoubleTapStatus;
        uint8_t u8WakeUpStatus;
        uint8_t u8StepStatus;
        uint8_t u8TiltStatus;
        uint8_t u8D6DOrientationStatus;
        uint8_t u8WristTiltStatus;

        uint8_t u8Axes;
    } LSM6DSL_Event_Status_t;
#pragma pack()

    //============================================================================//
    // User Externs
    //============================================================================//
    extern ACCELERO_ConfigTypeDef tAclrConfig;
    extern GYRO_ConfigTypeDef tGyroConfig;
    extern LSM6DSL_AccDataTypeDef tAccData;
    extern LSM6DSL_GyroDataTypeDef tGyroData;
    extern ACCELERO_FilterConfigTypeDef tAccFilter;
    extern GYRO_FilterConfigTypeDef tGyroFilterl;
    extern LSM6DSL_IntConfigTypeDef tLSM6DSL_IntConfig;
    extern LSM6DSL_InfoTypeDef tG_SenData;
    extern bool bSetCurrOrien;
    //============================================================================//
    // User Function Prototypes
    /**************** General Function  *******************/

    // Init function
    void LSM6DSL_Init(void);

    bool LSM6DSL_WriteReg(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t u8Dat);

    bool LSM6DSL_WriteRegs(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Buf, uint16_t u16Len);

    bool LSM6DSL_ReadReg(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Dat);

    bool LSM6DSL_ReadRegs(uint8_t u8DevAddr, uint8_t u8Reg, uint8_t *pu8Buf, uint16_t u16Len);

    bool LSM6DSL_SetFullScaleXL(float ffullScale);

    bool LSM6DSL_EnableTiltDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_DisableTiltDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_EnableWakeUpDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_DisableWakeUpDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_EnablePedometer(void);

    bool LSM6DSL_DisablePedometer(void);

    bool LSM6DSL_EnableSingleTapDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_DisableSingleTapDetection(void);

    bool LSM6DSL_EnableDoubleTapDetection(LSM6DSL_Interrupt_Pin_t tIntPin);

    bool LSM6DSL_DisableDoubleTapDetection(void);

    void LSM6DSL_I2C_DeInit(void);

    bool LSM6DSL_AccReadXYZ(ACCELERO_ConfigTypeDef *pConfig, LSM6DSL_AccDataTypeDef *pData);

    bool LSM6DSL_GyroReadXYZAngRate(GYRO_ConfigTypeDef *pConfig, LSM6DSL_GyroDataTypeDef *pData);

    void LSM6DSL_ScanBus(void);

    bool LSM6DSL_GetEventStatus(LSM6DSL_Event_Status_t *pStatus);

    bool LSM6DSL_GetStepCounter(uint16_t *pStepCount);

    void LSM6DSL_Proc(void);

    bool LSM6DSL_GyroInit(GYRO_ConfigTypeDef *pGyroInit);

    bool LSM6DSL_AccInit(ACCELERO_ConfigTypeDef *pAccInit);

    bool LSM6DSL_EnableAWTiltDetection(void);

    bool LSM6DSL_DisableAWTiltDetection(void);

    void LSM6DSL_SetStandbyMode(void);

    void LSM6DSL_DeInit(void);

    bool LSM6DSL_SetOrienTation(LSM6DSL_AccDataTypeDef CurrentData, LSM6DSL_AccDataTypeDef LastData);
    //============================================================================//

#ifdef __cplusplus
}
#endif

#endif // __LSM6DSL__H
