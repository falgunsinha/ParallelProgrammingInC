//compile: icc ceil.c -o ceil.out

#include <stdio.h>
#include <math.h>

int main() {
	float c;
	while(0==0){
		printf("Input number: "); scanf("%f",&c);
		printf("c= %f, floor: %f, ceil: %f, round: %f\n",c,floor(c),ceil(c),round(c));
		printf("c= %f, floor: %f, ceil: %f, round: %i\n",c,floor(c),ceil(c),(int)round(c));
	}
	return 0;
} // end of MAIN
