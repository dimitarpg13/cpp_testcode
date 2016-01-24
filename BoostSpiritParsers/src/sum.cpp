/*
 * sum.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: root
 */
#include "sum.h"

namespace client
{
   bool test_sum()
   {
	   std::cout << "//////////////////////////////\n\n";
	   std::cout << "\t\tA parser for summing a list of numbers...\n\n";
	   std::cout << "//////////////////////////////\n\n";

	   std::cout << "Give me a comma separated list of numbers: \n";
	   std::cout << "The numbers are added using Phoenix.\n";
	   std::cout << "Type [q or Q] to quit\n\n";

	   std::string str;
	   while (getline(std::cin, str))
	   {
		  if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			  break;

		  double n;
		  if (client::adder(str.begin(), str.end(), n))
		  {
			  std::cout << "---------------------\n";
			  std::cout << "Parsing succeeded\n";
			  std::cout << str << " Parses OK: " << std::endl;

			  std::cout << "sum = " << n;
			  std::cout << "\n----------------------\n";
		  }
		  else
		  {
			  std::cout << "---------------------\n";
			  std::cout << "Parsing Failed\n";
			  std::cout << "---------------------\n";

		  }
	   }

	   return true;
   }
}



