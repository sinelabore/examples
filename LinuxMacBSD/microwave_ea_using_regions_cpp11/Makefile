CC=g++
JAVA=java
CFLAGS=-Wall -pedantic -Wextra --std=c++11 -I.

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set classpath to the codegen jar files.
JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main  

objects = oven.o main.o

oven.exe : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects) -pthread

main.o : main.cpp oven.cpp oven.h ovenbase.h SimpleTimer.hpp SafeQueue.hpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o

oven.o : oven.cpp oven.xml ovenbase.h 
	$(CC) $(CFLAGS) oven.cpp -c -o oven.o

oven.cpp : oven.xml
	$(JAVA) $(JFLAGS) -l cppx -p EA -t "Model:implementation:oven" -o oven  oven.xml

clean:
	rm -f oven oven.exe *.o
	touch oven.xml
