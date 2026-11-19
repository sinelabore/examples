# Microwave oven on FreeRTOS, on the POSIX port (macOS, Linux): the model of
# LinuxMacBSD/microwave_builtin_editor_timer_c, generated with -l cx, with its
# timers on FreeRTOS software timers (lib/timer_service/freertos).
#
#   make            fetches the FreeRTOS kernel on first use, then builds ./oven
#   ./oven          keys + - o c p P q
#
# FREERTOS_KERNEL names an existing checkout instead; otherwise the tag
# FREERTOS_TAG is cloned into ./FreeRTOS-Kernel. CODEGEN_PATH names the folder
# of codegen.jar. TIMER_SERVICE is the timer service library; its freertos
# folder holds the FreeRTOS version.
CC=cc
JAVA=java
FREERTOS_TAG ?= V11.3.3
FREERTOS_KERNEL ?= FreeRTOS-Kernel
TIMER_SERVICE ?= ../../lib/timer_service
TS_PORT = $(TIMER_SERVICE)/freertos

KERNEL = $(abspath $(FREERTOS_KERNEL))
PORT = $(KERNEL)/portable/ThirdParty/GCC/Posix
INCLUDES = -I$(CURDIR) -I$(TS_PORT) -I$(KERNEL)/include -I$(PORT) -I$(PORT)/utils
KERNEL_CFLAGS = -std=c11 -O1 -g -Wall $(INCLUDES)
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g $(INCLUDES)
LDLIBS = -lpthread

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS = -cp "$(CODEGEN_PATH)/*" codegen.Main

# The kernel and the POSIX port: tasks, queues, software timers, malloc as the heap.
KERNEL_SRCS = $(KERNEL)/tasks.c $(KERNEL)/queue.c $(KERNEL)/list.c $(KERNEL)/timers.c \
	$(KERNEL)/portable/MemMang/heap_3.c $(PORT)/port.c $(PORT)/utils/wait_for_event.c

objects = main.o oven.o oven_hlp.o freertos_timer_service.o
headers = oven.h oven_ext.h oven_hlp.h $(TS_PORT)/timer_service.h FreeRTOSConfig.h

oven : $(objects) build/libfreertos.a
	$(CC) $(LDFLAGS) -o oven $(objects) build/libfreertos.a $(LDLIBS)

%.o : %.c $(headers) $(KERNEL)/tasks.c
	$(CC) $(CFLAGS) -c $< -o $@

freertos_timer_service.o : $(TS_PORT)/freertos_timer_service.c $(headers) $(KERNEL)/tasks.c
	$(CC) $(CFLAGS) -c $< -o $@

oven.c oven.h oven_ext.h oven_dbg.h : oven.xml codegen.cfg
	$(JAVA) $(JFLAGS) -p ssc -l cx -o oven oven.xml

build/libfreertos.a : $(KERNEL)/tasks.c FreeRTOSConfig.h
	rm -rf build/kernel && mkdir -p build/kernel
	cd build/kernel && $(CC) $(KERNEL_CFLAGS) -c $(KERNEL_SRCS)
	ar rcs $@ build/kernel/*.o

$(KERNEL)/tasks.c :
	git clone --depth 1 --branch $(FREERTOS_TAG) https://github.com/FreeRTOS/FreeRTOS-Kernel.git $(KERNEL)

clean :
	rm -rf oven *.dSYM *.o build
	touch oven.xml

# also the fetched kernel
distclean : clean
	rm -rf FreeRTOS-Kernel

.PHONY : clean distclean
