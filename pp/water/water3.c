#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "mpi.h"

#define LENGTH 80
#define MAXNUM 100000

double rcutsq = 1.44;

void read_file(char name[], int *nmol, double *box_size, double data[][3][3])
{
    FILE *fp;
    int natoms;
    char line[LENGTH], nothing[LENGTH];

    fp = fopen(name, "r");
    fgets(line, LENGTH, fp);
    fgets(line, LENGTH, fp);
    sscanf(line, "%i", &natoms);

    *nmol = natoms / 3;

    // Read in
    for (int i = 0; i < *nmol; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            fgets(line, LENGTH, fp);
            sscanf(line, "%s %s %s %lf %lf %lf", nothing, nothing, nothing,
                   &data[i][j][0], &data[i][j][1], &data[i][j][2]);
        }
    }

    fscanf(fp, "%lf", box_size);
    fclose(fp);
}

void read_seq(char name[], int me, int nproc, int *nmol, double *box_size,
              double data[][3][3])
{
    for (int i = 0; i <= nproc; i++)
    {
        if (i == me)
        {
            printf("%i is reading\n", me);
            read_file(name, nmol, box_size, data);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

double sqr(double a) { return a * a; }

double energy12(int i1, int i2, double L, double r[][3][3])
{
    int m, n, xyz;
    double shift[3], dr[3], mn[3], r6, distsq, dist, ene = 0;
    const double sig = 0.3166, eps = 0.65, eps0 = 8.85e-12, e = 1.602e-19,
                 Na = 6.022e23, q[3] = {-0.8476, 0.4238, 0.4238};
    double elst, sig6;
    elst = e * e / (4 * 3.141593 * eps0 * 1e-9) * Na / 1e3, sig6 = pow(sig, 6);

    // periodic boundary conditions
    for (xyz = 0; xyz <= 2; xyz++)
    {
        dr[xyz] = r[i1][0][xyz] - r[i2][0][xyz];
        shift[xyz] = -L * floor(dr[xyz] / L + .5);
        dr[xyz] = dr[xyz] + shift[xyz];
    }

    distsq = sqr(dr[0]) + sqr(dr[1]) + sqr(dr[2]);

    if (distsq < rcutsq)
    { // calculate energy if within cutoff
        r6 = sig6 / pow(distsq, 3);
        ene = 4 * eps * r6 * (r6 - 1.);
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

float get_time_diff(struct timeval start, struct timeval end)
{
    return ((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
}

int main(int argc, char *argv[])
{

    // Metrics
    struct timeval ts1, ts2, ts3;
    float walltime1, walltime2, walltime3;
    float walltime1min, walltime2min, walltime3min;
    float walltime1max, walltime2max, walltime3max;
    clock_t cputs1, cputs2, cputs3;
    float cputime1, cputime2, cputime3;
    float cputime1min, cputime2min, cputime3min;
    float cputime1max, cputime2max, cputime3max;

    // Data
    char name[20];
    int me, nproc, nmol;
    int npair = 0, npairmin = 0, npairmax = 0;
    double box_size, energy = 0.0, energy_total = 0.0;
    double data[MAXNUM][3][3];

    // Init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if (me == 0)
    {
        printf("Program to calculate energy of water\n");
        printf("Input NAME of configuration file : ");
        scanf("%s", name);
    }

    MPI_Bcast(&name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    // First metrics
    gettimeofday(&ts1, NULL);
    cputs1 = clock();

    // Read file
    read_seq(name, me, nproc, &nmol, &box_size, data);

    // Second metrics
    gettimeofday(&ts2, NULL);
    cputs2 = clock();

    // Calculation
    for (int i = me; i < nmol - 1; i += nproc)
    {
        for (int j = i + 1; j < nmol; j++)
        {
            energy += energy12(i, j, box_size, data);
            npair++;
        }
    }

    // Third metrics
    gettimeofday(&ts3, NULL);
    cputs3 = clock();

    // Output
    printf("%i result %lf\n", me, energy);
    printf("%i calced %i\n", me, npair);

    walltime1 = get_time_diff(ts1, ts2);
    walltime2 = get_time_diff(ts1, ts3);
    walltime3 = get_time_diff(ts2, ts3);
    cputime1 = (float)(cputs2 - cputs1) / CLOCKS_PER_SEC;
    cputime2 = (float)(cputs3 - cputs1) / CLOCKS_PER_SEC;
    cputime3 = (float)(cputs3 - cputs2) / CLOCKS_PER_SEC;

    printf("%i Walltime 1: \t%f\n", me, walltime1);
    printf("%i Walltime 2: \t%f\n", me, walltime2);
    printf("%i Walltime 3: \t%f\n", me, walltime3);
    printf("%i Cputime 1: \t%f\n", me, cputime1);
    printf("%i Cputime 2: \t%f\n", me, cputime2);
    printf("%i Cputime 3: \t%f\n", me, cputime3);

    MPI_Reduce(&energy, &energy_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Collect metrics
    MPI_Reduce(&npair, &npairmin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&npair, &npairmax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&walltime1, &walltime1min, 1, MPI_FLOAT, MPI_MIN, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&walltime1, &walltime1max, 1, MPI_FLOAT, MPI_MAX, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&walltime2, &walltime2min, 1, MPI_FLOAT, MPI_MIN, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&walltime2, &walltime2max, 1, MPI_FLOAT, MPI_MAX, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&walltime3, &walltime3min, 1, MPI_FLOAT, MPI_MIN, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&walltime3, &walltime3max, 1, MPI_FLOAT, MPI_MAX, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&cputime1, &cputime1min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cputime1, &cputime1max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cputime2, &cputime2min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cputime2, &cputime2max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cputime3, &cputime3min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cputime3, &cputime3max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Here so the final results are definetely at the bottom
    MPI_Barrier(MPI_COMM_WORLD);

    if (me == 0)
    {
        printf("Npairmin: %i\n", npairmin);
        printf("Npairmax: %i\n", npairmax);
        printf("Imbalance: %f\n",
               2. * (npairmax - npairmin) / (0. + npairmax + npairmin));
        printf("Walltime no:\tmin\t\tmax\n");
        printf("\t1\t%f\t%f\n", walltime1min, walltime1max);
        printf("\t2\t%f\t%f\n", walltime2min, walltime2max);
        printf("\t3\t%f\t%f\n", walltime3min, walltime3max);
        printf("Cputime no:\tmin\t\tmax\n");
        printf("\t1\t%f\t%f\n", cputime1min, cputime1max);
        printf("\t2\t%f\t%f\n", cputime2min, cputime2max);
        printf("\t3\t%f\t%f\n", cputime3min, cputime3max);
        printf("\n\n\nFinal result: %lf\n", energy_total);
    }

    MPI_Finalize();

    return 0;
}