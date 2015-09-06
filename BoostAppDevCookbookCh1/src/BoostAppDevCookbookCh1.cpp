//============================================================================
// Name        : BoostAppDevCookbookCh1.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/variant/multivisitors.hpp>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <typeinfo>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <set>

using namespace std;
// Boost C++ application dev cookbook chapter 1
// example of how to use boost::any
using boost::any_cast;
typedef list<boost::any> many;



// Boost C++ application dev cookbook chapter 1
void append_int(many & values, int value)
{
	boost::any to_append = value;
	values.push_back(to_append);
}

// Boost C++ application dev cookbook chapter 1
void append_string(many & values, const std::string & value)
{
	values.push_back(value);
}

// Boost C++ application dev cookbook chapter 1
void append_char_ptr(many & values, const char * value)
{
	values.push_back(value);
}

// Boost C++ application dev cookbook chapter 1
void append_any(many & values, const boost::any & value)
{
	values.push_back(value);
}

// Boost C++ application dev cookbook chapter 1
void append_nothing(many & values)
{
	values.push_back(boost::any());
}

// Boost C++ application dev cookbook chapter 1
bool is_any_empty(const boost::any & operand)
{
	return operand.empty();
}

// Boost C++ application dev cookbook chapter 1
bool is_int(const boost::any & operand)
{
	return operand.type() == typeid(int);
}

// Boost C++ application dev cookbook chapter 1
bool is_char_ptr(const boost::any & operand)
{
	try
	{
		any_cast<const char*>(operand);
		return true;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

// Boost C++ application dev cookbook chapter 1
bool is_string(const boost::any & operand)
{
	return any_cast<std::string>(&operand);
}

// Boost C++ application dev cookbook chapter 1
void count_all(many & values, std::ostream & out)
{
	out << "#empty == "
		<< std::count_if(values.begin(), values.end(), is_any_empty) << std::endl;
    out << "#int == "
    	<< std::count_if(values.begin(), values.end(), is_int) << std::endl;
    out << "#const char * == "
    	<< std::count_if(values.begin(), values.end(), is_char_ptr) << std::endl;
    out << "#string == "
    	<< std::count_if(values.begin(), values.end(), is_string) << std::endl;
}

// Boost.Variant tutorial - boost_1_56_pdf
void times_two(boost::variant< int , std::string > & operand )
{
   if ( int* pi = boost::get<int> ( &operand ) )
	   *pi *= 2;
   else if ( std::string * pstr = boost::get<std::string> ( &operand ) )
	   *pstr += " " + *pstr;
}


// Boost.Variant tutorial - boost_1_56_pdf
class times_two_visitor : public boost::static_visitor<>
{
public :
	void operator() (int & i) const
	{
		i *= 2;
	}

	void operator() (std::string & str) const
	{
		str += " " + str;
	}
};

// Boost.Variant tutorial - boost_1_56_pdf
class times_two_generic : public boost::static_visitor<>
{
public:
	template<typename T>
	void operator() ( T & operand ) const
	{
		operand += operand;
	}
};

class console_print_generic : public boost::static_visitor<>
{
public:
   template<typename T>
   void operator() ( T & operand ) const
   {
	   std::cout << operand << std::endl;
   }
};

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with recursive_wrapper
struct add;
struct sub;

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with recursive_wrapper
template<typename OpTag> struct binary_op;
typedef boost::variant<
           int,
   		boost::recursive_wrapper< binary_op<add> >,
   		boost::recursive_wrapper< binary_op<sub> >
   > expression;

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with recursive_wrapper
template<typename OpTag>
struct binary_op
{
	expression left;
	expression right;

	binary_op( const expression & lhs, const expression & rhs )
	: left(lhs), right(rhs)
	{

	}
};

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with recursive_wrapper
class calculator : public boost::static_visitor<int>
{
public:
	int operator() (int value) const
	{
		return value;
	}

    int operator() (const binary_op<add> & binary) const
    {
    	return boost::apply_visitor( calculator(), binary.left )
    		 + boost::apply_visitor( calculator(), binary.right );
    }

    int operator() (const binary_op<sub> & binary) const
    {
    	return boost::apply_visitor( calculator(), binary.left )
    	     - boost::apply_visitor( calculator(), binary.right );
    }

};

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with recursive_wrapper
expression f()
{
	// result = ((7-3) + 8) = 12
	expression result (
		binary_op<add> ( binary_op<sub> (7,3), 8 )
	);

	//std::cout << result << std::endl;
	return result;
};

// Boost.Variant tutorial - boost_1_56_pdf
// recursive types with make_recursive_variant
typedef boost::make_recursive_variant<
		int,
		std::vector< boost::recursive_variant_ >
>::type int_tree_t;

// Boost.Variant tutorial - boost_1_56_pdf
// binary visitaion
class are_strict_equals : public boost::static_visitor<bool>
{
public:
	template <typename T, typename U>
	bool operator() ( const T &, const U & )
	{
		return false; // cannot compare different types
	}

	template <typename T>
	bool operator() ( const T & lhs, const T & rhs )
	{
		return lhs == rhs;
	}
};

// Boost.Variant tutorial - boost_1_56_pdf
// multi visitaion
typedef boost::variant<int, double, bool> bool_like_t;
typedef boost::variant<int, double, bool> arithmetics_t;

struct if_visitor : public boost::static_visitor<arithmetics_t>
{
   template<class T1, class T2>
   arithmetics_t operator() (bool b, T1 v1, T2 v2) const {
	   if (b)
	   {
		   return v1;
	   }
	   else
	   {
		   return v2;
	   }
   }
};


// Boost C++ application dev cookbook chapter 1
// using a safer way to work with a container that
// stores multiple chosen types

// This typedefs and methods will be in our header,
// that wraps around native SQL interface
typedef boost::any cell_any_t;
typedef std::vector<cell_any_t> db_row_any_t;
// this is just an example, no actual work with database
db_row_any_t get_any_row(const char * /* query */)
{
    // in real application 'query' parameter shall have a 'const char *' or
	// 'const std::string&' type? see a recipe "Using a reference to string type"
	// in Ch07 "Manipulating strings" for an answer.
    db_row_any_t row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back(std::string("hello again"));
    return row;
}

// This is how a user will use your classes
struct db_sum : public std::unary_function<boost::any,void>
{
private:
	double & sum_;
public:
	explicit db_sum(double & sum) : sum_(sum)
	{
	}

	void operator() (const cell_any_t& value)
	{
       const std::type_info& ti = value.type();
       if (ti == typeid(int))
       {
    	   sum_ += boost::any_cast<int>(value);
       }
       else if (ti == typeid(float))
       {
    	   sum_ += boost::any_cast<float>(value);
       }
	};

};


typedef boost::variant<int, float, std::string> cell_ifs_t;
typedef std::vector<cell_ifs_t> db_row_ifs_t;
// this is just an example, no actual work with database
db_row_ifs_t get_ifs_row(const char * /* query */)
{
    // in real application 'query' parameter shall have a 'const char *' or
	// 'const std::string&' type? see a recipe "Using a reference to string type"
	// in Ch07 "Manipulating strings" for an answer.
    db_row_ifs_t row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back(std::string("hello again"));
    return row;
}

// This is the code required to sum values
// we can provide no template parameter to boost::static_visitor<> if our visitor returns nothing
struct db_sum_visitor : public boost::static_visitor<double>
{
	double operator() (int value) const {
		return value;
	}

	double operator() (float value) const {
		return value;
	}

	double operator() (const std::string& /* value */) const
	{
		return 0.0;
	}
};


// Boost.Optional tutorial
// quick start with convert function
boost::optional<int> convert(const std::string& text)
{
	std::stringstream s(text);
	int i;
	if ((s >> i) && s.get() == std::char_traits<char>::eof())
	   return i;
	else
	   return boost::none;
};

boost::optional<int> convert2(const std::string& text)
{
	boost::optional<int> ans;
    std::stringstream s(text);
    int i;
    if ((s >> i) && s.get() == std::char_traits<char>::eof())
    	ans = i;

    return ans;
};

int fallback_to_default()
{
	cout << "could not convert; using -1 instead" << std::endl;
	return -1;
}

// this class simulates a code which attempts to lock a device and fails
// roughly half of the times it attempts to do so

class locked_device {
	explicit locked_device(const char * /*param*/)
	{
		// we have unique access to the device
		std::cout << "Device is locked\n";
	}

public:
	~locked_device() {
		// releasing device lock
	}

	void use()
	{
		std::cout << "Success!" << endl;
	}

	static boost::optional<locked_device> try_lock_device()
	{
        if (rand()%2)
        {
        	// failed to lock device
        	return boost::none;
        }

        // success!
        return locked_device("device name");
	};

};

struct add_1 : public std::unary_function<char, void>
{
	void operator() (char & c) const {
		++c;
	}

	// if you are not in a mood to write functional objects,
	// but do not know what does 'boost::bind(std::plus<char>,_1,1)' do
	// then read the recipe 'Binding a value as a function parameter'

};

typedef boost::array<char,4> array4_t;
array4_t& vector_advance(array4_t& val)
{
	// boost::array has begin(), cbegin(), end(), cend(),
	// rbegin(), size(), empty(), and other functions that are common for STL containers
	std::for_each(val.begin(), val.end(), add_1());
	return val;
};

/*template<int indx, typename L>
int print_me_imp()
{
	return indx+1;
};




template<int indx, typename F, typename... R>
int print_me_imp()
{
   //std::cout << boost::get<indx>
   return print_me_imp<indx+1,R...>();
};*/




template<int idx, typename F, typename... R>
struct printer_imp
{

	static const int Indx = printer_imp<idx+1,R...>::Indx;
	//void operator() (char & c) const {
	//
	//	}


};

template<int idx, typename T>
struct printer_imp<idx,T>
{
	static const int Indx = idx + 1;
};

template<typename... V>
void print_me(boost::tuple<V...> t)
{
	//int totCnt = count<V...>(0);
	//for (const int i=0; i < totCnt; i++)
	//{
	//   std::cout << boost::get<i>(t) << endl;
	//}
	//print_me<indx+1, V...>(t);
	//std::cout << print_me_imp<V...>(0) << endl;
};

template<typename... Vals>
struct print_tuple : public std::unary_function<boost::tuple<Vals...>, void>
{


	void operator() (boost::tuple<Vals...> t) const
	{
       //print_me_imp<Vals...>(0);
	}

	// if you are not in a mood to write functional objects,
	// but do not know what does 'boost::bind(std::plus<char>,_1,1)' do
	// then read the recipe 'Binding a value as a function parameter'

} ;



// example code from:
// Boost C++ application dev cookbook chapter 1
// Boost.Variant tutorial - boost_1_56_pdf
int main() {
	cout << "Boost C++ Application Dev Cookbook Chapter 1" << endl; // prints Boost C++ Application Dev Cookbook Chapter 1

	// Boost C++ application dev cookbook chapter 1
	// storing multiple chosen types into a variable/container
	std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char* c_str = "Hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow"));
    std::string & s = boost::any_cast<std::string&> (some_values.back());
    s += " That is great!\n";
    std::cout << s;


    // Boost.Variant tutorial - boost_1_56_pdf
    // basic usage
    boost::variant<int , std::string> v;
    v = "hello";
    std::cout << v << std::endl;
    std::string& str = boost::get<std::string>(v);
    str += " world";
    std::cout << v << std::endl;
    times_two(v);
    std::cout << v << std::endl;
    boost::apply_visitor( times_two_visitor(), v);
    std::cout << v << std::endl;
    boost::apply_visitor( times_two_generic(), v);
    std::cout << v << std::endl;

    std::vector< boost::variant<int, std::string> > vec;
    vec.push_back( 21 );
    vec.push_back( "hello" );

    times_two_generic tt_visitor;
    std::for_each(
        vec.begin(), vec.end(),
        boost::apply_visitor(tt_visitor)
    );

    console_print_generic cp_visitor;
    std::for_each(
    	vec.begin(), vec.end(),
    	boost::apply_visitor(cp_visitor)
    );

    typedef boost::mpl::vector< std::string > types_initial;
    typedef boost::mpl::push_front< types_initial, int >::type types;
    boost::make_variant_over< types >::type v1;

    v1 = 11;
    std::cout << v1 << endl;

    // Boost.Variant tutorial - boost_1_56_pdf
    // recursive types with recursive_wrapper
    //std::cout << f() << endl;
    f();

    // Boost.Variant tutorial - boost_1_56_pdf
    // recursive types with make_recursive_variant
    std::vector< int_tree_t > subresult;
    subresult.push_back(3);
    subresult.push_back(5);

    std::vector< int_tree_t > result;
    result.push_back(1);
    result.push_back(subresult);
    result.push_back(7);

    int_tree_t var(result);

    std::cout << boost::get<int>(result[0]) << endl;
    std::vector< int_tree_t > t = boost::get<std::vector< int_tree_t >>(var);

    try
    {
        std::cout << boost::get<int>(t[0]) << endl;
    }
    catch (boost::bad_get & e)
    {
    	std::cout << e.what();
    }

    // Boost.Variant tutorial - boost_1_56_pdf
    // binary visitation
    boost::variant< int, std::string > var1( "hello" );

    boost::variant< double, std::string > var2( "hello" );
    assert( boost::apply_visitor(are_strict_equals(), var1, var2) );

    boost::variant< int, const char * > var3( "hello" );
    assert( !boost::apply_visitor(are_strict_equals(), var1, var3) );

    typedef boost::variant<double, std::string> my_variant;
    std::vector< my_variant > seq1;
    seq1.push_back("pi is close to ");
    seq1.push_back(3.14);

    std::list< my_variant > seq2;
    seq2.push_back("pi is close to ");
    seq2.push_back(3.14);

    are_strict_equals visitor;
    assert( std::equal ( seq1.begin(), seq1.end(), seq2.begin(), boost::apply_visitor( visitor )) );

    // Boost.Variant tutorial - boost_1_56_pdf
    // multi visitation
    bool_like_t mvar0(1), mvar1(true), mvar2(1.0);

    assert( boost::apply_visitor(if_visitor(), mvar0, mvar1, mvar2) == arithmetics_t(true) );

    // Boost C++ application dev cookbook chapter 1
    // using a safer way to work with a container that
    // stores multiple chosen types
    db_row_any_t row_any = get_any_row("Query: Give me some row, please.");
    double res = 0.0;
    std::for_each(row_any.begin(), row_any.end(), db_sum(res));
    std::cout << "Sum of arithmetic types in database row is: " << res << std::endl;

    // This is the code required to sum values
    // we can provide no template parameter to boost::static_visitor<> if our visitor returns nothing
    db_row_ifs_t row_ifs = get_ifs_row("Query: give me some row, please.");
    res = 0.0;
    db_row_ifs_t::const_iterator it = row_ifs.begin(), end = row_ifs.end();
    for (; it != end; ++it)
    {
    	res += boost::apply_visitor(db_sum_visitor(), *it);
    }
    std::cout << "Sum of arithmetic types in database row is : " << res << endl;


    // Boost.Optional tutorial
    // quick start with convert function
    const std::string & text = "Hello people!";
    boost::optional<int> oi = convert(text);
    if (oi)
    {
    	int i = *oi;
    	std::cout << "after conversion: " << i << std::endl;
    }
    else
    {
    	std::cout << "not convertible!" << std::endl;
    }

    int i = *convert("100");
	std::cout << "after conversion: " << i << std::endl;

    try
    {
    	int j = convert(text).value();
    }
    catch (const boost::bad_optional_access &)
    {
    	std::cout << "bad_optional_access thrown!" << std::endl;
    }

    int l = convert(text).value_or_eval(fallback_to_default);
    std::cout << "the value of l is " << l << std::endl;


    // Boost has library called Random. If you wonder why it was written when stdlib.h has rand() function
    // see the recipe "Using a true random generator" in Chapter 12,
    // scratching the tip of the iceberg
    srandom(5);
    bool lock_result=false;
    for (unsigned i = 0; i < 10; ++i)
    {
    	boost::optional<locked_device> t = locked_device::try_lock_device();
    	// optional is convertible to bool
    	if (t)
    	{
    		t->use();
    		lock_result=true;
    		break;
    	}
    	else
    	{
    		std::cout << "...trying again" << std::endl;
    	}
    }
    if (lock_result==false)
        std::cout << "Failure!" << std::endl;

    // Boost.array example from the app development cookbook
    // we can initialize boost::array just like an array in C++11
    // array4_t val = {0, 1, 2, 3};
    // but in C++03 additional pair of curly brackets is required
    // array4_t val = {{0, 1, 2, 3}};
    array4_t val = {0, 1, 2, 3};
    // boost::array works like an usual array
    array4_t val_res;  // it can be default constructible
    val_res = vector_advance(val);   // assignable if value type supports default construction and assignment
    assert(val.size()==4);
    assert(val[0] == 1);
    /* val[4]; */ // will trigger an assert because max index is 3
    // we can make this assert work at compile time . See 'Checking sizes at compile time' in Chapter 4 'Compile time tricks'
    assert(sizeof(val) == sizeof(char) * array4_t::static_size);

    // tuple usage from the Cookbook topic 'Combining multiple values into one'
    boost::tuple<int, std::string> almost_a_pair(10, "Pair");
    boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);
    int int_from_tuple = boost::get<0>(almost_a_pair);
    const std::string& str_from_tuple = boost::get<1>(almost_a_pair);
    double double_from_quad = boost::get<2>(quad);
    std::cout << "int from tuple: " << int_from_tuple << endl;
    std::cout << "string from tuple: " << str_from_tuple << endl;
    std::cout << "double from quad: " << double_from_quad << endl;

    // tuple comparisons needs to be included if we are to insert tuples in a set
    //
    std::set< boost::tuple<int, double, int> > set_of_tuples;
    set_of_tuples.insert(boost::make_tuple(1, 1.0, 2));
    set_of_tuples.insert(boost::make_tuple(2, 10.0, 2));
    set_of_tuples.insert(boost::make_tuple(3, 100.0, 2));

    print_tuple<int, double, int> tuple_out;

    std::for_each(set_of_tuples.begin(),set_of_tuples.end(), tuple_out);
    //std::cout << "print_me_imp : " << print_me_imp<0, int, double, int>() << std::endl;
    std::cout << "printer_imp: " << printer_imp<0, int, double, int>::Indx << std::endl;
	return 0;
}
