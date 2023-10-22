PGM=${ARCH}/oven.exe
JFLAGS=-Djava.ext.dirs=../../../bin/ -jar -ea "../../../bin/codegen.jar"
JAVA=/opt/jdk/bin/java

# optional managers required
MANAGERS=all

# C source names
CSRCS = init.c oven.c oven_hlp.c
COBJS = ${ARCH}/init.o $(ARCH)/oven.o  $(ARCH)/oven_hlp.o

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

CFLAGS+=-I../example1

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

oven.c : oven.cdd
	$(JAVA) $(JFLAGS) -p CADIFRA -o oven oven.cdd 

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
	
image:
	cat ${ARCH}/oven.exe | gzip -9 > ${ARCH}/kernel.gz
	mcopy ${ARCH}/kernel.gz a:
	qemu -curses -fda ~/floppy.img
