/*
 * thread_pool.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: simonenkos
 */

#include <thool/impl/thread_pool.hpp>

namespace thool
{

namespace
{
   const unsigned DEFAULT_TASK_QUEUE_SIZE = 10;
}

thread_pool::thread_pool() : available_thread_number_(0)
{
   thread_list_.resize(boost::thread::hardware_concurrency());

   for (int index = 0; index < thread_list_.size(); index++)
   {
      thread_list_[index] = std::make_shared<thread>(DEFAULT_TASK_QUEUE_SIZE);
   }
};

thread_pool::~thread_pool()
{ };

/**
 * Add a task with specified priority for execution at the thread pool.
 * Return 'true' if task was successfully added to thread's task queue,
 * otherwise 'false'.
 */
bool thread_pool::add_task(const task & tsk)
{
   if (thread_list_.empty())
      return false;

   // Tying to add task to first thread that available for adding.
   if (thread_list_[available_thread_number_]->add_task(tsk))
      return true;

   // If it is not possible, go over other threads and look for someone
   // who will take the task.
   unsigned number = available_thread_number_;

   do
   {
      number = (number == thread_list_.size()) ? 0
                                                 : number + 1;
      if (thread_list_[number]->add_task(tsk))
      {
         available_thread_number_ = number;
         return true;
      }
   }
   while (available_thread_number_ != number);

   // If all threads are busy, add task to first one with block until
   // a queue of the thread will allow to put new task.
   return thread_list_[available_thread_number_]->add_task(tsk, true);
};

/**
 * Change size of a threads at the thread pool.
 */
void thread_pool::change_size(unsigned new_size)
{
   // ToDo
};

/**
 *  Set maximum amount of tasks per thread.
 */
void thread_pool::set_max_task_count(unsigned new_count)
{
   for (int index = 0; index < thread_list_.size(); index++)
   {
      thread_list_[index]->resize_queue(new_count);
   }
};

/**
 *  Interrupt work of the thread pool with waiting for all tasks are finished.
 */
void thread_pool::stop()
{
   for (int index = 0; index < thread_list_.size(); index++)
   {
      thread_list_[index]->stop(false);
   }
};

/**
 * Interrupt work of the thread pool without waiting for all tasks are finished.
 */
void thread_pool::forced_stop()
{
   for (int index = 0; index < thread_list_.size(); index++)
   {
      thread_list_[index]->stop(true);
   }
};

} /* namespace thool */
