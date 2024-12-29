// sumo1.c
// sums first N integers
#include <stdio.h>
#include <omp.h>

int main(){
	int i,n,sum=0, b=5;
	int count;
	printf("Program for summing first N integer numbers\n ");
	printf("Enter N: ");scanf("%i", &n);

	
	#pragma omp parallel for private(count, i) reduction(+:sum)
	for(i=1;i<=n;i=i+1)
	{
		count++;
		sum=sum+i;
	   	printf("process %d has %d numbers\n", omp_get_thread_num(), count);
	}	
	count = 0;
	printf("b=%i\n",b);
	
	printf("Sum of first %i integers equals   : %i\n",n,sum);
	printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);

	return 0;
}

