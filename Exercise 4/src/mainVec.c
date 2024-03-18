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
	char *argument1 = argv[1];
	char *argument2 = argv[2];
	if(argument1[0] == '-'){
		printf("Please enter a valid array size > 0KiB\n");
		exit(EXIT_FAILURE);
	}
	if(argument2[0] == '-'){
		printf("Please enter a valid minimal runtime >= 1000ms\n");
		exit(EXIT_FAILURE);
	}
	sizeInBytes = strtoull(argument1, NULL, 10) * 1024;
	if(sizeInBytes == 0){
		printf("Please enter a valid array size > 0KiB\n");
		exit(EXIT_FAILURE);
	}
	
	arrayLength = sizeInBytes / sizeof(float);
	
	//allocate memory and initialize it (allocating eg. 10KiB) + error handling
	vectorArray = (float*) malloc(sizeof(float) * arrayLength);			
	if(vectorArray == NULL){
		printf("Memory for vectorArray could not be allocated\n");
		exit(EXIT_FAILURE);
	}
	//vectorArray[0] = 0.5;
	//for(int i = 1; i < arrayLength; i++){
	//	vectorArray[i] = (float) i;				
	//}
	//try out: random numbers for array entries
	for(int i = 0; i < arrayLength; i++){
		vectorArray[i] = (float) (rand()%100 + 1);
	}

	//measurement with a runtime of at least 1 s
	minimal_runtime = strtoull(argument2, NULL, 10);
	if(minimal_runtime < 1000){
		printf("The minimal runtime must be at least 1s (1000ms)\n");
		exit(EXIT_FAILURE);
	}
	
	//for debugging purposes:
	//printf("array-size: %luB, array-length: %u, min-runtime: %.1fs\n", sizeInBytes, arrayLength, (float)minimal_runtime/1000);

	//minimal_runtime(ms) converted to us:
	minimal_runtime *= 1000; 		
	uint64_t elementsProcessed = 0;
	float elementsPerSecond = 0.0f;

	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		//runs is a power of 2, vec_sum() is called incrementally 'runs' times -> execution time for inner loop increases until it reaches minimal_runtime
		for(uint64_t i = 0u; i < runs; i++) {
			vec_sum(vectorArray, arrayLength);
		}
		stop  = get_time_us();
		actual_runtime = (stop - start);	
		//prevent division by zero at low size
		if(actual_runtime == 0) actual_runtime = 1;

	}
	//calculate and print updates per second
	elementsProcessed = (runs>>1) * arrayLength;
	elementsPerSecond = (elementsProcessed / actual_runtime) * 1000000;
	
	//converting variables back
        sizeInBytes >>= 10;                     // B -> KiB (division by 1024)
        actual_runtime /= 1000;			// us -> ms (division by 1000)
        minimal_runtime /= 1000;
	
	//printing "ArraySize(converted to KiB),AdditionsPerSecond,ActualRuntime(converted to ms),MinimalRuntime(converted to ms)"
	fprintf(stdout, "%lu,%.1lf,%lu,%lu\n", (uint64_t) sizeInBytes, elementsPerSecond, (uint64_t) actual_runtime, (uint64_t) minimal_runtime);

	free(vectorArray);
	return 0;
}
