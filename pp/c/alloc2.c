// alloc.c - example of dynamic allocation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
long int my_power(long int x, int n){
	long int result=1;
	for (int i=1;i<=n;i++) result *= x;
	return result;
}

int main(){
	int i,n;
	long int li;
	long long int lli;
	float fn; double dn;
	long double ld;
	double *number;

	printf("size of int = %i\n",sizeof(int));
	printf("size of float = %i\n",sizeof(float));
	printf("size of double = %i\n",sizeof(double));
	printf("size of double = %i\n",sizeof(long double));
	printf("size of char = %i\n",sizeof(char));
	printf("size of long int = %i\n",sizeof(long int));
	printf("size of long long int = %i\n",sizeof(long long int));
	printf("pi      , %20.14f\n",acos(-1.));
	fn=acos(-1.);dn=acos(-1.);
	printf("pi float, %20.14lf\n",fn);
	printf("pi double, %20.14lf\n",dn);
	ld=acos(-1.);
	printf("pi long double, %20.14f\n",ld);
	ld =-1.;ld=acos(ld);
	printf("pi long double, %20.14f\n",ld);
	i=pow(2,32); printf("pi int 2^32, %40i\n",i);
	li=pow(2,32); printf("pi long int 2^32, %40li\n",li);
	li=my_power(2,32); printf("pi long int 2^32, %40li\n",li);
	lli=pow(2,32); printf("pi long long int 2^32, %40li\n",lli);

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
