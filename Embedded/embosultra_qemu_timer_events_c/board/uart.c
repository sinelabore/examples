#include "uart.h"
#include "RTOS.h"
#include "stm32f405.h"

/* embOS interrupts run at priorities 128 to 255 (upper 4 bits). */
#define PRIO_CONSOLE 0xC0U
#define PRIO_VIEW    0xD0U

/* ---- USART1, the console ---- */

static UART_RX_FN consoleRx;

void uartConsoleInit(UART_RX_FN onRx)
{
    consoleRx = onRx;
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    USART_CR1(USART1_BASE) = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    nvicEnable(IRQ_USART1, PRIO_CONSOLE);
}

void uartConsoleWrite(const char *s, size_t n)
{
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    USART_CR1(USART1_BASE) |= USART_CR1_UE | USART_CR1_TE;
    while (n-- > 0U) {
        while ((USART_SR(USART1_BASE) & USART_SR_TXE) == 0U) {
        }
        USART_DR(USART1_BASE) = (uint8_t)*s++;
    }
}

/* A key: hand it to the application, which turns it into an event. */
void USART1_IRQHandler(void)
{
    OS_INT_Enter();
    while ((USART_SR(USART1_BASE) & USART_SR_RXNE) != 0U) {
        const uint8_t c = (uint8_t)USART_DR(USART1_BASE);    /* reading clears RXNE */
        if (consoleRx != NULL) {
            consoleRx(c);
        }
    }
    OS_INT_Leave();
}

/* ---- USART3, embOSView ---- */

void uartViewInit(void)
{
    RCC_APB1ENR |= RCC_APB1ENR_USART3EN;
    USART_CR1(USART3_BASE) = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    nvicEnable(IRQ_USART3, PRIO_VIEW);
}

/* embOS sends a byte of its answer: write it, and let the transmit
 * interrupt ask for the next one. Called by embOS, from a task or from
 * OS_COM_OnTx() in the interrupt below. */
void OS_COM_Send1(OS_U8 c)
{
    const uint32_t primask = irqSave();     /* CR1 is shared with the interrupt */
    USART_DR(USART3_BASE) = c;
    USART_CR1(USART3_BASE) |= USART_CR1_TXEIE;
    irqRestore(primask);
}

void USART3_IRQHandler(void)
{
    uint32_t sr;
    OS_INT_Enter();
    sr = USART_SR(USART3_BASE);
    while ((sr & USART_SR_RXNE) != 0U) {
        OS_COM_OnRx((OS_U8)USART_DR(USART3_BASE));          /* a byte of embOSView's request */
        sr = USART_SR(USART3_BASE);
    }
    if (((sr & USART_SR_TXE) != 0U) && ((USART_CR1(USART3_BASE) & USART_CR1_TXEIE) != 0U)) {
        if (OS_COM_OnTx() != 0U) {      /* nothing more to send */
            const uint32_t primask = irqSave();
            USART_CR1(USART3_BASE) &= ~USART_CR1_TXEIE;
            irqRestore(primask);
        }
    }
    OS_INT_Leave();
}
