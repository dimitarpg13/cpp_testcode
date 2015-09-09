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

  void BTSolver::get_list_of_assignments(Symbol * s,list<char> & assignments)
  {

  }


  bool BTSolver::process_ranked_candidates(
		  map<unsigned short,list<pair<Symbol*,list<char> > > > & rankedCandidates)
  {
     bool res=true;
     if (m_pSol != NULL && m_lError == 0)
     {
    	HorizLine *  curRow=NULL;
    	Symbol * curSymbol=NULL;
    	list<char> curAssignments;
        for (int i = 0; i < m_pSol->getDim(); i++)
        {
           curRow = m_pSol->getRows()[i];
           for (int j = 0; j < m_pSol->getDim(); j++)
           {
               curSymbol = curRow->getSymbols()[j];
               if (curSymbol->isEmpty())
               {
                   curAssignments.clear();
                   get_list_of_assignments(curSymbol,curAssignments);
               }
           }

        }
     }
     else
    	 res=false;

     return res;
  }




}



