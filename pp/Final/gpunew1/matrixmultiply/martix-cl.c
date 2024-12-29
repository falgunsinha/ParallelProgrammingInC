#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void energy(
        uint nthreads, 
        uint matsize,
        __global double *matrix_a, 
        __global double *matrix_b,
        __global double *matrix_c)  
{
    uint me = get_global_id(0);
    uint row = me / matsize;
    uint col = me - row * matsize;
    
    double sum = 0;
    for (uint k = 0; k < matsize; k++) {
        sum += matrix_a[row*matsize + k] * matrix_b[k*matsize + col];
    }
    matrix_c[me] = sum; 
}