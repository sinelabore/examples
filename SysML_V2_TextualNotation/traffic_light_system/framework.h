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

// map time value and unit to milli seconds
inline std::chrono::milliseconds msFromTimeUnit(float tv, const std::string& unit) {
    using namespace std::chrono;

    if (unit == "SI::hour") {
        return duration_cast<milliseconds>(duration<float, std::ratio<3600>>(tv));
    } else if (unit == "SI::minute") {
        return duration_cast<milliseconds>(duration<float, std::ratio<60>>(tv));
    } else if (unit == "SI::second") {
        return duration_cast<milliseconds>(duration<float>(tv));
    } else {
        return milliseconds(0);
    }
}

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

// --- InputPort (Input queue + processing) ---
template<typename T>
class InputPort {
    ThreadsafeQueue<T> queue;
    std::vector<std::function<void(const T&)>> handlers;
public:
    void receive(const T& data) {
        queue.push(data);
    }

    void process() {
        T data;
        while (queue.try_pop(data)) {
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

// --- TimeoutManager with generic callback and cancellation ---
class TimeoutManager {
public:
    using Callback = std::function<void()>;
    using TimeoutHandle = int64_t;
    std::thread workerThread;

    TimeoutManager() {
        running = true;
        nextId = 1;
        workerThread = std::thread([this]() {
            run();
        });
    }

    ~TimeoutManager() {
        stop();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

private:
    struct Entry {
        std::chrono::steady_clock::time_point time;
        TimeoutHandle id;
        Callback cb;
        bool operator<(const Entry& other) const {
            return time > other.time;  // min-heap
        }
    };

    std::priority_queue<Entry> queue;
    std::unordered_set<TimeoutHandle> canceled;
    std::mutex m;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::atomic<TimeoutHandle> nextId;

public:

    TimeoutHandle startTimer(std::chrono::milliseconds delay, Callback cb) {
        TimeoutHandle id = nextId++;
        auto time = std::chrono::steady_clock::now() + delay;
        {
            std::lock_guard<std::mutex> lock(m);
            queue.push({time, id, std::move(cb)});
        }
        cv.notify_one();
        return id;
    }

    void cancelTimer(TimeoutHandle id) {
        std::lock_guard<std::mutex> lock(m);
        canceled.insert(id);
    }

    void run() {
        std::unique_lock<std::mutex> lock(m);
        while (running) {
            if (queue.empty()) {
                cv.wait(lock);
            } else {
                auto now = std::chrono::steady_clock::now();
                auto& top = queue.top();
                if (now >= top.time) {
                    Entry e = std::move(top);
                    queue.pop();

                    if (canceled.count(e.id)) {
                        canceled.erase(e.id);
                        continue;
                    }

                    lock.unlock();
                    e.cb();
                    lock.lock();
                } else {
                    cv.wait_until(lock, top.time);
                }
            }
        }
    }

    void stop() {
        running = false;
        cv.notify_all();
    }
};

// --- Part base class ---
class Part {
public:
    virtual void process() = 0;
    virtual ~Part() = default;
};
