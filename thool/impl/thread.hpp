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
 *
 */
class thread
{
   task_queue queue_;     // Queue to store tasks for the thread according to their priority.
   boost::thread thread_; // Boos't thread ... ToDo

public:
   thread(unsigned task_queue_size);
   ~thread();

   /** */
   bool add_task(const task & tsk);
   /** */
   bool steal_task(task & tsk);
   /** */
   void stop(bool forced = false);

private:
   thread(const thread & other);
   thread & operator=(const thread & other);

   void run();
};

typedef boost::shared_ptr<thread> thread_ptr;

} /* namespace thool */

#endif /* THOOL_IMPL_THREAD_HPP_ */
