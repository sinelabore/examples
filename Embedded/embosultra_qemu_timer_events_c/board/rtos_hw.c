/*
 * What embOS-Ultra needs from the hardware, for QEMU's netduinoplus2
 * (STM32F405): the functions RTOS.h declares for the board, OS_InitHW(),
 * OS_Idle(), BSP_OS_GetCycles() and BSP_OS_StartTimer(), plus the SysTick
 * handler and OS_Error().
 *
 * embOS-Ultra has no periodic tick. It reads the time from a free-running
 * counter and programs a one-shot timer for its next deadline:
 *
 * - The counter is TIM2. QEMU clocks it at 1 GHz whatever the clock setup
 *   says; with prescaler 10 it counts at 100 MHz, and wraps after 42.9 s.
 *   BSP_OS_GetCycles() extends it to 64 bits. (QEMU has no DWT cycle
 *   counter, which an STM32 board would use.)
 * - The one-shot timer is SysTick at the core clock, 168 MHz in QEMU. Its
 *   24 bits reach 99.9 ms; a longer deadline fires early and embOS sets the
 *   timer again. So the counter is read at least every 100 ms, which keeps
 *   its extension right.
 */
#include "RTOS.h"
#include "stm32f405.h"
#include "uart.h"
#include "semihosting.h"

#define CORE_HZ     168000000U          /* QEMU's core and SysTick clock */
#define TIM2_IN_HZ  1000000000U         /* QEMU's TIM2 input clock */
#define COUNTER_HZ  100000000U          /* TIM2 after the prescaler: embOS's cycles */

#define SYSTICK_MAX 0xFFFFFFU           /* SysTick counts 24 bits */

/* embOS interrupts run at priorities 128 to 255; the STM32 uses the upper
 * 4 bits. PendSV, the scheduler, gets the lowest from embOS itself. */
#define PRIO_SYSTICK 0xE0U

/* RTOS.h: the buffer for embOSView over J-Link. Not used: embOSView talks
 * to this board over USART3. */
const OS_U32 OS_JLINKMEM_BufferSize = 0U;

OS_U64 BSP_OS_GetCycles(void)
{
    static uint32_t high;               /* TIM2 wraps counted */
    static uint32_t last;               /* TIM2 at the previous call */
    const uint32_t primask = irqSave();
    const uint32_t now = TIM2_CNT;
    OS_U64 cycles;
    if (now < last) {
        high++;
    }
    last = now;
    cycles = ((OS_U64)high << 32) | now;
    irqRestore(primask);
    return cycles;
}

void BSP_OS_StartTimer(OS_U32 Cycles)
{
    /* counter cycles at 100 MHz to SysTick ticks at 168 MHz */
    uint64_t ticks = ((uint64_t)Cycles * CORE_HZ) / COUNTER_HZ;
    if (ticks > SYSTICK_MAX) {
        ticks = SYSTICK_MAX;            /* fires early; embOS sets the timer again */
    }
    if (ticks == 0U) {
        ticks = 1U;
    }
    SYST_CSR = 0U;
    SYST_RVR = (uint32_t)ticks;
    SYST_CVR = 0U;
    SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
    /* One shot: SysTick_Handler stops the counter. Clearing the reload
     * value here instead, the usual way on hardware, can make QEMU's
     * SysTick fire again and again: it may not have loaded the reload value
     * yet. */
}

void SysTick_Handler(void)
{
    SYST_CSR = 0U;                      /* one shot: stop */
    (void)BSP_OS_GetCycles();           /* keeps the counter's extension current */
    OS_INT_EnterNestable();
    OS_TICK_Handle();                   /* embOS: timeouts, software timers, the next deadline */
    OS_INT_LeaveNestable();
}

void OS_InitHW(void)
{
    OS_SYSTIMER_CONFIG timerConfig = { COUNTER_HZ };

    OS_INT_IncDI();
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2_PSC = (TIM2_IN_HZ / COUNTER_HZ) - 1U;
    TIM2_ARR = 0xFFFFFFFFU;
    TIM2_EGR = TIM_EGR_UG;              /* load the prescaler */
    TIM2_CR1 = TIM_CR1_CEN;

    OS_TIME_ConfigSysTimer(&timerConfig);
    SCB_SHPR3 = (SCB_SHPR3 & 0x00FFFFFFU) | ((uint32_t)PRIO_SYSTICK << 24);
    BSP_OS_StartTimer(0xFFFFFFFFU);

    uartViewInit();                     /* embOSView on USART3 */
    OS_INT_DecRI();
}

/* No task is ready. WFI sleeps until the next interrupt; with
 * -icount shift=auto,sleep=on QEMU then waits in real time. */
void OS_Idle(void)
{
    for (;;) {
        __asm volatile ("wfi");
    }
}

/* embOS's debug library reports a misuse of its API here (the codes are in
 * RTOS.h, OS_ERR_...). Print it and end the run. */
void OS_Error(OS_STATUS ErrCode)
{
    static const char text[] = "embOS error ";
    char digits[6];
    unsigned code = (unsigned)ErrCode;
    int n = 0;
    do {
        digits[n++] = (char)('0' + (code % 10U));
        code /= 10U;
    } while ((code != 0U) && (n < 5));
    uartConsoleWrite(text, sizeof text - 1U);
    while (n > 0) {
        uartConsoleWrite(&digits[--n], 1U);
    }
    uartConsoleWrite("\n", 1U);
    qemuExit(2);
}
