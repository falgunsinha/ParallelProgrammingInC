// write.c - reads numbers from input.txt and writes than to output.txt together with their squares
#include <stdio.h>
#define LENGTH 80

int main(){
	int i=0, n;
	double number[1000];
	char line[LENGTH];
	FILE *fp;

	fp=fopen("input.txt", "r");

	while(1)
	{
		if(fgets(line, LENGTH,fp)==NULL) 
			break;
		if(sscanf(line, "%lf", &number[i])==-1)
		{
			printf("Blank line # %i\n", i+1);
			break;
		}
		if(sscanf(line, "%lf", &number[i])==0)
		{
			printf("Not a number on line # %i\n", i+1); 
			break;
		}
		
		i++;
		printf("%i. number: %lf\n", i,number[i-1]);
	}
	
	printf("Read %i numbers\n",i);
	n = i;
	fclose(fp);

	fp=fopen("output.txt", "w");
	for(i=0; i<n; i++)
	{
		fprintf(fp, "Number %lf square %lf\n", number[i], number[i]*number[i]);
	}

	fclose(fp);
	return 0;
}

//input.txt
//2.2453
//1.7
//8.9
//10.0
