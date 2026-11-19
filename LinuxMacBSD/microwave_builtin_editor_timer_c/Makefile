# Microwave oven with the reference timer service: `make`, then `./oven` and the
# keys + - o c p P q. CODEGEN_PATH names the folder of codegen.jar;
# TIMER_SERVICE the timer service library.
CC=gcc
JAVA=java
TIMER_SERVICE ?= ../../lib/timer_service
CFLAGS=-Wall -Wextra -g -I. -I$(TIMER_SERVICE)

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

objects = oven.o main.o oven_hlp.o timer_service.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h oven_hlp.h
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c oven_hlp.h
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hlp.o : oven_hlp.c oven_hlp.h
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

timer_service.o : $(TIMER_SERVICE)/timer_service.c $(TIMER_SERVICE)/timer_service.h
	$(CC) $(CFLAGS) $(TIMER_SERVICE)/timer_service.c -c -o timer_service.o

oven.c oven.h : oven.xml codegen.cfg
	$(JAVA) $(JFLAGS) -p ssc -l cx -o oven oven.xml

clean:
	rm -f oven *.o
	touch *.xml

.PHONY: clean
