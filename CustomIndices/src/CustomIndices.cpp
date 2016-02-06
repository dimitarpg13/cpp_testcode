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
#include <stack>

const int max_quotes = 10000000;
const int max_indices = 10000;
const int max_comp = 10000;

typedef std::pair<std::string,double> qtype;

struct cindex
{
	std::string name;
	bool composite() { return !name1.empty() && !name2.empty(); }
	bool plus;
	std::string name1;
	std::string name2;

};

std::stack<qtype> quotes;

bool process_quote(std::vector<std::string> & quote)
{


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
            process_quote(quote);


        }
    }
    // Step 2: Output
    //
    // Print the Custom Indices value.

    return 0;
}
