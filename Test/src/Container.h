/*
 * Container.h
 *
 *  Created on: Oct 4, 2014
 *      Author: root
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_

namespace Algorithms {

class Container {
public:

	virtual double& operator[](int) = 0; // pure virtual function
	virtual int size() const = 0; // const member function
	virtual ~Container() {};
};

} /* namespace Algorithms */

#endif /* CONTAINER_H_ */
