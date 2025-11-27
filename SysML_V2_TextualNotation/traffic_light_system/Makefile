CC=clang++
JAVA=java
CFLAGS=-Wall -g -I.  -std=c++17 -pthread -Wall -Wpedantic
GEN_SYS_NAME=system
INPUT_SYSML_FILE=tl.sysml

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set the class path to where  astah is installed.
#Note: make/gmake does not support spaces in path names!!!
#Linux/Mac convention for classpath
JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

objects = main.o TrafficLightControllerSM.o TrafficManagementCenterSM.o

testcase : $(objects)
	$(CC) $(LFLAGS) -o testcase $(objects)

main.o : $(GEN_SYS_NAME).h main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o

TrafficLightControllerSM.o : TrafficLightControllerSM.cpp TrafficLightControllerSM.h $(GEN_SYS_NAME).h
	$(CC) $(CFLAGS) TrafficLightControllerSM.cpp -c -o TrafficLightControllerSM.o

TrafficManagementCenterSM.o : TrafficManagementCenterSM.cpp TrafficManagementCenterSM.h $(GEN_SYS_NAME).h
	$(CC) $(CFLAGS) TrafficManagementCenterSM.cpp -c -o TrafficManagementCenterSM.o

$(GEN_SYS_NAME).h : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -d -l cppx -p sysml2text -o $(GEN_SYS_NAME) $(INPUT_SYSML_FILE)

clean:
	rm -f *.o
	rm -f $(GEN_SYS_NAME).*
	rm -rf TrafficLightControllerSM*.*
	rm -rf TrafficManagementCenterSM*.*
	rm -rf testcase

ref:
	./testcase > tst.ref
