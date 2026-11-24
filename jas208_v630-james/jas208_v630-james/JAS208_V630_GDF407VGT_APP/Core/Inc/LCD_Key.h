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

#define TIMER_LCD_PROC               100

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

#define SPI1_CS_LOW			             HW_PBClr(SPI1_CS)
#define SPI1_CS_HIGH		             HW_PBSet(SPI1_CS)

#define LOW                          0
#define HIGH                         1

#define WORD_IC_DUMMY_BYTE           0xFF
//#define ALL_USE_FONT_16X16           1U


//============================================================================//
// User Externs
//============================================================================//
extern uint32_t u32GetTick2;
extern uint8_t u8ShowMenuItem;
extern uint8_t u8LastPageNum;
extern bool bShowErrorFlag;
extern bool bLCD_Running;
extern bool bLCD_Stop;
extern bool bLCD_TurnOn;
extern bool bLCD_TurnOff;
extern bool bFirstGPSFixFlag;

//============================================================================//
// User Functions
//============================================================================//
void LCD_Init(void);
void LCD_Stop(void);
void LCD_SwitchHandle(void);
void LCD_Key_Process(void);
bool LCD_GetShowErrorFlagStatus(void);
void LCD_SetShowErrorFlagStatus(bool bSet);
void LCD_SetShowAutoTestPage(uint8_t u8Page);
void LCD_DisplaySubMenu(uint8_t u8Item);
void LCD_ScreenClear(void);
void LCD_DisplayASCII_AllBlock(uint8_t u8x, uint8_t u8y);
void LCD_DisplayASCIIS_Squre16(uint8_t u8x, uint8_t u8y, uint8_t u8Data); // [Nancy add]
void LCD_DisplayASCII_16x16_String(uint8_t u8x, uint8_t u8y, uint8_t *pu8text); // [Nancy add]
void LCD_DisplayASCII_16x16_String_Line(uint8_t u8x, uint8_t u8y, uint8_t *pu8Text, uint8_t u8LineX); // [Nancy add]

//============================================================================//
#endif //_LCD_KEY_H_

