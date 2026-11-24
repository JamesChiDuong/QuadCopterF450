//============================================================================//
// Include
//============================================================================//
#include "IAP_Driver.h"

#include <Intrins.h>

#include "API_Macro_MG82F6D64.H"
#include "Config.h"
#include "REG_MG82F6D64.H"
#include "UART_Driver.h"
//============================================================================//
// Define
//============================================================================//
// Define the MWT macro if not already defined

//============================================================================//
// Variable
//============================================================================//
xdata NV_SET NV_Set = {0};

//============================================================================//
// Function
//============================================================================//

//============================================================================//
// @brief  Read from Page P.
// @param  u8Addr: Address in memory to read from.
// @retval u8: Read data byte.
//============================================================================//
u8 PageP_Read(u8 u8Addr)
{
    xdata BYTE bData;
    bit bEA = EA;

    EA = 0;            // Turn off Interrupt
    ISPCR = 0x80;      // Enable ISP/IAP
    IFADRH = 0;        // IFADRH must be 0
    IFADRL = u8Addr;   // sfr Address
    IFMT = ISP_READ_P; // 5
    SCMD = 0x46;
    SCMD = 0xB9;
    // nop();
    IFMT = 0;
    ISPCR = 0; // clear
    EA = bEA;  // restore interrupt
    bData = IFD;
    return bData;
}

//============================================================================//
// @brief  Write to page P.
// @param  u8Addr: Address in memory to write.
//         u8Data: Data byte to write to memory.
// @retval None.
//============================================================================//
void PageP_Write(u8 u8Addr, u8 u8Data)
{
    bit bEA = EA;
    EA = 0;          // Turn off Interrupt
    ISPCR = 0x80;    // Enable ISP/IAP
    IFADRH = 0;      // IFADRH must be 0
    IFADRL = u8Addr; // sfr Address
    IFD = u8Data;    // sfr data
    IFMT = ISP_WRITE_P;
    SCMD = 0x46;
    SCMD = 0xB9;
    _nop_();
    IFMT = 0;
    ISPCR = 0; // clear
    EA = bEA;  // restore interrupt
}

//============================================================================//
// @brief  Set IAP low boundary.
// @param  None.
// @retval None.
//============================================================================//
void IAP_LB(void)
{
    BYTE xdata bIAPAddr[2] = {0};

    bIAPAddr[0] = PageP_Read(IAPLB_ADDR);
    if (bIAPAddr[0] != IAP_ADDR_MSB) // 0xF6
    {
        PageP_Write(IAPLB_ADDR, IAP_ADDR_MSB);
        bIAPAddr[1] = PageP_Read(IAPLB_ADDR);
    }
}

//============================================================================//
// @brief  Erase IAP memory.
// @param  wAddr: Erase start address in memory.
//         u8PageNum: Number of pages to erase.
// @retval None.
//============================================================================//
void IAP_RM(WORD wAddr, u8 u8PageNum)
{
    xdata u8 u8i;
    bit bEA = EA;

    EA = 0;           // Disable interrupt
    ISPCR = 0x80;     // Enable ISP/IAP
    IFMT = ISP_ERASE; // 0x03
    for (u8i = 0; u8i < u8PageNum; u8i++)
    {
        IFADRH = (u8)(wAddr >> 8) + u8i * 2; // IAP Address High byte
        IFADRL = 0x00;                       // IAP Address Low byte					//Turn off interrupt
        SCMD = 0x46;
        SCMD = 0xB9;
    }
    _nop_();
    IFMT = 0;
    ISPCR = 0; // clear
    EA = bEA;  // restore interrupt
}

//============================================================================//
// @brief  Write into IAP memory.
// @param  wAddr: Start address in memory to write to.
//         pData: Write data buffer.
//         nBytes: Write data length.
// @retval None.
//============================================================================//
void IAP_WR(WORD wAddr, BYTE *pData, int nBytes)
{
    int xdata i;
    bit bEA = EA;

    EA = 0;           // Turn off interrupt
    ISPCR = 0x80;     // ISPCR.7=1, enable ISP
    IFMT = ISP_WRITE; // 0x02;	//select program mode
    for (i = 0; i < nBytes; i++)
    {
        IFADRH = (BYTE)(wAddr >> 8);
        IFADRL = (BYTE)(wAddr & 0x00FF);
        IFD = pData[i];
        SCMD = 0x46; // trigger ISP processing
        SCMD = 0xB9;
        // nop();
        wAddr++;
    }
    IFMT = 0;
    ISPCR = 0;
    EA = bEA; // restore interrupt
}

//============================================================================//
// @brief  Read from IAP memory.
// @param  wAddr: Start address in memory to read from.
//         pData: Read data buffer.
//         nBytes: Read data length.
// @retval None.
//============================================================================//
void IAP_RD(WORD wAddr, BYTE *pData, int nBytes)
{
    int xdata i;
    bit bEA = EA;

    EA = 0;          // Turn off interrupt
    ISPCR = 0x80;    // ISPCR.7=1, enable ISP
    IFMT = ISP_READ; // 0x01;	//select read mode
    for (i = 0; i < nBytes; i++)
    {
        IFADRH = (wAddr >> 8);
        IFADRL = (BYTE)(wAddr & 0x00FF);
        SCMD = 0x46; // trigger ISP processing
        SCMD = 0xB9;
        // nop();
        pData[i] = IFD;
        wAddr++;
    }
    IFMT = 0;
    ISPCR = 0;
    pData[i] = 0;
    EA = bEA; // restore interrupt
}

//============================================================================//
// @brief  Clear NV backup data in IAP.
// @param  None
// @retval None.
//============================================================================//
void ClearNV(void)
{
    NV_Set.wHeader = NV_HEADER;
    NV_Set.wFooter = NV_FOOTER;
    NV_Set.u8Baud[0] = 24;      // 24*4800=115,200
    NV_Set.u8Baud[1] = 24;      // 24*4800=115,200
    NV_Set.u8Baud[2] = 24;      // 24*4800=115,200
    NV_Set.u8Baud[3] = 24;      // 24*4800=115,200
    NV_Set.u8EnableMCU_WDT = 1; // setting default to disable WDT
    NV_Set.u8MCU_WDT_Reload = 10;
    NV_Set.tAlarm.u8Enable = 0;
    NV_Set.tAlarm.u8Repeat = 0;
    NV_Set.tPedWkup.u8Enable = 0;
    NV_Set.tPedWkup.u32Timeout = 0;
    NV_Set.tPedWkup.u32Counter = 0;
    NV_Set.u8KeyResetEn = 1;
}

//============================================================================//
// @brief  Save NV backup data into IAP.
// @param  None
// @retval None.
//============================================================================//
void SaveNV(void)
{
    // if (memcmp(&NV_Set_Original, &NV_Set, sizeof(NV_Set)) == 0)
    IAP_RM(IAP_ADDR, 1); // 256 bytes = 1 page
    IAP_WR(IAP_ADDR, (BYTE *)&NV_Set, sizeof(NV_SET));
}

//============================================================================//
// @brief  Read NV backup data from IAP.
// @param  None
// @retval None.
//============================================================================//
void LoadNV(void)
{
    BYTE u8Valid = 1;
    IAP_RD(IAP_ADDR, (BYTE *)&NV_Set, sizeof(NV_SET));
    if ((NV_Set.wHeader != NV_HEADER) || (NV_Set.wFooter != NV_FOOTER))
        u8Valid = 0;
    if (u8Valid == 0)
    {
        ClearNV(); // change to default value
    }
}
