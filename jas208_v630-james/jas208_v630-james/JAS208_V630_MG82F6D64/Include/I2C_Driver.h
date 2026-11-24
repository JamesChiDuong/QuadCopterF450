#ifndef _I2C_DRIVER_H
#define _I2C_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define TWI0_SCL P40
#define TWI0_SDA P41
#define TWI1_SCL P42
#define TWI1_SDA P43
#define TWI0_SLAVE_ADDRESS 0xA8 // TWI0, 8-bit device address, bit0=RnW
#define INVALID_REG_ADDR 0xFF
#define TWI_OV_TIME_MAX 5 // unit ms
// I2C slave command
// master block write, slave receive
#define I2C_CMD_UART_TX 0x01		// I2C slave receive, master block write, UART TX
#define I2C_CMD_UART_BR_SET 0x02	// I2C slave receive, master set UART Baudrate
#define I2C_CMD_UART_RST 0x03		// I2C slave receive, master clear UART buffer
#define I2C_CMD_UART_RST_ALL 0x04	// I2C slave receive, master clear all UART buffer
#define I2C_CMD_IO_SET 0x07			// I2C slave receive, set IO-OUT
#define I2C_CMD_DT_SET 0x08			// I2C slave receive, set date time
#define I2C_CMD_MCU_WDT_SET 0x09	// I2C slave receive, MCU WDT enable/disable
#define I2C_CMD_MCU_ADC_SET 0x0A	// I2C slave receive, ADC0 enable/disable
#define I2C_CMD_SW_RESET 0x0B		// I2C slave receive, MCU software reset
#define I2C_CMD_PWR_MODE 0x0C		// I2C slave receive, set device power mode
#define I2C_CMD_RS485_SET 0x0D		// I2C slave receive, MCU UART5 mode select between RS232 and RS485
#define I2C_CMD_4G_DTR 0x0E			// I2C slave receive, change 4G DTR pin state
#define I2C_CMD_KEY_RST 0x0F		// I2C slave receive, enable/ disable key reset
#define I2C_CMD_SHUTDOWN 0x10		// I2C slave receive, turn off MAIN_PWR + enter PWR_DOWN
#define I2C_CMD_RTC_WKUP 0x11		// I2C slave receive, set alarm mode or periodic wake up mode for RTC
#define I2C_CMD_USBPROTECT_SET 0x12 // I2C slave receive, set USB Enable
// master block read, slave transmit
#define I2C_CMD_UART_CHK 0x80	  // I2C slave transmit, get all UART available data size
#define I2C_CMD_UART0_RX 0x81	  // I2C slave transmit, UART0 RX
#define I2C_CMD_UART1_RX 0x82	  // I2C slave transmit, UART1 RX
#define I2C_CMD_UART2_RX 0x83	  // I2C slave transmit, UART2 RX
#define I2C_CMD_UART3_RX 0x84	  // I2C slave transmit, UART3 RX
#define I2C_CMD_UART_BR 0x85	  // I2C slave transmit, get all UART BR
#define I2C_CMD_UART0_BR 0x86	  // I2C slave transmit, UART0 BR
#define I2C_CMD_UART1_BR 0x87	  // I2C slave transmit, UART1 BR
#define I2C_CMD_UART2_BR 0x88	  // I2C slave transmit, UART2 BR
#define I2C_CMD_UART3_BR 0x89	  // I2C slave transmit, UART3 BR
#define I2C_CMD_DT_GET 0x8A		  // I2C slave transmit, get date time
#define I2C_CMD_IO_GET 0x8B		  // I2C slave transmit, get IO status
#define I2C_CMD_MCU_WDT_GET 0x8C  // I2C slave transmit, get MCU WDT Enable status
#define I2C_CMD_ADC_MODE 0x8D	  // I2C slave transmit, get ADC0 Enable status
#define I2C_CMD_ADC_GET 0x8E	  // I2C slave transmit, get ADC0 value
#define I2C_CMD_RS485_MODE 0x8F	  // I2C slave transmit, UART5 mode as RS232 or RS485
#define I2C_CMD_GET_VER 0x90	  // I2C slave transmit, get FW version
#define I2C_CMD_OW_CHK_STAT 0x91  // I2C slave transmit, check 1-Wire operation status
#define I2C_CMD_OW_GET_DATA 0x92  // I2C slave transmit, get data from 1-Wire data buffer
#define I2C_CMD_OW_GET 0x93		  // I2C slave transmit, get 1-Wire operation status and data from 1-Wire data buffer
#define I2C_CMD_GET_WKUP_SRC 0x94 // I2C slave transmit, get MCU wake-up source
#define I2C_CMD_GET_KEY_RST 0x95  // I2C slave transmit, get key reset option
#define I2C_CMD_GPS_RMC 0x96	  // I2C slave transmit, get GPS RMC message
#define I2C_CMD_GPS_RESP 0x97	  // I2C slave transmit, get GPS response for PMTK packet
#define I2C_CMD_RTC_WKUP_GET 0x98 // I2C slave transmit, get current RTC wakeup setting

#define I2C_DATA_SIZE 128 // I2C data size 128 bytes
#define I2C_PKT_SIZE sizeof(I2C_PKT)
#define I2C_PKT_HEADER_SIZE (I2C_PKT_SIZE - I2C_DATA_SIZE)

//============================================================================//
// Typedef
//============================================================================//
typedef struct I2C_PKT // slave receiver (master to slave)
{
	u8 u8Command;
	u8 u8DataNum;
	u8 u8Param1;
	u8 u8Param2;
	u8 u8Data[I2C_DATA_SIZE];
} I2C_PKT;

//============================================================================//
// Extern
//============================================================================//
extern bit bTWI0ADFlag;
extern xdata u8 u8TWI0DevAddr;
extern xdata u8 u8TWI0RegAddr;
extern xdata u8 u8TWI1RegAddr;
extern xdata u8 u8TWI0TxInx;
extern xdata u8 u8TWI0RxInx;
extern xdata u8 u8TWI0OvTime;
extern xdata u8 u8TWI1OvTime;
extern xdata u8 u8TWI0BlockRead;  // master read
extern xdata u8 u8TWI0BlockWrite; // master write
extern bit bTWI0RxFlag;			  // TWI0 RX flag
extern xdata u8 TWI0TxBuf[I2C_PKT_SIZE];
extern xdata u8 TWI0RxBuf[I2C_PKT_SIZE];
extern xdata u8 u8TWI0UartBuf[I2C_PKT_SIZE]; // I2C to Uart TX

//============================================================================//
// Function
//============================================================================//
void TWI0_Init(bit bIntEn);
void TWI0_DeInit(void);
void TWI0_IntConfig(void);
#if ENABLE_TWI1
void TWI1_Init(bit bIntEn);
void TWI1_DeInit(void);
void TWI1_IntConfig(void);
#endif

#endif // _I2C_DRIVER_H