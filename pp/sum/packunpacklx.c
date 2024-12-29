#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me,nproc,position=0,size=1000;
    char buffer[size];
	//int buffer[size];
	//double buffer[size];
	int m,n;
	double p,q,r;

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1
    
	if(me==0){
		m=1,n=2;
        double p=1.2, q=123e10, r=0.456;
        MPI_Pack(&m,1,MPI_INT,&buffer,size,&position,MPI_COMM_WORLD);
		printf("Pos m:%d\n", position);
		MPI_Pack(&n,1,MPI_INT,&buffer,size,&position,MPI_COMM_WORLD);
		printf("Pos n:%d\n", position);

		MPI_Pack(&p,1,MPI_DOUBLE,&buffer,size,&position,MPI_COMM_WORLD);
		printf("Pos p:%d\n", position);
		MPI_Pack(&q,1,MPI_DOUBLE,&buffer,size,&position,MPI_COMM_WORLD);
		MPI_Pack(&r,1,MPI_DOUBLE,&buffer,size,&position,MPI_COMM_WORLD);

		MPI_Bcast(buffer,size,MPI_CHAR,0,MPI_COMM_WORLD);
	} else {
		MPI_Bcast(buffer,size,MPI_CHAR,0,MPI_COMM_WORLD);
		MPI_Unpack(&buffer,size,&position,&m,1,MPI_INT,MPI_COMM_WORLD);
		MPI_Unpack(&buffer,size,&position,&n,1,MPI_INT,MPI_COMM_WORLD);
		MPI_Unpack(&buffer,size,&position,&p,1,MPI_DOUBLE,MPI_COMM_WORLD);
		MPI_Unpack(&buffer,size,&position,&q,1,MPI_DOUBLE,MPI_COMM_WORLD);
		MPI_Unpack(&buffer,size,&position,&r,1,MPI_DOUBLE,MPI_COMM_WORLD);

		printf("Buffer: %s %d %d\n", buffer, buffer[0], buffer[5]);

		printf("Thread: %d, m:%d, n:%d, p:%.3lf, q:%.3e, r:%.3lf\n",me,m,n,p,q,r);
	}

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}