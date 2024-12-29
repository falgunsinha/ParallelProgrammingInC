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
	return pow(x,exponent) + 1; // add 1 to make the function non-zero
} // end of function

int main(int argc, char *argv[]) {
	int npoints,i, nproc, me;
	double x,a,b,sum=0, total; // the variable sum is nulled before summation
	double a[] = {0.0, 1.0, 2.0, 3.0}; // lower bounds of integration
	double b[] = {1.0, 2.0, 3.0, 4.0}; // upper bounds of integration
	int n = sizeof(a)/sizeof(a[0]); // number of intervals

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	// loop over all intervals
	for(i=me;i<n;i=i+nproc) {
		// read input data
		double lower = a[i];
		double upper = b[i];
		int npoints = 1000; // number of points for calculation of integral

		// loop over all interior points
		double step = (upper-lower)/npoints;
		for(int j=0;j<npoints;j++) {
			x = lower + j*step; // x is the real argument, point where the function is evaluated
			sum += f(x)*step;
		}
	}

	MPI_Allreduce(&sum,&total,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD); //sum the partial sums from threads to sum
	printf(" Partial sum of processor %i : %f out of %f \n",me,sum,total);

	if(me == 0){
		printf(" The integral equals numerically %lf \n",total);
		printf(" The integral equals analytically %lf \n",(pow(b[n-1],exponent+1)-pow(a[0],exponent+1))/(exponent+1));
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
} // end of main
