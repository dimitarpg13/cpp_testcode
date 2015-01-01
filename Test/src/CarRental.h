/*
 * CarRental.h
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#ifndef CARRENTAL_H_
#define CARRENTAL_H_

#include <vector>
#include "Car.h"

namespace Algorithms {

class CarRental {
public:
	CarRental();

	vector<Car*>& getCars();

private:
	vector<Car*> m_vCars;

};

} /* namespace Algorithms */

#endif /* CARRENTAL_H_ */
