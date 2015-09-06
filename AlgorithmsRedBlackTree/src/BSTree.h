/*
 * BSTree.h
 *
 *  Created on: Aug 22, 2015
 *      Author: root
 */

#ifndef BSTREE_H_
#define BSTREE_H_

#include <utility>

using namespace std;


template <class T>
class BSNode
{
private:
	T * m_pData;
	int m_iKey;
    RBNode * m_pParent;
    RBNode * m_pLeft;
    RBNode * m_pRight;
    int m_iBlackHeight; // we may need to use it to preserve the current value of black height
    					// the number of ancestors which a black up to the top
public:
    bool isRed() { return m_bRed; };
    void setIsRed(bool red) { m_bRed = red; };

    int getKey() { return m_iKey; };
    RBNode * getParent() { return m_pParent; };
    RBNode * getLeft() { return m_pLeft; };
    RBNode * getRight() { return m_pRight; };

    void incrBlackHeight() { m_iBlackHeight++; };
    void decrBlackHeight() { m_iBlackHeight--; };


    void setParent(RBNode * parent) { m_pParent = parent; }
    void setLeft(RBNode * left) { m_pLeft = left; };
    void setRight(RBNode * right) { m_pRight = right; };

    RBNode(T * data, int key)
    {
    	m_pData = data;
    	m_iKey = key;
    	m_iBlackHeight = 0;
    	m_pParent = nullptr;
    	m_pLeft = nullptr;
    	m_pRight = nullptr;
    }

    RBNode(T * data, int key, RBNode * parent, RBNode * left, RBNode * right)
    {
    	m_pData = data;
    	m_iKey = key;
    	m_pParent = parent;
    	m_pLeft = left;
    	m_pRight = right;
    	m_iBlackHeight = 0;
    };



};

// About red-black trees:
// a red-black tree with n internal nodes has a height at most 2*log(n+1)

template <class T>
class RBTree
{
private:
	RBNode<T> * m_pRoot;
    bool leftRotate(RBNode<T> &);
    bool rightRotate(RBNode<T> &);
    bool insertFixup(RBNode<T> &);

public:
	RBNode<T> * getRoot()  {  return this->m_pRoot; };
    RBTree() { m_pRoot = nullptr; };
    bool insert(long int key, T * val);
    T * find(long int key);
    T * erase(long int key);
    pair<T*,int> max();
    pair<T*,int> min();

};



#endif /* BSTREE_H_ */
