// sums.c
// sums first N integers
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int i, n, me, nproc, sum=0, total;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	if(me == 0)
	{
		printf("Program for summing first N integer numbers:\n");
		printf("Enter N: ");
		scanf("%i", &n);
	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(i=me+1;i<=n;i=i+nproc) 
	{ 
		sum=sum+i;
	}
	
	MPI_Allreduce(&sum, &total, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	
	if(me == 0)
	{
		printf("Sum of first %i intergers equals  : %i\n", n, total);
		printf("Sum of first %i integers should be: %i\n", n, (n*(n+1)/2));
	}

	MPI_Finalize();
	return 0;
}

