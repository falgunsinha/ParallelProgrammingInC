#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2 // Size of matrices

void printMatrix(int A[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrix(int A[N][N], int B[N][N], int C[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int A[N][N] = {{1, 1}, {1, 0}};
    int B[N][N] = {{2, 0}, {1, 1}};
    int C[N][N];

    // Perform matrix multiplication
    multiplyMatrix(A, B, C);

    // Print the matrices
    printf("Matrix A:\n");
    printMatrix(A);
    printf("\nMatrix B:\n");
    printMatrix(B);
    printf("\nResultant Matrix C:\n");
    printMatrix(C);

    return 0;
}
