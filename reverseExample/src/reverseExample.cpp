//============================================================================
// Name        : reverseExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iterator>
#include <vector>
#include <list>

using namespace std;

template< class BidirIt >
void my_reverse(BidirIt first, BidirIt last)
{
	typename std::iterator_traits<BidirIt>::difference_type n = std::distance(first, last);
	--n;
	while (n > 0)
	{
		typename std::iterator_traits<BidirIt>::value_type tmp = *first;
		*first++ = *--last;
		*last = tmp;
		n -= 2;
	}
}

int main() {
	cout << "reverse example" << endl; // prints reverse example
	vector<int> vect{1,2,3,4,5,6};
	cout << "difference: " << distance(vect.begin(),vect.end()) << endl;
	my_reverse(vect.begin(),vect.end());
	for (int v : vect)
	{
		cout << v << ' ';
	}
	cout << endl;

	list<int> lst{1,2,3,4,5};
	my_reverse(lst.begin(), lst.end());
	for (int v : lst)
	{
		cout << v << ' ';
	}
	cout << endl;


	return 0;
}
