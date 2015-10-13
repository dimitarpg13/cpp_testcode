//============================================================================
// Name        : BoostSpiritExamples.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>

//[tutorial_numlist1
namespace client
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   template<typename Iterator>
   bool parse_numbers(Iterator first, Iterator last)
   {
	   using qi::double_;
	   using qi::phrase_parse;
	   using ascii::space;

	   bool r = phrase_parse(
			      first,  						/* <start iterator>*/
			      last,							/* <end iterator> */
			      double_ >> *(',' >> double_), /* <the parser> */
			      space							/* <the skip-parser> */
	   	   	   );
	   if (first != last)  // fail if we did not get full match
		   return false;
	   return r;
   }
   //]
}

void spirit_comma_separated_parser1()
{
	std::cout << "spirit_comma_separated_parser1" << std::endl;
	std::cout << "enter a comma-separated list of numbers" << std::endl;
	std::cout << "[type q or Q to quit]" << std::endl;

	std::string str;
	while (getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		if (client::parse_numbers(str.begin(), str.end()))
			std::cout << "Parsing succeeded!" << std::endl;
		else
			std::cout << "Parsing failed!" << std::endl;
	}

	std::cout << "bye!" << std::endl;
}

int main() {
	std::cout << "Boost Spirit library examples" << std::endl; // prints Boost Spirit library examples

	spirit_comma_separated_parser1();

	return 0;
}
