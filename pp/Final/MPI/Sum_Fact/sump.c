// sump.c
// sums first N integers
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me, nproc, i, n, sum=0, total;
	int low, high;
	double prod=1, total_prod;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	if(me==0){
		printf("Program for summing first N integer numbers\n ");
		printf("Enter N: ");
        scanf("%i", &n);
	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* for(i= me+1; i <= n; i=i+nproc)
    { 
		//sum=sum+i;
		//prod=prod*i;
	} */

	low = 1 + me * n/nproc;
	high = (me+1) * n/nproc;

	for(i=low; i<=high; i++) {
		sum=sum+i;
		prod=prod*i;
	}

	MPI_Allreduce(&sum, &total, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(&prod, &total_prod, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);
	
    printf("me = %i Print A, nproc = %i, n = %i, low = %i, high = %i, count = %i, sum = %i, prod = %lf\n", me, nproc, n, low, high, high-low+1, sum, prod);
	if(me==0){
		printf("Sum of first %i integers equals   : %i\n",n,total);
		printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);

		printf("Factorial of %i integers equals   : %lf\n",n,total_prod);
	}

	MPI_Finalize();
    return 0;
}

