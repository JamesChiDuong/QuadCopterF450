/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_ll_spi.h"
// #include "Buzzer.h"
#include "Config.h"
#include "Default.h"
#include "DataLog.h"
#include "DataCollect.h"
#include "DWT_Delay.h"
#include "EXTIO.h"
#include "GPS_Driver.h"
#include "GSM_Driver.h"
#include "LCD_Key.h"
#if ENABLE_LSM6DSL
#include "LSM6DSL.h"
#endif
#include "MG82_Driver.h"
#include "MX25L_Driver.h"
#if ENABLE_RX8130
#include "RX8130_Driver.h"
#else
#include "PCF_Driver.h"
#endif
#include "PRN_Driver.h"
#include "Protocol.h"
// #include "RingBuf.h"
#include "RuleV161.h"
#include "SD_Module.h"
#include "System.h"
#include "UART_Driver.h"
#include "UART_ExtDriver.h"
#include "USB_Module.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define APPLICATION_BEGIN 4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

IWDG_HandleTypeDef hiwdg;

RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio_rx;
DMA_HandleTypeDef hdma_sdio_tx;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

osThreadId defaultTaskHandle;
osThreadId taskLogProcHandle;
osThreadId taskEXTIO_ProcHandle;
osThreadId taskGPS_ProcHandle;
osThreadId taskGSM_ProcHandle;
osThreadId taskLCD_KeyHandle;
osThreadId taskRS232_ProcHandle;
osThreadId taskHighLoadHandle;
osThreadId taskSD_ProcHandle;
osThreadId taskGSM_RXHandle;
osThreadId taskGSM_TXHandle;
osMessageQId GSM_BlockCpltHandle;
osMessageQId GSM_NonBlockCpltHandle;
osMessageQId GSM_NonBlockResHandle;
osMessageQId GSM_NonBlockResCodeHandle;
osTimerId timerRTOS_GSM_BlockHandle;
osTimerId timerRTOS_GSM_NonBlockHandle;
osTimerId timerRTOS_EXTIHandle;
osTimerId timerRTOS_PCTransHandle;
osTimerId timerACC_OffHandle;
osMutexId mutex_ttnowHandle;
osMutexId mutex_i2c1Handle;
osMutexId mutex_i2c2Handle;
osMutexId mutex_spi1Handle;
osMutexId mutex_spi2Handle;
osMutexId mutex_uart1Handle;
osMutexId mutex_uart2Handle;
osMutexId mutex_uart3Handle;
osMutexId mutex_uart4Handle;
osMutexId mutex_uart5Handle;
osMutexId mutex_lcdHandle;
osMutexId mutex_prnHandle;
osMutexId mutex_settingHandle;
osMutexId mutex_gsm_blockHandle;
osMutexId mutex_gsm_nonblockHandle;
osMutexId mutex_gsm_rxHandle;
osMutexId mutex_gsm_sdHandle;
osMutexId mutex_usbHandle;
osMutexId mutex_usb_sdHandle;
osMutexId mutex_sd_procHandle;
osMutexId mutex_data_buf1Handle;
osMutexId mutex_data_buf2Handle;
osMutexId mutex_data_buf3Handle;
osMutexId mutex_mallocHandle;
osMutexId mutex_upload_chkHandle;
osMutexId mutex_uart1extHandle;
osMutexId mutex_uart2extHandle;
osMutexId mutex_uart3extHandle;
osMutexId mutex_uart4extHandle;
/* USER CODE BEGIN PV */
UART_HandleTypeDef *puartHandle[5];

osMutexId mutex_i2c1_user;
osMutexId mutex_i2c2_user;
osMutexId mutex_spi1_user;
osMutexId mutex_spi2_user;
osMutexId mutex_uart_user[5];
osMutexId mutex_lcd_user;
osMutexId mutex_prn_user;
osMutexId mutex_ttnow_user;
osMutexId mutex_setting_user;
osMutexId mutex_gsm_block_user;
osMutexId mutex_gsm_nonblock_user;
osMutexId mutex_gsm_rx_user;
osMutexId mutex_gsm_sd_user;
osMutexId mutex_usb_user;
osMutexId mutex_usb_sd_user;
osMutexId mutex_sd_proc_user;
osMutexId mutex_data_buf1_user;
osMutexId mutex_data_buf2_user;
osMutexId mutex_data_buf3_user;
osMutexId mutex_malloc_user;
osMutexId mutex_upload_chk_user;
osMutexId mutex_uartext_user[4];

/*User variable*/
I2C_HandleTypeDef *hi2c;

osMutexId *mutex_i2c_user;

bool bRTOS_En = FALSE;

bool bSysInitFinish = FALSE;

uint16_t u16SubSecCnt = 0;

uint8_t u8SysCurrSec = 0, u8SysOldSec = 0;

uint8_t u8SysCurrMin = 0, u8SysOldMin = 0;

bool bOneMinFlag = FALSE;

bool bSecProcFlag = FALSE;

bool bDVRSecProcFlag = FALSE;

bool bFirstLogFlag = FALSE;

// bool bMidNightFlag = FALSE;

bool bIsCheckVDRProcFlag = FALSE;

#if ENABLE_GSM_NET
uint8_t u8DatalogIdx = 0;

bool bDataLogFlag[MAX_SOCKET_BANK] = {FALSE};

time_t ttDataLogTime[MAX_SOCKET_BANK],
    ttDataLogTimeOld[MAX_SOCKET_BANK] = {0};

bool bFirstACC = TRUE;
#endif

#if ENABLE_SPEED_INT_REC
bool bSpeedIntFlag = FALSE;
#endif

#if ENABLE_PULSE_COUNT_REC
uint32_t u32TIM2_Cnt = 0;
#endif

time_t ttSysLogTime;

struct tm tmRTCTimeInfoTemp = {0};

uint8_t u8DebugTime[7] = {0}, u8DebugTimeTemp[7] = {0};

uint8_t u8SysOldMinTemp;

bool bLogTimeErrFlag = FALSE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN1_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_IWDG_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const *argument);
void Start_taskLogProc(void const *argument);
void Start_taskEXTIO_Proc(void const *argument);
void Start_taskGPS_Proc(void const *argument);
void Start_taskGSM_Proc(void const *argument);
void Start_taskLCD_Key(void const *argument);
void Start_taskRS232_Proc(void const *argument);
void Start_taskHighLoad(void const *argument);
void Start_taskSD_Proc(void const *argument);
void Start_taskGSM_RX(void const *argument);
void Start_taskGSM_TX(void const *argument);
void timerRTOS_GSM_BlockCallback(void const *argument);
void timerRTOS_GSM_NonBlockCallback(void const *argument);
void timerRTOS_EXTI_Callback(void const *argument);
void timerRTOS_PCTransCallBack(void const *argument);
void timerACC_OffCallBack(void const *argument);

/* USER CODE BEGIN PFP */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void USER_PeriphInit(void);

void USER_I2C2_Init(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    // Remap vector table address for application at flash address
    // different than 0
    // If download application to flash directly
    // then comment out this line
    SCB->VTOR = APPLICATION_ADDR;

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    // System_EnterStandbyMode();
    /* USER CODE BEGIN Init */

    HAL_PWR_EnableBkUpAccess();

    if (HW_ReadBackupRegister(BKP_SLEEP_CHK) == BKP_SLEEP_VALUE)
    {
        HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_RST_VALUE);

        System_EnterStandbyMode();
    }
    else if (HW_ReadBackupRegister(RDP_DISABLE_CHK) == BKP_RDP_OFF_VALUE)
    {
        HW_WriteBackupRegister(RDP_DISABLE_CHK, BKP_RST_VALUE);

        System_ClrRDP();
    }

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_CAN1_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_SDIO_SD_Init();
    MX_SPI1_Init();
    MX_UART4_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_FATFS_Init();
    MX_IWDG_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    bRTOS_En = FALSE;

    puartHandle[UART_PORT1] = &huart1;
    puartHandle[UART_PORT2] = &huart2;
    puartHandle[UART_PORT3] = &huart3;
    puartHandle[UART_PORT4] = &huart4;

    System_Init();

    RTOSprintf("\r\n> ### FIRMWARE VERSION: %s%s ###", (char *)FIRMWARE_VERSION, (char *)FW_VERSION_CHK);

    System_ChkHardfault();

    /* USER CODE END 2 */

    /* Create the mutex(es) */
    /* definition and creation of mutex_ttnow */
    osMutexDef(mutex_ttnow);
    mutex_ttnowHandle = osMutexCreate(osMutex(mutex_ttnow));

    /* definition and creation of mutex_i2c1 */
    osMutexDef(mutex_i2c1);
    mutex_i2c1Handle = osMutexCreate(osMutex(mutex_i2c1));

    /* definition and creation of mutex_i2c2 */
    osMutexDef(mutex_i2c2);
    mutex_i2c2Handle = osMutexCreate(osMutex(mutex_i2c2));

    /* definition and creation of mutex_spi1 */
    osMutexDef(mutex_spi1);
    mutex_spi1Handle = osMutexCreate(osMutex(mutex_spi1));

    /* definition and creation of mutex_spi2 */
    osMutexDef(mutex_spi2);
    mutex_spi2Handle = osMutexCreate(osMutex(mutex_spi2));

    /* definition and creation of mutex_uart1 */
    osMutexDef(mutex_uart1);
    mutex_uart1Handle = osMutexCreate(osMutex(mutex_uart1));

    /* definition and creation of mutex_uart2 */
    osMutexDef(mutex_uart2);
    mutex_uart2Handle = osMutexCreate(osMutex(mutex_uart2));

    /* definition and creation of mutex_uart3 */
    osMutexDef(mutex_uart3);
    mutex_uart3Handle = osMutexCreate(osMutex(mutex_uart3));

    /* definition and creation of mutex_uart4 */
    osMutexDef(mutex_uart4);
    mutex_uart4Handle = osMutexCreate(osMutex(mutex_uart4));

    /* definition and creation of mutex_uart5 */
    osMutexDef(mutex_uart5);
    mutex_uart5Handle = osMutexCreate(osMutex(mutex_uart5));

    /* definition and creation of mutex_lcd */
    osMutexDef(mutex_lcd);
    mutex_lcdHandle = osMutexCreate(osMutex(mutex_lcd));

    /* definition and creation of mutex_prn */
    osMutexDef(mutex_prn);
    mutex_prnHandle = osMutexCreate(osMutex(mutex_prn));

    /* definition and creation of mutex_setting */
    osMutexDef(mutex_setting);
    mutex_settingHandle = osMutexCreate(osMutex(mutex_setting));

    /* definition and creation of mutex_gsm_block */
    osMutexDef(mutex_gsm_block);
    mutex_gsm_blockHandle = osMutexCreate(osMutex(mutex_gsm_block));

    /* definition and creation of mutex_gsm_nonblock */
    osMutexDef(mutex_gsm_nonblock);
    mutex_gsm_nonblockHandle = osMutexCreate(osMutex(mutex_gsm_nonblock));

    /* definition and creation of mutex_gsm_rx */
    osMutexDef(mutex_gsm_rx);
    mutex_gsm_rxHandle = osMutexCreate(osMutex(mutex_gsm_rx));

    /* definition and creation of mutex_gsm_sd */
    osMutexDef(mutex_gsm_sd);
    mutex_gsm_sdHandle = osMutexCreate(osMutex(mutex_gsm_sd));

    /* definition and creation of mutex_usb */
    osMutexDef(mutex_usb);
    mutex_usbHandle = osMutexCreate(osMutex(mutex_usb));

    /* definition and creation of mutex_usb_sd */
    osMutexDef(mutex_usb_sd);
    mutex_usb_sdHandle = osMutexCreate(osMutex(mutex_usb_sd));

    /* definition and creation of mutex_sd_proc */
    osMutexDef(mutex_sd_proc);
    mutex_sd_procHandle = osMutexCreate(osMutex(mutex_sd_proc));

    /* definition and creation of mutex_data_buf1 */
    osMutexDef(mutex_data_buf1);
    mutex_data_buf1Handle = osMutexCreate(osMutex(mutex_data_buf1));

    /* definition and creation of mutex_data_buf2 */
    osMutexDef(mutex_data_buf2);
    mutex_data_buf2Handle = osMutexCreate(osMutex(mutex_data_buf2));

    /* definition and creation of mutex_data_buf3 */
    osMutexDef(mutex_data_buf3);
    mutex_data_buf3Handle = osMutexCreate(osMutex(mutex_data_buf3));

    /* definition and creation of mutex_malloc */
    osMutexDef(mutex_malloc);
    mutex_mallocHandle = osMutexCreate(osMutex(mutex_malloc));

    /* definition and creation of mutex_upload_chk */
    osMutexDef(mutex_upload_chk);
    mutex_upload_chkHandle = osMutexCreate(osMutex(mutex_upload_chk));

    /* definition and creation of mutex_uart1ext */
    osMutexDef(mutex_uart1ext);
    mutex_uart1extHandle = osMutexCreate(osMutex(mutex_uart1ext));

    /* definition and creation of mutex_uart2ext */
    osMutexDef(mutex_uart2ext);
    mutex_uart2extHandle = osMutexCreate(osMutex(mutex_uart2ext));

    /* definition and creation of mutex_uart3ext */
    osMutexDef(mutex_uart3ext);
    mutex_uart3extHandle = osMutexCreate(osMutex(mutex_uart3ext));

    /* definition and creation of mutex_uart4ext */
    osMutexDef(mutex_uart4ext);
    mutex_uart4extHandle = osMutexCreate(osMutex(mutex_uart4ext));

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    mutex_i2c1_user = mutex_i2c1Handle;

    mutex_i2c2_user = mutex_i2c2Handle;

    mutex_spi1_user = mutex_spi1Handle;

    mutex_spi2_user = mutex_spi2Handle;

    mutex_uart_user[EXT_COMPORT_DEF] = mutex_uart1Handle;

    mutex_uart_user[GPS_COMPORT_DEF] = mutex_uart2Handle;

    mutex_uart_user[SET_COMPORT_DEF] = mutex_uart3Handle;

    mutex_uart_user[DB9_COMPORT_DEF] = mutex_uart4Handle;

    mutex_uart_user[GSM_COMPORT_DEF] = mutex_uart5Handle;

    mutex_lcd_user = mutex_lcdHandle;

    mutex_prn_user = mutex_prnHandle;

    mutex_ttnow_user = mutex_ttnowHandle;

    mutex_setting_user = mutex_settingHandle;

    mutex_gsm_block_user = mutex_gsm_blockHandle;

    mutex_gsm_nonblock_user = mutex_gsm_nonblockHandle;

    mutex_gsm_rx_user = mutex_gsm_rxHandle;

    mutex_gsm_sd_user = mutex_gsm_sdHandle;

    mutex_usb_user = mutex_usbHandle;

    mutex_usb_sd_user = mutex_usb_sdHandle;

    mutex_sd_proc_user = mutex_sd_procHandle;

    mutex_data_buf1_user = mutex_data_buf1Handle;

    mutex_data_buf2_user = mutex_data_buf2Handle;

    mutex_data_buf3_user = mutex_data_buf3Handle;

    mutex_malloc_user = mutex_mallocHandle;

    mutex_upload_chk_user = mutex_upload_chkHandle;

#if ENABLE_MG82
    mutex_uartext_user[UART_EXT_PORT0] = mutex_uart1extHandle;
    mutex_uartext_user[UART_EXT_PORT1] = mutex_uart2extHandle;
    mutex_uartext_user[UART_EXT_PORT2] = mutex_uart3extHandle;
    mutex_uartext_user[UART_EXT_PORT3] = mutex_uart4extHandle;
#endif

    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* Create the timer(s) */
    /* definition and creation of timerRTOS_GSM_Block */
    osTimerDef(timerRTOS_GSM_Block, timerRTOS_GSM_BlockCallback);
    timerRTOS_GSM_BlockHandle = osTimerCreate(osTimer(timerRTOS_GSM_Block), osTimerOnce, NULL);

    /* definition and creation of timerRTOS_GSM_NonBlock */
    osTimerDef(timerRTOS_GSM_NonBlock, timerRTOS_GSM_NonBlockCallback);
    timerRTOS_GSM_NonBlockHandle = osTimerCreate(osTimer(timerRTOS_GSM_NonBlock), osTimerOnce, NULL);

    /* definition and creation of timerRTOS_EXTI */
    osTimerDef(timerRTOS_EXTI, timerRTOS_EXTI_Callback);
    timerRTOS_EXTIHandle = osTimerCreate(osTimer(timerRTOS_EXTI), osTimerPeriodic, NULL);

    /* definition and creation of timerRTOS_PCTrans */
    osTimerDef(timerRTOS_PCTrans, timerRTOS_PCTransCallBack);
    timerRTOS_PCTransHandle = osTimerCreate(osTimer(timerRTOS_PCTrans), osTimerOnce, NULL);

    /* definition and creation of timerACC_Off */
    osTimerDef(timerACC_Off, timerACC_OffCallBack);
    timerACC_OffHandle = osTimerCreate(osTimer(timerACC_Off), osTimerOnce, NULL);

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* definition and creation of GSM_BlockCplt */
    osMessageQDef(GSM_BlockCplt, 10, uint16_t);
    GSM_BlockCpltHandle = osMessageCreate(osMessageQ(GSM_BlockCplt), NULL);

    /* definition and creation of GSM_NonBlockCplt */
    osMessageQDef(GSM_NonBlockCplt, 5, uint16_t);
    GSM_NonBlockCpltHandle = osMessageCreate(osMessageQ(GSM_NonBlockCplt), NULL);

    /* definition and creation of GSM_NonBlockRes */
    osMessageQDef(GSM_NonBlockRes, 5, uint16_t);
    GSM_NonBlockResHandle = osMessageCreate(osMessageQ(GSM_NonBlockRes), NULL);

    /* definition and creation of GSM_NonBlockResCode */
    osMessageQDef(GSM_NonBlockResCode, 5, uint32_t);
    GSM_NonBlockResCodeHandle = osMessageCreate(osMessageQ(GSM_NonBlockResCode), NULL);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 640);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of taskLogProc */
    osThreadDef(taskLogProc, Start_taskLogProc, osPriorityNormal, 0, 768);
    taskLogProcHandle = osThreadCreate(osThread(taskLogProc), NULL);

    /* definition and creation of taskEXTIO_Proc */
    osThreadDef(taskEXTIO_Proc, Start_taskEXTIO_Proc, osPriorityNormal, 0, 640);
    taskEXTIO_ProcHandle = osThreadCreate(osThread(taskEXTIO_Proc), NULL);

    /* definition and creation of taskGPS_Proc */
    osThreadDef(taskGPS_Proc, Start_taskGPS_Proc, osPriorityNormal, 0, 768);
    taskGPS_ProcHandle = osThreadCreate(osThread(taskGPS_Proc), NULL);

    /* definition and creation of taskGSM_Proc */
    osThreadDef(taskGSM_Proc, Start_taskGSM_Proc, osPriorityNormal, 0, 896);
    taskGSM_ProcHandle = osThreadCreate(osThread(taskGSM_Proc), NULL);

    /* definition and creation of taskLCD_Key */
    osThreadDef(taskLCD_Key, Start_taskLCD_Key, osPriorityNormal, 0, 640);
    taskLCD_KeyHandle = osThreadCreate(osThread(taskLCD_Key), NULL);

    /* definition and creation of taskRS232_Proc */
    osThreadDef(taskRS232_Proc, Start_taskRS232_Proc, osPriorityNormal, 0, 768);
    taskRS232_ProcHandle = osThreadCreate(osThread(taskRS232_Proc), NULL);

    /* definition and creation of taskHighLoad */
    osThreadDef(taskHighLoad, Start_taskHighLoad, osPriorityNormal, 0, 896);
    taskHighLoadHandle = osThreadCreate(osThread(taskHighLoad), NULL);

    /* definition and creation of taskSD_Proc */
    osThreadDef(taskSD_Proc, Start_taskSD_Proc, osPriorityNormal, 0, 640);
    taskSD_ProcHandle = osThreadCreate(osThread(taskSD_Proc), NULL);

    /* definition and creation of taskGSM_RX */
    osThreadDef(taskGSM_RX, Start_taskGSM_RX, osPriorityNormal, 0, 896);
    taskGSM_RXHandle = osThreadCreate(osThread(taskGSM_RX), NULL);

    /* definition and creation of taskGSM_TX */
    osThreadDef(taskGSM_TX, Start_taskGSM_TX, osPriorityNormal, 0, 768);
    taskGSM_TXHandle = osThreadCreate(osThread(taskGSM_TX), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */

    System_TaskInfoInit();

    memcpy(&(taskInfo[TASK_LOG_PROC].tTaskDef), osThread(taskLogProc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_EXTIO_PROC].tTaskDef), osThread(taskEXTIO_Proc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_GPS_PROC].tTaskDef), osThread(taskGPS_Proc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_GSM_PROC].tTaskDef), osThread(taskGSM_Proc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_LCD_KEY].tTaskDef), osThread(taskLCD_Key), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_RS232_PROC].tTaskDef), osThread(taskRS232_Proc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_HIGH_LOAD].tTaskDef), osThread(taskHighLoad), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_SD_PROC].tTaskDef), osThread(taskSD_Proc), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_GSM_RX].tTaskDef), osThread(taskGSM_RX), sizeof(osThreadDef_t));
    memcpy(&(taskInfo[TASK_GSM_TX].tTaskDef), osThread(taskGSM_TX), sizeof(osThreadDef_t));

#if !ENABLE_GSM_NET
    osThreadTerminate(taskGSM_ProcHandle);
    osThreadTerminate(taskGSM_RXHandle);
    osThreadTerminate(taskGSM_TXHandle);

    taskInfo[TASK_GSM_PROC].bTaskState = FALSE;
    taskInfo[TASK_GSM_RX].bTaskState = FALSE;
    taskInfo[TASK_GSM_TX].bTaskState = FALSE;
#endif

    UART_CheckInterrupt();

#if ENABLE_PULSE_COUNT_REC
    HAL_TIM_Base_Start(&htim2);
#endif

    RTOSprintf("\r\n> ### ENTER FREERTOS ###");

    bRTOS_En = TRUE;

    /* USER CODE END RTOS_THREADS */

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 240;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN1_Init(void)
{

    /* USER CODE BEGIN CAN1_Init 0 */

    /* USER CODE END CAN1_Init 0 */

    /* USER CODE BEGIN CAN1_Init 1 */

    /* USER CODE END CAN1_Init 1 */
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 6;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = ENABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN CAN1_Init 2 */

    /* USER CODE END CAN1_Init 2 */
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

    /* USER CODE BEGIN I2C2_Init 0 */

    /* USER CODE END I2C2_Init 0 */

    /* USER CODE BEGIN I2C2_Init 1 */

    /* USER CODE END I2C2_Init 1 */
    hi2c2.Instance = I2C2;
    hi2c2.Init.ClockSpeed = 400000;
    hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c2.Init.OwnAddress1 = 0;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2 = 0;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C2_Init 2 */

    /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void)
{

    /* USER CODE BEGIN IWDG_Init 0 */

    /* USER CODE END IWDG_Init 0 */

    /* USER CODE BEGIN IWDG_Init 1 */

    /* USER CODE END IWDG_Init 1 */
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
    hiwdg.Init.Reload = 4000;
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */

    /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

    /* USER CODE BEGIN RTC_Init 0 */
    // RTC_TimeTypeDef sTime;
    // RTC_DateTypeDef sDate;

    /* USER CODE END RTC_Init 0 */

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */
    /** Initialize RTC Only
     */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
     */
    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 0x1;
    sDate.Year = 0x20;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN RTC_Init 2 */
    // PCF_GetDate(&sDate, 1);
    // PCF_GetTime(&sTime, 1);

    // HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    // HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    /* USER CODE END RTC_Init 2 */
}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDIO_SD_Init(void)
{

    /* USER CODE BEGIN SDIO_Init 0 */

    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 1;
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART4_Init(void)
{

    /* USER CODE BEGIN UART4_Init 0 */

    /* USER CODE END UART4_Init 0 */

    /* USER CODE BEGIN UART4_Init 1 */

    /* USER CODE END UART4_Init 1 */
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 115200;
    huart4.Init.WordLength = UART_WORDLENGTH_9B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_ODD;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART4_Init 2 */

    /* USER CODE END UART4_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Stream3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    /* DMA2_Stream6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, LCD_SCL_Pin | P_MTA_M_Pin | P_MTA_P_Pin | P_MTB_M_Pin | P_MTB_P_Pin | GSM_PWRKEY_Pin | GSM_WAKEUP_Pin | GSM_RESET_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, EN_MOTOR_PWR_Pin | BUZZER_Pin | GPS_PWR_EN_Pin | EN_PRN_PWR_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, P_STB1_Pin | P_STB6_Pin | N_P_LAT_Pin | DRIVE_VBUS_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, SPI1_NSS_Pin | N_LCD_BL_Pin | WDT_EN_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, P_DI_Pin | P_CLK_Pin | N_8051_INT_Pin | EN_SD_PWR_Pin | LCD_SIO_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(N_LCD_CS_GPIO_Port, N_LCD_CS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(EN_3V3_IO_GPIO_Port, EN_3V3_IO_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(N_LCD_RST_GPIO_Port, N_LCD_RST_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : LCD_SCL_Pin P_MTA_M_Pin P_MTA_P_Pin P_MTB_M_Pin
                             P_MTB_P_Pin GSM_PWRKEY_Pin GSM_WAKEUP_Pin N_LCD_CS_Pin
                             GSM_RESET_Pin */
    GPIO_InitStruct.Pin = LCD_SCL_Pin | P_MTA_M_Pin | P_MTA_P_Pin | P_MTB_M_Pin | P_MTB_P_Pin | GSM_PWRKEY_Pin | GSM_WAKEUP_Pin | N_LCD_CS_Pin | GSM_RESET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    // /*Configure GPIO pins : N_IO6_Pin N_IO8_Pin N_IO7_Pin KEY4_Pin */
    // GPIO_InitStruct.Pin = N_IO6_Pin | N_IO8_Pin | N_IO7_Pin | KEY4_Pin;
    // GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin Output Level */
    // HAL_GPIO_WritePin(N_IO8_GPIO_Port, N_IO8_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : EN_MOTOR_PWR_Pin */
    //	GPIO_InitStruct.Pin = N_IO8_Pin;
    //	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //	GPIO_InitStruct.Pull = GPIO_NOPULL;
    //	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //	HAL_GPIO_Init(N_IO8_GPIO_Port, &GPIO_InitStruct);

    //	HAL_GPIO_WritePin(N_IO8_GPIO_Port, N_IO8_Pin, GPIO_PIN_RESET);
    /*Configure GPIO pins : N_IO6_Pin N_IO8_Pin N_IO7_Pin KEY4_Pin */
    GPIO_InitStruct.Pin = N_IO6_Pin | N_IO7_Pin | N_IO8_Pin | KEY4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : EN_MOTOR_PWR_Pin */
    GPIO_InitStruct.Pin = EN_MOTOR_PWR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EN_MOTOR_PWR_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BUZZER_Pin GPS_PWR_EN_Pin EN_3V3_IO_Pin EN_PRN_PWR_Pin */
    GPIO_InitStruct.Pin = BUZZER_Pin | GPS_PWR_EN_Pin | EN_3V3_IO_Pin | EN_PRN_PWR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : P_STB1_Pin P_STB6_Pin N_P_LAT_Pin SPI1_NSS_Pin
                             N_LCD_BL_Pin DRIVE_VBUS_Pin WDT_EN_Pin */
    GPIO_InitStruct.Pin = P_STB1_Pin | P_STB6_Pin | N_P_LAT_Pin | SPI1_NSS_Pin | N_LCD_BL_Pin | DRIVE_VBUS_Pin | WDT_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : P_DI_Pin P_CLK_Pin EN_SD_PWR_Pin LCD_SIO_Pin */
    GPIO_InitStruct.Pin = P_DI_Pin | P_CLK_Pin | EN_SD_PWR_Pin | LCD_SIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : RPM_IN_Pin */
    GPIO_InitStruct.Pin = RPM_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RPM_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : SPEED_IN_Pin */
    GPIO_InitStruct.Pin = SPEED_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SPEED_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : N_IO3_Pin N_IO1_Pin N_IO4_Pin N_IO2_Pin
                             N_IO5_Pin */
    GPIO_InitStruct.Pin = N_IO3_Pin | N_IO1_Pin | N_IO4_Pin | N_IO2_Pin | N_IO5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : SD_DETECT_Pin CAN_DETECT_Pin */
    GPIO_InitStruct.Pin = SD_DETECT_Pin | CAN_DETECT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : N_8051_INT_Pin */
    GPIO_InitStruct.Pin = N_8051_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(N_8051_INT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LCD_DET_Pin P_EMPTY_Pin KEY5_Pin KEY1_Pin
                             KEY2_Pin KEY3_Pin N_ACC_IN_Pin */
    GPIO_InitStruct.Pin = LCD_DET_Pin | P_EMPTY_Pin | KEY5_Pin | KEY1_Pin | KEY2_Pin | KEY3_Pin | N_ACC_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : CHG_STATE_Pin */
    GPIO_InitStruct.Pin = CHG_STATE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CHG_STATE_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : N_LCD_RST_Pin LCD_A0_Pin */
    GPIO_InitStruct.Pin = N_LCD_RST_Pin | LCD_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : G_SEN_INT_Pin */
    GPIO_InitStruct.Pin = G_SEN_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(G_SEN_INT_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/* USER CODE BEGIN 4 */

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
    if (UART_IsTerminalUsingSET())
        HAL_UART_Transmit(puartHandle[SET_COMPORT_DEF], (uint8_t *)&ch, 1, 0xFFFF);
    else
        HAL_UART_Transmit(puartHandle[DB9_COMPORT_DEF], (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

void USER_PeriphInit(void)
{
    MX_GPIO_Init();
    MX_I2C2_Init();
    MX_RTC_Init();
    // MX_SPI2_Init();
    MX_UART4_Init();
    MX_USART3_UART_Init();
    MX_FATFS_Init();
    MX_SPI1_Init();
    // MX_UART5_Init();
    MX_USART2_UART_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_CAN1_Init();
    MX_I2C1_Init();
}

/**
 * @brief  Call function MX_I2C2_Init() from outside file main.c.
 * @param  None
 * @retval None
 */
void USER_I2C2_Init(void)
{
    MX_I2C2_Init();
}
/**
 * @brief  Call function MX_I2C2_Init() from outside file main.c.
 * @param  None
 * @retval None
 */
void USER_I2C1_Init(void)
{
    MX_I2C1_Init();
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
    /* USER CODE BEGIN 5 */
    struct tm tmRTCTimeInfo;
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    USB_HostInit();

    uiRPM_TickIdx = 0;
    uiSPD_TickIdx = 0;

    osTimerStart(timerRTOS_EXTIHandle, S_2_mS(1));

    /* Infinite loop */
    for (;;)
    {
        HAL_IWDG_Refresh(&hiwdg);

        System_TaskCheck();

        USB_CheckDisconnection();

        if (bSystemStop == TRUE)
        {
            osDelay(1000);

            continue;
        }

        memset(&rtcTime, 0x00, sizeof(RTC_TimeTypeDef));
        memset(&rtcDate, 0x00, sizeof(RTC_DateTypeDef));

#if ENABLE_MG82_RTC
        if (bMG82_RTC_EN)
        {
            // Get current date time in Binary format
            if (!MG82_RTC_GetDateTime(&rtcDate, &rtcTime, 1))
            {
                if (bMG82_Stop == FALSE)
                {
                    RTOSprintf("\r\n> MG82 RTC Get Date Time ERROR");
                }

                continue;
            }
        }
#elif ENABLE_RX8130
        if (bRX8130_EN)
        {
            // Get current date time in Binary format
            if (!RX8130_GetDateTime(&rtcDate, &rtcTime, 1))
            {
                if (bRX8130_Stop == FALSE)
                {
                    RTOSprintf("\r\n> RX8130 Get Date Time ERROR");
                }

                continue;
            }
        }
#else
        if (g_bPCF_EN)
        {
            // Get current time in Binary format
            if (!PCF_GetTime(&rtcTime, 1))
            {
                if (bPCF_Stop == FALSE)
                {
                    // RTOSprintf("\r\n> PCF GetTime ERROR");
                }

                continue;
            }

            // Get current date in Binary format
            if (!PCF_GetDate(&rtcDate, 1))
            {
                if (bPCF_Stop == FALSE)
                {
                    // RTOSprintf("\r\n> PCF GetDate ERROR");
                }

                continue;
            }
        }
#endif
        // If using RTC as time source
        else
        {
            HAL_StatusTypeDef halRes;
            osStatus osRes;

            osRes = osMutexWait(mutex_ttnow_user, S_2_mS(3));

            if (osRes != osOK)
                continue;

            halRes = HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);

            // Get current time in Binary format
            if (halRes != HAL_OK)
            {
                RTOSprintf("\r\n> MCU RTC GetTime ERROR %d", halRes);

                osMutexRelease(mutex_ttnow_user);

                continue;
            }

            halRes = HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

            // Get current date in Binary format
            if (halRes != HAL_OK)
            {
                RTOSprintf("\r\n> MCU RTC GetDate ERROR %d", halRes);

                osMutexRelease(mutex_ttnow_user);

                continue;
            }

            osMutexRelease(mutex_ttnow_user);
        }

        // Year in tm struct is counted from 1900
        tmRTCTimeInfo.tm_year = rtcDate.Year + 2000 - 1900U;
        // Month in tm struct is counted from 0 corresponding to Jan
        tmRTCTimeInfo.tm_mon = rtcDate.Month - 1U;
        tmRTCTimeInfo.tm_mday = rtcDate.Date;
        tmRTCTimeInfo.tm_hour = rtcTime.Hours;
        tmRTCTimeInfo.tm_min = rtcTime.Minutes;
        tmRTCTimeInfo.tm_sec = rtcTime.Seconds;
        tmRTCTimeInfo.tm_isdst = -1;

        if ((rtcDate.Year < 20) ||
            (rtcDate.Year > 50))
        {
            continue;
        }

        // Convert time from calendar format to time to second
        g_SysInfo.ttNow = mktime(&tmRTCTimeInfo);

        // Add timezone offset to time in second
        // g_SysInfo.ttNow += g_Setting.i32TimeZoneOffset;

        u8SysCurrMin = tmRTCTimeInfo.tm_min;

        // Check new minute to set One Min Flag
        if (u8SysCurrMin != u8SysOldMin)
        {
            memcpy(u8SpeedBuf, u8SpeedBufTemp, 120);

            ttSysLogTime = g_SysInfo.ttNow - MINUTE_2_SECOND(1);

            if (bFirstLogFlag == TRUE)
            {
                bOneMinFlag = TRUE;

                // Record DTR for Min
                // bDatalogTickFlag = TRUE;

                SecToBCDTime(ttSysLogTime, u8DebugTime);

                if (BCD_2_Bin(u8DebugTime[5]) != u8SysOldMin)
                {
                    tmRTCTimeInfoTemp = tmRTCTimeInfo;

                    memcpy(u8DebugTimeTemp, u8DebugTime, 7);

                    u8SysOldMinTemp = u8SysOldMin;

                    bLogTimeErrFlag = TRUE;
                }
            }
            else
            {
                bFirstLogFlag = TRUE;
            }

            u8SysOldMin = u8SysCurrMin;
        }

        u8SysCurrSec = tmRTCTimeInfo.tm_sec;

        u8SecBk = u8SysCurrSec;

        // Check new second
        if (u8SysCurrSec != u8SysOldSec)
        {
            u16SubSecCnt = 0;

#if ENABLE_PULSE_COUNT_REC
// u32TIM2_Cnt = __HAL_TIM_GetCounter(&htim2);

//__HAL_TIM_SetCounter(&htim2, 0);
#endif

            // Record DTR for Sec
            bDatalogTickFlag = TRUE;

            bDVRSecProcFlag = TRUE;

#if ENABLE_MG82
            HAL_GPIO_TogglePin(WDT_EN_GPIO_Port, WDT_EN_Pin);
#endif

            if (((u8SysCurrSec % SAVE_RPM_TIME) == 0) && (u8SysOldSec != 0) &&
                (bRpmSecSaveFlag == FALSE))
            {
                memcpy(&stVdrRpmTemp, &stVdrRpm, sizeof(VdrRpmDataTypeDef));

                memset(&stVdrRpm, 0x00, sizeof(VdrRpmDataTypeDef));

                // RTOSprintf("\r\n> Set SAVE RPM flag.");

                bRpmSecSaveFlag = TRUE;
            }

            u8RpmBufIndex = u8SysCurrSec % SAVE_RPM_TIME;

            // Record Rpm for Sec
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u32UTCTime = g_SysInfo.ttNow - g_Setting.i32TimeZoneOffset;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16Rpm = g_SysInfo.tVehicleState.u16RPM;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u8GpsSpeed = (uint8_t)M_2_KM(g_SysInfo.tVehicleState.u32Speed_GPS);
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus._U8[1] = 0x00;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._07 = g_SysInfo.tEXTIO.bits.IN_ACC;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._06 = g_SysInfo.tEXTIO.bits.IN_5;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._05 = g_SysInfo.tEXTIO.bits.IN_6;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._04 = g_SysInfo.tEXTIO.bits.IN_7;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._03 = g_SysInfo.tEXTIO.bits.IN_1;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._02 = g_SysInfo.tEXTIO.bits.IN_2;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._01 = g_SysInfo.tEXTIO.bits.IN_3;
            stVdrRpm.stRpmBuf[u8RpmBufIndex].u16IOStatus.bit._00 = g_SysInfo.tEXTIO.bits.IN_4;

            if (bFirstLogFlag == TRUE)
            {
                bSecProcFlag = TRUE;
            }

            // if((tmRTCTimeInfo.tm_hour == 0) && (tmRTCTimeInfo.tm_min == 0) && (tmRTCTimeInfo.tm_sec == 10))
            //{
            //   bMidNightFlag = TRUE;
            // }

#if ENABLE_GSM_NET
            // Check if Datalog Flag need to be set
            for (int i = 0; i < MAX_SOCKET_BANK; i++)
            {
                if (((u8SysCurrSec % u8DATALOG_UPLOAD_MAX[i]) == 0) && (u8SysOldSec != 0) &&
                    (bDataLogFlag[i] == FALSE))
                {
                    ttDataLogTime[i] = g_SysInfo.ttNow - g_Setting.i32TimeZoneOffset - 1;
                    // ttDataLogTime[i] = g_SysInfo.ttNow - g_Setting.i32TimeZoneOffset - 30;

                    memcpy(&(datalogHandleTemp[i]), &(datalogHandle[i]), sizeof(DatalogUploadTypeDef));

                    memset(&(datalogHandle[i].DATA), 0x00, sizeof(UploadDataTypeDef) * u8DATALOG_UPLOAD_MAX[i]);

                    // RTOSprintf("\r\n> Set DATALOG flag.");

                    bDataLogFlag[i] = TRUE;
                }

                u8DatalogIdx = u8SysCurrSec % u8DATALOG_UPLOAD_MAX[i];

                datalogHandle[i].DATA[u8DatalogIdx].i32Longitude = g_SysInfo.tGPS_Data.gpsLocation.dLongitude * 1000000;
                datalogHandle[i].DATA[u8DatalogIdx].i32Latitude = g_SysInfo.tGPS_Data.gpsLocation.dLatitude * 1000000;
                datalogHandle[i].DATA[u8DatalogIdx].u8Course = (uint8_t)g_SysInfo.tGPS_Data.u16Course / 10;
                // datalogHandle[i].DATA[u8DatalogIdx].u8Speed = (uint8_t)M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);
                datalogHandle[i].DATA[u8DatalogIdx].u8Speed = (uint8_t)M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);
                datalogHandle[i].DATA[u8DatalogIdx].b50RPM = (uint8_t)(g_SysInfo.tVehicleState.u16RPM / 50);
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._7 = g_SysInfo.tEXTIO.bits.IN_ACC;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._6 = g_SysInfo.tEXTIO.bits.IN_5;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._5 = g_SysInfo.tEXTIO.bits.IN_6;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._4 = g_SysInfo.tEXTIO.bits.IN_7;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._3 = g_SysInfo.tEXTIO.bits.IN_1;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._2 = g_SysInfo.tEXTIO.bits.IN_2;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._1 = g_SysInfo.tEXTIO.bits.IN_3;
                datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._0 = g_SysInfo.tEXTIO.bits.IN_4;
                datalogHandle[i].DATA[u8DatalogIdx].u8Speed_GPS = (uint8_t)M_2_KM(g_SysInfo.tVehicleState.u32Speed_GPS);
                datalogHandle[i].DATA[u8DatalogIdx].u8Status.bit._7 = bUSB_Ready;
                datalogHandle[i].DATA[u8DatalogIdx].u8Status.bit._1 = g_SysInfo.tSpeedInfo.bIsOverSpeed;
                datalogHandle[i].DATA[u8DatalogIdx].u8Status.bit._0 = g_SysInfo.tSpeedInfo.bIsRPM_Idle;

                if (bFirstACC == TRUE)
                {
                    datalogHandle[i].DATA[u8DatalogIdx].EXTIO.bit._7 = 0;

                    bFirstACC = FALSE;
                }
            }
#endif

            if (((u8SysCurrSec % RECORD_DATA_SIZE) == 0) && (u8SysOldSec != 0))
            {
#if ENABLE_SPEED_INT_REC
                if (Datalog_IsRecordMode(REC_SPD_INT))
                {
                    ttRecordSpeedIntTime = g_SysInfo.ttNow - 30;

                    memcpy(&tSpeedIntTemp, &tSpeedInt, sizeof(SpeedIntTypeDef));

                    memset(&tSpeedInt, 0x00, sizeof(SpeedIntTypeDef));

                    bRecordSpeedIntFlag = TRUE;
                }
#endif

#if ENABLE_PULSE_COUNT_REC
                if (Datalog_IsRecordMode(REC_PULSE_CNT))
                {
                    ttRecordPulseCntTime = g_SysInfo.ttNow - 30;

                    memcpy(&tPulseCntTemp, &tPulseCnt, sizeof(PulseCountTypeDef));

                    memset(&tPulseCnt, 0x00, sizeof(PulseCountTypeDef));

                    bRecordPulseCntFlag = TRUE;
                }
#endif

#if ENABLE_GPS_SPEED_REC
                if (Datalog_IsRecordMode(REC_GPS_SPD))
                {
                    ttRecordGPS_SpeedTime = g_SysInfo.ttNow - 30;

                    memcpy(&gpsSpdRecTemp, &tGPS_Speed, sizeof(GPS_SpeedRecTypeDef));

                    memset(&tGPS_Speed, 0x00, sizeof(GPS_SpeedRecTypeDef));

                    bRecordGPS_SpeedFlag = TRUE;
                }
#endif
            }

#if ENABLE_PULSE_COUNT_REC || ENABLE_GPS_SPEED_REC
            uint8_t u8RecordIdx = u8SysCurrSec % RECORD_DATA_SIZE;
#endif

#if ENABLE_SPEED_INT_REC
            if (Datalog_IsRecordMode(REC_SPD_INT))
            {
                if (bSpeedIntFlag == TRUE)
                {
                    tSpeedInt.u32SpeedIntDebug = tSpeedInt.u32SpeedIntDebug | u32MaskBit[u8RecordIdx];

                    bSpeedIntFlag = FALSE;
                }
            }
#endif

#if ENABLE_PULSE_COUNT_REC
            if (Datalog_IsRecordMode(REC_PULSE_CNT))
            {
                tPulseCnt.u8PulseCnt[u8RecordIdx] = (uint8_t)(u32TIM2_Cnt / 10);
            }
#endif

#if ENABLE_GPS_SPEED_REC
            if (Datalog_IsRecordMode(REC_GPS_SPD))
            {
                tGPS_Speed.u8Speed[u8RecordIdx] = (uint8_t)M_2_KM(g_SysInfo.tGPS_Data.u32Speed);
            }
#endif

            u8SysOldSec = u8SysCurrSec;

            // RTOSprintf("\r\n> Current time:%02d/%02d/%02d,%02d:%02d:%02d", rtcDate.Year, rtcDate.Month, rtcDate.Date,
            //            rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
        }

        u16SubSecCnt++;

        // Check if Half Second Flag need to be set
        if (u16SubSecCnt == 1)
        {
            u8SpeedBufTemp[u8SysCurrSec][0] = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);

            bHalfSecFlag = TRUE;
        }

        // Check if Half Second Flag need to be set
        if (u16SubSecCnt == 60)
        {
            u8SpeedBufTemp[u8SysCurrSec][1] = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);

            bHalfSecFlag = TRUE;
        }

        if (bSysInitFinish == FALSE)
            bSysInitFinish = TRUE;

        // Update date time every 5 milliseconds
        osDelay(5);
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_taskLogProc */
/**
 * @brief Function implementing the taskLogProc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskLogProc */
void Start_taskLogProc(void const *argument)
{
    /* USER CODE BEGIN Start_taskLogProc */
    uint32_t u32ReloadTick;

    u32ReloadTick = HAL_GetTick();

    while (bSysInitFinish == FALSE)
    {
        if (HAL_GetTick() > (u32ReloadTick + S_2_mS(1)))
        {
            System_TaskWdgRefresh(TASK_LOG_PROC);

            u32ReloadTick = HAL_GetTick();
        }

        osDelay(1);
    }

    if (bSD_ReportErrFlag == FALSE)
    {
        Datalog_Init();

        if (bSystemMode == SYSTEM_MODE_NORMAL)
            Datalog_ChkData();

        // Rule V161 Datalog
        V161_CheckRecordFile();

#if ENABLE_INDEX_BUFFER
        RTOSprintf("\r\n> ### Load file index ###");

        V161_LoadFileIdxAll();
#endif

        bIsCheckVDRProcFlag = TRUE;

        LOGprintf("\r\n> ### Restart record ###");

        if (bSystemMode == SYSTEM_MODE_NORMAL)
            V161_RecordExternslData();

        V161_DetailSpeedInit();

        V161_PositionInit();

#if ENABLE_SD_BUFFER
        V161_SDBuf_SaveDataInit();
#endif

        procClearFileStep = PROC_STEP_1;
    }

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_LOG_PROC);

        if ((bSystemStop == TRUE) || (bSystemMode == SYSTEM_MODE_STEALTH) ||
            (bSystemTimeReady == FALSE) || (sdRequest.sdCmd == SD_MKFS))
        {
            osDelay(1000);

            continue;
        }

// Do SD task for old log
#if ENABLE_SD_BUFFER
        if (V161_SDBuf_IsHighLoading() == FALSE)
#endif
        {
            if (EXTIO_IsMainPowerNormal() && (bSD_ReportErrFlag == FALSE))
            {
                Datalog_Save_Proc();

                taskInfo[TASK_LOG_PROC].u16TaskStep++;

                Datalog_ClearFileProc();
            }
        }

        taskInfo[TASK_LOG_PROC].u16TaskStep++;

        // Do SD task for 30 sec rpm data
        if ((bRpmSecSaveFlag == TRUE) && (bSD_ReportErrFlag == FALSE))
        {
            bRpmSecSaveFlag = FALSE;

            // RTOSprintf("\r\n> ### 10 Sec write to rpm file (%d) ###", stVdrRpmTemp.stRpmBuf[0].u32UTCTime);

            V161_WriteRpmToVDR(stVdrRpmTemp);
        }

        taskInfo[TASK_LOG_PROC].u16TaskStep++;

#if ENABLE_SD_BUFFER
        if (EXTIO_IsMainPowerNormal() &&
            ((V161_SDBuf_IsHighLoading() == TRUE) || ((V161_SDBuf_IsHighLoading() == FALSE) && (bSD_SaveDatRequest == FALSE))))
#else
        if (EXTIO_IsMainPowerNormal())
#endif
        {
            taskInfo[TASK_LOG_PROC].u16TaskStep++;

#if ENABLE_GSM_NET
            for (int i = 0; i < MAX_SOCKET_BANK; i++)
            {
                if (bDataLogFlag[i] == TRUE)
                {
                    if (ttDataLogTime[i] > ttDataLogTimeOld[i])
                    {
                        GSM_LOG_Proc(i);

                        ttDataLogTimeOld[i] = ttDataLogTime[i];
                    }

                    bDataLogFlag[i] = FALSE;
                }
            }
#endif

            taskInfo[TASK_LOG_PROC].u16TaskStep++;

            if ((bSD_ReportErrFlag == FALSE) && (bOneMinFlag == TRUE))
            {
                // Do 1 min SD task for table 6, 8, 9 here
                V161_1MinLogProc();

                GPS_LastCoordinateProcess(GPS_POS_SAVE);

                System_SaveSetting();
                System_SaveBackupSetting();

                bOneMinFlag = FALSE;
            }

            taskInfo[TASK_LOG_PROC].u16TaskStep++;
        }

        if (bLogTimeErrFlag == TRUE)
        {
#if ENABLE_GSM_NET
            char cEventMsg[20] = {0};

            sprintf(cEventMsg, "%02d:%02d:%02d-%02d",
                    tmRTCTimeInfoTemp.tm_hour,
                    tmRTCTimeInfoTemp.tm_min,
                    tmRTCTimeInfoTemp.tm_sec,
                    u8SysOldMinTemp);

            GSM_TCP_ReportEvent((char *)EVENT_LOG_TIME_ERR, cEventMsg);
#endif

            bLogTimeErrFlag = FALSE;
        }

#if ENABLE_SD_BUFFER
        if (EXTIO_IsMainPowerNormal() && () && (bSD_ReportErrFlag == FALSE)(V161_SDBuf_IsHighLoading() == FALSE) && (bSD_SaveDatRequest == TRUE))
        {
            osDelay(100);

            V161_SDBuf_SaveDataProc();

            bSD_SaveDatRequest = FALSE;
        }
#endif

        if (bIsSaveSettingFlag == TRUE)
        {
            System_SaveSetting();
            System_SaveBackupSetting();

            if (g_Setting.tSpeedSet.u8Auto_Gain_Count == 128)
            {
                RTOSprintf("\r\n> ### Auto Gain OK ###");
            }

            bIsSaveSettingFlag = FALSE;
        }

#if ENABLE_GSM_NET == 1 &&      \
    ENABLE_FTP_DOWNLOAD == 1 && \
    ENABLE_FTP_BACKUP == 1
        if (bRestoreSettingFlag == TRUE)
        {
            System_RestoreSettingFromFTP();

            bRestoreSettingFlag = FALSE;
        }
#endif

        osDelay(5);
    }
    /* USER CODE END Start_taskLogProc */
}

/* USER CODE BEGIN Header_Start_taskEXTIO_Proc */
/**
 * @brief Function implementing the taskEXTIO_Proc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskEXTIO_Proc */
void Start_taskEXTIO_Proc(void const *argument)
{
    /* USER CODE BEGIN Start_taskEXTIO_Proc */

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_EXTIO_PROC);

        if ((bSystemStop == FALSE) && (sdRequest.sdCmd != SD_MKFS))
        {
            EXTIO_Proc();
        }
        else
        {
            EXTIO_DetectPWR();

            EXTIO_DetectACC_2();

            EXTIO_DetectCHG();
        }

        osDelay(5);
    }
    /* USER CODE END Start_taskEXTIO_Proc */
}

/* USER CODE BEGIN Header_Start_taskGPS_Proc */
/**
 * @brief Function implementing the taskGPS_Proc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskGPS_Proc */
void Start_taskGPS_Proc(void const *argument)
{
    /* USER CODE BEGIN Start_taskGPS_Proc */
    HAL_Delay(S_2_mS(1));

#if GPS_HARDWARE_TYPE == GPS_MODULE_SIM68
    if (bSystemStop == FALSE)
        GPS_CheckType();
#endif

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_GPS_PROC);

        if ((bSystemStop == FALSE) && (sdRequest.sdCmd != SD_MKFS))
            GPS_RX_Proc();

        osDelay(10);
    }
    /* USER CODE END Start_taskGPS_Proc */
}

/* USER CODE BEGIN Header_Start_taskGSM_Proc */
/**
 * @brief Function implementing the taskGSM_Proc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskGSM_Proc */
void Start_taskGSM_Proc(void const *argument)
{
    /* USER CODE BEGIN Start_taskGSM_Proc */

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_GSM_PROC);

#if ENABLE_GSM_NET
        // if((bSystemStop == FALSE) && EXTIO_IsPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
        GSM_Task_Proc();
#endif

        osDelay(5);
    }

    /* USER CODE END Start_taskGSM_Proc */
}

/* USER CODE BEGIN Header_Start_taskLCD_Key */
/**
 * @brief Function implementing the taskLCD_Key thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskLCD_Key */
void Start_taskLCD_Key(void const *argument)
{
    /* USER CODE BEGIN Start_taskLCD_Key */

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_LCD_KEY);

        LCD_SwitchHandle();

        if ((bSystemStop == FALSE) && EXTIO_IsPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
            LCD_Key_Process();

        osDelay(5);
    }
    /* USER CODE END Start_taskLCD_Key */
}

/* USER CODE BEGIN Header_Start_taskRS232_Proc */
/**
 * @brief Function implementing the taskRS232_Proc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskRS232_Proc */
void Start_taskRS232_Proc(void const *argument)
{
    /* USER CODE BEGIN Start_taskRS232_Proc */
    uint32_t u32TickCnt = 0, u32GetRX_Tick = 0;
#if ENABLE_LSM6DSL
    uint32_t u32GetGSenTick = 0;
#endif
#if ENABLE_DVR
    uint8_t u8DVROnCnt = 0, u8DVROffCnt = 0;
#endif
    // uint32_t u32TickCnt2 = 0;

    if (g_Setting.u8DB9Func == 0)
    {
        if (UART_IsTerminalUsingSET() == FALSE)
        {
            UART_Reset(SET_COMPORT_DEF);
            UART_ReInit(SET_COMPORT_DEF, BR_9600bps);
        }
        else
        {
            UART_Reset(DB9_COMPORT_DEF);
            UART_ReInit(DB9_COMPORT_DEF, BR_9600bps);
        }
    }
    else if (g_Setting.u8DB9Func == 1)
    {
        if (UART_IsTerminalUsingSET() == FALSE)
        {
            UART_Reset(SET_COMPORT_DEF);
            UART_ReInit(SET_COMPORT_DEF, BR_115200bps);
        }
        else
        {
            UART_Reset(DB9_COMPORT_DEF);
            UART_ReInit(DB9_COMPORT_DEF, BR_115200bps);
        }
    }

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_RS232_PROC);

        if ((bSystemStop == FALSE) && (g_SysInfo.tVehicleState.bPWR_ON == 1))
        {
#if ENABLE_DVR
            if (g_SysInfo.tVehicleState.bACC_ON == 1)
            {
                u8DVROffCnt = 0;

                if (bDVRSecProcFlag == TRUE)
                {
                    bDVRSecProcFlag = FALSE;

                    if (u8DVROnCnt >= 5)
                    {
                        UART_DVR_TransProc();
                    }
                    else
                    {
                        u8DVROnCnt++;
                    }
                }
            }
            else
            {
                u8DVROnCnt = 0;

                if (bDVRSecProcFlag == TRUE)
                {
                    bDVRSecProcFlag = FALSE;

                    if (u8DVROffCnt <= 3)
                    {
                        UART_DVR_TransProc();

                        u8DVROffCnt++;
                    }
                }
            }

            taskInfo[TASK_RS232_PROC].u16TaskStep++;
#endif
        }

        // if((bSystemStop == FALSE) && (bSD_ReportErrFlag == FALSE) && EXTIO_IsMainPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
        if ((bSystemStop == FALSE) && EXTIO_IsMainPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
        {
            if (HAL_GetTick() > (u32GetRX_Tick + UART_EXT_RX_CHK_TIME))
            {
                u32GetRX_Tick = HAL_GetTick();

#if ENABLE_MG82
                UART_EXT_GetRX();
#endif
            }
#if ENABLE_LSM6DSL
            if (HAL_GetTick() > (u32GetGSenTick + LSM6DSL_READ_INTERVAL))
            {
                u32GetGSenTick = HAL_GetTick();

                LSM6DSL_AccReadXYZ(&tAclrConfig, &tAccData);
                LSM6DSL_GyroReadXYZAngRate(&tGyroConfig, &tGyroData);
            }
#endif
            if (HAL_GetTick() >= (u32TickCnt + 1000))
            {
                u32TickCnt = HAL_GetTick();

                Debug_Proc();
            }

            if (bSD_ReportErrFlag == FALSE)
            {
#if ENABLE_PC_TRANS
                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                UART_PC_TransProc();
#endif
            }

            taskInfo[TASK_RS232_PROC].u16TaskStep++;

            UART_TerminalProc();

#if ENABLE_SPEED_INT_REC
            if (Datalog_IsRecordMode(REC_SPD_INT))
            {
                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                Datalog_RecordSpeedInt();
            }
#endif

#if ENABLE_PULSE_COUNT_REC
            if (Datalog_IsRecordMode(REC_PULSE_CNT))
            {
                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                Datalog_RecordPulseCount();
            }
#endif

#if ENABLE_GPS_SPEED_REC
            if (Datalog_IsRecordMode(REC_GPS_SPD))
            {
                taskInfo[TASK_RS232_PROC].u16TaskStep++;

                Datalog_RecordGPS_Speed();
            }
#endif

            /*
            if(HAL_GetTick() >= (u32TickCnt2 + 1000))
            {
                u32TickCnt2 = HAL_GetTick();

                UBaseType_t ubaseVal;


                ubaseVal = uxTaskGetStackHighWaterMark(defaultTaskHandle);

                RTOSprintf("\r\n> Default task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskLogProcHandle);

                RTOSprintf("\r\n> LogProc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskEXTIO_ProcHandle);

                RTOSprintf("\r\n> EXTIO_Proc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskGPS_ProcHandle);

                RTOSprintf("\r\n> GPS_Proc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskGSM_ProcHandle);

                RTOSprintf("\r\n> GSM_Proc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskLCD_ProcHandle);

                RTOSprintf("\r\n> LCD_Proc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskRS232_ProcHandle);

                RTOSprintf("\r\n> RS232_Proc task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskHighLoadHandle);

                RTOSprintf("\r\n> HighLoad task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskSD_ProcHandle);

                RTOSprintf("\r\n> SD_Handle task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskGSM_RXHandle);

                RTOSprintf("\r\n> GSM_RX task minimum unused stack size:%d", ubaseVal);

                ubaseVal = uxTaskGetStackHighWaterMark(taskGSM_TXHandle);

                RTOSprintf("\r\n> GSM_TX task minimum unused stack size:%d", ubaseVal);
            }
            */
        }

        osDelay(10);
    }
    /* USER CODE END Start_taskRS232_Proc */
}

/* USER CODE BEGIN Header_Start_taskHighLoad */
/**
 * @brief Function implementing the taskHighLoad thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskHighLoad */
void Start_taskHighLoad(void const *argument)
{
    /* USER CODE BEGIN Start_taskHighLoad */
    uint32_t u32ReloadTick;

    u32ReloadTick = HAL_GetTick();

    while (!bIsCheckVDRProcFlag)
    {
        if (HAL_GetTick() > (u32ReloadTick + S_2_mS(1)))
        {
            System_TaskWdgRefresh(TASK_HIGH_LOAD);

            u32ReloadTick = HAL_GetTick();
        }

        osDelay(1);
    }

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_HIGH_LOAD);

        if ((bSystemStop == TRUE) || (bSystemMode == SYSTEM_MODE_STEALTH) ||
            (bSD_ReportErrFlag == TRUE) || (sdRequest.sdCmd == SD_MKFS))
        {
            osDelay(10);

            continue;
        }

        if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) >= 10) && (bIsUSBHoldFlag == FALSE))
        {
            if (bUSB_Ready == TRUE)
            {
                // char cUSB_Event[20] = {0};
                uint32_t u32SpeedTemp;

                u32SpeedTemp = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);

                RTOSprintf("\r\n> ### USB Hold ON. Speed: %d ###", u32SpeedTemp);

                bIsUSBHoldFlag = TRUE;

                // sprintf(cUSB_Event, "SPD:%d", u32SpeedTemp);

                // #if ENABLE_GSM_NET
                // GSM_TCP_ReportEvent((char *)EVENT_USB_HOLD_ON, cUSB_Event);
                // #endif
            }
        }

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if ((M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed) <= 3) && (bIsUSBHoldFlag == TRUE))
        {
            if (bUSB_Ready == TRUE)
            {
                uint32_t u32SpeedTemp;

                u32SpeedTemp = M_2_KM(g_SysInfo.tSpeedInfo.u32RealSpeed);

                RTOSprintf("\r\n> ### USB Hold OFF. Speed: %d ###", u32SpeedTemp);

                bIsUSBHoldFlag = FALSE;

                // #if ENABLE_GSM_NET
                // GSM_TCP_ReportEvent((char *)EVENT_USB_HOLD_OFF, STR_NULL);
                // #endif
            }
        }

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if (EXTIO_IsMainPowerNormal() && (bDoVDR_HourFlag == FALSE) && (bDoVDR_DayFlag == FALSE) && (procCreateHourData == PROC_STEP_NULL))
        {
            USB_Proc();
        }

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if (bSystemTimeReady == FALSE)
        {
            osDelay(10);

            continue;
        }

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if (EXTIO_IsMainPowerNormal() && (bSD_ReportErrFlag == FALSE))
        {
            if (bDoVDR_HourFlag == TRUE)
            {
                V161_MakeHourVDR();
            }
            else if (procCreateHourData != PROC_STEP_NULL)
            {
                V161_CreateHourDataProc(u8HourDataCnt);
            }
        }

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if ((bSD_ReportErrFlag == FALSE) && (bDoVDR_HourFlag == FALSE) && (bDoVDR_DayFlag == FALSE) && (procCreateHourData == PROC_STEP_NULL))
            PRN_Proc();

        taskInfo[TASK_HIGH_LOAD].u16TaskStep++;

        if ((bSD_ReportErrFlag == FALSE) && (bDoHourVDRFlag == TRUE))
        {
            procCreateHourData = PROC_STEP_1;

            bDoHourVDRFlag = FALSE;
        }

        osDelay(2);
    }
    /* USER CODE END Start_taskHighLoad */
}

/* USER CODE BEGIN Header_Start_taskSD_Proc */
/**
 * @brief Function implementing the taskSD_Proc thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskSD_Proc */
void Start_taskSD_Proc(void const *argument)
{
    /* USER CODE BEGIN Start_taskSD_Proc */

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_SD_PROC);

        SD_RequestHandle();

        osDelay(2);
    }
    /* USER CODE END Start_taskSD_Proc */
}

/* USER CODE BEGIN Header_Start_taskGSM_RX */
/**
 * @brief Function implementing the taskGSM_RX thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskGSM_RX */
void Start_taskGSM_RX(void const *argument)
{
    /* USER CODE BEGIN Start_taskGSM_RX */
#if ENABLE_GSM_NET
    char cDebugLog[20] = {0};

    sprintf(cDebugLog, "GSM Module Init");

    GPRS_DebugLog(cDebugLog, strlen(cDebugLog));

    GSM_Reset();

#endif

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_GSM_RX);

#if ENABLE_GSM_NET
        if ((bSystemStop == FALSE) && EXTIO_IsPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
        {
            GSM_RX_Proc();
        }
#endif

        osDelay(2);
    }
    /* USER CODE END Start_taskGSM_RX */
}

/* USER CODE BEGIN Header_Start_taskGSM_TX */
/**
 * @brief Function implementing the taskGSM_TX thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_taskGSM_TX */
void Start_taskGSM_TX(void const *argument)
{
    /* USER CODE BEGIN Start_taskGSM_TX */

    /* Infinite loop */
    for (;;)
    {
        System_TaskWdgRefresh(TASK_GSM_TX);

#if ENABLE_GSM_NET
        if ((bSystemStop == FALSE) && EXTIO_IsPowerNormal() && (sdRequest.sdCmd != SD_MKFS))
        {
            GSM_TX_Proc();
        }
#endif

        osDelay(2);
    }
    /* USER CODE END Start_taskGSM_TX */
}

/* timerRTOS_GSM_BlockCallback function */
void timerRTOS_GSM_BlockCallback(void const *argument)
{
    /* USER CODE BEGIN timerRTOS_GSM_BlockCallback */

#if ENABLE_GSM_NET
    GSM_BlockCallBackHandle();
#endif
    /* USER CODE END timerRTOS_GSM_BlockCallback */
}

/* timerRTOS_GSM_NonBlockCallback function */
void timerRTOS_GSM_NonBlockCallback(void const *argument)
{
/* USER CODE BEGIN timerRTOS_GSM_NonBlockCallback */
#if ENABLE_GSM_NET
    GSM_NonBlockCallBackHandle();
#endif

    /* USER CODE END timerRTOS_GSM_NonBlockCallback */
}

/* timerRTOS_EXTI_Callback function */
void timerRTOS_EXTI_Callback(void const *argument)
{
    /* USER CODE BEGIN timerRTOS_EXTI_Callback */
    EXTIO_CallBackHandle();

    /* USER CODE END timerRTOS_EXTI_Callback */
}

/* timerRTOS_PCTransCallBack function */
void timerRTOS_PCTransCallBack(void const *argument)
{
    /* USER CODE BEGIN timerRTOS_PCTransCallBack */
    UART_PC_TransCallBackHandle();

    /* USER CODE END timerRTOS_PCTransCallBack */
}

/* timerACC_OffCallBack function */
void timerACC_OffCallBack(void const *argument)
{
    /* USER CODE BEGIN timerACC_OffCallBack */
    if ((HW_PBGetState(ACC_IN) == 0) || (g_SysInfo.tVehicleState.bPWR_ON == FALSE))
    {
        bLCD_TurnOff = TRUE;
        /*
        #if ENABLE_GSM_NET
        if((procGSM_FOTA_Step == PROC_STEP_NULL) &&
             (procGSM_Upload_Step == PROC_STEP_NULL))
        #endif
        {
            #if ENABLE_GSM_NET
            //QtEC21_TurnOffPwrKey();
            #endif

            //GPS_PowerOff();

            HW_WriteBackupRegister(BKP_SLEEP_CHK, BKP_SLEEP_VALUE);

            bRTOS_En = FALSE;

            LCD_Stop();

            printf("\r\n> ### SYSTEM ENTER STANDBY MODE ###");

            System_Reset();
        }
        #if ENABLE_GSM_NET
        else
        {
          //bWaitingEnterStandby = FALSE;
            // Turn off screen code here


        }
        #endif
        */
    }
    /* USER CODE END timerACC_OffCallBack */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM3 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM3)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    printf("\r\n ERROR HANDLER RESET SYSTEM!");

    for (int i = 0; i < MAX_SOCKET_BANK; i++)
    {
        GSM_TCP_SendEVENT_Direct((char *)EVENT_SYSTEM_RESET, (char *)"10", i);
    }

    System_Reset();

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
