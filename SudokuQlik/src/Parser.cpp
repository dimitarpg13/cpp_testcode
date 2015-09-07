/*
 * ProblemParser.cpp
 *
 *  Created on: Sep 4, 2015
 *      Author: Dimitar Gueorguiev
 */


#include "Parser.h"

namespace sudoku
{

 char Parser::symbolTable[] = {
  		                  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
  		                  0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
  		                  0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
  		                  0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
  		                  0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64,
  		                  0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
  		                  0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
  		                  0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
  		                  0x7d, 0x7e, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
  		                  0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
  		                  0x2f, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
  		                  0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60
                          };



Parser::Parser(int dim, char sep, char eol) : m_iDim(dim), m_cSep(sep), m_cEol(eol), m_lError(0)
{
	m_pSymbols = new set<char>(symbolTable,symbolTable+dim);
	m_pRows = new HorizLine* [m_iDim];
	m_pCols = new VertLine* [m_iDim];
	for (int i=0; i<m_iDim; i++)
	{
		m_pRows[i] = NULL;
		m_pCols[i] = NULL;
	}
}

Parser::~Parser()
{
	cleanup_rows_and_cols(m_iDim, m_iDim);
    delete m_pSymbols;
};


bool Parser::is_symbol(char c)
{
	set<char>::iterator it = m_pSymbols->find(c);
	if (it != m_pSymbols->end())
		return true;
	else
		return false;
}

bool Parser::is_separator(char c)
{
   return c == m_cSep;
}

bool Parser::is_end_of_line(char c)
{
	return c == m_cEol;
}

void Parser::cleanup_rows_and_cols(int rowCount, int colCount)
{
	if (m_pRows != NULL)
	{
	   for (int i = 0; i < rowCount; i++)
	      delete m_pRows[i];
	   delete [] m_pRows;
	}

	if (m_pCols != NULL)
	{
	   for (int i = 0; i < colCount; i++)
	     delete m_pCols[i];
	   delete [] m_pCols;
	}

	m_pRows = NULL;
	m_pCols = NULL;
}

bool Parser::parse(string inputFile)
{
	bool res=true;
	ifstream fs (inputFile.c_str(), std::ifstream::in);

	char c = fs.get();
    if (fs.bad())
    {
      m_lError |= SUDOKU_ERROR_INCORRECT_INPUT_FORMAT;

      delete [] m_pRows;
      delete [] m_pCols;

      m_pRows = NULL;
      m_pCols = NULL;

      return false;
    }

	unsigned short curRowIdx = 0, curColIdx = 0;

	Symbol * curSymbol = NULL;
    HorizLine * curRow = NULL;
    VertLine * curCol = NULL;
	while (fs.good())
	{

	  if (curRow == NULL)
		  curRow = new HorizLine();

	  if (m_pCols[curColIdx] != NULL)
		  curCol = m_pCols[curColIdx];
	  else
	  {
		  curCol = new VertLine();
		  m_pCols[curColIdx] = curCol;
	  }

	  if ( is_symbol(c) )
	  {
		  curSymbol = new Symbol(c);
          curRow->addSymbol(curSymbol);
          curCol->addSymbol(curSymbol);
          curColIdx++;
	  }
	  else if ( is_separator(c) )
	  {
		  curSymbol = new Symbol();
          curRow->addSymbol(curSymbol);
          curCol->addSymbol(curSymbol);
          curColIdx++;
	  }
	  else if ( is_end_of_line(c) )
	  {
		 m_pRows[curRowIdx] = curRow;
		 curRow = NULL;
		 curRowIdx++;
		 curColIdx = 0;
	  }

	  std::cout << c;
	  c = fs.get();
	}

	// the end of line char is missing on the last line of the input file
	if (curRow != NULL)
	{
		m_pRows[curRowIdx] = curRow;
		curRowIdx++;
	}

	fs.close();

    if (curRowIdx != m_iDim)
    {
        m_lError |= SUDOKU_ERROR_INCORRECT_INPUT_ROW_COUNT;

        cleanup_rows_and_cols(curRowIdx, curColIdx);

        res=false;
    }

    if (curColIdx != m_iDim)
    {
    	m_lError |= SUDOKU_ERROR_INCORRECT_INPUT_COL_COUNT;

    	cleanup_rows_and_cols(curRowIdx, curColIdx);

    	res=false;
    }

	return res;
}

}
