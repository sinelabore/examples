CC=cl.exe
JAVA=java.exe
CFLAGS=/W4

JFLAGS=-cp "../../../bin/*" codegen.Main

objects = oven_hlp.obj oven.obj main.obj

oven.exe : $(objects)
	$(CC) $(LFLAGS) $(objects) /link /out:oven.exe

main.obj : main.c oven.c oven.h
	$(CC) $(CFLAGS) -c main.c 

oven_hlp.obj : oven_hlp.c first_example_step3.xml
	$(CC) $(CFLAGS) -c oven_hlp.c 

oven.obj : oven.c first_example_step3.xml
	$(CC) $(CFLAGS) -c oven.c

oven.c	: first_example_step3.xml
	$(JAVA) $(JFLAGS) -p EA -t "Model:Class Model:first_example_step3" -o oven  first_example_step3.xml
	
clean:
	del oven.exe
	del $(objects) oven.c
