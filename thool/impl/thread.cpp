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

bool thread::add_task(const task & tsk)
{
   return queue_.push(tsk);
};

bool thread::steal_task(task & tsk)
{
   return queue_.try_pop(tsk);
};

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
}

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
