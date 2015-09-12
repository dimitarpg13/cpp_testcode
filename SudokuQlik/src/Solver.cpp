/*
 * Solver.cpp
 *
 *  Created on: Sep 7, 2015
 *      Author: Dimitar Gueorguiev
 */

#include "Solver.h"

namespace sudoku
{
   void BTSolver::print_ranked_candidates(map<unsigned short,rank_list > & c)
   {
	   cout << "Total number of distinct ranks: " << c.size() << endl;
	 	map<unsigned short,rank_list>::iterator itR;
	  	list<pair<Symbol*,list<char>* > >::iterator itRL;
	  	list<char>::iterator itCL;
	  	for (itR = c.begin(); itR != c.end(); itR++)
	  	{
	  		rank_list & rl = itR->second;
            cout << itR->first << " => " << endl;
            for (itRL = rl.begin(); itRL != rl.end(); itRL++)
            {
            	std::cout << "\t{";
            	for (itCL = itRL->second->begin(); itCL != itRL->second->end(); itCL++)
            	{
            		std::cout << *itCL << " ";
            	}
            	std::cout << "}" << endl;
            }
	  	}
   }


  bool BTSolver::solve()
  {
	  bool res = true;
      res &= assign_rank_to_candidates(m_mRankedCandidates);
      if (!res)
    	  return res;

      print_ranked_candidates(m_mRankedCandidates);



	  return res;
  }

  bool BTSolver::process_line_assignments(Line * l, set<char> & assigned)
  {
	  if (l == NULL)
	  {
	     m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	     return false;
	  }

	  Symbol * cur=NULL;
	  for (int i=0; i < l->getDim(); i++)
	  {
	     cur = l->getSymbols()[i];
	     if (cur != NULL)
	     {
	        if (!cur->isEmpty())
	           assigned.insert(cur->getValue());
	     }
	     else
	     {
	        m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	        return false;
	     }
	  }

	  return true;
  }

  bool BTSolver::process_region_assignments(Region * r, set<char> & assigned)
  {
	  if (r == NULL)
	  {
	     m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	     return false;
	  }

	  Symbol * cur=NULL;

	  for (int i=0; i < r->getSymbCount(); i++)
	  {
	     cur = r->getSymbols()[i];
	     if (cur != NULL)
	     {
	        if (!cur->isEmpty())
	           assigned.insert(cur->getValue());
	     }
	     else
	     {
	        m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	        return false;
	     }
	  }

	  return true;
  }


  bool BTSolver::get_available_assignments(Symbol * s,list<char>&  assignments)
  {
	  if (s == NULL)
		  return false;

      set<char> assigned;

      if (!process_line_assignments(s->getRow(),assigned))
    	  return false;

      if (!process_line_assignments(s->getCol(),assigned))
    	  return false;

      if (!process_region_assignments(s->getRegion(),assigned))
    	  return false;

      const set<char>& symbTable = *m_pSrc->getSymbolTable();


      cout << endl;
      print_collection(symbTable.begin(),symbTable.end());
      cout << endl;
      print_collection(assigned.begin(),assigned.end());
      cout << endl;

      set<char> diff;
      set_symmetric_difference(assigned.begin(),assigned.end(),symbTable.begin(),symbTable.end(),inserter(diff,diff.end()));
      print_collection(diff.begin(),diff.end());
      cout << endl;


      assignments.assign(diff.begin(), diff.end());


      return true;
  }


  bool BTSolver::assign_rank_to_candidates(
		  map<unsigned short,rank_list > & rankedCandidates)
  {
     bool res=true;
     if (m_pSol != NULL && m_lError == 0)
     {
    	HorizLine *  curRow=NULL;
    	Symbol * curSymbol=NULL;
    	list<char> * curAssignments = NULL;
    	unsigned short curRank=0;
    	map<unsigned short,rank_list>::iterator itR;
    	rank_list curRankList;
        for (int i = 0; i < m_pSol->getDim(); i++)
        {
           curRow = m_pSol->getRows()[i];
           for (int j = 0; j < m_pSol->getDim(); j++)
           {
        	   if (curRow == NULL)
        	   {
        	       m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
        	      return false;
        	   }

               curSymbol = curRow->getSymbols()[j];
               if (curSymbol == NULL)
               {
                  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
                  return false;
               }

               if (curSymbol->isEmpty())
               {
            	   if (curSymbol->getAssignments() == NULL)
                       curAssignments = new list<char>();
            	   else
            	   {
            		   curAssignments = curSymbol->getAssignments();
            		   curAssignments->clear();
            	   }

                   get_available_assignments(curSymbol,*curAssignments);
                   curSymbol->setAssignments(curAssignments);
                   curRank = (unsigned short) curAssignments->size();

                   itR = rankedCandidates.find(curRank);
                   if (itR != rankedCandidates.end())
                   {
                	   itR->second.push_back(rank_pair(curSymbol,curAssignments));
                   }
                   else
                   {
                	   curRankList.clear();
                	   curRankList.push_back(rank_pair(curSymbol,curAssignments));
                       rankedCandidates.insert(pair<unsigned short,rank_list>(curRank,curRankList));
                   }
               }
           }

        }
     }
     else
    	 res=false;

     return res;
  }

  bool BTSolver::update_ranked_list(map<unsigned short,rank_list > & c)
  {
	  bool res = true;

	  return res;
  }

  bool BTSolver::solve_internal(map<unsigned short,rank_list > & c)
  {
      bool res = true;
	  map<unsigned short, rank_list>::iterator itC;
	  list<rank_pair>::iterator itRL;
	  list<char>::iterator itCL;
	  Symbol * curSymbol=NULL;
	  list<char> * curAssignments=NULL;
	  for (itC = c.begin(); itC != c.end(); itC++)
	  {
          for (itRL = itC->second.begin(); itRL != itC->second.end(); itRL++)
          {
              curSymbol = itRL->first;
              if (curSymbol == NULL)
              {
            	  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
            	  return false;
              }
              if (curSymbol->isEmpty())
              {

              }

          }

	  }

	  return res;
  }


}



