/*
 * USER_Protocol.c
 *
 *  Terminal commands. See "@QC+HELP" or README.md.
 */
/*
 * OVERVIEW
 *   Interprets one complete terminal line such as "@QC+PID=RR,0.7,0.5,0.015".
 *
 *   Call chain (all in UARTTask, every 10 ms):
 *     USART1 RX interrupt -> ring buffer (USER_UART_Driver.c)
 *     UART_TerminalProc() finds "@QC+" ... "\r\n" in the ring buffer
 *     -> Protocl_ChkCmdPkg() -> Protocl_ProcCmd() -> handler from s_cmd_table
 *     -> prints "@ Command OK !!" or "@ Command ERROR !!".
 *
 *   Command syntax:
 *     @QC+NAME            no argument            -> handler(args = NULL)
 *     @QC+NAME=a,b,c      arguments after '='    -> handler(args = "a,b,c")
 *     @QC+NAME?           query                  -> handler(args = "?")
 *   Handlers split the argument text with Cmd_Split() and convert the parts
 *   with atoi()/strtof().
 *
 *   Command table pattern: instead of a long if/else chain on strings, every
 *   command is one line in s_cmd_table {name, handler function, help text}.
 *   Protocl_ProcCmd() just searches the table; @QC+HELP prints it.
 *
 *   How to add a new command (example "@QC+FOO=<n>"):
 *     1. Write a handler:  static bool Cmd_Foo(char *args) { ... return true; }
 *        Return true = "Command OK", false = "Command ERROR".
 *     2. Add one line to s_cmd_table: { "FOO", Cmd_Foo, "FOO=<n> does ..." },
 *     3. Done: parsing, HELP and the OK/ERROR reply are automatic.
 *   Keep handlers short: they run in UARTTask, and long work there delays the
 *   telemetry stream (the flight loop is not affected, it has its own task).
 */

#include "main.h"
#include "USER_Protocol.h"
#include "USER_System.h"
#include "USER_DataCollect.h"
#include "USER_GPS.h"

#define STR_CMD_OK  "\r\n@ Command OK !!\r\n"
#define STR_CMD_ERR "\r\n@ Command ERROR !!\r\n"

#define CMD_MAX_ARGS 5

/* Optional reply text copied out by Protocl_ChkCmdPkg() (legacy; the current
 * handlers print directly with RTOSprintf and leave it empty). */
char szCmdMsg[UART_BASE_BUFFER_SIZE];

/* Handler signature: args = text after '=' (modifiable), "?" or NULL.
 * Return true if the command was accepted. */
typedef bool (*CmdHandler)(char *args);

/* Build variants in which a command is useful (bit mask, for LIST / HELP) */
#define CMD_V_RC    (1U << 0)                     /* FC_VARIANT_RC_FULL   */
#define CMD_V_UART  (1U << 1)                     /* FC_VARIANT_UART_TEST */
#define CMD_V_PAD   (1U << 2)                     /* FC_VARIANT_GAMEPAD   */
#define CMD_V_ALL   (CMD_V_RC | CMD_V_UART | CMD_V_PAD)

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
#define CMD_V_THIS  CMD_V_RC
#elif (FC_VARIANT == FC_VARIANT_UART_TEST)
#define CMD_V_THIS  CMD_V_UART
#else
#define CMD_V_THIS  CMD_V_PAD
#endif

typedef struct
{
    const char *name;    /* text after "@QC+" up to '=' / '?' / end */
    CmdHandler  handler;
    const char *help;    /* one line, printed by HELP                       */
    const char *usage;   /* full syntax + examples, '\n' separated (LIST)   */
    uint8_t     variants;/* CMD_V_* mask of builds where the command applies */
} CmdEntry;

/* ------------------------------------------------------------------------- */
/* Helpers                                                                   */
/* ------------------------------------------------------------------------- */

/* Split "a,b,c" in place. Returns number of arguments. */
/* "In place" = every ',' is replaced by '\0' and argv[] points into args.
 * Example: args "RR,0.7,0.5" -> "RR\0 0.7\0 0.5", argv = {"RR","0.7","0.5"},
 * returns 3. Empty fields are kept ("1,,2" -> 3 args, middle one ""). */
static int Cmd_Split(char *args, char *argv[], int max)
{
    int argc = 0;

    if ((args == NULL) || (*args == '\0'))
        return 0;

    while ((argc < max) && (args != NULL))
    {
        argv[argc++] = args;
        args = strchr(args, ',');
        if (args != NULL)
            *args++ = '\0';
    }
    return argc;
}

/* Print "@ <msg>" or "@ REFUSED: <msg>" (msg usually comes from USER_System) */
static void Cmd_Reply(bool ok, const char *msg)
{
    RTOSprintf("\r\n@ %s%s", ok ? "" : "REFUSED: ", msg);
}

/* ------------------------------------------------------------------------- */
/* Handlers                                                                  */
/* ------------------------------------------------------------------------- */
/* The handlers never touch flight state directly: they call request
 * functions of USER_System.c (Sys_RequestArm, Sys_MotorTest, ...), which
 * check the safety conditions and return a reason text. */
static bool Cmd_Help(char *args);   /* forward declaration: Cmd_Help needs s_cmd_table, defined below */

static bool Cmd_Status(char *args)
{
    (void)args;
    Debug_PrintStatus();
    return true;
}

static bool Cmd_Arm(char *args)
{
    const char *reason;
    bool ok;

    (void)args;
    ok = Sys_RequestArm(&reason);
    Cmd_Reply(ok, reason);
    return ok;
}

static bool Cmd_Disarm(char *args)
{
    (void)args;
    Sys_RequestDisarm("Disarmed by command");
    Cmd_Reply(true, "Disarm requested");
    return true;
}

static bool Cmd_Calib(char *args)
{
    const char *reason;
    bool ok;

    (void)args;
    ok = Sys_RequestCalibration(&reason);
    Cmd_Reply(ok, reason);
    return ok;
}

static bool Cmd_Level(char *args)
{
    const char *reason;
    bool ok;

    (void)args;
    ok = Sys_RequestLevel(&reason);
    Cmd_Reply(ok, reason);
    return ok;
}

/* @QC+SP=<throttle_us>,<roll_deg>,<pitch_deg>,<yaw_rate_dps>
 * Only accepted in the UART TEST build (control source FC_SRC_UART). */
static bool Cmd_Setpoint(char *args)
{
    char *argv[CMD_MAX_ARGS];
    int argc = Cmd_Split(args, argv, CMD_MAX_ARGS);

    if (argc != 4)
    {
        RTOSprintf("\r\n@ SP=<throttle_us>,<roll_deg>,<pitch_deg>,<yaw_rate_dps>");
        return false;
    }
    if (Sys_GetStatus()->source != FC_SRC_UART)
    {
        Cmd_Reply(false, "Only in the UART TEST build (FC_VARIANT)");
        return false;
    }

    Sys_SetUartSetpoint(strtof(argv[0], NULL), strtof(argv[1], NULL),
                        strtof(argv[2], NULL), strtof(argv[3], NULL));
    Cmd_Reply(true, "Setpoint updated");
    return true;
}

static void Cmd_PrintPid(PID_IdTypeDef id)
{
    PID_TypeDef *pid = Sys_GetPid(id);

    RTOSprintf("\r\n@ PID %s: Kp=%.4f Ki=%.4f Kd=%.4f (out limit %.0f, I limit %.0f)",
               Sys_PidName(id), pid->kp, pid->ki, pid->kd, pid->out_limit, pid->i_limit);
}

/* @QC+PID? (or @QC+PID)       : print all PID gains
 * @QC+PID=RR                  : print one controller
 * @QC+PID=RR,0.7,0.5,0.015    : set kp, ki, kd of one controller (RAM only)
 * Names: RA/PA = roll/pitch angle loop, RR/PR/YR = roll/pitch/yaw rate loop.
 * Negative gains are refused. */
static bool Cmd_Pid(char *args)
{
    char *argv[CMD_MAX_ARGS];
    int argc;
    int id;

    if ((args == NULL) || (*args == '\0') || (*args == '?'))
    {
        for (id = 0; id < PID_ID_COUNT; id++)
            Cmd_PrintPid((PID_IdTypeDef)id);
        return true;
    }

    argc = Cmd_Split(args, argv, CMD_MAX_ARGS);
    for (id = 0; id < PID_ID_COUNT; id++)
    {
        if (strcmp(argv[0], Sys_PidName((PID_IdTypeDef)id)) == 0)
            break;
    }
    if (id >= PID_ID_COUNT)
    {
        RTOSprintf("\r\n@ PID=<RA|PA|RR|PR|YR>,<kp>,<ki>,<kd>");
        return false;
    }

    if (argc == 4)
    {
        float kp = strtof(argv[1], NULL);
        float ki = strtof(argv[2], NULL);
        float kd = strtof(argv[3], NULL);

        if ((kp < 0.0f) || (ki < 0.0f) || (kd < 0.0f))
            return false;

        PID_SetTunings(Sys_GetPid((PID_IdTypeDef)id), kp, ki, kd);
    }
    else if (argc != 1)
    {
        return false;
    }

    Cmd_PrintPid((PID_IdTypeDef)id);
    return true;
}

/* @QC+CONTROL_MOTOR=<motor 1..4 or 5 = all>,<pulse us>. Without arguments it
 * prints the usage and returns OK. Limits are checked in Sys_MotorTest(). */
static bool Cmd_ControlMotor(char *args)
{
    char *argv[CMD_MAX_ARGS];
    int argc = Cmd_Split(args, argv, CMD_MAX_ARGS);
    const char *reason;
    bool ok;

    if (argc != 2)
    {
        RTOSprintf("\r\n@ CONTROL_MOTOR=<motor 1..4, 5=all>,<pulse_us %u..%u>",
                   (unsigned)MOTOR_STOP_US, (unsigned)MOTOR_TEST_MAX_US);
        RTOSprintf("\r\n@   only when DISARMED. REMOVE PROPELLERS!");
        return (argc == 0);
    }

    ok = Sys_MotorTest((uint8_t)atoi(argv[0]), (uint16_t)atoi(argv[1]), &reason);
    Cmd_Reply(ok, reason);
    return ok;
}

static bool Cmd_StopMotor(char *args)
{
    (void)args;
    Sys_MotorTestStop();
    Cmd_Reply(true, "All motor test outputs stopped");
    return true;
}

static bool Cmd_GpTest(char *args)
{
    (void)args;
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    return Gamepad_SelfTest();
#else
    Cmd_Reply(false, "Only in the GAMEPAD build (FC_VARIANT 3)");
    return false;
#endif
}

/* @QC+PADLOG=1 / =0 : turn the gamepad event log on/off (see
 * Debug_PadEvents in USER_DataCollect.c). @QC+PADLOG? prints the state. */
static bool Cmd_PadLog(char *args)
{
#if (FC_VARIANT == FC_VARIANT_GAMEPAD)
    if ((args != NULL) && ((*args == '0') || (*args == '1')))
        Debug_SetPadLog(*args == '1');
    else if ((args != NULL) && (*args != '\0') && (*args != '?'))
        return false;
    RTOSprintf("\r\n@ Gamepad event log: %s", Debug_GetPadLog() ? "ON" : "OFF");
    return true;
#else
    (void)args;
    Cmd_Reply(false, "Only in the GAMEPAD build (FC_VARIANT 3)");
    return false;
#endif
}

/* @QC+DEBUG=<n> selects the telemetry stream (modes: USER_DataCollect.c).
 * @QC+DEBUG? prints the list and the current mode. */
static bool Cmd_PadTest(char *args)
{
    const char *reason;
    bool ok;

    if ((args == NULL) || (*args == '\0') || (*args == '?'))
    {
        RTOSprintf("\r\n@ PADTEST: %s", Sys_GetStatus()->stick_test ? "ON" : "OFF");
        return true;
    }
    if ((*args != '0') && (*args != '1'))
        return false;

    ok = Sys_StickTest(*args == '1', &reason);
    Cmd_Reply(ok, reason);
    return ok;
}

/* @QC+POSHOLD=1|0|? : GPS position hold (needs GPS_ENABLE = 1, ARMED) */
static bool Cmd_PosHold(char *args)
{
    const char *reason;
    const FC_StatusTypeDef *st = Sys_GetStatus();
    bool ok;

    if ((args == NULL) || (*args == '\0') || (*args == '?'))
    {
        RTOSprintf("\r\n@ POSHOLD: %s%s", st->poshold_mode ? "ON" : "OFF",
                   st->poshold_engaged ? " (holding)" : "");
        return true;
    }
    if ((*args != '0') && (*args != '1'))
        return false;
    ok = Sys_RequestPosHold(*args == '1', &reason);
    Cmd_Reply(ok, reason);
    return ok;
}

/* @QC+GPS? : one status line of the GPS / compass */
static bool Cmd_Gps(char *args)
{
    (void)args;
    Debug_PrintGps();
    return true;
}

/* @QC+MAGCAL : 30 s compass hard-iron calibration (turn the quad around all
 * axes while it runs). Not while ARMED. */
static bool Cmd_MagCal(char *args)
{
    (void)args;
#if (GPS_ENABLE == 1) && (GPS_HEADING_SOURCE == 0)
    if (Sys_GetStatus()->state == FC_STATE_ARMED)
    {
        Cmd_Reply(false, "Disarm first");
        return false;
    }
    Gps_MagCalStart();
    Cmd_Reply(true, "MAGCAL 30 s: rotate the quad slowly through every direction (roll, pitch, yaw)");
    return true;
#else
    Cmd_Reply(false, "needs GPS_ENABLE 1 and GPS_HEADING_SOURCE 0");
    return false;
#endif
}

static bool Cmd_List(char *args);

static bool Cmd_Debug(char *args)
{
    if ((args == NULL) || (*args == '\0') || (*args == '?'))
    {
        RTOSprintf("\r\n@ DEBUG=<n>  0:off 1:attitude 2:RC 3:motors 4:PID roll/pitch 5:PID yaw 6:$TLM (drone_viewer.py)");
        RTOSprintf("\r\n@ current = %d", (int)Debug_GetMode());
        return true;
    }
    if (!Debug_SetMode((uint8_t)atoi(args)))
        return false;
    Cmd_Reply(true, "Debug mode set");
    return true;
}

/* The command table. Names must be UPPER CASE and are matched exactly
 * (strcmp, case sensitive). Add new commands here. */
static const CmdEntry s_cmd_table[] =
{
    { "HELP", Cmd_Help, "Short list; HELP=<NAME> for details",
      "HELP            one line per command\n"
      "HELP=<NAME>     full syntax of one command   e.g. @QC+HELP=PID", CMD_V_ALL },
    { "LIST", Cmd_List, "All commands with syntax and examples",
      "LIST            every command, syntax, parameters, examples, builds", CMD_V_ALL },
    { "STATUS", Cmd_Status, "Print state, attitude, inputs, motors",
      "STATUS          state + last event, attitude, gyro calibration,\n"
      "                RC / gamepad, motor pulses, loop dt, IMU errors", CMD_V_ALL },
    { "ARM", Cmd_Arm, "Arm (UART TEST / GAMEPAD build; RC uses CH5)",
      "ARM             needs: DISARMED, gyro calibrated, tilt < 25 deg, throttle low\n"
      "                GAMEPAD: pad must be connected (same as holding START);\n"
      "                throttle = left stick, B = disarm, unplug = failsafe\n"
      "                UART: after a FAILSAFE the first ARM only clears it", CMD_V_UART | CMD_V_PAD },
    { "DISARM", Cmd_Disarm, "Disarm immediately",
      "DISARM          stop motors now (any build, any time); also ends PADTEST", CMD_V_ALL },
    { "CALIB", Cmd_Calib, "Recalibrate gyro (keep still)",
      "CALIB           new gyro bias, keep the frame still ~2 s (not while ARMED)", CMD_V_ALL },
    { "LEVEL", Cmd_Level, "Use current attitude as level (RAM only)",
      "LEVEL           frame on a flat surface -> roll/pitch become 0 (lost at reset)", CMD_V_ALL },
    { "SP", Cmd_Setpoint, "SP=<thr_us>,<roll>,<pitch>,<yawrate> (UART TEST build)",
      "SP=<thr>,<roll>,<pitch>,<yaw>\n"
      "                thr 1000..1300 us, roll/pitch +-30 deg, yaw +-150 deg/s\n"
      "                e.g. @QC+SP=1250,10,0,0  (roll 10 deg right)", CMD_V_UART },
    { "PID", Cmd_Pid, "PID? | PID=<RA|PA|RR|PR|YR>,<kp>,<ki>,<kd>",
      "PID?            print all gains\n"
      "PID=<ID>        print one: RA PA = angle roll/pitch, RR PR YR = rate\n"
      "PID=<ID>,<kp>,<ki>,<kd>   set (RAM only)  e.g. @QC+PID=RR,0.7,0.5,0.015", CMD_V_ALL },
    { "CONTROL_MOTOR", Cmd_ControlMotor, "CONTROL_MOTOR=<1..4|5>,<us> (disarmed, no props)",
      "CONTROL_MOTOR=<m>,<us>   m = 1..4 (M1 FL, M2 BL, M3 FR, M4 BR) or 5 = all\n"
      "                us 1000..1500, only DISARMED / CALIBRATING. PROPS OFF\n"
      "                e.g. @QC+CONTROL_MOTOR=1,1150", CMD_V_ALL },
    { "STOP_MOTOR", Cmd_StopMotor, "Stop all motor test outputs",
      "STOP_MOTOR      all test outputs back to 1000 us (also ends PADTEST)", CMD_V_ALL },
    { "DEBUG", Cmd_Debug, "DEBUG=<0..6> telemetry stream",
      "DEBUG=<n>       20 Hz stream: 0 off, 1 attitude, 2 RC / pad inputs,\n"
      "                3 motors, 4 PID roll+pitch, 5 PID yaw, 6 $TLM (drone_viewer)\n"
      "DEBUG?          current mode", CMD_V_ALL },
    { "GPTEST", Cmd_GpTest, "Gamepad decoder self test (GAMEPAD build)",
      "GPTEST          HID / XInput / Switch decoders on sample data, PASS/FAIL\n"
      "                unplug the pad first", CMD_V_PAD },
    { "PADLOG", Cmd_PadLog, "PADLOG=1|0 print gamepad buttons/sticks on change",
      "PADLOG=1|0      print pad buttons and stick moves when they change\n"
      "PADLOG?         show ON/OFF", CMD_V_PAD },
    { "PADTEST", Cmd_PadTest, "PADTEST=1|0 sticks drive the motors (no IMU, no arm)",
      "PADTEST=1       stick -> motor test, open loop. PROPELLERS OFF!\n"
      "                left Y = throttle (held), left X = yaw, right = roll/pitch\n"
      "                motors stay 1000 us until throttle > 1050, max 1500 us\n"
      "                stop: PADTEST=0, B/BACK, DISARM, STOP_MOTOR, unplug pad\n"
      "PADTEST?        show ON/OFF", CMD_V_PAD },
    { "POSHOLD", Cmd_PosHold, "POSHOLD=1|0 GPS position hold (GPS_ENABLE 1, armed)",
      "POSHOLD=1       hold the position with GPS (pad: RB toggles). Needs ARMED,\n"
      "                throttle up, 3D fix >= GPS_MIN_SATS, heading (compass)\n"
      "                throttle and yaw stay manual; roll/pitch stick = pilot\n"
      "POSHOLD=0       off      POSHOLD?  show state", CMD_V_ALL },
    { "GPS", Cmd_Gps, "GPS? fix, satellites, position, heading",
      "GPS?            fix, sats, HDOP, lat/lon, N/E metres from home, speed,\n"
      "                heading (compass), NMEA counters", CMD_V_ALL },
    { "MAGCAL", Cmd_MagCal, "Compass calibration, 30 s (rotate the quad)",
      "MAGCAL          hard-iron offsets of the QMC5883L, kept in RAM", CMD_V_ALL },
};

#define CMD_TABLE_SIZE (sizeof(s_cmd_table) / sizeof(s_cmd_table[0]))

/* Builds a command applies to, e.g. "RC FULL, GAMEPAD" (static buffer) */
static const char *Cmd_VariantText(uint8_t v)
{
    static char buf[40];

    buf[0] = '\0';
    if (v == CMD_V_ALL)
        return "all builds";
    if (v & CMD_V_RC)   strcat(buf, "RC FULL ");
    if (v & CMD_V_UART) strcat(buf, "UART TEST ");
    if (v & CMD_V_PAD)  strcat(buf, "GAMEPAD ");
    if (buf[0] != '\0')
        buf[strlen(buf) - 1U] = '\0';             /* drop the trailing space */
    return buf;
}

/* Print the full usage of one table entry, one terminal line per '\n' */
static void Cmd_PrintUsage(const CmdEntry *e)
{
    const char *p = e->usage;

    RTOSprintf("\r\n@ %s%s  [%s]%s", CMD_HEAD_STD, e->name, Cmd_VariantText(e->variants),
               (e->variants & CMD_V_THIS) ? "" : "  (not in this build)");
    while (*p != '\0')
    {
        const char *nl = strchr(p, '\n');
        int len = (nl != NULL) ? (int)(nl - p) : (int)strlen(p);

        RTOSprintf("\r\n@     %.*s", len, p);
        p += len;
        if (*p == '\n')
            p++;
    }
}

/* HELP: one line per command. HELP=<NAME>: full usage of that command. */
static bool Cmd_Help(char *args)
{
    size_t i;

    if ((args != NULL) && (*args != '\0') && (*args != '?'))
    {
        for (i = 0; i < CMD_TABLE_SIZE; i++)
        {
            if (strcmp(args, s_cmd_table[i].name) == 0)
            {
                Cmd_PrintUsage(&s_cmd_table[i]);
                return true;
            }
        }
        RTOSprintf("\r\n@ Unknown command '%s'. Send %sLIST", args, CMD_HEAD_STD);
        return false;
    }

    for (i = 0; i < CMD_TABLE_SIZE; i++)
        RTOSprintf("\r\n@ %s%-14s %s", CMD_HEAD_STD, s_cmd_table[i].name, s_cmd_table[i].help);
    RTOSprintf("\r\n@ Details: %sHELP=<NAME> or %sLIST", CMD_HEAD_STD, CMD_HEAD_STD);
    return true;
}

/* LIST: every command with full syntax, examples and the builds it applies to */
static bool Cmd_List(char *args)
{
    size_t i;

    (void)args;
    RTOSprintf("\r\n@ ===== %u commands, this build: %s =====", (unsigned)CMD_TABLE_SIZE,
               FC_VARIANT_NAME);
    RTOSprintf("\r\n@ Format: %s<NAME>[=args | ?] then CR LF (115200 8N1)", CMD_HEAD_STD);
    for (i = 0; i < CMD_TABLE_SIZE; i++)
        Cmd_PrintUsage(&s_cmd_table[i]);
    RTOSprintf("\r\n@ ===== end of list =====");
    return true;
}

//============================================================================//
// @brief  Process an input command.
// @param  pStr: Pointer to a string which stores the command.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
/*
 * Parsing walk-through for pStr = "@QC+PID=RR,0.7,0.5,0.015\r\n":
 *   1. StopChar cuts the line at the first '\n' and '\r'
 *      -> "@QC+PID=RR,0.7,0.5,0.015"
 *   2. check the "@QC+" prefix, name -> "PID=RR,0.7,..."
 *   3. strpbrk finds '=', it becomes '\0': name = "PID", args = "RR,0.7,..."
 *      (for "@QC+PID?" the '?' becomes '\0' and args = "?")
 *   4. look "PID" up in s_cmd_table and call Cmd_Pid(args).
 * pStr is modified in place.
 */
bool Protocl_ProcCmd(char *pStr)
{
    char *name;
    char *args;
    size_t i;

    StopChar(pStr, '\n');
    StopChar(pStr, '\r');

    memset(szCmdMsg, 0x00, sizeof(szCmdMsg));

    if (strncmp(pStr, CMD_HEAD_STD, strlen(CMD_HEAD_STD)) != 0)
        return false;

    name = pStr + strlen(CMD_HEAD_STD);

    /* Arguments start after '=' ; "NAME?" is a query with args "?" */
    args = strpbrk(name, "=?");
    if (args != NULL)
    {
        if (*args == '=')
        {
            *args++ = '\0';
        }
        else
        {
            *args = '\0';
            args = "?";
        }
    }

    for (i = 0; i < CMD_TABLE_SIZE; i++)
    {
        if (strcmp(name, s_cmd_table[i].name) == 0)
            return s_cmd_table[i].handler(args);
    }

    RTOSprintf("\r\n@ Unknown command. Send %sHELP", CMD_HEAD_STD);
    return false;
}

//============================================================================//
// @brief  Process the command related to command testing.
// @param  pcCmdPkg: Pointer to a string storing a RS232 command.
//         pcCmdMsg: Pointer to a string buffer to store the returned message.
// @retval TRUE: Command is processed successfully.
//         FALSE: Command processing is incompleted.
//============================================================================//
/* Entry point used by UART_TerminalProc(): run the command, then print the
 * final "@ Command OK !!" / "@ Command ERROR !!" line (host tools such as
 * drone_viewer.py can wait for it). */
bool Protocl_ChkCmdPkg(char *pcCmdPkg, char *pcCmdMsg)
{
    bool bResult;

    bResult = Protocl_ProcCmd(pcCmdPkg);

    if (pcCmdMsg != NULL)
    {
        strcpy(pcCmdMsg, szCmdMsg);
    }

    if (bResult == true)
        RTOSprintf(STR_CMD_OK);
    else
        RTOSprintf(STR_CMD_ERR);

    return bResult;
}
