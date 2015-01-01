/*
 * VectorT.h
 *
 *  Created on: Oct 6, 2014
 *      Author: root
 */

#ifndef VECTORT_H_
#define VECTORT_H_

#include <stdexcept>
#include <initializer_list>


namespace Algorithms {

template <typename T>
class VectorT {
private:
	T* elem; // elem points to an array of sz elements of type T
	int sz;
public:
	VectorT(int s)
    {
		if (s < 0)
			throw std::length_error{"Incorrect size in VectorT::VectorT(int)"};

		elem = new T[s];
		sz = s;
    }

	~VectorT() { delete[] elem; }
    //T& operator[] (int i);
    T& operator[] (int i) const
    {
    	if (i < 0 || i >= sz)
    		   throw std::out_of_range{"VectorT::operator[]"};
    	   return elem[i];
    };

    int size() const { return sz; }
    T* begin(VectorT<T>& x)
    {
    	return x.size() ? &x[0] : nullptr;
    }

    T* end(VectorT<T>& x)
    {
    	return begin(*this) + x.size();
    }
};

} /* namespace Algorithms */

#endif /* VECTORT_H_ */
