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
 * a client's requirements.
 */
struct task
{
   // Use boost's function wrapper for a void function without parameters
   // as a function for the task.
   typedef boost::function<void()> function;

   function function_;
   unsigned priority_;

   task(const function & func, unsigned priority) : function_(func)
                                                  , priority_(priority)
   { }

   bool operator<(const task & other) const
   {
      return priority_ < other.priority_;
   }
};

} /* namespace thool */

#endif /* THOOL_IMPL_TASK_HPP_ */
