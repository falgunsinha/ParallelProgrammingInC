#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int me, nproc, position;
    int m, n;
    float p;
    double q, r;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    // Values to pack
    m = 1; n = 2;
    p = 1.2; q = 1.23e10; r = 0.456;

    char buffer[1000];
    

    if(me==00)
    {
        position=0;

        MPI_Pack(&m, 1, MPI_INT, &buffer, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(&n, 1, MPI_INT, &buffer, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(&p, 1, MPI_FLOAT, &buffer, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(&q, 1, MPI_DOUBLE, &buffer, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(&r, 1, MPI_DOUBLE, &buffer, 1000, &position, MPI_COMM_WORLD);
        
        printf("Positions after packing:\n");
        printf("m: %d\n", position);
        printf("n: %d\n", position);
        printf("p: %d\n", position);
        printf("q: %d\n", position);
        printf("r: %d\n", position);
    }
    
        MPI_Bcast(buffer, 1000, MPI_CHAR, 0, MPI_COMM_WORLD);

    if(me!=00)
    {
        position=0;
        MPI_Unpack(&buffer, 1000, &position, &m, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(&buffer, 1000, &position, &n, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(&buffer, 1000, &position, &p, 1, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(&buffer, 1000, &position, &q, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(&buffer, 1000, &position, &r, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        
        printf("Positions after unpacking:\n");
        printf("m: %d\n", position);
        printf("n: %d\n", position);
        printf("p: %d\n", position);
        printf("q: %d\n", position);
        printf("r: %d\n", position);

        printf("Received values:\n");
        printf("m: %d\n", m);
        printf("n: %d\n", n);
        printf("p: %f\n", p);
        printf("q: %lf\n", q);
        printf("r: %lf\n", r);
    }
    
    MPI_Finalize();
    return 0;
}
