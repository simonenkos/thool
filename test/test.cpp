/*
 * test.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: simonenkos
 */

#include <boost/bind.hpp>

#include <thool/thool.hpp>

unsigned simple_fibonnachi(unsigned n)
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
   thool::instance().add_task(thool::task(&calculate_simple_fibonachi, 0));

   return 0;
};
