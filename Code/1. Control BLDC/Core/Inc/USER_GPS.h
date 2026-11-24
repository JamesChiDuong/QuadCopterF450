/*
 * USER_GPS.h
 *
 *  NMEA GPS receiver on UART5 (PC12 TX / PD2 RX) + optional QMC5883L compass
 *  on I2C2. Compiled only when GPS_ENABLE = 1 (USER_Config.h).
 */
/*
 * ===========================================================================
 * OVERVIEW (USER_GPS)
 * ===========================================================================
 *  Data path:
 *    GPS module --UART5 bytes--> UART5_IRQHandler --> 512 B ring buffer
 *    Gps_Process() (called by BLDCTask every 4 ms) --> NMEA parser
 *      $xxGGA : fix quality, satellites, HDOP, latitude, longitude, altitude
 *      $xxRMC : status, ground speed, course
 *    --> Gps_DataTypeDef (latest fix) --> local North / East metres from the
 *        home point (Gps_SetHome, taken at arm) + North / East speed.
 *
 *  Boot (GPS_UBX_CONFIG = 1, u-blox receivers): UBX messages switch the
 *  module to GPS_UBX_BAUD, GPS_RATE_HZ and GGA + RMC only. A module that is
 *  already at GPS_UBX_BAUD (configured before) is detected automatically.
 *
 *  Compass (GPS_HEADING_SOURCE = 0): QMC5883L at I2C address 0x0D on I2C2,
 *  read at 50 Hz from the flight task (same bus mutex as the MPU6050).
 *  Heading = tilt compensated magnetic heading + COMPASS_DECLINATION_DEG,
 *  fused with the gyro yaw rate (Gps_UpdateHeading).
 *
 *  Local coordinates: equirectangular approximation, exact to a few cm
 *  over a few km:  north = dLat * 111319.5 m/deg
 *                  east  = dLon * 111319.5 * cos(home latitude)
 * ===========================================================================
 */

#ifndef INC_USER_GPS_H_
#define INC_USER_GPS_H_

#include <stdint.h>
#include <stdbool.h>
#include "USER_Config.h"

typedef struct
{
    /* Latest fix (from GGA / RMC) */
    double   lat_deg;        /* + North                                      */
    double   lon_deg;        /* + East                                       */
    float    alt_m;          /* above mean sea level (information only)      */
    uint8_t  fix_quality;    /* GGA: 0 none, 1 GPS, 2 DGPS ...               */
    uint8_t  sats;           /* satellites used                              */
    float    hdop;
    float    speed_ms;       /* RMC ground speed                             */
    float    course_deg;     /* RMC course over ground, 0 = North            */
    bool     rmc_valid;      /* RMC status 'A'                               */
    uint32_t last_fix_ms;    /* HAL tick of the last GOOD fix                */

    /* Derived (valid when has_home) */
    bool     has_home;
    double   home_lat_deg;
    double   home_lon_deg;
    float    pos_n_m;        /* metres North of home                         */
    float    pos_e_m;        /* metres East of home                          */
    float    vel_n_ms;       /* ground speed North / East                    */
    float    vel_e_ms;

    /* Statistics */
    uint32_t sentences;      /* NMEA sentences with a good checksum          */
    uint32_t cksum_errors;
    uint32_t rx_overruns;    /* bytes lost (ring buffer full / UART ORE)     */
    uint32_t baud;           /* current UART5 baud rate                      */

    /* Heading */
    bool     compass_ok;     /* QMC5883L found and answering                 */
    float    mag_heading_deg;/* last tilt-compensated compass heading        */
    float    heading_deg;    /* fused heading used by POSHOLD (0..360)       */
    bool     heading_ok;
} Gps_DataTypeDef;

#if (GPS_ENABLE == 1)

/* UART5 + GPIO init, u-blox configuration, compass detection. Call once from
 * Sys_Init() (before the scheduler: it blocks for ~1.5 s). */
void Gps_Init(void);

/* Parse all received bytes; also reads the compass at 50 Hz. Call every
 * control cycle from the flight task. roll/pitch (deg) are used for the
 * compass tilt compensation, yaw_rate (deg/s) for the heading fusion. */
void Gps_Process(float roll_deg, float pitch_deg, float yaw_rate_dps, float dt);

/* true = 3D fix good enough for POSHOLD (sats, HDOP, not older than
 * GPS_TIMEOUT_MS). */
bool Gps_FixGood(void);

/* Home = current fix (called at arm). false if no good fix. */
bool Gps_SetHome(void);

/* GPS_HEADING_SOURCE 1: heading := 0 (the pilot pointed the nose North). */
void Gps_AlignHeadingNorth(void);

/* Compass hard-iron calibration: start (rotate the quad in all directions
 * for 30 s) / stop. Offsets kept in RAM. */
void Gps_MagCalStart(void);
bool Gps_MagCalRunning(void);

const Gps_DataTypeDef *Gps_Get(void);

#endif /* GPS_ENABLE */

#endif /* INC_USER_GPS_H_ */
