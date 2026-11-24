/**
 ******************************************************************************
 *
 * @file        MG82F6D64_SPI_DRV.h
 *
 * @brief       This is the C code format driver file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.02
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2020 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par 		Disclaimer
 *		The Demo software is provided "AS IS"  without any warranty, either
 *		expressed or implied, including, but not limited to, the implied warranties
 *		of merchantability and fitness for a particular purpose.  The author will
 *		not be liable for any special, incidental, consequential or indirect
 *		damages due to loss of data or any other reason.
 *		These statements agree with the world wide and local dictated laws about
 *		authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef MG82F6D64_SPI_DRV_H
#define MG82F6D64_SPI_DRV_H

/**
*****************************************************************************
* @brief        SPI Enable Interrupt
* @details      SPI Enable Interrupt
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_IT_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_IT_Cmd(__STATE__)\
    __STATE__==0?(EIE1=EIE1&(~ESPI)):(EIE1=EIE1|(ESPI))
/**
*******************************************************************************
* @brief        SPI Mode Select
* @details      Set SPEN SSIG nSS MSTR
* @param[in]    __MODE__ :
*  @arg\b       SPI_DISABLE
*  @arg\b       SPI_SLAVE_SELECT_BY_nSS
*  @arg\b       SPI_SLAVE_SELECT_BY_MSTR
*  @arg\b       SPI_MASTER_SELECT_BY_nSS
*  @arg\b       SPI_MASTER_SELECT_BY_MSTR
* @return       None
* @note         If nSS pin is driven low, then MSTR will be cleared to ‘0’ by H/W automatically, and SPEN is cleared, MODF is set.
* @par         Example
* @code
               __DRV_SPI_Mode_Select(SPI_DISABLE)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_Mode_Select
#define SPI_DISABLE               0x00
#define SPI_SLAVE_SELECT_BY_nSS   0x01
#define SPI_SLAVE_SELECT_BY_MSTR  0x02
#define SPI_MASTER_SELECT_BY_nSS  0x03
#define SPI_MASTER_SELECT_BY_MSTR 0x04
///@endcond
#define __DRV_SPI_Mode_Select(__MODE__)\
    MWT(\
        __DRV_SPI_SetSPEN(__MODE__);\
        __DRV_SPI_SetSSIG(__MODE__);\
        __DRV_SPI_SetMSTR(__MODE__);\
    ;)
/**
*****************************************************************************
* @brief        SPI Mode Select
* @details      Set SPEN
* @param[in]    __MODE__ :
*  @arg\b       SPI_DISABLE
*  @arg\b       SPI_SLAVE_SELECT_BY_nSS
*  @arg\b       SPI_SLAVE_SELECT_BY_MSTR
*  @arg\b       SPI_MASTER_SELECT_BY_nSS
*  @arg\b       SPI_MASTER_SELECT_BY_MSTR
* @return       None
* @note         If nSS pin is driven low, then MSTR will be cleared to ‘0’ by H/W automatically, and SPEN is cleared, MODF is set.
* @par          Example
* @code
                __DRV_SPI_SetSPEN(SPI_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPEN(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SPCON=SPCON&(~SPEN)):\
        (__MODE__^0x01)==0?(SPCON=SPCON|(SPEN)):\
        (__MODE__^0x02)==0?(SPCON=SPCON|(SPEN)):\
        (__MODE__^0x03)==0?(SPCON=SPCON|(SPEN)):\
        (__MODE__^0x04)==0?(SPCON=SPCON|(SPEN)):_nop_();\
    ;)
/**
*****************************************************************************
* @brief        SPI Mode Select
* @details      Set SSIG
* @param[in]    __MODE__ :
*  @arg\b       SPI_DISABLE
*  @arg\b       SPI_SLAVE_SELECT_BY_nSS
*  @arg\b       SPI_SLAVE_SELECT_BY_MSTR
*  @arg\b       SPI_MASTER_SELECT_BY_nSS
*  @arg\b       SPI_MASTER_SELECT_BY_MSTR
* @return       None
* @note         If nSS pin is driven low, then MSTR will be cleared to ‘0’ by H/W automatically, and SPEN is cleared, MODF is set.
* @par          Example
* @code
                __DRV_SPI_SetSSIG(SPI_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSSIG(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SPCON=SPCON&(~SSIG)):\
        (__MODE__^0x01)==0?(SPCON=SPCON&(~SSIG)):\
        (__MODE__^0x02)==0?(SPCON=SPCON|(SSIG)):\
        (__MODE__^0x03)==0?(SPCON=SPCON&(~SSIG)):\
        (__MODE__^0x04)==0?(SPCON=SPCON|(SSIG)):_nop_();\
    ;)
/**
*****************************************************************************
* @brief        SPI Mode Select
* @details      Set MSTR
* @param[in]    __MODE__ :
*  @arg\b       SPI_DISABLE
*  @arg\b       SPI_SLAVE_SELECT_BY_nSS
*  @arg\b       SPI_SLAVE_SELECT_BY_MSTR
*  @arg\b       SPI_MASTER_SELECT_BY_nSS
*  @arg\b       SPI_MASTER_SELECT_BY_MSTR
* @return       None
* @note         If nSS pin is driven low, then MSTR will be cleared to ‘0’ by H/W automatically, and SPEN is cleared, MODF is set.
* @par          Example
* @code
                __DRV_SPI_SetMSTR(SPI_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetMSTR(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SPCON=SPCON&(~MSTR)):\
        (__MODE__^0x01)==0?(SPCON=SPCON&(~MSTR)):\
        (__MODE__^0x02)==0?(SPCON=SPCON&(~MSTR)):\
        (__MODE__^0x03)==0?(SPCON=SPCON|(MSTR)):\
        (__MODE__^0x04)==0?(SPCON=SPCON|(MSTR)):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        SPI Clock Rate Select(Master Mode)
* @details      Set SPR2 SPR1 SPR0
* @param[in]    __SELECT__ :
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_4
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_8
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_16
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_32
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_64
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_2
*  @arg\b       SPI_CLOCK_RATE_S0TOF_DIV_6
*  @arg\b       SPI_CLOCK_RATE_T0OF_DIV_6
* @return       None
* @note         None
* @par         Example
* @code
               __DRV_SPI_Clock_Rate_Select(SPI_CLOCK_RATE_SYSCLK_DIV_4)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_Clock_Rate_Select
#define SPI_CLOCK_RATE_SYSCLK_DIV_4  0x00
#define SPI_CLOCK_RATE_SYSCLK_DIV_8  0x01
#define SPI_CLOCK_RATE_SYSCLK_DIV_16 0x02
#define SPI_CLOCK_RATE_SYSCLK_DIV_32 0x03
#define SPI_CLOCK_RATE_SYSCLK_DIV_64 0x04
#define SPI_CLOCK_RATE_SYSCLK_DIV_2  0x05
#define SPI_CLOCK_RATE_S0TOF_DIV_6   0x06
#define SPI_CLOCK_RATE_T0OF_DIV_6    0x07
///@endcond
#define __DRV_SPI_Clock_Rate_Select(__SELECT__)\
    MWT(\
        __DRV_SPI_SetSPR2(__SELECT__);\
        __DRV_SPI_SetSPR1(__SELECT__);\
        __DRV_SPI_SetSPR0(__SELECT__);\
    ;)
/**
*****************************************************************************
* @brief        SPI Clock Rate Select
* @details      Set SPR2
* @param[in]    __SELECT__ :
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_4
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_8
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_16
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_32
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_64
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_2
*  @arg\b       SPI_CLOCK_RATE_S0TOF_DIV_6
*  @arg\b       SPI_CLOCK_RATE_T0OF_DIV_6
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPR2(SPI_CLOCK_RATE_SYSCLK_DIV_4)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPR2(__SELECT__)\
    MWT(\
        (__SELECT__^0x00)==0?(SPSTAT=SPSTAT&(~SPR2_QDOE)):\
        (__SELECT__^0x01)==0?(SPSTAT=SPSTAT&(~SPR2_QDOE)):\
        (__SELECT__^0x02)==0?(SPSTAT=SPSTAT&(~SPR2_QDOE)):\
        (__SELECT__^0x03)==0?(SPSTAT=SPSTAT&(~SPR2_QDOE)):\
        (__SELECT__^0x04)==0?(SPSTAT=SPSTAT|(SPR2_QDOE)):\
        (__SELECT__^0x05)==0?(SPSTAT=SPSTAT|(SPR2_QDOE)):\
        (__SELECT__^0x06)==0?(SPSTAT=SPSTAT|(SPR2_QDOE)):\
        (__SELECT__^0x07)==0?(SPSTAT=SPSTAT|(SPR2_QDOE)):_nop_();\
    ;)
/**
*****************************************************************************
* @brief        SPI Clock Rate Select
* @details      Set SPR1
* @param[in]    __SELECT__ :
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_4
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_8
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_16
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_32
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_64
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_2
*  @arg\b       SPI_CLOCK_RATE_S0TOF_DIV_6
*  @arg\b       SPI_CLOCK_RATE_T0OF_DIV_6
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPR1(SPI_CLOCK_RATE_SYSCLK_DIV_4)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPR1(__SELECT__)\
    MWT(\
        (__SELECT__^0x00)==0?(SPCON=SPCON&(~SPR1)):\
        (__SELECT__^0x01)==0?(SPCON=SPCON&(~SPR1)):\
        (__SELECT__^0x02)==0?(SPCON=SPCON|(SPR1)):\
        (__SELECT__^0x03)==0?(SPCON=SPCON|(SPR1)):\
        (__SELECT__^0x04)==0?(SPCON=SPCON&(~SPR1)):\
        (__SELECT__^0x05)==0?(SPCON=SPCON&(~SPR1)):\
        (__SELECT__^0x06)==0?(SPCON=SPCON|(SPR1)):\
        (__SELECT__^0x07)==0?(SPCON=SPCON|(SPR1)):_nop_();\
    ;)
/**
*****************************************************************************
* @brief        SPI Clock Rate Select
* @details      Set SPR0
* @param[in]    __SELECT__ :
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_4
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_8
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_16
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_32
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_64
*  @arg\b       SPI_CLOCK_RATE_SYSCLK_DIV_2
*  @arg\b       SPI_CLOCK_RATE_S0TOF_DIV_6
*  @arg\b       SPI_CLOCK_RATE_T0OF_DIV_6
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPR0(SPI_CLOCK_RATE_SYSCLK_DIV_4)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPR0(__SELECT__)\
    MWT(\
        (__SELECT__^0x00)==0?(SPCON=SPCON&(~SPR0)):\
        (__SELECT__^0x01)==0?(SPCON=SPCON|(SPR0)):\
        (__SELECT__^0x02)==0?(SPCON=SPCON&(~SPR0)):\
        (__SELECT__^0x03)==0?(SPCON=SPCON|(SPR0)):\
        (__SELECT__^0x04)==0?(SPCON=SPCON&(~SPR0)):\
        (__SELECT__^0x05)==0?(SPCON=SPCON|(SPR0)):\
        (__SELECT__^0x06)==0?(SPCON=SPCON&(~SPR0)):\
        (__SELECT__^0x07)==0?(SPCON=SPCON|(SPR0)):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        SPI Data Mode Select
* @details      Set CPOL CPHA
* @param[in]    __MODE__ :
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_RISING_TRAILING_EDGE_SAMPLE_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_FALLING_TRAILING_EDGE_SETUP_RISING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_FALLING_TRAILING_EDGE_SAMPLE_RISING
* @return       None
* @note         None
* @par         Example
* @code
               __DRV_SPI_Data_Mode_Select(SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_Data_Mode_Select
#define SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING 0x00
#define SPI_DATA_LEADING_EDGE_SETUP_RISING_TRAILING_EDGE_SAMPLE_FALLING 0x01
#define SPI_DATA_LEADING_EDGE_SAMPLE_FALLING_TRAILING_EDGE_SETUP_RISING 0x02
#define SPI_DATA_LEADING_EDGE_SETUP_FALLING_TRAILING_EDGE_SAMPLE_RISING 0x03
///@endcond
#define __DRV_SPI_Data_Mode_Select(__MODE__)\
    MWT(\
        __DRV_SPI_SetCPOL(__MODE__);\
        __DRV_SPI_SetCPHA(__MODE__);\
    ;)

/**
*****************************************************************************
* @brief        SPI Data Mode Select
* @details      Set CPOL
* @param[in]    __MODE__ :
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_RISING_TRAILING_EDGE_SAMPLE_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_FALLING_TRAILING_EDGE_SETUP_RISING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_FALLING_TRAILING_EDGE_SAMPLE_RISING
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetCPOL(SPI_DATA_LEADING_EDGE_SETUP_FALLING_TRAILING_EDGE_SAMPLE_RISING)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetCPOL(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SPCON=SPCON&(~CPOL)):\
        (__MODE__^0x01)==0?(SPCON=SPCON&(~CPOL)):\
        (__MODE__^0x02)==0?(SPCON=SPCON|(CPOL)):\
        (__MODE__^0x03)==0?(SPCON=SPCON|(CPOL)):_nop_();\
    ;)
/**
*****************************************************************************
* @brief        SPI Data Mode Select
* @details      Set CPHA
* @param[in]    __MODE__ :
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_RISING_TRAILING_EDGE_SAMPLE_FALLING
*  @arg\b       SPI_DATA_LEADING_EDGE_SAMPLE_FALLING_TRAILING_EDGE_SETUP_RISING
*  @arg\b       SPI_DATA_LEADING_EDGE_SETUP_FALLING_TRAILING_EDGE_SAMPLE_RISING
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetCPHA(SPI_DATA_LEADING_EDGE_SAMPLE_RISING_TRAILING_EDGE_SETUP_FALLING)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetCPHA(__MODE__)\
    MWT(\
        (__MODE__^0x00)==0?(SPCON=SPCON&(~CPHA)):\
        (__MODE__^0x01)==0?(SPCON=SPCON|(CPHA)):\
        (__MODE__^0x02)==0?(SPCON=SPCON&(~CPHA)):\
        (__MODE__^0x03)==0?(SPCON=SPCON|(CPHA)):_nop_();\
    ;)
/**
*******************************************************************************
* @brief        SPI Pin MUX Select
* @details      Set SIPS0
* @param[in]    __SELECT__ :
*  @arg\b       SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P42_MISO_SPID1_P41_SPICLK_P40_SPID2_P36_SPID3_P37
*  @arg\b       SPI_nSS_P40_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P43_SPID2_P65_SPID3_P57
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P40_SPID2_P65_SPID3_P57
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_PinMux_Select(SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_PinMux_Select
#define SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54 0x00
#define SPI_nSS_P43_MOSI_SPID0_P42_MISO_SPID1_P41_SPICLK_P40_SPID2_P36_SPID3_P37 0x01
#define SPI_nSS_P40_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P43_SPID2_P65_SPID3_P57 0x02
#define SPI_nSS_P43_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P40_SPID2_P65_SPID3_P57 0x03
///@endcond

#define __DRV_SPI_PinMux_Select(__SELECT__)\
    MWT(\
        __DRV_SPI_SetSPIPS1(__SELECT__);\
        __DRV_SPI_SetSPIPS0(__SELECT__);\
    ;)

/**
*******************************************************************************
* @brief        SPI Pin MUX Select
* @details      Set SIPS1
* @param[in]    __SELECT__ :
*  @arg\b       SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P42_MISO_SPID1_P41_SPICLK_P40_SPID2_P36_SPID3_P37
*  @arg\b       SPI_nSS_P40_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P43_SPID2_P65_SPID3_P57
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P40_SPID2_P65_SPID3_P57
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPIPS1(SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPIPS1(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        (__SELECT__^0x00)==0?(AUXR10=AUXR10&(~SPIPS1)):\
        (__SELECT__^0x01)==0?(AUXR10=AUXR10&(~SPIPS1)):\
        (__SELECT__^0x02)==0?(AUXR10=AUXR10|(SPIPS1)):\
        (__SELECT__^0x03)==0?(AUXR10=AUXR10|(SPIPS1)):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        SPI Pin MUX Select
* @details      Set SIPS0
* @param[in]    __SELECT__ :
*  @arg\b       SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P42_MISO_SPID1_P41_SPICLK_P40_SPID2_P36_SPID3_P37
*  @arg\b       SPI_nSS_P40_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P43_SPID2_P65_SPID3_P57
*  @arg\b       SPI_nSS_P43_MOSI_SPID0_P41_MISO_SPID1_P42_SPICLK_P40_SPID2_P65_SPID3_P57
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPIPS0(SPI_nSS_P14_MOSI_SPID0_P15_MISO_SPID1_P16_SPICLK_P17_SPID2_P53_SPID3_P54)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPIPS0(__SELECT__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        (__SELECT__^0x00)==0?(AUXR10=AUXR10&(~SPIPS0)):\
        (__SELECT__^0x01)==0?(AUXR10=AUXR10|(SPIPS0)):\
        (__SELECT__^0x02)==0?(AUXR10=AUXR10&(~SPIPS0)):\
        (__SELECT__^0x03)==0?(AUXR10=AUXR10|(SPIPS0)):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        SPI Data Order Select
* @details      Set DORD
* @param[in]    __ORDER__ :
*  @arg\b       SPI_DATA_ORDER_MSB
*  @arg\b       SPI_DATA_ORDER_LSB
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_DataOrder_Select(SPI_DATA_ORDER_MSB)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_DataOrder_Select
#define SPI_DATA_ORDER_MSB 0
#define SPI_DATA_ORDER_LSB 1
///@endcond
#define __DRV_SPI_DataOrder_Select(__ORDER__)\
    __ORDER__==0?(SPCON=SPCON&(~DORD)):(SPCON=SPCON|(DORD))
/**
*******************************************************************************
* @brief        SPI Model Control Enable Daisy-Chain Connection
* @details      Set SPI0M0
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_Model_Control_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_Model_Control_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR7_Page);\
        __STATE__==0?(AUXR7=AUXR7&(~SPI0M0)):(AUXR7=AUXR7|(SPI0M0));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        SPI SPIF Enable Auto-Cleared
* @details      Set SPFACE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SPIF_Auto_Cleared_Cmd(MW_DISABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SPIF_Auto_Cleared_Cmd(__STATE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR10_Page);\
        __STATE__==0?(AUXR10=AUXR10&(~SPFACE)):(AUXR10=AUXR10|(SPFACE));\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*******************************************************************************
* @brief        SPI Enable QPI Mode
* @details      Set QPIEN
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_QPI_Mode_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_QPI_Mode_Cmd(__STATE__)\
    MWT(\
        __STATE__==0?(SPSTAT=SPSTAT&(~QPIEN)):(SPSTAT=SPSTAT|(QPIEN));\
    ;)
/**
*******************************************************************************
* @brief        SPI Enable QPI Data Output
* @details      Set SPR2 or QDOE
* @param[in]    __STATE__ :
*  @arg\b       MW_DISABLE
*  @arg\b       MW_ENABLE
* @return       None
* @note         If QPI mode disable,set SPR2,SPI clock rate select (associated with SPR1 and SPR0).If QPI mode enable,set QDOE,QPI Data Output Enable.
* @par          Example
* @code
                __DRV_SPI_QPI_Data_Output_Cmd(MW_ENABLE)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_QPI_Data_Output_Cmd(__STATE__)\
    MWT(\
        __STATE__==0?(SPSTAT=SPSTAT&(~SPR2_QDOE)):(SPSTAT=SPSTAT|(SPR2_QDOE));\
    ;)

/**
*******************************************************************************
* @brief        SPI Connection Select
* @details      Set SPI0M0 QPIEN
* @param[in]    __SELECT__ :
*  @arg\b       SPI_CONNECTION_TYPICAL
*  @arg\b       SPI_CONNECTION_DAISY_CHAIN
*  @arg\b       SPI_CONNECTION_QPI_MODE
* @return       None
* @note         None
* @par         Example
* @code
               __DRV_SPI_Data_Mode_Select(SPI_CONNECTION_TYPICAL)
* @endcode
*******************************************************************************
*/
///@cond __DRV_SPI_Connection_Select
#define SPI_CONNECTION_TYPICAL 0
#define SPI_CONNECTION_DAISY_CHAIN 2
#define SPI_CONNECTION_QPI_MODE 1
///@endcond
#define __DRV_SPI_Connection_Select(__SELECT__)\
    MWT(\
        __DRV_SPI_SetSPI0M0(__SELECT__);\
        __DRV_SPI_SetQPIEN(__SELECT__);\
    ;)

/**
*****************************************************************************
* @brief        SPI Connection Select
* @details      Set SPI0M0
* @param[in]    __MODE__ :
*  @arg\b       SPI_CONNECTION_TYPICAL
*  @arg\b       SPI_CONNECTION_DAISY_CHAIN
*  @arg\b       SPI_CONNECTION_QPI_MODE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetSPI0M0(SPI_CONNECTION_TYPICAL)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetSPI0M0(__MODE__)\
    MWT(\
        __DRV_SFR_PageIndex(AUXR7_Page);\
        (__MODE__^0)==0?(AUXR7=AUXR7&(~SPI0M0)):\
        (__MODE__^1)==0?(AUXR7=AUXR7&(~SPI0M0)):\
        (__MODE__^2)==0?(AUXR7=AUXR7|(SPI0M0)):_nop_();\
        __DRV_SFR_PageIndex(0);\
    ;)
/**
*****************************************************************************
* @brief        SPI Connection Select
* @details      Set QPIEN
* @param[in]    __MODE__ :
*  @arg\b       SPI_CONNECTION_TYPICAL
*  @arg\b       SPI_CONNECTION_DAISY_CHAIN
*  @arg\b       SPI_CONNECTION_QPI_MODE
* @return       None
* @note         None
* @par          Example
* @code
                __DRV_SPI_SetQPIEN(SPI_CONNECTION_TYPICAL)
* @endcode
*******************************************************************************
*/
#define __DRV_SPI_SetQPIEN(__MODE__)\
    MWT(\
        (__MODE__^0)==0?(SPSTAT=SPSTAT&(~QPIEN)):\
        (__MODE__^1)==0?(SPSTAT=SPSTAT|(QPIEN)):\
        (__MODE__^2)==0?(SPSTAT=SPSTAT&(~QPIEN)):_nop_();\
    ;)

///@cond
bool DRV_SPI_GetSPIF(void);
void DRV_SPI_ClearSPIF(void);
bool DRV_SPI_GetWCOL(void);
void DRV_SPI_ClearWCOL(void);
bool DRV_SPI_GetTHRF(void);
bool DRV_SPI_GetSPIBSY(void);
bool DRV_SPI_GetMODF(void);
void DRV_SPI_ClearMODF(void);
void DRV_SPI_SendSPDAT(uint8_t TXData);
uint8_t DRV_SPI_ReceiveSPDAT(void);
//@endcond
#endif
