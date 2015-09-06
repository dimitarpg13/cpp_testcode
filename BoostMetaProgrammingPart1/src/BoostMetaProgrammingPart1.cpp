//============================================================================
// Name        : BoostMetaProgrammingPart1.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <typeinfo>


using namespace std;

template < unsigned n >
struct factorial
{
	static const unsigned value = n * factorial<n-1>::value;
};

template<>
struct factorial<0>
{
	static const unsigned value = 1;
};

// permutation_holder<T>::type is an array type which can contain
// all permutations of a given T

// unspecified template for scalars
template< typename T >
struct permutation_holder
{
	typedef T type[1][1];
};

// specialization for array types
template < typename T, unsigned N >
struct permutation_holder<T[N]>
{
	typedef T type[factorial<N>::value][N];
};

template< typename First, typename Rest >
struct cons
{
	typedef First first;
	typedef Rest rest;
};

struct nil {};
typedef cons < int, cons <long, cons < std::vector<double>, nil > > > my_types;

// choose the larger of two types
template<
		typename T1,
		typename T2,
		bool choose1 = (sizeof(T1) > sizeof(T2)) // hands off
		>
struct choose_larger
{
	typedef T1 type;
};

// specialization for the case where sizeof(T2) >= sizeof(T1)
template< typename T1, typename T2 >
struct choose_larger< T1,T2,false >
{
	typedef T2 type;
};

// get the largest of a cons-list
template< typename T > struct largest;

// specialization to peel apart the cons list
template< typename First, typename Rest >
struct largest< cons<First,Rest> >
		: choose_larger< First, typename largest<Rest>::type >
{

};

// specialization for loop termination
template< typename First >
struct largest< cons<First,nil> >
{
	typedef First type;
};



int main() {
	cout << "Boost : Template Metaprogramming Part 1" << endl; // prints Boost : Template Metaprogramming Part 1

	// print the name of the largest of my_types
	std::cout << typeid(largest<my_types>::type).name()
				<< std::endl;

	std::cout << "single type test" << std::endl;
	//std::cout << typeid(largest<int>::type).name()
	//			<< std::endl;

	return 0;
}
