CC=gcc
JAVA=java
CFLAGS=-Wall -g -I.

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set the class path to where  astah is installed.
#Note: make/gmake does not support spaces in path names!!!
#Linux/Mac convention for classpath
JFLAGS=-cp $(CODEGEN_PATH)/*:../*:/Applications/astahsysml/astahsysml.app/Contents/Java/* codegen.Main

objects = main.o oven.o oven_hlp.o oven_selftest.o

oven.exe : $(objects)
	$(CC) $(LFLAGS) -o oven.exe $(objects)

main.o : main.c oven.c
	$(CC) $(CFLAGS) main.c -c -o main.o

oven_hlp.o : oven_hlp.c oven_model.asml
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.o : oven.c oven_model.asml oven_selftest.c
	$(CC) $(CFLAGS) oven.c -c -o oven.o
	
oven_selftest.o : oven_selftest.c oven_model.asml
	$(CC) $(CFLAGS) oven_selftest.c -c -o oven_selftest.o

oven.c : oven_model.asml
	$(JAVA) -Djava.awt.headless=true  $(JFLAGS) -l cx -v -p ASTAH -o oven -t "final:oven:machine"  oven_model.asml

oven_selftest.c : oven_model.asml oven.c
	$(JAVA) -Djava.awt.headless=true  $(JFLAGS) -A -l cx -v -p ASTAH -o oven -t "final:oven:selftest"  oven_model.asml

clean:
	rm -f *.o
	touch  oven_model.asml
