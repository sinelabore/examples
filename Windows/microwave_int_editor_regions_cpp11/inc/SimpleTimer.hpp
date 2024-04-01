/**
 * @file SimpleTimer.hpp
 * @author pmueller
 * @brief Reusable timer service
 * @version 0.2
 * @date 2024-02-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include "SafeQueue.hpp"

/**
 * @brief Represents the state of a SimpleTimer instance.
 * 
 * The SimpleTimerState enum class defines the possible states of a SimpleTimer
 * instance: OFF, ON, and PAUSE. These states indicate whether the timer is 
 * currently inactive, active, or paused, respectively.
 */
enum class SimpleTimerState : uint8_t {
    OFF,  ///< Timer is inactive.
    ON,   ///< Timer is active.
    PAUSE ///< Timer is paused.
};

/**
 * @brief Represents the kind of a SimpleTimer instance.
 * 
 * The SimpleTimerKind enum class defines the types of SimpleTimer instances:
 * SINGLE and CYCLIC. These types determine whether the timer is intended to 
 * fire only once or repeatedly at specified intervals, respectively.
 */
enum class SimpleTimerKind : uint8_t {
    SINGLE, ///< Timer fires only once.
    CYCLIC, ///< Timer fires repeatedly at specified intervals.
};

/**
 * @brief A simple timer implementation for scheduling events.
 * 
 * The SimpleTimer class provides a timer functionality
 * for scheduling events with specified timeouts. It allows starting, 
 * stopping, pausing, and resuming timers, as well as dynamically adjusting 
 * the timeout value.
 * 
 * This class is templated, allowing it to work with any type of event
 * message that can be stored in a SafeQueue. It internally manages a thread 
 * for timer operations, ensuring asynchronous behavior without blocking the 
 * main execution thread.
 * 
 * Usage:
 * 
 * 1. Create an instance of SimpleTimer with a reference to a SafeQueue.
 * 2. Use the create() method to schedule a new timer with an event message.
 * 3. Start the timer using the start() method with the desired timeout.
 * 4. Optionally adjust the timeout using inc() or dec() methods.
 * 5. Stop the timer with the stop() method.
 * 6. Pause and resume the timer using pause() and cont() methods.
 * 
 * @tparam T The type of event message to be stored in the SafeQueue.
 * 
 * @see SafeQueue
 */
template <class T>  class SimpleTimer {
  public:

    /**
     * @brief Constructs a SimpleTimer instance with a reference to a SafeQueue.
     * 
     * This constructor initializes a SimpleTimer instance with a reference to 
     * a SafeQueue where timeout messages will be stored. It sets the initial 
     * state of the timer to SimpleTimerState::OFF and initializes other member 
     * variables such as timeout value, elapsed time, and timer kind.
     * 
     * It's important to note that the caller must ensure the lifetime of the 
     * provided SafeQueue outlasts the lifetime of the SimpleTimer instance, as 
     * the timer may access the queue asynchronously during its operation.
     * 
     * @param q A reference to a SafeQueue<T> where timeout messages will be stored.
     * 
     * Example usage:
     * 
     * SafeQueue<std::string> queue;
     * SimpleTimer<std::string> timer(queue);
     * 
     * @see SafeQueue
     */
    explicit SimpleTimer(SafeQueue<T>& q) : squeue(q), state(SimpleTimerState::OFF), 
            millis(0), elapsed(0), kind(SimpleTimerKind::SINGLE) {
    }

    ~SimpleTimer() {
        myThread.join();
    }

    /**
     * @brief 
     * Shedule a new timer with timeout time t. The event is stored in queue q.
     * 
     * @param message contains the event that is put into the queue on timeout.
     */
    void create(T message) {
        state = SimpleTimerState::OFF;
        msg = message;
        clear();
        elapsed = 0;
        myThread = std::thread(&SimpleTimer::workerTread, this);
    }

    void start(SimpleTimerKind timerKind, uint32_t timeout) {
        elapsed = 0U;
        millis = timeout;
        kind = timerKind;
        state = SimpleTimerState::ON;
    }

    /**
     * @brief get actually set timeout value
     * 
     * @return uint16_t returns the set timeout value
     */
    uint16_t preset() {
        return millis;
    }

    /**
     * @brief Increments the timer by 1 second
     * 
     */
    void inc() {
        millis += 1000;
        std::cout << "Timer timeout set to "
                  << millis << " milli seconds" << std::endl;
    }

    /**
     * @brief Decrements the timer by 1 second
     * 
     */
    void dec() {
        if (millis >= 1000) {
            millis -= 1000;
        }
        std::cout << "Timer timeout set to "
                  << millis << " milli seconds" << std::endl;
    }

    /**
     * @brief Stop the timer
     * 
     */
    void stop() {
        state = SimpleTimerState::OFF;
        std::cout << "Timer stopped" << std::endl;
    }

    void clear() {
        millis = 0U;
    }

    /**
     * @brief Pause the timer at the current time. Can be continued
     * at any time again.
     * 
     */
    void pause() {
        state = SimpleTimerState::PAUSE;
        std::cout << "Timer paused" << std::endl;
    }

    /**
     * @brief Continue timer at a time previously paused.
     * 
     */
    void cont() {
        if (state == SimpleTimerState::PAUSE) {
            state = SimpleTimerState::ON;
            std::cout << "Timer continued" << std::endl;
        }
    }

 private:
    SimpleTimerState state;
    uint16_t millis;
    uint32_t elapsed;
    SafeQueue<T>& squeue;
    T msg;
    SimpleTimerKind kind;
    std::thread myThread;

    void workerTread(){
        while (1) {
            while (state == SimpleTimerState::ON) {
                // sleep for defined millis
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                elapsed += 10;
                if (elapsed % 100 == 0) {
                    std::cout << "." << std::endl;
                }
                if (elapsed >= millis) {
                    // timeout
                    std::cout << "Timer timeout fired" << std::endl;
                    if (kind == SimpleTimerKind::SINGLE) {
                        // stop timer
                        state = SimpleTimerState::OFF;
                        clear();
                    } else {
                        elapsed = 0U;
                    }
                    squeue.push(msg);  // send timeout msg
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};
