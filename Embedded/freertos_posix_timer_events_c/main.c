/*
 * Microwave oven on FreeRTOS with timeouts as events: the model of
 * ../freertos_posix_timer_c with TimeoutsAsEvents, its timers on FreeRTOS
 * software timers (lib/timer_service/freertos_events).
 *
 *   keyboard task  --- key event ------+
 *                                      +-->  queue  -->  machine task ---> oven (prints)
 *   timer task     --- timeout event --+                     |
 *      ^                                                     |
 *      +--- start, stop, time left (the timer service) ------+
 *
 * - The machine task owns the machine. It blocks on its queue and dispatches
 *   each event in the order it arrived: no polling cycle, and no steps
 *   without event. A timeout is an event like a key: cookTimerTimeout and
 *   Cooking_0Timeout (oven_ext.h).
 * - The timer service calls onTimeout() for each expiry, for an every timer
 *   once per period. It runs in the timer task, looks up the timer's event
 *   (ovenTimeoutEvent) and posts it without waiting.
 * - The keyboard task looks for a key every 20 ms and posts its event.
 *
 *   ./oven      keys: + - o c p P q
 */
#include <stdint.h>
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include "timer_service.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

/* power levels the 'p' and 'P' keys send with evPwr, in watts */
#define PWR_MEDIUM_W 400U
#define PWR_HIGH_W 1200U

/* Events the queue holds. Keys come one by one and the machine takes each
 * at once, timeouts at most one per timer and period: a few entries do. */
#define QUEUE_LENGTH 16U

static OVEN_INSTANCEDATA_T oven_inst = OVEN_INSTANCEDATA_INIT;

/* The timer service of the machine: a FreeRTOS timer per timer id. */
static TS_TIMER_T timerList[OVEN_TM_COUNT];
static TS_SERVICE_T timers;

/* The machine task's queue: every event of the machine, keys and timeouts. */
static QueueHandle_t queue;

/* ---- The keyboard ---- */

static struct termios init_tio;
static int keyboardRaw = 0;

static void close_keyboard(void)
{
    if (keyboardRaw) {
        tcsetattr(STDIN_FILENO, TCSANOW, &init_tio);
        keyboardRaw = 0;
    }
}

static void onSignal(int sig)
{
    close_keyboard();
    signal(sig, SIG_DFL);
    raise(sig);
}

/* Non-canonical and without echo for the whole run. */
static void init_keyboard(void)
{
    struct termios tio;
    if (tcgetattr(STDIN_FILENO, &init_tio) != 0) {
        return;                             /* not a terminal */
    }
    tio = init_tio;
    tio.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tio) == 0) {
        keyboardRaw = 1;
        atexit(close_keyboard);
        signal(SIGINT, onSignal);
        signal(SIGTERM, onSignal);
    }
}

/* A key is waiting. Must not block: a task that blocks in a system call
 * stalls the POSIX port's scheduler. */
static int kbhit(void)
{
    struct timeval tv = { 0, 0 };
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    return select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv) > 0;
}

/* The key, 0 for none (a read interrupted by the port's signals), 'q' at
 * the end of the input, so a closed stdin ends the program like q. */
static char readch(void)
{
    char ch = 0;
    const ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n == 0) {
        return 'q';
    }
    return (n == 1) ? ch : 0;
}

static void usage(void)
{
    printf("Use + for inc; - for dec; o for open and c for close; q to quit.\n");
    printf("Use 'p' to set medium power (400 W); 'P' to set high power (1200 W)\n");
    fflush(stdout);
}

static void keyboardTask(void *arg)
{
    OVEN_MSG_T msg;
    char ch;
    (void)arg;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(20U));     /* look for a key every 20 ms */
        if (!kbhit()) {
            continue;
        }
        ch = readch();
        msg.msg = OVEN_NO_MSG;
        if (ch == '+') {
            msg.msg = evInc;
        } else if (ch == '-') {
            msg.msg = evDec;
        } else if (ch == 'c') {
            msg.msg = evDoorClosed;
        } else if (ch == 'o') {
            msg.msg = evDoorOpen;
        } else if (ch == 'P') {
            msg.msg = evPwr;
            msg.data.evPwr.pwr = PWR_HIGH_W;
        } else if (ch == 'p') {
            msg.msg = evPwr;
            msg.data.evPwr.pwr = PWR_MEDIUM_W;
        } else if (ch == 'q') {
            close_keyboard();
            exit(EXIT_SUCCESS);
        } else if (ch != 0) {
            usage();
        }
        if (msg.msg != OVEN_NO_MSG) {
            (void)xQueueSendToBack(queue, &msg, portMAX_DELAY);   /* the machine task runs now */
        }
    }
}

/* ---- The timeouts ---- */

/* The timer service's function for a timeout (TS_TIMEOUT_FN): post the
 * timer's event. Runs in the timer task, or in the machine task for a timer
 * started with 0 ticks, so it must not wait: a full queue loses the timeout,
 * which is reported. */
static void onTimeout(void *context, TS_TIMER_ID_T timerId)
{
    OVEN_MSG_T msg;
    msg.msg = ovenTimeoutEvent(timerId);
    if (xQueueSendToBack((QueueHandle_t)context, &msg, 0U) != pdPASS) {
        fprintf(stderr, "queue full: the timeout of timer %u is lost\n", (unsigned)timerId);
    }
}

/* ---- The machine ---- */

static void dispatch(const OVEN_MSG_T *msg)
{
    (void)oven(&oven_inst, msg);
}

static void machineTask(void *arg)
{
    OVEN_MSG_T msg;
    (void)arg;

    msg.msg = OVEN_NO_MSG;
    dispatch(&msg);                         /* the first call enters Super and Idle */
    fflush(stdout);

    for (;;) {
        (void)xQueueReceive(queue, &msg, portMAX_DELAY);   /* blocked until an event arrives */
        dispatch(&msg);
        fflush(stdout);
    }
}

void vAssertCalled(const char *file, unsigned long line)
{
    close_keyboard();
    fprintf(stderr, "FreeRTOS assertion failed: %s:%lu\n", file, line);
    exit(EXIT_FAILURE);
}

int main(void)
{
    queue = xQueueCreate(QUEUE_LENGTH, sizeof(OVEN_MSG_T));
    configASSERT(queue != NULL);
    tsInit(&timers, onTimeout, queue, timerList, OVEN_TM_COUNT);
    oven_inst.timerService = &timers;

    /* The timer task (highest) carries out timer commands at once; the
     * machine task comes before the keyboard task that feeds it. */
    xTaskCreate(machineTask, "machine", configMINIMAL_STACK_SIZE * 4U, NULL, 2U, NULL);
    xTaskCreate(keyboardTask, "keys", configMINIMAL_STACK_SIZE, NULL, 1U, NULL);

    init_keyboard();
    usage();
    vTaskStartScheduler();
    return EXIT_FAILURE;                    /* only if the scheduler could not start */
}
