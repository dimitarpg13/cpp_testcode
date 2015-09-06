/*
 ============================================================================
 Name        : ParetoDistributionTest.c
 Author      : Dimitar Gueorguiev
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>






//using bounded pareto distribution to generate pareto distributed LBA
//http://en.wikipedia.org/wiki/Pareto_distribution#Bounded_Pareto_distribution
//
// x = [ - (  drand * LBAcount^alpha - drand  - LBAcount^alpha  ) / LBAcount^alpha ]^(-1/alpha)
//
// input: LBAcount - the total available address space in blocks
//        alpha - the parameter of the bounded Pareto distribution
// output: one-based index of the LBA which is currently selected by the pareto distribution
//
long int getParetoDistributedLBA(long int LBAcount, double alpha)
{
      double maxlrand48 = pow(2,31);
	  double drand = ((double) lrand48()) / maxlrand48;
      double LBAcountByalpha = pow(LBAcount,alpha);
	  long int res = (long int) floor ( pow ( - ( drand * LBAcountByalpha - drand - LBAcountByalpha ) / LBAcountByalpha, -1/alpha ) );
      if (res < 0) // floating point errors may trip the result cast as long int into negtive teritory
    	           // so set it to the max LBA count in such case
    	  res=LBAcount;
	  return res;
}


double getBoundedParetoParam(unsigned long int LBAcount, double P)
{

    double delta = 0.04;
	return - log2((1-P)/delta) / log2(LBAcount);
}

void testParetoDistribution()
{
	// the total number of IOs to be distributed
	int maxIOCount = 1000;

	// 2^31 - number of LBAs in a single device address space
	long int devLBAcount = (long int) pow(2,31);

	// the total number of devices
	int devCount = 10;

	// this is the total available address space in blocks
	long int totLBAcount = devLBAcount * devCount;

	// pareto factor
	double P = 0.8;

    double alpha = getBoundedParetoParam(totLBAcount,P);

    time_t t1;
    time(&t1);
    srand48((long) t1);

    FILE *f = fopen("lbaPareto.txt", "w");
    long int curLBA = -1;
    int i = 0;
    for (i = 0; i < maxIOCount; i++)
    {
      curLBA = getParetoDistributedLBA(totLBAcount,alpha);
      fprintf(f,"%ld\n",curLBA);
    }
    fclose(f);
}

void testCompositeDistribution()
{
	// the total number of IOs to be distributed
	int maxIOCount = 1000;

	// 2^31 - number of LBAs in a single device address space
	long int devLBAcount = (long int) pow(2,31);

	// the total number of devices
	int devCount = 10;

	// this is the total available address space in blocks
	long int totLBAcount = devLBAcount * devCount;

	// pareto factor
	double P = 0.8;

	time_t t1;
	time(&t1);
	srand48((long) t1);


	int devIdx = -1; //zero based index of the selected device
    long int lbaIdx = -1; //zero based index of the selected lba within the chosen device
    double maxlrand48 = pow(2,31); // the max value which can be obtained from lrand48
    double drand = -1.0;

    FILE *f = fopen("lbaComposite.txt", "w");
    long int curLBA = -1;
    int i = 0;
    for (i = 0; i < maxIOCount; i++)
    {
		//select a device
		drand = ((double) lrand48()) / maxlrand48;
		devIdx = (int) floor (devCount * drand);

		//select a device lba
		drand = ((double) lrand48()) / maxlrand48;
		lbaIdx = (long int) floor(devLBAcount * drand);
		curLBA = devIdx * devLBAcount + lbaIdx + 1; //one based index of the selected lba from the total address space
		fprintf(f,"%ld\n",curLBA);
    }
    fclose(f);

}

int main(void) {
	puts("Bounded Pareto distribution test code"); /* prints Pareto Distribution test code */
    testParetoDistribution();

    puts("Composite distribution test code"); /* prints composite distribution test code */
    testCompositeDistribution();

	return EXIT_SUCCESS;
}


