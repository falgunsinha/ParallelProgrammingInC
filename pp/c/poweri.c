//compile with icc - intel compiler includes mathematical functions - here pow(i,j)
#include <stdio.h>
#include <math.h>

int main() {
	int i,j;
	int a[7][7];
	// int i,j, a[7][7];
	
	for (i=1;i<=6;i++) {
		for (j=1;j<=6;j++) 
		a[i][j]=pow(i,j); // pow(3,2) equals 3^2=9; pow(2,3) equals 2^3=8
	}

	for (i=1;i<=6;i++) {
		for (j=1;j<=6;j++) {
			printf("%8i ",a[i][j]);
		}
		printf("\n");
	}
	return 0;
} // end of MAIN
