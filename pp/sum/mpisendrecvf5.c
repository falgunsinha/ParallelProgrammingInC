// sump_send_recv_multiple_tags_sum.c
// sums first N integers using MPI_Send and MPI_Recv with multiple tags and sum calculation
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, received_sum = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv); // initialise MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, me=0..nproc-1

    if (me == 0) {
        printf("Program for SUMMING first N integer numbers\n ");
        printf("Enter N: ");
        scanf("%i", &n);
    }

    if (me == 0) {
        for (i = 1; i < nproc; ++i) {
            MPI_Send(&n, 1, MPI_INT, i, 101, MPI_COMM_WORLD); // Send value with tag 101
            MPI_Send(&n, 1, MPI_INT, i, 102, MPI_COMM_WORLD); // Send value with tag 102
        }
    } else {
        MPI_Recv(&received_sum, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status); // Receive value with tag 101
        sum += received_sum; // Accumulate received value into the sum
        printf("Process %d received value with tag 101: %d\n", me, received_sum);

        MPI_Recv(&received_sum, 1, MPI_INT, 0, 102, MPI_COMM_WORLD, &status); // Receive value with tag 102
        sum += received_sum; // Accumulate received value into the sum
        printf("Process %d received value with tag 102: %d\n", me, received_sum);
    }

    // Calculate partial sum
    for (i = me + 1; i <= n; i = i + nproc) {
        sum = sum + i;
    }

    // Gather partial sums to process 0
    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (me == 0) {
        printf("Sum of first %i integers equals   : %i\n", n, total);
        printf("Sum of first %i integers should be: %i\n", n, (n * (n + 1)) / 2);
    }

    MPI_Finalize();

    return 0;
}
