// nthreads = number of GPU threads
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
// #pragma CL_USE_DEPRECATED_OPENCL_1_0_APIS 
__kernel void energy(
		uint nthreads, 
		uint matsize,
		__global double *matrix_a, 
		__global double *matrix_b, 
		__global double *matrix_c )
{
	// obtaining the index of a thread - analogue to  MPI_Comm_rank
	int me = get_global_id(0);
	for (int i=me; i<matsize*matsize; i+=nthreads)
		matrix_c[i] = matrix_a[i] + matrix_b[i];
}
