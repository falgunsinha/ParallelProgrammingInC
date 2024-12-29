// sump.c
// sums first N integers
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int me, nproc, i, n;
    double sum = 0, total, fac = 1, total_prod;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0)
    {
        printf("Program for summing first N integer numbers:\n ");
        printf("Enter N: ");
        scanf("%i", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (i = me + 1; i <= n; i = i + nproc)
    {
        printf("%i Print1 for Sum, %i %lf\n", me, n, sum);
        printf("me =%i PrintA, nproc=%i, n=%i, sum=%lf\n", me, nproc, n, sum);
        sum = sum + i;
        printf("%i Print2 for Factorial, %i %lf\n", me, n, fac);
        fac = fac * i;
    }

    MPI_Allreduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (me == 0)
    {
        printf("Sum of first %i integers equals   : %lf\n", n, total);
        printf("Sum of first %i integers should be: %lf\n", n, (n*(n+1))/2.0);
    }

    MPI_Allreduce(&fac, &total_prod, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    if (me == 0)
    {
        printf("Factorial of %i  : %lf\n", n, total_prod);
    }

    MPI_Finalize();
    return 0;
}