// sump_send_recv_multiple.c
// sums first N integers using MPI_Send and MPI_Recv with multiple sends
#include <stdio.h>
#include <mpi.h>

#define NUM_MESSAGES 3 // Number of messages to send

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, received_sum = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv); // initialise MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, me=0..nproc-1

    if (me == 0) {
        printf("Program for SUMMING first N integer numbers\n ");
        printf("Enter N: ");
        fflush(stdout); // Ensure buffer is flushed before input
        scanf("%i", &n);

        // Calculate partial sum for process 0
        for (i = me + 1; i <= n; i = i + nproc) {
            sum = sum + i;
        }

        total = sum; // Initialize total with the sum from process 0
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast N to all processes

    if (me != 0) {
        for (i = 0; i < NUM_MESSAGES; ++i) {
            // Calculate partial sum
            sum = 0;
            for (int j = me + 1; j <= n; j = j + nproc) {
                sum = sum + j;
            }
            MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Send partial sum to process 0
        }
    } else {
        for (i = 1; i < nproc; ++i) {
            for (int k = 0; k < NUM_MESSAGES; ++k) {
                MPI_Recv(&received_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status); // Receive partial sum from other processes
                total += received_sum; // Accumulate received sums
            }
        }

        printf("Sum of first %i integers equals   : %i\n", n, total);
        printf("Sum of first %i integers should be: %i\n", n, (n * (n + 1)) / 2);
    }

    MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

    return 0;
}
