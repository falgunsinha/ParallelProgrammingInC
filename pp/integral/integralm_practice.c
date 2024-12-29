//calculates integral of a function
// libraries
#include <stdio.h>
#include <math.h>
#include "mpi.h"

// global variable - to be used in main and function f
int exponent=2; // assign exponent of the power function

double f(double x) {
	return pow(x,exponent);
} // end of function

int main(int argc, char *argv[]) 
{
	int me, nproc, npoints, i;
	double x, a, b, sum=0.0, total=0.0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	printf("Initializing program with Process %d out of %d\n", me, nproc);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(me == 0)
	{
	printf("Input number of points for calculation of integral:\n");
	scanf("%i", &npoints);
	printf("Input lower/higher bound of integration:\n");
	scanf("%lf", &a);
	scanf("%lf", &b);
	}

	MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// loop over all interior points
	for(i=me+1; i <= npoints-1; i=i+nproc) 
	{
		x=a+i*(b-a)/npoints; // x is the real argument, point where the function is evaluated
		sum=sum+f(x);
	}

	MPI_Allreduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	printf("Partial sum of Process %d: %lf out of %lf \n", me, sum, total);

	MPI_Finalize();

	if(me == 0)
	{
	total += (f(a)+f(b))/2.0; // add first and last points 
	total *= (b-a)/npoints; // result of integration
	printf("The integral equals numerically %lf \n", total);
	printf("The integral equals analytically %lf \n",(pow(b,exponent+1)-pow(a,exponent+1))/(exponent+1));
	}

	return 0;
} 

