#include <stdio.h>
//#include <stdlib.h>
//#include <sys/param.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

int main(){
	int i,j,low,high;
	i=3;j=5;
	low=min(i,j);
	high=max(i,j);
	printf("low, high, %i %i",low,high);
	return 0;
}
