//  flowo+threads.c in ~predota/pp/openmp
#include<omp.h>
#include<stdio.h>

int main(){
	int me,n,nproc,i;

	printf("number of threads in serial part: %i\n",omp_get_num_threads());
	printf("thread id of the master thread is: %i\n",omp_get_thread_num());

	printf("input number of threads to set by #pragma\n");
	scanf("%i",&n);

	#pragma omp parallel num_threads(n)
	printf("number of threads in parallel part: %i\n",omp_get_num_threads());

	printf("number of threads in serial part: %i\n",omp_get_num_threads());

	#pragma omp parallel for num_threads(n)
	for(i=1;i<=20;i++) 
		printf("Thread %i, i=%i\n",omp_get_thread_num(),i);

	#pragma omp parallel private(me)
	{
	   	me = omp_get_thread_num();
		printf("parallel region 1 - thread id: %i\n",me);
		if(me==0)printf("number of threads in PR1: %i\n",omp_get_num_threads());
	}

	printf("input number of threads\n");
	scanf("%i",&n);
	omp_set_num_threads(n);

	#pragma omp parallel private(me)
	{
		me = omp_get_thread_num();
		printf("parallel region 2 - thread id: %i\n",me);
		if(me==0)printf("number of threads in PR2: %i\n",omp_get_num_threads());
	}

	return 0;
}
