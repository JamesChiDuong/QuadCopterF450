/*
 * USER_MPU.h
 *
 *  Created on: Apr 6, 2025
 *      Author: PC
 */
/*
 * OVERVIEW
 *  - MPU6050 (accelerometer + gyroscope) driver on I2C2 (SCL PF1, SDA PF0),
 *    powered from PF5 (VCC) / PF4 (GND). Address auto-detected 0x68 / 0x69.
 *  - MPU6050_Initialization: once at start-up. MPU6050_ProcessData: every
 *    BLDCTask cycle (250 Hz). MPU6050_Configure: retried every 2 s while the
 *    sensor is missing.
 *  - Output: struct MPU6050 holds raw counts and converted values
 *    (acc in g, gyro in deg/s, temperature in degC) in SENSOR axes; the
 *    mapping to body (FRD) axes is done by the caller (USER_Config.h).
 *  - All I2C accesses are protected by the mutex mutex_i2c2_user.
 *  - Details (bus recovery, pull-up check, DLPF / range choices): see USER_MPU.c.
 */

#ifndef INC_USER_MPU_H_
#define INC_USER_MPU_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"

#define MPU6050_ADDR      0xD0   /* AD0 = GND (0x68 << 1) */
#define MPU6050_ADDR_ALT  0xD2   /* AD0 = VCC (0x69 << 1) */


/* Register addresses (MPU6050 register map). ACCEL_XOUT_H is the first of the
 * 14 consecutive data registers (accel, temperature, gyro). */
#define MPU6050_SMPRT_DIV 0X19
#define MPU6050_WHO_AM_I 0X75
#define MPU6050_CONFIG 0X1A
#define MPU6050_GYRO_CONFIG 0X1B
#define MPU6050_ACCEL_CONFIG 0X1C
#define MPU6050_INT_PIN_CFG 0X37
#define MPU6050_INT_ENABLE 0X38
#define MPU6050_INT_STATUS 0X3A
#define MPU6050_ACCEL_XOUT_H 0X3B
#define MPU6050_ACCEL_XOUT_L 0X3C
#define MPU6050_PWR_MGMT_1 0X6B //most important



/* Sensor INT ("data ready") pin, read by MPU6050_DataReady (currently unused). */
#define MPU6050_INT_PORT 	GPIOC
#define MPU6050_INT_PIN 	GPIO_PIN_14

extern uint32_t u32MPU6050_ErrTotal;
extern bool     bMPU6050_Present;

/* Detect + configure (retryable, e.g. sensor plugged in after boot) */
bool MPU6050_Configure(bool verbose);

typedef struct _MPU6050{
	short acc_x_raw;
	short acc_y_raw;
	short acc_z_raw;
	short temperature_raw;
	short gyro_x_raw;
	short gyro_y_raw;
	short gyro_z_raw;

	float acc_x;
	float acc_y;
	float acc_z;
	float temperature;
	float gyro_x;
	float gyro_y;
	float gyro_z;
}Struct_MPU6050;

extern Struct_MPU6050 MPU6050;

bool MPU6050_Writebyte(uint8_t reg_addr, uint8_t val);
bool MPU6050_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t* data);
bool MPU6050_Readbyte(uint8_t reg_addr, uint8_t* data);
bool MPU6050_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data);
void MPU6050_Initialization(void);
bool MPU6050_Get6AxisRawData(Struct_MPU6050* mpu6050);
int MPU6050_DataReady(void);
void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC);
void MPU6050_DataConvert(Struct_MPU6050* mpu6050);
bool MPU6050_ProcessData(Struct_MPU6050* mpu6050);

#endif /* INC_USER_MPU_H_ */
