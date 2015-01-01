/*
 * String.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: root
 */

#include "String.h"

namespace Algorithms {


//template<typename C> String<C>::String() {
//	// TODO Auto-generated constructor stub
//
//}
//
//template<typename C> String<C>::~String() {
//	// TODO Auto-generated destructor stub
//}

//template<typename C> String<C>::String() : sz{0}, ptr{ch}
//{
//		ch[0] = {}; // terminating 0 of the appropriate character type
//};

template<typename C>
String<C>& String<C>::operator+=(C c)
{
	return *this;
}

} /* namespace Algorithms */
