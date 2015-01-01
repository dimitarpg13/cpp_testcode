/*
 * Vectorcontainer.h
 *
 *  Created on: Oct 4, 2014
 *      Author: root
 */

#ifndef VECTORCONTAINER_H_
#define VECTORCONTAINER_H_

#include "Container.h"
#include "Vector.h"

namespace Algorithms {

class Vector_container : public Container {
public:
	Vector_container(int s) : v(s) {};
	double& operator[] (int i) { return v[i]; };
	int size() const { return this->v.size(); };
    ~Vector_container();
private:
	Vector v;
};

} /* namespace Algorithms */

#endif /* VECTORCONTAINER_H_ */
