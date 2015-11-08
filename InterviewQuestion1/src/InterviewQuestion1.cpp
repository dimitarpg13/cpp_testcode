//============================================================================
// Name        : InterviewQuestion1.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

struct Node
{
	Node(int v) : val(v) {};
	int val;
	Node * parent;
	Node * left;
	Node * right;

};

void construct (Node * root)
{
    //if (root == NULL)
    //	return ;

    if (root == NULL )
    {
    	root = new Node(1);
    }

    (root)->left = new Node(2);
    (root)->right = new Node(3);
}

void cleanup(Node ** root)
{
	Node * cur = *root;
	while (!cur)
	{
       cleanup(&(cur->left));
       cleanup(&(cur->right));
       delete cur;
	}
};



int main() {
	cout << "Interview question 1" << endl; // prints Interview question 1

	Node * root = new Node(10);


    construct(root);


	cleanup(&root);

	return 0;
}
