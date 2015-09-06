//============================================================================
// Name        : stringstrTut02.cpp
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

const int ROWS = 100;
const int COLS = 80;
const int BUFFSIZE = 80;

int main() {
	cout << "stringstream tutorial 02" << endl; // prints stringstream tutorial 02
	// the array must be big enough to fit input data
	int array[ROWS][COLS];
	int row, col;
	char buff[BUFFSIZE]; // a buffer to temporarily park the data
	ifstream infile("input.txt");
	stringstream ss;
	// read input into the buffer a line at a time, until
	// end of the file is reached (newlines are automatically discarded).
	// The buffer must be big enough to fit an entire line from the file.
	// Notice that while reading from the file we check how
	// many rows have been read, to avoid writing beyond the end of the array.
	row = 0;
	while ( infile.getline( buff, 10, ',') && col < COLS ) {
		// copy the entire buffered line into the stringstream
		ss << buff;
		// read from ss back into the buffer. Now, ',' is specified as
		// the delimiter so it reads only until it reaches a comma (which
		// is automatically discarded) or reaches the 'eof', but of course
		// this 'eof' is just the end of the line of the original input.
		// The "10" means this will handle input numbers of 9 digits or less.
		// while reading from the stringstream , we check how many columns have
		// been read to avoid writing past the end of the array.
		col = 0;
		while ( ss.getline( buff, 10, ',' ) && col < COLS )
		{
			// next, use the stdlib atoi function to convert the
			// input value that was just read from ss to an int,
			// and copy it into the array.
			array[row][col] = atoi( buff );
			++col;
		}
		// this copies an empty string into ss, erasing the previous
		// contents.
		ss << "";
		// This clears the 'eof' flag. Otherwise, even after
		// writing new data to ss, we would not be able to read
		// from it.
		ss.clear();
		++row;
	}

	// now print the array to see the result
	for ( int _row = 0; _row < 10; ++_row )
	{
		for ( int _col = 0; _col < 10; ++_col )
		{
			cout << array[_row][_col] << " ";
		}
		cout << endl;
	}
	infile.close();

	return 0;
}
