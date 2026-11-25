/*
 * The two serial ports of the example:
 *
 * - USART1, the console (QEMU's first serial port, on the terminal): keys
 *   come in through its receive interrupt and are handed to a function of
 *   the application; printf() writes to it, waiting for each character.
 * - USART3, embOSView (QEMU's third serial port, on a TCP port): embOS's
 *   own protocol, driven by its interrupt.
 *
 * QEMU ignores baud rates and pins, so neither is set up here; on a board
 * the GPIO alternate functions and BRR would be.
 */
#ifndef UART_H
#define UART_H

#include <stddef.h>
#include <stdint.h>

/* Called in the receive interrupt, inside OS_INT_Enter()/OS_INT_Leave(),
 * once per received character: it may call embOS functions an interrupt
 * may call, e.g. OS_MAILBOX_Put(). */
typedef void (*UART_RX_FN)(uint8_t c);

/* Start the console; each received character goes to onRx. Call it after
 * OS_InitHW(). */
void uartConsoleInit(UART_RX_FN onRx);

/* Write to the console, waiting until each character is taken. Works
 * anywhere, also before uartConsoleInit() and in OS_Error(). */
void uartConsoleWrite(const char *s, size_t n);

/* Start the embOSView channel. Called by OS_InitHW(). */
void uartViewInit(void);

#endif
