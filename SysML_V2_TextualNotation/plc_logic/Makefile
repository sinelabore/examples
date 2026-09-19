CC=clang++
JAVA=java
CFLAGS=-Wall -g -I. -I.. -std=c++17 -pthread -Wall -Wno-unused-variable -Wpedantic
GEN_SYS_NAME=system
INPUT_SYSML_FILE=test.sysml 74xxx.sysml IEC61131_3.sysml LogicBasic.sysml

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

#set the class path to where the code generator is installed.
#Note: make/gmake does not support spaces in path names!!!
#Linux/Mac convention for classpath
JFLAGS=-cp "$(CODEGEN_PATH)/*" codegen.Main

objects = main.o

# The drawings this model produces, built by a plain 'make' along with the testcase:
# the network of blocks, and the state chart of one of the timers.
diagrams = TestOne.svg TestTOn.svg TestToff.svg TestFallingEdge.svg TestCountUp.svg TestCounter4.svg ShiftReg4.svg Decoder2to4.svg SRLatch.svg FullAdder.svg

all : testcase $(diagrams)

testcase : $(objects)
	$(CC) $(LFLAGS) -o testcase $(objects)

main.o : $(GEN_SYS_NAME).h main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o

# Generated without -d, so the tables main.cpp prints stay readable. Add -d to put the
# SYSML_TRACE lines in: one per action, accept and transition, each naming the instance
# that ran it.
$(GEN_SYS_NAME).h : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -l cppx -p sysml2text -o $(GEN_SYS_NAME) $(INPUT_SYSML_FILE)

svg: $(diagrams)

# -t names the part; the file is named after what is drawn in it. TestOne has no state
# machine of its own, so -t on it draws what it is made of: the blocks, their ports and
# what connects them.
TestOne.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestTOn -pc none -l svg -p sysml2text -o TestOne $(INPUT_SYSML_FILE)

# The same part again, with the text of each block in it. A block that has a state machine of
# its own would draw that instead, but none of the targets here asks for one.
TestTOn.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestTOn -l svg -p sysml2text -o TestTOn $(INPUT_SYSML_FILE)

TestToff.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestToff -l svg -p sysml2text -o TestToff $(INPUT_SYSML_FILE)

TestFallingEdge.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestFallingEdge -l svg -p sysml2text -o TestFallingEdge $(INPUT_SYSML_FILE)

TestCountUp.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestCountUp -l svg -p sysml2text -o TestCountUp $(INPUT_SYSML_FILE)

TestCounter4.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestCounter4 -pcd 2 -l svg -p sysml2text -o Counter4 $(INPUT_SYSML_FILE)


ShiftReg4.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestShiftReg4 -pcd 0 -l svg -p sysml2text -o ShiftReg4 $(INPUT_SYSML_FILE)


Decoder2to4.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestDecoder2to4 -pcd 2 -l svg -p sysml2text -o Decoder2to4 $(INPUT_SYSML_FILE)


SRLatch.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t BoolTests::TestSRLatch -pcd 2 -l svg -p sysml2text -o SRLatch $(INPUT_SYSML_FILE)

# The gate network of one bit. TestAdder4 as a whole is four of these plus a counter, which is
# a true but unreadable picture; the bit is what there is to look at.
FullAdder.svg : $(INPUT_SYSML_FILE)
	$(JAVA) $(JFLAGS) -t Logic74xxx::FullAdder -l svg -p sysml2text -o FullAdder $(INPUT_SYSML_FILE)


clean:
	rm -f *.o
	rm -f $(GEN_SYS_NAME).*
	rm -f $(diagrams)
	rm -rf testcase

ref:
	./testcase > tst.ref
