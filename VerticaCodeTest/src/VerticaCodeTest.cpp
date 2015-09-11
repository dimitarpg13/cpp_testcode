//============================================================================
// Name        : VerticaCodeTest.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <list>


using namespace std;

void print_all(vector<int>& v)
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		cout << "i = " << i << ", v[i] = " << v[i] << endl;
	}

}


const int MAX_VAL = 100;


int sum_k_largest(int k, int array[], int sz)
{
    if (k <= 0 || array == NULL || sz <= 0)
    	return -1;


    if (k > sz)
    	k = sz;

	int res = 0;
	vector< int > buffer(MAX_VAL, 0);



	for (int i = 0; i < sz; i++)
	{
	   buffer[array[i]]++;
	}

	int k_largest = k,
	    first_indx = -1;
	for ( int j=MAX_VAL-1; j >= 0 && k >= 0; j-- )
	{
       if (buffer[j] > 0)
       {
    	   k_largest -= buffer[j];

    	   if (k_largest == 0)
    	   {
    	      first_indx = j;
    	      break;
    	   }
       }
	}

    if (first_indx > -1)
    {
    	for (int i = first_indx; i < MAX_VAL; i++)
    	{
    		if (buffer[i] > 0)
    			res += buffer[i] * i;
    	}

    }
    else
    	return -1;



	return res;
}

int randomized_partition(int array[], int p, int r)
{
	int res = -1;

	return res;
}

int randomized_select(int array[],  int p,   int r, int i)
{

   if (p == r)
	   return array[p];
   int q = randomized_partition(array, p, r);
   int k = q - p + 1;
   if ( i ==  k)
   {
	   return array[q];
   }
   else if (i < k)
   {
	   return randomized_select(array, p, q - 1, i);
   }
   else
	   return randomized_select(array, q + 1 , r, i - k );
}


int main() {

	int sz = 8;
    int array[] = { 1, 3, 4, 11, 99, 21, 4, 13 };

    int res = sum_k_largest(3, array, sz);

    cout << "the sum of the top 3 elements is : " << res << endl;



	return 0;
}
