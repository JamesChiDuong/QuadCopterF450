/*
 * ============================================================================
 *  OVERVIEW - MPU6050 driver (3-axis accelerometer + 3-axis gyro) over I2C2
 * ============================================================================
 *  1. Hardware
 *     - I2C2: SCL = PF1, SDA = PF0. Bus speed MPU6050_I2C_SPEED_HZ
 *       (USER_Config.h, default 100 kHz; a 14-byte read takes ~1.6 ms).
 *     - The sensor is powered from GPIOs: MPU_VCC = PF5 (driven high),
 *       MPU_GND = PF4 (driven low), as in the original wiring.
 *     - 7-bit address 0x68 (AD0 = GND) or 0x69 (AD0 = VCC). The HAL wants the
 *       address shifted left by 1 ("8-bit" form): 0x68 << 1 = 0xD0,
 *       0x69 << 1 = 0xD2. Both are probed and the one that answers is used.
 *
 *  2. How an I2C register access works
 *     - Write: [START][addr+W][register][data...][STOP]
 *       -> one HAL_I2C_Master_Transmit of {reg, data...}.
 *     - Read : first transmit {reg} to set the sensor's internal register
 *       pointer, then HAL_I2C_Master_Receive of N bytes. The sensor
 *       auto-increments the pointer, so 14 bytes starting at ACCEL_XOUT_H
 *       give accel XYZ, temperature and gyro XYZ in one burst.
 *     - Every value is 16-bit signed, big-endian (high byte first).
 *
 *  3. Who calls it?
 *     - MPU6050_Initialization(): once at start-up (before the scheduler).
 *     - MPU6050_ProcessData(): BLDCTask, every control cycle (250 Hz).
 *     - MPU6050_Configure(false): BLDCTask, every IMU_RETRY_PERIOD_MS (2 s)
 *       while the sensor is missing (e.g. plugged in after boot).
 *
 *  4. Mutex
 *     I2C2 may be shared, so every transfer takes mutex_i2c2_user (timeout
 *     5 ms). Before the scheduler starts (bRTOS_En == false) no mutex is
 *     used. If the mutex cannot be taken, the call fails (returns false)
 *     instead of blocking the flight task.
 *
 *  5. Error handling and recovery
 *     - Every failed transfer increments u32MPU6050_ErrTotal.
 *     - HAL_BUSY or a bus error -> DeInit + Init of the I2C peripheral
 *       (the old code only did DeInit, so one BUSY killed I2C forever).
 *     - Bus recovery at start-up: if the sensor was reset in the middle of a
 *       byte it may hold SDA low forever. Clocking SCL 9 times by hand lets
 *       it finish that byte and release SDA, then a STOP is generated.
 *     - Pull-up detection trick: the pins are made inputs with the weak
 *       INTERNAL pull-down (~40 kOhm). An external pull-up (2.2..10 kOhm on
 *       the module) is much stronger and wins -> the pin reads HIGH. If it
 *       reads LOW there is no pull-up = module missing / unpowered /
 *       no resistors. Example: 4.7k up vs 40k down -> 3.3 V x 40/44.7
 *       = 2.95 V = HIGH.
 *     - The flight task stops trusting the IMU after IMU_MAX_READ_ERRORS
 *       consecutive errors (handled in USER_System.c).
 *
 *  6. Sensor configuration choices (see MPU6050_CFG_* below)
 *     - DLPF 3 (~42 Hz): removes most motor vibration while adding only a
 *       few ms of delay. The old 260 Hz setting let vibration through.
 *     - Gyro +-500 deg/s: +-250 is easily saturated by quick quad movements.
 *       Sensitivity 65.5 LSB per deg/s -> raw 6550 = 100 deg/s.
 *     - Accel +-8 g: 4096 LSB per g -> raw 4096 = 1 g (level, Z axis).
 * ============================================================================
 */
#include "USER_MPU.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "USER_Config.h"

extern I2C_HandleTypeDef hi2c2;
extern osMutexId mutex_i2c2_user;

/* Global sensor data (raw + converted), filled by MPU6050_ProcessData. */
Struct_MPU6050 MPU6050;

/* Conversion factors raw -> physical unit, set by MPU6050_Get_LSB_Sensitivity
 * according to the selected full-scale range. */
static float LSB_Sensitivity_ACC;
static float LSB_Sensitivity_GYRO;

/* Short timeouts: a 14-byte read at 100 kHz takes ~1.6 ms, so 5 ms is
 * enough and a stuck bus cannot block the 4 ms control loop for long. */
#define MPU6050_I2C_TIMEOUT_MS   5U
#define MPU6050_MUTEX_TIMEOUT_MS 5U

/* Sensor configuration
 *  DLPF 3   : accel 44 Hz / gyro 42 Hz bandwidth, gyro output 1 kHz
 *             (filters motor vibration; the old setting 0 = 260 Hz was noisy)
 *  SMPRT 0  : sample rate = 1 kHz / (1 + 0) = 1 kHz (we read the latest at 250 Hz)
 *  GYRO FS 1: +-500 deg/s (+-250 is easily saturated by a quadcopter)
 *  ACC FS 2 : +-8 g
 */
#define MPU6050_CFG_DLPF        3U
#define MPU6050_CFG_SMPRT_DIV   0U
#define MPU6050_CFG_GYRO_FS     1U
#define MPU6050_CFG_ACC_FS      2U

uint32_t u32MPU6050_ErrTotal = 0;     /* total failed I2C transfers (shown on UI / telemetry) */
bool     bMPU6050_Present = false;    /* true after a successful MPU6050_Configure */

static void MPU6050_I2C_ReInit(void);
static bool MPU6050_Lock(void);
static void MPU6050_Unlock(void);

/* 8-bit I2C address, auto-detected at init (0xD0 or 0xD2) */
static uint16_t s_mpu_addr = MPU6050_ADDR;

/* I2C2: PF0 = SDA, PF1 = SCL. If the sensor was reset in the middle of a
 * transfer it can hold SDA low forever; clocking SCL 9 times releases it. */
/* Must be called with the I2C peripheral de-initialised (the pins are
 * temporarily taken over as GPIOs). Uses HAL_Delay -> start-up only.
 * Side effect: prints a warning for each line that has no pull-up. */
static void MPU6050_BusRecovery(void)
{
    GPIO_InitTypeDef gpio = {0};
    int i;

    __HAL_RCC_GPIOF_CLK_ENABLE();
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    /* Open-drain, like real I2C: the MCU can only pull low; "1" = release.
     * Output latch is set to 1 BEFORE the pins become outputs so they do not
     * glitch low. */
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOF, &gpio);

    /* 9 clocks: worst case the slave is in the middle of sending a byte
     * (8 data bits + 1 ACK bit). Each pulse lets it shift out one bit; once
     * it sees no ACK it releases SDA. 1 ms per half period = ~500 Hz, slow
     * but safe. */
    for (i = 0; i < 9; i++)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);
        HAL_Delay(1);
    }
    /* STOP condition: SDA low -> high while SCL high */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_Delay(1);

    /* Pull-up check: inputs with the weak internal PULL-DOWN (~40 kOhm).
     * The pull-up resistors of an MPU6050 module (2.2..10 kOhm) win -> HIGH.
     * LOW means: no module / no pull-ups on that line (or line shorted). */
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOF, &gpio);
    HAL_Delay(2);
    if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_0) == GPIO_PIN_RESET)
        RTOSprintf("\r\n> I2C2: SDA (PF0) has NO pull-up -> sensor not connected/powered or missing pull-up resistor");
    if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_1) == GPIO_PIN_RESET)
        RTOSprintf("\r\n> I2C2: SCL (PF1) has NO pull-up -> sensor not connected/powered or missing pull-up resistor");

    /* Return the pins to reset state; HAL_I2C_Init (via the MSP callback)
     * will configure them as I2C alternate function again. */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1);
}

/* Converts a HAL I2C error bit mask into a readable hint for the terminal. */
static const char *MPU6050_I2CErrorText(uint32_t err)
{
    if (err & HAL_I2C_ERROR_AF)      return "NACK (no device answers at this address)";
    if (err & HAL_I2C_ERROR_BERR)    return "bus error (wiring / noise)";
    if (err & HAL_I2C_ERROR_ARLO)    return "arbitration lost (bus stuck?)";
    if (err & HAL_I2C_ERROR_TIMEOUT) return "timeout (SCL/SDA stuck, missing pull-ups?)";
    if (err == HAL_I2C_ERROR_NONE)   return "no I2C error";
    return "other";
}

/* Probe 0x68 then 0x69; returns true if one answers */
/* "Probe" = send only the address byte and check for an ACK
 * (HAL_I2C_IsDeviceReady, 3 attempts, 10 ms timeout).
 * Side effect: on success s_mpu_addr is set to the address that answered,
 * so all later reads/writes use it. verbose = print why each address failed. */
static bool MPU6050_Detect(bool verbose)
{
    static const uint16_t addr[2] = { MPU6050_ADDR, MPU6050_ADDR_ALT };
    HAL_StatusTypeDef res;
    uint32_t err;
    int i;

    for (i = 0; i < 2; i++)
    {
        if (!MPU6050_Lock())
            return false;
        res = HAL_I2C_IsDeviceReady(&hi2c2, addr[i], 3, 10);
        err = HAL_I2C_GetError(&hi2c2);
        if (res == HAL_BUSY)
            MPU6050_I2C_ReInit();   /* reset the peripheral so the next address can be tried */
        MPU6050_Unlock();

        if (res == HAL_OK)
        {
            s_mpu_addr = addr[i];
            RTOSprintf("\r\n> I2C2: device found at 0x%02X", addr[i] >> 1);
            return true;
        }
        if (verbose)
        {
            RTOSprintf("\r\n> I2C2: no answer at 0x%02X: %s (HAL err 0x%02lX)", addr[i] >> 1,
                       (res == HAL_BUSY) ? "bus BUSY (SDA/SCL held low?)" :
                       (err == HAL_I2C_ERROR_NONE) ? "NACK (no device answers)" : MPU6050_I2CErrorText(err),
                       (unsigned long)err);
        }
    }
    return false;
}

//============================================================================//
// @brief  Make the I2C bus ready to communicate again after a BUSY error.
//============================================================================//
static void MPU6050_I2C_ReInit(void)
{
    HAL_I2C_DeInit(&hi2c2);
    HAL_I2C_Init(&hi2c2);
}

/* Takes the I2C2 mutex (max 5 ms wait). Before the scheduler runs
 * (bRTOS_En == false) there is no other task, so it always succeeds.
 * Returns false if the mutex was not obtained -> the caller must give up. */
static bool MPU6050_Lock(void)
{
    if (bRTOS_En)
        return (osMutexWait(mutex_i2c2_user, MPU6050_MUTEX_TIMEOUT_MS) == osOK);
    return true;
}

static void MPU6050_Unlock(void)
{
    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);
}

/* Common error handling. Must be called with the bus locked. */
/* Returns true if halRes == HAL_OK. Otherwise counts the error and, if the
 * peripheral looks stuck (BUSY flag or bus error), re-initialises it so the
 * next transfer has a chance to work. A plain NACK needs no re-init. */
static bool MPU6050_CheckResult(HAL_StatusTypeDef halRes)
{
    if (halRes == HAL_OK)
        return true;

    u32MPU6050_ErrTotal++;
    if ((halRes == HAL_BUSY) || (HAL_I2C_GetError(&hi2c2) & HAL_I2C_ERROR_BERR))
        MPU6050_I2C_ReInit();

    return false;
}

bool MPU6050_Writebyte(uint8_t reg_addr, uint8_t val)
{
    return MPU6050_Writebytes(reg_addr, 1, &val);
}

/* Writes len (1..16) bytes starting at register reg_addr.
 * The register address and the data are sent in ONE transfer, so they are
 * packed into one buffer: buf = {reg, data[0], ..., data[len-1]}.
 * Returns true on success (false: bad length, mutex busy or I2C error). */
bool MPU6050_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t* data)
{
    bool bRes;
    uint8_t buf[17];

    if ((len == 0U) || (len > 16U))
        return false;

    buf[0] = reg_addr;
    memcpy(&buf[1], data, len);

    if (!MPU6050_Lock())
        return false;

    bRes = MPU6050_CheckResult(HAL_I2C_Master_Transmit(&hi2c2, s_mpu_addr, buf,
                                                       (uint16_t)(len + 1U), MPU6050_I2C_TIMEOUT_MS));
    MPU6050_Unlock();
    return bRes;
}

bool MPU6050_Readbyte(uint8_t reg_addr, uint8_t* data)
{
    return MPU6050_Readbytes(reg_addr, 1, data);
}

/* Reads len bytes starting at register reg_addr into data[].
 * Returns true on success; on failure data[] content is undefined/unchanged. */
bool MPU6050_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data)
{
    bool bRes;

    if (!MPU6050_Lock())
        return false;

    /* Register pointer write followed by a separate read (as validated on
     * this hardware, see project slides) */
    bRes = MPU6050_CheckResult(HAL_I2C_Master_Transmit(&hi2c2, s_mpu_addr, &reg_addr,
                                                       1, MPU6050_I2C_TIMEOUT_MS));
    if (bRes)
    {
        bRes = MPU6050_CheckResult(HAL_I2C_Master_Receive(&hi2c2, s_mpu_addr,
                                                          data, len, MPU6050_I2C_TIMEOUT_MS));
    }

    MPU6050_Unlock();
    return bRes;
}

/* Delay that works both before the scheduler (HAL_Delay busy-wait) and
 * inside a task (osDelay lets other tasks run meanwhile). */
static void MPU6050_Delay(uint32_t ms)
{
    if (bRTOS_En)
        osDelay(ms);          /* never busy-wait inside the flight task */
    else
        HAL_Delay(ms);
}

/* Writes one configuration register, prints `name` if it failed (verbose),
 * then waits 10 ms so the sensor can apply the setting. */
static bool MPU6050_WriteChecked(uint8_t reg, uint8_t val, const char *name, bool verbose)
{
    bool ok = MPU6050_Writebyte(reg, val);

    if (!ok && verbose)
        RTOSprintf("\r\n> MPU6050 write %s FAILED", name);
    MPU6050_Delay(10);
    return ok;
}

/* Detect the sensor and write its configuration. Can be called again later
 * (e.g. sensor plugged in after boot). Returns true on success. */
bool MPU6050_Configure(bool verbose)
{
    uint8_t who_am_i = 0;
    bool ok = true;

    if (!MPU6050_Detect(verbose))
    {
        bMPU6050_Present = false;
        return false;
    }

    /* WHO_AM_I holds the 7-bit address in bits 6..1 (0x68 for a genuine
     * MPU6050). Clones may return other IDs, so a mismatch is only a warning
     * and configuration continues. */
    if (!MPU6050_Readbyte(MPU6050_WHO_AM_I, &who_am_i) || ((who_am_i & 0x7E) != 0x68))
    {
        RTOSprintf("\r\n> MPU6050 WARNING: who_am_i = 0x%02x, expected 0x68", who_am_i);
    }

    /* Reset the whole module, then wake up (default is sleep mode) and use
     * the X gyro PLL as clock source (more stable than the internal 8 MHz) */
    ok &= MPU6050_WriteChecked(MPU6050_PWR_MGMT_1, 0x80, "PWR_MGMT_1 reset", verbose);
    MPU6050_Delay(100);
    ok &= MPU6050_WriteChecked(MPU6050_PWR_MGMT_1, 0x01, "PWR_MGMT_1 wake", verbose);

    ok &= MPU6050_WriteChecked(MPU6050_SMPRT_DIV, MPU6050_CFG_SMPRT_DIV, "SMPRT_DIV", verbose);
    ok &= MPU6050_WriteChecked(MPU6050_CONFIG, MPU6050_CFG_DLPF, "CONFIG", verbose);
    /* FS_SEL / AFS_SEL are bits 4..3 of the register, hence "<< 3".
     * Example: GYRO_FS 1 -> 0x08 (+-500 deg/s), ACC_FS 2 -> 0x10 (+-8 g). */
    ok &= MPU6050_WriteChecked(MPU6050_GYRO_CONFIG, MPU6050_CFG_GYRO_FS << 3, "GYRO_CONFIG", verbose);
    ok &= MPU6050_WriteChecked(MPU6050_ACCEL_CONFIG, MPU6050_CFG_ACC_FS << 3, "ACCEL_CONFIG", verbose);

    MPU6050_Get_LSB_Sensitivity(MPU6050_CFG_GYRO_FS, MPU6050_CFG_ACC_FS);

    /* INT pin: active high, 50 us pulse, cleared on any read; data ready IRQ */
    ok &= MPU6050_WriteChecked(MPU6050_INT_PIN_CFG, (0U << 7) | (0U << 5) | (1U << 4), "INT_PIN_CFG", verbose);
    ok &= MPU6050_WriteChecked(MPU6050_INT_ENABLE, 0x01, "INT_ENABLE", verbose);

    bMPU6050_Present = ok;
    RTOSprintf("\r\n> MPU6050 %s (addr 0x%02X, gyro %.1f LSB/dps, acc %.0f LSB/g)",
               ok ? "configured" : "configuration FAILED", s_mpu_addr >> 1,
               LSB_Sensitivity_GYRO, LSB_Sensitivity_ACC);
    return ok;
}

/* Start-up sequence (called once, before the scheduler; blocks ~1.3 s):
 * power the sensor from PF5/PF4, wait for it to boot, recover the bus,
 * re-init I2C2 at the configured speed, then detect + configure.
 * A missing sensor is not fatal: BLDCTask retries every 2 s. */
void MPU6050_Initialization(void)
{
    HAL_GPIO_WritePin(MPU_VCC_GPIO_Port, MPU_VCC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MPU_GND_GPIO_Port, MPU_GND_Pin, GPIO_PIN_RESET);

    HAL_Delay(1000);   /* let the sensor power up after its supply pin is switched on */

    /* Release a possibly stuck bus, then re-init I2C at the configured speed
     * (USER_Config.h, PCLK1 = 9 MHz supports up to 400 kHz). */
    HAL_I2C_DeInit(&hi2c2);
    MPU6050_BusRecovery();
    hi2c2.Init.ClockSpeed = MPU6050_I2C_SPEED_HZ;
    hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;   /* SCL low:high = 2:1, only used in 400 kHz fast mode */
    HAL_I2C_Init(&hi2c2);

    RTOSprintf("\r\n> Checking MPU6050 (I2C2 PF1=SCL PF0=SDA, %lu Hz)...",
               (unsigned long)MPU6050_I2C_SPEED_HZ);

    if (!MPU6050_Configure(true))
    {
        RTOSprintf("\r\n> MPU6050 NOT READY. Check: VCC (PF5 or 3V3), GND (PF4 or GND), SCL->PF1, SDA->PF0.");
        RTOSprintf("\r\n>   The flight task retries every 2 s; arming is impossible without the IMU.");
    }
}

/* Get raw data from sensor. Returns false on I2C error (data unchanged). */
bool MPU6050_Get6AxisRawData(Struct_MPU6050* mpu6050)
{
    uint8_t data[14];

    /* Burst read of registers 0x3B..0x48:
     *   data[0..5]  accel X,Y,Z   data[6..7] temperature   data[8..13] gyro X,Y,Z
     * Reading all in one transfer guarantees the 7 values come from the same
     * sample (the sensor does not update the registers mid-burst). */
    if (!MPU6050_Readbytes(MPU6050_ACCEL_XOUT_H, 14, data))
        return false;

    /* Combine high and low byte, then cast to int16_t to get the sign.
     * Example: 0xF0 0x00 -> 0xF000 -> -4096 (= -1 g at +-8 g). */
    mpu6050->acc_x_raw = (int16_t)((data[0] << 8) | data[1]);
    mpu6050->acc_y_raw = (int16_t)((data[2] << 8) | data[3]);
    mpu6050->acc_z_raw = (int16_t)((data[4] << 8) | data[5]);

    mpu6050->temperature_raw = (int16_t)((data[6] << 8) | data[7]);

    mpu6050->gyro_x_raw = (int16_t)((data[8] << 8) | data[9]);
    mpu6050->gyro_y_raw = (int16_t)((data[10] << 8) | data[11]);
    mpu6050->gyro_z_raw = (int16_t)((data[12] << 8) | data[13]);
    return true;
}

/* Selects the raw -> unit factors for the given full-scale codes (datasheet
 * table). Gyro: 0..3 = +-250/500/1000/2000 deg/s; accel: 0..3 = +-2/4/8/16 g.
 * Example: 32768 counts / 500 deg/s = 65.5 LSB per deg/s. */
void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC)
{
    switch (FS_SCALE_GYRO)
    {
    case 0:  LSB_Sensitivity_GYRO = 131.f;  break;
    case 1:  LSB_Sensitivity_GYRO = 65.5f;  break;
    case 2:  LSB_Sensitivity_GYRO = 32.8f;  break;
    default: LSB_Sensitivity_GYRO = 16.4f;  break;
    }
    switch (FS_SCALE_ACC)
    {
    case 0:  LSB_Sensitivity_ACC = 16384.f; break;
    case 1:  LSB_Sensitivity_ACC = 8192.f;  break;
    case 2:  LSB_Sensitivity_ACC = 4096.f;  break;
    default: LSB_Sensitivity_ACC = 2048.f;  break;
    }
}

/* Convert unit: acc_raw -> g, gyro_raw -> degree per second */
void MPU6050_DataConvert(Struct_MPU6050* mpu6050)
{
    mpu6050->acc_x = mpu6050->acc_x_raw / LSB_Sensitivity_ACC;
    mpu6050->acc_y = mpu6050->acc_y_raw / LSB_Sensitivity_ACC;
    mpu6050->acc_z = mpu6050->acc_z_raw / LSB_Sensitivity_ACC;

    /* Datasheet formula: T(degC) = raw / 340 + 36.53 (die temperature). */
    mpu6050->temperature = (float)(mpu6050->temperature_raw) / 340.0f + 36.53f;

    mpu6050->gyro_x = mpu6050->gyro_x_raw / LSB_Sensitivity_GYRO;
    mpu6050->gyro_y = mpu6050->gyro_y_raw / LSB_Sensitivity_GYRO;
    mpu6050->gyro_z = mpu6050->gyro_z_raw / LSB_Sensitivity_GYRO;
}

/* Level of the MPU6050 INT pin (PC14): 1 = new sample ready.
 * Currently not used by the flight code (it simply reads the latest sample
 * every 4 ms). */
int MPU6050_DataReady(void)
{
    return HAL_GPIO_ReadPin(MPU6050_INT_PORT, MPU6050_INT_PIN);
}

/* Read + convert in one call (used by BLDCTask at 250 Hz).
 * Returns false on I2C error; then the struct keeps the previous values. */
bool MPU6050_ProcessData(Struct_MPU6050* mpu6050)
{
    if (!MPU6050_Get6AxisRawData(mpu6050))
        return false;
    MPU6050_DataConvert(mpu6050);
    return true;
}
