#ifndef _IAP_DRIVER_H
#define _IAP_DRIVER_H

//============================================================================//
// Include
//============================================================================//
#include "Config.h"
#include "RTC_Driver.h"
#include "Type.h"

//============================================================================//
// Define
//============================================================================//
#define IAPLB_ADDR 0x03 // byte, P page SFR, IAP lower boundary, addres=0x03
#define IAP_ADDR 0xF600 // default, 1 kbytes
#define IAP_ADDR_MSB (u8)(IAP_ADDR >> 8)
#define IAP_PAGE_NUM 4  // 1 kbytes
#define ISP_ADDR 0xFA00 // default, 1.5 kbytes, start from 0xFA00
#define ISP_PAGE_NUM 6  // 1.5 kbytes
#define NV_HEADER 0x57B4
#define NV_FOOTER 0x9ACC

//============================================================================//
// Typedef
//============================================================================//
typedef struct NV_SET // non-volatile memory
{
    u16 wHeader;
    u8 u8Baud[UART_NUM_MAX];
    u8 u8EnableMCU_WDT;
    u8 u8MCU_WDT_Reload;
    RTC_ALM_TypeDef tAlarm;
    RTC_PED_WKUP_TypeDef tPedWkup;
    u8 u8KeyResetEn;
    u8 u8Reserved1[7];
    u8 u8Reserved2[100];
    u16 wFooter;
} NV_SET;

//============================================================================//
// Extern
//============================================================================/
extern xdata NV_SET NV_Set;

//============================================================================//
// Function
//============================================================================//
u8 PageP_Read(u8 u8Addr);
void PageP_Write(u8 u8Addr, u8 u8Data);
void IAP_LB(void);
void IAP_RM(WORD wAddr, u8 u8PageNum);
void IAP_WR(WORD wAddr, BYTE *pData, int nBytes);
void IAP_RD(WORD wAddr, BYTE *pData, int nBytes);
void ClearNV(void);
void SaveNV(void);
void LoadNV(void);
#endif