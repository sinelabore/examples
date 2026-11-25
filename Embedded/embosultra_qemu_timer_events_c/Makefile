# Microwave oven on embOS-Ultra, on an STM32F405 emulated by QEMU, with
# timeouts as events: the model of ../freertos_posix_timer_events_c,
# generated with -l cx, its timers on embOS-Ultra software timers
# (lib/timer_service/embos_ultra_events), keys from a receive interrupt.
#
#   make image      the Docker image: Arm GNU Toolchain 11.2 and QEMU (once)
#   make            builds build/oven.elf in the container
#   make run        runs it; this terminal is the board's console. Keys + - o c p P, Ctrl-C ends it
#   make view       as run, and embOSView can connect: Ethernet, 127.0.0.1:50021
#   make test       a scripted run, compared with test/expected.txt
#   make shell      a shell in the container
#   make clean
#
# EMBOS names your embOS-Ultra download for Cortex-M and GCC (the folder
# embOS_Ultra_CortexM_GCC_Obj_SFL_V5); RTOS.h and the library are used from
# there, nothing of it is copied. CODEGEN_PATH names the folder of codegen.jar
# (a version with TimeoutsAsEvents); it is needed only when oven.xml changes.
# TIMER_SERVICE is the timer service library; its embos_ultra_events folder
# holds this version.
EMBOS ?=
IMAGE ?= embosultra-qemu:11.2
TIMER_SERVICE ?= ../../lib/timer_service
JAVA = java
VIEW_UDP ?= 50021
VIEW_TCP ?= 4444

LIBFILE = $(EMBOS)/Start/Lib/libosT7VLDT.a
DOCKER = docker run --rm --user $(shell id -u):$(shell id -g) -v "$(CURDIR)":/w \
         -v "$(abspath $(TIMER_SERVICE))":/ts:ro -v "$(abspath $(EMBOS))":/embos:ro -w /w
# -icount: the emulated time follows the executed instructions, and the idle
# loop's WFI waits in real time, so the board keeps pace with the clock.
QEMU = qemu-system-arm -M netduinoplus2 -display none -monitor none \
       -icount shift=auto,sleep=on -semihosting-config enable=on,target=native \
       -kernel build/oven.elf
# QEMU's serial ports: USART1 the console, USART2 unused, USART3 embOSView
SERIAL_RUN = -serial stdio -serial null -serial null
SERIAL_VIEW = -serial stdio -serial null -serial tcp:0.0.0.0:4444,server=on,wait=off

all : oven.c check
	$(DOCKER) $(IMAGE) make --no-print-directory -f firmware.mk

run : all
	$(DOCKER) -it $(IMAGE) $(QEMU) $(SERIAL_RUN)

# QEMU's USART3 is published on VIEW_TCP; the bridge passes embOSView's UDP
# packets to it and back. Ctrl-C ends both.
view : all
	python3 -u tools/udp_uart_bridge.py --udp $(VIEW_UDP) --tcp $(VIEW_TCP) & bridge=$$!; \
	  trap 'kill $$bridge 2>/dev/null' EXIT INT TERM; \
	  $(DOCKER) -it -p 127.0.0.1:$(VIEW_TCP):4444 $(IMAGE) $(QEMU) $(SERIAL_VIEW)

test : all
	python3 test/run_test.py -- $(DOCKER) -i $(IMAGE) timeout 120 $(QEMU) $(SERIAL_RUN)

shell : check
	$(DOCKER) -it $(IMAGE) bash

image :
	docker build -t $(IMAGE) .

check :
	@test -n "$(EMBOS)" || { echo "Set EMBOS to your embOS-Ultra download, e.g. make EMBOS=~/embOS_Ultra_CortexM_GCC_Obj_SFL_V5"; exit 1; }
	@test -f "$(LIBFILE)" || { echo "$(LIBFILE) not found: EMBOS must name the embOS-Ultra download for Cortex-M and GCC"; exit 1; }

oven.c oven.h oven_ext.h oven_dbg.h : oven.xml codegen.cfg
	@test -n "$(CODEGEN_PATH)" || { echo "oven.xml changed: set CODEGEN_PATH to the folder of codegen.jar"; exit 1; }
	$(JAVA) -cp "$(CODEGEN_PATH)/*" codegen.Main -p ssc -l cx -o oven oven.xml

clean :
	rm -rf build

.PHONY : all run view test shell image check clean
