CC=clang
JAVA=java
CFLAGS=-Wall -g -I. -I..
JFLAGS= -Djava.ext.dirs="../../lib/":"/Applications/astahcommunity/astahcommunity.app/Contents/Java/" -Djava.awt.headless=true -jar ../../codegen.jar

objects = main.o  timer.o fifo.o vending_machine.o product_store.o display_with_coinbank.o product_store_sm.o activity_change_algorithm.o

vending_machine_exe : $(objects)
	$(CC) $(LFLAGS) -o vending_machine_exe $(objects)

main.o : main.c 
	$(CC) $(CFLAGS) main.c -c -o main.o

product_store.o : product_store.c 
	$(CC) $(CFLAGS) product_store.c -c -o product_store.o
	
fifo.o : ../lib/fifo.c 
	$(CC) $(CFLAGS) ../lib/fifo.c -c -o fifo.o

timer.o	: ../lib/timer.c 
	$(CC) $(CFLAGS) ../lib/timer.c -c -o timer.o
	
display_with_coinbank.o : display_with_coinbank.c 
	$(CC) $(CFLAGS) display_with_coinbank.c -c -o display_with_coinbank.o

vending_machine.o : vending_machine.c
	$(CC) $(CFLAGS) vending_machine.c -c -o vending_machine.o
	
activity_change_algorithm.o : activity_change_algorithm.c
	$(CC) $(CFLAGS) activity_change_algorithm.c -c -o activity_change_algorithm.o

product_store_sm.o : product_store_sm.c
	$(CC) $(CFLAGS) product_store_sm.c -c -o product_store_sm.o

vending_machine.c : vending_machine.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o vending_machine -t "machine:vending_machine:ui"  vending_machine.asta

product_store_sm.c : vending_machine.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o product_store_sm -t "machine:product_store_sm:sm"  vending_machine.asta

activity_change_algorithm.c : vending_machine.asta
	$(JAVA) $(JFLAGS) -A -l cx -v -p ASTAH -o activity -t "machine:coin_store:change_algorithm"  vending_machine.asta



clean:
	rm -f *.o
	touch  vending_machine.asta
