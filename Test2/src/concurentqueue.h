/*
 * concurentqueue.h
 *
 *  Created on: Oct 19, 2014
 *      Author: root
 */

#ifndef CONCURENTQUEUE_H_
#define CONCURENTQUEUE_H_

#include <iostream>
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>


using namespace std;


namespace Algorithms {

template<class D> class concurent_queue {
private:
	queue<D> the_queue;
	mutable boost::mutex the_mutex;
public:
	concurent_queue() {};
	virtual ~concurent_queue() {};
	void f();
};

} /* namespace Algorithms */

#endif /* CONCURENTQUEUE_H_ */
