//
// 
// www.sinelabore.com
//


#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <pthread.h>

#include "vending_machine_ext.h"
#include "vending_machine.h"
#include "display_with_coinbank.h"
#include "product_store.h"
#include "product_store_sm_ext.h"
#include "product_store_sm.h"

#include "lib/timer.h"
#include "lib/fifo.h"

VENDING_MACHINE_INSTANCEDATA_T vendingMachine = VENDING_MACHINE_INSTANCEDATA_INIT;
PRODUCT_STORE_SM_INSTANCEDATA_T productStoreMachine = PRODUCT_STORE_SM_INSTANCEDATA_INIT;


FIFO_T fifo2VendingMachine;
uint8_t fifo2VendingMachineRawMem[8];

FIFO_T fifo2ProductStoreMachine;
uint8_t fifo2ProductStoreMachineRawMem[8];


/* This function is run by the second thread
   It simulates the timer irq on an embedded system */
void *timer_irq_sim(void *x_void_ptr)
{
	static struct timespec tv_pthread={0,1000*1000*100}; // ~100ms

	while(1){
	
		nanosleep(&tv_pthread,0);
		//printf("tick\n");
		tick();
	}
}


static struct timespec tv={0,1000*1000*100}; // ~100ms

int main(void){

	fifoInit(&fifo2VendingMachine, fifo2VendingMachineRawMem, 8);
	fifoInit(&fifo2ProductStoreMachine, fifo2ProductStoreMachineRawMem, 8);

	timerInit();
	
	// init subsystems and let them know the queue so they can send us events
	display_init(&fifo2VendingMachine);
	product_store_init(&fifo2ProductStoreMachine);
	
	// init machines
	vending_machine(&vendingMachine, VENDING_MACHINE_NO_MSG);
	product_store_sm(&productStoreMachine, PRODUCT_STORE_SM_NO_MSG);	

	/* this variable is our reference to the second thread */
	pthread_t timer_irq_sim_thread;
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&timer_irq_sim_thread, NULL, timer_irq_sim, 0)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	
	
	while(1){
	
		nanosleep(&tv,0);


		//
		// Check if there are new events for the state machine. If yes, 
		// call state machine with event.
		//
		bool fifoEmpty = fifoIsEmpty(&fifo2VendingMachine);
		if(!fifoEmpty){
			uint8_t evt;
			
			fifoGet(&fifo2VendingMachine, &evt);
			vending_machine(&vendingMachine, evt);
		}
		
		fifoEmpty = fifoIsEmpty(&fifo2ProductStoreMachine);
		if(!fifoEmpty){
			uint8_t evt;
			
			fifoGet(&fifo2ProductStoreMachine, &evt);
			product_store_sm(&productStoreMachine, evt);
		}		
		
	}

	return 0;
}
