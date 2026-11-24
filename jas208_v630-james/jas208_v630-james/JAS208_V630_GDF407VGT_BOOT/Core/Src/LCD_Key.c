//============================================================================//
// User Includes
//============================================================================//

#include "LCD_Key.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "CommFunc.h"
#include "Default.h"
#include "MX25L_Driver.h"
#include "System.h"


//============================================================================//
// User Variables
//============================================================================//

const uint8_t u8InputASCII[] = 
{
	0x30, 0x31, 0x32, 0x33, 0x34, 
	0x35, 0x36, 0x37, 0x38, 0x39, 
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46
};

uint8_t u8DisplayBuffer[32];

uint8_t u8InitialTab[14] = 
{
	0xA2, // Bias set: 1/9
	0xA0, // ADC select: Normal, 0->132 (A1 mirror) 
	0xA6, // Normal/reverse  display
	0xC8, // Common output mode select: Reverse行掃瞄方向:SHL=1(COM64->COM1) (C0 mirror) 
	0xF8, // Booster ratio set
	0x00, // Booster ratio: 4x
	0x2F, // Power control set
	//0x26, // Set (Rb/Ra) Green monitor
	0x24, // Set (Rb/Ra) Org monitor
	0x81, // Vo output voltage set
	40, // Voltage volume = 9.0v ;24
	0x60, // Set start line  60==COM32 40==COM0 COM從0開始算 COM1算的話60==COM33
	0xAF, // Display on
};

bool bLCD_Running = FALSE;


//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Send a LCD command.
// @param  u8Command: LCD command ID.
// @retval None.
//============================================================================//
void LCD_InsTrans(uint8_t u8Command)
{ 
	LCDCSLow;
	
	LCDRSLow;
	
	for(int i = 0; i < 8; i++) 
	{
		if((u8Command & 0x80) == 0x80) 
			LCDSDAHigh;
		else 
			LCDSDALow;
		
		LCDSCLLow;
			
		Delay_uS(2);
		
		LCDSCLHigh;
			
		Delay_uS(2);
		
		LCDSCLLow;
			
		u8Command <<= 1;
	}

	LCDCSHigh;
}

//============================================================================//
// @brief  Send one data byte to LCD.
// @param  u8Command: Data byte to send.
// @retval None.
//============================================================================//
void LCD_DataTrans(uint8_t u8Command)
{ 
	LCDCSLow;
	
	LCDRSHigh;
	
  for(int i = 0; i < 8; i++) 
	{
    if((u8Command & 0x80) == 0x80) 
			LCDSDAHigh;
		else 
			LCDSDALow;

		LCDSCLLow;
		
    Delay_uS(2);
		
		LCDSCLHigh;
	    
		Delay_uS(2);
		
		LCDSCLLow;
        
		u8Command <<= 1;
  }
	
	LCDCSHigh;
}

//============================================================================//
// @brief  Set the working column in LCD.
// @param  u8Column: The column number.
// @retval None.
//============================================================================//
void LCD_ColumnSet(uint8_t u8Column)
{
	LCD_InsTrans(0x10 | (u8Column >> 4));
	LCD_InsTrans(0x00 | (u8Column & 0x0F));	 
}

//============================================================================//
// @brief  Send the working page in LCD.
// @param  u8Page: The page number.
// @retval None.
//============================================================================//
void LCD_PageSet(uint8_t u8Page)
{
	LCD_InsTrans(0xB0 + u8Page);
}

//============================================================================//
// @brief  Clear an area in the LCD.
// @param  u8StartRow: Start row to clear.
//         u8StartCol: Start column to clear.
//         u8Rows: Number of rows to clear.
//         u8Cols: Number of column to clear.
// @retval None.
//============================================================================//
void LCD_AreaClear(uint8_t u8StartRow, uint8_t u8StartCol, uint8_t u8Rows, uint8_t u8Cols)
{
	// Row loop
 	for(int i = u8StartRow; i < (u8StartRow + u8Rows); i++) 
	{ 
		LCD_PageSet(i);
		
	  LCD_ColumnSet(u8StartCol);
		
		// Column loop 
		for(int j = 0; j < u8Cols; j++) 
			LCD_DataTrans(0x00);
	}
}

//============================================================================//
// @brief  Clear the LCD screen.
// @param  None.
// @retval None.
//============================================================================//
void LCD_ScreenClear(void)
{
	if(bLCD_Running == FALSE)
		return;
	
	// Page loop
 	for(int i = 0; i < 8; i++) 
	{ 
		LCD_PageSet(i);
		
	  LCD_ColumnSet(0);
		
		// Column loop 0->132
		for(int j = 0; j < 132; j++) 
			LCD_DataTrans(0x00);
	}
}

//============================================================================//
// @brief  Display a ASCII character.
// @param  u8x: x position.
//         u8y: y position.
//         u8Data: Data byte to display.
// @retval None.
//============================================================================//
void LCD_DisplayASCII(uint8_t u8x, uint8_t u8y, uint8_t u8Data)
{
	uint32_t u32Addr;
	 
	
	u32Addr = (u8Data - 0x20) * 16;
	
	for(int i = 0; i < 16; i++) 
		u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

	LCD_PageSet(u8y);
	
	LCD_ColumnSet((u8x * 8) - 1);
	
	LCD_DataTrans(0x00);
	
	for(int j = 0; j < 16; j += 2) 
		LCD_DataTrans(u8DisplayBuffer[j]);
	
	LCD_PageSet(u8y + 1);
	
	LCD_ColumnSet((u8x * 8) - 1);	
	
	LCD_DataTrans(0x00);
	
	for(int j = 1; j < 16; j += 2) 
		LCD_DataTrans(u8DisplayBuffer[j]);
}

//============================================================================//
// @brief  Display an ASCII string.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_String(uint8_t u8x, uint8_t u8y, uint8_t *pu8text)
{
	uint8_t u8t;
	uint32_t u32Addr;
	
	
	u8t = 0;
	
	while(pu8text[u8t] > 0x00) 
	{
		u32Addr = (pu8text[u8t] - 0x20) * 16;
		
		for(int i = 0; i < 16; i++)
			u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

		LCD_PageSet(u8y);
		
		LCD_ColumnSet((u8x * 8) - 1);	
		
		LCD_DataTrans(0x00);
		
		for(int j = 0; j < 16; j += 2) 
			LCD_DataTrans(u8DisplayBuffer[j]);
		
		LCD_PageSet(u8y + 1);
		
		LCD_ColumnSet((u8x * 8) - 1);	
		
		LCD_DataTrans(0x00);
		
		for(int j = 1; j < 16; j += 2)
			LCD_DataTrans(u8DisplayBuffer[j]);
		
		u8t++;
		u8x++;
	}
}

//============================================================================//
// @brief  Display an ASCII string on a line.
// @param  u8x: x start position.
//         u8y: y start position.
//         u8LineX: Line number
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_String_Line(uint8_t u8x, uint8_t u8y, uint8_t *text, uint8_t u8LineX)
{
	uint8_t u8t;
	uint32_t u32Addr;
	
	
	u8t = 0;
	
	while(text[u8t] > 0x00) 
	{
		u32Addr = (text[u8t] - 0x20) * 16;
		
		for(int i = 0; i < 16; i++) 
			u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

		if(u8LineX == u8t) 
		{
			u32Addr = ('_' - 0x20) * 16;
			
			for(int i = 0; i < 16; i++) 
				u8DisplayBuffer[i] |= ASCII_WORD[u32Addr + i];
		}
		
		LCD_PageSet(u8y);
		
		LCD_ColumnSet((u8x * 8) - 1);
		
		LCD_DataTrans(0x00);
		
		for(int j = 0; j < 16; j += 2) 
			LCD_DataTrans(u8DisplayBuffer[j]);
		
		LCD_PageSet(u8y + 1);
		
		LCD_ColumnSet((u8x * 8) - 1);
		
		LCD_DataTrans(0x00);
		
		for(int j = 1; j < 16; j += 2) 
			LCD_DataTrans(u8DisplayBuffer[j]);
		
		u8t++;
		u8x++;
	}
}

//============================================================================//
// @brief  Display a character in font 16.
// @param  u8x: x start position.
//         u8y: y start position.
//         text: Character to display.
// @retval None.
//============================================================================//
void LCD_DisplayFont16(uint8_t u8x, uint8_t u8y, uint8_t *text)
{
	uint8_t u8t;
	uint8_t u8WordBuffer[32];
	uint16_t u16Word;
	uint32_t u32Addr;
	
	
	const uint8_t u8Bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	memset(u8DisplayBuffer, 0x00, sizeof(u8WordBuffer));
	memset(u8DisplayBuffer, 0x00, sizeof(u8DisplayBuffer));
	
	u16Word = 0x0000;
	u32Addr = 0x00;
	u16Word = u16Word | text[0];
	u16Word = u16Word << 8;
	u16Word = (u16Word & 0xFF00) | text[1];
	u32Addr = u16Word - 0xA000;
	u32Addr = u32Addr * 32;
	
	//SPI1_Read(u32Addr, 32, u8WordBuffer);
	MX25L_MultiBytesRead(u32Addr, u8WordBuffer, 32);
	
  u8t = 0;
	
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 1; j <= 8; j++) 
		{
			if((u8WordBuffer[32 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[u8t] |= 0x01;
			else 
				u8DisplayBuffer[u8t] |= 0x00;
		
			if((u8WordBuffer[16 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[u8t + 1] |= 0x01;
			else 
				u8DisplayBuffer[u8t + 1] |= 0x00;
			
			if((u8WordBuffer[33 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[16 + u8t] |= 0x01;
			else 
				u8DisplayBuffer[16 + u8t] |= 0x00;
			
			if((u8WordBuffer[17 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[17 + u8t] |= 0x01;
			else 
				u8DisplayBuffer[17 + u8t] |= 0x00;
			
			if (j != 8) 
			{
				u8DisplayBuffer[u8t] <<= 1;
				u8DisplayBuffer[u8t + 1] <<= 1;
				u8DisplayBuffer[16 + u8t] <<= 1;
				u8DisplayBuffer[17 + u8t] <<= 1;
			}
		}
		
		u8t += 2;
	}

	LCD_PageSet(u8y + 1);
	
	LCD_ColumnSet((u8x * 16) - 1);	
	
	LCD_DataTrans(0x00);
	
	for(int i = 0; i < 32; i += 2) 
		LCD_DataTrans(u8DisplayBuffer[i]);
	
	LCD_PageSet(u8y);
	
	LCD_ColumnSet((u8x * 16) - 1);	
	
	LCD_DataTrans(0x00);
	
	for(int i = 1; i < 32; i += 2) 
		LCD_DataTrans(u8DisplayBuffer[i]);
}

//============================================================================//
// @brief  Display a string in font 16.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayFont16_String(uint8_t u8x, uint8_t u8y, uint8_t *u8Text)
{
	uint8_t u8t, u8k;
	uint8_t u8WordBuffer[32];
	uint32_t u32Addr;
	uint16_t u16Word;
	
	
	const uint8_t u8Bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	u8t = 0;
	
	while(u8Text[u8t] > 0x00)
	{
		memset(u8DisplayBuffer, 0x00, sizeof(u8WordBuffer));
		memset(u8DisplayBuffer, 0x00, sizeof(u8DisplayBuffer));
		
		u16Word = 0x0000;
		u32Addr = 0x00;
		u16Word = u16Word | u8Text[u8t];
		u16Word = u16Word << 8;
		u16Word = (u16Word & 0xFF00) | u8Text[u8t + 1];
		u32Addr = u16Word - 0xA000;
		u32Addr = u32Addr * 32;
		
		//SPI1_Read(u32Addr, 32, u8WordBuffer);
		MX25L_MultiBytesRead(u32Addr, u8WordBuffer, 32);
		
		u8k = 0;
		
		for(int i = 0; i < 8; i++) 
		{
			for(int j = 1; j <= 8; j++) 
			{
				if((u8WordBuffer[32 - (j * 2)] & u8Bit[i]) > 0)
					u8DisplayBuffer[u8k] |= 0x01;
				else 
					u8DisplayBuffer[u8k] |= 0x00;
				
				if((u8WordBuffer[16 - (j * 2)] & u8Bit[i]) > 0) 
					u8DisplayBuffer[u8k + 1] |= 0x01;
				else 
					u8DisplayBuffer[u8k + 1] |= 0x00;
				
				if((u8WordBuffer[33 - (j * 2)] & u8Bit[i]) > 0) 
					u8DisplayBuffer[16 + u8k] |= 0x01;
				else 
					u8DisplayBuffer[16 + u8k] |= 0x00;
				
				if((u8WordBuffer[17 - (j * 2)] & u8Bit[i]) > 0) 
					u8DisplayBuffer[17 + u8k] |= 0x01;
				else 
					u8DisplayBuffer[17 + u8k] |= 0x00;
				
				if(j != 8) 
				{
					u8DisplayBuffer[u8k] <<= 1;
					u8DisplayBuffer[u8k + 1] <<= 1;
					u8DisplayBuffer[16 + u8k] <<= 1;
					u8DisplayBuffer[17 + u8k] <<= 1;
				}
			}
				
			u8k += 2;
		}

		LCD_PageSet(u8y + 1);
		
		LCD_ColumnSet((u8x * 16) - 1);	
		
		LCD_DataTrans(0x00);
		
		for(int i = 0; i < 32; i += 2) 
			LCD_DataTrans(u8DisplayBuffer[i]);
		
		LCD_PageSet(u8y);
		
		LCD_ColumnSet((u8x * 16)-1);	
		
		LCD_DataTrans(0x00);
		
		for(int i = 1; i < 32; i += 2) 
			LCD_DataTrans(u8DisplayBuffer[i]);
		
		u8x++;
		u8t += 2;
	}
}

//============================================================================//
// @brief  Create a part of a TWord .
// @param  u8x: x start position.
//         u8y: y start position.
//         u8Num: Part of the TWord to create, from 1 to 4.
// @retval None.
//============================================================================//
void LCD_DisplayTWord(uint8_t u8x, uint8_t u8y, uint8_t u8Num)
{
	const uint8_t u8Bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	
   
	memset(u8DisplayBuffer, 0x00, sizeof(u8DisplayBuffer));
	
	uint8_t u8t = 0;
	
	for(int i = 0; i < 8; i++) 
	{
		for(int j = 1; j <= 8; j++) 
		{
			if((TWord[u8Num][32 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[u8t] |= 0x01;
			else 
				u8DisplayBuffer[u8t] |= 0x00;
			
			if((TWord[u8Num][16 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[u8t + 1] |= 0x01;
			else
				u8DisplayBuffer[u8t + 1] |= 0x00;
			
			if((TWord[u8Num][33 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[16 + u8t] |= 0x01;
			else
				u8DisplayBuffer[16 + u8t] |= 0x00;
			
			if((TWord[u8Num][17 - (j * 2)] & u8Bit[i]) > 0) 
				u8DisplayBuffer[17 + u8t] |= 0x01;
			else 
				u8DisplayBuffer[17 + u8t] |= 0x00;
			
			if (j != 8) 
			{
				u8DisplayBuffer[u8t] <<= 1;
				u8DisplayBuffer[u8t + 1] <<= 1;
				u8DisplayBuffer[16 + u8t] <<= 1;
				u8DisplayBuffer[17 + u8t] <<= 1;
			}
		}
		
		u8t += 2;
	}

	LCD_PageSet(u8y + 1);
	
	LCD_ColumnSet((u8x * 16) - 1);	
	
	LCD_DataTrans(0x00);
	
	for(int i = 0; i < 32; i += 2) 
		LCD_DataTrans(u8DisplayBuffer[i]);
	
	LCD_PageSet(u8y);
	
	LCD_ColumnSet((u8x * 16) - 1);	
	
	LCD_DataTrans(0x00);
	
	for(int i = 1; i < 32; i += 2) 
		LCD_DataTrans(u8DisplayBuffer[i]);
}

//============================================================================//
// @brief  Display a TWord .
// @param  u8x: x start position.
//         u8y: y start position.
// @retval None.
//============================================================================//
void LCD_ShowTWord(uint8_t u8x, uint8_t u8y)
{
	LCD_DisplayTWord(u8x, u8y, 0);
	LCD_DisplayTWord(u8x + 1, u8y, 1);
	LCD_DisplayTWord(u8x, u8y + 2, 2);
	LCD_DisplayTWord(u8x + 1, u8y + 2, 3);
}

//============================================================================//
// @brief  Display the cursor.
// @param  u8Idx: Line to display the cursor.
// @retval None.
//============================================================================//
void LCD_ShowCursor(uint8_t u8Idx)
{
	LCD_DisplayASCII_String(3, u8Idx * 2, (uint8_t *)">");
}

//============================================================================//
// @brief  Clear the cursor.
// @param  u8Idx: Current line of the cursor.
// @retval None.
//============================================================================//
void ClearCursor(uint8_t u8Idx)
{
	LCD_DisplayASCII_String(3, u8Idx * 2, (uint8_t *)" ");
}

//============================================================================//
// @brief  Initialize LCD.
// @param  None.
// @retval None.
//============================================================================//
void LCD_ScreenInitial(void)
{
	for(int i = 0; i < 14; i++) 
		LCD_InsTrans(u8InitialTab[i]);
}

//============================================================================//
// @brief  Initialze the LCD.
// @param  None.
// @retval None.
//============================================================================//
void LCD_Init(void) 
{
	if(bLCD_Running == TRUE)
		return;
	
	BackLightEnable;
	
	LCDRSTLow; 
	
	HAL_Delay(10);
	
	LCDRSTHigh;
	
	LCD_ScreenInitial();
	
	LCD_ScreenClear();
	
	bLCD_Running = TRUE;
}

//============================================================================//
// @brief  Display a message on the LCD screen.
// @param  cMsg: Pointer to the message buffer.
//         u8Row: Row ID on which start to display from.
//         u8Column: Column ID on which start to display from. 
// @retval None.
//============================================================================//
void LCD_DisplayMsg(char *cMsg, uint8_t u8Row, uint8_t u8Col)
{	
	if(bLCD_Running == FALSE)
		return;
	
  LCD_DisplayASCII_String(u8Col, u8Row, (uint8_t *)cMsg);
}
