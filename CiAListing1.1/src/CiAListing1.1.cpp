//============================================================================
// Name        : 1.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>

using namespace std;

void hello()
{
	std::cout << "Hello Concurrent World" << endl;
}

int main() {
	cout << "Concurency In Action Listing 1.1" << endl; // prints Concurency In Action Listing 1.1
	std::thread t(hello);
	t.join();
	return 0;
}
