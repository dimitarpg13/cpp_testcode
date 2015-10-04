//============================================================================
// Name        : BoostAppDevCookbook.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp> // combining multiple values into one
#include <boost/tuple/tuple_comparison.hpp> // making tuples and comparing them
#include <boost/bind.hpp>


#include <vector>
#include <set>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <functional> // for the boost::bind example
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds



//these typedefs and methods will be in our header
//that wraps around native SQL iface
typedef boost::any cell_t_any;
typedef std::vector<cell_t_any> db_row_t_any;

typedef boost::variant<int, float, std::string> cell_t_var;
typedef std::vector<cell_t_var> db_row_t_var;


//this is just an example , no actual work with the database
db_row_t_any get_row_any(const char * /*query*/)
{
	// in real application 'query' parameter shall have a 'const
	// char* or 'const std::string&' type ?
	// see recipe Using a reference to string type in Chapter 7, Manipulating Strings
	// for an answer
    db_row_t_any row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back(std::string("hello again"));
    return row;
}

//this is just an example , no actual work with the database
db_row_t_var get_row_var(const char * /*query*/)
{
	// in real application 'query' parameter shall have a 'const
	// char* or 'const std::string&' type ?
	// see recipe Using a reference to string type in Chapter 7, Manipulating Strings
	// for an answer
    db_row_t_var row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back(std::string("hello again"));
    return row;
}


// this is how the user will use the classes
struct db_sum_any : public std::unary_function<boost::any, void>
{
private:
	double & sum_;
public:
	explicit db_sum_any(double & sum) : sum_(sum)
	{}

	void operator() (const cell_t_any & value)
	{
		const std::type_info & ti =
				value.type();
		if (ti == typeid(int))
		{
			sum_ += boost::any_cast<int>(value);
		}
		else if (ti == typeid(float))
		{
			sum_ += boost::any_cast<float>(value);
		}

	}
};

// this is how code required to sum values
// we can provide no template parameter
// to boost::static_visitor<> if our visitor
// returns nothing.
struct db_sum_visitor : public boost::static_visitor<double>
{
	double operator() (int value) const
	{
		return value;
	}

	double operator() (float value) const
	{
		return value;
	}

	double operator() (const std::string & /*value*/) const
	{
		return 0.0;
	}
};

class locked_device {
	explicit locked_device(const char * /*param*/)
	{
		// we have unique access to device
		std::cout << "Device is locked" << std::endl;
	}
public:
	~locked_device() {
		// releasing device lock
	}

	void use()
	{
		std::cout << "Success!" << std::endl;
	}

	static boost::optional<locked_device> try_lock_device()
	{
	   if (rand() % 2)
	   {
		   // failed to lock device
		   return boost::none;
	   }
	   // success!
	   return locked_device("device name");
	}
};

// functional object to increment array values by one
struct add_1 : public std::unary_function<char, void> {
	void operator() (char & c) const {
		++ c;
	}
};


// part of the returning an array from function example
typedef boost::array<char, 4> array4_t;
array4_t & vector_advance(array4_t & val)
{
	//array4_t res;
	//for (int i=0; i<4; i++)
	//   res[i] = val[i]+2;
    std::for_each(val.begin(), val.end(), add_1());
	return val;
};

class Number
{
public:
	//Number() {};
	Number(int val) : m_iVal(val) {};
	int m_iVal;
};

inline Number operator + (Number n1, Number n2)
{
	return Number(n1.m_iVal + n2.m_iVal);
}

struct mul_2_func_obj :
		public std::unary_function<Number, Number>
{
	Number operator() (Number n1) const {
		return n1 + n1;
	}
};

struct printer_obj
{
	void operator() (Number& n) const {
		std::cout << n.m_iVal << ", ";
	}
};


void mul_2_impl1(const std::vector<Number> & values)
{
	std::for_each(values.begin(), values.end(), mul_2_func_obj());

};


void mul_2_impl2(const std::vector<Number> & values)
{
	std::for_each(values.begin(), values.end(), boost::bind(std::plus<Number>(),_1,_1));
};


template <class T>
void mul_2_impl3(const std::vector<T> & values)
{
	std::for_each(values.begin(), values.end(), boost::bind(std::plus<T>(), _1, _1));
};


class Device1
{
private:
	short temperature() { return 12; };
	short wetness() { return 34; };
	int illumination() { return 100; };
	int atmospheric_pressure() { return 56; };
	void wait_for_data() { std::this_thread::sleep_for (std::chrono::seconds(1)); };


public:
	template<class FuncT>
	void watch(const FuncT & f)
	{


		while (1)
		{
			wait_for_data();
			f(
			   temperature(),
			   wetness(),
			   illumination(),
			   atmospheric_pressure()
			);
		}
	}
};


class Device2
{
private:
	short temperature() { return 43; };
	short wetness() { return 88; };
	int illumination() { return 101; };
    int atmospheric_pressure() { return 66; };
    void wait_for_data() { std::this_thread::sleep_for (std::chrono::milliseconds(100)); };
public:
    template <class FuncT>
    void watch(const FuncT& f)
    {
        while (1)
    	{
    		wait_for_data();
    		f
    		(
    		   wetness(),
    		   temperature(),
    		   atmospheric_pressure(),
    		   illumination()
    		);
    	}
    }
};

void detect_storm(int wetness, int temperature, int atmospheric_pressure)
{
	std::cout << "detect_storm called with wetness = " << wetness
			<< ", temperature = " << temperature
			<< ", atmospheric_pressure = " << atmospheric_pressure << std::endl;
};


void boost_any_example()
{
	std::cout << "boost_any_example:" << std::endl;
	db_row_t_any row = get_row_any("Query: Give me some row please.");
	double res = 0.0;
	std::for_each(row.begin(), row.end(), db_sum_any(res));
	std::cout << "Sum of arithmetic types in database row is: " << res << std::endl;
};


void boost_variant_example()
{
	std::cout << "boost_variant_example:" << std::endl;
	db_row_t_var row = get_row_var("Query: Give me some row, Please.");
	double res = 0.0;
	db_row_t_var::const_iterator it = row.begin(), end = row.end();
	for (; it != end; ++it)
	{
		res +=
		   boost::apply_visitor(db_sum_visitor(), *it);
	}

	std::cout << "Sum of the arithmetic types in database row is :" << res << std::endl;
};

int boost_optional_example()
{
	std::cout << "boost_optional_example:" << std::endl;
	// boost has a library called Random. if you wonder why it was written when stdlib has
	// rand() function, see recipe "using a true random generator" in Ch 12
	// Scratching the tip of the iceberg
    srandom(5);
    for (unsigned i = 0;  i < 10; ++i)
    {
    	boost::optional<locked_device> t = locked_device::try_lock_device();
    	// optional is convertible to bool
    	if (t) {
    		t->use();
    		return 0;
    	}
    	else
    	{
    		std::cout << "... trying again" << std::endl;
    	}
    }

    std::cout << "Failure!" << std::endl;
    return -1;
};



void boost_array_example()
{
   std::cout << "boost_array_example:" << std::endl;
   array4_t val;
   val[0]=41; val[1]=42; val[2]=43; val[3]=44;

   for (int i=0; i < 4; i++)
	   std::cout << val[i] << " ";

   array4_t res = vector_advance(val);
   std::cout << std::endl;
   for (int i=0; i < 4; i++)
	   std::cout << res[i] << " ";


};

void boost_multiple_values_example()
{
	std::cout << "boost_multiple_values_example:" << std::endl;
	boost::tuple<int,std::string> almost_a_pair(10, "Hello");
    boost::tuple<int, float, double, int> quad(10, 1.0f, 10.1, 1);
    int i = boost::get<0>(almost_a_pair);
    const std::string & str = boost::get<1>(almost_a_pair);
    double d = boost::get<2>(quad);
    std::cout << "boost::get<0>(...) = " << i << std::endl;
    std::cout << "boost::get<1>(...) = " << d << std::endl;
};

void boost_tuple_comparison_example()
{
	std::cout << "boost_tuple_comparison_example:" << std::endl;
	std::set<boost::tuple<int, double, int> > s;
    s.insert(boost::make_tuple(1, 1.0, 2));
    s.insert(boost::make_tuple(2, 10.0, 2));
    s.insert(boost::make_tuple(3, 100.0, 2));

    auto t = boost::make_tuple(0, -1.0, 2);
    assert(2 == boost::get<2>(t));

};

void boost_tie_example()
{
	std::cout << "boost_tie_example:" << std::endl;
	boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);
    int i;
    float f;
    double d;
    int i2;

    // passing values from 'quad' variables to variables 'i', 'f', 'd', 'i2'
    boost::tie(i,f, d, i2) = quad;
    assert(i == 10);
    assert(i2 == 1);
};


void boost_mul_using_func_obj_example()
{
	std::cout << "boost_mul_using_func_obj:" << std::endl;
	std::vector<Number> v;
    for (int i=0; i < 4; i++)
    {
      v.push_back(Number(i));
    }

    mul_2_impl1(v);

    std::for_each(v.begin(), v.end(), printer_obj());

};

void boost_argument_order_example()
{
   Device1 d1;
   // resulting functional object will silently ignore
   // additional parameters passed to the function call
   d1.watch(boost::bind(&detect_storm, _2, _1, _4));

   Device2 d2;

   d2.watch(boost::bind(&detect_storm, _1, _2, _3));

};


int main() {
	std::cout << "Boost C++ Application Development Cookbook Chapter 2" << std::endl; // prints Boost C++ Application Development Cookbook

	boost_any_example();
    boost_variant_example();
    boost_optional_example();
    boost_array_example();
    boost_multiple_values_example();
    boost_tuple_comparison_example();
    boost_tie_example();
    boost_mul_using_func_obj_example();
    boost_argument_order_example();

	return 0;
}