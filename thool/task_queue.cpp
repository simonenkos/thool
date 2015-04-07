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
void task_queue::push(const task_ptr & new_task_ptr) //wait_and_
{
   {
      std::lock_guard<std::mutex> lock(mutex_);
      queue_.push(new_task_ptr);
   }
   new_task_.notify_one();
};

///**
// * Method allows to add tasks to the queue without waiting.
// */
//bool task_queue::try_push(const task_ptr & new_task_ptr)
//{
//   {
//      std::lock_guard<std::mutex> lock(mutex_);
//      if (!lock) return false;
//      queue_.push(new_task_ptr);
//   }
//   new_task_.notify_one();
//   return true;
//};

/**
 * Method tries to get an element from the queue. It will be blocked when no data is available.
 */
task_ptr task_queue::wait_and_pop()
{
   std::unique_lock<std::mutex> lock(mutex_);
   // If queue is empty, wait for new task.
   while (queue_.empty()) new_task_.wait(lock);
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

   std::lock_guard<std::mutex> lock(mutex_); // , std::try_to_lock

   if (!queue_.empty()) // lock &&
   {
      top_task_ptr = queue_.top();
      queue_.pop();
   }
   return top_task_ptr;
};

/**
 * Method checks is the queue empty or not.
 */
bool task_queue::is_empty() const
{
   std::lock_guard<std::mutex> lock(mutex_);
   return queue_.empty();
};

} /* namespace thool */
