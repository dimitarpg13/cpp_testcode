//============================================================================
// Name        : ConcurrencyInAction.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <pthread.h> // for the pthread example
#include <string>


void do_some_work()
{
   std::cout << "I am doing work at the moment..." << std::endl;
}

void call_from_thread(int tid)
{
	std::cout << "Launched by thread " << tid << std::endl;
}

void * call_from_posix_thread(void *)
{
	std::cout << "Launched by a posix thread..." << std::endl;

	return NULL;
}

void independentThread()
{
	std::cout << "Starting concurrent thread." << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "Exiting concurrent thread." << std::endl;
}

void threadCaller()
{
	std::cout << "Starting thread caller" << std::endl;
    std::thread t(independentThread);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Exiting thread caller" << std::endl;
}

class thread_guard
{
	std::thread & t;
public:
	explicit thread_guard(std::thread & t_) : t(t_)
	{}

    ~thread_guard()
    {
    	if (t.joinable())
    	{
    		t.join();
    	}
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator= (thread_guard const &) = delete;
};

void do_something(int i)
{

};

struct func
{
	int & i;
	func(int & i_) : i(i_) {}
    void operator() ()
    {
    	for (int j = 0; j < 1000000; ++j )
    	{
    		do_something(i);
    	}
    }
};



void call_a_thread_example()
{
	std::cout << "call_a_thread_example:" << std::endl;
	std::thread t1(do_some_work);
	t1.join();
	std::cout << "Joined the thread. exiting..." << std::endl;
}

void call_a_posix_thread_example()
{
	std::cout << "call_a_posix_thread_example:" << std::endl;
    pthread_t t;

    pthread_create(&t, NULL, call_from_posix_thread, NULL);

    std::cout << "Launched from call_a_posix_thread_example" << std::endl;

    // join the thread with the main thread
    pthread_join(t, NULL);
    std::cout << "Joined the posix thread. exiting..." << std::endl;

};




static const int num_threads = 10;
void call_many_threads_example()
{
	std::cout << "call_many_threads_example:" << std::endl;
	//std::thread t1(do_some_work);
	std::thread t[num_threads];
	// launch a group of threads
	for (int i = 0; i < num_threads; i++)
	{
		t[i] = std::thread(call_from_thread, i);
	}

	std::cout << "Launched from call_many_threads_example" << std::endl;

	for (int i = 0; i < num_threads; ++i)
	{
	  t[i].join();
	}

	std::cout << "Joined all of the threads. exiting..." << std::endl;
}


void thread_detach_example()
{
	threadCaller();
	std::this_thread::sleep_for(std::chrono::seconds(5));
}


int main() {
	std::cout << "Concurrency in Action examples" << std::endl; // prints Concurrency in Action examples

	call_a_thread_example();
    call_a_posix_thread_example();
    call_many_threads_example();
    thread_detach_example();

	return 0;
}
