#include "stdio.h"

float my_sum(float a, float b) {
	return a+b;
} // end of function

int main()
{
	int i,j; float a,b,c;
	while(0==0){
		printf("Input two numbers: "); scanf("%f %f",&a,&b);
		c=my_sum(a,b);
		printf("a= %f, b= %f, sum= %f\n",a,b,c);
	}
	return 0;
}

