//
// (C) Peter Mueller
// www.sinelabore.com
//
// Demo application to show how a statemachine
// can be used with timers, queues
// This concept can be used without an OS
// or with a OS/RTOS (then using the OS timer / queue
// services).


#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <pthread.h>

// include generated statemachine headers
#include "test_ext.h"
#include "test.h"

// include library headers
#include "lib/timer.h"
#include "lib/fifo.h"

TEST_INSTANCEDATA_T testSMInstanceData = TEST_INSTANCEDATA_INIT;


FIFO_T testMachineFifoInst;
uint8_t testMachineFifoRawMem[8]; // able to store 8 events

/* This function is run by the second thread
   It simulates the timer irq on an embedded system */
void *timer_irq_sim(void *x_void_ptr)
{
	static struct timespec tv_pthread={0,1000*1000*100}; // ~100ms

	while(1){
	
		nanosleep(&tv_pthread,0);
		//printf(".");
		tick();
		fflush(stdout);
	}
}


static struct timespec tv={0,1000*1000*100}; // ~100ms

int main(void){

	fifoInit(&testMachineFifoInst, testMachineFifoRawMem, 8);
	timerInit();
	
	// init state machine
	test(&testSMInstanceData, TEST_NO_MSG);

	/* this variable is our reference to the second thread */
	pthread_t timer_irq_sim_thread;
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&timer_irq_sim_thread, NULL, timer_irq_sim, 0)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	
	
	while(1){
	
		nanosleep(&tv,0);
		fflush(stdout);

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
	}

	return 0;
}
