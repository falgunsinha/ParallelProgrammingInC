//compile with icc - intel compiler includes mathematical functions - here pow(i,j)
#include <stdio.h>

int main()
{
	float a, b;

	while(0 == 0)
	{
		printf("Enter two numbers:\n");
		scanf("%f %f", &a, &b);
		printf("Sum and product of two numbers are: %4.1f, %4.1f\n", a+b, a*b);
	}
	return 0;
}