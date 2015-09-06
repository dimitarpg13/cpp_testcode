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

   HorizLine ** m_pHorizLineArr;
   VertLine ** m_pVertLineArr;
   Region ** m_pRegionArr;
   Parser * m_pParser;

   void init()
   {
	   m_pHorizLineArr = new HorizLine* [m_iDim];
       m_pVertLineArr = new VertLine* [m_iDim];
       m_pRegionArr = new Region* [m_iDim];

       for (int i = 0; i < m_iDim; i++)
       {
    	   m_pHorizLineArr[i] = new HorizLine();
    	   m_pVertLineArr[i] = new VertLine();
    	   m_pRegionArr[i] = new Region();
       }

       m_pParser = new Parser('.', m_iDim);

   }

public:
   static const int CLASSIC_SUDOKU_DIM = 9;

   Puzzle()
   {
	  m_iDim = CLASSIC_SUDOKU_DIM;

	  init();
   }

   Puzzle(int dim) : m_iDim(dim)
   {
       init();
   }

   ~Puzzle()
   {
	   for (int i = 0; i < m_iDim; i++)
		   delete m_pHorizLineArr[i];
	   delete [] m_pHorizLineArr;

	   for (int i = 0; i < m_iDim; i++)
		   delete m_pVertLineArr[i];
	   delete [] m_pVertLineArr;

	   for (int i = 0; i < m_iDim; i++)
		   delete m_pRegionArr[i];
	   delete [] m_pRegionArr;


	   delete m_pParser;

   }

   bool load(string inputFile)
   {
      bool res = m_pParser->parse(inputFile);


	  return res;
   }


};

}

#endif /* PUZZLE_H_ */
