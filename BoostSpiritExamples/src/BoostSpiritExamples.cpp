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
#include <complex>

#include <boost/spirit/include/qi.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/phoenix_core.hpp> // for the complex number parser
#include <boost/spirit/include/phoenix_operator.hpp> // for the complex number parser


namespace client
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   //[tutorial_numlist1
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

   //[tutorial_semantic_actions_functions
   // a plain function
   void print(int const & i)
   {
	  std::cout << i << std::endl;
   }

   // a member function
   struct writer
   {
	  void print(int const & i) const
	  {
		  std::cout << i << std::endl;
	  }
   };

   struct print_action
   {
	   void operator()(int const & i, qi::unused_type, qi::unused_type) const
	   {
		   std::cout << i << std::endl;
	   }
   };
   //]

   //[tutorial_complex_parser
   template<typename Iterator>
   bool parse_complex(Iterator first, Iterator last, std::complex<double>& c)
   {
	   using boost::spirit::qi::double_;
	   using boost::spirit::qi::_1;
	   using boost::spirit::qi::phrase_parse;
	   using boost::spirit::ascii::space;
	   using boost::phoenix::ref;

       double rN = 0.0;
       double iN = 0.0;
       bool r = phrase_parse(first, last,
    	// begin grammar
    	(
    	  '(' >> double_[ref(rN) = _1]
    	      >> -(',' >> double_[ref(iN) = _1]) >> ')'
    	      | double_[ref(rN) = _1]
    	),
        // end grammar
       space);

       if (!r || first != last) // fail if we did not get a full match
    	   return false;
       c = std::complex<double>(rN,iN);
       return r;
   }
   //]

   //[tutorial_adder
   template<typename Iterator>
   bool adder(Iterator first, Iterator last, double& n)
   {

	   namespace qi = boost::spirit::qi;
	   namespace ascii = boost::spirit::ascii;
	   namespace phoenix = boost::phoenix;

	   using qi::double_;
	   using qi::_1;
	   using ascii::space;
	   using phoenix::ref;

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

void spirit_semantic_action_functions()
{
	using boost::spirit::qi::int_;
	using boost::spirit::qi::parse;
	using client::print;
	using client::writer;
	using client::print_action;

	{ // example using plain action
		char const *first = "{42}", *last = first + std::strlen(first);
		//[tutorial_attach_actions1
		parse(first, last, '{' >> int_[&print] >> '}');
	} //]


	{ // example using simple function object
		char const *first = "{43}", *last = first + std::strlen(first);
		//[tutorial_attach_actions2
		parse(first, last, '{' >> int_[print_action()] >> '}');
		//]
	}

	{ // example using boost.bind with a plain function
		char const *first = "{44}", *last = first + std::strlen(first);
		//[tutorial_attach_actions3
		parse(first, last, '{' >> int_[boost::bind(&print, _1)] >> '}');
		//]
	}

	{ // example using boost.bind with a member function

		char const *first = "{44}", *last = first + std::strlen(first);
		//[tutorial_attach_actions4
		writer w;
		parse(first, last, '{' >> int_[boost::bind(&writer::print, &w, _1)] >> '}');
		//]
	}

	{ // example using boost.lambda
	    namespace lambda = boost::lambda;
	    char const * first = "{45}", *last = first + std::strlen(first);
	    using lambda::_1;
	    //[tutorial_attach_actions5
	    parse(first, last, '{' >> int_[std::cout << _1 << '\n'] >> '}');
	    //]
	}

}

void spirit_complex_number_micro_parser()
{
	std::cout << "spirit_complex_number_micro_parser:" << std::endl;
	std::cout << "enter a complex number of the form r or (r) or (r,i):" << std::endl;
	std::cout << "[type q or Q to quit]" << std::endl;

	std::string str;
	while (getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		std::complex<double> c;
        if (client::parse_complex(str.begin(), str.end(), c))
        {
        	std::cout << "Parsing succeeded" << std::endl;
        }
        else
        {
        	std::cout << "Parsing failed" << std::endl;
        }
	}
	std::cout << "bye!" << std::endl;

}


int main() {
	std::cout << "Boost Spirit library examples" << std::endl; // prints Boost Spirit library examples

	spirit_comma_separated_parser1();
    spirit_semantic_action_functions();
    spirit_complex_number_micro_parser();

	return 0;
}
