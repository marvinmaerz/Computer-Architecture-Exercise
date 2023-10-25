#include <stdio.h>
#include <stdlib.h>
#include "vec_sum.h"
#include "get_time.h"

static void usage_msg(void) {
	fprintf(stderr, "Use the right format: ./vecSum <array size in kiB> <minimal runtime in milliseconds (eg. 1000)>\n");
	return;
}

int main(int argc, char *argv[]) {
	uint64_t minimal_runtime = 0u;
	uint64_t actual_runtime  = 0u;
	uint64_t runs            = 0u;
	uint64_t start           = 0u;
	uint64_t stop            = 0u;
	uint64_t sizeInBytes	 = 0u;
	uint32_t arrayLength	 = 0u;
	float *vectorArray		 = NULL;

	if(argc != 3 || argv == NULL) {
		usage_msg();
		return -1;
	}

	//parse parameter: size of the vector in KiB (1KiB = 1024B)
	sizeInBytes = strtoull(argv[1], NULL, 10) * 1024;	
	arrayLength = sizeInBytes / sizeof(float);

	//allocate memory and initialize it (allocating eg. 10KiB) + error handling
	vectorArray = malloc(sizeInBytes);			
	if(vectorArray == NULL){
		printf("Memory for vectorArray could not be allocated\n");
		exit(EXIT_FAILURE);
	}
	vectorArray[0] = 0.5;
	for(int i = 1; i < arrayLength; i++){
		vectorArray[i] = (float) i;
		//printf("vector array[%i]: %lf\n", i, vectorArray[i]);
	}

	//measurement with a runtime of at least 1 s
	minimal_runtime = strtoull(argv[2], NULL, 10);
	if(minimal_runtime < 1000){
		printf("The minimal runtime must be at least 1s (1000ms)\n");
		exit(EXIT_FAILURE);
	}

	//printf("array size: %luB, minimal runtime: %lums, length of array: %u \n", sizeInBytes, minimal_runtime, arrayLength);


	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		for(uint64_t i = 0u; i < runs; i++) {
			// TODO
			vec_sum(vectorArray, arrayLength);
		}
		stop  = get_time_us();
		actual_runtime = stop - start;
		//printf("actual runtime for %lu runs: %lums\n", runs, actual_runtime);
	}
 
	//calculate and print updates per second
	runs >>= 1u;
	uint64_t elementsProcessed = runs * arrayLength;
	float elementsPerSecond = elementsProcessed / actual_runtime * 1000;

	fprintf(stdout, "actual runtime: %lums, elements per second: %.2lf, array length: %u, runs: %lu\n", actual_runtime, elementsPerSecond, arrayLength, runs);
	//fprintf(stdout, "%lu,%lf,%lu,%lu\n", (uint64_t) 0, 0.0, (uint64_t) 0, (uint64_t) 0);

	free(vectorArray);
	return 0;
}
