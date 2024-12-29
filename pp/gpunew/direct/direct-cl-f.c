// nthreads = number of GPU threads
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
// #pragma CL_USE_DEPRECATED_OPENCL_1_0_APIS 
__kernel void matrix_multiply(
     __global double *matrix_a, 
     __global double *matrix_b,
     __global double *matrix_c,
     int matsize) // Add the matrix size as an argument
{
    // obtaining the index of a thread
    int me = get_global_id(0);
    int row = me / matsize; // Calculate row index
    int col = me % matsize; // Calculate column index
    
    // Perform matrix multiplication
    double sum = 0.0;
    for (int i = 0; i < matsize; ++i) {
        sum += matrix_a[row * matsize + i] * matrix_b[i * matsize + col];
    }
    
    matrix_c[me] = sum; // Store the result in the output matrix
}
