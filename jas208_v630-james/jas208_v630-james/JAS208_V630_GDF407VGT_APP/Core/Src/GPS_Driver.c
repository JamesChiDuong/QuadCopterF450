//============================================================================//
// User Includes
//============================================================================//

#include "GPS_Driver.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "CommFunc.h"
#include "EXTIO.h"
#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
#include "GSM_Driver.h"
// #include "GSM_QtEC21.h"
#endif
#include "LCD_Key.h"
#include "main.h"
#include "MG82_Driver.h"
#if ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "System.h"
#include "UART_Driver.h"

//============================================================================//
// User Variables
//============================================================================//
GPS_SIM_TypeDef tGPS_ModuleType = GPS_MODULE_TYPE_DEFAULT;

GPS_CheckVersionTypeDef tGPS_CheckStep = GPS_CHECK_STEP_NULL;

static const char *cGPS_ModuleTypeString[] =
    {
        "SIM68",
        "SIM65"};

GPS_ServiceTypeDef tGPS_ServiceType = GPS_GPS;

static const char cGPS_ServiceSign[] =
    {
        'G',
        'R',
        'E',
        'C'};

bool bGPS_ON = FALSE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
bool bGPS_InitFlag = FALSE;

bool bGPS_GetDataEn = FALSE;
#endif

bool bGPS_DataFixed = FALSE;

bool bMCU_RTC_Updated = FALSE;

uint8_t u8GPS_RetryCnt = 0;

bool bGPS_SystembootTestFlag = FALSE;

char cGPS_Content[1024]; // __attribute__ ((section("ccmram_var")));

bool bGPS_FirstDetection = TRUE;

bool bGPS_GetInt = TRUE;

uint32_t u32GPS_IntTick = 0;

uint8_t u8GPS_ResetCnt = 0;

bool bGPS_CheckIntStop = FALSE;

uint32_t u32GPS_ScanTime = 0;

bool bGPS_BaudCorrect = FALSE;

bool bGPS_ChangeFlag = FALSE;

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
const uint8_t u8EPO_Preamble[2] = {0x04, 0x24};
const uint8_t u8EPO_EndWord[2] = {0x0D, 0x0A};
const uint8_t u8EPO_SIM65_EndWord[2] = {0xAA, 0x44};
const uint8_t u8EPO_CmdID[2] = {0xD3, 0x02};
const uint8_t u8EPO_SIM65_CmdID[2] = {0xB1, 0x04};
EPO_UpdateTypeDef tEPO_Download = {0};
EPO_StepTypeDef tEPO_Step = EPO_STEP_NULL;
EPO_SIM65_StepTypeDef tEPO_SIM65_Step = EPO_SIM65_STEP_NULL;
uint8_t u8EPO_GetDatBuf[GPS_EPO_PACK_LEN][GPS_EPO_SAT_DATA_SIZE] = {0};
EPO_SIM65_DataPktTypeDef tEPO_SIM65_SendBuf = {0};
EPO_PktTypeDef tEPO_SendBuf = {0};
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
void GPS_XTRA_RequestUpdate(char *pcFilePath, uint32_t u32Size);
bool bGPS_XTRA_Check = TRUE;
#endif

//============================================================================//
// User Functions
//============================================================================//

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
//============================================================================//
// @brief  Send data over the GPS port.
// @param  pu8Buf: Pointer to the data buffer.
//         u8Len: Number of bytes to send.
// @retval None.
//============================================================================//
void GPS_SendBuffer(uint8_t *pu8Buf, uint8_t u8Len)
{
#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 || GNSS_ENABLE_GET_LOC == 0
    UART_SendBuffer(GPS_COMPORT_DEF, pu8Buf, u8Len);
#endif
}

//============================================================================//
// @brief  Turn on GPS module if use module SIM68 as GPS module.
// @param  None.
// @retval None.
//============================================================================//
void GPS_PowerOn(void)
{
    HAL_GPIO_WritePin(GPS_PWR_EN_GPIO_Port, GPS_PWR_EN_Pin, GPIO_PIN_SET);

    bGPS_ON = TRUE;

    u32GPS_ScanTime = HAL_GetTick();
}

//============================================================================//
// @brief  Turn off GPS module if use module SIM68 as GPS module.
// @param  None.
// @retval None.
//============================================================================//
void GPS_PowerOff(void)
{
    HAL_GPIO_WritePin(GPS_PWR_EN_GPIO_Port, GPS_PWR_EN_Pin, GPIO_PIN_RESET);

    bGPS_ON = FALSE;

    u32GPS_ScanTime = 0;
}

#elif GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
//============================================================================//
// @brief  Configure the message outport to UART port.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigOutport(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_OUTPORT, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigOutport get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_OUTPORT)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigOutport get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the outport to UART port.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigNMEA_Src(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_NMEASRC, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigNMEA_Src get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_NMEASRC)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigNMEA_Src get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the required NMEA message types for GPS.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigGPS_NMEA_Type(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GPSNMEATYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigGPS_NMEA_Type get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GPSNMEATYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigGPS_NMEA_Type get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the required NMEA message types for GLONASS.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigGLONASS_NMEA_Type(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GLONASSNMEATYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigGLONASS_NMEA_Type get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GLONASSNMEATYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigGLONASS_NMEA_Type get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the required NMEA message types for Galileo.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigGalileo_NMEA_Type(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GALILEONMEATYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigGalileo_NMEA_Type get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GALILEONMEATYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigGalileo_NMEA_Type get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the required NMEA message types for Beidou.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigBeidou_NMEA_Type(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_BEIDOUNMEATYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigBeidou_NMEA_Type get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_BEIDOUNMEATYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigBeidou_NMEA_Type get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure GSV message extension.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigGSV_Ext(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GSVEXTNMEATYPE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigGSV_Ext get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GSVEXTNMEATYPE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigGSV_Ext get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the required GNSS types.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigGNSS(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GNSSCONFIG, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigGNSS get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GNSSCONFIG)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigGNSS get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the GPS automatic mode.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigAutoGPS(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_AUTOGPS, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigAutoGPS get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_AUTOGPS)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigAutoGPS get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Delete GPS messages in storage.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_DelMsg(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_DELETE, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_DelMsg get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_DELETE)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_DelMsg get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure the GPS automatic mode.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_TurnOn(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_ON, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_TurnOn get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_ON)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_TurnOn get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Turn off GPS module.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_TurnOff(void)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_OFF, &u8RetVal, 0);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_TurnOff get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_OFF)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_TurnOff get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Get the GPS positioning information.
// @param  pcBuf: Pointer to a char array to store the positioning information.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_GetLoc(char *pcBuf)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_LOCATION, &u8RetVal, 1, pcBuf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_GetLoc get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_LOCATION)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_GetLoc get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Acquire NMEA message.
// @param  pcBuf: Pointer to a char array to store the NMEA message.
//         pcType: NMEA message type string (GGA, GSA, RMC, GSV, VTG, ...)
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_GetNMEA(char *pcBuf, char *pcType)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_GETNMEA, &u8RetVal, 2, pcBuf, pcBuf);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_GetNMEA get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_GETNMEA)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_GetNMEA get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Configure XTRA service.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_ConfigXTRA(bool bEnable)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_CONFIGXTRA, &u8RetVal, 1, bEnable);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_ConfigXTRA get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_CONFIGXTRA)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_ConfigXTRA get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Set GPS XTRA time.
// @param  pcTimeStamp: GPS time stamp in UTC.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_XTRA_Time(char *pcTimeStamp)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_XTRATIME, &u8RetVal, 1, pcTimeStamp);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_XTRA_Time get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_XTRATIME)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_XTRA_Time get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Active GPS XTRA function.
// @param  pcFileName: File name to inject GPS XTRA data file.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_XTRA_Data(char *pcFileName)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_XTRADATA, &u8RetVal, 1, pcFileName);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_XTRA_Data get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_XTRADATA)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_XTRA_Data get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

//============================================================================//
// @brief  Query GPS XTRA function status.
// @param  pu16DurTime: Pointer to a variable to store the returned XTRA file
//                      duration time.
// @retval TRUE: The operation is succesful.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_XTRA_DataQuery(uint16_t *pu16DurTime, time_t *pttValidTime)
{
    uint8_t u8RetVal;
    osEvent event;

    GSM_BuildBlockCmd(QTEC21_GPS_XTRADATA_QUERY, &u8RetVal, 2, pu16DurTime, pttValidTime);

    uint32_t u32LastTick = HAL_GetTick();

    while (1)
    {
        if (HAL_GetTick() > (u32LastTick + S_2_mS(GPS_RESPONSE_TIMEOUT + 5)))
        {
            GSMprintf("\r\n> GPS_XTRA_DataQuery get timeout.");

            u8RetVal = FALSE;

            break;
        }

        event = osMessagePeek(GSM_BlockCpltHandle, S_2_mS(GPS_RESPONSE_TIMEOUT + 5));

        if (event.status == osEventMessage)
        {
            if ((uint16_t)event.value.v == QTEC21_GPS_XTRADATA_QUERY)
            {
                osMessageGet(GSM_BlockCpltHandle, S_2_mS(1));

                break;
            }
        }
        else if ((event.status == osEventTimeout) || (event.status == osErrorOS))
        {
            u8RetVal = FALSE;

            GSMprintf("\r\n> GPS_XTRA_DataQuery get event status: %d", event.status);

            GSM_Handle_Block_osEventTimeout();

            break;
        }

        HAL_Delay(1);
    }

    HAL_Delay(1);

    return (bool)u8RetVal;
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21 && ENABLE_GPS_XTRA
//============================================================================//
// @brief  Create an XTRA download request.
// @param  None.
// @retval None.
//============================================================================//
void GPS_XTRA_RequestDownload(void)
{
    HTTP_DownloadRequestTypeDef tRequest = {0};

    // If XTRA URL is empty or error
    if ((strlen(g_Setting.cXTRA_URL) == 0) ||
        (strlen(g_Setting.cXTRA_URL) >= (sizeof(g_Setting.cXTRA_URL) - 1)))
    {
        System_RestoreBackupSetting(VAR_XTRA_URL);

        if ((strlen(g_Setting.cXTRA_URL) == 0) ||
            (strlen(g_Setting.cXTRA_URL) >= (sizeof(g_Setting.cXTRA_URL) - 1)))
        {
            memset(g_Setting.cXTRA_URL, 0x00, sizeof(g_Setting.cXTRA_URL));

            sprintf(g_Setting.cXTRA_URL, "%s", (char *)GPS_XTRA_FILE_URL);

            System_SaveSetting();
        }
    }

    sprintf(tRequest.cSrcPath, "%s", (char *)g_Setting.cXTRA_URL);
    sprintf(tRequest.cSrcFileName, "%s", (char *)GPS_XTRA_FILE_NAME);
    memset(tRequest.cDstDir, 0x00, sizeof(tRequest.cDstDir));
    memset(tRequest.cDstFileName, 0x00, sizeof(tRequest.cDstDir));
    tRequest.callback = &GSM_XTRA_RequestUpdate;
    tRequest.u8RetryCnt = 0;
    tRequest.bRequest = TRUE;

    GSM_HTTP_DownloadRequest(&tRequest);
}

//============================================================================//
// @brief  Check GPS XTRA file validality.
// @param  None.
// @retval None.
//============================================================================//
static void GPS_CheckXTRA(void)
{
    bool bRes = FALSE;
    uint16_t u16Duration;
    time_t ttValidDate, ttCurrTime, ttRemainTime;
    bool bUpdate = FALSE;

    bRes = GPS_XTRA_DataQuery(&u16Duration, &ttValidDate);

    if (bRes)
    {
        // tGPS_Xtra.bGPS_XTRA_Update = FALSE;
        bUpdate = FALSE;

        ttCurrTime = System_GetTime();

        switch (u16Duration)
        {
        // 1, 3, 7 day XTRA file
        case 1440:
        case 4320:
        case 10080:
        {
            if ((ttValidDate + u16Duration * 60) >= (ttCurrTime - g_Setting.i32TimeZoneOffset))
            {
                ttRemainTime = (ttValidDate + u16Duration * 60) - (ttCurrTime - g_Setting.i32TimeZoneOffset);

                uint32_t u32Seconds, u32Minutes, u32Hours, u32Days;

                u32Seconds = ttRemainTime % 60;
                u32Minutes = ttRemainTime / 60;
                u32Hours = u32Minutes / 60;
                u32Minutes = u32Minutes % 60;
                u32Days = u32Hours / 24;
                u32Hours = u32Hours % 24;

                GSMprintf("\r\n> GPS XTRA remaining time: %02d Days %02d Hours %02d Minutes %02d Seconds",
                          u32Days,
                          u32Hours,
                          u32Minutes,
                          u32Seconds);

                if (ttRemainTime < GPS_XTRA_VALID_DUR_MIN)
                {
                    // tGPS_Xtra.bGPS_XTRA_Update = TRUE;
                    bUpdate = TRUE;

                    GPS_XTRA_RequestDownload();
                }
            }
            else
            {
                GSMprintf("\r\n> GPS XTRA remaining time: 0");

                // tGPS_Xtra.bGPS_XTRA_Update = TRUE;
                bUpdate = TRUE;

                GPS_XTRA_RequestDownload();
            }

            break;
        }

        // If there is no valid XTRA file
        case 0:
        default:
            // tGPS_Xtra.bGPS_XTRA_Update = TRUE;
            bUpdate = TRUE;

            GPS_XTRA_RequestDownload();

            break;
        }

        if (bUpdate == TRUE)
        {
            GSMprintf("\r\n> GPS XTRA request update.");
        }
    }
}
#endif

//============================================================================//
// @brief  Configure the GPS module.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_Config(void)
{
    // Config NMEA messages output port to UART debug port
    // GPS_ConfigOutport();

    // Delay_mS(100);

    GPS_ConfigNMEA_Src();

    Delay_mS(10);

    // Config to send type of NMEA messages
    GPS_ConfigGPS_NMEA_Type();

    Delay_mS(10);

    // if(tGSM_ModuleType != GSM_MODULE_EC200)
    //  Enable GSV message
    //	GPS_ConfigGSV_Ext();

    Delay_mS(10);

    // Config GLONASS message types
    GPS_ConfigGLONASS_NMEA_Type();

    Delay_mS(10);

    // Config Beidou message types
    GPS_ConfigBeidou_NMEA_Type();

    Delay_mS(10);

    // Config GLONASS, Beidou, Galileo ON/OFF
    GPS_ConfigGNSS();

    Delay_mS(10);

    // Config to disable automatic GNSS
    GPS_ConfigAutoGPS();

    if (tGSM_ModuleType == GSM_MODULE_EC200)
        return;

    Delay_mS(10);

// Turn on QGPSXTRA service
#if ENABLE_GPS_XTRA
    GPS_ConfigXTRA(1);

    if (bGPS_XTRA_Check == TRUE)
    {
        bGPS_XTRA_Check = FALSE;

        Delay_mS(50);

        GPS_CheckXTRA();
    }
#else
    GPS_ConfigXTRA(0);
#endif
}

//============================================================================//
// @brief  Enable the GPS module to send message.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_EnableSendMsg(void)
{
    // Config NMEA messages output port to UART debug port
    GPS_ConfigOutport();

    u32GPS_IntTick = HAL_GetTick();

    u32GPS_ScanTime = HAL_GetTick();

    bGPS_Ready = TRUE;
}

//============================================================================//
// @brief  Enable the GPS UART port RX interrupt.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_EnableRecvMsg(void)
{
    QtEC21_GPS_EnableRecvMsg();
}

//============================================================================//
// @brief  Start the GPS module.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_Start(void)
{
    // Config the GNSS module
    GPS_Config();

    // Turn on GNSS module
    GPS_TurnOn();
}

//============================================================================//
// @brief  Finish the GPS operation.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_Finish(void)
{
    // Turn off GNSS module
    GPS_TurnOff();

#if ENABLE_GSM_NET
    bGPS_Ready = FALSE;
#endif
}

//============================================================================//
// @brief  Parse the positioning information to get the positioning data.
// @param  pcBuf: Pointer to a char array to store the positioning information.
// @retval TRUE: The operation is done succesfully.
//         FALSE: The function is fail.
//============================================================================//
bool GPS_GNSS_ParseLoc(char *pcBuf)
{
    // bool bRes = TRUE;
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[200] = {0};
    char cTemp[10];

    if (strlen(pcBuf) > sizeof(cContent))
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    strcpy(cContent, pcBuf);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    // Get time information
    if ((strlen(pcPtr) >= 6) && (strlen(pcPtr) <= 11))
    {
        // UTC hour
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Hour = atoi(cTemp);

        // UTC minute
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Min = atoi(cTemp);

        // UTC second
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u8Sec = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get latitude information
    if (strlen(pcPtr) == 10)
    {
        // dd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.gpsLat.u8dd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.gpsLat.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 5, 4);
        gpsTempHandle.gpsLat.u16mmmm = atoi(cTemp);
    }
    else
        return FALSE;

    // Latitude direction indicator
    gpsTempHandle.gpsLat.cIndicator = *(pcPtr + 9);

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get longitude information
    if (strlen(pcPtr) == 11)
    {
        // ddd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 3);
        gpsTempHandle.gpsLon.u8ddd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 3, 2);
        gpsTempHandle.gpsLon.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 6, 4);
        gpsTempHandle.gpsLon.u16mmmm = atoi(cTemp);
    }
    else
        return FALSE;

    // Longitude direction indicator
    gpsTempHandle.gpsLon.cIndicator = *(pcPtr + 10);

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // HDOP
    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 10))
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fHDOP = atof(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Altitude
    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 10))
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fAltitude = atof(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Position fix indicator
    if (strlen(pcPtr) == 1)
    {
        uint8_t u8Fix;

        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 1);
        u8Fix = atoi(cTemp);

        if ((u8Fix == 2) || (u8Fix == 3))
            gpsTempHandle.bStatus = 1;
        else
            gpsTempHandle.bStatus = 0;
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get course over ground information
    if ((strlen(pcPtr) >= 3) && (strlen(pcPtr) <= 10))
    {
        gpsTempHandle.u16Course = (uint16_t)atof(pcPtr);

        StopChar(pcPtr, '.');

        gpsTempHandle.gpsCourse.u16ddd = atoi(pcPtr);

        pcPtr += strlen(pcPtr) + 1;

        gpsTempHandle.gpsCourse.u8mm = atoi(pcPtr);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get speed over ground information
    if ((strlen(pcPtr) >= 3) && (strlen(pcPtr) <= 10))
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.u32Speed = (uint32_t)KM_2_M(atof(cTemp));
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Pass speed over ground in knots
    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get date information
    if (strlen(pcPtr) == 6)
    {
        // dd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Day = atoi(cTemp);
        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Month = atoi(cTemp);
        // yy
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u16Year = 2000 + atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, '\r');

    // Get number of GPS satellites
    if ((strlen(pcPtr) > 0) && (strlen(pcPtr) < 3))
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.u8StlNum = atoi(cTemp);
    }
    else
        return FALSE;

    if ((gpsTempHandle.bStatus == FALSE) && (g_SysInfo.tGPS_Data.bStatus == TRUE))
    {
        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_NOT_FIX);
    }

    if ((gpsTempHandle.bStatus == TRUE) && (g_SysInfo.tGPS_Data.bStatus == FALSE))
    {
        RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_FIX);
    }

    // Only accept data if the positioning status is valid
    if (gpsTempHandle.bStatus == TRUE)
        memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));

    return TRUE;
}
#endif

//============================================================================//
// @brief  Reset the GPS module.
// @param  u8Delay: Delay time before reset.
// @retval None.
//============================================================================//
void GPS_Reset(uint8_t u8Delay)
{
    HAL_Delay(u8Delay);

    if (bGPS_ChangeFlag == TRUE)
    {
        char cMsg[10] = {0};

        bGPS_ChangeFlag = FALSE;

        if (g_Setting.u32GPS_Baudrate == BR_9600bps)
        {
            RTOSprintf("\r\n> ### Change GPS BR 115200 ###");

            g_Setting.u32GPS_Baudrate = BR_115200bps;

            System_SaveSetting();
        }
        else
        {
            RTOSprintf("\r\n> ### Change GPS BR 9600 ###");

            g_Setting.u32GPS_Baudrate = BR_9600bps;

            System_SaveSetting();
        }

        sprintf(cMsg, "%d", g_Setting.u32GPS_Baudrate);

#if ENABLE_GSM_NET
        GSM_TCP_ReportEvent((char *)EVENT_GPS_CHANGE_BR, cMsg);
#endif
    }
    else
    {
        RTOSprintf("\r\n> ### GPS BR Last Setting %d ###", g_Setting.u32GPS_Baudrate);
    }

    u8GPS_RetryCnt = 0;

    bGPS_DataFixed = FALSE;

#if ENABLE_GSM_NET && GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    bGPS_GetDataEn = FALSE;

    GPS_TurnOff();

    HAL_Delay(10);

    bGPS_InitFlag = TRUE;
#elif GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    GPS_PowerOff();

    HAL_Delay(1000);

    GPS_PowerOn();
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 || GNSS_ENABLE_GET_LOC == 0
    UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);

    __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

    if (__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_ORE))
    {
        __HAL_UART_CLEAR_OREFLAG(puartHandle[GPS_COMPORT_DEF]);
    }

    if (__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE))
    {
        __HAL_UART_CLEAR_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE);
    }
#endif
}

//============================================================================//
// @brief  Check checksum for a NMEA message.
// @param  cMsg: Pointer to the message .
// @retval TRUE: Checksum is matched.
//         FALSE: Checksum is not matched.
//============================================================================//
bool GPS_NMEA_ChkSum(char *cMsg)
{
    uint8_t u8Buf[300] = {0};
    char *pcPtr;
    uint8_t u8CheckSumOrg, u8CheckSumCal;
    uint16_t u16Pos = 0;

    memcpy(u8Buf, cMsg, strlen(cMsg));

    pcPtr = (char *)u8Buf + 1;

    if (FindSequence(pcPtr, (char *)"*", strlen(pcPtr), 1, &u16Pos) == FALSE)
    {
        // GPSprintf("\r\n> GPS NMEA message has no *");

        return FALSE;
    }

    // Split the message content and the checksum byte
    StopChar(pcPtr, '*');

    // Calculate checksum for the message content
    u8CheckSumCal = CalCheckSum((uint8_t *)pcPtr, strlen(pcPtr));

    // Get checksum byte at the end of the message
    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, '\r');

    StopChar(pcPtr, '\n');

    if (strlen(pcPtr) != 2)
    {
        GPSprintf("\r\n> GPS NMEA ERROR: %s", cMsg);

        return FALSE;
    }

    u8CheckSumOrg = strtoul(pcPtr, NULL, 16);

    // Compare two checksum value
    if (u8CheckSumCal == u8CheckSumOrg)
    {
        return TRUE;
    }
    else
    {
        GPSprintf("\r\n> GPS chksum org: %02X, cal: %02X", u8CheckSumOrg, u8CheckSumCal);

        return FALSE;
    }
}

//============================================================================//
// @brief  Parse a NMEA GGA message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_GGA_Parse(char *cMsg)
{
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[300] = {0};
    char cTemp[10];

    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    if (strlen(cMsg) > sizeof(cContent))
        return FALSE;

    strcpy(cContent, cMsg);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    // Get time information
    if (strlen(pcPtr) >= 6)
    {
        // UTC hour
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Hour = atoi(cTemp);

        // UTC minute
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Min = atoi(cTemp);

        // UTC second
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u8Sec = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get latitude information
    if (strlen(pcPtr) >= 9)
    {
        // dd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.gpsLat.u8dd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.gpsLat.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 5, 4);
        gpsTempHandle.gpsLat.u16mmmm = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Latitude direction indicator
    if (strlen(pcPtr) == 1)
    {
        gpsTempHandle.gpsLat.cIndicator = *pcPtr;
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get longitude information
    if (strlen(pcPtr) >= 10)
    {
        // ddd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 3);
        gpsTempHandle.gpsLon.u8ddd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 3, 2);
        gpsTempHandle.gpsLon.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 6, 4);
        gpsTempHandle.gpsLon.u16mmmm = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Longitude direction indicator
    if (strlen(pcPtr) == 1)
    {
        gpsTempHandle.gpsLon.cIndicator = *pcPtr;
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Position fix indicator
    if (strlen(pcPtr) == 1)
    {
        // memset(cTemp, 0x00, sizeof(cTemp));
        // strncpy(cTemp, pcPtr, 1);
        // gpsTempHandle.bStatus = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Number of satellites used
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));

        strncpy(cTemp, pcPtr, strlen(pcPtr));

        gpsTempHandle.u8StlNum = atoi(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // HDOP
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fHDOP = atof(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Altitude
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fAltitude = atof(cTemp);
    }
    else
        return FALSE;

    memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));

    // GPSprintf("\r\n> GPS satellite number:%d", g_SysInfo.tGPS_Data.u8StlNum);

    return TRUE;
}

//============================================================================//
// @brief  Parse a NMEA GLL message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_GLL_Parse(char *cMsg)
{
    bool bRes;
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[300] = {0};
    char cTemp[10];

    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return FALSE;

    if (strlen(cMsg) > sizeof(cContent))
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    strcpy(cContent, cMsg);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    // Get latitude information
    if (strlen(pcPtr) >= 9)
    {
        // ddd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.gpsLat.u8dd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.gpsLat.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 5, 4);
        gpsTempHandle.gpsLat.u16mmmm = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Latitude direction indicator
    if (strlen(pcPtr) == 1)
    {
        gpsTempHandle.gpsLat.cIndicator = *pcPtr;

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get longitude information
    if (strlen(pcPtr) >= 10)
    {
        // ddd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 3);
        gpsTempHandle.gpsLon.u8ddd = atoi(cTemp);

        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 3, 2);
        gpsTempHandle.gpsLon.u8mm = atoi(cTemp);

        //.mmmmm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 6, 4);
        gpsTempHandle.gpsLon.u16mmmm = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Longitude direction indicator
    if (strlen(pcPtr) == 1)
    {
        gpsTempHandle.gpsLon.cIndicator = *pcPtr;

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get time information
    if (strlen(pcPtr) >= 6)
    {
        // UTC hour
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Hour = atoi(cTemp);

        // UTC minute
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Min = atoi(cTemp);

        // UTC second
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u8Sec = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // GPS data status
    if ((strlen(pcPtr) == 1) && (*pcPtr == 'A'))
    {
        // gpsTempHandle.bStatus = TRUE;

        bRes = TRUE;
    }
    else if ((strlen(pcPtr) == 1) && (*pcPtr == 'V'))
    {
        // gpsTempHandle.bStatus = FALSE;

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    if (bRes)
    {
        if (!((gpsTempHandle.bStatus == TRUE) && (g_SysInfo.tGPS_Data.bStatus == FALSE)))
            memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));
    }

    return bRes;
}

//============================================================================//
// @brief  Parse a NMEA GSA message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_GSA_Parse(char *cMsg)
{
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[300] = {0};
    char cTemp[10];

    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return FALSE;

    if (strlen(cMsg) > sizeof(cContent))
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    strcpy(cContent, cMsg);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Fix
    if (strlen(pcPtr) == 1)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.u8PosFix = atoi(cTemp);
    }
    else
        return FALSE;

    for (int i = 0; i < 13; i++)
    {
        pcPtr += strlen(pcPtr) + 1;

        StopChar(pcPtr, ',');
    }

    // PDOP
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fPDOP = atof(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // HDOP
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fHDOP = atof(cTemp);
    }
    else
        return FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // VDOP
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.fVDOP = atof(cTemp);
    }
    else
        return FALSE;

    memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));

    return TRUE;
}

//============================================================================//
// @brief  Parse a NMEA GSV message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_GSV_Parse(char *cMsg)
{
    // if(g_SysInfo.tGPS_Data.bStatus == FALSE)
    //	return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Parse a NMEA RMC message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_RMC_Parse(char *cMsg)
{
    bool bRes;
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[300] = {0};
    char cTemp[10];

    if (strlen(cMsg) > sizeof(cContent))
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    strcpy(cContent, cMsg);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    // Get time information
    if (strlen(pcPtr) >= 6)
    {
        // UTC hour
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Hour = atoi(cTemp);

        // UTC minute
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Min = atoi(cTemp);

        // UTC second
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u8Sec = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // GPS data status
    if ((strlen(pcPtr) == 1) && (*pcPtr == 'A'))
    {
        gpsTempHandle.bStatus = TRUE;

        bRes = TRUE;
    }
    else if ((strlen(pcPtr) == 1) && (*pcPtr == 'V'))
    {
        gpsTempHandle.bStatus = FALSE;

        // if(g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS)
        //{
        //	g_SysInfo.tVehicleState.u32DistCount_AGain = 0;

        //	g_SysInfo.tVehicleState.dDistBase_GPS = 0;

        //	g_SysInfo.tVehicleState.dDistance_GPS = 0;
        //}

        bRes = TRUE;
    }
    else
    {
        gpsTempHandle.bStatus = FALSE;

        // if(g_Setting.tSpeedSet.u8AutoGainSrc == AUTOGAIN_SOURCE_GPS)
        //{
        //	g_SysInfo.tVehicleState.u32DistCount_AGain = 0;

        //	g_SysInfo.tVehicleState.dDistBase_GPS = 0;

        //	g_SysInfo.tVehicleState.dDistance_GPS = 0;
        //}

        bRes = FALSE;
    }

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    if ((bRes == TRUE) && (gpsTempHandle.bStatus == TRUE))
    {
        // Get latitude information
        if (strlen(pcPtr) >= 9)
        {
            // ddd
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr, 2);
            gpsTempHandle.gpsLat.u8dd = atoi(cTemp);

            // mm
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr + 2, 2);
            gpsTempHandle.gpsLat.u8mm = atoi(cTemp);

            //.mmmmm
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr + 5, 4);
            gpsTempHandle.gpsLat.u16mmmm = atoi(cTemp);

            bRes = TRUE;
        }
        else
            bRes = FALSE;
    }
    else if ((bRes == TRUE) && (gpsTempHandle.bStatus == FALSE))
    {
        // Keep Last OK coordinate, not clear zero
        // gpsTempHandle.gpsLat.u8dd = 0;
        // gpsTempHandle.gpsLat.u8mm = 0;
        // gpsTempHandle.gpsLat.u16mmmm = 0;

        bRes = TRUE;
    }
    else
    {
        bRes = FALSE;
    }

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    if ((bRes == TRUE) && (gpsTempHandle.bStatus == TRUE))
    {
        // Latitude direction indicator
        if (strlen(pcPtr) == 1)
        {
            gpsTempHandle.gpsLat.cIndicator = *pcPtr;

            bRes = TRUE;
        }
        else
            bRes = FALSE;
    }
    else if ((bRes == TRUE) && (gpsTempHandle.bStatus == FALSE))
    {
        // Keep Last OK coordinate, not clear zero
        // gpsTempHandle.gpsLat.cIndicator = ' ';

        bRes = TRUE;
    }
    else
    {
        bRes = FALSE;
    }

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    if ((bRes == TRUE) && (gpsTempHandle.bStatus == TRUE))
    {
        // Get longitude information
        if (strlen(pcPtr) >= 10)
        {
            // ddd
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr, 3);
            gpsTempHandle.gpsLon.u8ddd = atoi(cTemp);

            // mm
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr + 3, 2);
            gpsTempHandle.gpsLon.u8mm = atoi(cTemp);

            //.mmmmm
            memset(cTemp, 0x00, sizeof(cTemp));
            strncpy(cTemp, pcPtr + 6, 4);
            gpsTempHandle.gpsLon.u16mmmm = atoi(cTemp);

            bRes = TRUE;
        }
        else
            bRes = FALSE;
    }
    else if ((bRes == TRUE) && (gpsTempHandle.bStatus == FALSE))
    {
        // Keep Last OK coordinate, not clear zero
        // gpsTempHandle.gpsLon.u8ddd = 0;
        // gpsTempHandle.gpsLon.u8mm = 0;
        // gpsTempHandle.gpsLon.u16mmmm = 0;

        bRes = TRUE;
    }
    else
    {
        bRes = FALSE;
    }

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    if ((bRes == TRUE) && (gpsTempHandle.bStatus == TRUE))
    {
        // Longitude direction indicator
        if (strlen(pcPtr) == 1)
        {
            gpsTempHandle.gpsLon.cIndicator = *pcPtr;

            bRes = TRUE;
        }
        else
            bRes = FALSE;
    }
    else if ((bRes == TRUE) && (gpsTempHandle.bStatus == FALSE))
    {
        // Keep Last OK coordinate, not clear zero
        // gpsTempHandle.gpsLon.cIndicator = ' ';

        bRes = TRUE;
    }
    else
    {
        bRes = FALSE;
    }

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get speed in m/h
    if (strlen(pcPtr) > 0)
    {
        if (gpsTempHandle.bStatus == TRUE)
        {
            // Keep Last OK coordinate, not clear zero
            gpsTempHandle.u32Speed = (int)(KNOT_2_M(atof(pcPtr)));
        }

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get course in degree
    if (strlen(pcPtr) > 0)
    {
        if (gpsTempHandle.bStatus == TRUE)
        {
            gpsTempHandle.u16Course = (uint16_t)atof(pcPtr);

            StopChar(pcPtr, '.');

            gpsTempHandle.gpsCourse.u16ddd = atoi(pcPtr);

            pcPtr += strlen(pcPtr) + 1;

            gpsTempHandle.gpsCourse.u8mm = atoi(pcPtr);
        }

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // Get date information
    if (strlen(pcPtr) == 6)
    {
        // dd
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Day = atoi(cTemp);
        // mm
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Month = atoi(cTemp);
        // yy
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u16Year = 2000 + atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    if (bRes)
    {
        if ((gpsTempHandle.bStatus == FALSE) && (g_SysInfo.tGPS_Data.bStatus == TRUE))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_NOT_FIX);
        }

        if ((gpsTempHandle.bStatus == TRUE) && (g_SysInfo.tGPS_Data.bStatus == FALSE))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_GPS_FIX);
        }

        memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));

        if (g_SysInfo.tGPS_Data.bStatus == TRUE)
        {
            g_Setting.gpsLonBK.u8ddd = g_SysInfo.tGPS_Data.gpsLon.u8ddd;
            g_Setting.gpsLonBK.u8mm = g_SysInfo.tGPS_Data.gpsLon.u8mm;
            g_Setting.gpsLonBK.u16mmmm = g_SysInfo.tGPS_Data.gpsLon.u16mmmm;
            g_Setting.gpsLonBK.u8ss = g_SysInfo.tGPS_Data.gpsLon.u8ss;
            g_Setting.gpsLonBK.cIndicator = g_SysInfo.tGPS_Data.gpsLon.cIndicator;

            g_Setting.gpsLatBK.u8dd = g_SysInfo.tGPS_Data.gpsLat.u8dd;
            g_Setting.gpsLatBK.u8mm = g_SysInfo.tGPS_Data.gpsLat.u8mm;
            g_Setting.gpsLatBK.u16mmmm = g_SysInfo.tGPS_Data.gpsLat.u16mmmm;
            g_Setting.gpsLatBK.u8ss = g_SysInfo.tGPS_Data.gpsLat.u8ss;
            g_Setting.gpsLatBK.cIndicator = g_SysInfo.tGPS_Data.gpsLat.cIndicator;
        }
    }

    return bRes;
}

//============================================================================//
// @brief  Parse a NMEA VTG message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_VTG_Parse(char *cMsg)
{
    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return FALSE;

    return TRUE;
}

//============================================================================//
// @brief  Parse a NMEA ZDA message.
// @param  cMsg: Pointer to the message buffer.
// @retval TRUE: Operation success.
//         FALSE: Operation failed.
//============================================================================//
bool GPS_NMEA_ZDA_Parse(char *cMsg)
{
    bool bRes;
    GPS_DataTypeDef gpsTempHandle;
    char *pcPtr;
    char cContent[300] = {0};
    char cTemp[10];

    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return FALSE;

    if (strlen(cMsg) > sizeof(cContent))
        return FALSE;

    memcpy(&gpsTempHandle, &(g_SysInfo.tGPS_Data), sizeof(GPS_DataTypeDef));

    strcpy(cContent, cMsg);

    pcPtr = (char *)cContent;

    StopChar(pcPtr, ',');

    // Get time information
    if (strlen(pcPtr) >= 6)
    {
        // UTC hour
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 2);
        gpsTempHandle.utcTime.u8Hour = atoi(cTemp);

        // UTC minute
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 2, 2);
        gpsTempHandle.utcTime.u8Min = atoi(cTemp);

        // UTC second
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr + 4, 2);
        gpsTempHandle.utcTime.u8Sec = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // UTC day
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.utcTime.u8Day = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // UTC month
    if (strlen(pcPtr) > 0)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, strlen(pcPtr));
        gpsTempHandle.utcTime.u8Month = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    pcPtr += strlen(pcPtr) + 1;

    StopChar(pcPtr, ',');

    // UTC year
    if (strlen(pcPtr) == 4)
    {
        memset(cTemp, 0x00, sizeof(cTemp));
        strncpy(cTemp, pcPtr, 4);
        gpsTempHandle.utcTime.u16Year = atoi(cTemp);

        bRes = TRUE;
    }
    else
        bRes = FALSE;

    if (bRes)
        memcpy(&(g_SysInfo.tGPS_Data), &gpsTempHandle, sizeof(GPS_DataTypeDef));

    return TRUE;
}

//============================================================================//
// @brief  Parse maximum 1 NMEA message in the GPS RX buffer.
// @param  None.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_NMEA_Parse(void)
{
#if (ENABLE_GSM_NET && ENABLE_GPS_GNSS) || GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    bool bRes = FALSE;
    char cMessage[300] = {0};
    static uint32_t u32LostSignalTick = 0;
#endif
#if ENABLE_GSM_NET && ENABLE_GPS_GNSS && GNSS_ENABLE_GET_LOC
    static uint32_t u32GetLocTick = 0;
#elif GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    const char cMsgStart[] = "$G";
    const char cMsgEnd[] = "\r\n";
    uint16_t u16StrLen, u16StrPos, u16StrOrgPos;
#endif

#if ENABLE_GSM_NET && ENABLE_GPS_GNSS && GNSS_ENABLE_GET_LOC
    // Get and parse positioning information every 1000mS
    if (HAL_GetTick() >= (u32GetLocTick + S_2_mS(1)))
    {
        u32GetLocTick = HAL_GetTick();

        bGPS_GetInt = TRUE;
        bGPS_BaudCorrect = TRUE;

        if ((EXTIO_IsMainPowerNormal() == TRUE) &&
            (bQtEC21_State == TRUE) && (bQtEC21_InitFinish == TRUE) &&
            (bGSM_ResetFlag == FALSE) && (bGSM_ResetCheckFTP_Flag == FALSE) &&
            (procXTRA_Update_Step == XTRA_UPDATE_STEP_NULL))
        {
            memset(cMessage, 0x00, sizeof(cMessage));

            bRes = GPS_GetLoc(cMessage);

            if (bRes)
            {
                GPSprintf("\r\n> %s", cMessage);

                bRes = GPS_GNSS_ParseLoc(cMessage);
            }
        }
    }
#elif GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    // If find NMEA message header
    if (UART_RecvFind(GPS_COMPORT_DEF, (char *)cMsgStart, &u16StrPos))
    {
        uint8_t u8Cnt = 0;

        u16StrOrgPos = u16StrPos;

        // GPSprintf("\r\n> Find NMEA message start sequence at position %d", u16StrOrgPos);

        while (u8Cnt < 50)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, (char *)cMsgEnd, &u16StrPos, u16StrOrgPos);

            HAL_Delay(1);

            if (bRes)
                break;

            u8Cnt++;
        }

        // If find terminate sequence
        if (bRes)
        {
            u8GPS_RetryCnt = 0;

            // GPSprintf("\r\n> Find NMEA message end sequence at position %d", u16StrPos);

            // Calculate the message length
            u16StrLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16StrPos + 2 - u16StrOrgPos) %
                        rbBuf[GPS_COMPORT_DEF].u16Size;

            if (u16StrLen >= sizeof(cMessage))
            {
                GPSprintf("\r\n> Incorrect NMEA length %d", u16StrLen);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

                return FALSE;
            }

            // GPSprintf("\r\n> NMEA message length:%d", u16StrLen);

            // GPSprintf("\r\n> GPS buffer Head position:%d", rbBuf[GPS_COMPORT_DEF].u16Head);

            // GPSprintf("\r\n> GPS buffer Tail position:%d", rbBuf[GPS_COMPORT_DEF].u16Tail);

            // Read the message to the buffer
            // while(rbBuf[GPS_COMPORT_DEF].u16Tail != u16StrOrgPos)
            //	UART_GetRecvData(GPS_COMPORT_DEF, (uint8_t *)cMessage, 1);

            // UART_GetRecvData(GPS_COMPORT_DEF, (uint8_t *)cMessage, u16StrLen);
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            // UART_GetRecvDataPos(GPS_COMPORT_DEF, u16StrOrgPos, (uint8_t *)cMessage, u16StrLen);

            cMessage[u16StrLen] = 0;

            // GPSprintf("\r\n> %s", cMessage);

            u16StrPos = 0;

            // GPSprintf("\r\n> NMEA message:\n%s\nNMEA message end.", cMessage);

            // Check if any message is mixed into the buffer
            for (int i = 0; i < u16StrLen; i++)
                if (cMessage[i] == '$')
                    u16StrPos = i;

            // GPSprintf("\r\n> NMEA message:\n%s\nNMEA message end.", cMessage);
            GPSprintf("\r\n> %s", cMessage);

            // Validate checksum
            if (!GPS_NMEA_ChkSum(&cMessage[u16StrPos]))
            {
                GPSprintf("\r\n> Checksum not matched.");

                bRes = FALSE;

                return bRes;
            }

            // g_SysInfo.tGPS_Data.utcTime.u16Year = 0;

            // Check the NMEA message type and parse correspondingly
            if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_GGA, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_GGA_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_GLL, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_GLL_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_GSA, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_GSA_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_GSV, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_GSV_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_RMC, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_RMC_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_VTG, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_VTG_Parse(&cMessage[u16StrPos + 7]);
            }
            else if (strncmp(&cMessage[u16StrPos + 3], (char *)NMEA_ZDA, 3) == 0)
            {
                bGPS_BaudCorrect = TRUE;

                bRes = GPS_NMEA_ZDA_Parse(&cMessage[u16StrPos + 7]);
            }
            else
            {
                bRes = FALSE;
            }

            // if(bRes == FALSE)
            //   GPSprintf("\r\n> GPS parse NMEA message fail!");

            /*
            if(bRes)
            {
                GPSprintf("\r\n> GPS Time:%02d-%02d-%04d %02d:%02d:%02d", g_SysInfo.tGPS_Data.utcTime.u8Day, g_SysInfo.tGPS_Data.utcTime.u8Month,
                                                                          g_SysInfo.tGPS_Data.utcTime.u16Year, g_SysInfo.tGPS_Data.utcTime.u8Hour,
                                                                          g_SysInfo.tGPS_Data.utcTime.u8Min, g_SysInfo.tGPS_Data.utcTime.u8Sec);

                char cLatInd[2], cLonInd[2];

                snprintf(cLatInd, 2, "%s", &(g_SysInfo.tGPS_Data.gpsLat.cIndicator));

                snprintf(cLonInd, 2, "%s", &(g_SysInfo.tGPS_Data.gpsLon.cIndicator));

                GPSprintf("\r\n> GPS coordinate:%02d%02d.%04d%s,%02d%02d.%04d%s", g_SysInfo.tGPS_Data.gpsLat.u8dd, g_SysInfo.tGPS_Data.gpsLat.u8mm,
                                                                                  g_SysInfo.tGPS_Data.gpsLat.u16mmmm, cLatInd,
                                                                                                                                g_SysInfo.tGPS_Data.gpsLon.u8ddd, g_SysInfo.tGPS_Data.gpsLon.u8mm,
                                                                                  g_SysInfo.tGPS_Data.gpsLon.u16mmmm, cLonInd);

                GPSprintf("\r\n> GPS speed (m/h):%d", g_SysInfo.tGPS_Data.u32Speed);

                GPSprintf("\r\n> GPS course (degree):%d", g_SysInfo.tGPS_Data.u16Course);

                GPSprintf("\r\n> GPS data fix indicator:%d", g_SysInfo.tGPS_Data.bStatus);

                GPSprintf("\r\n> GPS number of satellites used:%d", g_SysInfo.tGPS_Data.u8StlNum);

                GPSprintf("\r\n> GPS PDOP:%.1f", g_SysInfo.tGPS_Data.fPDOP);

                GPSprintf("\r\n> GPS HDOP:%.1f", g_SysInfo.tGPS_Data.fHDOP);

                GPSprintf("\r\n> GPS VDOP:%.1f", g_SysInfo.tGPS_Data.fVDOP);

                GPSprintf("\r\n> GPS Altitude (m):%.1f", g_SysInfo.tGPS_Data.fAltitude);
            }
            */
        }
        else // If not find message terminate sequence
        {
            bRes = FALSE;

            u8GPS_RetryCnt++;

            if (u8GPS_RetryCnt >= 5)
            {
                u8GPS_RetryCnt = 0;

                u16StrLen = UART_RecvDataLen(GPS_COMPORT_DEF);

                UART_GetRecvData(GPS_COMPORT_DEF, (uint8_t *)cGPS_Content, u16StrLen);

                cGPS_Content[u16StrLen] = 0;

                // GPSprintf("\r\n> %s", cContent);
            }
        }
    }
    else // If not find message header
    {
        bRes = FALSE;

        u16StrLen = UART_RecvDataLen(GPS_COMPORT_DEF);

        if (u16StrLen > (rbBuf[GPS_COMPORT_DEF].u16Size * 3 / 4))
        {
            UART_GetRecvData(GPS_COMPORT_DEF, (uint8_t *)cGPS_Content, u16StrLen);

            cGPS_Content[u16StrLen] = 0;

            // GPSprintf("\nGPS data:\n%s\nGPS data end.", cContent);
        }
        else
        {
            // UART_PeekRecvData(GPS_COMPORT_DEF, (rbBuf[GPS_COMPORT_DEF].u16Tail), (uint8_t *)cContent, u16StrLen);

            // cContent[u16StrLen] = 0;

            // GPSprintf("\nGPS data:\n%s\nGPS data end.", cContent);
        }
    }
#endif

#if (ENABLE_GSM_NET && ENABLE_GPS_GNSS) || GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    if ((bRes == TRUE) && (g_SysInfo.tGPS_Data.bStatus == TRUE))
    {
        u32LostSignalTick = HAL_GetTick();

        // GPS �y�Ю榡, �n���ഫ
        g_SysInfo.tGPS_Data.gpsLocation.dLatitude = GPS_LatitudeDMMtoDD(&g_SysInfo.tGPS_Data.gpsLat);

        // GPS �y�Ю榡, �g���ഫ
        g_SysInfo.tGPS_Data.gpsLocation.dLongitude = GPS_LongitudeDMMtoDD(&g_SysInfo.tGPS_Data.gpsLon);

        if (bGPS_DataFixed == FALSE)
        {
            // If UTC time is normal
            if ((g_SysInfo.tGPS_Data.utcTime.u16Year > 2019) && (g_SysInfo.tGPS_Data.utcTime.u16Year < 2050))
            {
                struct tm tmRTCTimeInfo = {0}, tmLocalTimeInfo = {0};
                RTC_DateTypeDef rtcDate;
                RTC_TimeTypeDef rtcTime;
                time_t ttTime;
                HAL_StatusTypeDef halRes;
                osStatus osRes;

                /*
                rtcDate.Date = g_SysInfo.tGPS_Data.utcTime.u8Day;
                rtcDate.Month = g_SysInfo.tGPS_Data.utcTime.u8Month;
                rtcDate.Year = (uint8_t)(g_SysInfo.tGPS_Data.utcTime.u16Year % 100);
                rtcTime.Hours = g_SysInfo.tGPS_Data.utcTime.u8Hour;
                rtcTime.Minutes = g_SysInfo.tGPS_Data.utcTime.u8Min;
                rtcTime.Seconds = g_SysInfo.tGPS_Data.utcTime.u8Sec;
                */

                // Year in tm struct is counted from 1900
                tmRTCTimeInfo.tm_year = (uint8_t)(g_SysInfo.tGPS_Data.utcTime.u16Year % 100) + 2000 - 1900;
                // Month in tm struct is counted from 0 corresponding to Jan
                tmRTCTimeInfo.tm_mon = g_SysInfo.tGPS_Data.utcTime.u8Month - 1;
                tmRTCTimeInfo.tm_mday = g_SysInfo.tGPS_Data.utcTime.u8Day;
                tmRTCTimeInfo.tm_hour = g_SysInfo.tGPS_Data.utcTime.u8Hour;
                tmRTCTimeInfo.tm_min = g_SysInfo.tGPS_Data.utcTime.u8Min;
                tmRTCTimeInfo.tm_sec = g_SysInfo.tGPS_Data.utcTime.u8Sec;
                tmRTCTimeInfo.tm_isdst = -1;

                ttTime = mktime(&tmRTCTimeInfo);

                ttTime += g_Setting.i32TimeZoneOffset;

                localtime_r(&ttTime, &tmLocalTimeInfo);

                rtcDate.Year = tmLocalTimeInfo.tm_year + 1900 - 2000;
                rtcDate.Month = tmLocalTimeInfo.tm_mon + 1;
                rtcDate.Date = tmLocalTimeInfo.tm_mday;
                rtcDate.WeekDay = tmLocalTimeInfo.tm_wday;
                rtcTime.Hours = tmLocalTimeInfo.tm_hour;
                rtcTime.Minutes = tmLocalTimeInfo.tm_min;
                rtcTime.Seconds = tmLocalTimeInfo.tm_sec;
                rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;

                if (bMCU_RTC_Updated == FALSE)
                {
                    osRes = osMutexWait(mutex_ttnow_user, S_2_mS(10));

                    if (osRes != osOK)
                        return FALSE;

                    // halRes = HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

                    // GPSprintf("\r\n> Set time result: %d", halRes);

                    // if(halRes == HAL_OK)
                    //{
                    // halRes = HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                    // GPSprintf("\r\n> Set date result: %d", halRes);
                    //}

                    halRes = HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                    // GPSprintf("\r\n> Set date result: %d", halRes);

                    if (halRes == HAL_OK)
                    {
                        halRes = HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

                        // GPSprintf("\r\n> Set time result: %d", halRes);
                    }

                    osMutexRelease(mutex_ttnow_user);

                    // HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
                    // HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

                    // GPSprintf("\r\n> Set time:%s, %02d/%02d/%02d, %02d:%02d:%02d",
                    //            cWeekDay[rtcDate.WeekDay], rtcDate.Year, rtcDate.Month, rtcDate.Date,
                    //            rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);

                    if (halRes == HAL_OK)
                    {
                        bMCU_RTC_Updated = TRUE;

                        RTOSprintf("\r\n> ### MCU RTC TIME VALID ###");

#if ENABLE_MG82_RTC
                        if (bMG82_RTC_EN == TRUE)
                            bMG82_RTC_EN = FALSE;
#elif ENABLE_RX8130
                        if (bRX8130_EN == TRUE)
                            bRX8130_EN = FALSE;
#else
                        if (g_bPCF_EN == TRUE)
                            g_bPCF_EN = FALSE;
#endif
                    }

#if ENABLE_MG82_RTC
                    MG82_RTC_SetDateTime(&rtcDate, &rtcTime, 1);
#elif ENABLE_RX8130
                    RX8130_SetDateTime(&rtcDate, &rtcTime, 1);
#else
                    PCF_SetDate(&rtcDate, 1);
                    PCF_SetTime(&rtcTime, 1);
#endif

                    HAL_Delay(10);

                    if (bSystemTimeReady == FALSE)
                        bSystemTimeReady = TRUE;
                }

                // HW_WriteBackupRegister(BKP_DFU_CHK, BKP_RST_VALUE);
                // HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_RST_VALUE);

                bGPS_DataFixed = TRUE;

                RTOSprintf("\r\n> ### GPS DATA VALID ###");

                // GSM_TCP_ReportEvent((char *)EVENT_GPS_OK, STR_NULL);

                bFirstGPSFixFlag = TRUE;
            }
        }
    }
    else if ((bRes == TRUE) && (g_SysInfo.tGPS_Data.bStatus == FALSE))
    {
        if (bGPS_FirstDetection == FALSE)
        {
            bGPS_FirstDetection = TRUE;
        }

        if (bGPS_DataFixed == TRUE)
        {
            if (HAL_GetTick() > (u32LostSignalTick + S_2_mS(180)))
            {
                bGPS_DataFixed = FALSE;

                RTOSprintf("\r\n> ### GPS LOST SIGNAL!!! ###");

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_GPS_NOT_VALID, STR_NULL);
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
// GPS_EPO_Check();
#endif
            }
        }
    }

    return bRes;
#else
    return TRUE;
#endif
}

//============================================================================//
// @brief  Process GPS task.
// @param  None.
// @retval None.
//============================================================================//
void GPS_RX_Proc(void)
{
#if !(ENABLE_GPS_GNSS && GNSS_ENABLE_GET_LOC)
    uint16_t u16DatLen;
#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
    uint16_t u16Pos;
    char cContent[1024];
    bool bRes;
#endif
#endif

#if ENABLE_GPS_GNSS && GNSS_ENABLE_GET_LOC
    // if((EXTIO_IsMainPowerNormal() == TRUE) &&
    //	 (bQtEC21_State == TRUE) && (bQtEC21_InitFinish == TRUE) &&
    //	 (bGSM_ResetFlag == FALSE) && (bGSM_ResetCheckFTP_Flag == FALSE))
    {
        GPS_NMEA_Parse();
    }
#else
    u16DatLen = UART_RecvDataLen(GPS_COMPORT_DEF);

    taskInfo[TASK_GPS_PROC].u16TaskStep++;

    if (u16DatLen)
    {
        bGPS_SystembootTestFlag = TRUE;

        // GPSprintf("\r\n> nReceive %d bytes from GPS port.", u16DatLen);

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
        bRes = UART_RecvFind(GPS_COMPORT_DEF, (char *)"mdm9607-perf login:", &u16Pos);
#endif

        taskInfo[TASK_GPS_PROC].u16TaskStep++;

#if ENABLE_GPS_EPO
        if (tEPO_Step == EPO_STEP_NULL)
#endif
        {
            GPS_NMEA_Parse();
        }

        taskInfo[TASK_GPS_PROC].u16TaskStep++;

#if GPS_HARDWARE_TYPE == GPS_MODULE_QTEC21
        if (bRes)
        {
            GPSprintf("\nGPS module is ready.");

            bGPS_GetDataEn = TRUE;

            u16DatLen = UART_RecvDataLen(GPS_COMPORT_DEF);

            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16Pos, (uint8_t *)cContent, u16DatLen);

            cContent[u16DatLen] = 0;

            GPSprintf("\nGPS data:\n%s\nGPS data end.", cContent);

            GPS_EnableSendMsg();
        }

        taskInfo[TASK_GPS_PROC].u16TaskStep++;
#endif
    }

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    if (tGPS_CheckStep != GPS_CHECK_STEP_NULL)
    {
        GPS_CheckTypeProc();
    }

    taskInfo[TASK_GPS_PROC].u16TaskStep++;

#if ENABLE_GPS_EPO
    GPS_EPO_UpdateInit();

    taskInfo[TASK_GPS_PROC].u16TaskStep++;

    if (tEPO_SIM65_Step != EPO_SIM65_STEP_NULL)
    {
        bGPS_GetInt = TRUE;

        bGPS_BaudCorrect = TRUE;

        GPS_EPO_SIM65_UpdateProc();
    }

    taskInfo[TASK_GPS_PROC].u16TaskStep++;

    if (tEPO_Step != EPO_STEP_NULL)
    {
        bGPS_GetInt = TRUE;

        bGPS_BaudCorrect = TRUE;

        GPS_EPO_UpdateProc();
    }

    taskInfo[TASK_GPS_PROC].u16TaskStep++;
#endif
#endif

    if (bWaitingEnterStandby == TRUE)
    {
        bGPS_GetInt = TRUE;
    }

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    if ((HAL_GetTick() >= (u32GPS_IntTick + S_2_mS(GPS_INT_TIMEOUT))) &&
        (bGPS_CheckIntStop == FALSE))
#else
    if ((bGPS_Ready == TRUE) &&
        (HAL_GetTick() >= (u32GPS_IntTick + S_2_mS(GPS_INT_TIMEOUT))) &&
        (bGPS_CheckIntStop == FALSE))
#endif
    {
        if (bGPS_GetInt == TRUE)
        {
            // uint8_t u8Date[7] = {0};

            // SecToBCDTime(g_SysInfo.ttNow, u8Date);

            // RTOSprintf("\r\n> ### GPS check int OK, %02X%02X/%02X/%02X %02X:%02X:%02X ###",
            //					 u8Date[0], u8Date[1], u8Date[2],
            //					 u8Date[3], u8Date[4], u8Date[5],
            //					 u8Date[6]);

            bGPS_GetInt = FALSE;

            u8GPS_ResetCnt = 0;

            if ((bGPS_BaudCorrect == FALSE) && (bWaitingEnterStandby == FALSE))
            {
                if ((HAL_GetTick() >= (u32GPS_ScanTime + S_2_mS(60))) && (u32GPS_ScanTime > 0))
                {
                    RTOSprintf("\r\n> GPS not get correct data, change Baudrate.");

                    bGPS_ChangeFlag = TRUE;

                    GPS_Reset(1);
                }
            }
            else
            {
                u32GPS_ScanTime = 0;

                bGPS_ChangeFlag = FALSE;
            }
        }
        else // If GPS interrupt not fire
        {
            u8GPS_ResetCnt++;

            if (u8GPS_ResetCnt < GPS_RESET_NUM_MAX)
            {
                RTOSprintf("\r\n> GPS UART no data. Reset GPS module and GPS UART.");

                // If MCU still receives GPS NMEA messages
                if (__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE))
                {
                    GPS_Reset(1);

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_GPS_NO_NMEA, (char *)"1");
#endif
                }
                else // If MCU not receive any GPS NMEA messages
                {
                    GPS_Reset(1);

#if ENABLE_GSM_NET
                    GSM_TCP_ReportEvent((char *)EVENT_GPS_NO_NMEA, (char *)"2");
#endif
                }
            }
            else
            {
                u8GPS_ResetCnt = 0;

                bGPS_CheckIntStop = TRUE;

                RTOSprintf("\r\n> GPS is unable to recover.");

#if ENABLE_GSM_NET
                GSM_TCP_ReportEvent((char *)EVENT_GPS_MODULE_FAIL, STR_NULL);
#endif
            }
        }

        u32GPS_IntTick = HAL_GetTick();
    }
#endif
}

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
//============================================================================//
// @brief  Request to perform GPS check module type process.
// @param  None.
// @retval None.
//============================================================================//
void GPS_CheckType(void)
{
    tGPS_CheckStep = GPS_CHECK_STEP_INIT;
}

//============================================================================//
// @brief  Perform GPS check module type process.
// @param  None.
// @retval None.
//============================================================================//
void GPS_CheckTypeProc(void)
{
    static uint32_t u32GPS_WaitTick = 0;
    static bool bGPS_Res = FALSE;
    bool bRes = FALSE;
    static uint8_t u8RetryCnt = 0;

    if (tGPS_CheckStep == GPS_CHECK_STEP_NULL)
        return;

    switch (tGPS_CheckStep)
    {
    // GPS_CHECK_STEP_INIT: Init the GPS module type check process
    case GPS_CHECK_STEP_INIT:
    {
        if ((g_Setting.u32GPS_Baudrate != BR_9600bps) ||
            (g_Setting.u32GPS_Baudrate != BR_115200bps))
        {
            g_Setting.u32GPS_Baudrate = BR_9600bps;
        }

        UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);

        RTOSprintf("\r\n> GPS CHECK begin BR: %d", g_Setting.u32GPS_Baudrate);

        tGPS_CheckStep = GPS_CHECK_STEP_SIM65;

        u8RetryCnt = 0;

        break;
    }

    // GPS_CHECK_STEP_SIM65: Send SIM65 type query command
    case GPS_CHECK_STEP_SIM65:
    {
        uint8_t u8ChkSum;
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PAIR123");

        u8Len = strlen(pcPtr);

        u8ChkSum = CalCheckSum((uint8_t *)pcPtr + 1, u8Len - 1);

        pcPtr += u8Len;

        sprintf(pcPtr, "*%02X\r\n", u8ChkSum);

        u8Len += strlen(pcPtr);

        RTOSprintf("\r\n> GPS CHECK send SIM65 check type command: %s", (char *)&tEPO_SendBuf);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        u32GPS_WaitTick = HAL_GetTick();

        tGPS_CheckStep = GPS_CHECK_STEP_SIM65_WAIT;

        break;
    }

    // GPS_CHECK_STEP_SIM65_WAIT: Wait for response from GPS module
    case GPS_CHECK_STEP_SIM65_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[] = "$PAIR123,", cPreambleSeq2[] = "$PAIR001,",
             cEndWordSeq[] = "\r\n";
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32GPS_WaitTick + S_2_mS(2)))
        {
            RTOSprintf("\r\n> GPS CHECK wait SIM65 response timeout.");

            if (u8RetryCnt == 0)
            {
                u8RetryCnt++;

                if (g_Setting.u32GPS_Baudrate == BR_9600bps)
                    g_Setting.u32GPS_Baudrate = BR_115200bps;
                else
                    g_Setting.u32GPS_Baudrate = BR_9600bps;

                System_SaveSetting();

                RTOSprintf("\r\n> GPS change baudrate to %d", g_Setting.u32GPS_Baudrate);

                UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);

                /*
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                if(__HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE) == 0)
                    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_ORE))
                {
                    __HAL_UART_CLEAR_OREFLAG(puartHandle[GPS_COMPORT_DEF]);
                }

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE))
                {
                    __HAL_UART_CLEAR_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE);
                }
                */

                tGPS_CheckStep = GPS_CHECK_STEP_SIM65;
            }
            else
            {
                u8RetryCnt = 0;

                if (g_Setting.u32GPS_Baudrate == BR_9600bps)
                    g_Setting.u32GPS_Baudrate = BR_115200bps;
                else
                    g_Setting.u32GPS_Baudrate = BR_9600bps;

                System_SaveSetting();

                RTOSprintf("\r\n> GPS change baudrate to %d", g_Setting.u32GPS_Baudrate);

                UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);

                /*
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                if(__HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE) == 0)
                    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_ORE))
                {
                    __HAL_UART_CLEAR_OREFLAG(puartHandle[GPS_COMPORT_DEF]);
                }

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE))
                {
                    __HAL_UART_CLEAR_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE);
                }
                */

                tGPS_CheckStep = GPS_CHECK_STEP_SIM68;
            }

            break;
        }

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tGPS_CheckStep = GPS_CHECK_STEP_SIM65_WAIT;

            break;
        }

        tGPS_ModuleType = GPS_MODULE_TYPE_SIM65;

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 40);

            RTOSprintf("\r\n> GPS CHECK SIM65 response incomplete: %s", (char *)&tRespBuf);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bGPS_Res = TRUE;

            tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen > sizeof(tRespBuf))
        {
            u16RespLen = sizeof(tRespBuf);
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        RTOSprintf("\r\n> GPS CHECK SIM65 response: %s", &tRespBuf);

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq2, &u16FindPos1);

        if (bRes == FALSE)
        {
            RTOSprintf("\r\n> GPS CHECK SIM65 no 1st response.");

            bGPS_Res = TRUE;

            tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 40);

            RTOSprintf("\r\n> GPS CHECK SIM65 response incomplete: %s", (char *)&tRespBuf);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bGPS_Res = TRUE;

            tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen > sizeof(tRespBuf))
        {
            u16RespLen = sizeof(tRespBuf);
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        RTOSprintf("\r\n> GPS CHECK SIM65 response: %s", &tRespBuf);

        bGPS_Res = TRUE;

        tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

        break;
    }

    // GPS_CHECK_STEP_SIM68: Send SIM68 type query command
    case GPS_CHECK_STEP_SIM68:
    {
        uint8_t u8ChkSum;
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PMTK605");

        u8Len = strlen(pcPtr);

        u8ChkSum = CalCheckSum((uint8_t *)pcPtr + 1, u8Len - 1);

        pcPtr += u8Len;

        sprintf(pcPtr, "*%02X\r\n", u8ChkSum);

        u8Len += strlen(pcPtr);

        RTOSprintf("\r\n> GPS CHECK send SIM68 check type command: %s", (char *)&tEPO_SendBuf);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        u32GPS_WaitTick = HAL_GetTick();

        tGPS_CheckStep = GPS_CHECK_STEP_SIM68_WAIT;

        break;
    }

    // GPS_CHECK_STEP_SIM68_WAIT: Wait for response from GPS module
    case GPS_CHECK_STEP_SIM68_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[] = "$PMTK705,",
             cEndWordSeq[] = "\r\n";
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32GPS_WaitTick + S_2_mS(2)))
        {
            RTOSprintf("\r\n> GPS CHECK wait SIM68 response timeout.");

            bGPS_Res = FALSE;

            if (u8RetryCnt == 0)
            {
                u8RetryCnt++;

                if (g_Setting.u32GPS_Baudrate == BR_9600bps)
                    g_Setting.u32GPS_Baudrate = BR_115200bps;
                else
                    g_Setting.u32GPS_Baudrate = BR_9600bps;

                System_SaveSetting();

                RTOSprintf("\r\n> GPS change baudrate to %d", g_Setting.u32GPS_Baudrate);

                UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);

                /*
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_DISABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);
                __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_ERR);

                if(__HAL_UART_GET_IT_SOURCE(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE) == 0)
                    __HAL_UART_ENABLE_IT(puartHandle[GPS_COMPORT_DEF], UART_IT_RXNE);

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_ORE))
                {
                    __HAL_UART_CLEAR_OREFLAG(puartHandle[GPS_COMPORT_DEF]);
                }

                if(__HAL_UART_GET_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE))
                {
                    __HAL_UART_CLEAR_FLAG(puartHandle[GPS_COMPORT_DEF], UART_FLAG_RXNE);
                }
                */

                tGPS_CheckStep = GPS_CHECK_STEP_SIM68;
            }
            else
            {
                u8RetryCnt = 0;

                tGPS_CheckStep = GPS_CHECK_STEP_FINISH;
            }

            break;
        }

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tGPS_CheckStep = GPS_CHECK_STEP_SIM68_WAIT;

            break;
        }

        tGPS_ModuleType = GPS_MODULE_TYPE_SIM68;

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 40);

            RTOSprintf("\r\n> GPS CHECK SIM68 response incomplete: %s", (char *)&tRespBuf);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bGPS_Res = TRUE;

            tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen > sizeof(tRespBuf))
        {
            u16RespLen = sizeof(tRespBuf);
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        RTOSprintf("\r\n> GPS CHECK SIM68 response: %s", &tRespBuf);

        bGPS_Res = TRUE;

        tGPS_CheckStep = GPS_CHECK_STEP_FINISH;

        break;
    }

    // GPS_CHECK_STEP_FINISH: Finish the GPS module type check process
    case GPS_CHECK_STEP_FINISH:
    {
        if (bGPS_Res == FALSE)
        {
            tGPS_ModuleType = GPS_MODULE_TYPE_DEFAULT;

            if (GPS_MODULE_TYPE_DEFAULT == GPS_MODULE_TYPE_SIM65)
            {
                g_Setting.u32GPS_Baudrate = BR_115200bps;

                System_SaveSetting();

                RTOSprintf("\r\n> GPS change baudrate to %d", g_Setting.u32GPS_Baudrate);

                UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);
            }
            else
            {
                g_Setting.u32GPS_Baudrate = BR_9600bps;

                System_SaveSetting();

                RTOSprintf("\r\n> GPS change baudrate to %d", g_Setting.u32GPS_Baudrate);

                UART_ReInit(GPS_COMPORT_DEF, g_Setting.u32GPS_Baudrate);
            }
        }

        RTOSprintf("\r\n> GPS module type: %s", cGPS_ModuleTypeString[tGPS_ModuleType]);

        // Disable NMEA messages which are not GGA or RMC for module SIM65M
        if (tGPS_ModuleType == GPS_MODULE_TYPE_SIM65)
        {
            for (int i = 1; i < 7; i++)
            {
                if (i == 4)
                    continue;

                memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

                char *pcPtr = (char *)&tEPO_SendBuf;
                uint8_t u8Len, u8ChkSum;

                sprintf(pcPtr, "$PAIR062,%d,0", i);

                u8Len = strlen(pcPtr);

                u8ChkSum = CalCheckSum((uint8_t *)pcPtr + 1, u8Len - 1);

                pcPtr += u8Len;

                sprintf(pcPtr, "*%02X\r\n", u8ChkSum);

                u8Len += strlen(pcPtr);

                RTOSprintf("\r\n> GPS CHECK send SIM65 set output rate: %s", (char *)&tEPO_SendBuf);

                UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

                HAL_Delay(500);
            }
        }

#if ENABLE_GPS_EPO
        if (bGPS_Res && (bSystemStop == FALSE) && EXTIO_IsMainPowerNormal())
            GPS_EPO_Check();
#endif

        tGPS_CheckStep = GPS_CHECK_STEP_NULL;

        break;
    }

    default:
        break;
    }
}
#endif

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68 && ENABLE_GPS_EPO
//============================================================================//
// @brief  Check if the EPO file needs to be updated.
// @param  None.
// @retval None.
//============================================================================//
void GPS_EPO_Check(void)
{
    GSMprintf("\r\n> GPS EPO update check begin.");

    if (tGPS_ModuleType == GPS_MODULE_TYPE_SIM65)
        tEPO_SIM65_Step = EPO_SIM65_STEP_VERIFY;
    else
        tEPO_Step = EPO_STEP_VERIFY;
}

//============================================================================//
// @brief  Create an EPO download request.
// @param  None.
// @retval None.
//============================================================================//
void GPS_EPO_RequestDownload(void)
{
    HTTP_DownloadRequestTypeDef tRequest = {0};

    // If EPO URL is empty or error
    if ((strlen(g_Setting.cEPO_URL) == 0) ||
        (strlen(g_Setting.cEPO_URL) >= (sizeof(g_Setting.cEPO_URL) - 1)))
    {
        System_RestoreBackupSetting(VAR_EPO_URL);

        if ((strlen(g_Setting.cEPO_URL) == 0) ||
            (strlen(g_Setting.cEPO_URL) >= (sizeof(g_Setting.cEPO_URL) - 1)))
        {
            memset(g_Setting.cEPO_URL, 0x00, sizeof(g_Setting.cEPO_URL));

            sprintf(g_Setting.cEPO_URL, "%s", (char *)HTTP_EPO_GR_FILE_URL);

            System_SaveSetting();
        }
    }

    sprintf(tRequest.cSrcPath, "%s", g_Setting.cEPO_URL);
    sprintf(tRequest.cSrcFileName, "%s", (char *)GPS_EPO_FILE_NAME_RAM);
    sprintf(tRequest.cDstDir, "%s", (char *)GPS_EPO_DIR);
    sprintf(tRequest.cDstFileName, "%s%s", (char *)GPS_EPO_FILE_NAME, (char *)GPS_EPO_FILE_NAME_EXT);
    tRequest.callback = &GPS_EPO_RequestUpdate;
    tRequest.u8RetryCnt = 0;
    tRequest.bRequest = TRUE;

    GSM_HTTP_DownloadRequest(&tRequest);
}

//============================================================================//
// @brief  Create an EPO update request.
// @param  pcFileName: EPO file path.
//         u32Size: EPO file size.
// @retval None.
//============================================================================//
void GPS_EPO_RequestUpdate(char *pcFilePath, uint32_t u32Size)
{
    if ((tEPO_Download.bRequest == TRUE) || (tEPO_Step != EPO_STEP_NULL))
    {
        return;
    }

    if (strlen(pcFilePath) > sizeof(tEPO_Download.cFilePath))
    {
        GSMprintf("\r\n> EPO request file path ERROR: %s", strlen(pcFilePath));

        return;
    }

    if (u32Size > GPS_EPO_SIZE_MAX)
    {
        GSMprintf("\r\n> EPO request file size ERROR: %d", u32Size);

        return;
    }

    memcpy(tEPO_Download.cFilePath, pcFilePath, strlen(pcFilePath));
    tEPO_Download.u32FileSize = u32Size;
    tEPO_Download.bRequest = TRUE;

    GSMprintf("\r\n> Add GPS EPO update request OK");
}

//============================================================================//
// @brief  Convert GPS time since GPS epoch to calendar time since epoch.
// @param  u32GPS_Week: Number of GPS week.
//         u32GPS_Sec: Number of seconds in the GPS week.
// @retval Time in seconds since calendar epoch.
//============================================================================//
uint32_t GPS_Epoch_2_Calendar(uint32_t u32GPS_Week, uint32_t u32GPS_Sec)
{
    uint32_t u32SecSinceCalendarEpoch = 0;

    // GPS epoch starts on a Saturday, so add the number of days since Saturday
    uint32_t u32DaysSinceGPS_Epoch = u32GPS_Week * 7;
    uint32_t u32SecondsSinceGPS_Epoch = u32GPS_Sec + DAY_2_SECOND(u32DaysSinceGPS_Epoch);

    // Add the difference in seconds between GPS Epoch and Calendar Epoch
    u32SecSinceCalendarEpoch = u32SecondsSinceGPS_Epoch + 315964800U;

    // GSMprintf("\r\n> JAMES u32GPS_Week: %d, u32GPS_Sec %d, u32DaysSinceGPS_Epoch %d, u32SecondsSinceGPS_Epoch %d u32SecSinceCalendarEpoch %d", u32GPS_Week, u32GPS_Sec, u32DaysSinceGPS_Epoch, u32SecondsSinceGPS_Epoch, u32SecSinceCalendarEpoch);
    return u32SecSinceCalendarEpoch;
}

//============================================================================//
// @brief  Begin EPO update process.
// @param  None.
// @retval None.
//============================================================================//
void GPS_EPO_UpdateInit(void)
{
    if ((tEPO_Download.bRequest == TRUE) && (tEPO_Step == EPO_STEP_NULL) &&
        (tEPO_SIM65_Step == EPO_SIM65_STEP_NULL))
    {
        // tEPO_Download.bRequest = FALSE;

        if (tGPS_ModuleType == GPS_MODULE_TYPE_SIM65)
            tEPO_SIM65_Step = EPO_SIM65_STEP_INIT;
        else
            tEPO_Step = EPO_STEP_INIT;
    }
}

//============================================================================//
// @brief  Perform EPO update process for SIM65.
// @param  None.
// @retval None.
//============================================================================//
void GPS_EPO_SIM65_UpdateProc(void)
{
    static uint8_t u8EPO_Retry = 0;
    static char cFilePath[128] = {0};
    static uint32_t u32FileSize = 0, u32DatCnt = 0, u32DatSize = 0;
    static uint16_t u16EPO_SeqID = 0;
    static uint32_t u32EPO_WaitTick = 0;
    static uint8_t u8CurrEPO_Pack = 0;
    static bool bEPO_Res = FALSE;
    bool bRes = FALSE;
    static uint32_t u32StartTime = 0, u32EndTime = 0;

    if (tEPO_SIM65_Step == EPO_SIM65_STEP_NULL)
        return;

    switch (tEPO_SIM65_Step)
    {
    // EPO_SIM65_STEP_INIT: Init the EPO update process for SIM65
    case EPO_SIM65_STEP_INIT:
    {
        GSMprintf("\r\n> GPS EPO SIM65 Init");

        memset(cFilePath, 0x00, sizeof(cFilePath));

        sprintf(cFilePath, "%s", tEPO_Download.cFilePath);

        if (SD_CheckFileExist(cFilePath) == FALSE)
        {
            GSMprintf("\r\n> GPS EPO SIM65 request file does not exist. Terminate operation.");

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

            break;
        }

        u32FileSize = SD_GetFileSize(cFilePath);

        if (u32FileSize != tEPO_Download.u32FileSize)
        {
            GSMprintf("\r\n> GPS EPO SIM65 request file size incorrect: %d-%d. Terminate operation.", u32FileSize, tEPO_Download.u32FileSize);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

            break;
        }

        if ((u32FileSize % GPS_EPO_SET_SIZE) != 0)
        {
            GSMprintf("\r\n> GPS EPO SIM65 request file size not a multiple of EPO set size: %d", u32FileSize);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

            break;
        }

        u8EPO_Retry = 0;

        tEPO_SIM65_Step = EPO_SIM65_STEP_ERASE;

        break;
    }

    // EPO_SIM65_STEP_ERASE: Send $PAIR472 command to erase existing EPO data
    case EPO_SIM65_STEP_ERASE:
    {
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PAIR472*3B\r\n");

        u8Len = strlen(pcPtr);

        // GSMprintf("\r\n> GPS EPO SIM65 send erase command: %s", (char *)&tEPO_SendBuf);
        GSMprintf("\r\n> GPS EPO SIM65 send erase command.");

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        u32EPO_WaitTick = HAL_GetTick();

        tEPO_SIM65_Step = EPO_SIM65_STEP_ERASE_WAIT;

        break;
    }

    // EPO_SIM65_STEP_ERASE_WAIT: Wait for response from GPS module
    case EPO_SIM65_STEP_ERASE_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[] = "$PAIR001", cEndWordSeq[] = "\r\n";
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(GPS_EPO_RESP_TIMEOUT)))
        {
            GSMprintf("\r\n> GPS EPO SIM65 erase command wait response timeout.");

            // bEPO_Res = FALSE;

            // tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;
            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN;

            break;
        }

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_ERASE_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 20);

            GSMprintf("\r\n> GPS EPO SIM65 erase command response incomplete: %s", (char *)&tRespBuf);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen > sizeof(tRespBuf))
        {
            u16RespLen = sizeof(tRespBuf);
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO SIM65 erase command response: %s", &tRespBuf);

        u8EPO_Retry = 0;

        HAL_Delay(S_2_mS(1));

        tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN;

        break;
    }

    // EPO_SIM65_STEP_ERASE_RETRY: Check if retransmission is needed
    case EPO_SIM65_STEP_ERASE_RETRY:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO SIM65 erase command maximum retry reached.");

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN;

            break;
        }

        GSMprintf("\r\n> GPS EPO SIM65 erase command retry in the next loop.");

        tEPO_SIM65_Step = EPO_SIM65_STEP_ERASE;

        break;
    }

    // EPO_SIM65_STEP_SEND_BEGIN: Change UART mode back to PMTK mode
    case EPO_SIM65_STEP_SEND_BEGIN:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;
        EPO_SIM65_PktTypeDef tSendBuf = {0};

        // Preamble
        memcpy(tSendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Command ID
        tSendBuf.u8CmdID[0] = 0xB0;
        tSendBuf.u8CmdID[1] = 0x04;
        // Length
        tSendBuf.u8Length[0] = 0x01;
        tSendBuf.u8Length[1] = 0x00;
        // Data
        tSendBuf.u8Dat = cGPS_ServiceSign[tGPS_ServiceType];
        // Checksum
        u8Len = sizeof(tSendBuf.u8CmdID) + sizeof(tSendBuf.u8Length) +
                sizeof(tSendBuf.u8Dat);
        u8ChkSum = CalCheckSum(&(tSendBuf.u8CmdID[0]), u8Len);
        tSendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tSendBuf.u8EndWord, u8EPO_SIM65_EndWord, 2);

        GSMprintf("\r\n> GPS EPO SIM65 send begin packet.");

        // uint8_t *pu8Ptr = (uint8_t *)&tSendBuf;

        // for(int i = 0; i < sizeof(EPO_SIM65_PktTypeDef); i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tSendBuf, sizeof(EPO_SIM65_PktTypeDef));

        u32EPO_WaitTick = HAL_GetTick();

        tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN_WAIT;

        break;
    }

    // EPO_SIM65_STEP_SEND_BEGIN_WAIT: Wait for response from GPS module
    case EPO_SIM65_STEP_SEND_BEGIN_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_SIM65_RespPktTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(5)))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send begin packet wait response timeout.");

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_SIM65_EndWord, 2);

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_BEGIN_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 1000)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO SIM65 send begin packet response incomplete: ");

            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 20);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 13)
        {
            if (u16RespLen > 13)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO SIM65 send begin packet response length ERROR: %d, response: %s, \r\n", u16RespLen, &tRespBuf);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO SIM65 send begin packet response: ");

        // uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        // for(int i = 0; i < u16RespLen; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0xE8) || (tRespBuf.u8CmdID[1] != 0x03))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send begin packet response message ID ERROR: %02X%02X",
                      tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[0] != 0xB0) || (tRespBuf.u8Dat[1] != 0x04))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send begin packet response command ID ERROR: %02X%02X",
                      tRespBuf.u8Dat[0],
                      tRespBuf.u8Dat[1]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[2] != 0x00) || (tRespBuf.u8Dat[3] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send begin packet response code ERROR: %02X%02X",
                      tRespBuf.u8Dat[2],
                      tRespBuf.u8Dat[3]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        u8EPO_Retry = 0;

        tEPO_SIM65_Step = EPO_SIM65_STEP_BEGIN_TRANSFER;

        break;
    }

    // EPO_SIM65_STEP_BEGIN_TRANSFER: Begin the EPO file transfer process
    case EPO_SIM65_STEP_BEGIN_TRANSFER:
    {
        u32DatCnt = 0;

        if ((u32FileSize - u32DatCnt) >= (GPS_EPO_SAT_DATA_SIZE * GPS_EPO_PACK_LEN))
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * GPS_EPO_PACK_LEN;
        else
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * ((u32FileSize - u32DatCnt) / GPS_EPO_SAT_DATA_SIZE);

        memset(u8EPO_GetDatBuf, 0x00, sizeof(u8EPO_GetDatBuf));

        bRes = SD_ReadAddrToBuff(cFilePath, u8EPO_GetDatBuf[0], u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            u8EPO_Retry++;

            if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
            {
                u8EPO_Retry = 0;

                GSMprintf("\r\n> GPS EPO SIM65 read file at address %d ERROR. Maximum retry reached.", u32DatCnt);

                bEPO_Res = FALSE;

                tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

                break;
            }

            GSMprintf("\r\n> GPS EPO SIM65 read file at address %d ERROR. Retry in the next loop.", u32DatCnt);

            tEPO_SIM65_Step = EPO_SIM65_STEP_BEGIN_TRANSFER;

            break;
        }

        // u8EPO_Retry = 0;

        u8CurrEPO_Pack = 0;

        u16EPO_SeqID = 0;

        u32StartTime = HAL_GetTick();

        tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT;

        break;
    }

    // EPO_SIM65_STEP_SEND_PKT: Send EPO packet to GPS module
    case EPO_SIM65_STEP_SEND_PKT:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SIM65_SendBuf, 0x00, sizeof(EPO_SIM65_DataPktTypeDef));

        // Preamble
        memcpy(tEPO_SIM65_SendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Command ID
        memcpy(tEPO_SIM65_SendBuf.u8CmdID, u8EPO_SIM65_CmdID, 2);
        // Length
        tEPO_SIM65_SendBuf.u8Length[0] = GPS_EPO_SAT_DATA_SIZE;
        tEPO_SIM65_SendBuf.u8Length[1] = 0x00;
        // Data
        memcpy(tEPO_SIM65_SendBuf.u8Dat, u8EPO_GetDatBuf[u8CurrEPO_Pack], GPS_EPO_SAT_DATA_SIZE);
        // Checksum
        u8Len = sizeof(tEPO_SIM65_SendBuf.u8Length) + sizeof(tEPO_SIM65_SendBuf.u8CmdID) +
                sizeof(tEPO_SIM65_SendBuf.u8Dat);
        u8ChkSum = CalCheckSum(&(tEPO_SIM65_SendBuf.u8CmdID[0]), u8Len);
        tEPO_SIM65_SendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tEPO_SIM65_SendBuf.u8EndWord, u8EPO_SIM65_EndWord, 2);

        // GSMprintf("\r\n> GPS EPO SIM65 send packet %03d (buffer ID: %02d, ChkSum: %02X):", u16EPO_SeqID, u8CurrEPO_Pack, tEPO_SIM65_SendBuf.u8ChkSum);
        GSMprintf("\r\n> GPS EPO SIM65 send packet %03d.", u16EPO_SeqID);

        u16EPO_SeqID++;

        // uint8_t *pu8Ptr = (uint8_t *)&tEPO_SIM65_SendBuf;

        // for(int i = 0; i < 6; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SIM65_SendBuf, sizeof(EPO_SIM65_DataPktTypeDef));

        u8CurrEPO_Pack++;

        u32DatCnt += GPS_EPO_SAT_DATA_SIZE;

        // u8EPO_Retry = 0;

        if ((u8CurrEPO_Pack >= GPS_EPO_PACK_LEN) && (u32DatCnt < u32FileSize))
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_GET_DATA;
        }
        else
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_WAIT_RESP;
        }

        u32EPO_WaitTick = HAL_GetTick();

        break;
    }

    // EPO_SIM65_STEP_GET_DATA: Get the EPO SAT data
    case EPO_SIM65_STEP_GET_DATA:
    {
        if (u32FileSize - u32DatCnt >= (GPS_EPO_SAT_DATA_SIZE * GPS_EPO_PACK_LEN))
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * GPS_EPO_PACK_LEN;
        else
            // u32DatSize = GPS_EPO_SAT_DATA_SIZE * ((u32FileSize - u32DatCnt) / GPS_EPO_SAT_DATA_SIZE);
            u32DatSize = u32FileSize - u32DatCnt;

        memset(u8EPO_GetDatBuf, 0x00, sizeof(u8EPO_GetDatBuf));

        bRes = SD_ReadAddrToBuff(cFilePath, u8EPO_GetDatBuf[0], u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            u8EPO_Retry++;

            if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
            {
                u8EPO_Retry = 0;

                GSMprintf("\r\n> GPS EPO SIM65 read file at address %d ERROR. Maximum retry reached.", u32DatCnt);

                bEPO_Res = FALSE;

                tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

                break;
            }

            GSMprintf("\r\n> GPS EPO SIM65 read file at address %d ERROR. Retry in the next loop.", u32DatCnt);

            tEPO_SIM65_Step = EPO_SIM65_STEP_GET_DATA;

            break;
        }

        // u8EPO_Retry = 0;

        u8CurrEPO_Pack = 0;

        tEPO_SIM65_Step = EPO_SIM65_STEP_WAIT_RESP;

        break;
    }

    // EPO_SIM65_STEP_WAIT_RESP: Wait for response from GPS module
    case EPO_SIM65_STEP_WAIT_RESP:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_SIM65_RespPktTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GPS EPO SIM65 wait response timeout.");

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_SIM65_EndWord, 2);

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_WAIT_RESP;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response incomplete: ");

            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 20);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 13)
        {
            if (u16RespLen > 13)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO SIM65 send packet response length ERROR: %d, response: %s, \r\n", u16RespLen, &tRespBuf);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO SIM65 send packet response: ");

        // uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        // for(int i = 0; i < u16RespLen; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0xE8) || (tRespBuf.u8CmdID[1] != 0x03))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response message ID ERROR: %02X%02X",
                      tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[0] != 0xB1) || (tRespBuf.u8Dat[1] != 0x04))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response command ID ERROR: %02X%02X",
                      tRespBuf.u8Dat[0],
                      tRespBuf.u8Dat[1]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[2] != 0x00) || (tRespBuf.u8Dat[3] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response code ERROR: %02X%02X",
                      tRespBuf.u8Dat[2],
                      tRespBuf.u8Dat[3]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if (u32DatCnt < u32FileSize)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT;
        }
        else
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_FINAL;
        }

        break;
    }

    // EPO_SIM65_STEP_SEND_FINAL: Send ending packet to GPS module
    case EPO_SIM65_STEP_SEND_FINAL:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;
        EPO_SIM65_PktTypeDef tSendBuf = {0};

        memset(&tSendBuf, 0x00, sizeof(EPO_SIM65_PktTypeDef));

        // Preamble
        memcpy(tSendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Command ID
        tSendBuf.u8CmdID[0] = 0xB2;
        tSendBuf.u8CmdID[1] = 0x04;
        // Length
        tSendBuf.u8Length[0] = 0x01;
        tSendBuf.u8Length[1] = 0x00;
        // Data
        tSendBuf.u8Dat = cGPS_ServiceSign[tGPS_ServiceType];
        // Checksum
        u8Len = sizeof(tSendBuf.u8CmdID) + sizeof(tSendBuf.u8Length) +
                sizeof(tSendBuf.u8Dat);
        u8ChkSum = CalCheckSum(&(tSendBuf.u8CmdID[0]), u8Len);
        tSendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tSendBuf.u8EndWord, u8EPO_SIM65_EndWord, 2);

        GSMprintf("\r\n> GPS EPO SIM65 send final packet.");

        // uint8_t *pu8Ptr = (uint8_t *)&tSendBuf;

        // for(int i = 0; i < sizeof(EPO_SIM65_PktTypeDef); i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tSendBuf, sizeof(EPO_SIM65_PktTypeDef));

        // u8EPO_Retry = 0;

        tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_FINAL_WAIT;

        u32EPO_WaitTick = HAL_GetTick();

        break;
    }

    // EPO_SIM65_STEP_SEND_FINAL_WAIT: Wait for ending response from GPS module
    case EPO_SIM65_STEP_SEND_FINAL_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_SIM65_RespPktTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(3)))
        {
            GSMprintf("\r\n> GPS EPO SIM65 wait response timeout.");

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_SIM65_EndWord, 2);

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_FINAL_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response incomplete: ");

            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 20);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 13)
        {
            if (u16RespLen > 13)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO SIM65 send packet response length ERROR: %d, response: %s, \r\n", u16RespLen, &tRespBuf);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO SIM65 send final packet response: ");

        // uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        // for(int i = 0; i < u16RespLen; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0xE8) || (tRespBuf.u8CmdID[1] != 0x03))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response message ID ERROR: %02X%02X",
                      tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[0] != 0xB2) || (tRespBuf.u8Dat[1] != 0x04))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response command ID ERROR: %02X%02X",
                      tRespBuf.u8Dat[0],
                      tRespBuf.u8Dat[1]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        if ((tRespBuf.u8Dat[2] != 0x00) || (tRespBuf.u8Dat[3] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO SIM65 send packet response code ERROR: %02X%02X",
                      tRespBuf.u8Dat[2],
                      tRespBuf.u8Dat[3]);

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_SEND_PKT_RETRY;

            break;
        }

        u8EPO_Retry = 0;

        bEPO_Res = TRUE;

        u32EndTime = HAL_GetTick();

        tEPO_SIM65_Step = EPO_SIM65_STEP_VERIFY;

        break;
    }

    // EPO_SIM65_STEP_SEND_PKT_RETRY: Check if retransmission is needed
    case EPO_SIM65_STEP_SEND_PKT_RETRY:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO SIM65 transfer maximum retry reached.");

            bEPO_Res = FALSE;

            tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;

            break;
        }

        GSMprintf("\r\n> GPS EPO SIM65 transfer retry in the next loop.");

        tEPO_SIM65_Step = EPO_SIM65_STEP_ERASE;

        break;
    }

    // EPO_SIM65_STEP_VERIFY: Verify EPO update result
    case EPO_SIM65_STEP_VERIFY:
    {
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PAIR470,0*25\r\n");

        u8Len = strlen(pcPtr);

        RTOSprintf("\r\n> GPS EPO SIM65 send verification command: %s", (char *)&tEPO_SendBuf);

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        u32EPO_WaitTick = HAL_GetTick();

        tEPO_SIM65_Step = EPO_SIM65_STEP_VERIFY_WAIT;

        break;
    }

    // EPO_SIM65_STEP_VERIFY_WAIT: Wait for response from GPS module
    case EPO_SIM65_STEP_VERIFY_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        uint8_t u8RespBuf[128] = {0};
        EPO_InfoTypeDef tEPO_Info;
        char cPreambleSeq[] = "$PAIR470,", cEndWordSeq[] = "\r\n";
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(3)))
        {
            RTOSprintf("\r\n> GPS EPO SIM65 wait verification response timeout.");

            // tEPO_SIM65_Step = EPO_SIM65_STEP_RETRY_VERIFY;
            tEPO_SIM65_Step = EPO_SIM65_STEP_NULL;

            break;
        }

        bRes = UART_RecvFind(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1);

        if (bRes == FALSE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_VERIFY_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, sizeof(u8RespBuf) - 1);

            RTOSprintf("\r\n> GPS EPO SIM65 verification response incomplete: %s", (char *)u8RespBuf);
            // RTOSprintf("\r\n> GPS EPO SIM65 verification response incomplete.");

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            tEPO_SIM65_Step = EPO_SIM65_STEP_RETRY_VERIFY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen >= sizeof(u8RespBuf))
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, sizeof(u8RespBuf) - 1);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            RTOSprintf("\r\n> GPS EPO SIM65 verification response length ERROR: %d", u16RespLen);

            tEPO_SIM65_Step = EPO_SIM65_STEP_RETRY_VERIFY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, u16RespLen);

        RTOSprintf("\r\n> GPS EPO SIM65 verification response: %s", (char *)u8RespBuf);

        bEPO_Res = TRUE;

        uint8_t u8Num;
        uint32_t u32CurrTime = 0,
                 u32FirstTime, u32LastTime,
                 u32FirstCurrTime, u32LastCurrTime;
        // uint16_t u16EndYear = 0;
        struct tm tmTimeInfo = {0};

        u8Num = sscanf((char *)u8RespBuf, "%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d*",
                       tEPO_Info.cHeader,
                       (int *)&(tEPO_Info.tService),
                       &(tEPO_Info.u32SetNum),
                       &(tEPO_Info.u32FirstWN),
                       &(tEPO_Info.u32FirstTOW),
                       &(tEPO_Info.u32LastWN),
                       &(tEPO_Info.u32LastTOW),
                       &(tEPO_Info.u32FirstCurrWN),
                       &(tEPO_Info.u32FirstCurrTOW),
                       &(tEPO_Info.u32LastCurrWN),
                       &(tEPO_Info.u32LastCurrTOW));

        if (u8Num != 11)
        {
            RTOSprintf("\r\n> GPS EPO SIM65 Info parse error: %d", u8Num);

            tEPO_SIM65_Step = EPO_SIM65_STEP_RETRY_VERIFY;

            break;
        }

        RTOSprintf("\r\n> EPO GPS service: %c", cGPS_ServiceSign[tEPO_Info.tService]);

        u32CurrTime = System_GetTime() - HOUR_2_SECOND(8);
        u32FirstTime = GPS_Epoch_2_Calendar(tEPO_Info.u32FirstWN, tEPO_Info.u32FirstTOW);
        u32LastTime = GPS_Epoch_2_Calendar(tEPO_Info.u32LastWN, tEPO_Info.u32LastTOW);
        u32FirstCurrTime = GPS_Epoch_2_Calendar(tEPO_Info.u32FirstCurrWN, tEPO_Info.u32FirstCurrTOW);
        u32LastCurrTime = GPS_Epoch_2_Calendar(tEPO_Info.u32LastCurrWN, tEPO_Info.u32LastCurrTOW);

        localtime_r(&u32FirstTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current File Begin Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32LastTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current File End Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        // u16EndYear = tmTimeInfo.tm_year + 1900;

        localtime_r(&u32FirstCurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current Set Begin Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32LastCurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current Set End Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32CurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> System Current UTC Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        if (tEPO_Download.bRequest == TRUE)
        {
            tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;
        }
        else
        {
            if ((u32CurrTime < u32FirstTime) || ((u32CurrTime + HOUR_2_SECOND(16)) >= u32LastTime))
            {
                RTOSprintf("\r\n> GPS EPO SIM65 update required.");

                GPS_EPO_RequestDownload();
            }
            else
            {
                RTOSprintf("\r\n> GPS EPO SIM65 update check OK.");
            }

            tEPO_SIM65_Step = EPO_SIM65_STEP_NULL;
        }

        u8EPO_Retry = 0;

        break;
    }

    // EPO_SIM65_STEP_RETRY_VERIFY: Check if retry is needed
    case EPO_SIM65_STEP_RETRY_VERIFY:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX_2)
        {
            u8EPO_Retry = 0;

            RTOSprintf("\r\n> GPS EPO SIM65 verification maximum retry reached.");

            bEPO_Res = TRUE;

            if (tEPO_Download.bRequest == TRUE)
            {
                tEPO_SIM65_Step = EPO_SIM65_STEP_FINISH;
            }
            else
            {
                GPS_EPO_RequestDownload();

                tEPO_SIM65_Step = EPO_SIM65_STEP_NULL;
            }

            break;
        }

        RTOSprintf("\r\n> GPS EPO SIM65 verification retry in the next loop.");

        tEPO_SIM65_Step = EPO_SIM65_STEP_RETRY_VERIFY;

        break;
    }

    // EPO_SIM65_STEP_FINISH: Complete the EPO process
    case EPO_SIM65_STEP_FINISH:
    {
        tEPO_Download.bRequest = FALSE;

        if (bEPO_Res == FALSE)
        {
            GSMprintf("\r\n> GPS EPO SIM65 update FAIL. Terminate operation.");

            GSMprintf("\r\n> GPS reset.");

            bGPS_ChangeFlag = FALSE;

            GPS_Reset(1);
        }
        else
        {
            GSMprintf("\r\n> GPS EPO SIM65 update OK, time: %d", u32EndTime - u32StartTime);
        }

        tEPO_SIM65_Step = EPO_SIM65_STEP_NULL;

        break;
    }

    default:
        break;
    }
}

//============================================================================//
// @brief  Perform EPO update process.
// @param  None.
// @retval None.
//============================================================================//
void GPS_EPO_UpdateProc(void)
{
    static uint8_t u8EPO_Retry = 0;
    static char cFilePath[128] = {0};
    static uint32_t u32FileSize = 0, u32DatCnt = 0, u32DatSize = 0;
    static uint16_t u16EPO_SeqID = 0;
    static uint16_t u16StartPos = 0;
    static uint32_t u32EPO_WaitTick = 0;
    static bool bEPO_Res = FALSE;
    bool bRes = FALSE;
    static uint32_t u32StartTime = 0, u32EndTime = 0;

    if (tEPO_Step == EPO_STEP_NULL)
        return;

    switch (tEPO_Step)
    {
    // EPO_STEP_INIT: Init the EPO update process
    case EPO_STEP_INIT:
    {
        memset(cFilePath, 0x00, sizeof(cFilePath));

        sprintf(cFilePath, "%s", tEPO_Download.cFilePath);

        if (SD_CheckFileExist(cFilePath) == FALSE)
        {
            GSMprintf("\r\n> GPS EPO request file does not exist. Terminate operation.");

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_FINISH;

            break;
        }

        u32FileSize = SD_GetFileSize(cFilePath);

        if (u32FileSize != tEPO_Download.u32FileSize)
        {
            GSMprintf("\r\n> GPS EPO request file size incorrect: %d-%d. Terminate operation.", u32FileSize, tEPO_Download.u32FileSize);

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_FINISH;

            break;
        }

        if ((u32FileSize % GPS_EPO_SET_SIZE) != 0)
        {
            GSMprintf("\r\n> GPS EPO request file size not a multiple of EPO set size: %d", u32FileSize);

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_FINISH;

            break;
        }

        u8EPO_Retry = 0;

        tEPO_Step = EPO_STEP_CHANGE_UART_MODE;

        break;
    }

    // EPO_STEP_CHANGE_UART_MODE: Switch UART mode from NMEA mode to binary mode
    case EPO_STEP_CHANGE_UART_MODE:
    {
        uint8_t u8ChkSum;
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PMTK253,1,115200");
        // sprintf(pcPtr, "$PMTK253,1,9600");

        u8Len = strlen(pcPtr);

        u8ChkSum = CalCheckSum((uint8_t *)pcPtr + 1, u8Len - 1);

        pcPtr += u8Len;

        sprintf(pcPtr, "*%02X\r\n", u8ChkSum);

        u8Len += strlen(pcPtr);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        GSMprintf("\r\n> GPS EPO send UART change mode command: %s", (char *)&tEPO_SendBuf);

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        HAL_Delay(1);

        bRes = UART_ReInit(GPS_COMPORT_DEF, BR_115200bps);

        HAL_Delay(100);

        // u32EPO_WaitTick = HAL_GetTick();

        tEPO_Step = EPO_STEP_BEGIN_TRANSFER;
        // tEPO_Step = EPO_STEP_CHANGE_UART_MODE_WAIT;

        break;
    }

    // EPO_STEP_CHANGE_UART_MODE_WAIT: Wait for response from GPS module
    case EPO_STEP_CHANGE_UART_MODE_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(GPS_EPO_RESP_TIMEOUT)))
        {
            GSMprintf("\r\n> GPS EPO wait change UART response timeout.");

            // bEPO_Res = FALSE;

            // tEPO_Step = EPO_STEP_FINISH;
            tEPO_Step = EPO_STEP_BEGIN_TRANSFER;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_EndWord, 2);

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_CHANGE_UART_MODE_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO change UART response incomplete.");

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_UART_MODE;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 12)
        {
            if (u16RespLen > 12)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO change UART response length ERROR: %d, response: %s, \r\n", u16RespLen, &tRespBuf);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < u16RespLen; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            tEPO_Step = EPO_STEP_RETRY_CHANGE_UART_MODE;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        GSMprintf("\r\n> GPS EPO change UART response: %s, \r\n", &tRespBuf);

        uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        for (int i = 0; i < u16RespLen; i++)
        {
            GSMprintf(" %02X", *pu8Ptr);

            pu8Ptr++;
        }

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0x01) || (tRespBuf.u8CmdID[1] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO change UART response command ID ERROR: %02X%02X", tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_UART_MODE;

            break;
        }

        uint16_t u16ID = (uint16_t)tRespBuf.u8Seq[1] * 256 + tRespBuf.u8Seq[0];

        if (u16ID != 0xFD)
        {
            GSMprintf("\r\n> GPS EPO change UART response packet type ERROR: %d", u16ID);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_UART_MODE;

            break;
        }

        if (tRespBuf.u8ResCode != 0x03)
        {
            GSMprintf("\r\n> GPS EPO change UART response result ERROR: %d", tRespBuf.u8ResCode);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_UART_MODE;

            break;
        }

        u8EPO_Retry = 0;

        tEPO_Step = EPO_STEP_BEGIN_TRANSFER;

        break;
    }

    // EPO_STEP_RETRY_CHANGE_UART_MODE: Check if retransmission is needed
    case EPO_STEP_RETRY_CHANGE_UART_MODE:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO change UART maximum retry reached.");

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_BEGIN_TRANSFER;

            break;
        }

        GSMprintf("\r\n> GPS EPO change UART retry in the next loop.");

        tEPO_Step = EPO_STEP_CHANGE_UART_MODE;

        break;
    }

    // EPO_STEP_BEGIN_TRANSFER: Begin the EPO file transfer process
    case EPO_STEP_BEGIN_TRANSFER:
    {
        u32DatCnt = 0;

        if (u32FileSize - u32DatCnt >= (GPS_EPO_SAT_DATA_SIZE * 3))
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * 3;
        else
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * ((u32FileSize - u32DatCnt) / GPS_EPO_SAT_DATA_SIZE);

        memset(u8EPO_GetDatBuf, 0x00, sizeof(u8EPO_GetDatBuf));

        bRes = SD_ReadAddrToBuff(cFilePath, u8EPO_GetDatBuf[0], u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            u8EPO_Retry++;

            if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
            {
                u8EPO_Retry = 0;

                GSMprintf("\r\n> GPS EPO read file at address %d ERROR. Maximum retry reached.", u32DatCnt);

                bEPO_Res = FALSE;

                tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

                break;
            }

            GSMprintf("\r\n> GPS EPO read file at address %d ERROR. Retry in the next loop.", u32DatCnt);

            tEPO_Step = EPO_STEP_BEGIN_TRANSFER;

            break;
        }

        // u8EPO_Retry = 0;

        u32StartTime = HAL_GetTick();

        tEPO_Step = EPO_STEP_SEND_PKT;

        break;
    }

    // EPO_STEP_SEND_PKT: Send EPO packet to GPS module
    case EPO_STEP_SEND_PKT:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        // Preamble
        memcpy(tEPO_SendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Length
        tEPO_SendBuf.u8Length[0] = GPS_EPO_PKT_LEN;
        tEPO_SendBuf.u8Length[1] = 0x00;
        // Command ID
        memcpy(tEPO_SendBuf.u8CmdID, u8EPO_CmdID, 2);
        // Sequence number
        u16EPO_SeqID = u32DatCnt / (GPS_EPO_SAT_DATA_SIZE * 3);
        tEPO_SendBuf.u8Seq[0] = (uint8_t)(u16EPO_SeqID % 256);
        tEPO_SendBuf.u8Seq[1] = (uint8_t)(u16EPO_SeqID / 256);
        // Data
        memcpy(tEPO_SendBuf.u8Dat, u8EPO_GetDatBuf, (GPS_EPO_SAT_DATA_SIZE * 3));
        // Checksum
        u8Len = sizeof(tEPO_SendBuf.u8Length) + sizeof(tEPO_SendBuf.u8CmdID) +
                sizeof(tEPO_SendBuf.u8Seq) + sizeof(tEPO_SendBuf.u8Dat);
        u8ChkSum = CalCheckSum(&(tEPO_SendBuf.u8Length[0]), u8Len);
        tEPO_SendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tEPO_SendBuf.u8EndWord, u8EPO_EndWord, 2);

        GSMprintf("\r\n> GPS EPO send packet %d.", u16EPO_SeqID);

        // uint8_t *pu8Ptr = (uint8_t *)&tEPO_SendBuf;

        // for(int i = 0; i < 8; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, sizeof(EPO_PktTypeDef));

        u32DatCnt += u32DatSize;

        // u8EPO_Retry = 0;

        if (u32DatCnt < u32FileSize)
        {
            tEPO_Step = EPO_STEP_GET_DATA;
        }
        else
        {
            tEPO_Step = EPO_STEP_WAIT_RESP;
        }

        u32EPO_WaitTick = HAL_GetTick();

        break;
    }

    // EPO_STEP_GET_DATA: Get the EPO SAT data while waiting for GPS module response
    case EPO_STEP_GET_DATA:
    {
        if (u32FileSize - u32DatCnt >= (GPS_EPO_SAT_DATA_SIZE * 3))
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * 3;
        else
            u32DatSize = GPS_EPO_SAT_DATA_SIZE * ((u32FileSize - u32DatCnt) / GPS_EPO_SAT_DATA_SIZE);

        memset(u8EPO_GetDatBuf, 0x00, sizeof(u8EPO_GetDatBuf));

        bRes = SD_ReadAddrToBuff(cFilePath, u8EPO_GetDatBuf[0], u32DatSize, u32DatCnt);

        if (bRes == FALSE)
        {
            u8EPO_Retry++;

            if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
            {
                u8EPO_Retry = 0;

                GSMprintf("\r\n> GPS EPO read file at address %d ERROR. Maximum retry reached.", u32DatCnt);

                bEPO_Res = FALSE;

                tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

                break;
            }

            GSMprintf("\r\n> GPS EPO read file at address %d ERROR. Retry in the next loop.", u32DatCnt);

            tEPO_Step = EPO_STEP_GET_DATA;

            break;
        }

        // u8EPO_Retry = 0;

        tEPO_Step = EPO_STEP_WAIT_RESP;

        break;
    }

    // EPO_STEP_WAIT_RESP: Wait for response from GPS module
    case EPO_STEP_WAIT_RESP:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(GPS_EPO_RESP_TIMEOUT)))
        {
            GSMprintf("\r\n> GPS EPO wait response timeout.");

#if 0
				if(u16EPO_SeqID == 0)
				{
					u8EPO_Retry++;
				
					if(u8EPO_Retry > GPS_EPO_RETRY_MAX)
					{
						u8EPO_Retry = 0;
						
						GSMprintf("\r\n> GPS EPO transfer maximum retry reached.");
						
						bEPO_Res = FALSE;
						
						tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;
						
						break;
					}
					
					GSMprintf("\r\n> GPS EPO transfer retry in the next loop.");
					
					tEPO_Step = EPO_STEP_CHANGE_UART_MODE;
				}
				else
#endif
            {
                tEPO_Step = EPO_STEP_RETRY_TRANSFER;
            }

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_EndWord, 2);

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_WAIT_RESP;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO response incomplete: ");

            UART_PeekRecvData(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

            // GSMprintf("\r\n> GPS EPO response ERROR: ");

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            // tEPO_Step = EPO_STEP_RETRY_TRANSFER;
            if (u32DatCnt < u32FileSize)
            {
                tEPO_Step = EPO_STEP_SEND_PKT;
            }
            else
            {
                tEPO_Step = EPO_STEP_SEND_FINAL_PKT;
            }

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 12)
        {
            if (u16RespLen > 12)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO response length ERROR (%d): ", u16RespLen);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO response:");

        // uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        // for(int i = 0; i < u16RespLen; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0x02) || (tRespBuf.u8CmdID[1] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO response command ID ERROR: %02X%02X", tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        uint16_t u16ID = (uint16_t)tRespBuf.u8Seq[1] * 256 + tRespBuf.u8Seq[0];

        if (u16ID != u16EPO_SeqID)
        {
            GSMprintf("\r\n> GPS EPO response sequence ID ERROR: %d", u16ID);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        if (tRespBuf.u8ResCode != 1)
        {
            GSMprintf("\r\n> GPS EPO response result ERROR: %d", tRespBuf.u8ResCode);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        if (u32DatCnt < u32FileSize)
        {
            tEPO_Step = EPO_STEP_SEND_PKT;
        }
        else
        {
            tEPO_Step = EPO_STEP_SEND_FINAL_PKT;
        }

        break;
    }

    // EPO_STEP_SEND_FINAL_PKT: Send ending packet to GPS module
    case EPO_STEP_SEND_FINAL_PKT:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        // Preamble
        memcpy(tEPO_SendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Length
        tEPO_SendBuf.u8Length[0] = GPS_EPO_PKT_LEN;
        tEPO_SendBuf.u8Length[1] = 0x00;
        // Command ID
        memcpy(tEPO_SendBuf.u8CmdID, u8EPO_CmdID, 2);
        // Sequence number
        u16EPO_SeqID = 0xFFFF;
        tEPO_SendBuf.u8Seq[0] = 0xFF;
        tEPO_SendBuf.u8Seq[1] = 0xFF;
        // Checksum
        u8Len = sizeof(tEPO_SendBuf.u8Length) + sizeof(tEPO_SendBuf.u8CmdID) +
                sizeof(tEPO_SendBuf.u8Seq) + sizeof(tEPO_SendBuf.u8Dat);
        u8ChkSum = CalCheckSum(&(tEPO_SendBuf.u8Length[0]), u8Len);
        tEPO_SendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tEPO_SendBuf.u8EndWord, u8EPO_EndWord, 2);

        GSMprintf("\r\n> GPS EPO send final packet %d.", u16EPO_SeqID);

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, sizeof(EPO_PktTypeDef));

        // u8EPO_Retry = 0;

        tEPO_Step = EPO_STEP_WAIT_FINAL_RESP;

        u32EPO_WaitTick = HAL_GetTick();

        break;
    }

    // EPO_STEP_WAIT_FINAL_RESP: Wait for ending response from GPS module
    case EPO_STEP_WAIT_FINAL_RESP:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(GPS_EPO_RESP_TIMEOUT)))
        {
            GSMprintf("\r\n> GPS EPO wait response timeout.");

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_EndWord, 2);

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_WAIT_FINAL_RESP;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO response incomplete: ");

            UART_PeekRecvData(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

            // GSMprintf("\r\n> GPS EPO response ERROR: ");

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            u8EPO_Retry = 0;

            bEPO_Res = TRUE;

            u32EndTime = HAL_GetTick();

            // tEPO_Step = EPO_STEP_RETRY_TRANSFER;
            tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 12)
        {
            if (u16RespLen > 12)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO response length ERROR (%d): ", u16RespLen);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        // GSMprintf("\r\n> GPS EPO response:");

        // uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        // for(int i = 0; i < 12; i++)
        //{
        //	GSMprintf(" %02X", *pu8Ptr);

        //	pu8Ptr++;
        //}

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0x02) || (tRespBuf.u8CmdID[1] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO response command ID ERROR: %02X%02X", tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        uint16_t u16ID = (uint16_t)tRespBuf.u8Seq[1] * 256 + tRespBuf.u8Seq[0];

        if (u16ID != u16EPO_SeqID)
        {
            GSMprintf("\r\n> GPS EPO response sequence ID ERROR: %d", u16ID);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        if (tRespBuf.u8ResCode != 1)
        {
            GSMprintf("\r\n> GPS EPO response result ERROR: %d", tRespBuf.u8ResCode);

            tEPO_Step = EPO_STEP_RETRY_TRANSFER;

            break;
        }

        u8EPO_Retry = 0;

        bEPO_Res = TRUE;

        u32EndTime = HAL_GetTick();

        // tEPO_Step = EPO_STEP_RESET_UART_MODE;
        tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

        break;
    }

    // EPO_STEP_RETRY_TRANSFER: Check if retransmission is needed
    case EPO_STEP_RETRY_TRANSFER:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO transfer maximum retry reached.");

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

            break;
        }

        GSMprintf("\r\n> GPS EPO transfer retry in the next loop.");

        tEPO_Step = EPO_STEP_BEGIN_TRANSFER;
        // tEPO_Step = EPO_STEP_CHANGE_UART_MODE;

        break;
    }

    // EPO_STEP_CHANGE_PMTK_MODE: Change UART mode back to PMTK mode
    case EPO_STEP_CHANGE_PMTK_MODE:
    {
        uint8_t u8ChkSum = 0;
        uint8_t u8Len = 0;
        MTK_Type253_TypeDef tSendBuf = {0};

        // Preamble
        memcpy(tSendBuf.u8Preamble, u8EPO_Preamble, 2);
        // Length
        tSendBuf.u8Length[0] = 0x0E;
        tSendBuf.u8Length[1] = 0x00;
        // Command ID
        tSendBuf.u8CmdID[0] = 0xFD;
        tSendBuf.u8CmdID[1] = 0x00;
        // Protocol
        tSendBuf.u8Protocol = 0x00;
        // Data
        tSendBuf.u32BaudRate = 9600;
        // Checksum
        u8Len = sizeof(tSendBuf.u8Length) + sizeof(tSendBuf.u8CmdID) +
                sizeof(tSendBuf.u8Protocol) + sizeof(tSendBuf.u32BaudRate);
        u8ChkSum = CalCheckSum(&(tSendBuf.u8Length[0]), u8Len);
        tSendBuf.u8ChkSum = u8ChkSum;
        // End word
        memcpy(tSendBuf.u8EndWord, u8EPO_EndWord, 2);

        GSMprintf("\r\n> GPS EPO send change PMTK packet: ");

        uint8_t *pu8Ptr = (uint8_t *)&tSendBuf;

        for (int i = 0; i < sizeof(MTK_Type253_TypeDef); i++)
        {
            GSMprintf(" %02X", *pu8Ptr);

            pu8Ptr++;
        }

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tSendBuf, sizeof(MTK_Type253_TypeDef));

        HAL_Delay(1);

        UART_ReInit(GPS_COMPORT_DEF, BR_9600bps);

        // u8EPO_Retry = 0;

        // tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE_WAIT;
        if (bEPO_Res == FALSE)
            tEPO_Step = EPO_STEP_FINISH;
        else
            tEPO_Step = EPO_STEP_VERIFY;

        // HAL_Delay(100);

        u32EPO_WaitTick = HAL_GetTick();

        break;
    }

    // EPO_STEP_CHANGE_PMTK_MODE_WAIT: Wait for response from GPS module
    case EPO_STEP_CHANGE_PMTK_MODE_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(2)))
        {
            GSMprintf("\r\n> GPS EPO wait change PMTK response timeout.");

            // for(int i = 0; i < 3; i++)
            //{
            //	bRes = UART_EXT_ReInit(GPS_COMPORT_DEF, BR_9600bps);

            //	if(bRes)
            //		break;

            //	HAL_Delay(100);
            //}

            // bEPO_Res = FALSE;

            // tEPO_Step = EPO_STEP_FINISH;
            if (bEPO_Res == FALSE)
                tEPO_Step = EPO_STEP_FINISH;
            else
                tEPO_Step = EPO_STEP_VERIFY;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_EndWord, 2);

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 1000)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO change PMTK response incomplete: ");

            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, 12);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            // tEPO_Step = EPO_STEP_RETRY_CHANGE_PMTK_MODE;
            if (bEPO_Res == FALSE)
                tEPO_Step = EPO_STEP_FINISH;
            else
                tEPO_Step = EPO_STEP_VERIFY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 12)
        {
            if (u16RespLen > 12)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO change PMTK response length ERROR: %d, response: %s, \r\n", u16RespLen, &tRespBuf);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            // UART_ReInit(GPS_COMPORT_DEF, BR_9600bps);

            if (bEPO_Res == FALSE)
                tEPO_Step = EPO_STEP_FINISH;
            else
                tEPO_Step = EPO_STEP_VERIFY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        GSMprintf("\r\n> GPS EPO change PMTK response: ");

        uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        for (int i = 0; i < u16RespLen; i++)
        {
            GSMprintf(" %02X", *pu8Ptr);

            pu8Ptr++;
        }

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0x01) || (tRespBuf.u8CmdID[1] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO change PMTK response command ID ERROR: %02X%02X", tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_PMTK_MODE;

            break;
        }

        uint16_t u16ID = (uint16_t)tRespBuf.u8Seq[1] * 256 + tRespBuf.u8Seq[0];

        if (u16ID != 0xFD)
        {
            GSMprintf("\r\n> GPS EPO change PMTK response packet type ERROR: %d", u16ID);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_PMTK_MODE;

            break;
        }

        if (tRespBuf.u8ResCode != 0x03)
        {
            GSMprintf("\r\n> GPS EPO change PMTK response result ERROR: %d", tRespBuf.u8ResCode);

            tEPO_Step = EPO_STEP_RETRY_CHANGE_PMTK_MODE;

            break;
        }

        u8EPO_Retry = 0;

        // UART_ReInit(GPS_COMPORT_DEF, BR_9600bps);

        // tEPO_Step = EPO_STEP_RESET_UART_MODE;
        if (bEPO_Res == FALSE)
            tEPO_Step = EPO_STEP_FINISH;
        else
            tEPO_Step = EPO_STEP_VERIFY;

        break;
    }

    // EPO_STEP_RETRY_CHANGE_PMTK_MODE: Check if retransmission is needed
    case EPO_STEP_RETRY_CHANGE_PMTK_MODE:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX_2)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO change PMTK maximum retry reached.");

            // bEPO_Res = FALSE;

            if (bEPO_Res == FALSE)
                tEPO_Step = EPO_STEP_FINISH;
            else
                tEPO_Step = EPO_STEP_VERIFY;

            break;
        }

        GSMprintf("\r\n> GPS EPO change PMTK retry in the next loop.");

        tEPO_Step = EPO_STEP_CHANGE_PMTK_MODE;

        break;
    }

    // EPO_STEP_RESET_UART_MODE: Change UART mode back to NMEA mode
    case EPO_STEP_RESET_UART_MODE:
    {
        uint8_t u8ChkSum;
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PMTK253,0,9600");

        u8Len = strlen(pcPtr);

        u8ChkSum = CalCheckSum((uint8_t *)pcPtr + 1, u8Len - 1);

        pcPtr += u8Len;

        sprintf(pcPtr, "*%02X\r\n", u8ChkSum);

        u8Len += strlen(pcPtr);

        RTOSprintf("\r\n> GPS EPO send UART reset mode command: %s", (char *)&tEPO_SendBuf);

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        UART_ReInit(GPS_COMPORT_DEF, BR_9600bps);

        u32EPO_WaitTick = HAL_GetTick();

        if (bEPO_Res == FALSE)
            tEPO_Step = EPO_STEP_FINISH;
        else
            tEPO_Step = EPO_STEP_VERIFY;
        // tEPO_Step = EPO_STEP_RESET_UART_MODE_WAIT;

        HAL_Delay(100);

        break;
    }

    // EPO_STEP_RESET_UART_MODE_WAIT: Wait for response from GPS module
    case EPO_STEP_RESET_UART_MODE_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        EPO_PktRespTypeDef tRespBuf = {0};
        char cPreambleSeq[3] = {0}, cEndWordSeq[3] = {0};
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(GPS_EPO_RESP_TIMEOUT)))
        {
            GSMprintf("\r\n> GPS EPO wait reset mode response timeout.");

            // bEPO_Res = FALSE;

            // tEPO_Step = EPO_STEP_FINISH;
            if (bEPO_Res == FALSE)
                tEPO_Step = EPO_STEP_FINISH;
            else
                tEPO_Step = EPO_STEP_VERIFY;

            break;
        }

        memcpy(cPreambleSeq, u8EPO_Preamble, 2);
        memcpy(cEndWordSeq, u8EPO_EndWord, 2);

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_RESET_UART_MODE_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            GSMprintf("\r\n> GPS EPO reset mode response incomplete.");

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            tEPO_Step = EPO_STEP_RETRY_RESET_UART_MODE;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen != 12)
        {
            if (u16RespLen > 12)
            {
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, sizeof(tRespBuf) - 1);

                RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);
            }
            else
                UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

            GSMprintf("\r\n> GPS EPO reset mode response length ERROR (%d): ", u16RespLen);

            uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

            for (int i = 0; i < 20; i++)
            {
                GSMprintf(" %02X", *pu8Ptr);

                pu8Ptr++;
            }

            tEPO_Step = EPO_STEP_RETRY_RESET_UART_MODE;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)&tRespBuf, u16RespLen);

        GSMprintf("\r\n> GPS EPO reset mode response: ");

        uint8_t *pu8Ptr = (uint8_t *)&tRespBuf;

        for (int i = 0; i < 20; i++)
        {
            GSMprintf(" %02X", *pu8Ptr);

            pu8Ptr++;
        }

        // Check response ID
        if ((tRespBuf.u8CmdID[0] != 0x01) || (tRespBuf.u8CmdID[1] != 0x00))
        {
            GSMprintf("\r\n> GPS EPO reset mode response command ID ERROR: %02X%02X", tRespBuf.u8CmdID[1],
                      tRespBuf.u8CmdID[0]);

            tEPO_Step = EPO_STEP_RETRY_RESET_UART_MODE;

            break;
        }

        uint16_t u16ID = (uint16_t)tRespBuf.u8Seq[1] * 256 + tRespBuf.u8Seq[0];

        if (u16ID != 0xFD)
        {
            GSMprintf("\r\n> GPS EPO reset mode response packet type ERROR: %d", u16ID);

            tEPO_Step = EPO_STEP_RETRY_RESET_UART_MODE;

            break;
        }

        if (tRespBuf.u8ResCode != 0x03)
        {
            GSMprintf("\r\n> GPS EPO reset mode response result ERROR: %d", tRespBuf.u8ResCode);

            tEPO_Step = EPO_STEP_RETRY_RESET_UART_MODE;

            break;
        }

        u8EPO_Retry = 0;

        if (bEPO_Res == FALSE)
            tEPO_Step = EPO_STEP_FINISH;
        else
            tEPO_Step = EPO_STEP_VERIFY;

        break;
    }

    // EPO_STEP_RETRY_RESET_UART_MODE: Check if retry is needed
    case EPO_STEP_RETRY_RESET_UART_MODE:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX_2)
        {
            u8EPO_Retry = 0;

            GSMprintf("\r\n> GPS EPO reset mode maximum retry reached.");

            bEPO_Res = FALSE;

            tEPO_Step = EPO_STEP_FINISH;

            break;
        }

        GSMprintf("\r\n> GPS EPO reset mode retry in the next loop.");

        tEPO_Step = EPO_STEP_RESET_UART_MODE;

        break;
    }

    // EPO_STEP_VERIFY: Verify EPO update result
    case EPO_STEP_VERIFY:
    {
        char *pcPtr = 0;
        uint8_t u8Len = 0;

        memset(&tEPO_SendBuf, 0x00, sizeof(EPO_PktTypeDef));

        pcPtr = (char *)&tEPO_SendBuf;

        sprintf(pcPtr, "$PMTK607*33\r\n");

        u8Len = strlen(pcPtr);

        RTOSprintf("\r\n> GPS EPO send verification command: %s", (char *)&tEPO_SendBuf);

        RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

        u16StartPos = rbBuf[GPS_COMPORT_DEF].u16Head;

        UART_SendBuffer(GPS_COMPORT_DEF, (uint8_t *)&tEPO_SendBuf, u8Len);

        u32EPO_WaitTick = HAL_GetTick();

        tEPO_Step = EPO_STEP_VERIFY_WAIT;

        break;
    }

    // EPO_STEP_VERIFY_WAIT: Wait for response from GPS module
    case EPO_STEP_VERIFY_WAIT:
    {
        uint16_t u16FindPos1, u16FindPos2;
        uint16_t u16RespLen = 0;
        uint8_t u8RespBuf[128] = {0};
        EPO_InfoTypeDef tEPO_Info;
        char cPreambleSeq[] = "$PMTK707", cEndWordSeq[] = "\r\n";
        uint16_t u16TickCnt = 0;

        if (HAL_GetTick() > (u32EPO_WaitTick + S_2_mS(3)))
        {
            RTOSprintf("\r\n> GPS EPO wait verification response timeout.");

            tEPO_Step = EPO_STEP_RETRY_VERIFY;

            break;
        }

        bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cPreambleSeq, &u16FindPos1, u16StartPos);

        if (bRes == FALSE)
        {
            tEPO_Step = EPO_STEP_VERIFY_WAIT;

            break;
        }

        u16TickCnt = 0;

        while (u16TickCnt < 100)
        {
            bRes = UART_RecvFindPos(GPS_COMPORT_DEF, cEndWordSeq, &u16FindPos2, u16FindPos1);

            if (bRes)
                break;

            u16TickCnt++;

            HAL_Delay(1);
        }

        if (bRes == FALSE)
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, sizeof(u8RespBuf) - 1);

            RTOSprintf("\r\n> GPS EPO verification response incomplete: %s", (char *)u8RespBuf);
            // RTOSprintf("\r\n> GPS EPO verification response incomplete.");

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            tEPO_Step = EPO_STEP_RETRY_VERIFY;

            break;
        }

        u16RespLen = (rbBuf[GPS_COMPORT_DEF].u16Size + u16FindPos2 + 2 - u16FindPos1) %
                     rbBuf[GPS_COMPORT_DEF].u16Size;

        if (u16RespLen >= sizeof(u8RespBuf))
        {
            UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, sizeof(u8RespBuf) - 1);

            RingBuf_Reset(&rbBuf[GPS_COMPORT_DEF]);

            RTOSprintf("\r\n> GPS EPO verification response length ERROR: %d", u16RespLen);

            tEPO_Step = EPO_STEP_RETRY_VERIFY;

            break;
        }

        UART_GetRecvDataPos(GPS_COMPORT_DEF, u16FindPos1, (uint8_t *)u8RespBuf, u16RespLen);

        RTOSprintf("\r\n> GPS EPO verification response: %s", (char *)u8RespBuf);

        bEPO_Res = TRUE;

        uint8_t u8Num;
        uint32_t u32CurrTime = 0,
                 u32FirstTime, u32LastTime,
                 u32FirstCurrTime, u32LastCurrTime;
        // uint16_t u16EndYear = 0;
        struct tm tmTimeInfo = {0};

        u8Num = sscanf((char *)u8RespBuf, "%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d*",
                       tEPO_Info.cHeader,
                       &(tEPO_Info.u32SetNum),
                       &(tEPO_Info.u32FirstWN),
                       &(tEPO_Info.u32FirstTOW),
                       &(tEPO_Info.u32LastWN),
                       &(tEPO_Info.u32LastTOW),
                       &(tEPO_Info.u32FirstCurrWN),
                       &(tEPO_Info.u32FirstCurrTOW),
                       &(tEPO_Info.u32LastCurrWN),
                       &(tEPO_Info.u32LastCurrTOW));

        if (u8Num != 10)
        {
            RTOSprintf("\r\n> GPS EPO Info parse error: %d", u8Num);

            // if(tEPO_Download.bRequest == TRUE)
            //{
            //	tEPO_Step = EPO_STEP_FINISH;
            // }
            // else
            //{
            //	tEPO_Step = EPO_STEP_NULL;

            //	GPS_EPO_RequestDownload();
            //}
            tEPO_Step = EPO_STEP_RETRY_VERIFY;

            break;
        }

        u32CurrTime = System_GetTime() - HOUR_2_SECOND(8);
        u32FirstTime = GPS_Epoch_2_Calendar(tEPO_Info.u32FirstWN, tEPO_Info.u32FirstTOW);
        u32LastTime = GPS_Epoch_2_Calendar(tEPO_Info.u32LastWN, tEPO_Info.u32LastTOW);
        u32FirstCurrTime = GPS_Epoch_2_Calendar(tEPO_Info.u32FirstCurrWN, tEPO_Info.u32FirstCurrTOW);
        u32LastCurrTime = GPS_Epoch_2_Calendar(tEPO_Info.u32LastCurrWN, tEPO_Info.u32LastCurrTOW);

        localtime_r(&u32FirstTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current File Begin Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32LastTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current File End Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        // u16EndYear = tmTimeInfo.tm_year + 1900;

        localtime_r(&u32FirstCurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current Set Begin Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32LastCurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> EPO Current Set End Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        localtime_r(&u32CurrTime, &tmTimeInfo);

        RTOSprintf("\r\n> System Current UTC Time: %04d/%02d/%02d, %02d:%02d:%02d",
                   tmTimeInfo.tm_year + 1900, tmTimeInfo.tm_mon + 1, tmTimeInfo.tm_mday,
                   tmTimeInfo.tm_hour, tmTimeInfo.tm_min, tmTimeInfo.tm_sec);

        if (tEPO_Download.bRequest == TRUE)
        {
            tEPO_Step = EPO_STEP_FINISH;
        }
        else
        {
            if ((u32CurrTime < u32FirstTime) || ((u32CurrTime + HOUR_2_SECOND(16)) >= u32LastTime))
            {
                RTOSprintf("\r\n> GPS EPO update required.");

                GPS_EPO_RequestDownload();
            }
            else
            {
                RTOSprintf("\r\n> GPS EPO update check OK.");
            }

            tEPO_Step = EPO_STEP_NULL;
        }

        u8EPO_Retry = 0;

        break;
    }

    // EPO_STEP_RETRY_VERIFY: Check if retry is needed
    case EPO_STEP_RETRY_VERIFY:
    {
        u8EPO_Retry++;

        if (u8EPO_Retry > GPS_EPO_RETRY_MAX_2)
        {
            u8EPO_Retry = 0;

            RTOSprintf("\r\n> GPS EPO verification maximum retry reached.");

            bEPO_Res = TRUE;

            if (tEPO_Download.bRequest == TRUE)
            {
                tEPO_Step = EPO_STEP_FINISH;
            }
            else
            {
                GPS_EPO_RequestDownload();

                tEPO_Step = EPO_STEP_NULL;
            }

            break;
        }

        RTOSprintf("\r\n> GPS EPO verification retry in the next loop.");

        tEPO_Step = EPO_STEP_VERIFY;

        break;
    }

    // EPO_STEP_FINISH: Complete the EPO process
    case EPO_STEP_FINISH:
    {
        tEPO_Download.bRequest = FALSE;

        if (bEPO_Res == FALSE)
        {
            GSMprintf("\r\n> GPS EPO update FAIL. Terminate operation.");

            GSMprintf("\r\n> GPS reset.");

            bGPS_ChangeFlag = FALSE;

            g_Setting.u32GPS_Baudrate = BR_9600bps;

            System_SaveSetting();

            GPS_Reset(1);
        }
        else
        {
            GSMprintf("\r\n> GPS EPO update OK, time: %d", u32EndTime - u32StartTime);
        }

        tEPO_Step = EPO_STEP_NULL;

        break;
    }

    default:
        break;
    }
}
#endif

//============================================================================//
// @brief  Get the status of the GPS data.
// @param  None.
// @retval TRUE: The GPS data is fixed.
//         FALSE: The GPS data is not fixed.
//============================================================================//
bool GPS_GetFixed(void)
{
    return g_SysInfo.tGPS_Data.bStatus;
}

//============================================================================//
// @brief  Get GPS UTC time.
// @param  pttUTC: Pointer to a time_t variable.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_GetUTC(time_t *pttUTC)
{
    time_t rawtime;
    struct tm timeinfo;

    if ((g_SysInfo.tGPS_Data.utcTime.u16Year == 0) && (g_SysInfo.tGPS_Data.utcTime.u8Month == 0) && (g_SysInfo.tGPS_Data.utcTime.u8Day == 0))
        return FALSE;

    // �j�� �褸2037 �L��
    if (g_SysInfo.tGPS_Data.utcTime.u16Year > 37)
        return FALSE;

    rawtime = System_GetTime() - g_Setting.i32TimeZoneOffset;

    localtime_r(&rawtime, &timeinfo);
    timeinfo.tm_year = g_SysInfo.tGPS_Data.utcTime.u16Year + 100;
    timeinfo.tm_mon = g_SysInfo.tGPS_Data.utcTime.u8Month - 1;
    timeinfo.tm_mday = g_SysInfo.tGPS_Data.utcTime.u8Day;
    timeinfo.tm_hour = g_SysInfo.tGPS_Data.utcTime.u8Hour;
    timeinfo.tm_min = g_SysInfo.tGPS_Data.utcTime.u8Min;
    timeinfo.tm_sec = g_SysInfo.tGPS_Data.utcTime.u8Sec;

    *pttUTC = mktime(&timeinfo);

    return g_SysInfo.tGPS_Data.bStatus;
}

//============================================================================//
// @brief  Get GPS longitude value.
// @param  p_gpsLongitude: Pointer to a GPS_LonTypeDef variable.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_GetLongitude(GPS_LonTypeDef *p_gpsLongitude)
{
    *p_gpsLongitude = g_SysInfo.tGPS_Data.gpsLon;

    return g_SysInfo.tGPS_Data.bStatus;
}

//============================================================================//
// @brief  Get GPS latitude value.
// @param  p_gpsLatitude: Pointer to a GPS_LatTypeDef variable.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
bool GPS_GetLatitude(GPS_LatTypeDef *p_gpsLatitude)
{
    *p_gpsLatitude = g_SysInfo.tGPS_Data.gpsLat;

    return g_SysInfo.tGPS_Data.bStatus;
}

//============================================================================//
// @brief  Get GPS coordinate.
// @param  p_gpsLocation: Pointer to a GpsCoordTypeDef variable.
// @retval TRUE: Operation is successful.
//         FALSE: Operation is fail.
//============================================================================//
void GPS_GetCoordinate(GpsCoordTypeDef *p_gpsLocation)
{
    *p_gpsLocation = g_SysInfo.tGPS_Data.gpsLocation;
}

//============================================================================//
// @brief  Get GPS speed.
// @param  None.
// @retval GPS speed in format uint32_t.
//============================================================================//
uint32_t GPS_GetSpeed(void)
{
    static int iCurrSpeed, iOldSpeed = 0, iCurrSpeedRes = 0;
    int iSpeedDiff;
    static GPS_SpeedStateTypeDef gpsSpeedState = GPS_SPEED_NORMAL;
    static float fAvgData = 0.0;
    const float fAvgRatio = 0.35;
    static uint32_t u32PeakCnt = 0, u32NotchCnt = 0;
    static bool bPeakFlag = FALSE, bNotchFlag = FALSE;
    int iRandNum = 0;

    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return 0;

    // if(u32RealSpeedTemp == 0)
    {
        iCurrSpeed = g_SysInfo.tGPS_Data.u32Speed;

        iRandNum = (rand() % 6 - 3) * 1000;

        if (bGPS_FirstDetection == FALSE)
        {
            iSpeedDiff = iCurrSpeed - iOldSpeed;
        }
        else
        {
            iSpeedDiff = 0;
        }

        iOldSpeed = iCurrSpeed;

        if (bPeakFlag)
        {
            if ((iCurrSpeed < (fAvgData + 10000)) || (u32PeakCnt > GPS_STATE_LOCK_CNT))
            {
                gpsSpeedState = GPS_SPEED_PEAK_END;
            }
            else
            {
                gpsSpeedState = GPS_SPEED_PEAK_GOING;
            }
        }
        else
        {
            if (iSpeedDiff > 18000)
            {
                gpsSpeedState = GPS_SPEED_PEAK_BEGIN;
            }
            else if (bNotchFlag)
            {
                if (iCurrSpeed >= 5000 || (u32NotchCnt > GPS_STATE_LOCK_CNT))
                {
                    gpsSpeedState = GPS_SPEED_NOTCH_END;
                }
                else
                {
                    gpsSpeedState = GPS_SPEED_NOTCH_GOING;
                }
            }
            else if (iSpeedDiff < -26000)
            {
                gpsSpeedState = GPS_SPEED_NOTCH_BEGIN;
            }
            else if (iCurrSpeed >= 150000)
            {
                gpsSpeedState = GPS_SPEED_OVER_TH;
            }
            else
            {
                gpsSpeedState = GPS_SPEED_NORMAL;
            }
        }

        switch ((int)gpsSpeedState)
        {
        case GPS_SPEED_NORMAL:
            iCurrSpeedRes = iCurrSpeed;

            fAvgData = (1 - fAvgRatio) * fAvgData + fAvgRatio * iCurrSpeed;

            break;

        case GPS_SPEED_PEAK_BEGIN:
            u32PeakCnt++;

            bPeakFlag = TRUE;

            iCurrSpeedRes = (int)fAvgData;

            // RTOSprintf("\r\n> ### GPS SPEED PEAK EVENT BEGIN, DIFF: %d (KM/H) ###", M_2_KM(iSpeedDiff));

            // End any NOTCH event
            u32NotchCnt = 0;

            bNotchFlag = FALSE;

            break;

        case GPS_SPEED_PEAK_GOING:
            gpsSpeedState = GPS_SPEED_PEAK_GOING;

            u32PeakCnt++;

            iCurrSpeedRes = fAvgData + iRandNum;

            if (iCurrSpeedRes < 0)
                iCurrSpeedRes = 0;

            break;

        case GPS_SPEED_PEAK_END:
            u32PeakCnt = 0;

            bPeakFlag = FALSE;

            iCurrSpeedRes = iCurrSpeed;

            // RTOSprintf("\r\n> ### GPS SPEED PEAK EVENT END ###");

            break;

        case GPS_SPEED_NOTCH_BEGIN:
            u32NotchCnt++;

            bNotchFlag = TRUE;

            iCurrSpeedRes = (int)fAvgData;

            // RTOSprintf("\r\n> ### GPS SPEED NOTCH EVENT BEGIN, DIFF: %d (KM/H) ###", M_2_KM(iSpeedDiff));

            // End any PEAK event
            u32PeakCnt = 0;

            bPeakFlag = FALSE;

            break;

        case GPS_SPEED_NOTCH_GOING:
            u32NotchCnt++;

            iCurrSpeedRes = fAvgData + iRandNum;

            if (iCurrSpeedRes < 0)
                iCurrSpeedRes = 0;

            break;

        case GPS_SPEED_NOTCH_END:
            u32NotchCnt = 0;

            bNotchFlag = FALSE;

            iCurrSpeedRes = iCurrSpeed;

            // RTOSprintf("\r\n> ### GPS SPEED NOTCH EVENT END ###");

            break;

        case GPS_SPEED_OVER_TH:

            // iCurrSpeedRes = iOldSpeed;
            iCurrSpeedRes = fAvgData + iRandNum;

            break;

        default:
            break;
        }
    }
    /*
    else
    {
        // If the difference between real speed and GPS speed is higher than the defined threshold
        // Replace GPS speed with real speed
      if((g_SysInfo.tGPS_Data.u32Speed > (u32RealSpeedTemp + SPEED_DIFF_THRESHOLD)) ||
             (u32RealSpeedTemp > (g_SysInfo.tGPS_Data.u32Speed + SPEED_DIFF_THRESHOLD)))
        {
          iCurrSpeedRes = u32RealSpeedTemp;
        }
        else
        {
          iCurrSpeedRes = g_SysInfo.tGPS_Data.u32Speed;
        }

        iOldSpeed = iCurrSpeedRes;
    }
    */

    if (bGPS_FirstDetection == TRUE)
    {
        bGPS_FirstDetection = FALSE;
    }

    return (uint32_t)iCurrSpeedRes;
}

//============================================================================//
// @brief  Configure the GPS automatic mode.
// @param  None.
// @retval GPS course in format uint16_t.
//============================================================================//
uint16_t GPS_GetCourse(void)
{
    if (g_SysInfo.tGPS_Data.bStatus == FALSE)
        return 0;

    return g_SysInfo.tGPS_Data.u16Course;
}

//============================================================================//
// @brief  Calculate the distance between two GPS locations.
// @param  p_gpsLocat1: Pointer to the GPS location 1.
//         p_gpsLocat2: Pointer to the GPS location 2.
// @retval The distance between two GPS locations in double format.
//============================================================================//
double GPS_CalcDist_2D(GpsCoordTypeDef *p_gpsLocat1, GpsCoordTypeDef *p_gpsLocat2)
{
    double dDist;
    double dTheta = p_gpsLocat2->dLongitude - p_gpsLocat1->dLongitude;
    double dLa1, dLa2;

    dTheta *= radius;

    dLa1 = p_gpsLocat1->dLatitude * radius;

    dLa2 = p_gpsLocat2->dLatitude * radius;

    dDist = acos(sin(dLa1) * sin(dLa2) + cos(dLa1) * cos(dLa2) * cos(dTheta));

    if (dDist < 0)
    {
        dDist = dDist + PI;
    }

    dDist = dDist * EarthRadius;

    return dDist;
}

//============================================================================//
// @brief  Convert the GPS latitude degree from DMM to double.
// @param  p_gpsLatitude_DMM: Pointer to a GPS_LatTypeDef variable.
// @retval GPS latitude degree in double format.
//============================================================================//
double GPS_LatitudeDMMtoDD(GPS_LatTypeDef *p_gpsLatitude_DMM)
{
    double dDegree;

    dDegree = p_gpsLatitude_DMM->u8mm * 10000 + p_gpsLatitude_DMM->u16mmmm;

    dDegree /= 600000;

    dDegree += p_gpsLatitude_DMM->u8dd;

    if (p_gpsLatitude_DMM->cIndicator == 'S')
    {
        dDegree *= -1;
    }

    return dDegree;
}

//============================================================================//
// @brief  Convert the GPS longitude degree from DMM to double.
// @param  p_gpsLongitude_DMM: Pointer to a GPS_LonTypeDef variable.
// @retval GPS longitude degree in double format.
//============================================================================//
double GPS_LongitudeDMMtoDD(GPS_LonTypeDef *p_gpsLongitude_DMM)
{
    double dDegree;

    dDegree = p_gpsLongitude_DMM->u8mm * 10000 + p_gpsLongitude_DMM->u16mmmm;

    dDegree /= 600000;

    dDegree += p_gpsLongitude_DMM->u8ddd;

    if (p_gpsLongitude_DMM->cIndicator == 'W')
    {
        dDegree *= -1;
    }

    return dDegree;
}

//============================================================================//
// @brief  Convert the GPS latitude degree from double to DMM.
// @param  dLatitude_DD: GPS latitude in double.
//         p_gpsLatitude_DMM: Pointer to a GPS_LatTypeDef variable.
// @retval GPS latitude degree in double format.
//============================================================================//
void GPS_LatitudeDDtoDMM(double dLatitude_DD, GPS_LatTypeDef *p_gpsLatitude_DMM)
{
    double dDegree;

    dDegree = dLatitude_DD;

    p_gpsLatitude_DMM->cIndicator = 'N';

    if (dDegree < 0)
    {
        p_gpsLatitude_DMM->cIndicator = 'S';

        dDegree *= -1;
    }

    p_gpsLatitude_DMM->u8dd = (int)dDegree;

    dDegree -= (int)dDegree;

    dDegree *= 600000;

    p_gpsLatitude_DMM->u16mmmm = (int)dDegree % 10000;

    p_gpsLatitude_DMM->u8mm = (int)dDegree / 10000;
}

//============================================================================//
// @brief  Convert the GPS longitude degree from double to DMM.
// @param  dLongitude_DD: GPS longitude in double.
//         p_gpsLongitude_DMM: Pointer to a GPS_LonTypeDef variable.
// @retval GPS longitude degree in double format.
//============================================================================//
void GPS_LongitudeDDtoDMM(double dLongitude_DD, GPS_LonTypeDef *p_gpsLongitude_DMM)
{
    double dDegree;

    dDegree = dLongitude_DD;

    p_gpsLongitude_DMM->cIndicator = 'E';

    if (dDegree < 0)
    {
        p_gpsLongitude_DMM->cIndicator = 'W';

        dDegree *= -1;
    }

    p_gpsLongitude_DMM->u8ddd = (int)dDegree;

    dDegree -= (int)dDegree;

    dDegree *= 600000;

    p_gpsLongitude_DMM->u16mmmm = (int)dDegree % 10000;

    p_gpsLongitude_DMM->u8mm = (int)dDegree / 10000;
}

//============================================================================//
// @brief  Save or load GPS position.
// @param  Mode: GPS_POS_SAVE: Save GPS position.
//               GPS_POS_LOAD: Load GPS position.
// @retval None.
//============================================================================//
void GPS_LastCoordinateProcess(GpsModeTypeDef Mode)
{
    switch (Mode)
    {
    case GPS_POS_SAVE:
        if (g_SysInfo.tGPS_Data.bStatus == TRUE)
        {
            RTOSprintf("\r\n> ### Save Last GPS ###");

            g_Setting.gpsLonBK.u8ddd = g_SysInfo.tGPS_Data.gpsLon.u8ddd;
            g_Setting.gpsLonBK.u8mm = g_SysInfo.tGPS_Data.gpsLon.u8mm;
            g_Setting.gpsLonBK.u16mmmm = g_SysInfo.tGPS_Data.gpsLon.u16mmmm;
            g_Setting.gpsLonBK.u8ss = g_SysInfo.tGPS_Data.gpsLon.u8ss;
            g_Setting.gpsLonBK.cIndicator = g_SysInfo.tGPS_Data.gpsLon.cIndicator;

            g_Setting.gpsLatBK.u8dd = g_SysInfo.tGPS_Data.gpsLat.u8dd;
            g_Setting.gpsLatBK.u8mm = g_SysInfo.tGPS_Data.gpsLat.u8mm;
            g_Setting.gpsLatBK.u16mmmm = g_SysInfo.tGPS_Data.gpsLat.u16mmmm;
            g_Setting.gpsLatBK.u8ss = g_SysInfo.tGPS_Data.gpsLat.u8ss;
            g_Setting.gpsLatBK.cIndicator = g_SysInfo.tGPS_Data.gpsLat.cIndicator;
        }
        else
        {
            RTOSprintf("\r\n> ### GPS Lose, not save ###");
        }
        break;

    case GPS_POS_LOAD:
        RTOSprintf("\r\n> ### Load Last GPS ###");

        g_SysInfo.tGPS_Data.gpsLon.u8ddd = g_Setting.gpsLonBK.u8ddd;
        g_SysInfo.tGPS_Data.gpsLon.u8mm = g_Setting.gpsLonBK.u8mm;
        g_SysInfo.tGPS_Data.gpsLon.u16mmmm = g_Setting.gpsLonBK.u16mmmm;
        g_SysInfo.tGPS_Data.gpsLon.u8ss = g_Setting.gpsLonBK.u8ss;
        g_SysInfo.tGPS_Data.gpsLon.cIndicator = g_Setting.gpsLonBK.cIndicator;
        g_SysInfo.tGPS_Data.gpsLocation.dLongitude = GPS_LongitudeDMMtoDD(&g_SysInfo.tGPS_Data.gpsLon);

        g_SysInfo.tGPS_Data.gpsLat.u8dd = g_Setting.gpsLatBK.u8dd;
        g_SysInfo.tGPS_Data.gpsLat.u8mm = g_Setting.gpsLatBK.u8mm;
        g_SysInfo.tGPS_Data.gpsLat.u16mmmm = g_Setting.gpsLatBK.u16mmmm;
        g_SysInfo.tGPS_Data.gpsLat.u8ss = g_Setting.gpsLatBK.u8ss;
        g_SysInfo.tGPS_Data.gpsLat.cIndicator = g_Setting.gpsLatBK.cIndicator;
        g_SysInfo.tGPS_Data.gpsLocation.dLatitude = GPS_LatitudeDMMtoDD(&g_SysInfo.tGPS_Data.gpsLat);
        break;

    default:
        break;
    }
}
