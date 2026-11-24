/*
 * USER_CommFunc.c
 *
 *  Created on: Mar 8, 2025
 *      Author: PC
 */
#include <USER_CommFunc.h>
//============================================================================//
// @brief  Set the first occurence of a character in a string to null character.
// @param  pcStr: Pointer to the string.
//         cEnd: Character to be changed.
// @retval None.
//============================================================================//
void StopChar(char *pcStr, char cEnd)
{
    ChangeChar(pcStr, cEnd, '\0');
}
//============================================================================//
// @brief  Set the first occurence of a character in a string to
//         another character.
// @param  pcStr: Pointer to the string.
//         cSource: Character to be changed.
//         cTarget: Character for changing to.
// @retval None.
//============================================================================//
void ChangeChar(char *pcStr, char cSource, char cTarget)
{
    while (*pcStr)
    {
        if (*(pcStr) == cSource)
        {
            *(pcStr) = cTarget;

            return;
        }

        pcStr++;
    }
}


//============================================================================//
// @brief  Print a string with consideration to RTOS implementation.
// @param  format: formatted string for print.
//         ... varied length argument
// @retval None.
//============================================================================//
void RTOSprintf(char *format, ...)
{
    va_list arg;
    osStatus osRes;

    va_start(arg, format);
    if(bPrintfEn)
    {
        if (bRTOS_En)
        {
        	osRes = osMutexWait(mutex_uart_user[SET_COMPORT_DEF], S_2_mS(5));

            if (osRes != osOK)
            {
                va_end(arg);

                return;
            }
        }

        vfprintf(stdout, format, arg);

        if (bRTOS_En)
        {
        	osMutexRelease(mutex_uart_user[SET_COMPORT_DEF]);
        }
    }

    va_end(arg);

}
//============================================================================//
// @brief  malloc() with consideration of FreeRTOS implementation.
// @param  size: Number of bytes to allocate.
// @retval void pointer to the allocated memory block.
//============================================================================//
void *RTOSmalloc(size_t size)
{
    void *ptr = NULL;
    osStatus osRes;

    /*
    if(size > 0)
    {
        // We simply wrap the FreeRTOS call into a standard form
        if(bRTOS_En)
          ptr = pvPortMalloc(size);
          //ptr = malloc(size);
        else
            ptr = malloc(size);
    }
    */

    if (size == 0)
        return NULL;

    if (bRTOS_En)
    {
        osRes = osMutexWait(mutex_malloc_user, S_2_mS(10));

        if (osRes != osOK)
        {
            RTOSprintf("\r\n> RTOSmalloc wait mutex ERROR: %d", osRes);

            return NULL;
        }
    }

    ptr = malloc(size);

    if (bRTOS_En)
        osMutexRelease(mutex_malloc_user);

    return ptr;
}
