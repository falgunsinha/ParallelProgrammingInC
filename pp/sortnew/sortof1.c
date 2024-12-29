#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <limits.h> // Include for INT_MAX and INT_MIN
#include <time.h>

#define max_len 400000
#define LENGTH 40

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1.0e9;
}

int main(int argc, char *argv[]) {
    int number_of_lines = 0;
    int items[max_len];
    int low = INT_MAX;
    int high = INT_MIN;

    FILE *fp = fopen("sort.txt", "r");
    if (fp == NULL) {
        printf("Input file not found\n");
        return 0;
    }

    // Find min and max values
    while (1) {
        char line[LENGTH];
        if (fgets(line, LENGTH, fp) == NULL) break;
        if (sscanf(line, "%d", &items[number_of_lines]) == -1) break;

        if (items[number_of_lines] < low) low = items[number_of_lines];
        if (items[number_of_lines] > high) high = items[number_of_lines];

        number_of_lines++;
    }

    fclose(fp);

    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    int interval = (high - low) / num_threads;

    double start_time = get_time();

    #pragma omp parallel default(none) shared(items, number_of_lines, low, high, interval, num_threads)
    {
        int me = omp_get_thread_num();
        int low_me = low + me * interval;
        int high_me = (me == num_threads - 1) ? high : (low_me + interval);

        int b_me[max_len];
        int N_me = 0;

        // Find values in the assigned interval
        for (int i = 0; i < number_of_lines; i++) {
            if (items[i] >= low_me && items[i] <= high_me) {
                b_me[N_me] = items[i];
                N_me++;
            }
        }

        // Insertion Sort for the interval
        for (int i = 1; i < N_me; i++) {
            int key = b_me[i];
            int j = i - 1;
            while (j >= 0 && b_me[j] > key) {
                b_me[j + 1] = b_me[j];
                j = j - 1;
            }
            b_me[j + 1] = key;
        }

        // Append sorted values to the output file "sorted-p.txt" directly by individual threads
        FILE *out_fp;
        #pragma omp barrier // Wait for all threads before appending
        #pragma omp critical
        {
            out_fp = fopen("sorted-p.txt", "a");
            if (out_fp != NULL) {
                for (int i = 0; i < N_me; i++) {
                    fprintf(out_fp, "%d\n", b_me[i]);
                }
                fclose(out_fp);
            }
        }
    }

    double end_time = get_time();
    printf("Execution time: %f seconds\n", end_time - start_time);

    return 0;
}
