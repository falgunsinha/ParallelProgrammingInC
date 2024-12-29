// waters.c
#include <stdio.h>
#include <math.h>
#include <time.h>     // for CPU time
#include <sys/time.h> //for gettimeofday
#include <mpi.h>
#define LENGTH 80

// global variables
const int maxnum = 100000;
double r[maxnum][3][3], rcutsq = 1.44, L;
// r(number of molecule, atom 0=O,1=H,2=H, coordinate 0=x,1=y,2=z)

double sqr(double a) { return a * a; }

double energy12(int i1, int i2)
{
    // ============================
    int m, n, xyz;
    double shift[3], dr[3], mn[3], r6, distsq, dist, ene = 0;
    const double sig = 0.3166, eps = 0.65, eps0 = 8.85e-12, e = 1.602e-19, Na = 6.022e23, q[3] = {-0.8476, 0.4238, 0.4238};
    double elst, sig6;
    elst = e * e / (4 * 3.141593 * eps0 * 1e-9) * Na / 1e3, sig6 = pow(sig, 6);

    // periodic boundary conditions
    for (xyz = 0; xyz <= 2; xyz++)
    {
        dr[xyz] = r[i1][0][xyz] - r[i2][0][xyz];
        shift[xyz] = -L * floor(dr[xyz] / L + .5); // round dr[xyz]/L to nearest integer
        dr[xyz] = dr[xyz] + shift[xyz];
    }
    distsq = sqr(dr[0]) + sqr(dr[1]) + sqr(dr[2]);
    if (distsq < rcutsq)
    { // calculate energy if within cutoff
        r6 = sig6 / pow(distsq, 3);
        ene = 4 * eps * r6 * (r6 - 1.); // LJ energy
        for (m = 0; m <= 2; m++)
        {
            for (n = 0; n <= 2; n++)
            {
                for (xyz = 0; xyz <= 2; xyz++)
                    mn[xyz] = r[i1][m][xyz] - r[i2][n][xyz] + shift[xyz];
                dist = sqrt(sqr(mn[0]) + sqr(mn[1]) + sqr(mn[2]));
                ene = ene + elst * q[m] * q[n] / dist;
            }
        }
    }
    return ene;
}

int main(int argc, char *argv[])
{
    int i = 0, j = 0, natoms = 0, nmol = 0, me = 0, nproc = 0, k = 0;
    int source = 0, dest = 0, tag = 101;
    double dtime[4], energy = 0, total = 0, load_imbalance = 0, npair = 0, maxpair = 0, minpair = 0, totalnpair = 0;
    double time_reading[2], time_calculating[2], time_total[2];
    double minreadtime[2], mincalctime[2], mintotaltime[2];
    double maxreadtime[2], maxcalctime[2], maxtotaltime[2];
    FILE *fp;
    char line[LENGTH], nothing[LENGTH], name[20];
    clock_t cputime[4], cputime1; /* clock_t defined in <time.h> and <sys/types.h> as int */
    struct timeval start, end;

    // cputime1 = clock();         // assign initial CPU time (IN CPU CLOCKS)
    // gettimeofday(&start, NULL); // returns structure with time in s and us (microseconds)

    // printf("Elapsed wall time for thread %i : %lf\n", me, dtime);
    // printf("Elapsed CPU  time for thread %i : %lf\n", me, (float)(cputime / CLOCKS_PER_SEC));
    // MPI_Status status;

    MPI_Init(&argc, &argv);                // initialise MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &me);    // return number of this processor, me=0..nproc-1

    cputime[0] = clock(); // calculate  cpu clock time as difference of times after-before
    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);
    dtime[0] = ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    cputime[0] = (float)cputime[0] / CLOCKS_PER_SEC;
    // printf("\n\tCPUTIME1: %lf \n", cputime[0]);
    // printf("\n\tFor thread %i CPUTIME is %lf\n", me, cputime[0]);

    if (me == 0)
    {
        printf("Program to calculate energy of water\n");
        printf("Input NAME of configuration file\n");
        scanf("%s", name); // reading of filename from keyboard
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);
    for (i = 0; i <= nproc - 1; i++)
    {
        if (me == i)
        {
            fp = fopen(name, "r");   // opening of file and beginning of reading from HDD
            fgets(line, LENGTH, fp); // skip first line
            fgets(line, LENGTH, fp);
            sscanf(line, "%i", &natoms);
            nmol = natoms / 3;
            printf("\nNumber of molecules %i\n", nmol);

            for (k = 0; k < nmol; k++)
            {
                for (j = 0; j <= 2; j++)
                {
                    fgets(line, LENGTH, fp);
                    sscanf(line, "%s %s %s %lf %lf %lf", nothing, nothing, nothing, &r[k][j][0], &r[k][j][1], &r[k][j][2]);
                }
            }
            printf("first line %lf %lf %lf\n", r[0][0][0], r[0][0][1], r[0][0][2]);
            fscanf(fp, "%lf", &L); // read box size
            printf("Box size %lf\n\n", L);
            fclose(fp);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    // // MPI_Bcast(&name,20,MPI_CHAR,0,MPI_COMM_WORLD);
    // MPI_Bcast(&nmol, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&L, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&r, 900000, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    cputime[1] = clock() - cputime[0]; // calculate  cpu clock time as difference of times after-before
    gettimeofday(&end, NULL);
    dtime[1] = ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    cputime[1] = (float)cputime[1] / CLOCKS_PER_SEC;
    // printf("\n\tFor thread %i CPUTIME is %lf\n", me, cputime[1]);
    for (i = me; i < nmol - 1; i = nproc + i)
    { // calculate energy as sum over all pairs
        for (j = i + 1; j < nmol; j++)
        { // if (me != 0)

            energy = energy + energy12(i, j);
            npair += 1;
        }
        // printf("For thread %i R value: %lf\n", me, r[i][j][0]);
    }
    // if (me != 0)

    // printf("For thread %i npair value: %i\n", me, npair);

    cputime[2] = clock() - cputime[0]; // calculate  cpu clock time as difference of times after-before
    gettimeofday(&end, NULL);
    dtime[2] = ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    cputime[2] = (float)cputime[2] / CLOCKS_PER_SEC;
    // printf("\n\tFor thread %i CPUTIME is %lf\n", me, cputime[2]);
    // printf("\nENERGY for thread %i\t%lf\n", me, energy);

    // printf("\nBEFORE\t%lf\n", energy);

    time_reading[0] = cputime[1] - cputime[0];
    time_reading[1] = dtime[1] - dtime[0];
    // printf("\nThread %i CPU Time: %lf, %lf\n", me, cputime[1], cputime[0]);
    // printf("Thread %i D Time: %lf, %lf\n", me, dtime[1], dtime[0]);
    // printf("Thread %i Read Time: %lf, %lf\n\n", me, time_reading[0], time_reading[1]);

    time_calculating[0] = cputime[2] - cputime[1];
    time_calculating[1] = dtime[2] - dtime[1];
    // printf("Thread %i CPU Time: %lf, %lf\n", me, cputime[2], cputime[1]);
    // printf("Thread %i D Time: %lf, %lf\n", me, dtime[2], dtime[1]);
    // printf("Thread %i Calculating Time: %lf, %lf\n\n", me, time_calculating[0], time_calculating[1]);

    time_total[0] = cputime[2] - cputime[0];
    time_total[1] = dtime[2] - dtime[0];
    // printf("Thread %i CPU Time: %lf, %lf\n", me, cputime[2], cputime[0]);
    // printf("Thread %i D Time: %lf, %lf\n", me, dtime[2], dtime[0]);
    // printf("Thread %i Total Time: %lf, %lf\n\n", me, time_total[0], time_total[1]);

    MPI_Allreduce(&energy, &total, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&npair, &totalnpair, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&npair, &maxpair, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&npair, &minpair, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    MPI_Allreduce(&time_reading[0], &minreadtime[0], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_reading[1], &minreadtime[1], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_reading[0], &maxreadtime[0], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&time_reading[1], &maxreadtime[1], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    MPI_Allreduce(&time_calculating[0], &mincalctime[0], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_calculating[1], &mincalctime[1], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_calculating[0], &maxcalctime[0], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&time_calculating[1], &maxcalctime[1], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    MPI_Allreduce(&time_total[0], &mintotaltime[0], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_total[1], &mintotaltime[1], 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&time_total[0], &maxtotaltime[0], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&time_total[1], &maxtotaltime[1], 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // MPI_Allreduce(&time_calculating, &mincalctime, 2, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    // MPI_Allreduce(&time_total, &mintotaltime, 2, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    // printf("\nTOTAL\t%lf\n", energy);

    load_imbalance = 2 * ((maxpair - minpair) / (double)(maxpair + minpair));

    // cputime[3] = clock() - cputime[3]; // calculate  cpu clock time as difference of times after-before
    // gettimeofday(&end, NULL);
    // dtime[3] = ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);

    // // cputime = clock() - cputime; // calculate  cpu clock time as difference of times after-before
    // // gettimeofday(&end, NULL);
    // // dtime = ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    // printf("Elapsed wall time for thread %i : %lf\n", me, dtime[3]);
    // printf("Elapsed CPU  time for thread %i : %lf\n", me, (float)(cputime[3] / CLOCKS_PER_SEC));

    if (me == 0)
    {
        printf("\nTotal energy %lf \n", total);
        printf("Energy per molecule %lf \n", total / nmol);
        printf("Max Pair: %lf\n", maxpair);
        printf("Min Pair: %lf\n", minpair);
        printf("\n\nTotal of pairs calculated by threads: %lf\n", totalnpair);
        printf("Total of pairs theoretically: %lf\n", (nmol * (nmol - 1) / 2));
        printf("\nLoad Imbalance: %lf\n\n", load_imbalance);

        // printf("Read Time\n");
        // printf("Minimum Read time for CPU across threads: %lf\n", minreadtime[0]);
        // printf("Minimum Wall time  time across threads: %lf\n", minreadtime[1]);
        // printf("Maximum Read time for CPU across threads: %lf\n", maxreadtime[0]);
        printf("Maximum Wall time time across threads (Reading): %lf\n", maxreadtime[1]);

        // printf("\nCalculation Time\n");
        // printf("Minimum Calculation time for CPU across threads: %lf\n", mincalctime[0]);
        // printf("Minimum Wall time  time across threads: %lf\n", mincalctime[1]);
        // printf("Maximum Calculation time for CPU across threads: %lf\n", maxcalctime[0]);
        printf("Maximum Wall time time across threads (Calculating): %lf\n", maxcalctime[1]);

        // printf("\nTotal Time\n");
        // printf("Minimum Total time for CPU across threads: %lf\n", mintotaltime[0]);
        // printf("Minimum Wall time  time across threads: %lf\n", mintotaltime[1]);
        // printf("Maximum Total time for CPU across threads: %lf\n", maxtotaltime[0]);
        printf("Maximum Wall time time across threads (Total Time): %lf\n\n", maxtotaltime[1]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Pair Calculation by thread %i: %lf\n", me, npair);

    MPI_Finalize();
    // return 0;
}