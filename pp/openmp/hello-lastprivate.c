/** helloo.c
 *   In this simple example, the master thread forks a parallel region.
 *   All threads in the team obtain their unique thread number and print it.
 *   The master thread only prints the total number of threads.  Two OpenMP
 *   library routines are used to obtain the number of threads and each
 *   thread's number. **/
#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int nthreads, me,a=5,b=6;

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel for lastprivate(a,b) private(me) schedule(dynamic)
	for(int i=1;i<10;i++){
		b=i;a=i*i;
		me = omp_get_thread_num();  
		printf("thread = %d i=%i\n", me,i);
		/* a=100*me; b=1000*me; 
		   printf("modified at thread = %d a=%i b=%i\n", me,a,b); */
	}
	/* All threads join master thread and disband */
	printf("Hello World serial  a=%i b=%i\n", a,b);

	return 0;
}


