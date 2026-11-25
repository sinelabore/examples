/*
 * STM32F405 as QEMU's netduinoplus2 emulates it: 1 MB flash, 128 KB RAM.
 *
 * embOS needs __stack_start__ and __stack_end__ around the system stack
 * (main, interrupts, software timers, the idle loop), and the vector table's
 * first entry must be __stack_end__. The heap for newlib lies between
 * __heap_start__ and __heap_end__.
 */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

STACK_SIZE = 4K;
HEAP_SIZE  = 8K;

SECTIONS
{
    .isr_vector : {
        KEEP(*(.isr_vector))
    } > FLASH

    .text : {
        *(.text .text.*)
        *(.rodata .rodata.*)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH

    .preinit_array : {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > FLASH
    .init_array : {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > FLASH
    .fini_array : {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    __data_load__ = LOADADDR(.data);
    .data : {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data .data.*)
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM AT > FLASH

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD) : {
        . = ALIGN(8);
        __heap_start__ = .;
        . += HEAP_SIZE;
        __heap_end__ = .;
    } > RAM

    .stack (NOLOAD) : {
        . = ALIGN(8);
        __stack_start__ = .;
        . += STACK_SIZE;
        __stack_end__ = .;
    } > RAM

    end = __heap_start__;               /* for newlib */
}
