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

thread_pool::thread_pool()
{
//   unsigned default_thread_count = boost::thread::hardware_concurrency();
//
//   task_queue_list_.resize(default_thread_count, task_queue(task_queue_size_));
//
//   for (unsigned counter = 0; counter < default_thread_count; counter++)
//   {
//      thread_list_.push_back(boost::thread(&thread_pool::process_tasks, this, counter));
//   }

   // ToDo think about 'thread' class that holds thread and queue.
};

thread_pool::~thread_pool()
{
   // ToDo
};

/**
 * Add a task with specified priority for execution at the thread pool.
 * Return 'true' if task was successfully added to thread's task queue,
 * otherwise 'false'.
 */
bool thread_pool::add_task(const task & task)
{
   // ToDo
   return false;
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
void thread_pool::set_max_task_count()
{
   // ToDo
};

/**
 *  Interrupt work of the thread pool with waiting for all tasks are finished.
 */
void thread_pool::stop()
{
   // ToDo
};

/**
 * Interrupt work of the thread pool without waiting for all tasks are finished.
 */
void thread_pool::forced_stop()
{
   // ToDo
};

} /* namespace thool */
