CC=gcc
JAVA=java
CFLAGS=-Wall -g -I.

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main 

objects = main.o oven.o oven_hlp.o oven_selftest.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven.exe $(objects)

main.o : main.c oven.c
	$(CC) $(CFLAGS) main.c -c -o main.o

oven_hlp.o : oven_hlp.c oven.xmi
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.o : oven.c oven.xmi oven_selftest.c
	$(CC) $(CFLAGS) oven.c -c -o oven.o
	
oven_selftest.o : oven_selftest.c oven.xmi
	$(CC) $(CFLAGS) oven_selftest.c -c -o oven_selftest.o

oven.c : oven.xmi
	$(JAVA) $(JFLAGS) -l cx -v -p Modelio -o oven -t "OvenClass"  oven.xmi

oven_selftest.c : oven.xmi oven.c
	$(JAVA) $(JFLAGS) -A -l cx -v -p Modelio -o oven -t "OvenClass"  oven.xmi

clean:
	rm -f oven *.o
	touch  oven.xmi
