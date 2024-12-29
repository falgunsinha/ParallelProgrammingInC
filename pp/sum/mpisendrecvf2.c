// sump_send_recv_delay_modified.c
// sums first N integers using MPI_Send and MPI_Recv with delays and non-reception
#include <stdio.h>
#include <mpi.h>
#include <unistd.h> // for sleep

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, received_sum = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0) {
        printf("Program for summing first N integer numbers\n");
        printf("Enter N: \n");
        scanf("%i", &n);
    }

    if (me == 0) {
        for (i = 1; i < nproc; ++i) {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else if (me == 2) {
        // Simulate not receiving message
        printf("Process %d does not receive.\n", me);
    } else {
        // Simulate delay before sending message
        if (me != 0) {
            sleep(10);
            MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }

    // Calculate partial sum for all processes including me == 2
    for (i = me + 1; i <= n; i = i + nproc) {
        sum = sum + i;
    }

    // Send partial sum to process 0
    if (me != 0) {
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        total = sum; // Initialize total with the sum from process 0
        for (i = 1; i < nproc; i++) {
            if (i != 2) {
                MPI_Recv(&received_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                total += received_sum; // Accumulate sums from other processes
            }
        }
        printf("Sum of first %i integers equals   : %i\n", n, total);
        printf("Sum of first %i integers should be: %i\n", n, (n * (n + 1)) / 2);
    }

    MPI_Finalize();

    return 0;
}
