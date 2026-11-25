/*
 * Ending the QEMU run from the target, through Arm semihosting (QEMU is
 * started with -semihosting-config enable=on). On a board without a
 * debugger attached, the breakpoint instruction this uses would fault.
 */
#ifndef SEMIHOSTING_H
#define SEMIHOSTING_H

/* End QEMU: with exit code 0 for status 0, 1 otherwise. Does not return. */
void qemuExit(int status) __attribute__((noreturn));

#endif
