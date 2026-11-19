/*
 * Microwave oven with the reference timer service.
 *
 * The timer service counts the cooking time. cookTimer is a named timer:
 * + and - change its time in every state (cookTimer: increment(1s)), Cooking
 * starts it with that time (cookTimer: after()), and leaving Cooking keeps
 * the time left, so the door pauses it. The model holds no time of its own.
 *
 * The loop runs every CYCLE_MS: poll the timer service, hand each timeout
 * to the machine, let the machine take the timeouts in steps without event,
 * then dispatch a key, or no event. All calls of the state machine are in
 * this loop.
 *
 *   ./oven         keys: + - o c p P q
 */
#include "mydefs.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include "timer_service.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>

#define CYCLE_MS 100U                   /* the loop's period */

// power levels the 'p' and 'P' keys send with evPwr, in watts
#define PWR_MEDIUM_W 400u
#define PWR_HIGH_W   1200u

static OVEN_INSTANCEDATA_T instData = OVEN_INSTANCEDATA_INIT;
static TS_TIMER_T timerTable[OVEN_TM_COUNT];
static TS_SERVICE_T timers;
static uint32_t now;                    /* ms since start, counted by the loop */

static struct termios init_tio;
static int keyboardRaw = 0;

/* Restore the terminal as it was at the start. */
static void close_keyboard(void)
{
    if(keyboardRaw){
        tcsetattr(STDIN_FILENO,TCSANOW,&init_tio);
        keyboardRaw = 0;
    }
}

static void onSignal(int sig)
{
    close_keyboard();
    signal(sig, SIG_DFL);
    raise(sig);
}

/* Non-canonical and without echo for the whole run, so a key needs no
   Enter and does not land in front of the next output line. */
static void init_keyboard(void)
{
    struct termios tio;
    if(tcgetattr(STDIN_FILENO,&init_tio) != 0){
        return;                         /* not a terminal */
    }
    tio = init_tio;
    tio.c_lflag &= ~(ICANON | ECHO);
    tio.c_cc[VMIN]  = 1;
    tio.c_cc[VTIME] = 0;
    if(tcsetattr(STDIN_FILENO,TCSANOW,&tio) == 0){
        keyboardRaw = 1;
        atexit(close_keyboard);
        signal(SIGINT, onSignal);
        signal(SIGTERM, onSignal);
    }
}

/* A key is waiting; does not block. */
static int kbhit(void)
{
    struct timeval tv = { 0, 0 };
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO,&rfds);
    return (select(STDIN_FILENO + 1,&rfds,NULL,NULL,&tv) > 0) ? 1 : 0;
}

static int readch(void)
{
    char ch = 0;
    if(read(STDIN_FILENO,&ch,1) != 1){
        ch = 0;
    }
    return ch;
}

static void usage(void)
{
    printf("Use + for inc; - for dec; o for open and c for close; q to quit.\n");
    printf("Use 'p' to set medium power (400 W); 'P' to set high power (1200 W)\n");
}

/* Milliseconds on a monotonic clock. */
static uint32_t clockMs(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint32_t)((uint64_t)t.tv_sec * 1000U + (uint64_t)t.tv_nsec / 1000000U);
}

/* Wait until the cycle's time on the clock. The target is absolute, so a
   sleep that ends late shortens the next one and `now` keeps pace with the
   clock. */
static void waitForCycle(uint32_t start)
{
    uint32_t elapsed = clockMs() - start;
    if ((int32_t)(now - elapsed) > 0) {
        uint32_t ms = now - elapsed;
        struct timespec d = { (time_t)(ms / 1000U), (long)(ms % 1000U) * 1000000L };
        nanosleep(&d, NULL);
    }
}

int main(void)
{
    const uint32_t start = clockMs();
    TS_TIMEOUT_T expired[OVEN_TM_COUNT];
    uint16_t n;
    uint16_t i;
    // message to state machine: the event id, and for evPwr the power in watts
    OVEN_MSG_T msg;

    tsInit(&timers, timerTable, OVEN_TM_COUNT, now);
    instData.timerService = &timers;
    init_keyboard();
    usage();

    for(;;){
        waitForCycle(start);                /* a device waits for its tick here */

        /* 1. Timeouts: poll the service and hand each one to the machine. */
        n = tsPoll(&timers, now, expired, OVEN_TM_COUNT);
        for(i = 0U; i < n; i++){
            if(expired[i].machine == &instData){
                ovenSetTimeout(&instData, expired[i].timerId);
            }
        }
        /* 2. Let the machine take them in steps without event. A step takes
              one timeout per region at most, so step until none is left;
              bounded, so conditions that keep winning cannot hang the loop. */
        for(i = 0U; (i < (uint16_t)OVEN_TM_COUNT) && (ovenTimeoutPending(&instData) != 0U); i++){
            msg.msg = OVEN_NO_MSG;
            oven(&instData, &msg);
        }

        /* 3. The event of this cycle: a key, else none. */
        msg.msg = OVEN_NO_MSG;
        if(kbhit()){
            char ch = (char)readch();
            if(ch=='+'){
                msg.msg=evInc;
            }else if(ch=='-'){
                msg.msg=evDec;
            }else if(ch=='c'){
                msg.msg=evDoorClosed;
            }else if(ch=='o'){
                msg.msg=evDoorOpen;
            }else if(ch=='q'){
                close_keyboard();
                break;
            }else if(ch=='P'){
                msg.msg=evPwr;
                msg.data.evPwr.pwr=PWR_HIGH_W;
            }else if(ch=='p'){
                msg.msg=evPwr;
                msg.data.evPwr.pwr=PWR_MEDIUM_W;
            }else{
                usage();
            }
        }
        oven(&instData, &msg);              /* also with no event: conditions, do code */

        now += CYCLE_MS;                    /* the counter */
    }
    return 0;
}
