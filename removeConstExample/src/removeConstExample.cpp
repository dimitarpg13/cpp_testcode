//============================================================================
// Name        : removeConstExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <type_traits>

using namespace std;

int main() {
	cout << "remove_cv, remove_const example" << endl; // prints remove_cv, remove_const example
	typedef std::remove_cv<const int>::type type1;
    typedef std::remove_cv<volatile int>::type type2;
    typedef std::remove_cv<const volatile int>::type type3;
    typedef std::remove_cv<const volatile int*>::type type4;

	cout << "is_same<int, remove_cv<const int>::type> = " << (is_same<int, type1>::value ? "true" : "false") << endl;
	cout << "is_same<int, remove_cv<volatile int>::type> = " << (is_same<int, type2>::value ? "true" : "false") << endl;
	cout << "is_same<int, remove_cv<const volatile int>::type> = " << (is_same<int, type3>::value ? "true" : "false") << endl;
	cout << "is_same<int, remove_cv<const volatile int*>::type> = " << (is_same<int, type4>::value ? "true" : "false") << endl;

	return 0;
}
