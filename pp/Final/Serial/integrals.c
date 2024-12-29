// compilation: cc  -lm integral.c
// compilation: icc -lm integral.c
// compilation: icc     integral.c

//calculates integral of a function

// libraries
#include <stdio.h>
#include <math.h>

// global variable - to be used in main and function f
int exponent=2; // assign exponent of the power function

double f(double x) {
	return pow(x,exponent);
} // end of function

int main() {
	int npoints,i;
	double x,a,b,sum=0; // the variable sum is nulled before summation

	// read input data
	printf(" Input number of points for calculation of integral:\n ");
	scanf("%i", &npoints);
	printf(" Input lower/higher bound of integration: \n");
	scanf("%lf", &a);scanf("%lf", &b);

	// loop over all interior points
	for(i=1;i<=npoints-1;i=i+1) {
		x=a+i*(b-a)/npoints; // x is the real argument, point where the function is evaluated
		sum=sum+f(x);
	}

	printf(" Sum equals %lf \n",sum);

	sum=sum+(f(a)+f(b))/2; // add first and last points 
	sum=sum*(b-a)/npoints; // result of integration
	printf(" The integral equals numerically %lf \n",sum);
	printf(" The integral equals analytically %lf \n",(pow(b,exponent+1)-pow(a,exponent+1))/(exponent+1));

	return 0;
} // end of main

