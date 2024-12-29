#include <time.h> //for CPU time
#include <sys/time.h> //for gettimeofdaygettimeofday=> wall clock
#include <mpi.h>
main()
{
clock_t cpu0,cpu1; //clock_t defined in < time.h > and sys types.h > as int
struct timeval time0, time1; // for wall clock in s and ms (us)
double dtime01; //for wall clock in s (real number)
cpu0 = clock (); //assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time0, NULL); //returns structure with time in s and us (microseconds)

// part of code to be timed
cpu1 = clock (); //assign CPU time (IN CPU CLOCKS)
gettimeofday(&time1, NULL);

dtime01 = ((time1.tv_sec - time0.tv_sec)+(time1.tv_usec - time0.tv_usec)/1e6);
printf("Elapsed wall time     CPU time\n");
printf("Duration 01 %f %f\n", dtime01,(float) (cpu1 - cpu0)/CLOCKS_PER_SEC);
}