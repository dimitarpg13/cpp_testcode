//============================================================================
// Name        : isTriviallyDestructibleExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <type_traits>

using namespace std;

struct A {};
struct B { ~B(){} };
struct C : B {};
struct D : A {};

int main() {
	cout << "is_trivially_destructible example" << endl; // prints is_trivially_destructible example

	cout << std::boolalpha;
    cout << "int: " << std::is_trivially_destructible<int>::value << std::endl;
    cout << "A: " << std::is_trivially_destructible<A>::value << std::endl;
    cout << "B: " << std::is_trivially_destructible<B>::value << std::endl;
    cout << "C: " << std::is_trivially_destructible<C>::value << std::endl;
    cout << "D: " << std::is_trivially_destructible<D>::value << std::endl;

	return 0;
}
