// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Peter Mueller
// Checks of the reference timer service (C++); `make test`. The same checks as
// test_timer_service.c, through the C++ interface, and those of a machine's
// timers. Built with -DSM_TIMER_ERROR_HANDLER=testError.
#include <cassert>
#include <cstdio>
#include <type_traits>
#include "timer_service.hpp"

using sm::Ticks;
using sm::TimerMode;
using sm::TimerError;

// The error handler of these checks: it records the error and returns, so the
// failed call can be checked too.
static int errors;
static TimerError lastError;
void testError(TimerError error, const sm::MachineTimers* timers, sm::TimerId id)
{
    (void)timers;
    (void)id;
    ++errors;
    lastError = error;
}

// The timers of a machine, which the service knows the machine by, with their
// two states in one object for these checks: the states are a base, so they
// exist before the timers are given them. The destructor stops the timers, as
// the destructor of a generated machine does where it has one
// (StateMachineClassHasDestructor=yes).
struct TestStates {
    sm::TimerState states[2] = {};
};
struct TestTimers : private TestStates, public sm::MachineTimers {
    explicit TestTimers(sm::ITimerService& service) : sm::MachineTimers(states, service) {}
    ~TestTimers() { stopAll(); }
};

// A machine as the generator writes its timer part: the states, then the
// timers. Without a destructor (the default) it is trivially destructible, so
// one with static storage registers nothing to run at exit.
struct PlainMachine {
    explicit PlainMachine(sm::ITimerService& service) : m_timers(m_timerState, service) {}
    sm::TimerState m_timerState[2] = {};
    sm::MachineTimers m_timers;
};
static_assert(std::is_trivially_destructible<PlainMachine>::value, "no destructor to register at exit");

using Service = sm::TimerService<3U>;

// A value the compiler cannot know, as an expression in a model is: given
// constant arguments, the compiler evaluates the constexpr conversions itself.
static std::uintmax_t runtime(std::uintmax_t x)
{
    volatile std::uintmax_t v = x;
    return v;
}
static const Ticks half = static_cast<Ticks>(Ticks{1U} << (sizeof(Ticks) * 8U - 1U));

// Durations are constant expressions where they fit.
static_assert(sm::ms<200U>() == sm::ms(200U), "ms<>() and ms() agree");
static constexpr std::uintmax_t ticksPerMs = 1000U / sm::tickUs;
static_assert(sm::msFits(sm::ticksMax / ticksPerMs), "the longest duration in ms fits");
static_assert(!sm::msFits(static_cast<std::uintmax_t>(sm::ticksMax) / ticksPerMs + 1U), "one more does not");

int main()
{
    sm::Timeout out[3];

    {   // once: fires at its deadline, once
        Service ts(0U);
        TestTimers m1(ts);
        assert(ts.start(m1, 7U, 100U, TimerMode::Once));
        assert(ts.poll(99U, out, 3U) == 0U);
        assert(ts.poll(100U, out) == 1U && out[0].timers == &m1 && out[0].timerId == 7U);
        assert(out[0].periods == 1U);
        assert(ts.poll(1000U, out) == 0U && ts.running() == 0U);
    }
    {   // across the wrap of the clock
        Service ts(static_cast<Ticks>(Ticks{0U} - 0x100U));
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 0x200U, TimerMode::Once);                         // due 0x100
        assert(ts.poll(static_cast<Ticks>(Ticks{0U} - 1U), out) == 0U);
        assert(ts.poll(0x0FFU, out) == 0U);
        assert(ts.poll(0x100U, out) == 1U);
    }
    {   // every: keeps its phase; a late poll reports the missed periods once, and counts them
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Every);
        assert(ts.poll(350U, out) == 1U);                                       // 100, 200, 300 in one report
        assert(out[0].periods == 3U);                                           // timeouts as events: 3 events
        assert(ts.poll(399U, out) == 0U);
        assert(ts.poll(400U, out) == 1U && ts.running() == 1U);
        assert(out[0].periods == 1U);
        assert(!ts.start(m1, 2U, 0U, TimerMode::Every) && lastError == TimerError::NoPeriod);
    }
    {   // restart: counts again from the last poll, still one entry
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);
        assert(ts.poll(50U, out) == 0U);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);                           // due 150
        assert(ts.running() == 1U);
        assert(ts.poll(100U, out) == 0U);
        assert(ts.poll(150U, out) == 1U);
    }
    {   // the same id in two machines are two timers; stop and stopAll
        Service ts(0U);
        TestTimers m1(ts), m2(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);
        (void)ts.start(m2, 1U, 100U, TimerMode::Once);
        (void)ts.start(m2, 2U, 100U, TimerMode::Once);
        ts.stop(m1, 1U);
        ts.stop(m1, 1U);                                                        // not running: no effect
        assert(ts.running() == 2U);
        ts.stopAll(m2);
        assert(ts.running() == 0U && ts.poll(200U, out) == 0U);
    }
    {   // a full table refuses, counts and reports
        Service ts(0U);
        TestTimers m1(ts);
        errors = 0;
        (void)ts.start(m1, 1U, 10U, TimerMode::Once);
        (void)ts.start(m1, 2U, 20U, TimerMode::Once);
        (void)ts.start(m1, 3U, 30U, TimerMode::Once);
        assert(!ts.start(m1, 4U, 40U, TimerMode::Once) && ts.refused() == 1U);
        assert(errors == 1 && lastError == TimerError::TableFull);
        // the time left counts from the last poll
        assert(ts.timeLeft(m1, 2U) == 20U);
        (void)ts.poll(15U, out);
        assert(ts.timeLeft(m1, 2U) == 5U);
        // more expired than out[] holds: the rest comes with the next poll
        assert(ts.poll(100U, out, 1U) == 1U);
        assert(ts.poll(100U, out) == 1U);
        assert(ts.running() == 0U);
    }
    {   // time left, and changing it while the timer runs
        Service ts(0U);
        TestTimers m1(ts), m2(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);                           // due 100
        assert(ts.timeLeft(m1, 1U) == 100U);
        assert(ts.timeLeft(m1, 2U) == 0U && ts.timeLeft(m2, 1U) == 0U);
        assert(ts.poll(40U, out) == 0U && ts.timeLeft(m1, 1U) == 60U);
        assert(ts.setTimeLeft(m1, 1U, 160U));                                   // due 200
        assert(ts.poll(199U, out) == 0U && ts.timeLeft(m1, 1U) == 1U);
        assert(ts.setTimeLeft(m1, 1U, 0U));                                     // due at the next poll
        assert(ts.timeLeft(m1, 1U) == 0U);
        assert(ts.poll(199U, out) == 1U && out[0].timerId == 1U);
        assert(!ts.setTimeLeft(m1, 1U, 10U));                                   // gone: not restarted
        assert(ts.running() == 0U && ts.timeLeft(m1, 1U) == 0U);
        (void)ts.start(m1, 2U, 50U, TimerMode::Every);                          // due 249
        assert(ts.setTimeLeft(m1, 2U, 10U));                                    // due 209, then every 50
        assert(ts.poll(209U, out) == 1U && ts.timeLeft(m1, 2U) == 50U);
    }
    {   // increment and decrement, as the generated code writes them: the time
        // left set to more or less of it; used up, the timer expires now
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);                           // due 100
        assert(ts.setTimeLeft(m1, 1U, sm::timeAdd(ts.timeLeft(m1, 1U), 50U)) && ts.timeLeft(m1, 1U) == 150U);
        assert(ts.setTimeLeft(m1, 1U, sm::timeSub(ts.timeLeft(m1, 1U), 30U)) && ts.timeLeft(m1, 1U) == 120U);
        assert(ts.poll(119U, out) == 0U && ts.poll(120U, out) == 1U);
        assert(!ts.setTimeLeft(m1, 1U, sm::timeAdd(ts.timeLeft(m1, 1U), 10U)));  // gone
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);                           // due 220
        assert(ts.setTimeLeft(m1, 1U, sm::timeSub(ts.timeLeft(m1, 1U), 500U)));  // more than left
        assert(ts.timeLeft(m1, 1U) == 0U);
        assert(ts.poll(120U, out) == 1U && out[0].timers == &m1);             // the timeout, now
        assert(ts.running() == 0U);
        (void)ts.start(m1, 3U, 10U, TimerMode::Once);
        (void)ts.setTimeLeft(m1, 3U, sm::timeAdd(ts.timeLeft(m1, 3U), sm::ticksMax));  // capped
        assert(ts.timeLeft(m1, 3U) == sm::ticksMax);
        assert(ts.poll(121U, out) == 0U && ts.timeLeft(m1, 3U) == sm::ticksMax - 1U);
    }
    {   // an every timer keeps its period when its time left changes
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 50U, TimerMode::Every);                          // due 50
        (void)ts.poll(20U, out);                                                // 30 left
        assert(ts.setTimeLeft(m1, 1U, sm::timeAdd(ts.timeLeft(m1, 1U), 100U))); // due 150
        assert(ts.poll(149U, out) == 0U);
        assert(ts.poll(150U, out) == 1U && ts.timeLeft(m1, 1U) == 50U);       // then every 50
    }
    {   // a timer may be longer than half the counter's range
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, static_cast<Ticks>(half + 10U), TimerMode::Once);
        assert(ts.poll(half, out) == 0U && ts.timeLeft(m1, 1U) == 10U);
        assert(ts.poll(static_cast<Ticks>(half + 10U), out) == 1U);
    }
    {   // every timers that did not fit into out[] keep their phase
        Service ts(0U);
        TestTimers m1(ts), m2(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Every);
        (void)ts.start(m2, 1U, 100U, TimerMode::Every);
        assert(ts.poll(100U, out, 1U) == 1U && out[0].timers == &m1);
        assert(ts.timeLeft(m2, 1U) == 0U);                                      // expired, not reported yet
        assert(ts.poll(150U, out) == 1U && out[0].timers == &m2);
        assert(ts.timeLeft(m2, 1U) == 50U);                                     // next at 200, as m1
        assert(ts.poll(200U, out) == 2U);
    }
    {   // an expiry not reported yet is taken back by setting the time left
        Service ts(0U);
        TestTimers m1(ts), m2(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Once);
        (void)ts.start(m2, 1U, 100U, TimerMode::Once);
        assert(ts.poll(100U, out, 1U) == 1U && out[0].timers == &m1);
        assert(ts.setTimeLeft(m2, 1U, 30U));
        assert(ts.poll(100U, out) == 0U && ts.running() == 1U);
        assert(ts.poll(130U, out) == 1U && out[0].timers == &m2);
    }
    {   // a pending timer: stopped its report is dropped, restarted it starts fresh,
        // otherwise it is reported by the next poll
        Service ts(0U);
        TestTimers m1(ts), m2(ts);
        (void)ts.start(m1, 1U, 10U, TimerMode::Once);
        (void)ts.start(m2, 1U, 10U, TimerMode::Once);
        (void)ts.start(m2, 2U, 10U, TimerMode::Once);
        assert(ts.poll(10U, out, 0U) == 0U);                                    // no room: all three pending
        assert(ts.timeLeft(m1, 1U) == 0U && ts.running() == 3U);
        ts.stop(m2, 1U);                                                       // dropped
        assert(ts.start(m2, 2U, 50U, TimerMode::Once));                        // fresh: 50 from 10
        assert(ts.poll(20U, out) == 1U && out[0].timers == &m1);             // the pending report
        assert(ts.poll(59U, out) == 0U);
        assert(ts.poll(60U, out) == 1U && out[0].timerId == 2U);
    }
    {   // a once timer started with 0 ticks expires on the next poll, even at the same time
        Service ts(100U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 0U, TimerMode::Once);
        assert(ts.poll(100U, out) == 1U);
    }
    {   // every across the wrap of the clock keeps its phase
        Service ts(static_cast<Ticks>(Ticks{0U} - 150U));                        // 150 before the wrap
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 100U, TimerMode::Every);                         // due at -50, 50, 150, 250
        assert(ts.poll(static_cast<Ticks>(Ticks{0U} - 50U), out) == 1U);
        assert(ts.poll(49U, out) == 0U && ts.timeLeft(m1, 1U) == 1U);
        assert(ts.poll(175U, out) == 1U && ts.timeLeft(m1, 1U) == 75U);       // 50 and 150 merged
    }
    {   // no out[] is an error where timeouts could go into it
        Service ts(0U);
        errors = 0;
        assert(ts.poll(10U, nullptr, 3U) == 0U && lastError == TimerError::NoOutput);
        assert(ts.poll(10U, nullptr, 0U) == 0U && errors == 1);
    }
    {   // a machine's timers: the generated calls, and a machine that goes away
        Service ts(0U);
        {
            TestTimers m(ts);
            m.start(0U, 100U, TimerMode::Once);
            m.start(1U, 200U, TimerMode::Every);
            assert(ts.running() == 2U && m.runs(0U) && !m.timeoutPending());
            assert(m.setTimeLeft(0U, sm::timeAdd(m.timeLeft(0U), 10U)) && m.timeLeft(0U) == 110U);
            assert(ts.poll(110U, out) == 1U && out[0].timers == &m);
            out[0].timers->setTimeout(out[0].timerId);                          // as the loop does
            assert(m.timeoutPending() && m.expired(0U));
            m.consume(0U, TimerMode::Once);
            assert(!m.timeoutPending() && !m.runs(0U));
            m.setTimeout(0U);                                                   // a stale timeout is ignored
            m.setTimeout(7U);                                                   // so is an id it does not have
            assert(!m.timeoutPending());
            assert(ts.poll(200U, out) == 1U);                                   // the every timer
            out[0].timers->setTimeout(out[0].timerId);
            m.consume(1U, TimerMode::Every);                                    // taken, it runs on
            assert(m.runs(1U) && !m.timeoutPending());
            assert(m.setTimeLeft(1U, 30U) && m.setTimeLeft(1U, sm::timeSub(m.timeLeft(1U), 10U)));
            assert(m.timeLeft(1U) == 20U);
            m.stop(1U);
            assert(!m.runs(1U) && ts.running() == 0U);
            m.start(0U, 10U, TimerMode::Once);
            m.stopAll();                                                        // the generated destructor
            assert(!m.runs(0U) && ts.running() == 0U);
            m.start(1U, 10U, TimerMode::Every);                                 // left running for the destructor
        }
        assert(ts.running() == 0U);                                              // its destructor stopped it
    }
    {   // a machine holds its timers as a member, and the loop reaches them through the service
        Service ts(0U);
        PlainMachine machine(ts);
        machine.m_timers.start(1U, 10U, TimerMode::Once);
        assert(machine.m_timerState[1] == sm::TimerState::Running);
        assert(ts.poll(10U, out) == 1U && out[0].timers == &machine.m_timers);
        out[0].timers->setTimeout(out[0].timerId);
        assert(machine.m_timers.timeoutPending() && machine.m_timerState[1] == sm::TimerState::Expired);
    }
    {   // the timers set the states they are given idle
        Service ts(0U);
        sm::TimerState states[2] = {sm::TimerState::Expired, sm::TimerState::Running};
        sm::MachineTimers timers(states, ts);
        assert(!timers.timeoutPending() && !timers.runs(0U) && !timers.runs(1U));
        timers.setTimeout(2U);                                                  // no such id: two states, two timers
        assert(states[0] == sm::TimerState::Idle && states[1] == sm::TimerState::Idle);
    }
    {   // the time kept outside the service, at its limits
        assert(sm::timeAdd(sm::ticksMax, 0U) == sm::ticksMax && sm::timeAdd(0U, sm::ticksMax) == sm::ticksMax);
        assert(sm::timeAdd(1U, static_cast<Ticks>(sm::ticksMax - 1U)) == sm::ticksMax);
        assert(sm::timeAdd(1U, sm::ticksMax) == sm::ticksMax);
        assert(sm::timeSub(0U, 0U) == 0U && sm::timeSub(sm::ticksMax, sm::ticksMax) == 0U);
        assert(sm::timeSub(sm::ticksMax, 1U) == sm::ticksMax - 1U);
    }
    {   // durations at run time: the same ticks as at compile time, and an error past the range
        errors = 0;
        assert(sm::ms(1500U) == sm::ms<1500U>() && sm::s(2U) == sm::s<2U>() && sm::us(1001U) == sm::us<1001U>());
        assert(sm::ms(static_cast<std::uintmax_t>(-1)) == sm::ticksMax && lastError == TimerError::TooLong);
        assert(sm::us(static_cast<std::uintmax_t>(-1)) == sm::ticksMax && sm::s(static_cast<std::uintmax_t>(-1)) == sm::ticksMax);
        errors = 0;
#if TS_TICK_US == 1000U
        assert(sm::usFits(static_cast<std::uintmax_t>(sm::ticksMax) * 1000U));
        assert(!sm::usFits(static_cast<std::uintmax_t>(sm::ticksMax) * 1000U + 1U));
        assert(sm::ms(sm::ticksMax) == sm::ticksMax && errors == 0);
        assert(sm::ms(static_cast<std::uintmax_t>(sm::ticksMax) + 1U) == sm::ticksMax && errors == 1);
        assert(sm::us(70000U) == 70U);                                          // divided, then converted
        assert(sm::us(static_cast<std::uintmax_t>(sm::ticksMax) * 1000U) == sm::ticksMax && errors == 1);       // the limit
        assert(sm::us(static_cast<std::uintmax_t>(sm::ticksMax) * 1000U - 999U) == sm::ticksMax && errors == 1); // rounds up to it
        assert(sm::us(static_cast<std::uintmax_t>(sm::ticksMax) * 1000U + 1U) == sm::ticksMax && errors == 2);   // past it
        assert(sm::s(static_cast<std::uintmax_t>(sm::ticksMax) / 1000U) == static_cast<Ticks>(sm::ticksMax / 1000U * 1000U));
        if (sizeof(Ticks) == 2U) {                                              // 16-bit ticks of 1 ms: at most 65.535 s
            assert(sm::sFits(65U) && !sm::sFits(66U) && sm::s(65U) == 65000U);
            assert(sm::s(70U) == sm::ticksMax && lastError == TimerError::TooLong);
        }
#endif
    }
    {   // durations with values known only at run time, as generated for expressions
        errors = 0;
        assert(sm::us(runtime(1001U)) == sm::us<1001U>() && sm::ms(runtime(1500U)) == sm::ms<1500U>());
        assert(sm::us(runtime(2000U)) == sm::us<2000U>());                       // divides exactly
        assert(sm::s(runtime(2U)) == sm::s<2U>() && errors == 0);
        assert(sm::toMs(sm::ms(runtime(1500U))) == 1500U);
        assert(sm::ms(runtime(static_cast<std::uintmax_t>(-1))) == sm::ticksMax && errors == 1);
        assert(sm::s(runtime(static_cast<std::uintmax_t>(-1))) == sm::ticksMax && errors == 2);
        assert(sm::us(runtime(static_cast<std::uintmax_t>(-1))) == sm::ticksMax && errors == 3);
#if TS_TICK_US == 1000U
        const std::uintmax_t limit = static_cast<std::uintmax_t>(sm::ticksMax) * 1000U;
        errors = 0;
        assert(sm::us(runtime(limit)) == sm::ticksMax && errors == 0);               // exactly the limit
        assert(sm::us(runtime(limit - 999U)) == sm::ticksMax && errors == 0);        // rounds up to it
        assert(sm::us(runtime(limit + 1U)) == sm::ticksMax && errors == 1);          // rounds up past it
        assert(sm::us(runtime(limit + 1000U)) == sm::ticksMax && errors == 2);       // a whole tick past it
#endif
    }
    {   // durations to ticks
#if TS_TICK_US == 1000U
        assert(sm::ms(200U) == 200U && sm::s(2U) == 2000U);
        assert(sm::us(1U) == 1U && sm::us(1000U) == 1U && sm::us(1001U) == 2U);  // rounded up
        assert(sm::toMs(sm::ms(1500U)) == 1500U);
#elif TS_TICK_US == 1U
        assert(sm::us(7U) == 7U && sm::ms(200U) == 200000U && sm::s(2U) == 2000000U);
        assert(sm::toMs(sm::ms(1500U)) == 1500U && sm::toMs(1999U) == 1U);     // rounded down
#endif
    }

    if (static_cast<std::uintmax_t>(static_cast<Ticks>(~Ticks{0U})) > 200000U) {
        // the count of periods stops at 65535: in one poll, and summed over polls without room
        Service ts(0U);
        TestTimers m1(ts);
        (void)ts.start(m1, 1U, 1U, TimerMode::Every);
        assert(ts.poll(static_cast<Ticks>(70000U), out) == 1U && out[0].periods == 0xFFFFU);
        assert(ts.poll(static_cast<Ticks>(70010U), out, 0U) == 0U);              // 10, kept
        assert(ts.poll(static_cast<Ticks>(140010U), out, 0U) == 0U);             // 10 + 65535 or more
        assert(ts.poll(static_cast<Ticks>(140010U), out) == 1U && out[0].periods == 0xFFFFU);
    }

    std::printf("timer_service.hpp: all checks passed (%u-bit time, %u us ticks, C++ %ld)\n",
                static_cast<unsigned>(sizeof(Ticks) * 8U), static_cast<unsigned>(sm::tickUs),
                static_cast<long>(__cplusplus));
    return 0;
}
