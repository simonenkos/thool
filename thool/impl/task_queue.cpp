/*
 * task_queue.cpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#include <thool/impl/task_queue.hpp>

namespace thool
{

task_queue::task_queue(unsigned size) : size_(size)
{ };

task_queue::task_queue(const task_queue & other)
{
   queue_ = other.queue_;
   size_  = other.size_;
};

task_queue & task_queue::operator=(const task_queue & other)
{
   queue_ = other.queue_;
   size_  = other.size_;
   return * this;
};

bool task_queue::try_push()
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   // ToDo
   return false;
};

bool task_queue::try_pop()
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   // ToDo
   return false;
};

} /* namespace thool */
