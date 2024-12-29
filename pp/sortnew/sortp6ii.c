#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#define max_len 400000
#define LENGTH 40

double timediff(struct timeval start, struct timeval end)
{
	return ((end.tv_sec  - start.tv_sec)+(end.tv_usec - start.tv_usec)/1e6);
}

int main(int argc, char* argv[]){
	int number_of_lines=0;
	int swap;
	char in_name[] = "sort.txt";
	char out_name[] = "sorted-p1.txt";
	char line[LENGTH];
	int items[max_len],max,min;
	FILE *fp;
	struct timeval t1,t2;
	double time;

	int me,nproc,j,items1[max_len];
	long long int split;
	long long int low,high;

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	//read the input
	if(me==0){
	fp=fopen(in_name,"r");
  if(fp==NULL){printf("Input file %s not found\n",in_name);  return 0;}
	while(1)
	{
		if(fgets(line, LENGTH,fp)==NULL) break; // finish reading when empty line is read
		if(sscanf(line, "%d",&items[number_of_lines])==-1) break; // finish reading after error
			
		number_of_lines++;
	}

	printf("Number of items: %i\n",number_of_lines);
	max=items[0];
	min=items[0];
	for (int i = 1; i < number_of_lines; i++){
		if (items[i] > max) max=items[i];
		if (items[i] < min) min=items[i];
	}
	printf("%i,%i\n",max,min);
	}
	MPI_Bcast(&number_of_lines,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&items,number_of_lines,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&max,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&min,1,MPI_INT,0,MPI_COMM_WORLD);


	gettimeofday(&t1, NULL);

	if(min<0){
		if(max>0){
			split=(long long int)(max-min)/nproc;
		}
		else{
			split=(long long int)(-1*(max+min))/nproc;
		}
	}else{
		split=(long long int)(max+min)/nproc;
	}

	if(me==1) printf("%lld\n",split);

	j=0;
	low=min+me*split;
	high=min+(me+1)*split;
	for(int i=0;i<number_of_lines;i++)
	{
		if (((items[i] >= low) & (items[i] < high)) | ((me==nproc-1) & items[i]>=high))
		{
				items1[j]=items[i];
				j=j+1;
		}
	}

	//sorting bubble sort
	for (int i = 0; i < j-1; i++)
	{
		for (int k = 0; k < j-i-1; k++)
		{	
			if (items1[k] > items1[k+1])
			{
				swap = items1[k];
				items1[k] = items1[k+1];
				items1[k+1] = swap;
			}

		}
	}

	gettimeofday(&t2, NULL);
	time = timediff(t1,t2);

	printf("time: %lf\n", time);

	
	fp = fopen(out_name, "w");
	fclose(fp);
	MPI_Barrier(MPI_COMM_WORLD);

	for(int p=0;p<nproc;p++)
	{
		if(p==me){
			fp = fopen(out_name, "a");
			for(int i = 0; i < j; i++) 
			{
				fprintf(fp,"%d\n",items1[i]);
			}
			fclose(fp);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	// fclose(fp);
	
	MPI_Finalize(); 
}