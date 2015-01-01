/*
 * Vector.h
 *
 *  Created on: Oct 3, 2014
 *      Author: root
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdexcept>
#include <initializer_list>

using namespace std;

namespace Algorithms {

class Vector {
public:
	Vector(int s);
	Vector(initializer_list<double>);
	Vector(const Vector&);
	Vector(Vector&&);
	double& operator[] (int i);
	Vector& operator= (const Vector&);
	Vector& operator= (Vector&&);
	int size();
	virtual ~Vector();
private:
	double * elem; // elem points to array of size double
	int sz;

};



} /* namespace Algorithms */

#endif /* VECTOR_H_ */
