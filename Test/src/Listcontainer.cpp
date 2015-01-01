/*
 * Listcontainer.cpp
 *
 *  Created on: Oct 4, 2014
 *      Author: root
 */

#include <stdexcept>
#include "Listcontainer.h"

namespace Algorithms {

List_container::List_container() {
	// TODO Auto-generated constructor stub

}

List_container::~List_container() {
	// TODO Auto-generated destructor stub
}

double& List_container::operator[] (int i)
{
	for (auto& x : ld)
	{
		if (i == 0)
			return x;
		--i;
	}
	throw out_of_range("List_container");
}

} /* namespace Algorithms */
