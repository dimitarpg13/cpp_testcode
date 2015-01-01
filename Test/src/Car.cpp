/*
 * Car.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#include "Car.h"

namespace Algorithms {

Car::Car(string brand) : m_sBrand (brand) {}


Car::~Car() {

}

int Car::getPassengerCount()
{
	return m_iPassengerCount;
}

void Car::setPassengerCount(int val)
{
	m_iPassengerCount = val;
}

string Car::getLicensePlate()
{
	return m_sLicensePlate;
}

void Car::setLicensePlate(string val)
{
	m_sLicensePlate = val;
}

string Car::getBrand()
{
	return m_sBrand;
}

} /* namespace Algorithms */
