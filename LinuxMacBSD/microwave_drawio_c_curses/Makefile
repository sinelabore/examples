CC=gcc
JAVA=java
CFLAGS=-Wall -g

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*"  codegen.Main 
LFLAGS=-lncurses

objects = oven.o main.o oven_hlp.o oven_hardwareTest.o 

oven : $(objects)
	$(CC)  -o oven $(objects) $(LFLAGS)

main.o : main.c oven.c oven.h oven.drawio
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hardwareTest.o : oven.c
	$(CC) $(CFLAGS) oven_hardwareTest.c -c -o oven_hardwareTest.o

oven_hlp.o : oven_hlp.c
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.c : oven.drawio
	$(JAVA) $(JFLAGS) -p drawio -l cx -o oven oven.drawio

	
	
clean:
	rm -f oven *.o
	touch *.drawio
