//============================================================================
// Name        : friendTut01.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

using namespace std;

struct X
{
	char * foo(int);
	X(char);
	~X();
};

class Y
{
	int data; // private member
	// the non-member function operator << will have access to Y's private members
	friend std::ostream& operator<< (std::ostream& out, const Y& o);

	friend char* X::foo(int);

	friend X::X(char), X::~X();


};

// friend declaration does not declare a member function
// this operator << still needs to be defined as a non-member
std::ostream& operator<< (std::ostream& out, const Y& y)
{
	return out << y.data;  // can access private member Y::data
}

int main() {
	cout << "friend tutorial 01" << endl; // prints friend tutorial 01
	return 0;
}
