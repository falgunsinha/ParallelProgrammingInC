// sumo.c
// sums first N integers
#include <stdio.h>
#include <omp.h>
int main(){
	int i,n,sum=0;

	printf("Program for summing first N integer numbers\n ");
	printf("Enter N: ");scanf("%i", &n);
	
	#pragma omp parallel 
	{  
		int count = 0; //meaning - decalre int count privately under parallel region (not globally)
		printf("Number of threads %i\n",omp_get_num_threads());
		
		#pragma omp for private(i) reduction(+:sum)
		for(i=1;i<=n;i=i+1) { 
			sum=sum+i;
			count++;
		}
		printf("process %d has %d numbers\n", omp_get_thread_num(), count);
	}
	
	printf("Sum of first %i integers equals   : %i\n",n,sum);
	printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);

	return 0;
}
