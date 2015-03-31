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
                           , task_queue_size_(DEFAULT_TASK_QUEUE_SIZE)
{
   change_size(boost::thread::hardware_concurrency());
};

thread_pool::~thread_pool()
{
   forced_stop();
};

/**
 * Get free task from thread pool to process by another thread.
 */
task_ptr thread_pool::steal_task()
{
   task_ptr stealed_task_ptr;

   for (unsigned index = 0; (index < thread_list_.size()) && !stealed_task_ptr; index++)
   {
      stealed_task_ptr = thread_list_[index]->get_task();
   }
   return stealed_task_ptr;
};

/**
 * Add a task with specified priority for execution at the thread pool.
 */
void thread_pool::add_task(const task_ptr & new_task_ptr)
{
   // Go over other threads and look for someone who will take the task,
   // starting with first thread that is marked as available for adding.
   unsigned number = available_thread_number_;

   do
   {
      bool result = thread_list_[number]->add_task(new_task_ptr);

      number = (number == thread_list_.size()) ? 0
                                               : number + 1;
      if (result)
      {
         available_thread_number_ = number;
         return;
      }
   }
   while (available_thread_number_ != number);

   // If all threads are busy, add task to first one with block until
   // a queue of the thread will allow to put new task.
   thread_list_[available_thread_number_]->add_task(new_task_ptr, true);
};

/**
 * Change size of a threads at the thread pool.
 */
bool thread_pool::change_size(unsigned new_size)
{
   unsigned old_size = thread_list_.size();

   if (new_size == 0)
   {
      // Can't set threads count to zero.
      return false;
   }
   else if (new_size > old_size)
   {
      thread_list_.resize(new_size);

      // Add new threads.
      for (unsigned index = old_size; index < new_size; index++)
      {
         thread_list_[index] = boost::make_shared<thread>(task_queue_size_);
      }
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

      task_ptr task_to_move_ptr;
      thread_ptr deletable_thread_ptr;

      // Move tasks from removable threads to other threads, stop
      // an empty thread and remove it from the temporary list.
      while (!threads_to_delete.empty())
      {
         deletable_thread_ptr = threads_to_delete.front();

         while ((task_to_move_ptr = deletable_thread_ptr->get_task()) != 0)
         {
            add_task(task_to_move_ptr);
         }
         deletable_thread_ptr->stop();

         threads_to_delete.erase(threads_to_delete.begin());
      }
   }
   return true;
};

/**
 *  Set maximum amount of tasks per thread.
 */
bool thread_pool::set_max_task_count(unsigned new_count)
{
   if (new_count == 0) return false;

   task_queue_size_ = new_count;

   for (int index = 0; index < thread_list_.size(); index++)
   {
      thread_list_[index]->resize_queue(task_queue_size_);
   }
   return true;
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
