/*
 * LCS.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: root
 */

#include "LCS.h"

namespace Algorithms {

  LCS::LCS()
  {
  }

  void LCS::print(vector<int> s)
  {
      vector<int> lcs, buf;
      bool asc = false;
      bool desc = false;
      for (int i = 0; i < s.size(); i++)
      {
    	  if (i + 1 < s.size())
    	  {
			  if (s[i+1] - s[i] == 1)
			  {
				 if (desc) // this is a new sequence so reset the buffer
				 {
					 buf.push_back(s[i]);
					 if (buf.size() > lcs.size())
						lcs.assign(buf.begin(),buf.end());

					 buf.clear();
					 desc = false;
				 }

				 buf.push_back(s[i]);

				 asc = true;
			  }
			  else if (s[i] - s[i+1] == 1)
			  {
				 if (asc) // this is a new sequence so reset the buffer
				 {
					 buf.push_back(s[i]);
					 if (buf.size() > lcs.size())
						lcs.assign(buf.begin(),buf.end());

					 buf.clear();
					 asc = false;
				 }

				 buf.push_back(s[i]);

				 desc = true;
			  }
			  else
			  {
				  if (asc || desc)
				  {
					 // this is the end of the current sequence
					 buf.push_back(s[i]);

					 if (buf.size() > lcs.size())
						lcs.assign(buf.begin(),buf.end());
					 else
						buf.clear();

					 asc = false;
					 desc = false;
				  }
			  }
    	  }
    	  else
    	  {
    		  if (asc || desc)
    		  {
    		  	   // this is the end of the current sequence
    		       buf.push_back(s[i]);

    		  	   if (buf.size() > lcs.size())
    		  			lcs.assign(buf.begin(),buf.end());

    		  }


    	  }
      }

      cout << "\n[";
      for (vector<int>::iterator it = lcs.begin(); it != lcs.end(); it++)
      {
    	  cout << *it;
    	  if (it != lcs.end() -1 )
    		  cout << ", ";
      }
      cout << "]\n";

  }


} /* namespace Algorithms */
