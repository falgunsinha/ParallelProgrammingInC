#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int me,nproc,i,n,sum=0,total;
	int low,high;

    int dest, source=0, tag=101;
	int arr[3];
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
			if(me!=4) {//3
            	MPI_Send(&n ,1, MPI_INT,dest,tag,MPI_COMM_WORLD);
			}
        }
    } else if(me==2){ //2
		sleep(3);
		MPI_Recv(&n ,1, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
	} else if(me==3) { //2
		
	} else {
        MPI_Recv(&n ,1, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
    }

	if(me==0){ //4
		arr[0] = 1; arr[1] = 2; arr[2] = 3;
        for(i=0;i<=2;i++) {
			MPI_Send(&arr[i],1, MPI_INT,6,102,MPI_COMM_WORLD);
        }
    } else if(me==6) {
		MPI_Recv(&arr[0] ,1, MPI_INT,0,102,MPI_COMM_WORLD,&status);
		MPI_Recv(&arr[1] ,1, MPI_INT,0,102,MPI_COMM_WORLD,&status);
		printf("4. Thread 6, arr = {%d, %d, %d}\n", arr[0], arr[1], arr[2]);
	}

	if(me==0){ //5
        MPI_Send(&n ,1, MPI_INT,7,103,MPI_COMM_WORLD);
		MPI_Send(&n ,1, MPI_INT,7,104,MPI_COMM_WORLD);
    } else if(me==7) {
		MPI_Recv(&n ,1, MPI_INT,0,104,MPI_COMM_WORLD,&status);
		MPI_Recv(&n ,1, MPI_INT,0,103,MPI_COMM_WORLD,&status);
		printf("5. Thread 7, n = %d\n", n);
	}
    

	low = 1 + me*n/nproc;
	high = (me+1) * n/nproc;

	for(i=low;i<=high;i++) {
		sum=sum+i;
	}

	if(me!=0) {
        MPI_Send(&sum,1,MPI_INT,0,tag,MPI_COMM_WORLD);
    } else {
        total = sum;
        for(source=1;source<=nproc-1;source++) {
            MPI_Recv(&sum ,1, MPI_INT ,source,tag,MPI_COMM_WORLD,&status);
            total += sum;
        }
    }

	printf("me = %i Print A, nproc = %i, n = %i, low = %i, high = %i, count = %i, sum = %i\n", me, nproc, n, low, high, high-low+1, sum);
	if(me==0){
		printf("Sum of first %i integers equals   : %i\n",n,total);
		printf("Sum of first %i integers should be: %i\n",n,(n*(n+1))/2);
	}

	tag = 110;
	if(me==0){ //6
        for(dest=1;dest<=nproc-1;dest++) {
            MPI_Send(&total ,1, MPI_INT,dest,tag,MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&total ,1, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
    }

	printf("Me=%i, after reduced integers equals: %i\n",me,total);

	MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

	return 0;
}

