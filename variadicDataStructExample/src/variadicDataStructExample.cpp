//============================================================================
// Name        : variadicDataStructExample.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

using namespace std;

template <class... Ts> struct mytuple {};

template <class T, class... Ts>
struct mytuple<T, Ts...> : mytuple<Ts...> {
	mytuple(T t, Ts... ts) : mytuple<Ts...>(ts...), tail(t)  {}

	T tail;
};

template <size_t,class,class...>
struct elem_type_holder;

template <class T, class... Ts>
struct elem_type_holder<0, mytuple<T, Ts...>> {
	typedef T type;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, mytuple<T, Ts...>> {
	typedef typename elem_type_holder<k-1, mytuple<Ts...>>::type type;
};

template <size_t k, class... Ts>
typename enable_if<
	k == 0, typename elem_type_holder<0, mytuple<Ts...>>::type&>::type
get(mytuple<Ts...>& t) {
	return t.tail;
}

template <size_t k, class T, class... Ts>
typename enable_if<
	k != 0, typename elem_type_holder<k, mytuple<T, Ts...>>::type&>::type
get(mytuple<T, Ts...>& t)
{
	mytuple<Ts...>& base = t;
	return get<k-1>(base);
}


int main() {
	cout << "variadic data structure example from Eli Bendersky's web site" << endl; // prints variadic data structure example from Eli Bendersky's web site
	mytuple<double, int, const char*> t1(12.2, 42, "big");

    cout << "0th elem is " << get<0>(t1) << endl;
    cout << "1th elem is " << get<1>(t1) << endl;
    cout << "2nd elem is " << get<2>(t1) << endl;

    get<1>(t1) = 103;
    cout << "1th elem is " << get<1>(t1) << endl;

	return 0;
}
