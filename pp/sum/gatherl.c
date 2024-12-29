#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me,nproc,i;
	double source[10], array[40];

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

    for(i=0;i<10;i++) { 
		source[i] = 1 * (i+1) + (me / 10.0);
        MPI_Gather(&source[i],1,MPI_DOUBLE,&array[i*nproc],1,MPI_DOUBLE,0,MPI_COMM_WORLD); // Multiple Gather
	}

    // MPI_Gather(&source,10,MPI_DOUBLE,&array[0],10,MPI_DOUBLE,0,MPI_COMM_WORLD); // Single Gather

    //printf("me=%d, i=%d,  %.1lf ", me, i-1, source[i-1]);
    
	if(me==0){
        printf("Array:");
        for(i=0;i < sizeof(array)/sizeof(array[0]);i++){
            printf("%.1lf ", array[i]);
        }
		printf("\n");
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}