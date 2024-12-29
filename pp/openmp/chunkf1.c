/******************************************************************************
 * FILE: omp_workshare1.c
 * DESCRIPTION:
 *   OpenMP Example - Loop Work-sharing - C/C++ Version
 *   In this example, the iterations of a loop are scheduled dynamically
 *   across the team of threads.  A thread will perform CHUNK iterations
 *   at a time before being scheduled for the next CHUNK of work.
 * AUTHOR: Blaise Barney  5/99
 * LAST REVISED: 04/06/05
 ******************************************************************************/

#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE   12 //1b. change chunk size CHUNKSIZE
#define N       100

int main (int argc, char *argv[]) 
{
    int nthreads, me, i, chunk;
    float a[N], b[N], c[N];
    int thread_count[N] = {0}; // Counter for each thread

    /* Some initializations */
    for (i=0; i < N; i++)
        a[i] = b[i] = i * 1.0;
    chunk = CHUNKSIZE;
    printf("Chunksize is %i\n", chunk);

    #pragma omp parallel shared(b, c, nthreads, chunk) private(a, i, me) //1c. set e.g. variable a as private
    {
        me = omp_get_thread_num();
        if (me == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %i starting...\n", me);

        //#pragma omp for schedule(dynamic,chunk) //1a. remove specification schedule
        //#pragma omp for schedule(static, chunk) //1d. change scheduling to schedule (static, chunk)
        //#pragma omp for schedule(guided, chunk) //1e. change scheduling to schedule (guided, chunk)
        //#pragma omp for schedule(auto) 

        for (i = 0; i < N; i++)
        {
            c[i] = a[i] + b[i];
            printf("Thread %i: c[%i]= %f\n", me, i, c[i]);
            thread_count[me]++; // Increment counter for this thread
        }

    }  /* end of parallel section */

    // Print the number of items calculated by each thread
    printf("\nItems calculated by each thread:\n");
    for (i = 0; i < nthreads; i++)
    {
        printf("Thread %d: %d\n", i, thread_count[i]);
    }

    return 0;
}
