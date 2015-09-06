/*
 * Parser.h
 *
 *  Created on: Sep 4, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#include "Line.h"
#include "Symbol.h"

using namespace std;

namespace sudoku
{

enum Error
{
  INCORRECT_FORMAT=0,
  INCORRECT_ROW_COUNT=1,
  INCORRECT_COL_COUNT=2
};

class Parser
{
public:
	Parser(int dim);
	Parser(int dim, char sep);
	Parser(int dim, char sep, char eol);
	bool parse(string inputFile);
	~Parser() { delete m_pSymbols; };
    vector<HorizLine*> & getRows() { return m_vRows; }
    vector<VertLine*> & getCols() { return m_vCols; }

private:
	bool is_symbol(char c);
	bool is_separator(char c);
	bool is_end_of_line(char c);
    set<char> * m_pSymbols;
    int m_iDim;
    char m_cSep;
    char m_cEol;
    vector<HorizLine*> m_vRows;
    vector<VertLine*> m_vCols;

    static char symbolTable[];
};

}

#endif /* PARSER_H_ */
