#ifndef _MC321X_DRIVER_H_
#define _MC321X_DRIVER_H_
//============================================================================//


//============================================================================//
// User Includes
//============================================================================//
#include "Config.h"
#include "DataType.h"
#include "main.h"


//============================================================================//
// User Defines
//============================================================================//
#if ENABLE_GS_MC3210
// MC321X I2C slave address
#define MC321X_SLAVE_ADDR                  0x4C // 01001100

// MC321X address when communicating using STM32 HAL I2C Driver
#define MC321X_ADDR_I2C                    MC321X_SLAVE_ADDR << 1 // 0x98 10011000

#define	MC321X_PCODE_ADDR3B				              (0x90)

#define	MC321X_ORIGIN_VALUE				              (512U)

#define	MC321X_METE_MA					                (3U)

//#define	READ_START_ADDR					                (0x0D)

#define	MC321X_ADDR_VALUE_MODE_0x07_STANDBY	    (0x03)

#define	MC321X_ADDR_VALUE_TAPEN_0x09			      (0x00)
#define	MC321X_ADDR_VALUE_TAPP_0x0A			        (0x00)

#define	MC321X_ADDR_VALUE_DROP_0x0B			        (0x00)

#define	MC321X_ADDR_VALUE_SHDB_0x0C			        (0x1F)

#define	MC321X_ADDR_VALUE_OUTCFG_0x20			      (0x33) // 2g range, 10bit resolution

#define	MC321X_ADDR_VALUE_SHAKE_TH_0x2B		      (0x44)
#define	MC321X_ADDR_VALUE_UD_Z_TH_0x2C			    (0x95)
#define	MC321X_ADDR_VALUE_RL_Z_TH_0x2E			    (0x95)

#define	MC321X_ADDR_VALUE_FB_Z_TH_0x30			    (0x40)
#define	MC321X_ADDR_VALUE_DROP_TH_0x31			    (0xAB)
#define	MC321X_ADDR_VALUE_TAP_TH_0x32			      (0x64)

#define	MC321X_ADDR_VALUE_MODE_0x07_WAKE		    (0x01)

#define MC321X_TIMEOUT                          100
#endif


//============================================================================//
// User Function Prototypes
//============================================================================//
#if ENABLE_GS_MC3210
void MC321X_Init(void);
void MC3210_Process(void);
#endif


//============================================================================//
#endif //_MC321X_DRIVER_H_
