/*
 * roman.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: root
 */



#include <iostream>
#include "roman.h"

namespace client
{

  bool test_roman()
  {

    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tRoman Numerals Parser\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Type a Roman Numeral ...or [q or Q] to quit\n\n";

    typedef std::string::const_iterator iterator_type;
    typedef client::roman<iterator_type> roman;

    roman roman_parser; // Our grammar
    std::string str;

    unsigned result;
    while (std::getline(std::cin, str))
    {
       if (str.empty() || str[0] == 'q' || str[0] == 'Q')
         break;

       std::string::const_iterator iter = str.begin();
       std::string::const_iterator end = str.end();

       bool r = parse(iter, end, roman_parser, result);
       if (r && iter == end)
       {
          std::cout << "-------------------------\n";
          std::cout << "Parsing succeeded\n";
          std::cout << "result = " << result << std::endl;
          std::cout << "-------------------------\n";
       }
       else
       {
          std::string rest(iter, end);
          std::cout << "-------------------------\n";
          std::cout << "Parsing failed\n";
          std::cout << "stopped at: \": " << rest << "\"\n";
          std::cout << "-------------------------\n";
       }
       //]
     }

     std::cout << "Bye... :-) \n\n";
     return true;
  }
}
