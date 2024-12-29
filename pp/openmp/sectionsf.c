#include <omp.h>
#include <stdio.h>
#define N     50

int main (int argc, char *argv[]) 
{
int i, nthreads, me;
double a[N], b[N], c[N], d[N];

/* Some initializations */
for (i=0; i<N; i++) {
  a[i] = i * 1.5;
  b[i] = i + 22.35;
  c[i] = d[i] = 0.0;
  }
//#pragma omp parallel default (none) shared(a,b,c,d,nthreads) private(i)
  {
  int me = omp_get_thread_num();
  if (me == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n",me);
  //#pragma omp sections nowait
    {
    //#pragma omp section
      {
      printf("Thread %d doing section 1\n",me);
      for (i=0; i<N; i++)
        {
        c[i] = a[i] + b[i];
        printf("Thread %d: c[%d]= %f\n",me,i,c[i]);
        }
      }
    //#pragma omp section
      {
      printf("Thread %d doing section 2\n",me);
      for (i=0; i<N; i++)
        {
        d[i] = a[i] * b[i];
        printf("Thread %d: d[%d]= %f\n",me,i,d[i]);
        }
      }
    }  /* end of sections */
    printf("Thread %d done.\n",me);
  }  /* end of parallel section */
  return 0;
 }
