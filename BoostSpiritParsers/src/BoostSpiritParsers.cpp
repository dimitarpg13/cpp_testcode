//============================================================================
// Name        : BoostSpiritParsers.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include "miniXml.h"

using namespace std;

int main() {
	cout << "Boost Spirit Parsers" << endl; // prints Boost Spirit Parsers
	client::parse_print_minixml("./src/1.toyxml");
	return 0;
}
