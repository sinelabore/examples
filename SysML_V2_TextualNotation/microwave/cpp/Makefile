# The C++ target of ../oven.sysml. The model and the drawings are one level up; only
# what is specific to C++ is here.
#
# Windows (Developer Command Prompt): nmake -f Makefile.nmake
# Remote Windows Docker (keeps the image): ./msvc_docker.sh

CC=clang++
JAVA=java
CFLAGS=-Wall -g -I. -I../.. -std=c++17 -pthread -Wall -Wno-unused-variable -Wpedantic
GEN_SYS_NAME=system
INPUT_SYSML_FILE=../oven.sysml

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

all : oven

oven : $(objects)
	$(CC) $(LFLAGS) -o oven $(objects)

main.o : $(GEN_SYS_NAME).h main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o

# -d puts the SYSML_TRACE lines in: one per action and one per accept, each naming
# the instance that ran it. The 'd' key in main.cpp turns them on and off.
$(GEN_SYS_NAME).h : $(INPUT_SYSML_FILE) $(CODEGEN_JAR)
	$(JAVA) $(JFLAGS) -l cppx -p sysml2text -o $(GEN_SYS_NAME) $(INPUT_SYSML_FILE)

run : oven
	./oven

clean:
	rm -f *.o
	rm -f $(GEN_SYS_NAME).*
	rm -f oven

ref:
	./oven > tst.ref
