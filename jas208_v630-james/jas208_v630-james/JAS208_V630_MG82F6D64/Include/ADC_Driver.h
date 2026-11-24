#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define ADC_CHANNEL_NUM					8U // 32U
#define ADC_DATA_CNT						1U
#define ADC_CONV_RATE_1HZ       1U
#define ADC_CONV_RATE_2HZ       2U
#define ADC_CONV_RATE           ADC_CONV_RATE_2HZ

//============================================================================//
// Extern
//============================================================================//
extern bit bADC_CpltFlag;
extern xdata WordTypeDef tADC_Buf[ADC_CHANNEL_NUM],
												 tADC_Val[ADC_CHANNEL_NUM], 
												 tADC_Val_Temp[ADC_CHANNEL_NUM];
extern xdata uint8_t u8ADC_Busy[ADC_CHANNEL_NUM];

//============================================================================//
// Function
//============================================================================//
#if ENABLE_ADC0 == 1
void ADC_Init(bit bIntEn);
void ADC_DeInit(void);
#if 0
void DMA_ConfigADC0_2_XRAM(void);
#endif
void DMA_IntConfig(void);
#endif
void GetADC(int nCh);

#endif // _IO_DRIVER_H