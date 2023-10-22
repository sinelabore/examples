CC=gcc
JAVA=java
CFLAGS=-Wall -g -pedantic

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main 

objects = oven.o main.o oven_hlp.o oven_cb.o observer.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h
	$(CC) $(CFLAGS) main.c -c -o main.o

observer.o : observer.c
	$(CC) $(CFLAGS) observer.c -c -o observer.o

oven.o : oven.c oven.xml
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_cb.o :  oven.xml
	$(CC) $(CFLAGS) oven_cb.c -c -o oven_cb.o

oven_hlp.o : oven_hlp.c oven.xml
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.c : oven.xml
	$(JAVA) $(JFLAGS) -p ssc -o oven  oven.xml

clean:
	rm -f oven *.o
	touch *.xml
