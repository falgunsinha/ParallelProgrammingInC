// sums.c
// sums first N integers
#include <stdio.h>

int main()
{
	int i,n,sum=0;

	printf("Program for summing first N integer numbers\n");
	printf("Enter N: ");
	scanf("%i", &n);
	
	for(i=1;i<=n;i=i+1) { 
		sum=sum+i;
	}
	
	printf("Sum of first %i integers equals   : %i\n",n,sum);
	printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);
	
	return 0;
}
