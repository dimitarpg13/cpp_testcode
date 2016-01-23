

#include "miniXmlParser.h"

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
}
