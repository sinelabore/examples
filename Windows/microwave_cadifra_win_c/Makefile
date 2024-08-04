CC=cl.exe
JAVA=java
CFLAGS=/W4

!IF "$(CODEGEN_PATH)" == ""
!ERROR CODEGEN_PATH is not set. Please set it before running this Makefile.
!ENDIF

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

objects = oven_hlp.obj oven.obj main.obj

oven.exe : $(objects)
	$(CC) $(LFLAGS) $(objects) /link /out:oven.exe

main.obj : main.c oven.c oven.h
	$(CC) $(CFLAGS) -c main.c 

oven_hlp.obj : oven_hlp.c first_example_step3.cdd
	$(CC) $(CFLAGS) -c oven_hlp.c 

oven.obj : oven.c first_example_step3.cdd
	$(CC) $(CFLAGS) -c oven.c
	
oven.c : first_example_step3.cdd
	$(JAVA) $(JFLAGS) -L ../../bin/License.txt -p Cadifra -l cx -doxygen -o oven first_example_step3.cdd
	
clean:
	del oven.exe
	del $(objects) oven.c

	