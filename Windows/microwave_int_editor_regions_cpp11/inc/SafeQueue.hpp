/**
 * @file SafeQueue.hpp
 * @author pmueller
 * @brief Mutex protected queue
 * @version 0.2
 * @date 2024-02-22
 *
 * @copyright Copyright (c) 2024
 */

#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

/**
 * @brief A thread-safe queue implementation.
 *
 * The SafeQueue class provides a thread-safe queue implementation, allowing
 * multiple threads to push and pop elements concurrently without causing data
 * races or undefined behavior. It internally uses mutexes and condition
 * variables to synchronize access to the underlying std::queue.
 *
 * Usage:
 *
 * 1. Create an instance of SafeQueue for the desired element type.
 * 2. Use the push() method to add elements to the queue safely from any thread.
 * 3. Use the waitAndPop() method to remove elements from the queue safely,
 *    waiting if the queue is empty until an element becomes available.
 *
 * @tparam T The type of elements stored in the queue.
 */
template <class T>
class SafeQueue
{
private:
  std::queue<T> queue;
  mutable std::mutex guard;
  std::condition_variable signal;

public:
  /**
   * @brief Pushes an element onto the queue.
   *
   * This method adds a single element to the end of the queue safely from
   * multiple threads. It locks the guard mutex to ensure exclusive access
   * to the queue while pushing the element and then notifies waiting threads.
   *
   * @param _data The element to be added to the queue.
   */
  void push(T const &_data)
  {
    {
      // Creates a lock_guard object named lock that locks the mutex
      // guard upon construction and automatically unlocks it when
      // lock goes out of scope (i.e., at the end of the block).
      std::lock_guard<std::mutex> lock(guard);
      queue.push(_data);
    }
    signal.notify_one();
  }

  /**
   * @brief Waits for and pops an element from the queue.
   *
   * This method removes and returns a single element from the front of
   * the queue safely. If the queue is empty, it waits until an element
   * becomes available, blocking the calling thread.
   *
   * @param _value A reference where the popped element will be stored.
   */
  void waitAndPop(T &_value)
  {
    // Creates a lock_guard object named lock that locks the mutex
    // guard upon construction and automatically unlocks it when
    // lock goes out of scope (i.e., at the end of the block).
    std::unique_lock<std::mutex> lock(guard);
    while (queue.empty())
    {
      signal.wait(lock);
    }
    _value = queue.front();
    queue.pop();
  }
};
