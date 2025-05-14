CC=gcc
JAVA=java
CFLAGS=-Wall -g
JFLAGS=-cp $(CODEGEN_PATH)/*: codegen.Main

objects=complex.o main.o
complex : $(objects)
	$(CC) $(LFLAGS) -o complex $(objects)

main.o : main.c complex.c complex.h
	$(CC) $(CFLAGS) main.c -c -o main.o

complex.o : complex.c complex.cdd
	$(CC) $(CFLAGS) complex.c -c -o complex.o


complex.c complex_trace.c : complex.cdd
	$(JAVA) $(JFLAGS) -Trace -p CADIFRA -o complex complex.cdd

clean:
	rm -f *.exe *.o
	touch complex.cdd
