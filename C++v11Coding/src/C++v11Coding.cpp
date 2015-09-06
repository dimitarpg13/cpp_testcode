//============================================================================
// Name        : C++v11Coding.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <fstream>
#include <utility>
#include <vector>
#include <string>

#include <iterator>
#include <algorithm>
#include <array>

#include <fstream>
#include <iostream>

using namespace std;

void tie_example()
{
	// tie example
	std::ostream *prevstr;
	std::ofstream ofs;
	ofs.open("test.txt");

	std::cout << "----------- tie_example ------------" << endl;;

	*std::cin.tie() << "This is inserted into cout";
	prevstr = std::cin.tie(&ofs);
	*std::cin.tie() << "This is inserted into the file";
	std::cin.tie(prevstr);
    std::cout << endl << endl;
	ofs.close();
}

void move_example()
{
	std::string str = "Hello";
	std::vector<std::string> v;

	// uses the push_back(const T&) overload, which means
	// we will incur the cost of copying str
	v.push_back(str);

	std::cout << "--------- move_example --------------" << endl;
    std::cout << "After copy, str is \"" << str << "\"\n";

    // uses rvalue reference push_back(T&&) overload,
    // which means no strings will be copied; instead, the contents
    // of str will be moved into vector. This is less expensive but
    // it also means str might be empty
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";

    std::cout << "The contents of the vector are \"" << v[0]
                                            << "\", \"" << v[1] << "\"\n";

    // string move assignment operator is often implemented as a swap,
    // in this case, the moved-from object is not empty
    std::string str2 = "Good-bye";
    std::cout << "Before move from str2, str2 = '" << str2 << "'\n";
    v[0] = std::move(str2);
    std::cout << "After move from str2, str2 = '" << str2 << "'\n";

    std::cout << "After move the contents of the vector are \"" << v[0]
                                               << "\", \"" << v[1] << "\"\n";
    std::cout << endl << endl;
}

void array_example()
{
	// construction uses aggregate initialization
	std::array<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++ 11 not in C++ 14
	std::array<int, 3> a2 = { 1, 2, 3 }; // never required after =
    std::array<std::string, 2> a3 = { std::string("a"), "b" };

    // container operations are supported
    std::sort(a1.begin(), a1.end());
    std::reverse_copy(a2.begin(), a2.end(),
    				  std::ostream_iterator<int>(std::cout, " "));

    std::cout << "----------- array_example --------------" << endl;

    // ranged for loop is supported
    for (const auto& s : a3)
    	std::cout << s << ' ';
    std::cout << endl << endl;
};

void fstream_example()
{
	char str[10];

	// creates an instance of ofstream and opens example.txt
    ofstream a_file ("example.txt");

    std::cout << "----------- fstream_example ------------------" << endl;
    // outputs to example.txt through a_file
    a_file << "This text will be inside of example.txt";
    a_file.close();

    ifstream b_file ("example.txt");
    b_file >> str;
    // should output 'This' only
    std::cout << str << endl;
    b_file.close();
    std::cout << endl;
};

void basic_ostream_example()
{
	std::cout << "------------ basic_ostream_example -------------" << endl;
    std::cout.put('a'); // normal usage
    std::cout.put('\n');

    std::ofstream s("/does/not/exist");
    s.clear(); // pretend the stream is good
    std::cout << "Unformatted output: ";
    s.put('c'); // this will set a badbit but not a failbit
    std::cout << " fail=" << bool(s.rdstate() & s.failbit);
    std::cout << " bad=" << s.bad() << std::endl;
    s.clear();
    std::cout << "Formatted output: ";
    s << 'c'; // this will set badbit and failbit
    std::cout << " fail=" << bool(s.rdstate() & s.failbit);
    std::cout << " bad=" << s.bad() << std::endl;

    std::cout << endl << endl;
}

class Rectangle
{
	int width, height;
public:
	Rectangle() {};
	Rectangle (int x, int y) : width(x), height(y) {};
	int area() { return width * height; }
    friend Rectangle duplicate (const Rectangle & );
};

Rectangle duplicate (const Rectangle& param)
{
	Rectangle res;
	res.width = param.width * 2;
	res.height = param.height * 2;
	return res;
};

void friend_function_example()
{
	Rectangle foo;
	Rectangle bar(2,3);
	foo = duplicate (bar);
	std::cout << "----------- friend_function_example -------------" << endl;
    std::cout << "foo.area = " << foo.area() << endl;
};

int main() {
	cout << "C++v11 coding examples" << endl; // prints C++v11 coding examples

    tie_example();

    move_example();

    array_example();

    fstream_example();

    basic_ostream_example();

    friend_function_example();


	return 0;
}
