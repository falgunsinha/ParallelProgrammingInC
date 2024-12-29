#include <omp.h>
#include <stdio.h>

int main() {
    int n;
    printf("Input number of parallel threads: ");
    scanf("%i", &n);

    #pragma omp parallel num_threads(n)
    {
        int me = omp_get_thread_num();
        FILE *file;
        
        // Open file in write mode for thread 0, append mode for other threads
        if (me == 0) {
            file = fopen("output.txt", "w");
        } else {
            file = fopen("output.txt", "a");
        }

        #pragma omp for ordered
        for (int i = 0; i < omp_get_num_threads(); i++) {
            #pragma omp ordered
            {
                fprintf(file, "Thread %d: Line %d\n", me, i);
            }
        }

        fclose(file);
    }

    return 0;
}
