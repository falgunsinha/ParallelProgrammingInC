#include <stdio.h>
#include <math.h>
#define min(a,b) (a<b ? a : b)
#define max(a,b) (a>b ? a : b)

int main() {
	int i,j;
	float x,y;

	printf(" Input values of x and y: ");
	scanf("%f %f", &x,&y);
	printf(" Minimum and maximum of x and y: %f %f\n", min(x,y), max(x,y));
	printf(" Rounded values of x and y: %i %i\n", (int) floor(x+0.5), (int) floor(y+0.5));
	if(x<y) 
		printf("x<y\n");
	else
		printf("x>=y\n");

	if(x<y) 
		printf("x<y\n");
	else {
		if(x>y) 
			printf("x>y\n");
		else
			printf("x=y\n");
	}

	if(x==y) printf("x and y are equal\n");
	if(x!=y) printf("x and y are different\n");

	printf("\nSwitch\n");
	i=x; //implicit truncation of decimal digits
	printf("i= %i\n",i);
	switch (i)
	{ case 0: printf("i=0\n"); break;
		case 1: printf("i=1\n"); break;
		default: printf("i!=0,i!=1\n");
	}

	return 0;
} // end of MAIN
