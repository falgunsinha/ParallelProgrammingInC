#include <omp.h>
#include <stdio.h>
main(){
int n;
float sum;

n=6;sum=0;
#pragma omp parallel default(none) shared(n,sum)
  {int me=omp_get_thread_num();
  printf("Number of threads %i\n",omp_get_num_threads());
  #pragma omp sections
  {
    #pragma omp section
    for(int i=1;i<=n;i++){
      sum=sum+i;
      printf("%i i= %i\n",me,i);
    }
    #pragma omp section
    printf("%i A\n",me);
    #pragma omp section
    {
     printf("%i B1\n",me);
     printf("%i B2\n",me);
    }
    #pragma omp section
    printf("%i C\n",me);
  } // end of sections
  printf("sum= %f %i\n",sum,omp_get_thread_num());
} // end of parallel region

printf("Sum of numbers is %f %f\n",sum,(n*(n+1.0))/2);
return 0;
}

