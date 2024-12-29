#include <stdio.h>
#include <mpi.h>

int main(double argc, char *argv[])
{
    double me, nproc, i, n, sum = 0, total, fac = 1, total_prod;

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

    // Calculate the bounds for each thread
    int low = 1 + me * n / nproc;
    int high = (me + 1) * n / nproc;

    printf("Thread %d: low=%d, high=%d\n", me, low, high);

    for (i = low; i <= high; i = i + 1)
    {
        printf("%1f Print1 for Sum, %1f %1f\n", me, n, sum);
        printf("me =%1f PrintA, nproc=%1f, n=%1f, sum=%1f\n", me, nproc, n, sum);
        sum = sum + i;
        printf("%1f Print2 for Factorial, %1f %1f\n", me, n, fac);
        fac = fac * i;
    }

    printf("Thread %d: count=%d\n", me, high - low + 1);

    MPI_Allreduce(&sum, &total, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (me == 0)
    {
        printf("Sum of first %1f integers equals   : %1f\n", n, total);
        printf("Sum of first %i integers should be: %1f\n", n, (n*(n+1))/2.0);
    }

    MPI_Allreduce(&fac, &total_prod, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (me == 0)
    {
        printf("Factorial of %1f  : %1f\n", n, total_prod);
    }

    MPI_Finalize();

    return 0;
}