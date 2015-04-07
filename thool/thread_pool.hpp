/*
 * thread_pool.hpp
 *
 *  Created on: Mar 14, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_THREAD_POOL_HPP_
#define THOOL_THREAD_POOL_HPP_

#include <thool/task_queue.hpp>
#include <thool/thread.hpp>

namespace thool
{

/**
 * Thread pool organizes control of a group of threads, that process bunch of tasks.
 * This class is not thread-safe, so thread pool does not provide any synchronization.
 */
class thread_pool
{
   friend class thread;

   typedef std::vector<thread_ptr> thread_list;

   thread_list thread_list_;          // List of threads at the pool.
   unsigned available_thread_number_; // Number of selected thread for available to task adding.
   std::mutex list_mutex_;            // Mutex to protect a task stealing mechanism from changes of a size of the thread list.

   thread_pool();

   thread_pool(const thread_pool &  pool) = delete;
   thread_pool(const thread_pool && pool) = delete;

   thread_pool & operator=(const thread_pool &  pool) = delete;
   thread_pool & operator=(const thread_pool && pool) = delete;

   ~thread_pool();

   /** Get free task from some thread to process by another thread. */
   task_ptr steal_task();

public:

   /** Add a task with specified priority for execution at the thread pool. */
   void add_task(task_ptr new_task_ptr);

   /** Change size of a threads at the thread pool. */
   bool change_size(unsigned new_size);

   /** Interrupt a work of the thread pool with waiting for all tasks to be finished. */
   void stop();

   /** Method allows to get an instance of the thread pool. */
   static thread_pool & instance();
};

} /* namespace thool */

#endif /* THOOL_THREAD_POOL_HPP_ */
