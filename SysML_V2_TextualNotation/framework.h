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
    std::mutex m;
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

// --- InputPort (Input queue + processing) ---
// Inherits from payload type (e.g. PortEventDef) so port attributes (msg, etc.) are direct members.
// Codegen can emit SysML access as-is: myReceivePort.msg.theEvent (no extra "current").
template<typename T>
class InputPort : public PayloadType<T> {
    ThreadsafeQueue<T> queue;
    std::vector<std::function<void(const T&)>> handlers;
public:
    void receive(const T& data) {
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

    void process() {
        T data;
        while (queue.try_pop(data)) {
            static_cast<PayloadType<T>&>(*this) = std::get<PayloadType<T>>(data);
            for (auto& h : handlers) {
                if (h) h(data);
            }
        }
    }

    void addHandler(std::function<void(const T&)> h) {
        handlers.push_back(std::move(h));
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
    std::tm tm = *std::localtime(&t);

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
    
    public:
        
        Part() {}
             
        virtual void process() {};
        virtual void init() {};
        virtual ~Part() = default;
    };
