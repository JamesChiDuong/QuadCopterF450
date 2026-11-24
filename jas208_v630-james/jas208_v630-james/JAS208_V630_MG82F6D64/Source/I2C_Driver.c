//============================================================================//
// Include
//============================================================================//
#include "I2C_Driver.h"

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "REG_MG82F6D64.H"

//============================================================================//
// Define
//============================================================================//

//============================================================================//
// Variable
//============================================================================//
bit bTWI0ADFlag; //addressed (TWI0) address match
xdata u8 u8TWI0DevAddr = 0x00;
xdata u8 u8TWI0RegAddr = 0x00;
xdata u8 u8TWI1RegAddr = 0x01;
xdata u8 u8TWI0TxInx;
xdata u8 u8TWI0RxInx;
xdata u8	u8TWI0OvTime; // TWI0 timeout counter
xdata u8	u8TWI1OvTime; // TWI1 timeout counter
xdata u8	u8TWI0BlockRead = 0; //master read
xdata u8	u8TWI0BlockWrite = 0; //master write
bit bTWI0RxFlag = 0; // TWI0 RX flag
xdata u8 TWI0TxBuf[I2C_PKT_SIZE];	//used by master i2c block read, slave transmit
xdata u8 TWI0RxBuf[I2C_PKT_SIZE];	//used by master i2c block write, slave receive
xdata u8 u8TWI0UartBuf[I2C_PKT_SIZE]; //I2C to Uart TX

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  TWI0 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void TWI0_IntConfig(void)
{
	INT_SetIPXHigh(IPX_TWI0);
	//INT_SetIPXHighest(IPX_TWI0);
	INT_EnTWI0(); // Enable TWI0 interrupt, I2C slave
}

//============================================================================//
// @brief  TWI0 init.
// @param  bIntEn: 0: Disable TWI0 interrupt, 1: Enable TWI0 interrupt.
// @retval None.
//============================================================================//
void TWI0_Init(bit bIntEn)
{
	TWI0_Clear();
	TWI0_SetClock(TWI0_CLK_SYSCLK_32);
	TWI0_SetSCLSDAP40P41();
	TWI0_Enable();
	TWI0_SetSlaveAddr(TWI0_SLAVE_ADDRESS);
	TWI0_SendACK();
	if(bIntEn)
		TWI0_IntConfig();
}

//============================================================================//
// @brief  TWI0 deinit.
// @param  None.
// @retval None.
//============================================================================//
void TWI0_DeInit(void)
{
	INT_DisTWI0();
	TWI0_Disable();
}

#if ENABLE_TWI1
//============================================================================//
// @brief  TWI1 interrupt config.
// @param  None.
// @retval None.
//============================================================================//
void TWI1_IntConfig(void)
{
	//INT_SetIPX2Highest(IPX2_TWI1); // TWI1 only supports highest prio
	INT_EnTWI1(); // Enable TWI1 interrupt, I2C master
}

//============================================================================//
// @brief  TWI1 init.
// @param  bIntEn: 0: Disable TWI1 interrupt, 1: Enable TWI1 interrupt.
// @retval None.
//============================================================================//
void TWI1_Init(bit bIntEn)
{
	TWI1_Clear();
	TWI1_SetClock(TWI1_CLK_SYSCLK_128);
	TWI1_SetSCLSDAP42P43();
	TWI1_Enable();
	if(bIntEn)
		TWI1_IntConfig();
}

//============================================================================//
// @brief  TWI1 deinit.
// @param  None.
// @retval None.
//============================================================================//
void TWI1_DeInit(void)
{
	INT_DisTWI1();
	TWI1_Disable();
}
#endif

