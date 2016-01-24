//============================================================================
// Name        : BoostSpiritParsers.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include "miniXml.h"
#include "complexNumb.h"
#include "sum.h"

using namespace std;

int main() {
	cout << "Boost Spirit Parsers" << endl; // prints Boost Spirit Parsers
	//client::test_minixml("./src/1.toyxml");
	//client::test_complexnumb();
	client::test_sum();
	return 0;
}
