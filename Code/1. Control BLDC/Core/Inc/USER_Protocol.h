/*
 * USER_Protocol.h
 *
 *  Terminal command protocol (USART1, 115200 8N1). Every command starts with
 *  "@QC+" and ends with "\r\n". Send "@QC+HELP" for the list.
 */
/*
 * OVERVIEW
 *   Protocl_ChkCmdPkg(line, NULL) is called by UART_TerminalProc() (UARTTask)
 *   with one complete "@QC+..." line; it parses it, runs the matching handler
 *   and prints OK/ERROR. Protocl_ProcCmd() does the parsing only. (The
 *   "Protocl" spelling is the original name; kept so other files still link.)
 *   The command list and "how to add a command" are in USER_Protocol.c.
 */

#ifndef INC_USER_PROTOCOL_H_
#define INC_USER_PROTOCOL_H_

#include <stdbool.h>

#define CMD_HEAD_STD "@QC+"

bool Protocl_ProcCmd(char *pStr);
bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg);

#endif /* INC_USER_PROTOCOL_H_ */
