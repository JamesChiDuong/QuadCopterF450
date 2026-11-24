/*
 * USER_DataCollect.c
 *
 *  Created on: Apr 13, 2025
 *      Author: PC
 */
#include <USER_DataCollect.h>
#include <USER_CommFunc.h>
void Debug_ShowGSensor()
{
	RTOSprintf("\r\n> MPU6050 Accelerometer[mg] XYZ: X=%.2f, Y=%.2f, Z=%.2f| Gyroscope[mdps] angle PRY : P=%.2f, R=%.2f, Y=%.2f", MPU6050.acc_x, MPU6050.acc_y,
			MPU6050.acc_z, MPU6050.gyro_x, MPU6050.gyro_y, MPU6050.gyro_z);
}
//============================================================================//
// @brief  Handle debug process.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
bool Debug_Proc(void)
{
    //if (UART_IsDebugMode(DBG_GS))
    Debug_ShowGSensor();
    return true;
}
