//============================================================================//
// User Includes
//============================================================================//

#include "UART_Driver.h"

#include <stdarg.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "ff.h"
#include "main.h"
#include "PRN_Driver.h"
#include "Protocol.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//

RingBufTypeDef rbBuf[5] = {0};

// RingBufTypeDef rbRS232Buf;

uint16_t u16UART_BufSize[] =
    {
        UART1_BUFFER_SIZE,
        UART2_BUFFER_SIZE,
        UART3_BUFFER_SIZE,
        UART4_BUFFER_SIZE,
        UART5_BUFFER_SIZE};

bool bUART_DMA_Wait[UART_BUFFER_QUANTITY] = {0};

uint32_t u32UART_DMA_WaitTick[UART_BUFFER_QUANTITY] = {0};

PC_DownloadStepTypeDef u8FilTransStep = STEP_IDLE;

uint8_t u8ProcRetry = 0;

uint8_t u8TransBuf[RS232FILE_TRANSFER_MAX_LEN + 8]; // __attribute__ ((section("ccmram_var")));

bool bPC_Trans = FALSE;

bool bMakeFile = FALSE;

//============================================================================//
// User Function Prototypes
//============================================================================//

//============================================================================//
// @brief  Check if the main port for terminal communication is SET or not.
// @param  None.
// @retval TRUE: The main port for terminal communication is SET.
//         FALSE: The main port for terminal communication is DB9.
//============================================================================//
bool UART_IsTerminalUsingSET(void)
{
    bool bRes;

#if CUSTOMERTYPE == CUSTOMER_ARTC
    bRes = 1;
#else
    bRes = 0;
#endif

    return bRes;
}

//============================================================================//
// @brief  Check if a debug mode is ON or OFF.
// @param  tDebugMode: Input debug mode to check.
// @retval TRUE: The input debug mode is ON.
//         FALSE: The input debug mode is OFF.
//============================================================================//
bool UART_IsDebugMode(DebugModeTypeDef tDebugMode)
{
    if (tDebugMode == DBG_OFF)
    {
        if (g_Setting.u32DebugMode == DebugModeValue[DBG_OFF])
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if (g_Setting.u32DebugMode & DebugModeValue[tDebugMode])
            return TRUE;
        else
            return FALSE;
    }
}

//============================================================================//
// @brief  Setup additional parameters for a UART interface.
// @param  uartPort: Pointer to the UART_PortTypeDef instance.
// @retval None.
//============================================================================//
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

        // RTOSprintf("\r\n> UART%d RXNE En: %d", uartPort + 1, bRXNE_En);

        if (bRXNE_En == 0)
        {
            // RTOSprintf("\r\n> Restart UART%d RXNE EN", uartPort + 1);

            __HAL_UART_ENABLE_IT(puartHandle[uartPort], UART_IT_RXNE);
        }
        else
            break;
    }
}

//============================================================================//
// @brief  Setup additional parameters for all used UART interfaces.
// @param  None.
// @retval None.
//============================================================================//
void UART_SetupAll(void)
{
    if (UART_IsTerminalUsingSET() == FALSE)
    {
        HAL_UART_DeInit(puartHandle[UART_PORT3]);

        puartHandle[UART_PORT3]->Instance = USART3;
        // if(g_Setting.tAdControl.u8DB9Func == 0)
        //{
        //	puartHandle[UART_PORT3]->Init.BaudRate = 9600;
        // }
        // else
        //{
        puartHandle[UART_PORT3]->Init.BaudRate = 115200;
        //}
        puartHandle[UART_PORT3]->Init.WordLength = UART_WORDLENGTH_8B;
        puartHandle[UART_PORT3]->Init.StopBits = UART_STOPBITS_1;
        puartHandle[UART_PORT3]->Init.Parity = UART_PARITY_NONE;
        puartHandle[UART_PORT3]->Init.Mode = UART_MODE_TX_RX;
        puartHandle[UART_PORT3]->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        puartHandle[UART_PORT3]->Init.OverSampling = UART_OVERSAMPLING_16;

        if (HAL_UART_Init(puartHandle[UART_PORT3]) != HAL_OK)
        {
            Error_Handler();
        }

        HAL_UART_DeInit(puartHandle[UART_PORT4]);

        puartHandle[UART_PORT4]->Instance = UART4;
        puartHandle[UART_PORT4]->Init.BaudRate = 115200;
        puartHandle[UART_PORT4]->Init.WordLength = UART_WORDLENGTH_8B;
        puartHandle[UART_PORT4]->Init.StopBits = UART_STOPBITS_1;
        puartHandle[UART_PORT4]->Init.Parity = UART_PARITY_NONE;
        puartHandle[UART_PORT4]->Init.Mode = UART_MODE_TX_RX;
        puartHandle[UART_PORT4]->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        puartHandle[UART_PORT4]->Init.OverSampling = UART_OVERSAMPLING_16;

        if (HAL_UART_Init(puartHandle[UART_PORT4]) != HAL_OK)
        {
            Error_Handler();
        }
    }

    // UART_Setup(EXT_COMPORT_DEF);

    UART_Setup(SET_COMPORT_DEF);

    UART_Setup(DB9_COMPORT_DEF);

    UART_Setup(GSM_COMPORT_DEF);

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    // Set the ring buffer size
    rbBuf[GPS_COMPORT_DEF].u16Size = u16UART_BufSize[GPS_COMPORT_DEF];

    // Initialize the ring buffer
    RingBuf_Init(&rbBuf[GPS_COMPORT_DEF]);

    UART_ReInit(GPS_COMPORT_DEF, 115200U);
#elif GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    UART_Setup(GPS_COMPORT_DEF);
#endif

    // rbRS232Buf.u16Size = RS232_BUFFER_SIZE;

    // RingBuf_Init(&rbRS232Buf);
}

//============================================================================//
// @brief  Reinitialize an UART module.
// @param  uartPort: UART port for reinitialization.
//         u32BaudRate: BaudRate for the UART port.
// @retval True: Operation is done successfully.
//         False: Operation is failed.
//============================================================================//
bool UART_ReInit(UART_PortTypeDef uartPort, uint32_t u32BaudRate)
{
    if (HAL_UART_DeInit(puartHandle[uartPort]) != HAL_OK)
        return FALSE;

    switch (uartPort)
    {
    case UART_PORT1:
        puartHandle[uartPort]->Instance = USART1;

        break;

    case UART_PORT2:
        puartHandle[uartPort]->Instance = USART2;

        break;

    case UART_PORT3:
        puartHandle[uartPort]->Instance = USART3;

        break;

    case UART_PORT4:
        puartHandle[uartPort]->Instance = UART4;

        break;

    case UART_PORT5:
        puartHandle[uartPort]->Instance = UART5;

        break;

    default:
        break;
    }

    puartHandle[uartPort]->Init.BaudRate = u32BaudRate;
    puartHandle[uartPort]->Init.WordLength = UART_WORDLENGTH_8B;
    puartHandle[uartPort]->Init.StopBits = UART_STOPBITS_1;
    puartHandle[uartPort]->Init.Parity = UART_PARITY_NONE;
    puartHandle[uartPort]->Init.Mode = UART_MODE_TX_RX;
    puartHandle[uartPort]->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    puartHandle[uartPort]->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(puartHandle[uartPort]) != HAL_OK)
        return FALSE;
    else
        return TRUE;
}

//============================================================================//
// @brief  Check UART interrupt.
// @param  None.
// @retval None.
//============================================================================//
void UART_CheckInterrupt(void)
{
    for (int i = 0; i < 5; i++)
    {
        if (__HAL_UART_GET_FLAG(puartHandle[i], UART_FLAG_ORE))
        {
            __HAL_UART_CLEAR_OREFLAG(puartHandle[i]);
        }

        if (__HAL_UART_GET_FLAG(puartHandle[i], UART_FLAG_RXNE))
        {
            __HAL_UART_CLEAR_FLAG(puartHandle[i], UART_FLAG_RXNE);
        }
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
        return TRUE;

    if (bRTOS_En)
    {
        osStatus osRes;

        switch (uartPort)
        {
        case SET_COMPORT_DEF:
            // Acquire uart3 synchronization
            osRes = osMutexWait(mutex_uart_user[SET_COMPORT_DEF], S_2_mS(1));

            if (osRes != osOK)
                return FALSE;

            break;

        case GSM_COMPORT_DEF:
            // Acquire uart5 synchronization
            osRes = osMutexWait(mutex_uart_user[GSM_COMPORT_DEF], S_2_mS(1));

            if (osRes != osOK)
                return FALSE;

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
        // vTaskSuspendAll();

        // HAL_IWDG_Refresh(&hiwdg);

        halStat = HAL_UART_Transmit(puartHandle[uartPort], pu8Dat, u16DatLen, 0xFFFF);

        // xTaskResumeAll();

        /*
        for(int i = 0; i < u16DatLen; i++)
        {
        HAL_UART_Transmit(puartHandle[uartPort], &pu8Dat[i], 1, 0xFFFF);

        //HAL_Delay(1);
        }
        */
    }
    else if (uartPort == GSM_COMPORT_DEF)
    {
        // for(int i = 0; i < u16DatLen; i++)
        //{
        //	halStat = HAL_UART_Transmit(puartHandle[uartPort], &pu8Dat[i], 1, 0xFFFF);

        //	HAL_Delay(1);
        //}

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
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Send an amount of data through UART using DMA.
// @param  uartPort: Pointer to a UART_PortTypeDef instance, only accept USART3
//                   and UART4.
//         pu8Dat: Pointer to an uint8_t data array.
//         u16DatLen: Number of bytes to send.
// @retval TRUE/FALSE.
//============================================================================//
bool UART_SendBufferDMA(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen)
{
    HAL_StatusTypeDef halStat;

    if (uartPort != GPS_COMPORT_DEF)
        return FALSE;

    if (u16DatLen == 0)
        return TRUE;

    if (bRTOS_En)
    {
        osStatus osRes;

        switch (uartPort)
        {
        case GPS_COMPORT_DEF:
            // Acquire uart1 synchronization
            osRes = osMutexWait(mutex_uart_user[GPS_COMPORT_DEF], S_2_mS(1));

            if (osRes != osOK)
                return FALSE;

        default:
            break;
        }
    }

    u32UART_DMA_WaitTick[uartPort] = HAL_GetTick();

    bUART_DMA_Wait[uartPort] = TRUE;

    halStat = HAL_UART_Transmit_DMA(puartHandle[uartPort], pu8Dat, u16DatLen);

    if (halStat != HAL_OK)
    {
        RTOSprintf("\r\n> UART%d send buffer DMA ERROR %d", uartPort, halStat);

        bUART_DMA_Wait[uartPort] = FALSE;
    }

    if (bRTOS_En)
        switch (uartPort)
        {
        case GPS_COMPORT_DEF:
            // Release uart1 synchronization
            osMutexRelease(mutex_uart_user[GPS_COMPORT_DEF]);

            break;

        default:
            break;
        }

    if (halStat == HAL_OK)
        return TRUE;
    else
        return FALSE;
}

//============================================================================//
// @brief  Save data read from a UART port into the corresponding ring buffer.
// @param  uartPort: Pointer to a UART_PortTypeDef instance.
//         pu8Dat: Pointer to a data array for saving.
//         u16DatLen: Length of the data array for saving.
// @retval TRUE/FALSE.
//============================================================================//
bool UART_SaveRecvData(UART_PortTypeDef uartPort, uint8_t *pu8Dat, uint16_t u16DatLen)
{
    // If there is still available space for writing
    if (RingBuf_GetFreeSpace(&rbBuf[uartPort]) >= u16DatLen)
    {
        RingBuf_Write(&rbBuf[uartPort], pu8Dat, u16DatLen);

        return TRUE;
    }
    else
        return FALSE;
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

/*
//============================================================================//
// @brief  Process received data from the SET COMPORT UART.
// @param  None.
// @retval None.
//============================================================================//
void UART_SET_COM_PORT_Proc(void)
{
    bool bRes;
    static char cFileName[50] = {0};
    //static char cFindName[50] = {0};
    char cMessage[128];
    const uint8_t u8PC_CmdHeader[] = {0xAA, 0x75},
                  u8PC_RespHeader[] = {0x55, 0x7A};
    char cRS232CmdHeader[] = "$AS+",
         cRS232CmdEnd[] = "\r\n";
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos, u16DatCnt;
    static uint32_t u32FilSize, u32FilDatCnt;
    uint16_t u16DatBlockLen;
    uint8_t u8CheckSumCal;
    uint8_t u8CheckSumOrg;
    static bool bFullDay;
    static uint32_t u32ExeTime = 0;


    u16StrLen = UART_RecvDataLen(SET_COMPORT_DEF);

    if(!u16StrLen)
        return;

    //UART_PeekRecvData(SET_COMPORT_DEF, rbBuf[SET_COMPORT_DEF].u16Tail, (uint8_t *)cMessage, u16StrLen);

    //cMessage[u16StrLen] = 0;

    //DB9printf("\r\n> Receive %d bytes from DB9 COMPORT:\n%s\nDB9 message end.", u16StrLen, cMessage);

    // If ring buffer is full before reading data then reset ring buffer
    if(u16StrLen >= (rbBuf[SET_COMPORT_DEF].u16Size * 3 / 4))
    {
        RingBuf_Reset(&rbBuf[SET_COMPORT_DEF]);

        return;
    }

    // If find RS232 command header
    if(UART_RecvFind(SET_COMPORT_DEF, cRS232CmdHeader, &u16StrOrgPos))
    {
        uint8_t u8TimeOut = 100;


        bRes = FALSE;

        while(u8TimeOut--)
        {
          bRes = UART_RecvFindPos(SET_COMPORT_DEF, cRS232CmdEnd, &u16StrPos, u16StrOrgPos);

            if(bRes)
                break;

            HAL_Delay(1);
        }

        // If find RS232 command end
        if(bRes)
        {
            u16StrLen = (rbBuf[SET_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                        rbBuf[SET_COMPORT_DEF].u16Size;

            memset(cMessage, 0x00, sizeof(cMessage));

            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            //cMessage[u16StrLen] = 0;

            if(u16StrLen < UART_BASE_BUFFER_SIZE)
            {
                Protocl_ChkCmdPkg(cMessage, NULL);
            }
        }
        else
        {
          u16StrLen = strlen(cRS232CmdHeader);

            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            cMessage[u16StrLen] = 0;
        }
    }

    // If find the PC command header sequence
    if(UART_RecvFind(SET_COMPORT_DEF, (char*)u8PC_CmdHeader, &u16StrPos))
    {
        //DB9printf("\r\n> Found message header.");

        bPC_Trans = TRUE;

        HAL_Delay(100);

        u16StrOrgPos = u16StrPos;

        HAL_Delay(10);

        // Move to the command byte
        u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

        u16DatCnt = 2;

        // Peek 1 byte command
        u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 1);

        // If cannot get 1 byte
        if(!u16StrLen)
        {
            u8ProcRetry++;

            // If error more than 3 times
            if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
            {
                u8ProcRetry = 0;

                // Read out all data in the Ring buffer
                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                // Send Error message to PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                bPC_Trans = FALSE;

                DB9printf("\r\n> Fail to parse PC command.");

                DB9printf("\r\n> PC message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);
            }

            return;
        }

        u16StrPos = (u16StrPos + 1) % rbBuf[SET_COMPORT_DEF].u16Size;

        u16DatCnt += 1;

        uint8_t u8Cmd = (uint8_t)cMessage[0];

        //DB9printf("\r\n> Command:0x%02x", u8Cmd);

        // Check command
        switch(u8Cmd)
        {
            case RS232CMD_PC_DATA: // Case data requirement message from PC
            {
                osTimerStop(timerRTOS_PCTransHandle);

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if(u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC data message.");

                        DB9printf("\r\n> PC data message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                                 ((uint16_t)cMessage[0] & 0x00FF);

                //DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                // If require 24h data
                if(u16DatBlockLen == 4)
                {
                    bFullDay = TRUE;

                    DB9printf("\r\n> 24 hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_DAY);

                    // Peek 5 bytes from the ring buffer, including 4 bytes data and 1 bytes checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    // If cannot get 5 bytes
                    if(u16StrLen != 5)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message date data.");

                            DB9printf("\r\n> PC data message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u16StrPos = (u16StrPos + 5) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 5;

                    // Create file name to file
                    sprintf(cFileName, "D%02d%02d%02d%02d_235959_%s.VDR",
                                    cMessage[0], cMessage[1], cMessage[2], cMessage[3],
                                    g_Setting.tBusInfo.cBus_ID);

                    //DB9printf("\r\n> Find file name:%s", cFileName);

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    //DB9printf("\r\n> PC data message:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    uint8_t u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    uint8_t u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t*)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t*)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    //DB9printf("\r\n> PC data message checksum matched.");

                    // Find the required file in the SD card
                    //bRes = SD_FindFileName(cFindName, cFileName);

                    bRes = SD_CheckFileExist(cFileName);

                    // If the file is not found
                    if(!bRes)
                    {
                        // Send error message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> File not found.");

                        return;
                    }

                    // Get the file size
                    u32FilSize = SD_GetFileSize(cFileName);

                    DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                    // Send response message F0 to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F0;
                    cMessage[3] = 0x04;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                    cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                    cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                    cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                    cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                    u8FilTransStep = STEP_F5;

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                    DB9printf("\r\n> Sent F0.");
                    //DB9printf("\r\n> Sent F0 to PC:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);
                }
                else if(u16DatBlockLen == 1) // If 1 hour data is required
                {
                    bFullDay = FALSE;

                    DB9printf("\r\n> Hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_HOUR);

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if(u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message hour data.");

                            DB9printf("\r\n> PC data message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    //DB9printf("\r\n> PC data message:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    //DB9printf("\r\n> PC data message checksum matched.");

                    if(bMakeFile)
                    {
                        bMakeFile = FALSE;

                        if(bDoneHourVDRFile == FALSE)
                        {
                            DB9printf("\r\n> HourVDR file is not done yet.", cFileName);

                            // Send F0 message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F0;
                            cMessage[3] = 0x04;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = 0x00;
                            cMessage[7] = 0x00;
                            cMessage[8] = 0x00;
                            cMessage[9] = 0x00;
                            cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                            u8FilTransStep = STEP_IDLE;

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                            osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                            DB9printf("\r\n> Sent F0.");

                            break;
                        }

                        sprintf(cFileName, "%s", cPC_HourFileName);
                    }
                    else
                    {
                      struct tm tmTimeNow = {0};
                        time_t ttTimeNow = System_GetTime();


                        // Convert local time in second to local time in calendar format
                        localtime_r(&ttTimeNow, &tmTimeNow);

                        // Get current date
                        uint16_t u16Year = tmTimeNow.tm_year + 1900U;
                        uint8_t u8Month = tmTimeNow.tm_mon + 1U;
                        uint8_t u8Day = tmTimeNow.tm_mday;
                        uint8_t u8Hour = tmTimeNow.tm_hour;

                        // Create file name to find
                        sprintf(cFileName, "D%02d%02d%02d%02d_%02d_%02d_%s.VDR",
                                        (uint8_t)(u16Year / 100), (uint8_t)(u16Year % 100), u8Month, u8Day,
                                        u8Hour, u8HourCnt, g_Setting.tBusInfo.cBus_ID);
                    }

                    DB9printf("\r\n> Check the existance of file name in SD card:%s", cFileName);

                    // Find the required file in the SD card
                    bRes = SD_CheckFileExist(cFileName);

                    // If the required file is not found
                    if(!bRes)
                    {
                        DB9printf("\r\n> Not find file %s in SD card.", cFileName);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = 0x00;
                        cMessage[7] = 0x00;
                        cMessage[8] = 0x00;
                        cMessage[9] = 0x00;
                        cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                        u8FilTransStep = STEP_IDLE;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F0.");

                        //return;
                    }
                    else
                    {
                        // Get file size
                        u32FilSize = SD_GetFileSize(cFileName);

                        DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                        cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                        cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                        cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                        cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                        u8FilTransStep = STEP_F5;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Send F0.");
                    }
                }
                else // If the data length is both not 1 and 4
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Unidentified PC data message data length.");

                    DB9printf("\r\n> PC data message:");

                    for(int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }
            case RS232CMD_PC_ACK_OK: // Case data ACK
            {
                // Peek 2 bytes from the ring buffer, including the reserve and checksum byte
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                // If cannot get 2 bytes
                if(u16StrLen < 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Parse ACK message fail.");

                        DB9printf("\r\n> PC ACK message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 2;

                // Read out the current message
                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                //DB9printf("\r\n> PC message:");

                //for(int i = 0; i < u16DatCnt; i++)
                //  DB9printf("0x%02x ", cMessage[i]);

                // Calculate checksum
                u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                u8CheckSumOrg = cMessage[u16DatCnt - 1];

                u16DatCnt = 0;

                // If checksum is not matched
                if(u8CheckSumCal != u8CheckSumOrg)
                {
                    // Send Error message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> ACK message checksum unmatched");

                    osTimerStop(timerRTOS_PCTransHandle);

                    return;
                }

                //DB9printf("\r\n> Checksum matched.");

                DB9printf("\r\n> Receive ACK OK.");

                //if(u8FilTransStep > 0)
                //  DB9printf("\r\n> Step: F%d", u8FilTransStep - 1);

                // Check the current step of the session
                switch((uint8_t)u8FilTransStep)
                {
                    case STEP_IDLE:
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        // Send error message
                        //cMessage[0] = u8PC_RespHeader[0];
                        //cMessage[1] = u8PC_RespHeader[1];
                        //cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        //cMessage[3] = 0x00;
                        //cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        //UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F0: // Step F0
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        break;
                    }

                    case STEP_F1: // Step F1
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        // Send F1 message
                        u32FilDatCnt = 0;

                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F1;
                        cMessage[3] = 0x00;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                        u8FilTransStep = STEP_F2;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                        DB9printf("\r\n> Sent F1.");

                        u32ExeTime = HAL_GetTick();

                        //if(SD_CheckFileExist("PC_Download_Data.txt"))
                        //  SD_RemoveFile("PC_Download_Data.txt");

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        break;
                    }

                    case STEP_F2: // Step F2
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        // If there still is data to send
                        if(u32FilDatCnt < u32FilSize)
                        {
                            //DB9printf("\r\n> DatCnt:%d", u32FilDatCnt);

                            // Calculate the amount of data to transfer
                            uint32_t u32DatSize = u32FilSize - u32FilDatCnt;

                            if(u32DatSize > RS232FILE_TRANSFER_MAX_LEN)
                                u32DatSize = RS232FILE_TRANSFER_MAX_LEN;
                            else if(u32DatSize < RS232FILE_TRANSFER_MAX_LEN)
                                memset(u8TransBuf, 0x00, sizeof(u8TransBuf));

                            uint8_t *u8BufPtr = (uint8_t *)(&u8TransBuf[6]);

                            // Read the data from the file in the SD card to the buffer
                            bRes = SD_ReadAddrToBuff(cFileName, u8BufPtr, u32DatSize, u32FilDatCnt);

                            // If not read successfully
                            if(!bRes)
                            {
                                // Terminate current session
                                u8FilTransStep = STEP_IDLE;

                                // Send Error message to the server
                                cMessage[0] = u8PC_RespHeader[0];
                                cMessage[1] = u8PC_RespHeader[1];
                                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                                cMessage[3] = 0x00;
                                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                                UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                                bPC_Trans = FALSE;

                                DB9printf("\r\n> Read file fail.");

                                if(bFullDay == FALSE)
                                    SD_RemoveFile(cFileName);

                                break;
                            }

                            u32FilDatCnt += u32DatSize;

                            // Send data message F2
                            u8TransBuf[0] = u8PC_RespHeader[0];
                            u8TransBuf[1] = u8PC_RespHeader[1];
                            u8TransBuf[2] = RS232CMD_RESP_F2;
                            u8TransBuf[3] = (uint8_t)(u32DatSize & 0xFF);
                            u8TransBuf[4] = (uint8_t)((u32DatSize >> 8) & 0xFF);
                            u8TransBuf[5] = 0x00;
                            u8TransBuf[6 + RS232FILE_TRANSFER_MAX_LEN] = CalCheckSum((uint8_t *)u8TransBuf, (6 + RS232FILE_TRANSFER_MAX_LEN));

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)u8TransBuf, (7 + RS232FILE_TRANSFER_MAX_LEN));

                            //DB9printf("\r\n> Sent F2.");
                            //DB9printf("\r\n> Sent F2. Length: %d bytes.", 7 + RS232FILE_TRANSFER_MAX_LEN);

                            //SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)"\r\n", 2);

                            //SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)u8TransBuf, 7 + RS232FILE_TRANSFER_MAX_LEN);
                        }
                        else // If all data in the file is already sent
                        {
                            // Send F3 message
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F3;
                            cMessage[3] = 0x00;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                            u8FilTransStep = STEP_F3;

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                            DB9printf("\r\n Sent F3.");

                            u32ExeTime = HAL_GetTick() - u32ExeTime;

                            DB9printf("\r\n> Download time: %dms", u32ExeTime);

                            if(bFullDay == FALSE)
                                SD_RemoveFile(cFileName);
                        }

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        break;
                    }

                    case STEP_F3: // Case step F3
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        DB9printf("\r\n> Finish transfer data file.");

                        // Finish the current session
                        u8FilTransStep = STEP_IDLE;

                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F4:
                    {
                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F5:
                    {
                        uint16_t u16FileNameLen = strlen(cFileName);

                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F5;
                        cMessage[3] = (uint8_t)u16FileNameLen;
                        cMessage[4] = (uint8_t)(u16FileNameLen >> 8);
                        cMessage[5] = 0x00;

                        //strcpy(&cMessage[6], cFileName);
                        memcpy(&cMessage[6], cFileName, u16FileNameLen);

                        cMessage[6 + u16FileNameLen] = CalCheckSum((uint8_t *)cMessage, 6 + u16FileNameLen);

                        u8FilTransStep = STEP_F1;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, (7 + u16FileNameLen));

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F5.");

                        break;
                    }

                    default:
                        break;
                }

                break;
            }
            case RS232CMD_PC_ACK_ERR: // Case ACK error
            {
                if((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                // Terminate the current session
                u8FilTransStep = STEP_IDLE;

                // Read out the current message
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                if(u16StrLen != 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u16DatCnt += u16StrLen;

                        u8ProcRetry = 0;

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse ACK error message.");

                        DB9printf("\r\n> PC ACK error message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16DatCnt += 2;

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> PC ACK error message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                break;
            }
            case RS232CMD_PC_MAKEFILE:
            {
                bFullDay = FALSE;

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if(u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC makefile message.");

                        DB9printf("\r\n> PC makefile message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                                 ((uint16_t)cMessage[0] & 0x00FF);

                DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                if(u16DatBlockLen == 1) // If 1 hour data is required
                {
                    DB9printf("\r\n> Hours data required.");

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if(u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC makefile hour data.");

                            DB9printf("\r\n> PC makefile message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC makefile message checksum unmatched.");

                        return;
                    }

                    DB9printf("\r\n> PC makefile message checksum matched.");

                    // Send ACK to make file message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F4;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F4.");

                    DB9printf("\r\n> Create hour file.");

                    bMakeFile = TRUE;

                    bFullDay = FALSE;

                    u8HourDataCnt = u8HourCnt;

                    bHourDataFlag = TRUE;

                    bDLRS232Flag = TRUE;

                    bDoHourVDRFlag = TRUE;

                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;
                }
                else // If the data length is not 1
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    //cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> PC makefile message data length is not 1.");

                    DB9printf("\r\n> PC makefile message:");

                    for(int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }

            default: // Case the message is unidentified
            {
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 8);

                u16DatCnt += u16StrLen;

                // Send ERROR message to the PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                if((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> Unidentified PC command: 0x%02X.", u8Cmd);

                DB9printf("\r\n> PC message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                u8FilTransStep = STEP_IDLE;

                break;
            }
        }
    }
}

//============================================================================//
// @brief  Process received data from the DB9 COMPORT UART.
// @param  None.
// @retval None.
//============================================================================//
void UART_DB9_COM_PORT_Proc(void)
{
    bool bRes;
    static char cFileName[50] = {0};
    //static char cFindName[50] = {0};
    char cMessage[128];
    const uint8_t u8PC_CmdHeader[] = {0xAA, 0x75},
                  u8PC_RespHeader[] = {0x55, 0x7A};
    char cRS232CmdHeader[] = "$AS+",
         cRS232CmdEnd[] = "\r\n";
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos, u16DatCnt;
    static uint32_t u32FilSize, u32FilDatCnt;
    uint16_t u16DatBlockLen;
    uint8_t u8CheckSumCal;
    uint8_t u8CheckSumOrg;
    static bool bFullDay;
    static uint32_t u32ExeTime = 0;


    u16StrLen = UART_RecvDataLen(DB9_COMPORT_DEF);

    if(!u16StrLen)
        return;

    //UART_PeekRecvData(DB9_COMPORT_DEF, rbBuf[DB9_COMPORT_DEF].u16Tail, (uint8_t *)cMessage, u16StrLen);

    //cMessage[u16StrLen] = 0;

    //DB9printf("\r\n> Receive %d bytes from DB9 COMPORT:\n%s\nDB9 message end.", u16StrLen, cMessage);

    // If ring buffer is full before reading data then reset ring buffer
    if(u16StrLen >= (rbBuf[DB9_COMPORT_DEF].u16Size * 3 / 4))
    {
        RingBuf_Reset(&rbBuf[DB9_COMPORT_DEF]);

        return;
    }

    // If find Terminal command header
    if(UART_RecvFind(DB9_COMPORT_DEF, cRS232CmdHeader, &u16StrOrgPos))
    {
        uint8_t u8TimeOut = 100;


        bRes = FALSE;

        while(u8TimeOut--)
        {
          bRes = UART_RecvFindPos(DB9_COMPORT_DEF, cRS232CmdEnd, &u16StrPos, u16StrOrgPos);

            if(bRes)
                break;

            HAL_Delay(1);
        }

        // If find RS232 command end
        if(bRes)
        {
            u16StrLen = (rbBuf[DB9_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                        rbBuf[DB9_COMPORT_DEF].u16Size;

            memset(cMessage, 0x00, sizeof(cMessage));

            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            //cMessage[u16StrLen] = 0;

            if(u16StrLen < UART_BASE_BUFFER_SIZE)
            {
                Protocl_ChkCmdPkg(cMessage, NULL);
            }
        }
        else
        {
          u16StrLen = strlen(cRS232CmdHeader);

            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            cMessage[u16StrLen] = 0;
        }
    }

    // If find the PC command header sequence
    if(UART_RecvFind(DB9_COMPORT_DEF, (char*)u8PC_CmdHeader, &u16StrPos))
    {
        //DB9printf("\r\n> Found message header.");

        bPC_Trans = TRUE;

        HAL_Delay(100);

        u16StrOrgPos = u16StrPos;

        HAL_Delay(10);

        // Move to the command byte
        u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

        u16DatCnt = 2;

        // Peek 1 byte command
        u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 1);

        // If cannot get 1 byte
        if(!u16StrLen)
        {
            u8ProcRetry++;

            // If error more than 3 times
            if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
            {
                u8ProcRetry = 0;

                // Read out all data in the Ring buffer
                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                // Send Error message to PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                bPC_Trans = FALSE;

                DB9printf("\r\n> Fail to parse PC command.");

                DB9printf("\r\n> PC message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);
            }

            return;
        }

        u16StrPos = (u16StrPos + 1) % rbBuf[DB9_COMPORT_DEF].u16Size;

        u16DatCnt += 1;

        uint8_t u8Cmd = (uint8_t)cMessage[0];

        //DB9printf("\r\n> Command:0x%02x", u8Cmd);

        // Check command
        switch(u8Cmd)
        {
            case RS232CMD_PC_DATA: // Case data requirement message from PC
            {
                osTimerStop(timerRTOS_PCTransHandle);

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if(u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC data message.");

                        DB9printf("\r\n> PC data message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                                 ((uint16_t)cMessage[0] & 0x00FF);

                //DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                // If require 24h data
                if(u16DatBlockLen == 4)
                {
                    bFullDay = TRUE;

                    DB9printf("\r\n> 24 hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_DAY);

                    // Peek 5 bytes from the ring buffer, including 4 bytes data and 1 bytes checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    // If cannot get 5 bytes
                    if(u16StrLen != 5)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message date data.");

                            DB9printf("\r\n> PC data message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u16StrPos = (u16StrPos + 5) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 5;

                    // Create file name to file
                    sprintf(cFileName, "%s/D%02d%02d%02d%02d_235959_%s.VDR",
                                    FOLDERNAME_VDR, cMessage[0], cMessage[1], cMessage[2], cMessage[3],
                                    g_Setting.tBusInfo.cBus_ID);

                    //DB9printf("\r\n> Find file name:%s", cFileName);

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    //DB9printf("\r\n> PC data message:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    uint8_t u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    uint8_t u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t*)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t*)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    //DB9printf("\r\n> PC data message checksum matched.");

                    // Find the required file in the SD card
                    //bRes = SD_FindFileName(cFindName, cFileName);

                    bRes = SD_CheckFileExist(cFileName);

                    // If the file is not found
                    if(!bRes)
                    {
                        // Send error message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> File not found.");

                        return;
                    }

                    // Get the file size
                    u32FilSize = SD_GetFileSize(cFileName);

                    DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                    // Send response message F0 to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F0;
                    cMessage[3] = 0x04;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                    cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                    cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                    cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                    cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                    u8FilTransStep = STEP_F5;

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                    DB9printf("\r\n> Sent F0.");
                    //DB9printf("\r\n> Sent F0 to PC:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);
                }
                else if(u16DatBlockLen == 1) // If 1 hour data is required
                {
                    bFullDay = FALSE;

                    DB9printf("\r\n> Hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_HOUR);

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if(u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message hour data.");

                            DB9printf("\r\n> PC data message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    //DB9printf("\r\n> PC data message:");

                    //for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    //DB9printf("\r\n> PC data message checksum matched.");

                    if(bMakeFile)
                    {
                        bMakeFile = FALSE;

                        if(bDoneHourVDRFile == FALSE)
                        {
                            DB9printf("\r\n> HourVDR file is not done yet.", cFileName);

                            // Send F0 message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F0;
                            cMessage[3] = 0x04;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = 0x00;
                            cMessage[7] = 0x00;
                            cMessage[8] = 0x00;
                            cMessage[9] = 0x00;
                            cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                            u8FilTransStep = STEP_IDLE;

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                            osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                            DB9printf("\r\n> Sent F0.");

                            break;
                        }

                        sprintf(cFileName, "%s", cPC_HourFileName);
                    }
                    else
                    {
                      struct tm tmTimeNow = {0};
                        time_t ttTimeNow = System_GetTime();


                        // Convert local time in second to local time in calendar format
                        localtime_r(&ttTimeNow, &tmTimeNow);

                        // Get current date
                        uint16_t u16Year = tmTimeNow.tm_year + 1900U;
                        uint8_t u8Month = tmTimeNow.tm_mon + 1U;
                        uint8_t u8Day = tmTimeNow.tm_mday;
                        uint8_t u8Hour = tmTimeNow.tm_hour;

                        // Create file name to find
                        sprintf(cFileName, "D%02d%02d%02d%02d_%02d_%02d_%s.VDR",
                                        (uint8_t)(u16Year / 100), (uint8_t)(u16Year % 100), u8Month, u8Day,
                                        u8Hour, u8HourCnt, g_Setting.tBusInfo.cBus_ID);
                    }

                    DB9printf("\r\n> Check the existance of file name in SD card:%s", cFileName);

                    // Find the required file in the SD card
                    bRes = SD_CheckFileExist(cFileName);

                    // If the required file is not found
                    if(!bRes)
                    {
                        DB9printf("\r\n> Not find file %s in SD card.", cFileName);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = 0x00;
                        cMessage[7] = 0x00;
                        cMessage[8] = 0x00;
                        cMessage[9] = 0x00;
                        cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                        u8FilTransStep = STEP_IDLE;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F0.");

                        //return;
                    }
                    else
                    {
                        // Get file size
                        u32FilSize = SD_GetFileSize(cFileName);

                        DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                        cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                        cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                        cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                        cMessage[10] = CalCheckSum((uint8_t*)cMessage, 10);

                        u8FilTransStep = STEP_F5;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Send F0.");
                    }
                }
                else // If the data length is both not 1 and 4
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Unidentified PC data message data length.");

                    DB9printf("\r\n> PC data message:");

                    for(int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }
            case RS232CMD_PC_ACK_OK: // Case data ACK
            {
                // Peek 2 bytes from the ring buffer, including the reserve and checksum byte
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                // If cannot get 2 bytes
                if(u16StrLen < 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Parse ACK message fail.");

                        DB9printf("\r\n> PC ACK message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 2;

                // Read out the current message
                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                //DB9printf("\r\n> PC message:");

                //for(int i = 0; i < u16DatCnt; i++)
                //  DB9printf("0x%02x ", cMessage[i]);

                // Calculate checksum
                u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                u8CheckSumOrg = cMessage[u16DatCnt - 1];

                u16DatCnt = 0;

                // If checksum is not matched
                if(u8CheckSumCal != u8CheckSumOrg)
                {
                    // Send Error message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> ACK message checksum unmatched");

                    osTimerStop(timerRTOS_PCTransHandle);

                    return;
                }

                //DB9printf("\r\n> Checksum matched.");

                DB9printf("\r\n> Receive ACK OK.");

                //if(u8FilTransStep > 0)
                //  DB9printf("\r\n> Step: F%d", u8FilTransStep - 1);

                // Check the current step of the session
                switch((uint8_t)u8FilTransStep)
                {
                    case STEP_IDLE:
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        // Send error message
                        //cMessage[0] = u8PC_RespHeader[0];
                        //cMessage[1] = u8PC_RespHeader[1];
                        //cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        //cMessage[3] = 0x00;
                        //cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        //UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F0: // Step F0
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        break;
                    }

                    case STEP_F1: // Step F1
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        // Send F1 message
                        u32FilDatCnt = 0;

                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F1;
                        cMessage[3] = 0x00;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                        u8FilTransStep = STEP_F2;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                        DB9printf("\r\n> Sent F1.");

                        u32ExeTime = HAL_GetTick();

                        //if(SD_CheckFileExist("PC_Download_Data.txt"))
                        //  SD_RemoveFile("PC_Download_Data.txt");

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        break;
                    }

                    case STEP_F2: // Step F2
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        // If there still is data to send
                        if(u32FilDatCnt < u32FilSize)
                        {
                            //DB9printf("\r\n> DatCnt:%d", u32FilDatCnt);

                            // Calculate the amount of data to transfer
                            uint32_t u32DatSize = u32FilSize - u32FilDatCnt;

                            if(u32DatSize > RS232FILE_TRANSFER_MAX_LEN)
                                u32DatSize = RS232FILE_TRANSFER_MAX_LEN;
                            else if(u32DatSize < RS232FILE_TRANSFER_MAX_LEN)
                                memset(u8TransBuf, 0x00, sizeof(u8TransBuf));

                            uint8_t *u8BufPtr = (uint8_t *)(&u8TransBuf[6]);

                            // Read the data from the file in the SD card to the buffer
                            bRes = SD_ReadAddrToBuff(cFileName, u8BufPtr, u32DatSize, u32FilDatCnt);

                            // If not read successfully
                            if(!bRes)
                            {
                                // Terminate current session
                                u8FilTransStep = STEP_IDLE;

                                // Send Error message to the server
                                cMessage[0] = u8PC_RespHeader[0];
                                cMessage[1] = u8PC_RespHeader[1];
                                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                                cMessage[3] = 0x00;
                                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                                UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                                bPC_Trans = FALSE;

                                DB9printf("\r\n> Read file fail.");

                                if(bFullDay == FALSE)
                                    SD_RemoveFile(cFileName);

                                break;
                            }

                            u32FilDatCnt += u32DatSize;

                            // Send data message F2
                            u8TransBuf[0] = u8PC_RespHeader[0];
                            u8TransBuf[1] = u8PC_RespHeader[1];
                            u8TransBuf[2] = RS232CMD_RESP_F2;
                            u8TransBuf[3] = (uint8_t)(u32DatSize & 0xFF);
                            u8TransBuf[4] = (uint8_t)((u32DatSize >> 8) & 0xFF);
                            u8TransBuf[5] = 0x00;
                            u8TransBuf[6 + RS232FILE_TRANSFER_MAX_LEN] = CalCheckSum((uint8_t *)u8TransBuf, (6 + RS232FILE_TRANSFER_MAX_LEN));

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)u8TransBuf, (7 + RS232FILE_TRANSFER_MAX_LEN));

                            //DB9printf("\r\n> Sent F2.");
                            //DB9printf("\r\n> Sent F2. Length: %d bytes.", 7 + RS232FILE_TRANSFER_MAX_LEN);

                            //SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)"\r\n", 2);

                            //SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)u8TransBuf, 7 + RS232FILE_TRANSFER_MAX_LEN);
                        }
                        else // If all data in the file is already sent
                        {
                            // Send F3 message
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F3;
                            cMessage[3] = 0x00;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                            u8FilTransStep = STEP_F3;

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                            DB9printf("\r\n Sent F3.");

                            u32ExeTime = HAL_GetTick() - u32ExeTime;

                            DB9printf("\r\n> Download time: %dms", u32ExeTime);

                            if(bFullDay == FALSE)
                                SD_RemoveFile(cFileName);
                        }

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        break;
                    }

                    case STEP_F3: // Case step F3
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        DB9printf("\r\n> Finish transfer data file.");

                        // Finish the current session
                        u8FilTransStep = STEP_IDLE;

                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F4:
                    {
                        bPC_Trans = FALSE;

                        break;
                    }

                    case STEP_F5:
                    {
                        uint16_t u16FileNameLen = strlen(cFileName);

                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F5;
                        cMessage[3] = (uint8_t)u16FileNameLen;
                        cMessage[4] = (uint8_t)(u16FileNameLen >> 8);
                        cMessage[5] = 0x00;

                        //strcpy(&cMessage[6], cFileName);
                        memcpy(&cMessage[6], cFileName, u16FileNameLen);

                        cMessage[6 + u16FileNameLen] = CalCheckSum((uint8_t *)cMessage, 6 + u16FileNameLen);

                        u8FilTransStep = STEP_F1;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, (7 + u16FileNameLen));

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F5.");

                        break;
                    }

                    default:
                        break;
                }

                break;
            }
            case RS232CMD_PC_ACK_ERR: // Case ACK error
            {
                if((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                // Terminate the current session
                u8FilTransStep = STEP_IDLE;

                // Read out the current message
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                if(u16StrLen != 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u16DatCnt += u16StrLen;

                        u8ProcRetry = 0;

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse ACK error message.");

                        DB9printf("\r\n> PC ACK error message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16DatCnt += 2;

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> PC ACK error message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                break;
            }
            case RS232CMD_PC_MAKEFILE:
            {
                bFullDay = FALSE;

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if(u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC makefile message.");

                        DB9printf("\r\n> PC makefile message:");

                        for(int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                                 ((uint16_t)cMessage[0] & 0x00FF);

                DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                if(u16DatBlockLen == 1) // If 1 hour data is required
                {
                    DB9printf("\r\n> Hours data required.");

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if(u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if(u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC makefile hour data.");

                            DB9printf("\r\n> PC makefile message:");

                            for(int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if(u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC makefile message checksum unmatched.");

                        return;
                    }

                    DB9printf("\r\n> PC makefile message checksum matched.");

                    // Send ACK to make file message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F4;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F4.");

                    DB9printf("\r\n> Create hour file.");

                    bMakeFile = TRUE;

                    bFullDay = FALSE;

                    u8HourDataCnt = u8HourCnt;

                    bHourDataFlag = TRUE;

                    bDLRS232Flag = TRUE;

                    bDoHourVDRFlag = TRUE;

                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;
                }
                else // If the data length is not 1
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    //cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> PC makefile message data length is not 1.");

                    DB9printf("\r\n> PC makefile message:");

                    for(int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }
            default: // Case the message is unidentified
            {
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 8);

                u16DatCnt += u16StrLen;

                // Send ERROR message to the PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                if((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> Unidentified PC command: 0x%02X.", u8Cmd);

                DB9printf("\r\n> PC message:");

                for(int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                u8FilTransStep = STEP_IDLE;

                break;
            }
        }
    }
}
*/

//============================================================================//
// @brief  Handle communication with PC terminal.
// @param  None.
// @retval None.
//============================================================================//
void UART_TerminalProc(void)
{
    bool bRes;
    char cMessage[128];
    char cRS232CmdHeader[] = "$AS+",
         cRS232CmdEnd[] = "\r\n";
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos;

    // If use SET comport to communicate with PC terminal
    if (UART_IsTerminalUsingSET() == TRUE)
    {
        u16StrLen = UART_RecvDataLen(SET_COMPORT_DEF);

        taskInfo[TASK_RS232_PROC].u16TaskStep++;

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

            bRes = FALSE;

            while (u8TimeOut--)
            {
                bRes = UART_RecvFindPos(SET_COMPORT_DEF, cRS232CmdEnd, &u16StrPos, u16StrOrgPos);

                if (bRes)
                    break;

                HAL_Delay(1);
            }

            taskInfo[TASK_RS232_PROC].u16TaskStep++;

            // If find Terminal command end
            if (bRes)
            {
                u16StrLen = (rbBuf[SET_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                            rbBuf[SET_COMPORT_DEF].u16Size;

                memset(cMessage, 0x00, sizeof(cMessage));

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                if (u16StrLen < UART_BASE_BUFFER_SIZE)
                {
                    Protocl_ChkCmdPkg(cMessage, NULL);
                }
            }
            else
            {
                u16StrLen = strlen(cRS232CmdHeader);

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                cMessage[u16StrLen] = 0;
            }
        }
    }
    else // If use DB9 comport to communicate with PC terminal
    {
        u16StrLen = UART_RecvDataLen(DB9_COMPORT_DEF);

        taskInfo[TASK_RS232_PROC].u16TaskStep++;

        if (!u16StrLen)
            return;

        // If ring buffer is full before reading data then reset ring buffer
        if (u16StrLen >= (rbBuf[DB9_COMPORT_DEF].u16Size * 3 / 4))
        {
            RingBuf_Reset(&rbBuf[DB9_COMPORT_DEF]);

            return;
        }

        // If find Terminal command header
        if (UART_RecvFind(DB9_COMPORT_DEF, cRS232CmdHeader, &u16StrOrgPos))
        {
            uint8_t u8TimeOut = 100;

            bRes = FALSE;

            while (u8TimeOut--)
            {
                bRes = UART_RecvFindPos(DB9_COMPORT_DEF, cRS232CmdEnd, &u16StrPos, u16StrOrgPos);

                if (bRes)
                    break;

                HAL_Delay(1);
            }

            taskInfo[TASK_RS232_PROC].u16TaskStep++;

            // If find Terminal command end
            if (bRes)
            {
                u16StrLen = (rbBuf[DB9_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                            rbBuf[DB9_COMPORT_DEF].u16Size;

                memset(cMessage, 0x00, sizeof(cMessage));

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                if (u16StrLen < UART_BASE_BUFFER_SIZE)
                {
                    Protocl_ChkCmdPkg(cMessage, NULL);
                }
            }
            else
            {
                u16StrLen = strlen(cRS232CmdHeader);

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

                cMessage[u16StrLen] = 0;
            }
        }
    }
}

//============================================================================//
// @brief  Handle PC file transfer.
// @param  None.
// @retval None.
//============================================================================//
void UART_PC_TransProc(void)
{
    bool bRes;
    static char cFileName[50] = {0};
    char cMessage[128];
    const uint8_t u8PC_CmdHeader[] = {0xAA, 0x75},
                  u8PC_RespHeader[] = {0x55, 0x7A};
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos, u16DatCnt;
    static uint32_t u32FilSize, u32FilDatCnt;
    uint16_t u16DatBlockLen;
    uint8_t u8CheckSumCal;
    uint8_t u8CheckSumOrg;
    static bool bFullDay;
    static uint32_t u32ExeTime = 0;

    // If use SET comport for PC file transfer
    if (UART_IsTerminalUsingSET() == FALSE)
    {
        // If find the PC command header sequence
        if (UART_RecvFind(SET_COMPORT_DEF, (char *)u8PC_CmdHeader, &u16StrPos))
        {
            // DB9printf("\r\n> Found message header.");
            RTOSprintf("\r\n> Found message header.");

            bPC_Trans = TRUE;

            HAL_Delay(100);

            u16StrOrgPos = u16StrPos;

            HAL_Delay(10);

            // Move to the command byte
            u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

            u16DatCnt = 2;

            // Peek 1 byte command
            u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 1);

            // If cannot get 1 byte
            if (!u16StrLen)
            {
                u8ProcRetry++;

                // If error more than 3 times
                if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                {
                    u8ProcRetry = 0;

                    // Read out all data in the Ring buffer
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Send Error message to PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Fail to parse PC command.");

                    DB9printf("\r\n> PC message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                return;
            }

            u16StrPos = (u16StrPos + 1) % rbBuf[SET_COMPORT_DEF].u16Size;

            u16DatCnt += 1;

            uint8_t u8Cmd = (uint8_t)cMessage[0];

            // DB9printf("\r\n> Command:0x%02x", u8Cmd);

            // Check command
            switch (u8Cmd)
            {
            case RS232CMD_PC_DATA: // Case data requirement message from PC
            {
                osTimerStop(timerRTOS_PCTransHandle);

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if (u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC data message.");

                        DB9printf("\r\n> PC data message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                 ((uint16_t)cMessage[0] & 0x00FF);

                // DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                // If require 24h data
                if (u16DatBlockLen == 4)
                {
                    bFullDay = TRUE;

                    DB9printf("\r\n> 24 hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_DAY);

                    // Peek 5 bytes from the ring buffer, including 4 bytes data and 1 bytes checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    // If cannot get 5 bytes
                    if (u16StrLen != 5)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message date data.");

                            DB9printf("\r\n> PC data message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u16StrPos = (u16StrPos + 5) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 5;

                    // Create file name to file
                    sprintf(cFileName, "%s/D%02d%02d%02d%02d_235959_%s.VDR",
                            FOLDERNAME_VDR, cMessage[0], cMessage[1], cMessage[2], cMessage[3],
                            g_Setting.tBusInfo.cBus_ID);

                    // DB9printf("\r\n> Find file name:%s", cFileName);

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    // DB9printf("\r\n> PC data message:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    uint8_t u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    uint8_t u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    // DB9printf("\r\n> PC data message checksum matched.");

                    // Find the required file in the SD card
                    // bRes = SD_FindFileName(cFindName, cFileName);

                    bRes = SD_CheckFileExist(cFileName);

                    // If the file is not found
                    if (!bRes)
                    {
                        // Send error message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> File not found.");

                        return;
                    }

                    // Get the file size
                    u32FilSize = SD_GetFileSize(cFileName);

                    DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                    // Send response message F0 to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F0;
                    cMessage[3] = 0x04;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                    cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                    cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                    cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                    cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                    u8FilTransStep = STEP_F5;

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                    DB9printf("\r\n> Sent F0.");
                    // DB9printf("\r\n> Sent F0 to PC:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);
                }
                else if (u16DatBlockLen == 1) // If 1 hour data is required
                {
                    bFullDay = FALSE;

                    DB9printf("\r\n> Hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_HOUR);

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if (u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message hour data.");

                            DB9printf("\r\n> PC data message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    // DB9printf("\r\n> PC data message:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    // DB9printf("\r\n> PC data message checksum matched.");

                    if (bMakeFile)
                    {
                        bMakeFile = FALSE;

                        if (bDoneHourVDRFile == FALSE)
                        {
                            DB9printf("\r\n> HourVDR file is not done yet.", cFileName);

                            // Send F0 message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F0;
                            cMessage[3] = 0x04;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = 0x00;
                            cMessage[7] = 0x00;
                            cMessage[8] = 0x00;
                            cMessage[9] = 0x00;
                            cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                            u8FilTransStep = STEP_IDLE;

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                            osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                            DB9printf("\r\n> Sent F0.");

                            break;
                        }

                        sprintf(cFileName, "%s", cPC_HourFileName);
                    }
                    else
                    {
                        struct tm tmTimeNow = {0};
                        time_t ttTimeNow = System_GetTime();

                        // Convert local time in second to local time in calendar format
                        localtime_r(&ttTimeNow, &tmTimeNow);

                        // Get current date
                        uint16_t u16Year = tmTimeNow.tm_year + 1900U;
                        uint8_t u8Month = tmTimeNow.tm_mon + 1U;
                        uint8_t u8Day = tmTimeNow.tm_mday;
                        uint8_t u8Hour = tmTimeNow.tm_hour;

                        // Create file name to find
                        sprintf(cFileName, "%s/D%02d%02d%02d%02d_%02d_%02d_%s.VDR",
                                FOLDERNAME_VDR, (uint8_t)(u16Year / 100), (uint8_t)(u16Year % 100), u8Month, u8Day,
                                u8Hour, u8HourCnt, g_Setting.tBusInfo.cBus_ID);
                    }

                    DB9printf("\r\n> Check the existance of file name in SD card:%s", cFileName);

                    // Find the required file in the SD card
                    bRes = SD_CheckFileExist(cFileName);

                    // If the required file is not found
                    if (!bRes)
                    {
                        DB9printf("\r\n> Not find file %s in SD card.", cFileName);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = 0x00;
                        cMessage[7] = 0x00;
                        cMessage[8] = 0x00;
                        cMessage[9] = 0x00;
                        cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                        u8FilTransStep = STEP_IDLE;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F0.");

                        // return;
                    }
                    else
                    {
                        // Get file size
                        u32FilSize = SD_GetFileSize(cFileName);

                        DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                        cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                        cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                        cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                        cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                        u8FilTransStep = STEP_F5;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Send F0.");
                    }
                }
                else // If the data length is both not 1 and 4
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Unidentified PC data message data length.");

                    DB9printf("\r\n> PC data message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }
            case RS232CMD_PC_ACK_OK: // Case data ACK
            {
                // Peek 2 bytes from the ring buffer, including the reserve and checksum byte
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                // If cannot get 2 bytes
                if (u16StrLen < 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Parse ACK message fail.");

                        DB9printf("\r\n> PC ACK message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 2;

                // Read out the current message
                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                // DB9printf("\r\n> PC message:");

                // for(int i = 0; i < u16DatCnt; i++)
                //   DB9printf("0x%02x ", cMessage[i]);

                // Calculate checksum
                u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                u8CheckSumOrg = cMessage[u16DatCnt - 1];

                u16DatCnt = 0;

                // If checksum is not matched
                if (u8CheckSumCal != u8CheckSumOrg)
                {
                    // Send Error message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> ACK message checksum unmatched");

                    osTimerStop(timerRTOS_PCTransHandle);

                    return;
                }

                // DB9printf("\r\n> Checksum matched.");

                DB9printf("\r\n> Receive ACK OK.");

                // if(u8FilTransStep > 0)
                //   DB9printf("\r\n> Step: F%d", u8FilTransStep - 1);

                // Check the current step of the session
                switch ((uint8_t)u8FilTransStep)
                {
                case STEP_IDLE:
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    u8ProcRetry = 0;

                    // Send error message
                    // cMessage[0] = u8PC_RespHeader[0];
                    // cMessage[1] = u8PC_RespHeader[1];
                    // cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    // cMessage[3] = 0x00;
                    // cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    // UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F0: // Step F0
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    break;
                }

                case STEP_F1: // Step F1
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    // Send F1 message
                    u32FilDatCnt = 0;

                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F1;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    u8FilTransStep = STEP_F2;

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F1.");

                    u32ExeTime = HAL_GetTick();

                    // if(SD_CheckFileExist("PC_Download_Data.txt"))
                    //   SD_RemoveFile("PC_Download_Data.txt");

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    break;
                }

                case STEP_F2: // Step F2
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    // If there still is data to send
                    if (u32FilDatCnt < u32FilSize)
                    {
                        // DB9printf("\r\n> DatCnt:%d", u32FilDatCnt);

                        // Calculate the amount of data to transfer
                        uint32_t u32DatSize = u32FilSize - u32FilDatCnt;

                        if (u32DatSize > RS232FILE_TRANSFER_MAX_LEN)
                            u32DatSize = RS232FILE_TRANSFER_MAX_LEN;
                        else if (u32DatSize < RS232FILE_TRANSFER_MAX_LEN)
                            memset(u8TransBuf, 0x00, sizeof(u8TransBuf));

                        uint8_t *u8BufPtr = (uint8_t *)(&u8TransBuf[6]);

                        // Read the data from the file in the SD card to the buffer
                        bRes = SD_ReadAddrToBuff(cFileName, u8BufPtr, u32DatSize, u32FilDatCnt);

                        // If not read successfully
                        if (!bRes)
                        {
                            // Terminate current session
                            u8FilTransStep = STEP_IDLE;

                            // Send Error message to the server
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Read file fail.");

                            if (bFullDay == FALSE)
                                SD_RemoveFile(cFileName);

                            break;
                        }

                        u32FilDatCnt += u32DatSize;

                        // Send data message F2
                        u8TransBuf[0] = u8PC_RespHeader[0];
                        u8TransBuf[1] = u8PC_RespHeader[1];
                        u8TransBuf[2] = RS232CMD_RESP_F2;
                        u8TransBuf[3] = (uint8_t)(u32DatSize & 0xFF);
                        u8TransBuf[4] = (uint8_t)((u32DatSize >> 8) & 0xFF);
                        u8TransBuf[5] = 0x00;
                        u8TransBuf[6 + RS232FILE_TRANSFER_MAX_LEN] = CalCheckSum((uint8_t *)u8TransBuf, (6 + RS232FILE_TRANSFER_MAX_LEN));

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)u8TransBuf, (7 + RS232FILE_TRANSFER_MAX_LEN));

                        DB9printf("\r\n> Sent F2.");

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        // DB9printf("\r\n> Sent F2. Length: %d bytes.", 7 + RS232FILE_TRANSFER_MAX_LEN);

                        // SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)"\r\n", 2);

                        // SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)u8TransBuf, 7 + RS232FILE_TRANSFER_MAX_LEN);
                    }
                    else // If all data in the file is already sent
                    {
                        // Send F3 message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F3;
                        cMessage[3] = 0x00;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                        u8FilTransStep = STEP_F3;

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                        DB9printf("\r\n Sent F3.");

                        u32ExeTime = HAL_GetTick() - u32ExeTime;

                        DB9printf("\r\n> Download time: %dms", u32ExeTime);

                        if (bFullDay == FALSE)
                            SD_RemoveFile(cFileName);
                    }

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    break;
                }

                case STEP_F3: // Case step F3
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    DB9printf("\r\n> Finish transfer data file.");

                    // Finish the current session
                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F4:
                {
                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F5:
                {
                    char cFileNameRaw[64] = {0};
                    char *pcPtr;
                    uint16_t u16SeqPos;

                    pcPtr = cFileName;

                    if (FindSequence(cFileName, (char *)"/", strlen(cFileName), 1, &u16SeqPos) == TRUE)
                    {
                        pcPtr += u16SeqPos + 1;
                    }

                    strcpy(cFileNameRaw, pcPtr);

                    uint16_t u16FileNameLen = strlen(cFileNameRaw);

                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F5;
                    cMessage[3] = (uint8_t)u16FileNameLen;
                    cMessage[4] = (uint8_t)(u16FileNameLen >> 8);
                    cMessage[5] = 0x00;

                    memcpy(&cMessage[6], cFileNameRaw, u16FileNameLen);

                    cMessage[6 + u16FileNameLen] = CalCheckSum((uint8_t *)cMessage, 6 + u16FileNameLen);

                    u8FilTransStep = STEP_F1;

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, (7 + u16FileNameLen));

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    DB9printf("\r\n> Sent F5.");

                    break;
                }

                default:
                    break;
                }

                break;
            }
            case RS232CMD_PC_ACK_ERR: // Case ACK error
            {
                if ((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                // Terminate the current session
                u8FilTransStep = STEP_IDLE;

                // Read out the current message
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                if (u16StrLen != 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u16DatCnt += u16StrLen;

                        u8ProcRetry = 0;

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse ACK error message.");

                        DB9printf("\r\n> PC ACK error message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16DatCnt += 2;

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> PC ACK error message:");

                for (int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                break;
            }
            case RS232CMD_PC_MAKEFILE:
            {
                bFullDay = FALSE;

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if (u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC makefile message.");

                        DB9printf("\r\n> PC makefile message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[SET_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                 ((uint16_t)cMessage[0] & 0x00FF);

                DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                if (u16DatBlockLen == 1) // If 1 hour data is required
                {
                    DB9printf("\r\n> Hours data required.");

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if (u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC makefile hour data.");

                            DB9printf("\r\n> PC makefile message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[SET_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    // Read out the current message
                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC makefile message checksum unmatched.");

                        return;
                    }

                    DB9printf("\r\n> PC makefile message checksum matched.");

                    // Send ACK to make file message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F4;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F4.");

                    DB9printf("\r\n> Create hour file.");

                    bMakeFile = TRUE;

                    bFullDay = FALSE;

                    u8HourDataCnt = u8HourCnt;

                    bHourDataFlag = TRUE;

                    bDLRS232Flag = TRUE;

                    bDoHourVDRFlag = TRUE;

                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;
                }
                else // If the data length is not 1
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    // cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> PC makefile message data length is not 1.");

                    DB9printf("\r\n> PC makefile message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }

            default: // Case the message is unidentified
            {
                u16StrLen = UART_PeekRecvData(SET_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 8);

                u16DatCnt += u16StrLen;

                // Send ERROR message to the PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)cMessage, 5);

                if ((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                UART_GetRecvDataPos(SET_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> Unidentified PC command: 0x%02X.", u8Cmd);

                DB9printf("\r\n> PC message:");

                for (int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                u8FilTransStep = STEP_IDLE;

                break;
            }
            }
        }
    }
    else // If use DB9 comport for PC file transfer
    {
        // If find the PC command header sequence
        if (UART_RecvFind(DB9_COMPORT_DEF, (char *)u8PC_CmdHeader, &u16StrPos))
        {
            // DB9printf("\r\n> Found message header.");
            RTOSprintf("\r\n> Found message header.");

            bPC_Trans = TRUE;

            HAL_Delay(100);

            u16StrOrgPos = u16StrPos;

            HAL_Delay(10);

            // Move to the command byte
            u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

            u16DatCnt = 2;

            // Peek 1 byte command
            u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 1);

            // If cannot get 1 byte
            if (!u16StrLen)
            {
                u8ProcRetry++;

                // If error more than 3 times
                if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                {
                    u8ProcRetry = 0;

                    // Read out all data in the Ring buffer
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Send Error message to PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Fail to parse PC command.");

                    DB9printf("\r\n> PC message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                return;
            }

            u16StrPos = (u16StrPos + 1) % rbBuf[DB9_COMPORT_DEF].u16Size;

            u16DatCnt += 1;

            uint8_t u8Cmd = (uint8_t)cMessage[0];

            // DB9printf("\r\n> Command:0x%02x", u8Cmd);

            // Check command
            switch (u8Cmd)
            {
            case RS232CMD_PC_DATA: // Case data requirement message from PC
            {
                osTimerStop(timerRTOS_PCTransHandle);

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if (u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC data message.");

                        DB9printf("\r\n> PC data message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                 ((uint16_t)cMessage[0] & 0x00FF);

                // DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                // If require 24h data
                if (u16DatBlockLen == 4)
                {
                    bFullDay = TRUE;

                    DB9printf("\r\n> 24 hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_DAY);

                    // Peek 5 bytes from the ring buffer, including 4 bytes data and 1 bytes checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    // If cannot get 5 bytes
                    if (u16StrLen != 5)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message date data.");

                            DB9printf("\r\n> PC data message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u16StrPos = (u16StrPos + 5) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 5;

                    // Create file name to file
                    sprintf(cFileName, "%s/D%02d%02d%02d%02d_235959_%s.VDR",
                            FOLDERNAME_VDR, cMessage[0], cMessage[1], cMessage[2], cMessage[3],
                            g_Setting.tBusInfo.cBus_ID);

                    // DB9printf("\r\n> Find file name:%s", cFileName);

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    // DB9printf("\r\n> PC data message:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    uint8_t u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    uint8_t u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    // DB9printf("\r\n> PC data message checksum matched.");

                    // Find the required file in the SD card
                    // bRes = SD_FindFileName(cFindName, cFileName);

                    bRes = SD_CheckFileExist(cFileName);

                    // If the file is not found
                    if (!bRes)
                    {
                        // Send error message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> File not found.");

                        return;
                    }

                    // Get the file size
                    u32FilSize = SD_GetFileSize(cFileName);

                    DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                    // Send response message F0 to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F0;
                    cMessage[3] = 0x04;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                    cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                    cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                    cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                    cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                    u8FilTransStep = STEP_F5;

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                    DB9printf("\r\n> Sent F0.");
                    // DB9printf("\r\n> Sent F0 to PC:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);
                }
                else if (u16DatBlockLen == 1) // If 1 hour data is required
                {
                    bFullDay = FALSE;

                    DB9printf("\r\n> Hours data required.");

                    DB9printf("\r\n> [SYS]%04X", SYS_STATE_UART_DL_HOUR);

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if (u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC data message hour data.");

                            DB9printf("\r\n> PC data message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    // DB9printf("\r\n> PC data message:");

                    // for(int i = 0; i < u16DatCnt; i++)
                    //	DB9printf("0x%02x ", cMessage[i]);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC data message checksum unmatched.");

                        return;
                    }

                    // DB9printf("\r\n> PC data message checksum matched.");

                    if (bMakeFile)
                    {
                        bMakeFile = FALSE;

                        if (bDoneHourVDRFile == FALSE)
                        {
                            DB9printf("\r\n> HourVDR file is not done yet.", cFileName);

                            // Send F0 message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_F0;
                            cMessage[3] = 0x04;
                            cMessage[4] = 0x00;
                            cMessage[5] = 0x00;
                            cMessage[6] = 0x00;
                            cMessage[7] = 0x00;
                            cMessage[8] = 0x00;
                            cMessage[9] = 0x00;
                            cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                            u8FilTransStep = STEP_IDLE;

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                            osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                            DB9printf("\r\n> Sent F0.");

                            break;
                        }

                        sprintf(cFileName, "%s", cPC_HourFileName);
                    }
                    else
                    {
                        struct tm tmTimeNow = {0};
                        time_t ttTimeNow = System_GetTime();

                        // Convert local time in second to local time in calendar format
                        localtime_r(&ttTimeNow, &tmTimeNow);

                        // Get current date
                        uint16_t u16Year = tmTimeNow.tm_year + 1900U;
                        uint8_t u8Month = tmTimeNow.tm_mon + 1U;
                        uint8_t u8Day = tmTimeNow.tm_mday;
                        uint8_t u8Hour = tmTimeNow.tm_hour;

                        // Create file name to find
                        sprintf(cFileName, "%s/D%02d%02d%02d%02d_%02d_%02d_%s.VDR",
                                FOLDERNAME_VDR, (uint8_t)(u16Year / 100), (uint8_t)(u16Year % 100), u8Month, u8Day,
                                u8Hour, u8HourCnt, g_Setting.tBusInfo.cBus_ID);
                    }

                    DB9printf("\r\n> Check the existance of file name in SD card:%s", cFileName);

                    // Find the required file in the SD card
                    bRes = SD_CheckFileExist(cFileName);

                    // If the required file is not found
                    if (!bRes)
                    {
                        DB9printf("\r\n> Not find file %s in SD card.", cFileName);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = 0x00;
                        cMessage[7] = 0x00;
                        cMessage[8] = 0x00;
                        cMessage[9] = 0x00;
                        cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                        u8FilTransStep = STEP_IDLE;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Sent F0.");

                        // return;
                    }
                    else
                    {
                        // Get file size
                        u32FilSize = SD_GetFileSize(cFileName);

                        DB9printf("\r\n> Found file %s with size:%d", cFileName, u32FilSize);

                        // Send F0 message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F0;
                        cMessage[3] = 0x04;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = (uint8_t)(u32FilSize & 0xFF);
                        cMessage[7] = (uint8_t)((u32FilSize >> 8) & 0xFF);
                        cMessage[8] = (uint8_t)((u32FilSize >> 16) & 0xFF);
                        cMessage[9] = (uint8_t)((u32FilSize >> 24) & 0xFF);
                        cMessage[10] = CalCheckSum((uint8_t *)cMessage, 10);

                        u8FilTransStep = STEP_F5;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 11);

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        DB9printf("\r\n> Send F0.");
                    }
                }
                else // If the data length is both not 1 and 4
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> Unidentified PC data message data length.");

                    DB9printf("\r\n> PC data message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }
            case RS232CMD_PC_ACK_OK: // Case data ACK
            {
                // Peek 2 bytes from the ring buffer, including the reserve and checksum byte
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                // If cannot get 2 bytes
                if (u16StrLen < 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        osTimerStop(timerRTOS_PCTransHandle);

                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Parse ACK message fail.");

                        DB9printf("\r\n> PC ACK message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 2;

                // Read out the current message
                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                // DB9printf("\r\n> PC message:");

                // for(int i = 0; i < u16DatCnt; i++)
                //   DB9printf("0x%02x ", cMessage[i]);

                // Calculate checksum
                u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                u8CheckSumOrg = cMessage[u16DatCnt - 1];

                u16DatCnt = 0;

                // If checksum is not matched
                if (u8CheckSumCal != u8CheckSumOrg)
                {
                    // Send Error message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> ACK message checksum unmatched");

                    osTimerStop(timerRTOS_PCTransHandle);

                    return;
                }

                // DB9printf("\r\n> Checksum matched.");

                DB9printf("\r\n> Receive ACK OK.");

                // if(u8FilTransStep > 0)
                //   DB9printf("\r\n> Step: F%d", u8FilTransStep - 1);

                // Check the current step of the session
                switch ((uint8_t)u8FilTransStep)
                {
                case STEP_IDLE:
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    u8ProcRetry = 0;

                    // Send error message
                    // cMessage[0] = u8PC_RespHeader[0];
                    // cMessage[1] = u8PC_RespHeader[1];
                    // cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    // cMessage[3] = 0x00;
                    // cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    // UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F0: // Step F0
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    break;
                }

                case STEP_F1: // Step F1
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    // Send F1 message
                    u32FilDatCnt = 0;

                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F1;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    u8FilTransStep = STEP_F2;

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F1.");

                    u32ExeTime = HAL_GetTick();

                    // if(SD_CheckFileExist("PC_Download_Data.txt"))
                    //   SD_RemoveFile("PC_Download_Data.txt");

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    break;
                }

                case STEP_F2: // Step F2
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    // If there still is data to send
                    if (u32FilDatCnt < u32FilSize)
                    {
                        // DB9printf("\r\n> DatCnt:%d", u32FilDatCnt);

                        // Calculate the amount of data to transfer
                        uint32_t u32DatSize = u32FilSize - u32FilDatCnt;

                        if (u32DatSize > RS232FILE_TRANSFER_MAX_LEN)
                            u32DatSize = RS232FILE_TRANSFER_MAX_LEN;
                        else if (u32DatSize < RS232FILE_TRANSFER_MAX_LEN)
                            memset(u8TransBuf, 0x00, sizeof(u8TransBuf));

                        uint8_t *u8BufPtr = (uint8_t *)(&u8TransBuf[6]);

                        // Read the data from the file in the SD card to the buffer
                        bRes = SD_ReadAddrToBuff(cFileName, u8BufPtr, u32DatSize, u32FilDatCnt);

                        // If not read successfully
                        if (!bRes)
                        {
                            // Terminate current session
                            u8FilTransStep = STEP_IDLE;

                            // Send Error message to the server
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Read file fail.");

                            if (bFullDay == FALSE)
                                SD_RemoveFile(cFileName);

                            break;
                        }

                        u32FilDatCnt += u32DatSize;

                        // Send data message F2
                        u8TransBuf[0] = u8PC_RespHeader[0];
                        u8TransBuf[1] = u8PC_RespHeader[1];
                        u8TransBuf[2] = RS232CMD_RESP_F2;
                        u8TransBuf[3] = (uint8_t)(u32DatSize & 0xFF);
                        u8TransBuf[4] = (uint8_t)((u32DatSize >> 8) & 0xFF);
                        u8TransBuf[5] = 0x00;
                        u8TransBuf[6 + RS232FILE_TRANSFER_MAX_LEN] = CalCheckSum((uint8_t *)u8TransBuf, (6 + RS232FILE_TRANSFER_MAX_LEN));

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)u8TransBuf, (7 + RS232FILE_TRANSFER_MAX_LEN));

                        DB9printf("\r\n> Sent F2.");

                        osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                        // DB9printf("\r\n> Sent F2. Length: %d bytes.", 7 + RS232FILE_TRANSFER_MAX_LEN);

                        // SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)"\r\n", 2);

                        // SD_AddBuffToFile("PC_Download_Data.txt", (uint8_t *)u8TransBuf, 7 + RS232FILE_TRANSFER_MAX_LEN);
                    }
                    else // If all data in the file is already sent
                    {
                        // Send F3 message
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_F3;
                        cMessage[3] = 0x00;
                        cMessage[4] = 0x00;
                        cMessage[5] = 0x00;
                        cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                        u8FilTransStep = STEP_F3;

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                        DB9printf("\r\n Sent F3.");

                        u32ExeTime = HAL_GetTick() - u32ExeTime;

                        DB9printf("\r\n> Download time: %dms", u32ExeTime);

                        if (bFullDay == FALSE)
                            SD_RemoveFile(cFileName);
                    }

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    break;
                }

                case STEP_F3: // Case step F3
                {
                    osTimerStop(timerRTOS_PCTransHandle);

                    DB9printf("\r\n> Finish transfer data file.");

                    // Finish the current session
                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F4:
                {
                    bPC_Trans = FALSE;

                    break;
                }

                case STEP_F5:
                {
                    char cFileNameRaw[64] = {0};
                    char *pcPtr;
                    uint16_t u16SeqPos;

                    pcPtr = cFileName;

                    if (FindSequence(cFileName, (char *)"/", strlen(cFileName), 1, &u16SeqPos) == TRUE)
                    {
                        pcPtr += u16SeqPos + 1;
                    }

                    strcpy(cFileNameRaw, pcPtr);

                    uint16_t u16FileNameLen = strlen(cFileNameRaw);

                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F5;
                    cMessage[3] = (uint8_t)u16FileNameLen;
                    cMessage[4] = (uint8_t)(u16FileNameLen >> 8);
                    cMessage[5] = 0x00;

                    memcpy(&cMessage[6], cFileNameRaw, u16FileNameLen);

                    cMessage[6 + u16FileNameLen] = CalCheckSum((uint8_t *)cMessage, 6 + u16FileNameLen);

                    u8FilTransStep = STEP_F1;

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, (7 + u16FileNameLen));

                    osTimerStart(timerRTOS_PCTransHandle, (uint32_t)RS232FILE_TRANSFER_TIMEOUT);

                    DB9printf("\r\n> Sent F5.");

                    break;
                }

                default:
                    break;
                }

                break;
            }
            case RS232CMD_PC_ACK_ERR: // Case ACK error
            {
                if ((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                // Terminate the current session
                u8FilTransStep = STEP_IDLE;

                // Read out the current message
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                if (u16StrLen != 2)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u16DatCnt += u16StrLen;

                        u8ProcRetry = 0;

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse ACK error message.");

                        DB9printf("\r\n> PC ACK error message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                u8ProcRetry = 0;

                u16DatCnt += 2;

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> PC ACK error message:");

                for (int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                break;
            }
            case RS232CMD_PC_MAKEFILE:
            {
                bFullDay = FALSE;

                // Peek 3 bytes from the ring buffer, including 2 bytes data length and 1 byte
                // reserve
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 3);

                // If cannot get 3 bytes
                if (u16StrLen != 3)
                {
                    u8ProcRetry++;

                    // If error more than 3 times
                    if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                    {
                        u8ProcRetry = 0;

                        u16DatCnt += u16StrLen;

                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                        cMessage[u16DatCnt] = 0;

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> Fail to parse PC makefile message.");

                        DB9printf("\r\n> PC makefile message:");

                        for (int i = 0; i < u16DatCnt; i++)
                            DB9printf("0x%02x ", cMessage[i]);
                    }

                    return;
                }

                // Move to the data sequence
                u16StrPos = (u16StrPos + 3) % rbBuf[DB9_COMPORT_DEF].u16Size;

                u16DatCnt += 3;

                // Get the length of the data sequence
                u16DatBlockLen = (((uint16_t)cMessage[1] << 8) & 0xFF00) |
                                 ((uint16_t)cMessage[0] & 0x00FF);

                DB9printf("\r\n> Data block length:%d", u16DatBlockLen);

                if (u16DatBlockLen == 1) // If 1 hour data is required
                {
                    DB9printf("\r\n> Hours data required.");

                    // Try to peek 2 bytes, including 1 byte data and 1 byte checksum
                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 2);

                    // If cannot get 2 bytes
                    if (u16StrLen != 2)
                    {
                        u8ProcRetry++;

                        // If error more than 3 times
                        if (u8ProcRetry > RS232FILE_TRANSFER_MAX_RETRY)
                        {
                            u8ProcRetry = 0;

                            u16DatCnt += u16StrLen;

                            // Send Error message to the PC
                            cMessage[0] = u8PC_RespHeader[0];
                            cMessage[1] = u8PC_RespHeader[1];
                            cMessage[2] = RS232CMD_RESP_ACK_ERR;
                            cMessage[3] = 0x00;
                            cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                            UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                            cMessage[u16DatCnt] = 0;

                            bPC_Trans = FALSE;

                            DB9printf("\r\n> Fail to get PC makefile hour data.");

                            DB9printf("\r\n> PC makefile message:");

                            for (int i = 0; i < u16DatCnt; i++)
                                DB9printf("0x%02x ", cMessage[i]);
                        }

                        return;
                    }

                    u8ProcRetry = 0;

                    u16StrPos = (u16StrPos + 2) % rbBuf[DB9_COMPORT_DEF].u16Size;

                    u16DatCnt += 2;

                    // Get the targeted hour
                    uint8_t u8HourCnt = cMessage[0];

                    DB9printf("\r\n> %d hours required.", u8HourCnt);

                    // Read out the current message
                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    // Calculate checksum
                    u8CheckSumCal = CalCheckSum((uint8_t *)cMessage, u16DatCnt - 1);

                    u8CheckSumOrg = cMessage[u16DatCnt - 1];

                    u16DatCnt = 0;

                    // If checksum is not matched
                    if (u8CheckSumCal != u8CheckSumOrg)
                    {
                        // Send Error message to the PC
                        cMessage[0] = u8PC_RespHeader[0];
                        cMessage[1] = u8PC_RespHeader[1];
                        cMessage[2] = RS232CMD_RESP_ACK_ERR;
                        cMessage[3] = 0x00;
                        cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                        UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                        bPC_Trans = FALSE;

                        DB9printf("\r\n> PC makefile message checksum unmatched.");

                        return;
                    }

                    DB9printf("\r\n> PC makefile message checksum matched.");

                    // Send ACK to make file message to the PC
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_F4;
                    cMessage[3] = 0x00;
                    cMessage[4] = 0x00;
                    cMessage[5] = 0x00;
                    cMessage[6] = CalCheckSum((uint8_t *)cMessage, 6);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 7);

                    DB9printf("\r\n> Sent F4.");

                    DB9printf("\r\n> Create hour file.");

                    bMakeFile = TRUE;

                    bFullDay = FALSE;

                    u8HourDataCnt = u8HourCnt;

                    bHourDataFlag = TRUE;

                    bDLRS232Flag = TRUE;

                    bDoHourVDRFlag = TRUE;

                    u8FilTransStep = STEP_IDLE;

                    bPC_Trans = FALSE;
                }
                else // If the data length is not 1
                {
                    cMessage[0] = u8PC_RespHeader[0];
                    cMessage[1] = u8PC_RespHeader[1];
                    cMessage[2] = RS232CMD_RESP_ACK_ERR;
                    cMessage[3] = 0x00;
                    cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                    UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                    u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 5);

                    u16DatCnt += u16StrLen;

                    // cMessage[u16DatCnt] = 0;

                    UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                    cMessage[u16DatCnt] = 0;

                    u8ProcRetry = 0;

                    bPC_Trans = FALSE;

                    DB9printf("\r\n> PC makefile message data length is not 1.");

                    DB9printf("\r\n> PC makefile message:");

                    for (int i = 0; i < u16DatCnt; i++)
                        DB9printf("0x%02x ", cMessage[i]);
                }

                break;
            }

            default: // Case the message is unidentified
            {
                u16StrLen = UART_PeekRecvData(DB9_COMPORT_DEF, u16StrPos, (uint8_t *)cMessage, 8);

                u16DatCnt += u16StrLen;

                // Send ERROR message to the PC
                cMessage[0] = u8PC_RespHeader[0];
                cMessage[1] = u8PC_RespHeader[1];
                cMessage[2] = RS232CMD_RESP_ACK_ERR;
                cMessage[3] = 0x00;
                cMessage[4] = CalCheckSum((uint8_t *)cMessage, 4);

                UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)cMessage, 5);

                if ((u8FilTransStep == STEP_F1) || (u8FilTransStep == STEP_F2) || (u8FilTransStep == STEP_F3))
                {
                    osTimerStop(timerRTOS_PCTransHandle);
                }

                UART_GetRecvDataPos(DB9_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16DatCnt);

                cMessage[u16DatCnt] = 0;

                u8ProcRetry = 0;

                bPC_Trans = FALSE;

                DB9printf("\r\n> Unidentified PC command: 0x%02X.", u8Cmd);

                DB9printf("\r\n> PC message:");

                for (int i = 0; i < u16DatCnt; i++)
                    DB9printf("0x%02x ", cMessage[i]);

                u8FilTransStep = STEP_IDLE;

                break;
            }
            }
        }
    }
}

//============================================================================//
// @brief  Process timer callback for PC download function.
// @param  None.
// @retval None.
//============================================================================//
void UART_PC_TransCallBackHandle(void)
{
    printf("\r\n> Waiting for PC response timeout. Terminate the current file transfer operation!");

    bPrintfEn = TRUE;

    u8FilTransStep = STEP_IDLE;

    u8ProcRetry = 0;

    bPC_Trans = FALSE;
}

#if ENABLE_DVR
//============================================================================//
// @brief  Handle DVR transfer.
// @param  None.
// @retval None.
//============================================================================//
void UART_DVR_TransProc(void)
{
    // RTOSprintf("\r\n> ### UART_DVR_TransProc ###");

    if (g_Setting.u8DB9Func == 0)
    {
        UART_DVR_RelayControl();
    }
    else if (g_Setting.u8DB9Func == 1)
    {
        UART_DVR_SendSecData();
    }
}

//============================================================================//
// @brief  Send Sec Data.
// @param  None.
// @retval None.
//============================================================================//
void UART_DVR_SendSecData(void)
{
    char cTime[20];
    char *pStr;
    uint8_t ChkSum = 0x00, cBuf[128];
    time_t ttNowtime;
    uint16_t u16RpmValue;
    uint32_t u32SpeedValue;
    struct tm tmTime;

    // RTOSprintf("\r\n> ### UART_DVR_TransProc ###");

    ttNowtime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    localtime_r(&ttNowtime, &tmTime);

    strftime(cTime, sizeof(cTime), "%Y%m%d%H%M%S", &tmTime);

    if ((g_SysInfo.tVehicleState.bACC_ON == 1) && (g_SysInfo.tVehicleState.bPWR_ON == 1))
    {
        u16RpmValue = g_SysInfo.tVehicleState.u16RPM;
        u32SpeedValue = M_2_100M(g_SysInfo.tVehicleState.u32Speed);
    }
    else
    {
        u16RpmValue = 0;
        u32SpeedValue = 0;
    }

    memset(cBuf, 0x00, sizeof(cBuf));

    pStr = (char *)cBuf;

    sprintf(pStr, "*%10s,%08X,%s,%c%04d%02d%04d,%c%02d%02d%04d,%d%d%d%d%d%d%d%d,%04d,%04d,%010d,",
            g_Setting.tBusInfo.cBus_ID,
            g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum,
            cTime,
            'E',
            g_SysInfo.tGPS_Data.gpsLon.u8ddd,
            g_SysInfo.tGPS_Data.gpsLon.u8mm,
            g_SysInfo.tGPS_Data.gpsLon.u16mmmm,
            'N',
            g_SysInfo.tGPS_Data.gpsLat.u8dd,
            g_SysInfo.tGPS_Data.gpsLat.u8mm,
            g_SysInfo.tGPS_Data.gpsLat.u16mmmm,
            g_SysInfo.tVehicleState.bACC_ON,
            g_SysInfo.tEXTIO.bits.IN_1,
            g_SysInfo.tEXTIO.bits.IN_2,
            g_SysInfo.tEXTIO.bits.IN_3,
            g_SysInfo.tEXTIO.bits.IN_4,
            g_SysInfo.tEXTIO.bits.IN_5,
            g_SysInfo.tEXTIO.bits.IN_6,
            g_SysInfo.tEXTIO.bits.IN_7,
            u32SpeedValue,
            u16RpmValue,
            (uint32_t)g_SysInfo.tVehicleState.dDistance);

    do
    {
        ChkSum ^= *pStr++;

    } while (*pStr);

    ChkSum &= 0xFF;

    sprintf(pStr, "%02X\r\n", ChkSum);

    pStr = (char *)cBuf;

    if (UART_IsTerminalUsingSET())
    {
        UART_SendBuffer(DB9_COMPORT_DEF, cBuf, strlen(pStr));
    }
    else
    {
        UART_SendBuffer(SET_COMPORT_DEF, cBuf, strlen(pStr));
    }
}

//============================================================================//
// @brief  Control external relay.
// @param  None.
// @retval None.
//============================================================================//
void UART_DVR_RelayControl(void)
{
    static bool bRelayControlFlag = FALSE, bDoOneFlag = FALSE, bDoOneFlag2 = FALSE;
    static uint32_t u32TimeCnt = 0;
    char cBuf;

    if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) >= 3) && (bDoOneFlag == FALSE))
    {
        u32TimeCnt = HAL_GetTick();

        bRelayControlFlag = TRUE;

        bDoOneFlag = TRUE;

        bDoOneFlag2 = FALSE;

        cBuf = 'A';

        if (UART_IsTerminalUsingSET())
        {
            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
        else
        {
            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
    }

    if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) < 3) && (bDoOneFlag2 == FALSE))
    {
        u32TimeCnt = HAL_GetTick();

        bRelayControlFlag = FALSE;

        bDoOneFlag = FALSE;

        bDoOneFlag2 = TRUE;

        cBuf = 'a';

        if (UART_IsTerminalUsingSET())
        {
            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
        else
        {
            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
    }

    if ((HAL_GetTick() - u32TimeCnt) >= RELAY_RESEND_TIME)
    {
        u32TimeCnt = HAL_GetTick();

        if (bRelayControlFlag == TRUE)
        {
            cBuf = 'A';
        }
        else
        {
            cBuf = 'a';
        }

        if (UART_IsTerminalUsingSET())
        {
            UART_SendBuffer(DB9_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
        else
        {
            UART_SendBuffer(SET_COMPORT_DEF, (uint8_t *)&cBuf, 1);
        }
    }
}

#endif
