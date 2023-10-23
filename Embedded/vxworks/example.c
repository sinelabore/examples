#include <stdio.h>
#include <stdint.h>
#include <vxWorks.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <time.h>
#include "state_machine_ext.h"
#include "state_machine.h"

TASK_ID tid1,tid2,tidLowPrio;
MSG_Q_ID qid;
timer_t timerID;
struct itimerspec value, ovalue;
STATE_MACHINE_INSTANCEDATA_T smInstance = STATE_MACHINE_INSTANCEDATA_INIT;

// Helper to start timer from state machine.
// In practise you might use a more sophisticated routine considering multiple timers ...
void startTimer(void){
    printf("Start timer!\n");
    timer_settime (timerID, TIMER_RELTIME, &value, &ovalue);
}

// helper to stop timer
void stopTimer(void){
    printf("Stop timer!\n");
    timer_cancel(timerID);
}

void task1() { 
    #define BUF_LEN 5
    char buf[BUF_LEN];

    printf("task 1 started!\n");
    while(1) {
        msgQReceive(qid, buf,1,WAIT_FOREVER);
        state_machine(&smInstance, *buf );
    }
}

char msgs[]= {evStart,STATE_MACHINE_NO_MSG, STATE_MACHINE_NO_MSG,STATE_MACHINE_NO_MSG,STATE_MACHINE_NO_MSG,
    STATE_MACHINE_NO_MSG,STATE_MACHINE_NO_MSG,STATE_MACHINE_NO_MSG,evStop,evStart,evStop,evShutdown,
    STATE_MACHINE_NO_MSG,STATE_MACHINE_NO_MSG};

void task2() { 
    printf("task 2 started!\n");
    for(uint8_t i=0; i<sizeof(msgs); i++) {
        msgQSend(qid, &msgs[i],1,WAIT_FOREVER,MSG_PRI_NORMAL);
        taskDelay(CLOCKS_PER_SEC);
    }
}

/* timer callback routine */ 
void timerhandler(timer_t timerID, int targ)
{   
    static char tout=evTimeout;
    //printf("Timeout\n");
    msgQSend(qid, &tout,1,WAIT_FOREVER,MSG_PRI_NORMAL);
}

void lowPriTask()
{
    volatile int forever = 1;
    while (forever) {}
}

int main(void)
{

    qid = msgQCreate(20, 1 /* msg len in byte */,MSG_Q_FIFO);

    // create a cyclic timer for demo purposes used in the state machine
    timer_create (CLOCK_REALTIME, NULL, &timerID);
    timer_connect (timerID, (VOIDFUNCPTR)timerhandler, NULL);
    value.it_value.tv_nsec = 1; 
    value.it_value.tv_sec = 0; 
    value.it_interval.tv_nsec = 500*1000*1000; // 500ms
    value.it_interval.tv_sec = 0;

    // init state var once
    state_machine(&smInstance, STATE_MACHINE_NO_MSG );

    tid1 = taskSpawn("/task1" , 107 , VX_NO_STACK_FILL , 2000 , (FUNCPTR)task1, 0,0,0,0,0,0,0,0,0,0);
    tid2 = taskSpawn("/task2" , 106 , VX_NO_STACK_FILL , 2000 , (FUNCPTR)task2, 0,0,0,0,0,0,0,0,0,0);
    tidLowPrio = taskSpawn("/taskidle" , 255 , VX_NO_STACK_FILL , 2000 , (FUNCPTR)lowPriTask, 0,0,0,0,0,0,0,0,0,0);


    printf("init finished!\n");

    while(1){
        taskDelay (CLOCKS_PER_SEC);
    }  

    taskSuspend (0);

    return 0;
}