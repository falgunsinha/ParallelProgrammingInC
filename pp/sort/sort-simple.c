//sort-simple.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#define max_len 40000
#define LENGTH 40

int main(){
	int i=1,len,j,k;
	double b[max_len+1],c[max_len+1],bnew,cnew,time;
	char name[LENGTH]="1k.txt",line[LENGTH];
	FILE *fp;
	clock_t cpu0,cpu1,cpu2,cpu3; // clock_t defined in <time.h> and <sys/types.h> as int
	struct timeval time0, time1,time2,time3; // for wall clock in s and us
	double  dtime12,dtime03; // for wall clock in s (real number)

	cpu0 = clock();    // assign initial CPU time (IN CPU CLOCKS)
	gettimeofday(&time0, NULL); // returns structure with time in s and us (microseconds)
	//printf("Input filename\n"); scanf("%s",name);
	fp=fopen(name,"r");

	while(1){ //1 serves as true, i.e. condition which is always true
		if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
		if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
		i++;
	}
	
	len=i-1;fclose(fp);
	
	printf("Number of items to sort: %i\n",len);
	
	cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
	gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
	
	for(j=2;j<=len;j++){ // start sorting with the second item
		bnew=b[j];cnew=c[j];
		for(i=1;i<j;i++){
			if(bnew==b[i]) printf("Equal numbers %lf\n",bnew);
			if((bnew<b[i]) | ((bnew==b[i])&(cnew<c[i])))
			{
				for(k=j;k>i;k--) { b[k]=b[k-1]; c[k]=c[k-1]; } //shift larger numbers
				b[i]=bnew; c[i]=cnew; //insert number j on ith position
				break; //no need to check larger items which are already sorted !
			} // else do nothing - j is the largest number
		}   // continue checking other numbers if larger or not
	}
	
	cpu2 = clock();    // assign CPU time (IN CPU CLOCKS)
	gettimeofday(&time2, NULL);
	
	dtime12 = ((time2.tv_sec  - time1.tv_sec)+(time2.tv_usec - time1.tv_usec)/1e6);
	
	printf("Elapsed wall time sorting         CPU time\n");
	printf("Duration 12 %f %f\n", dtime12,(float) (cpu2-cpu1)/CLOCKS_PER_SEC);
	
	fp=fopen("sorted.txt","w");
	
	for(i=1;i<=len;i++){
		fprintf(fp,"%lf %lf\n",b[i],c[i]);
	}
	
	fclose(fp);
	
	cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
	gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
	
	dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
	
	printf("Elapsed wall time complete         CPU time\n");
	printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
	
	return 0;
}

