#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
extern "C" {
#include "get_time.h"
}

	//kernel function
__global__ void triadBenchmark(float *A, float *B, float *C, uint64_t arrayLength/*, uint64_t runs*/){
	// for(uint64_t j = 0; j < runs; j++){ //without runs
		//calculation
		for(int i = blockDim.x * blockIdx.x + threadIdx.x; i < arrayLength; i++){
			A[i] = B[i] * 3 + C[i];
		}
	//}	
}

int main(int argc, char *argv[]) {	
	//printf("Angang main");
	//uint64_t minimal_runtime = 1000;
	uint64_t actual_runtime  = 0u;
	//uint64_t runs            = 0u;
	uint64_t start           = 0u;
	uint64_t stop            = 0u;
	uint64_t startCopy		 = 0u;
	uint64_t stopCopy		 = 0u;
	uint64_t copyTime		 = 0u;
	uint64_t arrayLength	 = 268435456;   // = 1GiB divided by 4 (sizeof(float))
	uint64_t arraySizeInBytes= 0u;	
	double result = 			0.0;
	float *vectorA		 = NULL;
	float *vectorB		 = NULL;
	float *vectorC		 = NULL;
	float *cudaVectorA	 = NULL;
	float *cudaVectorB	 = NULL;
	float *cudaVectorC	 = NULL;
	
	arraySizeInBytes = arrayLength * sizeof(float);		// = 1GiB
	//allocate memory and initialize it (allocating eg. 10KiB) + error handling
	vectorA = (float*) malloc(arraySizeInBytes);			
	if(vectorA == NULL){
		printf("Memory for vectorA could not be allocated\n");
		exit(EXIT_FAILURE);
	}
	
	vectorB = (float*) malloc(arraySizeInBytes);
	if(vectorB == NULL){
		printf("Memory for vectorB could not be allocated\n");
		exit(EXIT_FAILURE);
	}

	vectorC = (float*) malloc(arraySizeInBytes);
	if(vectorA == NULL){
		printf("Memory for vecotrC could not be allocated\n");
		exit(EXIT_FAILURE);
	}

	//allocate cuda memory
	cudaMalloc(&cudaVectorA, arraySizeInBytes);
	cudaMalloc(&cudaVectorB, arraySizeInBytes);
	cudaMalloc(&cudaVectorC, arraySizeInBytes);
	
	//printf("cudaMalloc ausgeführt");

	//vector init	
	for(uint64_t i = 0; i < arrayLength; i++){
		vectorA[i] = 0.0;
		vectorB[i] = (float) (rand()%100 + 1);
		vectorC[i] = (float) (rand()%100 + 1);
	}

	startCopy = get_time_us();
	//copy local mem do cuda mem
	//cudaMemcpy(cudaVectorA, vectorA, arraySizeInBytes, cudaMemcpyHostToDevice); // A muss nicht kopiert werden
	cudaMemcpy(cudaVectorB, vectorB, arraySizeInBytes, cudaMemcpyHostToDevice);
	cudaMemcpy(cudaVectorC, vectorC, arraySizeInBytes, cudaMemcpyHostToDevice);

	//printf("cudaMemcpy ausgeführt");
	
	//measurement with a runtime of at least 1 ms
	//minimal_runtime *= 1000; 												//ms -> us (multiply by 1000)
	uint64_t elementsProcessed = 0;
	float MegaUpdatesPerSecond = 0.0f;
	
	//Measurement
	//for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
	start = get_time_us();
	
	//printf("Before kernel call");
	
	//call function on kernel
				   //blocks, threadsPerBlock
	triadBenchmark<<<262144, 1024>>>(cudaVectorA, cudaVectorB, cudaVectorC, arrayLength/*, runs*/);
	cudaDeviceSynchronize();		
	
	stop  = get_time_us();
	actual_runtime = (stop - start);	
	if(actual_runtime == 0) actual_runtime = 1; 						//prevent division by zero at low size

	//printf("Actual runtime: %lu",(uint64_t) actual_runtime);
	//}
	
	cudaMemcpy(vectorA, cudaVectorA, arraySizeInBytes, cudaMemcpyDeviceToHost);
	stopCopy = get_time_us();

	copyTime = stopCopy - startCopy;
	if(copyTime == 0) copyTime = 1;		
	
	//calculate MUp/s
	elementsProcessed = /*(runs >> 1) * */ arrayLength * 3;					
	MegaUpdatesPerSecond = (elementsProcessed / actual_runtime);

	//converting variables back
	//arraySizeInBytes >>= 10;														// B -> KiB (division by 1024)
	//actual_runtime /= 1000;													// us -> ms (division by 1000)
	//minimal_runtime /= 1000;												// us -> ms (division by 1000)
	copyTime = copyTime - actual_runtime;
	result = (double) (arraySizeInBytes / (actual_runtime / 1000000.0));

	//"ArraySize[KiB],MegaUpdatesPerSecond,ActualRuntime[ms],MinimalRuntime[ms],EdgeSize"
	printf("%lf,%.1lf,%lu,%lu,%lu\n", result, MegaUpdatesPerSecond, (uint64_t) actual_runtime, copyTime/*, (uint64_t) minimal_runtime*/, arrayLength);

	cudaFree(cudaVectorA);
	cudaFree(cudaVectorB);
	cudaFree(cudaVectorC);
	free(vectorA);
	free(vectorB);
	free(vectorC);
	return 0;
}

