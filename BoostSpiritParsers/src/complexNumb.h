#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>
#include <complex>


// our complex number parser/compiler
namespace client
{
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
			    |	double_[ref(rN) = _1]
		  ),
		  // end grammar

		  space);

	   if (!r || first != last)  // fail if we did not get a full match
		   return false;
	   c = std::complex<double>(rN,iN);
	   return r;
   }

   bool test_complexnumb();

}
