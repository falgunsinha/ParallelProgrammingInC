#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me,nproc,i;
	double array[40], number[10];

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	if(me==0){
        for(i=0;i<40;i++) { 
			array[i] = (i+1) / 10.0;
		}
	}

    MPI_Scatter(&array,10,MPI_DOUBLE,&number[0],10,MPI_DOUBLE,0,MPI_COMM_WORLD); 

    for(i=0;i<nproc;i++){
		if(me==i) {
			for(int j=0;j < 10;j++){
            	printf("Thread %d has value: %.1lf\n", me, number[j]);
        	}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}