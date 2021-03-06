/*
 * thread.hpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_THREAD_HPP_
#define THOOL_THREAD_HPP_

#include <thool/task_queue.hpp>
#include <thread>


namespace thool
{

/**
 * Class describes a thread into the thread pool, that holds a queue for a tasks
 * witch should be processed and execute them at a separate thread.
 */
class thread
{
   task_queue  queue_;    // Queue to store tasks for the thread according to their priority.
   std::thread thread_;   // Thread to process tasks at a separate thread.

   std::atomic<bool> stop_flag_; // Flag to notify thread about stop request.

public:
   thread();

   thread(const thread &  other) = delete;
   thread(const thread && other) = delete;

   thread & operator=(const thread &  other) = delete;
   thread & operator=(const thread && other) = delete;

   ~thread();

   /** Method to add new task for the thread to process. */
   void add_task(const task_ptr & new_task_ptr);

   /** Method to get a task that waiting to be processed from the thread. */
   task_ptr get_task();

   /** Method to stop the thread. */
   void stop();

private:
   void run();
};

typedef std::shared_ptr<thread> thread_ptr;

} /* namespace thool */

#endif /* THOOL_THREAD_HPP_ */
