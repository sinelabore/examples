CC=cl.exe
JAVA=java
CFLAGS=/W4

!IF "$(CODEGEN_PATH)" == ""
!ERROR CODEGEN_PATH is not set. Please set it before running this Makefile.
!ENDIF

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

LFLAGS=

objects = oven_hlp.obj oven_selftest.obj oven.obj main.obj

oven.exe : $(objects)
	$(CC) $(LFLAGS) $(objects) /link /out:oven.exe

main.obj : main.c oven.c oven.h
	$(CC) $(CFLAGS) -c main.c 

oven_selftest.obj : oven_selftest.c oven.xmi
	$(CC) $(CFLAGS) -c oven_selftest.c 

oven_hlp.obj : oven_hlp.c oven.xmi
	$(CC) $(CFLAGS) -c oven_hlp.c 

oven.obj : oven.c oven.xmi
	$(CC) $(CFLAGS) -c oven.c

oven.c : oven.xmi
	$(JAVA) $(JFLAGS) -p Umodel -l cx -t "MicrowaveOven:MachineClass" -doxygen -o oven oven.xmi
	$(JAVA) $(JFLAGS) -p Umodel -A -l cx -t "MicrowaveOven:MachineClass" -o oven oven.xmi 

clean:
	del *.obj
	del oven.exe
