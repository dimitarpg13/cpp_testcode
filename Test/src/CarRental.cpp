/*
 * CarRental.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#include "CarRental.h"

namespace Algorithms {
  CarRental::CarRental() {};
  vector<Car*>& CarRental::getCars()
  {
	  return m_vCars;
  }

} /* namespace Algorithms */
