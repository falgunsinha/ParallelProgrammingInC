#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char *argv[]) {
    int rank, size;
    double source[ARRAY_SIZE]; // Array to hold thread-specific values

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        source[i] = (rank * 0.1) + 1.0 + i; // Generating values for each thread
    }

    double gatheredValues[size * ARRAY_SIZE]; // To gather values from different threads

    // Task 3: Gather values from all threads to a single core
    MPI_Gather(source, ARRAY_SIZE, MPI_DOUBLE, gatheredValues, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
    printf("Values gathered from all threads:\n");
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%.1f ", gatheredValues[j * ARRAY_SIZE + i]);
        }
        printf("\n");
    }
}

    // Task 4: Scatter values from 0.1 to 4.0 across threads
    double scatteredValues[ARRAY_SIZE];
    double scatterData[40];
    if (rank == 0) {
        for (int i = 0; i < 40; ++i) {
            scatterData[i] = 0.1 * (i + 1); // Generating values from 0.1 to 4.0
        }
    }

    MPI_Scatter(scatterData, ARRAY_SIZE, MPI_DOUBLE, scatteredValues, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("Thread %d has value: %.6f\n", rank, scatteredValues[i]);
    }

    MPI_Finalize();
    return 0;
}