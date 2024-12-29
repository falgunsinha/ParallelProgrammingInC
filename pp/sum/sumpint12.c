// sump.c
// sums first N integers
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me,nproc,i,n,sum=0,total,fac=1,total_prod;

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	if(me==0){
		printf("Program for summing first N integer numbers:\n ");
		printf("Enter N: ");scanf("%i", &n);
	}
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD); //broadcast the input from thread 0 to all threads

	for(i=me+1;i<=n;i=i+nproc) { 
		printf("%i Print1 for Sum, %i %i\n",me,n,sum);     
		printf("me =%i PrintA, nproc=%i, n=%i, sum=%i\n",me,nproc, n,sum);
		sum=sum+i;
		printf("%i Print2 for Factorial, %i %i\n",me,n,fac);
		fac=fac*i;
	}

	MPI_Allreduce(&sum,&total,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	
	if(me==0){
		printf("Sum of first %i integers equals   : %i\n",n,total);
		printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);
	}

	MPI_Allreduce(&fac,&total_prod,1,MPI_INT,MPI_PROD,MPI_COMM_WORLD);
	
	if(me==0){
		printf("Factorial of %i  : %i\n",n,total_prod);
		// printf("Factorial of %i  : %i\n",n,(n*(n+1))/2);
	}

	MPI_Finalize();
	return 0;
}