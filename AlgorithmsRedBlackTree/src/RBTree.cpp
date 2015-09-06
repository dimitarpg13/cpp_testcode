/*
 * RBTree.cpp
 *
 *  Created on: Aug 22, 2015
 *      Author: root
 */
#include "RBTree.h"

template <class T>
bool RBTree<T>::insert(long int key, T * val)
{

    RBNode<T> * y = nullptr;
    RBNode<T> * x = m_pRoot;
    RBNode<T> * z = new RBNode<T>(key,val);

    while (x != nullptr)
    {
    	y = x;
        if (x->getKey() > z->getKey())
        	x = x->getLeft();
        else
        	x = x->getRight();
    }
    z->setParent(y);
    if (y == nullptr)
      m_pRoot = z;
    else if (z->getKey() < y->getKey() )
    	y->setLeft(z);
    else
    	y->setRight(z);
    z->setLeft(nullptr);
    z->setRight(nullptr);
    z->setIsRed(true);

    return insertFixup(*z);
}

template <class T>
bool RBTree<T>::insertFixup(RBNode<T> & nd)
{
	RBNode<T> * z = &nd;
    RBNode<T> * y = nullptr;

	// loop until the parent of z is red
    while (z->getParent()->isRed())
    {
        if (z->getParent() == z->getParent().getParent().getLeft())
        { // z's parent is daughter (i.e. the child with less value)
        	y = z->getParent()->getParent()->getRight(); // y is z's uncle
        	if (y->isRed() == true)
        	{ // z's uncle color is red
              z->getParent()->setIsRed(false);
              y->setIsRed(false);
              z->getParent()->getParent()->setIsRed(true); // set the color of the z's grandparent
                                                           // to be red
              z = z->getParent()->getParent(); // set the new z to be the old z's grandparent
            } // z's uncle color is red
            else if (z == z->getParent()->getRight())
            { //z's uncle is black and z is a son (the child with higher value)
        	   z = z->getParent();
        	   leftRotate(*z);
            } //z's uncle is black and z is a son (the child with higher value)
            else
            { //z's uncle is black and z is daughter (the child with the lesser value)
        	   z->getParent()->setIsRed(false);
        	   z->getParent()->getParent()->setIsRed(true);
        	   rightRotate(*z);
            } //z's uncle is black and z is daughter (the child with the lesser value)
        } // z's parent is daughter (i.e. the child with less value)
        else
        { // z's parent is son (i.e. the child with the higher value)
        	y = z->getParent()->getParent()->getLeft(); // y is z's aunt
        	if (y->isRed() == true)
        	{ // z's aunt's color is red
        	    z->getParent()->setIsRed(false);
        	    y->setIsRed(false);
        	    z->getParent()->getParent()->setIsRed(true); // set the color of the z's grandparent
        	                                                 // to be red
        	    z = z->getParent()->getParent(); // set the new z to be the old z's grandparent
        	} // z's aunt's color is red
        	else if (z == z->getParent()->getLeft())
            { //z's aunt is black and z is a daughter (the child with the lesser value)
        	    z = z->getParent();
        	    rightRotate(*z);
        	} //z's aunt is black and z is a daughter (the child with the lesser value)
        	else
        	{ //z's aunt is black and z is a boy (the child with the higher value)
        	    z->getParent()->setIsRed(false);
        	    z->getParent()->getParent()->setIsRed(true);
        	    leftRotate(*z);
        	} //z's aunt is black and z is a boy (the child with the higher value)

        } // z's parent is son (i.e. the child with the higher value)
    }
	return true;
}

template <class T>
T * RBTree<T>::find(long int key)
{
	// TO DO:

	return nullptr;
}

template <class T>
T * erase(long int key)
{
	// TO DO:

	return nullptr;
}

template <class T>
pair<T*,int> RBTree<T>::max()
{
	// TO DO:

	return pair<T*,int>(nullptr,-1);
}

template <class T>
pair<T*,int> RBTree<T>::min()
{
	// TO DO:

	return pair<T*,int>(nullptr,-1);
}


/*
 * 	     x	                    y
 *      / \                    / \
 *     a   y        =>        x   c
 *        / \                / \
 *       b   c              a   b
 *
 */


template <class T>
bool RBTree<T>::leftRotate(RBNode<T>& x)
{
	RBNode<T> * y = x.getRight();
	x.setRight(y->getLeft());
	if (y->getLeft() != nullptr)
		y->getLeft()->setParent(&x);
	y->setParent(x.getParent());
    if (x.getParent() == nullptr)
    	m_pRoot = y;
    else if (x.getParent()->getLeft() == &x)
    	x.getParent()->setLeft(y);
    else
    	x.getParent()->setRight(y);
    y->setLeft(&x);
    x->setParent(y);

	return true;
}


/*
 * 				x                y
 *             / \              / \
 *            y   c     =>     a   x
 *           / \                  / \
 *          a   b                b   c
 *
 */

template <class T>
bool RBTree<T>::rightRotate(RBNode<T>& x)
{
	RBNode<T> * y = x.getLeft();
	x.setLeft(y->getRight());
	if (y->getRight() != nullptr)
		y->setRight()->setParent(&x);
    y->setParent(x.getParent());
    if (x.getParent() == nullptr)
    	m_pRoot = y;
    else if (x.getParent()->getRight() == &x)
        x.getParent()->setRight(y);
    else
    	x.getParent()->setLeft(y);
    y->setRight(&x);
    x->setParent(y);

	return false;
}
