/*
 * The C library's (newlib-nano) connection to the board: printf() writes to
 * the console, the heap lies between __heap_start__ and __heap_end__, and
 * exit() ends QEMU. Every other system call is newlib's stub (nosys.specs).
 *
 * Only the machine task prints, so newlib needs no locks here. Several
 * printing tasks would need __malloc_lock() and friends, or a mutex.
 */
#include <errno.h>
#include <stddef.h>
#include "uart.h"
#include "semihosting.h"

extern char __heap_start__, __heap_end__;    /* from the linker script */

int _write(int fd, const char *buf, int len)
{
    if ((fd != 1) && (fd != 2)) {
        errno = EBADF;
        return -1;
    }
    uartConsoleWrite(buf, (size_t)len);
    return len;
}

void *_sbrk(ptrdiff_t increment)
{
    static char *brk = &__heap_start__;
    char *const old = brk;
    if ((increment > 0) && (increment > (&__heap_end__ - brk))) {
        errno = ENOMEM;
        return (void *)-1;
    }
    brk += increment;
    return old;
}

void _exit(int status)
{
    qemuExit(status);
}
