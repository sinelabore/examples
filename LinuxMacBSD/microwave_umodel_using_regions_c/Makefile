ediCC=gcc
JAVA=java
CFLAGS=-Wall -g -O2

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main -p UModel

objects = oven.o main.o oven_hlp.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c oven.xmi
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hlp.o : oven_hlp.c oven.xmi
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.c : oven.xmi
	$(JAVA) $(JFLAGS) -l cx -t "MicrowaveOvenWithRegions:MachineClass" -o oven oven.xmi 

clean:
	rm -f oven *.o
	touch *.xmi
