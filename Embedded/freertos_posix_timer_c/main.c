/*
 * Microwave oven on FreeRTOS: the generated C machine of
 * LinuxMacBSD/microwave_builtin_editor_timer_c, with its timers on FreeRTOS
 * software timers (lib/timer_service/freertos).
 *
 *   keyboard task  --- key bit ----------->  machine task  ---> oven (prints)
 *   timer task     --- timer bits -------->      |
 *      ^                                         |
 *      +--- start, stop, time left (the timer service)
 *
 * - The machine task owns the machine. It waits for its notification bits
 *   and wakes for a key or a timeout: no polling cycle. Each timer of the
 *   machine sets a bit of its own when it expires (bit = timer id); a
 *   timeout is handed to the machine and taken in steps without event, then
 *   a key is dispatched.
 * - The keyboard task looks for a key every 20 ms. It passes the key in
 *   keyMsg and sets KEY_BIT; the machine task has the higher priority, so it
 *   takes keyMsg before the keyboard task runs again.
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

/* power levels the 'p' and 'P' keys send with evPwr, in watts */
#define PWR_MEDIUM_W 400U
#define PWR_HIGH_W 1200U

/* The key's notification bit; the timers use bits 0 to OVEN_TM_COUNT - 1. */
#define KEY_BIT ((uint32_t)1U << 31)

static OVEN_INSTANCEDATA_T oven_inst = OVEN_INSTANCEDATA_INIT;

/* The timer service of the machine: a FreeRTOS timer per timer id. */
static TS_TIMER_T timerList[OVEN_TM_COUNT];
static TS_SERVICE_T timers;

static TaskHandle_t machineTaskHandle;

/* The key's event, from the keyboard task to the machine task */
static OVEN_MSG_T keyMsg;

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
    char ch;
    (void)arg;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(20U));     /* look for a key every 20 ms */
        if (!kbhit()) {
            continue;
        }
        ch = readch();
        keyMsg.msg = OVEN_NO_MSG;
        if (ch == '+') {
            keyMsg.msg = evInc;
        } else if (ch == '-') {
            keyMsg.msg = evDec;
        } else if (ch == 'c') {
            keyMsg.msg = evDoorClosed;
        } else if (ch == 'o') {
            keyMsg.msg = evDoorOpen;
        } else if (ch == 'P') {
            keyMsg.msg = evPwr;
            keyMsg.data.evPwr.pwr = PWR_HIGH_W;
        } else if (ch == 'p') {
            keyMsg.msg = evPwr;
            keyMsg.data.evPwr.pwr = PWR_MEDIUM_W;
        } else if (ch == 'q') {
            close_keyboard();
            exit(EXIT_SUCCESS);
        } else if (ch != 0) {
            usage();
        }
        if (keyMsg.msg != OVEN_NO_MSG) {
            (void)xTaskNotify(machineTaskHandle, KEY_BIT, eSetBits);   /* the machine task runs now */
        }
    }
}

/* ---- The machine ---- */

static void dispatch(const OVEN_MSG_T *msg)
{
    (void)oven(&oven_inst, msg);
}

static void machineTask(void *arg)
{
    OVEN_MSG_T none;
    uint32_t bits;
    uint16_t id;
    uint16_t i;
    (void)arg;

    none.msg = OVEN_NO_MSG;
    dispatch(&none);                        /* the first call enters Super and Idle */
    fflush(stdout);

    for (;;) {
        (void)xTaskNotifyWait(0U, 0xFFFFFFFFU, &bits, portMAX_DELAY);   /* takes and clears all bits */

        /* 1. Timeouts: hand each one to the machine, then let it take them in
         *    steps without event; bounded, so conditions that keep winning
         *    cannot hang the task. */
        for (id = 0U; id < OVEN_TM_COUNT; id++) {
            if ((bits & TS_BIT(id)) != 0U) {
                ovenSetTimeout(&oven_inst, id);
            }
        }
        for (i = 0U; (i < OVEN_TM_COUNT) && (ovenTimeoutPending(&oven_inst) != 0U); i++) {
            dispatch(&none);
        }

        /* 2. A key */
        if ((bits & KEY_BIT) != 0U) {
            dispatch(&keyMsg);
        }
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
    /* The timer task (highest) carries out timer commands at once; the
     * machine task comes before the keyboard task that feeds it. */
    xTaskCreate(machineTask, "machine", configMINIMAL_STACK_SIZE * 4U, NULL, 2U, &machineTaskHandle);
    xTaskCreate(keyboardTask, "keys", configMINIMAL_STACK_SIZE, NULL, 1U, NULL);
    tsInit(&timers, machineTaskHandle, timerList, OVEN_TM_COUNT);
    oven_inst.timerService = &timers;

    init_keyboard();
    usage();
    vTaskStartScheduler();
    return EXIT_FAILURE;                    /* only if the scheduler could not start */
}
