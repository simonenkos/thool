/*
 * task.hpp
 *
 *  Created on: Mar 15, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_IMPL_TASK_HPP_
#define THOOL_IMPL_TASK_HPP_

#include <boost/function.hpp>

namespace thool
{

/**
 * Simple structure that presents a task for the thread pool. It takes a function for
 * execution and it's priority value, which allows to process the task according to
 * a client's requirements. Higher priority value means higher priority for a task.
 */
class task
{
   // Use boost's function wrapper for a void function without parameters
   // as a function for the task.
   typedef boost::function<void()> function;
   typedef boost::atomic<bool> flag;

   function function_;
   unsigned priority_;
   flag is_completed_;

public:
   task(const function & func, unsigned priority) : function_(func)
                                                  , priority_(priority)
                                                  , is_completed_(false)
   { }

   void execute()
   {
      function_();
      is_completed_ = true;
   };

   bool is_completed() const
   {
      return is_completed_;
   }

   // Compare method for priority queue.
   bool operator<(const task & other) const
   {
      return priority_ < other.priority_;
   }
};

typedef boost::shared_ptr<task> task_ptr;

} /* namespace thool */

#endif /* THOOL_IMPL_TASK_HPP_ */
