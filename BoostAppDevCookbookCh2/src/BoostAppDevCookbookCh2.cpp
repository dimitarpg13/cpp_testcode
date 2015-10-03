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

#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iostream>
#include <stdlib.h>



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

int main() {
	std::cout << "Boost C++ Application Development Cookbook Chapter 2" << std::endl; // prints Boost C++ Application Development Cookbook

	boost_any_example();
    boost_variant_example();
    boost_optional_example();
    boost_array_example();

	return 0;
}
