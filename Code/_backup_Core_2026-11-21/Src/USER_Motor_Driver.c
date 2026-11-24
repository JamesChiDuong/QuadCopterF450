/*
 * USER_Motor_Driver.c
 *
 *  Created on: Mar 8, 2025
 *      Author: PC
 */
#include <USER_Motor_Driver.h>
#include <main.h>
uint32_t AHBFrequency = 9000000;
static void __Motor_SetMotor1(void);
static void __Motor_SetMotor2(void);
static void __Motor_SetMotor3(void);
static void __Motor_SetMotor4(void);

static void __Motor_SetMotor1(void)
{
	// BLDC1
	HAL_GPIO_WritePin(BLDC1_VCC_PG15_GPIO_Port, BLDC1_VCC_PG15_Pin, 1);
	HAL_GPIO_WritePin(BLDC1_VCC_PG15_GPIO_Port, BLDC1_GND_PG13_Pin, 0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,100);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,50);
	HAL_Delay(2000);
}
static void __Motor_SetMotor2(void)
{
	// BLDC2
	HAL_GPIO_WritePin(BLDC2_VCC_PC8_GPIO_Port, BLDC2_VCC_PC8_Pin, 1);
	HAL_GPIO_WritePin(BLDC2_GND_PC6_GPIO_Port, BLDC2_GND_PC6_Pin, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,50);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,10);
	HAL_Delay(2000);
}
static void __Motor_SetMotor3(void)
{
	// BLDC3
	HAL_GPIO_WritePin(BLDC3_VCC_PC4_GPIO_Port, BLDC3_VCC_PC4_Pin, 1);
	HAL_GPIO_WritePin(BLDC3_GND_PA6_GPIO_Port, BLDC3_GND_PA6_Pin, 0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,100);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,50);
	HAL_Delay(2000);
}
static void __Motor_SetMotor4(void)
{
// BLDC4
	HAL_GPIO_WritePin(BLDC4_VCC_PE14_GPIO_Port, BLDC4_VCC_PE14_Pin, 1); // Turn on power BLDC1
	HAL_GPIO_WritePin(BLDC4_GND_PE12_GPIO_Port, BLDC4_GND_PE12_Pin, 0); // Set GND
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,100);
	HAL_Delay(1000);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,50);
	HAL_Delay(2000);
}
void Motor_Init(void)
{
	__Motor_SetMotor1();
	__Motor_SetMotor2();
	__Motor_SetMotor3();
	__Motor_SetMotor4();
}
bool Motor_SetParam(gMotorParameter MotorParam)
{
	uint32_t Arr;
	uint32_t CCr;
	bool bRes = false;
	switch (MotorParam.u8Motor)
	{
	case 1:
		Arr = ((uint32_t)((AHBFrequency)/(MotorParam.u32Frequency *(htim3.Init.Prescaler+1))) -1);
		CCr = (((uint32_t)MotorParam.u8DutyCle*Arr)/100);
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,CCr);
		bRes = true;
		break;
	case 2:
		Arr = ((uint32_t)((AHBFrequency)/(MotorParam.u32Frequency *(htim1.Init.Prescaler+1))) -1);
		CCr = (((uint32_t)MotorParam.u8DutyCle*Arr)/100);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim1,Arr);
		bRes = true;
		break;
	case 3:
		Arr = ((uint32_t)((AHBFrequency)/(MotorParam.u32Frequency *(htim3.Init.Prescaler+1))) -1);
		CCr = (((uint32_t)MotorParam.u8DutyCle*Arr)/100);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		bRes = true;
		break;
	case 4:
		Arr = ((uint32_t)((AHBFrequency)/(MotorParam.u32Frequency *(htim2.Init.Prescaler+1))) -1);
		CCr = (((uint32_t)MotorParam.u8DutyCle*Arr)/100);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim2,Arr);
		bRes = true;
		break;
	default:
		bRes = false;
		break;
	}
	RTOSprintf("Set motor %d,Arr = %d, CCR = %d",MotorParam.u8Motor,Arr, CCr);
	return bRes;
}

bool Motor_Stop(gMotorParameter MotorParam)
{
	uint32_t Arr = 0;
	uint32_t CCr = 0;
	bool bRes = false;
	switch (MotorParam.u8Motor)
	{
	case 1:
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,CCr);
		bRes = true;
		break;
	case 2:
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim1,Arr);
		bRes = true;
		break;
	case 3:
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		bRes = true;
		break;
	case 4:
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim2,Arr);
		bRes = true;
		break;
	case 5:
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,CCr);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim1,Arr);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim3,Arr);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,CCr);
		__HAL_TIM_SET_AUTORELOAD(&htim2,Arr);
		break;
	default:
		bRes = false;
		break;
	}
	return bRes;
}
