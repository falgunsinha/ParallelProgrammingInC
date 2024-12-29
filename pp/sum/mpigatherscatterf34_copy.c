#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char *argv[]) 
{
    int i, j, me, nproc;
    double source[ARRAY_SIZE]; // Array to hold thread-specific values

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    for (i = 0; i < ARRAY_SIZE; i++) 
    {
        source[i] = (me * 0.1) + 1.0 + i; // Generating values for each thread
    }

    double gatheredValues[ARRAY_SIZE * nproc]; // To gather values from different threads

    // Task 3: Gather values from all threads to a single core in multiple MPI_Gather calls
    
    for (i = 0; i < ARRAY_SIZE; i++) 
    {
        MPI_Gather(&source[i], 1, MPI_DOUBLE, &gatheredValues[i * nproc], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    if (me == 0) 
    {
        printf("Values gathered from all threads:\n");
        for (i = 0; i < ARRAY_SIZE; i++) 
        {
            for (j = 0; j < nproc; j++) 
            {
                printf("%.1f ", gatheredValues[j + i * nproc]);
            }
            //printf("\n");
        }
    }

    // Task 4: Scatter values from 0.1 to 4.0 across threads
    double scatteredValues[ARRAY_SIZE];
    double scatterData[40];

    if (me == 0) 
    {
        printf("\nScattered Values:\n");
        for (i = 0; i < 40; i++) 
        {
            scatterData[i] = 0.1 * (i + 1); // Generating values from 0.1 to 4.0
        }
    }
    
    MPI_Scatter(scatterData, ARRAY_SIZE, MPI_DOUBLE, scatteredValues, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (i = 0; i < ARRAY_SIZE; i++) 
    {
        printf("Thread %d has value: %.6f\n", me, scatteredValues[i]);
    }

    MPI_Finalize();
    return 0;
}
