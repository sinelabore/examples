CC=clang
JAVA=java
CFLAGS=-Wall -g -I. -I..
CLASS_PATH = -cp "../bin/*" codegen.Main

objects =  test.o main.o  timer.o fifo.o

test : $(objects)
	$(CC) $(LFLAGS) -lpthread -o test $(objects)

main.o : main.c 
	$(CC) $(CFLAGS) main.c -c -o main.o
	
test.o : test.xml test.c
	$(CC) $(CFLAGS) test.c -c -o test.o
	
test.c : test.xml 
	$(JAVA) $(CLASS_PATH) -p ssc -o test test.xml

fifo.o : ../lib/fifo.c 
	$(CC) $(CFLAGS) ../lib/fifo.c -c -o fifo.o

timer.o	: ../lib/timer.c 
	$(CC) $(CFLAGS) ../lib/timer.c -c -o timer.o
	

clean:
	rm -f *.o test.exe test.c test.h test_ext.h test_dbg.h
	touch  test.xml
