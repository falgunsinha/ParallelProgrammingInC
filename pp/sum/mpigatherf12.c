#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char *argv[]) 
{
    int i, me, nproc;
    double source[ARRAY_SIZE]; // Array to hold thread-specific values

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    // Assigning values to each thread
    for (i = 0; i < ARRAY_SIZE; i++) 
    {
        source[i] = (me * 0.1) + 1.0 + i; // Generating unique values for each thread
    }

    double gatheredValues[ARRAY_SIZE * nproc]; // To gather values from different threads

    // Gather values from all threads to a single core
    MPI_Gather(source, ARRAY_SIZE, MPI_DOUBLE, gatheredValues, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (me == 0) {
        printf("Gathered values from all threads:\n");
        for (i = 0; i < nproc * ARRAY_SIZE; i++)
         {
            printf("%.1f ", gatheredValues[i]);
             
        }
    }

    MPI_Finalize();
    return 0;
}
