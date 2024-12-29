// sumo2.c
// sums first N integers and calculates factorial
#include <stdio.h>
#include <omp.h>

double factorial (int num){
	double result=1;
	for (int i=1; i<=num; ++i)
		result*=i;
	return result;
}

int main()
{
	int i,n,sum=0;double prod=1;

	printf("Program for summing and multiplication of first N integer numbers\n ");
	printf("Enter N: ");scanf("%i", &n);

	#pragma omp parallel for private(i) reduction(+:sum) reduction (*:prod)
	for(i=1;i<=n;i=i+1){
		sum+=i;prod*=i;printf(" number %i, thread %i\n",i,omp_get_thread_num());
	}
	
	printf("Sum of first %i integers equals   : %i\n",n,sum);
	printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);
	printf("Product of first %i integers equals   : %5.0lf\n",n,prod);
	printf("Product of first %i integers should be: %5.0lf\n",n,factorial(n));

	return 0;
}

