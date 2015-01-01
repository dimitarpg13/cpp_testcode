/*
 * SUV.h
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#ifndef SUV_H_
#define SUV_H_

#include "Car.h"

using namespace std;

namespace Algorithms {

class SUV : public Car {
public:
	SUV(string);
	virtual ~SUV();
	bool getThirdRowSeat();
private:
	bool m_bThirdRowSeat;
};

} /* namespace Algorithms */

#endif /* SUV_H_ */
