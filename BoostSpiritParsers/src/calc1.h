#pragma once

// plain calculator example demonstrating the grammar. The parser is synthax checker
// only and does not do any semantic evaluation

// With BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
// the developer is responsible for creating instances of terminals which are needed
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <string>

namespace client
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;

   // our calculator grammar
   template <typename Iterator>
   struct calculator : qi::grammar<Iterator, ascii::space_type>
   {
	   calculator() : calculator::base_type(expression)
	   {
		  qi::uint_type uint_;

		  expression =
				  term
				  >> *(  ('+' >> term)
					  |  ('-' >> term)
					  )
				  ;

		  term =
				  factor
				  >> *(  ('*' >> factor)
					  |  ('/' >> factor)
					  )
				  ;

		  factor =
				  uint_
				  |  '(' >> expression >> ')'
				  |   ('-' >> factor)
				  |   ('+' >> factor)
				  ;

	   }

	   qi::rule<Iterator, ascii::space_type> expression, term, factor;
   };
}
