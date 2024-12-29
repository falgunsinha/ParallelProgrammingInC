#include <stdio.h>
#define LENGTH 200

int main(){
	int i=0,n;
	char line[LENGTH];
    double a2,a8;
    double myarray[1000][2];
	FILE *fp;

	fp=fopen("table.txt", "r");

	while(1){ //1 serves as true, i.e. condition which is always true
		if(fgets(line, LENGTH,fp)==NULL) 
			break; // finish reading at EOF
		if(sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &a2, &a2, &a8, &a8, &a8, &a8, &a8, &a8)==-1){
			printf("Blank line # %i\n",i+1);
			break;
		} // finish reading after blank line
		if(sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &a2, &a2, &a8, &a8, &a8, &a8, &a8, &a8)==0){
			printf("Not a number on line # %i\n",i+1); 
			break;
		} // finish reading after error
		
		i++;
        myarray[i-1][0] = a8;
        myarray[i-1][1] = a2;
		printf("%i. number: %lf %lf\n", i,a8,a2);
	}
	
	printf("Read %i numbers\n",i);n=i;
	fclose(fp);

	fp=fopen("tableout.txt", "w");
	for(i=0;i<n;i++){
		fprintf(fp, "%i %lf %lf\n", i+1, myarray[i][0], myarray[i][1]);
    }
	fclose(fp);

	return 0;
}

//input.txt
//2.2453
//1.7
//8.9
//10.0
