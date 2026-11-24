/*
 * USER_Protocol.h
 *
 *  Created on: Mar 8, 2025
 *      Author: PC
 */

#ifndef INC_USER_PROTOCOL_H_
#define INC_USER_PROTOCOL_H_

#include <main.h>

#define CMD_HEAD_STD "@QC+"
#define STR_QC_CONTROL_MOTOR "@QC+CONTROL_MOTOR="
#define STR_QC_STOP_MOTOR "@QC+STOP_MOTOR="
typedef enum
{
    DBG_OFF = 0,
    DBG_TIME,
    DBG_DATA,
    DBG_LOG,
    DBG_GPS,
    DBG_GPS1,
    DBG_GSM,
    DBG_CAN,
    DBG_DB9,
    DBG_EXT,
    DBG_GS,
    DBG_ENG,
    DBG_RESERVED1,
    DBG_RESERVED2,
    DBG_RESERVED3,
    DBG_ALL,
} DebugModeTypeDef;

bool CMD_ControlMotor(char *pStr);
bool CMD_StopMotor(char* pStr);
bool Protocl_ProcCmd(char *pStr);

bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg);
extern const uint32_t DebugModeValue[];
#endif /* INC_USER_PROTOCOL_H_ */
