// hellop.c
// Prints "Hello, world"
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int me,nproc,i;
	
	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processes
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this process, me=0..nproc-1
	printf("Thread %i from %i total: Hello, world\n",me,nproc);
	
	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}

