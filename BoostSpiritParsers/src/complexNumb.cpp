/*
 * complexNumb.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: root
 */
#include "complexNumb.h"
namespace client
{
    bool test_complexnumb()
    {
      std::cout << "//////////////////////////////////" << std::endl << std::endl;
      std::cout << "\t\tA complex number micro parser for Spirit" << std::endl << std::endl;
      std::cout << "//////////////////////////////////" << std::endl << std::endl;

      std::cout << "Give me a complex number of the form r or (r) or (r,i)" << std::endl;
      std::cout << "Type [q or Q] to quit" << std::endl << std::endl;

      std::string str;
      while (getline(std::cin, str))
      {
    	  if (str.empty() || str[0] == 'q' || str[0] == 'Q')
    		  break;

    	  std::complex<double> c;
    	  if (client::parse_complex(str.begin(), str.end(), c))
    	  {
    		  std::cout << "----------------------" << std::endl;
    		  std::cout << "Parsing succeeded" << std::endl;
    		  std::cout << "got: " << c << std::endl << std::endl;
    		  std::cout << "----------------------" << std::endl;
    	  }
    	  else
    	  {
    		  std::cout << "----------------------" << std::endl;
    		  std::cout << "Parsing failed" << std::endl;
    		  std::cout << "----------------------" << std::endl;
    	  }
      }

	  return true;
    }
}

