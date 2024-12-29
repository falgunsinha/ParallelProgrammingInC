// reads number of greetings from keyboard and then greets repeatedly
#include <stdio.h>

int main()
{
	int i,num;
	printf("Input number of greetings: ");scanf("%i", &num);
	for(i=1;i<=num;i=i+1) { 
		printf("%2i. Hello, world\n",i);
	}

	return 0;
}

