#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, local_sum = 0;
    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0) {
        printf("Program for SUMMING first N integer numbers\n");
        printf("Enter N: ");
        scanf("%i", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = me + 1; i <= n; i += nproc) {
        if(i <= n) {
            sum += i;
        }
    }

    if (me != 0) {
        MPI_Isend(&sum, 1, MPI_INT, 0, me, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    } else {
        total = sum; // Assign the local sum of process 0 as the total initially
        printf("Process %d, partial sum: %d\n", me, sum);
        for (int proc = 1; proc < nproc; ++proc) {
            MPI_Irecv(&local_sum, 1, MPI_INT, proc, proc, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            total += local_sum;
            printf("Process %d, partial sum: %d\n", proc, local_sum);
        }
        printf("Sum of first %i integers equals   : %d\n", n, total);
        printf("Sum of first %i integers should be: %d\n", n, (n * (n + 1)) / 2);
    }

    MPI_Finalize();
    return 0;
}