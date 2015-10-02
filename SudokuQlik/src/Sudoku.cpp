/*
 * Sudoku.cpp
 *
 *  Created on: Sep 7, 2015
 *      Author: Dimitar Gueorguiev
 */

#include "Sudoku.h"


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



Parser::Parser(unsigned char dim, unsigned char regionDim, char sep, char eol) :
		m_iDim(dim), m_iRegionDim(regionDim), m_cSep(sep), m_cEol(eol),
		m_lError(0), m_pRows(NULL), m_pCols(NULL), m_pRegions(NULL)
{
	m_pSymbols = new set<char>(symbolTable,symbolTable+dim);
}

Parser::~Parser()
{
    delete m_pSymbols;
};


void Parser::init()
{
	m_pRows = new HorizLine* [m_iDim];
	m_pCols = new VertLine* [m_iDim];
	m_pRegions = new Region* [m_iDim];
	for (int i=0; i<m_iDim; i++)
	{
		m_pRows[i] = NULL;
		m_pCols[i] = NULL;
		m_pRegions[i] = NULL;
	}

	m_lError=0;
}

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

void Parser::cleanup(unsigned char rowCount, unsigned char colCount, unsigned char regCount)
{
	if (m_pRows != NULL)
	{
	   for (unsigned short i = 0; i < rowCount; i++)
		 if (m_pRows[i] != NULL)
	      delete m_pRows[i];
	   delete [] m_pRows;
	}

	if (m_pCols != NULL)
	{
	   for (unsigned short i = 0; i < colCount; i++)
		 if (m_pCols[i] != NULL)
	       delete m_pCols[i];
	   delete [] m_pCols;
	}

	if (m_pRegions != NULL)
	{
	   for (unsigned short i = 0; i < regCount; i++)
		 if (m_pRegions[i] != NULL)
	       delete m_pRegions[i];
	   delete [] m_pRegions;
	}

	m_pRows = NULL;
	m_pCols = NULL;
	m_pRegions = NULL;
}

// input:
//   symbolRowIdx - the row index of the current symbol
//   symbolColIdx - the col index of the current symbol
// return:
//   the index of the region in which the current symbol is in
//
unsigned char Parser::getRegionIdx(unsigned char symbolRowIdx, unsigned char symbolColIdx)
{
   unsigned char M = m_iDim / m_iRegionDim; // the number of regions spanning a single line
   unsigned char regRowIdx = symbolRowIdx / m_iRegionDim;
   unsigned char regColIdx = symbolColIdx / m_iRegionDim;
   return regRowIdx * M + regColIdx;
}

bool Parser::parse(string inputFile)
{
	bool res=true;
	unsigned char M = m_iDim / m_iRegionDim;
	cleanup(m_iDim, m_iDim, M*M );

	ifstream fs (inputFile.c_str(), std::ifstream::in);

	char c = fs.get();
    if (fs.bad())
    {
      m_lError |= SUDOKU_ERROR_INCORRECT_INPUT_FORMAT;
      return false;
    }

    init();

	unsigned char curRowIdx = 0, curColIdx = 0, curRegIdx = 0;

	Symbol * curSymbol = NULL;
    HorizLine * curRow = NULL;
    VertLine * curCol = NULL;
    Region * curRegion = NULL;

	while (fs.good())
	{

	  if (curRow == NULL)
#ifndef _DEBUG
		  curRow = new HorizLine(m_iDim, m_iRegionDim);
#else
	      curRow = new HorizLine(m_iDim, m_iRegionDim, curRowIdx);
#endif

	  if (m_pCols[curColIdx] != NULL)
		  curCol = m_pCols[curColIdx];
	  else
	  {
#ifndef _DEBUG
		  curCol = new VertLine(m_iDim, m_iRegionDim);
#else
		  curCol = new VertLine(m_iDim, m_iRegionDim, curColIdx);
#endif
		  m_pCols[curColIdx] = curCol;
	  }

	  curRegIdx = getRegionIdx(curRowIdx, curColIdx);
	  if (m_pRegions[curRegIdx] != NULL)
	    curRegion = m_pRegions[curRegIdx];
	  else
	  {
	    curRegion = new Region(m_iDim,m_iRegionDim);
	    m_pRegions[curRegIdx] = curRegion;
	  }

	  if ( is_symbol(c) )
	  {
		  curSymbol = new Symbol(c,curRow,curCol);
          curRow->addSymbol(curSymbol);
          if (curColIdx % m_iRegionDim == 0)
        	  curRow->addRegion(curRegion);

          curCol->addSymbol(curSymbol);
          if (curRowIdx % m_iRegionDim == 0)
        	  curCol->addRegion(curRegion);

          curRegion->addSymbol(curSymbol);
          if (curColIdx == 0)
        	  curRegion->addRow(curRow);

          curRegion->addCol(curCol);

          curColIdx++;
	  }
	  else if ( is_separator(c) )
	  {
		  curSymbol = new Symbol(curRow,curCol);
          curRow->addSymbol(curSymbol);
          if (curColIdx % m_iRegionDim == 0)
             curRow->addRegion(curRegion);

          curCol->addSymbol(curSymbol);
          if (curRowIdx % m_iRegionDim == 0)
             curCol->addRegion(curRegion);

          curRegion->addSymbol(curSymbol);
          if (curColIdx == 0)
        	  curRegion->addRow(curRow);

          curRegion->addCol(curCol);

          curColIdx++;
	  }
	  else if ( is_end_of_line(c) )
	  {
		 m_pRows[curRowIdx] = curRow;
		 curRow = NULL;
		 curRowIdx++;
		 curColIdx = 0;
	  }

#ifdef _DEBUG
	  std::cout << c;
#endif

	  c = fs.get();
	}

	std::cout << endl;

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

        cleanup(curRowIdx, curColIdx, curRegIdx);

        res=false;
    }

    if (curColIdx != m_iDim)
    {
    	m_lError |= SUDOKU_ERROR_INCORRECT_INPUT_COL_COUNT;

    	cleanup(curRowIdx, curColIdx, curRegIdx);

    	res=false;
    }

	return res;
}


Puzzle::~Puzzle()
{
    if (m_pRows != NULL)
	{
	  for (int i = 0; i < m_iDim; i++)
		 if (m_pRows[i] != NULL)
			delete m_pRows[i];
	  delete [] m_pRows;
	}

	if (m_pCols != NULL)
	{
	  for (int i = 0; i < m_iDim; i++)
		 if (m_pCols[i] != NULL)
			delete m_pCols[i];
	  delete [] m_pCols;
	}

	if (m_pRegions != NULL)
	{
	   for (int i = 0; i < m_iDim; i++)
	     if (m_pRegions[i] != NULL)
		    delete m_pRegions[i];
	   delete [] m_pRegions;
	}

	if (m_pParser != NULL)
	   delete m_pParser;

}


bool Puzzle::process_parsed_config()
{
   bool res = true;

   m_lError |= m_pParser->getError();

   if (m_lError == SUDOKU_NO_ERROR)
   {
      m_pRows = m_pParser->getRows();
      m_pCols = m_pParser->getCols();
      m_pRegions = m_pParser->getRegions();
   }
   else
	   res = false;

   return res;

}


Puzzle * const Puzzle::getCopy()
{
	HorizLine ** copyRows = new HorizLine* [m_iDim];
	VertLine ** copyCols = new VertLine* [m_iDim];
	Region ** copyRegions = new Region* [m_iDim];
	for (int i=0; i<m_iDim; i++)
	{
		copyRows[i] = NULL;
		copyCols[i] = NULL;
		copyRegions[i] = NULL;
	}

    HorizLine * curRow=NULL, *copyRow=NULL;
    VertLine * copyCol=NULL;
    Region * copyRegion=NULL;
    Symbol * curSymbol=NULL, * copySymbol=NULL;

    int k=0; // region index
    for (int i=0; i < m_iDim; i++) // i - row indx
    {
    	curRow = m_pRows[i];
#ifndef _DEBUG
  	    copyRow = new HorizLine(m_iDim,m_iRegionDim);
#else
  	    copyRow = new HorizLine(m_iDim,m_iRegionDim,(unsigned char) i);
#endif
        copyRows[i] = copyRow;

        for (int j=0; j < m_iDim; j++) // j - col indx
        {
      	    if (copyCols[j] != NULL)
      	    	copyCol = copyCols[j];
      	    else
      	    {
#ifndef _DEBUG
      	    	copyCol = new VertLine(m_iDim,m_iRegionDim);
#else
      	    	copyCol = new VertLine(m_iDim,m_iRegionDim,(unsigned char) j);
#endif
      	    	copyCols[j] = copyCol;
      	    }

      	    k = m_pParser->getRegionIdx(i,j);
      	    if (copyRegions[k] != NULL)
      	    	copyRegion = copyRegions[k];
      	    else
      	    {
      	    	copyRegion = new Region(m_iDim,m_iRegionDim);
      	    	copyRegions[k] = copyRegion;
      	    }

            curSymbol = curRow->getSymbols()[j];
            if (curSymbol->isEmpty())
               copySymbol = new Symbol(copyRow,copyCol,copyRegion);
            else
               copySymbol = new Symbol(curSymbol->getValue(),copyRow,copyCol,copyRegion);
            copyRow->addSymbol(copySymbol);
            if (j % m_iRegionDim == 0)
                copyRow->addRegion(copyRegion);

            copyCol->addSymbol(copySymbol);
            if (i % m_iRegionDim == 0)
                copyCol->addRegion(copyRegion);

            copyRegion->addSymbol(copySymbol);
            if (j == 0)
               copyRegion->addRow(copyRow);

            copyRegion->addCol(copyCol);

        }

    }

    return new Puzzle(m_iDim,m_iRegionDim,copyRows,copyCols,copyRegions);

}


void Puzzle::printToConsole()
{
	HorizLine * curRow=NULL;
	Symbol * curSymbol=NULL;
	char sep = ' ';
    for (int i=0; i < m_iDim; i++)
    {
       curRow = m_pRows[i];
       for (int j=0; j < m_iDim; j++)
       {
           curSymbol = curRow->getSymbols()[j];
           if (!curSymbol->isEmpty())
              cout << curSymbol->getValue();
           else
        	   cout << ".";
           if (j < m_iDim - 1)
        	   cout << sep;
       }
       cout << endl;

    }

}


}
