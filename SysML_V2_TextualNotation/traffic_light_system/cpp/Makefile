# The C++ target of ../tl.sysml and ../signalling.sysml. The model and the drawings are
# one level up; only what is specific to C++ is here.
#
#   make        generate system.h and build ./testcase
#   make run    build and run it
#   make ref    build, run and keep the output as tst.ref

CC=clang++
JAVA=java
CFLAGS=-Wall -g -I. -I../.. -std=c++17 -pthread -Wno-unused-variable -Wpedantic
GEN_SYS_NAME=system
INPUT_SYSML_FILE=../tl.sysml ../signalling.sysml

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

# A generated file depends on the generator, not only on the model: change the jar
# and nothing here would regenerate, so the example would build output from an older
# one. Via $(wildcard ...) so a missing jar leaves the prerequisite off and the Java
# error speaks for itself.
CODEGEN_JAR := $(wildcard $(CODEGEN_PATH)/codegen.jar)

objects = main.o

.PHONY: all run clean ref

all : testcase

testcase : $(objects)
	$(CC) $(LFLAGS) -o testcase $(objects)

main.o : $(GEN_SYS_NAME).h main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o

# Both .sysml files are listed on one command and land in one system.h. Anything only
# reached through import would stay bind-only, so signalling.sysml is named explicitly
# and 'Lamp' is generated with the rest.
#
# -d puts the SYSML_TRACE lines in: one per action and one per accept, each naming the
# instance that ran it. The Python target generates with -d too, so the two runs report
# the same events.
$(GEN_SYS_NAME).h : $(INPUT_SYSML_FILE) $(CODEGEN_JAR)
	$(JAVA) $(JFLAGS) -d -l cppx -p sysml2text -o $(GEN_SYS_NAME) $(INPUT_SYSML_FILE)

run : all
	./testcase

clean:
	rm -f *.o
	rm -f $(GEN_SYS_NAME).h
	rm -rf testcase

ref: all
	./testcase > tst.ref
