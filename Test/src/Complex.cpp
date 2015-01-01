/*
 * Complex.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: root
 */

#include "Complex.h"

namespace Algorithms {


Complex::~Complex() {
	// TODO Auto-generated destructor stub
}

Complex& Complex::operator *= (Complex a)
{
	double newRe = this->re * a.re - this->im * a.im ;
	double newIm = this->re * a.im + this->im * a.re ;
	this->re = newRe;
	this->im = newIm;
    return *this;
}


Complex& Complex::operator /= (Complex a)
{
	double newRe =((this->re * a.re) + (this->im * a.im)) / (( a.re * a.re )+( a.im * a.im ));
	double newIm  =((this->im * a.re) - (this->re * a.im)) / ((a.re * a.re)+(a.im * a.im));
	this->re = newRe;
	this->im = newIm;
	return *this;
}


} /* namespace Algorithms */
