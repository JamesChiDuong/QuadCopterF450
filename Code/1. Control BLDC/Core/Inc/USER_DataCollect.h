/*
 * USER_DataCollect.h
 *
 *  Telemetry / debug output on the terminal UART.
 */
/*
 * OVERVIEW
 *   Interface of the telemetry module (details and line formats in
 *   USER_DataCollect.c). The DebugModeTypeDef value is the number sent with
 *   @QC+DEBUG=<n>, so the order of the enum must not change.
 *   All functions are meant to be called from UARTTask.
 */

#ifndef INC_USER_DATACOLLECT_H_
#define INC_USER_DATACOLLECT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    DBG_MODE_OFF = 0,
    DBG_MODE_ATTITUDE,     /* format parsed by host/3D-Visualize.py */
    DBG_MODE_RC,
    DBG_MODE_MOTORS,
    DBG_MODE_PID_RP,
    DBG_MODE_PID_YAW,
    DBG_MODE_TLM,          /* "$TLM,..." parsed by host/drone_viewer.py */
    DBG_MODE_COUNT
} DebugModeTypeDef;

bool    Debug_SetMode(uint8_t mode);
uint8_t Debug_GetMode(void);

/* Call periodically from the UART task */
bool    Debug_Proc(void);

void    Debug_PrintStatus(void);
void    Debug_PrintGps(void);      /* GPS / heading / POSHOLD lines */

/* GAMEPAD build: print pad events (buttons, stick moves, plug/unplug).
 * Call often (every UART task loop). No-op in the other builds. */
void    Debug_PadEvents(void);
bool    Debug_SetPadLog(bool on);
bool    Debug_GetPadLog(void);

#endif /* INC_USER_DATACOLLECT_H_ */
