CC=gcc
JAVA=java
CFLAGS=-Wall -g

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main -p UModel

objects = oven.o main.o oven_hlp.o oven_selftest.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c oven.xmi
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hlp.o : oven_hlp.c oven.xmi
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven_selftest.o : oven_selftest.c oven.xmi
	$(CC) $(CFLAGS) oven_selftest.c -c -o oven_selftest.o

oven.c : oven.xmi
	$(JAVA) $(JFLAGS) -l cx -t "MicrowaveOven:MachineClass" -doxygen -o oven oven.xmi
	$(JAVA) $(JFLAGS) -A -l cx -t "MicrowaveOven:MachineClass" -doxygen -o oven oven.xmi 

clean:
	rm -f  oven *.o
	touch *.xmi
