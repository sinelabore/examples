//
// This code simulates a display + coin store
// There is just enough code to run this simulation
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
#include "display_with_coinbank.h"
#include "activity_change_algorithm.h"
#include "lib/fifo.h"


FIFO_T* fifo;

uint8_t numTenCentCoins = 20;
uint8_t numTwentyCentCoints = 10;
uint8_t numFifetyCentCoins = 4;
uint8_t lastCoin=0;

void init_keyboard(void);
void close_keyboard(void);
void display_exec(void);
int kbhit(void);
int readch(void);

static struct termios init_tio;

void init_keyboard()
{            
    tcgetattr(0,&init_tio);
}
void close_keyboard()
{            
    tcsetattr(0,TCSANOW,&init_tio);
}

int kbhit()
{
    struct termios tio;
    struct timeval tv;
    fd_set rfds;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON);
    tcsetattr(0,TCSANOW,&tio);
    // do not wait
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    tv.tv_usec = 0;
    tv.tv_sec  = 0;
    select(1,&rfds,NULL,NULL,&tv);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return (FD_ISSET(0,&rfds)?1:0);
}

int readch()
{
    int ch;
    struct termios tio;
    // set up terminal
    memcpy(&tio,&init_tio,sizeof(struct termios));
    tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&tio);
    // input key
    read(0,&ch,1);
    // back to initial terminal mode
    tcsetattr(0,TCSANOW,&init_tio);
    return ch;
}

void *keyboard_thread(void *x_void_ptr)
{
	static struct timespec tv_pthread={0,1000*1000*100}; // ~100ms

	while(1){
		bool status=0;
	
		nanosleep(&tv_pthread,0);
		if(kbhit()){
			char ch=readch();
			//printf("Put keyb event to queue\n");
			if(ch=='A'){
				status=fifoPut(fifo, evProductA);
			}else if(ch=='B'){
				status=fifoPut(fifo, evProductB);
			}else if(ch=='C'){
				status=fifoPut(fifo, evProductC);
			}else if(ch=='1'){
				lastCoin=10;
				status=fifoPut(fifo, evCoin);
			}else if(ch=='2'){
				lastCoin=20;
				status=fifoPut(fifo, evCoin);
			}else if(ch=='5'){
				lastCoin=50;
				status=fifoPut(fifo, evCoin);
			}else if(ch=='R'){
				status=fifoPut(fifo, evClearSelection);
			}else{
				status=fifoPut(fifo, evUnknownCoin);
			}
			
			if(status!=0){
				printf("status of fifoPut=%d\n",status);
				exit(1);
			}
		}
	}
}

void display_init( FIFO_T* const f){

	fifo = f;

	init_keyboard();
	
	/* this variable is our reference to the second thread */
	pthread_t keyboard_sim_thread;
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&keyboard_sim_thread, NULL, keyboard_thread, 0)) {
		fprintf(stderr, "Error creating thread\n");
		exit(1);
	}	
}

void displayValue(uint8_t val){

	printf("Display:: Display value: %d\n",val);
	
}


uint8_t getCoinValue(void){
	uint8_t coin = lastCoin;
	lastCoin=0;
	
	return coin;
	
}




void releaseCoins(uint8_t centsToRelease){
	change_algorithm(centsToRelease);
}
