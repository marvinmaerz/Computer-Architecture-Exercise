#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
extern "C" {
#include "get_time.h"
}

__global__ void update_grid(double* grid_source, double* grid_target, uint32_t X){
	for(uint32_t y = blockDim.y * blockIdx.y + threadIdx.y; y < X-1; y++){
		if(y==0) continue;
		for(uint32_t x = blockDim.x * blockIdx.x + threadIdx.x; x < X-1; x++){
			if(x==0) continue;
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
		}
	}
}

/*void jacobi(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	for(int y = 1; y < Y-1; y++){
		for(int x = 1; x < X-1; x++){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;

		}
	}
}*/

int main(int argc, char *argv[]) {	
	const uint64_t ONE_GIBIBYTE 		= 1 << 30;
	const uint32_t THREADS_PER_BLOCK 	= 1024;
	uint64_t minimal_runtime = 0u;
	uint64_t actual_runtime  = 0u;
	uint64_t runs            = 0u;
	uint64_t start           = 0u;
	uint64_t stop            = 0u;
	uint64_t startCopy		 = 0u;
	uint64_t stopCopy		 = 0u;
	uint64_t copyTime		 = 0u;

	uint64_t arrayLength	 	= 0u;   						
	uint64_t arraySizeInBytes	= 0u;
	uint64_t width 				= 0u;
	uint32_t blocks 			= 0u;
	uint64_t elementsProcessed 	= 0;
	float MegaUpdatesPerSecond 	= 0.0f;
	float MUPsWithCopy			= 0.0f;

	double *grid_source 	= NULL;
	double *grid_target		= NULL;
	double *cuda_source		= NULL;
	double *cuda_target 	= NULL;


	// init variables
	arraySizeInBytes 		= 3 * ONE_GIBIBYTE;						// 3GiB = (3221225472 B)
	arrayLength				= arraySizeInBytes / sizeof(double);	// = 402653184 elements (3GiB / 8B)
	width					= (uint64_t) sqrt(arrayLength);			// = ca. 200066
	blocks					= arrayLength / THREADS_PER_BLOCK;		// = 393216


	// get minimal runtime from command line input (argv)
	if(argc != 2 || argv == NULL){
		printf("run the program as follows: ./jacobiCUDA minimal_runtime\n");
		exit(EXIT_FAILURE);
	}
	char *input = argv[1];
	if(input[0] == '-'){
		printf("Please enter a positive minimal runtime\n");
		exit(EXIT_FAILURE);
	}
	minimal_runtime = strtoull(input, NULL, 10);
	if(minimal_runtime < 100){
		printf("The minimal runtime must be at least 1000ms\n");
		exit(EXIT_FAILURE);
	}
	minimal_runtime *= 1000; 		// ms -> us (*1000)


	// alloce host grids	
	grid_source = (double*) malloc(arraySizeInBytes);
	grid_target = (double*) malloc(arraySizeInBytes);
	if(grid_source == NULL || grid_target == NULL){
		printf("Memory for grid_source or grid_target could not be allocated\n");
		exit(EXIT_FAILURE);
	}
	

	// allocate cuda/device grids
	cudaMalloc(&cuda_source, arraySizeInBytes);
	cudaMalloc(&cuda_target, arraySizeInBytes);
	

	// grids init
	for(uint64_t i = 0; i < arrayLength; i++){
		if(i < width || i % width == 0){
			grid_source[i] = 1.0;
			grid_target[i] = 1.0;		
		} else {
			grid_source[i] = 0.0;
			grid_target[i] = 0.0;				
		}
	}


	//copy local mem to cuda mem -> read/write measurement
	startCopy = get_time_us();
	cudaMemcpy(cuda_source, grid_source, arraySizeInBytes, cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_target, grid_target, arraySizeInBytes, cudaMemcpyHostToDevice);	
	

	// runtime measurement
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		
		for(uint64_t i = 0u; i < runs; i++) {
			// kernel call:
						//131072, 1024
			update_grid<<<blocks, THREADS_PER_BLOCK>>>(cuda_source, cuda_target, width);
			cudaDeviceSynchronize();	
		}	
	
		stop  = get_time_us();
		actual_runtime = (stop - start);	
		if(actual_runtime == 0) actual_runtime = 1; 						//prevent division by zero at low size
	}
	

	cudaMemcpy(grid_target, cuda_target, arraySizeInBytes, cudaMemcpyDeviceToHost);
	stopCopy = get_time_us();
	copyTime = stopCopy - startCopy;
	if(copyTime == 0) copyTime = 1;										// prevent division by zero
	

	//calculate MUp/s
	elementsProcessed = (runs >> 1) * arrayLength;					// *2 because of source and target array
	MegaUpdatesPerSecond = (elementsProcessed / actual_runtime);		// runtime in us, but cancels out with Mega (*10^6)
	MUPsWithCopy = (elementsProcessed / copyTime);
	copyTime = copyTime - actual_runtime;
	// result = (double) (arraySizeInBytes / (actual_runtime / 1000000.0));		// bandwidth
	// convert times back from us -> ms (divide by 1000)
	minimal_runtime /= 1000;												
	actual_runtime /= 1000;
	copyTime /= 1000;


	//printf("MinimalRuntime[ms],ArraySize[GiB],MUP/s[kernel runtime], MUP/s[including CopyTime],ActualRuntime[ms],CopyTime[ms]\n");
	printf("%lu,%lu,%.1lf,%.1lf,%lu,%lu\n", minimal_runtime, arraySizeInBytes >> 30, MegaUpdatesPerSecond, MUPsWithCopy, actual_runtime, copyTime);

	cudaFree(cuda_source);
	cudaFree(cuda_target);
	free(grid_source);
	free(grid_target);
	return 0;
}

