// nthreads = number of GPU threads
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
// #pragma CL_USE_DEPRECATED_OPENCL_1_0_APIS 
__kernel void energy(
     __global double *matrix_a, 
	   __global double *matrix_b,
	   __global double *matrix_c )
{
	// obtaining the index of a thread - analogue to  MPI_Comm_rank
	uint me = get_global_id(0);
	matrix_c[me] = matrix_a[me] + matrix_b[me];
}
