CC=gcc
JAVA=java
CFLAGS=-Wall -g -I.

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

objects = main.o oven.o oven_hlp.c

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h
	$(CC) $(CFLAGS) main.c -c -o main.o


oven_hlp.o : oven_hlp.c oven_hlp.h
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o


oven.o : oven.c oven.xmi.xml
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven.c : oven.xmi.xml
	$(JAVA) $(JFLAGS) -l cx -p md -t "Model:oven_package:oven" -o oven oven.xmi.xml 

clean:
	rm -f oven *.o
	touch oven.mdzip oven.xmi.xml
