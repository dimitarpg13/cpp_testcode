/*
 * Sudoku.h
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
#include <list>
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
   SUDOKU_ERROR_MISSING_INPUT_FILE=1,
   SUDOKU_ERROR_INCORRECT_INPUT_FORMAT=2,
   SUDOKU_ERROR_INCORRECT_INPUT_ROW_COUNT=4,
   SUDOKU_ERROR_INCORRECT_INPUT_COL_COUNT=8,
   SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE=16,
   SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION=32
};




class Symbol
{
private:
	char m_cVal;

	HorizLine * m_pRow;
	VertLine * m_pCol;
	Region * m_pRegion;
	list<char> * m_pAssignments;

	char m_cLastRemoved;
    bool m_bCanChoose;
    unsigned short m_iFailedCount;

public:


	Symbol(char val, HorizLine * row, VertLine * col, Region * region = NULL) :
		m_cVal(val), m_pRow(row), m_pCol(col), m_pRegion(region),
		m_pAssignments(NULL), m_cLastRemoved(0), m_bCanChoose(true), m_iFailedCount(0)
    { }


	Symbol(HorizLine * row, VertLine * col, Region * region = NULL) :
		m_cVal(0), m_pRow(row), m_pCol(col), m_pRegion(region),
		m_pAssignments(NULL), m_cLastRemoved(0), m_bCanChoose(true), m_iFailedCount(0)
	{ }


	~Symbol() { if (m_pAssignments != NULL) delete m_pAssignments; }

	bool isEmpty() {  return m_cVal == 0; }

	char getValue() { return m_cVal; }
    void setValue(char val) { m_cVal = val; }

	Region * getRegion() { return m_pRegion; }
	HorizLine * getRow() { return m_pRow; }
	VertLine * getCol() { return m_pCol; }
	list<char> * getAssignments() { return m_pAssignments; }
	char getLastRemoved() { return m_cLastRemoved; }
	bool getCanChoose() { return m_bCanChoose; }
	unsigned short getFailedCount() { return m_iFailedCount; }

    void setRegion(Region * region) { m_pRegion = region; }
    void setRow(HorizLine * row) { m_pRow = row; }
    void setCol(VertLine * col) { m_pCol = col; }
    void setAssignments(list<char> * assignments) { m_pAssignments = assignments; }
    void setLastRemoved(char lastRemoved) { m_cLastRemoved = lastRemoved; }
    void setCanChoose(bool canChoose) { m_bCanChoose = canChoose; }
    void incrementFailedCount() { m_iFailedCount++; }
    void resetFailedCount() { m_iFailedCount = 0; }
};


class Line
{
protected:
	unsigned char m_iDim, m_iRegionDim, m_iRegionCount;
	Symbol **	m_pSymbols;
	Region **   m_pRegions;
	unsigned char m_iLastSymbolIdx;
	unsigned char m_iLastRegionIdx;
#ifdef _DEBUG
	unsigned char m_iMyIdx;
#endif

public:
    unsigned char getDim() { return m_iDim; }
    unsigned char getRegionDim() { return m_iRegionDim; }
	Symbol ** const getSymbols() { return m_pSymbols; }
	Region ** const getRegions() { return m_pRegions; }
#ifdef _DEBUG
	unsigned char getIdx() { return m_iMyIdx; }
#endif

#ifndef _DEBUG
	Line(unsigned char dim, unsigned char regDim) :
		m_iDim(dim), m_iRegionDim(regDim), m_iLastSymbolIdx(0), m_iLastRegionIdx(0)
    {
		m_pSymbols = new Symbol* [m_iDim];
		m_iRegionCount = m_iDim / m_iRegionDim;
	    m_pRegions = new Region* [m_iRegionCount]; // the number of regions spanning single line
    }
#else
	Line(unsigned char dim, unsigned char regDim, unsigned char myIdx) :
			m_iDim(dim), m_iRegionDim(regDim), m_iLastSymbolIdx(0), m_iLastRegionIdx(0),
			m_iMyIdx(myIdx)
	    {
			m_pSymbols = new Symbol* [m_iDim];
			m_iRegionCount = m_iDim / m_iRegionDim;
		    m_pRegions = new Region* [m_iRegionCount]; // the number of regions spanning single line
	    }

#endif

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
        if (m_iLastRegionIdx < m_iRegionCount)
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
#ifndef _DEBUG
	HorizLine(unsigned char dim, unsigned char regionDim) : Line(dim, regionDim) {};
#else
	HorizLine(unsigned char dim, unsigned char regionDim, unsigned char myIdx) : Line(dim, regionDim, myIdx) {};
#endif

    ~HorizLine()
    {
    	// the symbols will be cleaned up by the HorizLine's destructors
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
#ifndef _DEBUG
   VertLine(unsigned char dim, unsigned char regionDim) : Line(dim, regionDim) {};
#else
   VertLine(unsigned char dim, unsigned char regionDim, unsigned char myIdx) : Line(dim, regionDim, myIdx) {};
#endif

   ~VertLine()
   {
      	// the symbols will be cleaned up by the HorizLine's destructors
      	if (m_pSymbols != NULL)
        {
      	   delete [] m_pSymbols;
      	}
   }

};


class Region
{
public:
	Region(unsigned char dim, unsigned char regionDim) :
		m_iDim(dim), m_iRegionDim(regionDim),
		m_iLastSymbolIdx(0), m_iLastRowIdx(0), m_iLastColIdx(0)
    {
		m_iSymbCount = m_iRegionDim * m_iRegionDim;
		m_pSymbols = new Symbol* [m_iSymbCount]; // the number of symbols in single region
		m_pRows = new HorizLine* [m_iRegionDim];
		m_pCols = new VertLine* [m_iRegionDim];
    };

	unsigned char getDim() { return m_iDim; };
	unsigned char getSymbCount() { return m_iSymbCount; };
	Symbol ** const getSymbols() { return m_pSymbols; };

    ~Region()
    {
    	// the symbols will be cleaned up by the HorizLine's destructors
    	if (m_pSymbols != NULL)
    	   delete [] m_pSymbols;

    	// the rows will be cleaned up by Puzzle's destructor
    	if (m_pRows != NULL)
    		delete [] m_pRows;

    	// the columns will be cleaned up by Puzzle's destructor
    	if (m_pCols != NULL)
    		delete [] m_pCols;

    };

    bool addSymbol(Symbol * symbol)
    {
    	if (m_iLastSymbolIdx < m_iSymbCount)
           m_pSymbols[m_iLastSymbolIdx++] = symbol;
    	else
    		return false;

    	return true;
    }

    bool addRow(HorizLine * row)
    {
    	if (m_iLastRowIdx < m_iRegionDim)
    		m_pRows[m_iLastRowIdx++] = row;
    	else
    		return false;

        return true;
    }

    bool addCol(VertLine * col)
    {
    	if (m_iLastColIdx < m_iRegionDim)
    		m_pCols[m_iLastColIdx++] = col;
    	else
    		return false;

    	return true;
    }

private:
	Symbol ** m_pSymbols;
	HorizLine ** m_pRows;
	VertLine ** m_pCols;

	unsigned char m_iDim;
    unsigned char m_iRegionDim;
    unsigned char m_iSymbCount;

    unsigned char m_iLastSymbolIdx;
    unsigned char m_iLastRowIdx;
    unsigned char m_iLastColIdx;
};





class Parser
{
public:
	Parser(unsigned char dim, unsigned char regionDim, char sep = '.', char eol = '\n');

	bool parse(string inputFile);
	~Parser();
    HorizLine ** getRows() { return m_pRows; }
    VertLine **  getCols() { return m_pCols; }
    Region ** getRegions() { return m_pRegions; }
    long long getError() { return m_lError; }
    const set<char> * getSymbolTable() { return m_pSymbols; }
    unsigned char getRegionIdx(unsigned char rowIdx, unsigned char colIdx);

private:
	bool is_symbol(char c);
	bool is_separator(char c);
	bool is_end_of_line(char c);
	void init();
	void cleanup(unsigned char rowCount, unsigned char colCount, unsigned char regCount);

    set<char> * m_pSymbols;
    unsigned char m_iDim, m_iRegionDim;
    char m_cSep;
    char m_cEol;
    long long m_lError;
    HorizLine ** m_pRows;
    VertLine ** m_pCols;
    Region ** m_pRegions;
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

   unsigned char getDim() { return m_iDim; }
   unsigned char getRegionDim() { return m_iRegionDim; }

   HorizLine ** const getRows() { return m_pRows; }
   VertLine ** const getCols() { return m_pCols; }
   Region ** const getRegions() { return m_pRegions; }

   long long getError() { return m_lError; };
   const set<char> * getSymbolTable() { return m_pParser->getSymbolTable(); }

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

   Puzzle(unsigned char dim, unsigned char regionDim,
		  HorizLine ** const rows, VertLine ** const cols, Region ** const regions) :
		  m_iDim(dim), m_iRegionDim(regionDim), m_pRows(rows), m_pCols(cols),
		  m_pRegions(regions), m_lError(0)
   {
	   init();
   }

   ~Puzzle();


   bool load(string inputFile)
   {

      m_lError = 0;
	  m_pRows=NULL;
	  m_pCols=NULL;
	  m_pRegions=NULL;

      bool res = m_pParser->parse(inputFile);

      res = process_parsed_config();

	  return res;
   }

   Puzzle * const getCopy();

   void printToConsole();


};





}

#endif /* SUDOKU_H_ */
