/*
 * task_queue.hpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_IMPL_TASK_QUEUE_HPP_
#define THOOL_IMPL_TASK_QUEUE_HPP_

#include <queue>

#include <boost/thread.hpp>

#include <thool/impl/task.hpp>

namespace thool
{

/**
 * This class describes a queue of tasks that are intended to be processed
 * by some specific thread that is binded to the queue.
 */
class task_queue
{
   typedef std::priority_queue<task> internal_queue;

   mutable boost::mutex mutex_;         // Mutex to synchronize access to the queue.
   boost::condition_variable new_data_; // Condition variable to wait for a new data at the queue.
   internal_queue queue_;               // Use STL's priority queue as a internal container.
   unsigned size_;                      // Maximum size of the queue.

public:
   task_queue(unsigned size);

private:
   task_queue(const task_queue & other);
   task_queue & operator=(const task_queue & other);

public:
   /** Method allows to add tasks to the queue. */
   bool push(const task & tsk);
   /** Method tries to get an element from the queue. It will be blocked when no data is available. */
   void wait_and_pop(task & tsk);
   /** Method tries to get an element from the queue without blocking, */
   bool try_pop(task & tsk);
   /** Method checks is the queue empty or not. */
   bool is_empty() const;

private:
   bool is_not_empty() const;
};

} /* namespace thool */

#endif /* THOOL_IMPL_TASK_QUEUE_HPP_ */
