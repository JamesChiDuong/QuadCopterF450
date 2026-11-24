//============================================================================//
// Include
//============================================================================//
#include "ADC_Driver.h"

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "REG_MG82F6D64.H"

//============================================================================//
// Define
//============================================================================//       

//============================================================================//
// Variable
//============================================================================//
bit bADC_CpltFlag;
xdata WordTypeDef tADC_Buf[ADC_CHANNEL_NUM] = {0},
									tADC_Val[ADC_CHANNEL_NUM] = {0}, 
                  tADC_Val_Temp[ADC_CHANNEL_NUM] = {0};
xdata uint8_t u8ADC_Busy[ADC_CHANNEL_NUM] = {0};

//============================================================================//
// Extern
//============================================================================//

//============================================================================//
// Function
//============================================================================//

#if ENABLE_ADC0 == 1
//============================================================================//
// @brief  ADC DMA init.
// @param  None.
// @retval None.
//============================================================================//
void DMA_IntConfig(void)
{
	INT_SetIPXHigh(IPX_ADC); // ADC0 must have the same or higher priority than TWI0
	INT_EnDMA();					// Enable DMA interrrupt
}

//============================================================================//
// @brief  ADC init channel 0.
// @param  bIntEn: Enable ADC0 interrupt.
// @retval None.
//============================================================================//
void ADC_Init(bit bIntEn)
{
	ADC_Enable();									// Enable ADC
	ADC_SetClock_SYSCLK();						    // ADC Clock = SYSCLK       sps= 12M/30=600K
	//ADC_SetMode_FreeRunning();						// ADC Trigger mode: Freeruning
	ADC_SetMode_SetADCS();				// ADC Trigger mode: set ADCS
	ADC_SetRightJustified();						// ADC Right-Justified
	ADC_SetADCData_12Bit();							// ADC Data resolution: 12bit
	ADC_SetChannel_AIN0();							// ADC set channel 0
	if(bIntEn)
		DMA_IntConfig();
}

//============================================================================//
// @brief  ADC deinit channel 0.
// @param  None.
// @retval None.
//============================================================================//
void ADC_DeInit(void)
{
	INT_DisDMA();	
	ADC_Disable();
}

#if 0
//============================================================================//
// @brief  Config transfer DMA ADC0 to XRAM.
// @param  None.
// @retval None.
//============================================================================//
void DMA_ConfigADC0_2_XRAM(void)
{
	DMA_SetSource_ADC0();
	DMA_SetDestination_XRAM();
	DMA_SetXRAMStartaddr((u16)(&tADC_Buf[0].BUF[0]));
	DMA_SetXRAMStartaddrRL((u16)(&tADC_Buf[0].BUF[0]));
	DMA_SetCount(65536-(ADC_DATA_CNT*2));
	DMA_SetCountRL(65536-(ADC_DATA_CNT*2));
	DMA_SetTrig_Software();
	DMA_EnDCF0Int();
	DMA_DisLoop();
	DMA_Enable();
}
#endif

//============================================================================//
// @brief  Get the ADC value from an ADC channel.
// @param  nCh: ID of the ADC channel, 0-7.
// @retval None.
//============================================================================//
void GetADC(int nCh)
{
	xdata u8 u8Ch;
	
	u8Ch = (u8)nCh;
	ADCON0 = ADCON0|ADCS|u8Ch;						// set ADCS, Start ADC
  while((ADCON0&ADCI)==0);           				// wait ADC complete
	u8ADC_Busy[nCh] = 1;
	tADC_Val[nCh].BUF[0] = ADCDL;
	tADC_Val[nCh].BUF[1] = ADCDH;
	u8ADC_Busy[nCh] = 0;
}
#endif

