// compilation: cc  -lm integral.c
// compilation: icc -lm integral.c
// compilation: icc     integral.c

// calculates the integral of a function

// libraries
#include <stdio.h>
#include <math.h>
#include <mpi.h>

// global variable - to be used in main and function f
int exponent = 2; // assign exponent of the power function

double f(double x) {
    return pow(x, exponent);
} // end of function

int main(int argc, char *argv[]) {
    int nproc, me;
    double sum = 0, total; // the variable sum is nulled before summation
    int n = 4; // Number of intervals
    double lower_bounds[n]; // Array to store lower bounds
    double upper_bounds[n]; // Array to store upper bounds
    double npoints; // Number of points for calculation of integral

    MPI_Init(&argc, &argv); // initialise MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, me=0..nproc-1

    if (me == 0) {
        printf("Input lower bounds of integration for %d intervals: \n", n);
        for (int i = 0; i < n; i++) {
            scanf("%lf", &lower_bounds[i]);
        }
        printf("Input upper bounds of integration for %d intervals: \n", n);
        for (int i = 0; i < n; i++) {
            scanf("%lf", &upper_bounds[i]);
        }
        printf("Input number of points for calculation of integral: ");
        scanf("%lf", &npoints);
    }

    MPI_Bcast(lower_bounds, n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // broadcast the input from thread 0 to all threads
    MPI_Bcast(upper_bounds, n, MPI_DOUBLE, 0, MPI_COMM_WORLD); // broadcast the input from thread 0 to all threads
    MPI_Bcast(&npoints, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); // broadcast the input from thread 0 to all threads

    // perform the integral calculation
    for (int i = me; i < n; i = i + nproc) {
        // calculate the difference between lower and upper bounds
        double diff = upper_bounds[i] - lower_bounds[i];

        // loop over all interior points
        double step = diff / npoints;
        for (int j = 0; j < npoints; j++) {
            double x = lower_bounds[i] + j * step; // x is the real argument, point where the function is evaluated
            sum += f(x) * step;
        }
    }

    MPI_Allreduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); // sum the partial sums from threads to sum
    printf("Partial sum of processor %i : %f out of %f \n", me, sum, total);

    if (me == 0) {
        total = total + (f(lower_bounds[0]) + f(upper_bounds[n - 1])) / 2; // add first and last points
        total = total * (upper_bounds[n - 1] - lower_bounds[0]) / npoints; // result of integration

        printf("The integral equals: %lf \n", total);
        //printf("The integral equals analytically %lf \n", (pow(upper_bounds[n - 1], exponent + 1) - pow(lower_bounds[0], exponent + 1)) / (exponent + 1));
    }

    MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

    return 0;
} // end of main
