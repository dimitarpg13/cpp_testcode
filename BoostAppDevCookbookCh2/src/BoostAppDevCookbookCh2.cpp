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
#include <boost/ref.hpp>
#include <boost/swap.hpp> // cpp11_move_emulation_example
#include <boost/container/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp> // for numeric-to-numeric conversions
#include <boost/cast.hpp> // for polymorphic cast example
#include <boost/spirit/include/qi.hpp> // for parsing files with Boost.Spirit lib
#include <boost/spirit/include/phoenix_core.hpp> // for parsing files with Boost.Spirit lib
#include <boost/spirit/include/phoenix_operator.hpp> // for parsing files with Boost.Spirit lib



#include <vector>
#include <set>
#include <deque>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <functional> // for the boost::bind example
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <memory>
#include <random>
#include <locale>  // used in the lexical_cast example
#include <iterator>
#include <iosfwd>  // for user-defined types conversions
#include <stdexcept> // for user-defined types conversions


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

void f (int n1, int n2, int n3, const int & n4, int n5)
{
	std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << std::endl;
}

int g (int n1)
{
	return n1;
}

struct Foo
{
	void print_sum(int n1, int n2)
	{
		std::cout << n1+n2 << std::endl;
	}
	int data = 10;
};

int f1(int a, int b)
{
	return a + b;
};

int g1(int a, int b, int c)
{
	return a + b + c;
};

// cpp11_move_emulation_example
namespace other
{
   // its default characteristics is cheap/fast class characteristics{};
   class characteristics
   {

   };

};

// cpp11_move_emulation_example
struct person_info {

	bool is_male_;
	std::string name_;
	std::string second_name_;
	other::characteristics characteristic_;


private:
	BOOST_COPYABLE_AND_MOVABLE(person_info)



public:
	// for the simplicity of the example we will assume that person_info default
    // constructor and swap are very fast/cheap to call
    person_info() {}

    person_info(const person_info & p) : is_male_(p.is_male_),
    									 name_(p.name_),
    									 second_name_(p.second_name_),
    									 characteristic_(p.characteristic_)
    {

    }

    person_info(BOOST_RV_REF(person_info) person)
    {
    	swap(person);
    }

    person_info & operator= (BOOST_COPY_ASSIGN_REF(person_info) person)
    {
    	if (this != &person)
    	{
    		person_info tmp(person);
    		swap(tmp);
    	}
    	return *this;
    }

    person_info& operator= (BOOST_RV_REF(person_info) person)
    {
    	if (this != &person)
    	{
    		swap(person);
    		person_info tmp;
    		tmp.swap(person);
    	}
    	return *this;
    }


	void swap(person_info & rhs)
	{
		std::swap(is_male_, rhs.is_male_);
		name_.swap(rhs.name_);
		second_name_.swap(rhs.second_name_);
		boost::swap(characteristic_, rhs.characteristic_);
	};


};

// this class is copyable and this create resource deallocation problem
// after copying an instance of descriptor_owner to another local instance
class descriptor_owner
{
	void * descriptor_;

public:
	explicit descriptor_owner(const char * params);
    ~descriptor_owner() {
    	//system_api_free_descriptor(descriptor_);
    };
};

// by inheriting from boost::noncopyable we preclude the user
// from doing bad things with this class
class descriptor_owner_fixed : private boost::noncopyable
{
	void * descriptor_;

public:
	explicit descriptor_owner_fixed(const char * params);
    ~descriptor_owner_fixed() {
    	//system_api_free_descriptor(descriptor_);
    };
};

// this will only work on C++11 compatible compilers
class descriptor_owner1
{
    void * descriptor_;

public:
    descriptor_owner1() : descriptor_(NULL) {};
    explicit descriptor_owner1(const char * param) : descriptor_(strdup(param))
    {
    }

    descriptor_owner1(descriptor_owner1 && param) : descriptor_(param.descriptor_)
    {
    	param.descriptor_ = NULL;
    }

    descriptor_owner1 & operator= (descriptor_owner1 && param)
    {
    	clear();
    	std::swap(descriptor_, param.descriptor_);
    	return *this;
    }

    void clear()
    {
    	free(descriptor_);
    	descriptor_ = NULL;
    }

    bool empty() const
    {
    	return !descriptor_;
    }

    ~descriptor_owner1()
    {
    	clear();
    }

};



// gcc compiles the following with std=c++0x
descriptor_owner1 construct_descriptor2()
{
	return descriptor_owner1("Construct using this string");
};

void foo_rv()
{
	std::cout << "C++11" << std::endl;
	descriptor_owner1 desc;
	desc = construct_descriptor2();
	assert(!desc.empty());
};



class descriptor_owner_movable
{
private:
    void * descriptor_;

    BOOST_MOVABLE_BUT_NOT_COPYABLE(descriptor_owner_movable);

public:
    descriptor_owner_movable() : descriptor_(NULL) {};
    explicit descriptor_owner_movable(const char * param) : descriptor_(strdup(param))
    {
    }

    descriptor_owner_movable(descriptor_owner_movable && param) : descriptor_(param.descriptor_)
    {
    	param.descriptor_ = NULL;
    }

    descriptor_owner_movable & operator= (descriptor_owner_movable && param)
    {
    	clear();
    	std::swap(descriptor_, param.descriptor_);
    	return *this;
    }

    void clear()
    {
    	free(descriptor_);
    	descriptor_ = NULL;
    }

    bool empty() const
    {
    	return !descriptor_;
    }

    ~descriptor_owner_movable()
    {
    	clear();
    }

};


descriptor_owner_movable construct_descriptor3()
{
	return descriptor_owner_movable("Construct using this string");
};

template<class T> void swap(T& a, T& b)
{
	T tmp(boost::move(a));
	a = boost::move(b);
	b = boost::move(tmp);
};

class move_test
{
private:
	int val;

public:
	move_test(int newVal) : val(newVal) {};
	int getVal()  { return val; };
};

template<class ContainerT>
std::vector<long int> container_to_longs(const ContainerT& container)
{
	typedef typename ContainerT::value_type value_type;
	std::vector<long int> ret;
	typedef long int (*func_t) (const value_type &);
    func_t f = &boost::lexical_cast<long int, value_type>;
    std::transform(container.begin(), container.end(), std::back_inserter(ret), f);

	return ret;
}

void some_function(unsigned short param)
{

}

bool some_extremely_rare_condition()
{
	bool res = false;

	return res;

}

bool another_extremely_rare_condition()
{
	bool res = false;

	return res;
}


int foo()
{
	if (some_extremely_rare_condition())
		return -1;
	else if (another_extremely_rare_condition())
		return 1000000;

	return 65535;
}

void numeric_conversion_correct_implementation()
{
	some_function(boost::numeric_cast<unsigned short>(foo()));
}

template<class SourceT, class TargetT>
struct mythrow_overflow_handler
{
	void operator()(boost::numeric::range_check_result r)
	{
	   if (r != boost::numeric::cInRange)
	   {
		   throw std::logic_error("Not in range");
	   }
	}
};

template<class TargetT, class SourceT>
TargetT my_numeric_cast(const SourceT& in)
{
	using namespace boost;
	typedef numeric::conversion_traits<TargetT, SourceT> conv_traits;
	typedef numeric::numeric_cast_traits<TargetT, SourceT> cast_traits;
	typedef boost::numeric::converter
			<
			  TargetT,SourceT,conv_traits,mythrow_overflow_handler<SourceT,TargetT>
	        > converter;
	return converter::convert(in);

};

//negative number that does not store minus sign
class negative_number
{
	unsigned short number_;
public:
	explicit negative_number(unsigned short number) : number_(number)
	{
	}

	negative_number() : number_(1) {};

    unsigned short value_without_sign() const
    {
    	return number_;
    }
};

std::ostream& operator<< (std::ostream& os, const negative_number& num)
{
	os << '-'  << num.value_without_sign();
	return os;
};

std::istream& operator>> (std::istream& is, negative_number& num)
{
	char ch;
	is >> ch;
	if (ch != '-')
	{
		throw std::logic_error("negative_number class designed to store only negative values");
	}

	unsigned short s;
    is >> s;
	num = negative_number(s);
	return is;

};

template<class CharT>
std::basic_ostream<CharT>&
operator<<(std::basic_ostream<CharT>& os,
		const negative_number & num)
{
	os << static_cast<CharT>('-') << num.value_without_sign() ;
	return os;
};

template<class CharT>
std::basic_istream<CharT>& operator>>
(std::basic_istream<CharT>& is,
		negative_number& num)
{
	CharT ch;
	is >> ch;
	if (ch != static_cast<CharT>('-'))
	{
		throw std::logic_error("negative_number is designed to store only negative values");
	}
    unsigned short s;
    is >> s;
    num = negative_number(s);
    return is;

}


struct object
{
	virtual ~object() {};
};


struct banana : public object
{
	void eat() const {}
	virtual ~banana() {}
};

struct pidgin : public object
{
	void fly() const {}
	virtual ~pidgin() {}
};

object * try_produce_banana()
{
	return new banana();
};

void try_eat_banana_impl1()
{
	const object  * obj = try_produce_banana();
	if (!obj)
	{
		throw std::bad_cast();
	}
	dynamic_cast<const banana&>(*obj).eat();
};

void try_eat_banana_impl2()
{
	const object * obj = try_produce_banana();
	boost::polymorphic_cast<const banana*>(obj)->eat();
};

struct date
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
};

date parse_date_time1(const std::string& s)
{
    using boost::spirit::qi::_1;
    using boost::spirit::qi::ushort_;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;

	date res;

	const char * first = s.data();
	const char * end = first + s.size();
	bool success =
			boost::spirit::qi::parse(first,end,
				   ushort_[ref(res.year) = _1] >> char('-')
				>> ushort_[ref(res.month) = _1] >> char('-')
				>> ushort_[ref(res.day) = _1] );

	if (!success && first != end)
	{
       throw std::logic_error("Parsing failed");
	}

	return res;

};

date parse_date_time2(const std::string & s)
{
	using boost::spirit::qi::_1;
	using boost::spirit::qi::uint_parser;
	using boost::spirit::qi::char_;
	using boost::phoenix::ref;

	// use unsigned short as output type
	// require radix 10, and from 2 to 2 digits
	uint_parser<unsigned short, 10, 2, 2> u2_;

	// use unsigned short  as output type
	// require radix 10, and from 4 to 4 digits
	uint_parser<unsigned short, 10, 4, 4> u4_;

    date res;
    const char * first = s.data();
    const char * const end = first + s.size();
    bool success =
    		boost::spirit::qi::parse(first, end,
    				u4_ [ref(res.year) = _1] >> char_('-') >>
    				u2_ [ref(res.month) = _1] >> char('-') >>
    				u2_ [ref(res.day) = _1]);

    if (!success && first != end)
    {
    	throw std::logic_error("Parsing failed");
    }
	return res;

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
	std::cout << "boost_mul_using_func_obj_example:" << std::endl;
	std::vector<Number> v;
    for (int i=0; i < 4; i++)
    {
      v.push_back(Number(i));
    }

    mul_2_impl1(v);

    std::for_each(v.begin(), v.end(), printer_obj());
    std::cout << std::endl;

};

void boost_argument_order_example()
{
   std::cout << "boost_argument_order_example:" << std::endl;
   Device1 d1;
   // resulting functional object will silently ignore
   // additional parameters passed to the function call
   d1.watch(boost::bind(&detect_storm, _2, _1, _4));

   Device2 d2;

   d2.watch(boost::bind(&detect_storm, _1, _2, _3));


};

void stl_bind_example()
{
   std::cout << "stl_bind_example:" << std::endl;
   // demonstrates argument reordering and pass by reference
   int n = 7;

   // (_1 and _2 are from std::placeholders and represent future arguments that will
   // be passed to f1
   auto f1 = std::bind(f, std::placeholders::_2, std::placeholders::_1, 42, std::cref(n), n);
   n = 10;
   f1(1, 2, 1001); // 1 is bound by _2, 2 is bound by _1, 1001 is unused

   // nested bind subexpressions share the placeholders
   auto f2 = std::bind(f, std::placeholders::_3,
		   std::bind(g, std::placeholders::_3), std::placeholders::_3, 4, 5);
   f2(10, 11, 12);

   // common use case : binding an RNG with a distribution
   std::default_random_engine e;
   std::uniform_int_distribution<> d(0, 10);
   std::function<int()> rnd = std::bind(d, e); // copy of e is stored in rnd
   for (int i = 0; i < 10; ++i)
   {
	   std::cout << rnd() << ' ';
   }

   std::cout << std::endl;

   // bind to a member function
   Foo foo;
   auto f3 = std::bind(&Foo::print_sum, &foo, 95, std::placeholders::_1);
   f3(5);

   // bind to member data
   auto f4 = std::bind(&Foo::data, std::placeholders::_1);
   std::cout << f4(foo) << std::endl;

   // smart pointers can be used to call members of the referenced objects, too
   std::cout << f4(std::make_shared<Foo>(foo)) << std::endl
		   << f4(std::unique_ptr<Foo>(new Foo(foo))) << std::endl;




};

void boost_bind_example()
{
	std::cout << "boost_bind_example:" << std::endl;
	auto f5 = boost::bind(f1, 1, 2);

	std::cout << f5() << std::endl;

};

void boost_binding_val_as_func_par_example()
{
   std::cout << "boost_binding_val_as_func_par_example:" << std::endl;
   boost::array<int, 12> values = {{ 1, 2, 3, 4, 5, 6,
		   7, 100, 99, 98, 97, 96 }};

   std::size_t count0 =
		   std::count_if(values.begin(), values.end(),
				   std::bind1st(std::less<int>(), 5));
   std::size_t count1 =
	   std::count_if(values.begin(), values.end(),
				   std::bind(std::less<int>(), 5, std::placeholders::_1));
  assert(count0 == count1);


  boost::array<std::string, 3> str_values =
  {{ "We ", "are", " the champions!" }};

  count0 = std::count_if(str_values.begin(), str_values.end(),
		  std::mem_fun_ref(&std::string::empty));


  count1 = std::count_if(str_values.begin(), str_values.end(),
		  boost::bind(&std::string::empty, _1));
  assert(count0 == count1);


  count1 =
		  std::count_if(str_values.begin(),
				        str_values.end(),
				        boost::bind(std::less<std::size_t>(),
				        boost::bind(&std::string::size, _1), 5));
  assert(count1 == 2);

  std::string s("Expensive copy constructor of std::string will be called when binding");
  count0 =
		  std::count_if(str_values.begin(),
				  	    str_values.end(),
				  	    std::bind2nd(std::less<std::string>(),s));
  count1 =
		  std::count_if(str_values.begin(),
				        str_values.end(),
				        boost::bind(std::less<std::string>(), _1, s));
  assert(count1 == count0);

  std::string s1("Expensive copy constructor of std::string now won't be called when binding");
  count0 = std::count_if(str_values.begin(), str_values.end(),
		   std::bind2nd(std::less<std::string>(),
		   boost::cref(s)));
  count1 = std::count_if(str_values.begin(),
		                 str_values.end(),
		                 boost::bind(std::less<std::string>(), _1, boost::cref(s)));
  assert(count0 == count1);

};


void stl_mem_func_ref_example()
{
	std::cout << "stl_mem_func_ref_example:" << std::endl;
	std::vector<std::string> v = {"once", "upon", "a", "time"};
	std::transform(v.begin(), v.end(),
			std::ostream_iterator<std::size_t>(std::cout, " "),
			std::mem_fun_ref(&std::string::size));

};


void cpp11_move_emulation_example()
{
   person_info vasya;
   vasya.name_ = "Vasya";
   vasya.second_name_ = "Snow";
   vasya.is_male_ = true;

   person_info new_vasya(boost::move(vasya));
   assert(new_vasya.name_ == "Vasya");
   assert(new_vasya.second_name_ == "Snow");
   assert(vasya.name_.empty());
   assert(vasya.second_name_.empty());

   vasya = boost::move(new_vasya);
   assert(vasya.name_ == "Vasya");
   assert(vasya.second_name_ == "Snow");
   assert(new_vasya.name_.empty());
   assert(new_vasya.second_name_.empty());

};


void boost_movable_descriptor_example()
{
	descriptor_owner_movable movable;
	movable = construct_descriptor3();

	boost::container::vector<descriptor_owner_movable> vec;
	vec.resize(10);
	vec.push_back(construct_descriptor3());
	vec.back() = boost::move(vec.front());
};

void boost_move_example()
{
   std::cout << "boos_move_example:" << std::endl;
   move_test t1(1), t2(2);
   std::cout << "Before swap: t1.val = " << t1.getVal() << ", t2.val = " << t2.getVal() << std::endl;
   swap(t1,t2);
   std::cout << "After swap: t1.val = " << t1.getVal() << ", t2.val = " << t2.getVal() << std::endl;
};

void boost_lexical_cast_example()
{
   std::cout << "boos_lexical_cast_example:" << std::endl;
   int i = boost::lexical_cast<int>("100");
   char chars[] = {'1', '0', '0'};
   int ii = boost::lexical_cast<int>(chars,3);
   assert(i == 100);
   assert(i == ii);

   //std::locale::global(std::locale("ru_RU.UTF"));
   //float f = boost::lexical_cast<float> ("1,0");
   //assert(f < 1.01 && f > 0.99);

   std::set<std::string> str_set;
   str_set.insert("1");
   assert(container_to_longs(str_set).front() == 1);

   std::deque<const char*> char_deque;
   char_deque.push_front("1");
   char_deque.push_back("2");
   assert(container_to_longs(char_deque).front() == 1);
   assert(container_to_longs(char_deque).back() == 2);

   // obfuscating people with curly braces
   typedef boost::array<unsigned char,2> element_t;
   boost::array<element_t,2> arrays = {{ {{'1','0'}}, {{'2','0'}} }};
   assert(container_to_longs(arrays).front() == 10);
   assert(container_to_longs(arrays).back() == 20);

   std::string s = boost::lexical_cast<std::string>(100);
   assert(s == "100");

};

void boost_numeric_conversion_example()
{
	for (unsigned int i = 0; i < 100; ++i)
	{
		try
		{
			numeric_conversion_correct_implementation();
		}
		//catch (const boost::numeric::bad_numeric_cast& e)
		//{
		//	std::cout << '#' << i << ' ' << e.what() << std::endl;
		//}
		catch (const boost::numeric::positive_overflow& e)
		{
			// do something specific for positive overflow
			std::cout << "POS OVERFLOW in #" << i << ' ' << e.what() << std::endl;
		}
		catch (const boost::numeric::negative_overflow& e)
		{
			// do something specific for negative overflow
			std::cout << "NEG OVERFLOW in #" << i << ' ' << e.what() << std::endl;
		}
	}

	try
	{
		my_numeric_cast<short>(100000);
	}
	catch (const std::logic_error & e)
	{
		std::cout << "It works! " << e.what() << std::endl;
	}
};


void boost_user_defined_type_conversion_example()
{
	negative_number n = boost::lexical_cast<negative_number>("-100");
	assert(n.value_without_sign() == 100);
	int i = boost::lexical_cast<int>(n);
	assert(i == -100);

	typedef boost::array<char, 10> arr_t;
	arr_t arr =
			boost::lexical_cast<arr_t>(n);
	assert(arr[0] == '-');
	assert(arr[1] == '1');
	assert(arr[2] == '0');
	assert(arr[3] == '0');
	assert(arr[4] == '\0');

	negative_number n1 = boost::lexical_cast<negative_number>(L"-1");
	assert(n1.value_without_sign() == 1);
	typedef boost::array<wchar_t, 10> warr_t;
	warr_t arr1 = boost::lexical_cast<warr_t>(n1);
	assert(arr1[0] == L'-');
	assert(arr1[1] == L'1');
	assert(arr1[2] == L'\0');


}

void boost_simple_date_parser_example()
{
	date d = parse_date_time1("2012-12-31");
	assert(d.year == 2012);
	assert(d.month == 12);
	assert(d.day == 31);

	date d2 = parse_date_time2("2012-12-31");
	assert(d2.year == 2012);
	assert(d2.month == 12);
	assert(d2.day == 31);

}

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
   // boost_argument_order_example();
    boost_bind_example();
    stl_bind_example();
    boost_binding_val_as_func_par_example();
    stl_mem_func_ref_example();
    cpp11_move_emulation_example();
    boost_move_example();
    boost_lexical_cast_example();
    boost_numeric_conversion_example();
    boost_user_defined_type_conversion_example();
    boost_simple_date_parser_example();


	return 0;
}
