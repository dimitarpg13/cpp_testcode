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



void do_some_work()
{
   std::cout << "I am doing work at the moment..." << std::endl;
}

void * call_from_posix_thread(void *)
{
	std::cout << "Launched by a posix thread..." << std::endl;

	return NULL;
}

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

    // join the thread with the main thread
    pthread_join(t, NULL);
    std::cout << "Joined the posix thread. exiting..." << std::endl;

}

int main() {
	std::cout << "Concurrency in Action examples" << std::endl; // prints Concurrency in Action examples

	call_a_thread_example();
    call_a_posix_thread_example();


	return 0;
}
