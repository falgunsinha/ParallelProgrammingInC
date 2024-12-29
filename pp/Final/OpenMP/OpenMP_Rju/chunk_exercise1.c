#include <omp.h>
#include <stdio.h>
#define CHUNKSIZE 10
#define N 100

int main(int argc, char *argv[])
{
	int nthreads, me, i, chunk;
	float a[N], b[N], c[N];

	/* Some initializations */
	for (i = 0; i < N; i++)
		a[i] = b[i] = i * 1.0;
	chunk = CHUNKSIZE;
	printf("Chunksize is %i\n", chunk);

	int loop_counters[omp_get_max_threads()]; // Array to track loop executions for each thread

	// Initialize loop counters to zero
	for (int t = 0; t < omp_get_max_threads(); ++t)
	{
		loop_counters[t] = 0;
	}

#pragma omp parallel shared(a, b, c, nthreads, chunk, loop_counters) private(i, me)
	{
		me = omp_get_thread_num();
		if (me == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Number of threads = %d\n", nthreads);
		}
		printf("Thread %i starting...\n", me);

#pragma omp for
		for (i = 0; i < N; i++)
		{
			c[i] = a[i] + b[i];
			printf("Thread %i: c[%i]= %f\n", me, i, c[i]);

			// Increment the thread-specific loop counter
#pragma omp atomic
			loop_counters[me]++;
		}

	} /* end of parallel section */

	// Print the number of loop executions for each thread
	for (int t = 0; t < nthreads; ++t)
	{
		printf("Thread %d executed the loop %d times.\n", t, loop_counters[t]);
	}

	return 0;
}
