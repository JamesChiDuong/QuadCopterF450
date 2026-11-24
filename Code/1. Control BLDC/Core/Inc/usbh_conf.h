/*
 * usbh_conf.h
 *
 *  Configuration of the ST USB Host Library (Middlewares/ST/
 *  STM32_USB_Host_Library, v3.3.4) for the QuadCopter gamepad build.
 *  Low level glue: Core/Src/usbh_conf.c. Class driver: USER_Gamepad.c.
 */
/*
 * OVERVIEW
 *   The ST USB Host Library includes this file by name ("usbh_conf.h") to get
 *   its compile-time sizes and hooks. Every project using the library must
 *   provide its own copy. Notes on the values:
 *   - MAX_NUM_INTERFACES/ENDPOINTS: big enough for an XInput pad's full
 *     configuration descriptor (otherwise descriptor parsing is truncated).
 *   - KEEP_CFG_DESCRIPTOR = 1: keeps the raw config descriptor in
 *     phost->device.CfgDesc_Raw; USER_Gamepad.c needs it to find the HID
 *     descriptor (report descriptor length).
 *   - MAX_DATA_BUFFER 0x200 = 512 bytes: size of phost->device.Data, which
 *     also receives the HID report descriptor (so up to 512 bytes).
 *   - USBH_USE_OS = 0: the library does not create its own thread/queue;
 *     Gamepad_Task calls USBH_Process() itself every 1 ms.
 *   - Log macros are empty: the library prints nothing (saves flash and keeps
 *     the terminal clean); Gamepad_Task prints its own short messages.
 *   - USBH_malloc = malloc: the library itself does not allocate in this
 *     setup (class data is static in USER_Gamepad.c).
 */

#ifndef USBH_CONF_H
#define USBH_CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* XInput pads expose 4 interfaces, interface 1 has 4 endpoints */
#define USBH_MAX_NUM_ENDPOINTS                8U
#define USBH_MAX_NUM_INTERFACES               6U
#define USBH_MAX_NUM_CONFIGURATION            1U
#define USBH_KEEP_CFG_DESCRIPTOR              1U
#define USBH_MAX_NUM_SUPPORTED_CLASS          2U    /* XInput (0xFF) + HID (0x03) */
#define USBH_MAX_SIZE_CONFIGURATION           0x200U
#define USBH_MAX_DATA_BUFFER                  0x200U
#define USBH_DEBUG_LEVEL                      0U
#define USBH_USE_OS                           0U    /* polled from a FreeRTOS task */

#define HOST_HS                               0
#define HOST_FS                               1

#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy

#define USBH_UsrLog(...)          do { } while (0)
#define USBH_ErrLog(...)          do { } while (0)
#define USBH_DbgLog(...)          do { } while (0)

#endif /* USBH_CONF_H */
