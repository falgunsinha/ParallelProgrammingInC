#include <omp.h>
#include <stdio.h>
#define N 50

int main(int argc, char *argv[]) {
    int i, nthreads, me;
    double a[N], b[N], c[N], d[N];
    int n = 6;
    float sum = 0;

    /* Some initializations for Code 1 */
    for (i = 0; i < N; i++) {
        a[i] = i * 1.5;
        b[i] = i + 22.35;
        c[i] = d[i] = 0.0;
    }

    /* Parallel Section combining Code 1 and Code 2 */
    #pragma omp parallel default(none) shared(a, b, c, d, n, sum, nthreads) private(i, me)
    {
        me = omp_get_thread_num();
        if (me == 0) {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", me);

        /* Code 1 Sections */
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                printf("Thread %d doing section 1\n", me);
                for (i = 0; i < N; i++) {
                    c[i] = a[i] + b[i];
                    printf("Thread %d: c[%d]= %f\n", me, i, c[i]);
                }
            }

            #pragma omp section
            {
                printf("Thread %d doing section 2\n", me);
                for (i = 0; i < N; i++) {
                    d[i] = a[i] * b[i];
                    printf("Thread %d: d[%d]= %f\n", me, i, d[i]);
                }
            }
        } /* end of sections */

        /* Additional Sections (from Code 2) */
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                printf("Thread %d doing section 3\n", me);
                for (i = 0; i < N; i++) {
                    c[i] = a[i] - b[i];
                    printf("Thread %d: c[%d]= %f\n", me, i, c[i]);
                }
            }

            #pragma omp section
            {
                printf("Thread %d doing section 4\n", me);
                for (i = 0; i < N; i++) {
                    d[i] = a[i] / b[i];
                    printf("Thread %d: d[%d]= %f\n", me, i, d[i]);
                }
            }
        } /* end of sections */

        /* Code 2 Sections */
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                printf("Thread %d doing section 5\n", me);
                for (i = 1; i <= n; i++) {
                    sum = sum + i;
                    printf("Thread %d: i= %d\n", me, i);
                }
            }

            #pragma omp section
            printf("Thread %d: A\n", me);

            #pragma omp section
            {
                printf("Thread %d: B1\n", me);
                printf("Thread %d: B2\n", me);
            }

            #pragma omp section
            printf("Thread %d: C\n", me);
        } /* end of sections */

        printf("Thread %d done.\n", me);
    } /* end of parallel section */

    #pragma omp master
    printf("Sum of numbers is %f %f\n", sum, (n * (n + 1.0)) / 2);

    return 0;
}