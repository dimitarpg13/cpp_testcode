//============================================================================
// Name        : stringstrTut01.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

const int ROWS = 6;
const int COLS = 5;
const int BUFFSIZE = 80;

int main() {
	cout << "stringstream tutorial 01" << endl; // prints stringstream tutorial
    int array[ROWS][COLS];
    char buff[BUFFSIZE]; // a buffer to temporarily park the data
    ifstream infile("./input.txt");
    stringstream ss;
    for ( int row = 0; row < ROWS; ++row )
    {
    	// read a full line of input into the buffer (newline is
    	// automatically discarded
        infile.getline( buff, BUFFSIZE );
        // copy the entire line into stringstream
        ss << buff;
        for ( int col = 0; col < COLS; ++col )
        {
        	// read from ss back into the buffer. Here ',' is
        	// specified as the delimiter so it reads only until
        	// it reaches comma (which is automatically discarded)
        	// or reaches the 'eof', but of course this 'eof' is really
        	// just the end of a line of the original input. The "6" is
        	// because I figured the input numbers would be 5 digits or less.
        	ss.getline( buff, 6, ',' );

        	// next use the stdlib atoi function to convert the input value
        	// that was just read from ss to an int , and copy it into the array
        	array[row][col] = atoi(buff);
        }

        // This copies an empty string into ss erasing the previous contents
        ss << "";

        // This clears the "eof" flag. Otherwise, even after writing a new data
        // to ss we would not be able to read from it
        ss.clear();
    }

    // now print the array to see the result
    for ( int row = 0; row < ROWS; ++row )
    {
    	for ( int col = 0; col < COLS; ++col )
    	{
    		cout << array[row][col] << " ";
    	}
    	cout << endl;
    }
    infile.close();

	return 0;
}
