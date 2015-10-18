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
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
//#include <boost/fusion/sequence/intrinsic/at_c.hpp>


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

	   bool r = qi::phrase_parse(first, last,
		// begin grammar
		(
		  double_[ref(n) = _1] >> *(',' >> double_[ref(n) += _1])
		)
	    // end grammar
	    ,space);

	   if (first != last)  // fail if we did not get full match
		   return false;
	   return r;
   }
   //]

   //[tutorial_numlist2
   template<typename Iterator>
   bool parse_numbers(Iterator first, Iterator last, std::vector<double>& v)
   {
	   namespace qi = boost::spirit::qi;
	   namespace ascii = boost::spirit::ascii;
	   namespace phoenix = boost::phoenix;

	   using qi::double_;
	   using qi::phrase_parse;
	   using qi::_1;
	   using ascii::space;
	   using phoenix::push_back;

	   bool r = phrase_parse(first, last,
	   // begin grammar
	   (
		  double_[push_back(phoenix::ref(v), _1)]
		          >> *(',' >> double_[push_back(phoenix::ref(v), qi::_1)])
	   )
	   //end grammar
	   ,space);

	   return r;
   }

   //[tutorial_numlist3
   template<typename Iterator>
   bool parse_numbers_redux(Iterator first, Iterator last, std::vector<double> & v)
   {
	   using qi::double_;
	   using qi::phrase_parse;
	   using qi::_1;
	   using ascii::space;
	   using boost::phoenix::push_back;
       bool r = phrase_parse(first, last,
    		   // begin grammar
    		   (
    			 double_[push_back(boost::phoenix::ref(v), _1)] % ','
    		   )
    		   ,
    		   // end grammar
    		   space);

       if (first != last) // fail if we do not get full match
    	   return false;

       return r;
   }


   // parse roman hundreds (100..900) numerals using a symbol table
   // notice that the data associated with each slot is the parser's
   // attribute (which is passed to attached semantic actions).
   //[tutorial_roman_hundreds
   struct hundreds_ : qi::symbols<char, unsigned>
   {
   	hundreds_()
   	{
   	   add
   	   	   ("C", 100)
   	   	   ("CC", 200)
   	   	   ("CCC", 300)
   	   	   ("CD", 400)
   	   	   ("D", 500)
   	   	   ("DC", 600)
   	   	   ("DCC", 700)
   	   	   ("DCCC", 800)
   	   	   ("CM", 900)
   	   ;
   	}

   } hundreds;
   //]

   //parse roman tens (10..90) using the symbol table
   //[tutorial_roman_tens
   struct tens_ : qi::symbols<char, unsigned>
   {
	  tens_()
	  {
		 add
		     ("X", 10)
		     ("XX", 20)
		     ("XXX", 30)
		     ("XL", 40)
		     ("L", 50)
		     ("LX", 60)
		     ("LXX", 70)
		     ("LXXX", 80)
		     ("XC", 90)
		 ;
	  }

   } tens;
   //]

   //parse roman ones (1..9) using the symbol table
   //[tutorial_roman_ones
   struct ones_ : qi::symbols<char, unsigned>
   {
   	  ones_()
   	  {
   		 add
   		     ("I", 1)
   		     ("II", 2)
   		     ("III", 3)
   		     ("IV", 4)
   		     ("V", 5)
   		     ("VI", 6)
   		     ("VII", 7)
   		     ("VIII", 8)
   		     ("IX", 9)
   		 ;
   	  }

   } ones;
   //]


   //[roman numerals grammar
   template<typename Iterator>
   struct roman : qi::grammar<Iterator, unsigned()>
   {
	   roman() : roman::base_type(start)
	   {
		   using qi::eps;
		   using qi::lit;
		   using qi::_val;
		   using qi::_1;
		   using ascii::char_;

		   start = eps  	[_val = 0]  >>
		   (
			 +lit('M')		[_val += 1000]
			  ||  hundreds 	[_val += _1]
			  ||  tens		[_val += _1]
			  ||  ones  	[_val += _1]
		   )
		   ;
	   }

	   qi::rule<Iterator, unsigned()> start;
   };

   //tutorial employee_struct
   struct employee
   {
	   int age;
	   std::string surname;
	   std::string forename;
	   double salary;
   };
   //]



};


//[tutorial_employee_adapt_struct
BOOST_FUSION_ADAPT_STRUCT(
   client::employee,
   (int, age)
   (std::string, surname)
   (std::string, forename)
   (double, salary)
)
//]


namespace client
{
	//[tutorial_employee_parser
	template<typename Iterator>
	struct employee_parser : qi::grammar<Iterator, employee(), ascii::space_type>
	{
	    employee_parser()
		{
           using qi::int_;
           using qi::lit;
           using qi::double_;
           using qi::lexeme;
           using ascii::char_;

           quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

           start %=
        	   lit("employee")
        	   >> '{'
        	   >> int_ >> ','
        	   >> quoted_string >> ','
        	   >> quoted_string >> ','
        	   >> double_
        	   >> '}'
        	   ;
		}

	    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
	    qi::rule<Iterator, employee(), ascii::space_type> start;
	};
	//]

	namespace fusion = boost::fusion;
	namespace phoenix = boost::phoenix;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	//[tutorial_xml1_strustures
	struct mini_xml;

	typedef
		boost::variant<
				boost::recursive_wrapper<mini_xml>
				, std::string
				>
		mini_xml_node;

	struct mini_xml
	{
		std::string name;					 // tag name
		std::vector<mini_xml_node> children; // childred
	};
	//]
};

// we need to tell fusion about our mini_xml struct
// to make it a first class fusion citizen
//[tutorial_xml1_adapt_structures
BOOST_FUSION_ADAPT_STRUCT(
  client::mini_xml,
  (std::string, name)
  (std::vector<client::mini_xml_node>, children)
)
//]

namespace client
{
	// print out mini xml tree
	int const tabsize = 4;

	void tab(int indent)
	{
		for (int i = 0; i < indent; ++i)
			std::cout << ' ';
	}

	struct mini_xml_printer
	{
		mini_xml_printer(int indent = 0)
			: indent(indent)
		{
		}

		void operator()(mini_xml const & xml) const;

	    int indent;
	};


    struct mini_xml_node_printer : boost::static_visitor<>
    {
		mini_xml_node_printer(int indent = 0)
				: indent(indent)
		{
		}

		void operator()(mini_xml const & xml) const
		{
			mini_xml_printer(indent+tabsize)(xml);
		}

		void operator() (std::string const & text) const
		{
			tab(indent+tabsize);
			std::cout << "text: \"" << text << '"' << std::endl;
		}

		int indent;
    };

    void mini_xml_printer::operator() (mini_xml const & xml) const
    {
    	tab(indent);
    	std::cout << "tag: " << xml.name << std::endl;
    	tab(indent);
    	std::cout << '{' << std::endl;

    	BOOST_FOREACH(mini_xml_node const& node, xml.children)
    	{
    		boost::apply_visitor(mini_xml_node_printer(indent), node);
    	}

    	tab(indent);
    	std::cout << '}' << std::endl;
    };


    //[tutorial_xml1_grammar
    template<typename Iterator>
    struct mini_xml_grammar : qi::grammar<Iterator, mini_xml(), ascii::space_type>
    {
    	mini_xml_grammar() : mini_xml_grammar::base_type(xml)
    	{
    		using qi::lit;
    		using qi::lexeme;
    		using qi::_1;
    		using ascii::char_;
    		using ascii::string;
    		using namespace qi::labels;

    		using boost::fusion::at_c;
    		using boost::phoenix::push_back;

    		text = lexeme[+(char_ - '<')   [_val += _1]];
    		node = (xml | text)		       [_val = _1];

    		start_tag =
    				'<'
    			>> !lit('/')
    			>> lexeme[+(char_ - '>')  [_val += _1]]
    			>> '>'
    		;

    		end_tag =
    				"</"
    			>> string(_r1)
    			>> '>'
    		;

    		xml =
    			start_tag 					[at_c<0>(_val) = 1]
    	    >> *node						[push_back(at_c<1>(_val), _1)]
    	    >> end_tag(at_c<0>(_val))
    	    ;

    	}

    	qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
    	qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
    	qi::rule<Iterator, std::string(), ascii::space_type> text;
    	qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
    	qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
    };
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

};

void spirit_adder1()
{
	std::cout << "spirit_adder1:" << std::endl;
	std::cout << "enter a comma separated list of numbers:" << std::endl;
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


void spirit_comma_separated_list_parser()
{
	std::cout << "spirit_comma_separated_list_parser:" << std::endl;
	std::cout << "enter a comma separated list of numbers:" << std::endl;
	std::cout << "[type q or Q to quit]" << std::endl;

	std::string str;
	while (getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
			break;

		std::vector<double> v;
		if (client::parse_numbers(str.begin(),str.end(),v))
		{
			std::cout << "Parsing succeeded" << std::endl;

		}
	}
}


void spirit_roman_numerals_parser()
{
	std::cout << "spirit_roman_numerals_parser:" << std::endl;
	std::cout << "enter a comma separated list of numbers:" << std::endl;
	std::cout << "[type q or Q to quit]" << std::endl;


	typedef std::string::const_iterator iterator_type;
	typedef client::roman<iterator_type> roman;

	roman roman_parser; //our grammar

	std::string str;
	unsigned result;
	while (std::getline(std::cin, str))
	{
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
		  break;

		std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        //[tutorial_roman_grammar_parse
        bool r = parse(iter, end, roman_parser, result);

        if (r && iter == end)
        {
        	std::cout << "Parsing succeeded." << std::endl;
        }
        else
        {
        	std::string rest(iter, end);
        	std::cout << "Parsing failed. Stopped at: \"" << rest << "\"" << std::endl;
        }
        //]


	}

}

void spirit_employee_parser()
{
	std::cout << "spirit_employee_parser:" << std::endl;
	std::cout << "employee{age,\"surname\",\"forename\",salary}" << std::endl;
	std::cout << "[type q or Q to quit]" << std::endl;

	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef client::employee_parser<iterator_type> employee_parser;

}

void spirit_mini_xml1_parser()
{

}

int main() {
	std::cout << "Boost Spirit library examples" << std::endl; // prints Boost Spirit library examples

	//spirit_comma_separated_parser1();
    //spirit_semantic_action_functions();
    //spirit_complex_number_micro_parser();
    //spirit_comma_separated_list_parser();
    spirit_roman_numerals_parser();

	return 0;
}
