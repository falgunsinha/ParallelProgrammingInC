#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int me, nproc, i, n, sum = 0, total = 0, send_count_option_a = 1, send_count_option_b = 1;
    MPI_Status status;

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); 
    MPI_Comm_rank(MPI_COMM_WORLD, &me); 

    for (int num_threads = 2; num_threads <= 8; num_threads += 2) {
        if (nproc == num_threads) {
            // Option A: MPI_Reduce + MPI_Bcast
            if (me == 0) {
                printf("\nOption A with %d threads\n", num_threads);
                printf("Enter N: ");
                fflush(stdout);
                scanf("%i", &n);
            }
            MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
            for (i = me + 1; i <= n; i = i + nproc) {
                sum = sum + i;
            }
            MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if (me == 0) {
                send_count_option_a = nproc - 1;
            }
            MPI_Barrier(MPI_COMM_WORLD); // Synchronize before Option B

            // Option B: Direct MPI_Send
            if (me == 0) {
                printf("\nOption B with %d threads\n", num_threads);
                printf("Enter N: ");
                fflush(stdout);
                scanf("%i", &n);
                for (i = 1; i < nproc; ++i) {
                    MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    send_count_option_b = nproc * (nproc - 1);
                }
            } else {
                MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
            }

            for (i = me + 1; i <= n; i = i + nproc) {
                sum = sum + i;
            }
            MPI_Barrier(MPI_COMM_WORLD); // Synchronize before the next MPI_Send

            if (me != 0) {
                MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                send_count_option_b = nproc * (nproc - 1);
            } else {
                total = sum;
                for (i = 1; i < nproc; i++) {
                    MPI_Recv(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                    total += sum;
                }
            }

            if (me == 0) {
                printf("\nTotal MPI_Send for Option A: %d\n", send_count_option_a);
                printf("Total MPI_Send for Option B: %d\n", send_count_option_b);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
