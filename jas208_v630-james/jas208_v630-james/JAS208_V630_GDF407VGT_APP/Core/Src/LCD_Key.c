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
#include "EXTIO.h"
#include "GSM_QtEC21.h"
#include "MX25L_Driver.h"
#include "PRN_Driver.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"
#include "USB_Module.h"

//============================================================================//
// User Variables
//============================================================================//
/*
const uint8_t u8InputASCII[] =
{
    0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46
};
*/
const uint8_t u8PassWordSet[] =
    {
        //	5, 4, 3, 5
        5, 3, 3, 0};

uint8_t u8DisplayBuffer[32];

uint8_t u8InitialTab[14] =
    {
        0xA2, // Bias set: 1/9
        0xA0, // ADC select: Normal, 0->132 (A1 mirror)
        0xA6, // Normal/reverse  display
        0xC8, // Common output mode select: Reverse�汽�ˤ�V:SHL=1(COM64->COM1) (C0 mirror)
        0xF8, // Booster ratio set
        0x00, // Booster ratio: 4x
        0x2F, // Power control set
        // 0x26, // Set (Rb/Ra) Green monitor
        0x24, // Set (Rb/Ra) Org monitor
        0x81, // Vo output voltage set
        40,   // Voltage volume = 9.0v ;24
        0x60, // Set start line  60==COM32 40==COM0 COM�q0�}�l�� COM1�⪺��60==COM33
        0xAF, // Display on
};

bool bReFlashMenuFlag = FALSE;
bool bShowErrorFlag = FALSE;
bool bMoveCursor = FALSE;
bool bClearPageFlag = TRUE;
bool bChangPageFlag = FALSE;

// uint8_t u8CurrKey = 0;
uint8_t u8ShowMenuItem;
uint8_t u8CursorIndex = 0;
uint8_t u8InputIdx = 0;
uint8_t u8InputBCDTime[7];
uint8_t u8InputBCDTime2[7];
uint8_t u8InputBCDBuf[6];
uint8_t u8InputBCDBuf2[6];
// uint8_t u8NumIdx[8];
uint8_t u8DLHour;
uint8_t u8SetDriverStatus;
uint8_t u8MainPageIdx = 0;
uint8_t u8InputIDIndex = 0;
uint8_t u8ChangPageNum = 0;
uint8_t u8LastPageNum = 0;
uint8_t u8CarTypeIdx = 0;
uint8_t u8PassWord[4] = {0};
uint8_t u8InputIdx2 = 0;
uint8_t u8PrePwPage = 0;

uint16_t u16KGain = 0;

char szInputStrBuf[20];  // __attribute__ ((section("ccmram_var")));
char szInputStrBuf2[20]; // __attribute__ ((section("ccmram_var")));
char szInputStrBuf3[20]; // __attribute__ ((section("ccmram_var")));
char u8InputGainBuf[3];

uint32_t u32GetTick = 0;
uint32_t u32WarningTick = 0;
uint32_t u32GetTick2 = 0;

bool bLCD_Running = FALSE;
bool bLCD_Stop = FALSE;
bool bLCD_TurnOn = FALSE;
bool bLCD_TurnOff = FALSE;

bool bFirstGPSFixFlag = FALSE;

SelectLangTypeDef u8SelectLang = LANG_CHN;

time_t ttSBuf, ttEBuf, ttMBuf;

char cDLFileName[50];

bool bDLRes;

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

    for (int i = 0; i < 8; i++)
    {
        if ((u8Command & 0x80) == 0x80)
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
void LCD_DataTrans(uint8_t u8Data)
{
    LCDCSLow;

    LCDRSHigh;

    for (int i = 0; i < 8; i++)
    {
        if ((u8Data & 0x80) == 0x80)
            LCDSDAHigh;
        else
            LCDSDALow;

        LCDSCLLow;

        Delay_uS(2);

        LCDSCLHigh;

        Delay_uS(2);

        LCDSCLLow;

        u8Data <<= 1;
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
    // Page loop
    for (int i = u8StartRow; i < (u8StartRow + u8Rows); i++)
    {
        LCD_PageSet(i);

        LCD_ColumnSet(u8StartCol);

        // Column loop
        for (int j = 0; j < u8Cols; j++)
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
    // Page loop
    for (int i = 0; i < 8; i++)
    {
        LCD_PageSet(i);

        LCD_ColumnSet(0);

        // Column loop 0->132
        for (int j = 0; j < 132; j++)
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

    for (int i = 0; i < 16; i++)
        u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 0; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);

    LCD_PageSet(u8y + 1);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 1; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);
}

//============================================================================//
// [Nancy add]
// @brief  Display a ASCII character.(shape:16*16)
// @param  u8x: x position.
//         u8y: y position.
//         u8Data: Data byte to display.
// @retval None.
//============================================================================//
void LCD_DisplayASCIIS_Squre16(uint8_t u8x, uint8_t u8y, uint8_t u8Data)
{
    uint32_t u32Addr;

    u32Addr = (u8Data - 0x20) * 32; // [Nancy command] 0x20 = 32: minus unuse sign before 20 (check ascii code)
                                    // 					*32: each row have 32 member (check tabke: ASCII_WORD)

    for (int i = 0; i < 32; i++)
        u8DisplayBuffer[i] = ASCII_WORD_16x16[u32Addr + i];

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 0; j < 16; j++)
        LCD_DataTrans(u8DisplayBuffer[j]);

    LCD_PageSet(u8y + 1);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 16; j < 32; j++)
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

    while (pu8text[u8t] > 0x00)
    {
        u32Addr = (pu8text[u8t] - 0x20) * 16;

        for (int i = 0; i < 16; i++)
            u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

        LCD_PageSet(u8y);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 0; j < 16; j += 2)
            LCD_DataTrans(u8DisplayBuffer[j]);

        LCD_PageSet(u8y + 1);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 1; j < 16; j += 2)
            LCD_DataTrans(u8DisplayBuffer[j]);

        u8t++;
        u8x++;
    }
}

//============================================================================//
// [Nancy add]
// @brief  Display an ASCII string.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_16x16_String(uint8_t u8x, uint8_t u8y, uint8_t *pu8text)
{
    uint8_t u8t;
    uint32_t u32Addr;

    u8t = 0;

    while (pu8text[u8t] > 0x00)
    {
        u32Addr = (pu8text[u8t] - 0x20) * 32;

        for (int i = 0; i < 32; i++)
            u8DisplayBuffer[i] = ASCII_WORD_16x16[u32Addr + i];

        LCD_PageSet(u8y);

        // LCD_ColumnSet((u8x * 16) - 1);
        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 0; j < 16; j++)
            LCD_DataTrans(u8DisplayBuffer[j]);

        LCD_PageSet(u8y + 1);

        // LCD_ColumnSet((u8x * 16) - 1);
        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 16; j < 32; j++)
            LCD_DataTrans(u8DisplayBuffer[j]);

        u8t++;
        u8x += 2;
    }
}

//============================================================================//
// [Nancy add]
// @brief  Display an ASCII string on a line.
// @param  u8x: x start position.
//         u8y: y start position.
//         u8LineX: Line number
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_16x16_String_Line(uint8_t u8x, uint8_t u8y, uint8_t *pu8Text, uint8_t u8LineX)
{
    uint8_t u8t;
    uint32_t u32Addr;

    u8t = 0;

    while (pu8Text[u8t] > 0x00)
    {
        u32Addr = (pu8Text[u8t] - 0x20) * 32;

        for (int i = 0; i < 32; i++)
            u8DisplayBuffer[i] = ASCII_WORD_16x16[u32Addr + i];

        if (u8LineX == u8t)
        {
            u32Addr = ('_' - 0x20) * 32;

            for (int i = 0; i < 32; i++)
                u8DisplayBuffer[i] |= ASCII_WORD_16x16[u32Addr + i];
        }

        LCD_PageSet(u8y);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 0; j < 16; j++)
            LCD_DataTrans(u8DisplayBuffer[j]);

        LCD_PageSet(u8y + 1);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 16; j < 32; j++)
            LCD_DataTrans(u8DisplayBuffer[j]);

        u8t++;
        u8x += 2;
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
void LCD_DisplayASCII_String_Line(uint8_t u8x, uint8_t u8y, uint8_t *pu8Text, uint8_t u8LineX)
{
    uint8_t u8t;
    uint32_t u32Addr;

    u8t = 0;

    while (pu8Text[u8t] > 0x00)
    {
        u32Addr = (pu8Text[u8t] - 0x20) * 16;

        for (int i = 0; i < 16; i++)
            u8DisplayBuffer[i] = ASCII_WORD[u32Addr + i];

        if (u8LineX == u8t)
        {
            u32Addr = ('_' - 0x20) * 16;

            for (int i = 0; i < 16; i++)
                u8DisplayBuffer[i] |= ASCII_WORD[u32Addr + i];
        }

        LCD_PageSet(u8y);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 0; j < 16; j += 2)
            LCD_DataTrans(u8DisplayBuffer[j]);

        LCD_PageSet(u8y + 1);

        LCD_ColumnSet((u8x * 8) - 1);

        LCD_DataTrans(0x00);

        for (int j = 1; j < 16; j += 2)
            LCD_DataTrans(u8DisplayBuffer[j]);

        u8t++;
        u8x++;
    }
}

//============================================================================//
// @brief  Display all block ASCII string.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_AllBlock(uint8_t u8x, uint8_t u8y)
{
    for (int i = 0; i < 16; i++)
        u8DisplayBuffer[i] = 0xFF;

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 0; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);

    LCD_PageSet(u8y + 1);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 1; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);
}

//============================================================================//
// @brief  Display all block ASCII string.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayASCII_SPWord(uint8_t u8x, uint8_t u8y)
{
    for (int i = 0; i < 16; i++)
        u8DisplayBuffer[i] = u8SP_WORD[i];

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 0; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);

    LCD_PageSet(u8y + 1);

    LCD_ColumnSet((u8x * 8) - 1);

    LCD_DataTrans(0x00);

    for (int j = 1; j < 16; j += 2)
        LCD_DataTrans(u8DisplayBuffer[j]);
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

    // SPI1_Read(u32Addr, 32, u8WordBuffer);
    MX25L_MultiBytesRead(EXFLASH_CHARACTERS_LIB_START_ADDR + u32Addr, u8WordBuffer, 32);

    u8t = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            if ((u8WordBuffer[32 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[u8t] |= 0x01;
            else
                u8DisplayBuffer[u8t] |= 0x00;

            if ((u8WordBuffer[16 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[u8t + 1] |= 0x01;
            else
                u8DisplayBuffer[u8t + 1] |= 0x00;

            if ((u8WordBuffer[33 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[16 + u8t] |= 0x01;
            else
                u8DisplayBuffer[16 + u8t] |= 0x00;

            if ((u8WordBuffer[17 - (j * 2)] & u8Bit[i]) > 0)
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

    for (int i = 0; i < 32; i += 2)
        LCD_DataTrans(u8DisplayBuffer[i]);

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 16) - 1);

    LCD_DataTrans(0x00);

    for (int i = 1; i < 32; i += 2)
        LCD_DataTrans(u8DisplayBuffer[i]);
}

//============================================================================//
// @brief  Display a string in font 16.
// @param  u8x: x start position.
//         u8y: y start position.
//         pu8Text: Pointer to an array storing the string.
// @retval None.
//============================================================================//
void LCD_DisplayFont16_String(uint8_t u8x, uint8_t u8y, uint8_t *pu8Text)
{
    uint8_t u8t, u8k;
    uint8_t u8WordBuffer[32];
    uint32_t u32Addr;
    uint16_t u16Word;

    const uint8_t u8Bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    u8t = 0;

    while (pu8Text[u8t] > 0x00)
    {
        memset(u8DisplayBuffer, 0x00, sizeof(u8WordBuffer));
        memset(u8DisplayBuffer, 0x00, sizeof(u8DisplayBuffer));

        u16Word = 0x0000;
        u32Addr = 0x00;
        u16Word = u16Word | pu8Text[u8t];
        u16Word = u16Word << 8;
        u16Word = (u16Word & 0xFF00) | pu8Text[u8t + 1];
        u32Addr = u16Word - 0xA000;
        u32Addr = u32Addr * 32;

        // SPI1_Read(u32Addr, 32, u8WordBuffer);
        MX25L_MultiBytesRead(EXFLASH_CHARACTERS_LIB_START_ADDR + u32Addr, u8WordBuffer, 32);

        u8k = 0;

        for (int i = 0; i < 8; i++)
        {
            for (int j = 1; j <= 8; j++)
            {
                if ((u8WordBuffer[32 - (j * 2)] & u8Bit[i]) > 0)
                    u8DisplayBuffer[u8k] |= 0x01;
                else
                    u8DisplayBuffer[u8k] |= 0x00;

                if ((u8WordBuffer[16 - (j * 2)] & u8Bit[i]) > 0)
                    u8DisplayBuffer[u8k + 1] |= 0x01;
                else
                    u8DisplayBuffer[u8k + 1] |= 0x00;

                if ((u8WordBuffer[33 - (j * 2)] & u8Bit[i]) > 0)
                    u8DisplayBuffer[16 + u8k] |= 0x01;
                else
                    u8DisplayBuffer[16 + u8k] |= 0x00;

                if ((u8WordBuffer[17 - (j * 2)] & u8Bit[i]) > 0)
                    u8DisplayBuffer[17 + u8k] |= 0x01;
                else
                    u8DisplayBuffer[17 + u8k] |= 0x00;

                if (j != 8)
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

        for (int i = 0; i < 32; i += 2)
            LCD_DataTrans(u8DisplayBuffer[i]);

        LCD_PageSet(u8y);

        LCD_ColumnSet((u8x * 16) - 1);

        LCD_DataTrans(0x00);

        for (int i = 1; i < 32; i += 2)
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
void LCD_Displayu8TWord(uint8_t u8x, uint8_t u8y, uint8_t u8Num)
{
    const uint8_t u8Bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    memset(u8DisplayBuffer, 0x00, sizeof(u8DisplayBuffer));

    uint8_t u8t = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 1; j <= 8; j++)
        {
            if ((u8TWord[u8Num][32 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[u8t] |= 0x01;
            else
                u8DisplayBuffer[u8t] |= 0x00;

            if ((u8TWord[u8Num][16 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[u8t + 1] |= 0x01;
            else
                u8DisplayBuffer[u8t + 1] |= 0x00;

            if ((u8TWord[u8Num][33 - (j * 2)] & u8Bit[i]) > 0)
                u8DisplayBuffer[16 + u8t] |= 0x01;
            else
                u8DisplayBuffer[16 + u8t] |= 0x00;

            if ((u8TWord[u8Num][17 - (j * 2)] & u8Bit[i]) > 0)
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

    for (int i = 0; i < 32; i += 2)
        LCD_DataTrans(u8DisplayBuffer[i]);

    LCD_PageSet(u8y);

    LCD_ColumnSet((u8x * 16) - 1);

    LCD_DataTrans(0x00);

    for (int i = 1; i < 32; i += 2)
        LCD_DataTrans(u8DisplayBuffer[i]);
}

//============================================================================//
// @brief  Display a TWord .
// @param  u8x: x start position.
//         u8y: y start position.
// @retval None.
//============================================================================//
void LCD_Showu8TWord(uint8_t u8x, uint8_t u8y)
{
    LCD_Displayu8TWord(u8x, u8y, 0);
    LCD_Displayu8TWord(u8x + 1, u8y, 1);
    LCD_Displayu8TWord(u8x, u8y + 2, 2);
    LCD_Displayu8TWord(u8x + 1, u8y + 2, 3);
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
void LCD_ClearCursor(uint8_t u8Idx)
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
    for (int i = 0; i < 14; i++)
        LCD_InsTrans(u8InitialTab[i]);
}

//============================================================================//
// @brief  Display version.
// @param  None.
// @retval None.
//============================================================================//
void LCD_DisplayVersion(void)
{
    LCD_ScreenClear();
}

//============================================================================//
// @brief  Display the main menu.
// @param  None
// @retval None.
//============================================================================//
void LCD_DisplayMainMenu(void)
{
    char szStrBuf[20] = {'\0'};
    time_t ttTime;
    uint8_t TimeBCDBuf[7], u8Status;
    uint8_t u8AutoGainPa = 0;
    double dAutoGainBuf = 0;

    if (bClearPageFlag == TRUE)
    {
        LCD_ScreenClear();

        bClearPageFlag = FALSE;
    }

    if (u8MainPageIdx == 0)
    {
        ttTime = System_GetTime();

        // ttTime += g_Setting.i32TimeZoneOffset;

        SecToBCDTime((uint32_t)ttTime, TimeBCDBuf);

        sprintf(szStrBuf, "%02X%02X/%02X/%02X", TimeBCDBuf[0], TimeBCDBuf[1], TimeBCDBuf[2], TimeBCDBuf[3]);

        LCD_DisplayASCII_String(3, 0, (uint8_t *)szStrBuf);

        sprintf(szStrBuf, "%02X:%02X:%02X", TimeBCDBuf[4], TimeBCDBuf[5], TimeBCDBuf[6]);

        LCD_DisplayASCII_String(4, 2, (uint8_t *)szStrBuf);

        if (CUSTOMER_TYPE == CUSTOMER_SCANIA)
        {
            if (u8SelectLang == LANG_CHN)
                LCD_DisplayFont16_String(0, 4, (uint8_t *)"�t��");
            else if (u8SelectLang == LANG_ENG)
                LCD_DisplayASCII_String(0, 4, (uint8_t *)"Speed ");
        }
        else
            LCD_DisplayFont16_String(0, 4, (uint8_t *)"�t��");

        sprintf(szStrBuf, " %03d.%d  km/h", M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed),
                (M_2_100M(g_SysInfo.tSpeedInfo.u32RealSpeed) % 10));

        LCD_DisplayASCII_String(4, 4, (uint8_t *)szStrBuf);

        if (CUSTOMER_TYPE == CUSTOMER_SCANIA)
        {
            if (u8SelectLang == LANG_CHN)
                LCD_DisplayFont16_String(0, 6, (uint8_t *)"�r�p");
            else if (u8SelectLang == LANG_ENG)
                LCD_DisplayASCII_String(0, 6, (uint8_t *)"Drv ");
        }
        else
            LCD_DisplayFont16_String(0, 6, (uint8_t *)"�r�p");

        u8Status = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u8DrvStatus;

        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum);

        LCD_DisplayASCII_String(4, 6, (uint8_t *)szInputStrBuf);

        if (u8Status == 0)
            LCD_DisplayFont16_String(6, 6, (uint8_t *)"����");
        else if (u8Status == 1)
            LCD_DisplayFont16_String(6, 6, (uint8_t *)"��p");
        else if (u8Status == 2)
            LCD_DisplayFont16_String(6, 6, (uint8_t *)"�ݯZ");
        else if (u8Status == 3)
            LCD_DisplayFont16_String(6, 6, (uint8_t *)"��");

        if (g_Setting.tSpeedSet.u8Auto_Gain_Count < 128)
        {
            if (g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_CAN)
                dAutoGainBuf = g_SysInfo.tVehicleState.dDistance_CAN;
            else
                dAutoGainBuf = g_SysInfo.tVehicleState.dDistance_GPS;

            if (dAutoGainBuf > g_Setting.u16AG_Dist_Limit_Temp)
                dAutoGainBuf = g_Setting.u16AG_Dist_Limit_Temp;

            u8AutoGainPa = (((512 + 3000) * (g_Setting.tSpeedSet.u8Auto_Gain_Count) + 512 * ((g_Setting.u16AG_Dist_Limit_Temp > 1000) ? 1 : 0) + dAutoGainBuf) / ((512 + 3000) * 3)) * 100;

            if (u8AutoGainPa <= 100)
            {
                sprintf(szStrBuf, "%2d%%", u8AutoGainPa);
                LCD_DisplayASCII_String_Line(13, 2, (uint8_t *)szStrBuf, 0xFF);
            }
        }
        else
        {
            LCD_DisplayASCII_String(13, 2, "    ");
        }
    }
    else if (u8MainPageIdx == 1)
    {
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�D�r�p");

        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[0].u32DrvIdNum);

        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf);

        if (g_Setting.tDrvInfo[0].u8DrvStatus == 0)
            LCD_DisplayFont16_String(5, 2, (uint8_t *)"������");
        else if (g_Setting.tDrvInfo[0].u8DrvStatus == 1)
            LCD_DisplayFont16_String(5, 2, (uint8_t *)"��p��");
        else if (g_Setting.tDrvInfo[0].u8DrvStatus == 2)
            LCD_DisplayFont16_String(5, 2, (uint8_t *)"�ݯZ��");
        else if (g_Setting.tDrvInfo[0].u8DrvStatus == 3)
            LCD_DisplayFont16_String(5, 2, (uint8_t *)"�𮧤�");

        LCD_DisplayFont16_String(0, 4, (uint8_t *)"�@�P�r�p");

        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[1].u32DrvIdNum);

        LCD_DisplayASCII_String(1, 6, (uint8_t *)szInputStrBuf);

        if (g_Setting.tDrvInfo[1].u8DrvStatus == 0)
            LCD_DisplayFont16_String(5, 6, (uint8_t *)"������");
        else if (g_Setting.tDrvInfo[1].u8DrvStatus == 1)
            LCD_DisplayFont16_String(5, 6, (uint8_t *)"��p��");
        else if (g_Setting.tDrvInfo[1].u8DrvStatus == 2)
            LCD_DisplayFont16_String(5, 6, (uint8_t *)"�ݯZ��");
        else if (g_Setting.tDrvInfo[1].u8DrvStatus == 3)
            LCD_DisplayFont16_String(5, 6, (uint8_t *)"�𮧤�");
    }
    else if (u8MainPageIdx == 2)
    {
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"ĵ�i��T");

        if ((bPreWoringFlag == TRUE) && (bShowErrorFlag == FALSE))
        {
            LCD_DisplayFont16_String(1, 2, (uint8_t *)"�Y�N�W�ɾr�p");
        }
        else if (bShowErrorFlag == TRUE)
        {
            LCD_DisplayFont16_String(1, 2, (uint8_t *)"�r�p�W�ɶ}��");
        }
        else
        {
            LCD_DisplayASCII_String(0, 2, (uint8_t *)"                ");
        }

        if (HAL_GPIO_ReadPin(P_EMPTY_GPIO_Port, P_EMPTY_Pin) == 0)
        {
            LCD_DisplayFont16_String(1, 4, (uint8_t *)"�L�����ʯ�");
        }
        else
        {
            LCD_DisplayASCII_String(0, 4, (uint8_t *)"                ");
        }

        if (stPwrOffEvent.bIsShowFlag == TRUE)
        {
            sprintf(szInputStrBuf, "%02X:%02X:%02X",
                    stPwrOffEvent.u8EventTime[0], stPwrOffEvent.u8EventTime[1], stPwrOffEvent.u8EventTime[2]);
            LCD_DisplayASCII_String(0, 6, (uint8_t *)szInputStrBuf);
            LCD_DisplayFont16_String(4, 6, (uint8_t *)"�_�q�o��");
        }
    }
    else if (u8MainPageIdx == 3)
    {
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"BUS-ID: ");
        sprintf(szStrBuf, "%s", g_Setting.tBusInfo.cBus_ID);
        LCD_DisplayASCII_String(7, 0, (uint8_t *)szStrBuf);

        LCD_DisplayASCII_String(0, 2, (uint8_t *)"HW-ID: ");
        LCD_DisplayASCII_String(7, 2, (uint8_t *)HARDWARE_VERSION);

        LCD_DisplayASCII_String(0, 4, (uint8_t *)"FW-ID: ");
        sprintf(szStrBuf, "%d.%03d%s", (VERSION_BUILD / 1000), (VERSION_BUILD % 1000), FW_VER_LCM_SHOW);
        LCD_DisplayASCII_String(7, 4, (uint8_t *)szStrBuf);

        // LCD_DisplayASCII_String(0, 6, (uint8_t *)"DATE: ");
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"���");
        LCD_DisplayASCII_String(4, 6, (uint8_t *)": ");
        LCD_DisplayASCII_String(6, 6, (uint8_t *)FIRMWARE_DATE);
    }
    else if (u8MainPageIdx == 4)
    {
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"GPS");
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"���A");
        LCD_DisplayASCII_String(8, 0, (uint8_t *)": ");
        if (g_SysInfo.tGPS_Data.bStatus == TRUE)
            LCD_DisplayASCII_String(10, 0, (uint8_t *)"A");
        else
            LCD_DisplayASCII_String(10, 0, (uint8_t *)"V");

        // LCD_DisplayASCII_String(0, 2, (uint8_t *)"ST: ");
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�ìP��");
        LCD_DisplayASCII_String(6, 2, (uint8_t *)": ");
        sprintf(szStrBuf, "%02d", g_SysInfo.tGPS_Data.u8StlNum);
        LCD_DisplayASCII_String(8, 2, (uint8_t *)szStrBuf);

        // LCD_DisplayASCII_String(0, 4, (uint8_t *)"DATE: ");
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"���");
        LCD_DisplayASCII_String(4, 4, (uint8_t *)": ");

        if (bFirstGPSFixFlag == TRUE)
        {
            sprintf(szStrBuf, "%04d/%02d/%02d",
                    g_SysInfo.tGPS_Data.utcTime.u16Year,
                    g_SysInfo.tGPS_Data.utcTime.u8Month,
                    g_SysInfo.tGPS_Data.utcTime.u8Day);
        }
        else
        {
            sprintf(szStrBuf, "----/--/--");
        }
        LCD_DisplayASCII_String(6, 4, (uint8_t *)szStrBuf);

        // LCD_DisplayASCII_String(0, 6, (uint8_t *)"TIME: ");
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"�ɶ�");
        LCD_DisplayASCII_String(4, 6, (uint8_t *)": ");

        if (bFirstGPSFixFlag == TRUE)
        {
            sprintf(szStrBuf, "%02d:%02d:%02d",
                    g_SysInfo.tGPS_Data.utcTime.u8Hour,
                    g_SysInfo.tGPS_Data.utcTime.u8Min,
                    g_SysInfo.tGPS_Data.utcTime.u8Sec);
        }
        else
        {
            sprintf(szStrBuf, "--:--:--");
        }

        LCD_DisplayASCII_String(6, 6, (uint8_t *)szStrBuf);
    }
    else if (u8MainPageIdx == 5)
    {
        // sprintf(szStrBuf,"R:%05d", g_SysInfo.VehicleState.u16RPM);

        // LCD_DisplayASCII_String(0, 0, (uint8_t *)szStrBuf);

        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���{");

        // sprintf(szStrBuf,"%09.1f km", (float)M_2_100M(g_SysInfo.tVehicleState.dDistance)/10);
        sprintf(szStrBuf, "%09.1f km", M_2_KM(g_SysInfo.tVehicleState.dDistance));

        LCD_DisplayASCII_String(4, 0, (uint8_t *)szStrBuf);

        if (strlen(g_Setting.tBusInfo.cVIN) > 10)
        {
            memset(szInputStrBuf2, 0x00, sizeof(szInputStrBuf2));
            strncpy(szInputStrBuf2, g_Setting.tBusInfo.cVIN, 10);
            sprintf(szInputStrBuf, "VIN: %s", szInputStrBuf2);
            LCD_DisplayASCII_String(0, 2, (uint8_t *)szInputStrBuf);

            memset(szInputStrBuf2, 0x00, sizeof(szInputStrBuf2));
            strncpy(szInputStrBuf2, g_Setting.tBusInfo.cVIN + 10, (strlen(g_Setting.tBusInfo.cVIN) - 10));
            sprintf(szInputStrBuf, "     %s", szInputStrBuf2);
            LCD_DisplayASCII_String(0, 4, (uint8_t *)szInputStrBuf);
        }
        else
        {
            sprintf(szStrBuf, "VIN: %s", g_Setting.tBusInfo.cVIN);
            LCD_DisplayASCII_String(0, 2, (uint8_t *)szStrBuf);
        }
    }
    else if (u8MainPageIdx == 6)
    {
#if ENABLE_GSM_NET
        sprintf(szStrBuf, "GSM CSQ: %02d", g_Setting.tGSM_SET.tNetSet.u8GSM_CSQ);
        LCD_DisplayASCII_String(0, 0, (uint8_t *)szStrBuf);

        sprintf(szStrBuf, "APN: %s", g_Setting.tGSM_SET.tNetSet.cAPN);
        LCD_DisplayASCII_String(0, 2, (uint8_t *)szStrBuf);

        if (bTCPIP_Connect[SOCKET_BANK_DEF] == TRUE)
        {
            // LCD_DisplayASCII_String(0, 4, (uint8_t *)"Server: OnLine");
            LCD_DisplayFont16_String(0, 4, (uint8_t *)"���A��");
            LCD_DisplayFont16_String(5, 4, (uint8_t *)"�s�u");
        }
        else
        {
            // LCD_DisplayASCII_String(0, 4, (uint8_t *)"Server: OffLine");
            LCD_DisplayFont16_String(0, 4, (uint8_t *)"���A��");
            LCD_DisplayFont16_String(5, 4, (uint8_t *)"���u");
        }
#endif
    }
    else if (u8MainPageIdx == 7)
    {
#if ENABLE_GSM_NET
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"���A����T");

        LCD_DisplayASCII_String(0, 2, (uint8_t *)"IP Addr:");
        sprintf(szStrBuf, "%3d.%3d.%3d.%3d",
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[3],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[2],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[1],
                g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u32IP_Addr._U8[0]);
        LCD_DisplayASCII_String(1, 4, (uint8_t *)szStrBuf);

        sprintf(szStrBuf, "Port: %5d", g_Setting.tGSM_SET.tServerInfo[SOCKET_BANK_DEF].u16Port);
        LCD_DisplayASCII_String(0, 6, (uint8_t *)szStrBuf);
#endif
    }
    else if (u8MainPageIdx == 8)
    {
#if ENABLE_GSM_NET

#endif
    }
#if CUSTOMER_TYPE == CUSTOMER_SCANIA
    else if (u8MainPageIdx == 9)
    {
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�����y��");
        LCD_DisplayASCII_String(0, 2, (uint8_t *)"Change Language");

        if (u8SelectLang == LANG_CHN)
        {
            LCD_DisplayASCII_String(0, 4, (uint8_t *)"English(");
            LCD_DisplayFont16_String(4, 4, (uint8_t *)"�^��");
            LCD_DisplayASCII_String(12, 4, (uint8_t *)")");
        }
        else if (u8SelectLang == LANG_ENG)
        {

            LCD_DisplayFont16_String(0, 4, (uint8_t *)"����");
            LCD_DisplayASCII_String(5, 4, (uint8_t *)"(Chinese)");
        }

        LCD_DisplayASCII_String(0, 6, (uint8_t *)"> OK (");
        LCD_DisplayFont16_String(3, 6, (uint8_t *)"�T�w");
        LCD_DisplayASCII_String(10, 6, (uint8_t *)")");
    }
#endif
}

//============================================================================//
// @brief  Display a menu.
// @param  u8Item: Menu ID to display.
// @retval None.
//============================================================================//
void LCD_DisplaySubMenu(uint8_t u8Item)
{
    uint32_t u32Buf;
    uint8_t BCDBuf[7];
    time_t ttTime;

    LCD_ScreenClear();

    switch (u8Item)
    {
    case 0:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�r�p�]�w");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�C�L�\��");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"��ƤU��");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�t�ΰѼ�");

        break;

    case 1:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�r�p���ʿ�J");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�r�p�N�X��J");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�r�p�������");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�r�p�H����J");

        break;

    case 2:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�D�r�p");

        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[0].u32DrvIdNum);

        LCD_DisplayASCII_String(4, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�@�P�r�p");

        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[1].u32DrvIdNum);

        LCD_DisplayASCII_String(4, 6, (uint8_t *)szInputStrBuf);

        break;

    case 3:
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�ݯZ��");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�𮧤�");

        break;

    case 4:
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�r�p�N�X��J");
        LCD_DisplayASCII_String_Line(4, 4, (uint8_t *)szInputStrBuf, u8InputIdx);

        break;

    case 5:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�ثe�֭p�r�p");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�֭p�ݯZ��");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"���g�֭p��p");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�W�g�֭p��p");

        break;

    case 6:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�C�L�\��");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�̪�");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"24");
        LCD_DisplayFont16_String(5, 2, (uint8_t *)"�p��");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"���w���");

        break;

    case 7:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���w�����J");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�~���");

        sprintf(szInputStrBuf, "%c%c/%c%c/%c%c",
                u8InputBCDBuf[0], u8InputBCDBuf[1], u8InputBCDBuf[2],
                u8InputBCDBuf[3], u8InputBCDBuf[4], u8InputBCDBuf[5]);

        LCD_DisplayASCII_String_Line(3, 4, (uint8_t *)szInputStrBuf, u8InputIdx);

        break;

    case 8:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�r�p����");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�ƥ�ƬG��T");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�޳N���");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�t�צ��u��");

        break;

    case 9:
        if (g_Setting.bVdrNow == FALSE)
        {
            LCD_DisplayFont16_String(0, 0, (uint8_t *)"��ƤU��");
        }
        else
        {
            LCD_DisplayFont16_String(2, 0, (uint8_t *)"�릸�U��");
        }

        // LCD_DisplayFont16_String(2, 2, (uint8_t *)"�̪�");
        // LCD_DisplayASCII_String(8, 2, (uint8_t *)"24");
        // LCD_DisplayFont16_String(5, 2, (uint8_t *)"�p��");
        // LCD_DisplayFont16_String(2, 2, (uint8_t *)"���w����϶�");
        // LCD_DisplayFont16_String(2, 4, (uint8_t *)"���w���");
        // LCD_DisplayFont16_String(2, 6, (uint8_t *)"����O����");
        // LCD_DisplayASCII_SPWord(0, 6);

        LCD_DisplayFont16_String(2, 2, (uint8_t *)"���w���");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"����O����");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�̪�p�ɤU��");

        break;

    case 10:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�̪�");
        LCD_DisplayASCII_String(4, 0, (uint8_t *)"24");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�p�ɤU��");

        sprintf(szInputStrBuf, "%02d", u8DLHour);

        LCD_DisplayASCII_String(4, 4, (uint8_t *)szInputStrBuf);

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�p��");

        break;

    case 11:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���w����U��");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�~���");

        sprintf(szInputStrBuf, "%c%c/%c%c/%c%c",
                u8InputBCDBuf[0], u8InputBCDBuf[1], u8InputBCDBuf[2],
                u8InputBCDBuf[3], u8InputBCDBuf[4], u8InputBCDBuf[5]);

        LCD_DisplayASCII_String_Line(3, 4, (uint8_t *)szInputStrBuf, u8InputIdx);

        break;

    case 12:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�ɶ������]�w");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"���t�����]�w");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�����t�Ӹ��");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"��ܫ̳]�w");

        break;

    case 13:
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�t�ήɶ�");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�s���p���e");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�֭p�𮧪��e");

        break;

    case 14:
        if (bIsEngModeFlag == TRUE)
        {
            LCD_DisplayFont16_String(2, 0, (uint8_t *)"���");
            LCD_DisplayASCII_String(8, 0, (uint8_t *)"Gain");
            LCD_DisplayFont16_String(6, 0, (uint8_t *)"��");
        }

        LCD_DisplayFont16_String(2, 2, (uint8_t *)"���t");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"Gain");
        LCD_DisplayFont16_String(6, 2, (uint8_t *)"��");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�۰ʮե��ɶ�");

        if (bIsEngModeFlag == TRUE)
        {
            LCD_DisplayASCII_String(4, 6, (uint8_t *)"K Gain");
            LCD_DisplayFont16_String(5, 6, (uint8_t *)"��");
        }
        break;

    case 15:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"��l���{�]�w");

        sprintf(szInputStrBuf, "%09.1f km", (float)M_2_100M(g_SysInfo.tVehicleState.dDistance) / 10);

        LCD_DisplayASCII_String(0, 4, (uint8_t *)szInputStrBuf);

        break;

    case 16:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"Gain");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�п�ܨ���");

        // LCD_DisplayASCII_String(0, 2, (uint8_t *)szCarType[u8CarTypeIdx]);
        LCD_DisplayASCII_String(0, 2, (uint8_t *)SetCarType[u8CarTypeIndex[u8CarTypeIdx]].cCarTypeName);

        LCD_DisplayASCII_String(0, 6, (uint8_t *)"> ");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�T�w");
        /*
                    LCD_DisplayFont16_String(2, 2, (uint8_t *)"���t");
                    LCD_DisplayASCII_String(8, 2, (uint8_t *)"Gain");
                    LCD_DisplayFont16_String(6, 2, (uint8_t *)"��");

                    sprintf(u8InputGainBuf, "%03d", (int)g_Setting.tSpeedSet.fGain);

                    LCD_DisplayASCII_String(5, 4, (uint8_t *)u8InputGainBuf);
        */
        break;

    case 17:
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"��t");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"Gain");
        LCD_DisplayFont16_String(6, 2, (uint8_t *)"��");

        sprintf(u8InputGainBuf, "%03d", g_Setting.tRPM_Set.u16DIV);

        LCD_DisplayASCII_String(5, 4, (uint8_t *)u8InputGainBuf);

        break;

    case 18:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�ثe�֭p�r�p");

        sprintf(szInputStrBuf, "1. %02d hr %02d min", (g_Setting.tDrvInfo[0].stDrvRecDay[13].u32WorkTime / 3600),
                ((g_Setting.tDrvInfo[0].stDrvRecDay[13].u32WorkTime % 3600) / 60));

        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf);

        sprintf(szInputStrBuf, "2. %02d hr %02d min", (g_Setting.tDrvInfo[1].stDrvRecDay[13].u32WorkTime / 3600),
                ((g_Setting.tDrvInfo[1].stDrvRecDay[13].u32WorkTime % 3600) / 60));

        LCD_DisplayASCII_String(1, 4, (uint8_t *)szInputStrBuf);

        break;

    case 19:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�֭p�ݯZ��");

        sprintf(szInputStrBuf, "1. %02d:%02d, %02d:%02d",
                (g_Setting.tDrvInfo[0].stDrvRecDay[13].u32StandByTime / 3600),
                ((g_Setting.tDrvInfo[0].stDrvRecDay[13].u32StandByTime % 3600) / 60),
                (g_Setting.tDrvInfo[0].stDrvRecDay[13].u32RestTime / 3600),
                ((g_Setting.tDrvInfo[0].stDrvRecDay[13].u32RestTime % 3600) / 60));

        LCD_DisplayASCII_String(0, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"�֭p�ݯZ��");

        sprintf(szInputStrBuf, "2. %02d:%02d, %02d:%02d",
                (g_Setting.tDrvInfo[1].stDrvRecDay[13].u32StandByTime / 3600),
                ((g_Setting.tDrvInfo[1].stDrvRecDay[13].u32StandByTime % 3600) / 60),
                (g_Setting.tDrvInfo[1].stDrvRecDay[13].u32RestTime / 3600),
                ((g_Setting.tDrvInfo[1].stDrvRecDay[13].u32RestTime % 3600) / 60));

        LCD_DisplayASCII_String(0, 6, (uint8_t *)szInputStrBuf);

        break;

    case 20:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���g�֭p��p");

        u32Buf = 0;

        for (int i = 7; i < 14; i++)
            u32Buf += g_Setting.tDrvInfo[0].stDrvRecDay[i].u32WorkTime;

        sprintf(szInputStrBuf, "1. %02d hr %02d min", (u32Buf / 3600), ((u32Buf % 3600) / 60));

        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf);

        u32Buf = 0;

        for (int i = 7; i < 14; i++)
            u32Buf += g_Setting.tDrvInfo[1].stDrvRecDay[i].u32WorkTime;

        sprintf(szInputStrBuf, "2. %02d hr %02d min", (u32Buf / 3600), ((u32Buf % 3600) / 60));

        LCD_DisplayASCII_String(1, 4, (uint8_t *)szInputStrBuf);

        break;

    case 21:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�W�g�֭p��p");

        u32Buf = 0;

        for (int i = 0; i < 7; i++)
            u32Buf += g_Setting.tDrvInfo[0].stDrvRecDay[i].u32WorkTime;

        sprintf(szInputStrBuf, "1. %02d hr %02d min", (u32Buf / 3600), ((u32Buf % 3600) / 60));

        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf);

        u32Buf = 0;

        for (int i = 0; i < 7; i++)
        {
            u32Buf += g_Setting.tDrvInfo[1].stDrvRecDay[i].u32WorkTime;
        }
        sprintf(szInputStrBuf, "2. %02d hr %02d min", (u32Buf / 3600), ((u32Buf % 3600) / 60));

        LCD_DisplayASCII_String(1, 4, (uint8_t *)szInputStrBuf);

        break;

    case 22:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�]�w���e�r�p��");

        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�D�r�p");

        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�@�P�r�p");

        break;

    case 23:
        ttTime = System_GetTime();

        SecToBCDTime((uint32_t)ttTime, BCDBuf);

        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�{�b�ɶ�");

        sprintf(szInputStrBuf, " %02X%02X/%02X/%02X", BCDBuf[0], BCDBuf[1], BCDBuf[2], BCDBuf[3]);

        LCD_DisplayASCII_String(0, 2, (uint8_t *)szInputStrBuf);

        sprintf(szInputStrBuf, " %02X:%02X:%02X", BCDBuf[4], BCDBuf[5], BCDBuf[6]);

        LCD_DisplayASCII_String(3, 4, (uint8_t *)szInputStrBuf);

        break;

    case 24:
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�s���p���e��");

        sprintf(szInputStrBuf, "%d hour %d min", g_Setting.tDRV_SET.u8DriveThreshold[0], g_Setting.tDRV_SET.u8DriveThreshold[1]);

        LCD_DisplayASCII_String(2, 4, (uint8_t *)szInputStrBuf);

        break;

    case 25:
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�֭p�𮧪��e��");

        sprintf(szInputStrBuf, "%d hour %d min", g_Setting.tDRV_SET.u8RestThreshold[0], g_Setting.tDRV_SET.u8RestThreshold[1]);

        LCD_DisplayASCII_String(2, 4, (uint8_t *)szInputStrBuf);

        break;

    case 26:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���Ū���U����");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�еy��");

        break;

    case 27:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"ĵ�i");

        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�а�����A�ާ@");

        break;

    case 28:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayFont16_String(1, 4, (uint8_t *)"ĵ�i");

        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�z�w�W�ɾr�p");

        break;

    case 29:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"��ƳB�z�C�L��");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�еy��");

        break;

    case 30:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�}���ۧڴ���");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�еy��");

        break;

    case 31:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�}���ۧڴ���");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"����");

        break;

    case 32:
        LCD_DisplayFont16_String(1, 0, (uint8_t *)"�п�ܤH��");

        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�D�r�p");

        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�@�P�r�p");

        break;

    case 33:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�̪�");

        LCD_DisplayASCII_String(4, 0, (uint8_t *)"24");

        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�p�ɦC�L");

        sprintf(szInputStrBuf, "%02d", u8DLHour);

        LCD_DisplayASCII_String(4, 4, (uint8_t *)szInputStrBuf);

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�p��");

        break;

    case 34:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���Ū���C�L��");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�еy��");

        break;

    case 35:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�ìP�w��G��");

        LCD_DisplayFont16_String(3, 6, (uint8_t *)"�к���");

        break;

    case 36:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�L�����G��");

        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�к��״���");

        break;

    case 37:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayFont16_String(2, 4, (uint8_t *)"��ܫ̬G��");

        LCD_DisplayFont16_String(3, 6, (uint8_t *)"�к���");

        break;

    case 38:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���Ū���U��");

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"����");

        break;

    case 39:
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�t�αN�b");

#if ENABLE_ADJUST_OFF_TIME
        if (g_SysInfo.tVehicleState.bPWR_ON == 1)
        {
            // LCD_DisplayFont16_String(1, 4, (uint8_t *)"���̬�������");
            LCD_DisplayFont16_String(2, 4, (uint8_t *)"�y������");
        }
        else
        {
            // LCD_DisplayFont16_String(1, 4, (uint8_t *)"�G�̬�������");
            LCD_DisplayFont16_String(2, 4, (uint8_t *)"���W����");
        }
#else
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�G�̬�������");
#endif

        break;

    case 40:

        break;

    case 41:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���Ū���C�L");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"����");

        break;

    case 42:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�̪�");
        LCD_DisplayASCII_String(4, 0, (uint8_t *)"24");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�p�ɸ��");

        sprintf(szInputStrBuf, "%02d", u8DLHour);

        LCD_DisplayASCII_String(4, 4, (uint8_t *)szInputStrBuf);

        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�p��");

        break;

    case 43:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���Ū���U��");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"����");

        break;

    case 44:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�s�y��");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���@�L���");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�ѥ��������q");

        break;

    case 45:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�a�}");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�s�_�����s��");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�s�_�j�D���q");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�|�@�@���̼�");

        break;

    case 46:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�]�ƽs��");
        LCD_DisplayASCII_String(8, 0, (uint8_t *)"JAS208");
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�n�鸹�X");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"�s�y�~��");
        LCD_DisplayASCII_String(8, 4, (uint8_t *)"2019");
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"�{�Ҹ��X");
        LCD_DisplayASCII_String(8, 6, (uint8_t *)"JS300001");

        break;

    case 47:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�n��w�ˤ��");
        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayASCII_String(1, 4, (uint8_t *)szInputStrBuf2);
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"���P���X");
        LCD_DisplayASCII_String(8, 6, (uint8_t *)szInputStrBuf3);

        break;

    case 48:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"��ܫ̭I���}��");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�I���}");
        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�I����");

        break;

    case 49:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayFont16_String(1, 4, (uint8_t *)"ĵ�i");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�z�N�W�ɾr�p");

        break;

    case 51:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayASCII_String(3, 4, (uint8_t *)"SIM");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"����");
        LCD_DisplayASCII_String(3, 6, (uint8_t *)"GSM");
        LCD_DisplayFont16_String(3, 6, (uint8_t *)"�\�ఱ��");

        break;

    case 52:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayASCII_String(3, 4, (uint8_t *)"GSM");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�Ҳե���");
        LCD_DisplayASCII_String(3, 6, (uint8_t *)"GSM");
        LCD_DisplayFont16_String(3, 6, (uint8_t *)"�\�ఱ��");

        break;

    case 53:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayASCII_String(4, 4, (uint8_t *)"SD");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�d����");
        LCD_DisplayFont16_String(2, 6, (uint8_t *)"�t�ΰ���");

        bLCD_Stop = TRUE;

        break;

    case 54:
        LCD_Showu8TWord(3, 0);

        // LCD_DisplayFont16_String(1, 4, (uint8_t *)"�~���O���饢��");
        // LCD_DisplayFont16_String(2, 6, (uint8_t *)"�t�ΰ���");
        LCD_DisplayASCII_String(2, 4, (uint8_t *)"MX25L ERROR");
        LCD_DisplayASCII_String(2, 4, (uint8_t *)"SYSTEM STOP");

        bLCD_Stop = TRUE;

        break;

    case 55:
        LCD_Showu8TWord(3, 0);

        LCD_DisplayASCII_String(3, 4, (uint8_t *)"I2C");
        LCD_DisplayFont16_String(3, 4, (uint8_t *)"�Ҳե���");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�гq���A�Ȥ���");

        break;

    case 56:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�п�J�K�X");

        sprintf(szInputStrBuf, "%d%d%d%d", u8PassWord[0], u8PassWord[1], u8PassWord[2], u8PassWord[3]);

        LCD_DisplayASCII_String_Line(4, 4, (uint8_t *)szInputStrBuf, u8InputIdx);

        break;

    case 57:
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�K�X���~");

        LCD_DisplayFont16_String(2, 4, (uint8_t *)"�]�w����");

        break;

    case 58:
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"���t");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"Gain");
        LCD_DisplayFont16_String(6, 2, (uint8_t *)"��");

        sprintf(u8InputGainBuf, "%03d", (int)g_Setting.tSpeedSet.fGain);

        LCD_DisplayASCII_String(5, 4, (uint8_t *)u8InputGainBuf);
        break;

    case 59:
        ttTime = g_Setting.tSpeedSet.ttAgain_Time + g_Setting.i32TimeZoneOffset;
        SecToBCDTime((uint32_t)ttTime, BCDBuf);
        LCD_DisplayFont16_String(1, 0, (uint8_t *)"�U���ե��ɶ�");
        sprintf(szInputStrBuf, "%02X%02X/%02X/%02X", BCDBuf[0], BCDBuf[1], BCDBuf[2], BCDBuf[3]);
        LCD_DisplayASCII_String(2, 2, (uint8_t *)szInputStrBuf);
        sprintf(szInputStrBuf, "%02X:%02X:%02X", BCDBuf[4], BCDBuf[5], BCDBuf[6]);
        LCD_DisplayASCII_String(4, 4, (uint8_t *)szInputStrBuf);

        break;

    case 60:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"��ƤU��");

        // LCD_DisplayFont16_String(2, 2, (uint8_t *)"���w�϶�");
        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�̪�");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"24");
        LCD_DisplayFont16_String(5, 2, (uint8_t *)"�p��");

        break;

    case 61:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���w�϶��U��");

        LCD_DisplayFont16_String(2, 2, (uint8_t *)"�~���");

        sprintf(szInputStrBuf, "%c%c/%c%c/%c%c",
                u8InputBCDBuf[0], u8InputBCDBuf[1], u8InputBCDBuf[2],
                u8InputBCDBuf[3], u8InputBCDBuf[4], u8InputBCDBuf[5]);

        sprintf(szInputStrBuf2, "%c%c/%c%c/%c%c",
                u8InputBCDBuf2[0], u8InputBCDBuf2[1], u8InputBCDBuf2[2],
                u8InputBCDBuf2[3], u8InputBCDBuf2[4], u8InputBCDBuf2[5]);

        if (u8InputIdx < 8)
        {
            LCD_DisplayASCII_String_Line(3, 4, (uint8_t *)szInputStrBuf, u8InputIdx);
            LCD_DisplayASCII_String(3, 6, (uint8_t *)szInputStrBuf2);
        }
        else
        {
            LCD_DisplayASCII_String(3, 4, (uint8_t *)szInputStrBuf);
            LCD_DisplayASCII_String_Line(3, 6, (uint8_t *)szInputStrBuf2, (u8InputIdx - 8));
        }

        break;

    case 63:
        LCD_Showu8TWord(3, 0);
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"ĵ�i");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�_�q�ƥ�o��");

        break;

    // USB read setup BUS ID ERROR
    case 64:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[BUSID");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�u���J");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)":");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�Ʀr�B�^��B");
        LCD_DisplayASCII_String(13, 4, (uint8_t *)"-");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup BUS SN ERROR
    case 65:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[BUSSN");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�u���J");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)":");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�Ʀr�B�^��B");
        LCD_DisplayASCII_String(13, 4, (uint8_t *)"-");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup VIN ERROR
    case 66:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[VIN");
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(12, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�u���J");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)":");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�Ʀr�B�^��B");
        LCD_DisplayASCII_String(13, 4, (uint8_t *)"-");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup DRIVER ID ERROR
    case 67:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[DRVID");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"�u���J");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)":");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�Ʀr�B");
        LCD_DisplayASCII_String(7, 4, (uint8_t *)"A-F");
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup BUS ID ERROR
    case 68:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[BUSID");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���פ��i");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)" >");

        sprintf(szInputStrBuf, "%02d", (OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_ID) - 1));
        LCD_DisplayASCII_String(12, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(7, 2, (uint8_t *)"�r");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�B����������"); // chinese word font: 3 bytes/word
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup BUS SN ERROR
    case 69:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[BUSSN");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���פ��i");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)" >");

        sprintf(szInputStrBuf, "%02d", (OFFSET_SIZE(gSettingTypeDef, tBusInfo.cBus_SN) - 1));
        LCD_DisplayASCII_String(12, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(7, 2, (uint8_t *)"�r");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�B����������"); // chinese word font: 3 bytes/word
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    // USB read setup BUS VIN ERROR
    case 70:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[VIN");
        LCD_DisplayFont16_String(2, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(12, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���פ��i");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)" >");

        sprintf(szInputStrBuf, "%02d", (OFFSET_SIZE(gSettingTypeDef, tBusInfo.cVIN) - 1));
        LCD_DisplayASCII_String(12, 2, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(7, 2, (uint8_t *)"�r");
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�B����������"); // chinese word font: 3 bytes/word
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");

        break;

    case 71:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���P���]�w");
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�榡�d��");
        LCD_DisplayASCII_String(8, 2, (uint8_t *)"ABC-1234");
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"�Ы��T�w��J");

        // LCD_DisplayFont16_String(0, 0, (uint8_t *)"���P���]�w");
        // LCD_DisplayFont16_String(0, 2, (uint8_t *)"�榡��");
        // LCD_DisplayASCII_String(6, 2, (uint8_t *)"XXX-XXXX");
        // LCD_DisplayFont16_String(0, 4, (uint8_t *)"�d��");
        // LCD_DisplayASCII_String(4, 4, (uint8_t *)": ABC-1234");
        // LCD_DisplayFont16_String(0, 6, (uint8_t *)"�Ы��T�w��J");
        break;

    case 72:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"�Ы��W�U����");
        LCD_DisplayASCII_String_Line(1, 4, (uint8_t *)szInputStrBuf, u8InputIdx);
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"��J�����Ы��T�w");
        // LCD_DisplayFont16_String(0, 0, (uint8_t *)"�п�J����");
        // LCD_DisplayFont16_String(0, 2, (uint8_t *)"���W�U���^�Ʀr");
        // LCD_DisplayASCII_String_Line(1, 4, (uint8_t *)szInputStrBuf, u8InputIdx);
        // LCD_DisplayFont16_String(0, 6, (uint8_t *)"������Ы��T�w");
        break;

    case 73:
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"���P��J���~");
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"�Э��s��J");
        break;

    case 74:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"���~");
        LCD_DisplayFont16_String(0, 2, (uint8_t *)"�Х����J�H����");
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"�A���s�ާ@");
        break;

    case 75:
        LCD_DisplayASCII_String(0, 0, (uint8_t *)"[DRVID");
        LCD_DisplayFont16_String(3, 0, (uint8_t *)"�榡���~");
        LCD_DisplayASCII_String(14, 0, (uint8_t *)"]");
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���פ��i");
        LCD_DisplayASCII_String(10, 2, (uint8_t *)" >8");
        LCD_DisplayFont16_String(7, 2, (uint8_t *)"�r");

        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�B����������"); // chinese word font: 3 bytes/word
        LCD_DisplayFont16_String(1, 6, (uint8_t *)"�Щް�");
        LCD_DisplayASCII_String(9, 6, (uint8_t *)"USB!!");
        break;

    case 76:
        LCD_DisplayFont16_String(0, 0, (uint8_t *)"��J���P��");
        LCD_DisplayASCII_String(1, 2, (uint8_t *)szInputStrBuf2);
        LCD_DisplayFont16_String(0, 4, (uint8_t *)"���W�䭫�s��J");
        LCD_DisplayFont16_String(0, 6, (uint8_t *)"���T�Ы��T�w");
        break;

    case 77:
        LCD_DisplayFont16_String(1, 2, (uint8_t *)"���P��J����");
        break;

    case 78:
        LCD_DisplayFont16_String(1, 0, (uint8_t *)"�ثe");
        LCD_DisplayASCII_String(6, 0, (uint8_t *)"K");
        LCD_DisplayFont16_String(4, 0, (uint8_t *)"��");
        LCD_DisplayASCII_String(11, 0, (uint8_t *)szInputStrBuf);
        LCD_DisplayFont16_String(1, 4, (uint8_t *)"�п�J�s");
        LCD_DisplayASCII_String(10, 4, (uint8_t *)"K");
        LCD_DisplayFont16_String(6, 4, (uint8_t *)"��");
        LCD_DisplayASCII_String_Line(4, 6, (uint8_t *)szInputStrBuf2, u8InputIdx);
        break;

    case 90:
        LCD_DisplayASCII_String(1, 2, (uint8_t *)"Downloading");
        LCD_DisplayASCII_String(1, 4, (uint8_t *)"font file");
        LCD_DisplayASCII_String(1, 6, (uint8_t *)"Please wait");

        break;

    case 91:
        LCD_DisplayASCII_String(1, 2, (uint8_t *)"Downloading");
        LCD_DisplayASCII_String(1, 4, (uint8_t *)"font file");
        LCD_DisplayASCII_String(1, 6, (uint8_t *)"OK");

        break;

    case 92:
        LCD_DisplayASCII_String(2, 2, (uint8_t *)"DB9 RS232");
        LCD_DisplayASCII_String(2, 4, (uint8_t *)"Test OK");

        break;

    case 93:
        LCD_DisplayASCII_String(2, 2, (uint8_t *)"DB9 RS232");
        LCD_DisplayASCII_String(2, 4, (uint8_t *)"Test NG");

        break;

    case 94:
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                LCD_DisplayASCII_AllBlock(i, j * 2);
            }
        }
        break;

    default:
        break;
    }
}

//============================================================================//
// @brief  Scan the key status.
// @param  None.
// @retval None.
//============================================================================//
void Key_Scan(void)
{
    uint8_t u8PressSignal = 0;
    uint32_t u32Dist;
    uint16_t u16YY;
    uint8_t u8DD, u8MM;
    time_t ttTime;
    char cBufStr[30];
    TechnicalDataTypeDef stTechnical;

    if (g_bPTestMode == FALSE)
    {
        if (HW_PBGetState(KEY1) == 0)
        {
            // HAL_Delay(20);
            HAL_Delay(100);

            if (HW_PBGetState(KEY1) != 0)
                return;

            u8PressSignal = 1;
        }
        else if (HW_PBGetState(KEY2) == 0)
        {
            // HAL_Delay(20);
            HAL_Delay(100);

            if (HW_PBGetState(KEY2) != 0)
                return;

            u8PressSignal = 2;
        }
        else if (HW_PBGetState(KEY3) == 0)
        {
            // HAL_Delay(20);
            HAL_Delay(100);

            if (HW_PBGetState(KEY3) != 0)
                return;

            u8PressSignal = 3;
        }
        else if (HW_PBGetState(KEY4) == 0)
        {
            // HAL_Delay(20);
            HAL_Delay(100);

            if (HW_PBGetState(KEY4) != 0)
                return;

            u8PressSignal = 4;
        }
        else if (HW_PBGetState(KEY5) == 0)
        {
            // HAL_Delay(20);
            HAL_Delay(100);

            if (HW_PBGetState(KEY5) != 0)
                return;

            u8PressSignal = 5;
        }
    }
    else
    {
        if (HW_PBGetState(KEY1) == 0)
        {
            RTOSprintf("\r\n> ### Key 1 ###");

            sprintf(cBufStr, "$AS+TEST RS232=\r\n");

            Protocl_ChkCmdPkg(cBufStr, NULL);
        }
        else if (HW_PBGetState(KEY2) == 0)
        {
            RTOSprintf("\r\n> ### Key 2 ###");

            USB_DownloadFile(FILENAME_SYSLOG, FILENAME_SYSLOG);
        }
        else if (HW_PBGetState(KEY3) == 0)
        {
            RTOSprintf("\r\n> ### Key 3 ###");

            if (HAL_GPIO_ReadPin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin) == 1)
            {
                HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_SET);
            }
        }
        else if (HW_PBGetState(KEY4) == 0)
        {
            RTOSprintf("\r\n> ### Key 4 ###");

            LCD_SetShowAutoTestPage(94);
        }
        else if (HW_PBGetState(KEY5) == 0)
        {
            RTOSprintf("\r\n> ### Key 5 ###");

            PRN_Printing(4);
        }

        u8PressSignal = 0;
    }

    if (bReFlashMenuFlag == TRUE)
    {
        u8PressSignal = 6;
    }

    if (u8PressSignal > 0)
    {
        bMoveCursor = FALSE;

        switch (u8PressSignal)
        {
        case 1:
            // if(0 == HW_PBGetState(KEY1))
            {
                EXTprintf("\r\n> ### Key 1 ###");

                if ((u8ShowMenuItem == 26) || (u8ShowMenuItem == 39))
                    return;

                u32GetTick = HAL_GetTick();

                if (u8ShowMenuItem == 28)
                {
                    u32ErrorPageTick = u32GetTick;
                }

                if (u8ShowMenuItem != 0)
                {
                    LCD_ClearCursor(u8CursorIndex);

                    u8ShowMenuItem = 0;

                    u8CursorIndex = 0;

                    bReFlashMenuFlag = TRUE;

                    // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8CursorIndex: %d ###", u8ShowMenuItem, u8CursorIndex);
                }
                else
                {
                    u8ShowMenuItem = 50;

                    u8MainPageIdx = 0;

                    bClearPageFlag = TRUE;
                }

                break;
            }

        case 2:
            if (0 == HW_PBGetState(KEY2))
            {
                EXTprintf("\r\n> ### Key 2 ###");

                u32GetTick = HAL_GetTick();

                LCD_ClearCursor(u8CursorIndex);

                switch (u8ShowMenuItem)
                {
                case 0:
                    // if(u8ShowMenuItem == 0)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 2;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 1:
                    // else if(u8ShowMenuItem == 1)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 2;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 2:
                    // else if(u8ShowMenuItem == 2)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 3:
                    // else if(u8ShowMenuItem == 3)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 4:
                    // else if(u8ShowMenuItem == 4)
                    {
                        if (szInputStrBuf[u8InputIdx] == '0')
                            szInputStrBuf[u8InputIdx] = 'F';
                        else if (szInputStrBuf[u8InputIdx] == '1')
                            szInputStrBuf[u8InputIdx] = '0';
                        else if (szInputStrBuf[u8InputIdx] == '2')
                            szInputStrBuf[u8InputIdx] = '1';
                        else if (szInputStrBuf[u8InputIdx] == '3')
                            szInputStrBuf[u8InputIdx] = '2';
                        else if (szInputStrBuf[u8InputIdx] == '4')
                            szInputStrBuf[u8InputIdx] = '3';
                        else if (szInputStrBuf[u8InputIdx] == '5')
                            szInputStrBuf[u8InputIdx] = '4';
                        else if (szInputStrBuf[u8InputIdx] == '6')
                            szInputStrBuf[u8InputIdx] = '5';
                        else if (szInputStrBuf[u8InputIdx] == '7')
                            szInputStrBuf[u8InputIdx] = '6';
                        else if (szInputStrBuf[u8InputIdx] == '8')
                            szInputStrBuf[u8InputIdx] = '7';
                        else if (szInputStrBuf[u8InputIdx] == '9')
                            szInputStrBuf[u8InputIdx] = '8';
                        else if (szInputStrBuf[u8InputIdx] == 'A')
                            szInputStrBuf[u8InputIdx] = '9';
                        else if (szInputStrBuf[u8InputIdx] == 'B')
                            szInputStrBuf[u8InputIdx] = 'A';
                        else if (szInputStrBuf[u8InputIdx] == 'C')
                            szInputStrBuf[u8InputIdx] = 'B';
                        else if (szInputStrBuf[u8InputIdx] == 'D')
                            szInputStrBuf[u8InputIdx] = 'C';
                        else if (szInputStrBuf[u8InputIdx] == 'E')
                            szInputStrBuf[u8InputIdx] = 'D';
                        else if (szInputStrBuf[u8InputIdx] == 'F')
                            szInputStrBuf[u8InputIdx] = 'E';

                        break;
                    }

                case 5:
                    // else if(u8ShowMenuItem == 5)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 2;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 6:
                    // else if(u8ShowMenuItem == 6)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 7:
                    // else if(u8ShowMenuItem == 7)
                    {
                        switch (u8InputIdx)
                        {
                        case 0:
                            // if(u8InputIdx == 0)
                            {
                                if (u8InputBCDBuf[0] <= '0')
                                    u8InputBCDBuf[0] = '9';
                                else
                                    u8InputBCDBuf[0]--;

                                break;
                            }

                        case 1:
                            // else if(u8InputIdx == 1)
                            {
                                if (u8InputBCDBuf[1] <= '0')
                                    u8InputBCDBuf[1] = '9';
                                else
                                    u8InputBCDBuf[1]--;

                                break;
                            }

                        case 3:
                            // else if(u8InputIdx == 3)
                            {
                                if (u8InputBCDBuf[2] <= '0')
                                    u8InputBCDBuf[2] = '9';
                                else
                                    u8InputBCDBuf[2]--;

                                break;
                            }

                        case 4:
                            // else if(u8InputIdx == 4)
                            {
                                if (u8InputBCDBuf[3] <= '0')
                                    u8InputBCDBuf[3] = '9';
                                else
                                    u8InputBCDBuf[3]--;

                                break;
                            }

                        case 6:
                            // else if(u8InputIdx == 6)
                            {
                                if (u8InputBCDBuf[4] <= '0')
                                    u8InputBCDBuf[4] = '9';
                                else
                                    u8InputBCDBuf[4]--;

                                break;
                            }

                        case 7:
                            // else if(u8InputIdx == 7)
                            {
                                if (u8InputBCDBuf[5] <= '0')
                                    u8InputBCDBuf[5] = '9';
                                else
                                    u8InputBCDBuf[5]--;

                                break;
                            }

                        default:
                            break;
                        }

                        break;
                    }

                case 8:
                    // else if(u8ShowMenuItem == 8)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 2;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 9:
                    // else if(u8ShowMenuItem == 9)
                    {
                        if (g_Setting.bVdrNow == FALSE)
                        {
                            if (u8CursorIndex == 1)
                                u8CursorIndex = 3;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 1;
                            else if (u8CursorIndex == 3)
                                u8CursorIndex = 2;
                        }
                        else
                        {
                            if (u8CursorIndex == 0)
                                u8CursorIndex = 3;
                            else if (u8CursorIndex == 1)
                                u8CursorIndex = 0;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 1;
                            else if (u8CursorIndex == 3)
                                u8CursorIndex = 2;
                        }

                        bMoveCursor = TRUE;

                        break;
                    }

                case 10:
                    // else if(u8ShowMenuItem == 10)
                    {
                        if (u8DLHour > 1)
                            u8DLHour--;
                        else
                            u8DLHour = 24;

                        // RTOSprintf("\r\n> ### u8DLHour: %d ###", u8DLHour);

                        break;
                    }

                case 11:
                    // else if(u8ShowMenuItem == 11)
                    {

                        switch (u8InputIdx)
                        {
                        case 0:
                            // if(u8InputIdx == 0)
                            {
                                if (u8InputBCDBuf[0] <= '0')
                                    u8InputBCDBuf[0] = '9';
                                else
                                    u8InputBCDBuf[0]--;

                                break;
                            }

                        case 1:
                            // else if(u8InputIdx == 1)
                            {
                                if (u8InputBCDBuf[1] <= '0')
                                    u8InputBCDBuf[1] = '9';
                                else
                                    u8InputBCDBuf[1]--;

                                break;
                            }

                        case 3:
                            // else if(u8InputIdx == 3)
                            {
                                if (u8InputBCDBuf[2] <= '0')
                                    u8InputBCDBuf[2] = '9';
                                else
                                    u8InputBCDBuf[2]--;

                                break;
                            }

                        case 4:
                            // else if(u8InputIdx == 4)
                            {
                                if (u8InputBCDBuf[3] <= '0')
                                    u8InputBCDBuf[3] = '9';
                                else
                                    u8InputBCDBuf[3]--;

                                break;
                            }

                        case 6:
                            // else if(u8InputIdx == 6)
                            {
                                if (u8InputBCDBuf[4] <= '0')
                                    u8InputBCDBuf[4] = '9';
                                else
                                    u8InputBCDBuf[4]--;

                                break;
                            }

                        case 7:
                            // else if(u8InputIdx == 7)
                            {
                                if (u8InputBCDBuf[5] <= '0')
                                    u8InputBCDBuf[5] = '9';
                                else
                                    u8InputBCDBuf[5]--;

                                break;
                            }

                        default:
                            break;
                        }

                        break;
                    }

                case 12:
                    // else if(u8ShowMenuItem == 12)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 2;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 13:
                    // else if(u8ShowMenuItem == 13)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 0;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 14:
                    // else if(u8ShowMenuItem == 14)
                    {
                        if (bIsEngModeFlag == TRUE)
                        {
                            if (u8CursorIndex == 0)
                                u8CursorIndex = 3;
                            else if (u8CursorIndex == 1)
                                u8CursorIndex = 0;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 1;
                            else if (u8CursorIndex == 3)
                                u8CursorIndex = 2;
                        }
                        else
                        {
                            if (u8CursorIndex == 1)
                                u8CursorIndex = 2;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 1;
                        }

                        bMoveCursor = TRUE;

                        break;
                    }

                case 15:
                    // else if(u8ShowMenuItem == 15)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);

                        if (u8InputIdx != 7)
                        {
                            if (szInputStrBuf[u8InputIdx] <= '0')
                                szInputStrBuf[u8InputIdx] = '9';
                            else
                                szInputStrBuf[u8InputIdx]--;
                        }

                        break;
                    }

                case 16:
                    // else if(u8ShowMenuItem == 16)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);
                        // if(u8CarTypeIdx > 0)
                        //{
                        //	u8CarTypeIdx--;
                        // }

                        if (u8CarTypeIdx > 0)
                        {
                            u8CarTypeIdx--;
                        }
                        else
                        {
                            u8CarTypeIdx = CARTYPE_END - 1;
                        }

                        break;
                    }

                case 17:
                    // else if(u8ShowMenuItem == 17)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);

                        break;
                    }

                case 22:
                    // else if(u8ShowMenuItem == 22)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 32:
                    // else if(u8ShowMenuItem == 32)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 33:
                    // else if(u8ShowMenuItem == 33)
                    {
                        if (u8DLHour > 1)
                            u8DLHour--;
                        else
                            u8DLHour = 24;

                        break;
                    }

                case 38:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 42:
                    // else if(u8ShowMenuItem == 42)
                    {
                        if (u8DLHour > 1)
                            u8DLHour--;
                        else
                            u8DLHour = 24;

                        // RTOSprintf("\r\n> ### u8DLHour: %d ###", u8DLHour);

                        break;
                    }

                case 44:
                {
                    SD_ReadFileToBuff(FILENAME_TECHNICAL_TMP, (uint8_t *)&stTechnical, sizeof(TechnicalDataTypeDef));

                    sprintf(szInputStrBuf, "%02X%02X/%02X/%02X", stTechnical.u8FwDateBCD[0], stTechnical.u8FwDateBCD[1],
                            stTechnical.u8FwDateBCD[2], stTechnical.u8FwDateBCD[3]);

                    sprintf(szInputStrBuf2, "%02X:%02X:%02X", stTechnical.u8FwDateBCD[4], stTechnical.u8FwDateBCD[5],
                            stTechnical.u8FwDateBCD[6]);

                    sprintf(szInputStrBuf3, "%s", g_Setting.tBusInfo.cBus_ID);

                    u8ShowMenuItem = 47;

                    break;
                }

                case 45:
                {
                    u8ShowMenuItem = 44;

                    break;
                }

                case 46:
                {
                    u8ShowMenuItem = 45;

                    break;
                }

                case 47:
                {
                    sprintf(szInputStrBuf, "%04d", VERSION_BUILD);

                    u8ShowMenuItem = 46;

                    break;
                }

                case 48:
                {
                    if (u8CursorIndex == 1)
                        u8CursorIndex = 2;
                    else if (u8CursorIndex == 2)
                        u8CursorIndex = 1;

                    break;
                }

                case 50:
                    // else if(u8ShowMenuItem == 50)
                    {
#if ENABLE_GSM_NET
#if CUSTOMER_TYPE == CUSTOMER_SCANIA
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 9;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 0;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 4;
                        else if (u8MainPageIdx == 6)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 7)
                            u8MainPageIdx = 6;
                        else if (u8MainPageIdx == 9)
                            u8MainPageIdx = 7;
#else
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 7;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 0;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 4;
                        else if (u8MainPageIdx == 6)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 7)
                            u8MainPageIdx = 6;
#endif
#else
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 0;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 4;
#endif

                        bClearPageFlag = TRUE;

                        break;

                    case 56:
                        if (u8PassWord[u8InputIdx] > 0)
                        {
                            u8PassWord[u8InputIdx]--;
                        }
                        else
                        {
                            u8PassWord[u8InputIdx] = 9;
                        }
                        break;
                    }

                case 60:
                {
                    if (u8CursorIndex == 1)
                    {
                        u8ShowMenuItem = 9;
                        u8CursorIndex = 1;
                    }

                    break;
                }

                case 61:
                {
                    switch (u8InputIdx)
                    {
                    case 0:
                        // if(u8InputIdx == 0)
                        {
                            if (u8InputBCDBuf[0] <= '0')
                                u8InputBCDBuf[0] = '9';
                            else
                                u8InputBCDBuf[0]--;

                            break;
                        }

                    case 1:
                        // else if(u8InputIdx == 1)
                        {
                            if (u8InputBCDBuf[1] <= '0')
                                u8InputBCDBuf[1] = '9';
                            else
                                u8InputBCDBuf[1]--;

                            break;
                        }

                    case 3:
                        // else if(u8InputIdx == 3)
                        {
                            if (u8InputBCDBuf[2] <= '0')
                                u8InputBCDBuf[2] = '9';
                            else
                                u8InputBCDBuf[2]--;

                            break;
                        }

                    case 4:
                        // else if(u8InputIdx == 4)
                        {
                            if (u8InputBCDBuf[3] <= '0')
                                u8InputBCDBuf[3] = '9';
                            else
                                u8InputBCDBuf[3]--;

                            break;
                        }

                    case 6:
                        // else if(u8InputIdx == 6)
                        {
                            if (u8InputBCDBuf[4] <= '0')
                                u8InputBCDBuf[4] = '9';
                            else
                                u8InputBCDBuf[4]--;

                            break;
                        }

                    case 7:
                        // else if(u8InputIdx == 7)
                        {
                            if (u8InputBCDBuf[5] <= '0')
                                u8InputBCDBuf[5] = '9';
                            else
                                u8InputBCDBuf[5]--;

                            break;
                        }

                    case 8:
                        // if(u8InputIdx == 0)
                        {
                            if (u8InputBCDBuf2[0] <= '0')
                                u8InputBCDBuf2[0] = '9';
                            else
                                u8InputBCDBuf2[0]--;

                            break;
                        }

                    case 9:
                        // else if(u8InputIdx == 1)
                        {
                            if (u8InputBCDBuf2[1] <= '0')
                                u8InputBCDBuf2[1] = '9';
                            else
                                u8InputBCDBuf2[1]--;

                            break;
                        }

                    case 11:
                        // else if(u8InputIdx == 3)
                        {
                            if (u8InputBCDBuf2[2] <= '0')
                                u8InputBCDBuf2[2] = '9';
                            else
                                u8InputBCDBuf2[2]--;

                            break;
                        }

                    case 12:
                        // else if(u8InputIdx == 4)
                        {
                            if (u8InputBCDBuf2[3] <= '0')
                                u8InputBCDBuf2[3] = '9';
                            else
                                u8InputBCDBuf2[3]--;

                            break;
                        }

                    case 14:
                        // else if(u8InputIdx == 6)
                        {
                            if (u8InputBCDBuf2[4] <= '0')
                                u8InputBCDBuf2[4] = '9';
                            else
                                u8InputBCDBuf2[4]--;

                            break;
                        }

                    case 15:
                        // else if(u8InputIdx == 7)
                        {
                            if (u8InputBCDBuf2[5] <= '0')
                                u8InputBCDBuf2[5] = '9';
                            else
                                u8InputBCDBuf2[5]--;

                            break;
                        }

                    default:
                        break;
                    }

                    break;
                }

                case 72:
                {
                    if (u8InputIdx2 > 0)
                    {
                        u8InputIdx2--;
                    }
                    else
                    {
                        u8InputIdx2 = BUSID_WORD_MAX;
                    }
                    szInputStrBuf[u8InputIdx] = u8BusIDWord[u8InputIdx2];

                    break;
                }

                case 76:
                    u8InputIdx = 0;
                    u8InputIdx2 = BUSID_WORD_MAX;
                    memset(szInputStrBuf, u8BusIDWord[u8InputIdx2], 10);
                    u8ShowMenuItem = 72;
                    break;

                case 78:
                    if (szInputStrBuf2[u8InputIdx] <= '0')
                    {
                        szInputStrBuf2[u8InputIdx] = '9';
                    }
                    else
                    {
                        szInputStrBuf2[u8InputIdx]--;
                    }
                    break;

                default:
                    break;
                }

                bReFlashMenuFlag = TRUE;

                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8CursorIndex: %d ###", u8ShowMenuItem, u8CursorIndex);
                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8MainPageIdx: %d ###", u8ShowMenuItem, u8MainPageIdx);

                break;
            }

        case 3:
            // if(0 == HW_PBGetState(KEY3))
            {
                EXTprintf("\r\n> ### Key 3 ###");

                u32GetTick = HAL_GetTick();

                LCD_ClearCursor(u8CursorIndex);

                switch (u8ShowMenuItem)
                {
                case 0:
                    // if(u8ShowMenuItem == 0)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 1:
                    // else if(u8ShowMenuItem == 1)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 2:
                    // else if(u8ShowMenuItem == 2)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 3:
                    // else if(u8ShowMenuItem == 3)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 4:
                    // else if(u8ShowMenuItem == 4)
                    {
                        if (szInputStrBuf[u8InputIdx] == '0')
                            szInputStrBuf[u8InputIdx] = '1';
                        else if (szInputStrBuf[u8InputIdx] == '1')
                            szInputStrBuf[u8InputIdx] = '2';
                        else if (szInputStrBuf[u8InputIdx] == '2')
                            szInputStrBuf[u8InputIdx] = '3';
                        else if (szInputStrBuf[u8InputIdx] == '3')
                            szInputStrBuf[u8InputIdx] = '4';
                        else if (szInputStrBuf[u8InputIdx] == '4')
                            szInputStrBuf[u8InputIdx] = '5';
                        else if (szInputStrBuf[u8InputIdx] == '5')
                            szInputStrBuf[u8InputIdx] = '6';
                        else if (szInputStrBuf[u8InputIdx] == '6')
                            szInputStrBuf[u8InputIdx] = '7';
                        else if (szInputStrBuf[u8InputIdx] == '7')
                            szInputStrBuf[u8InputIdx] = '8';
                        else if (szInputStrBuf[u8InputIdx] == '8')
                            szInputStrBuf[u8InputIdx] = '9';
                        else if (szInputStrBuf[u8InputIdx] == '9')
                            szInputStrBuf[u8InputIdx] = 'A';
                        else if (szInputStrBuf[u8InputIdx] == 'A')
                            szInputStrBuf[u8InputIdx] = 'B';
                        else if (szInputStrBuf[u8InputIdx] == 'B')
                            szInputStrBuf[u8InputIdx] = 'C';
                        else if (szInputStrBuf[u8InputIdx] == 'C')
                            szInputStrBuf[u8InputIdx] = 'D';
                        else if (szInputStrBuf[u8InputIdx] == 'D')
                            szInputStrBuf[u8InputIdx] = 'E';
                        else if (szInputStrBuf[u8InputIdx] == 'E')
                            szInputStrBuf[u8InputIdx] = 'F';
                        else if (szInputStrBuf[u8InputIdx] == 'F')
                            szInputStrBuf[u8InputIdx] = '0';

                        break;
                    }

                case 5:
                    // else if(u8ShowMenuItem == 5)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 6:
                    // else if(u8ShowMenuItem == 6)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 7:
                    // else if(u8ShowMenuItem == 7)
                    {
                        switch (u8InputIdx)
                        {
                        case 0:
                            // if(u8InputIdx == 0)
                            {
                                if (u8InputBCDBuf[0] >= '9')
                                    u8InputBCDBuf[0] = '0';
                                else
                                    u8InputBCDBuf[0]++;

                                break;
                            }

                        case 1:
                            // else if(u8InputIdx == 1)
                            {
                                if (u8InputBCDBuf[1] >= '9')
                                    u8InputBCDBuf[1] = '0';
                                else
                                    u8InputBCDBuf[1]++;

                                break;
                            }

                        case 3:
                            // else if(u8InputIdx == 3)
                            {
                                if (u8InputBCDBuf[2] >= '9')
                                    u8InputBCDBuf[2] = '0';
                                else
                                    u8InputBCDBuf[2]++;

                                break;
                            }

                        case 4:
                            // else if(u8InputIdx == 4)
                            {
                                if (u8InputBCDBuf[3] >= '9')
                                    u8InputBCDBuf[3] = '0';
                                else
                                    u8InputBCDBuf[3]++;

                                break;
                            }

                        case 6:
                            // else if(u8InputIdx == 6)
                            {
                                if (u8InputBCDBuf[4] >= '9')
                                    u8InputBCDBuf[4] = '0';
                                else
                                    u8InputBCDBuf[4]++;

                                break;
                            }

                        case 7:
                            // else if(u8InputIdx == 7)
                            {
                                if (u8InputBCDBuf[5] >= '9')
                                    u8InputBCDBuf[5] = '0';
                                else
                                    u8InputBCDBuf[5]++;

                                break;
                            }

                        default:
                            break;
                        }

                        break;
                    }

                case 8:
                    // else if(u8ShowMenuItem == 8)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 9:
                    // else if(u8ShowMenuItem == 9)
                    {
                        if (g_Setting.bVdrNow == FALSE)
                        {
                            if (u8CursorIndex == 1)
                            {
                                u8CursorIndex = 2;
                                bMoveCursor = TRUE;
                            }
                            else if (u8CursorIndex == 2)
                            {
                                u8CursorIndex = 3;
                                bMoveCursor = TRUE;
                            }
                            else if (u8CursorIndex == 3)
                            {
                                // u8CursorIndex = 1;
                                // u8ShowMenuItem = 60;
                                u8CursorIndex = 1;
                                bMoveCursor = TRUE;
                            }
                        }
                        else
                        {
                            if (u8CursorIndex == 0)
                            {
                                u8CursorIndex = 1;
                                bMoveCursor = TRUE;
                            }
                            else if (u8CursorIndex == 1)
                            {
                                u8CursorIndex = 2;
                                bMoveCursor = TRUE;
                            }
                            else if (u8CursorIndex == 2)
                            {
                                u8CursorIndex = 3;
                                bMoveCursor = TRUE;
                            }
                            else if (u8CursorIndex == 3)
                            {
                                // u8CursorIndex = 0;
                                // u8ShowMenuItem = 60;
                                u8CursorIndex = 0;
                                bMoveCursor = TRUE;
                            }
                        }

                        break;
                    }

                case 10:
                    // else if(u8ShowMenuItem == 10)
                    {
                        if (u8DLHour < 24)
                            u8DLHour++;
                        else
                            u8DLHour = 1;

                        // RTOSprintf("\r\n> ### u8DLHour: %d ###", u8DLHour);

                        break;
                    }

                case 11:
                    // else if(u8ShowMenuItem == 11)
                    {
                        switch (u8InputIdx)
                        {
                        case 0:
                            // if(u8InputIdx == 0)
                            {
                                if (u8InputBCDBuf[0] >= '9')
                                    u8InputBCDBuf[0] = '0';
                                else
                                    u8InputBCDBuf[0]++;

                                break;
                            }

                        case 1:
                            // else if(u8InputIdx == 1)
                            {
                                if (u8InputBCDBuf[1] >= '9')
                                    u8InputBCDBuf[1] = '0';
                                else
                                    u8InputBCDBuf[1]++;

                                break;
                            }

                        case 2:
                            // else if(u8InputIdx == 3)
                            {
                                if (u8InputBCDBuf[2] >= '9')
                                    u8InputBCDBuf[2] = '0';
                                else
                                    u8InputBCDBuf[2]++;

                                break;
                            }

                        case 4:
                            // else if(u8InputIdx == 4)
                            {
                                if (u8InputBCDBuf[3] >= '9')
                                    u8InputBCDBuf[3] = '0';
                                else
                                    u8InputBCDBuf[3]++;

                                break;
                            }

                        case 6:
                            // else if(u8InputIdx == 6)
                            {
                                if (u8InputBCDBuf[4] >= '9')
                                    u8InputBCDBuf[4] = '0';
                                else
                                    u8InputBCDBuf[4]++;

                                break;
                            }

                        case 7:
                            // else if(u8InputIdx == 7)
                            {
                                if (u8InputBCDBuf[5] >= '9')
                                    u8InputBCDBuf[5] = '0';
                                else
                                    u8InputBCDBuf[5]++;

                                break;
                            }

                        default:
                            break;
                        }

                        break;
                    }

                case 12:
                    // else if(u8ShowMenuItem == 12)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 3;
                        else if (u8CursorIndex == 3)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 13:
                    // else if(u8ShowMenuItem == 13)
                    {
                        if (u8CursorIndex == 0)
                            u8CursorIndex = 1;
                        else if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 0;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 14:
                    // else if(u8ShowMenuItem == 14)
                    {
                        if (bIsEngModeFlag == TRUE)
                        {
                            if (u8CursorIndex == 0)
                                u8CursorIndex = 1;
                            else if (u8CursorIndex == 1)
                                u8CursorIndex = 2;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 3;
                            else if (u8CursorIndex == 3)
                                u8CursorIndex = 0;
                        }
                        else
                        {
                            if (u8CursorIndex == 1)
                                u8CursorIndex = 2;
                            else if (u8CursorIndex == 2)
                                u8CursorIndex = 1;
                        }

                        bMoveCursor = TRUE;

                        break;
                    }

                case 15:
                    // else if(u8ShowMenuItem == 15)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);

                        if (u8InputIdx != 7)
                        {
                            if (szInputStrBuf[u8InputIdx] >= '9')
                                szInputStrBuf[u8InputIdx] = '0';
                            else
                                szInputStrBuf[u8InputIdx]++;
                        }

                        break;
                    }

                case 16:
                    // else if(u8ShowMenuItem == 16)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);
                        // if(u8CarTypeIdx < 18)
                        //{
                        //	u8CarTypeIdx++;
                        // }

                        if (u8CarTypeIdx < (CARTYPE_END - 1))
                        {
                            u8CarTypeIdx++;
                        }
                        else
                        {
                            u8CarTypeIdx = 0;
                        }

                        break;
                    }

                case 17:
                    // else if(u8ShowMenuItem == 17)
                    {
                        // RTOSprintf("\r\n> ### u8InputIdx: %d ###", u8InputIdx);

                        break;
                    }

                case 22:
                    // else if(u8ShowMenuItem == 22)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 32:
                    // else if(u8ShowMenuItem == 32)
                    {
                        if (u8CursorIndex == 1)
                            u8CursorIndex = 2;
                        else if (u8CursorIndex == 2)
                            u8CursorIndex = 1;

                        bMoveCursor = TRUE;

                        break;
                    }

                case 33:
                    // else if(u8ShowMenuItem == 33)
                    {
                        if (u8DLHour < 24)
                            u8DLHour++;
                        else
                            u8DLHour = 1;

                        break;
                    }

                case 38:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 42:
                    // else if(u8ShowMenuItem == 42)
                    {
                        if (u8DLHour < 24)
                            u8DLHour++;
                        else
                            u8DLHour = 1;

                        // RTOSprintf("\r\n> ### u8DLHour: %d ###", u8DLHour);

                        break;
                    }

                case 44:
                {
                    u8ShowMenuItem = 45;
                    break;
                }

                case 45:
                {
                    sprintf(szInputStrBuf, "%04d", VERSION_BUILD);

                    u8ShowMenuItem = 46;
                    break;
                }

                case 46:
                {
                    SD_ReadFileToBuff(FILENAME_TECHNICAL_TMP, (uint8_t *)&stTechnical, sizeof(TechnicalDataTypeDef));

                    sprintf(szInputStrBuf, "%02X%02X/%02X/%02X", stTechnical.u8FwDateBCD[0], stTechnical.u8FwDateBCD[1],
                            stTechnical.u8FwDateBCD[2], stTechnical.u8FwDateBCD[3]);

                    sprintf(szInputStrBuf2, "%02X:%02X:%02X", stTechnical.u8FwDateBCD[4], stTechnical.u8FwDateBCD[5],
                            stTechnical.u8FwDateBCD[6]);

                    sprintf(szInputStrBuf3, "%s", g_Setting.tBusInfo.cBus_ID);

                    u8ShowMenuItem = 47;
                    break;
                }

                case 47:
                {
                    u8ShowMenuItem = 44;
                    break;
                }

                case 48:
                {
                    if (u8CursorIndex == 1)
                        u8CursorIndex = 2;
                    else if (u8CursorIndex == 2)
                        u8CursorIndex = 1;

                    break;
                }

                case 50:
                    // else if(u8ShowMenuItem == 50)
                    {
#if ENABLE_GSM_NET
#if CUSTOMER_TYPE == CUSTOMER_SCANIA
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 4;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 6;
                        else if (u8MainPageIdx == 6)
                            u8MainPageIdx = 7;
                        else if (u8MainPageIdx == 7)
                            u8MainPageIdx = 9;
                        else if (u8MainPageIdx == 9)
                            u8MainPageIdx = 0;
#else
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 4;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 6;
                        else if (u8MainPageIdx == 6)
                            u8MainPageIdx = 7;
                        else if (u8MainPageIdx == 7)
                            u8MainPageIdx = 0;
#endif
#else
                        if (u8MainPageIdx == 0)
                            u8MainPageIdx = 1;
                        else if (u8MainPageIdx == 1)
                            u8MainPageIdx = 2;
                        else if (u8MainPageIdx == 2)
                            u8MainPageIdx = 3;
                        else if (u8MainPageIdx == 3)
                            u8MainPageIdx = 4;
                        else if (u8MainPageIdx == 4)
                            u8MainPageIdx = 5;
                        else if (u8MainPageIdx == 5)
                            u8MainPageIdx = 0;
#endif

                        bClearPageFlag = TRUE;

                        break;

                    case 56:
                        if (u8PassWord[u8InputIdx] < 9)
                        {
                            u8PassWord[u8InputIdx]++;
                        }
                        else
                        {
                            u8PassWord[u8InputIdx] = 0;
                        }
                        break;
                    }

                case 61:
                {
                    switch (u8InputIdx)
                    {
                    case 0:
                        // if(u8InputIdx == 0)
                        {
                            if (u8InputBCDBuf[0] >= '9')
                                u8InputBCDBuf[0] = '0';
                            else
                                u8InputBCDBuf[0]++;
                            break;
                        }

                    case 1:
                        // else if(u8InputIdx == 1)
                        {
                            if (u8InputBCDBuf[1] >= '9')
                                u8InputBCDBuf[1] = '0';
                            else
                                u8InputBCDBuf[1]++;
                            break;
                        }
                    case 3:
                        // else if(u8InputIdx == 3)
                        {
                            if (u8InputBCDBuf[2] >= '9')
                                u8InputBCDBuf[2] = '0';
                            else
                                u8InputBCDBuf[2]++;
                            break;
                        }
                    case 4:
                        // else if(u8InputIdx == 4)
                        {
                            if (u8InputBCDBuf[3] >= '9')
                                u8InputBCDBuf[3] = '0';
                            else
                                u8InputBCDBuf[3]++;
                            break;
                        }
                    case 6:
                        // else if(u8InputIdx == 6)
                        {
                            if (u8InputBCDBuf[4] >= '9')
                                u8InputBCDBuf[4] = '0';
                            else
                                u8InputBCDBuf[4]++;
                            break;
                        }
                    case 7:
                        // else if(u8InputIdx == 7)
                        {
                            if (u8InputBCDBuf[5] >= '9')
                                u8InputBCDBuf[5] = '0';
                            else
                                u8InputBCDBuf[5]++;
                            break;
                        }

                    case 8:
                        // if(u8InputIdx == 0)
                        {
                            if (u8InputBCDBuf2[0] >= '9')
                                u8InputBCDBuf2[0] = '0';
                            else
                                u8InputBCDBuf2[0]++;
                            break;
                        }

                    case 9:
                        // else if(u8InputIdx == 1)
                        {
                            if (u8InputBCDBuf2[1] >= '9')
                                u8InputBCDBuf2[1] = '0';
                            else
                                u8InputBCDBuf2[1]++;
                            break;
                        }
                    case 11:
                        // else if(u8InputIdx == 3)
                        {
                            if (u8InputBCDBuf2[2] >= '9')
                                u8InputBCDBuf2[2] = '0';
                            else
                                u8InputBCDBuf2[2]++;
                            break;
                        }
                    case 12:
                        // else if(u8InputIdx == 4)
                        {
                            if (u8InputBCDBuf2[3] >= '9')
                                u8InputBCDBuf2[3] = '0';
                            else
                                u8InputBCDBuf2[3]++;
                            break;
                        }
                    case 14:
                        // else if(u8InputIdx == 6)
                        {
                            if (u8InputBCDBuf2[4] >= '9')
                                u8InputBCDBuf2[4] = '0';
                            else
                                u8InputBCDBuf2[4]++;
                            break;
                        }
                    case 15:
                        // else if(u8InputIdx == 7)
                        {
                            if (u8InputBCDBuf2[5] >= '9')
                                u8InputBCDBuf2[5] = '0';
                            else
                                u8InputBCDBuf2[5]++;
                            break;
                        }

                    default:
                        break;
                    }

                    break;
                }

                case 72:
                {
                    if (u8InputIdx2 < BUSID_WORD_MAX)
                    {
                        u8InputIdx2++;
                    }
                    else
                    {
                        u8InputIdx2 = 0;
                    }
                    szInputStrBuf[u8InputIdx] = u8BusIDWord[u8InputIdx2];

                    break;
                }

                case 78:
                    if (szInputStrBuf2[u8InputIdx] >= '9')
                    {
                        szInputStrBuf2[u8InputIdx] = '0';
                    }
                    else
                    {
                        szInputStrBuf2[u8InputIdx]++;
                    }
                    break;

                default:
                    break;
                }

                bReFlashMenuFlag = TRUE;

                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8CursorIndex: %d ###", u8ShowMenuItem, u8CursorIndex);
                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8MainPageIdx: %d ###", u8ShowMenuItem, u8MainPageIdx);

                break;
            }

        case 4:
            // if(0 == HW_PBGetState(KEY4))
            {
                EXTprintf("\r\n> ### Key 4 ###");

                if ((u8ShowMenuItem == 26) || (u8ShowMenuItem == 39))
                    return;

                u32GetTick = HAL_GetTick();

                LCD_ClearCursor(u8CursorIndex);

                switch (u8ShowMenuItem)
                {
                case 0:
                    // if(u8ShowMenuItem == 0)
                    {
                        if (u8CursorIndex == 0)
                            u8ShowMenuItem = 1;
                        else if (u8CursorIndex == 1)
                            u8ShowMenuItem = 8;
                        else if (u8CursorIndex == 2)
                            u8ShowMenuItem = 9;
                        else if (u8CursorIndex == 3)
                            u8ShowMenuItem = 12;

                        break;
                    }

                case 1:
                    // else if(u8ShowMenuItem == 1)
                    {
                        if (u8CursorIndex == 0)
                            u8ShowMenuItem = 2;
                        else if (u8CursorIndex == 1)
                        {
                            u8ShowMenuItem = 32;

                            // u8ShowMenuItem = 4;
                            // u8InputIdx = 0;
                            // sprintf(szInputStrBuf, "%08X", g_Setting.u32DrvIdNum);
                        }
                        else if (u8CursorIndex == 2)
                            u8ShowMenuItem = 5;
                        else if (u8CursorIndex == 3)
                            u8ShowMenuItem = 22;

                        break;
                    }

                case 2:
                    // else if(u8ShowMenuItem == 2)
                    {
                        if (u8CursorIndex == 0)
                            u8SetDriverStatus = 1;
                        else if (u8CursorIndex == 2)
                            u8SetDriverStatus = 2;

                        u8ShowMenuItem = 3;

                        break;
                    }

                case 3:
                    // else if(u8ShowMenuItem == 3)
                    {
                        if (u8CursorIndex == 1)
                        {
                            if (u8SetDriverStatus == 1)
                            {
                                if (g_Setting.tDrvInfo[0].u8DrvStatus == 3)
                                    g_Setting.tDrvInfo[0].u8DrvStatus = 2;
                            }
                            else if (u8SetDriverStatus == 2)
                            {
                                if (g_Setting.tDrvInfo[1].u8DrvStatus == 3)
                                {
                                    g_Setting.tDrvInfo[1].u8DrvStatus = 2;
                                }
                            }
                        }
                        else if (u8CursorIndex == 2)
                        {
                            if (u8SetDriverStatus == 1)
                            {
                                if (g_Setting.tDrvInfo[0].u8DrvStatus == 2)
                                    g_Setting.tDrvInfo[0].u8DrvStatus = 3;
                            }
                            else if (u8SetDriverStatus == 2)
                            {
                                if (g_Setting.tDrvInfo[1].u8DrvStatus == 2)
                                    g_Setting.tDrvInfo[1].u8DrvStatus = 3;
                            }
                        }

                        u8ShowMenuItem = 0;

                        break;
                    }

                case 4:
                    // else if(u8ShowMenuItem == 4)
                    {
                        if (u8InputIdx < 7)
                            u8InputIdx++;
                        else
                        {
                            u8InputIdx = 0;

                            u8ShowMenuItem = 0;

                            u32Dist = 0;

                            for (int i = 0; i < 8; i++)
                            {
                                // RTOSprintf("\r\n> ### szInputStrBuf[%d]: %c ###", i, szInputStrBuf[i]);

                                if (szInputStrBuf[i] == '0')
                                    u32Dist = u32Dist | 0x00;
                                else if (szInputStrBuf[i] == '1')
                                    u32Dist = u32Dist | 0x01;
                                else if (szInputStrBuf[i] == '2')
                                    u32Dist = u32Dist | 0x02;
                                else if (szInputStrBuf[i] == '3')
                                    u32Dist = u32Dist | 0x03;
                                else if (szInputStrBuf[i] == '4')
                                    u32Dist = u32Dist | 0x04;
                                else if (szInputStrBuf[i] == '5')
                                    u32Dist = u32Dist | 0x05;
                                else if (szInputStrBuf[i] == '6')
                                    u32Dist = u32Dist | 0x06;
                                else if (szInputStrBuf[i] == '7')
                                    u32Dist = u32Dist | 0x07;
                                else if (szInputStrBuf[i] == '8')
                                    u32Dist = u32Dist | 0x08;
                                else if (szInputStrBuf[i] == '9')
                                    u32Dist = u32Dist | 0x09;
                                else if (szInputStrBuf[i] == 'A')
                                    u32Dist = u32Dist | 0x0A;
                                else if (szInputStrBuf[i] == 'B')
                                    u32Dist = u32Dist | 0x0B;
                                else if (szInputStrBuf[i] == 'C')
                                    u32Dist = u32Dist | 0x0C;
                                else if (szInputStrBuf[i] == 'D')
                                    u32Dist = u32Dist | 0x0D;
                                else if (szInputStrBuf[i] == 'E')
                                    u32Dist = u32Dist | 0x0E;
                                else if (szInputStrBuf[i] == 'F')
                                    u32Dist = u32Dist | 0x0F;

                                if (i < 7)
                                    u32Dist = u32Dist << 4;
                            }

                            g_Setting.tDrvInfo[u8InputIDIndex].u32DrvIdNum = u32Dist;
                            g_Setting.tDrvInfo[u8InputIDIndex].u32DrWorkTime = 0;
                            g_Setting.tDrvInfo[u8InputIDIndex].u32DrRestTime = 0;
                            g_Setting.tDrvInfo[u8InputIDIndex].dDriverDistance = 0;

                            for (int i = 0; i < 14; i++)
                            {
                                g_Setting.tDrvInfo[u8InputIDIndex].stDrvRecDay[i].u32WorkTime = 0;
                                g_Setting.tDrvInfo[u8InputIDIndex].stDrvRecDay[i].u32PauseTime = 0;
                                g_Setting.tDrvInfo[u8InputIDIndex].stDrvRecDay[i].u32StandByTime = 0;
                                g_Setting.tDrvInfo[u8InputIDIndex].stDrvRecDay[i].u32RestTime = 0;
                            }

                            if (EXTIO_IsMainPowerNormal())
                            {
                                System_SaveSetting();
                            }

                            g_Setting.tDRV_SET.ttDrvWorkStartTime = System_GetTime();

                            // RTOSprintf("\r\n> ### Indrx: %d, DrvID: %08X ###", u8InputIDIndex, u32Dist);
                        }

                        break;
                    }

                case 5:
                    // else if(u8ShowMenuItem == 5)
                    {
                        if (u8CursorIndex == 0)
                        {
                            // Show 1
                            u8ShowMenuItem = 18;
                        }
                        else if (u8CursorIndex == 1)
                        {
                            // Show 2
                            u8ShowMenuItem = 19;
                        }
                        else if (u8CursorIndex == 2)
                        {
                            // Show 3
                            u8ShowMenuItem = 20;
                        }
                        else if (u8CursorIndex == 3)
                        {
                            // Show 4
                            u8ShowMenuItem = 21;
                        }

                        break;
                    }

                case 6:
                    // else if(u8ShowMenuItem == 6)
                    {
                        if (u8CursorIndex == 1)
                        {
                            // Printer 24 hr Data
                            u8ShowMenuItem = 33;

                            u8DLHour = 1;
                        }
                        else if (u8CursorIndex == 2)
                        {
                            u8ShowMenuItem = 7;

                            ttTime = System_GetTime();

                            SecToBCDTime((uint32_t)ttTime, u8InputBCDTime);

                            u8InputBCDBuf[0] = HIGHBCDToASCII(u8InputBCDTime[1]);
                            u8InputBCDBuf[1] = LowBCDToASCII(u8InputBCDTime[1]);
                            u8InputBCDBuf[2] = HIGHBCDToASCII(u8InputBCDTime[2]);
                            u8InputBCDBuf[3] = LowBCDToASCII(u8InputBCDTime[2]);
                            u8InputBCDBuf[4] = HIGHBCDToASCII(u8InputBCDTime[3]);
                            u8InputBCDBuf[5] = LowBCDToASCII(u8InputBCDTime[3]);

                            u8InputIdx = 0;
                        }

                        break;
                    }

                case 7:
                    // else if(u8ShowMenuItem == 7)
                    {
                        if (u8InputIdx < 7)
                            u8InputIdx++;
                        else
                        {
                            u8InputIdx = 0;

                            u8ShowMenuItem = 34;

                            LCD_DisplaySubMenu(34);

                            u8InputBCDTime[0] = 0x20;
                            u8InputBCDTime[1] = ((u8InputBCDBuf[0] & 0x0F) << 4) | (u8InputBCDBuf[1] & 0x0F);
                            u8InputBCDTime[2] = ((u8InputBCDBuf[2] & 0x0F) << 4) | (u8InputBCDBuf[3] & 0x0F);
                            u8InputBCDTime[3] = ((u8InputBCDBuf[4] & 0x0F) << 4) | (u8InputBCDBuf[5] & 0x0F);
                            u8InputBCDTime[4] = 0x23;
                            u8InputBCDTime[5] = 0x59;
                            u8InputBCDTime[6] = 0x59;

                            // Debug Message
                            RTOSprintf("\r\n> ### EndSec: %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                                       u8InputBCDTime[0], u8InputBCDTime[1], u8InputBCDTime[2], u8InputBCDTime[3],
                                       u8InputBCDTime[4], u8InputBCDTime[5], u8InputBCDTime[6]);

                            // u32Dist is a Buffer value
                            u32Dist = BCDTimeToSec(u8InputBCDTime);

                            u8HourDataCnt = 24;

                            bGetSpeedRecord = TRUE;

                            while (bGetSpeedRecord)
                            {
                                PRN_GetSpeedRecord();
                            }

                            LCD_SetShowAutoTestPage(41);
                            // RTOSprintf("\r\n> ### Y: %d, M: %d, D: %d ###", u16YY, u8MM, u8DD);
                        }

                        break;
                    }

                case 8:
                    // else if(u8ShowMenuItem == 8)
                    {
                        if (u8CursorIndex == 0)
                        {
                            // Printer Table 1
                            PRN_Printing(1);
                        }
                        else if (u8CursorIndex == 1)
                        {
                            // Printer Table 2
                            // PRN_Printing(2);
                            u8DLHour = 24;
                            u8ShowMenuItem = 42;
                        }
                        else if (u8CursorIndex == 2)
                        {
                            // Printer Table 3
                            PRN_Printing(3);
                        }
                        else if (u8CursorIndex == 3)
                        {
                            // Printer Speed Graph
                            // u8ShowMenuItem = 6;
                            u8DLHour = 1;

                            u8ShowMenuItem = 33;
                        }

                        break;
                    }

                case 9:
                    // else if(u8ShowMenuItem == 9)
                    {
                        if (bSD_ReportErrFlag == FALSE)
                        {
                            if (bUSB_Ready == TRUE)
                            {
                                if (u8CursorIndex == 0)
                                {
                                    if (g_Setting.bVdrNow == TRUE)
                                    {
                                        u8ShowMenuItem = 26;

                                        V161_MakeVDRNowFile();
                                    }
                                }
                                else if (u8CursorIndex == 1)
                                {
                                    /*
                                                                    u8ShowMenuItem = 61;

                                                                    ttTime = System_GetTime();

                                                                    SecToBCDTime((uint32_t)ttTime, u8InputBCDTime);

                                                                    u8InputBCDBuf[0] = HIGHBCDToASCII(u8InputBCDTime[1]);
                                                                    u8InputBCDBuf[1] = LowBCDToASCII(u8InputBCDTime[1]);
                                                                    u8InputBCDBuf[2] = HIGHBCDToASCII(u8InputBCDTime[2]);
                                                                    u8InputBCDBuf[3] = LowBCDToASCII(u8InputBCDTime[2]);
                                                                    u8InputBCDBuf[4] = HIGHBCDToASCII(u8InputBCDTime[3]);
                                                                    u8InputBCDBuf[5] = LowBCDToASCII(u8InputBCDTime[3]);

                                                                    memcpy(u8InputBCDBuf2, u8InputBCDBuf, sizeof(u8InputBCDBuf));

                                                                    u8InputIdx  = 0;
                                    */
                                    ttTime = System_GetTime();

                                    SecToBCDTime((uint32_t)ttTime, u8InputBCDTime);

                                    u8InputBCDBuf[0] = HIGHBCDToASCII(u8InputBCDTime[1]);
                                    u8InputBCDBuf[1] = LowBCDToASCII(u8InputBCDTime[1]);
                                    u8InputBCDBuf[2] = HIGHBCDToASCII(u8InputBCDTime[2]);
                                    u8InputBCDBuf[3] = LowBCDToASCII(u8InputBCDTime[2]);
                                    u8InputBCDBuf[4] = HIGHBCDToASCII(u8InputBCDTime[3]);
                                    u8InputBCDBuf[5] = LowBCDToASCII(u8InputBCDTime[3]);

                                    u8InputIdx = 0;

                                    u8ShowMenuItem = 11;
                                }
                                else if (u8CursorIndex == 2)
                                {
                                    u8ShowMenuItem = 26;

                                    bDoVDR_HourFlag = TRUE;
                                }
                                else if (u8CursorIndex == 3)
                                {
                                    u8ShowMenuItem = 10;

                                    u8DLHour = 1;
                                }
                            }
                            else
                            {
                                u8ShowMenuItem = 74;

                                u32GetTick2 = HAL_GetTick();
                            }
                        }

                        break;
                    }

                case 10:
                    // else if(u8ShowMenuItem == 10)
                    {
                        if (bSD_ReportErrFlag == FALSE)
                        {
                            u8ShowMenuItem = 26;

                            // RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_USB_DL_HOUR);
                            V161_SaveEvent(6);

                            u8HourDataCnt = u8DLHour;

                            bDoHourVDRFlag = TRUE;
                        }

                        break;
                    }

                case 11:
                    // else if(u8ShowMenuItem == 11)
                    {
                        if (u8InputIdx < 7)
                            u8InputIdx++;
                        else
                        {
                            char cSD_FileName[50] = {0};

                            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_USB_DL_DAY);

                            u8InputIdx = 0;

                            u8ShowMenuItem = 26;

                            u16YY = 2000 + (u8InputBCDBuf[0] - 0x30) * 10 + (u8InputBCDBuf[1] - 0x30);

                            u8MM = (u8InputBCDBuf[2] - 0x30) * 10 + (u8InputBCDBuf[3] - 0x30);

                            u8DD = (u8InputBCDBuf[4] - 0x30) * 10 + (u8InputBCDBuf[5] - 0x30);

                            // RTOSprintf("\r\n> ### Y: %d, M: %d, D: %d ###", u16YY, u8MM, u8DD);

                            // memset(g_cVDRFileName, '\0', sizeof(g_cVDRFileName));

                            // sprintf(g_cVDRFileName, "D%04d%02d%02d_235959_%s.VDR", u16YY, u8MM, u8DD, g_Setting.tBusInfo.cBus_ID);

                            // g_bUSBDownloadFlg = TRUE;

                            sprintf(cSD_FileName, "%s/D%04d%02d%02d_235959_%s.VDR", FOLDERNAME_VDR, u16YY, u8MM, u8DD, g_Setting.tBusInfo.cBus_ID);

                            USB_DownloadFile(cSD_FileName, cSD_FileName);
                        }

                        break;
                    }

                case 12:
                    // else if(u8ShowMenuItem == 12)
                    {
                        if (u8CursorIndex == 0)
                            u8ShowMenuItem = 13;
                        else if (u8CursorIndex == 1)
                            u8ShowMenuItem = 14;
                        else if (u8CursorIndex == 2)
                            u8ShowMenuItem = 44;
                        // u8ShowMenuItem = 15;
                        else if (u8CursorIndex == 3)
                            u8ShowMenuItem = 48;

                        break;
                    }

                case 13:
                    // else if(u8ShowMenuItem == 13)
                    {
                        if (u8CursorIndex == 0)
                        {
                            // RTC CAL
                            u8ShowMenuItem = 23;
                        }
                        else if (u8CursorIndex == 1)
                        {
                            // ??
                            u8ShowMenuItem = 24;
                        }
                        else if (u8CursorIndex == 2)
                        {
                            // ??
                            u8ShowMenuItem = 25;
                        }

                        break;
                    }

                case 14:
                    // else if(u8ShowMenuItem == 14)
                    {
                        if (u8CursorIndex == 0)
                        {
                            u8ShowMenuItem = 58;

                            u8InputIdx = 0;
                            u8CarTypeIdx = 0;
                        }
                        else if (u8CursorIndex == 1)
                        {
                            u8ShowMenuItem = 16;

                            u8InputIdx = 0;

                            for (u8CarTypeIdx = 0; u8CarTypeIdx < CARTYPE_END; u8CarTypeIdx++)
                            {
                                if (g_Setting.tBusInfo.tCarType == u8CarTypeIndex[u8CarTypeIdx])
                                    break;
                            }
                        }
                        else if (u8CursorIndex == 2)
                        {
                            u8ShowMenuItem = 59;

                            u8InputIdx = 0;
                            u8CarTypeIdx = 0;
                        }
                        else if (u8CursorIndex == 3)
                        {
                            u8ShowMenuItem = 78;

                            sprintf(szInputStrBuf, "%05d", g_Setting.tSpeedSet.u32KGAIN_Val);
                            sprintf(szInputStrBuf2, "%05d", g_Setting.tSpeedSet.u32KGAIN_Val);

                            u8InputIdx = 0;
                        }

                        break;
                    }

                case 15:
                    // else if(u8ShowMenuItem == 15)
                    {
                        break;
                    }

                case 16:
                    // else if(u8ShowMenuItem == 16)
                    {
                        // RTOSprintf("\r\n> ### Select SpeedGain Value %d", u8SpeedGainValue[u8CarTypeIdx]);
                        u8PrePwPage = 16;
                        u8ShowMenuItem = 56;
                        u8PassWord[0] = 0;
                        u8PassWord[1] = 0;
                        u8PassWord[2] = 0;
                        u8PassWord[3] = 0;
                        u8InputIdx = 0;

                        break;
                    }

                case 17:
                    // else if(u8ShowMenuItem == 17)
                    {
                        break;
                    }

                case 22:
                    // else if(u8ShowMenuItem == 22)
                    {
                        if (M_2_KM(g_SysInfo.tVehicleState.u32Speed) == 0)
                        {
                            if (u8CursorIndex == 1)
                            {
                                if (g_Setting.u8CurrDrvID == 1)
                                {
                                    g_Setting.u8CurrDrvID = 0;

                                    g_Setting.tDRV_SET.ttDrvWorkStartTime = System_GetTime();
                                }
                            }
                            else if (u8CursorIndex == 2)
                            {
                                if (g_Setting.u8CurrDrvID == 0)
                                {
                                    g_Setting.u8CurrDrvID = 1;

                                    g_Setting.tDRV_SET.ttDrvWorkStartTime = System_GetTime();
                                }
                            }

                            LCD_SetShowErrorFlagStatus(FALSE);

                            u8ShowMenuItem = 0;
                        }
                        else
                            u8ShowMenuItem = 27;

                        break;
                    }

                case 28:
                {
                    RTOSprintf("\r\n> ### Input Key 4, u32ErrorPageTick Set Now ###");

                    u32ErrorPageTick = HAL_GetTick();

                    u8ShowMenuItem = 50;

                    break;
                }

                case 32:
                    // else if(u8ShowMenuItem == 32)
                    {
                        u8ShowMenuItem = 4;

                        u8InputIdx = 0;

                        if (u8CursorIndex == 1)
                        {
                            u8InputIDIndex = 0;
                        }
                        else if (u8CursorIndex == 2)
                        {
                            u8InputIDIndex = 1;
                        }

                        sprintf(szInputStrBuf, "%08X", g_Setting.tDrvInfo[u8InputIDIndex].u32DrvIdNum);

                        break;
                    }

                case 33:
                    // else if(u8ShowMenuItem == 33)
                    {
                        u8ShowMenuItem = 34;

                        LCD_DisplaySubMenu(34);

                        u8HourDataCnt = u8DLHour;

                        bGetSpeedRecord = TRUE;

                        while (bGetSpeedRecord)
                        {
                            PRN_GetSpeedRecord();
                        }

                        break;
                    }

                case 38:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 42:
                    // else if(u8ShowMenuItem == 42)
                    {
                        u8ShowMenuItem = 29;

                        u8HourDataCnt = u8DLHour;

                        PRN_Printing(2);

                        break;
                    }

                case 48:
                {
                    if (u8CursorIndex == 1)
                    {
                        HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_RESET);

                        bIsBacklightFlag = FALSE;
                    }
                    else if (u8CursorIndex == 2)
                    {
                        HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_SET);

                        bIsBacklightFlag = TRUE;
                    }

                    u8ShowMenuItem = 12;

                    break;
                }

                case 49:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

#if CUSTOMER == CUSTOMER_SCANIA
                case 50:
                {
                    if (u8MainPageIdx == 9)
                    {
                        if (u8SelectLang == LANG_CHN)
                        {
                            u8SelectLang = LANG_ENG;
                        }
                        else if (u8SelectLang == LANG_ENG)
                        {
                            u8SelectLang = LANG_CHN;
                        }
                    }

                    u8MainPageIdx = 0;

                    break;
                }
#endif

                case 51:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 52:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 55:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 56:
                {
                    if (u8InputIdx < 3)
                    {
                        u8InputIdx++;
                    }
                    else
                    {
                        if ((u8PassWordSet[0] == u8PassWord[0]) && (u8PassWordSet[1] == u8PassWord[1]) &&
                            (u8PassWordSet[2] == u8PassWord[2]) && (u8PassWordSet[3] == u8PassWord[3]))
                        {
                            // RTOSprintf("\r\n> ### Set Speed Gain OK, Set Value: %d ###", u16CarTypeGain[u8CarTypeIdx]);

                            // g_Setting.tBusInfo.tCarType = u8CarTypeIdx;

                            // g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;
                            // g_SysInfo.tVehicleState.u32DistCount = 0;
                            // g_Setting.tSpeedSet.fGain = u16CarTypeGain[u8CarTypeIdx];

                            // if(bSD_ReportErrFlag == FALSE)
                            //	V161_SaveEvent(3);

                            switch (u8PrePwPage)
                            {
                            case 16:
                                sprintf(szInputStrBuf, "$AS+CAR TYPE=%d\r\n", u8CarTypeIndex[u8CarTypeIdx]);

                                Protocl_ChkCmdPkg(szInputStrBuf, NULL);
                                break;

                            case 78:

                                sprintf(szInputStrBuf, "$AS+KGAIN=%d\r\n", u16KGain);

                                Protocl_ChkCmdPkg(szInputStrBuf, NULL);

#if ENABLE_GSM_NET
                                GSM_TCP_ReportEvent((char *)EVENT_KGAIN_CHANGE, STR_NULL);
#endif
                                break;
                            }

                            u8ShowMenuItem = 50;
                        }
                        else
                        {
                            u8ShowMenuItem = 57;
                        }
                    }

                    break;
                }

                case 57:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 60:
                {
                    /*
                                            u8ShowMenuItem = 61;

                                            ttTime = System_GetTime();

                                            SecToBCDTime((uint32_t)ttTime, u8InputBCDTime);

                                            u8InputBCDBuf[0] = HIGHBCDToASCII(u8InputBCDTime[1]);
                                            u8InputBCDBuf[1] = LowBCDToASCII(u8InputBCDTime[1]);
                                            u8InputBCDBuf[2] = HIGHBCDToASCII(u8InputBCDTime[2]);
                                            u8InputBCDBuf[3] = LowBCDToASCII(u8InputBCDTime[2]);
                                            u8InputBCDBuf[4] = HIGHBCDToASCII(u8InputBCDTime[3]);
                                            u8InputBCDBuf[5] = LowBCDToASCII(u8InputBCDTime[3]);

                                            memcpy(u8InputBCDBuf2, u8InputBCDBuf, sizeof(u8InputBCDBuf));

                                            u8InputIdx  = 0;
                    */
                    u8ShowMenuItem = 10;

                    u8DLHour = 24;

                    break;
                }

                case 61:
                {
                    if (u8InputIdx < 15)
                    {
                        u8InputIdx++;
                    }
                    else
                    {
                        u8InputIdx = 0;

                        u8InputBCDTime[0] = 0x20;
                        u8InputBCDTime[1] = ((u8InputBCDBuf[0] & 0x0F) << 4) | (u8InputBCDBuf[1] & 0x0F);
                        u8InputBCDTime[2] = ((u8InputBCDBuf[2] & 0x0F) << 4) | (u8InputBCDBuf[3] & 0x0F);
                        u8InputBCDTime[3] = ((u8InputBCDBuf[4] & 0x0F) << 4) | (u8InputBCDBuf[5] & 0x0F);
                        u8InputBCDTime[4] = 0x23;
                        u8InputBCDTime[5] = 0x59;
                        u8InputBCDTime[6] = 0x59;

                        u8InputBCDTime2[0] = 0x20;
                        u8InputBCDTime2[1] = ((u8InputBCDBuf2[0] & 0x0F) << 4) | (u8InputBCDBuf2[1] & 0x0F);
                        u8InputBCDTime2[2] = ((u8InputBCDBuf2[2] & 0x0F) << 4) | (u8InputBCDBuf2[3] & 0x0F);
                        u8InputBCDTime2[3] = ((u8InputBCDBuf2[4] & 0x0F) << 4) | (u8InputBCDBuf2[5] & 0x0F);
                        u8InputBCDTime2[4] = 0x23;
                        u8InputBCDTime2[5] = 0x59;
                        u8InputBCDTime2[6] = 0x59;

                        // Debug Message
                        // RTOSprintf("\r\n> ### Start Date: %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                        //							u8InputBCDTime[0], u8InputBCDTime[1], u8InputBCDTime[2], u8InputBCDTime[3],
                        //							u8InputBCDTime[4], u8InputBCDTime[5], u8InputBCDTime[6]);

                        // RTOSprintf("\r\n> ### End Date: %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                        //							u8InputBCDTime2[0], u8InputBCDTime2[1], u8InputBCDTime2[2], u8InputBCDTime2[3],
                        //							u8InputBCDTime2[4], u8InputBCDTime2[5], u8InputBCDTime2[6]);

                        ttSBuf = BCDTimeToSec(u8InputBCDTime);
                        ttEBuf = BCDTimeToSec(u8InputBCDTime2);
                        ttMBuf = ttSBuf;

                        do
                        {
                            memset(u8InputBCDTime, 0x00, sizeof(u8InputBCDTime));
                            memset(cDLFileName, 0x00, sizeof(cDLFileName));
                            SecToBCDTime(ttMBuf, u8InputBCDTime);

                            sprintf(cDLFileName, "%s/D%02X%02X%02X%02X_%02X%02X%02X_%s.VDR",
                                    FOLDERNAME_VDR, u8InputBCDTime[0], u8InputBCDTime[1],
                                    u8InputBCDTime[2], u8InputBCDTime[3], u8InputBCDTime[4],
                                    u8InputBCDTime[5], u8InputBCDTime[6], g_Setting.tBusInfo.cBus_ID);

                            bDLRes = SD_CheckFileExist(cDLFileName);

                            RTOSprintf("\r\n> ### Find: %s, Res: %s ###", cDLFileName, (bDLRes == TRUE ? "OK" : "FAIL"));

                            if (bDLRes == TRUE)
                            {
                                USB_DownloadFile(cDLFileName, cDLFileName);
                            }

                            ttMBuf = ttMBuf + DAY_2_SECOND(1);

                        } while (ttMBuf <= ttEBuf);
                    }

                    break;
                }

                case 63:
                {
                    u8ShowMenuItem = 50;

                    break;
                }

                case 71:
                {
                    u8InputIdx = 0;
                    u8InputIdx2 = BUSID_WORD_MAX;
                    memset(szInputStrBuf, u8BusIDWord[u8InputIdx2], 10);
                    u8ShowMenuItem = 72;

                    break;
                }

                case 72:
                {
                    if (u8InputIdx < 9)
                    {
                        u8InputIdx++;
                        u8InputIdx2 = BUSID_WORD_MAX;
                    }
                    else
                    {
                        u8ShowMenuItem = 76;

                        memcpy(szInputStrBuf2, szInputStrBuf, sizeof(szInputStrBuf));

                        for (int i = 10; i > 0; i--)
                        {
                            if (szInputStrBuf2[i - 1] == 0x2D)
                            {
                                szInputStrBuf2[i - 1] = 0x20;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                    break;
                }

                case 76:
                {
                    // u8ShowMenuItem = 50;
                    // RTOSprintf("\r\n> ### BusID: %c %c %c %c %c %c %c %c %c %c ###",
                    //		szInputStrBuf[0], szInputStrBuf[1], szInputStrBuf[2], szInputStrBuf[3], szInputStrBuf[4],
                    //		szInputStrBuf[5], szInputStrBuf[6], szInputStrBuf[7], szInputStrBuf[8], szInputStrBuf[9]
                    //);

                    for (int i = 10; i > 0; i--)
                    {
                        if (szInputStrBuf[i - 1] == 0x2D)
                        {
                            szInputStrBuf[i - 1] = 0x00;
                        }
                        else
                        {
                            break;
                        }
                    }

                    // RTOSprintf("\r\n> ### BusID: %c %c %c %c %c %c %c %c %c %c ###",
                    //		szInputStrBuf[0], szInputStrBuf[1], szInputStrBuf[2], szInputStrBuf[3], szInputStrBuf[4],
                    //		szInputStrBuf[5], szInputStrBuf[6], szInputStrBuf[7], szInputStrBuf[8], szInputStrBuf[9]
                    //);

                    bool bIsBusIdSetFlag = TRUE;
                    uint8_t u8ChkCnt = 0, u8SpaceCnt = 0;

                    for (int i = 0; i < 10; i++)
                    {
                        if (szInputStrBuf[i] != 0x00)
                        {
                            if (szInputStrBuf[i] == 0x2D)
                            {
                                u8ChkCnt++;

                                if (u8ChkCnt >= 2)
                                {
                                    bIsBusIdSetFlag = FALSE;
                                    break;
                                }

                                if ((i < 2) || (i >= 5))
                                {
                                    bIsBusIdSetFlag = FALSE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if (i == 0)
                                bIsBusIdSetFlag = FALSE;

                            break;
                        }
                    }

                    for (int i = 0; i < 10; i++)
                    {
                        if (szInputStrBuf[i] == 0x2D)
                            u8SpaceCnt++;
                    }

                    if (u8SpaceCnt != 1)
                    {
                        bIsBusIdSetFlag = FALSE;
                    }

                    if (bIsBusIdSetFlag == FALSE)
                    {
                        RTOSprintf("\r\n> ### BusID input error. ###");

                        u8ShowMenuItem = 73;
                        u32GetTick2 = HAL_GetTick();
                    }
                    else
                    {
                        u8ShowMenuItem = 77;

                        char cCmdBuf[50] = {0};
                        memset(cCmdBuf, 0x00, sizeof(cCmdBuf));
                        sprintf(cCmdBuf, "$AS+BUS ID=%s", szInputStrBuf);
                        RTOSprintf("\r\n> ### %s ###", cCmdBuf);
                        Protocl_ChkCmdPkg(cCmdBuf, NULL);

                        u32GetTick2 = HAL_GetTick();
                    }

                    break;
                }

                case 78:
                    if (u8InputIdx < 4)
                    {
                        u8InputIdx++;
                    }
                    else
                    {
                        u16KGain = ((szInputStrBuf2[0] - 0x30) * 10000) + ((szInputStrBuf2[1] - 0x30) * 1000) +
                                   ((szInputStrBuf2[2] - 0x30) * 100) + ((szInputStrBuf2[3] - 0x30) * 10) +
                                   (szInputStrBuf2[4] - 0x30);

                        RTOSprintf("\r\n> ### Input K:%d ###", u16KGain);

                        u8PrePwPage = 78;
                        u8PassWord[0] = 0;
                        u8PassWord[1] = 0;
                        u8PassWord[2] = 0;
                        u8PassWord[3] = 0;
                        u8InputIdx = 0;
                        u8ShowMenuItem = 56;
                    }
                    break;

                default:
                    break;
                }

                bReFlashMenuFlag = TRUE;

                if (u8ShowMenuItem == 9)
                {
                    if (g_Setting.bVdrNow == FALSE)
                        u8CursorIndex = 1;
                    else
                        u8CursorIndex = 0;
                }
                else if (u8ShowMenuItem == 14)
                    u8CursorIndex = 1;
                else if (u8ShowMenuItem == 22)
                    u8CursorIndex = 1;
                else if (u8ShowMenuItem == 32)
                    u8CursorIndex = 1;
                else if (u8ShowMenuItem == 3)
                    u8CursorIndex = 1;
                else if (u8ShowMenuItem == 6)
                    u8CursorIndex = 1;
                else if (u8ShowMenuItem == 48)
                    u8CursorIndex = 1;
                else
                    u8CursorIndex = 0;

                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8CursorIndex: %d ###", u8ShowMenuItem, u8CursorIndex);

                break;
            }

        case 5:
            // if(0 == HW_PBGetState(KEY5))
            {
                EXTprintf("\r\n> ### Key 5 ###");

                //======================================
                // Test VDR file make
                // bDoVDR_DayFlag = TRUE;
                //======================================
                // Test hour data file make
                // Choose hour
                // u8HourDataCnt = 4;

                // Make RS232 dl file
                // bHourDataFlag = TRUE;

                // bDLRS232Flag = FALSE;

                // Test hour VDR file make
                // bDoHourVDRFlag = TRUE;
                //======================================//
                // USB_DownloadFile((char *)"SD_USB_Test3.tmp", (char *)"SD_USB_Test3.tmp");

                // HAL_Delay(200);

                // return;

                if ((u8ShowMenuItem == 26) || (u8ShowMenuItem == 39) || (u8ShowMenuItem == 71) ||
                    (u8ShowMenuItem == 72) || (u8ShowMenuItem == 73) || (u8ShowMenuItem == 76) ||
                    (u8ShowMenuItem == 77))
                    return;

                u32GetTick = HAL_GetTick();

                if (u8ShowMenuItem != 6)
                {
                    LCD_ClearCursor(u8CursorIndex);

                    u8ShowMenuItem = 6;

                    u8CursorIndex = 1;

                    bReFlashMenuFlag = TRUE;
                }

                // RTOSprintf("\r\n> ### u8ShowMenuItem: %d, u8CursorIndex: %d ###", u8ShowMenuItem, u8CursorIndex);

                break;
            }

        default:
            break;
        }

        if ((u8PressSignal > 0) && (u8PressSignal < 7))
        {
            if (u8ShowMenuItem == 27)
            {
                if (u32WarningTick == 0)
                    u32WarningTick = HAL_GetTick();
                else
                {
                    if (HAL_GetTick() >= (u32WarningTick + (mS_2_SysTick(S_2_mS(3)))))
                    {
                        u32WarningTick = 0;

                        LCD_ClearCursor(u8CursorIndex);

                        u8ShowMenuItem = 22;

                        u8CursorIndex = 1;

                        bReFlashMenuFlag = TRUE;
                    }
                }
            }

            if (bReFlashMenuFlag == TRUE)
            {
                if (bMoveCursor == FALSE)
                {
                    LCD_DisplaySubMenu(u8ShowMenuItem);
                }

                if ((u8ShowMenuItem != 4) && (u8ShowMenuItem != 7) && (u8ShowMenuItem != 10) &&
                    (u8ShowMenuItem != 11) && (u8ShowMenuItem != 15) && (u8ShowMenuItem != 16) &&
                    (u8ShowMenuItem != 17) && (u8ShowMenuItem != 18) && (u8ShowMenuItem != 19) &&
                    (u8ShowMenuItem != 20) && (u8ShowMenuItem != 21) && (u8ShowMenuItem != 23) &&
                    (u8ShowMenuItem != 24) && (u8ShowMenuItem != 25) && (u8ShowMenuItem != 26) &&
                    (u8ShowMenuItem != 27) && (u8ShowMenuItem != 28) && (u8ShowMenuItem != 29) &&
                    (u8ShowMenuItem != 30) && (u8ShowMenuItem != 31) && (u8ShowMenuItem != 33) &&
                    (u8ShowMenuItem != 34) && (u8ShowMenuItem != 35) && (u8ShowMenuItem != 36) &&
                    (u8ShowMenuItem != 37) && (u8ShowMenuItem != 38) && (u8ShowMenuItem != 39) &&
                    (u8ShowMenuItem != 40) && (u8ShowMenuItem != 41) && (u8ShowMenuItem != 42) &&
                    (u8ShowMenuItem != 43) && (u8ShowMenuItem != 44) && (u8ShowMenuItem != 45) &&
                    (u8ShowMenuItem != 46) && (u8ShowMenuItem != 47) && (u8ShowMenuItem != 49) &&
                    (u8ShowMenuItem != 50) && (u8ShowMenuItem != 51) && (u8ShowMenuItem != 52) &&
                    (u8ShowMenuItem != 53) && (u8ShowMenuItem != 54) && (u8ShowMenuItem != 55) &&
                    (u8ShowMenuItem != 56) && (u8ShowMenuItem != 57) && (u8ShowMenuItem != 58) &&
                    (u8ShowMenuItem != 59) && (u8ShowMenuItem != 61) && (u8ShowMenuItem != 63) &&
                    (u8ShowMenuItem != 64) && (u8ShowMenuItem != 65) && (u8ShowMenuItem != 66) &&
                    (u8ShowMenuItem != 67) && (u8ShowMenuItem != 68) && (u8ShowMenuItem != 69) &&
                    (u8ShowMenuItem != 70) && (u8ShowMenuItem != 71) && (u8ShowMenuItem != 72) &&
                    (u8ShowMenuItem != 73) && (u8ShowMenuItem != 74) && (u8ShowMenuItem != 75) &&
                    (u8ShowMenuItem != 76) && (u8ShowMenuItem != 77) && (u8ShowMenuItem != 78) &&
                    (u8ShowMenuItem != 90) && (u8ShowMenuItem != 91) && (u8ShowMenuItem != 92) &&
                    (u8ShowMenuItem != 93) && (u8ShowMenuItem != 94))
                {
                    LCD_ShowCursor(u8CursorIndex);
                }

                bReFlashMenuFlag = FALSE;
            }
        }

        HAL_Delay(200);
    }
}

//============================================================================//
// @brief  Handle the LCD task.
// @param  None.
// @retval None.
//============================================================================//
void LCD_Key_Process(void)
{
    if ((bLCD_Stop == TRUE) || (bLCD_Running == FALSE))
        return;

    if (bChangPageFlag == TRUE)
    {
        LCD_ScreenClear();

        u8ShowMenuItem = u8ChangPageNum;

        u8LastPageNum = u8ShowMenuItem;

        u8CursorIndex = 0;

        bReFlashMenuFlag = TRUE;

        bChangPageFlag = FALSE;
    }

    taskInfo[TASK_LCD_KEY].u16TaskStep++;

    if (u8ShowMenuItem == 30)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_AUTO_TEST);
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_AUTO_TEST);
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_PTR_AUTO_TEST);

            LCD_SetShowAutoTestPage(31);
        }
    }
    else if (u8ShowMenuItem == 31)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            // GPS
            if (bGPS_SystembootTestFlag == TRUE)
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_AUTO_TEST_OK);

                g_u8HwInfo = (g_u8HwInfo | 0x04);
            }
            else
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_NG);

                g_u8HwInfo = (g_u8HwInfo & 0xFB);
            }

            /// Monitor
            if (HAL_GPIO_ReadPin(LCD_DET_GPIO_Port, LCD_DET_Pin) == 1)
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_AUTO_TEST_OK);

                g_u8HwInfo = (g_u8HwInfo | 0x02);
            }
            else
            {
                g_u8HwInfo = (g_u8HwInfo & 0xFD);

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_NG);
            }

            /// Printer
            if (HAL_GPIO_ReadPin(P_EMPTY_GPIO_Port, P_EMPTY_Pin) == 1)
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_PTR_AUTO_TEST_OK);

                g_u8HwInfo = (g_u8HwInfo | 0x01);
            }
            else
            {
                g_u8HwInfo = (g_u8HwInfo & 0xFE);

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_PTR_NG);
            }

            RTOSprintf("\r\n> HW-Info: 0x%02X", g_u8HwInfo);

            if ((bUSB_PrepData == TRUE) || (procUSB_TransStep != PROC_STEP_NULL))
            {
                LCD_SetShowAutoTestPage(26);
            }
            else if (u8PStep != PRT_IDLE)
            {
                LCD_SetShowAutoTestPage(29);
            }
            else
            {
                if ((bPreWoringFlag == TRUE) && (bShowErrorFlag == FALSE))
                {
                    LCD_SetShowAutoTestPage(49);
                }
                else if (bShowErrorFlag == TRUE)
                {
                    LCD_SetShowAutoTestPage(28);
                }
                else
                {
                    LCD_SetShowAutoTestPage(50);

                    // if(ChkDashCharacter() == FALSE)
                    //{
                    //	LCD_SetShowAutoTestPage(71);
                    // }
                    // else
                    //{
                    //	LCD_SetShowAutoTestPage(50);
                    // }
                }
            }
        }
    }
    // else if(u8ShowMenuItem == 35)
    //{
    //	if(HAL_GetTick() >= (u32GetTick2 + S_2_mS(10)))
    //	{
    //		LCD_SetShowAutoTestPage(36);
    //	}
    // }
    else if (u8ShowMenuItem == 36)
    {
        u8ShowMenuItem = 50;

        bClearPageFlag = TRUE;

        // if(HAL_GPIO_ReadPin(P_EMPTY_GPIO_Port, P_EMPTY_Pin) == 1)
        //{
        //	u8ShowMenuItem = 50;
        //	bClearPageFlag = TRUE;
        // }

        // if(HAL_GetTick() >= (u32GetTick2 + S_2_mS(30)))
        //{
        //	u8ShowMenuItem = 50;
        //	bClearPageFlag = TRUE;
        // }
    }
    else if (u8ShowMenuItem == 37)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(10)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 28)
    {
        if (bShowErrorFlag == FALSE)
        {
            if (bPreWoringFlag == TRUE)
            {
                u8ShowMenuItem = 49;
            }
            else
            {
                u8ShowMenuItem = 50;
            }

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 29)
    {
    }
    else if (u8ShowMenuItem == 38)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(600)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }

        if (M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) >= 3)
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 39)
    {
        // if(HAL_GetTick() >= (u32GetTick2 + S_2_mS(20)))
        //{
        //	HAL_GPIO_WritePin(N_LCD_BL_GPIO_Port, N_LCD_BL_Pin, GPIO_PIN_SET);
        //	u8ShowMenuItem = 50;
        //   bClearPageFlag = TRUE;
        // }
    }
    else if (u8ShowMenuItem == 41)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 43)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(5)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 49)
    {
        if (bShowErrorFlag == TRUE)
        {
            u8ShowMenuItem = 28;

            bClearPageFlag = TRUE;
        }
        else
        {
            if (bPreWoringFlag == FALSE)
            {
                u8ShowMenuItem = 50;

                bClearPageFlag = TRUE;
            }
        }
    }
    else if (u8ShowMenuItem == 51)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(30)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 52)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(30)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 55)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(30)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 63)
    {
    }
    else if (u8ShowMenuItem == 64)
    {
    }
    else if (u8ShowMenuItem == 65)
    {
    }
    else if (u8ShowMenuItem == 66)
    {
    }
    else if (u8ShowMenuItem == 67)
    {
    }
    else if (u8ShowMenuItem == 68)
    {
    }
    else if (u8ShowMenuItem == 69)
    {
    }
    else if (u8ShowMenuItem == 70)
    {
    }
    else if (u8ShowMenuItem == 71)
    {
    }
    else if (u8ShowMenuItem == 72)
    {
    }
    else if (u8ShowMenuItem == 73)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8InputIdx = 0;
            u8InputIdx2 = BUSID_WORD_MAX;
            memset(szInputStrBuf, u8BusIDWord[u8InputIdx2], 10);
            u8ShowMenuItem = 72;

            bClearPageFlag = TRUE;
            bReFlashMenuFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 73)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;
        }
    }
    else if (u8ShowMenuItem == 90)
    {
    }
    else if (u8ShowMenuItem == 91)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 92)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 93)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 94)
    {
        if (HAL_GetTick() >= (u32GetTick2 + S_2_mS(3)))
        {
            u8ShowMenuItem = 50;

            bClearPageFlag = TRUE;
        }
    }
    else if (u8ShowMenuItem == 50)
    {
        LCD_DisplayMainMenu();
    }
    else
    {
        if (HAL_GetTick() >= (u32GetTick + 20000))
        {
            if ((u8ShowMenuItem != 26) && (u8ShowMenuItem != 29))
            {
                u8ShowMenuItem = 50;

                u8MainPageIdx = 0;

                bClearPageFlag = TRUE;
            }
            /*
            if(bShowErrorFlag == FALSE)
            {
                if((u8ShowMenuItem != 26) && (u8ShowMenuItem != 29))
                {
                    u8ShowMenuItem = 50;

                    bClearPageFlag = TRUE;
                }
            }
            else
            {
                if(u8ShowMenuItem != 28)
                {
                    u8ShowMenuItem = 28;

                    bReFlashMenuFlag = TRUE;
                }
            }
            */
        }
    }

    taskInfo[TASK_LCD_KEY].u16TaskStep++;

    // if(EXTIO_IsMainPowerNormal() == FALSE)
    //	return;

    Key_Scan();

    // g_SysInfo.u8VdrInfo = g_SysInfo.u8VdrInfo | 0x02;

    // if(bUSB_TransFinish == TRUE)
    //{
    //	LCD_SetShowAutoTestPage(38);

    //	bUSB_TransFinish = FALSE;
    //}
}

//============================================================================//
// @brief  Initialze the LCD.
// @param  None.
// @retval None.
//============================================================================//
void LCD_Init(void) // ����Ʀb�Ҧ�LCD�ާ@���e�ݭn������
{
    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_AUTO_TEST); // 0X54

    BackLightEnable;

    LCDRSTLow;

    Delay_mS(10);

    LCDRSTHigh;

    LCD_ScreenInitial();

    LCD_ScreenClear();

    u8ShowMenuItem = 50;
    /*
    u8ShowMenuItem = 0;
    u8CursorIndex = 0;
    LCD_DisplaySubMenu(u8ShowMenuItem);
    LCD_ShowCursor(u8CursorIndex);
    bReFlashMenuFlag = false;
    */

    if (HW_PBGetState(BAT_DETECT))
    {
        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_AUTO_TEST_OK); // 0X55
    }
    else
    {
        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_LCM_NG);
    }

    bLCD_Running = TRUE;
}

//============================================================================//
// @brief  Stop LCD.
// @param  None.
// @retval None.
//============================================================================//
void LCD_Stop(void)
{
    LCD_ScreenClear();

    BackLightDisable;

    bLCD_Running = FALSE;
}

//============================================================================//
// @brief  Switch LCD on/off.
// @param  None.
// @retval None.
//============================================================================//
void LCD_SwitchHandle(void)
{
    if (bLCD_TurnOn == TRUE)
    {
        bLCD_TurnOn = FALSE;

        LCD_Init();

        LCD_SetShowAutoTestPage(30);

        HAL_Delay(100);
    }

    if (bLCD_TurnOff == TRUE)
    {
        bLCD_TurnOff = FALSE;

        LCD_Stop();

        HAL_Delay(100);
    }
}

//============================================================================//
// @brief  Get the current value of the bShowErrorFlag.
// @param  None.
// @retval Value of bShowErrorFlag.
//============================================================================//
bool LCD_GetShowErrorFlagStatus(void)
{
    return bShowErrorFlag;
}

//============================================================================//
// @brief  Set value for bShowErrorFlag.
// @param  bSet: Value to set for bShowErrorFlag.
// @retval None.
//============================================================================//
void LCD_SetShowErrorFlagStatus(bool bSet)
{
    bShowErrorFlag = bSet;
}

//============================================================================//
// @brief  Show a page.
// @param  u8Page: Number of the page to display.
// @retval None.
//============================================================================//
void LCD_SetShowAutoTestPage(uint8_t u8Page)
{
    if ((u8ShowMenuItem == 39) && (bWaitingEnterStandby == TRUE))
        return;

    // RTOSprintf("\r\n> ### SetShowAutoTestPage: %d ###", u8Page);

    bChangPageFlag = TRUE;

    u8ChangPageNum = u8Page;

    u32GetTick2 = HAL_GetTick();
}
