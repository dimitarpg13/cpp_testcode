//============================================================================
// Name        : BoostTest.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//#include <boost/accumulators/accumulators.hpp>
//#include <boost/accumulators/statistics/stats.hpp>
//#include <boost/accumulators/statistics/mean.hpp>
//#include <boost/accumulators/statistics/moment.hpp>
//using namespace boost::accumulators;
//
//using namespace std;
//
//int main()
//{
//    // Define an accumulator set for calculating the mean and the
//    // 2nd moment ...
//    accumulator_set<double, stats<tag::mean, tag::moment<2> > > acc;
//
//    // push in some data ...
//    acc(1.2);
//    acc(2.3);
//    acc(3.4);
//    acc(4.5);
//
//    // Display the results ...
//    std::cout << "Mean:   " << mean(acc) << std::endl;
//    std::cout << "Moment: " << moment<2>(acc) << std::endl;
//
//    return 0;
//}

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

boost::mutex io_mutex; // The iostreams are not guaranteed to be thread-safe!

class counter
{
public:
    counter() : count(0) { }

    int increment() {
        boost::mutex::scoped_lock scoped_lock(mutex);
        return ++count;
    }

private:
    boost::mutex mutex;
    int count;
};

counter c;

void change_count()
{
    int i = c.increment();
    boost::mutex::scoped_lock scoped_lock(io_mutex);
    std::cout << "count == " << i << std::endl;
}

int main(int, char*[])
{
    const int num_threads = 4;
    boost::thread_group thrds;
    for (int i=0; i < num_threads; ++i)
        thrds.create_thread(&change_count);

    thrds.join_all();

    return 0;
}
