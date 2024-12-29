#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, local_sum = 0;
    int send_count = 0; // Counter for MPI_Send commands

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0) {
        printf("Program for SUMMING first N integer numbers\n ");
        printf("Enter N: ");
        scanf("%i", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = me + 1; i <= n; i = i + nproc) {
        sum = sum + i;
    }

    MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("Process %d, partial sum: %d\n", me, sum);

    // Gather the partial sums to process 0 and calculate total sum
    if (me == 0) {
        printf("Reduced sum for each process:\n");
        printf("Process %d, partial sum: %d\n", me, sum); // Print process 0's sum
        for (int proc = 1; proc < nproc; ++proc) {
            MPI_Recv(&local_sum, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += local_sum;
            printf("Process %d, partial sum: %d\n", proc, local_sum);
        }
    } else {
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        send_count++; // Increment the MPI_Send command count
    }

    if (me == 0) {
        printf("Sum of first %i integers equals   : %i\n", n, total);
        printf("Sum of first %i integers should be: %i\n", n, (n * (n + 1)) / 2);
        printf("Number of MPI_Send commands used: %d\n", send_count);
    }

    MPI_Finalize();
    return 0;
}
