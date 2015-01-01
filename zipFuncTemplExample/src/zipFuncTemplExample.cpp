//============================================================================
// Name        : zipFuncTemplExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

using namespace std;

template <class ContainerA>
unsigned commonLength(unsigned len, const ContainerA &first)
{
	unsigned firstLen = first.size();
	if (len > firstLen)
	{
		len = firstLen;
	}
	return len;
}

template<class ContainerA, class... Containers>
unsigned commonLength(unsigned len, const ContainerA & first, const Containers&... rest)
{
   unsigned firstLen = first.size();
   if (len > firstLen)
   {
	   len = firstLen;
   }
   return commonLength(len, rest...);
}

int main() {
	cout << "Python zip function  example implemented with templates" << endl; // prints Python zip function  example implemented with templates
	return 0;
}
