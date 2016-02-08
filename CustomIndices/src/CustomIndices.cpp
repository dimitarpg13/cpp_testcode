//============================================================================
// Name        : CustomIndices.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <algorithm>

const int MAX_QUOTES = 10000000;
const int MAX_INDICES = 10000;
const int MAX_COMP = 10000;

const int NOT_AVAIL = -1.0;

struct cindex;
struct stock
{
	stock (std::string name_) : name(name_), value(NOT_AVAIL) {};

	std::string name;
	virtual bool composite() { return false; }
	double value;
	std::set<cindex *> derivatives; // any index which includes this component
	virtual ~stock() {};
};

struct cindex : stock
{
	cindex(std::string name_) : stock(name_), plus(false) {};
    bool plus;
	bool composite() { return true; }
	std::map<stock *,int> components_add; // either stock or another comp index
	std::map<stock *,int> components_subtract; // either stock or another comp index
    std::set<std::string> undefined; // container for all undefined components
};



std::map<std::string,stock *> squotes;
std::map<std::string,cindex *> ciquotes;
std::map<std::string,std::stack<cindex *> > undefined;


bool is_float( std::string str) {
    std::istringstream iss(str);
    float f;
    iss >> std::noskipws >> f; // leading whitespace invalid
    // checks the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

bool process_quote(std::vector<std::string>::const_iterator qbegin, std::vector<std::string>::const_iterator qend, std::map<std::string,cindex*> & output)
{

	std::string name = *qbegin;
	std::map<std::string,stock*>::iterator sres = squotes.find(name);
	if (sres == squotes.end())
		return false;  // bad quote

	std::string sval = *++qbegin;
	if (!is_float(sval))
		return false;

	double val = atof(sval.c_str());

	sres->second->value = val;

	std::set<cindex*> & deriv = sres->second->derivatives;


	std::cout << "derivative count : " << deriv.size() << std::endl;



	for (std::set<cindex*>::iterator itder = deriv.begin(); itder != deriv.end(); itder++)
	{
		bool not_avail = false;
		double cival = 0.0;

		//std::cout << "derivative name: " << (*itder)->name << std::endl;
        //std::cout << "component_add count: " << (*itder)->components_add.size() << std::endl;
        //std::cout << "component_subtract count: " << (*itder)->components_subtract.size() << std::endl;

		for (std::map<stock*,int>::iterator itcompa = (*itder)->components_add.begin(); itcompa != (*itder)->components_add.end(); itcompa++)
		{
			if (itcompa->first->value == NOT_AVAIL)
			{
				not_avail = true;
				break;
			}

		    cival += itcompa->first->value * itcompa->second;
		}

		if (!not_avail)
		{
			for (std::map<stock*,int>::iterator itcomps = (*itder)->components_subtract.begin(); itcomps != (*itder)->components_subtract.end(); itcomps++)
			{
				if (itcomps->first->value == NOT_AVAIL)
				{
				   not_avail = true;
				   break;
				}
			    cival -= itcomps->first->value * itcomps->second;
			}

			if (!not_avail)
			{
			   (*itder)->value = cival;
			   //std::cout << (*itder)->name << ": " << cival << std::endl;
			   std::pair<std::map<std::string,cindex*>::iterator,bool> res = output.insert(std::make_pair((*itder)->name,*itder));
               if (res.second==false)
            	  res.first->second->value = cival;
			}
		}


	}




	return true;
};




bool process_config(std::vector<std::string>::const_iterator cbegin, std::vector<std::string>::const_iterator cend)
{
	std::string name = *cbegin++;
    std::string op = *cbegin++;

    // check if the new component has already been referenced
    bool referenced = false;
    std::map<std::string,std::stack<cindex*> >::iterator itref = undefined.find(name);
    if (itref != undefined.end())
    	referenced = true;

    if (op == "S")
    {
       stock * s = new stock(name);
       std::pair<std::map<std::string,stock*>::iterator,bool> res = squotes.insert(std::make_pair(name,s));
       if (res.second == false)
       {
    	   delete s;
    	   return false; // bad input file - no stock redefinition is allowed
       }
       std::pair<std::map<stock *,int>::iterator,bool> resas;
       if (referenced) // the current component has already been referenced
       {               // and we need to fix the references
    	   std::stack<cindex*> & st = itref->second;
    	   cindex * cur = NULL;
    	   while (!st.empty())
    	   {
    		  cur = st.top();
    		  if (cur->plus)
    		  {
                 resas = cur->components_add.insert(std::make_pair(s,1));
                 if (!resas.second)
                	 resas.first->second++;
    		  }
    		  else
    		  {
    			  if (cur->components_add.size() == 0)
    			  {
    				  resas = cur->components_add.insert(std::make_pair(s,1));
    				  if (!resas.second)
    				     resas.first->second++;
    			  }
    			  else
    			  {
    				  resas = cur->components_subtract.insert(std::make_pair(s,1));
    				  if (!resas.second)
    				       resas.first->second++;
    			  }
    		  }

              s->derivatives.insert(cur);



        	  cur->undefined.erase(s->name);

              st.pop();
    	   }

    	   undefined.erase(itref);
       }
    }
    else
    {
        cindex * ci = new cindex(name);
        std::pair<std::map<std::string,cindex*>::iterator,bool> res = ciquotes.insert(std::make_pair(name,ci));
        if (res.second == false)
        {
         	delete ci;
         	return false; // bad input file - no index redefinition is allowed
        }

        if (op == "+")
    	   ci->plus=true;


        std::map<std::string,stock*>::iterator sres;
        std::map<std::string,cindex*>::iterator cires;
        std::pair<std::map<std::string,std::stack<cindex*> >::iterator, bool> unres;
	    while (cbegin != cend)
	    {
	    	sres = squotes.find(*cbegin);
	        if (sres == squotes.end())
	        {
	        	cires = ciquotes.find(*cbegin);
                if (cires == ciquotes.end())
                { // the referenced component
                  // is not defined yet which
                  // creates a small problem
                   std::stack<cindex*> st;
                   st.push(ci);
                   unres = undefined.insert(std::make_pair(*cbegin,st));
                   if (!unres.second)
                   {  // there are more composites which contain the same
                	  // undefined component so push the current composite on
                	  // the undefined component's stack
                	  unres.first->second.push(ci);
                   }
                   ci->undefined.insert(*cbegin);
                }
                else
                { // the referenced component is an index
                  // so add its components to the component vector of the current index


                	std::pair<std::map<stock *,int>::iterator,bool> resas;
                	if (ci->plus)
                	{
                		for (std::map<stock*,int>::iterator ita = cires->second->components_add.begin(); ita != cires->second->components_add.end(); ita++)
                		{
                	       resas = ci->components_add.insert(*ita);
                	       if (!resas.second)
                	        resas.first->second++;
                		}
                		for (std::map<stock*,int>::iterator its = cires->second->components_subtract.begin(); its != cires->second->components_subtract.end(); its++)
                		{
                	        resas = ci->components_subtract.insert(*its);
                	        if (!resas.second)
                	          resas.first->second++;
                		}
                	}
                	else
                	{
                		if (ci->components_add.size() == 0)
                		{
                			for (std::map<stock*,int>::iterator ita = cires->second->components_add.begin(); ita != cires->second->components_add.end(); ita++)
                			{
                		        resas = ci->components_add.insert(*ita);
                		        if (!resas.second)
                		          resas.first->second++;
                			}

                			for (std::map<stock*,int>::iterator its= cires->second->components_subtract.begin(); its != cires->second->components_subtract.end(); its++)
                			{
                		       resas = ci->components_subtract.insert(*its);
                		       if (!resas.second)
                		         resas.first->second++;
                			}
                		}
                		else
                		{

                			for (std::map<stock*,int>::iterator ita = cires->second->components_add.begin(); ita != cires->second->components_add.end(); ita++)
                			{
                		        resas = ci->components_subtract.insert(*ita);
                		        if (!resas.second)
                		           resas.first->second++;
                			}

                			for (std::map<stock*,int>::iterator its= cires->second->components_subtract.begin(); its != cires->second->components_subtract.end(); its++)
                			{
                		       resas = ci->components_add.insert(*its);
                		       if (!resas.second)
                		           resas.first->second++;
                			}
                		}
                	}

                	// update each of the components of the referenced index to
                	// point to the current composite index through its derivatives array
                	for (std::map<stock *,int>::iterator it = cires->second->components_add.begin(); it != cires->second->components_add.end(); it++)
                	{
                        it->first->derivatives.insert(ci);
                	}

                	for (std::map<stock *,int>::iterator it = cires->second->components_subtract.begin(); it != cires->second->components_subtract.end(); it++)
                	{
                	   it->first->derivatives.insert(ci);
                	}
                }
	        }
	        else
	        {  // the referenced component is a stock
	           // so add it to the component vector of the current index
	        	std::pair<std::map<stock *,int>::iterator,bool> resas;
	        	if(ci->plus)
	        	{
	        	   resas = ci->components_add.insert(std::make_pair(sres->second,1));
	        	   if (!resas.second)
	        	      resas.first->second++;
	        	}
	        	else
	        	{
	        		if (ci->components_add.size() == 0)
	        			ci->components_add.insert(std::make_pair(sres->second,1));
	        		else
	        		{
	        			resas = ci->components_subtract.insert(std::make_pair(sres->second,1));
	        			if (!resas.second)
	        			  resas.first->second++;
	        		}
	        	}
	        	sres->second->derivatives.insert(ci);
	        }


		   cbegin++;
	    }



	    if (referenced) // the current composite index ci has already been referenced
	    {               // and we need to fix the references
	        std::stack<cindex*> & st = itref->second;
	        cindex * cur = NULL;
	        std::pair<std::map<stock *,int>::iterator,bool> resas;
	        while (!st.empty())
	        {
	           cur = st.top();
	           if (ci->plus)
	           {
	        	   for (std::map<stock*,int>::iterator ita = ci->components_add.begin(); ita != ci->components_add.end(); ita++)
	        	   {
	        	       resas = cur->components_add.insert(*ita);
	        	       if (!resas.second)
	        	         resas.first->second++;
	        	   }

	           }
	           else
	           {
	        	  /* for (std::map<stock*,int>::iterator ita = ci->components_add.begin(); ita != ci->components_add.end(); ita++)
	        	   {
	        	  	   resas = cur->components_add.insert(ita);
	        	  	   if (!resas.second)
	        	  	      resas.first->second++;
	        	  }
	        	   resas = cur->components_subtract.insert(ci->components_subtract.begin(), ci->components_subtract.end());
	        	   if (!resas.second)
	        	   	   resas.first->second++;*/
	           }


	           ci->derivatives.insert(cur);



	           cur->undefined.erase(ci->name);

	           st.pop();
	        }

	        undefined.erase(itref);
	     }

    }

	return true;
};

bool consistency_test(std::map<std::string,cindex *> & ciquotes, std::map<std::string,std::stack<cindex *> > & undefined)
{

	if (undefined.size() > 0)
		return false;

	for (std::map<std::string,cindex *>::const_iterator it = ciquotes.begin(); it != ciquotes.end(); it++)
		if (it->second->undefined.size() > 0)
			return false;

	return true;
};

bool process_input_line(std::vector<std::string> & quote,  std::map<std::string,cindex*> & output)
{
   if (quote.size() < 2)
		return false;

   bool end_of_config=false;

  if (quote[0] == "Q")
  {
	  if (!end_of_config)
	  {
		 if (!consistency_test(ciquotes,undefined))
			 return false; // error: inconsistent configuration
		 end_of_config = true;
	  }

	  if (!process_quote(++quote.begin(),quote.end(),output))
		  return false;
  }
  else
  {
	  if (!process_config(++quote.begin(),quote.end()))
		  return false;
  }

   return true;
}

void print_output(std::map<std::string,cindex*> & output)
{
	std::cout.precision(2);
	//std::cout << std::fixed;
	for (std::map<std::string,cindex*>::const_iterator it = output.begin(); it != output.end(); it++)
		std::cout <<  std::fixed << it->second->name << ": " << it->second->value << std::endl;
}

int main (int argc, char ** argv) {
    // Initialization
    //
    // In this part of the program you can perform any kind of
    // initialization routine before processing the stream
    // of data.

	std::ifstream ifs;

	ifs.open (argv[1], std::ifstream::in);

	std::map<std::string,cindex*> output;

    std::string line;
    std::vector<std::string> quote;
    std::string item;

    while(std::getline(ifs, line)) {
        if(! line.empty()) {
            quote.clear();
            std::istringstream ss(line);
            while (std::getline(ss, item, '|')) {
                quote.push_back(item);
            }
            // Step 1: Input.
            //
            // Process the new line on the stdin.
            process_input_line(quote,output);


        }
    }
    // Step 2: Output
    //
    // Print the Custom Indices value.
    print_output(output);

    return 0;
}
