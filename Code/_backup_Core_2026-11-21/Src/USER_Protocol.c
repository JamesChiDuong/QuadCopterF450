/*
 * USER_Protocol.c
 *
 *  Created on: Mar 8, 2025
 *      Author: PC
 */

#include <USER_Protocol.h>
char szCmdMsg[UART_BASE_BUFFER_SIZE];
#define STR_CMD_OK "\r\n@ Command OK !!\r\n"
#define STR_CMD_ERR "\r\n@ Command ERROR !!\r\n"

static const char *DebugModeString[] =
    {
        "OFF",
        "TIME",
        "DATA",
        "LOG",
        "GPS",
        "GPS1",
        "GPRS",
        "CAN",
        "DB9",
        "EXT",
        "GS",
        "ENG",
        "RESERVED1",
        "RESERVED2",
        "RESERVED3",
        "ALL"};

uint32_t const DebugModeValue[] =
    {
        0x00000000,
        0x00000001,
        0x00000002,
        0x00000004,
        0x00000008,
        0x00000010,
        0x00000020,
        0x00000040,
        0x00000080,
        0x00000100,
        0x00000200,
        0x00000400,
        0x00000800,
        0x00001000,
        0x00002000,
        0x00003FFF};
// @brief  Get the argument of a command.
// @param  pStr: Pointer to a string which stores the command.
// @retval Position where the argument of the command starts.
//============================================================================//
static uint8_t Protocl_CmdStopHead(char *pStr)
{
    StopChar(pStr, '=');

    return strlen(pStr) + 1;
}

bool CMD_ControlMotor(char *pStr)
{
    bool bResult = false;
    if (strncmp(pStr, STR_QC_CONTROL_MOTOR, strlen(STR_QC_CONTROL_MOTOR)) != 0)
        return false;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
        {
        case '\0':
            RTOSprintf("\r\n@ %s<Number of motor>,<Frequency>,<Duty cycle>", STR_QC_CONTROL_MOTOR);
            RTOSprintf("\r\n@ \t<Number of motor>: 0: Motor 1, 2: Motor 2, 3: Motor 3, 4: Motor 4");
            RTOSprintf("\r\n@ \t<Frequency>:Frequency of timer - default 50 Hz");
            RTOSprintf("\r\n@ \t<Dutycle>: Duty cycle. Range 0-100");
            bResult = true;

            break;

        case '?':
            snprintf(szCmdMsg,sizeof(szCmdMsg), "@\r\n@ \t<Number of motor>: %d\r\n@ \t<Frequency>: %ld (s)\r\n@ \t<Dutycle>: %d (s)\r\n",
					g_MotorParam.u8Motor,
                    g_MotorParam.u32Frequency,
                    g_MotorParam.u8DutyCle);
            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = true;

            break;
        default:
        {
        	StopChar(pStr, ',');

        	g_MotorParam.u8Motor = atoi(pStr);
        	if((g_MotorParam.u8Motor > 4) || (g_MotorParam.u8Motor <= 0))
        	{
        		RTOSprintf("\r\n%s", "Invalid motor");
        		break;
        	}

        	pStr += strlen(pStr) + 1;

        	StopChar(pStr, ',');

        	g_MotorParam.u32Frequency = atoi(pStr);

        	pStr += strlen(pStr) + 1;

        	g_MotorParam.u8DutyCle = atoi(pStr);

        	if((g_MotorParam.u8DutyCle > 100) || (g_MotorParam.u8DutyCle < 0))
        	{
        		RTOSprintf("\r\n%s", "Invalid duty cycle");
        		break;
        	}
            snprintf(szCmdMsg, sizeof(szCmdMsg),"@ %s%d%ld%d Set OK",
            		STR_QC_CONTROL_MOTOR,
					g_MotorParam.u8Motor,
					g_MotorParam.u32Frequency,
					g_MotorParam.u8DutyCle);

            RTOSprintf(szCmdMsg);
            Motor_SetParam(g_MotorParam);
            bResult = true;
        	break;
        }
      }
    return bResult;
}

bool CMD_StopMotor(char *pStr)
{
    bool bResult = false;
    if (strncmp(pStr, STR_QC_STOP_MOTOR, strlen(STR_QC_STOP_MOTOR)) != 0)
        return false;

    pStr += Protocl_CmdStopHead(pStr);

    switch (*pStr)
        {
        case '\0':
            RTOSprintf("\r\n@ %s<Number of motor>,<Frequency>,<Duty cycle>", STR_QC_STOP_MOTOR);
            RTOSprintf("\r\n@ \t<Number of motor>: 0: Motor 1, 2: Motor 2, 3: Motor 3, 4: Motor 4, 5: All motor");
            bResult = true;

            break;

        case '?':
            snprintf(szCmdMsg,sizeof(szCmdMsg), "@\r\n@ \tSTOP MOTOR");
            RTOSprintf("\r\n%s", szCmdMsg);

            bResult = true;

            break;
        default:
        {
        	StopChar(pStr, ',');

        	g_MotorParam.u8Motor = atoi(pStr);
        	if((g_MotorParam.u8Motor > 5) || (g_MotorParam.u8Motor <= 0))
        	{
        		RTOSprintf("\r\n%s", "Invalid motor");
        		break;
        	}
        	Motor_Stop(g_MotorParam);
            snprintf(szCmdMsg, sizeof(szCmdMsg),"@ %s%d Set OK",
            		STR_QC_STOP_MOTOR,
					g_MotorParam.u8Motor);

            RTOSprintf(szCmdMsg);
            bResult = true;
        	break;
        }
      }
    return bResult;
}

//============================================================================//
// @brief  Process an input command.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool Protocl_ProcCmd(char *pStr)
{
    StopChar(pStr, '\n');

    StopChar(pStr, '\r');

    memset(szCmdMsg, 0x00, sizeof(szCmdMsg));
    if (strncmp(pStr, CMD_HEAD_STD, strlen(CMD_HEAD_STD)) == 0)
    {
    	if(CMD_ControlMotor(pStr) == true)
    	{
    		return true;
    	}
    	if(CMD_StopMotor(pStr) == true)
    		return true;
    }
    return false;
}

//============================================================================//
// @brief  Process the command related to command testing.
// @param  pcCmdPkg: Pointer to a string storing a RS232 command.
//         pcCmdMsg: Pointer to a string buffer to store the returned message.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg)
{
    bool bResult;

    bResult = Protocl_ProcCmd(pcCmdPkg);

    if (pcCmdMsg != NULL)
    {
        strcpy(pcCmdMsg, szCmdMsg);
    }

    if (bResult == true)
        RTOSprintf(STR_CMD_OK);
    else
        RTOSprintf(STR_CMD_ERR);

    return bResult;
}
