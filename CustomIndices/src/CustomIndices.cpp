//============================================================================
// Name        : CustomIndices.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>

#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

const int max_quotes = 10000000;
const int max_indices = 10000;
const int max_comp = 10000;

struct cindex;
struct stock
{
	stock (std::string name_) : name(name_), value(-1.0) {};

	std::string name;
	virtual bool composite() { return false; }
	double value;
	std::vector<cindex *> derivatives; // any index which includes this component
	virtual ~stock() {};
};

struct cindex : stock
{
	cindex(std::string name_) : stock(name_), plus(false) {};

	bool composite() { return true; }
	bool plus;
	std::vector<stock *> components; // either stock or another comp index
    std::vector<std::string> undefined; // container for all undefined components
};



std::map<std::string,stock *> squotes;
std::map<std::string,cindex *> ciquotes;
std::map<std::string,std::stack<cindex *> > undefined;

bool process_quote(std::vector<std::string>::const_iterator qbegin, std::vector<std::string>::const_iterator qend)
{

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
              cur->components.push_back(s);

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

                	ci->components.assign(cires->second->components.begin(),cires->second->components.end());

                }
	        }
	        else
	        {  // the referenced component is a stock
	           // so add it to the component vector of the current index
	        	ci->components.push_back(sres->second);
	        }


		   cbegin++;
	    }



	    if (referenced) // the current component has already been referenced
	    {               // and we need to fix the references
	        std::stack<cindex*> & st = itref->second;
	        cindex * cur = NULL;

	        while (!st.empty())
	        {
	           cur = st.top();
	           cur->components.insert(cur->components.end(),ci->components.begin(), ci->components.end());

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

bool process_input_line(std::vector<std::string> & quote)
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

	  process_quote(++quote.begin(),quote.end());
  }
  else
	   process_config(++quote.begin(),quote.end());

   return true;
}


int main (int argc, char ** argv) {
    // Initialization
    //
    // In this part of the program you can perform any kind of
    // initialization routine before processing the stream
    // of data.

	std::ifstream ifs;

	ifs.open (argv[1], std::ifstream::in);

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
            process_input_line(quote);


        }
    }
    // Step 2: Output
    //
    // Print the Custom Indices value.

    return 0;
}
