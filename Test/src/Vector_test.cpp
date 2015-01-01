/*
 * Vector_test.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: root
 */

#include "Vector.h"

namespace Algorithms {
   void Vector_test()
   {
	   try
	   {
		   Vector(-27);
	   }
	   catch (std::length_error& e)
	   {
		   // handle negative size
	   }
	   catch (std::bad_alloc& e)
	   {
		   // handle out of memory exception
	   }
   }
} /* namespace Algorithms */
