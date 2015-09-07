/*
 * Puzzle.cpp
 *
 *  Created on: Sep 6, 2015
 *      Author: Dimitar Gueorguiev
 */

#include "Puzzle.h"

namespace sudoku
{

bool Puzzle::process_parsed_config()
{
   bool res = true;

   m_lError |= m_pParser->getError();

   if (m_lError == SUDOKU_NO_ERROR)
   {
      m_pRows = m_pParser->getRows();
      m_pCols = m_pParser->getCols();



   }
   else
	   res = false;

   return res;

}

}
