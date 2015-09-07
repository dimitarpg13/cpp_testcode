//============================================================================
// Name        : SudokuQlik.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include "Parser.h"
#include "Symbol.h"
#include "Puzzle.h"

using namespace std;
using namespace sudoku;

int main() {
	cout << "SudokuQlik" << endl; // prints SudokuQlik



	Parser parser(9,'.');

	parser.parse("./src/example1.txt");

    Symbol s('1');
    Region * r = s.getRegion();
    vector<Region *> v;
    v.push_back(r);

    cout << endl << endl << "Problem instance:" << endl;

    Puzzle * p = new Puzzle();
    p->load("./src/example1.txt");
    delete p;

    cout << endl << endl;
    char c = 0101;
    cout << c ;

	return 0;
}
