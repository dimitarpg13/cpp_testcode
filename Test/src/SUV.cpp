/*
 * SUV.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#include "SUV.h"

namespace Algorithms {

SUV::SUV(string brand) : Car(brand) {

}

SUV::~SUV() {
	// TODO
}

bool SUV::getThirdRowSeat()
{
	return m_bThirdRowSeat;
}

} /* namespace Algorithms */
