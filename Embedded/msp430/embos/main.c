#include "RTOS.h"
#include "BSP.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "state_model/sm_ext.h"
#include "state_model/sm.h"

// state machine instance
SM_INSTANCEDATA_T instanceVar = SM_INSTANCEDATA_INIT;

static OS_STACKPTR int Stack_TASK_1[128];   /* Task stacks */
static OS_TASK         TCB_TASK_1;         /* Task-control-blocks */


#define MAX_MSG_SIZE (1U)
#define MAX_MSG_NUM (32U)
static OS_MAILBOX      MyMailbox;
static char            MyMailboxBuffer[MAX_MSG_NUM*MAX_MSG_SIZE];

static OS_Q            MyQueue;
static char            MyQBuffer[100];

OS_TIMER MyTimer;

char txbuf[32] ;

static void MyTimerCallback(void) {

    uint8_t msg=evtTimeout;

    OS_Q_Put(&MyQueue, &msg, 1);
}

static void TaskRunningStateMachine(void) {
    char* pData;

    while (1) {
        volatile int Len = OS_Q_GetPtr(&MyQueue, (void**)&pData);
        volatile char msg = *pData;

        sm(&instanceVar, (SM_EVENT_T)msg);

        OS_Q_Purge(&MyQueue);

    }
}



int main(void) {
    OS_InitKern();                   /* Initialize OS                 */
    OS_InitHW();                     /* Initialize Hardware for OS    */
    BSP_Init();                      /* Initialize LED ports          */

    /* You need to create at least one task before calling OS_Start() */
    OS_Q_Create(&MyQueue, &MyQBuffer, sizeof(MyQBuffer));
    OS_CreateMB(&MyMailbox, MAX_MSG_SIZE, MAX_MSG_NUM, &MyMailboxBuffer);

    OS_CREATETASK(&TCB_TASK_1, "TASK_1", TaskRunningStateMachine, 100, Stack_TASK_1);

    OS_CREATETIMER(&MyTimer,  MyTimerCallback,   2000);

    OS_Start();                      /* Start multitasking            */
    return 0;
}
