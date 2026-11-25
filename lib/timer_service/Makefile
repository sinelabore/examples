# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Peter Mueller
# Unit checks of the reference timer service, C (C99; 32-, 64- and 16-bit
# ticks; the default error handler) and C++ (as C++11 and C++17; 16-bit
# ticks), and a compile check that a global C++ service is constant-
# initialized (C++20 constinit). The checks are in test/, what they build
# goes to build/.
#
#   make test       build and run every check
#   make coverage   line and branch coverage of both services
#   make doc        the API documentation with Doxygen, build/doc/html/index.html;
#                   fails on anything public left undocumented
#   make clean
CC ?= cc
CXX ?= c++

B = build
T = test
CCHECK = -std=c99 -Wall -Wextra -pedantic -I.
CXXCHECK = -Wall -Wextra -pedantic -I. -DSM_TIMER_ERROR_HANDLER=testError
C_DEPS = $(T)/test_timer_service.c timer_service.c timer_service.h | $(B)
CPP_DEPS = $(T)/test_timer_service.cpp timer_service.hpp | $(B)

test: $(B)/test_timer_service $(B)/test_timer_service_us $(B)/test_timer_service_16 $(B)/test_error_exit \
      $(B)/test_timer_service_cpp11 $(B)/test_timer_service_cpp17 $(B)/test_timer_service_cpp_16 $(B)/test_error_exit_cpp
	./$(B)/test_timer_service
	./$(B)/test_timer_service_us
	./$(B)/test_timer_service_16
	@./$(B)/test_error_exit 2>/dev/null; test $$? -eq 1 && echo "timer_service: the default error handler exits"
	./$(B)/test_timer_service_cpp11
	./$(B)/test_timer_service_cpp17
	./$(B)/test_timer_service_cpp_16
	@./$(B)/test_error_exit_cpp 2>/dev/null; test $$? -eq 1 && echo "timer_service.hpp: the default error handler exits"
	$(CXX) -std=c++20 -Wall -Wextra -pedantic -I. -fsyntax-only $(T)/check_constinit.cpp

$(B):
	mkdir -p $(B)

$(B)/test_timer_service: $(C_DEPS)
	$(CC) $(CCHECK) -DTS_ERROR_HANDLER=testError -o $@ $(T)/test_timer_service.c timer_service.c

# the same checks with 64-bit time in microseconds
$(B)/test_timer_service_us: $(C_DEPS)
	$(CC) $(CCHECK) -DTS_TICKS_TYPE=uint64_t -DTS_TICK_US=1U -DTS_ERROR_HANDLER=testError -o $@ $(T)/test_timer_service.c timer_service.c

# the same checks with 16-bit ticks: the operands are promoted to int, where a
# missing conversion back to the tick type would show
$(B)/test_timer_service_16: $(C_DEPS)
	$(CC) $(CCHECK) -DTS_TICKS_TYPE=uint16_t -DTS_ERROR_HANDLER=testError -o $@ $(T)/test_timer_service.c timer_service.c

# the default error handler, tsErrorExit(): a start without a service must end the program with 1
$(B)/test_error_exit: $(T)/test_error_exit.c timer_service.c timer_service.h | $(B)
	$(CC) $(CCHECK) -o $@ $(T)/test_error_exit.c timer_service.c

# the C++ service: the same checks as C++11, the oldest standard the generated
# C++ code uses, and as C++17; and with 16-bit ticks
$(B)/test_timer_service_cpp11: $(CPP_DEPS)
	$(CXX) -std=c++11 $(CXXCHECK) -o $@ $(T)/test_timer_service.cpp
$(B)/test_timer_service_cpp17: $(CPP_DEPS)
	$(CXX) -std=c++17 $(CXXCHECK) -o $@ $(T)/test_timer_service.cpp
$(B)/test_timer_service_cpp_16: $(CPP_DEPS)
	$(CXX) -std=c++11 $(CXXCHECK) -DTS_TICKS_TYPE=std::uint16_t -o $@ $(T)/test_timer_service.cpp
$(B)/test_error_exit_cpp: $(T)/test_error_exit.cpp timer_service.hpp | $(B)
	$(CXX) -std=c++11 -Wall -Wextra -pedantic -I. -o $@ $(T)/test_error_exit.cpp

# Line and branch coverage of both services under these checks: `make coverage`.
# GCOV is gcov for GCC, or llvm-cov's gcov for clang (on macOS /usr/bin/gcov is
# that one); both print the same summary. C++ is measured without exceptions:
# the header throws none, so their edges are no paths through it.
GCOV ?= gcov
COV = $(B)/coverage
coverage:
	rm -rf $(COV) && mkdir -p $(COV)
	cd $(COV) && $(CC) -std=c99 -O0 --coverage -I../.. -DTS_ERROR_HANDLER=testError -o cov_c ../../$(T)/test_timer_service.c ../../timer_service.c && ./cov_c >/dev/null
	cd $(COV) && $(CXX) -std=c++11 -O0 --coverage -fno-exceptions -I../.. -DSM_TIMER_ERROR_HANDLER=testError -o cov_cpp ../../$(T)/test_timer_service.cpp && ./cov_cpp >/dev/null
	@cd $(COV) && $(GCOV) -b -c cov_c-timer_service.gcda 2>/dev/null | grep -A4 "timer_service\.c'" | grep -v "^--"
	@cd $(COV) && $(GCOV) -b -c cov_cpp-test_timer_service.gcda 2>/dev/null | grep -A4 "timer_service\.hpp'" | grep -v "^--"

# The reference service from Doxyfile, then each RTOS header in a run of its
# own (they declare the same names), into build/doc/<folder>. Doxygen needs
# no OS headers.
DOXYGEN ?= doxygen
doc:
	rm -rf $(B)/doc && mkdir -p $(B)/doc
	$(DOXYGEN) Doxyfile
	@for v in freertos trampoline freertos_events trampoline_events embos_ultra_events; do \
	  ( cat Doxyfile; \
	    echo "PROJECT_NAME = \"SinelaboreRT timer service ($$v)\""; \
	    echo "INPUT = $$v/timer_service.h"; \
	    echo "USE_MDFILE_AS_MAINPAGE ="; \
	    echo "OUTPUT_DIRECTORY = $(B)/doc/$$v" ) | $(DOXYGEN) - || exit 1; \
	done
	@echo "timer_service: documentation in $(B)/doc/html and $(B)/doc/<rtos folder>/html"

clean:
	rm -rf $(B)

.PHONY: test coverage doc clean
