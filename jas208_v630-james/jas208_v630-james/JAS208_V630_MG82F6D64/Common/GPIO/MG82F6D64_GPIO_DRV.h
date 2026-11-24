/**
 ******************************************************************************
 *
 * @file        MG82F6D64_GPIO_DRV.H
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.10
 * @date        2022/03/28
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IF"  without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These statements agree with the world wide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef __MG82F6D64_GPIO_DRV_H
#define __MG82F6D64_GPIO_DRV_H

/*--------------------------------------------------------------
GPIO Function :
    1.Port4 Alternated Function Control
        |----P44 , P45 Function : __DRV_GPIO_SetOCD2IO(__STATE__);
        |----P47 Function       : __DRV_GPIO_SetRST2IO(__STATE__);
        __STATE__ :
            MW_ENABLE/MW_DISABLE

    2.GPIO Mode Control
        |----Portx Mode Assignment : __DRV_GPIO_PxModeSelect(__MODE__);     (x = 0~7)
        __MODE__ :
            P0ALL_InputOnly         P0x_InputOnly       (x = 0~7)
            P0ALL_OpenDrainPullUp   P0x_OpenDrainPullUp (x = 0~7)
            P0ALL_OpenDrain         P0x_OpenDrain       (x = 0~7)
            P0ALL_PushPull          P0x_PushPull        (x = 0~7)

            P1ALL_InputOnly         P1x_InputOnly       (x = 0~7)
            P1ALL_OpenDrainPullUp   P1x_OpenDrainPullUp (x = 0~7)
            P1ALL_OpenDrain         P1x_OpenDrain       (x = 0~7)
            P1ALL_PushPull          P1x_PushPull        (x = 0~7)

            P2ALL_InputOnly         P2x_InputOnly       (x = 0~7)
            P2ALL_OpenDrainPullUp   P2x_OpenDrainPullUp (x = 0~7)
            P2ALL_OpenDrain         P2x_OpenDrain       (x = 0~7)
            P2ALL_PushPull          P2x_PushPull        (x = 0~7)

            P3ALL_QuasiMode         P3x_QuasiMode       (x = 0~7)
            P3ALL_PushPull          P3x_PushPull        (x = 0~7)
            P3ALL_InputOnly         P3x_InputOnly       (x = 0~7)
            P3ALL_OpenDrain         P3x_OpenDrain       (x = 0~7)

            P4ALL_InputOnly         P4x_InputOnly       (x = 0~7)
            P4ALL_OpenDrainPullUp   P4x_OpenDrainPullUp (x = 0~7)
            P4ALL_OpenDrain         P4x_OpenDrain       (x = 0~7)
            P4ALL_PushPull          P4x_PushPull        (x = 0~7)

            P5ALL_InputOnly         P5x_InputOnly       (x = 0~7)
            P5ALL_OpenDrainPullUp   P5x_OpenDrainPullUp (x = 0~7)
            P5ALL_OpenDrain         P5x_OpenDrain       (x = 0~7)
            P5ALL_PushPull          P5x_PushPull        (x = 0~7)

            P6ALL_InputOnly         P6x_InputOnly       (x = 0~7)
            P6ALL_OpenDrainPullUp   P6x_OpenDrainPullUp (x = 0~7)
            P6ALL_OpenDrain         P6x_OpenDrain       (x = 0~7)
            P6ALL_PushPull          P6x_PushPull        (x = 0~7)

            P7ALL_InputOnly         P7x_InputOnly       (x = 0~2)
            P7ALL_OpenDrainPullUp   P7x_OpenDrainPullUp (x = 0~2)
            P7ALL_OpenDrain         P7x_OpenDrain       (x = 0~2)
            P7ALL_PushPull          P7x_PushPull        (x = 0~2)
        |----Portx Set Mode Register 0: __DRV_GPIO_SetPxM0(__MODE__);     (x = 0~7)
        |----Portx Get Mode Register 0: __DRV_GPIO_GetPxM0(__MODE__);     (x = 0~7)
        |----Portx Set Mode Register 1: __DRV_GPIO_SetPxM1(__MODE__);     (x = 0~7)
        |----Portx Get Mode Register 1: __DRV_GPIO_GetPxM1(__MODE__);     (x = 0~7)

    3.GPIO Output Driving Strength Control
        |----Portx Driving Strength Control : __DRV_GPIO_PxOutputDrivingConfig(__PORT__,__STATE__); (x = 0~7)
        __PORT__ :
            P00_to_P03_Driving / P04_to_P07_Driving
            P10_to_P13_Driving / P14_to_P17_Driving
            P20_to_P23_Driving / P24_to_P27_Driving
            P30_to_P33_Driving / P34_to_P37_Driving
            P40_to_P43_Driving / P44_to_P46_Driving
            P50_to_P53_Driving / P54_to_P57_Driving
            P62_to_P63_Driving / P64_to_P67_Driving
            P70_to_P72_Driving
        __STATE__ :
            HIGH / LOW
        |----Portx Set Driving Strength Register: __DRV_GPIO_SetPDRVCx(__MODE__);     (x = 0~1)
        |----Portx Get Driving Strength Register: __DRV_GPIO_GetPDRVCx(__MODE__);     (x = 0~1)

    4.GPIO Port Output Fast Driving Control
        |----Portx Fast Driving Control : __DRV_GPIO_SetPxFastDriving(__PORT__,__STATE__); (x = 0~7)
        __PORT__ :
            P0PinALL    P0Pinx  (x = 0~7)
            P1PinALL    P1Pinx  (x = 0~7)
            P2PinALL    P2Pinx  (x = 0~7)
            P3PinALL    P3Pinx  (x = 0~7)
            P4PinALL    P4Pinx  (x = 0~6)
            P5PinALL    P5Pinx  (x = 0~7)
            P6PinALL    P6Pinx  (x = 2~7)
        __STATE__ :
            FAST / NORMAL
        |----Portx Set Fast Drive Register: __DRV_GPIO_SetPxFDC(__MODE__);    (x = 0~7)
        |----Portx Get Fast Drive Register: __DRV_GPIO_GetPxFDC(__MODE__);    (x = 0~7)

    5.GPIO Port Protect
        |----Port4 : __DRV_GPIO_ProtectP4(__STATE__);
        |----Port6 : __DRV_GPIO_ProtectP6(__STATE__);
        __STATE__ :
            MW_ENABLE/MW_DISABLE

    *Other GPIO Function
        |----Default Init : __DRV_GPIO_PxDeInit(void);              (x = 0~7)
        |----Write        : __DRV_GPIO_WritePx(__PORT__, __VALUE__);(x = 0~7)
        |----Read         : __DRV_GPIO_ReadPx(__DATA__);            (x = 0~7)
        |----Inverse      : __DRV_GPIO_InversePinPx(__PORT__);  (x = 0~7)
--------------------------------------------------------------*/

/// @cond __DRV_GPIO_define

#define HIGH 0
#define LOW 1

#define FAST 1
#define NORMAL 0

//!@{
//! defgroup P0Pinx Pin number
#define P0PinALL (0xFF)
#define P0Pin0 (0x01)
#define P0Pin1 (0x02)
#define P0Pin2 (0x04)
#define P0Pin3 (0x08)
#define P0Pin4 (0x10)
#define P0Pin5 (0x20)
#define P0Pin6 (0x40)
#define P0Pin7 (0x80)
//!@}

//!@{
//! defgroup P1Pinx Pin number
#define P1PinALL (0xFF)
#define P1Pin0 (0x01)
#define P1Pin1 (0x02)
#define P1Pin2 (0x04)
#define P1Pin3 (0x08)
#define P1Pin4 (0x10)
#define P1Pin5 (0x20)
#define P1Pin6 (0x40)
#define P1Pin7 (0x80)
//!@}

//!@{
//! defgroup P2Pinx Pin number
#define P2PinALL (0xFF)
#define P2Pin0 (0x01)
#define P2Pin1 (0x02)
#define P2Pin2 (0x04)
#define P2Pin3 (0x08)
#define P2Pin4 (0x10)
#define P2Pin5 (0x20)
#define P2Pin6 (0x40)
#define P2Pin7 (0x80)
//!@}

//!@{
//! defgroup P3Pinx Pin number
#define P3PinALL (0xFF)
#define P3Pin0 (0x01)
#define P3Pin1 (0x02)
#define P3Pin2 (0x04)
#define P3Pin3 (0x08)
#define P3Pin4 (0x10)
#define P3Pin5 (0x20)
#define P3Pin6 (0x40)
#define P3Pin7 (0x80)
//!@}

//!@{
//! defgroup P4Pinx Pin number
#define P4PinALL (0xFF)
#define P4Pin0 (0x01)
#define P4Pin1 (0x02)
#define P4Pin2 (0x04)
#define P4Pin3 (0x08)
#define P4Pin4 (0x10)
#define P4Pin5 (0x20)
#define P4Pin6 (0x40)
#define P4Pin7 (0x80)
//!@}

//!@{
//! defgroup P5Pinx Pin number
#define P5PinALL (0xFF)
#define P5Pin0 (0x01)
#define P5Pin1 (0x02)
#define P5Pin2 (0x04)
#define P5Pin3 (0x08)
#define P5Pin4 (0x10)
#define P5Pin5 (0x20)
#define P5Pin6 (0x40)
#define P5Pin7 (0x80)
//!@}

//!@{
//! defgroup P6Pinx Pin number
#define P6PinALL (0xFF)
#define P6Pin0 (0x01)
#define P6Pin1 (0x02)
#define P6Pin2 (0x04)
#define P6Pin3 (0x08)
#define P6Pin4 (0x10)
#define P6Pin5 (0x20)
#define P6Pin6 (0x40)
#define P6Pin7 (0x80)
//!@}

//!@{
//! defgroup P7Pinx Pin number
#define P7PinALL (0x07)
#define P7Pin0 (0x01)
#define P7Pin1 (0x02)
#define P7Pin2 (0x04)
//!@}

// posite => high word , value => low word

//!@{
//! defgroup P0IOMODE Port0 IO mode
#define P0ALL_InputOnly (0xFFFF00FF)
#define P00_InputOnly (0x01010001)
#define P01_InputOnly (0x02020002)
#define P02_InputOnly (0x04040004)
#define P03_InputOnly (0x08080008)
#define P04_InputOnly (0x10100010)
#define P05_InputOnly (0x20200020)
#define P06_InputOnly (0x40400040)
#define P07_InputOnly (0x80800080)

#define P0ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P00_OpenDrainPullUp (0x01010101)
#define P01_OpenDrainPullUp (0x02020202)
#define P02_OpenDrainPullUp (0x04040404)
#define P03_OpenDrainPullUp (0x08080808)
#define P04_OpenDrainPullUp (0x10101010)
#define P05_OpenDrainPullUp (0x20202020)
#define P06_OpenDrainPullUp (0x40404040)
#define P07_OpenDrainPullUp (0x80808080)

#define P0ALL_OpenDrain (0xFFFF0000)
#define P00_OpenDrain (0x01010000)
#define P01_OpenDrain (0x02020000)
#define P02_OpenDrain (0x04040000)
#define P03_OpenDrain (0x08080000)
#define P04_OpenDrain (0x10100000)
#define P05_OpenDrain (0x20200000)
#define P06_OpenDrain (0x40400000)
#define P07_OpenDrain (0x80800000)

#define P0ALL_PushPull (0xFFFFFF00)
#define P00_PushPull (0x01010100)
#define P01_PushPull (0x02020200)
#define P02_PushPull (0x04040400)
#define P03_PushPull (0x08080800)
#define P04_PushPull (0x10101000)
#define P05_PushPull (0x20202000)
#define P06_PushPull (0x40404000)
#define P07_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P0IOMODE Port0 IO mode
#define P1ALL_InputOnly (0xFFFF00FF)
#define P10_InputOnly (0x01010001)
#define P11_InputOnly (0x02020002)
#define P12_InputOnly (0x04040004)
#define P13_InputOnly (0x08080008)
#define P14_InputOnly (0x10100010)
#define P15_InputOnly (0x20200020)
#define P16_InputOnly (0x40400040)
#define P17_InputOnly (0x80800080)

#define P1ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P10_OpenDrainPullUp (0x01010101)
#define P11_OpenDrainPullUp (0x02020202)
#define P12_OpenDrainPullUp (0x04040404)
#define P13_OpenDrainPullUp (0x08080808)
#define P14_OpenDrainPullUp (0x10101010)
#define P15_OpenDrainPullUp (0x20202020)
#define P16_OpenDrainPullUp (0x40404040)
#define P17_OpenDrainPullUp (0x80808080)

#define P1ALL_OpenDrain (0xFFFF0000)
#define P10_OpenDrain (0x01010000)
#define P11_OpenDrain (0x02020000)
#define P12_OpenDrain (0x04040000)
#define P13_OpenDrain (0x08080000)
#define P14_OpenDrain (0x10100000)
#define P15_OpenDrain (0x20200000)
#define P16_OpenDrain (0x40400000)
#define P17_OpenDrain (0x80800000)

#define P1ALL_PushPull (0xFFFFFF00)
#define P10_PushPull (0x01010100)
#define P11_PushPull (0x02020200)
#define P12_PushPull (0x04040400)
#define P13_PushPull (0x08080800)
#define P14_PushPull (0x10101000)
#define P15_PushPull (0x20202000)
#define P16_PushPull (0x40404000)
#define P17_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P2IOMODE Port0 IO mode
#define P2ALL_InputOnly (0xFFFF00FF)
#define P20_InputOnly (0x01010001)
#define P21_InputOnly (0x02020002)
#define P22_InputOnly (0x04040004)
#define P23_InputOnly (0x08080008)
#define P24_InputOnly (0x10100010)
#define P25_InputOnly (0x20200020)
#define P26_InputOnly (0x40400040)
#define P27_InputOnly (0x80800080)

#define P2ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P20_OpenDrainPullUp (0x01010101)
#define P21_OpenDrainPullUp (0x02020202)
#define P22_OpenDrainPullUp (0x04040404)
#define P23_OpenDrainPullUp (0x08080808)
#define P24_OpenDrainPullUp (0x10101010)
#define P25_OpenDrainPullUp (0x20202020)
#define P26_OpenDrainPullUp (0x40404040)
#define P27_OpenDrainPullUp (0x80808080)

#define P2ALL_OpenDrain (0xFFFF0000)
#define P20_OpenDrain (0x01010000)
#define P21_OpenDrain (0x02020000)
#define P22_OpenDrain (0x04040000)
#define P23_OpenDrain (0x08080000)
#define P24_OpenDrain (0x10100000)
#define P25_OpenDrain (0x20200000)
#define P26_OpenDrain (0x40400000)
#define P27_OpenDrain (0x80800000)

#define P2ALL_PushPull (0xFFFFFF00)
#define P20_PushPull (0x01010100)
#define P21_PushPull (0x02020200)
#define P22_PushPull (0x04040400)
#define P23_PushPull (0x08080800)
#define P24_PushPull (0x10101000)
#define P25_PushPull (0x20202000)
#define P26_PushPull (0x40404000)
#define P27_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P3IOMODE Port0 IO mode
#define P3ALL_QuasiMode (0xFFFF0000)
#define P30_QuasiMode (0x01010000)
#define P31_QuasiMode (0x02020000)
#define P32_QuasiMode (0x04040000)
#define P33_QuasiMode (0x08080000)
#define P34_QuasiMode (0x10100000)
#define P35_QuasiMode (0x20200000)
#define P36_QuasiMode (0x40400000)
#define P37_QuasiMode (0x80800000)

#define P3ALL_PushPull (0xFFFF00FF)
#define P30_PushPull (0x01010001)
#define P31_PushPull (0x02020002)
#define P32_PushPull (0x04040004)
#define P33_PushPull (0x08080008)
#define P34_PushPull (0x10100010)
#define P35_PushPull (0x20200020)
#define P36_PushPull (0x40400040)
#define P37_PushPull (0x80800080)

#define P3ALL_InputOnly (0xFFFFFF00)
#define P30_InputOnly (0x01010100)
#define P31_InputOnly (0x02020200)
#define P32_InputOnly (0x04040400)
#define P33_InputOnly (0x08080800)
#define P34_InputOnly (0x10101000)
#define P35_InputOnly (0x20202000)
#define P36_InputOnly (0x40404000)
#define P37_InputOnly (0x80808000)

#define P3ALL_OpenDrain (0xFFFFFFFF)
#define P30_OpenDrain (0x01010101)
#define P31_OpenDrain (0x02020202)
#define P32_OpenDrain (0x04040404)
#define P33_OpenDrain (0x08080808)
#define P34_OpenDrain (0x10101010)
#define P35_OpenDrain (0x20202020)
#define P36_OpenDrain (0x40404040)
#define P37_OpenDrain (0x80808080)
//!@}

//!@{
//! defgroup P4IOMODE Port0 IO mode
#define P4ALL_InputOnly (0xFFFF00FF)
#define P40_InputOnly (0x01010001)
#define P41_InputOnly (0x02020002)
#define P42_InputOnly (0x04040004)
#define P43_InputOnly (0x08080008)
#define P44_InputOnly (0x10100010)
#define P45_InputOnly (0x20200020)
#define P46_InputOnly (0x40400040)
#define P47_InputOnly (0x80800080)

#define P4ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P40_OpenDrainPullUp (0x01010101)
#define P41_OpenDrainPullUp (0x02020202)
#define P42_OpenDrainPullUp (0x04040404)
#define P43_OpenDrainPullUp (0x08080808)
#define P44_OpenDrainPullUp (0x10101010)
#define P45_OpenDrainPullUp (0x20202020)
#define P46_OpenDrainPullUp (0x40404040)
#define P47_OpenDrainPullUp (0x80808080)

#define P4ALL_OpenDrain (0xFFFF0000)
#define P40_OpenDrain (0x01010000)
#define P41_OpenDrain (0x02020000)
#define P42_OpenDrain (0x04040000)
#define P43_OpenDrain (0x08080000)
#define P44_OpenDrain (0x10100000)
#define P45_OpenDrain (0x20200000)
#define P46_OpenDrain (0x40400000)
#define P47_OpenDrain (0x80800000)

#define P4ALL_PushPull (0xFFFFFF00)
#define P40_PushPull (0x01010100)
#define P41_PushPull (0x02020200)
#define P42_PushPull (0x04040400)
#define P43_PushPull (0x08080800)
#define P44_PushPull (0x10101000)
#define P45_PushPull (0x20202000)
#define P46_PushPull (0x40404000)
#define P47_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P5IOMODE Port0 IO mode
#define P5ALL_InputOnly (0xFFFF00FF)
#define P50_InputOnly (0x01010001)
#define P51_InputOnly (0x02020002)
#define P52_InputOnly (0x04040004)
#define P53_InputOnly (0x08080008)
#define P54_InputOnly (0x10100010)
#define P55_InputOnly (0x20200020)
#define P56_InputOnly (0x40400040)
#define P57_InputOnly (0x80800080)

#define P5ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P50_OpenDrainPullUp (0x01010101)
#define P51_OpenDrainPullUp (0x02020202)
#define P52_OpenDrainPullUp (0x04040404)
#define P53_OpenDrainPullUp (0x08080808)
#define P54_OpenDrainPullUp (0x10101010)
#define P55_OpenDrainPullUp (0x20202020)
#define P56_OpenDrainPullUp (0x40404040)
#define P57_OpenDrainPullUp (0x80808080)

#define P5ALL_OpenDrain (0xFFFF0000)
#define P50_OpenDrain (0x01010000)
#define P51_OpenDrain (0x02020000)
#define P52_OpenDrain (0x04040000)
#define P53_OpenDrain (0x08080000)
#define P54_OpenDrain (0x10100000)
#define P55_OpenDrain (0x20200000)
#define P56_OpenDrain (0x40400000)
#define P57_OpenDrain (0x80800000)

#define P5ALL_PushPull (0xFFFFFF00)
#define P50_PushPull (0x01010100)
#define P51_PushPull (0x02020200)
#define P52_PushPull (0x04040400)
#define P53_PushPull (0x08080800)
#define P54_PushPull (0x10101000)
#define P55_PushPull (0x20202000)
#define P56_PushPull (0x40404000)
#define P57_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P6IOMODE Port0 IO mode
#define P6ALL_InputOnly (0xFFFF00FF)
#define P60_InputOnly (0x01010001)
#define P61_InputOnly (0x02020002)
#define P62_InputOnly (0x04040004)
#define P63_InputOnly (0x08080008)
#define P64_InputOnly (0x10100010)
#define P65_InputOnly (0x20200020)
#define P66_InputOnly (0x40400040)
#define P67_InputOnly (0x80800080)

#define P6ALL_OpenDrainPullUp (0xFFFFFFFF)
#define P60_OpenDrainPullUp (0x01010101)
#define P61_OpenDrainPullUp (0x02020202)
#define P62_OpenDrainPullUp (0x04040404)
#define P63_OpenDrainPullUp (0x08080808)
#define P64_OpenDrainPullUp (0x10101010)
#define P65_OpenDrainPullUp (0x20202020)
#define P66_OpenDrainPullUp (0x40404040)
#define P67_OpenDrainPullUp (0x80808080)

#define P6ALL_OpenDrain (0xFFFF0000)
#define P60_OpenDrain (0x01010000)
#define P61_OpenDrain (0x02020000)
#define P62_OpenDrain (0x04040000)
#define P63_OpenDrain (0x08080000)
#define P64_OpenDrain (0x10100000)
#define P65_OpenDrain (0x20200000)
#define P66_OpenDrain (0x40400000)
#define P67_OpenDrain (0x80800000)

#define P6ALL_PushPull (0xFFFFFF00)
#define P60_PushPull (0x01010100)
#define P61_PushPull (0x02020200)
#define P62_PushPull (0x04040400)
#define P63_PushPull (0x08080800)
#define P64_PushPull (0x10101000)
#define P65_PushPull (0x20202000)
#define P66_PushPull (0x40404000)
#define P67_PushPull (0x80808000)
//!@}

//!@{
//! defgroup P7IOMODE Port0 IO mode
#define P7ALL_InputOnly (0x07070007)
#define P70_InputOnly (0x01010001)
#define P71_InputOnly (0x02020002)
#define P72_InputOnly (0x04040004)

#define P7ALL_OpenDrainPullUp (0x07070707)
#define P70_OpenDrainPullUp (0x01010101)
#define P71_OpenDrainPullUp (0x02020202)
#define P72_OpenDrainPullUp (0x04040404)

#define P7ALL_OpenDrain (0x07070000)
#define P70_OpenDrain (0x01010000)
#define P71_OpenDrain (0x02020000)
#define P72_OpenDrain (0x04040000)

#define P7ALL_PushPull (0x07070700)
#define P70_PushPull (0x01010100)
#define P71_PushPull (0x02020200)
#define P72_PushPull (0x04040400)
//!@}

//!@{
//! defgroup Pack_Port_Pins
#define P00_to_P03_Driving P0DC0
#define P04_to_P07_Driving P0DC1
#define P10_to_P13_Driving P1DC0
#define P14_to_P17_Driving P1DC1
#define P20_to_P23_Driving P2DC0
#define P24_to_P27_Driving P2DC1
#define P30_to_P33_Driving P3DC0
#define P34_to_P37_Driving P3DC1
#define P40_to_P43_Driving P4DC0
#define P44_to_P46_Driving P4DC1
#define P50_to_P53_Driving P5DC0
#define P54_to_P57_Driving P5DC1
#define P62_to_P63_Driving P6DC0
#define P64_to_P67_Driving P6DC1
#define P70_to_P72_Driving P7DC0
//!@}
/// @endcond

///@cond
bool DRV_GPIO_GetPin60(void);
bool DRV_GPIO_GetPin61(void);
bool DRV_GPIO_GetPin62(void);
bool DRV_GPIO_GetPin63(void);
bool DRV_GPIO_GetPin64(void);
bool DRV_GPIO_GetPin65(void);
bool DRV_GPIO_GetPin66(void);
bool DRV_GPIO_GetPin67(void);

bool DRV_GPIO_GetPin70(void);
bool DRV_GPIO_GetPin71(void);
bool DRV_GPIO_GetPin72(void);
///@endcond

/**
 *******************************************************************************
 * @brief       Initializing Port0 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port0 mode.
 *  @arg\b      P0ALL_InputOnly (Default)
 *  @arg\b      P00_InputOnly
 *  @arg\b      P01_InputOnly
 *  @arg\b      P02_InputOnly
 *  @arg\b      P03_InputOnly
 *  @arg\b      P04_InputOnly
 *  @arg\b      P05_InputOnly
 *  @arg\b      P06_InputOnly
 *  @arg\b      P07_InputOnly
 *  @arg\b      P0ALL_OpenDrainPullUp
 *  @arg\b      P00_OpenDrainPullUp
 *  @arg\b      P01_OpenDrainPullUp
 *  @arg\b      P02_OpenDrainPullUp
 *  @arg\b      P03_OpenDrainPullUp
 *  @arg\b      P04_OpenDrainPullUp
 *  @arg\b      P05_OpenDrainPullUp
 *  @arg\b      P06_OpenDrainPullUp
 *  @arg\b      P07_OpenDrainPullUp
 *  @arg\b      P0ALL_OpenDrain
 *  @arg\b      P00_OpenDrain
 *  @arg\b      P01_OpenDrain
 *  @arg\b      P02_OpenDrain
 *  @arg\b      P03_OpenDrain
 *  @arg\b      P04_OpenDrain
 *  @arg\b      P05_OpenDrain
 *  @arg\b      P06_OpenDrain
 *  @arg\b      P07_OpenDrain
 *  @arg\b      P0ALL_PushPull
 *  @arg\b      P00_PushPull
 *  @arg\b      P01_PushPull
 *  @arg\b      P02_PushPull
 *  @arg\b      P03_PushPull
 *  @arg\b      P04_PushPull
 *  @arg\b      P05_PushPull
 *  @arg\b      P06_PushPull
 *  @arg\b      P07_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P0ModeSelect(P00_OpenDrainPullUp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P0ModeSelect(__MODE__)    \
    MWT(                                     \
        P0M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P0M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P0M1_Page);      \
        P0M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P0M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port0 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port0 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP0M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP0M0(__MODE__) P0M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port0 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port0 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP0M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP0M0(__MODE__) __MODE__ = P0M0

/**
 *******************************************************************************
 * @brief       Set Port0 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port0 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP0M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP0M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P0M1_Page); \
        P0M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port0 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port0 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP0M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP0M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P0M1_Page); \
        __MODE__ = P0M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Initializing Port1 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port1 mode.
 *  @arg\b      P1ALL_InputOnly (Default)
 *  @arg\b      P10_InputOnly
 *  @arg\b      P11_InputOnly
 *  @arg\b      P12_InputOnly
 *  @arg\b      P13_InputOnly
 *  @arg\b      P14_InputOnly
 *  @arg\b      P15_InputOnly
 *  @arg\b      P16_InputOnly
 *  @arg\b      P17_InputOnly
 *  @arg\b      P1ALL_OpenDrainPullUp
 *  @arg\b      P10_OpenDrainPullUp
 *  @arg\b      P11_OpenDrainPullUp
 *  @arg\b      P12_OpenDrainPullUp
 *  @arg\b      P13_OpenDrainPullUp
 *  @arg\b      P14_OpenDrainPullUp
 *  @arg\b      P15_OpenDrainPullUp
 *  @arg\b      P16_OpenDrainPullUp
 *  @arg\b      P17_OpenDrainPullUp
 *  @arg\b      P1ALL_OpenDrain
 *  @arg\b      P10_OpenDrain
 *  @arg\b      P11_OpenDrain
 *  @arg\b      P12_OpenDrain
 *  @arg\b      P13_OpenDrain
 *  @arg\b      P14_OpenDrain
 *  @arg\b      P15_OpenDrain
 *  @arg\b      P16_OpenDrain
 *  @arg\b      P17_OpenDrain
 *  @arg\b      P1ALL_PushPull
 *  @arg\b      P10_PushPull
 *  @arg\b      P11_PushPull
 *  @arg\b      P12_PushPull
 *  @arg\b      P13_PushPull
 *  @arg\b      P14_PushPull
 *  @arg\b      P15_PushPull
 *  @arg\b      P16_PushPull
 *  @arg\b      P17_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P1ModeSelect(P10_OpenDrainPullUp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P1ModeSelect(__MODE__)    \
    MWT(                                     \
        P1M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P1M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        P1M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P1M1 |= (LOBYTE(LOWORD(__MODE__)));)

/**
 *******************************************************************************
 * @brief       Set Port1 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port1 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP1M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP1M0(__MODE__) P1M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port1 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port1 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP1M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP1M0(__MODE__) __MODE__ = P1M0

/**
 *******************************************************************************
 * @brief       Set Port1 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port1 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP1M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP1M1(__MODE__) P1M1 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port1 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port1 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP1M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP1M1(__MODE__) __MODE__ = P1M1

/**
 *******************************************************************************
 * @brief       Initializing Port2 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port2 mode.
 *  @arg\b      P2ALL_InputOnly (Default)
 *  @arg\b      P20_InputOnly
 *  @arg\b      P21_InputOnly
 *  @arg\b      P22_InputOnly
 *  @arg\b      P23_InputOnly
 *  @arg\b      P24_InputOnly
 *  @arg\b      P25_InputOnly
 *  @arg\b      P26_InputOnly
 *  @arg\b      P27_InputOnly
 *  @arg\b      P2ALL_OpenDrainPullUp
 *  @arg\b      P20_OpenDrainPullUp
 *  @arg\b      P21_OpenDrainPullUp
 *  @arg\b      P22_OpenDrainPullUp
 *  @arg\b      P23_OpenDrainPullUp
 *  @arg\b      P24_OpenDrainPullUp
 *  @arg\b      P25_OpenDrainPullUp
 *  @arg\b      P26_OpenDrainPullUp
 *  @arg\b      P27_OpenDrainPullUp
 *  @arg\b      P2ALL_OpenDrain
 *  @arg\b      P20_OpenDrain
 *  @arg\b      P21_OpenDrain
 *  @arg\b      P22_OpenDrain
 *  @arg\b      P23_OpenDrain
 *  @arg\b      P24_OpenDrain
 *  @arg\b      P25_OpenDrain
 *  @arg\b      P26_OpenDrain
 *  @arg\b      P27_OpenDrain
 *  @arg\b      P2ALL_PushPull
 *  @arg\b      P20_PushPull
 *  @arg\b      P21_PushPull
 *  @arg\b      P22_PushPull
 *  @arg\b      P23_PushPull
 *  @arg\b      P24_PushPull
 *  @arg\b      P25_PushPull
 *  @arg\b      P26_PushPull
 *  @arg\b      P27_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P2ModeSelect(P20_OpenDrainPullUp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P2ModeSelect(__MODE__)    \
    MWT(                                     \
        P2M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P2M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P2M1_Page);      \
        P2M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P2M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port2 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port2 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP2M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP2M0(__MODE__) P2M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port2 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port2 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP2M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP2M0(__MODE__) __MODE__ = P2M0

/**
 *******************************************************************************
 * @brief       Set Port2 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port2 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP2M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP2M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P2M1_Page); \
        P2M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port2 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port2 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP2M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP2M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P2M1_Page); \
        __MODE__ = P2M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Initializing Port3 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port3 mode.
 *  @arg\b      P3ALL_QuasiMode (Default)
 *  @arg\b      P30_QuasiMode
 *  @arg\b      P31_QuasiMode
 *  @arg\b      P32_QuasiMode
 *  @arg\b      P33_QuasiMode
 *  @arg\b      P34_QuasiMode
 *  @arg\b      P35_QuasiMode
 *  @arg\b      P36_QuasiMode
 *  @arg\b      P37_QuasiMode
 *  @arg\b      P3ALL_PushPull
 *  @arg\b      P30_PushPull
 *  @arg\b      P31_PushPull
 *  @arg\b      P32_PushPull
 *  @arg\b      P33_PushPull
 *  @arg\b      P34_PushPull
 *  @arg\b      P35_PushPull
 *  @arg\b      P36_PushPull
 *  @arg\b      P37_PushPull
 *  @arg\b      P3ALL_InputOnly
 *  @arg\b      P30_InputOnly
 *  @arg\b      P31_InputOnly
 *  @arg\b      P32_InputOnly
 *  @arg\b      P33_InputOnly
 *  @arg\b      P34_InputOnly
 *  @arg\b      P35_InputOnly
 *  @arg\b      P36_InputOnly
 *  @arg\b      P37_InputOnly
 *  @arg\b      P3ALL_OpenDrain
 *  @arg\b      P30_OpenDrain
 *  @arg\b      P31_OpenDrain
 *  @arg\b      P32_OpenDrain
 *  @arg\b      P33_OpenDrain
 *  @arg\b      P34_OpenDrain
 *  @arg\b      P34_OpenDrain
 *  @arg\b      P34_OpenDrain
 *  @arg\b      P34_OpenDrain
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P3ModeSelect(P30_QuasiMode);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P3ModeSelect(__MODE__)    \
    MWT(                                     \
        P3M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P3M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        P3M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P3M1 |= (LOBYTE(LOWORD(__MODE__)));)

/**
 *******************************************************************************
 * @brief       Set Port3 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port3 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP3M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP3M0(__MODE__) P3M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port3 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port3 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP3M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP3M0(__MODE__) __MODE__ = P3M0

/**
 *******************************************************************************
 * @brief       Set Port3 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port3 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP3M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP3M1(__MODE__) P3M1 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port3 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port3 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP3M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP3M1(__MODE__) __MODE__ = P3M1

/**
 *******************************************************************************
 * @brief       Initializing Port4 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port4 mode.
 *  @arg\b      P4ALL_InputOnly (Default)
 *  @arg\b      P40_InputOnly
 *  @arg\b      P41_InputOnly
 *  @arg\b      P42_InputOnly
 *  @arg\b      P43_InputOnly
 *  @arg\b      P44_InputOnly
 *  @arg\b      P45_InputOnly
 *  @arg\b      P46_InputOnly
 *  @arg\b      P47_InputOnly
 *  @arg\b      P4ALL_OpenDrainPullUp
 *  @arg\b      P40_OpenDrainPullUp
 *  @arg\b      P41_OpenDrainPullUp
 *  @arg\b      P42_OpenDrainPullUp
 *  @arg\b      P43_OpenDrainPullUp
 *  @arg\b      P44_OpenDrainPullUp
 *  @arg\b      P45_OpenDrainPullUp
 *  @arg\b      P46_OpenDrainPullUp
 *  @arg\b      P47_OpenDrainPullUp
 *  @arg\b      P4ALL_OpenDrain
 *  @arg\b      P40_OpenDrain
 *  @arg\b      P41_OpenDrain
 *  @arg\b      P42_OpenDrain
 *  @arg\b      P43_OpenDrain
 *  @arg\b      P44_OpenDrain
 *  @arg\b      P45_OpenDrain
 *  @arg\b      P46_OpenDrain
 *  @arg\b      P47_OpenDrain
 *  @arg\b      P4ALL_PushPull
 *  @arg\b      P40_PushPull
 *  @arg\b      P41_PushPull
 *  @arg\b      P42_PushPull
 *  @arg\b      P43_PushPull
 *  @arg\b      P44_PushPull
 *  @arg\b      P45_PushPull
 *  @arg\b      P46_PushPull
 *  @arg\b      P47_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P4ModeSelect(P40_InputOnly);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P4ModeSelect(__MODE__)    \
    MWT(                                     \
        P4M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P4M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P4M1_Page);      \
        P4M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P4M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port4 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port4 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP4M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP4M0(__MODE__) P4M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port4 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port4 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP4M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP4M0(__MODE__) __MODE__ = P4M0

/**
 *******************************************************************************
 * @brief       Set Port4 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port4 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP4M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP4M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P4M1_Page); \
        P4M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port4 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port4 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP4M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP4M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P4M1_Page); \
        __MODE__ = P4M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Initializing Port5 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port4 mode.
 *  @arg\b      P5ALL_InputOnly (Default)
 *  @arg\b      P50_InputOnly
 *  @arg\b      P51_InputOnly
 *  @arg\b      P52_InputOnly
 *  @arg\b      P53_InputOnly
 *  @arg\b      P54_InputOnly
 *  @arg\b      P55_InputOnly
 *  @arg\b      P56_InputOnly
 *  @arg\b      P57_InputOnly
 *  @arg\b      P5ALL_OpenDrainPullUp
 *  @arg\b      P50_OpenDrainPullUp
 *  @arg\b      P51_OpenDrainPullUp
 *  @arg\b      P52_OpenDrainPullUp
 *  @arg\b      P53_OpenDrainPullUp
 *  @arg\b      P54_OpenDrainPullUp
 *  @arg\b      P55_OpenDrainPullUp
 *  @arg\b      P56_OpenDrainPullUp
 *  @arg\b      P57_OpenDrainPullUp
 *  @arg\b      P5ALL_OpenDrain
 *  @arg\b      P50_OpenDrain
 *  @arg\b      P51_OpenDrain
 *  @arg\b      P52_OpenDrain
 *  @arg\b      P53_OpenDrain
 *  @arg\b      P54_OpenDrain
 *  @arg\b      P55_OpenDrain
 *  @arg\b      P56_OpenDrain
 *  @arg\b      P57_OpenDrain
 *  @arg\b      P5ALL_PushPull
 *  @arg\b      P50_PushPull
 *  @arg\b      P51_PushPull
 *  @arg\b      P52_PushPull
 *  @arg\b      P53_PushPull
 *  @arg\b      P54_PushPull
 *  @arg\b      P55_PushPull
 *  @arg\b      P56_PushPull
 *  @arg\b      P57_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P5ModeSelect(P50_InputOnly);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P5ModeSelect(__MODE__)    \
    MWT(                                     \
        P5M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P5M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P5M1_Page);      \
        P5M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P5M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port5 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port5 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP5M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP5M0(__MODE__) P5M0 = __MODE__

/**
 *******************************************************************************
 * @brief       Get Port5 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port5 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP5M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP5M0(__MODE__) __MODE__ = P5M0

/**
 *******************************************************************************
 * @brief       Set Port5 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port5 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP5M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP5M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P5M1_Page); \
        P5M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port5 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port5 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP5M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP5M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P5M1_Page); \
        __MODE__ = P5M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Default initial Port6 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port6 mode.
 *  @arg\b      P6ALL_InputOnly (Default)
 *  @arg\b      P60_InputOnly
 *  @arg\b      P61_InputOnly
 *  @arg\b      P62_InputOnly
 *  @arg\b      P63_InputOnly
 *  @arg\b      P64_InputOnly
 *  @arg\b      P65_InputOnly
 *  @arg\b      P66_InputOnly
 *  @arg\b      P67_InputOnly
 *  @arg\b      P6ALL_OpenDrainPullUp
 *  @arg\b      P60_OpenDrainPullUp
 *  @arg\b      P61_OpenDrainPullUp
 *  @arg\b      P62_OpenDrainPullUp
 *  @arg\b      P63_OpenDrainPullUp
 *  @arg\b      P64_OpenDrainPullUp
 *  @arg\b      P65_OpenDrainPullUp
 *  @arg\b      P66_OpenDrainPullUp
 *  @arg\b      P67_OpenDrainPullUp
 *  @arg\b      P6ALL_OpenDrain
 *  @arg\b      P60_OpenDrain
 *  @arg\b      P61_OpenDrain
 *  @arg\b      P62_OpenDrain
 *  @arg\b      P63_OpenDrain
 *  @arg\b      P64_OpenDrain
 *  @arg\b      P65_OpenDrain
 *  @arg\b      P66_OpenDrain
 *  @arg\b      P67_OpenDrain
 *  @arg\b      P6ALL_PushPull
 *  @arg\b      P60_PushPull
 *  @arg\b      P61_PushPull
 *  @arg\b      P62_PushPull
 *  @arg\b      P63_PushPull
 *  @arg\b      P64_PushPull
 *  @arg\b      P65_PushPull
 *  @arg\b      P66_PushPull
 *  @arg\b      P67_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P6ModeSelect(P60_InputOnly);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P6ModeSelect(__MODE__)    \
    MWT(                                     \
        __DRV_SFR_PageIndex(P6M0_Page);      \
        P6M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P6M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P6M1_Page);      \
        P6M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P6M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port6 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port6 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP6M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP6M0(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P6M0_Page); \
        P6M0 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port6 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port6 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP6M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP6M0(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P6M0_Page); \
        __MODE__ = P6M0;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port6 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port6 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP6M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP6M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P6M1_Page); \
        P6M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port6 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port6 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP6M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP6M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P6M1_Page); \
        __MODE__ = P6M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Default initial Port7 IO mode
 * @details
 * @param[in]   __MODE__ :  specifies the Port6 mode.
 *  @arg\b      P7ALL_InputOnly (Default)
 *  @arg\b      P70_InputOnly
 *  @arg\b      P71_InputOnly
 *  @arg\b      P72_InputOnly
 *  @arg\b      P7ALL_OpenDrainPullUp
 *  @arg\b      P70_OpenDrainPullUp
 *  @arg\b      P71_OpenDrainPullUp
 *  @arg\b      P72_OpenDrainPullUp
 *  @arg\b      P7ALL_OpenDrain
 *  @arg\b      P70_OpenDrain
 *  @arg\b      P71_OpenDrain
 *  @arg\b      P72_OpenDrain
 *  @arg\b      P7ALL_PushPull
 *  @arg\b      P70_PushPull
 *  @arg\b      P71_PushPull
 *  @arg\b      P72_PushPull
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P7ModeSelect(P70_InputOnly);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P7ModeSelect(__MODE__)    \
    MWT(                                     \
        __DRV_SFR_PageIndex(P7M0_Page);      \
        P7M0 &= ~(HIBYTE(HIWORD(__MODE__))); \
        P7M0 |= (HIBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(P7M1_Page);      \
        P7M1 &= ~(LOBYTE(HIWORD(__MODE__))); \
        P7M1 |= (LOBYTE(LOWORD(__MODE__)));  \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port7 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Set the Port7 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP7M0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP7M0(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P7M0_Page); \
        P7M0 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port7 Mode Register 0
 * @details
 * @param[in]   __MODE__ :  Get the Port7 mode register 0.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP7M0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP7M0(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P7M0_Page); \
        __MODE__ = P7M0;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port7 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Set the Port7 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP7M1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP7M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P7M1_Page); \
        P7M1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port7 Mode Register 1
 * @details
 * @param[in]   __MODE__ :  Get the Port7 mode register 1.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP7M1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP7M1(__MODE__)    \
    MWT(                                \
        __DRV_SFR_PageIndex(P7M1_Page); \
        __MODE__ = P7M1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Default initial Port0 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P0DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P0DeInit() \
    MWT(                      \
        __DRV_GPIO_P0ModeSelect(P0ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port1 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P1DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P1DeInit() \
    MWT(                      \
        __DRV_GPIO_P1ModeSelect(P1ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port2 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P2DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P2DeInit() \
    MWT(                      \
        __DRV_GPIO_P2ModeSelect(P2ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port3 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P3DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P3DeInit() \
    MWT(                      \
        __DRV_GPIO_P3ModeSelect(P3ALL_QuasiMode);)

/**
 *******************************************************************************
 * @brief       Default initial Port4 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P4DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P4DeInit() \
    MWT(                      \
        __DRV_GPIO_P4ModeSelect(P4ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port5 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P5DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P5DeInit() \
    MWT(                      \
        __DRV_GPIO_P5ModeSelect(P5ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port6 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P6DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P6DeInit() \
    MWT(                      \
        __DRV_GPIO_P6ModeSelect(P6ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Default initial Port7 IO mode
 * @details
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P7DeInit();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P7DeInit() \
    MWT(                      \
        __DRV_GPIO_P7ModeSelect(P7ALL_InputOnly);)

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port0 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP0(P00,1);
 *   __DRV_GPIO_WriteP0(P0,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP0(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port1 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP1(P10,1);
 *   __DRV_GPIO_WriteP1(P1,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP1(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port2 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP2(P20,1);
 *   __DRV_GPIO_WriteP2(P2,0x04);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP2(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port3 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP3(P30,1);
 *   __DRV_GPIO_WriteP3(P3,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP3(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port4 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP4(P45,1);
 *   __DRV_GPIO_WriteP4(P4,0x10);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP4(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port5 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP5(P50,1);
 *   __DRV_GPIO_WriteP5(P5,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP5(__PORT__, __VALUE__) __PORT__ = __VALUE__

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port6 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP6(P60,1);
 *   __DRV_GPIO_WriteP6(P6,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP6(__PORT__, __VALUE__) \
    MWT(                                        \
        __DRV_SFR_PageIndex(P6_Page);           \
        __PORT__ = __VALUE__;                   \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Writing GPIO's Port7 state.
 * @details
 * @param[in]   __PORT__ : specifies a whole port or port pins.
 * @param[in]   __VALUE__ : specifies the value to be written.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_WriteP7(P70,1);
 *   __DRV_GPIO_WriteP7(P7,0x01);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_WriteP7(__PORT__, __VALUE__) \
    MWT(                                        \
        __DRV_SFR_PageIndex(P7_Page);           \
        __PORT__ = __VALUE__;                   \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return      Port state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP0(__DATA__) __DATA__ = P0

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin00();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin00() P00

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin01();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin01() P01

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin02();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin02() P02

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin03();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin03() P03

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin04();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin04() P04

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin05();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin05() P05

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin06();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin06() P06

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port0 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin07();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin07() P07

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return      Port state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP1(__DATA__) __DATA__ = P1

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin10();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin10() P10

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin11();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin11() P11

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin12();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin12() P12

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin13();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin13() P13

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin14();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin14() P14

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin15();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin15() P15

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin16();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin16() P16

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port1 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin17();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin17() P17

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return      Port state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP2(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP2(__DATA__) __DATA__ = P2

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin20();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin20() P20

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin21();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin21() P21

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin22();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin22() P22

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin23();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin23() P23

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin24();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin24() P24

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin25();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin25() P25

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin26();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin26() P26

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port2 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin27();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin27() P27

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return      Port state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP3(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP3(__DATA__) __DATA__ = P3

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin30();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin30() P30

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin31();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin31() P31

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin32();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin32() P32

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin33();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin33() P33

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin34();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin34() P34

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin35();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin35() P35

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin36();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin36() P36

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port3 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin37();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin37() P37

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_Read(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP4(__DATA__) __DATA__ = P4

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin40();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin40() P40

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin41();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin41() P41

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin42();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin42() P42

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin43();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin43() P43

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin44();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin44() P44

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin45();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin45() P45

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin46();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin46() P46

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port4 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin47();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin47() P47

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP5(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP5(__DATA__) __DATA__ = P5

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin0 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin50();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin50() P50

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin1 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin51();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin51() P51

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin2 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin52();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin52() P52

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin3 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin53();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin53() P53

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin4 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin54();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin54() P54

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin5 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin55();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin55() P55

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin6 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin56();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin56() P56

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port5 Pin7 state.
 * @details
 * @return      Pin state
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   tmp = __DRV_GPIO_GetPin57();
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPin57() P57

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port6 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP6(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP6(__DATA__)   \
    MWT(                              \
        __DRV_SFR_PageIndex(P6_Page); \
        __DATA__ = P6;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Reading GPIO's Port7 state.
 * @details
 * @param[out]   __DATA__ : the place to store data.
 * @return
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_ReadP7(P7);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_ReadP7(__DATA__)   \
    MWT(                              \
        __DRV_SFR_PageIndex(P7_Page); \
        __DATA__ = P7;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Inversing Port0 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P0
 *  @arg\b      P00
 *  @arg\b      P01
 *  @arg\b      P02
 *  @arg\b      P03
 *  @arg\b      P04
 *  @arg\b      P05
 *  @arg\b      P06
 *  @arg\b      P07
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P0_InversePinP0(P00);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP0(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port1 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P1
 *  @arg\b      P10
 *  @arg\b      P11
 *  @arg\b      P12
 *  @arg\b      P13
 *  @arg\b      P14
 *  @arg\b      P15
 *  @arg\b      P16
 *  @arg\b      P17
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P1_InversePinP1(P10);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP1(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port2 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P2
 *  @arg\b      P20
 *  @arg\b      P21
 *  @arg\b      P22
 *  @arg\b      P23
 *  @arg\b      P24
 *  @arg\b      P25
 *  @arg\b      P26
 *  @arg\b      P27
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P2_InversePinP2(P20);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP2(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port3 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P3
 *  @arg\b      P30
 *  @arg\b      P31
 *  @arg\b      P32
 *  @arg\b      P33
 *  @arg\b      P34
 *  @arg\b      P35
 *  @arg\b      P36
 *  @arg\b      P37
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P3_InversePinP3(P30);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP3(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port4 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P4
 *  @arg\b      P40
 *  @arg\b      P41
 *  @arg\b      P42
 *  @arg\b      P43
 *  @arg\b      P44
 *  @arg\b      P45
 *  @arg\b      P46
 *  @arg\b      P47
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P4_InversePinP4(P44);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP4(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port5 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P5
 *  @arg\b      P50
 *  @arg\b      P51
 *  @arg\b      P52
 *  @arg\b      P53
 *  @arg\b      P54
 *  @arg\b      P55
 *  @arg\b      P56
 *  @arg\b      P57
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P5_InversePinP5(P50);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP5(__PORT__) __PORT__ = ~(__PORT__)

/**
 *******************************************************************************
 * @brief       Inversing Port6 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P6
 *  @arg\b      P60
 *  @arg\b      P61
 *  @arg\b      P62
 *  @arg\b      P63
 *  @arg\b      P64
 *  @arg\b      P65
 *  @arg\b      P66
 *  @arg\b      P67
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P6_InversePinP6(P60);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP6(__PORT__) \
    MWT(                                  \
        __DRV_SFR_PageIndex(P6_Page);     \
        __PORT__ = ~(__PORT__);           \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Inversing Port7 pin state.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P7
 *  @arg\b      P70
 *  @arg\b      P71
 *  @arg\b      P72
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_P7_InversePinP7(P70);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_InversePinP7(__PORT__) \
    MWT(                                  \
        __DRV_SFR_PageIndex(P7_Page);     \
        __PORT__ = ~(__PORT__);           \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port0 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P00_to_P03_Driving
 *  @arg\b      P04_to_P07_Driving
 * @param[in]   __STATE__ :
 *  @arg\b      HIGH : specifies P0 with high driving current strength.
 *  @arg\b      LOW : specifies P0 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P00_to_P03_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P04_to_P07_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P0OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC0_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC0 |= __PORT__) : (PDRVC0 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port1 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P10_to_P13_Driving
 *  @arg\b      P14_to_P17_Driving
 * @param[in]   __STATE__ :
 *  @arg\b      HIGH : specifies P1 with high driving current strength.
 *  @arg\b      LOW : specifies P1 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P10_to_P13_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P14_to_P17_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P1OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC0_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC0 |= __PORT__) : (PDRVC0 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port2 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P20_to_P23_Driving
 *  @arg\b      P24_to_P27_Driving
 * @param[in]   __STATE__ :
 *  @arg\b      HIGH : specifies P2 with high driving current strength.
 *  @arg\b      LOW : specifies P2 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P20_to_P23_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P24_to_P27_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P2OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC0_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC0 |= __PORT__) : (PDRVC0 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port3 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P30_to_P33_Driving
 *  @arg\b      P34_to_P37_Driving
 * @param[in]   __STATE__ : enables high driving or low driving.
 *  @arg\b      HIGH : specifies P3 with high driving current strength.
 *  @arg\b      LOW : specifies P3 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P30_to_P33_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P30_to_P33_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P3OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC0_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC0 |= __PORT__) : (PDRVC0 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port0/1/2/3 driving current strength register
 * @details
 * @param[in]   __MODE__ :  Set Port0/1/2/3 driving current strength register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetPDRVC0(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetPDRVC0(__MODE__)    \
    MWT(                                  \
        __DRV_SFR_PageIndex(PDRVC0_Page); \
        PDRVC0 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port0/1/2/3 driving current strength register
 * @details
 * @param[in]   __MODE__ :  Get Port0/1/2/3 driving current strength register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetPDRVC0(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPDRVC0(__MODE__)    \
    MWT(                                  \
        __DRV_SFR_PageIndex(PDRVC0_Page); \
        __MODE__ = PDRVC0;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port4 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P40_to_P43_Driving
 *  @arg\b      P44_to_P46_Driving
 * @param[in]   __STATE__ : enables high driving or low driving.
 *  @arg\b      HIGH : specifies P4 with high driving current strength.
 *  @arg\b      LOW : specifies P4 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P44_to_P46_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P44_to_P46_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P4OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC1_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC1 |= __PORT__) : (PDRVC1 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port5 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P50_to_P53_Driving
 *  @arg\b      P54_to_P57_Driving
 * @param[in]   __STATE__ : enables high driving or low driving.
 *  @arg\b      HIGH : specifies P5 with high driving current strength.
 *  @arg\b      LOW : specifies P5 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P50_to_P53_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P50_to_P53_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P5OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC1_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC1 |= __PORT__) : (PDRVC1 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port6 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P62_to_P63_Driving
 *  @arg\b      P64_to_P67_Driving
 * @param[in]   __STATE__ : enables high driving or low driving.
 *  @arg\b      HIGH : specifies P6 with high driving current strength.
 *  @arg\b      LOW : specifies P6 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P62_to_P63_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P62_to_P63_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P6OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC1_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC1 |= __PORT__) : (PDRVC1 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Selecting Port7 driving current strength.
 * @details
 * @param[in]   __PORT__ : specifies port pins
 *  @arg\b      P70_to_P72_Driving
 * @param[in]   __STATE__ : enables high driving or low driving.
 *  @arg\b      HIGH : specifies P7 with high driving current strength.
 *  @arg\b      LOW : specifies P7 with low driving current strength.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_OutputDrivingConfig(P70_to_P72_Driving, HIGH);
 *   __DRV_GPIO_OutputDrivingConfig(P70_to_P72_Driving, LOW);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P7OutputDrivingConfig(__PORT__, __STATE__)              \
    MWT(                                                                   \
        __DRV_SFR_PageIndex(PDRVC1_Page);                                  \
        (__STATE__ == LOW) ? (PDRVC1 |= __PORT__) : (PDRVC1 &= ~__PORT__); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port4/5/6/7 driving current strength register
 * @details
 * @param[in]   __MODE__ :  Set Port4/5/6/7 driving current strength register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetPDRVC1(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetPDRVC1(__MODE__)    \
    MWT(                                  \
        __DRV_SFR_PageIndex(PDRVC1_Page); \
        PDRVC1 = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port4/5/6/7 driving current strength register
 * @details
 * @param[in]   __MODE__ :  Get Port4/5/6/7 driving current strength register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetPDRVC1(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetPDRVC1(__MODE__)    \
    MWT(                                  \
        __DRV_SFR_PageIndex(PDRVC1_Page); \
        __MODE__ = PDRVC1;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port0 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P0PinALL
 *  @arg\b      P0Pin0
 *  @arg\b      P0Pin1
 *  @arg\b      P0Pin2
 *  @arg\b      P0Pin3
 *  @arg\b      P0Pin4
 *  @arg\b      P0Pin5
 *  @arg\b      P0Pin6
 *  @arg\b      P0Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP0FastDriving(P0PinALL, NORMAL);
 *   __DRV_GPIO_SetP0FastDriving(P0PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP0FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P0FDC_Page);                                    \
        __STATE__ == FAST ? (P0FDC |= (__PORT__)) : (P0FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port0 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port0 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP0FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP0FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P0FDC_Page); \
        P0FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port0 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port0 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP0FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP0FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P0FDC_Page); \
        __MODE__ = P0FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port1 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P1PinALL
 *  @arg\b      P1Pin0
 *  @arg\b      P1Pin1
 *  @arg\b      P1Pin2
 *  @arg\b      P1Pin3
 *  @arg\b      P1Pin4
 *  @arg\b      P1Pin5
 *  @arg\b      P1Pin6
 *  @arg\b      P1Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP1FastDriving(P1PinALL, NORMAL);
 *   __DRV_GPIO_SetP1FastDriving(P1PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP1FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P1FDC_Page);                                    \
        __STATE__ == FAST ? (P1FDC |= (__PORT__)) : (P1FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port1 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port1 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP1FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP1FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P1FDC_Page); \
        P1FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port1 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port1 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP1FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP1FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P1FDC_Page); \
        __MODE__ = P1FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port2 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P2PinALL
 *  @arg\b      P2Pin0
 *  @arg\b      P2Pin1
 *  @arg\b      P2Pin2
 *  @arg\b      P2Pin3
 *  @arg\b      P2Pin4
 *  @arg\b      P2Pin5
 *  @arg\b      P2Pin6
 *  @arg\b      P2Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP2FastDriving(P2PinALL, NORMAL);
 *   __DRV_GPIO_SetP2FastDriving(P2PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP2FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P2FDC_Page);                                    \
        __STATE__ == FAST ? (P2FDC |= (__PORT__)) : (P2FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port2 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port2 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP2FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP2FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P2FDC_Page); \
        P2FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port2 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port2 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP2FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP2FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P2FDC_Page); \
        __MODE__ = P2FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port3 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P3PinALL
 *  @arg\b      P3Pin0
 *  @arg\b      P3Pin1
 *  @arg\b      P3Pin2
 *  @arg\b      P3Pin3
 *  @arg\b      P3Pin4
 *  @arg\b      P3Pin5
 *  @arg\b      P3Pin6
 *  @arg\b      P3Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP3FastDriving(P3PinALL, NORMAL);
 *   __DRV_GPIO_SetP3FastDriving(P3PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP3FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P3FDC_Page);                                    \
        __STATE__ == FAST ? (P3FDC |= (__PORT__)) : (P3FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port3 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port3 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP3FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP3FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P3FDC_Page); \
        P3FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port3 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port3 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP3FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP3FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P3FDC_Page); \
        __MODE__ = P3FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port4 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P4PinALL
 *  @arg\b      P4Pin0
 *  @arg\b      P4Pin1
 *  @arg\b      P4Pin2
 *  @arg\b      P4Pin3
 *  @arg\b      P4Pin4
 *  @arg\b      P4Pin5
 *  @arg\b      P4Pin6
 *  @arg\b      P4Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP4FastDriving(P4PinALL, NORMAL);
 *   __DRV_GPIO_SetP4FastDriving(P4PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP4FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P4FDC_Page);                                    \
        __STATE__ == FAST ? (P4FDC |= (__PORT__)) : (P4FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port4 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port4 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP4FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP4FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P4FDC_Page); \
        P4FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port4 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port4 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP4FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP4FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P4FDC_Page); \
        __MODE__ = P4FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port5 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P5PinALL
 *  @arg\b      P5Pin0
 *  @arg\b      P5Pin1
 *  @arg\b      P5Pin2
 *  @arg\b      P5Pin3
 *  @arg\b      P5Pin4
 *  @arg\b      P5Pin5
 *  @arg\b      P5Pin6
 *  @arg\b      P5Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP4FastDriving(P5PinALL, NORMAL);
 *   __DRV_GPIO_SetP4FastDriving(P5PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP5FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P5FDC_Page);                                    \
        __STATE__ == FAST ? (P5FDC |= (__PORT__)) : (P5FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port5 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port5 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP5FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP5FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P5FDC_Page); \
        P5FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port5 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port5 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP5FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP5FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P5FDC_Page); \
        __MODE__ = P5FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port6 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P6PinALL
 *  @arg\b      P6Pin2
 *  @arg\b      P6Pin3
 *  @arg\b      P6Pin4
 *  @arg\b      P6Pin5
 *  @arg\b      P6Pin6
 *  @arg\b      P6Pin7
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP6FastDriving(P6PinALL, NORMAL);
 *   __DRV_GPIO_SetP6FastDriving(P6PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP6FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P6FDC_Page);                                    \
        __STATE__ == FAST ? (P6FDC |= (__PORT__)) : (P6FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port6 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port6 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP6FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP6FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P6FDC_Page); \
        P6FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port6 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port6 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP6FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP6FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P6FDC_Page); \
        __MODE__ = P6FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Port7 pin output fast driving control.
 * @details
 * @param[in]   __PORT__ : port pin to enable fast driving.
 *  @arg\b      P7PinALL
 *  @arg\b      P7Pin0
 *  @arg\b      P7Pin1
 *  @arg\b      P7Pin2
 * @param[in]   __STATE__ : enables/disables fast driving.
 *  @arg\b      NORMAL : disables fast driving on port pin output. (Default)
 *  @arg\b      FAST : enables fast driving on port pin output.
 * @return  None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP7FastDriving(P7PinALL, NORMAL);
 *   __DRV_GPIO_SetP7FastDriving(P7PinALL, FAST);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP7FastDriving(__PORT__, __STATE__)                    \
    MWT(                                                                    \
        __DRV_SFR_PageIndex(P7FDC_Page);                                    \
        __STATE__ == FAST ? (P7FDC |= (__PORT__)) : (P7FDC &= ~(__PORT__)); \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Set Port7 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Set Port7 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetP7FDC(0xFF);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetP7FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P7FDC_Page); \
        P7FDC = __MODE__;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       Get Port7 pin output fast driving control register.
 * @details
 * @param[in]   __MODE__ :  Get Port7 pin output fast driving control register.
 * @return      None
 * @note
 * @par Example
 * @code
 *   unsigned char tmp = 0;
 *   __DRV_GPIO_GetP7FDC(tmp);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_GetP7FDC(__MODE__)    \
    MWT(                                 \
        __DRV_SFR_PageIndex(P7FDC_Page); \
        __MODE__ = P7FDC;                \
        __DRV_SFR_PageIndex(0);)

/**
 *******************************************************************************
 * @brief       OCD enable.
 * @details
 * @param[in]   __STATE__ : enables/disables OCD interface on P44 and P45
 *  @arg\b      MW_ENABLE : enables OCD interface on P44 and P45. (Default)
 *  @arg\b      MW_DISABLE : disables OCD interface on P44 and P45.
 * @return None
 * @note
 * @par Example
 * @code
 *   __DRV_GPIO_SetOCD2IO(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetOCD2IO(__STATE__)                               \
    MWT(                                                              \
        DRV_PageP_Read(DCON0_P);                                      \
        __STATE__ == MW_DISABLE ? (IFD &= ~OCDE_P) : (IFD |= OCDE_P); \
        DRV_PageP_Write(DCON0_P, IFD);)

/**
 *******************************************************************************
 * @brief       Reset function on I/O.
 * @details
 * @param[in]   __STATE__ :
 *  @arg\b      MW_ENABLE : Selects I/O pad function for external reset input, RST. (Default)
 *  @arg\b      MW_DISABLE : Selects I/O pad function for P47.
 * @return      None
 * @note
 * @code
 *   __DRV_GPIO_SetRST2IO(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_SetRST2IO(__STATE__)                                 \
    MWT(                                                                \
        DRV_PageP_Read(DCON0_P);                                        \
        __STATE__ == MW_DISABLE ? (IFD &= ~RSTIO_P) : (IFD |= RSTIO_P); \
        DRV_PageP_Write(DCON0_P, IFD);)

/**
 *******************************************************************************
 * @brief       P4 SFR access control.
 * @details
 * @param[in]   __STATE__ : enables/disables P4 SFR protection.
 *  @arg\b      MW_ENABLE : it will disable the P4 SFR modifications in page 0~F. P4 in page 0~F only keeps SFR read function.
 *                      But software always owns the modification capability in SFR Page P.
 *  @arg\b      MW_DISABLE : modifications and read functions are both available in page 0~F and Page P. (Default)
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P4_Protected( MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P4_Protected(__STATE__)                             \
    MWT(                                                               \
        DRV_PageP_Read(SPCON0_P);                                      \
        __STATE__ == MW_ENABLE ? (IFD |= P4CTL_P) : (IFD &= ~P4CTL_P); \
        DRV_PageP_Write(SPCON0_P, IFD);)

/**
 *******************************************************************************
 * @brief       P6 SFR access control.
 * @details
 * @param[in]   __STATE__ : enables/disables P6 SFR protection.
 *  @arg\b      MW_ENABLE : it will disable the P6 SFR modifications in page 0~F. P6 in page 0~F only keeps SFR read function.
 *                      But software always owns the modification capability in SFR Page P.
 *  @arg\b      MW_DISABLE : modifications and read functions are both available in page 0~F and Page P. (Default)
 * @return      None
 * @note
 * @par Example
 * @code
 *  __DRV_GPIO_P6_Protected(MW_ENABLE);
 * @endcode
 *******************************************************************************
 */
#define __DRV_GPIO_P6_Protected(__STATE__)                             \
    MWT(                                                               \
        DRV_PageP_Read(SPCON0_P);                                      \
        __STATE__ == MW_ENABLE ? (IFD |= P6CTL_P) : (IFD &= ~P6CTL_P); \
        DRV_PageP_Write(SPCON0_P, IFD);)

/**
 *******************************************************************************
 * void function
 *******************************************************************************
 */
bool DRV_GPIO_GetPin60(void);
bool DRV_GPIO_GetPin61(void);
bool DRV_GPIO_GetPin62(void);
bool DRV_GPIO_GetPin63(void);
bool DRV_GPIO_GetPin64(void);
bool DRV_GPIO_GetPin65(void);
bool DRV_GPIO_GetPin66(void);
bool DRV_GPIO_GetPin67(void);
bool DRV_GPIO_GetPin70(void);
bool DRV_GPIO_GetPin71(void);
bool DRV_GPIO_GetPin72(void);


#endif
