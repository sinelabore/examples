/*
 * The few STM32F405 and Cortex-M4 registers this example uses, from the
 * reference manual (RM0090) and the Cortex-M4 generic user guide. A real
 * project would include the vendor's CMSIS headers instead.
 */
#ifndef STM32F405_H
#define STM32F405_H

#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))
#define REG8(addr)  (*(volatile uint8_t *)(addr))

/* System control block and SysTick */
#define SCB_CPACR       REG32(0xE000ED88U)      /* coprocessor access: the FPU */
#define SCB_SHPR3       REG32(0xE000ED20U)      /* priorities of PendSV [23:16] and SysTick [31:24] */
#define SYST_CSR        REG32(0xE000E010U)      /* SysTick control and status */
#define SYST_RVR        REG32(0xE000E014U)      /* SysTick reload value */
#define SYST_CVR        REG32(0xE000E018U)      /* SysTick current value */
#define SYST_CSR_ENABLE     (1U << 0)
#define SYST_CSR_TICKINT    (1U << 1)
#define SYST_CSR_CLKSOURCE  (1U << 2)           /* the core clock */

/* Interrupt controller */
#define NVIC_ISER(n)    REG32(0xE000E100U + 4U * (n))   /* set enable, 32 interrupts each */
#define NVIC_IPR(irq)   REG8(0xE000E400U + (irq))       /* priority, upper 4 bits used */

/* Reset and clock control */
#define RCC_APB1ENR     REG32(0x40023840U)
#define RCC_APB2ENR     REG32(0x40023844U)
#define RCC_APB1ENR_TIM2EN    (1U << 0)
#define RCC_APB1ENR_USART3EN  (1U << 18)
#define RCC_APB2ENR_USART1EN  (1U << 4)

/* TIM2, a 32-bit timer */
#define TIM2_BASE       0x40000000U
#define TIM2_CR1        REG32(TIM2_BASE + 0x00U)
#define TIM2_EGR        REG32(TIM2_BASE + 0x14U)
#define TIM2_CNT        REG32(TIM2_BASE + 0x24U)
#define TIM2_PSC        REG32(TIM2_BASE + 0x28U)
#define TIM2_ARR        REG32(TIM2_BASE + 0x2CU)
#define TIM_CR1_CEN     (1U << 0)
#define TIM_EGR_UG      (1U << 0)

/* USART1 and USART3 */
#define USART1_BASE     0x40011000U
#define USART3_BASE     0x40004800U
#define USART_SR(b)     REG32((b) + 0x00U)
#define USART_DR(b)     REG32((b) + 0x04U)
#define USART_BRR(b)    REG32((b) + 0x08U)
#define USART_CR1(b)    REG32((b) + 0x0CU)
#define USART_SR_RXNE   (1U << 5)
#define USART_SR_TXE    (1U << 7)
#define USART_CR1_RE    (1U << 2)
#define USART_CR1_TE    (1U << 3)
#define USART_CR1_RXNEIE (1U << 5)
#define USART_CR1_TXEIE (1U << 7)
#define USART_CR1_UE    (1U << 13)

/* Interrupt numbers */
#define IRQ_USART1      37U
#define IRQ_USART3      39U

/* Interrupts on and off around a few instructions (PRIMASK). */
static inline uint32_t irqSave(void)
{
    uint32_t primask;
    __asm volatile ("mrs %0, primask\n cpsid i" : "=r" (primask) : : "memory");
    return primask;
}

static inline void irqRestore(uint32_t primask)
{
    __asm volatile ("msr primask, %0" : : "r" (primask) : "memory");
}

static inline void nvicEnable(uint32_t irq, uint8_t priority)
{
    NVIC_IPR(irq) = priority;
    NVIC_ISER(irq / 32U) = 1U << (irq % 32U);
}

#endif
