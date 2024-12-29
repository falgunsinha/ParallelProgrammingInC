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
#define CHUNKSIZE   10
#define N       100

int main (int argc, char *argv[]) 
{
	int nthreads, me, i, chunk;
	float a[N], b[N], c[N];

	/* Some initializations */
	for (i=0; i < N; i++)
		a[i] = b[i] = i * 1.0;
	chunk = CHUNKSIZE;
	printf("Chunksize is %i\n",chunk);

	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,me)
	{
		me = omp_get_thread_num();
		if (me == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Number of threads = %d\n", nthreads);
		}
		printf("Thread %i starting...\n",me);

		#pragma omp for schedule(dynamic,chunk)
		for (i=0; i<N; i++)
		{
			c[i] = a[i] + b[i];
			printf("Thread %i: c[%i]= %f\n",me,i,c[i]);
		}

	}  /* end of parallel section */

	return 0;
}


