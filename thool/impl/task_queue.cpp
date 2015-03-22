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

/**
 * Method allows to add tasks to the queue with waiting if there is no free space.
 */
void task_queue::wait_and_push(const task & tsk)
{
   boost::unique_lock<boost::mutex> lock(mutex_);
   not_full_.wait(lock, boost::bind(&task_queue::is_not_full, this, _1));
   queue_.push(tsk);
   not_empty_.notify_all();
};

/**
 * Method allows to add tasks to the queue.
 */
bool task_queue::push(const task & tsk)
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   if (queue_.size() == size_)
      return false;
   queue_.push(tsk);
   not_empty_.notify_all();
   return true;
};

/**
 * Method tries to get an element from the queue. It will be blocked when no data is available.
 */
void task_queue::wait_and_pop(task & tsk)
{
   boost::unique_lock<boost::mutex> lock(mutex_);
   not_empty_.wait(lock, boost::bind(&task_queue::is_not_empty, this, _1));
   tsk = queue_.top();
   queue_.pop();
   not_full_.notify_all();
};

/**
 * Method tries to get an element from the queue without blocking,
 */
bool task_queue::try_pop(task & tsk)
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   if (queue_.empty())
      return false;
   tsk = queue_.top;
   queue_.pop();
   not_full_.notify_all();
   return true;
};

/**
 * Method checks is the queue empty or not.
 */
bool task_queue::is_empty() const
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   return queue_.empty();
};

/**
 * Method allows to change size of the queue.
 */
void task_queue::resize(unsigned new_size)
{
   boost::lock_guard<boost::mutex> lock(mutex_);
   size_ = new_size;
};

bool task_queue::is_not_empty() const
{
   return !queue_.empty();
};

bool task_queue::is_not_full() const
{
   return queue_.size() < size_;
};

} /* namespace thool */
