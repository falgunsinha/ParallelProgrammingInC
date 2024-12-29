//calculation of energy of water box

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//  include library OpenCL CL/cl.h
#include "CL/cl.h"
//define number of threads
//#define nthreads 10000
const int LENGTH = 80;
int main ()
{
	cl_ulong time_start, time_end;
	long total_time;

	// reading of source code of kernel to string variable =======
	FILE *fp;
	char *source_code;
	long file_length;
	long read_length;

	fp = fopen("martix-cl.c","r");
	fseek(fp,0L, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);

	source_code = malloc(sizeof(char)*(file_length+1)); //extra character for final EOF (end of file)
	read_length = fread(source_code,1,file_length,fp);
	if(read_length!= file_length)
	{
		printf("Can't read file\n");
		exit(1);
	}
	source_code[file_length+1]='\0';
	fclose(fp);

	//================ initiation of matrix =====================
	int matsize,i,j,print_items;
	printf("Input matrix size: ");  scanf("%i",&matsize);

	cl_uint cl_nthreads = matsize*matsize; //number of threads set to number of cells
	printf("Number of GPU threads set to: %i\n",cl_nthreads);
	if(matsize*matsize<20)
		print_items=matsize*matsize;
	else
		print_items=20; 

	cl_double *matrix_a, *matrix_b, *matrix_c, *matrix_d;
	matrix_a = malloc(sizeof(cl_double)*matsize*matsize);
	matrix_b = malloc(sizeof(cl_double)*matsize*matsize);
	matrix_c = malloc(sizeof(cl_double)*matsize*matsize);
	matrix_d = malloc(sizeof(cl_double)*matsize*matsize);

	for (i=0;i<matsize;i++){
		for(j=0;j<matsize;j++){
			matrix_a[i*matsize+j]=i+j;
			matrix_b[i*matsize+j]=j*1e-6;
		}
	}
	printf("FIRST %i  elements of matrix A   B \n",print_items);
	for (i=0;i<print_items;i++)
		printf("%lf %lf \n ",matrix_a[i],matrix_b[i]);

	//opencl variables needed to communicate with GPU
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;   
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_event prof_event;

	//====== variables with auxiliary information ========
	char dname[500];
	size_t paramsi, p_size;
	cl_ulong long_entries;
	cl_uint entries;
	int bd=0;
	size_t locala;
	//=====================
	// query available platforms
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// query list of available devices on platform
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_devices);

	/* obtain information about platform */
	clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,500,dname,NULL);
	printf("CL_PLATFORM_NAME = %s\n", dname);
	clGetPlatformInfo(platform_id,CL_PLATFORM_VERSION,500,dname,NULL);
	printf("CL_PLATFORM_VERSION = %s\n", dname);

	/* ====== query devices for information ======= */
	clGetDeviceInfo(device_id, CL_DEVICE_NAME, 500, dname,NULL);
	printf("Device #%d name = %s\n", bd, dname);
	clGetDeviceInfo(device_id,CL_DRIVER_VERSION, 500, dname,NULL);
	printf("\tDriver version = %s\n", dname);
	clGetDeviceInfo(device_id,CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
	printf("\tGlobal Memory (MB):\t%lu\n",long_entries/1024/1024);
	clGetDeviceInfo(device_id,CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,sizeof(cl_ulong),&long_entries,NULL);
	printf("\tGlobal Memory Cache (MB):\t%lu\n",long_entries/1024/1024);
	clGetDeviceInfo(device_id,CL_DEVICE_LOCAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
	printf("\tLocal Memory (KB):\t%lu\n",long_entries/1024);
	clGetDeviceInfo(device_id,CL_DEVICE_MAX_CLOCK_FREQUENCY,sizeof(cl_ulong),&long_entries,NULL);
	printf("\tMax clock (MHz) :\t%lu\n",long_entries);
	clGetDeviceInfo(device_id,CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t),&p_size,NULL);
	printf("\tMax Work Group Size- # work_items in work_group (max threads in compute unit):\t%lu\n",p_size);
	clGetDeviceInfo(device_id,CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint),&entries,NULL);
	printf("\tNumber of parallel compute cores:\t%d\n",entries);
	//       CL_DEVICE_MAX_PARAMETER_SIZE
	clGetDeviceInfo(device_id,CL_DEVICE_MAX_PARAMETER_SIZE,sizeof(size_t),&paramsi,NULL);
	printf("\tMax size of argument which can be passed to kernel(in bytes) :\t%lu\n",paramsi);
	//       CL_DEVICE_MAX_WORK_ITEM_SIZES
	size_t workitem_size[3];
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL);
	printf("\tCL_DEVICE_MAX_WORK_ITEM_SIZES(max # threads in each dimension of work_group):\t%lu / %lu / %lu \n",
			workitem_size[0], workitem_size[1], workitem_size[2]);
	/* ====== query devices for information ========== */

	// creating OpenCL context on one or more devices. Context controls execution of OpenCL objects such as
	// command_queue, memory, program, and kernel.
	cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret); // 1 is number of devices; &device_id pointer to list of devices

	// creating command queue on specified device
	// operations acting on OpenCL objects are executed usign this command queue
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

	// loading opencl code for kernel to context
	cl_program program = clCreateProgramWithSource(context, 1,(const char **)&source_code,NULL, &ret);

	// compiles (and links) kernel program AT RUNTIME!
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	cl_kernel kernel; // OpenCL variable
	// creating kernel object from compiled program
	kernel = clCreateKernel(program, "energy", &ret);

	/* returns information about kernel object ======== */
	clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, 500, dname, NULL);
	clGetKernelInfo(kernel, CL_KERNEL_NUM_ARGS, sizeof(cl_uint), &entries, NULL);
	printf("Kernel information: FUNCTION_NAME= %s,# of arguments= %d args\n\n", dname, entries);

	//query maximum number of threads in one work-group
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(locala), &locala, NULL);
	printf("\tNumber of threads in one work-group:\t%lu\n",locala);

	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(long_entries), &long_entries, NULL);
	printf("\tSize of private memory:\t%lu\n",long_entries);

	// (in bytes): includes local memory needed for execution of kernel, variables declared within kernel,
	// local memory assigned for __local arguments
	clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_LOCAL_MEM_SIZE, sizeof(long_entries), &long_entries, NULL);
	printf("\tAmount of used local memory:\t%lu\n",long_entries);

	// Creating I/O buffer objects for transfering values of host variables to memory objects of kernel and vice versa
	cl_mem buffer_a = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
			sizeof(cl_double)*matsize*matsize, matrix_a, NULL);
	cl_mem buffer_b = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
			sizeof(cl_double)*matsize*matsize, matrix_b, NULL);
//	  cl_mem buffer_c = clCreateBuffer(context,CL_MEM_READ_ONLY,
	  cl_mem buffer_c = clCreateBuffer(context,CL_MEM_WRITE_ONLY,
//	cl_mem buffer_c = clCreateBuffer(context,CL_MEM_READ_WRITE,
			sizeof(cl_double)*matsize*matsize, NULL, NULL);


	// setting correspondence of buffers to variables transfering values of host variables to memory objects of kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_uint),&cl_nthreads);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_uint),&matsize);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem),&buffer_a);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem),&buffer_b);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem),&buffer_c);

	// setting global work size - # of cl_nthreads and dimension
	size_t global[1]; // setting the dimension
	global[0]= cl_nthreads; // setting cl_nthreads

	// command to execute command queue on kernel of device
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global, NULL, 0, NULL, &prof_event);

	// synchronisation point {barrier}, waits for completion of list of events given by 2nd argument
	ret = clWaitForEvents(1, &prof_event);
	clFinish(command_queue);

	// profiling - gathering information about execution of kernel
	// query time when program was initialised by kernel
	clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	// query time when program was finished by kernel
	clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	// calculation of execuion time by kernel
	total_time = (time_end - time_start);
	// return output array from kernel to RAM
	ret = clEnqueueReadBuffer(command_queue, buffer_c, CL_TRUE, 0,
			sizeof(cl_double)*matsize*matsize, matrix_c, 0, NULL, NULL);

	double timeTaken=(double)total_time*1e-9;
	printf("Time taken = %.3lf seconds\n", timeTaken);
	// printing out output of calculation
	printf("FIRST %i elements of matrix C GPU   CPU\n",print_items);
	for (i=0;i<matsize*matsize;i++) {
		matrix_d[i] = matrix_a[i]+ matrix_b[i];
	}
	for (i=0;i<print_items;i++)
		printf("%lf %lf \n",matrix_c[i],matrix_d[i]);

	// releasing objects and memory
	// releasing objects and memory
	free(matrix_a);free(matrix_b);free(matrix_c);free(matrix_d);
	clReleaseMemObject(buffer_a); clReleaseMemObject(buffer_b); clReleaseMemObject(buffer_c);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
}
