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
	std::vector<cindex *> derivatives; // any index which includes this component
	virtual ~stock() {};
};

struct cindex : stock
{
	cindex(std::string name_) : stock(name_), plus(false) {};
    bool plus;
	bool composite() { return true; }
	std::vector<stock *> components_add; // either stock or another comp index
	std::vector<stock *> components_subtract; // either stock or another comp index
    std::vector<std::string> undefined; // container for all undefined components
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

	std::vector<cindex*> & deriv = sres->second->derivatives;


	for (std::vector<cindex*>::iterator itder = deriv.begin(); itder != deriv.end(); itder++)
	{
		bool not_avail = false;
		double cival = 0.0;

		for (std::vector<stock*>::iterator itcompa = (*itder)->components_add.begin(); itcompa != (*itder)->components_add.end(); itcompa++)
		{
			if ((*itcompa)->value == NOT_AVAIL)
			{
				not_avail = true;
				break;
			}

		    cival += (*itcompa)->value;
		}

		if (!not_avail)
		{
			for (std::vector<stock*>::iterator itcomps = (*itder)->components_subtract.begin(); itcomps != (*itder)->components_subtract.end(); itcomps++)
			{
				if ((*itcomps)->value == NOT_AVAIL)
				{
				   not_avail = true;
				   break;
				}
			    cival -= (*itcomps)->value;
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

       if (referenced) // the current component has already been referenced
       {               // and we need to fix the references
    	   std::stack<cindex*> & st = itref->second;
    	   cindex * cur = NULL;
    	   while (!st.empty())
    	   {
    		  cur = st.top();
    		  if (cur->plus)
                 cur->components_add.push_back(s);
    		  else
    		  {
    			  if (cur->components_add.size() == 0)
    				  cur->components_add.push_back(s);
    			  else
    				  cur->components_subtract.push_back(s);
    		  }

              s->derivatives.push_back(cur);

              std::vector<std::string>::iterator itdel;
        	  itdel = std::find(cur->undefined.begin(),cur->undefined.end(),s->name);
        	  if (itdel != cur->undefined.end())
        	      cur->undefined.erase(itdel);

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
                   ci->undefined.push_back(*cbegin);
                }
                else
                { // the referenced component is an index
                  // so add its components to the component vector of the current index


                	if (ci->plus)
                	{

                	   ci->components_add.insert(ci->components_add.end(),cires->second->components_add.begin(),cires->second->components_add.end());
                	   ci->components_subtract.insert(ci->components_subtract.end(),cires->second->components_subtract.begin(),cires->second->components_subtract.end());
                	}
                	else
                	{
                		if (ci->components_add.size() == 0)
                		{
                		   ci->components_add.insert(ci->components_add.end(),cires->second->components_add.begin(),cires->second->components_add.end());
                		   ci->components_subtract.insert(ci->components_subtract.end(),cires->second->components_subtract.begin(),cires->second->components_subtract.end());
                		}
                		else
                		{
                		   ci->components_subtract.insert(ci->components_subtract.end(),cires->second->components_add.begin(),cires->second->components_add.end());
                		   ci->components_add.insert(ci->components_add.end(),cires->second->components_subtract.begin(),cires->second->components_subtract.end());
                		}
                	}

                	// update each of the components of the referenced index to
                	// point to the current composite index through its derivatives array
                	for (std::vector<stock *>::iterator it = cires->second->components_add.begin(); it != cires->second->components_add.end(); it++)
                	{
                        (*it)->derivatives.push_back(ci);
                	}

                	for (std::vector<stock *>::iterator it = cires->second->components_subtract.begin(); it != cires->second->components_subtract.end(); it++)
                	{
                	   (*it)->derivatives.push_back(ci);
                	}
                }
	        }
	        else
	        {  // the referenced component is a stock
	           // so add it to the component vector of the current index
	        	if(ci->plus)
	        	   ci->components_add.push_back(sres->second);
	        	else
	        	{
	        		if (ci->components_add.size() == 0)
	        			ci->components_add.push_back(sres->second);
	        		else
	        			ci->components_subtract.push_back(sres->second);
	        	}
	        	sres->second->derivatives.push_back(ci);
	        }


		   cbegin++;
	    }



	    if (referenced) // the current composite index ci has already been referenced
	    {               // and we need to fix the references
	        std::stack<cindex*> & st = itref->second;
	        cindex * cur = NULL;

	        while (!st.empty())
	        {
	           cur = st.top();
	           if (ci->plus)
	               cur->components_add.insert(cur->components_add.end(),ci->components_add.begin(), ci->components_add.end());
	           else
	           {
	        	   cur->components_add.insert(cur->components_add.end(),ci->components_add.begin(), ci->components_add.end());
	        	   cur->components_subtract.insert(cur->components_subtract.end(),ci->components_subtract.begin(), ci->components_subtract.end());
	           }


	           ci->derivatives.push_back(cur);

	           std::vector<std::string>::iterator itdel;
	           itdel = std::find(cur->undefined.begin(),cur->undefined.end(),ci->name);
	           if (itdel != cur->undefined.end())
	        	   cur->undefined.erase(itdel);

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
