// alloc.c - example of dynamic allocation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	int i,n;
	double *number;

	printf("Enter number of values: ");
	scanf("%i",&n);

	number = malloc(sizeof(double)*n);

	for(i=1;i<=n;i++)
		number[i-1]=sqrt(i);
	printf("%i values were assigned\n",n);

	for(i=1;i<=n;i++)
		printf("number[%i]=%f\n",i,number[i-1]);
	free(number);

	return 0;
}
