CC=g++
JAVA=java
CFLAGS=-Wall -g -pedantic -pedantic -Wextra --std=c++11 -Wall

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set classpath to the codegen jar files.
JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main  

objects = oven.o main.o oven_hlp.o ovenbase.o

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : main.cpp oven.cpp oven.h
	$(CC) $(CFLAGS) main.cpp -c -o main.o

ovenbase.o : ovenbase.cpp
	$(CC) $(CFLAGS) ovenbase.cpp -c -o ovenbase.o

oven.o : oven.cpp oven.xml
	$(CC) $(CFLAGS) oven.cpp -c -o oven.o

oven_hlp.o : oven_hlp.c oven.xml
	$(CC) $(CFLAGS) oven_hlp.c -c -o oven_hlp.o

oven.cpp : oven.xml
	$(JAVA) $(JFLAGS) -l cppx -p EA -t "Model:implementation:oven" -o oven  oven.xml

clean:
	rm -f oven  *.o
	touch oven.xml
