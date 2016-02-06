/*
 * roman.h
 *
 *  Created on: Jan 24, 2016
 *      Author: root
 */

// roman numerals parser

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    // parse roman hundreds (100..900) numerals using the symbol table
    // notice that the data associated with each slot is the parser's attribute
    // (which is passed to attached semantic actions).

    struct hundreds_ : qi::symbols<char, unsigned>
    {
    	hundreds()
		{
    		add
    			("C"  , 100)
    			("CC" , 200)
    			("CCC", 300)
    			("CD" , 400)
    			("D"  , 500)
    			("DC" , 600)
		}
    }



}
