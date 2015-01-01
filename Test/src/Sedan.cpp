/*
 * Sedan.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#include "Sedan.h"

namespace Algorithms {

Sedan::Sedan(string brand) : Car(brand) {
	// TODO Auto-generated constructor stub

}

Sedan::~Sedan() {
	// TODO Auto-generated destructor stub
}

bool Sedan::getSportPackage()
{
	return m_bSportPackage;
}

} /* namespace Algorithms */
