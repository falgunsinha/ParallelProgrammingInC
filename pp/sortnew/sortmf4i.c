#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

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
    char in_name[] = "sort.txt";
    int items[max_len];
    char out_name[] = "sorted-p.txt";
    char line[LENGTH];
    FILE *fp;
    struct timeval t1, t2;
    double time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        fp = fopen(in_name, "r");
        if (fp == NULL) {
            printf("Input file %s not found\n", in_name);
            MPI_Finalize();
            return 0;
        }

        while (1) {
            if (fgets(line, LENGTH, fp) == NULL) break;
            if (sscanf(line, "%d", &items[number_of_lines]) == -1) break;

            number_of_lines++;
        }

        printf("Number of items: %i\n", number_of_lines);
        fclose(fp);
    }

    gettimeofday(&t1, NULL);
    MPI_Bcast(&number_of_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int remainder = number_of_lines % num_procs;
    int local_len = number_of_lines / num_procs;
    int last_len = local_len + (rank < remainder ? 1 : 0);

    int *local_items = (int *)malloc(last_len * sizeof(int));

    MPI_Scatter(items, local_len, MPI_INT, local_items, last_len, MPI_INT, 0, MPI_COMM_WORLD);

    bubble_sort(local_items, last_len);

    MPI_Gather(local_items, last_len, MPI_INT, items, last_len, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        bubble_sort(items, number_of_lines);
        gettimeofday(&t2, NULL);
        time = timediff(t1, t2);
        printf("Time taken: %lf\n", time);

        fp = fopen(out_name, "w");
        for (int i = 0; i < number_of_lines; i++) {
            fprintf(fp, "%d\n", items[i]);
        }
        fclose(fp);
    }

    free(local_items);
    MPI_Finalize();
    return 0;
}
