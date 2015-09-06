//============================================================================
// Name        : playground.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <type_traits>
#include <assert.h>
using namespace std;

void tempfun() {};

// template explicit instantiation
template<class T> class Y { void mf() {} }; // template definition
template class Y<char*>;
template void Y<double>::mf();

int main() {
	cout << "testing code " << endl; // prints testing code 
	assert( ( std::is_same<short,short>::value ) );

	return 0;
}
