/**
 ******************************************************************************
 *
 * @file        MG82F6D64_BEEPER_DRV.h
 *
 * @brief       This is the C code format driver head file.
 *
 * @par         Project
 *              MG82F6D64
 * @version     V1.01
 * @date        2022/06/09
 * @author      Megawin Software Center
 * @copyright   Copyright (c) 2019 MegaWin Technology Co., Ltd.
 *              All rights reserved.
 *
 ******************************************************************************
 * @par         Disclaimer
 *      The Demo software is provided "AS IS" without any warranty, either
 *      expressed or implied, including, but not limited to, the implied warranties
 *      of merchantability and fitness for a particular purpose.  The author will
 *      not be liable for any special, incidental, consequential or indirect
 *      damages due to loss of data or any other reason.
 *      These statements agree with the worldwide and local dictated laws about
 *      authorship and violence against these laws.
 ******************************************************************************
 ******************************************************************************
 */

#ifndef __MG82F6D64_BEEPER_DRV_H
#define __MG82F6D64_BEEPER_DRV_H

/// @cond Parameter_definition

//!@{
//! defgroup Beeper Modes
//Beeper P44 Function
#define P44_AS_IO 0
#define ILRCO_32  1
#define ILRCO_16  2
#define ILRCO_8   3
//!@}
/// @endcond

/**
 *******************************************************************************
 * @brief       Initializing beeper mode
 * @details
 * @param[in]   __MODE__ :  specifies the beeper mode.
 *  @arg\b      P44_AS_IO
 *  @arg\b      ILRCO_32
 *  @arg\b      ILRCO_16
 *  @arg\b      ILRCO_8
 * @return      None
 * @note
 * @par Example
 * @code
 *   __DRV_BEEPER_ModeSelect(ILRCO_32);
 * @endcode
 *******************************************************************************
 */
 #define __DRV_BEEPER_ModeSelect(__MODE__)\
     MWT(\
        AUXR3 &= ~(BPOC1|BPOC0);\
        AUXR3 |= (__MODE__) << 4 ;\
      )
#endif
