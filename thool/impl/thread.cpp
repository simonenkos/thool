/*
 * thread.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

#include <thool/thool.hpp>
#include <thool/impl/thread.hpp>

namespace thool
{

thread::thread(unsigned task_queue_size) : queue_(task_queue_size)
                                         , thread_(&thread::run, this)
{ };

thread::~thread()
{
   if (thread_.joinable()) thread_.join();
};

/**
 * Method to add new task for the thread to process.
 */
bool thread::add_task(const task_ptr & new_task_ptr, bool wait)
{
   if (wait)
   {
      queue_.wait_and_push(new_task_ptr);
      return true;
   }
   return queue_.try_push(new_task_ptr);
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
void thread::stop(bool forced)
{
   if (forced)
   {
      thread_.interrupt();
   }
   else if (thread_.joinable())
   {
      thread_.join();
   }
};

/**
 * Method to resize thread's task queue.
 */
void thread::resize_queue(unsigned new_size)
{
   queue_.resize(new_size);
};

void thread::run()
{
   while (thread_.joinable())
   {
      // Trying to steal task from another threads.
      task_ptr active_task_ptr = thool::instance().steal_task();
      // If there are no free tasks, trying to get task from thread's task queue.
      if (!active_task_ptr) active_task_ptr = queue_.wait_and_pop();
      // Execute task.
      active_task_ptr->execute();
   }
};

} /* namespace thool */
