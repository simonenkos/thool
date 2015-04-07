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

void calculate_simple_fibonachi(unsigned number)
{
   std::cout << number << " fibonacci number is: " << simple_fibonnachi(number) << std::endl;
};

int main(int argc, char ** argv)
{
   auto & thread_pool = thool::thread_pool::instance();

   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 0),  0));
   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 1),  0));
   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 2),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 3),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 4),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 5),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 6),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 7),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 8),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 9),  0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 10), 0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 11), 0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 12), 0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 13), 0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 14), 0));
//   thread_pool.add_task(std::make_shared<thool::task>(std::bind(calculate_simple_fibonachi, 15), 0));

   thread_pool.stop();

   return 0;
};
