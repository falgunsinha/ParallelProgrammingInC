#include <stdio.h>
#include <limits.h>
#define max_len 400000
#define LENGTH 40

main(){
	int line_num=1,len;
	long number, prev_number = LONG_MIN;
	char name[LENGTH]="sorted-p.txt",line[LENGTH];
	FILE *fp;

	//printf("Input filename\n"); scanf("%s",name);
	fp=fopen(name,"r");
  if(fp==NULL){printf("File sorted-p.txt not found\n");  return 0;}
	while(1) //1 serves as true, i.e. condition which is always true
	{
		if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
		if(sscanf(line, "%ld",&number)==-1) break; // finish reading after error
		if(number<prev_number)
		{
			printf("Number %ld in line %i is not in order\n",number,line_num); 
			return 0;
		}
		line_num++;
		prev_number = number;
	}
	len=line_num-1;
	fclose(fp);
	printf("Number of items sorted: %i\n",len);
}

