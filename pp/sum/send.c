#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int me,nproc,i,n,sum=0,total, sent=0;
	int low,high;

    int dest, source=0, tag=101;
    MPI_Status status;

	MPI_Init(&argc,&argv); // initialise MPI
	MPI_Comm_size(MPI_COMM_WORLD,&nproc); // return total number of processors
	MPI_Comm_rank(MPI_COMM_WORLD,&me); // return number of this processor, me=0..nproc-1

	if(me==0){
		printf("Program for SUMMING first N integer numbers\n ");
		printf("Enter N: ");scanf("%i", &n);
	}
	
	if(me==0){
        for(dest=1;dest<=nproc-1;dest++) {
            MPI_Send(&n ,1, MPI_INT,dest,tag,MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n ,1, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
    }
    

	low = 1 + me*n/nproc;
	high = (me+1) * n/nproc;

	for(i=low;i<=high;i++) {
		sum=sum+i;
	}

    //6.A
	if(me!=0) {
        MPI_Send(&sum,1,MPI_INT,0,tag,MPI_COMM_WORLD);
        sent++;
    } else {
        total = sum;
        for(source=1;source<=nproc-1;source++) {
            MPI_Recv(&sum ,1, MPI_INT ,source,tag,MPI_COMM_WORLD,&status);
            total += sum;
        }
    }

    // reduce by send + broadcast
    tag = 110;
	if(me==0){
        for(dest=1;dest<=nproc-1;dest++) {
            MPI_Send(&total ,1, MPI_INT,dest,tag,MPI_COMM_WORLD);
            sent++;
        }
    } else {
        MPI_Recv(&total ,1, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
    }

	printf("me = %i Print A, nproc = %i, n = %i, low = %i, high = %i, count = %i, sent =%i, sum = %i\n", me, nproc, n, low, high, high-low+1, sent, sum);
	if(me==0){
		printf("Sum of first %i integers equals   : %i\n",n,total);
		printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);
	}

    printf("Me=%i, after reduced integers equals: %i\n",me,total);

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}

