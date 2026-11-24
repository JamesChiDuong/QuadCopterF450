/*
 * USER_CommFunc.h
 *
 *  Created on: Mar 8, 2025
 *      Author: PC
 */
/*
 * OVERVIEW
 *   Common helpers used everywhere: RTOSprintf (thread-safe printf to the
 *   terminal), RTOSmalloc (mutex-protected malloc), StopChar/ChangeChar
 *   (replace the first occurrence of a character in a string).
 *   S_2_mS converts seconds to milliseconds (mutex timeouts are in ms).
 */

#ifndef INC_USER_COMMFUNC_H_
#define INC_USER_COMMFUNC_H_
#include <main.h>

#define S_2_mS(S)				            ((S) * 1000) // Second to millisecond
void ChangeChar(char *pcStr, char cSource, char cTarget);
void StopChar(char *pcStr, char cEnd);
void RTOSprintf(char *format, ...);
void *RTOSmalloc(size_t size);
#endif /* INC_USER_COMMFUNC_H_ */
