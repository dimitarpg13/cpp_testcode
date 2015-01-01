/*
 * Vector.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: root
 */
#include "Vector.h"

using namespace std;

namespace Algorithms {

Vector::Vector(int s) {

	if (s < 0)
			throw std::length_error{"Incorrect size in Vector::Vector(int)"};
	elem = new double[s];
	sz = s;

	for (int i = 0; i < s; i++)
		elem[i] = 0;

}

Vector::Vector(initializer_list<double> lst) : elem{new double[lst.size()]},
		sz{static_cast<int>(lst.size())}
{
    copy(lst.begin(),lst.end(),elem);
}

Vector::Vector(const Vector& a) : elem{new double[sz]}, sz{a.sz}
{
	for (int i=0; i!=sz; ++i)
	{
		elem[i]=a.elem[i];
	}
}



Vector& Vector::operator =(const Vector& a)
{
	double * p = new double[a.sz];
	for (int i = 0; i!=a.sz; ++i)
		p[i] = a.elem[i];
	delete[] elem;

	elem = p;
	sz = a.sz;
	return *this;
}



Vector::~Vector() {
	// TODO Auto-generated destructor stub
}


double& Vector::operator[](int i)
{
   if (i < 0 || size() <= i)
	   throw std::out_of_range{"Vector::operator[]"};
	return elem[i];
}

int Vector::size()
{

	return sz;
}

/*template<typename T>
VectorT<T>::VectorT(int s)
{
	if (s < 0)
		throw std::length_error{"Incorrect size in VectorT::VectorT(int)"};

	elem = new T[s];
	sz = s;
}


template<typename T>
T& VectorT<T>::operator[] (int i)
{
   if (i < 0 || i >= sz)
	   throw std::out_of_range{"VectorT::operator[]"};
   return elem[i];
}

template<typename T>
T& VectorT<T>::operator[] (int i) const
{
   if (i < 0 || i >= sz)
	   throw std::out_of_range{"VectorT::operator[]"};
   return elem[i];
}

template<typename T>
T* VectorT<T>::begin(VectorT<T>& x)
{
	return x.size() ? &x[0] : nullptr;
}

template<typename T>
T* VectorT<T>::end(VectorT<T>& x)
{
	return begin(*this) + x.size();
}*/


} /* namespace Algorithms */
