//============================================================================
// Name        : BoostTest06.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/thread/thread.hpp>
#include <boost/lockfree/stack.hpp>
#include <boost/atomic.hpp>

#include <iostream>

using namespace std;

boost::atomic_int producer_count(0);
boost::atomic_int consumer_count(0);

boost::lockfree::stack<int> stack(128);

int main() {

	return 0;
}
