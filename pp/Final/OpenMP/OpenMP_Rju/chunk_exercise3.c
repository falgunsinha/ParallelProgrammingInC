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

#pragma omp parallel shared(b, c, nthreads, chunk) private(a, i, me)
	{
		me = omp_get_thread_num();
		if (me == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Number of threads = %d\n", nthreads);
		}
		printf("Thread %i starting...\n", me);

#pragma omp for schedule(dynamic, chunk)
		for (i = 0; i < N; i++)
		{
			c[i] = a[i] + b[i];
			printf("Thread %i: c[%i]= %f\n", me, i, c[i]);
		}

	} /* end of parallel section */

	return 0;
}