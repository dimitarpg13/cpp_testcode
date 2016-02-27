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
#include "numList2.h"
#include "numList3.h"
#include "numList4.h"
#include "roman.h"

using namespace std;

int main() {
	cout << "Boost Spirit Parsers" << endl; // prints Boost Spirit Parsers
	//client::test_minixml("./src/1.toyxml");
	//client::test_complexnumb();
	//client::test_sum();
	//client::test_numList2();
	//client::test_numList3();
	client::test_numList4();

	return 0;
}
