CC=gcc
JAVA=java
CFLAGS=-Wall -g

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main 

objects = oven.o main.o oven_hlp.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.c oven.c oven.h oven.plantuml
	$(CC) $(CFLAGS) main.c -c -o main.o

oven.o : oven.c oven.plantuml
	$(CC) $(CFLAGS) oven.c -c -o oven.o

oven_hlp.o : oven_hlp.c oven.plantuml
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.c : oven.plantuml
	$(JAVA) $(JFLAGS) -Lstates -v -p plantuml -o oven  oven.plantuml

clean:
	rm -f oven *.o 
	touch *.plantuml
