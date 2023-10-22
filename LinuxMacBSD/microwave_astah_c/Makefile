CC=gcc
JAVA=java
CFLAGS=-Wall -g -I.

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set the class path to where  astah is installed.
#Note: make/gmake does not support spaces in path names!!!
#Linux/Mac convention for classpath
JFLAGS=-cp $(CODEGEN_PATH)/*:../*:/Applications/astahprofessional/astahprofessional.app/Contents/Java/* codegen.Main



objects = main.o oven.o oven_hlp.o oven_selftest.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c
	$(CC) $(CFLAGS) main.c -c -o main.o

oven_hlp.o : oven_hlp.c oven_model.asta
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.o : oven.c oven_model.asta oven_selftest.c
	$(CC) $(CFLAGS) oven.c -c -o oven.o
	
oven_selftest.o : oven_selftest.c oven_model.asta
	$(CC) $(CFLAGS) oven_selftest.c -c -o oven_selftest.o

oven.c : oven_model.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o oven -t "final:oven:machine"  oven_model.asta

oven_selftest.c : oven_model.asta oven.c
	$(JAVA) $(JFLAGS) -A -l cx -v -p ASTAH -o oven -t "final:oven:selftest"  oven_model.asta

clean:
	rm -f oven *.o
	touch  oven_model.asta
