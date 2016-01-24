/*
 * sum.h
 *
 *  Created on: Jan 23, 2016
 *      Author: root
 */

#pragma once

// a parser for summing comma-separated list of numbers using phoenix
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <iostream>
#include <string>

namespace client
{
   namespace qi = boost::spirit::qi;
   namespace ascii = boost::spirit::ascii;
   namespace phoenix = boost::phoenix;

   using qi::double_;
   using qi::_1;
   using ascii::space;
   using phoenix::ref;

   template <typename Iterator>
   bool adder(Iterator first, Iterator last, double & n)
   {
      bool r = qi::phrase_parse(first, last,

    		  // Begin grammar
    		  (
    			double_[ref(n) = _1] >> *(',' >> double_[ref(n) += _1])
    		  )
    		  ,
    		  // End grammar
    		  space);

      if (first != last)  // fail if we did not get a full match
    	  return false;
      return r;
   }

   bool test_sum();
}
