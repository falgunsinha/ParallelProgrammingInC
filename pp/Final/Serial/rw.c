// rw.c - reads numbers from input.txt and writes than to outputrw.txt together with their third power
#include <stdio.h>
#define LENGTH 80

int main()
{
	int i=0;
	double number;
	char line[LENGTH];
	FILE *fr, *fw;
	
	fr=fopen("input.txt", "r");
	fw=fopen("outputrw.txt", "w");
	
	while(1)
	{
		if(fgets(line, LENGTH, fr)==NULL)
			break;
		if(sscanf(line, "%lf", &number)==-1) 
			break;
		
		i++;
		printf("%i. number: %lf\n", i, number);
		fprintf(fw, "Number %lf third power %lf\n", number, number*number*number);
	}
	
	printf("Read %i numbers\n", i);
	
	fclose(fr);
	fclose(fw);
	
	return 0;
}

//input.txt
//2.2453
//1.7
//8.9
//10.0
