#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <unistd.h>

int main()
{
	double myArray[6] = {};

	#pragma omp parallel
	{
		double private_myArray[6] = {};

		#pragma omp for 
		for (int i=1; i<=50; i++){
			int a = 1; // Or something non-trivial justifying the parallelism...
			
			for (int n = 0; n<6; n++){
				private_myArray[n] += 1/pow(i,n);
			}
		}

		#pragma omp critical
		sleep (1);
		printf("thread %i private_myArray",omp_get_thread_num());
		
		for (int n = 0; n<6; ++n){
			myArray[n] += private_myArray[n];
			printf("% lf",private_myArray[n]);
		}
		
		printf("\n");
	} // end of parallel region
	
	// Print the array elements to see them summed   
	for (int n = 0; n<6; ++n){
		double tmp=0;
		for (int i=1; i<=50; i++) tmp+=1/pow(i,n);
		printf("myArray[%i]=%lf, serial %lf\n",n,myArray[n],tmp);
	}

	return 0;
}
