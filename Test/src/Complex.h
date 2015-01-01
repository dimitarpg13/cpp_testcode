/*
 * Complex.h
 *
 *  Created on: Oct 3, 2014
 *      Author: root
 */

#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <vector>
#include <initializer_list>

using namespace std;

namespace Algorithms {

class Complex {
public:
	Complex(double r, double i) : re{r}, im{i} {};
    Complex(double r) : re{r}, im{0} {};
    Complex() : re{0}, im{0} {};
//    Complex(initializer_list<double> il)
//    {
//
//    }

    double real() const { return re; };
    void real(double d) { re=d; }
    double imag() const { return im; };
    void imag(double d) { im=d; };

    Complex& operator+= (Complex z) {
    	re+=z.re;
    	im+=z.im;
    	return *this;
    }

    Complex& operator-=(Complex z) {
    	re-=z.re, im-=z.im; return *this;
    }

    Complex& operator *= (Complex);

    Complex& operator /= (Complex);

	virtual ~Complex();
private:
	double re, im;
};

} /* namespace Algorithms */

#endif /* COMPLEX_H_ */
