/*
 * thread_pool.hpp
 *
 *  Created on: Mar 14, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_IMPL_THREAD_POOL_HPP_
#define THOOL_IMPL_THREAD_POOL_HPP_

#include <boost/thread.hpp>

#include <thool/impl/thread.hpp>
#include <thool/impl/task_queue.hpp>

namespace thool
{

/**
 * Thread pool organizes control of a group of threads, that process bunch of tasks.
 * This class is not thread-safe, so thread pool does not provide any synchronization.
 */
class thread_pool
{
   friend thread_pool * instance();
   friend class thread;

   typedef std::vector<thread_ptr> thread_list;

   thread_list thread_list_;
   unsigned available_thread_number_;
   unsigned task_queue_size_;

   thread_pool();
   thread_pool(const thread_pool & pool);
   thread_pool & operator=(const thread_pool & pool);

   ~thread_pool();

   /** Get free task from thread pool to process by another thread. */
   task_ptr steal_task();

public:

   /** Add a task with specified priority for execution at the thread pool. */
   void add_task(const task_ptr & new_task_ptr);

   /** Change size of a threads at the thread pool. */
   bool change_size(unsigned new_size);

   /** Set maximum amount of tasks per thread. */
   bool set_max_task_count(unsigned new_count);

   /** Interrupt work of the thread pool with waiting for all tasks are finished. */
   void stop();

   /** Interrupt work of the thread pool without waiting for all tasks are finished. */
   void forced_stop();
};

} /* namespace thool */

#endif /* THOOL_IMPL_THREAD_POOL_HPP_ */
