/**
 * @file SafeQueue.hpp
 * @author pmueller
 * @brief Mutex protected queue
 * @version 0.1
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <class T> class SafeQueue {
 private:
  std::queue<T> queue;
  std::mutex mtx;
  mutable std::mutex guard;
  std::condition_variable signal;

 public:
  // push a single element to the queue
  void push(T const& _data) {
    {
      std::lock_guard<std::mutex> lock(guard);
      queue.push(_data);
    }
    signal.notify_one();
  }

  // pop a single element from the queue
  void waitAndPop(T& _value) {
    std::unique_lock<std::mutex> lock(guard);
    while (queue.empty()) {
        signal.wait(lock);
    }
    _value = queue.front();
    queue.pop();
  }
};
