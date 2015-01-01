/*
 * Listcontainer.h
 *
 *  Created on: Oct 4, 2014
 *      Author: root
 */

#ifndef LISTCONTAINER_H_
#define LISTCONTAINER_H_

#include <initializer_list>
#include <list>
#include "Container.h"

using namespace std;

namespace Algorithms {

class List_container : public Container {
public:
	List_container();
	List_container(initializer_list<double> il) : ld(il) { }
    double& operator[] (int i);
    int size() const { return ld.size(); }
	virtual ~List_container();
private:
	list<double> ld;

};

} /* namespace Algorithms */

#endif /* LISTCONTAINER_H_ */
