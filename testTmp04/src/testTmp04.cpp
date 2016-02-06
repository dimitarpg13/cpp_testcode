//============================================================================
// Name        : testTmp04.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class A
{
	char * stor;
public:
	A()
    {
	    stor = new char[20];
	    for (int i=0; i < 20; i++)
	    	stor[i] = '1';
    }

	A(int sz)
	{
		stor = new char[sz];
		for (int i = 0; i < sz; i++)
			stor[i] = '2';
	}

	char * data() { return stor; }

	~A()
	{
		if (stor != NULL)
		{
			delete[] stor;
			stor = NULL;
		}
	}
};

A * ptr = NULL;
A ** ptr2 = NULL;

int main() {
	cout << "Test pointer disposal" << endl; // prints Test pointer disposal


	ptr = new A();
	ptr2 = &ptr;

	if (ptr != NULL)
	{
		char * data = ptr->data();

		for (int i = 0; i < 5; i++)
			std::cout << data[i];

		data[0] = 'A';
	}

	delete ptr2;

	return 0;
}
