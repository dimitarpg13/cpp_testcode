//============================================================================
// Name        : templBoostDimensions.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include <assert.h>
//#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace std;

namespace mpl = boost::mpl; // namespace alias


typedef int dimension[7]; // m l t ...
dimension const arr_mass = { 1, 0, 0, 0, 0, 0, 0 };
dimension const arr_length = { 0, 1, 0, 0, 0, 0, 0 };
dimension const arr_time = { 0, 0, 1, 0, 0, 0, 0 };
dimension const arr_force = { 1, 1, -2, 0, 0, 0, 0 };

typedef boost::mpl::vector<signed char, short, int, long> signed_types;

static int const five = mpl::int_<5>::value;

typedef mpl::vector_c<int,1,0,0,0,0,0,0> v_mass;
typedef mpl::vector_c<int,0,1,0,0,0,0,0> v_length; // position
typedef mpl::vector_c<int,0,0,1,0,0,0,0> v_time;
typedef mpl::vector_c<int,0,0,0,1,0,0,0> v_charge;
typedef mpl::vector_c<int,0,0,0,0,1,0,0> v_temperature;
typedef mpl::vector_c<int,0,0,0,0,0,1,0> v_intensity;
typedef mpl::vector_c<int,0,0,0,0,0,0,1> v_amount; // amount of substance

typedef mpl::integral_c<short,8> eight;

bool fun()
{
   return true;
}

int main() {
	cout << "Dimensions chapter examples of templates metaprogramming book" << endl; // prints Dimensions chapter examples of templates metaprogramming book
	cout << five << endl;
	//BOOST_MPL_ASSERT(( is_same< eight::value_type, short > ));
	//BOOST_MPL_ASSERT (( std::is_same< eight::value_type, short > ));

	//boost::assert( ( is_same<short,short>::value )  );
	BOOST_MPL_ASSERT ( true  );
    //boost::assert( eight() == 8 );
	return 0;
}
