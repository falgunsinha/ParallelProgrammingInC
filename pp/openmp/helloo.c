/** helloo.c
 *   In this simple example, the master thread forks a parallel region.
 *   All threads in the team obtain their unique thread number and print it.
 *   The master thread only prints the total number of threads.  Two OpenMP
 *   library routines are used to obtain the number of threads and each
 *   thread's number. **/
#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int nthreads, me;

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(nthreads,me)
	{
		me = omp_get_thread_num();   /* Obtain thread number */
		printf("Hello World from thread = %d\n", me);

		if (me == 0){   /* Only master thread does this */
			nthreads = omp_get_num_threads();
			printf("Number of threads = %d\n", nthreads);
		}
	}  /* All threads join master thread and disband */

	return 0;
}


