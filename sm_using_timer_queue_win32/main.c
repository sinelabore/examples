//
// (C) Peter Mueller
// www.sinelabore.com
//
// Demo application to show how a statemachine
// can be used with timers, queues
// This concept can be used without an OS
// or with a OS/RTOS (then using the OS timer / queue
// services).
//

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// include generated statemachine headers
#include "test_ext.h"
#include "test.h"

// include library headers
#include "lib/timer.h"
#include "lib/fifo.h"

TEST_INSTANCEDATA_T testSMInstanceData = TEST_INSTANCEDATA_INIT;


FIFO_T testMachineFifoInst;
uint8_t testMachineFifoRawMem[8]; // able to store 8 events

DWORD WINAPI thread_function(LPVOID arg){
	while(true){
		// Simulate on doing some job...
		Sleep(100);
		tick();
		//printf(".");
	}

}

int main()

{

    HANDLE a_thread;
    DWORD a_threadId;

    // Create a new thread.
    a_thread = CreateThread(NULL, 0, thread_function, (LPVOID)NULL, 0,&a_threadId);

    if (a_thread == NULL)
    {
		printf("LCreateThread() failed, error %lu\n", GetLastError());
		exit(EXIT_FAILURE);
    }else{
        //printf("LCurrent process ID is %lu\n", GetCurrentProcessId());
		//printf("LCreateThread() is  OK, thread ID is %lu\n", a_threadId);
		fflush(stdout);
		
		fifoInit(&testMachineFifoInst, testMachineFifoRawMem, 8);
		timerInit();

		// init state machine
		test(&testSMInstanceData, TEST_NO_MSG);
		
		while(true){
			//
			// Check if there are new events for the state machine. If yes, 
			// call state machine with event.
			//
			bool fifoEmpty = fifoIsEmpty(&testMachineFifoInst);
			if(!fifoEmpty){
				uint8_t evt;
				
				fifoGet(&testMachineFifoInst, &evt);
				test(&testSMInstanceData, evt);
			}			
			Sleep(100);
		}
    }
	exit(EXIT_SUCCESS);

}