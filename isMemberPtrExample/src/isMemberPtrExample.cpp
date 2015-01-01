//============================================================================
// Name        : isMemberPtrExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <type_traits>

struct A { int x; };

class AA { void member() {}; };

void my_func1() {};

using namespace std;



int main() {
	cout << "is_member_pointer and is_member_function_pointer example" << endl; // prints is_member_pointer and is_member_function_pointer example

	int A::* pt = &A::x;

	cout << boolalpha;

	cout << pt << endl;
	cout << "is_member_pointer: " << endl;
    cout << "A*: " << is_member_pointer<A*>::value << endl;
    cout << "int A::*: " << is_member_pointer<int A::*>::value << endl;
    cout << "void(A::*)(): " << is_member_pointer<void(A::*)()>::value << endl;
    cout << "decltype(pt): " << is_member_pointer<decltype(pt)>::value << endl;

    cout << "decltype(&AA::member): " << is_member_function_pointer<decltype(&AA::member)>::value << endl;

    cout << "is_fundamental<unsigned>: " << is_fundamental<unsigned>::value << endl;
    cout << "is_fundamental<float>: " << is_fundamental<float>::value << endl;
    cout << "is_fundamental<float*>: " << is_fundamental<float*>::value << endl;
    cout << "is_fundamental<decltype(nullptr)>: " << is_fundamental<decltype(nullptr)>::value << endl;

    cout << "is_function<>: " << is_function<decltype(my_func1)>::value << endl;

	return 0;
}
