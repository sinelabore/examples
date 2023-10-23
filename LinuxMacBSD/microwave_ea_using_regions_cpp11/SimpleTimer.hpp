/**
 * @file SimpleTimer.hpp
 * @author pmueller
 * @brief Reusable timer service
 * @version 0.1
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include "SafeQueue.hpp"

enum class SimpleTimerState : uint8_t {
    OFF,
    ON,
    PAUSE
};

enum class SimpleTimerKind : uint8_t {
    SINGLE,
    CYCLIC,
};

template <class T>  class SimpleTimer {
 public:
    explicit SimpleTimer(SafeQueue<T>& q) : squeue(q) {
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
