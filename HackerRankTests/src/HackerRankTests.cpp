//============================================================================
// Name        : HackerRankTests.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <limits.h>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cstdio>

#include <vector>
#include <cstdlib>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

#include <utility>

using namespace std;

struct node {
   struct node *left,*right;
   int val, val1;
};

typedef struct node node;
node * addElement(node * root, int x);
node * addRandomElement (node * root, int x, int index);


void arithmetic_progression_hole_test()
{
	int N = 2500;
	int n;

	cin >> n;

	int v[n], i = 0;
    for (i=0; i<n; i++)
    {
        cin >> v[i] ;
    }

    int maxDiff = -1,
        minDiff = INT_MAX,
        curDiff = -1,
        gap = -1;

    for (i = 0; i < n-1; i++)
    {
       curDiff = v[i+1] - v[i];
       if (curDiff > maxDiff)
       {
    	   maxDiff = curDiff;
    	   gap = v[i];
       }

       if (curDiff < minDiff)
       {
    	   minDiff = curDiff;
       }
    }

    int missingTerm = gap + minDiff;

	cout << missingTerm;
}

int maxDepthOfTree(node * root, int curDepth)
{
   if (root != NULL)
   {
	   int leftDepth = maxDepthOfTree(root->left, curDepth+1);
	   int rightDepth = maxDepthOfTree(root->right, curDepth+1);
       if (leftDepth > rightDepth)
    	   return leftDepth;
       else
    	   return rightDepth;
   }
   else
	  return curDepth;

}

/* Write your custom functions here */
int diameterOfTree(node * root) {
/* For your reference
  struct node {
   struct node *left,*right;
   int val;
};
*/
	//pair<int,int> maxDepths(0,0);
	//maxDepthsOfTree(root,maxDepths);


	return maxDepthOfTree(root, 0) - 1;

}


int main(){


  ifstream fin ("./input001.txt");

  node * root = NULL;
  int a[1000],K,i = 0,j = 0;
  int isBst = 0;

  std::string line;

  if (fin.bad())
  {
    cout << "Could not open input file!" << endl;
  }


  while (std::getline(fin, line))
  {
    std::istringstream iss(line);
    int n;


    if (i >= 2)
    {
       iss >> a[i-2];
    }
    else if (i==0)
    {
    	iss >> isBst;
    }
    else
    {
    	iss >> K;
    }
    i++;
    // do something useful with v
  }

  //scanf("%d", &isBst);
  //scanf("%d", &K);
  //
  //for( j = 0; j < K; j++ ) {
  // scanf("%d",&a[i++]);
  //
  //}

  fin.close();

  for( i = 0; i < K;i++ ){
       if( !isBst ){
           root = addRandomElement(root,a[i],i);
       } else {
           root = addElement(root,a[i]);
       }

   }

   cout << diameterOfTree(root) << endl;


return 0;
}

node * addElement(node * root, int x ){
   if( root == NULL ) {
       root = (node *) (malloc(sizeof(node)));
               root->left = NULL; root->right = NULL;
       root->val = x;root->val1 = -1;
       return root;
   }
   if( x < root->val ) {
        root->left = addElement(root->left,x);
   }
   else {
       root->right = addElement(root->right,x);
   }
   return root;
}

node * addRandomElement(node *root, int x, int index){
   if( root == NULL ) {
       root = (node *) (malloc(sizeof(node)));
                root->left = NULL; root->right = NULL;
               root->val = x;root->val1 = -1;
       return root;
   }

   if( index <=2 ){
       root->left = addRandomElement(root->left,x,index);
   } else {
       root->right = addRandomElement(root->right,x,index);
   }

   return root;
}
