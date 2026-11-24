//============================================================================//
// Include
//============================================================================//
#include "IO_Driver.h"

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "main.h"
#include "REG_MG82F6D64.H"
#include "System.h"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
code u8 u8IOMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//============================================================================//
// Extern
//============================================================================//
extern void DelayXms(u16 xMs);

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  INT0 init.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT0
void INT0_Init(void)
{
	// INT_SetIPHigh(IP_INT0);
	INT_SetINT0P14();				   // nINT0 : P14
	INT_SetINT0_DetectFallingEdge();   // nINT0 Detect: falling edge
	INT_SetINT0Filter_SysclkDiv6_x3(); // nINT0 filter: sysclk/6*3
	INT_SetIPHigh(IP_INT0);
	// INT_EnINT0();
}
#endif

//============================================================================//
// @brief  INT1 init.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT1
void INT1_Init(void)
{
	INT_SetINT1P15();				   // nINT1 : P15
	INT_SetINT1_DetectFallingEdge();   // nINT1 Detect: falling edge
	INT_SetINT1Filter_SysclkDiv6_x3(); // nINT1 filter:sysclk/6*3
	INT_SetIPHigh(IP_INT1);
	// INT_EnINT1();
}
#endif

//============================================================================//
// @brief  INT2 init.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT2
void INT2_Init(void)
{
	INT_SetINT2P11();				   // nINT2 : P11
	INT_SetINT2_DetectFallingEdge();   // nINT0 Detect: falling edge
	INT_SetINT2Filter_SysclkDiv6_x3(); // nINT2 filter:sysclk/6*3
	INT_SetIPHigh(IP_INT2);
	// INT_EnINT2();
}
#endif

//============================================================================//
// @brief  INT3 init.
// @param  None.
// @retval None.
//============================================================================//
#if ENABLE_INT3
void INT3_Init(void)
{
	INT_SetINT3P42();				   // nINT3 : P42
	INT_SetINT3_DetectFallingEdge();   // nINT3 Detect: falling edge
	INT_SetINT3Filter_SysclkDiv6_x3(); // nINT3 filter: sysclk/6*3
	INT_SetIPHigh(IP_INT3);
	// INT_EnINT3();
}
#endif

//============================================================================//
// @brief  GPIO Ports init.
// @param  None.
// @retval None.
//============================================================================//
void InitPort(void)
{
	// PORT0
	PORT_SetP0OpenDrainPu(BIT3 | BIT4); // KEY1, KEY5
	P0 = 0xFF;
	PORT_SetP0PushPull(BIT1);
	// IO_EN3 = 1;
	PWR_MAIN_EN = 1;

	// PORT1
	// PORT_SetP1PushPull(BIT3);			// P13=TX
	PORT_SetP1OpenDrainPu(BIT1 | BIT4 | BIT5); // nG_SEN_INT, nACC_IN, n4G_RI: Input interrupt
	P11 = 1;
	P14 = 1;
	P15 = 1;
	// P1 = 0xFF; //bit=1:open drain open

	// PORT2
	PORT_SetP2PushPull(BIT0 | BIT1 | BIT3 | BIT4 | BIT5); // MCU_WKUP, nRESET_MCU,
														  // IO_EN0, IO_EN1, IO_EN2
	P2 = 0x02;											  // bit=1, MCU_WKUP = 0, nRESET_MCU = 1,
	// IO_EN0 = 1;

	// PORT3
	// PORT_SetP3PushPull(BIT1|BIT3|BIT7);		// P31=TX0,P33=TX2,P37=TX3
	// P3 = 0xFF; //bit=1:open drain open
	PORT_SetP3OpenDrain(BIT5); // WDT_EN
	P3 = 0xFF;				   // bit=1:open drain open
	P32 = 0;				   // Always put 4G module to wake up

	PORT_SetP3PushPull(BIT6);
	PORT_SetP3PushPull(BIT7);
	// PORT4
	// Set P40,P41 as open-drain with pull-high(TWI0:SCL/SDA)
	// Set P42 as INT3 input
	// Set P45 as RTCKO
	PORT_SetP4OpenDrainPu(BIT0 | BIT1 | BIT2 | BIT5);
	P4 = 0xFF; // bit=1:open drain open
}

//============================================================================//
// @brief  Set output pin level.
// @param  nPin: GPIO pin.
//         nOn: 0: Set low, 1: Set high.
// @retval None.
//============================================================================//
void SetOutPin(int nPin, int nOn)
{
	if (nOn == 1)
	{ // output=H
		SysParam.u8IO_Out[nPin] = nOn;
		OUTPUT_PORT |= u8IOMask[nPin];
	}
	else
	{ // output=L
		SysParam.u8IO_Out[nPin] = nOn;
		OUTPUT_PORT &= (~u8IOMask[nPin]);
	}
}

//============================================================================//
// @brief  Send MCU wakeup pulse.
// @param  nWkupPin: GPIO pin assigned as wakeup pin.
//         nDuration: Pulse duration in ms.
// @retval None.
//============================================================================//
void SendWkupPulse(int nWkupPin, int nDuration)
{
	SetOutPin(nWkupPin, 0);

	DelayXms(nDuration);

	SetOutPin(nWkupPin, 1);

	DelayXms(nDuration);

	SetOutPin(nWkupPin, 0);
}

//============================================================================//
// @brief  Send MCU reset pulse.
// @param  nRstPin: GPIO pin assigned as reset pin.
//         nDuration: Pulse duration in ms.
// @retval None.
//============================================================================//
void SendRstPulse(int nRstPin, int nDuration)
{
	SetOutPin(nRstPin, 0);

	DelayXms(nDuration);

	SetOutPin(nRstPin, 1);
}
