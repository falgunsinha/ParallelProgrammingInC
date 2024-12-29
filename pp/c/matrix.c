#include <stdio.h>

int main() {
	int i,j,a[11][11];
	float x[20];

	for(i=0;i<20;i++) {
		x[i]=1.1*i;
		printf("Assigning x[%i]=%f\n",i,x[i]);
	}

	for(i=0;i<11;i++)
		for(j=0;j<11;j++)
			a[i][j]=i*j;

	while(i>0 && j>0){
		printf("Input two numbers to multiply: "); scanf("%i %i",&i,&j);
		printf("Their product is %i\n",a[i][j]);
	}

	return 0;
} // end of MAIN
