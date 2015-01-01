//============================================================================
// Name        : typeIdExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

struct Base {}; // non-polymorphic
struct Derived : Base {};

struct Base2 {}; // polymorphic
struct Derived2 : Base2 {};

int main() {
	cout << "typeid usage example" << endl; // prints typeid usage example
	int myint = 50;
	string mystr = "string";
	const string mystr2 = "string";
	string & mystr3 = mystr;
	double *mydoubleptr = nullptr;

	const type_info& ti1 = typeid(mystr);
	const type_info& ti2 = typeid(mystr2);
	const type_info& ti3 = typeid(mystr3);

	cout << boolalpha;
	cout << "ti1 == ti3 : " << (ti1 == ti3) << endl;

	cout << "myint has type: " << typeid(myint).name() << '\n'
		 << "mystr has type: " << typeid(mystr).name() << '\n'
		 << "mystr2 has type: " << typeid(mystr2).name() << '\n'
		 << "mydoubleptr has type: " << typeid(mydoubleptr).name() << '\n';

	// std::cout << is a glvalue expression of polymorphic type ; it is evaluated
	const type_info& r1 = typeid(cout << myint);

	cout << "std::cout << myint has type : " << r1.name() << '\n';

	// std::printf() is not glvalue expression of polymorphic type ; NOT evaluated
	const type_info& r2 = typeid(printf("%d\n", myint));
	cout << "printf(\"%d\\n\",myint) has type : " << r2.name() << '\n';

	// non-polymorphic lvalue is a static type
	Derived d1;
	Base& b1 = d1;
	cout << "reference to non-polymorphic base: " << typeid(b1).name() << '\n';

	Derived2 d2;
	Base2& b2 = d2;
	cout << "reference to polymorphic base: " << typeid(b2).name() << '\n';


	return 0;
}
