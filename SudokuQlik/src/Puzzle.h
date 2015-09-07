/*
 * Problem.h
 *
 *  Created on: Sep 5, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include "Symbol.h"
#include "Line.h"
#include "Region.h"
#include "Parser.h"

using namespace std;

namespace sudoku
{

class Puzzle
{

private:
   int m_iDim;

   HorizLine ** m_pRows;
   VertLine ** m_pCols;
   Region ** m_pRegions;
   Parser * m_pParser;

   long long m_lError;

   void init()
   {
       m_pParser = new Parser(m_iDim,'.');
   }

   bool process_parsed_config();

public:
   static const int CLASSIC_SUDOKU_DIM = 9;

   Puzzle() : m_pRows(NULL), m_pCols(NULL), m_pRegions(NULL), m_lError(0)
   {
	  m_iDim = CLASSIC_SUDOKU_DIM;

	  init();
   }

   Puzzle(int dim) : m_iDim(dim), m_pRows(NULL), m_pCols(NULL), m_pRegions(NULL), m_lError(0)
   {
       init();
   }

   ~Puzzle()
   {

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

#endif /* PUZZLE_H_ */
