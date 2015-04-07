/*
 * thread.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

#include <iostream>

#include <thool/thread_pool.hpp>
#include <thool/thread.hpp>

namespace thool
{

thread::thread() : thread_(&thread::run, this)
{ };

thread::~thread()
{
   stop();
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
   if (thread_.joinable()) thread_.join();
};

void thread::run()
{
   while (thread_.joinable())
   {
      // Trying to steal task from another threads.
      auto active_task_ptr = thread_pool::instance().steal_task();
      // If there are no free tasks, trying to get task from thread's task queue.
      if (!active_task_ptr) active_task_ptr = queue_.wait_and_pop();
      // Execute task.
      active_task_ptr->execute();
   }
};

} /* namespace thool */
