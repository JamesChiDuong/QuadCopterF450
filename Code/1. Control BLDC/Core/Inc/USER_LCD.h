/*
 * USER_LCD.h
 *
 *  ILI9341 240x320 TFT of the STM32F429I-DISC1, driven over SPI5 ONLY
 *  (no LTDC / no SDRAM). The LTDC + SDRAM pins of the board are already used
 *  by the motors and the MPU6050 in this project, so the controller's
 *  internal GRAM is written directly through its serial interface.
 *
 *  Pins (fixed by the board):
 *      PF7  SPI5_SCK
 *      PF9  SPI5_MOSI (ILI9341 SDA)
 *      PC2  LCD CSX   (chip select, active low)
 *      PD13 LCD WRX   (D/CX: 0 = command, 1 = data)
 *
 *  SPI5 is on APB2 (4.5 MHz with the current clock tree) -> SCK 2.25 MHz,
 *  about 0.55 s for a full screen. Draw only what changes.
 *
 *  Not thread safe: call from one task only (the UI task).
 */
/*
 * OVERVIEW
 *  - Small drawing API for the on-board 240 x 320 LCD: filled rectangles,
 *    lines, outlines, 8x8 text (scalable). No frame buffer: every call is
 *    sent straight to the ILI9341 over SPI5.
 *  - Coordinates: (0,0) = top-left, x to the right (0..239), y down (0..319)
 *    with the default LCD_MADCTL_VALUE.
 *  - Colours are RGB565 (16 bit): rrrrrggg gggbbbbb.
 *    LCD_RGB(r, g, b) converts 8-bit-per-channel values, e.g.
 *    LCD_RGB(255, 128, 0) = (0xF8 << 8) | (0x80 << 3) | (0 >> 3) = 0xFC00.
 *  - Used only by USER_UI.c (UI task, 10 Hz).
 */

#ifndef INC_USER_LCD_H_
#define INC_USER_LCD_H_

#include <stdint.h>
#include <stdbool.h>

#define LCD_WIDTH   240
#define LCD_HEIGHT  320

/* RGB565 colours */
#define LCD_RGB(r, g, b) ((uint16_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)))
#define LCD_BLACK    0x0000
#define LCD_WHITE    0xFFFF
#define LCD_RED      0xF800
#define LCD_GREEN    0x07E0
#define LCD_BLUE     0x001F
#define LCD_YELLOW   0xFFE0
#define LCD_CYAN     0x07FF
#define LCD_MAGENTA  0xF81F
#define LCD_ORANGE   0xFD20
#define LCD_GRAY     0x8410
#define LCD_DARKGRAY 0x39E7
#define LCD_NAVY     0x000F

void LCD_Init(void);
void LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void LCD_FillScreen(uint16_t color);
void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color);
void LCD_DrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void LCD_DrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void LCD_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

/* 8x8 font, scaled by 'scale' (1 = 8x8 px, 2 = 16x16 px). bg is always painted. */
void LCD_DrawChar(int16_t x, int16_t y, char c, uint16_t fg, uint16_t bg, uint8_t scale);
void LCD_DrawString(int16_t x, int16_t y, const char *s, uint16_t fg, uint16_t bg, uint8_t scale);

#endif /* INC_USER_LCD_H_ */
