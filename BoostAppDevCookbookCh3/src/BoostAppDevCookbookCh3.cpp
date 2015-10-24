//============================================================================
// Name        : BoostAppDevCookbookCh3.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;



class foo_class
{
private:
	std::string str_;
public:
	foo_class(std::string str) : str_(str)
    {

    }
};

bool some_function1(foo_class * fc_ptr)
{
	bool res = false;

	return res;
}

bool some_function2(foo_class * fc_ptr)
{
	bool res = false;

	return res;
}

bool foo3()
{
	boost::scoped_ptr<foo_class> p(new foo_class("Some initialization data"));
	bool something_else_happened = some_function1(p.get());
	if (something_else_happened)
	{
		return false;
	}

	some_function2(p.get());
	return true;
}


void process1(const foo_class * p)
{

};

void process2(const foo_class * p)
{

};

void process3(const foo_class * p)
{

};

foo_class * get_data()
{
	return new foo_class("my_data");
};

void foo1()
{
	while (foo_class * p = get_data()) //
	{
		// there will be too many threads soon see recipe "Executing different tasks in parallel"
		// for a good way to avoid uncontrolled growth of threads
		boost::thread(boost::bind(&process1,p)).detach();


		// this is a no-no - leads to a crash
		delete p;
	}
};

void process_sp1(const boost::shared_ptr<foo_class>& p)
{

};

void process_sp2(const boost::shared_ptr<foo_class>& p)
{

};

void process_sp3(const boost::shared_ptr<foo_class>& p)
{

};


void foo2()
{
	typedef boost::shared_ptr<foo_class> ptr_t;

	ptr_t p;
	while (p = ptr_t(get_data()))  // C way
	{
		boost::thread(boost::bind(&process_sp1, p)).detach();
		boost::thread(boost::bind(&process_sp2, p)).detach();
		boost::thread(boost::bind(&process_sp3, p)).detach();

	}
};

void scoped_ptr_example()
{
	std::cout << "scoped_ptr_example:" << std::endl;
	bool res = foo3();

}





int main() {
	cout << "Boost Application Development Cookbook Chapter 3" << endl; // prints Boost Application Development Cookbook Chapter 3


	return 0;
}
