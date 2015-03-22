/*
 * thread.hpp
 *
 *  Created on: Mar 17, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_IMPL_THREAD_HPP_
#define THOOL_IMPL_THREAD_HPP_

#include <boost/thread.hpp>

#include <thool/impl/task_queue.hpp>

namespace thool
{

/**
 * Class describes a thread into thread pool, that holds a queue for a tasks
 * witch should be processed. To run in a separate thread boost's thread is
 * used.
 */
class thread
{
   task_queue queue_;     // Queue to store tasks for the thread according to their priority.
   boost::thread thread_; // Boost's thread to process tasks at a separate thread.

public:
   thread(unsigned task_queue_size);
   ~thread();

   /** Method to add new task for the thread to process. */
   bool add_task(const task & tsk, bool wait = false);

   /** Method to get a task that waiting to be processed from the thread. */
   bool get_task(task & tsk);

   /** Method to stop the thread. */
   void stop(bool forced = false);

   /** Method to resize thread's task queue. */
   void resize_queue(unsigned new_size);

private:
   thread(const thread & other);
   thread & operator=(const thread & other);

   void run();
};

typedef boost::shared_ptr<thread> thread_ptr;

} /* namespace thool */

#endif /* THOOL_IMPL_THREAD_HPP_ */
