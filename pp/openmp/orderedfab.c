#include <omp.h>
#include <stdio.h>

int main() {
    int n;
    printf("Input number of parallel threads: ");
    scanf("%i", &n);

    #pragma omp parallel num_threads(n)
    {
        int me = omp_get_thread_num();
        printf("A by thread %i\n", me);
        
        #pragma omp for ordered
        for (int i = 0; i < omp_get_num_threads(); i++)
        {
            printf("B by thread %i\n", me);
            
            #pragma omp ordered
            {
                printf("ORDERED by thread %i\n", me);
            }
        }
        
        // Ensure synchronization before proceeding
        #pragma omp barrier
        
        printf("C by thread %i\n", me);
    }
    return 0;
}
