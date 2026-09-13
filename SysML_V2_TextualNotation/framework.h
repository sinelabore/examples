#pragma once

#include <iostream>
#include <queue>
#include <functional>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include <atomic>
#include <string>
#include <memory>
#include <stdexcept>
#include <limits>
#include <initializer_list>
#include <cmath>
#include <type_traits>
#include <variant>
#include <iomanip>
// std::ostringstream, used by the timestamp in the default trace sink. libstdc++ and
// libc++ reach it through <iostream>; the Microsoft STL does not, and cl reports
// "'oss' uses undefined class std::basic_ostringstream".
#include <sstream>

enum class PreFillPolicy { None, Min, Max, FixedOnly };

/**
 * ConstrainedVector is a template wrapper around std::vector<T> that 
 * enforces minimum and maximum size constraints. It prevents the vector 
 * from growing beyond a maximum or shrinking below a minimum.
 */
template<typename T>
class ConstrainedVector {
private:
    std::vector<T> data_; // underlying vector data
    size_t min_size_; // minimum required elements
    size_t max_size_; // maximum allowed elements
    std::string name_; // name of the vector
    
public:
    // Constructor with PreFillPolicy
    ConstrainedVector(size_t min_sz, size_t max_sz,
                      PreFillPolicy policy = PreFillPolicy::FixedOnly,
                      const std::string& name = "")
        : min_size_(min_sz), max_size_(max_sz), name_(name)
    {
        if (min_sz > max_sz) {
            throw std::invalid_argument(name + ": min_size > max_size");
        }

        //data_.reserve(max_sz);

        // Decide how many elements to pre-fill
        size_t fill_count = 0;
        switch (policy) {
            case PreFillPolicy::None:
                fill_count = 0;
                break;
            case PreFillPolicy::Min:
                fill_count = min_size_;
                break;
            case PreFillPolicy::Max:
                fill_count = max_size_;
                break;
            case PreFillPolicy::FixedOnly:
                fill_count = (min_size_ == max_size_) ? min_size_ : 0;
                break;
        }
        std::cout << "emplace:" << fill_count << std::endl;
        for (size_t i = 0; i < fill_count; ++i){
            data_.emplace_back();  // default-construct T
        }
    }  
    
    
    // Constructor with initializer list
    ConstrainedVector(size_t min_sz, size_t max_sz, const std::string& name,
                     std::initializer_list<T> init)
        : min_size_(min_sz), max_size_(max_sz), name_(name) {
        if (init.size() > max_sz) {
            throw std::invalid_argument(
                name + ": initializer list size " + std::to_string(init.size()) +
                " exceeds maximum " + std::to_string(max_sz)
            );
        }
        data_.assign(init.begin(), init.end());
    }
    
    /**
     * Constructor with vector.
     * @param min_sz - the minimum size of the vector
     * @param max_sz - the maximum size of the vector
     * @param name - the name of the vector
     * @param vec - the vector to initialize the constrained vector with
     */
    ConstrainedVector(size_t min_sz, size_t max_sz, const std::string& name,
                     const std::vector<T>& vec)
        : min_size_(min_sz), max_size_(max_sz), name_(name) {
        if (vec.size() > max_sz) {
            throw std::invalid_argument(
                name + ": vector size " + std::to_string(vec.size()) +
                " exceeds maximum " + std::to_string(max_sz)
            );
        }
        data_ = vec;
    }
    
    /**
     * Push a new element to the end of the vector.
     * @param value - the element to push back
     * @return true if the element was pushed back, false if the vector is full
     */
    bool push_back(const T& value) {
        if (data_.size() >= max_size_) {
            return false;
        }
        data_.push_back(value);
        return true;
    }
    
    /**
     * Emplace a new element to the end of the vector. 
     * @tparam Args - the types of the arguments to construct the element
     * @param args - the arguments to construct the element
     * @return true if the element was emplaced, false if the vector is full
     */
    template<typename... Args>
    bool emplace_back(Args&&... args) {
        if (data_.size() >= max_size_) {
            return false;
        }
        data_.emplace_back(std::forward<Args>(args)...);
        return true;
    }
    
    /**
     * Check if the vector is valid. 
     * @return true if the vector is valid, false otherwise
     */
    bool isValid() const {
        return data_.size() >= min_size_ && data_.size() <= max_size_;
    }
    
    size_t size() const { return data_.size(); }
    size_t minSize() const { return min_size_; }
    size_t maxSize() const { return max_size_; }
    bool isUnbounded() const { return max_size_ == SIZE_MAX; }
    
    /**
     * Access the element at the given index.
     * @param index - the index of the element to access
     * @return a reference to the element at the given index
     */
    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
    
    /**
     * Reserve capacity for the vector.
     * @param capacity - the capacity to reserve
     */
    void reserve(size_t capacity) {
        if (capacity <= max_size_) {
            data_.reserve(capacity);
        }
    }
    
    /**
     * Clear the vector.
     */ 
    void clear() { data_.clear(); }
};

// --- Threadsafe Queue ---
template<typename T>
class ThreadsafeQueue {
    std::queue<T> q;
    mutable std::mutex m;
public:
    void push(T v) {
        std::lock_guard<std::mutex> lock(m);
        q.push(std::move(v));
    }
    bool try_pop(T& v) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) return false;
        v = std::move(q.front());
        q.pop();
        return true;
    }
    size_t size() const {
        std::lock_guard<std::mutex> lock(m);
        return q.size();
    }
};

// --- Port (Output) ---
template<typename T>
class Port {
    std::vector<std::function<void(const T&)>> receivers;
public:
    void send(const T& data) {
        for (auto& r : receivers) {
            if (r) r(data);
        }
    }
    void addReceiver(std::function<void(const T&)> r) {
        receivers.push_back(std::move(r));
    }
};

// --- InputPort: T = std::variant<Payload,...>. Payload type = first alternative (e.g. PortEventDef).
template<typename T>
using PayloadType = std::variant_alternative_t<0, T>;

// --- InputPort (Input queue) ---
// Inherits from payload type (e.g. PortEventDef) so port attributes (msg, etc.) are direct members.
// Codegen can emit SysML access as-is: myReceivePort.msg.theEvent (no extra "current").
//
// Polling is the only consumption path: a state machine takes events off the queue itself, one per
// hasEvent() call, from whichever state is active - matching the SysML v2 rule that a transition is
// only triggered "during a performance of its source". There is deliberately no drain-and-dispatch
// alternative: two ways to consume the same queue can race for the same event.
//
// setObserver() is a read-only side channel: it is told about an arrival but does not pop and must
// never itself dispatch a state machine. It runs synchronously on the sender's thread, inside
// Port::send(), so it sees the event before the receiving machine polls for it - keep it cheap.
template<typename T>
class InputPort : public PayloadType<T> {
    ThreadsafeQueue<T> queue;
    std::function<void(const PayloadType<T>&)> observer;
public:
    void receive(const T& data) {
        if (observer) {
            observer(std::get<PayloadType<T>>(data));
        }
        queue.push(data);
    }

    // if data is available load it into the port and return true,
    // otherwise return false
    bool hasEvent() {
        T data;
        if (!queue.try_pop(data)) {
            return false;
        }
        static_cast<PayloadType<T>&>(*this) = std::get<PayloadType<T>>(data);
        return true;
    }

    PayloadType<T> getPayload() const {
        return static_cast<const PayloadType<T>&>(*this);
    }

    /** Number of events queued but not yet taken by hasEvent(). Read-only. */
    size_t queueDepth() const {
        return queue.size();
    }

    /**
     * Read-only side channel: told about every arrival, in receive() order, before it is
     * queued - unlike hasEvent(), it does not consume anything and can never race polling for
     * the same event.
     */
    void setObserver(std::function<void(const PayloadType<T>&)> o) {
        observer = std::move(o);
    }
};

// --- Connect ports ---
template<typename T>
void connect(Port<T>* out, InputPort<T>* in) {
    out->addReceiver([in](const T& data) {
        in->receive(data);
    });
}

inline std::string currentTime()
{
    auto now = std::chrono::system_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    // Not std::localtime: it returns a pointer into one static buffer shared by every
    // thread, and this framework is threaded. Both replacements write into the caller's
    // own tm, but no standard library offers both - glibc and Apple's libc have the POSIX
    // localtime_r and not localtime_s, the Microsoft CRT the reverse, and its localtime_s
    // takes its arguments the other way round and returns errno_t. C11 Annex K, which
    // would have settled it, went unimplemented outside Microsoft.
    std::tm tm{};
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << tm.tm_hour << ":"
        << std::setw(2) << tm.tm_min << ":"
        << std::setw(2) << tm.tm_sec << ":"
        << std::setw(3) << ms.count();

    return oss.str();
}


inline std::chrono::milliseconds msFromTimeUnit(double value, const std::string& unit) {
    // Precomputed scale factors to milliseconds
    static const std::unordered_map<std::string, double> unitToMs{
        {"SI::nanosecond", 1.0 / 1e6},
        {"SI::microsecond", 1.0 / 1000.0},
        {"SI::millisecond", 1.0},
        {"SI::second", 1000.0},
        {"SI::minute", 60.0 * 1000.0},
        {"SI::hour", 3600.0 * 1000.0}
    };

    auto it = unitToMs.find(unit);
    if (it == unitToMs.end()) {
        throw std::invalid_argument("Unknown time unit: " + unit);
    }

    double ms = value * it->second;
    return std::chrono::milliseconds(static_cast<long long>(std::round(ms)));
}

// --- Part base class ---
/*
class Part {
public:
    virtual void process() {};
    virtual void init() {};
    virtual ~Part() = default;
};
*/

// --- Part base class ---
class Part {

        // Containment path of this object, e.g. "System.monitor" - see setInstanceName.
        std::string instanceName_;

    public:

        Part() {}

        /**
         * Where this object sits in the model, as a path from the root part.
         *
         * C++ has no way to recover a variable's name at run time (RTTI gives the type, never
         * the instance), so the generator supplies it: each part's constructor seeds this with
         * its own type name, and its owner overwrites it with the full path in init(). That
         * ordering means an object always has a usable name even if init() was never called.
         *
         * The root keeps its type name, since nothing owns it and the name of the variable
         * holding it is not visible to generated code - call setInstanceName() on it to use the
         * name from your main().
         */
        const std::string& instanceName() const { return instanceName_; }

        void setInstanceName(std::string name) { instanceName_ = std::move(name); }

        /** Names this object as {@code <owner>.<member>}; called by the owner during init(). */
        void setOwnedName(const Part& owner, const char* member) {
            instanceName_ = owner.instanceName_.empty()
                    ? std::string(member)
                    : owner.instanceName_ + "." + member;
        }

        virtual void process() {};
        virtual void init() {};
        virtual ~Part() = default;
    };

// --- Tracing ---------------------------------------------------------------
//
// Generated code reports what it is doing through SYSML_TRACE, and only when it
// was generated with -d. The indirection exists so the report can be routed
// somewhere other than std::cout: an embedded target without <iostream> defines
// SYSML_TRACE to its own logger (or to nothing) before including this header and
// pays no cost at all, since nothing below is then referenced.
//
// The payload names the SysML construct, not the C++ that implements it - the
// point is to be able to read a run against the model that produced it.

/**
 * What one call to an action's operator() achieved.
 *
 * An action body is generated as a step function rather than a run-to-completion call,
 * because a SysML action may legitimately never complete - a behaviour whose successions
 * form a cycle with no 'done' node (an engine that idles, runs and stops, forever) has no
 * terminating execution at all. Each call therefore does a bounded amount of work and says
 * whether there is more, leaving the caller in charge of the loop.
 */
enum class ActionStatus {
    Finished,  // reached a done/terminate node; the action is complete and may be re-run
    Yielded,   // completed one pass around a loop; call again to continue
    Blocked    // waiting on an accept that has not been satisfied; nothing changed
};

inline const char* actionStatusName(ActionStatus status) {
    switch (status) {
    case ActionStatus::Finished: return "finished";
    case ActionStatus::Yielded:  return "yielded";
    case ActionStatus::Blocked:  return "blocked";
    }
    return "?";
}

/** What kind of SysML construct is reporting. */
enum class TraceKind {
    Action,      // an action def / action usage body running
    Accept,      // accept ... via <port> taking an event
    Assign,      // assign x := ...
    // Both report as "<source> -> <target>", i.e. the direction the value actually moved,
    // which for a bind is the reverse of how it is written ('bind target = source').
    Flow,        // flow <source> to <target>  - a value streamed between action pins
    Bind,        // bind <target> = <source>   - a binding connector transferring a value
    Send,        // send ... via <port>
    StateEntry,  // entry into a state
    StateExit,   // exit out of a state
    Transition   // a transition firing, including its guard/trigger
};

inline const char* traceKindName(TraceKind kind) {
    switch (kind) {
    case TraceKind::Action:     return "action";
    case TraceKind::Accept:     return "accept";
    case TraceKind::Assign:     return "assign";
    case TraceKind::Flow:       return "flow";
    case TraceKind::Bind:       return "bind";
    case TraceKind::Send:       return "send";
    case TraceKind::StateEntry: return "entry";
    case TraceKind::StateExit:  return "exit";
    case TraceKind::Transition: return "transition";
    }
    return "?";
}

/**
 * One thing the model did.
 *
 * `context` is the owning part or state machine as written in the model, and
 * `instance` distinguishes two objects of that same type - without it a trace
 * from one of several identical parts is unattributable. `detail` carries
 * whatever the construct makes concrete: the target state of a transition, the
 * port an accept drew from, and is null when there is nothing to add.
 */
struct TraceEvent {
    TraceKind   kind;
    const char* context;
    const char* name;
    const char* detail;
    const void* instance;
};

/** Where trace events go. Replace to redirect; see SYSML_TRACE to remove entirely. */
using TraceSink = std::function<void(const TraceEvent&)>;

inline TraceSink& traceSink() {
    static TraceSink sink = [](const TraceEvent& e) {
        std::cout << "debug: ";
        if (e.context != nullptr && e.context[0] != '\0') {
            std::cout << e.context << '.';
        }
        std::cout << e.name << " [" << traceKindName(e.kind) << ']';
        if (e.detail != nullptr) {
            std::cout << ' ' << e.detail;
        }
        std::cout << std::endl;
    };
    return sink;
}

/**
 * Where an absolute point in time comes from, for 'accept at <instant>'.
 *
 * Wall clock, not the steady clock the 'accept after <duration>' timers use: a steady clock is
 * monotonic but its zero is arbitrary, so it can measure elapsed time and nothing else. An
 * instant has to be compared against something with the same origin as the value the model
 * holds.
 *
 * Replaceable for the same reason traceSink() is - a test that has to reach a given instant
 * otherwise waits for the real clock to get there, which is neither quick nor repeatable. Set
 * this to a function returning a fixed value and a timed transition becomes as deterministic as
 * any other:
 *
 *     double fakeNow = 0.0;
 *     timeSource() = [&fakeNow] { return fakeNow; };
 *
 * Seconds since the epoch as a double, matching how Time::DateTime values are compared. The
 * SysML library types an instant as TimeInstantValue with a TimeScale it is measured against;
 * that scale is not modelled here, so two instants are only comparable if the model means them
 * on the same one.
 */
using TimeSource = std::function<double()>;

inline TimeSource& timeSource() {
    static TimeSource source = [] {
        return std::chrono::duration<double>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    };
    return source;
}

/**
 * True only when {@code condition} has just become true - the edge a 'when' trigger waits for.
 *
 * SysML's TriggerWhen monitors its condition "for changing from false to true" (Triggers.kerml),
 * so a transition guarded by one fires once per change, not on every cycle the condition happens
 * to hold. {@code previous} is the machine's memory of the last value seen, cleared when the
 * source state is entered - ObserveChange waits only "if the result of the changeSignal.
 * signalCondition is false", so a condition already true when the observation starts still fires,
 * and it is the repeats that are suppressed.
 */
inline bool sysmlOnRisingEdge(bool& previous, bool condition) {
    bool rising = condition && !previous;
    previous = condition;
    return rising;
}

/** Current instant, or 0 if the source was cleared. */
inline double sysmlNow() {
    const TimeSource& source = timeSource();
    return source ? source() : 0.0;
}

inline void emitTrace(TraceKind kind, const char* context, const char* name,
                      const char* detail, const void* instance) {
    const TraceSink& sink = traceSink();
    if (sink) {
        sink(TraceEvent{kind, context, name, detail, instance});
    }
}

#ifndef SYSML_TRACE
#define SYSML_TRACE(kind, context, name, detail, instance) \
    ::emitTrace((kind), (context), (name), (detail), (instance))
#endif
