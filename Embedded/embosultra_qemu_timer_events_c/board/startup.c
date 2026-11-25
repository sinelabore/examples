/*
 * Reset and the vector table of the STM32F405.
 *
 * Reset copies the initialised data to RAM, clears the rest, switches the
 * FPU on (the embOS library and the compiler use it) and calls main().
 * Clocks are left as they come out of reset: QEMU runs the core at its own
 * rate whatever the clock registers say (see rtos_hw.c).
 */
#include <stdint.h>
#include <stdlib.h>
#include "stm32f405.h"
#include "semihosting.h"

extern uint32_t __data_load__, __data_start__, __data_end__;   /* from the linker script */
extern uint32_t __bss_start__, __bss_end__;
extern uint32_t __stack_end__;

int main(void);

/* The handlers this example has; every other vector goes to faultHandler. */
void PendSV_Handler(void);              /* the embOS scheduler, in the library */
void SysTick_Handler(void);             /* embOS's timer, rtos_hw.c */
void USART1_IRQHandler(void);           /* the console's keys, uart.c */
void USART3_IRQHandler(void);           /* embOSView, uart.c */

void Reset_Handler(void)
{
    const uint32_t *src = &__data_load__;
    uint32_t *dst;
    for (dst = &__data_start__; dst < &__data_end__; dst++) {
        *dst = *src++;
    }
    for (dst = &__bss_start__; dst < &__bss_end__; dst++) {
        *dst = 0U;
    }
    SCB_CPACR |= 0xFU << 20;            /* full access to CP10 and CP11, the FPU */
    __asm volatile ("dsb\n isb" : : : "memory");
    exit(main());
}

/* An exception this example does not expect: a fault or a stray interrupt.
 * On QEMU it ends the run with an error, so a test fails instead of hanging. */
static void faultHandler(void)
{
    qemuExit(3);
}

#define VECTORS_IRQ 82U                 /* the STM32F405's interrupts */

typedef void (*VECTOR_T)(void);

/* Every interrupt goes to faultHandler (a GNU range), except the two this
 * example uses, which override it below. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init"
__attribute__((section(".isr_vector"), used))
static const VECTOR_T vectors[16U + VECTORS_IRQ] = {
    [0] = (VECTOR_T)(uintptr_t)&__stack_end__,  /* initial stack pointer */
    [1] = Reset_Handler,
    [2] = faultHandler,                 /* NMI */
    [3] = faultHandler,                 /* HardFault */
    [4] = faultHandler,                 /* MemManage */
    [5] = faultHandler,                 /* BusFault */
    [6] = faultHandler,                 /* UsageFault */
    [11] = faultHandler,                /* SVCall */
    [12] = faultHandler,                /* DebugMonitor */
    [14] = PendSV_Handler,
    [15] = SysTick_Handler,
    [16 ... (16U + VECTORS_IRQ - 1U)] = faultHandler,
    [16U + IRQ_USART1] = USART1_IRQHandler,
    [16U + IRQ_USART3] = USART3_IRQHandler,
};
#pragma GCC diagnostic pop
