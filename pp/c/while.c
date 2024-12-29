//compile with icc - intel compiler includes mathematical functions - here pow(i,j)
#include <stdio.h>

int main() {
	float c,d;
	while(0==0){
		printf("Input two numbers: "); scanf("%f %f",&c,&d);
		printf("Sum and product of the two numbers are: %f %f\n",c+d,c*d);
	}
	return 0;
} // end of MAIN
