#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 3 // Size of matrices

void printMatrix(int A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrix(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "This program requires at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int A[N][N] = {{1, 1, 1}, {1, 0, 1}, {2, 1, 0}};
    int B[N][N] = {{2, 0, 1}, {1, 1, 1}, {1, -1, 1}};
    int C[N][N];

    // Scatter rows of matrix A to different processes
    int scatterBuffer[N * N / size];
    MPI_Scatter(A, N * N / size, MPI_INT, scatterBuffer, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process performs local matrix multiplication
    int localC[N][N];
    multiplyMatrix((int (*)[N])scatterBuffer, B, localC);

    // Gather the results from all processes
    MPI_Gather(localC, N * N / size, MPI_INT, C, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the result in the root process
    if (rank == 0) {
        printf("Matrix A:\n");
        printMatrix(A);
        printf("\nMatrix B:\n");
        printMatrix(B);
        printf("\nResultant Matrix C:\n");
        printMatrix(C);
    }

    MPI_Finalize();

    return 0;
}
