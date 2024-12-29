// sums.c
// sums first N integers
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, n, me, nproc, sum = 0, total;
    int dest, source;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0)
    {
        printf("Program for summing first N integer numbers:\n");
        printf("Enter N: ");
        scanf("%i", &n);
    }

    // Send N from process 0 to all other processes
    if (me == 0)
    {
        for (dest = 1; dest <= nproc-1; dest++)
        {
            MPI_Send(&n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        // Receive N from process 0
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    // Compute partial sum
    for (i = me + 1; i <= n; i = i + nproc)
    {
        sum = sum + i;
    }

    // Send partial sum from each process to process 0
    if (me != 0)
    {
        MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        // Receive partial sums from all other processes and accumulate
        total = sum;
        for (source = 1; source <= nproc-1; source++)
        {
            MPI_Recv(&sum, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            total += sum;
        }
    }

    // Process 0 prints the result
    if (me == 0)
    {
        printf("Sum of first %i integers equals  : %i\n", n, total);
        printf("Sum of first %i integers should be: %i\n", n, (n * (n + 1) / 2));
    }

    MPI_Finalize();
    return 0;
}
