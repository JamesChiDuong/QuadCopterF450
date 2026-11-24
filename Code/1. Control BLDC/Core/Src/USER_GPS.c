/*
 * USER_GPS.c
 *
 *  NMEA GPS on UART5 + QMC5883L compass on I2C2. See USER_GPS.h (overview).
 *  Whole file is empty when GPS_ENABLE = 0.
 *
 *  Notes
 *   - UART5 RX uses a direct register interrupt (no HAL callbacks), so the
 *     only shared state with the ISR is a single-producer / single-consumer
 *     ring buffer (head written by the ISR, tail by the flight task).
 *   - Gps_Process() runs in the flight task (BLDCTask): parsing a 5 Hz GGA +
 *     RMC stream costs a few microseconds per 4 ms cycle.
 *   - The compass shares I2C2 (and its mutex) with the MPU6050. Both are
 *     read from the same task, so they never collide; the mutex is only for
 *     the UART commands that may touch the bus.
 */

#include "main.h"
#include "USER_GPS.h"

#if (GPS_ENABLE == 1)

#include <math.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c2;
extern osMutexId mutex_i2c2_user;

/* ------------------------------------------------------------------------- */
/* UART5 receive ring buffer                                                  */
/* ------------------------------------------------------------------------- */
#define GPS_RX_BUF_SIZE     512U            /* power of 2                     */
#define GPS_LINE_MAX        100U            /* longest NMEA sentence is 82    */

static UART_HandleTypeDef s_huart5;
static volatile uint8_t  s_rx_buf[GPS_RX_BUF_SIZE];
static volatile uint16_t s_rx_head;         /* written by the ISR             */
static volatile uint16_t s_rx_tail;         /* written by Gps_Process         */
static volatile uint32_t s_rx_lost;

static Gps_DataTypeDef s_gps;

/* NMEA line assembler */
static char     s_line[GPS_LINE_MAX + 1];
static uint16_t s_line_len;
static bool     s_in_line;

/* Interrupt: one byte per RXNE. Reading SR then DR also clears ORE. */
void UART5_IRQHandler(void)
{
    uint32_t sr = UART5->SR;
    uint8_t c;
    uint16_t next;

    if (sr & (USART_SR_RXNE | USART_SR_ORE))
    {
        c = (uint8_t)UART5->DR;
        if (sr & USART_SR_ORE)
            s_rx_lost++;
        next = (uint16_t)((s_rx_head + 1U) & (GPS_RX_BUF_SIZE - 1U));
        if (next != s_rx_tail)
        {
            s_rx_buf[s_rx_head] = c;
            s_rx_head = next;
        }
        else
        {
            s_rx_lost++;
        }
    }
}

static void Gps_UartSetBaud(uint32_t baud)
{
    HAL_NVIC_DisableIRQ(UART5_IRQn);
    s_huart5.Instance = UART5;
    s_huart5.Init.BaudRate = baud;
    s_huart5.Init.WordLength = UART_WORDLENGTH_8B;
    s_huart5.Init.StopBits = UART_STOPBITS_1;
    s_huart5.Init.Parity = UART_PARITY_NONE;
    s_huart5.Init.Mode = UART_MODE_TX_RX;
    s_huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    s_huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    (void)HAL_UART_Init(&s_huart5);
    s_gps.baud = baud;
    s_rx_tail = s_rx_head;                  /* drop bytes of the old baud     */
    s_in_line = false;
    __HAL_UART_ENABLE_IT(&s_huart5, UART_IT_RXNE);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
}

static void Gps_UartInit(void)
{
    GPIO_InitTypeDef g = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_UART5_CLK_ENABLE();

    /* PC12 = UART5_TX, PD2 = UART5_RX (AF8). Pull-up on RX: idle high when
     * no module is plugged (no garbage bytes). */
    g.Mode = GPIO_MODE_AF_PP;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    g.Alternate = GPIO_AF8_UART5;
    g.Pin = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC, &g);
    g.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &g);

    /* Priority 6: below configLIBRARY_MAX_SYSCALL (5) is not needed because
     * the ISR uses no FreeRTOS API; same level as the USB host. */
    HAL_NVIC_SetPriority(UART5_IRQn, 6, 0);
}

/* ------------------------------------------------------------------------- */
/* UBX configuration (u-blox)                                                 */
/* ------------------------------------------------------------------------- */
#if (GPS_UBX_CONFIG == 1)
/* Sends one UBX frame: B5 62 class id len16 payload ckA ckB (8-bit Fletcher
 * checksum over class..payload). */
static void Gps_UbxSend(uint8_t cls, uint8_t id, const uint8_t *payload, uint16_t len)
{
    uint8_t hdr[6];
    uint8_t ck[2] = {0, 0};
    uint16_t i;

    hdr[0] = 0xB5;
    hdr[1] = 0x62;
    hdr[2] = cls;
    hdr[3] = id;
    hdr[4] = (uint8_t)(len & 0xFFU);
    hdr[5] = (uint8_t)(len >> 8);
    for (i = 2; i < 6; i++)
    {
        ck[0] = (uint8_t)(ck[0] + hdr[i]);
        ck[1] = (uint8_t)(ck[1] + ck[0]);
    }
    for (i = 0; i < len; i++)
    {
        ck[0] = (uint8_t)(ck[0] + payload[i]);
        ck[1] = (uint8_t)(ck[1] + ck[0]);
    }
    (void)HAL_UART_Transmit(&s_huart5, hdr, 6, 100);
    if (len)
        (void)HAL_UART_Transmit(&s_huart5, (uint8_t *)payload, len, 100);
    (void)HAL_UART_Transmit(&s_huart5, ck, 2, 100);
    HAL_Delay(30);                          /* let the receiver apply it      */
}

static void Gps_Put32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

/* CFG-PRT: UART1 of the module, 8N1, new baud, in UBX+NMEA, out NMEA. */
static void Gps_UbxSetBaud(uint32_t baud)
{
    uint8_t p[20] = {0};

    p[0] = 1;                               /* portID = UART1                 */
    Gps_Put32(&p[4], 0x000008D0UL);         /* mode: 8 bit, no parity, 1 stop */
    Gps_Put32(&p[8], baud);
    p[12] = 0x03;                           /* inProtoMask  UBX | NMEA        */
    p[14] = 0x02;                           /* outProtoMask NMEA              */
    Gps_UbxSend(0x06, 0x00, p, sizeof(p));
}

/* CFG-MSG (3 byte form = current port), NMEA class 0xF0 */
static void Gps_UbxNmeaRate(uint8_t msg_id, uint8_t rate)
{
    uint8_t p[3] = { 0xF0, msg_id, rate };
    Gps_UbxSend(0x06, 0x01, p, sizeof(p));
}

static void Gps_UbxConfigureMessages(void)
{
    uint8_t rate[6];
    uint8_t nav5[36] = {0};
    uint16_t ms = (uint16_t)(1000U / GPS_RATE_HZ);

    Gps_UbxNmeaRate(0x00, 1);               /* GGA on                         */
    Gps_UbxNmeaRate(0x04, 1);               /* RMC on                         */
    Gps_UbxNmeaRate(0x01, 0);               /* GLL off                        */
    Gps_UbxNmeaRate(0x02, 0);               /* GSA off                        */
    Gps_UbxNmeaRate(0x03, 0);               /* GSV off                        */
    Gps_UbxNmeaRate(0x05, 0);               /* VTG off                        */

    /* CFG-RATE: measurement period, 1 navigation per measurement, GPS time */
    rate[0] = (uint8_t)ms; rate[1] = (uint8_t)(ms >> 8);
    rate[2] = 1; rate[3] = 0;
    rate[4] = 1; rate[5] = 0;
    Gps_UbxSend(0x06, 0x08, rate, sizeof(rate));

    /* CFG-NAV5: mask = dynamic model only, model 6 = airborne < 1 g (better
     * filtering for a multirotor than the default "portable") */
    nav5[0] = 0x01;
    nav5[2] = 6;
    Gps_UbxSend(0x06, 0x24, nav5, sizeof(nav5));
}
#endif /* GPS_UBX_CONFIG */

/* ------------------------------------------------------------------------- */
/* NMEA parsing                                                               */
/* ------------------------------------------------------------------------- */
static int Gps_Hex(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

/* Small decimal parser (no strtod: exact enough and independent of the C
 * library). Empty field -> 0, *ok = false. */
static double Gps_ParseNum(const char *s, bool *ok)
{
    double v = 0.0, scale = 1.0;
    bool neg = false, any = false, frac = false;

    if (*s == '-') { neg = true; s++; }
    for (; *s && *s != ','; s++)
    {
        if (*s == '.') { frac = true; continue; }
        if (*s < '0' || *s > '9') break;
        any = true;
        if (frac)
        {
            scale *= 0.1;
            v += (*s - '0') * scale;
        }
        else
        {
            v = v * 10.0 + (*s - '0');
        }
    }
    if (ok) *ok = any;
    return neg ? -v : v;
}

/* "ddmm.mmmm" / "dddmm.mmmm" + hemisphere -> signed degrees */
static double Gps_ParseLatLon(const char *f, char hemi, bool *ok)
{
    double raw = Gps_ParseNum(f, ok);
    double deg = floor(raw / 100.0);
    double val = deg + (raw - deg * 100.0) / 60.0;
    if (hemi == 'S' || hemi == 'W')
        val = -val;
    return val;
}

/* Splits the sentence body in place; returns the number of fields. */
static uint8_t Gps_Split(char *s, const char *field[], uint8_t max)
{
    uint8_t n = 0;

    field[n++] = s;
    for (; *s && n < max; s++)
    {
        if (*s == ',')
        {
            *s = '\0';
            field[n++] = s + 1;
        }
    }
    return n;
}

static void Gps_UpdateLocal(void)
{
    const double m_per_deg = 111319.5;
    if (!s_gps.has_home)
        return;
    s_gps.pos_n_m = (float)((s_gps.lat_deg - s_gps.home_lat_deg) * m_per_deg);
    s_gps.pos_e_m = (float)((s_gps.lon_deg - s_gps.home_lon_deg) * m_per_deg *
                            cos(s_gps.home_lat_deg * 0.017453292519943295));
}

static void Gps_HandleGGA(const char *f[], uint8_t n)
{
    bool ok_lat, ok_lon, ok;
    double lat, lon;

    if (n < 10)
        return;
    s_gps.fix_quality = (uint8_t)Gps_ParseNum(f[6], &ok);
    s_gps.sats = (uint8_t)Gps_ParseNum(f[7], &ok);
    s_gps.hdop = (float)Gps_ParseNum(f[8], &ok);
    if (!ok)
        s_gps.hdop = 99.0f;
    s_gps.alt_m = (float)Gps_ParseNum(f[9], &ok);

    lat = Gps_ParseLatLon(f[2], f[3][0], &ok_lat);
    lon = Gps_ParseLatLon(f[4], f[5][0], &ok_lon);
    if (s_gps.fix_quality == 0U || !ok_lat || !ok_lon)
        return;

    s_gps.lat_deg = lat;
    s_gps.lon_deg = lon;
    Gps_UpdateLocal();
    if ((s_gps.sats >= GPS_MIN_SATS) && (s_gps.hdop <= GPS_MAX_HDOP))
        s_gps.last_fix_ms = HAL_GetTick();
}

static void Gps_HandleRMC(const char *f[], uint8_t n)
{
    bool ok;
    float kn, crs, r;

    if (n < 9)
        return;
    s_gps.rmc_valid = (f[2][0] == 'A');
    if (!s_gps.rmc_valid)
    {
        s_gps.vel_n_ms = s_gps.vel_e_ms = 0.0f;
        return;
    }
    kn = (float)Gps_ParseNum(f[7], &ok);
    crs = (float)Gps_ParseNum(f[8], &ok);
    if (!ok)
        crs = 0.0f;                         /* course empty when not moving   */
    s_gps.speed_ms = kn * 0.514444f;
    s_gps.course_deg = crs;
    r = crs * 0.01745329252f;
    s_gps.vel_n_ms = s_gps.speed_ms * cosf(r);
    s_gps.vel_e_ms = s_gps.speed_ms * sinf(r);
}

/* One complete line "GPGGA,...*hh" (without '$'). */
static void Gps_HandleLine(char *line)
{
    char *star = strchr(line, '*');
    const char *f[20];
    uint8_t cks = 0, n;
    int h1, h2;
    char *p;

    if (star == NULL)
        return;
    for (p = line; p < star; p++)
        cks ^= (uint8_t)*p;
    h1 = Gps_Hex(star[1]);
    h2 = Gps_Hex(star[2]);
    if (h1 < 0 || h2 < 0 || (uint8_t)((h1 << 4) | h2) != cks)
    {
        s_gps.cksum_errors++;
        return;
    }
    *star = '\0';
    s_gps.sentences++;

    n = Gps_Split(line, f, 20);
    /* f[0] = "GPGGA" / "GNGGA" / "GLRMC": compare the last 3 letters */
    if (strlen(f[0]) != 5)
        return;
    if (strcmp(&f[0][2], "GGA") == 0)
        Gps_HandleGGA(f, n);
    else if (strcmp(&f[0][2], "RMC") == 0)
        Gps_HandleRMC(f, n);
}

static void Gps_ParseBytes(void)
{
    uint8_t c;

    while (s_rx_tail != s_rx_head)
    {
        c = s_rx_buf[s_rx_tail];
        s_rx_tail = (uint16_t)((s_rx_tail + 1U) & (GPS_RX_BUF_SIZE - 1U));

        if (c == '$')
        {
            s_in_line = true;
            s_line_len = 0;
        }
        else if (s_in_line)
        {
            if (c == '\r' || c == '\n')
            {
                s_line[s_line_len] = '\0';
                s_in_line = false;
                Gps_HandleLine(s_line);
            }
            else if (s_line_len < GPS_LINE_MAX)
            {
                s_line[s_line_len++] = (char)c;
            }
            else
            {
                s_in_line = false;          /* too long: garbage / wrong baud */
            }
        }
    }
    s_gps.rx_overruns = s_rx_lost;
}

/* Listens for up to ms milliseconds; true if valid sentences arrived. */
static bool Gps_Listen(uint32_t ms)
{
    uint32_t t0 = HAL_GetTick();
    uint32_t start = s_gps.sentences;

    while ((HAL_GetTick() - t0) < ms)
    {
        Gps_ParseBytes();
        if (s_gps.sentences >= start + 2U)
            return true;
        HAL_Delay(5);
    }
    return false;
}

/* ------------------------------------------------------------------------- */
/* Compass QMC5883L                                                           */
/* ------------------------------------------------------------------------- */
#if (GPS_HEADING_SOURCE == 0)
#define QMC_ADDR            (0x0DU << 1)
#define QMC_REG_DATA        0x00U
#define QMC_REG_STATUS      0x06U
#define QMC_REG_CTRL1       0x09U
#define QMC_REG_SETRESET    0x0BU
#define QMC_REG_CHIPID      0x0DU
#define MAG_CAL_TIME_MS     30000U

static float    s_mag_off[3];               /* hard-iron offsets (raw counts) */
static float    s_cal_min[3], s_cal_max[3];
static bool     s_cal_run;
static uint32_t s_cal_t0;
static uint32_t s_mag_last_ms;
static bool     s_heading_init;

static bool Mag_Lock(void)
{
    if (bRTOS_En)
        return (osMutexWait(mutex_i2c2_user, 2) == osOK);
    return true;
}

static void Mag_Unlock(void)
{
    if (bRTOS_En)
        osMutexRelease(mutex_i2c2_user);
}

static bool Mag_Write(uint8_t reg, uint8_t val)
{
    bool ok;
    if (!Mag_Lock())
        return false;
    ok = (HAL_I2C_Mem_Write(&hi2c2, QMC_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 5) == HAL_OK);
    Mag_Unlock();
    return ok;
}

static bool Mag_Read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    bool ok;
    if (!Mag_Lock())
        return false;
    ok = (HAL_I2C_Mem_Read(&hi2c2, QMC_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 5) == HAL_OK);
    Mag_Unlock();
    return ok;
}

static void Mag_Init(void)
{
    uint8_t id = 0;

    s_gps.compass_ok = false;
    if (!Mag_Read(QMC_REG_CHIPID, &id, 1) || id != 0xFFU)
    {
        RTOSprintf("\r\n> Compass QMC5883L not found (I2C2 0x0D, id 0x%02X)", id);
        return;
    }
    /* SET/RESET period = 1 (datasheet), then continuous, 200 Hz, 8 G, OSR 512 */
    (void)Mag_Write(QMC_REG_SETRESET, 0x01);
    s_gps.compass_ok = Mag_Write(QMC_REG_CTRL1, 0x1D);
    RTOSprintf("\r\n> Compass QMC5883L %s", s_gps.compass_ok ? "OK" : "config failed");
}

static float Gps_Wrap180(float a)
{
    while (a > 180.0f) a -= 360.0f;
    while (a < -180.0f) a += 360.0f;
    return a;
}

/* Reads one sample and computes the tilt compensated heading. */
static void Mag_Step(float roll_deg, float pitch_deg)
{
    uint8_t b[6];
    float s[3], mx, my, mz, cr, sr, cp, sp, xh, yh, h;
    uint8_t i;

    if (!s_gps.compass_ok || !Mag_Read(QMC_REG_DATA, b, 6))
        return;
    for (i = 0; i < 3; i++)
        s[i] = (float)(int16_t)((uint16_t)b[2 * i] | ((uint16_t)b[2 * i + 1] << 8));

    if (s_cal_run)
    {
        for (i = 0; i < 3; i++)
        {
            if (s[i] < s_cal_min[i]) s_cal_min[i] = s[i];
            if (s[i] > s_cal_max[i]) s_cal_max[i] = s[i];
        }
        if ((HAL_GetTick() - s_cal_t0) >= MAG_CAL_TIME_MS)
        {
            for (i = 0; i < 3; i++)
                s_mag_off[i] = 0.5f * (s_cal_min[i] + s_cal_max[i]);
            s_cal_run = false;
            RTOSprintf("\r\n> MAGCAL done: offsets %.0f %.0f %.0f (RAM only)",
                       s_mag_off[0], s_mag_off[1], s_mag_off[2]);
        }
    }
    for (i = 0; i < 3; i++)
        s[i] -= s_mag_off[i];

    mx = MAG_BODY_X(s[0], s[1], s[2]);
    my = MAG_BODY_Y(s[0], s[1], s[2]);
    mz = MAG_BODY_Z(s[0], s[1], s[2]);

    /* Tilt compensation (FRD body, NED world): project the field on the
     * horizontal plane, then heading = atan2(-Yh, Xh), clockwise from North. */
    cr = cosf(roll_deg * 0.01745329252f);
    sr = sinf(roll_deg * 0.01745329252f);
    cp = cosf(pitch_deg * 0.01745329252f);
    sp = sinf(pitch_deg * 0.01745329252f);
    xh = mx * cp + my * sr * sp + mz * cr * sp;
    yh = my * cr - mz * sr;
    h = atan2f(-yh, xh) * 57.2957795f + COMPASS_DECLINATION_DEG;
    if (h < 0.0f) h += 360.0f;
    if (h >= 360.0f) h -= 360.0f;
    s_gps.mag_heading_deg = h;
    s_mag_last_ms = HAL_GetTick();

    if (!s_heading_init)
    {
        s_gps.heading_deg = h;
        s_heading_init = true;
    }
}
#endif /* GPS_HEADING_SOURCE == 0 */

/* ------------------------------------------------------------------------- */
/* Public                                                                     */
/* ------------------------------------------------------------------------- */
void Gps_Init(void)
{
    memset(&s_gps, 0, sizeof(s_gps));
    s_gps.hdop = 99.0f;
    Gps_UartInit();

#if (GPS_UBX_CONFIG == 1)
    /* Already at the fast baud (configured during a previous boot)? */
    Gps_UartSetBaud(GPS_UBX_BAUD);
    if (!Gps_Listen(1200))
    {
        Gps_UartSetBaud(GPS_UART_BAUD);
        if (Gps_Listen(1500))
        {
            Gps_UbxSetBaud(GPS_UBX_BAUD);
            HAL_Delay(100);
            Gps_UartSetBaud(GPS_UBX_BAUD);
        }
    }
    Gps_UbxConfigureMessages();
    if (Gps_Listen(1500))
        RTOSprintf("\r\n> GPS on UART5: NMEA at %lu baud, %u Hz", (unsigned long)s_gps.baud, GPS_RATE_HZ);
    else
        RTOSprintf("\r\n> GPS on UART5: no NMEA data (module missing / wiring PC12->RX, PD2<-TX)");
#else
    Gps_UartSetBaud(GPS_UART_BAUD);
    RTOSprintf("\r\n> GPS on UART5: %s", Gps_Listen(2000) ? "NMEA OK" : "no NMEA data");
#endif

#if (GPS_HEADING_SOURCE == 0)
    Mag_Init();
#endif
}

void Gps_Process(float roll_deg, float pitch_deg, float yaw_rate_dps, float dt)
{
    static uint8_t div;

    Gps_ParseBytes();

    /* Heading: integrate the gyro yaw rate (nose right = heading increases) */
    s_gps.heading_deg += yaw_rate_dps * dt;

#if (GPS_HEADING_SOURCE == 0)
    /* Compass at 50 Hz (every 5th 4 ms cycle), then pull the gyro heading
     * towards it with time constant COMPASS_TAU_S */
    if (++div >= 5U)
    {
        div = 0;
        Mag_Step(roll_deg, pitch_deg);
    }
    if (s_heading_init && ((HAL_GetTick() - s_mag_last_ms) < 500U))
    {
        s_gps.heading_deg += Gps_Wrap180(s_gps.mag_heading_deg - s_gps.heading_deg) *
                             (dt / COMPASS_TAU_S);
        s_gps.heading_ok = !s_cal_run;
    }
    else
    {
        s_gps.heading_ok = false;
    }
#else
    (void)roll_deg;
    (void)pitch_deg;
    (void)div;
#endif

    while (s_gps.heading_deg >= 360.0f) s_gps.heading_deg -= 360.0f;
    while (s_gps.heading_deg < 0.0f) s_gps.heading_deg += 360.0f;
}

bool Gps_FixGood(void)
{
    return (s_gps.fix_quality >= 1U) && s_gps.rmc_valid &&
           (s_gps.sats >= GPS_MIN_SATS) && (s_gps.hdop <= GPS_MAX_HDOP) &&
           (s_gps.last_fix_ms != 0U) &&
           ((HAL_GetTick() - s_gps.last_fix_ms) < GPS_TIMEOUT_MS);
}

bool Gps_SetHome(void)
{
    if (!Gps_FixGood())
        return false;
    s_gps.home_lat_deg = s_gps.lat_deg;
    s_gps.home_lon_deg = s_gps.lon_deg;
    s_gps.has_home = true;
    Gps_UpdateLocal();
    return true;
}

void Gps_AlignHeadingNorth(void)
{
    s_gps.heading_deg = 0.0f;
#if (GPS_HEADING_SOURCE == 1)
    s_gps.heading_ok = true;
#endif
}

void Gps_MagCalStart(void)
{
#if (GPS_HEADING_SOURCE == 0)
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        s_cal_min[i] = 1e9f;
        s_cal_max[i] = -1e9f;
        s_mag_off[i] = 0.0f;
    }
    s_cal_t0 = HAL_GetTick();
    s_cal_run = true;
#endif
}

bool Gps_MagCalRunning(void)
{
#if (GPS_HEADING_SOURCE == 0)
    return s_cal_run;
#else
    return false;
#endif
}

const Gps_DataTypeDef *Gps_Get(void)
{
    return &s_gps;
}

#endif /* GPS_ENABLE */
