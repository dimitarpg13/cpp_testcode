/*
 * Solver.cpp
 *
 *  Created on: Sep 7, 2015
 *      Author: Dimitar Gueorguiev
 */

#include "Solver.h"

namespace sudoku
{
  bool BTSolver::solve()
  {
	  bool res = true;
      res &= process_ranked_candidates(m_mRankedCandidates);
	  return res;
  }

  bool BTSolver::process_ranked_candidates(
		  map<unsigned short,list<pair<Symbol*,list<char> > > > & rankedCandidates)
  {

  }

}



