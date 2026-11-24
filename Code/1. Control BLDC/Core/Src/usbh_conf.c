/*
 * usbh_conf.c
 *
 *  Low level glue between the ST USB Host Library and the HAL HCD driver for
 *  the STM32F429I-DISC1 USB OTG connector CN6. Adapted from ST's
 *  STM32CubeF4 example Projects/STM32F429I-Discovery/Applications/USB_Host
 *  (usbh_conf.c):
 *
 *    USB_OTG_HS core, embedded PHY -> Full Speed only
 *    PB14 = OTG_HS_DM, PB15 = OTG_HS_DP (AF12), PB13 = VBUS sense
 *    PC4  = VBUS power switch STMPS2141 enable, ACTIVE LOW
 *           (LOW = 5 V on CN6 for the gamepad). LD5 green = VBUS on.
 *
 *  Only used by FC_VARIANT_GAMEPAD; harmless (never called) otherwise.
 */
/*
 * OVERVIEW
 *   This is the "glue" (low level, LL) layer between the hardware-independent
 *   ST USB Host core and the STM32 HAL HCD (Host Controller Driver):
 *     library -> hardware : USBH_LL_xxx() functions below call HAL_HCD_xxx().
 *     hardware -> library : the OTG interrupt runs HAL_HCD_IRQHandler(), which
 *                           calls HAL_HCD_xxx_Callback() below, which forward
 *                           events to USBH_LL_xxx() of the core.
 *
 *   Why OTG_HS in Full Speed mode? On the STM32F429I-DISC1 the user USB
 *   connector CN6 is wired to the OTG_HS peripheral's pins PB14/PB15, but
 *   without an external ULPI high-speed PHY. OTG_HS then uses its embedded
 *   Full Speed PHY: 12 Mbit/s, plenty for a gamepad (FS and LS devices).
 *
 *   Pins:
 *     PB14 = D-, PB15 = D+   (alternate function AF12)
 *     PB13 = VBUS sense      (input only; plain GPIO here)
 *     PC4  = enable of the STMPS2141 power switch that feeds 5 V to CN6.
 *            ACTIVE LOW: PC4 = 0 -> 5 V on, PC4 = 1 -> off.
 *            NOTE: PC4 used to be BLDC3_VCC in the old wiring; in the
 *            GAMEPAD build nothing else may be connected to PC4.
 *
 *   Who calls what:
 *     Gamepad_Task (usbTask) -> USBH_Init/Start/Process -> USBH_LL_xxx here.
 *     OTG_HS_IRQHandler (interrupt, priority 6) -> HAL -> callbacks here.
 *
 *   Interrupt priority 6: on Cortex-M a LOWER number = MORE urgent. FreeRTOS
 *   only allows ISRs with priority number >= configLIBRARY_MAX_SYSCALL (5) to
 *   call "FromISR" APIs, and masks them during critical sections. 6 is
 *   therefore safe (and the ISR here calls no RTOS API at all), while staying
 *   below more time-critical interrupts.
 */
#include "main.h"
#include "usbh_core.h"

#define HOST_POWERSW_PORT   GPIOC
#define HOST_POWERSW_VBUS   GPIO_PIN_4

HCD_HandleTypeDef hhcd_USB_OTG_HS;   /* HAL handle; linked both ways to the USBH handle via pData */

/* ------------------------------------------------------------------------- */
/* MSP                                                                       */
/* ------------------------------------------------------------------------- */
/* MSP = "MCU Support Package": board-specific part of HAL_HCD_Init().
 * Called by HAL_HCD_Init(): clocks, GPIO, interrupt. */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
    GPIO_InitTypeDef gpio = {0};

    if (hhcd->Instance != USB_OTG_HS)
        return;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* DM / DP */
    gpio.Pin = GPIO_PIN_14 | GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &gpio);

    /* VBUS sense */
    gpio.Pin = GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio);

    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

    /* Power switch, start DISABLED (pin high) */
    /* (Write the level BEFORE switching the pin to output, so it never
     *  glitches low and powers the port too early.) */
    HAL_GPIO_WritePin(HOST_POWERSW_PORT, HOST_POWERSW_VBUS, GPIO_PIN_SET);
    gpio.Pin = HOST_POWERSW_VBUS;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HOST_POWERSW_PORT, &gpio);

    /* Priority 6: below configMAX_SYSCALL (5), the ISR calls no RTOS API */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{
    if (hhcd->Instance == USB_OTG_HS)
    {
        HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    }
}

/* Vector table entry (overrides the weak default in the startup file) */
void OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}

/* ------------------------------------------------------------------------- */
/* HCD -> library callbacks                                                  */
/* ------------------------------------------------------------------------- */
/* All of these run in interrupt context. hhcd->pData points to the
 * USBH_HandleTypeDef (set in USBH_LL_Init). */
/* SOF every 1 ms: advances phost->Timer and calls the class SOFProcess (GP_SOF) */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer(hhcd->pData);
}

void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Connect(hhcd->pData);
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Disconnect(hhcd->pData);
}

void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_PortEnabled(hhcd->pData);
}

void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_PortDisabled(hhcd->pData);
}

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum,
                                         HCD_URBStateTypeDef urb_state)
{
    (void)hhcd; (void)chnum; (void)urb_state;   /* polled, no OS sync needed */
    /* (With USBH_USE_OS = 1 this would post a message to wake the USB
     *  thread; here usbTask simply re-checks URB states every 1 ms.) */
}

/* ------------------------------------------------------------------------- */
/* Library -> HCD                                                            */
/* ------------------------------------------------------------------------- */
/* Called by USBH_Init(): configure the OTG_HS core as a host.
 * Host_channels = 11: hardware channels available to open pipes (2 for the
 * control pipe + up to 2 for the pad). No DMA, SOF output pin not used. */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
    hhcd_USB_OTG_HS.Instance = USB_OTG_HS;
    hhcd_USB_OTG_HS.Init.Host_channels = 11;
    hhcd_USB_OTG_HS.Init.dma_enable = 0;
    hhcd_USB_OTG_HS.Init.low_power_enable = 0;
    hhcd_USB_OTG_HS.Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd_USB_OTG_HS.Init.Sof_enable = 0;
    hhcd_USB_OTG_HS.Init.speed = HCD_SPEED_HIGH;   /* core limits to FS (embedded PHY) */
    hhcd_USB_OTG_HS.Init.use_external_vbus = 1;   /* VBUS comes from the external switch (PC4) */

    hhcd_USB_OTG_HS.pData = phost;
    phost->pData = &hhcd_USB_OTG_HS;

    if (HAL_HCD_Init(&hhcd_USB_OTG_HS) != HAL_OK)
        return USBH_FAIL;

    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hhcd_USB_OTG_HS));
    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_HCD_DeInit(phost->pData);
    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Start(phost->pData);
    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Stop(phost->pData);
    return USBH_OK;
}

/* HAL speed code (0 HS, 1 FS, 2 LS) -> library enum. With the embedded PHY
 * only FS or LS can actually occur. */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    switch (HAL_HCD_GetCurrentSpeed(phost->pData))
    {
    case 0:  return USBH_SPEED_HIGH;
    case 1:  return USBH_SPEED_FULL;
    case 2:  return USBH_SPEED_LOW;
    default: return USBH_SPEED_FULL;
    }
}

USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    HAL_HCD_ResetPort(phost->pData);
    return USBH_OK;
}

uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t epnum,
                                    uint8_t dev_address, uint8_t speed, uint8_t ep_type,
                                    uint16_t mps)
{
    HAL_HCD_HC_Init(phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_HCD_HC_Halt(phost->pData, pipe);
    return USBH_OK;
}

/* Start one transfer on a pipe; its progress is then read with
 * USBH_LL_GetURBState() (non-blocking). */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction,
                                     uint8_t ep_type, uint8_t token, uint8_t *pbuff,
                                     uint16_t length, uint8_t do_ping)
{
    HAL_HCD_HC_SubmitRequest(phost->pData, pipe, direction, ep_type, token, pbuff, length, do_ping);
    return USBH_OK;
}

USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState(phost->pData, pipe);
}

/* Switch the 5 V supply of CN6. state: 1 = on, 0 = off (the library uses
 * these values). The 200 ms wait lets VBUS rise and the device power up
 * before the core looks for a connection. */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
    (void)phost;
    /* STMPS2141 enable is active LOW */
    HAL_GPIO_WritePin(HOST_POWERSW_PORT, HOST_POWERSW_VBUS,
                      (state == 0U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    USBH_Delay(200);
    return USBH_OK;
}

/* DATA0/DATA1 toggle: USB alternates the data PID on every successful
 * packet so a repeated (retried) packet can be detected. The class driver
 * resets it to DATA0 after opening a pipe. */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
    HCD_HandleTypeDef *h = phost->pData;

    if (h->hc[pipe].ep_is_in)
        h->hc[pipe].toggle_in = toggle;
    else
        h->hc[pipe].toggle_out = toggle;
    return USBH_OK;
}

uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HCD_HandleTypeDef *h = phost->pData;

    return h->hc[pipe].ep_is_in ? h->hc[pipe].toggle_in : h->hc[pipe].toggle_out;
}

/* Delay in ms requested by the library (e.g. after port reset, VBUS on).
 * Must never be called from an interrupt. */
void USBH_Delay(uint32_t Delay)
{
    /* Called from the USB task once the scheduler runs: do not busy-wait */
    /* osDelay() blocks only usbTask and lets the flight loop and other tasks
     * run; HAL_Delay() would spin and starve lower-priority tasks (usbTask is
     * AboveNormal). Before the scheduler starts osDelay() is not allowed, so
     * HAL_Delay() is the fallback. */
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
        osDelay(Delay);
    else
        HAL_Delay(Delay);
}
