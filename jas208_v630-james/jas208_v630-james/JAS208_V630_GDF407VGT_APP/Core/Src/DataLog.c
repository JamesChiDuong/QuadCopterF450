//============================================================================//
// Includes
//============================================================================//
#include "DataLog.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "stm32fxxx_hal_user.h"
#include "CAN_Driver.h"
#include "Default.h"
#include "diskio.h"
#include "EXTIO.h"
#include "ff.h"
#include "GSM_Driver.h"
#include "main.h"
#include "RuleV161.h"
#include "SD_Module.h"

//============================================================================//
// User Variables
//============================================================================//
// static DatalogSimpleTypeDef g_dlSimple = {0};	// Heinz

static time_t ttDatalog = 0;

// uint32_t u32StartTickCnt;
// uint32_t u32UseTickCnt;

// GpsCoordTypeDef Location_Log;

// uint8_t gLog_SaveBuf[SD_BLOCK_SIZE * 8] = {0};  // Heinz  4
// uint8_t gLog_SaveBuf[SD_BLOCK_SIZE * 4] = {0};

// uint16_t u16Log_SaveBufLen;

// uint32_t u32FileIdxBk = 0;

// char szFileNameBk[32];

bool bLog_Ready = FALSE;

// bool bLog_BuildAll;

// bool bDatalog_Stop_REC = 0; // Heinz

// uint16_t bUploadDataTypeDefTick = 0;

// time_t ttReUploadIdxTime = 0;
// time_t ttReUploadBegin = 0;
// time_t ttReUploadEnd = 0;

bool bDatalogTickFlag = FALSE;

#if ENABLE_GSM_NET
DatalogUploadTypeDef datalogHandle[MAX_SOCKET_BANK],
    datalogHandleTemp[MAX_SOCKET_BANK];

uint8_t u8DATALOG_UPLOAD_MAX[MAX_SOCKET_BANK] = {DATALOG_UPLOAD_MAX_30};
#endif

#if ENABLE_SPEED_INT_REC
SpeedIntTypeDef tSpeedInt, tSpeedIntTemp;

bool bRecordSpeedIntFlag = FALSE;

time_t ttRecordSpeedIntTime;

const uint32_t u32MaskBit[32] =
    {
        0x00000001, 0x00000002, 0x00000004, 0x00000008,
        0x00000010, 0x00000020, 0x00000040, 0x00000080,
        0x00000100, 0x00000200, 0x00000400, 0x00000800,
        0x00001000, 0x00002000, 0x00004000, 0x00008000,
        0x00010000, 0x00020000, 0x00040000, 0x00080000,
        0x00100000, 0x00200000, 0x00400000, 0x00800000,
        0x01000000, 0x02000000, 0x04000000, 0x08000000,
        0x10000000, 0x20000000, 0x40000000, 0x80000000};
#endif

#if ENABLE_PULSE_COUNT_REC
PulseCountTypeDef tPulseCnt, tPulseCntTemp;

bool bRecordPulseCntFlag = FALSE;

time_t ttRecordPulseCntTime = 0;
#endif

#if ENABLE_GPS_SPEED_REC
GPS_SpeedRecTypeDef tGPS_Speed, gpsSpdRecTemp;

bool bRecordGPS_SpeedFlag = FALSE;

time_t ttRecordGPS_SpeedTime = 0;
#endif

const char *RecordModeFile[] =
    {
        NULL,
        FILENAME_SPEED_INT_FILE,
        FILENAME_CAN_DEBUG_MSG_FILE,
        FILENAME_PULSE_CNT_FILE,
        FILENAME_GPS_SPEED_FILE,
        NULL};

ProcStepTypeDef procClearFileStep = PROC_STEP_NULL;

//============================================================================//
// User Functions
//============================================================================//

//============================================================================//
// @brief  Prepare for datalog process.
// @param  None.
// @retval None.
//============================================================================//
void Datalog_Init(void)
{
    ttDatalog = 0;

    // memset(gLog_SaveBuf, 0x00, sizeof(gLog_SaveBuf));

    // u16Log_SaveBufLen = 0;

    // bLog_BuildAll = TRUE;

    bLog_Ready = TRUE;

    // bDatalog_Stop_REC = 0;
}

//============================================================================//
// @brief  Reset for datalog process.
// @param  None.
// @retval None.
//============================================================================//
void Datalog_Reset(void)
{
    Datalog_Init();

    // SD_Delete(FILENAME_DATALOG_BIN);
    SD_RemoveFile(FILENAME_DATALOG_BIN);
}

//============================================================================//
// @brief  Backup datalog.
// @param  None.
// @retval None.
//============================================================================//
static void Datalog_Backup(time_t BackupTime)
{
    char *pOld, *pNew, BufStr[256] = {0};
    struct tm tmTime = {0};

    // DTR file

    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME2);

    RTOSprintf("\r\n> ### Make DTR Proc ###");

    localtime_r(&BackupTime, &tmTime);

    pOld = pNew = BufStr;

    // sprintf(pOld, "%d:/%s", DRV_USER, FILENAME_DATALOG_BIN);
    sprintf(pOld, "%s", FILENAME_DATALOG_BIN);

    pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

    strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/%Y%m%d.DTR", &tmTime);

    // SD_RemoveFile(pNew); // delete [/DTR/YYYYMMDD.DTR]
    if (SD_CheckFileExist(pNew) == FALSE)
    {
        // V161_CopyFile(pOld, pNew); // copy [/DATALOG.BIN] to [DTR/YYYYMMDD.DTR]
        SD_Rename(pOld, pNew);

        SD_RemoveFile(pOld); // delete [/DATALOG.BIN]

        LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
    }

    // Event file

    memset(BufStr, 0x00, sizeof(BufStr));

    pOld = pNew = BufStr;

    sprintf(pOld, "%s", FILENAME_DATALOG_EVENT_BIN);

    pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

    strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/%Y%m%d_Event.DTR", &tmTime);

    // SD_RemoveFile(pNew); // delete [/DTR/YYYYMMDD_Event.DTR]
    if (SD_CheckFileExist(pNew) == FALSE)
    {
        // V161_CopyFile(pOld, pNew); // copy [/DATALOG_EVENT.BIN] to [/DTR/YYYYMMDD_Event.DTR]
        SD_Rename(pOld, pNew);

        SD_RemoveFile(pOld); // delete [/DATALOG_EVENT.BIN]

        LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
    }

    RTOSprintf("\r\n> ### Make DTR End ###");
}

//============================================================================//
// @brief  Backup debug files.
// @param  None.
// @retval None.
//============================================================================//
void Datalog_BackupDebugFiles(time_t ttBackupTime)
{
    char *pOld, *pNew, BufStr[256] = {0};
    struct tm tmTime = {0};

    RTOSprintf("\r\n> ### Backup debug files ###");

    localtime_r(&ttBackupTime, &tmTime);

#if ENABLE_SPEED_INT_REC
    if (Datalog_IsRecordMode(REC_SPD_INT))
    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_SPEED_INT_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/SpeedInt_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/SpeedInt_YYYYMMDD.tmp]

            // CanSpeed File // copy [/SpeedInt.tmp] to [/DTR/SpeedInt_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/SpeedInt.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }
#endif

#if ENABLE_DEBUG_CAN_REC
    if (Datalog_IsRecordMode(REC_CAN_DBG))
    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_CAN_DEBUG_MSG_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/CanDebug_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/CanDebug_YYYYMMDD.tmp]

            // CanDebug File // copy [/CanDebug.tmp] to [/DTR/CanDebug_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/CanDebug.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }
#endif

#if ENABLE_PULSE_COUNT_REC
    if (Datalog_IsRecordMode(REC_PULSE_CNT))
    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_PULSE_CNT_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/PulseCount_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/PulseCount_YYYYMMDD.tmp]

            // PulseCount File // copy [/PulseCount.tmp] to [/DTR/PulseCount_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/PulseCount.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }
#endif

#if ENABLE_GPS_SPEED_REC
    if (Datalog_IsRecordMode(REC_GPS_SPD))
    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_GPS_SPEED_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/GPS_Speed_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/GPS_Speed_YYYYMMDD.tmp]

            // GPS_Speed File // copy [/GPS_Speed.tmp] to [/DTR/GPS_Speed_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/GPS_Speed.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }
#endif

    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_GPRS_LOG_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/GPRS_Log_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/GPRS_Log_YYYYMMDD.tmp]

            // GPS_Speed File // copy [/GPRS_Log.tmp] to [/DTR/GPRS_Log_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/GPRS_Log.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }

    {
        memset(BufStr, 0x00, sizeof(BufStr));

        pOld = pNew = BufStr;

        sprintf(pOld, "%s", FILENAME_AGAIN_LOG_FILE);

        pNew = pOld + strlen(pOld) + 1; // pNew is place after pOld

        strftime(pNew, sizeof(BufStr) - strlen(pOld), "DTR/AGAIN_Log_%Y%m%d.tmp", &tmTime);

        if (SD_CheckFileExist(pOld))
        {
            SD_RemoveFile(pNew); // delete [/DTR/AGAIN_Log_YYYYMMDD.tmp]

            // GPS_Speed File // copy [/AGAIN_Log.tmp] to [/DTR/AGAIN_Log_YYYYMMDD.tmp]
            SD_Rename(pOld, pNew);

            SD_RemoveFile(pOld); // delete [/AGAIN_Log.tmp]

            LOGprintf("\r\n> SD card file name %s rename %s.", pOld, pNew);
        }
    }

    RTOSprintf("\r\n> ### Backup debug files OK ###");
}

//============================================================================//
// ttDatalog=0, read the last data log and save into g_SysInfo.VehicleState
// if not the same day, save DATALOG.BIN to YYYYMMDD.DTR
// set ttDatalog = ttLocal;
//============================================================================//
bool Datalog_ChkData(void)
{
    time_t ttLocal = 0;
    // DatalogV161_TypeDef dlSimple;
    DatalogSimpleTypeDef dlSimple;
    uint32_t u32FileSize;
    bool bGetData;
    uint8_t u8ErrCnt, u8BCD[7];
#if ENABLE_GSM_NET
    char cTime[20] = {0};
#endif

    // RTOSprintf("\r\n> ### Datalog_ChkData ###");

    memset(&dlSimple, 0x00, sizeof(DatalogSimpleTypeDef));

    ttLocal = System_GetTime();

    if (ttDatalog == 0)
    {
        u8ErrCnt = 0;

        do
        {
            u32FileSize = SD_GetFileSize(FILENAME_DATALOG_BIN);

            // RTOSprintf("\r\n> DATALOG_BIN filesize: %d ", u32FileSize);

            if (u32FileSize <= 0)
            {
                RTOSprintf("\r\n> ### DATALOG_BIN NG or NULL ###");

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_DATALOG_FILE_NG);

                break;
            }

            // Ū���̫�@���O��
            bGetData = SD_ReadAddrToBuff(FILENAME_DATALOG_BIN, (uint8_t *)&dlSimple,
                                         sizeof(DatalogSimpleTypeDef), u32FileSize - sizeof(DatalogSimpleTypeDef));

            if (bGetData == TRUE)
            {
                if (dlSimple.HEAD.CMD == CMD_DATALOG_SIMPLE)
                {
                    ttDatalog = dlSimple.HEAD.UTC + g_Setting.i32TimeZoneOffset;

                    SecToBCDTime(ttDatalog, u8BCD);
                    // RTOSprintf("\r\n> ### (1)ttDatalog %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                    //						u8BCD[0],u8BCD[1],u8BCD[2],u8BCD[3],u8BCD[4],u8BCD[5],u8BCD[6]);

                    // System_RestoreBackupSetting(VAR_DISTANCE);
                    // g_SysInfo.tVehicleState.dDistance = dlSimple.HEAD.dwDistance;

                    // g_SysInfo.tVehicleState.dDistBase = g_SysInfo.tVehicleState.dDistance;

                    g_SysInfo.tVehicleState.u32DistCount = 0;

                    g_SysInfo.tVehicleState.u32DistCount_AGain = 0;

                    g_SysInfo.tVehicleState.dDistBase_GPS = 0;

                    g_SysInfo.tVehicleState.dDistance_GPS = 0;
                }
                else
                {
                    RTOSprintf("\r\n> ### Remove DATALOG_BIN ###");

                    RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_DATALOG_HEAD_NG);

                    SD_RemoveFile(FILENAME_DATALOG_BIN);
                }

                break;
            }
            else
            {
                RTOSprintf("\r\n> ### Read DATALOG_BIN Fail ###");

                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_DATALOG_READ_NG);

                memset(&dlSimple, 0x00, sizeof(DatalogSimpleTypeDef));
            }

            u8ErrCnt++;

        } while (u8ErrCnt < DATALOG_MAX_ERR_CNT);
    }

    /*
    if((g_Setting.ttLastVDR_Time != 0) &&
         (g_Setting.ttLastVDR_Time != 0xFFFFFFFF))
    {
      if((((g_Setting.ttLastVDR_Time - 5) / DAY_2_SECOND(1)) !=
             ((ttLocal - 5) / DAY_2_SECOND(1))) &&
           (bDoVDR_DayFlag == FALSE))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME2);

            ttLastLogTime = g_Setting.ttLastVDR_Time; // - MINUTE_2_SECOND(1);

            SecToBCDTime(g_Setting.ttLastVDR_Time, u8BCD);

            RTOSprintf("\r\n> ### (2)ttLastLogTime %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                                 u8BCD[0],u8BCD[1],u8BCD[2],u8BCD[3],u8BCD[4],u8BCD[5],u8BCD[6]);

            // Make V161 one Min VDR File
            bDoVDR_DayFlag = TRUE;

            SecToBCDTime(ttLocal, u8BCD);

            memset(cTime, 0x00, sizeof(cTime));

            sprintf(cTime, "%02X%02X%02X%02X-%02X%02X%02X",
                    u8BCD[0],u8BCD[1],u8BCD[2],u8BCD[3],u8BCD[4],u8BCD[5],u8BCD[6]);

            GSM_TCP_ReportEvent((char *)EVENT_VDR_REQUEST, cTime);
        }
    }
    else
    {
        RTOSprintf("\r\n> ### INVALID LAST VDR TIME ###");

        SecToBCDTime(g_Setting.ttLastVDR_Time, u8BCD);

        memset(cTime, 0x00, sizeof(cTime));

        sprintf(cTime, "%02X%02X%02X%02X-%02X%02X%02X",
                        u8BCD[0],u8BCD[1],u8BCD[2],u8BCD[3],u8BCD[4],u8BCD[5],u8BCD[6]);

        GSM_TCP_ReportEvent((char *)EVENT_VDR_TIME_FAIL, cTime);

      g_Setting.ttLastVDR_Time = System_GetTime();

        System_SaveSetting();
    }
    */

    if (ttDatalog != 0)
    {
        // if((ttDatalog / DAY_2_SECOND(1)) != (ttLocal / DAY_2_SECOND(1)))
        //{
        //	RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME1);

        // Make DTR File
        //	Datalog_Backup(ttDatalog);
        //}

        if (((ttDatalog - 5) / DAY_2_SECOND(1)) != ((ttLocal - 5) / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME2);

            ttLastLogTime = ttDatalog - MINUTE_2_SECOND(1);

            SecToBCDTime(ttDatalog, u8BCD);
            RTOSprintf("\r\n> ### (2)ttLastLogTime %02X%02X/%02X/%02X %02X:%02X:%02X ###",
                       u8BCD[0], u8BCD[1], u8BCD[2], u8BCD[3], u8BCD[4], u8BCD[5], u8BCD[6]);

#if ENABLE_GSM_NET
            memset(cTime, 0x00, sizeof(cTime));

            sprintf(cTime, "%02X%02X%02X%02X-%02X%02X%02X",
                    u8BCD[0], u8BCD[1], u8BCD[2], u8BCD[3], u8BCD[4], u8BCD[5], u8BCD[6]);

            GSM_TCP_ReportEvent((char *)EVENT_VDR_REQUEST, cTime);
#endif

            // Make V161 daily VDR File
            // bDoVDR_DayFlag = TRUE;

            V161_MakeDailyVDR();
        }

        if (((ttDatalog - 6) / DAY_2_SECOND(1)) != ((ttLocal - 6) / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME1);

            // Make DTR File
            Datalog_Backup(ttDatalog);
        }

        if (((ttDatalog - 7) / DAY_2_SECOND(1)) != ((ttLocal - 7) / DAY_2_SECOND(1)))
        {
            RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME1);

            // Make debug files
            Datalog_BackupDebugFiles(ttDatalog);

// Request to send backup setting to FTP server
#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
            System_UploadBackupSettingToFTP(ttLocal);
#endif
        }

        /*
            if(((ttDatalog + MINUTE_2_SECOND(1)) / DAY_2_SECOND(1)) != ((ttLocal+MINUTE_2_SECOND(1)) / DAY_2_SECOND(1)))
            {
                RTOSprintf("\r\n> [SYS]%04X", SYS_STATE_SYS_RENAME2);

                // Add 161 Make VDR File
                //V161_MoveDataProc();

                bKeepDayDataFlag = FALSE;

                bDoVDRFlag = TRUE;
            }
        */
    }

    if (ttDatalog == ttLocal)
        return FALSE;

    ttDatalog = ttLocal;

    return TRUE;
}

bool Datalog_Save_Proc(void)
{
    DatalogSimpleTypeDef dlSimple;
    time_t ttLogTime;

    if (bDatalogTickFlag == TRUE)
    {
        bDatalogTickFlag = FALSE;

        if (bLog_Ready == FALSE)
            return FALSE;

        ttLogTime = System_GetTime();

        if (Datalog_ChkData() == TRUE)
        {
            memset(&dlSimple, 0x00, sizeof(DatalogSimpleTypeDef));

            dlSimple.HEAD.CMD = CMD_DATALOG_SIMPLE;

            dlSimple.HEAD.u16Version = DATALOG_VERSION_DEF;

            dlSimple.HEAD.UTC = ttLogTime - g_Setting.i32TimeZoneOffset;

            dlSimple.HEAD.dwDistance = g_SysInfo.tVehicleState.dDistance;

            dlSimple.HEAD.u32DRV_IN = g_Setting.tDrvInfo[g_Setting.u8CurrDrvID].u32DrvIdNum;

            dlSimple.HEAD.u8DataCnt = 1;

            dlSimple.DATA.i32Longitude = g_SysInfo.tGPS_Data.gpsLocation.dLongitude * 1000000;

            dlSimple.DATA.i32Latitude = g_SysInfo.tGPS_Data.gpsLocation.dLatitude * 1000000;

            dlSimple.DATA.u8Course = g_SysInfo.tGPS_Data.u16Course / 10;

            dlSimple.DATA.u8Speed = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);

            dlSimple.DATA.b50RPM = g_SysInfo.tVehicleState.u16RPM / 50;

            dlSimple.DATA.EXTIO.bit._7 = g_SysInfo.tEXTIO.bits.IN_ACC;

            dlSimple.DATA.EXTIO.bit._6 = g_SysInfo.tEXTIO.bits.IN_1;

            dlSimple.DATA.EXTIO.bit._5 = g_SysInfo.tEXTIO.bits.IN_2;

            dlSimple.DATA.EXTIO.bit._4 = g_SysInfo.tEXTIO.bits.IN_3;

            dlSimple.DATA.EXTIO.bit._3 = g_SysInfo.tEXTIO.bits.IN_4;

            dlSimple.DATA.EXTIO.bit._2 = g_SysInfo.tEXTIO.bits.IN_5;

            dlSimple.DATA.EXTIO.bit._1 = g_SysInfo.tEXTIO.bits.IN_6;

            dlSimple.DATA.EXTIO.bit._0 = g_SysInfo.tEXTIO.bits.IN_7;

            dlSimple.DATA.u8Speed_GPS = (uint8_t)M_2_KM(g_SysInfo.tVehicleState.u32Speed_GPS);

            SD_AddBuffToFile(FILENAME_DATALOG_BIN, (uint8_t *)&dlSimple, sizeof(DatalogSimpleTypeDef));

            // Datalog_Build((uint8_t*)&g_dlSimple, sizeof(DatalogV161_TypeDef));
        }

        // RTOSprintf("\r\n> ### Log Bin record ###");

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if ENABLE_CAN == 1 && CAN_CUSTOM == CAN_CUSTOM_GDBUS
//============================================================================//
// @brief  Get CAN battery data for datalog.
// @param  None.
// @retval None.
//============================================================================//
void Datalog_GetBatInfo(void)
{
    Datalog_BAT_InfoTypeDef tBAT_INFO;

    memset(&tBAT_INFO, 0x00, sizeof(Datalog_BAT_InfoTypeDef));

    tBAT_INFO.CMD = CMD_DATALOG_BATTERY;

    tBAT_INFO.MOT_SPEED = CAN_GetMotSpeed();

    tBAT_INFO.CHG_STATE = CAN_GetBatChg();

    tBAT_INFO.BAT_VOL = CAN_GetBatVoltage();

    tBAT_INFO.BAT_CUR = CAN_GetBatCurrent();

    tBAT_INFO.BAT_SOC = CAN_GetSOC();

    tBAT_INFO.BAT_CELL_VMAX = CAN_GetCellVolMax();

    tBAT_INFO.BAT_CELL_VMIN = CAN_GetCellVolMin();

    tBAT_INFO.BAT_CELL_TMAX = CAN_GetCellTmpMax();

    tBAT_INFO.BAT_CELL_TMIN = CAN_GetCellTmpMin();
}
#endif

#if ENABLE_GSM_NET
//============================================================================//
// @brief  Resend LOG messages in a time interval.
// @param  ttStart: Indicate the starting point of the interval in which LOG
//         messages need to be sent again.
//         ttStop: Indicate the end point of the interval in which LOG messages
//         need to be sent again.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool Datalog_ResendLog(time_t ttStart, time_t ttStop, uint8_t u8SocketBank)
{
    bool bRes;
    char cFileName[40];
    time_t ttNow, ttBuf;
    uint8_t u8Cnt;
    uint32_t u32FileSize, u32LogBegin, u32LogIdx, u32LogEnd;
    DatalogSimpleTypeDef stGetDTRDataLog;
    DatalogUploadTypeDef reUploadDataLog;
    struct tm tmTime = {0};

    // RTOSprintf("\r\n> ttStart: %d, ttStop: %d", ttStart, ttStop);

    ttNow = System_GetTime();

    ttBuf = ttStop + g_Setting.i32TimeZoneOffset;

    if ((ttNow - ttBuf) < DAY_2_SECOND(1))
    {
        sprintf(cFileName, "%s", FILENAME_DATALOG_BIN);
    }
    else
    {
        localtime_r(&ttBuf, &tmTime);

        strftime(cFileName, sizeof(cFileName), "DTR/%Y%m%d.DTR", &tmTime);
    }

    u32FileSize = SD_GetFileSize(cFileName);

    RTOSprintf("\r\n> ### File: %s, Size: %d ###", cFileName, u32FileSize);

    if (u32FileSize == 0)
    {
        RTOSprintf("\r\n> DataLog File Has No Data.\r\n");

        return TRUE;
    }

    bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&stGetDTRDataLog, sizeof(DatalogSimpleTypeDef), 0);

    if (bRes != TRUE)
    {
        RTOSprintf("\r\n> DataLog File Error.\r\n");

        return FALSE;
    }

    if ((bRes == TRUE) && (stGetDTRDataLog.HEAD.CMD != CMD_DATALOG_SIMPLE))
    {
        RTOSprintf("\r\n> DataLog Head Error.\r\n");

        return FALSE;
    }

    memset(&stGetDTRDataLog, 0x00, sizeof(DatalogSimpleTypeDef));

    u32LogIdx = 0;
    u32LogBegin = 0;

    u32LogEnd = u32FileSize / sizeof(DatalogSimpleTypeDef);

    do
    {
        u32LogIdx = (u32LogEnd - u32LogBegin) / 2 + u32LogBegin;

        if ((u32LogIdx <= u32LogBegin) || (u32LogIdx >= u32LogEnd))
            break;

        bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&stGetDTRDataLog,
                                 sizeof(DatalogSimpleTypeDef), sizeof(DatalogSimpleTypeDef) * u32LogIdx);

        if ((bRes == TRUE) &&
            (stGetDTRDataLog.HEAD.CMD == CMD_DATALOG_SIMPLE) &&
            (stGetDTRDataLog.HEAD.UTC == ttStop))
        {
            RTOSprintf("\r\n> Stop Index Get it.");

            break;
        }

        if (stGetDTRDataLog.HEAD.UTC > ttStop)
        {
            u32LogEnd = u32LogIdx;
        }

        if (stGetDTRDataLog.HEAD.UTC < ttStop)
        {
            u32LogBegin = u32LogIdx;
        }

    } while (u32LogEnd != u32LogBegin);

    // RTOSprintf("\r\n> Stop Index: %d, Addr: %08X\r\n", u32LogIdx, sizeof(DatalogSimpleTypeDef) * u32LogIdx);

    HAL_Delay(100);

    memset(&reUploadDataLog, 0x00, sizeof(DatalogUploadTypeDef));

    u8Cnt = 0;

    if (bRTOS_En)
    {
        osStatus osRes;

        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            return FALSE;
        }
    }

    if ((bSystemMode == SYSTEM_MODE_STEALTH) && (bWaitingEnterStandby == TRUE))
    {
        RTOSprintf("\r\n> Resend LOG refresh enter standby timer.");

        // osTimerStop(timerACC_OffHandle);

        // osTimerStart(timerACC_OffHandle, S_2_mS(60));

        u32PwrDownDelayTick = HAL_GetTick();
    }

    do
    {
        bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&stGetDTRDataLog,
                                 sizeof(DatalogSimpleTypeDef), sizeof(DatalogSimpleTypeDef) * u32LogIdx);

        u32LogIdx--;

        if (stGetDTRDataLog.HEAD.UTC >= ttStart)
        {
            if (u8Cnt == 0)
            {
                memcpy(&reUploadDataLog.HEAD, &stGetDTRDataLog.HEAD, sizeof(stGetDTRDataLog.HEAD));
            }

            memcpy(&reUploadDataLog.DATA[u8Cnt], &stGetDTRDataLog.DATA, sizeof(stGetDTRDataLog.DATA));

            u8Cnt++;

            if (u8Cnt >= u8DATALOG_UPLOAD_MAX[u8SocketBank])
            {
                reUploadDataLog.HEAD.u8DataCnt = u8Cnt;

                GSM_TCP_ReSendLOG(&reUploadDataLog, u8SocketBank);

                u8Cnt = 0;

                memset(&reUploadDataLog, 0x00, sizeof(DatalogUploadTypeDef));
            }
        }
        else
        {
            reUploadDataLog.HEAD.u8DataCnt = u8Cnt;

            GSM_TCP_ReSendLOG(&reUploadDataLog, u8SocketBank);

            u8Cnt = 0;

            memset(&reUploadDataLog, 0x00, sizeof(DatalogUploadTypeDef));
        }

        // RTOSprintf("\r\n> UTC: %d, ttStart: %d", stGetDTRDataLog.HEAD.UTC, ttStart);

    } while (stGetDTRDataLog.HEAD.UTC >= ttStart);

    if (bRTOS_En)
        // Release GSM SD synchronization
        osMutexRelease(mutex_gsm_sd_user);

    RTOSprintf("\r\n> ### ReSend OK!! ###\r\n");

    return TRUE;
}

//============================================================================//
// @brief  Resend EVENT messages in a time interval.
// @param  ttStart: The starting point of the time interval in which the EVENT
//         messages need to be sent again.
//         ttEnd: The end point of the time interval in which the EVENT messages
//         need to be sent again.
// @retval TRUE: The operation is successful.
//         FALSE: The operation is failed.
//============================================================================//
bool Datalog_ResendEvent(time_t ttStart, time_t ttStop, uint8_t u8SocketBank)
{
    bool bRes;
    char cFileName[40];
    time_t ttNow;
    uint32_t u32FileSize, u32LogIdx, u32LogEnd;
    DatalogEventlogTypeDef stDatalogEvent;
    struct tm tmTime = {0};

    // ttStart = ttStart - g_Setting.i32TimeZoneOffset;

    // ttStop = ttStop - g_Setting.i32TimeZoneOffset;

    // RTOSprintf("\r\n> [Datalog_ResendEvent] ttStart: %d, ttStop: %d", ttStart, ttStop);

    ttNow = System_GetTime();

    if ((ttNow - ttStop) < DAY_2_SECOND(1))
    {
        sprintf(cFileName, "%s", FILENAME_DATALOG_EVENT_BIN);
    }
    else
    {
        localtime_r(&ttStop, &tmTime);

        strftime(cFileName, sizeof(cFileName), "DTR/%Y%m%d_Event.DTR", &tmTime);
    }

    u32FileSize = SD_GetFileSize(cFileName);

    RTOSprintf("\r\n> ### File: %s, Size: %d ###", cFileName, u32FileSize);

    if (u32FileSize == 0)
    {
        RTOSprintf("\r\n> DataEvent File Has No Data.\r\n");

        return TRUE;
    }

    memset(&stDatalogEvent, 0x00, sizeof(DatalogEventlogTypeDef));

    u32LogIdx = 0;
    u32LogEnd = u32FileSize / sizeof(DatalogEventlogTypeDef);

    // RTOSprintf("\r\n> ### u32LogIdx: %d, u32LogEnd: %d ###", u32LogIdx, u32LogEnd);

    if (bRTOS_En)
    {
        osStatus osRes;

        // Acquire GSM_SD synchronization
        osRes = osMutexWait(mutex_gsm_sd_user, S_2_mS(10));

        if (osRes != osOK)
        {
            return FALSE;
        }
    }

    if ((bSystemMode == SYSTEM_MODE_STEALTH) && (bWaitingEnterStandby == TRUE))
    {
        RTOSprintf("\r\n> Resend EVENT refresh enter standby timer.");

        // osTimerStop(timerACC_OffHandle);

        // osTimerStart(timerACC_OffHandle, S_2_mS(60));

        u32PwrDownDelayTick = HAL_GetTick();
    }

    do
    {
        bRes = SD_ReadAddrToBuff(cFileName, (uint8_t *)&stDatalogEvent,
                                 sizeof(DatalogEventlogTypeDef), sizeof(DatalogEventlogTypeDef) * u32LogIdx);

        if ((bRes == TRUE) && (stDatalogEvent.CMD == CMD_DATALOG_EVENT))
        {
            if ((stDatalogEvent.UTC >= ttStart) && (stDatalogEvent.UTC <= ttStop))
            {
                // RTOSprintf("\r\n> ### ReSend Data ###");

                GSM_TCP_ReSendEVENT(stDatalogEvent, u8SocketBank);
            }
        }

        u32LogIdx++;

    } while (u32LogIdx < u32LogEnd);

    if (bRTOS_En)
        // Release GSM SD synchronization
        osMutexRelease(mutex_gsm_sd_user);

    RTOSprintf("\r\n> ### Datalog_ResendEvent OK!! ###\r\n");

    return TRUE;
}
#endif

//============================================================================//
// @brief  Check if a record mode is ON or OFF.
// @param  tRecordMode: Input record mode to check.
// @retval TRUE: The input record mode is ON.
//         FALSE: The input record mode is OFF.
//============================================================================//
bool Datalog_IsRecordMode(RecordModeTypeDef tRecordMode)
{
    if (tRecordMode == REC_OFF)
    {
        if (g_Setting.u32RecordMode == RecordModeValue[REC_OFF])
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if (g_Setting.u32RecordMode & RecordModeValue[tRecordMode])
            return TRUE;
        else
            return FALSE;
    }
}

//============================================================================//
// @brief  Write speed interrupt record to SD card.
// @param  None.
// @retval TRUE: if operation successful.
//         FALSE: if operation fail.
//============================================================================//
#if ENABLE_SPEED_INT_REC
bool Datalog_RecordSpeedInt(void)
{
    uint8_t u8MyTime[7];

    if (bRecordSpeedIntFlag == TRUE)
    {
        SecToBCDTime(ttRecordSpeedIntTime, u8MyTime);

        tSpeedIntTemp.u8BCDTime[0] = 0xEE;
        tSpeedIntTemp.u8BCDTime[1] = u8MyTime[4];
        tSpeedIntTemp.u8BCDTime[2] = u8MyTime[5];
        tSpeedIntTemp.u8BCDTime[3] = u8MyTime[6];

        SD_AddBuffToFile(FILENAME_SPEED_INT_FILE, (uint8_t *)&tSpeedIntTemp, sizeof(SpeedIntTypeDef));

        memset(&tSpeedIntTemp, 0x00, sizeof(SpeedIntTypeDef));

        bRecordSpeedIntFlag = FALSE;
    }

    return TRUE;
}
#endif

//============================================================================//
// @brief  Write speed pulse count data to SD card.
// @param  None.
// @retval TRUE: if operation successful.
//         FALSE: if operation fail.
//============================================================================//
#if ENABLE_PULSE_COUNT_REC
bool Datalog_RecordPulseCount(void)
{
    uint8_t u8MyTime[7];

    if (bRecordPulseCntFlag == TRUE)
    {
        SecToBCDTime(ttRecordPulseCntTime, u8MyTime);

        tPulseCntTemp.u8BCDTime[0] = 0xEE;
        tPulseCntTemp.u8BCDTime[1] = u8MyTime[4];
        tPulseCntTemp.u8BCDTime[2] = u8MyTime[5];
        tPulseCntTemp.u8BCDTime[3] = u8MyTime[6];

        SD_AddBuffToFile(FILENAME_PULSE_CNT_FILE, (uint8_t *)&tPulseCntTemp, sizeof(PulseCountTypeDef));

        memset(&tPulseCntTemp, 0x00, sizeof(PulseCountTypeDef));

        bRecordPulseCntFlag = FALSE;
    }

    return TRUE;
}
#endif

//============================================================================//
// @brief  Write speed debug data to SD card.
// @param  None.
// @retval TRUE: if operation successful.
//         FALSE: if operation fail.
//============================================================================//
#if ENABLE_GPS_SPEED_REC
bool Datalog_RecordGPS_Speed(void)
{
    uint8_t u8MyTime[7];

    if (bRecordGPS_SpeedFlag == TRUE)
    {
        SecToBCDTime(ttRecordGPS_SpeedTime, u8MyTime);

        gpsSpdRecTemp.u8BCDTime[0] = 0xEE;
        gpsSpdRecTemp.u8BCDTime[1] = u8MyTime[4];
        gpsSpdRecTemp.u8BCDTime[2] = u8MyTime[5];
        gpsSpdRecTemp.u8BCDTime[3] = u8MyTime[6];

        SD_AddBuffToFile(FILENAME_GPS_SPEED_FILE, (uint8_t *)&gpsSpdRecTemp, sizeof(GPS_SpeedRecTypeDef));

        memset(&gpsSpdRecTemp, 0x00, sizeof(GPS_SpeedRecTypeDef));

        bRecordGPS_SpeedFlag = FALSE;
    }

    return TRUE;
}
#endif

//============================================================================//
// @brief  Process to clear outdated files.
// @param  None.
// @retval None
//============================================================================//
void Datalog_ClearFileProc(void)
{
    bool bRes = TRUE;
    static time_t ttCurrTimeStamp = 0;
    static char cFileDir[32] = {0};
    static char cFilePattern[32] = {0};
    static ClearFileTypedef tClearFileType = CLEAR_FILE_NONE;
    static uint16_t u16ClearFileMax = 100;
    static uint16_t u16ClearFileCnt = 0;

    if ((bSD_ReportErrFlag == TRUE) || (sdRequest.sdCmd == SD_MKFS))
    {
        if (procClearFileStep != PROC_STEP_NULL)
            procClearFileStep = PROC_STEP_NULL;

        return;
    }

    if ((EXTIO_IsMainPowerNormal() == FALSE) || (bSystemTimeReady == FALSE) ||
        (procClearFileStep == PROC_STEP_NULL))
        return;

    switch ((int)procClearFileStep)
    {
    // Step 1: Get timestamp to calculate clear file time
    case PROC_STEP_1:
    {
        RTOSprintf("\r\n> ### BEGIN CLEAR OUTDATED FILES ###");

        ttCurrTimeStamp = System_GetTime();

        tClearFileType = CLEAR_FILE_VDR;

        procClearFileStep = PROC_STEP_2;

        break;
    }

    // Init clearing files parameters
    case PROC_STEP_2:
    {
        time_t ttClearTime = 0;
        struct tm tmTime = {0};

        u16ClearFileCnt = 0;

        memset(cFileDir, 0x00, sizeof(cFileDir));

        memset(cFilePattern, 0x00, sizeof(cFilePattern));

        // Init clear file parameter for each file type
        switch ((int)tClearFileType)
        {
        case CLEAR_FILE_VDR:
        {
            // Calculate VDR files clear time and set VDR file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(VDR_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_VDR);

            strftime(cFilePattern, sizeof(cFilePattern), "D%Y%m*.VDR", &tmTime);

            u16ClearFileMax = VDR_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED VDR FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_DTR:
        {
            // Calculate DTR files clear time and set DTR file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(DTR_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "%Y%m*.DTR", &tmTime);

            u16ClearFileMax = DTR_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED DTR FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_CAN_DEBUG:
        {
            // Calculate CAN Debug files clear time and set CAN Debug file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "CanDebug_%Y%m*.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED CAN DEBUG LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_SPEED_INT:
        {
            // Calculate Speed Int files clear time and set Speed Int file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "SpeedInt_%Y%m*.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED SPEED INT LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_PULSE_CNT:
        {
            // Calculate Pulse Count files clear time and set Pulse Count file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "PulseCount_%Y%m*.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED PULSE COUNT LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_GPS_SPEED:
        {
            // Calculate GPS Speed files clear time and set Pulse Count file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "GPS_Speed_%Y%m*.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED GPS SPEED LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_GPRS_LOG:
        {
            // Calculate GPRS Log files clear time and set Pulse Count file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "GPRS_Log_%Y%m*.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED GPRS LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        case CLEAR_FILE_AGAIN_LOG:
        {
            // Calculate GPRS Log files clear time and set Pulse Count file pattern
            ttClearTime = ttCurrTimeStamp - DAY_2_SECOND(LOG_RETAIN_PERIOD + MONTH_2_DAYS_MAX);

            localtime_r(&ttClearTime, &tmTime);

            sprintf(cFileDir, "%s", (char *)FOLDERNAME_DTR);

            strftime(cFilePattern, sizeof(cFilePattern), "AGAIN_Log_%Y%m%d.tmp", &tmTime);

            u16ClearFileMax = LOG_CLEAR_FILES_MAX;

            LOGprintf("\r\n> ### BEGIN CLEAR OUTDATED AGAIN LOG FILES IN %02d/%04d ###",
                      tmTime.tm_mon + 1, tmTime.tm_year + 1900U);

            break;
        }

        default:
        {
            procClearFileStep = PROC_STEP_NULL;

            return;
        }
        }

        procClearFileStep = PROC_STEP_3;
    }

    // Step 3: Clear outdated VDR files
    case PROC_STEP_3:
    {
        char cFileName[64] = {0},
             cClearFileName[96] = {0};

        if ((SD_FindFilePattern(cFileDir, cFilePattern, cFileName) == FALSE) ||
            (u16ClearFileCnt >= u16ClearFileMax))
        {
            u16ClearFileCnt = 0;

            if (tClearFileType >= CLEAR_FILE_GPRS_LOG)
            {
                RTOSprintf("\r\n> ### CLEAR OUTDATED FILES COMPLETE ###");

                procClearFileStep = PROC_STEP_NULL;

                break;
            }

            tClearFileType++;

            procClearFileStep = PROC_STEP_2;

            break;
        }

        memset(cClearFileName, 0x00, sizeof(cClearFileName));

        if (strlen(cFileDir))
            sprintf(cClearFileName, "%s/%s", cFileDir, cFileName);
        else
            sprintf(cClearFileName, "%s", cFileName);

        for (int i = 0; i < 3; i++)
        {
            bRes = SD_RemoveFile(cClearFileName);

            if (bRes)
                break;
        }

        LOGprintf("\r\n> Clear file %s OK", cClearFileName);

        u16ClearFileCnt++;

        break;
    }

    default:
        break;
    }
}
