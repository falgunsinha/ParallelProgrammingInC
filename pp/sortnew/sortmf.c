#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

#define max_len 400000
#define LENGTH 40

double timediff(struct timeval start, struct timeval end) {
    return ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
}

int main(int argc, char *argv[]) {
    int rank, size;
    int number_of_lines = 0;
    int swap;
    char in_name[] = "sort.txt";
    char out_name[] = "sorted-p.txt";
    char line[LENGTH];
    int *items = NULL;
    FILE *fp;
    struct timeval t1, t2;
    double time_serial, time_parallel;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        fp = fopen(in_name, "r");
        if (fp == NULL) {
            printf("Input file %s not found\n", in_name);
            MPI_Finalize();
            return 0;
        }

        // Count the number of items in the file
        while (fgets(line, LENGTH, fp) != NULL) {
            if (sscanf(line, "%d", &swap) == 1) {
                number_of_lines++;
            }
        }

        fclose(fp);

        printf("Number of items: %i\n", number_of_lines);

        // Allocate memory for items based on the number of lines
        items = (int *)malloc(number_of_lines * sizeof(int));
        if (items == NULL) {
            printf("Memory allocation failed\n");
            MPI_Finalize();
            return 0;
        }

        // Read the file again to populate the items array
        fp = fopen(in_name, "r");
        for (int i = 0; i < number_of_lines; i++) {
            if (fgets(line, LENGTH, fp) == NULL) break;
            if (sscanf(line, "%d", &items[i]) != 1) break;
        }
        fclose(fp);
    }

    gettimeofday(&t1, NULL);

    // Broadcast the number of lines to all processes
    MPI_Bcast(&number_of_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter the data among processes
    int chunk_size = number_of_lines / size;
    int *local_items = (int *)malloc(number_of_lines * sizeof(int));
    MPI_Scatter(items, chunk_size, MPI_INT, local_items, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the received data (each process sorts its own data)
    for (int i = 0; i < number_of_lines - 1; i++) {
        for (int j = 0; j < number_of_lines - i - 1; j++) {
            if (local_items[j] > local_items[j + 1]) {
                swap = local_items[j];
                local_items[j] = local_items[j + 1];
                local_items[j + 1] = swap;
            }
        }
    }

    // Gather sorted data to the root process
    MPI_Gather(local_items, chunk_size, MPI_INT, items, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    free(local_items);

    gettimeofday(&t2, NULL);
    time_parallel = timediff(t1, t2);

    if (rank == 0) {
        printf("Parallel Time: %lf\n", time_parallel);
    }

    // Serial sorting by a single process for comparison
    if (rank == 0) {
        gettimeofday(&t1, NULL);

        for (int i = 0; i < number_of_lines - 1; i++) {
            for (int j = 0; j < number_of_lines - i - 1; j++) {
                if (items[j] > items[j + 1]) {
                    swap = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = swap;
                }
            }
        }

        gettimeofday(&t2, NULL);
        time_serial = timediff(t1, t2);

        printf("Serial Time: %lf\n", time_serial);

        // Speedup calculation based on Amdahl's Law
        double speedup = 100 / ((time_serial / time_parallel) + (1 - 1.0 / size));
        printf("Speedup: %lf\n", speedup);
    }

    free(items);

    MPI_Finalize();
    return 0;
}
