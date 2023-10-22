#include "mydefs.h"
#include "oven_hlp.h"
#include "oven.h"
#include <stdio.h>

static T_TIMER_STATE timerStatus;
static unsigned short tickval=0;
static unsigned char presetval=0;

extern T_PWR pwr;

//
// State change functions
//

void ovenChangeToState(OVEN_INSTANCEDATA_T *instance, OVEN_STATES_T state) {
	instance->stateVar = state;
}

void ovenChangeToStateSuper(OVEN_INSTANCEDATA_T *instance, OVEN_STATES_T state) {
	instance->stateVarSuper = state;
}


//
// Timer functions
//

// tick is called every 100ms
unsigned char tick(){
	if(timerStatus==TIMER_ON){		
		
		if(tickval<(presetval*10)){
			tickval++;
			printf("Timer %ds left\n",presetval - tickval/10);
		}
			
		if(tickval==(presetval*10)){
			printf("Timer::timeout\n");
			timerStatus=TIMER_OFF;
			return 1;
		}
	}
	return 0;
}

void timer_pause(void){
	timerStatus=TIMER_PAUSE;
}

void timer_cont(void){
	timerStatus=TIMER_ON;
}

void timer_clear(void){
	presetval=0;
}

unsigned char timer_preset(){
	return presetval;
}

T_TIMER_STATE timer_start(){
	printf("Timer::start\n");
	
	if(presetval!=0){
		tickval=0;
		timerStatus = TIMER_ON;
		return TIMER_ON;
	}else{
		timerStatus = TIMER_OFF;
		return TIMER_OFF;
	}
}

void timer_stop(void){
	printf("Timer::stop\n");
	timerStatus=TIMER_OFF;
}

T_TIMER_STATE timer_status(void){
	return timerStatus;
}

void timer_inc(void){
	if(presetval<60){
		presetval++;
		printf("Timer::inc -> preset=%d\n",presetval);
	}
}

void timer_dec(void){
	if(presetval>0)
		presetval--;
		printf("Timer::dec -> preset=%d\n",presetval);
}

// Pwr Functions

void ovenSetPwr(T_PWR pwr_in){
	if(pwr_in==PWR_LOW)
		printf("Oven::set to 'PWR_LOW'\n");
	else if(pwr_in==PWR_HIGH)
		printf("Oven::set to 'PWR_HIGH'\n");
}

void ovenOff(void){
	printf("Oven::off\n");
}

void ovenOn(void){
	printf("Oven::on\n");
}

