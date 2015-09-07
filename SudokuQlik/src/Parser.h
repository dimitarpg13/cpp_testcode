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
  ERROR_INCORRECT_INPUT_FORMAT=1,
  ERROR_INCORRECT_INPUT_ROW_COUNT=2,
  ERROR_INCORRECT_INPUT_COL_COUNT=4
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
    long long getError() { return m_lError; }

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
    long long m_lError;
    static char symbolTable[];
};

}

#endif /* PARSER_H_ */
