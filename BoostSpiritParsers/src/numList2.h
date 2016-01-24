/*
 * numList2.h
 *
 *  Created on: Jan 23, 2016
 *      Author: root
 */


#pragma once

// this sample demonstrates a parser from a comma separated list of numbers.
// the numbers are inserted in a vector using phoenix

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;


    // our number list compiler

}
