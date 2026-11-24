#ifndef _LCD_KEY_H_
#define _LCD_KEY_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//

#include <stdint.h>

#include "DataType.h"


//============================================================================//
// User Defines
//============================================================================//

#define TIMER_LCD_PROC 300

#define LCDCSLow		           	     HW_PBClr(LCD_CS)
#define LCDCSHigh			               HW_PBSet(LCD_CS)
#define LCDRSTLow			               HW_PBClr(LCD_RST)
#define LCDRSTHigh			             HW_PBSet(LCD_RST)
#define LCDRSLow			               HW_PBClr(LCD_A0)
#define LCDRSHigh			               HW_PBSet(LCD_A0)
#define LCDSCLLow			               HW_PBClr(LCD_SCL)
#define LCDSCLHigh			             HW_PBSet(LCD_SCL)
#define LCDSDALow			               HW_PBClr(LCD_SIO)
#define LCDSDAHigh			             HW_PBSet(LCD_SIO)
#define BackLightEnable 	           HW_PBClr(LCD_BL)
#define BackLightDisable 	           HW_PBSet(LCD_BL)

#define KeyCancelEnable		           HW_PBGetState(KEY1) == 0
#define KeyDownEnable		             HW_PBGetState(KEY2) == 0
#define KeyUpEnable			             HW_PBGetState(KEY3) == 0
#define KeyOKEnable			             HW_PBGetState(KEY4) == 0
#define KeyPrintEnable		           HW_PBGetState(KEY5) == 0

#define LOW    0
#define HIGH   1

#define WordIC_DUMMY_BYTE            0xFF 


//============================================================================//
// User Externs
//============================================================================//

extern bool bLCD_Running;


//============================================================================//
// User Functions
//============================================================================//

void LCD_Init(void);
void LCD_DisplayASCII_String(uint8_t u8x, uint8_t u8y, uint8_t *pu8text);
void LCD_DisplayMsg(char *cMsg, uint8_t u8Row, uint8_t u8Col);
void LCD_AreaClear(uint8_t u8StartRow, uint8_t u8StartCol, uint8_t u8Rows, uint8_t u8Cols);
void LCD_ScreenClear(void);


//============================================================================//
#endif //_LCD_KEY_H_

