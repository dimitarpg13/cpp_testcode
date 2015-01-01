//============================================================================
// Name        : typeDescriptorExcercise.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

struct type_descriptor_base
{
	string value;
	friend ostream &operator<<( ostream &output,
		                        const type_descriptor_base &td )
    {
	   output << td.value;
	   return output;
    }
};

template<typename type>
struct type_descriptor : public type_descriptor_base
{
	type_descriptor()
	{
	   value = "test";
	}


};

template<>
struct type_descriptor<int> : public type_descriptor_base
{
	type_descriptor()
	{
	   value = "int";
	}
};

template<>
struct type_descriptor<char> : public type_descriptor_base
{
	type_descriptor()
	{
	   value = "char";
	}
};

int main() {
	cout << "type descriptor exercise 2.3" << endl; // prints type descriptor exericise 2.3
	cout << type_descriptor<int>() << endl;
	return 0;
}
