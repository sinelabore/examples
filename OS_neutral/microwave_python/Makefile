CC=gcc
JAVA=java
CFLAGS=-Wall -g

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

oven.py : oven.xml
	$(JAVA) $(JFLAGS) -l PYTHON -p ssc -o oven oven.xml

clean:
	rm -rf __pycache__ oven.py *.pyc
	touch *.xml
