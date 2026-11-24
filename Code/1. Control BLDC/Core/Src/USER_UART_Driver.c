/*
 * UART_Driver.c
 *
 *  Created on: Mar 1, 2025
 *      Author: PC
 */
/*
 * OVERVIEW
 *   UART layer of the terminal (only UART_PORT1 = USART1 is used: 115200 8N1,
 *   reaches the PC through the ST-LINK virtual COM port).
 *
 *   Receive path (byte by byte, interrupt driven):
 *     1. UART_Setup(SET_COMPORT_DEF) (called from USER_System.c at start-up)
 *        allocates the ring buffer and enables the RXNE ("RX register not
 *        empty") and ERR interrupts of USART1.
 *     2. Every received byte raises USART1_IRQHandler (stm32f4xx_it.c,
 *        priority 5). It reads the byte and calls UART_SaveRecvData(), which
 *        appends it to the ring buffer rbBuf[UART_PORT1] (moves the head).
 *     3. UARTTask calls UART_TerminalProc() every 10 ms. It looks for a
 *        complete frame "@QC+" ... "\r\n" in the buffered bytes, copies it
 *        out (moves the tail) and hands it to Protocl_ChkCmdPkg().
 *
 *   Example: the PC sends "xx@QC+STATUS\r\n" (garbage first). The ring holds
 *   x x @ Q C + S T A T U S \r \n. UART_RecvFind finds "@QC+" at
 *   position tail+2, UART_RecvFindPos finds "\r\n" 12 bytes later, and the
 *   14-byte frame "@QC+STATUS\r\n" is copied out and processed.
 *
 *   Transmit path: UART_SendBuffer() uses blocking HAL_UART_Transmit,
 *   protected by the UART mutex once the RTOS runs. Most printing actually
 *   goes through RTOSprintf() (USER_CommFunc.c).
 *
 *   The port/size tables keep room for 5 UARTs (GPS, GSM, DB9 names come from
 *   an earlier project), but only UART1 has a buffer here.
 */
#include <USER_UART_Driver.h>
#include "main.h"

/* One ring buffer per UART port (index = UART_PortTypeDef) */
RingBuffTypeDef rbBuf[3] = {0};

uint16_t u16UART_BufSize[] =
    {
        UART1_BUFFER_SIZE,
        UART2_BUFFER_SIZE,
        UART3_BUFFER_SIZE};

/* Clear stale flags of USART1 at start-up: ORE (overrun = a byte arrived
 * before the previous one was read) and RXNE, so the first interrupt starts
 * from a clean state. */
void UART_CheckInterrupt(void)
{
    if (__HAL_UART_GET_FLAG(puartHandle[UART_PORT1], UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(puartHandle[UART_PORT1]);
    }

    if (__HAL_UART_GET_FLAG(puartHandle[UART_PORT1], UART_FLAG_RXNE))
    {
        __HAL_UART_CLEAR_FLAG(puartHandle[UART_PORT1], UART_FLAG_RXNE);
    }
//    for (int i = 0; i < 5; i++)
//    {
//        if (__HAL_UART_GET_FLAG(puartHandle[i], UART_FLAG_ORE))
//        {
//            __HAL_UART_CLEAR_OREFLAG(puartHandle[i]);
//        }
//
//        if (__HAL_UART_GET_FLAG(puartHandle[i], UART_FLAG_RXNE))
//        {
//            __HAL_UART_CLEAR_FLAG(puartHandle[i], UART_FLAG_RXNE);
//        }
//    }
}
/* Prepare a port for interrupt reception: allocate its ring buffer (size
 * from u16UART_BufSize, 1536 bytes for UART1) and enable RXNE + ERR
 * interrupts. The loop re-tries enabling RXNE up to 3 times and stops as soon
 * as the enable bit reads back as set. */
void UART_Setup(UART_PortTypeDef uartPort)
{
    bool bRXNE_En;

    // Set the ring buffer size
    rbBuf[uartPort].u16Size = u16UART_BufSize[uartPort];

    // Initialize the ring buffer
    RingBuf_Init(&rbBuf[uartPort]);

    // Enable only RXNE and ERR interrupt
    __HAL_UART_ENABLE_IT(puartHandle[uartPort], UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(puartHandle[uartPort], UART_IT_ERR);

    for (int i = 0; i < 3; i++)
    {
        bRXNE_En = __HAL_UART_GET_IT_SOURCE(puartHandle[uartPort], UART_IT_RXNE);


        if (bRXNE_En == 0)
        {

            __HAL_UART_ENABLE_IT(puartHandle[uartPort], UART_IT_RXNE);
        }
        else
            break;
    }
}
//============================================================================//
// @brief  Reset a UART ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
// @retval None.
//============================================================================//
void UART_Reset(UART_PortTypeDef uartPort)
{
    RingBuf_Reset(&rbBuf[uartPort]);
}
//============================================================================//
// @brief  Send an amount of data through UART.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         pu8Dat: Pointer to an uint8_t data array.
//         u16DatLen: Number of bytes to send.
// @retval TRUE/FALSE.
//============================================================================//
bool UART_SendBuffer(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen)
{
    HAL_StatusTypeDef halStat;

    if (u16DatLen == 0)
        return true;

    /* bRTOS_En becomes true in main() just before the scheduler starts; before
     * that there is only one thread of execution and no mutex is needed. */
    if (bRTOS_En)
    {
        osStatus osRes;

        /* Wait at most S_2_mS(1) = 1000 ms for the port; give up otherwise */
        switch (uartPort)
        {
        case SET_COMPORT_DEF:
            // Acquire uart3 synchronization
            osRes = osMutexWait(mutex_uart_user[SET_COMPORT_DEF], S_2_mS(1));

            if (osRes != osOK)
                return false;

            break;

        case GSM_COMPORT_DEF:
            // Acquire uart5 synchronization
            osRes = osMutexWait(mutex_uart_user[GSM_COMPORT_DEF], S_2_mS(1));

            if (osRes != osOK)
                return false;

            break;

        default:
            break;
        }
    }

    if ((uartPort != DB9_COMPORT_DEF) && (uartPort != GSM_COMPORT_DEF))
    {
        halStat = HAL_UART_Transmit(puartHandle[uartPort], pu8Dat, u16DatLen, 0xFFFF);

        // for(int i = 0; i < u16DatLen; i++)
        //{
        //   HAL_UART_Transmit(puartHandle[uartPort], &pu8Dat[i], 1, 0xFFFF);

        // HAL_Delay(1);
        //}
    }
    else if (uartPort == DB9_COMPORT_DEF)
    {

        halStat = HAL_UART_Transmit(puartHandle[uartPort], pu8Dat, u16DatLen, 0xFFFF);
    }
    else if (uartPort == GSM_COMPORT_DEF)
    {

        halStat = HAL_UART_Transmit(puartHandle[uartPort], pu8Dat, u16DatLen, 0xFFFF);
    }

    if (bRTOS_En)
        switch (uartPort)
        {
        case SET_COMPORT_DEF:
            // Release uart3 synchronization
            osMutexRelease(mutex_uart_user[SET_COMPORT_DEF]);

            break;

        case GSM_COMPORT_DEF:
            // Release uart5 synchronization
            osMutexRelease(mutex_uart_user[GSM_COMPORT_DEF]);

            break;

        default:
            break;
        }

    if (halStat == HAL_OK)
        return true;
    else
        return false;
}

//============================================================================//
// @brief  Save data read from a UART port into the corresponding ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         pu8Dat: Pointer to a data array for saving.
//         u16DatLen: Length of the data array for saving.
// @retval TRUE/FALSE.
//============================================================================//
// Called from USART1_IRQHandler (interrupt context) for every received byte.
// When the buffer is full the byte is simply dropped (returns false).
bool UART_SaveRecvData(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen)
{
    // If there is still available space for writing
    if (RingBuf_GetFreeSpace(&rbBuf[uartPort]) >= u16DatLen)
    {
        RingBuf_Write(&rbBuf[uartPort], pu8Dat, u16DatLen);

        return true;
    }
    else
        return false;
}
//============================================================================//
// @brief  Read data acquired from a UART port from the corresponding ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         pu8Dat: Pointer to array for storing read data.
//         u16DatLen: Length of data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_GetRecvData(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen)
{
    return RingBuf_Read(&rbBuf[uartPort], pu8Dat, u16DatLen);
}

//============================================================================//
// @brief  Read data acquired from a UART port from a position in the
//         corresponding ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         u16Pos: Position of the ring buffer to read from.
//         pu8Dat: Pointer to array for storing read data.
//         u16DatLen: Length of data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_GetRecvDataPos(UART_PortTypeDef uartPort, uint16_t u16Pos,
                             uint8_t *pu8Dat, uint16_t u16DatLen)
{
    return RingBuf_ReadPos(&rbBuf[uartPort], u16Pos, pu8Dat, u16DatLen);
}

//============================================================================//
// @brief  Peek data acquired from a UART port from the corresponding ring buffer
//         without deleting the bytes and moving the tail of the ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         u16Pos: Position to peek from.
//         pu8Dat: Pointer to an array for storing read data.
//         u16DatLen: Length of the data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_PeekRecvData(UART_PortTypeDef uartPort, uint16_t u16Pos, uint8_t *pu8Dat,
                           uint16_t u16DatLen)
{
    return RingBuf_Peek(&rbBuf[uartPort], u16Pos, pu8Dat, u16DatLen);
}
//============================================================================//
// @brief  Get the current length of data stored in a UART ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
// @retval Length of data in uint16_t format.
//============================================================================//
uint16_t UART_RecvDataLen(UART_PortTypeDef uartPort)
{
    uint16_t u16BufLen;

    u16BufLen = RingBuf_GetDataLength(&rbBuf[uartPort]);

    return u16BufLen;
}
//============================================================================//
// @brief  Get the current length of data stored in a UART ring buffer from a position.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         u16Pos: Position to count data from.
// @retval Length of data in uint16_t format.
//============================================================================//
uint16_t UART_RecvDataLenPos(UART_PortTypeDef uartPort, uint16_t u16Pos)
{
    uint16_t u16BufLen;

    u16BufLen = RingBuf_GetDataLenPos(&rbBuf[uartPort], u16Pos);

    return u16BufLen;
}
//============================================================================//
// @brief  Find the first occurence of a characters sequence in the data sent
//         to a UART port.
// @param  uartPort: UART port to find the sequence among received data.
//         pcDat: Pointer to an char data array need for finding.
//         pu16Pos: Pointer to a variable holding the first occurence position
//                  if the sequence exists.
// @retval TRUE: The characters sequence is found.
//         FALSE: The characters sequence is not found.
//============================================================================//
bool UART_RecvFind(UART_PortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos)
{
    return RingBuf_Find(&rbBuf[uartPort], pcDat, pu16Pos);
}

//============================================================================//
// @brief  Find the first occurence of a characters sequence in the data sent
//         to a UART port.
// @param  uartPort: UART port to find the sequence among received data.
//         pcDat: Pointer to an char data array need for finding.
//         pu16Pos: Pointer to a variable holding the first occurence position
//                  if the sequence exists.
//         u16StartPos: Start position to find.
// @retval TRUE: The characters sequence is found.
//         FALSE: The characters sequence is not found.
//============================================================================//
bool UART_RecvFindPos(UART_PortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos,
                      uint16_t u16StartPos)
{
    return RingBuf_FindPos(&rbBuf[uartPort], pcDat, pu16Pos, u16StartPos);
}

//============================================================================//
// @brief  Handle communication with PC terminal.
// @param  None.
// @retval None.
//============================================================================//
// Called by UARTTask every loop (~10 ms). Steps:
//  1. nothing buffered -> return.
//  2. buffer at least 3/4 full -> flush it (protects against a flood of
//     bytes that never form a valid frame).
//  3. find the header "@QC+"; then wait up to 100 x 1 ms for "\r\n" after it
//     (the rest of the line may still be arriving).
//  4. found: frame length = from the header up to and including "\r\n":
//        len = (size + endPos + 2 - startPos) % size
//     The "+ size ... % size" handles a frame that wraps around the end of
//     the ring. Example: size 1536, header at 1530, "\r\n" at 4
//        -> (1536 + 4 + 2 - 1530) % 1536 = 12 bytes.
//     The frame is copied (and removed) with UART_GetRecvDataPos and passed
//     to Protocl_ChkCmdPkg().
//  5. not found within 100 ms: the header bytes are read out (removed) so the
//     incomplete line is not found again on the next call.
void UART_TerminalProc(void)
{
    bool bRes;
    char cMessage[128];
    char cRS232CmdHeader[] = "@QC+",
         cRS232CmdEnd[] = "\r\n";
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos;

    // If use SET comport to communicate with PC terminal
        u16StrLen = UART_RecvDataLen(SET_COMPORT_DEF);


        if (!u16StrLen)
            return;

        // If ring buffer is full before reading data then reset ring buffer
        if (u16StrLen >= (rbBuf[SET_COMPORT_DEF].u16Size * 3 / 4))
        {
            RingBuf_Reset(&rbBuf[SET_COMPORT_DEF]);

            return;
        }

        // If find Terminal command header
        if (UART_RecvFind(SET_COMPORT_DEF, cRS232CmdHeader, &u16StrOrgPos))
        {
            uint8_t u8TimeOut = 100;

            bRes = false;

            while (u8TimeOut--)
            {
                bRes = UART_RecvFindPos(SET_COMPORT_DEF, cRS232CmdEnd, &u16StrPos, u16StrOrgPos);

                if (bRes)
                    break;

                HAL_Delay(1);
            }

            // If find Terminal command end
            if (bRes)
            {
                u16StrLen = (rbBuf[SET_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                            rbBuf[SET_COMPORT_DEF].u16Size;

                memset(cMessage, 0x00, sizeof(cMessage));

                /* Bug fix: the length must be checked BEFORE copying into
                 * cMessage (the old code copied first and compared with 256
                 * while the buffer was 128 bytes -> stack overflow on long
                 * lines). Too long: drop everything received and report it. */
                if (u16StrLen >= sizeof(cMessage))
                {
                    RingBuf_Reset(&rbBuf[SET_COMPORT_DEF]);
                    RTOSprintf("\r\n@ Command line too long (max %u chars), ignored",
                               (unsigned)(sizeof(cMessage) - 1U));
                    return;
                }

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                Protocl_ChkCmdPkg(cMessage, NULL);
            }
            else
            {
                u16StrLen = strlen(cRS232CmdHeader);

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                cMessage[u16StrLen] = 0;
            }
        }
}

//============================================================================//
// @brief  Check if a debug mode is ON or OFF.
// @param  tDebugMode: Input debug mode to check.
// @retval TRUE: The input debug mode is ON.
//         FALSE: The input debug mode is OFF.
//============================================================================//
//bool UART_IsDebugMode(DebugModeTypeDef tDebugMode)
//{
//    if (tDebugMode == DBG_OFF)
//    {
//        if (g_Setting.u32DebugMode == DebugModeValue[DBG_OFF])
//            return true;
//        else
//            return false;
//    }
//    else
//    {
//        if (g_Setting.u32DebugMode & DebugModeValue[tDebugMode])
//            return true;
//        else
//            return false;
//    }
//}
