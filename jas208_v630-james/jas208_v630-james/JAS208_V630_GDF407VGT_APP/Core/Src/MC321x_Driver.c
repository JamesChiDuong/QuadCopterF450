//============================================================================//
// User Includes
//============================================================================//
#include "MC321x_Driver.h"

#include "stm32fxxx_hal_user.h"
#include "Default.h"
#include "EXTIO.h"
#include "GPS_Driver.h"

//============================================================================//
// User Variables
//============================================================================//
#if ENABLE_GS_MC3210
static MEM_U16_t G_Last_X;
static MEM_U16_t G_Last_Y;
static MEM_U16_t G_Last_Z;
#endif

//============================================================================//
// User Functions
//============================================================================//
#if ENABLE_GS_MC3210
//============================================================================//
// @brief  Show data read from MC321X registers.
// @param  u8READ_START_ADDR: Start address of the registers to read.
//         pu8Data: Pointer to a uint8_t array storing data to display.
//         u8READ_DATA_LEN: Number of bytes to display.
// @retval None.
//============================================================================//
void MC321X_ShowData(uint8_t u8READ_START_ADDR, uint8_t *pu8Data, uint8_t u8READ_DATA_LEN)
{
    GSprintf("\r\n> G-SENSOR data:");

    for (int i = 0; i < u8READ_DATA_LEN; i++)
    {
        GSprintf("\r\n @< Addr[0x%02X]= 0x%02x = %d %d %d %d  %d %d %d %d",
                 u8READ_START_ADDR + i,
                 pu8Data[i],
                 (pu8Data[i] & 0x80) ? 1 : 0,
                 (pu8Data[i] & 0x40) ? 1 : 0,
                 (pu8Data[i] & 0x20) ? 1 : 0,
                 (pu8Data[i] & 0x10) ? 1 : 0,
                 (pu8Data[i] & 0x08) ? 1 : 0,
                 (pu8Data[i] & 0x04) ? 1 : 0,
                 (pu8Data[i] & 0x02) ? 1 : 0,
                 (pu8Data[i] & 0x01) ? 1 : 0);
    }
}

//============================================================================//
// @brief  Convert a value regarding a change of origin.
// @param  u16Value: Original value.
// @retval Converted value in uint16_t format.
//============================================================================//
uint16_t MC321X_ChangeOrigin(uint16_t u16Value)
{
    uint16_t u16Tvalue = 0;

    // Change range for 0~1023, 512 corresponds to 0
    if (u16Value > MC321X_ORIGIN_VALUE)
    {
        u16Tvalue = 0xFFFF - u16Value + MC321X_ORIGIN_VALUE;
    }
    else
    {
        u16Tvalue = MC321X_ORIGIN_VALUE - u16Value;
    }

    return u16Tvalue;
}

//============================================================================//
// @brief  Get the difference between old value and new value.
// @param  u16OldValue: Old value.
//         u16NewValue: New value.
// @retval Difference between old value and new value in uint16_t format.
//============================================================================//
uint16_t MC321X_ChangeMete(uint16_t u16OldValue, uint16_t u16NewValue)
{
    uint16_t u16Tvalue = 0;

    if (u16OldValue > u16NewValue)
    {
        u16Tvalue = u16OldValue - u16NewValue;
    }
    else
    {
        u16Tvalue = u16NewValue - u16OldValue;
    }

    return u16Tvalue;
}

//============================================================================//
// @brief  Go to Standby mode.
// @param  None.
// @retval None.
//============================================================================//
void MC321X_ToStandby(void)
{
    uint8_t u8Data[10];

    // To STANDBY mode
    u8Data[0] = 0x07;
    u8Data[1] = MC321X_ADDR_VALUE_MODE_0x07_STANDBY;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return;
    //}

    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                            2, MC321X_TIMEOUT);

    // if(bRTOS_En)
    //   osMutexRelease(mutex_i2c1_user);
}

//============================================================================//
// @brief  Go to Wake mode.
// @param  None.
// @retval None.
//============================================================================//
void MC321X_ToWake(void)
{
    uint8_t u8Data[10];

    // To WAKE mode
    u8Data[0] = 0x07;
    u8Data[1] = MC321X_ADDR_VALUE_MODE_0x07_WAKE;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return;
    //}

    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                            2, MC321X_TIMEOUT);

    if (bRTOS_En)
        osMutexRelease(mutex_i2c1_user);
}

//============================================================================//
// @brief  Check Product code.
// @param  None.
// @retval TRUE: Product code is correct.
//         FALSE: Product code is incorrect.
//============================================================================//
bool MC321X_CheckPCode(void)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8TX_Data[10] = {0};
    uint8_t u8RX_Data[10] = {0};

    //	Test Data Is True
    u8TX_Data[0] = 0x3B;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return FALSE;
    //}

    halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8TX_Data,
                                     1, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS check P code send data HAL ERROR %d", halRes);

        return FALSE;
    }

    halRes = HAL_I2C_Master_Receive(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8RX_Data,
                                    1, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS check P code receive data HAL ERROR %d", halRes);

        return FALSE;
    }

    // if(bRTOS_En)
    //   osMutexRelease(mutex_i2c1_user);

    if (u8RX_Data[0] != MC321X_PCODE_ADDR3B)
    {
        GSprintf("\r\n> GS check PCode response:0x%02X", u8RX_Data[0]);

        return FALSE;
    }

    return TRUE;
}

//============================================================================//
// @brief  Initialize the MC321X IC.
// @param  None.
// @retval None.
//============================================================================//
void MC321X_Init(void)
{
    HAL_StatusTypeDef halRes;
    static uint8_t u8InitOK = 0;
    uint8_t u8Data[10];

    if (MC321X_CheckPCode() == FALSE)
    {
        RTOSprintf("\r\n> G-SENSOR ERROR!!!");

        return;
    }
    else
    {
        // GSprintf("\r\n> G-SENSOR OK!!!");
    }

    if (u8InitOK == 0)
    {
        // TO STANDBY
        MC321X_ToStandby();

        // if(bRTOS_En)
        //{
        //	osStatus osRes;

        //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

        //	if(osRes != osOK)
        //		return;
        //}

        u8Data[0] = 0x09;
        u8Data[1] = MC321X_ADDR_VALUE_TAPEN_0x09;
        u8Data[2] = MC321X_ADDR_VALUE_TAPP_0x0A;
        u8Data[3] = MC321X_ADDR_VALUE_DROP_0x0B;
        u8Data[4] = MC321X_ADDR_VALUE_SHDB_0x0C;

        halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                                         5, MC321X_TIMEOUT);

        if (halRes != HAL_OK)
        {
            GSprintf("\r\n> GS setting TAPEN/TAPP/DROP/SHDB HAL ERROR %d", halRes);

            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_NOT_CALIB, STR_NULL);

            return;
        }

        u8Data[0] = 0x20;
        u8Data[1] = MC321X_ADDR_VALUE_OUTCFG_0x20;

        halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                                         2, MC321X_TIMEOUT);

        if (halRes != HAL_OK)
        {
            GSprintf("\r\n> GS setting OUTCFG HAL ERROR %d", halRes);

            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_NOT_CALIB, STR_NULL);

            return;
        }

        u8Data[0] = 0x2B;
        u8Data[1] = MC321X_ADDR_VALUE_SHAKE_TH_0x2B;
        u8Data[2] = MC321X_ADDR_VALUE_UD_Z_TH_0x2C;

        halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                                         3, MC321X_TIMEOUT);

        if (halRes != HAL_OK)
        {
            GSprintf("\r\n> GS setting SHAKE_TH/UD_Z_TH HAL ERROR %d", halRes);

            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_NOT_CALIB, STR_NULL);

            return;
        }

        u8Data[0] = 0x2E;
        u8Data[1] = MC321X_ADDR_VALUE_RL_Z_TH_0x2E;

        halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                                         2, MC321X_TIMEOUT);

        if (halRes != HAL_OK)
        {
            GSprintf("\r\n> GS setting RL_Z_TH HAL ERROR %d", halRes);

            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_NOT_CALIB, STR_NULL);

            return;
        }

        u8Data[0] = 0x30;
        u8Data[1] = MC321X_ADDR_VALUE_FB_Z_TH_0x30;
        u8Data[2] = MC321X_ADDR_VALUE_DROP_TH_0x31;
        u8Data[3] = MC321X_ADDR_VALUE_TAP_TH_0x32;

        halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8Data,
                                         4, MC321X_TIMEOUT);

        if (halRes != HAL_OK)
        {
            GSprintf("\r\n> GS setting FB_Z_TH/DROP_TH/TAP_TH HAL ERROR %d", halRes);

            GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_NOT_CALIB, STR_NULL);

            return;
        }

        // if(bRTOS_En)
        //   osMutexRelease(mutex_i2c1_user);

        u8InitOK = 0xFF;

        // To WAKE mode
        MC321X_ToWake();

        GSM_TCP_ReportEvent((char *)EVENT_G_SENSOR_CALIB, STR_NULL);
    }
}

//============================================================================//
// @brief  Get sensor MA.
// @param  u16NowGSensorMete: Sensor Mete data.
// @retval Sensor MA in uint16_t format.
//============================================================================//
uint16_t MC3210_GetMA(uint16_t u16NowGSensorMete)
{
    static uint16_t u16GSensorMeteMA[MC321X_METE_MA] = {0};
    static uint8_t u8MACount = 0;
    uint32_t u32TotalMA;

    u32TotalMA = 0;

    u16GSensorMeteMA[u8MACount] = u16NowGSensorMete;

    u8MACount++;

    if (u8MACount >= MC321X_METE_MA)
        u8MACount = 0;

    for (int i = 0; i < MC321X_METE_MA; i++)
        u32TotalMA += u16GSensorMeteMA[i];

    return (u32TotalMA / MC321X_METE_MA);
}

//============================================================================//
// @brief  Get current movement status.
// @param  None.
// @retval None.
//============================================================================//
void MC3210_GetState(void)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8RX_Data[10];
    uint8_t u8TX_Data[10];

    u8TX_Data[0] = 0x03;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return;
    //}

    halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8TX_Data,
                                     1, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS get TILT status send data HAL ERROR %d", halRes);

        return;
    }

    halRes = HAL_I2C_Master_Receive(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8RX_Data,
                                    2, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS get TILT status receive data HAL ERROR %d", halRes);

        return;
    }

    // if(bRTOS_En)
    //   osMutexRelease(mutex_i2c1_user);

    MC321X_ShowData(u8TX_Data[0], u8RX_Data, 2);
}

//============================================================================//
// @brief  Get X-Y-Z position.
// @param  None.
// @retval X-Y-Z data in uint16_t format.
//============================================================================//
uint16_t MC3210_GetXYZMete(void)
{
    HAL_StatusTypeDef halRes;
    uint8_t u8RX_Data[10];
    uint8_t u8TX_Data[10];
    uint16_t u16NowGSensorMete;
    uint16_t u16MeteMA;
    MEM_U16_t Now_X;
    MEM_U16_t Now_Y;
    MEM_U16_t Now_Z;
    uint16_t MeteX = 0;
    uint16_t MeteY = 0;
    uint16_t MeteZ = 0;
    static uint32_t u32MyTick = 0;

    u16NowGSensorMete = 0;

    //	READ XYZ DATA
    u8TX_Data[0] = 0x0D;

    // if(bRTOS_En)
    //{
    //	osStatus osRes;

    //	osRes = osMutexWait(mutex_i2c1_user, S_2_mS(10));

    //	if(osRes != osOK)
    //		return FALSE;
    //}

    halRes = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8TX_Data,
                                     1, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS get X-Y-Z data send data HAL ERROR %d", halRes);

        return 0;
    }

    halRes = HAL_I2C_Master_Receive(&hi2c1, (uint16_t)MC321X_ADDR_I2C, u8RX_Data,
                                    6, MC321X_TIMEOUT);

    if (halRes != HAL_OK)
    {
        GSprintf("\r\n> GS get X-Y-Z data receive data HAL ERROR %d", halRes);

        return 0;
    }

    // if(bRTOS_En)
    //   osMutexRelease(mutex_i2c1_user);

    Now_X._U8[0] = u8RX_Data[0];
    Now_X._U8[1] = u8RX_Data[1];
    Now_Y._U8[0] = u8RX_Data[2];
    Now_Y._U8[1] = u8RX_Data[3];
    Now_Z._U8[0] = u8RX_Data[4];
    Now_Z._U8[1] = u8RX_Data[5];

    Now_X._U16 = MC321X_ChangeOrigin(Now_X._U16);
    Now_Y._U16 = MC321X_ChangeOrigin(Now_Y._U16);
    Now_Z._U16 = MC321X_ChangeOrigin(Now_Z._U16);

    if ((G_Last_X._U16 == 0) && (G_Last_Y._U16 == 0) && (G_Last_Z._U16 == 0))
    {
        G_Last_X._U16 = Now_X._U16;
        G_Last_Y._U16 = Now_Y._U16;
        G_Last_Z._U16 = Now_Z._U16;
    }

    if (G_Last_X._U16 != Now_X._U16)
    {
        MeteX = MC321X_ChangeMete(G_Last_X._U16, Now_X._U16);

        u16NowGSensorMete += MeteX;
    }

    if (G_Last_Y._U16 != Now_Y._U16)
    {
        MeteY = MC321X_ChangeMete(G_Last_Y._U16, Now_Y._U16);

        u16NowGSensorMete += MeteY;
    }
    if (G_Last_Z._U16 != Now_Z._U16)
    {
        MeteZ = MC321X_ChangeMete(G_Last_Z._U16, Now_Z._U16);

        u16NowGSensorMete += MeteZ;
    }

    G_Last_X._U16 = Now_X._U16;
    G_Last_Y._U16 = Now_Y._U16;
    G_Last_Z._U16 = Now_Z._U16;

    u16MeteMA = MC3210_GetMA(u16NowGSensorMete);

    if (UART_IsDebugMode(DBG_GS) == TRUE)
    {
        uint32_t u32Speed;
        time_t rawtime;
        char szTime[32];
        struct tm tmTime = {0};

        u32Speed = GPS_GetSpeed();

        rawtime = System_GetTime();

        localtime_r(&rawtime, &tmTime);

        strftime(szTime, sizeof(szTime), "%H:%M:%S", &tmTime);

        if (HAL_GetTick() >= (u32MyTick + 3000))
        {
            u32MyTick = HAL_GetTick();

            GSprintf("\r\n> G-SENSOR XYZ %s: X=%04d, Y=%04d, Z=%04d : METE X=%04d, Y=%04d, Z=%04d : Total=%05d : MA(%d)=%04d ; SPEED-GPS:%03d/KM-%05d-GSS%05d",
                     szTime,
                     G_Last_X._U16,
                     G_Last_Y._U16,
                     G_Last_Z._U16,
                     MeteX,
                     MeteY,
                     MeteZ,
                     u16NowGSensorMete,
                     MC321X_METE_MA,
                     u16MeteMA,
                     M_2_KM(u32Speed),
                     M_2_100M(u32Speed),
                     M_2_100M(g_SysInfo.tVehicleState.u32Speed));
        }
    }

    return u16MeteMA;
}

//============================================================================//
// @brief  Do MC321X related task.
// @param  None.
// @retval None.
//============================================================================//
void MC3210_Process(void)
{
    g_SysInfo.u16GSensorMeter = MC3210_GetXYZMete();

    return;
}
#endif // ENABLE_GS_MC321X
