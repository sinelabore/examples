CC=gcc
JAVA=java
CFLAGS=-Wall -g

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main -p CADIFRA

objects = oven.o main.o oven_hlp.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c first_example_step3.cdd
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hlp.o : oven_hlp.c first_example_step3.cdd
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.c : first_example_step3.cdd
	$(JAVA) $(JFLAGS) -l cx -doxygen -o oven first_example_step3.cdd 

clean:
	rm -f oven *.o
	touch *.cdd
