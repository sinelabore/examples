CC=cl.exe
JAVA=java
CFLAGS=/W4

#set the class path to where  astah is installed.
#Note: make/gmake does not support spaces in path names!!!
#Windows convention for classpath
JFLAGS=-cp "../../../bin/*;../*;C:\Program Files\astah-professional\*" codegen.Main



objects = oven_hlp.obj oven_selftest.obj oven.obj main.obj

oven.exe : $(objects)
	$(CC) $(LFLAGS) $(objects) /link /out:oven.exe

main.obj : main.c oven.c oven.h
	$(CC) $(CFLAGS) -c main.c 

oven_selftest.obj : oven_selftest.c oven_model.asta
	$(CC) $(CFLAGS) -c oven_selftest.c 

oven_hlp.obj : oven_hlp.c oven_model.asta
	$(CC) $(CFLAGS) -c oven_hlp.c 

oven.obj : oven.c oven_model.asta
	$(CC) $(CFLAGS) -c oven.c

oven.c : oven_model.asta
	$(JAVA) $(JFLAGS) -l cx -v -p ASTAH -o oven -t "final:oven:machine"  oven_model.asta

oven_selftest.c : oven_model.asta oven.c
	$(JAVA) $(JFLAGS) -A -l cx -v -p ASTAH -o oven -t "final:oven:selftest"  oven_model.asta

clean:
	del *.exe *.obj
