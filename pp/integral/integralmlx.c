// compilation: cc  -lm integral.c
// compilation: icc -lm integral.c
// compilation: icc     integral.c

//calculates integral of a function

// libraries
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// global variable - to be used in main and function f
int exponent=2; // assign exponent of the power function

double f(double x) {
	return pow(x,exponent);
} // end of function

int main(int argc, char *argv[]) {
	int npoints,i, nproc, me;
	double x,a,b,sum=0, total; // the variable sum is nulled before summation

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	// read input data
	if(me==0){
		printf(" Input number of points for calculation of integral:\n ");
		scanf("%i", &npoints);
		printf(" Input lower/higher bound of integration: \n");
		scanf("%lf", &a);scanf("%lf", &b);
	}

	MPI_Bcast(&npoints,1,MPI_INT,0,MPI_COMM_WORLD); //broadcast the input from thread 0 to all threads
	MPI_Bcast(&a,1,MPI_DOUBLE,0,MPI_COMM_WORLD); //broadcast the input from thread 0 to all threads
	MPI_Bcast(&b,1,MPI_DOUBLE,0,MPI_COMM_WORLD); //broadcast the input from thread 0 to all threads
	

	// loop over all interior points
	for(i=me+1;i<=npoints-1;i=i+nproc) {
		x=a+i*(b-a)/npoints; // x is the real argument, point where the function is evaluated
		sum=sum+f(x);
	}

	//printf(" Sum equals %lf \n",sum);

	MPI_Allreduce(&sum,&total,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD); //sum the partial sums from threads to sum
	printf(" Partial sum of processor %i : %f out of %f \n",me,sum,total);

	if(me == 0){
		total=total+(f(a)+f(b))/2; // add first and last points
		total=total*(b-a)/npoints; // result of integration

		printf(" The integral equals numerically %lf \n",total);
		printf(" The integral equals analytically %lf \n",(pow(b,exponent+1)-pow(a,exponent+1))/(exponent+1));
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
} // end of main

