// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Peter Mueller

/// @file timer_service.hpp
/// @brief Reference timer service for SinelaboreRT state machines (C++11).
///
/// The same design as timer_service.h: polled, no callbacks into a machine,
/// no heap, no threads. The application calls sm::TimerService::poll()
/// regularly and hands each expired timer to its machine's timers with
/// `timers->setTimeout(id)`. Each timer counts its ticks down: poll() takes
/// the time since the last poll off every running timer, so the counter may
/// wrap as long as the application polls at least once per counter range,
/// and a timer may use the whole range.
///
/// Three parts:
///
/// | Class                | Role |
/// | -------------------- | ---- |
/// | sm::ITimerService    | what a generated machine calls |
/// | sm::MachineTimers    | the timers of a generated machine, which the machine holds as a member: the service given to the machine's constructor and the timer states, an array the machine declares; setTimeout(), timeoutPending(); trivially destructible |
/// | sm::TimerService<N>  | the reference service with a table of N entries |
///
/// It needs no C++ standard library beyond `<cstddef>` and `<cstdint>` (the
/// default error handler adds `<cstdio>` and `<cstdlib>`), so it suits
/// generated code with `UseStdLibrary=NO`. Every call must come from the
/// context that also runs the machines. The same settings as
/// timer_service.h: #TS_TICKS_TYPE, #TS_TICK_US, #TS_TIMER_ID_TYPE, and
/// #SM_TIMER_ERROR_HANDLER. Durations convert with sm::us(), sm::ms() and
/// sm::s(); see there for literals.
#ifndef SM_TIMER_SERVICE_HPP
#define SM_TIMER_SERVICE_HPP

#include <cstddef>
#include <cstdint>

/// @name Settings
/// Define them before including this header, the same in every file, or with `-D`.
/// @{
#ifndef TS_TICKS_TYPE
/// The type of the tick counter, unsigned; default std::uint32_t.
#define TS_TICKS_TYPE std::uint32_t
#endif
#ifndef TS_TICK_US
/// The length of one tick in microseconds, default 1000; it must divide 1000.
#define TS_TICK_US 1000U
#endif
#ifndef TS_TIMER_ID_TYPE
/// The type of a machine's timer id, default std::uint16_t. The generator's
/// `TimerIdType` should name the same type.
#define TS_TIMER_ID_TYPE std::uint16_t
#endif
/// @}

/// The timer service for generated C++ state machines.
namespace sm {

using Ticks = TS_TICKS_TYPE;          ///< ticks of #TS_TICK_US microseconds
using TimerId = TS_TIMER_ID_TYPE;     ///< a machine's timer id; see #TS_TIMER_ID_TYPE

static_assert(static_cast<Ticks>(-1) > Ticks{0U}, "TS_TICKS_TYPE must be an unsigned type");
/// One tick in microseconds, #TS_TICK_US.
constexpr std::uint32_t tickUs = TS_TICK_US;
static_assert(tickUs > 0U && 1000U % tickUs == 0U, "TS_TICK_US must divide 1000");

/// The largest Ticks: the longest time a timer can have left. For a type
/// narrower than int, `~` works on the promoted int, so the cast is needed.
constexpr Ticks ticksMax = static_cast<Ticks>(~Ticks{0U});

/// How a timer repeats.
enum class TimerMode : std::uint8_t {
    Once,               ///< after(...): fires once
    Every               ///< every(...): fires until its state is left
};

/// The state of one timer as its machine keeps it. The machine declares one
/// per timer id, `sm::TimerState m_timerState[TM_COUNT]`, for its MachineTimers.
enum class TimerState : std::uint8_t {
    Idle,               ///< not started, stopped, or an after() whose timeout was taken
    Running,            ///< started; its timeout has not arrived
    Expired             ///< its timeout arrived and waits for a step without event
};

/// What went wrong, as the error handler is told.
enum class TimerError : std::uint8_t {
    TableFull = 1,      ///< start() found no free entry: the table is too small
    NoPeriod,           ///< start() of an every timer with 0 ticks
    TooLong,            ///< us(), ms() or s() at run time: the duration does not fit Ticks
    NoOutput            ///< poll() without out[] but with room for timeouts
};

class MachineTimers;

}  // namespace sm

// The error handler. Name your own with -DSM_TIMER_ERROR_HANDLER=myHandler, a
// function at global scope with this signature; see SM_TIMER_ERROR_HANDLER below.
#ifdef SM_TIMER_ERROR_HANDLER
void SM_TIMER_ERROR_HANDLER(sm::TimerError error, const sm::MachineTimers* timers, sm::TimerId id);
#else
#include <cstdio>
#include <cstdlib>
namespace sm {
/// The default error handler: prints the error to stderr and ends the
/// program with `exit(EXIT_FAILURE)`. Defined only without a handler of the
/// application's (#SM_TIMER_ERROR_HANDLER).
/// @param error   what went wrong
/// @param timers  the machine's timers of the call that failed; nullptr where it has none
/// @param id      the timer id of the call that failed; 0 where it has none
[[noreturn]] inline void timerErrorExit(TimerError error, const MachineTimers* timers, TimerId id)
{
    (void)std::fprintf(stderr, "timer service: error %d (timers %p, timer %lu)\n",
                       static_cast<int>(error), static_cast<const void*>(timers),
                       static_cast<unsigned long>(id));
    std::exit(EXIT_FAILURE);
}
}  // namespace sm
/// The name of the error handler, default sm::timerErrorExit.
///
/// Name your own with `-DSM_TIMER_ERROR_HANDLER=myHandler`, a function at
/// global scope with the parameters of sm::timerErrorExit():
/// `void myHandler(sm::TimerError error, const sm::MachineTimers* timers, sm::TimerId id)`;
/// timers and id are the call's, nullptr and 0 where it has none. The
/// default is defined only without it, so a target without exit() needs a
/// handler of its own. If a handler returns, the call that failed does
/// nothing and returns false or 0.
#define SM_TIMER_ERROR_HANDLER sm::timerErrorExit
#endif

namespace sm {

namespace detail {
constexpr std::uintmax_t perMs = 1000U / tickUs;     // ticks per millisecond
constexpr std::uintmax_t perS = 1000000UL / tickUs;  // ticks per second
// The longest duration in microseconds that fits Ticks: ticksMax ticks, or
// all of std::uintmax_t where that is less.
constexpr std::uintmax_t usMax =
    (static_cast<std::uintmax_t>(ticksMax) > static_cast<std::uintmax_t>(-1) / tickUs)
        ? static_cast<std::uintmax_t>(-1)
        : static_cast<std::uintmax_t>(ticksMax) * tickUs;

// A duration too long for the ticks: reported, and the longest there is.
inline Ticks tooLong()
{
    SM_TIMER_ERROR_HANDLER(TimerError::TooLong, nullptr, 0U);
    return ticksMax;
}

// One step of the long division below: the next 16 bits of the dividend.
inline void usStep(std::uintmax_t& q, std::uint32_t& r, std::uint32_t bits16)
{
    const std::uint32_t chunk = (r << 16U) | bits16;   // r < tickUs <= 1000: fits
    q = (q << 16U) | (chunk / tickUs);
    r = chunk % tickUs;
}

static_assert(sizeof(std::uintmax_t) == 8U, "usWide() divides a 64-bit std::uintmax_t");

// x / tickUs, rounded up, for x beyond 32 bits: long division in 16-bit
// steps, each a 32-bit division, and only shifts by constants, so a 32-bit
// target needs no 64-bit division or shift routine. x fits (usMax).
inline Ticks usWide(std::uintmax_t x)
{
    const std::uint32_t hi = static_cast<std::uint32_t>(x >> 32U);
    const std::uint32_t lo = static_cast<std::uint32_t>(x);
    std::uintmax_t q = 0U;
    std::uint32_t r = 0U;
    usStep(q, r, hi >> 16U);
    usStep(q, r, hi & 0xFFFFU);
    usStep(q, r, lo >> 16U);
    usStep(q, r, lo & 0xFFFFU);
    return static_cast<Ticks>(q + ((r != 0U) ? 1U : 0U));
}
}  // namespace detail

/// @name Whether a duration fits Ticks
/// Each compares with a limit computed at compile time, so a check at run
/// time divides nothing.
/// @{

/// @param x a duration in microseconds  @return true if us(x) fits Ticks
constexpr bool usFits(std::uintmax_t x) { return x <= detail::usMax; }
/// @param x a duration in milliseconds  @return true if ms(x) fits Ticks
constexpr bool msFits(std::uintmax_t x) { return x <= ticksMax / detail::perMs; }
/// @param x a duration in seconds  @return true if s(x) fits Ticks
constexpr bool sFits(std::uintmax_t x) { return x <= ticksMax / detail::perS; }
/// @}

/// @name Durations to ticks
/// us() rounds up, so a timer never fires early. A duration that does not
/// fit Ticks is an error: at compile time where the call is a constant
/// expression, else at run time through the error handler (TimerError::TooLong;
/// if it returns, the result is ticksMax). A negative value converts to a
/// very large one and is reported the same way. Once a duration fits, the
/// arithmetic is done in Ticks, or for us() in 32 bits: a 32-bit target needs
/// no 64-bit multiplication or division routine.
/// @{

/// @param x a duration in microseconds  @return the ticks, rounded up
constexpr Ticks us(std::uintmax_t x)
{
    return !usFits(x) ? detail::tooLong()
        : (x <= 0xFFFFFFFFU)
            ? static_cast<Ticks>(static_cast<std::uint32_t>(x) / tickUs
                                 + ((static_cast<std::uint32_t>(x) % tickUs != 0U) ? 1U : 0U))
            : detail::usWide(x);
}
/// @param x a duration in milliseconds  @return the ticks
constexpr Ticks ms(std::uintmax_t x)
{
    return msFits(x) ? static_cast<Ticks>(static_cast<Ticks>(x) * static_cast<Ticks>(detail::perMs)) : detail::tooLong();
}
/// @param x a duration in seconds  @return the ticks
constexpr Ticks s(std::uintmax_t x)
{
    return sFits(x) ? static_cast<Ticks>(static_cast<Ticks>(x) * static_cast<Ticks>(detail::perS)) : detail::tooLong();
}
/// @}

/// @name Literal durations to ticks
/// The same for literals, checked at compile time wherever they stand:
/// `start(id, sm::ms<200U>(), ...)` does not compile if 200 ms does not fit.
/// The generated code uses these for literal durations and the functions
/// above for expressions.
/// @{

/// @tparam X a duration in microseconds  @return the ticks, rounded up
template <std::uintmax_t X>
constexpr Ticks us()
{
    static_assert(usFits(X), "the duration does not fit sm::Ticks; see TS_TICKS_TYPE and TS_TICK_US");
    return us(X);
}
/// @tparam X a duration in milliseconds  @return the ticks
template <std::uintmax_t X>
constexpr Ticks ms()
{
    static_assert(msFits(X), "the duration does not fit sm::Ticks; see TS_TICKS_TYPE and TS_TICK_US");
    return ms(X);
}
/// @tparam X a duration in seconds  @return the ticks
template <std::uintmax_t X>
constexpr Ticks s()
{
    static_assert(sFits(X), "the duration does not fit sm::Ticks; see TS_TICKS_TYPE and TS_TICK_US");
    return s(X);
}
/// @}

/// The inverse of ms().
/// @param t ticks  @return whole milliseconds, rounded down
constexpr Ticks toMs(Ticks t) { return static_cast<Ticks>(t / detail::perMs); }

/// @name Arithmetic on times
/// For the commands increment and decrement of named timers, on the time
/// left of a running timer and on the time a named timer keeps while it
/// does not run.
/// @{

/// @param time a time  @param ticks the ticks to add
/// @return time + ticks, at most ticksMax
constexpr Ticks timeAdd(Ticks time, Ticks ticks)
{
    return (ticks > ticksMax - time) ? ticksMax : static_cast<Ticks>(time + ticks);
}
/// @param time a time  @param ticks the ticks to take off
/// @return time - ticks, at least 0
constexpr Ticks timeSub(Ticks time, Ticks ticks)
{
    return (ticks >= time) ? Ticks{0U} : static_cast<Ticks>(time - ticks);
}
/// @}

/// One expired timer, as TimerService::poll() reports it.
struct Timeout {
    MachineTimers* timers;      ///< the timers of the machine that started it
    TimerId timerId;            ///< its id there
    /// How many times it expired since the last report: 1, more for an every
    /// timer whose periods a late poll missed, i.e. an overrun. The timeout is
    /// handled once; the count lets the application recognise the overrun if
    /// it wants to. setTimeout() ignores it.
    std::uint16_t periods;
};

/// What a generated machine needs from a timer service.
///
/// The service knows a machine by its timers (sm::MachineTimers, a member of
/// the machine). A timer that does not run is no error: stop() does nothing,
/// and the functions for the time left answer false or 0.
class ITimerService {
public:
    /// @name Start and stop: needed by every machine with a timer
    /// @{

    /// Start or restart a timer.
    /// @param timers  the machine's timers, which identify the machine
    /// @param id      the machine's timer id
    /// @param ticks   the time until it expires, and an every timer's period;
    ///                0 with TimerMode::Every is TimerError::NoPeriod
    /// @param mode    TimerMode::Once or TimerMode::Every
    /// @retval true   started
    /// @retval false  refused (the error handler was told)
    virtual bool start(MachineTimers& timers, TimerId id, Ticks ticks, TimerMode mode) = 0;
    /// Stop a timer; stopping a timer that does not run does nothing.
    /// @param timers  the machine's timers
    /// @param id      the machine's timer id
    virtual void stop(MachineTimers& timers, TimerId id) = 0;
    /// @}

    /// @name The time left: needed only for TIME_LEFT and the commands of named timers
    /// A service built on other timers needs real implementations only for
    /// models that read or change a running time. Times count from the last
    /// poll. The generated set, increment and decrement are each one
    /// setTimeLeft().
    /// @{

    /// The ticks until a timer expires.
    /// @param timers  the machine's timers
    /// @param id      the machine's timer id
    /// @return the ticks left; 0 when the timer does not run or has expired
    virtual Ticks timeLeft(const MachineTimers& timers, TimerId id) const = 0;
    /// Let a running timer expire @p ticks from the last poll; an every
    /// timer keeps its period.
    /// @param timers  the machine's timers
    /// @param id      the machine's timer id
    /// @param ticks   the new time left; 0 means now, reported by the next poll
    /// @retval true   done
    /// @retval false  the timer does not run
    virtual bool setTimeLeft(MachineTimers& timers, TimerId id, Ticks ticks) = 0;
    /// @}

protected:
    ~ITimerService() = default;
};

/// The timers of one generated machine.
///
/// The machine holds them as a member, beside the array of timer states it
/// declares for them:
///
///     sm::TimerState m_timerState[TM_COUNT]{};
///     sm::MachineTimers m_timers;          // m_timers(m_timerState, timerService)
///
/// The service is the one the machine's constructor is given, so it must be
/// constructed before the machine and destroyed after it. Not copyable: the
/// service keeps the address.
///
/// The destructor is trivial, so a machine with static storage costs no
/// registration of a destructor at exit (`__cxa_atexit`). A machine destroyed
/// while its timers run must stop them first, or the service reports timers
/// that no longer exist: the generated destructor calls stopAll().
class MachineTimers {
public:
    /// The states are set idle.
    /// @tparam N        the number of timer ids
    /// @param state     the machine's timer states, one per timer id
    /// @param service   the service the machine's constructor is given
    template <std::size_t N>
    MachineTimers(TimerState (&state)[N], ITimerService& service)
        : m_state(state), m_count(static_cast<TimerId>(N)), m_service(service)
    {
        for (TimerId i = 0U; i < m_count; ++i) {
            m_state[i] = TimerState::Idle;
        }
    }
    MachineTimers(const MachineTimers&) = delete;
    MachineTimers& operator=(const MachineTimers&) = delete;

    /// @name For the application's loop
    /// @{

    /// Hand a timeout from the service to the machine. Ignored unless the
    /// timer is running, so a timeout of a timer the machine has stopped in
    /// the meantime has no effect; an id beyond the machine's is ignored too.
    /// @param id  the timer id from Timeout::timerId
    void setTimeout(TimerId id)
    {
        if (id < m_count && m_state[id] == TimerState::Running) {
            m_state[id] = TimerState::Expired;
        }
    }

    /// @return true while a timeout waits for a step without event
    bool timeoutPending() const
    {
        for (TimerId i = 0U; i < m_count; ++i) {
            if (m_state[i] == TimerState::Expired) {
                return true;
            }
        }
        return false;
    }
    /// @}

    /// @name For the generated code: the entry and exit code, and the timeout check
    /// @{

    /// Start (or restart) a timer in the service; the state is Running even
    /// if the service refuses.
    /// @param id     the timer id
    /// @param ticks  the time until it expires, and an every timer's period
    /// @param mode   TimerMode::Once or TimerMode::Every
    void start(TimerId id, Ticks ticks, TimerMode mode)
    {
        m_state[id] = TimerState::Running;
        (void)m_service.start(*this, id, ticks, mode);
    }
    /// Stop a timer in the service, and set it idle.
    /// @param id  the timer id
    void stop(TimerId id)
    {
        m_service.stop(*this, id);
        m_state[id] = TimerState::Idle;
    }
    /// @param id  the timer id  @return true if its timeout waits to be taken
    bool expired(TimerId id) const { return m_state[id] == TimerState::Expired; }
    /// Take a timeout: after(...) is spent, every(...) runs on.
    /// @param id    the timer id
    /// @param mode  the timer's mode
    void consume(TimerId id, TimerMode mode)
    {
        m_state[id] = (mode == TimerMode::Every) ? TimerState::Running : TimerState::Idle;
    }
    /// @}

    /// @name For TIME_LEFT and the commands of named timers
    /// A timer that does not run answers 0 or false: the generated code then
    /// uses the time the machine keeps for it.
    /// @{

    /// @param id  the timer id  @return true unless the timer is idle
    bool runs(TimerId id) const { return m_state[id] != TimerState::Idle; }
    /// @param id  the timer id  @return the service's ITimerService::timeLeft()
    Ticks timeLeft(TimerId id) const { return m_service.timeLeft(*this, id); }
    /// @param id     the timer id
    /// @param ticks  the new time left
    /// @return the service's ITimerService::setTimeLeft()
    bool setTimeLeft(TimerId id, Ticks ticks) { return m_service.setTimeLeft(*this, id, ticks); }
    /// @}

    /// Stop every timer that is not idle; called by the generated destructor
    /// (`StateMachineClassHasDestructor=yes`).
    void stopAll()
    {
        for (TimerId i = 0U; i < m_count; ++i) {
            if (m_state[i] != TimerState::Idle) {
                stop(i);
            }
        }
    }

private:
    TimerState* m_state;
    TimerId m_count;
    ITimerService& m_service;
};

/// The reference service: a table of N entries, searched linearly.
///
/// Each running timer holds the ticks it has left, and poll() counts them all
/// down by the time since the last poll.
/// @tparam N  the number of entries: the most timers, of all machines, that run at once
template <std::size_t N>
class TimerService final : public ITimerService {
public:
    /// constexpr: a global service is constant-initialized, so it is ready
    /// before any constructor runs, in any file. A machine elsewhere may
    /// therefore start timers in its constructor (`CallInitializeInCtor`).
    /// @param now  the current time; the first poll counts from it
    constexpr explicit TimerService(Ticks now = 0U) : m_table(), m_now(now), m_refused(0U) {}

    /// @copydoc ITimerService::start
    /// A running timer is restarted; a full table is TimerError::TableFull,
    /// counted in refused().
    bool start(MachineTimers& timers, TimerId id, Ticks ticks, TimerMode mode) override
    {
        if (mode == TimerMode::Every && ticks == 0U) {
            SM_TIMER_ERROR_HANDLER(TimerError::NoPeriod, &timers, id);
            return false;
        }
        Entry* entry = entryOf(&timers, id);            // running already: restart it
        if (entry == nullptr) {
            entry = firstFreeEntry();                   // else a free entry
        }
        if (entry == nullptr) {
            ++m_refused;
            SM_TIMER_ERROR_HANDLER(TimerError::TableFull, &timers, id);
            return false;
        }
        entry->timers = &timers;
        entry->id = id;
        entry->every = (mode == TimerMode::Every);
        entry->period = ticks;
        setLeft(*entry, ticks);
        return true;
    }

    void stop(MachineTimers& timers, TimerId id) override
    {
        Entry* entry = entryOf(&timers, id);
        if (entry != nullptr) {
            entry->timers = nullptr;
        }
    }

    Ticks timeLeft(const MachineTimers& timers, TimerId id) const override
    {
        for (const Entry& entry : m_table) {
            if (entry.timers == &timers && entry.id == id) {
                return left(entry);
            }
        }
        return 0U;
    }

    /// @copydoc ITimerService::setTimeLeft
    /// A timeout that expired but was not reported yet is taken back.
    bool setTimeLeft(MachineTimers& timers, TimerId id, Ticks ticks) override
    {
        Entry* entry = entryOf(&timers, id);
        if (entry == nullptr) {
            return false;
        }
        setLeft(*entry, ticks);        // an every timer keeps its period from then on
        return true;
    }

    /// Stop every timer of one machine, e.g. before it goes away.
    /// @param timers  the machine's timers
    void stopAll(const MachineTimers& timers)
    {
        for (Entry& entry : m_table) {
            if (entry.timers == &timers) {
                entry.timers = nullptr;
            }
        }
    }

    /// Count the running timers down by the time since the last poll, and
    /// report the ones that expired in out[], one entry per timer.
    ///
    /// A once timer is removed once reported, an every timer goes on with its
    /// next period; periods missed by a late poll are counted in
    /// Timeout::periods, still one entry (an overrun, for the application to
    /// check if it wants to).
    ///
    /// @param now  the current time
    /// @param out  receives the expired timers; nullptr with @p max above 0
    ///             is TimerError::NoOutput, and if the handler returns, the
    ///             timers are still counted down and none is reported
    /// @param max  the number of entries @p out has room for
    /// @return n, the number of entries written to out[0] .. out[n-1]: the
    ///         number of timers reported, not of periods. It is at most
    ///         @p max. When it equals @p max, more timers may have expired;
    ///         they stay expired and the next call reports them (a call with
    ///         the same @p now does so at once).
    /// @retval 0 nothing expired, or @p out is nullptr (after the error handler)
    std::size_t poll(Ticks now, Timeout* out, std::size_t max)
    {
        std::size_t room = max;
        if (out == nullptr && max > 0U) {
            SM_TIMER_ERROR_HANDLER(TimerError::NoOutput, nullptr, 0U);
            room = 0U;                 // if the handler returns: count down, report nothing
        }
        const Ticks elapsed = static_cast<Ticks>(now - m_now);   // modulo the range: a wrap is harmless
        m_now = now;
        std::size_t n = 0U;
        for (Entry& entry : m_table) {
            if (entry.timers != nullptr) {
                const std::uint16_t ended = countDown(entry, elapsed);
                if (ended > 0U) {
                    entry.pending = true;
                    entry.periods = addPeriods(entry.periods, ended);
                }
                if (entry.pending && n < room) {
                    out[n] = Timeout{entry.timers, entry.id, entry.periods};
                    ++n;
                    entry.pending = false;
                    entry.periods = 0U;
                    if (!entry.every) {
                        entry.timers = nullptr;
                    }
                }
            }
        }
        return n;
    }

    /// poll() into an array, with its size as max.
    /// @tparam M   the array's size
    /// @param now  the current time
    /// @param out  receives the expired timers
    /// @return the number of entries written, as poll(Ticks, Timeout*, std::size_t)
    template <std::size_t M>
    std::size_t poll(Ticks now, Timeout (&out)[M])
    {
        return poll(now, out, M);
    }

    /// @return the number of running timers, of all machines
    std::size_t running() const
    {
        std::size_t n = 0U;
        for (const Entry& entry : m_table) {
            if (entry.timers != nullptr) {
                ++n;
            }
        }
        return n;
    }

    /// @return the starts refused because every entry was in use (TimerError::TableFull)
    std::uint16_t refused() const { return m_refused; }

private:
    struct Entry {
        MachineTimers* timers = nullptr;        // nullptr: free
        TimerId id = 0U;
        bool every = false;                     // goes on with its period after expiring
        bool pending = false;                   // expired, but poll() had no room for it yet
        std::uint16_t periods = 0U;             // expiries not reported yet; more than 1 after a late poll
        Ticks period = 0U;                      // an every timer's period
        Ticks left = 0U;                        // ticks until it expires, from the last poll
    };

    // The entry of the timer if it runs. nullptr is an answer,
    // not an error: the timer does not run.
    Entry* entryOf(const MachineTimers* timers, TimerId id)
    {
        for (Entry& entry : m_table) {
            if (entry.timers == timers && entry.id == id) {
                return &entry;
            }
        }
        return nullptr;
    }

    // The first free entry, for a timer that starts; nullptr when all are in use.
    Entry* firstFreeEntry()
    {
        for (Entry& entry : m_table) {
            if (entry.timers == nullptr) {
                return &entry;
            }
        }
        return nullptr;
    }

    // Ticks the entry has left; 0 once it has expired.
    static Ticks left(const Entry& entry) { return entry.pending ? Ticks{0U} : entry.left; }

    // Give the entry `ticks` left. An expiry that was not reported yet is taken back.
    static void setLeft(Entry& entry, Ticks ticks)
    {
        entry.left = ticks;
        entry.pending = false;
        entry.periods = 0U;
    }

    // Count the entry down by `elapsed` ticks; how many times it expired within
    // them, 0 for none. An every timer then goes on with its next period: the
    // phase is kept, and each period a late poll missed counts (at most 65535).
    static std::uint16_t countDown(Entry& entry, Ticks elapsed)
    {
        if (entry.left > elapsed) {
            entry.left = static_cast<Ticks>(entry.left - elapsed);
            return 0U;
        }
        if (entry.every) {
            const Ticks late = static_cast<Ticks>(elapsed - entry.left);
            const Ticks missed = static_cast<Ticks>(late / entry.period);
            entry.left = static_cast<Ticks>(entry.period - late % entry.period);
            return missed >= Ticks{0xFFFFU} ? std::uint16_t{0xFFFFU} : static_cast<std::uint16_t>(missed + 1U);
        }
        entry.left = 0U;
        return 1U;
    }

    // a + b, at most 65535
    static std::uint16_t addPeriods(std::uint16_t a, std::uint16_t b)
    {
        return b > static_cast<std::uint16_t>(0xFFFFU - a) ? std::uint16_t{0xFFFFU} : static_cast<std::uint16_t>(a + b);
    }

    Entry m_table[N];
    Ticks m_now;
    std::uint16_t m_refused;
};

}  // namespace sm

#endif  // SM_TIMER_SERVICE_HPP
