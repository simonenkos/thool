/*
 * task_queue.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#include <thool/task_queue.hpp>

namespace thool
{

/**
 * Method allows to add tasks to the queue with waiting if there is no free space.
 */
void task_queue::push(const task_ptr & new_task_ptr)
{
   {
      std::lock_guard<std::mutex> lock(mutex_);
      queue_.push(new_task_ptr);
   }
   new_task_.notify_one();
};

/**
 * Method tries to get an element from the queue. It will be blocked when no data is available.
 */
task_ptr task_queue::wait_and_pop()
{
   std::unique_lock<std::mutex> lock(mutex_);
   // If queue is empty, wait for new task.
   new_task_.wait(lock, [this]{ return !queue_.empty(); });
   // Get task from the queue.
   task_ptr top_task_ptr = queue_.top();
   queue_.pop();
   return top_task_ptr;
};

/**
 * Method tries to get an element from the queue without blocking,
 */
task_ptr task_queue::try_pop()
{
   task_ptr top_task_ptr;
   std::lock_guard<std::mutex> lock(mutex_);

   if (!queue_.empty())
   {
      top_task_ptr = queue_.top();
      queue_.pop();
   }
   return top_task_ptr;
};

} /* namespace thool */
