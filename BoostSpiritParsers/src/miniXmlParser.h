#pragma once
// a mini XML-like parser
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace client
{
  namespace fusion = boost::fusion;
  namespace phoenix = boost::phoenix;
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  // our mini XML tree representation
  struct mini_xml;

  typedef
		  boost::variant<
		  	  boost::recursive_wrapper<mini_xml>
  	  	  	  , std::string
  	  	  	  >
  	  	  mini_xml_node;

  struct mini_xml
  {
	  std::string name;   // tag name
	  std::vector<mini_xml_node> children;
  };

}

// register our mini_xml struct with boost
BOOST_FUSION_ADAPT_STRUCT(
  client::mini_xml,
  (std::string, name)
  (std::vector<client::mini_xml_node>, children)
)




