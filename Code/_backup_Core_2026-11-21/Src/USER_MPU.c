#include "USER_MPU.h"
#include "stm32f4xx_hal.h"
#include "main.h"
extern I2C_HandleTypeDef hi2c2;
extern osMutexId mutex_i2c2_user;
Struct_MPU6050 MPU6050;

static float LSB_Sensitivity_ACC;
static float LSB_Sensitivity_GYRO;
uint8_t u8MPU6050_ErrCnt = 0;
bool bMPU6050_Stop = false;
//============================================================================//
// @brief  Make the LSM6DSL IC ready to communicate again.
// @param  None.
// @retval None.
//============================================================================//
static void MPU6050_I2C_ReInit(void)
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
//    HW_PBClr(I2C2_SCL);
//    HW_PBInit(I2C2_SCL, FALSE);
//	HW_PBInit(I2C2_SDA, FALSE);

    if (bRTOS_En)
    {
        osMutexRelease(mutex_i2c2_user);
    }
}
bool MPU6050_Writebyte(uint8_t reg_addr, uint8_t val)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8DatBuf[2] = {0};

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return false;
    }

    u8DatBuf[0] = reg_addr;
    u8DatBuf[1] = val;

    halRes = HAL_I2C_Master_Transmit(&hi2c2, MPU6050_ADDR, u8DatBuf,
                                     2, 100);
    if (halRes != HAL_OK)
    {
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        if (halRes == HAL_BUSY)
        {
        	MPU6050_I2C_ReInit();
        }
        else
        {
        	u8MPU6050_ErrCnt++;

            if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
            {
            	u8MPU6050_ErrCnt = 0;

                if (bMPU6050_Stop == false)
                {
                    RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

                    //MPU6050_Restart();
                }
            }
        }

        RTOSprintf("\r\n> MPU6050 write from register 0x%0X send address ERROR %d", reg_addr, halRes);

        return false;
    }
    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);

    return true;
}

bool MPU6050_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t* data)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return false;
    }
    halRes = HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1);

    if (halRes != HAL_OK)
        {
            if (bRTOS_En)
                osMutexRelease(mutex_i2c2_user);

            if (halRes == HAL_BUSY)
            {
            	MPU6050_I2C_ReInit();
            }
            else
            {
            	u8MPU6050_ErrCnt++;

                if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
                {
                	u8MPU6050_ErrCnt = 0;

                    if (bMPU6050_Stop == false)
                    {
                        RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

                        //MPU6050_Restart();
                    }
                }
            }

            RTOSprintf("\r\n> MPU6050 read from register 0x%0X send address ERROR %d", reg_addr, halRes);

            return false;
        }
        if (bRTOS_En)
            osMutexRelease(mutex_i2c2_user);

        return true;
}

bool MPU6050_Readbyte(uint8_t reg_addr, uint8_t* data)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return false;
    }
    halRes = HAL_I2C_Master_Transmit(&hi2c2, MPU6050_ADDR, &reg_addr,
                                     1, 100);

    if (halRes != HAL_OK)
	{
		if (bRTOS_En)
			osMutexRelease(mutex_i2c2_user);

		if (halRes == HAL_BUSY)
		{
			MPU6050_I2C_ReInit();
		}
		else
		{
			u8MPU6050_ErrCnt++;

			if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
			{
				u8MPU6050_ErrCnt = 0;

				if (bMPU6050_Stop == false)
				{
					RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

					//MPU6050_Restart();
				}
			}
		}

		RTOSprintf("\r\n> MPU6050 read from register 0x%0X send address ERROR %d", reg_addr, halRes);

		return false;
    }
    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)MPU6050_ADDR, data,
                                    1, 100);
    // Read data
    if (halRes != HAL_OK)
            {
                if (bRTOS_En)
                    osMutexRelease(mutex_i2c2_user);

                if (halRes == HAL_BUSY)
                {
                	MPU6050_I2C_ReInit();
                }
                else
                {
                	u8MPU6050_ErrCnt++;

                    if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
                    {
                    	u8MPU6050_ErrCnt = 0;

                        if (bMPU6050_Stop == false)
                        {
                            RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

                            //MPU6050_Restart();
                        }
                    }
                }

                RTOSprintf("\r\n> MPU6050 read from register 0x%0X send address ERROR %d", reg_addr, halRes);

                return false;
            }
            if (bRTOS_En)
                osMutexRelease(mutex_i2c2_user);

    return true;
}

bool MPU6050_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data)
{
    HAL_StatusTypeDef halRes;

    if (bRTOS_En)
    {
        osStatus osRes;

        osRes = osMutexWait(mutex_i2c2_user, S_2_mS(10));

        if (osRes != osOK)
            return false;
    }
    halRes = HAL_I2C_Master_Transmit(&hi2c2, MPU6050_ADDR, &reg_addr,
                                     1, 100);

    if (halRes != HAL_OK)
	{
		if (bRTOS_En)
			osMutexRelease(mutex_i2c2_user);

		if (halRes == HAL_BUSY)
		{
			MPU6050_I2C_ReInit();
		}
		else
		{
			u8MPU6050_ErrCnt++;

			if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
			{
				u8MPU6050_ErrCnt = 0;

				if (bMPU6050_Stop == false)
				{
					RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

					//MPU6050_Restart();
				}
			}
		}

		RTOSprintf("\r\n> MPU6050 read from register 0x%0X send address ERROR %d", reg_addr, halRes);

		return false;
    }

    int j = 0xFFF;
    while (j--)
        ;

    // Read data
    halRes = HAL_I2C_Master_Receive(&hi2c2, (uint16_t)MPU6050_ADDR, data,
    		len, 100);

    if (halRes != HAL_OK)
	{
		if (bRTOS_En)
			osMutexRelease(mutex_i2c2_user);

		if (halRes == HAL_BUSY)
		{
			MPU6050_I2C_ReInit();
		}
		else
		{
			u8MPU6050_ErrCnt++;

			if (u8MPU6050_ErrCnt >= MPU6050_ERR_CNT_MAX)
			{
				u8MPU6050_ErrCnt = 0;

				if (bMPU6050_Stop == false)
				{
					RTOSprintf("\r\n> Maximum MPU6050 errors reached. Restart IC MPU6050.");

					//MPU6050_Restart();
				}
			}
		}

		RTOSprintf("\r\n> MPU6050 read from register 0x%0X send address ERROR %d", reg_addr, halRes);

		return false;
	}
	if (bRTOS_En)
		osMutexRelease(mutex_i2c2_user);

	return true;
}

void MPU6050_Initialization(void)
{

    HAL_GPIO_WritePin(MPU_VCC_GPIO_Port, MPU_VCC_Pin, 1);
    HAL_GPIO_WritePin(MPU_GND_GPIO_Port, MPU_GND_Pin, 0);

	HAL_Delay(1000);

	HAL_I2C_DeInit(&hi2c2);
	HAL_Delay(10);
	HAL_I2C_Init(&hi2c2);


	uint8_t who_am_i = 0;
	RTOSprintf("\r\nChecking MPU6050...\n");

	if(MPU6050_Readbyte(MPU6050_WHO_AM_I, &who_am_i) !=true)
	{
		RTOSprintf("ERROR!\n");
		RTOSprintf("\r\nMPU6050 who_am_i : 0x%02x should be 0x68\n", who_am_i);
	}

	//Reset the whole module before initialization
	if(MPU6050_Writebyte(MPU6050_PWR_MGMT_1, 0x1<<7) !=true)
	{
		RTOSprintf("\r\nMPU6050 MPU6050_PWR_MGMT_1 false\n");
	}
	HAL_Delay(100);

	//Power Management setting
	/* Default is sleep mode
	 * necessary to wake up MPU6050*/
	if(MPU6050_Writebyte(MPU6050_PWR_MGMT_1, 0x00)!=true)
	{
		RTOSprintf("\r\nMPU6050 wake up false\n");
	}
	HAL_Delay(50);

	//Sample rate divider
	/*Sample Rate = Gyroscope Output Rate / (1 + SMPRT_DIV) */
	//	MPU6050_Writebyte(MPU6050_SMPRT_DIV, 0x00); // ACC output rate is 1kHz, GYRO output rate is 8kHz
	if(MPU6050_Writebyte(MPU6050_SMPRT_DIV, 39)!=true) // Sample Rate = 200Hz
	{
		RTOSprintf("\r\nMPU6050 MPU6050_SMPRT_DIV false\n");
	}
	HAL_Delay(50);

	//FSYNC and DLPF setting
	/*DLPF is set to 0*/
	if(MPU6050_Writebyte(MPU6050_CONFIG, 0x00) !=true)
	{
		RTOSprintf("\r\nMPU6050 MPU6050_CONFIG false\n");
	}
	HAL_Delay(50);

	//GYRO FULL SCALE setting
	/*FS_SEL  Full Scale Range
	  0    	+-250 degree/s
	  1		+-500 degree/s
	  2		+-1000 degree/s
	  3		+-2000 degree/s	*/
	uint8_t FS_SCALE_GYRO = 0x0;
	if(MPU6050_Writebyte(MPU6050_GYRO_CONFIG, FS_SCALE_GYRO<<3) != true)
	{
		RTOSprintf("\r\nMPU6050 MPU6050_CONFIG false\n");
	}
	HAL_Delay(50);

	//ACCEL FULL SCALE setting
	/*FS_SEL  Full Scale Range
	  0    	+-2g
	  1		+-4g
	  2		+-8g
	  3		+-16g	*/
	uint8_t FS_SCALE_ACC = 0x0;
	if(MPU6050_Writebyte(MPU6050_ACCEL_CONFIG, FS_SCALE_ACC<<3) !=true)
	{
		RTOSprintf("\r\nMPU6050 MPU6050_ACCEL_CONFIG false\n");
	}
	HAL_Delay(50);

	MPU6050_Get_LSB_Sensitivity(FS_SCALE_GYRO, FS_SCALE_ACC);
	RTOSprintf("\r\nLSB_Sensitivity_GYRO: %0.2f, LSB_Sensitivity_ACC: %0.2f\n",LSB_Sensitivity_GYRO, LSB_Sensitivity_ACC);

	//Interrupt PIN setting
	uint8_t INT_LEVEL = 0x0; //0 - active high, 1 - active low
	uint8_t LATCH_INT_EN = 0x0; //0 - INT 50us pulse, 1 - interrupt clear required
	uint8_t INT_RD_CLEAR = 0x1; //0 - INT flag cleared by reading INT_STATUS, 1 - INT flag cleared by any read operation
	if(MPU6050_Writebyte(MPU6050_INT_PIN_CFG, (INT_LEVEL<<7)|(LATCH_INT_EN<<5)|(INT_RD_CLEAR<<4)) != true) //
	{
		RTOSprintf("\r\\nMPU6050 MPU6050_INT_PIN_CFG false\n");
	}
	HAL_Delay(50);

	//Interrupt enable setting
	uint8_t DATA_RDY_EN = 0x1; // 1 - enable, 0 - disable
	if(MPU6050_Writebyte(MPU6050_INT_ENABLE, DATA_RDY_EN) !=true)
	{
		RTOSprintf("\r\nMPU6050 MPU6050_INT_ENABLE false\n");
	}
	HAL_Delay(50);

	RTOSprintf("\r\nMPU6050 setting is finished\n");
}
/*Get Raw Data from sensor*/
void MPU6050_Get6AxisRawData(Struct_MPU6050* mpu6050)
{
	uint8_t data[14];
	if(MPU6050_Readbytes(MPU6050_ACCEL_XOUT_H, 14, data))
	{
		mpu6050->acc_x_raw = (data[0] << 8) | data[1];
		mpu6050->acc_y_raw = (data[2] << 8) | data[3];
		mpu6050->acc_z_raw = (data[4] << 8) | data[5];

		mpu6050->temperature_raw = (data[6] << 8) | data[7];

		mpu6050->gyro_x_raw = ((data[8] << 8) | data[9]);
		mpu6050->gyro_y_raw = ((data[10] << 8) | data[11]);
		mpu6050->gyro_z_raw = ((data[12] << 8) | data[13]);
	}
	else
	{
		RTOSprintf("\r\n MPU6050 Get 6Axis Raw Data FAILED");
	}
}

void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC)
{
	switch(FS_SCALE_GYRO)
	{
	case 0:
		LSB_Sensitivity_GYRO = 131.f;
		break;
	case 1:
		LSB_Sensitivity_GYRO = 65.5f;
		break;
	case 2:
		LSB_Sensitivity_GYRO = 32.8f;
		break;
	case 3:
		LSB_Sensitivity_GYRO = 16.4f;
		break;
	}
	switch(FS_SCALE_ACC)
	{
	case 0:
		LSB_Sensitivity_ACC = 16384.f;
		break;
	case 1:
		LSB_Sensitivity_ACC = 8192.f;
		break;
	case 2:
		LSB_Sensitivity_ACC = 4096.f;
		break;
	case 3:
		LSB_Sensitivity_ACC = 2048.f;
		break;
	}
}

/*Convert Unit. acc_raw -> g, gyro_raw -> degree per second*/
void MPU6050_DataConvert(Struct_MPU6050* mpu6050)
{
	//printf("LSB_Sensitivity_GYRO: %f, LSB_Sensitivity_ACC: %f\n",LSB_Sensitivity_GYRO,LSB_Sensitivity_ACC);
	mpu6050->acc_x = mpu6050->acc_x_raw / LSB_Sensitivity_ACC;
	mpu6050->acc_y = mpu6050->acc_y_raw / LSB_Sensitivity_ACC;
	mpu6050->acc_z = mpu6050->acc_z_raw / LSB_Sensitivity_ACC;

	mpu6050->temperature = (float)(mpu6050->temperature_raw)/340+36.53;

	mpu6050->gyro_x = mpu6050->gyro_x_raw / LSB_Sensitivity_GYRO;
	mpu6050->gyro_y = mpu6050->gyro_y_raw / LSB_Sensitivity_GYRO;
	mpu6050->gyro_z = mpu6050->gyro_z_raw / LSB_Sensitivity_GYRO;
}


int MPU6050_DataReady(void)
{
	//old school way
	/*
	static uint8_t INT_STATE_FLAG = 0;
	static uint8_t DATA_RDY_INT_FLAG = 0;
	static uint8_t INT_PIN = 0;
	INT_PIN = LL_GPIO_IsInputPinSet(MPU6050_INT_PORT, MPU6050_INT_PIN);
	if(INT_PIN == 1)
	{
		MPU6050_Readbyte(MPU6050_INT_STATUS, &INT_STATE_FLAG); //flag cleared automatically within the sensor
		DATA_RDY_INT_FLAG = INT_STATE_FLAG & 0x01;
		if(DATA_RDY_INT_FLAG == 1)
		{
			INT_STATE_FLAG = 0; //flag clearing
			DATA_RDY_INT_FLAG = 0;
			INT_PIN = 0;
			return 1;
		}
	}
	return 0;
	 */
	return HAL_GPIO_ReadPin(MPU6050_INT_PORT, MPU6050_INT_PIN);
}

void MPU6050_ProcessData(Struct_MPU6050* mpu6050)
{
	MPU6050_Get6AxisRawData(mpu6050);
	MPU6050_DataConvert(mpu6050);
}
