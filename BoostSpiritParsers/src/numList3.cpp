/*
 * numList3.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: root
 */

#include "numList3.h"

namespace client
{
  bool test_numList3()
  {
	  std::cout << "/////////////////////////////////////////////////////////\n\n";
	  std::cout << "\t\tA comma separated list parser for Spirit...\n\n";
	  std::cout << "/////////////////////////////////////////////////////////\n\n";

	  std::cout << "Give me a comma separated list of numbers.\n";
	  std::cout << "The numbers will be inserted in a vector of numbers\n";
	  std::cout << "Type [q or Q] to quit\n\n";

	  std::string str;
	  while (getline(std::cin, str))
	  {
	      if (str.empty() || str[0] == 'q' || str[0] == 'Q')
	         break;

	      std::vector<double> v;
	      if (client::parse_numbers3(str.begin(), str.end(), v))
	      {
	         std::cout << "-------------------------\n";
	         std::cout << "Parsing succeeded\n";
	         std::cout << str << " Parses OK: " << std::endl;

	         for (std::vector<double>::size_type i = 0; i < v.size(); ++i)
	            std::cout << i << ": " << v[i] << std::endl;

	         std::cout << "\n-------------------------\n";
	      }
	      else
	      {
	         std::cout << "-------------------------\n";
	         std::cout << "Parsing failed\n";
	         std::cout << "-------------------------\n";
	      }
	  }

	  return true;
  }

}


