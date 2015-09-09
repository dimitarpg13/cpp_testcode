//============================================================================
// Name        : SudokuQlik.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>


#include "Sudoku.h"
#include "Solver.h"


using namespace std;
using namespace sudoku;

int main() {
	cout << "SudokuQlik" << endl << endl; // prints SudokuQlik


    Puzzle * p = new Puzzle();

    cout << "Example1:" << endl;
    p->load("./src/example1.txt");
    cout << endl << endl;

    cout << "Example2:" << endl;
    p->load("./src/example2.txt");
    cout << endl << endl;

    cout << "Example3:" << endl;
    p->load("./src/example3.txt");
    cout << endl << endl;

    cout << "Example4:" << endl;
    p->load("./src/example4.txt");
    cout << endl << endl;

    Solver * s = new BTSolver(p);
    s->solve();

    delete p;


    char c = 0101;
    cout << c ;

	return 0;
}
