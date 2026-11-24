//============================================================================//
// Include
//============================================================================//
#include "Timer_Driver.h"

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "REG_MG82F6D64.H"
#include "System.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
xdata u16 u16TickTime;
xdata u8 u8Tick1Hz;
xdata u8 u8Tick2Hz;
xdata u8 u8Tick5Hz;
xdata u8 u8Tick10Hz;
xdata u8 u8Tick20Hz;

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  T0 init.
// @param  None.
// @retval None.
//============================================================================//
void Timer0_Init(void)
{
	TM_SetT0Mode_1_16BIT_TIMER();			// Set T0 mode:16bit timer
	TM_SetT0Clock_SYSCLKDiv12();			// Set T0 clock:SYSCLK/12
	TM_SetT0Gate_Disable();
	TM_SetT0LowByte(TIMER_12T_1ms_TL);		// Set T0 low byte
	TM_SetT0HighByte(TIMER_12T_1ms_TH);		// Set T0 high byte
	TM_EnableT0();							// Enable T0
	INT_SetIPHigh(IP_T0); // Timer0's priority should be smaller than UART's priority
	INT_EnTIMER0();			// Enable Timer0 interrupt
}

//============================================================================//
// @brief  T0 deinit.
// @param  None.
// @retval None.
//============================================================================//
void Timer0_DeInit(void)
{
	INT_DisTIMER0();	
	TM_EnableT0();
}

//============================================================================//
// @brief  Init WDT.
// @param  None.
// @retval None.
//============================================================================//
void InitWDT(void)
{
	//WDTCR &= 0x7F;
	if ((PCON1 & 0x01) != 0) { //WDTF, watchdog reset
		PCON1 |= 0x01; //write 1 to clear WDTF flag
		SysParam.u8WDTRST = 1;
		WDTCR |= 0x10;
		WDTCR |= 0x10;
	}
	else {
		SysParam.u8WDTRST = 0;
	}
	//if (NV_Set.bEnableWDT == 1)
	WDTCR |= 0xA0;	//WREN=1(7),ENW=1(5),WDT reset enabled
	                //NSW=1(6), WDT continous mode disabled
	WDTCR &= 0xF8;	//1111_1000
	WDTCR |= 0x17;	//CLRW=1,0001_0111, prescale=111, 1.984 s
}

//============================================================================//
// @brief  DeInit WDT.
// @param  None.
// @retval None.
//============================================================================//
void DeInitWDT(void)
{
	WDTCR = 0x00;
}

//============================================================================//
// @brief  Reset WDT.
// @param  None.
// @retval None.
//============================================================================//
void ResetWDT(void)
{
	if (SysParam.u8EnableWDT == 1)
		WDTCR |= 0x10;	//CLRW,clear 9-bit WDT counter to 0000H
}
