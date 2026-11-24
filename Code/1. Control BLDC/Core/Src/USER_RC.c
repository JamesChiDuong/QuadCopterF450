/*
 * USER_RC.c
 *
 *  See USER_RC.h. The EXTI IRQ handlers are defined here (they override the
 *  weak defaults from the startup file), so no CubeMX change is required.
 *  If you later enable these EXTI lines in CubeMX, remove the generated
 *  handlers from stm32f4xx_it.c or these ones.
 */
/*
 * ============================================================================
 *  OVERVIEW - PWM RC receiver input (one wire per channel) + CPU cycle timer
 * ============================================================================
 *  1. What is an RC PWM signal?
 *     On each channel the receiver outputs a high pulse 1000..2000 us long,
 *     repeated about every 20 ms. Pulse width = stick / switch position
 *     (1000 us = one end, 1500 us = centre, 2000 us = other end).
 *
 *  2. Hardware
 *       PE2 -> channel 0 (CH1 roll)      PE3 -> channel 1 (CH2 pitch)
 *       PE4 -> channel 2 (CH3 throttle)  PE5 -> channel 3 (CH4 yaw)
 *       PE6 -> channel 4 (CH5 ARM switch)
 *     Every pin is an external interrupt (EXTI) on BOTH edges (rising and
 *     falling). Interrupt lines: PE2->EXTI2, PE3->EXTI3, PE4->EXTI4, PE5 and
 *     PE6 share EXTI9_5 (the STM32 groups lines 5..9 into one vector).
 *     No hardware timer is used - time is measured with DWT->CYCCNT.
 *
 *  3. DWT cycle counter
 *     The DWT (Data Watchpoint and Trace) unit of the Cortex-M4 has a 32-bit
 *     CYCCNT register that increments once per CPU cycle (72 MHz -> 1 tick
 *     = 13.9 ns). It wraps after 2^32 / 72e6 = 59.6 s. Thanks to unsigned
 *     subtraction (now - start) the result is still correct across a wrap,
 *     as long as the measured interval is < 59.6 s.
 *
 *  4. Measuring one pulse (in the ISR, see RC_Edge)
 *       - Rising edge : store the time, s_rise_cyc[ch] = CYCCNT.
 *       - Falling edge: width = (CYCCNT - s_rise_cyc) / cycles_per_us.
 *         Example: 108000 cycles / 72 = 1500 us.
 *       - Only pulses inside the validity window 900..2100 us
 *         (RC_PULSE_MIN/MAX_VALID_US) are accepted, rejecting noise and
 *         truncated pulses; a valid pulse also refreshes s_last_ms[ch].
 *
 *  5. Signal loss (timeout)
 *     RC_IsSignalOk() returns false if any channel never had a pulse, or its
 *     last valid pulse is older than RC_SIGNAL_TIMEOUT_MS (100 ms). The upper
 *     layer (USER_System.c) uses this for failsafe / disarm.
 *
 *  6. Who uses it?
 *     - RC_Init(): called once at start-up (the UART TEST build calls it too,
 *       to enable the DWT, because Time_Cycles() measures the control-loop dt).
 *     - EXTIx ISRs: run on every edge (~5 channels x 2 edges x 50 Hz =
 *       ~500 times/s).
 *     - BLDCTask (250 Hz) calls RC_GetUs()/RC_IsSignalOk() to get commands.
 *
 *  7. Note for the upper layer - EDGE logic of the ARM switch
 *     This module only reports a LEVEL (the current pulse width).
 *     USER_System.c uses s_arm_sw_prev so that it only arms on an OFF -> ON
 *     transition (rising edge), and it assumes "ON" at start. So if the board
 *     powers up with the switch already ON, or the RC link comes back while
 *     the switch is ON, it does NOT arm by itself - the pilot must flip the
 *     switch OFF and ON again. This is an important safety feature.
 *
 *  8. Interrupt priority
 *     RC_IRQ_PRIO = 6 (higher number = lower priority), i.e. below the FreeRTOS
 *     threshold configMAX_SYSCALL_INTERRUPT_PRIORITY (5) -> it cannot
 *     interrupt kernel critical sections. These ISRs make no FreeRTOS calls.
 * ============================================================================
 */
#include "USER_RC.h"
#include "main.h"

#define RC_GPIO_PORT    GPIOE
#define RC_IRQ_PRIO     6U

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
/* PE2..PE6 -> channel 0..4 */
static const uint16_t s_rc_pin[RC_NUM_CHANNELS] =
{
    GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6
};

static volatile uint32_t s_rise_cyc[RC_NUM_CHANNELS];   /* CYCCNT at the rising edge */
#endif
/* volatile: written in the ISR, read in a task -> the compiler must re-read RAM. */
static volatile uint16_t s_pulse_us[RC_NUM_CHANNELS];   /* last valid width (us), 0 = none yet */
static volatile uint32_t s_last_ms[RC_NUM_CHANNELS];    /* HAL_GetTick() of the last valid pulse */
static uint32_t s_cyc_per_us = 72U;                     /* CPU cycles per us */

/* Enables the DWT->CYCCNT cycle counter:
 *  - TRCENA in DEMCR must be set or the DWT block does not run,
 *  - CYCCNTENA starts the counter.
 * Also computes s_cyc_per_us from SystemCoreClock (72 MHz -> 72). */
static void Time_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    s_cyc_per_us = SystemCoreClock / 1000000U;
    if (s_cyc_per_us == 0U)
        s_cyc_per_us = 1U;
}

/* Reads the current CPU cycle counter (used to measure the control-loop dt). */
uint32_t Time_Cycles(void)
{
    return DWT->CYCCNT;
}

/* Cycles -> seconds. Example: 288000 cycles / 72e6 = 0.004 s (4 ms). */
float Time_CyclesToSec(uint32_t cycles)
{
    return (float)cycles / (float)SystemCoreClock;
}

/* Init: enable the DWT, clear channel data; in the RC FULL build also
 * configure PE2..PE6 as both-edge EXTI interrupts and enable their vectors.
 * Called once at start-up, before the scheduler runs. */
void RC_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    uint8_t i;

    Time_Init();

    for (i = 0; i < RC_NUM_CHANNELS; i++)
    {
        s_pulse_us[i] = 0;
        s_last_ms[i] = 0;
    }

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();   /* SYSCFG selects which port (PE) drives each EXTI line */

    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    /* Pull-down: with the receiver unplugged the pins sit at 0, produce no
     * false edges -> no valid pulses -> timeout -> "signal lost". */
    gpio.Pull = GPIO_PULLDOWN;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RC_GPIO_PORT, &gpio);

    HAL_NVIC_SetPriority(EXTI2_IRQn, RC_IRQ_PRIO, 0);
    HAL_NVIC_SetPriority(EXTI3_IRQn, RC_IRQ_PRIO, 0);
    HAL_NVIC_SetPriority(EXTI4_IRQn, RC_IRQ_PRIO, 0);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, RC_IRQ_PRIO, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
#else
    (void)gpio;   /* UART test build: no receiver, PE2..PE6 left untouched */
#endif
}

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
/* Handles one edge on channel ch (called from the EXTI ISR).
 * The pin level is read RIGHT AFTER the edge to tell a rising edge (pin = 1)
 * from a falling edge (pin = 0). CYCCNT is captured first to keep the error
 * from interrupt latency minimal (a few hundred ns, negligible vs 1 us). */
static void RC_Edge(uint8_t ch)
{
    uint32_t now = DWT->CYCCNT;

    if (RC_GPIO_PORT->IDR & s_rc_pin[ch])
    {
        s_rise_cyc[ch] = now;
    }
    else
    {
        /* Unsigned subtraction is correct even if CYCCNT wrapped between
         * the two edges. Example: 144000 cycles / 72 = 2000 us. */
        uint32_t width_us = (now - s_rise_cyc[ch]) / s_cyc_per_us;

        if ((width_us >= RC_PULSE_MIN_VALID_US) && (width_us <= RC_PULSE_MAX_VALID_US))
        {
            s_pulse_us[ch] = (uint16_t)width_us;
            s_last_ms[ch] = HAL_GetTick();
        }
    }
}
#endif

uint16_t RC_GetUs(uint8_t channel)
{
    if (channel >= RC_NUM_CHANNELS)
        return 0;
    return s_pulse_us[channel];
}

/* true when ALL channels had a valid pulse within the last
 * RC_SIGNAL_TIMEOUT_MS (100 ms). Builds without RC always return false.
 * (now - s_last_ms) is unsigned, so it is safe when the 32-bit tick wraps. */
bool RC_IsSignalOk(void)
{
#if (FC_VARIANT != FC_VARIANT_RC_FULL)
    return false;
#else
    uint32_t now = HAL_GetTick();
    uint8_t i;

    for (i = 0; i < RC_NUM_CHANNELS; i++)
    {
        if ((s_pulse_us[i] == 0U) || ((now - s_last_ms[i]) > RC_SIGNAL_TIMEOUT_MS))
            return false;
    }
    return true;
#endif
}

#if (FC_VARIANT == FC_VARIANT_RC_FULL)
/* ---------------------------------------------------------------------- */
/* IRQ handlers                                                           */
/* ---------------------------------------------------------------------- */
/* These handlers override the "weak" defaults in the startup file.
 * Each handler: check the line's pending flag in EXTI->PR, clear it by
 * WRITING 1 to that bit (PR is "write-1-to-clear"), then process the edge.
 * The flag is cleared first so an edge arriving during processing is not lost.
 * EXTI9_5 is shared by PE5 and PE6, so each bit must be checked. */
void EXTI2_IRQHandler(void)
{
    if (EXTI->PR & GPIO_PIN_2)
    {
        EXTI->PR = GPIO_PIN_2;
        RC_Edge(0);
    }
}

void EXTI3_IRQHandler(void)
{
    if (EXTI->PR & GPIO_PIN_3)
    {
        EXTI->PR = GPIO_PIN_3;
        RC_Edge(1);
    }
}

void EXTI4_IRQHandler(void)
{
    if (EXTI->PR & GPIO_PIN_4)
    {
        EXTI->PR = GPIO_PIN_4;
        RC_Edge(2);
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR & GPIO_PIN_5)
    {
        EXTI->PR = GPIO_PIN_5;
        RC_Edge(3);
    }
    if (EXTI->PR & GPIO_PIN_6)
    {
        EXTI->PR = GPIO_PIN_6;
        RC_Edge(4);
    }
}
#endif /* FC_VARIANT_RC_FULL */
