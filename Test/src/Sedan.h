/*
 * Sedan.h
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#ifndef SEDAN_H_
#define SEDAN_H_

#include "Car.h"

namespace Algorithms {

class Sedan : public Car {
public:
	Sedan(string);
	virtual ~Sedan();
	bool getSportPackage();
private:
	bool m_bSportPackage;
};

} /* namespace Algorithms */

#endif /* SEDAN_H_ */
