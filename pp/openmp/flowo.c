//  flowo.c
#include<omp.h>
#include<stdio.h>

int main(){
	int me,n,nproc;

	printf("number of threads in serial part: %i\n",omp_get_num_threads());
	printf("thread id of the master thread is: %i\n",omp_get_thread_num());

	#pragma omp parallel private(me)
	{
	   	me = omp_get_thread_num();
		printf("parallel region 1 - thread id: %i\n",me);
		if(me==0)printf("number of threads in PR1: %i\n",omp_get_num_threads());
	}

	printf("input number of threads\n");scanf("%i",&n);
	omp_set_num_threads(n);

	#pragma omp parallel private(me)
	{
	   	me = omp_get_thread_num();
		printf("parallel region 2 - thread id: %i\n",me);
		if(me==0)printf("number of threads in PR2: %i\n",omp_get_num_threads());
	}

	return 0;
}
