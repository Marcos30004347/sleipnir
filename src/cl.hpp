
#include "OpenCL/cl.h"
#include "OpenCL/opencl.h"

#include <stdio.h>
#include <fstream>
#include <string>
#include <time.h>
#include "types.hpp"


void mul_matrix(
	f32* A,
	f32* B,
	f32* C,
	i32 block_width,
	i32 block_heigth,
	i32 lines,
	i32 columns,
	i32 A_columns,
	i32 B_lines
) {
	std::ifstream ifs("src/kernels/matrix_mul.cl");
  	std::string content((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()    ) );

	const char *KernelSource =content.c_str();
	cl_int err;
	// printf("A\n");
	cl_context context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_ALL, NULL, NULL, &err);
	
	// printf("err=%i\n", err);
	cl_uint num = 1;
	cl_device_id devices;

	// printf("B\n");

	clGetDeviceIDs(0, CL_DEVICE_TYPE_GPU, 0, NULL, &num);
	// printf("num=%i\n", num);

	clGetDeviceIDs(0, CL_DEVICE_TYPE_GPU, num, &devices, &num);
	cl_command_queue queue = clCreateCommandQueue(context, devices, 0, &err);
	// printf("err=%i\n", err);

	// printf("C\n");

	// printf("D\n");

	cl_mem memobjs[] = {
		clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * lines*A_columns, NULL, NULL),
		clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * B_lines*columns, NULL, NULL),
		clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * lines*columns, NULL, NULL),
	};

	// printf("E\n");

	clEnqueueWriteBuffer(queue, memobjs[0], CL_TRUE, 0, sizeof(cl_float)*lines*A_columns, A, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, memobjs[1], CL_TRUE, 0, sizeof(cl_float)*B_lines*columns, B, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, memobjs[2], CL_TRUE, 0, sizeof(cl_float)*lines*columns, C, 0, NULL, NULL);
	
	// printf("E\n");

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)& KernelSource, NULL, &err);
	// printf("err=%i\n", err);

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	// printf("err=%i\n", err);

	// printf("F\n");

	cl_kernel kernel = clCreateKernel(program, "matrix_mul", NULL);
	// printf("G\n");


	clSetKernelArg(kernel, 0, sizeof(cl_int), (void*)&block_heigth);
	clSetKernelArg(kernel, 1, sizeof(cl_int), (void*)&block_width);
	clSetKernelArg(kernel, 2, sizeof(cl_int), (void*)&lines);
	clSetKernelArg(kernel, 3, sizeof(cl_int), (void*)&columns);
	clSetKernelArg(kernel, 4, sizeof(cl_int), (void*)&A_columns);
	clSetKernelArg(kernel, 5, sizeof(cl_int), (void*)&B_lines);
	clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&memobjs[0]);
	clSetKernelArg(kernel, 7, sizeof(cl_mem), (void *)&memobjs[1]);
	clSetKernelArg(kernel, 8, sizeof(cl_mem), (void *)&memobjs[2]);

	size_t global_work_size[2];
	global_work_size[0] = (size_t)(lines/block_heigth);
	global_work_size[1] = (size_t)(columns/block_width);

	size_t local_work_size[2];
	local_work_size[0] = (size_t)(block_heigth);
	local_work_size[1] = (size_t)(block_width);

	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	// printf("err = %i\n", err);
	
	err = clEnqueueReadBuffer(queue, memobjs[2], CL_TRUE, 0, lines*columns*sizeof(cl_float), (void*)C, 0, NULL, NULL);
	// printf("err = %i\n", err);

	// printf("\n");
}