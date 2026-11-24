#ifndef _MAIN_H
#define _MAIN_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
// MAIN_PWR_EN
#define PWR_EN P23
// Input key
#define KEY_1 P03
#define KEY_5 P04
// nG_SEN_INT
#define N_G_SEN_INT P11
// nACC_IN
#define N_ACC_IN P14
// n4G_RI
#define N_4G_RI P15
// MCU WKUP
#define MCU_WKUP P20
// nRESET_MCU
#define N_RESET_MCU P21
// IO simulation
#define IO_EN0 P23
#define IO_EN1 P24
#define IO_EN2 P25
#define IO_EN3 P01
// Test
#define TEST0 P27
#define TEST1 P26

// Power control
#define PWR_MAIN_EN IO_EN0
#define PWR_4G_EN IO_EN1
// WDT_EN
#define WDT_EN P35
// WDT enable function
#define WDT_EN_FUC P36

#define USB_PROTECT_SET P37
//============================================================================//
// Extern
//============================================================================//
extern char cFW_Ver[64];
extern xdata u8 u8ACC_ON_Cnt;
extern xdata WkupSourceTypeDef tWkupSrc;

#endif // _MAIN_H