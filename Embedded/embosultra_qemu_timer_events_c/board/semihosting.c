#include "semihosting.h"

#define SYS_EXIT                  0x18U
#define ADP_STOPPED_APP_EXIT      0x20026U  /* QEMU exits with 0 */
#define ADP_STOPPED_RUNTIME_ERROR 0x20023U  /* QEMU exits with 1 */

void qemuExit(int status)
{
    /* On 32-bit Arm, SYS_EXIT takes the reason code itself in r1. */
    register unsigned r0 __asm__("r0") = SYS_EXIT;
    register unsigned r1 __asm__("r1") = (status == 0) ? ADP_STOPPED_APP_EXIT : ADP_STOPPED_RUNTIME_ERROR;
    __asm volatile ("bkpt 0xAB" : "+r" (r0) : "r" (r1) : "memory");
    for (;;) {
    }
}
