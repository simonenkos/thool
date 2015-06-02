/*
 * thread_pool.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: simonenkos
 */

#include <thool/thread_pool.hpp>

namespace thool
{

thread_pool::thread_pool() : available_thread_number_(0)
{
   change_size(std::thread::hardware_concurrency());
};

thread_pool::~thread_pool()
{ };

/**
 * Get free task from some thread to process by another thread.
 */
task_ptr thread_pool::steal_task()
{
   task_ptr stealed_task_ptr;

   std::lock_guard<std::mutex> lock(list_mutex_);

   for (unsigned index = 0; (index < thread_list_.size()) && !stealed_task_ptr; index++)
   {
      stealed_task_ptr = thread_list_[index]->get_task();
   }
   return stealed_task_ptr;
};

/**
 * Add a task with specified priority for execution at the thread pool.
 */
void thread_pool::add_task(task_ptr new_task_ptr)
{
   thread_list_[available_thread_number_]->add_task(new_task_ptr);

   available_thread_number_ = (available_thread_number_ == (thread_list_.size() - 1))
                            ? 0
                            : available_thread_number_ + 1;
};

/**
 * Change size of a threads at the thread pool.
 */
bool thread_pool::change_size(unsigned new_size)
{
   std::lock_guard<std::mutex> lock(list_mutex_);
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
         thread_list_[index] = std::make_shared<thread>();
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
 *  Interrupt work of the thread pool with waiting for all tasks are finished.
 */
void thread_pool::stop()
{
   for (auto & thread : thread_list_) thread->stop();
};

/**
 * Method allows to get an instance of the thread pool in a thread-safe manner.
 */
thread_pool & thread_pool::instance()
{
   static thread_pool pool;
   return pool;
};

} /* namespace thool */
