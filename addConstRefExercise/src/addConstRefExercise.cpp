//============================================================================
// Name        : addConstRefExercise.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <type_traits>

using namespace std;


template<class T>
struct add_const_ref
{
   typedef T const& type;
};

template<class T>
struct add_const_ref<T &>
{
   typedef T & type;
};







int main() {
   cout << "Template Metaprogramming Exercise 2.0 : add_const_reference" << endl; // prints Template Metaprogramming Exercise 2.0 : add_const_reference

   typedef add_const_ref<int &>::type t1;


   cout << boolalpha;


   cout << is_same<t1, int &>::value << endl;

   return 0;
}
