CC=clang
JAVA=java
CFLAGS=-Wall -g -I. -I.. -Igenerated

.PHONY: generated

objects =  vending_machine.o product_store_sm.o activity_change_algorithm.o product_store.o  display_with_coinbank.o main.o  timer.o fifo.o

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

vending_machine.o : generated
	$(CC) $(CFLAGS) generated/vending_machine.c -c -o vending_machine.o
	
activity_change_algorithm.o : generated
	$(CC) $(CFLAGS) generated/activity_change_algorithm.c -c -o activity_change_algorithm.o

product_store_sm.o : generated
	$(CC) $(CFLAGS) generated/product_store_sm.c -c -o product_store_sm.o

generated:
	$(MAKE) -C generated


clean:
	$(MAKE) -C generated clean
	rm -f *.o
	touch  vending_machine.asta
