//============================================================================
// Name        : streamTut01.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

// timestamp returns the current time as a string
std::string timestamp();

class LogStatement;
ostream& operator<<(ostream& ost, const LogStatement& ls);

class LogStatement
{
public:
	LogStatement(std::string s) : data(s), time_string( timestamp() )
    {};

	friend ostream& operator<<(ostream&, const LogStatement& );
};

int main() {
	cout << "stream application tutorial 01" << endl; // prints stream application tutorial 01

	return 0;
}
