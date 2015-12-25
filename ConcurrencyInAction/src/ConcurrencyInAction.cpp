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
#include <numeric> // for std::accumulate
#include <vector>
#include <algorithm> // for std::for_each
#include <list>
#include <mutex>
#include <deque>
#include <stack>
#include <functional>
#include <memory>
#include <exception>
#include <cstdlib>
#include <time.h>

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

void open_document_and_display_gui(std::string const & filename)
{

};

bool done_editing()
{
	return true;
};

enum command_type
{
	open_new_document
};

struct user_command
{
	command_type type;
	user_command() : type(open_new_document)
	{
	}
};

user_command get_user_input()
{
	return user_command();
};

std::string get_filename_from_user()
{
	return "foo.doc";
};

void process_user_input(user_command const & cmd)
{

};

void edit_document(std::string const & filename)
{
	open_document_and_display_gui(filename);
	while (!done_editing())
	{
		user_command cmd = get_user_input();
		if (cmd.type == open_new_document)
		{
			std::string const new_name = get_filename_from_user();
			std::thread t(edit_document,new_name);
			t.detach();
		}
		else
		{
			process_user_input(cmd);
		}
	}
};

class scoped_thread
{
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) : t(std::move(t_))
	{
		if (!t.joinable())
			throw std::logic_error("No thread");
	}

	~scoped_thread()
	{
		t.join();
	}
	scoped_thread(scoped_thread const &) = delete;
	scoped_thread & operator = (scoped_thread const &) = delete;
};

void do_something2(int & i)
{
	++i;
};

struct func2
{
	int & i;
	func2(int & i_) : i(i_) {}

	void operator()()
	{
		for (unsigned j=0; j < 1000000; ++j)
		{
			do_something2(i);
		}
	}
};

void do_something_in_current_thread()
{};




// naive parallel version of accumulate
template<typename Iterator, typename T>
struct accumulate_block
{
	void operator() (Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first,last,result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first,last);

	if (!length)
		return init;

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads =
			(length + min_per_thread-1)/min_per_thread;

	unsigned long const hardware_threads =
			std::thread::hardware_concurrency();

	unsigned long const num_threads =
			std::min(hardware_threads!=0?hardware_threads:2,max_threads);

	unsigned long const block_size = length/num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads-1);

	Iterator block_start = first;
	for (unsigned long i = 0; i < num_threads - 1; ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end,block_size);
		threads[i]=std::thread(
		   accumulate_block<Iterator, T>(),
		   block_start, block_end, std::ref(results[i]));
		block_start=block_end;
	}

	accumulate_block<Iterator,T>()(
			block_start,last,results[num_threads-1]);

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(),results.end(),init);
}

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
	std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(),some_list.end(),value_to_find) != some_list.end();

}

class some_data
{
	int a;
	std::string b;
public:
	void do_something() {};

};

class data_wrapper
{
private:
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data * unprotected;

void malicious_function(some_data & protected_data)
{
	unprotected = &protected_data;
}

data_wrapper x;

void foo3()
{
	x.process_data(malicious_function);
	unprotected->do_something();
}

template<typename T,typename Container=std::deque<T> >
class mystack
{
public:
	explicit mystack (const Container &);
	explicit mystack (Container && = Container());
	template <class Alloc> explicit mystack (const Alloc &);
	template <class Alloc> mystack(const Container &, const Alloc &);
    template <class Alloc> mystack(Container &&, const Alloc &);
    template <class Alloc> mystack(mystack&&, const Alloc &);

    bool empty() const;
    size_t size() const;
    T& top();
    T const & top() const;
    void push(T const&);
    void push(T&&);
    void pop();
    void swap(mystack&&);
};

struct empty_stack : std::exception
{
	const char * what() const throw()
	{
	   return "empty stack";
	}
};

template <typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;

public:
	threadsafe_stack() {};
	threadsafe_stack(const threadsafe_stack & other)
	{
	  std::lock_guard<std::mutex> lock(other.m);
	  data = other.data;
	};

	threadsafe_stack& operator= (const threadsafe_stack &) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	};

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		std::shared_ptr<T> const res (std::make_shared<T>(data.top()));
		data.pop();
		return res;
	};

    void pop(T & value)
    {
    	std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    };

    bool empty() const
    {
    	std::lock_guard<std::mutex> lock(m);
    	return data.empty();
    }

};

void do_work_spawn_threads(unsigned int id)
{
    std::cout << "called from thread " << id << std::endl;
};


template<typename Iterator, typename T>
struct accumulate_block2
{
	void operator() (Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first,last,result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate2(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first,last);
    if (!length)
    	return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
    		(length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads =
    		std::thread::hardware_concurrency();

    unsigned long const num_threads =
    		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size =
    		length / num_threads;

    std::vector<T> results(num_threads);

    std::vector<std::thread> threads(num_threads-1);

    Iterator block_start=first;
    for (unsigned long i = 0; i < (num_threads-1); ++i)
    {
    	Iterator block_end = block_start;
    	std::advance(block_end,block_size);
    	threads[i] = std::thread(
    			accumulate_block2<Iterator,T>(),
    			block_start,block_end,std::ref(results[i]));
    	block_start = block_end;
    }
    accumulate_block2<Iterator,T>() (
    		block_start,last,results[num_threads-1]);

    std::for_each(threads.begin(),threads.end(),
    		std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(), results.end(), init);
};


class some_big_object
{
private:
	std::size_t n = 1000000;

public:
	int id;
	std::vector<int> blob;
	some_big_object() : blob(n)
	{

       id = rand() % 1000000 + 1;
       for (unsigned int i = 0; i < n; i++)
    	   blob[i] = rand() % 256;
	}

	some_big_object(some_big_object const & sbo) : blob(sbo.blob)
	{

	   //id = rand() % 1000000 + 1;
         id = sbo.id;
	}
};

void swap(some_big_object & lhs, some_big_object & rhs)
{
    lhs.blob.swap(rhs.blob);
    std::cout << "swapping blobs: lhs id = " << lhs.id << ", rhs id = " << rhs.id << std::endl;
};

class X
{
private:

    mutable std::mutex m;
public:
    X() {};
    X(some_big_object const & sd) : some_detail(sd) {}
    X( const X& x) : some_detail(x.some_detail)
    {}
    some_big_object some_detail;
    friend void myswap(X& lhs, X& rhs);
};

void myswap(X& lhs, X& rhs)
{
	if (&lhs==&rhs)
		return;
	std::lock(lhs.m,rhs.m);
	std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
	std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
	swap(lhs.some_detail,rhs.some_detail);
};

void print_blob(some_big_object & o)
{
	std::cout << std::endl << "---- blob id " << o.id << " data ------ " << std::endl;
    for (unsigned int i = 0; i < 1000; ++i)
    {
	   std::cout << o.blob[i];
	   if (i % 50 == 0 && i > 0)
		   std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
}

void threadsafe_swap_example()
{
	srand(time(NULL));
	X obj1, obj2, obj3, obj4;
	print_blob(obj1.some_detail);
	print_blob(obj2.some_detail);
	print_blob(obj3.some_detail);
	print_blob(obj4.some_detail);
	std::cout << "obj1 id = " << obj1.some_detail.id <<
			     ", obj2 id = " << obj2.some_detail.id <<
			     ", obj3 id = " << obj3.some_detail.id <<
			     ", obj4 id = " << obj4.some_detail.id << std::endl;



	std::vector<X> v;
	v.push_back(obj1);
	v.push_back(obj2);
	v.push_back(obj3);
	v.push_back(obj4);

	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 20; i++)
	{
		threads.push_back(std::thread(myswap,std::ref(v[i % 4]), std::ref(v[(i + 1) % 4])));
	};

	std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));

};

void threadsafe_stack_example()
{
	threadsafe_stack<int> si;
	si.push(5);
	si.pop();
	if (!si.empty())
	{
		int x;
		si.pop();
	}
}

void spawn_threads_example()
{
	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 20; i++)
	{
		threads.push_back(std::thread(do_work_spawn_threads,i));
	};

	std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));
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


void edit_document_example()
{
	edit_document("bar.doc");
	edit_document("foo.doc");
	edit_document("coo.doc");
}


void parallel_accumulate_example()
{
	std::vector<int> vi;
	for (int i = 0; i < 10; ++i)
	{
		vi.push_back(10);
	}

	int sum = parallel_accumulate(vi.begin(), vi.end(), 5);
	std::cout << "sum=" << sum << std::endl;
}

void scoped_thread_example()
{
	int some_local_state;
	scoped_thread t(std::thread(func2(some_local_state)));

	do_something_in_current_thread();
};

void list_with_a_guard_example()
{
	add_to_list(42);
	std::cout << "contains(1)=" << list_contains(1) << ", contains(42)=" << list_contains(42) << std::endl;
};

void protecting_shared_data_example()
{
	foo3();
}



int main() {
	std::cout << "Concurrency in Action examples" << std::endl; // prints Concurrency in Action examples

	//call_a_thread_example();
    //call_a_posix_thread_example();
    //call_many_threads_example();
    //thread_detach_example();
	//edit_document_example();
    //parallel_accumulate_example();
	//scoped_thread_example();
	//protecting_shared_data_example();
    //spawn_threads_example();
    //threadsafe_stack_example();
    threadsafe_swap_example();

	return 0;
}
