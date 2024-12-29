#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 40
#define NUM_THREADS 4 // Change the number of threads as needed

double timediff(struct timeval start, struct timeval end) {
    return ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
}

int main(int argc, char *argv[]) {
    int number_of_lines = 0;
    int swap;
    char in_name[] = "sort.txt";
    char out_name[] = "sorted-p.txt";
    char line[LENGTH];
    int items[max_len];
    FILE *fp;
    struct timeval t1, t2;
    double time;

    // read the input
    fp = fopen(in_name, "r");
    if (fp == NULL) {
        printf("Input file %s not found\n", in_name);
        return 0;
    }

    while (1) {
        if (fgets(line, LENGTH, fp) == NULL) break; // finish reading when empty line is read
        if (sscanf(line, "%d", &items[number_of_lines]) == -1) break; // finish reading after error

        number_of_lines++;
    }

    printf("Number of items: %i\n", number_of_lines);

    gettimeofday(&t1, NULL);

    int min_val = items[0];
    int max_val = items[0];

    // Find min and max values in the dataset
    for (int i = 1; i < number_of_lines; i++) {
        if (items[i] < min_val) min_val = items[i];
        else if (items[i] > max_val) max_val = items[i];
    }

    int nproc = NUM_THREADS; // number of threads
    int interval = (max_val - min_val) / nproc; // divide the interval

    #pragma omp parallel num_threads(nproc)
    {
        int thread_id = omp_get_thread_num();
        int start = min_val + thread_id * interval;
        int end = (thread_id == nproc - 1) ? max_val : (start + interval);

        // Assign data into subsets based on intervals
        int subset[max_len];
        int subset_size = 0;
        for (int i = 0; i < number_of_lines; i++) {
            if (items[i] >= start && items[i] < end) {
                subset[subset_size++] = items[i];
            }
        }

        // Perform insertion sort in parallel
        for (int i = 1; i < subset_size; i++) {
            int key = subset[i];
            int j = i - 1;

            while (j >= 0 && subset[j] > key) {
                subset[j + 1] = subset[j];
                j = j - 1;
            }
            subset[j + 1] = key;
        }

        // Synchronization point before writing to the output file
        #pragma omp barrier

        // Write to the output file in the correct order
        #pragma omp critical
        {
            FILE *out_fp = fopen(out_name, "a");
            for (int i = 0; i < subset_size; i++) {
                fprintf(out_fp, "%d\n", subset[i]);
            }
            fclose(out_fp);
        }
    }

    gettimeofday(&t2, NULL);
    time = timediff(t1, t2);

    printf("time: %lf\n", time);

    fclose(fp);
}
