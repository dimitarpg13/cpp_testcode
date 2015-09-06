/*
 * Region.h
 *
 *  Created on: Sep 5, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef REGION_H_
#define REGION_H_
#include <vector>
#include "Symbol.h"

using namespace std;


namespace sudoku
{

class Symbol;

class Region
{
private:
	vector<Symbol*> m_vSymbols;

};



}



#endif /* REGION_H_ */
