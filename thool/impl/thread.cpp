/*
 * thread.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

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
bool thread::add_task(const task & tsk, bool wait)
{
   if (wait)
   {
      queue_.wait_and_push(tsk);
      return true;
   }
   return queue_.push(tsk);
};

/**
 * Method to get a task that waiting to be processed from the thread.
 */
bool thread::get_task(task & tsk)
{
   return queue_.try_pop(tsk);
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
      task active_task;
      queue_.wait_and_pop(active_task);
      active_task.function_();
   }
};

} /* namespace thool */
