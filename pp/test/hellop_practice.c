// hellop.c
// Prints "Hello, world"
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int i, nproc, me;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	printf("Thread %i from %i total: Hello world\n", me, nproc);

	MPI_Finalize();
	return 0;

}


