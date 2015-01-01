/*
 * IndustryTree.h
 *
 *  Created on: Oct 11, 2014
 *      Author: root
 */

#ifndef INDUSTRYTREE_H_
#define INDUSTRYTREE_H_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "IndustryInfo.h"

using namespace std;

namespace Algorithms {

enum PropertyName
{
  CompanyName,
  MarketCap,
  Revenue
};


class IndustryTree {
public:

	IndustryTree(string fileName);
	virtual ~IndustryTree();
	bool addCompanies(string fileName);
	bool addIndustries(string fileName);


    void printIndustries();
    void findCompanies(string industryName, string propertyName);
    void findIndustries(string companyName);


private:
	IndustryInfo* m_pRoot;
	bool parseIndustries (string fileName);
    string getCompanyKey(string name, float marketCap, float revenue);
    void printIndustries(IndustryInfo * industry, string indent);
    void findCompanies(IndustryInfo * industry, set<CompanyInfo*>& companies);

    // the industry key is the industry name. The assumption is that the industry names
    // are unique
    map<string, IndustryInfo*> m_mapIndustries;
    // the company key is the pair "[company name]:[market cap %.2f]:[revenue %.3f]"
    map<string, CompanyInfo*> m_mapCompanies;


    struct CompanyNameComparer {
               bool operator() (CompanyInfo  * lhs, CompanyInfo * rhs) const {
               	 if (lhs->getName().compare(rhs->getName()) < 0)
               		   return true;
               	   else
               		   return false;
               };
    } m_foCompNameCmp;

    struct CompanyMarketCapComparer {
                 bool operator() (CompanyInfo  * lhs, CompanyInfo * rhs) const {
                 	 if (lhs->getMarketCap() < rhs->getMarketCap())
                 		   return true;
                 	   else
                 		   return false;
                 };
    } m_foCompMrktCapCmp;


    struct CompanyRevenueComparer {
                    bool operator() (CompanyInfo  * lhs, CompanyInfo * rhs) const {
                    	 if (lhs->getRevenue() < rhs->getRevenue())
                    		   return true;
                    	   else
                    		   return false;
                    };
    } m_foCompRevenueCmp;

};

} /* namespace Algorithms */

#endif /* INDUSTRYTREE_H_ */
