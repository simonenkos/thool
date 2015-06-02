/*
 * task_queue.hpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_TASK_QUEUE_HPP_
#define THOOL_TASK_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <condition_variable>

#include <thool/task.hpp>

namespace thool
{

/**
 * This class describes a queue of tasks that are intended to be processed
 * by some specific thread that is binded to the queue. This class provide
 * synchronization mechanisms to work safely with the queue from different
 * threads.
 */
class task_queue
{
   typedef std::priority_queue<task_ptr> internal_queue;

   std::mutex mutex_;                 // Mutex to synchronize access to the queue.
   std::condition_variable new_task_; // Condition variable to wait for a new task at the queue.
   internal_queue queue_;             // Use STL's priority queue as a internal container.

public:
   task_queue() { };

   task_queue(const task_queue &  other) = delete;
   task_queue(const task_queue && other) = delete;

   task_queue & operator=(const task_queue &  other) = delete;
   task_queue & operator=(const task_queue && other) = delete;

   ~task_queue() { };

public:
   /** Method allows to add tasks to the queue without waiting. */
   void push(const task_ptr & new_task_ptr);

   /** Method tries to get an element from the queue. It will be blocked when no data is available. */
   task_ptr wait_and_pop();

   /** Method tries to get an element from the queue without blocking, */
   task_ptr try_pop();
};

} /* namespace thool */

#endif /* THOOL_TASK_QUEUE_HPP_ */
