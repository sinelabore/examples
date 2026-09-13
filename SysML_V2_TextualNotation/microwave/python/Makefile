# The Python target of ../oven.sysml. The model and the drawings are one level up; only
# what is specific to Python is here.
#
# framework.py is a symlink to the runtime beside framework.h, the way the Python
# testcases link theirs; do not remove it.

JAVA=java
PYTHON=python3
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

.PHONY: all run clean ref

# Python is not compiled, so "building" the target is generating the module.
all : $(GEN_SYS_NAME).py

# -d as for C++, so the two runs report the same actions and accepts.
$(GEN_SYS_NAME).py : $(INPUT_SYSML_FILE) $(CODEGEN_JAR)
	$(JAVA) $(JFLAGS) -l python -p sysml2text -o $(GEN_SYS_NAME) $(INPUT_SYSML_FILE)

run : all
	$(PYTHON) main.py

clean:
	rm -f $(GEN_SYS_NAME).py
	rm -rf __pycache__

ref: all
	$(PYTHON) main.py > tst.ref
