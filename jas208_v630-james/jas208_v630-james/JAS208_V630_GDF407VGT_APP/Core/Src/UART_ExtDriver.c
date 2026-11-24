//============================================================================//
// User Includes
//============================================================================//

#include "UART_ExtDriver.h"

#include <stdarg.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "ff.h"
#include "main.h"
#include "Protocol.h"
#include "SD_Module.h"
#include "System.h"
#include "UART_Driver.h"

//============================================================================//
// User Variables
//============================================================================//
#if ENABLE_MG82
RingBufTypeDef rbBufExt[4] = {0};

uint16_t u16UART_EXT_BufSize[] =
    {
        UART0_EXT_BUFFER_SIZE,
        UART1_EXT_BUFFER_SIZE,
        UART2_EXT_BUFFER_SIZE,
        UART3_EXT_BUFFER_SIZE};

uint8_t u8UART_ExtBufTmp[UART_EXT_TX_LEN_MAX];
#endif

//============================================================================//
// User Function Prototypes
//============================================================================//
#if ENABLE_MG82
//============================================================================//
// @brief  Setup additional parameters for a UART EXT port.
// @param  uartPort: UART_ExtPortTypeDef instance.
// @retval None.
//============================================================================//
void UART_EXT_Setup(UART_ExtPortTypeDef uartPort)
{
    // Set the ring buffer size
    rbBufExt[uartPort].u16Size = u16UART_EXT_BufSize[uartPort];

    // Initialize the ring buffer
    RingBuf_Init(&rbBufExt[uartPort]);
}

//============================================================================//
// @brief  Setup additional parameters for all used UART EXT interfaces.
// @param  None.
// @retval None.
//============================================================================//
void UART_EXT_SetupAll(void)
{
    UART_EXT_Setup(UART_EXT_PORT0);

    UART_EXT_Setup(UART_EXT_PORT1);

    UART_EXT_Setup(UART_EXT_PORT2);

    UART_EXT_Setup(UART_EXT_PORT3);
}

//============================================================================//
// @brief  Change the baudrate of an UART EXT port.
// @param  uartPort: UART EXT port.
//         u32BaudRate: New BaudRate for the UART EXT port.
// @retval True: Operation is done successfully.
//         False: Operation is failed.
//============================================================================//
bool UART_EXT_ReInit(UART_ExtPortTypeDef uartPort, uint32_t u32BaudRate)
{
    bool bRes;

    bRes = MG82_UART_SetBR((uint8_t)uartPort, (UART_BaudRateDef_t)u32BaudRate);

    return bRes;
}

//============================================================================//
// @brief  Reset a UART EXT ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
// @retval None.
//============================================================================//
void UART_EXT_Reset(UART_ExtPortTypeDef uartPort)
{
    RingBuf_Reset(&rbBufExt[uartPort]);
}

//============================================================================//
// @brief  Send an amount of data through UART EXT.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         pu8Dat: Pointer to an uint8_t data array.
//         u16DatLen: Number of bytes to send.
// @retval TRUE/FALSE.
//============================================================================//
bool UART_EXT_SendBuffer(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat,
                         uint16_t u16DatLen)
{
    bool bRes = FALSE;
    uint16_t u16DatCnt = 0, u16DatSize = 0;
    uint8_t *pu8Ptr;

    if (u16DatLen == 0)
        return TRUE;

    if (bMG82_Stop == TRUE)
        return FALSE;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_uartext_user[uartPort], S_2_mS(1));

    //	if(osRes != osOK)
    //		return FALSE;
    //}

    u16DatCnt = 0;
    pu8Ptr = pu8Dat;

    while (u16DatCnt < u16DatLen)
    {
        u16DatSize = u16DatLen - u16DatCnt;

        if (u16DatSize > UART_EXT_TX_LEN_MAX)
            u16DatSize = UART_EXT_TX_LEN_MAX;

        bRes = MG82_UART_Send((uint8_t)uartPort, pu8Ptr, (uint8_t)u16DatSize);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> UART EXT%d send at offset %d ERROR", uartPort, u16DatCnt);

            break;
        }

        u16DatCnt += u16DatSize;
        pu8Ptr += u16DatSize;

        HAL_Delay(1);
    }

    // if(bRTOS_En)
    //	osMutexRelease(mutex_uartext_user[uartPort]);

    return bRes;
}

//============================================================================//
// @brief  Check the RX buffer of UART EXT ports in MG82 IC and get any available
//         data.
// @param  void.
// @retval TRUE/FALSE.
//============================================================================//
void UART_EXT_GetRX(void)
{
    // static uint32_t u32TickCnt = 0;
    bool bRes = FALSE;
    uint16_t u16Len[UART_EXT_BUFFER_QUANTITY] = {0};
    uint16_t u16DatCnt = 0, u16DatSize = 0;
    uint16_t u16FreeSpace;

    if (bMG82_Stop == TRUE)
        return;

    // if(HAL_GetTick() > (u32TickCnt + UART_EXT_RX_CHK_TIME))
    {
        // u32TickCnt = HAL_GetTick();

        bRes = MG82_UART_GetLen(u16Len);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> UART EXT get RX len ERROR.");

            return;
        }

        for (int i = 0; i < UART_EXT_BUFFER_QUANTITY; i++)
        {
            if (u16Len[i] > 0)
            {
                u16FreeSpace = RingBuf_GetFreeSpace(&rbBufExt[i]);

                if (u16Len[i] > u16FreeSpace)
                    u16Len[i] = u16FreeSpace;

                u16DatCnt = 0;

                while (u16DatCnt < u16Len[i])
                {
                    u16DatSize = u16Len[i] - u16DatCnt;

                    if (u16DatSize > UART_EXT_TX_LEN_MAX)
                    {
                        u16DatSize = UART_EXT_TX_LEN_MAX;
                    }

                    memset(u8UART_ExtBufTmp, 0x00, sizeof(u8UART_ExtBufTmp));

                    bRes = MG82_UART_Read(i, u8UART_ExtBufTmp, (uint8_t)u16DatSize);

                    if (bRes == FALSE)
                    {
                        RTOSprintf("\r\n> UART EXT read port %d ERROR.", i);

                        break;
                    }

                    UART_EXT_SaveRecvData((UART_ExtPortTypeDef)i, u8UART_ExtBufTmp, u16DatSize);

                    u16DatCnt += u16DatSize;
                }
            }
        }
    }
}

//============================================================================//
// @brief  Save data read from a UART EXT port into the corresponding ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         pu8Dat: Pointer to a data array for saving.
//         u16DatLen: Length of the data array for saving.
// @retval TRUE/FALSE.
//============================================================================//
bool UART_EXT_SaveRecvData(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat,
                           uint16_t u16DatLen)
{
    // If there is still available space for writing
    if (RingBuf_GetFreeSpace(&rbBufExt[uartPort]) >= u16DatLen)
    {
        RingBuf_Write(&rbBufExt[uartPort], pu8Dat, u16DatLen);

        return TRUE;
    }
    else
        return FALSE;
}

//============================================================================//
// @brief  Read data acquired from a UART EXT port from the corresponding ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         pu8Dat: Pointer to array for storing read data.
//         u16DatLen: Length of data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_EXT_GetRecvData(UART_ExtPortTypeDef uartPort, uint8_t *pu8Dat,
                              uint16_t u16DatLen)
{
    return RingBuf_Read(&rbBufExt[uartPort], pu8Dat, u16DatLen);
}

//============================================================================//
// @brief  Read data acquired from a UART EXT port from a position in the
//         corresponding ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         u16Pos: Position of the ring buffer to read from.
//         pu8Dat: Pointer to array for storing read data.
//         u16DatLen: Length of data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_EXT_GetRecvDataPos(UART_ExtPortTypeDef uartPort, uint16_t u16Pos,
                                 uint8_t *pu8Dat, uint16_t u16DatLen)
{
    return RingBuf_ReadPos(&rbBufExt[uartPort], u16Pos, pu8Dat, u16DatLen);
}

//============================================================================//
// @brief  Peek data acquired from a UART EXT port from the corresponding ring buffer
//         without deleting the bytes and moving the tail of the ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         u16Pos: Position to peek from.
//         pu8Dat: Pointer to an array for storing read data.
//         u16DatLen: Length of the data for reading.
// @retval Number of bytes read in uint16_t format.
//============================================================================//
uint16_t UART_EXT_PeekRecvData(UART_ExtPortTypeDef uartPort, uint16_t u16Pos,
                               uint8_t *pu8Dat, uint16_t u16DatLen)
{
    return RingBuf_Peek(&rbBufExt[uartPort], u16Pos, pu8Dat, u16DatLen);
}

//============================================================================//
// @brief  Get the current length of data stored in a UART EXT ring buffer.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
// @retval Length of data in uint16_t format.
//============================================================================//
uint16_t UART_EXT_RecvDataLen(UART_ExtPortTypeDef uartPort)
{
    uint16_t u16BufLen;

    u16BufLen = RingBuf_GetDataLength(&rbBufExt[uartPort]);

    return u16BufLen;
}

//============================================================================//
// @brief  Get the current length of data stored in a UART EXT ring buffer from a position.
// @param  uartPort: Pointer to a UART_ExtPortTypeDef instance.
//         u16Pos: Position to count data from.
// @retval Length of data in uint16_t format.
//============================================================================//
uint16_t UART_EXT_RecvDataLenPos(UART_ExtPortTypeDef uartPort, uint16_t u16Pos)
{
    uint16_t u16BufLen;

    u16BufLen = RingBuf_GetDataLenPos(&rbBufExt[uartPort], u16Pos);

    return u16BufLen;
}

//============================================================================//
// @brief  Find the first occurence of a characters sequence in the data sent
//         to a UART EXT port.
// @param  uartPort: UART EXT port to find the sequence among received data.
//         pcDat: Pointer to an char data array need for finding.
//         pu16Pos: Pointer to a variable holding the first occurence position
//                  if the sequence exists.
// @retval TRUE: The characters sequence is found.
//         FALSE: The characters sequence is not found.
//============================================================================//
bool UART_EXT_RecvFind(UART_ExtPortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos)
{
    return RingBuf_Find(&rbBufExt[uartPort], pcDat, pu16Pos);
}

//============================================================================//
// @brief  Find the first occurence of a characters sequence in the data sent
//         to a UART EXT port.
// @param  uartPort: UART EXT port to find the sequence among received data.
//         pcDat: Pointer to an char data array need for finding.
//         pu16Pos: Pointer to a variable holding the first occurence position
//                  if the sequence exists.
//         u16StartPos: Start position to find.
// @retval TRUE: The characters sequence is found.
//         FALSE: The characters sequence is not found.
//============================================================================//
bool UART_EXT_RecvFindPos(UART_ExtPortTypeDef uartPort, char *pcDat, uint16_t *pu16Pos,
                          uint16_t u16StartPos)
{
    return RingBuf_FindPos(&rbBufExt[uartPort], pcDat, pu16Pos, u16StartPos);
}
#endif
