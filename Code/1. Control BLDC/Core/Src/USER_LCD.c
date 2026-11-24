/*
 * USER_LCD.c
 *
 *  ILI9341 over SPI5 (register level, TX only / bidirectional mode).
 *  See USER_LCD.h for the wiring and limitations.
 */
/*
 * ============================================================================
 *  OVERVIEW - minimal ILI9341 driver over SPI5 (write only)
 * ============================================================================
 *  1. Why SPI only?
 *     The DISC1 board normally drives this LCD with LTDC (parallel RGB) from
 *     a frame buffer in SDRAM. Those pins are used here by the motors
 *     (PB0, PB10, PA6, PC6, PE12, PE14, PG15) and the MPU6050 (PF0, PF1,
 *     PF4, PF5), so instead we talk to the ILI9341 controller through its
 *     serial interface and write straight into its internal frame memory
 *     (GRAM, 240 x 320 pixels). No frame buffer in the MCU.
 *
 *  2. Hardware
 *       PF7  SPI5_SCK
 *       PF9  SPI5_MOSI (the ILI9341 "SDA" data line)
 *       PC2  CSX  chip select, active low
 *       PD13 D/CX 0 = the byte on the bus is a COMMAND, 1 = it is DATA
 *     SPI5 runs in "bidirectional mode, output enabled" (BIDIMODE + BIDIOE):
 *     it only transmits. In normal full-duplex mode every byte sent would
 *     also receive a byte that nobody reads, causing an overrun (OVR) error.
 *
 *  3. Speed
 *     SPI5 is on APB2 = 4.5 MHz; the fastest SPI divider is /2 -> SCK =
 *     2.25 MHz. One pixel = 16 bits -> ~140,000 pixels/s. A full screen
 *     (240 x 320 x 16 bit = 1.23 Mbit) takes ~0.55 s, so the UI only redraws
 *     the parts that changed.
 *
 *  4. How drawing works (ILI9341 protocol)
 *     a) CASET (0x2A) + 4 data bytes: first and last column (x0, x1).
 *     b) PASET (0x2B) + 4 data bytes: first and last row    (y0, y1).
 *     c) RAMWR (0x2C), then stream (x1-x0+1)*(y1-y0+1) pixels, 2 bytes each.
 *        The controller fills the window left -> right, top -> bottom by
 *        itself, so a filled rectangle is just "set window + repeat colour".
 *     Colour format RGB565: 5 bits red, 6 bits green, 5 bits blue, sent
 *     high byte first. Example: pure red = 0xF800 -> bytes 0xF8, 0x00.
 *
 *  5. Who calls it?
 *     Only the UI task (USER_UI.c, 10 Hz, low priority). Not thread safe.
 *     LCD_Init uses osDelay, so it must run inside a task.
 *     All waits are busy-waits on SPI flags; because the UI task has a low
 *     priority, the flight task simply pre-empts it when it needs the CPU.
 * ============================================================================
 */
#include "USER_LCD.h"
#include "USER_Config.h"
#include "main.h"

/* Font table from USER_Font8x8.c: 95 printable ASCII chars (0x20..0x7E),
 * 8 bytes per char (one byte per row). */
extern const uint8_t g_Font8x8[95][8];

/* CS and D/C are toggled through BSRR: writing bit n sets pin n, writing
 * bit n+16 resets it - a single atomic store, faster than HAL_GPIO_WritePin. */
#define LCD_CS_PORT   GPIOC
#define LCD_CS_PIN    GPIO_PIN_2
#define LCD_DC_PORT   GPIOD
#define LCD_DC_PIN    GPIO_PIN_13

#define LCD_CS_LOW()  (LCD_CS_PORT->BSRR = (uint32_t)LCD_CS_PIN << 16U)
#define LCD_CS_HIGH() (LCD_CS_PORT->BSRR = LCD_CS_PIN)
#define LCD_DC_CMD()  (LCD_DC_PORT->BSRR = (uint32_t)LCD_DC_PIN << 16U)
#define LCD_DC_DATA() (LCD_DC_PORT->BSRR = LCD_DC_PIN)

/* ILI9341 commands */
#define ILI_SWRESET   0x01
#define ILI_SLPOUT    0x11
#define ILI_DISPON    0x29
#define ILI_CASET     0x2A
#define ILI_PASET     0x2B
#define ILI_RAMWR     0x2C
#define ILI_MADCTL    0x36
#define ILI_PIXFMT    0x3A

/* ------------------------------------------------------------------------- */
/* Low level SPI                                                             */
/* ------------------------------------------------------------------------- */
/* Sends one byte: wait until the TX buffer is empty (TXE), then write DR.
 * DR is written as an 8-bit access on purpose so exactly one byte is queued.
 * Does NOT wait for the byte to leave the wire (see LCD_SpiFlush). */
static inline void LCD_SpiWrite(uint8_t b)
{
    while ((SPI5->SR & SPI_SR_TXE) == 0U) { }
    *(volatile uint8_t *)&SPI5->DR = b;
}

/* Waits until every queued byte has been completely shifted out
 * (TXE = 1 and BSY = 0). Needed before changing D/C or raising CS,
 * otherwise the last byte would be read with the wrong D/C level. */
static inline void LCD_SpiFlush(void)
{
    while ((SPI5->SR & SPI_SR_TXE) == 0U) { }
    while ((SPI5->SR & SPI_SR_BSY) != 0U) { }
}

/* Sends one command byte with D/C = 0, then leaves D/C = 1 so the following
 * bytes are taken as data (parameters / pixels). CS must already be low. */
static void LCD_WriteCmd(uint8_t cmd)
{
    LCD_SpiFlush();
    LCD_DC_CMD();
    LCD_SpiWrite(cmd);
    LCD_SpiFlush();
    LCD_DC_DATA();
}

/* One complete transaction: CS low, command, len parameter bytes, CS high.
 * data may be NULL when len == 0. */
static void LCD_CmdData(uint8_t cmd, const uint8_t *data, uint8_t len)
{
    uint8_t i;

    LCD_CS_LOW();
    LCD_WriteCmd(cmd);
    for (i = 0; i < len; i++)
        LCD_SpiWrite(data[i]);
    LCD_SpiFlush();
    LCD_CS_HIGH();
}

/* Open a drawing window and leave CS low, ready for pixel data */
/* Coordinates are inclusive pixels, sent as 16-bit big-endian values.
 * Example: x 10..19, y 0..7 -> CASET 00 0A 00 13, PASET 00 00 00 07,
 * then RAMWR expects 10 x 8 = 80 pixels. Must be followed by LCD_EndWrite. */
static void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    LCD_CS_LOW();
    LCD_WriteCmd(ILI_CASET);
    LCD_SpiWrite((uint8_t)(x0 >> 8)); LCD_SpiWrite((uint8_t)x0);
    LCD_SpiWrite((uint8_t)(x1 >> 8)); LCD_SpiWrite((uint8_t)x1);
    LCD_WriteCmd(ILI_PASET);
    LCD_SpiWrite((uint8_t)(y0 >> 8)); LCD_SpiWrite((uint8_t)y0);
    LCD_SpiWrite((uint8_t)(y1 >> 8)); LCD_SpiWrite((uint8_t)y1);
    LCD_WriteCmd(ILI_RAMWR);
}

/* Closes a pixel stream started by LCD_SetWindow (flush, CS high). */
static void LCD_EndWrite(void)
{
    LCD_SpiFlush();
    LCD_CS_HIGH();
}

/* Sends one RGB565 pixel, high byte first (as the ILI9341 expects). */
static inline void LCD_PushColor(uint16_t c)
{
    LCD_SpiWrite((uint8_t)(c >> 8));
    LCD_SpiWrite((uint8_t)c);
}

/* ------------------------------------------------------------------------- */
/* Init                                                                      */
/* ------------------------------------------------------------------------- */
/* Configures CS/D/C as GPIO outputs, PF7/PF9 as SPI5 pins, and SPI5 itself
 * directly through its registers (no HAL SPI handle is used). */
static void LCD_GpioSpiInit(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_SPI5_CLK_ENABLE();

    /* CS and D/C */
    /* Output latches are set BEFORE the pins become outputs, so CS never
     * glitches low (which could start a bogus transaction). */
    LCD_CS_HIGH();
    LCD_DC_DATA();
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(LCD_CS_PORT, &gpio);
    gpio.Pin = LCD_DC_PIN;
    HAL_GPIO_Init(LCD_DC_PORT, &gpio);

    /* SPI5 SCK (PF7) and MOSI (PF9), AF5 */
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_7 | GPIO_PIN_9;
    gpio.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &gpio);

    /* Master, CPOL0/CPHA0, 8 bit, MSB first, fPCLK/2, software NSS,
     * bidirectional mode with output enabled (TX only, no RX overrun). */
    /* CR1 is cleared first (SPE = 0) because the configuration bits may only
     * be changed while the SPI is disabled. SSM + SSI: NSS is handled in
     * software and forced high internally, so the master does not fault
     * (MODF) - the real chip select is the PC2 GPIO. SPE is set last. */
    SPI5->CR1 = 0;
    SPI5->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI |
                SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE;   /* BR = 000 -> /2 */
    SPI5->CR2 = 0;
    SPI5->CR1 |= SPI_CR1_SPE;
}

/* One entry of the init table: command byte + up to 15 parameter bytes. */
typedef struct
{
    uint8_t cmd;
    uint8_t len;
    uint8_t data[15];
} LCD_InitCmd;

/* Standard ILI9341 power-up sequence, 16 bit/pixel, system (SPI) interface */
/* Most values are the vendor's recommended defaults (same as the ST BSP).
 * The ones that matter for this project:
 *   MADCTL (0x36): scan direction / mirroring and BGR order -> change
 *                  LCD_MADCTL_VALUE in USER_Config.h if the image is
 *                  flipped or red/blue are swapped.
 *   PIXFMT (0x3A) = 0x55: 16 bits per pixel (RGB565).
 *   0xB0 / 0xF6: use the serial "system" interface, not the RGB (LTDC) one,
 *                so the panel shows the contents of GRAM. */
static const LCD_InitCmd s_init_seq[] =
{
    { 0xCB, 5,  { 0x39, 0x2C, 0x00, 0x34, 0x02 } },       /* Power control A    */
    { 0xCF, 3,  { 0x00, 0xC1, 0x30 } },                   /* Power control B    */
    { 0xE8, 3,  { 0x85, 0x00, 0x78 } },                   /* Driver timing A    */
    { 0xEA, 2,  { 0x00, 0x00 } },                         /* Driver timing B    */
    { 0xED, 4,  { 0x64, 0x03, 0x12, 0x81 } },             /* Power on sequence  */
    { 0xF7, 1,  { 0x20 } },                               /* Pump ratio         */
    { 0xC0, 1,  { 0x23 } },                               /* Power control 1    */
    { 0xC1, 1,  { 0x10 } },                               /* Power control 2    */
    { 0xC5, 2,  { 0x3E, 0x28 } },                         /* VCOM control 1     */
    { 0xC7, 1,  { 0x86 } },                               /* VCOM control 2     */
    { ILI_MADCTL, 1, { LCD_MADCTL_VALUE } },              /* Orientation / BGR  */
    { ILI_PIXFMT, 1, { 0x55 } },                          /* 16 bit / pixel     */
    { 0xB0, 1,  { 0x00 } },                               /* RGB interface off  */
    { 0xF6, 3,  { 0x01, 0x00, 0x00 } },                   /* System interface   */
    { 0xB1, 2,  { 0x00, 0x18 } },                         /* Frame rate         */
    { 0xB6, 3,  { 0x08, 0x82, 0x27 } },                   /* Display function   */
    { 0xF2, 1,  { 0x00 } },                               /* 3 gamma off        */
    { 0x26, 1,  { 0x01 } },                               /* Gamma curve        */
    { 0xE0, 15, { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                  0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 } },  /* +Gamma */
    { 0xE1, 15, { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                  0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F } },  /* -Gamma */
};

/* Full LCD start-up (~260 ms, uses osDelay -> call from the UI task):
 * SPI init, software reset, init table, exit sleep, display on.
 * The 120 ms waits after SWRESET and SLPOUT are required by the datasheet. */
void LCD_Init(void)
{
    uint32_t i;

    LCD_GpioSpiInit();

    /* The LCD reset line is tied to the board reset: use a software reset */
    LCD_CmdData(ILI_SWRESET, NULL, 0);
    osDelay(120);

    for (i = 0; i < (sizeof(s_init_seq) / sizeof(s_init_seq[0])); i++)
        LCD_CmdData(s_init_seq[i].cmd, s_init_seq[i].data, s_init_seq[i].len);

    LCD_CmdData(ILI_SLPOUT, NULL, 0);
    osDelay(120);
    LCD_CmdData(ILI_DISPON, NULL, 0);
    osDelay(20);
}

/* ------------------------------------------------------------------------- */
/* Drawing                                                                   */
/* ------------------------------------------------------------------------- */
/* Fills a w x h rectangle at (x, y) with one RGB565 colour. Parts outside the
 * screen are clipped, so callers may pass negative / oversized values.
 * Cost: w*h*16 bits on a 2.25 MHz bus, e.g. 100 x 20 px = 32,000 bits
 * = ~14 ms. */
void LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    uint32_t n;

    /* Clip */
    /* Example: x = -5, w = 20 -> w = 15, x = 0 (the 5 hidden columns are dropped). */
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if ((x + w) > LCD_WIDTH)  w = LCD_WIDTH - x;
    if ((y + h) > LCD_HEIGHT) h = LCD_HEIGHT - y;
    if ((w <= 0) || (h <= 0))
        return;

    LCD_SetWindow((uint16_t)x, (uint16_t)y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));
    for (n = (uint32_t)w * (uint32_t)h; n > 0U; n--)
        LCD_PushColor(color);
    LCD_EndWrite();
}

void LCD_FillScreen(uint16_t color)
{
    LCD_FillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

/* Single pixel. Expensive (11 bytes of window setup for 2 bytes of colour),
 * use FillRect for anything bigger than a few dots. */
void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
    if ((x < 0) || (y < 0) || (x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
        return;
    LCD_SetWindow((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
    LCD_PushColor(color);
    LCD_EndWrite();
}

void LCD_DrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_FillRect(x, y, w, 1, color);
}

void LCD_DrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_FillRect(x, y, 1, h, color);
}

/* Rectangle outline (1 px), drawn as 4 lines. */
void LCD_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    LCD_DrawHLine(x, y, w, color);
    LCD_DrawHLine(x, (int16_t)(y + h - 1), w, color);
    LCD_DrawVLine(x, y, h, color);
    LCD_DrawVLine((int16_t)(x + w - 1), y, h, color);
}

/* Draws one character with its top-left corner at (x, y).
 *   fg / bg : text / background colour (the background is always painted,
 *             so new text overwrites old text without clearing first)
 *   scale   : 1 = 8x8 px, 2 = 16x16 px, ...
 * Non-printable chars are shown as '?'. A char that does not fit entirely
 * on the screen is skipped (no partial clipping).
 * The whole char is ONE window, so all its pixels are streamed in a row:
 * for scale 2 that is 16 x 16 = 256 pixels after a single window setup. */
void LCD_DrawChar(int16_t x, int16_t y, char c, uint16_t fg, uint16_t bg, uint8_t scale)
{
    const uint8_t *glyph;
    uint8_t row, col, sy, sx;
    int16_t size;

    if ((c < 0x20) || (c > 0x7E))
        c = '?';
    if (scale == 0U)
        scale = 1U;

    size = (int16_t)(8 * scale);
    if ((x < 0) || (y < 0) || ((x + size) > LCD_WIDTH) || ((y + size) > LCD_HEIGHT))
        return;

    glyph = g_Font8x8[c - 0x20];   /* the table starts at ' ' (0x20) */

    /* Pixels must be sent in the exact order the controller fills the
     * window: row by row, left to right. Each font row is repeated `scale`
     * times (sy) and each font pixel `scale` times (sx) to enlarge it.
     * Bit 0 of a font byte is the LEFTMOST pixel, hence (glyph[row] >> col). */
    LCD_SetWindow((uint16_t)x, (uint16_t)y, (uint16_t)(x + size - 1), (uint16_t)(y + size - 1));
    for (row = 0; row < 8U; row++)
    {
        for (sy = 0; sy < scale; sy++)
        {
            for (col = 0; col < 8U; col++)
            {
                uint16_t color = ((glyph[row] >> col) & 1U) ? fg : bg;
                for (sx = 0; sx < scale; sx++)
                    LCD_PushColor(color);
            }
        }
    }
    LCD_EndWrite();
}

/* Draws a string on one line, advancing 8*scale px per char (no wrapping). */
void LCD_DrawString(int16_t x, int16_t y, const char *s, uint16_t fg, uint16_t bg, uint8_t scale)
{
    while (*s != '\0')
    {
        LCD_DrawChar(x, y, *s++, fg, bg, scale);
        x = (int16_t)(x + 8 * scale);
    }
}
