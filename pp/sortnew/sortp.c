#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#define MAX_LEN 400000
#define LENGTH 40

double time_diff(struct timeval start, struct timeval end) {
    return ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
}

void insertion_sort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main(int argc, char* argv[]) {
    int num_lines = 0;
    char input_file[] = "sort.txt";
    char output_file[] = "sorted-p.txt";
    char line[LENGTH];
    int items[MAX_LEN], max, min;
    FILE *fp;
    struct timeval start_time, end_time;
    double time, serial_time;
    
    int rank, num_procs, j, local_items[MAX_LEN];
    long int split;
    long int low, high;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        fp = fopen(input_file, "r");
        if (fp == NULL) {
            printf("Input file %s not found\n", input_file);
            return 0;
        }
        while (1) {
            if (fgets(line, LENGTH, fp) == NULL) break;
            if (sscanf(line, "%d", &items[num_lines]) == -1) break;

            num_lines++;
        }

        printf("Number of items: %i\n", num_lines);
        max = items[0];
        min = items[0];
        for (int i = 1; i < num_lines; i++) {
            if (items[i] > max) max = items[i];
            if (items[i] < min) min = items[i];
        }

        // Measure Serial Time
        int serial_items[MAX_LEN];
        for (int i = 0; i < num_lines; i++) {
            serial_items[i] = items[i];
        }

        gettimeofday(&start_time, NULL);
        insertion_sort(serial_items, num_lines);
        gettimeofday(&end_time, NULL);
        serial_time = time_diff(start_time, end_time);
        printf("Serial Time: %lf\n", serial_time);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Bcast(&num_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&items, num_lines, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&min, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (min < 0) {
        if (max > 0) {
            split = (long int)(max - min) / num_procs;
        } else {
            split = (long int)(-1 * (max + min)) / num_procs;
        }
    } else {
        split = (long int)(max + min) / num_procs;
    }

    j = 0;
    low = min + rank * split;
    high = min + (rank + 1) * split;
    for (int i = 0; i < num_lines; i++) {
        if (((items[i] >= low) && (items[i] < high)) || ((rank == num_procs - 1) && (items[i] >= high))) {
            local_items[j] = items[i];
            j++;
        }
    }
    
    gettimeofday(&start_time, NULL);
    insertion_sort(local_items, j);
    gettimeofday(&end_time, NULL);
    time = time_diff(start_time, end_time);

    //Speedup Calcualtion
    if (rank == 0) {
        printf("Time: %lf\n", time);
        double speedup = serial_time / time;
        printf("Speedup: %lf\n", speedup);
    }

    fp = fopen(output_file, "w");
    fclose(fp);
    MPI_Barrier(MPI_COMM_WORLD);

    //Appending to Output file
    for (int r = 0; r < num_procs; r++) {
        if (r == rank) {
            fp = fopen(output_file, "a");
            for (int i = 0; i < j; i++) {
                fprintf(fp, "%d\n", local_items[i]);
            }
            fclose(fp);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
