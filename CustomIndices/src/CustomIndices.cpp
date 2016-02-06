//============================================================================
// Name        : CustomIndices.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>

#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <stack>

const int max_quotes = 10000000;
const int max_indices = 10000;
const int max_comp = 10000;

struct cindex;
struct stock
{
	std::string name;
	virtual bool composite() { return false; }
	double value;
	std::vector<cindex *> derivatives;
	virtual ~stock() {};
};

struct cindex : stock
{
	std::string name;
	bool composite() { return true; }
	bool plus;
	std::vector<stock *> components;

};



std::map<std::string,stock *> squotes;
std::map<std::string,cindex *> ciquotes;


bool process_quote(std::vector<std::string>::const_iterator qbegin, std::vector<std::string>::const_iterator qend)
{

	return true;
};

bool process_config(std::vector<std::string>::const_iterator cbegin, std::vector<std::string>::const_iterator cend)
{
	while (cbegin != cend)
	{

		cbegin++;
	}

	return true;
};


bool process_input_line(std::vector<std::string> & quote)
{
   if (quote.size() < 2)
		return false;


  if (quote[0] == "Q")
       process_quote(++quote.begin(),quote.end());
  else
	   process_config(++quote.begin(),quote.end());

   return true;
}


int main (int argc, char ** argv) {
    // Initialization
    //
    // In this part of the program you can perform any kind of
    // initialization routine before processing the stream
    // of data.

	std::ifstream ifs;

	ifs.open (argv[1], std::ifstream::in);

    std::string line;
    std::vector<std::string> quote;
    std::string item;
    while(std::getline(ifs, line)) {
        if(! line.empty()) {
            quote.clear();
            std::istringstream ss(line);
            while (std::getline(ss, item, '|')) {
                quote.push_back(item);
            }
            // Step 1: Input.
            //
            // Process the new line on the stdin.
            process_input_line(quote);


        }
    }
    // Step 2: Output
    //
    // Print the Custom Indices value.

    return 0;
}
