/*
 * thread.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

#include <thool/thread_pool.hpp>
#include <thool/thread.hpp>

namespace thool
{

thread::thread() : thread_(&thread::run, this), stop_flag_(false)
{ };

thread::~thread()
{
   if (thread_.joinable()) thread_.join();
};

/**
 * Method to add new task for the thread to process.
 */
void thread::add_task(const task_ptr & new_task_ptr)
{
   queue_.push(new_task_ptr);
};

/**
 * Method to get a task that waiting to be processed from the thread.
 */
task_ptr thread::get_task()
{
   return queue_.try_pop();
};

/**
 * Method to stop the thread.
 */
void thread::stop()
{
   stop_flag_ = true;
   // Add a dummy task with the highest priority to bring the thread out of the waiting state.
   queue_.push(std::make_shared<task>([]{}, std::numeric_limits<unsigned>::max()));
};

void thread::run()
{
   // Wait for a first task.
   task_ptr active_task_ptr = queue_.wait_and_pop();

   while (!stop_flag_ || active_task_ptr)
   {
      // Trying to steal task from another thread.
      if (!active_task_ptr) active_task_ptr = thread_pool::instance().steal_task();
      // If there are no free tasks, start to waiting for a new task.
      if (!active_task_ptr) active_task_ptr = queue_.wait_and_pop();
      // Execute task.
      active_task_ptr->execute();
      // Trying to get a new task simultaneously from thread's task queue.
      active_task_ptr = queue_.try_pop();
   }
};

} /* namespace thool */
