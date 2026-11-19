/*
 * Microwave oven with the reference timer service, in C++.
 *
 * The timer service counts the cooking time. cookTimer is a named timer:
 * + and - change its time in every state (cookTimer: increment(1s)), Cooking
 * starts it with that time (cookTimer: after()), and leaving Cooking keeps
 * the time left, so the door pauses it. The model holds no time of its own.
 *
 * The loop runs every cycleMs: poll the timer service, hand each timeout
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
#include "timer_service.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

namespace {

constexpr std::uint32_t cycleMs = 100U;     // the loop's period

// power levels the 'p' and 'P' keys send with evPwr, in watts
constexpr std::uint16_t pwrMediumW = 400U;
constexpr std::uint16_t pwrHighW = 1200U;

// The service is constant-initialized, so it exists before the machine,
// which keeps a reference to it. One entry per timer of the machine.
sm::TimerService<oven::TM_COUNT> timers;
oven machine(timers);
std::uint32_t now;                          // ms since start, counted by the loop

struct termios init_tio;
bool keyboardRaw = false;

// Restore the terminal as it was at the start.
void close_keyboard()
{
    if (keyboardRaw) {
        tcsetattr(STDIN_FILENO, TCSANOW, &init_tio);
        keyboardRaw = false;
    }
}

extern "C" void onSignal(int sig)
{
    close_keyboard();
    signal(sig, SIG_DFL);
    raise(sig);
}

extern "C" void closeKeyboardAtExit()
{
    close_keyboard();
}

// Non-canonical and without echo for the whole run, so a key needs no
// Enter and does not land in front of the next output line.
void init_keyboard()
{
    if (tcgetattr(STDIN_FILENO, &init_tio) != 0) {
        return;                             // not a terminal
    }
    struct termios tio = init_tio;
    tio.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tio) == 0) {
        keyboardRaw = true;
        std::atexit(closeKeyboardAtExit);
        signal(SIGINT, onSignal);
        signal(SIGTERM, onSignal);
    }
}

// A key is waiting; does not block.
bool kbhit()
{
    struct timeval tv = { 0, 0 };
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    return select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv) > 0;
}

char readch()
{
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        ch = 0;
    }
    return ch;
}

void usage()
{
    std::printf("Use + for inc; - for dec; o for open and c for close; q to quit.\n");
    std::printf("Use 'p' to set medium power (400 W); 'P' to set high power (1200 W)\n");
}

// Milliseconds on a monotonic clock.
std::uint32_t clockMs()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return static_cast<std::uint32_t>(static_cast<std::uint64_t>(t.tv_sec) * 1000U
                                      + static_cast<std::uint64_t>(t.tv_nsec) / 1000000U);
}

// Wait until the cycle's time on the clock. The target is absolute, so a
// sleep that ends late shortens the next one and `now` keeps pace with the
// clock.
void waitForCycle(std::uint32_t start)
{
    const std::uint32_t elapsed = clockMs() - start;
    if (static_cast<std::int32_t>(now - elapsed) > 0) {
        const std::uint32_t ms = now - elapsed;
        struct timespec d = { static_cast<time_t>(ms / 1000U), static_cast<long>(ms % 1000U) * 1000000L };
        nanosleep(&d, nullptr);
    }
}

}  // namespace

int main()
{
    const std::uint32_t start = clockMs();
    sm::Timeout expired[oven::TM_COUNT];

    init_keyboard();
    usage();
    machine.initialize();                   // enters Super and Idle

    for (;;) {
        waitForCycle(start);                // a device waits for its tick here

        // 1. Timeouts: poll the service and hand each one to the timers of its machine.
        const std::size_t n = timers.poll(now, expired);
        for (std::size_t i = 0U; i < n; ++i) {
            expired[i].timers->setTimeout(expired[i].timerId);
        }
        // 2. Let the machine take them in steps without event. A step takes
        //    one timeout per region at most, so step until none is left;
        //    bounded, so conditions that keep winning cannot hang the loop.
        /*
        for (std::size_t i = 0U; (i < oven::TM_COUNT) && machine.timeoutPending(); ++i) {
            OVEN_MSG_T none{};
            none.id = OVEN_NO_MSG;
            machine.processEvent(none);
        }
        */

        // 3. The event of this cycle: a key, else none. The message carries
        //    the event id, and for evPwr the power in watts.
        OVEN_MSG_T msg{};
        msg.id = OVEN_NO_MSG;
        if (kbhit()) {
            const char ch = readch();
            if (ch == '+') {
                msg.id = evInc;
            } else if (ch == '-') {
                msg.id = evDec;
            } else if (ch == 'c') {
                msg.id = evDoorClosed;
            } else if (ch == 'o') {
                msg.id = evDoorOpen;
            } else if (ch == 'q') {
                close_keyboard();
                break;
            } else if (ch == 'P') {
                msg.id = evPwr;
                msg.data.evPwr.pwr = pwrHighW;
            } else if (ch == 'p') {
                msg.id = evPwr;
                msg.data.evPwr.pwr = pwrMediumW;
            } else {
                usage();
            }
        }
        machine.processEvent(msg);          // also with no event: conditions, do code

        now += cycleMs;                     // the counter
    }
    return 0;
}
