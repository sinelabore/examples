# Microwave oven in C++ with the reference timer service: `make`, then `./oven`
# and the keys + - o c p P q. The model is the one of
# ../microwave_builtin_editor_timer_c, generated with -l cppx. CODEGEN_PATH
# names the folder of codegen.jar; TIMER_SERVICE the timer service library.
CXX=c++
JAVA=java
TIMER_SERVICE ?= ../../lib/timer_service
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic -g -I. -I$(TIMER_SERVICE)

ifndef CODEGEN_PATH
$(error CODEGEN_PATH is not set. Please set it before running this Makefile.)
endif

JFLAGS= -cp "$(CODEGEN_PATH)/*" codegen.Main

objects = oven.o main.o oven_hlp.o
headers = oven.h oven_ext.h oven_hlp.h mydefs.h $(TIMER_SERVICE)/timer_service.hpp

oven : $(objects)
	$(CXX) $(LDFLAGS) -o oven $(objects)

main.o : main.cpp $(headers)
	$(CXX) $(CXXFLAGS) main.cpp -c -o main.o

oven.o : oven.cpp $(headers)
	$(CXX) $(CXXFLAGS) oven.cpp -c -o oven.o

oven_hlp.o : oven_hlp.cpp oven_hlp.h $(TIMER_SERVICE)/timer_service.hpp
	$(CXX) $(CXXFLAGS) oven_hlp.cpp -c -o oven_hlp.o

oven.cpp oven.h oven_ext.h : oven.xml codegen.cfg
	$(JAVA) $(JFLAGS) -p ssc -l cppx -o oven oven.xml

clean:
	rm -f oven *.o
	touch *.xml

.PHONY: clean
