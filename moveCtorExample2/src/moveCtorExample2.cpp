//============================================================================
// Name        : moveCtorExample2.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

using namespace std;

struct A {
	std::string s;
	A(string str) : s(str) {}
	A() : s("test") {}
	A(const A& o) : s(o.s) { std::cout << "move failed!" << endl; }
    A(A&& o) : s(std::move(o.s)) {}
    A& operator=(const A&) { std::cout << "copy assigned" << endl; return *this; }
    A& operator=(A&& other) {
    	s = std::move(other.s);
    	std::cout << "move assigned" << endl;
    	return *this;
    }
};

A f(A a) {
	return a;
};

struct B : A {
	std::string s2;
	int n;
	// implicit move constructor B::(B&&)
	// calls A's move constructor
	// calls s2's move constructor
	// and makes bitwise copy of n

};

struct C : B {
	~C() {}; // destructor prevents implicit move constructor C::(C&&)
};

struct D : B {
	D() {};
	~D() {}; // dtor would prevent implicit move ctor D::(D&&)
	D(D&&) = default; // force  a move ctor anyway
};


int main()
{
	std::cout << "move constructor example 2" << endl << endl; // prints move constructor example 2
	A a1, a2;
	std::cout << "Trying to move-assign A from rvalue temporary" << endl;
	std::cout << "Before move, a1.s =\"" << a1.s << "\"" << endl;
	a1 = f(A("newtest"));
	std::cout << "After move, a1.s =\"" << a1.s << "\"" << endl;
	std::cout << "Trying to move-assign A from xvalue" << endl;
	std::cout << "Before move, a2.s =\"" << a2.s << "\"" << endl;
	a2 = std::move(a1); // move assignment from xvalue
	std::cout << "After move, a2.s =\"" << a2.s << "\"" << endl;
    std::cout << endl;

	std::cout << "Trying to move B" << endl;
	B b1;
	std::cout << "Before move, b1.s =\"" << b1.s << "\"" << endl;
	B b2 = std::move(b1); // calls implicit move ctor
	std::cout << "After move, b1.s =\"" << b1.s << "\"" << endl;
	std::cout << endl;

	std::cout << "Trying to move C" << endl;
	C c1;
	std::cout << "Before move, c1.s =\"" << c1.s << "\"" << endl;
	C c2 = std::move(c1); // calls the copy constructor
	std::cout << "Before move, c1.s =\"" << c1.s << "\"" << endl;
	std::cout << endl;

	std::cout << "Trying to move D" << endl;
	D d1;
	std::cout << "Before move, d1.s =\"" << d1.s << "\"" << endl;
	D d2 = std::move(d1);
	std::cout << "After move, d1.s =\"" << d1.s << "\"" << endl;
	std::cout << endl;

	return 0;
}
