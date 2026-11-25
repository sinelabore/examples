# The firmware, built in the container: called by the Makefile with the
# example at /w, the timer service at /ts and the embOS download at /embos.
EMBOS ?= /embos
TIMER_SERVICE ?= /ts
TS_PORT = $(TIMER_SERVICE)/embos_ultra_events
OUT = build

CC = arm-none-eabi-gcc
CPU = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
# OS_LIBMODE_DT: the debug library with profiling and API trace, for embOSView
CFLAGS = $(CPU) -std=gnu11 -Og -g -Wall -Wextra -ffunction-sections -fdata-sections \
         -DOS_LIBMODE_DT -I. -Iboard -I$(TS_PORT) -I$(EMBOS)/Start/Inc
LIB = $(EMBOS)/Start/Lib/libosT7VLDT.a
LDFLAGS = $(CPU) -T board/stm32f405_qemu.ld --specs=nano.specs --specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map,$(OUT)/oven.map

SRCS = main.c oven.c oven_hlp.c $(TS_PORT)/embos_ultra_events_timer_service.c \
       board/startup.c board/rtos_hw.c board/uart.c board/syscalls.c board/semihosting.c
OBJS = $(addprefix $(OUT)/,$(notdir $(SRCS:.c=.o)))
HEADERS = $(wildcard *.h board/*.h) $(TS_PORT)/timer_service.h

vpath %.c . board $(TS_PORT)

$(OUT)/oven.elf : $(OBJS) $(LIB) board/stm32f405_qemu.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIB)

$(OUT)/%.o : %.c $(HEADERS) | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT) :
	mkdir -p $@
