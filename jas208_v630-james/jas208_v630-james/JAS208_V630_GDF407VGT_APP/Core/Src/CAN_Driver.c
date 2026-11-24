//============================================================================//
// Includes
//============================================================================//
#include "CAN_Driver.h"

#include <stdio.h>
#include <string.h>

#include "stm32fxxx_hal_user.h"
#include "DataCollect.h"
#include "Default.h"
#include "EXTIO.h"
#include "main.h"
#include "System.h"

//============================================================================//
// User Variables
//============================================================================//
CAN_TxHeaderTypeDef CAN_TxHeaderHandle;

CAN_RxHeaderTypeDef CAN_RxHeaderHandle;

uint8_t u8CAN_TxMsg[8], u8CAN_RxMsg[8];

CAN_DATA_t tCAN_DATA;

// uint8_t CAN_LinkCount = COUNT_RELINK;

bool bCAN_Ready = FALSE;

bool bCAN_GetRX = FALSE;
bool bCAN_GetMsg = FALSE;
bool bCANGetSpeed = FALSE;
bool bCAN_GetDist = FALSE;

uint32_t u32CAN_ResetTick = 0;

#if CAN_CUSTOM == CAN_CUSTOM_NORMAL
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        // CAN_J1939_PGN61440,
        // CAN_J1939_PGN61441,
        // CAN_J1939_PGN61443,
        CAN_J1939_PGN61444,
        // CAN_J1939_PGN61445,
        // CAN_J1939_PGN61449,
        // CAN_J1939_PGN64695,
        // CAN_J1939_PGN64777,
        // CAN_J1939_PGN64893,
        // CAN_J1939_PGN64932,
        // CAN_J1939_PGN64933,
        // CAN_J1939_PGN64962,
        // CAN_J1939_PGN64977,
        // CAN_J1939_PGN65102,
        // CAN_J1939_PGN65110,
        // CAN_J1939_PGN65112,
        // CAN_J1939_PGN65131,
        CAN_J1939_PGN65132,
        // CAN_J1939_PGN65136,
        // CAN_J1939_PGN65198,
        // CAN_J1939_PGN65199,
        // CAN_J1939_PGN65216,
        CAN_J1939_PGN65217,
        // CAN_J1939_PGN65237,
        CAN_J1939_PGN65248,
        // CAN_J1939_PGN65253,
        // CAN_J1939_PGN65254,
        // CAN_J1939_PGN65257,
        // CAN_J1939_PGN65258,
        // CAN_J1939_PGN65260,
        // CAN_J1939_PGN65262,
        CAN_J1939_PGN65265,
        // CAN_J1939_PGN65266,
        // CAN_J1939_PGN65269,
        // CAN_J1939_PGN65276
};
#elif CAN_CUSTOM == CAN_CUSTOM_DAEWOO
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_J1939_PGN61444,

        CAN_J1939_PGN65132,

        CAN_J1939_PGN65217,

        CAN_J1939_PGN65248};
#elif CAN_CUSTOM == CAN_CUSTOM_SUNWIN
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_J1939_PGN65265,

        CAN_J1939_PGN65266};
#elif CAN_CUSTOM == CAN_CUSTOM_DAF
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_J1939_PGN61441,

        CAN_J1939_PGN61444,

        CAN_J1939_PGN64893,

        CAN_J1939_PGN64933,

        CAN_J1939_PGN65102,

        CAN_J1939_PGN65132,

        CAN_J1939_PGN65217,

        CAN_J1939_PGN65248,

        CAN_J1939_PGN65265};
#elif CAN_CUSTOM == CAN_CUSTOM_KINGLONG
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_J1939_PGN61444,

        CAN_J1939_PGN65132,

        CAN_J1939_PGN65217,

        CAN_J1939_PGN65248};
#elif CAN_CUSTOM == CAN_CUSTOM_GDBUS
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_J1939_PGN65514,

        CAN_J1939_PGN65515,

        CAN_J1939_PGN65534};
#elif CAN_CUSTOM == CAN_CUSTOM_MBEYE
static const CAN_PGN_t CAN_ID_FILTER_PGN[] =
    {
        CAN_MBEYE_700,

        CAN_MBEYE_720,

        CAN_MBEYE_727,

        CAN_MBEYE_760};
#endif

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Detect CAN bus resistor presence.
// @param  None.
// @retval 1: CAN bus resistor not presents, 0: CAN bus resistor presents.
//============================================================================//
bool CAN_ResDetect(void)
{
    return HW_PBGetState(CAN_DETECT);
}

// If CAN_CUSTOM is CAN_CUSTOM_NORMAL
#if CAN_CUSTOM == CAN_CUSTOM_NORMAL
//============================================================================//
// @brief  Send CAN message based on J1979 protocol.
// @param  MSG_FLAG: Command for the CAN Bus.
// @retval None.
//============================================================================//
static void CAN_SendMsg_J1979(FlagStatus MSG_FLAG)
{
    static bool PID_INIT = FALSE;
    static uint8_t SEND_COUNT;
    static PID_DEF_t PID_DEF;
    static PID_DATA_t PID_DATA;
    uint32_t u32MailBoxNbr;

    if (MSG_FLAG == RESET)
    {
        PID_INIT = TRUE;

        SEND_COUNT = 0;

        PID_DEF = (PID_DEF_t)0;

        PID_DATA = (PID_DATA_t)0;
    }

    if (SEND_COUNT)
    {
        SEND_COUNT--;

        return;
    }

    SEND_COUNT = COUNT_SEND_MSG;

    memset(u8CAN_TxMsg, 0x00, sizeof(u8CAN_TxMsg));

    if (PID_INIT == TRUE)
    {
        switch (PID_DEF)
        {
        case PID_DEF_00_1:
        case PID_DEF_00_2:
        case PID_DEF_00_3:
            u8CAN_TxMsg[0] = 0x02;
            u8CAN_TxMsg[1] = 0x01;
            u8CAN_TxMsg[2] = 0x00;

            HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeaderHandle, u8CAN_TxMsg, &u32MailBoxNbr);

            break;

        default:
            break;
        }

        PID_DEF++;

        if (PID_DEF >= PID_DEF_END)
        {
            PID_INIT = FALSE;
        }
    }
    else
    {
        switch (PID_DATA)
        {
        case PID_DATA_0C:
            u8CAN_TxMsg[0] = 0x02;
            u8CAN_TxMsg[1] = 0x01;
            u8CAN_TxMsg[2] = 0x0C;

            HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeaderHandle, u8CAN_TxMsg, &u32MailBoxNbr);

            break;

        case PID_DATA_0D:
            u8CAN_TxMsg[0] = 0x02;
            u8CAN_TxMsg[1] = 0x01;
            u8CAN_TxMsg[2] = 0x0D;

            HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeaderHandle, u8CAN_TxMsg, &u32MailBoxNbr);

            break;

        default:
            break;
        }

        PID_DATA++;

        if (PID_DATA >= PID_DATA_END)
        {
            PID_DATA = (PID_DATA_t)0;
        }
    }
}

//============================================================================//
// @brief  Initialize the MCU CAN setup based on J1939 protocol.
// @param  None.
// @retval None.
//============================================================================//
static void CAN_LinkInit_J1939(void)
{
    CAN_FilterTypeDef CAN_FilterInitStructure = {0};
    uint8_t i;
    MEM_U32_t FilterId;
    HAL_StatusTypeDef halRes;

    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Stop(&hcan1);

    HAL_CAN_DeInit(&hcan1);

    HW_PBInit(CAN1_TX, FALSE);
    HW_PBSet(CAN1_TX);

    // CAN Baudrate = 250 Kbps
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 6;
    hcan1.Init.Mode = CAN_MODE_SILENT;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    // hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
    // hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    halRes = HAL_CAN_Init(&hcan1);

    if (halRes != HAL_OK)
    {
        CANprintf("\r\n> CAN HAL initialization ERROR.");
        CANprintf2("\r\n> CAN HAL initialization ERROR.");
    }

    FilterId._U32 = CAN_FILTER_ID(0x00FFFF00, CAN_ID_EXT, CAN_RTR_DATA);

    // CAN filter init
    CAN_FilterInitStructure.FilterMaskIdHigh = FilterId._U16[1];
    CAN_FilterInitStructure.FilterMaskIdLow = FilterId._U16[0];
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0; // choosing fifo0 ti receive message
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterInitStructure.FilterActivation = CAN_FILTER_ENABLE; // enable filter bank

    for (i = 0; i < sizeof(CAN_ID_FILTER_PGN) / sizeof(CAN_PGN_t); i++)
    {
        FilterId._U32 = CAN_FILTER_ID(
            CAN_EXT_ID(0, 0, CAN_ID_FILTER_PGN[i], 0),
            CAN_ID_EXT,
            CAN_RTR_DATA);

        CAN_FilterInitStructure.SlaveStartFilterBank = sizeof(CAN_ID_FILTER_PGN) / sizeof(CAN_PGN_t); // how many filter banks do we want to assign to CAN1
        CAN_FilterInitStructure.FilterBank = i;
        CAN_FilterInitStructure.FilterIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterIdLow = FilterId._U16[0];

        HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);
    }

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    memset(&CAN_RxHeaderHandle, 0x00, sizeof(CAN_RxHeaderTypeDef));

    memset(&CAN_TxHeaderHandle, 0x00, sizeof(CAN_TxHeaderTypeDef));

    CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1939]);
    CANprintf2("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1939]);

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Start(&hcan1);

    // TID_CanProc = //SWTIM_SetTimer(TIMER_CAN_PROC, TIMER_CONTINUE, CAN_PROC_J1939);

    // if(CAN_LinkCount == 0)
    return;
}

//============================================================================//
// @brief  Initialize the MCU CAN setup based on J1979 protocol.
// @param  CanLinkType: Version of protocol J1979.
// @retval None.
//============================================================================//
static void CAN_LinkInit_J1979(CAN_LinkTypeDef tCanLinkType)
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
    MEM_U32_t FilterId;
    HAL_StatusTypeDef halRes;

    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Stop(&hcan1);

    HAL_CAN_DeInit(&hcan1);

    hcan1.Instance = CAN1;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    // CAN baud rate config
    switch (tCanLinkType)
    {
    // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_1:
    // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_2:
        // CAN baud rate = 250 Kbps
        hcan1.Init.Prescaler = 6;
        hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
        hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
        hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;

        break;

    // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_3:
    // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_4:
        // CAN baud rate = 500 Kbps
        hcan1.Init.Prescaler = 6;
        hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
        hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
        hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

        break;

    default:
        break;
    }

    HAL_CAN_Init(&hcan1);

    // CAN ID config
    switch (tCanLinkType)
    {
    // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_1:
    // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_3:
        FilterId._U32 = CAN_FILTER_ID(((uint32_t)0x7FF << 18), CAN_ID_STD, CAN_RTR_DATA);
        // CAN filter init
        CAN_FilterInitStructure.FilterMaskIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterMaskIdLow = FilterId._U16[0];
        CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
        CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
        CAN_FilterInitStructure.FilterActivation = CAN_FILTER_ENABLE;

        FilterId._U32 = CAN_FILTER_ID(((uint32_t)0x7E8 << 18), CAN_ID_STD, CAN_RTR_DATA);

        CAN_FilterInitStructure.SlaveStartFilterBank = 14;
        CAN_FilterInitStructure.FilterBank = 0;
        CAN_FilterInitStructure.FilterIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterIdLow = FilterId._U16[0];

        HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);

        memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

        memset(&CAN_RxHeaderHandle, 0x00, sizeof(CAN_RxHeaderTypeDef));

        memset(&CAN_TxHeaderHandle, 0x00, sizeof(CAN_TxHeaderTypeDef));

        // 0x7DF
        CAN_TxHeaderHandle.StdId = 0x7DF;
        CAN_TxHeaderHandle.ExtId = 0;
        CAN_TxHeaderHandle.IDE = CAN_ID_STD;
        CAN_TxHeaderHandle.RTR = CAN_RTR_DATA;
        CAN_TxHeaderHandle.DLC = 8;
        CAN_TxHeaderHandle.TransmitGlobalTime = DISABLE;

        break;

    // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_2:
    // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_4:
        //
        FilterId._U32 = CAN_FILTER_ID(0x1FFFFF00, CAN_ID_EXT, CAN_RTR_DATA);
        // CAN filter init
        CAN_FilterInitStructure.FilterMaskIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterMaskIdLow = FilterId._U16[0];
        CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
        CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
        CAN_FilterInitStructure.FilterActivation = CAN_FILTER_ENABLE;

        FilterId._U32 = CAN_FILTER_ID(0x18DAF100, CAN_ID_EXT, CAN_RTR_DATA);

        CAN_FilterInitStructure.SlaveStartFilterBank = 14;
        CAN_FilterInitStructure.FilterBank = 0;
        CAN_FilterInitStructure.FilterIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterIdLow = FilterId._U16[0];

        HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);

        memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

        memset(&CAN_RxHeaderHandle, 0x00, sizeof(CAN_RxHeaderTypeDef));

        memset(&CAN_TxHeaderHandle, 0x00, sizeof(CAN_TxHeaderTypeDef));

        // 0x18DB33F1
        CAN_TxHeaderHandle.StdId = 0;
        CAN_TxHeaderHandle.ExtId = 0x18DB33F1;
        CAN_TxHeaderHandle.IDE = CAN_ID_EXT;
        CAN_TxHeaderHandle.RTR = CAN_RTR_DATA;
        CAN_TxHeaderHandle.DLC = 8;
        CAN_TxHeaderHandle.TransmitGlobalTime = DISABLE;

        break;

    default:
        break;
    }

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    halRes = HAL_CAN_Start(&hcan1);

    if (halRes != HAL_OK)
    {
        CANprintf("\r\n> CAN HAL initialization ERROR.");
        CANprintf2("\r\n> CAN HAL initialization ERROR.");
    }

    bCAN_GetMsg = FALSE;

    CAN_SendMsg_J1979(RESET);

    switch (tCanLinkType)
    {
    // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_1:
        CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_1]);
        CANprintf2("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_1]);

        break;

    // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_2:
        CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_2]);
        CANprintf2("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_2]);

        break;

    // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_3:
        CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_3]);
        CANprintf2("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_3]);

        break;

    // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_4:
        CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_4]);
        CANprintf2("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1979_4]);

        break;

    default:
        CANprintf("\r\n> CAN-BUS link by unknow!!");
        CANprintf2("\r\n> CAN-BUS link by unknow!!");

        break;
    }

    return;
}

//============================================================================//
// @brief  Try different CAN Bus setups and select the correct one.
// @param  None.
// @retval None.
//============================================================================//
void CAN_LinkOBD(void)
{
    u32CAN_ResetTick = HAL_GetTick();

    if (g_Setting.bCAN_RelinkAll == FALSE)
    {
        // g_Setting.tCanLinkType = LINK_J1939;

        // System_SaveSetting();

        CAN_LinkInit_J1939();

        bCAN_Ready = TRUE;
    }
    else
    {
        if (g_Setting.tCanLinkType == LINK_BEGIN)
        {
            g_Setting.tCanLinkType = LINK_J1939;

            System_SaveSetting();
        }

        switch (g_Setting.tCanLinkType)
        {
        case LINK_BEGIN:
            CANprintf("\r\n> CAN-BUS connect start!!");
            CANprintf2("\r\n> CAN-BUS connect start!!");

            ////SWTIM_SetTimer(S_2_mS(TIMER_CAN_LINK), TIMER_ONESHOT, CAN_Relink);

            g_Setting.tCanLinkType = LINK_J1939;

            System_SaveSetting();

            // break;

        // SAE J1939-71, 250Kbps, CAN-ID: 29bit
        case LINK_J1939:
            CAN_LinkInit_J1939();

            bCAN_Ready = TRUE;

            break;

        // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
        case LINK_J1979_1:
        // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
        case LINK_J1979_2:
        // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
        case LINK_J1979_3:
        // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
        case LINK_J1979_4:
            CAN_LinkInit_J1979(g_Setting.tCanLinkType);

            bCAN_Ready = TRUE;

            break;

        case LINK_END:
            bCAN_Ready = FALSE;

            // if(CAN_LinkCount > 0)
            //   CAN_LinkCount--;
            // else
            //	return;

            // if(CAN_LinkCount == 0)
            //{
            //   CANprintf("\r\n> CAN-BUS connect stop!!");

            //	g_Setting.tCanLinkType = LINK_BEGIN;

            //  System_SaveSetting();

            //	return;
            //}

            // CANprintf("\r\n> CAN restart connect procedure.");

            g_Setting.tCanLinkType = LINK_J1939;

            System_SaveSetting();

            CAN_LinkInit_J1939();

            bCAN_Ready = TRUE;

            return;

        default:
            break;
        }
    }

    return;
}

//============================================================================//
// @brief  Relink CAN Bus.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Relink(void)
{
    if (g_Setting.bCAN_RelinkAll == TRUE)
    {
        // If the MCU didn't receive any message with the current CAN type then
        // change to the next type
        if (bCAN_GetRX == FALSE)
        {
            g_Setting.tCanLinkType++;

            System_SaveSetting();
        }
    }

    bCANGetSpeed = FALSE;
    bCAN_GetRX = FALSE;

    CAN_LinkOBD();
}

//============================================================================//
// @brief  Prepare for CAN Bus setup.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Config(void)
{
    bCAN_Ready = FALSE;
    bCANGetSpeed = FALSE;
    bCAN_GetRX = FALSE;

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    if (g_Setting.bCAN_RelinkAll == FALSE)
    {
        if (g_Setting.tCanLinkType != LINK_J1939)
        {
            g_Setting.tCanLinkType = LINK_J1939;

            System_SaveSetting();
        }
    }
    else
    {
        if (g_Setting.tCanLinkType >= LINK_END)
        {
            g_Setting.tCanLinkType = LINK_J1939;

            System_SaveSetting();
        }
    }

    // CAN_LinkCount = COUNT_RELINK;

    CANprintf("\r\n> CAN init.");
    CANprintf2("\r\n> CAN init.");

    CAN_LinkOBD();
}

//============================================================================//
// @brief  Parse CAN messages following protocol J1939.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t u16RPM;
    MEM_U16_t u16Speed;
    MEM_U32_t u32DIST;

    if (UART_IsDebugMode(DBG_RESERVED1))
    {
#if CAN_RX_MODE == CAN_RX_MODE_POLLING
        RTOSprintf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
#else
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
#endif
    }
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (GET_EXT_PGN(CAN_RxHeaderHandle.ExtId))
    {
    case CAN_J1939_PGN61444:
        u16RPM._U8[0] = u8CAN_RxMsg[3];
        u16RPM._U8[1] = u8CAN_RxMsg[4];

        if (u16RPM._U16 == 0xFFFF)
        {
            bCAN_GetMsg = TRUE;

            break;
        }

        tCAN_DATA.u16RPM = u16RPM._U16 / 8;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65132:
        u16Speed._U8[0] = u8CAN_RxMsg[6];
        u16Speed._U8[1] = u8CAN_RxMsg[7];

        if (bCANGetSpeed == FALSE)
        {
            bCANGetSpeed = TRUE;

            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;
        }

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

#if ENABLE_SPEED_INT_REC
        bSpeedIntFlag = TRUE;
#endif

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65217:
        // Byte0-4, High Resolution Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[0];
        u32DIST._U8[1] = u8CAN_RxMsg[1];
        u32DIST._U8[2] = u8CAN_RxMsg[2];
        u32DIST._U8[3] = u8CAN_RxMsg[3];

        // 5 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 5;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65248:
        // Byte5-7, Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[4];
        u32DIST._U8[1] = u8CAN_RxMsg[5];
        u32DIST._U8[2] = u8CAN_RxMsg[6];
        u32DIST._U8[3] = u8CAN_RxMsg[7];

        // 125 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 125;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65265:
        u16Speed._U8[0] = u8CAN_RxMsg[1];
        u16Speed._U8[1] = u8CAN_RxMsg[2];

        if (bCANGetSpeed == FALSE)
        {
            bCANGetSpeed = TRUE;

            u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;
        }

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

#if ENABLE_SPEED_INT_REC
        bSpeedIntFlag = TRUE;
#endif

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61441:
    case CAN_J1939_PGN61443:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64695:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64893:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64933:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65102:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65131:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65237:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65266:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        bCAN_GetMsg = FALSE;

        break;
    }
}

//============================================================================//
// @brief  Parse CAN messages following J1979 protocol.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg_J1979(void)
{
    if (u8CAN_RxMsg[1] == 0x41)
    {
        MEM_U16_t u16RPM;
        uint16_t u16Speed;

        switch (u8CAN_RxMsg[2])
        {
        case 0x00:
            bCAN_GetMsg = FALSE;

            break;

        case 0x0C:
            u16RPM._U8[1] = u8CAN_RxMsg[3];
            u16RPM._U8[0] = u8CAN_RxMsg[4];

            tCAN_DATA.u16RPM = u16RPM._U16 / 4;

            // CANprintf("\r\n> Get RPM");

            bCAN_GetMsg = TRUE;

            break;

        case 0x0D:
            u16Speed = u8CAN_RxMsg[3];

            if (bCANGetSpeed == FALSE)
            {
                bCANGetSpeed = TRUE;

                u8SpdFilDelayCnt = SPD_FILTER_DELAY_NUM;
            }

            tCAN_DATA.u32Speed = KM_2_M(u16Speed);

            // CANprintf("\r\n> Get speed");

            bCAN_GetMsg = TRUE;

            break;

        default:
            bCAN_GetMsg = FALSE;

            break;
        }
    }
}

//============================================================================//
// @brief  Parse CAN message.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg(void)
{
    switch (g_Setting.tCanLinkType)
    {
    // SAE J1939-71, 250Kbps, CAN-ID: 29bit
    case LINK_J1939:
        CAN_ParseMsg_J1939();

        break;

    // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_1:
    // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_2:
    // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_3:
    // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_4:
        CAN_ParseMsg_J1979();

        break;

    default:
        break;
    }
}

//============================================================================//
// @brief  Perform CAN process following J1939 protocol.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
static bool CAN_PROC_J1939(void)
{
#if CAN_RX_MODE == CAN_RX_MODE_POLLING
    if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
    {
        HAL_StatusTypeDef halRes;

        halRes = HAL_CAN_GetRxMessage(&hcan1, 0, &CAN_RxHeaderHandle, u8CAN_RxMsg);

        if (halRes == HAL_OK)
        {
            // bCAN_GetRX = TRUE;

            CAN_ParseMsg();
        }
        else
        {
            CANprintf("\r\n> Get CAN MSG HAL ERROR %d", halRes);
            // CANprintf2("\r\n> Get CAN MSG HAL ERROR %d", halRes);
        }
    }

    if (__HAL_CAN_GET_FLAG(&hcan1, CAN_FLAG_FOV0))
    {
        __HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_FOV0);
    }
#endif

    // if(bCAN_GetRX == TRUE)
    //{
    // CANprintf("\r\n> CAN received message.");

    // bCAN_GetRX = FALSE;
    //}

    if (bCAN_GetMsg == TRUE)
    {
        u32CAN_ResetTick = HAL_GetTick();

        if (CAN_RxHeaderHandle.IDE == CAN_ID_STD)
        {
            CANprintf("\r\n> CAN message:* 0x%03X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.StdId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }
        else if (CAN_RxHeaderHandle.IDE == CAN_ID_EXT)
        {
            CANprintf("\r\n> CAN message:* 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.ExtId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }

        bCAN_GetMsg = FALSE;
    }
    else
    {
        if (HAL_GetTick() > (u32CAN_ResetTick + S_2_mS(CAN_RESET_TIMEOUT)))
        {
            CANprintf("\r\n> CAN relink.");
            CANprintf2("\r\n> CAN relink.");

            CAN_Relink();
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Perform CAN process following J1979 protocol.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
static bool CAN_PROC_J1979(void)
{
    uint32_t u32Timeout = 200;

    // bCAN_GetRX = FALSE;
    bCAN_GetMsg = FALSE;

    CAN_SendMsg_J1979(SET);

    // Delay_mS(200);
    while (u32Timeout--)
    {
#if CAN_RX_MODE == CAN_RX_MODE_POLLING
        if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
        {
            HAL_StatusTypeDef halRes;

            halRes = HAL_CAN_GetRxMessage(&hcan1, 0, &CAN_RxHeaderHandle, u8CAN_RxMsg);

            if (halRes == HAL_OK)
            {
                // bCAN_GetRX = TRUE;

                CAN_ParseMsg();
            }
            else
            {
                CANprintf("\r\n> Get CAN MSG HAL ERROR %d", halRes);
                // CANprintf2("\r\n> Get CAN MSG HAL ERROR %d", halRes);
            }
        }

        if (__HAL_CAN_GET_FLAG(&hcan1, CAN_FLAG_FOV0))
        {
            __HAL_CAN_CLEAR_FLAG(&hcan1, CAN_FLAG_FOV0);
        }
#endif

        if (bCAN_GetMsg == TRUE)
            break;

        HAL_Delay(1);
    }

    // if(bCAN_GetRX == TRUE)
    //{
    // CANprintf("\r\n> CAN received message.");

    // bCAN_GetRX = FALSE;
    //}

    if (bCAN_GetMsg == TRUE)
    {
        u32CAN_ResetTick = HAL_GetTick();

        if (CAN_RxHeaderHandle.IDE == CAN_ID_STD)
        {
            CANprintf("\r\n> CAN message:* 0x%03X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.StdId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }
        else if (CAN_RxHeaderHandle.IDE == CAN_ID_EXT)
        {
            CANprintf("\r\n> CAN message:* 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.ExtId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }
    }
    else
    {
        if (HAL_GetTick() > (u32CAN_ResetTick + S_2_mS(CAN_RESET_TIMEOUT)))
        {
            CANprintf("\r\n> CAN relink.");
            CANprintf2("\r\n> CAN relink.");

            CAN_Relink();
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Perform CAN process.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Proc(void)
{
    // if(CAN_LinkCount == 0)
    //	return;

    switch (g_Setting.tCanLinkType)
    {
    // ISO15765-4, 250Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_1:
    // ISO15765-4, 250Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_2:
    // ISO15765-4, 500Kbps, CAN-ID: 11bit, ISO15031-5
    case LINK_J1979_3:
    // ISO15765-4, 500Kbps, CAN-ID: 29bit, ISO15031-5
    case LINK_J1979_4:
        CAN_PROC_J1979();

        break;

    // SAE J1939-71, 250Kbps, CAN-ID: 29bit
    case LINK_J1939:
        CAN_PROC_J1939();

        break;

    default:
        break;
    }
}

#elif CAN_CUSTOM == CAN_CUSTOM_DAEWOO || \
    CAN_CUSTOM == CAN_CUSTOM_SUNWIN ||   \
    CAN_CUSTOM == CAN_CUSTOM_KINGLONG || \
    CAN_CUSTOM == CAN_CUSTOM_DAF ||      \
    CAN_CUSTOM == CAN_CUSTOM_GDBUS

//============================================================================//
// @brief  Initialize the MCU CAN setup based on J1939 protocol.
// @param  None.
// @retval None.
//============================================================================//
static void CAN_LinkInit_J1939(void)
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
    uint8_t i;
    MEM_U32_t FilterId;
    HAL_StatusTypeDef halRes;

    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Stop(&hcan1);

    HAL_CAN_DeInit(&hcan1);

    // CAN Baudrate = 250 Kbps
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 6;
    hcan1.Init.Mode = CAN_MODE_SILENT;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    halRes = HAL_CAN_Init(&hcan1);

    if (halRes != HAL_OK)
        CANprintf("\r\n> CAN HAL initialization ERROR.");

    FilterId._U32 = CAN_FILTER_ID(0x00FFFF00, CAN_ID_EXT, CAN_RTR_DATA);

    // CAN filter init
    CAN_FilterInitStructure.FilterMaskIdHigh = FilterId._U16[1];
    CAN_FilterInitStructure.FilterMaskIdLow = FilterId._U16[0];
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterInitStructure.FilterActivation = CAN_FILTER_ENABLE;

    for (i = 0; i < sizeof(CAN_ID_FILTER_PGN) / sizeof(CAN_PGN_t); i++)
    {
        FilterId._U32 = CAN_FILTER_ID(
            CAN_EXT_ID(0, 0, CAN_ID_FILTER_PGN[i], 0),
            CAN_ID_EXT,
            CAN_RTR_DATA);

        CAN_FilterInitStructure.FilterBank = i;
        CAN_FilterInitStructure.FilterIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterIdLow = FilterId._U16[0];

        HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);
    }

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    memset(&CAN_RxHeaderHandle, 0x00, sizeof(CAN_RxHeaderTypeDef));

    memset(&CAN_TxHeaderHandle, 0x00, sizeof(CAN_TxHeaderTypeDef));

    CANprintf("\r\n> CAN-BUS link by %s", szCanLinkType[LINK_J1939]);

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Start(&hcan1);

    return;
}

//============================================================================//
// @brief  Try different CAN Bus setups and select the correct one.
// @param  None.
// @retval None.
//============================================================================//
void CAN_LinkOBD(void)
{
    CAN_LinkInit_J1939();

    bCAN_Ready = TRUE;
}

//============================================================================//
// @brief  Relink CAN Bus.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Relink(void)
{
    if (CAN_LinkCount > 0)
        CAN_LinkCount--;
    else
        return;

    if (CAN_LinkCount == 0)
    {
        CANprintf("\r\n> CAN-BUS connect stop!!");

        return;
    }

    CAN_LinkOBD();
}

//============================================================================//
// @brief  Prepare for CAN Bus setup.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Config(void)
{
    bCAN_Ready = FALSE;

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    // g_Setting.tCanLinkType = LINK_BEGIN;

    // System_SaveSetting();

    CAN_LinkCount = COUNT_RELINK;

    CAN_LinkOBD();
}

//============================================================================//
// @brief  Parse CAN messages following protocol J1939.
// @param  None.
// @retval None.
//============================================================================//
#if CAN_CUSTOM == CAN_CUSTOM_DAEWOO
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t u16RPM;
    MEM_U16_t u16Speed;
    MEM_U32_t u32DIST;

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (GET_EXT_PGN(CAN_RxHeaderHandle.ExtId))
    {
    case CAN_J1939_PGN61444:
        u16RPM._U8[0] = u8CAN_RxMsg[3];
        u16RPM._U8[1] = u8CAN_RxMsg[4];

        if (u16RPM._U16 == 0xFFFF)
            break;

        tCAN_DATA.u16RPM = u16RPM._U16 / 8;

        break;

    case CAN_J1939_PGN65132:
        u16Speed._U8[0] = u8CAN_RxMsg[6];
        u16Speed._U8[1] = u8CAN_RxMsg[7];

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

        break;

    case CAN_J1939_PGN65217:
        // Byte0-4, High Resolution Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[0];
        u32DIST._U8[1] = u8CAN_RxMsg[1];
        u32DIST._U8[2] = u8CAN_RxMsg[2];
        u32DIST._U8[3] = u8CAN_RxMsg[3];

        // 5 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 5;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        break;

    case CAN_J1939_PGN65248:
        // Byte5-7, Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[4];
        u32DIST._U8[1] = u8CAN_RxMsg[5];
        u32DIST._U8[2] = u8CAN_RxMsg[6];
        u32DIST._U8[3] = u8CAN_RxMsg[7];

        // 125 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 125;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61441:
    case CAN_J1939_PGN61443:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64695:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64893:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64933:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65102:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65131:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65237:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65265:
    case CAN_J1939_PGN65266:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        break;
    }
}
#elif CAN_CUSTOM == CAN_CUSTOM_SUNWIN
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t u16RPM;
    MEM_U16_t u16Speed;

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (GET_EXT_PGN(CAN_RxHeaderHandle.ExtId))
    {
    case CAN_J1939_PGN65265:
        u16Speed._U16 = u8CAN_RxMsg[0];

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 2;

        break;

    case CAN_J1939_PGN65266:
        u16RPM._U8[0] = u8CAN_RxMsg[0];

        u16RPM._U8[1] = u8CAN_RxMsg[1];

        tCAN_DATA.u16RPM = u16RPM._U16;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61441:
    case CAN_J1939_PGN61443:
    case CAN_J1939_PGN61444:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64695:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64893:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64933:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65102:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65131:
    case CAN_J1939_PGN65132:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65217:
    case CAN_J1939_PGN65237:
    case CAN_J1939_PGN65248:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        break;
    }
}
#elif CAN_CUSTOM == CAN_CUSTOM_DAF
//============================================================================//
// @brief  Parse CAN J1939 messages.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t u16RPM;
    MEM_U16_t u16Speed;
    MEM_U32_t u32DIST;

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (GET_EXT_PGN(CAN_RxHeaderHandle.ExtId))
    {
    case CAN_J1939_PGN61441:
        if (g_Setting.tENABLE_SET.bits.CAN_OTHER == TRUE)
        {
            tCAN_DATA.tBrakeState.u8BrakePos = (u8CAN_RxMsg[1] * 4 / 10);
        }

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN61444:
        u16RPM._U8[0] = u8CAN_RxMsg[3];
        u16RPM._U8[1] = u8CAN_RxMsg[4];

        if (u16RPM._U16 == 0xFFFF)
        {
            bCAN_GetMsg = TRUE;

            break;
        }

        tCAN_DATA.u16RPM = u16RPM._U16 / 8;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN64893:
        if (g_Setting.tENABLE_SET.bits.CAN_OTHER == TRUE)
        {
            uint8_t u8BlockID;

            u8BlockID = u8CAN_RxMsg[0] & 0x0F;

            switch (u8BlockID)
            {
            case 0:
                tCAN_DATA.tLightState.u8HighHeadBeam = u8CAN_RxMsg[1] & 0x07;
                tCAN_DATA.tLightState.u8LowHeadBeam = (u8CAN_RxMsg[1] >> 4) & 0x07;
                tCAN_DATA.tLightState.u8TurnSignal = u8CAN_RxMsg[2] & 0x07;

                break;

            case 3:
                tCAN_DATA.tBrakeState.u8AEBS_State = (u8CAN_RxMsg[2] >> 4) & 0x07;
                tCAN_DATA.u8LDWS_Inc = u8CAN_RxMsg[3] & 0x07;
                tCAN_DATA.bLDWS_En = (u8CAN_RxMsg[7] >> 4) & 0x07;

                break;

            default:
                break;
            }
        }

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN64933:
        if (g_Setting.tENABLE_SET.bits.CAN_OTHER == TRUE)
        {
            tCAN_DATA.tDoorControl.u8DoorStat[0] = u8CAN_RxMsg[0] & 0x3F;
            tCAN_DATA.tDoorControl.u8DoorStat[1] = ((u8CAN_RxMsg[0] >> 6) & 0x03) | ((u8CAN_RxMsg[1] << 2) & 0x3C);
            tCAN_DATA.tDoorControl.u8DoorStat[2] = ((u8CAN_RxMsg[1] >> 4) & 0x0F) | ((u8CAN_RxMsg[2] << 4) & 0x30);
            tCAN_DATA.tDoorControl.u8DoorStat[3] = (u8CAN_RxMsg[2] >> 2) & 0x3F;
            tCAN_DATA.tDoorControl.u8DoorStat[4] = u8CAN_RxMsg[3] & 0x3F;
            tCAN_DATA.tDoorControl.u8DoorStat[5] = ((u8CAN_RxMsg[3] >> 6) & 0x03) | ((u8CAN_RxMsg[4] << 2) & 0x3C);
            tCAN_DATA.tDoorControl.u8DoorStat[6] = ((u8CAN_RxMsg[4] >> 4) & 0x0F) | ((u8CAN_RxMsg[5] << 4) & 0x30);
            tCAN_DATA.tDoorControl.u8DoorStat[7] = (u8CAN_RxMsg[5] >> 2) & 0x3F;
            tCAN_DATA.tDoorControl.u8DoorStat[8] = u8CAN_RxMsg[6] & 0x3F;
            tCAN_DATA.tDoorControl.u8DoorStat[9] = ((u8CAN_RxMsg[6] >> 6) & 0x03) | ((u8CAN_RxMsg[7] << 2) & 0x3C);

            tCAN_DATA.tDoorControl.tDoorState.byte = 0x00;

            for (int i = 0; i < 10; i++)
            {
                if (((tCAN_DATA.tDoorControl.u8DoorStat[0] >> 2) & 0x03) == 0x01)
                    tCAN_DATA.tDoorControl.tDoorState.byte |= (0x01 << i);
            }
        }

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65102:
        if (g_Setting.tENABLE_SET.bits.CAN_OTHER == TRUE)
        {
            tCAN_DATA.tDoorControl.u8AllDoorStat = u8CAN_RxMsg[0];
        }

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65132:
        u16Speed._U8[0] = u8CAN_RxMsg[6];
        u16Speed._U8[1] = u8CAN_RxMsg[7];

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65217:
        // Byte0-4, High Resolution Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[0];
        u32DIST._U8[1] = u8CAN_RxMsg[1];
        u32DIST._U8[2] = u8CAN_RxMsg[2];
        u32DIST._U8[3] = u8CAN_RxMsg[3];

        // 5 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 5;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65248:
        // Byte5-7, Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[4];
        u32DIST._U8[1] = u8CAN_RxMsg[5];
        u32DIST._U8[2] = u8CAN_RxMsg[6];
        u32DIST._U8[3] = u8CAN_RxMsg[7];

        // 125 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 125;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65265:
        u16Speed._U8[0] = u8CAN_RxMsg[1];
        u16Speed._U8[1] = u8CAN_RxMsg[2];

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

        if (g_Setting.tENABLE_SET.bits.CAN_OTHER == TRUE)
        {
            if ((u8CAN_RxMsg[3] >> 4) & 0x01)
                tCAN_DATA.tBrakeState.bBrakeState = 1;
            else
                tCAN_DATA.tBrakeState.bBrakeState = 0;
        }

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65266:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        bCAN_GetMsg = FALSE;

        break;
    }
}
#elif CAN_CUSTOM == CAN_CUSTOM_KINGLONG
//============================================================================//
// @brief  Parse CAN J1939 messages.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t u16RPM;
    MEM_U16_t u16Speed;
    MEM_U32_t u32DIST;

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (CAN_RxHeaderHandle.ExtId)
    {
    case CAN_J1939_PGN61444:
        u16RPM._U8[0] = u8CAN_RxMsg[3];
        u16RPM._U8[1] = u8CAN_RxMsg[4];

        if (u16RPM._U16 == 0xFFFF)
        {
            bCAN_GetMsg = FALSE;

            break;
        }

        tCAN_DATA.u16RPM = u16RPM._U16 / 8;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65132:
        u16Speed._U8[0] = u8CAN_RxMsg[6];
        u16Speed._U8[1] = u8CAN_RxMsg[7];

        tCAN_DATA.u32Speed = KM_2_M(u16Speed._U16) / 256;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65217:
        // Byte0-4, High Resolution Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[0];
        u32DIST._U8[1] = u8CAN_RxMsg[1];
        u32DIST._U8[2] = u8CAN_RxMsg[2];
        u32DIST._U8[3] = u8CAN_RxMsg[3];

        // 5 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 5;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN65248:
        // Byte5-7, Total Vehicle Distance
        u32DIST._U8[0] = u8CAN_RxMsg[4];
        u32DIST._U8[1] = u8CAN_RxMsg[5];
        u32DIST._U8[2] = u8CAN_RxMsg[6];
        u32DIST._U8[3] = u8CAN_RxMsg[7];

        // 125 m/bit , 0 offset
        tCAN_DATA.fDIST = u32DIST._U32 * 125;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        bCAN_GetMsg = TRUE;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61441:
    case CAN_J1939_PGN61443:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64695:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64893:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64933:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65102:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65131:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65237:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65265:
    case CAN_J1939_PGN65266:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        bCAN_GetMsg = TRUE;

        break;
    }
}
#elif CAN_CUSTOM == CAN_CUSTOM_GDBUS
//============================================================================//
// @brief  Parse CAN J1939 messages.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg_J1939(void)
{
    MEM_U16_t RPM;
    MEM_U16_t SPEED;
    MEM_U32_t DIST;

    MEM_U16_t MOT_SPEED; // Motor Speed.

    MEM_U16_t BAT_VOLTAGE; // Motor Speed.
    MEM_U16_t BAT_CURRENT; // Motor Speed.

    MEM_U16_t CELL_VOL_MAX; // Battery cell voltage max
    MEM_U16_t CELL_VOL_MIN; // Battery cell voltage min
    MEM_U16_t CELL_TMP_MAX; // Battery cell temperature max.
    MEM_U16_t CELL_TMP_MIN; // Battery cell temperature min.

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%04X", GET_EXT_PGN(CAN_RxHeaderHandle.ExtId));
    // CANprintf("\r\n> * 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.ExtId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (CAN_RxHeaderHandle.ExtId)
    {
    case CAN_J1939_PGN61444:
        RPM._U8[0] = u8CAN_RxMsg[3];
        RPM._U8[1] = u8CAN_RxMsg[4];

        if (RPM._U16 == 0xFFFF)
            break;

        tCAN_DATA.u16RPM = RPM._U16 / 8;

        break;

    case CAN_J1939_PGN65132:
        SPEED._U8[0] = u8CAN_RxMsg[6];
        SPEED._U8[1] = u8CAN_RxMsg[7];

        tCAN_DATA.u32Speed = KM_2_M(SPEED._U16) / 256;

        break;

    case CAN_J1939_PGN65217:
        // Byte0-4, High Resolution Total Vehicle Distance
        DIST._U8[0] = u8CAN_RxMsg[0];
        DIST._U8[1] = u8CAN_RxMsg[1];
        DIST._U8[2] = u8CAN_RxMsg[2];
        DIST._U8[3] = u8CAN_RxMsg[3];

        // 5 m/bit , 0 offset
        tCAN_DATA.fDIST = DIST._U32 * 5;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        break;

    case CAN_J1939_PGN65248:
        // Byte5-7, Total Vehicle Distance
        DIST._U8[0] = u8CAN_RxMsg[4];
        DIST._U8[1] = u8CAN_RxMsg[5];
        DIST._U8[2] = u8CAN_RxMsg[6];
        DIST._U8[3] = u8CAN_RxMsg[7];

        // 125 m/bit , 0 offset
        tCAN_DATA.fDIST = DIST._U32 * 125;

        if (bCAN_GetDist == FALSE)
            bCAN_GetDist = TRUE;

        break;

    case CAN_J1939_PGN65265:
        SPEED._U8[0] = u8CAN_RxMsg[1];

        SPEED._U8[1] = u8CAN_RxMsg[2];

        tCAN_DATA.u32Speed = KM_2_M(SPEED._U16) / 256;

        break;

    case CAN_J1939_PGN65514:
        MOT_SPEED._U8[0] = u8CAN_RxMsg[0];
        MOT_SPEED._U8[1] = u8CAN_RxMsg[1];

        tCAN_DATA.tPowerState.u16MOT_SPEED = MOT_SPEED._U16;

        tCAN_DATA.tPowerState.u8BAT_CHG = u8CAN_RxMsg[4] & 0xE0;

        break;

    case CAN_J1939_PGN65515:
        BAT_VOLTAGE._U8[0] = u8CAN_RxMsg[0];
        BAT_VOLTAGE._U8[1] = u8CAN_RxMsg[1];

        tCAN_DATA.tPowerState.u16BAT_VOLTAGE = BAT_VOLTAGE._U16 * 0.1;

        BAT_CURRENT._U8[0] = u8CAN_RxMsg[2];
        BAT_CURRENT._U8[1] = u8CAN_RxMsg[3];

        tCAN_DATA.tPowerState.i16BAT_CURRENT = (int16_t)BAT_CURRENT._U16;
        tCAN_DATA.tPowerState.i16BAT_CURRENT = (tCAN_DATA.tPowerState.i16BAT_CURRENT * 0.1) - 600;

        tCAN_DATA.tPowerState.u16BAT_SOC = u8CAN_RxMsg[4] * 0.4;

        break;

    case CAN_J1939_PGN65534:
        CELL_VOL_MAX._U8[0] = u8CAN_RxMsg[0];
        CELL_VOL_MAX._U8[1] = u8CAN_RxMsg[1];

        tCAN_DATA.tPowerState.u8CELL_VOL_MAX = (uint8_t)(CELL_VOL_MAX._U16 * 0.01);

        CELL_VOL_MIN._U8[0] = u8CAN_RxMsg[2];
        CELL_VOL_MIN._U8[1] = u8CAN_RxMsg[3];

        tCAN_DATA.tPowerState.u8CELL_VOL_MIN = (uint8_t)(CELL_VOL_MIN._U16 * 0.01);

        CELL_TMP_MAX._U8[0] = u8CAN_RxMsg[4];
        CELL_TMP_MAX._U8[1] = u8CAN_RxMsg[5];

        tCAN_DATA.tPowerState.u8CELL_TMP_MAX = (uint8_t)(CELL_TMP_MAX._U16 * 0.1) - 40;

        CELL_TMP_MIN._U8[0] = u8CAN_RxMsg[6];
        CELL_TMP_MIN._U8[1] = u8CAN_RxMsg[7];

        tCAN_DATA.tPowerState.u8CELL_TMP_MIN = (uint8_t)(CELL_TMP_MIN._U16 * 0.1) - 40;

        break;

    case CAN_J1939_PGN61440:
    case CAN_J1939_PGN61441:
    case CAN_J1939_PGN61443:
    case CAN_J1939_PGN61445:
    case CAN_J1939_PGN61449:
    case CAN_J1939_PGN64695:
    case CAN_J1939_PGN64777:
    case CAN_J1939_PGN64893:
    case CAN_J1939_PGN64932:
    case CAN_J1939_PGN64933:
    case CAN_J1939_PGN64962:
    case CAN_J1939_PGN64977:
    case CAN_J1939_PGN65102:
    case CAN_J1939_PGN65110:
    case CAN_J1939_PGN65112:
    case CAN_J1939_PGN65131:
    case CAN_J1939_PGN65136:
    case CAN_J1939_PGN65198:
    case CAN_J1939_PGN65199:
    case CAN_J1939_PGN65216:
    case CAN_J1939_PGN65237:
    case CAN_J1939_PGN65253:
    case CAN_J1939_PGN65254:
    case CAN_J1939_PGN65257:
    case CAN_J1939_PGN65258:
    case CAN_J1939_PGN65260:
    case CAN_J1939_PGN65262:
    case CAN_J1939_PGN65266:
    case CAN_J1939_PGN65269:
    case CAN_J1939_PGN65276:
        bCAN_GetMsg = TRUE;

        break;

    default:
        bCAN_GetMsg = TRUE;

        break;
    }
}
#endif

//============================================================================//
// @brief  Parse CAN message.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg(void)
{
    CAN_ParseMsg_J1939();
}

//============================================================================//
// @brief  Perform CAN process following J1939 protocol.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
static bool CAN_PROC_J1939(void)
{
    static uint16_t u16CAN_WaitRestartCnt = 0;

    if (bCAN_GetRX == TRUE)
    {
        // CANprintf("\r\n> CAN received message.");

        bCAN_GetRX = FALSE;
    }

    if (bCAN_GetMsg == TRUE)
    {
        u16CAN_WaitRestartCnt = 0;

        // ClrTID_CanRelink();
        if (CAN_RxHeaderHandle.IDE == CAN_ID_STD)
        {
            CANprintf("\r\n> CAN message:* 0x%03X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.StdId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }
        else if (CAN_RxHeaderHandle.IDE == CAN_ID_EXT)
        {
            CANprintf("\r\n> CAN message:* 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.ExtId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }

        bCAN_GetMsg = FALSE;
    }
    else
    {
        u16CAN_WaitRestartCnt++;

        if (u16CAN_WaitRestartCnt > 2000)
        {
            u16CAN_WaitRestartCnt = 0;

            CANprintf("\r\n> CAN relink.");

            CAN_Relink();
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Perform CAN process.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Proc(void)
{
    if (CAN_LinkCount == 0)
        return;

    CAN_PROC_J1939();
}
#elif CAN_CUSTOM == CAN_CUSTOM_MBEYE
//============================================================================//
// @brief  Initialize the MCU CAN setup based on J1939 protocol.
// @param  None.
// @retval None.
//============================================================================//
static void CAN_LinkInit_MBEYE(void)
{
    CAN_FilterTypeDef CAN_FilterInitStructure;
    uint8_t i;
    MEM_U32_t FilterId;
    HAL_StatusTypeDef halRes;

    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_DISABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Stop(&hcan1);

    HAL_CAN_DeInit(&hcan1);

    // CAN Baudrate = 500 Kbps
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 6;
    hcan1.Init.Mode = CAN_MODE_SILENT;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    halRes = HAL_CAN_Init(&hcan1);

    if (halRes != HAL_OK)
        CANprintf("\r\n> CAN HAL initialization ERROR.");

    FilterId._U32 = CAN_FILTER_ID(((uint32_t)0x7FF << 18), CAN_ID_STD, CAN_RTR_DATA);

    // CAN filter init
    CAN_FilterInitStructure.FilterMaskIdHigh = FilterId._U16[1];
    CAN_FilterInitStructure.FilterMaskIdLow = FilterId._U16[0];
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterInitStructure.FilterActivation = CAN_FILTER_ENABLE;

    for (i = 0; i < sizeof(CAN_ID_FILTER_PGN) / sizeof(CAN_PGN_t); i++)
    {
        FilterId._U32 = CAN_FILTER_ID(((uint32_t)CAN_ID_FILTER_PGN[i] << 18),
                                      CAN_ID_STD,
                                      CAN_RTR_DATA);

        CAN_FilterInitStructure.FilterBank = i;
        CAN_FilterInitStructure.FilterIdHigh = FilterId._U16[1];
        CAN_FilterInitStructure.FilterIdLow = FilterId._U16[0];

        HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure);
    }

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    memset(&CAN_RxHeaderHandle, 0x00, sizeof(CAN_RxHeaderTypeDef));

    memset(&CAN_TxHeaderHandle, 0x00, sizeof(CAN_TxHeaderTypeDef));

    CANprintf("\r\n> CAN-BUS link by CAN MOBILEYE, 500Kbps, 11bit");

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_FULL);
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);

    HAL_CAN_Start(&hcan1);

    return;
}

//============================================================================//
// @brief  Try different CAN Bus setups and select the correct one.
// @param  None.
// @retval None.
//============================================================================//
void CAN_LinkOBD(void)
{
    CAN_LinkInit_MBEYE();

    bCAN_Ready = TRUE;
}

//============================================================================//
// @brief  Relink CAN Bus.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Relink(void)
{
    if (CAN_LinkCount > 0)
        CAN_LinkCount--;
    else
        return;

    if (CAN_LinkCount == 0)
    {
        CANprintf("\r\n> CAN-BUS connect stop!!");

        return;
    }

    CAN_LinkOBD();
}

//============================================================================//
// @brief  Prepare for CAN Bus setup.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Config(void)
{
    bCAN_Ready = FALSE;

    memset(&tCAN_DATA, 0x00, sizeof(CAN_DATA_t));

    // g_Setting.tCanLinkType = LINK_BEGIN;

    // System_SaveSetting();

    CAN_LinkCount = COUNT_RELINK;

    CAN_LinkOBD();
}

void CAN_ParseMsg_MBEYE(void)
{
    static uint8_t u8Byte2_Status = 0;
    static uint8_t u8Byte4_Status = 0;
    static uint8_t u8LDW_Cnt = 0;
    static uint32_t u32LDWCurrTickCnt = 0;
    static uint32_t u32HWLVCurrTickCnt = 0;
    static bool bHWLV_Status = 0;
    static uint8_t u8TA_Flag = 0;
    static uint8_t u8TA2_Flag = 0;
    static uint32_t u32TACurrTickCnt = 0;
#if CUSTOMER_TYPE == CUSTOMER_FORMOSA
    static uint8_t u8FCW_Cnt = 0;
    static uint32_t u32FCWCurrTickCnt = 0;
#endif

    if (UART_IsDebugMode(DBG_RESERVED1))
        printf("\r\n> CAN ID:0x%03X", CAN_RxHeaderHandle.StdId);
    // CANprintf("\r\n> * 0x%03X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
    //					CAN_RxHeaderHandle.StdId,
    //					u8CAN_RxMsg[0],
    //					u8CAN_RxMsg[1],
    //					u8CAN_RxMsg[2],
    //					u8CAN_RxMsg[3],
    //					u8CAN_RxMsg[4],
    //					u8CAN_RxMsg[5],
    //					u8CAN_RxMsg[6],
    //					u8CAN_RxMsg[7]);

    switch (CAN_RxHeaderHandle.StdId)
    {
    case CAN_MBEYE_700:
    {
        if (u8Byte2_Status != u8CAN_RxMsg[2])
        {
            u8Byte2_Status = u8CAN_RxMsg[2];

            tCAN_DATA.tCAN_MBEYE.u8HeadwayMeasurement = ((u8CAN_RxMsg[2] >> 1) & 0x7F);
        }

        if (u8Byte4_Status != u8CAN_RxMsg[4])
        {
            u8Byte4_Status = u8CAN_RxMsg[4];

            tCAN_DATA.tCAN_MBEYE.bFCW_On = ((u8CAN_RxMsg[4] >> 3) & 0x01);

#if CUSTOMER_TYPE == CUSTOMER_FORMOSA
            if (tCAN_DATA.tCAN_MBEYE.bFCW_On == 1)
            {
                if (u16_100M_SpeedAvg > 400)
                {
                    GSM_TCP_ReportEvent(MBEYE_EVENT_900, STR_EVENT_MSG);

                    u8FCW_Cnt = 0;
                }
                else
                {
                    u8FCW_Cnt++;

                    if (u8FCW_Cnt == 1)
                    {
                        u32FCWCurrTickCnt = HAL_GetTick();
                    }
                    else
                    {
                        if ((HAL_GetTick() < u32FCWCurrTickCnt + mS_2_SysTick(S_2_mS(60))))
                        {
                            if (u8FCW_Cnt >= 2)
                            {
                                GSM_TCP_ReportEvent(MBEYE_EVENT_900, STR_EVENT_MSG);

                                u8FCW_Cnt = 0;
                            }
                        }
                        else
                        {
                            u8FCW_Cnt = 0;
                        }
                    }
                }
            }
#endif

#if CUSTOMER_TYPE == CUSTOMER_FCFC
            if (tCAN_DATA.tCAN_MBEYE.bFCW_On == 1)
            {
                if (u16_100M_SpeedAvg > 400)
                {
                    GSM_TCP_ReportEvent(MBEYE_EVENT_900, STR_EVENT_MSG);
                }
            }
#endif

            tCAN_DATA.tCAN_MBEYE.bLeft_LDW = ((u8CAN_RxMsg[4] >> 1) & 0x01);
            tCAN_DATA.tCAN_MBEYE.bRight_LDW = ((u8CAN_RxMsg[4] >> 2) & 0x01);

            if ((tCAN_DATA.tCAN_MBEYE.bLeft_LDW == 1) || (tCAN_DATA.tCAN_MBEYE.bRight_LDW == 1))
            {
                u8LDW_Cnt++;

                if (u8LDW_Cnt == 1)
                {
                    u32LDWCurrTickCnt = HAL_GetTick();
                }
                else
                {
                    if (HAL_GetTick() < (u32LDWCurrTickCnt + mS_2_SysTick(S_2_mS(g_Setting.tMBEYE_SET.LDWTime))))
                    {
                        if (u8LDW_Cnt >= g_Setting.tMBEYE_SET.LDWCnt)
                        {
                            GSM_TCP_ReportEvent(MBEYE_EVENT_912, STR_EVENT_MSG);

                            u8LDW_Cnt = 0;
                        }
                    }
                    else
                    {
                        u8LDW_Cnt = 0;
                    }
                }
            }
        }

        tCAN_DATA.tCAN_MBEYE.bTamperAlert = ((u8CAN_RxMsg[5] >> 5) & 0x01);

        if (tCAN_DATA.tCAN_MBEYE.bTamperAlert == 1)
        {
            if (u8TA_Flag == 0)
            {
                u32TACurrTickCnt = HAL_GetTick();

                u8TA_Flag = 1;
            }
            else
            {
                if (0 == u8TA2_Flag)
                {
                    if (HAL_GetTick() >= (u32TACurrTickCnt + mS_2_SysTick(LENS_MASK_TIME)))
                    {
                        GSM_TCP_ReportEvent(MBEYE_EVENT_913, STR_EVENT_MSG);

                        u8TA2_Flag = 1;

                        u8TA_Flag = 0;
                    }
                }
                else
                {
                    if (HAL_GetTick() >= (u32TACurrTickCnt) + mS_2_SysTick(LENS_MASK_TIME)))
                        {
                            GSM_TCP_ReportEvent(MBEYE_EVENT_914, STR_EVENT_MSG);

                            u8TA_Flag = 0;
                        }
                }
            }
        }
        else
        {
            u8TA_Flag = 0;

            u8TA2_Flag = 0;
        }

        tCAN_DATA.tCAN_MBEYE.u8HeadwayWarningLevel = (u8CAN_RxMsg[7] & 0x03);

        if ((tCAN_DATA.tCAN_MBEYE.u8HeadwayWarningLevel == 2) &&
            (tCAN_DATA.tCAN_MBEYE.u8HeadwayMeasurement <= 12))
        {
            if (FALSE == bHWLV_Status)
            {
                u32HWLVCurrTickCnt = HAL_GetTick();

                bHWLV_Status = TRUE;
            }
            else
            {
                if (HAL_GetTick() >= (u32HWLVCurrTickCnt + mS_2_SysTick(S_2_mS(g_Setting.tMBEYE_SET.HWTime))))
                {
                    GSM_TCP_ReportEvent(MBEYE_EVENT_922, STR_EVENT_MSG);

                    bHWLV_Status = FALSE;
                }
            }
        }

        if ((tCAN_DATA.tCAN_MBEYE.u8HeadwayWarningLevel == 0) ||
            (tCAN_DATA.tCAN_MBEYE.u8HeadwayWarningLevel == 1) ||
            (tCAN_DATA.tCAN_MBEYE.u8HeadwayMeasurement >= 13))
        {
            bHWLV_Status = FALSE;
        }

        bCAN_GetMsg = TRUE;

        break;
    }

    case CAN_MBEYE_720:
    case CAN_MBEYE_727:
    case CAN_MBEYE_760:
        bCAN_GetMsg = TRUE;

        break;

    default:
        bCAN_GetMsg = TRUE;

        break;
    }
}

//============================================================================//
// @brief  Parse CAN message.
// @param  None.
// @retval None.
//============================================================================//
void CAN_ParseMsg(void)
{
    CAN_ParseMsg_MBEYE();
}

//============================================================================//
// @brief  Perform CAN process following J1939 protocol.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is failed.
//============================================================================//
static bool CAN_PROC_MBEYE(void)
{
    static uint16_t u16CAN_WaitRestartCnt = 0;

    if (bCAN_GetRX == TRUE)
    {
        // CANprintf("\r\n> CAN received message.");

        bCAN_GetRX = FALSE;
    }

    if (bCAN_GetMsg == TRUE)
    {
        u16CAN_WaitRestartCnt = 0;

        if (CAN_RxHeaderHandle.IDE == CAN_ID_STD)
        {
            CANprintf("\r\n> CAN message:* 0x%03X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.StdId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }
        else if (CAN_RxHeaderHandle.IDE == CAN_ID_EXT)
        {
            CANprintf("\r\n> CAN message:* 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                      CAN_RxHeaderHandle.ExtId,
                      u8CAN_RxMsg[0],
                      u8CAN_RxMsg[1],
                      u8CAN_RxMsg[2],
                      u8CAN_RxMsg[3],
                      u8CAN_RxMsg[4],
                      u8CAN_RxMsg[5],
                      u8CAN_RxMsg[6],
                      u8CAN_RxMsg[7]);
        }

        bCAN_GetMsg = FALSE;
    }
    else
    {
        u16CAN_WaitRestartCnt++;

        if (u16CAN_WaitRestartCnt > 2000)
        {
            u16CAN_WaitRestartCnt = 0;

            CANprintf("\r\n> CAN relink.");

            CAN_Relink();
        }
    }

    return TRUE;
}

//============================================================================//
// @brief  Perform CAN process.
// @param  None.
// @retval None.
//============================================================================//
void CAN_Proc(void)
{
    if (CAN_LinkCount == 0)
        return;

    CAN_PROC_MBEYE();
}
#endif

//============================================================================//
// @brief  Get CAN speed.
// @param  None.
// @retval Return CAN speed in uint32_t format.
//============================================================================//
uint32_t CAN_GetSpeed(void)
{
    return tCAN_DATA.u32Speed;
}

//============================================================================//
// @brief  Get CAN RPM.
// @param  None.
// @retval Return CAN RPM in uint16_t format.
//============================================================================//
uint16_t CAN_GetRPM(void)
{
    return tCAN_DATA.u16RPM;
}

//============================================================================//
// @brief  Get CAN distance.
// @param  None.
// @retval Return CAN distance in double format.
//============================================================================//
double CAN_GetDist(void)
{
    return tCAN_DATA.fDIST;
}

#if CUSTOMER_TYPE == CUSTOMER_GDBUS
//============================================================================//
// @brief  Get CAN motor speed.
// @param  None.
// @retval Return CAN motor speed in uint16_t format.
//============================================================================//
uint16_t CAN_GetMotSpeed(void)
{
    return tCAN_DATA.tPowerState.u16MOT_SPEED;
}

//============================================================================//
// @brief  Get CAN battery charging state.
// @param  None.
// @retval Return CAN battery charging state in uint8_t format.
//============================================================================//
uint8_t CAN_GetBatChg(void)
{
    return tCAN_DATA.tPowerState.u8BAT_CHG;
}

//============================================================================//
// @brief  Get CAN battery voltage.
// @param  None.
// @retval Return CAN battery voltage in uint16_t format.
//============================================================================//
uint16_t CAN_GetBatVoltage(void)
{
    return tCAN_DATA.tPowerState.u16BAT_VOLTAGE;
}

//============================================================================//
// @brief  Get CAN battery current.
// @param  None.
// @retval Return CAN battery current in int16_t format.
//============================================================================//
int16_t CAN_GetBatCurrent(void)
{
    return tCAN_DATA.tPowerState.i16BAT_CURRENT;
}

//============================================================================//
// @brief  Get CAN battery SOC.
// @param  None.
// @retval Return CAN battery SOC in uint8_t format.
//============================================================================//
uint8_t CAN_GetSOC(void)
{
    return tCAN_DATA.tPowerState.u16BAT_SOC;
}

//============================================================================//
// @brief  Get CAN cell voltage max.
// @param  None.
// @retval Return CAN cell voltage max in uint8_t format.
//============================================================================//
uint8_t CAN_GetCellVolMax(void)
{
    return tCAN_DATA.tPowerState.u8CELL_VOL_MAX;
}

//============================================================================//
// @brief  Get CAN cell voltage min.
// @param  None.
// @retval Return CAN cell voltage min in uint8_t format.
//============================================================================//
uint8_t CAN_GetCellVolMin(void)
{
    return tCAN_DATA.tPowerState.u8CELL_VOL_MIN;
}

//============================================================================//
// @brief  Get CAN cell temperature max.
// @param  None.
// @retval Return CAN cell temperature max in uint8_t format.
//============================================================================//
uint8_t CAN_GetCellTmpMax(void)
{
    return tCAN_DATA.tPowerState.u8CELL_TMP_MAX;
}

//============================================================================//
// @brief  Get CAN cell temperature min.
// @param  None.
// @retval Return CAN cell temperature min in uint8_t format.
//============================================================================//
uint8_t CAN_GetCellTmpMin(void)
{
    return tCAN_DATA.tPowerState.u8CELL_TMP_MIN;
}

//============================================================================//
#endif // if CUSTOMER_TYPE == CUSTOMER_GDBUS

#if CAN_CUSTOM == CAN_CUSTOM_MBEYE
//============================================================================//
// @brief  Get CAN MBEYE left LDW.
// @param  None.
// @retval TRUE: Left LDW is ON.
//         FALSE: Left LDW is OFF.
//============================================================================//
bool MBEYE_LeftLDW(void)
{
    return tCAN_DATA.tCAN_MBEYE.bLeft_LDW;
}

//============================================================================//
// @brief  Get CAN MBEYE right LDW.
// @param  None.
// @retval TRUE: Right LDW is ON.
//         FALSE: Right LDW is OFF.
//============================================================================//
bool MBEYE_RightLDW(void)
{
    return tCAN_DATA.tCAN_MBEYE.bRight_LDW;
}

//============================================================================//
// @brief  Get CAN MBEYE FCW.
// @param  None.
// @retval TRUE: FCW is ON.
//         FALSE: FCW is OFF.
//============================================================================//
bool MBEYE_FCWOn(void)
{
    return tCAN_DATA.tCAN_MBEYE.bFCW_On;
}

//============================================================================//
// @brief  Get CAN MBEYE headway warning level.
// @param  None.
// @retval CAN MBEYE headway warning level in uint8_t format.
//============================================================================//
uint8_t MBEYE_HeadwayWarningLevel(void)
{
    return tCAN_DATA.tCAN_MBEYE.u8HeadwayWarningLevel;
}
#endif

//============================================================================//
// @brief  Check if CAN functions are required or not.
// @param  None.
// @retval TRUE: CAN functions need to be enabled.
//         FALSE: CAN functions are not needed.
//============================================================================//
bool CAN_CheckEnable(void)
{
    bool bCAN_En = FALSE;

    if ((g_Setting.tENABLE_SET.bits.CAN_SPEED == 1) ||
        (g_Setting.tENABLE_SET.bits.CAN_RPM == 1) ||
        (g_Setting.tENABLE_SET.bits.CAN_DIST == 1) ||
        (g_Setting.tENABLE_SET.bits.CAN_OTHER == 1))
    {
        bCAN_En = TRUE;
    }

    return bCAN_En;
}
