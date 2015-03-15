/*
 * thool.hpp
 *
 *  Created on: Mar 12, 2015
 *      Author: simonenkos
 */

#ifndef THOOL_THOOL_HPP_
#define THOOL_THOOL_HPP_

#include <thool/impl/thread_pool.hpp>

namespace thool
{

/**
 * To get access to a thread pool it is necessary to call function 'instance' to
 * get an instance of the thread pool. Note this function is not thread safe, so
 * it should be called from a single thread at a first time.
 */
thread_pool & instance()
{
   static thread_pool instance;
   return instance;
};

} /* namespace thool */

#endif /* THOOL_THOOL_HPP_ */
