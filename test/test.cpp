/*
 * test.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: simonenkos
 */

#include <memory>
#include <iostream>

#include <thool/thread_pool.hpp>

unsigned long long simple_fibonnachi(unsigned n)
{
   if (n == 0) return 0;
   if (n == 1) return 1;

   return simple_fibonnachi(n - 2) + simple_fibonnachi(n - 1);
};

void calculate_simple_fibonachi()
{
   std::cout << simple_fibonnachi(10) << std::endl;
};

int main(int argc, char ** argv)
{
   auto & thread_pool = thool::thread_pool::instance();
   auto fib_task_ptr = std::make_shared<thool::task>(&calculate_simple_fibonachi, 4);

   thread_pool.add_task(fib_task_ptr);

   while (true)
   {
      if (fib_task_ptr->is_completed())
      {
         break;
      }
   }

   thread_pool.stop();

   return 0;
};
