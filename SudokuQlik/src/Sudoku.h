/*
 * Line.h
 *
 *  Created on: Sep 5, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>


using namespace std;

namespace sudoku
{

class Region;
class HorizLine;
class VertLine;

enum Error
{
   SUDOKU_NO_ERROR=0,
   SUDOKU_ERROR_INCORRECT_INPUT_FORMAT=1,
   SUDOKU_ERROR_INCORRECT_INPUT_ROW_COUNT=2,
   SUDOKU_ERROR_INCORRECT_INPUT_COL_COUNT=4
};

class Symbol
{
private:
	char m_cVal;

	Region * m_pRegion;
	HorizLine * m_pHorizLine;
	VertLine * m_pVertLine;

public:
	Symbol(char val)  { m_cVal = val; }
	Symbol() { m_cVal = 0; }

	bool isEmpty() {  return m_cVal == 0; }

	char getValue() { return m_cVal; }
    void setValue(char val) { m_cVal = val; }

	Region * getRegion() { return m_pRegion; };
	HorizLine * getHorizLine() { return m_pHorizLine; }
	VertLine * getVertLine() { return m_pVertLine; }

    void setRegion(Region * region) { m_pRegion = region; }
    void setHorizLine(HorizLine * hline) { m_pHorizLine = hline; }
    void setVertLine(VertLine * vline) { m_pVertLine = vline; }

};

class Region
{
private:
	vector<Symbol*> m_vSymbols;

};


class Line
{
protected:
	unsigned char m_iDim, m_iRegionDim;
	Symbol **	m_pSymbols;
	Region **   m_pRegions;
	unsigned char m_iLastSymbolIdx;
	unsigned char m_iLastRegionIdx;
public:
	Line(unsigned char dim, unsigned char regDim) :
		m_iDim(dim), m_iRegionDim(regDim), m_iLastSymbolIdx(0), m_iLastRegionIdx(0)
    {
		m_pSymbols = new Symbol* [m_iDim];
	    m_pRegions = new Region* [m_iRegionDim];
    }

	bool addSymbol(Symbol* symb)
	{
		if (m_iLastSymbolIdx < m_iDim)
		{
           m_pSymbols[m_iLastSymbolIdx++] = symb;
           return true;
		}
		else
			return false;
	}

    bool addRegion(Region* region)
    {
        if (m_iLastRegionIdx < m_iRegionDim)
        {
    	   m_pRegions[m_iLastRegionIdx++] = region;
    	   return true;
        }
        else
        	return false;
    }
};

class HorizLine : public Line
{
public:
	HorizLine(unsigned char dim, unsigned char regionDim) : Line(dim, regionDim) {};


    ~HorizLine()
    {
    	// the symbols will be cleaned up by the HorizLine's ctors
    	if (m_pSymbols != NULL)
        {
    	   for (int i = 0; i < m_iDim; i++)
    		  delete m_pSymbols[i];
    	   delete [] m_pSymbols;
    	}
    }


};

class VertLine : public Line
{
public:
   VertLine(unsigned char dim, unsigned char regionDim) : Line(dim, regionDim) {};



};



class Parser
{
public:
	Parser(unsigned char dim, unsigned char regionDim, char sep = '.', char eol = '\n');

	bool parse(string inputFile);
	~Parser();
    HorizLine ** getRows() { return m_pRows; }
    VertLine **  getCols() { return m_pCols; }
    long long getError() { return m_lError; }

private:
	bool is_symbol(char c);
	bool is_separator(char c);
	bool is_end_of_line(char c);
	void cleanup_rows_and_cols(unsigned char rowCount, unsigned char colCount);

    set<char> * m_pSymbols;
    unsigned char m_iDim, m_iRegionDim;
    char m_cSep;
    char m_cEol;
    HorizLine ** m_pRows;
    VertLine ** m_pCols;
    long long m_lError;
    static char symbolTable[];
};

class Puzzle
{

private:
   unsigned char m_iDim;
   unsigned char m_iRegionDim;

   HorizLine ** m_pRows;
   VertLine ** m_pCols;
   Region ** m_pRegions;
   Parser * m_pParser;

   long long m_lError;

   void init()
   {
       m_pParser = new Parser(m_iDim, m_iRegionDim, '.');
   }

   bool process_parsed_config();

public:
   static const unsigned char CLASSIC_SUDOKU_DIM = 9;
   static const unsigned char CLASSIC_SUDOKU_REGION_DIM = 3;

   Puzzle() : m_pRows(NULL), m_pCols(NULL), m_pRegions(NULL), m_lError(0)
   {
	  m_iDim = CLASSIC_SUDOKU_DIM;
      m_iRegionDim = CLASSIC_SUDOKU_REGION_DIM;
	  init();
   }

   Puzzle(unsigned char dim, unsigned char regionDim) :
	   m_iDim(dim), m_iRegionDim(regionDim), m_pRows(NULL),
	   m_pCols(NULL), m_pRegions(NULL), m_lError(0)
   {
       init();
   }

   ~Puzzle()
   {
	   if (m_pRows != NULL)
	   {
		   for (int i = 0; i < m_iDim; i++)
			  delete m_pRows[i];
		   delete [] m_pRows;
	   }

	   if (m_pCols != NULL)
	   {
		   for (int i = 0; i < m_iDim; i++)
			 delete m_pCols[i];
		   delete [] m_pCols;
	   }

	   if (m_pRegions != NULL)
	   {
	      for (int i = 0; i < m_iDim; i++)
		     delete m_pRegions[i];
	      delete [] m_pRegions;
	   }

	   if (m_pParser != NULL)
	      delete m_pParser;

   }


   bool load(string inputFile)
   {
      bool res = m_pParser->parse(inputFile);

      res = process_parsed_config();

	  return res;
   }



};





}

#endif /* SUDOKU_H_ */
