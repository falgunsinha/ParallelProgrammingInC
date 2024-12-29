#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A, B;
    if (rank == 0) {
        // Get user input for range A and B
        printf("Enter the value for A: ");
        fflush(stdout);
        scanf("%d", &A);

        printf("Enter the value for B: ");
        fflush(stdout);
        scanf("%d", &B);
    }

    // Broadcast A and B to all processes
    MPI_Bcast(&A, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the local sum for each process
    int local_sum = 0;
    for (int i = A + rank; i <= B; i += size) {
        local_sum += i;
    }

    // Reduce the local sums to get the final result
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The sum of all numbers between %d and %d is: %d\n", A, B, global_sum);
    }

    MPI_Finalize();
    return 0;
}