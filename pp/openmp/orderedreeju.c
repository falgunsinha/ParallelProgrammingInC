#include <omp.h>
#include <stdio.h>

int main()
{
	int n;
	int counter = 0;
	printf("Input number of parallel threads \n");
	scanf("%i", &n);

	FILE *file;

#pragma omp parallel num_threads(n)
	{
		int me = omp_get_thread_num();
		char mode = (me == 0) ? 'w' : 'a';

#pragma omp barrier // Ensure all threads have reached this point before proceeding

		FILE *localFile;

#pragma omp critical
		{
			localFile = fopen("output.txt", &mode);
			if (localFile == NULL)
			{
				fprintf(stderr, "Error opening file\n");
				exit(1);
			}
		}

#pragma omp for ordered nowait
		for (int i = 1; i <= omp_get_num_threads(); i++)
		{
#pragma omp ordered
			{
				counter += 1;
				fprintf(localFile, "Line %d by thread %i, mode %c\n", counter, me, mode);
			}
		}

#pragma omp critical
		{
			fclose(localFile);
		}
	}

	return 0;
}
