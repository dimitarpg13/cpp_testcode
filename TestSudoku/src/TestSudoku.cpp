//============================================================================
// Name        : TestSudoku.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>
#include <stack>


using namespace std;

int main() {

	cout << "TestSudoku" << endl; // prints TestSudoku

//    std::set<char> s1, s2;
//	s1.insert('1');
//	s1.insert('2');
//	s1.insert('3');
//	s1.insert('4');
//	s1.insert('5');
//	s2.insert('2');
//	s2.insert('3');
//	const set<char> & s11 = s1;
//	const set<char> & s22 = s2;
//
//	// Fill in s1 and s2 with values
//	std::set<char> result;
//	std::set_difference(s11.begin(), s11.end(), s22.begin(), s22.end(),
//	    std::inserter(result, result.end()));
//
//	for (set<char>::iterator it = result.begin(); it != result.end(); it++)
//		cout << *it;

	stack<int*> s;
	if (!s.empty())
	{
    int * p = s.top();
    if (p != NULL)
      cout << "p not null!" << endl;
    else
      cout << "p is null" << endl;
	}
	else
		cout << "the stack is empty!" << endl;
	return 0;
}
