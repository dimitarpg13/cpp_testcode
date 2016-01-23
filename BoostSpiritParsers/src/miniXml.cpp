

#include "miniXml.h"

namespace client
{

	void tab(int indent)
	{
		for (int i = 0; i < indent; i++)
			std::cout << ' ';
	};

    void mini_xml_printer::operator()(mini_xml const & xml) const
    {
    	tab(indent);
    	std::cout << "tag: " << xml.name << std::endl;
    	tab(indent);
    	std::cout << '{' << std::endl;

    	BOOST_FOREACH(mini_xml_node const & node, xml.children)
    	{
    		boost::apply_visitor(mini_xml_node_printer(indent), node);
    	}

    	client::tab(indent);
    	std::cout << '}' << std::endl;
    };

    bool parse_print_minixml(std::string filename)
    {

    	std::ifstream in(filename, std::ios_base::in);
    	if (!in)
    	{
    		std::cerr << "Error: Could not open input file: "
    				<< filename << std::endl;
    		return false;
    	}

    	std::string storage;  // we will read the contents here
    	in.unsetf(std::ios::skipws); // no white space skipping

    	std::copy(
    	std::istream_iterator<char>(in),
    	std::istream_iterator<char>(),
    	std::back_inserter(storage));


    	typedef client::mini_xml_grammar<std::string::const_iterator> mini_xml_grammar;
    	mini_xml_grammar xml;  // our grammar
    	client::mini_xml ast;  // our tree

    	using boost::spirit::ascii::space;
    	std::string::const_iterator iter = storage.begin();
    	std::string::const_iterator end = storage.end();
    	bool r = phrase_parse(iter, end, xml, space, ast);

    	if (r && iter == end)
    	{
    		std::cout << "---------------------" << std::endl;
    		std::cout << "Parsing successful" << std::endl;
    		std::cout << "---------------------" << std::endl;
    		client::mini_xml_printer printer;
    		printer(ast);
    		return true;
    	}
    	else
    	{
    		std::cout << "---------------------" << std::endl;
    		std::cout << "Parsing failed" << std::endl;
    		std::cout << "---------------------" << std::endl;
    		return false;
    	}


    	return true;
    };
}
