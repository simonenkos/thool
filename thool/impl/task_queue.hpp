/*
 * task_queue.hpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_IMPL_TASK_QUEUE_HPP_
#define THOOL_IMPL_TASK_QUEUE_HPP_

#include <boost/heap/priority_queue.hpp>
#include <boost/thread.hpp>

#include <thool/impl/task.hpp>

namespace thool
{

class task_queue
{
   typedef boost::heap::priority_queue<task> internal_queue;

   boost::mutex mutex_;
   internal_queue queue_;
   unsigned size_;

public:
   task_queue(unsigned size);
   task_queue(const task_queue & other);
   task_queue & operator=(const task_queue & other);

public:
   bool try_push();
   bool try_pop();
};

} /* namespace thool */

#endif /* THOOL_IMPL_TASK_QUEUE_HPP_ */
