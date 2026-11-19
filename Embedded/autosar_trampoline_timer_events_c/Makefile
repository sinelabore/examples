# Microwave oven on Trampoline, an AUTOSAR OS / OSEK implementation, on its
# POSIX target, with timeouts as events: the model of
# ../autosar_trampoline_timer_c with TimeoutsAsEvents, generated with -l cx,
# its timers on OS alarms (lib/timer_service/trampoline_events), and one
# queue for buttons and timeouts.
#
#   make            fetches and builds Trampoline's tools on first use, then
#                   builds ./oven
#   make run        starts it with ViPER, which drives the OS tick;
#                   a: one second more, b: open or close the door, q: quit
#
# ./oven finds ViPER through VIPER_PATH, which run sets; to start it
# yourself: VIPER_PATH=$PWD/trampoline/viper ./oven
#
# The first make clones Trampoline at TRAMPOLINE_COMMIT into ./trampoline and
# builds goil, its OIL compiler, with JOBS compile jobs (a few minutes), and
# ViPER. Tested on macOS (Apple Silicon); Linux should work by the same
# steps but was not tried. CODEGEN_PATH names the folder of codegen.jar (a
# version with TimeoutsAsEvents).
# The timer service is the library's Trampoline version for timeouts as
# events, TS_PORT; make links it as ./timer_service, the name microwave.oil
# uses (APP_SRC, CFLAGS). Through the link goil keeps its objects in build/:
# it places them by source path.
JAVA=java
TS_PORT ?= ../../lib/timer_service/trampoline_events
TRAMPOLINE_COMMIT ?= 09e2dcb1206fb6ebaa73c84558f26ac21f57b2dd
JOBS ?= 4

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS = -cp "$(CODEGEN_PATH)/*" codegen.Main

# goil's build directory and binary, and the OS target, for this host
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_S),Darwin)
  GOIL_BUILD = trampoline/goil/makefile-macosx
  ifeq ($(UNAME_M),arm64)
    GOIL = $(GOIL_BUILD)/arm64/goil
    GOIL_TARGET = posix/darwin/arm64
  else
    GOIL = $(GOIL_BUILD)/x86_64/goil
    GOIL_TARGET = posix/darwin
  endif
else
  GOIL_BUILD = trampoline/goil/makefile-unix
  GOIL = $(GOIL_BUILD)/goil
  GOIL_TARGET = posix/linux
endif
VIPER = trampoline/viper/viper

sources = main.c oven.c oven_hlp.c $(TS_PORT)/trampoline_events_timer_service.c
headers = oven.h oven_ext.h oven_hlp.h $(TS_PORT)/timer_service.h

oven : microwave.oil $(sources) $(headers) timer_service $(GOIL) $(VIPER)
	$(GOIL) --target=$(GOIL_TARGET) --templates=trampoline/goil/templates/ microwave.oil
	python3 build.py all $(JOBS)

run : oven
	VIPER_PATH=$(CURDIR)/trampoline/viper ./oven

timer_service :
	ln -sfn $(TS_PORT) timer_service

oven.c oven.h oven_ext.h oven_dbg.h : oven.xml codegen.cfg
	$(JAVA) $(JFLAGS) -p ssc -l cx -o oven oven.xml

# Trampoline has no release tags: a fixed commit.
trampoline/README.md :
	git init -q trampoline
	git -C trampoline fetch -q --depth 1 https://github.com/TrampolineRTOS/trampoline.git $(TRAMPOLINE_COMMIT)
	git -C trampoline checkout -q FETCH_HEAD

# goil builds with -Werror, and newer compilers warn about its code
# (clang 21: -Wunnecessary-virtual-specifier); -Wno-error keeps the warnings.
$(GOIL) : trampoline/README.md
	python3 -c "import pathlib; p = pathlib.Path('trampoline/goil/build/libpm/python-makefiles/default_build_options.py'); \
	  p.write_text(p.read_text().replace('result.append (\"-Werror\")', 'result.append (\"-Wno-error\")'))"
	cd $(GOIL_BUILD) && python3 build.py release $(JOBS)

$(VIPER) : trampoline/README.md
	$(MAKE) -C trampoline/viper

# what goil and its build write
clean :
	rm -rf oven oven.dSYM build microwave build.py make.py readTrace.py timer_service
	touch oven.xml

# also Trampoline and its tools
distclean : clean
	rm -rf trampoline

.PHONY : run clean distclean
