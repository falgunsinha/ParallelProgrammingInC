#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

#define max_len 400000
#define LENGTH 40

double timediff(struct timeval start, struct timeval end) {
    return ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
}

void bubble_sort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, num_procs;
    int number_of_lines = 0;
    int swap;
    char in_name[] = "sort.txt";
    char out_name[] = "sorted-p.txt";
    char line[LENGTH];
    int items[max_len];
    FILE *fp;
    struct timeval t1, t2;
    double time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Read the input by a single thread (rank 0)
    if (rank == 0) {
        fp = fopen(in_name, "r");
        if (fp == NULL) {
            printf("Input file %s not found\n", in_name);
            MPI_Finalize();
            return 0;
        }

        while (1) {
            if (fgets(line, LENGTH, fp) == NULL) break; // finish reading when empty line is read
            if (sscanf(line, "%d", &items[number_of_lines]) == -1) break; // finish reading after error

            number_of_lines++;
        }

        printf("Number of items: %i\n", number_of_lines);
        fclose(fp);
    }

    gettimeofday(&t1, NULL);
    // Broadcast the number of lines to all processes
    MPI_Bcast(&number_of_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate range for each process
    int subrange = number_of_lines / num_procs;
    int start = rank * subrange;
    int end = (rank == num_procs - 1) ? number_of_lines : start + subrange;

    int local_len = end - start;
    int local_items[max_len];

    // Scatter the data among processes
    MPI_Scatter(items, local_len, MPI_INT, local_items, local_len, MPI_INT, 0, MPI_COMM_WORLD);

    // Sorting (bubble sort) the local portion
    bubble_sort(local_items, local_len);

    gettimeofday(&t2, NULL);

    time = timediff(t1, t2);

    //Serial Time calculation
    double serial_time; // Variable to hold the Serial Time
    if (rank == 0) {
    gettimeofday(&t1, NULL);
    bubble_sort(items, number_of_lines);
    gettimeofday(&t2, NULL);
    serial_time = timediff(t1, t2); // Calculate Serial Time
    
    }
    // Gather the sorted blocks back to the root process
    MPI_Gather(local_items, local_len, MPI_INT, items, local_len, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Sort the merged array in the root process
    if (rank == 0) {
        printf("time: %lf\n", time);
        bubble_sort(items, number_of_lines);

        printf("Serial Time: %lf\n", serial_time);

        double speedup = serial_time / time; // Calculate Speedup
        printf("Speedup: %lf\n", speedup);

        // Write the sorted result by the root process
        fp = fopen(out_name, "w");
        for (int i = 0; i < number_of_lines; i++) {
            fprintf(fp, "%d\n", items[i]);
        }
        fclose(fp);
    }

    MPI_Finalize();
    return 0;
}
