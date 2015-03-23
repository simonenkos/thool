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

   // Create threads with default parameters.
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

   // Go over other threads and look for someone who will take the task,
   // starting with first thread that is marked as available for adding.
   unsigned number = available_thread_number_;

   do
   {
      bool result = thread_list_[number]->add_task(tsk);

      number = (number == thread_list_.size()) ? 0
                                               : number + 1;
      if (result)
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
   unsigned old_size = thread_list_.size();

   if (new_size > old_size)
   {
      thread_list_.resize(new_size);
   }
   else if (new_size < old_size)
   {
      // Create a temporary list of threads that should be deleted.
      thread_list threads_to_delete(old_size - new_size);

      // Save removable threads for later processing into the
      // temporary list.
      while (new_size != old_size)
      {
         threads_to_delete.push_back
         (
               thread_list_[old_size - 1]
         );
         old_size--;
      }

      // Resize thread list to new size.
      thread_list_.resize(new_size);

      task task_to_move;
      thread_ptr ptr;

      // Move tasks from removable threads to other threads, stop
      // an empty thread and remove it from the temporary list.
      while (!threads_to_delete.empty())
      {
         ptr = threads_to_delete.front();

         while (ptr->get_task(task_to_move))
         {
            add_task(task_to_move);
         }
         ptr->stop();

         threads_to_delete.erase(threads_to_delete.begin());
      }
   }
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
