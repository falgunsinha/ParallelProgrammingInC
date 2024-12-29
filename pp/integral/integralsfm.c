// compilation: cc  -lm integral.c
// compilation: icc -lm integral.c
// compilation: icc     integral.c

//calculates integral of a function

// libraries
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int exponent = 2; // exponent of the power function

double f(double x) {
    return pow(x, exponent);
}

int main(int argc, char *argv[]) {
    int npoints, i, me, nproc;
    double x, a, b, sum = 0.0, total = 0.0;

    MPI_Init(&argc, &argv); // initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, me=0..nproc-1

    printf("Initiation of program, processor %d, out of %d\n", me, nproc);

    if (me == 0) {
        // read input data only in process 0
        printf("Input number of points for calculation of integral:\n");
        scanf("%i", &npoints);
        printf("Input lower bound of integration: \n");
        scanf("%lf", &a);
        printf("Input higher bound of integration: \n");
        scanf("%lf", &b);
    }

    // Broadcast inputs to all processes
    MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Each process computes its partial sum
    for (i = me + 1; i <= npoints - 1; i += nproc) {
        x = a + i * (b - a) / npoints;
        sum += f(x);
    }

    // Reduce all partial sums to get the total sum
    MPI_Allreduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Each process prints its partial sum and total
    printf("Partial sum of processor %d: %lf out of %lf\n", me, sum, total);

    MPI_Finalize(); // finalize MPI peacefully

    if (me == 0) {
        // Process 0 calculates the integral and prints the results
        total += (f(a) + f(b)) / 2.0; // add first and last points
        total *= (b - a) / npoints; // result of integration

        printf("The integral equals numerically %lf\n", total);
        printf("The integral equals analytically %lf\n", (pow(b, exponent + 1) - pow(a, exponent + 1)) / (exponent + 1));
    }

    return 0;
}
