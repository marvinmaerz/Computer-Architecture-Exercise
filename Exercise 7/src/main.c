#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include "jacobi.h"
#include "draw.h"
#include "get_time.h"
#include "barrier.h"

#ifndef bx
	#define bx 1
#endif

#ifndef by
	#define by 1
#endif

#ifndef THREADS
	#define THREADS 1
#endif

static void usage_msg(void) {
	fprintf(stderr, "Use the right format: ./jacobi <array size in kiB> <minimal runtime in milliseconds (eg. 1000)>\n");
	return;
}

struct work_package_s {
	double *grid_source;
        double *grid_target;
        uint32_t size_x;
        uint32_t yStart;
        uint32_t yStop;
        uint32_t runs;
	bool main; 
}pkgs[THREADS];

int main(int argc, char *argv[]) {
	void * worker_thread(void *void_args);
	uint64_t minimal_runtime = 0u;
	uint64_t actual_runtime  = 0u;
	uint64_t runs            = 0u;
	uint64_t start           = 0u;
	uint64_t stop            = 0u;
	uint64_t sizeInBytes	 = 0u;
	uint64_t arrayLength	 = 0u;
	uint32_t width		 = 0u;
	double *grid_source	 = NULL;
	double *grid_target	 = NULL;

	
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
	if(sizeInBytes <= 0){
		printf("Please enter a valid array size > 0KiB\n");
		exit(EXIT_FAILURE);
	}
	
	width = (uint32_t) sqrt(sizeInBytes / (2 * sizeof(double)));
	arrayLength = pow(width, 2); 
	
	//allocate memory and initialize it (allocating eg. 10KiB) + error handling
	grid_source = (double*) _mm_malloc(arrayLength * sizeof(double), 64);			
	if(grid_source == NULL){
		printf("Memory for grid_source could not be allocated\n");
		exit(EXIT_FAILURE);
	}

	grid_target = (double*) _mm_malloc(arrayLength * sizeof(double), 64);
	if(grid_target == NULL){
		printf("Memory for grid_target could not be allocated\n");
		exit(EXIT_FAILURE);
	}
	

	//initialise grid - left and top margins set to 1.0, remaining cells set to 0.0
	for(uint64_t i = 0; i < arrayLength; i++){
		if(i < width || i % width == 0){
			grid_source[i] = 1.0;
			grid_target[i] = 1.0;		
		} else {
			grid_source[i] = 0.0;
			grid_target[i] = 0.0;				
		}
	}

	//measurement with a runtime of at least 1 ms
	minimal_runtime = strtoull(argument2, NULL, 10);
	if(minimal_runtime < 1){
		printf("The minimal runtime must be at least 1ms\n");
		exit(EXIT_FAILURE);
	}

	minimal_runtime *= 1000; 												//ms -> us (multiply by 1000)
	uint64_t elementsProcessed = 0;
	float MegaUpdatesPerSecond = 0.0f;
	
	//pkgs init	
	uint32_t tSize = (width-2)/THREADS;
	uint32_t remaining = (width-2) - tSize*THREADS;		
	for (int i = 0; i < THREADS; i++){
		pkgs[i].grid_source = grid_source;
		pkgs[i].grid_target = grid_target;
		pkgs[i].size_x = width;
		pkgs[i].yStart = i * tSize + 1;
		if(i == THREADS-1){
			pkgs[i].yStop = (i+1) * tSize + remaining;
			pkgs[i].main = true;
		}else{
			pkgs[i].yStop = (i+1) * tSize;
			pkgs[i].main = false;
		}
	}
	
	pthread_t thread[THREADS-1];
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		for(int i = 0; i < THREADS-1; i++){
			pkgs[i].runs = runs;
			pthread_create(&thread[i], NULL, worker_thread, &pkgs[i]);
		}
		pkgs[THREADS-1].runs = runs;
		worker_thread(&pkgs[THREADS-1]);
		for(int i = 0; i < THREADS-1; i++){
			pthread_join(thread[i], NULL);
		}
		stop  = get_time_us();
		actual_runtime = (stop - start);	
		if(actual_runtime == 0) actual_runtime = 1; 						//prevent division by zero at low size
	}

	//calculate MUp/s
	elementsProcessed = (runs >> 1) * pow(width-2, 2);					
	MegaUpdatesPerSecond = (elementsProcessed / actual_runtime);

	//converting variables back
	sizeInBytes >>= 10;														// B -> KiB (division by 1024)
	actual_runtime /= 1000;													// us -> ms (division by 1000)
	minimal_runtime /= 1000;												// us -> ms (division by 1000)

	//"ArraySize[KiB],MegaUpdatesPerSecond,ActualRuntime[ms],MinimalRuntime[ms],EdgeSize"
	printf("%lu,%.1lf,%lu,%lu,%i,%i\n", (uint64_t) sizeInBytes, MegaUpdatesPerSecond, (uint64_t) actual_runtime, (uint64_t) minimal_runtime, width, THREADS);
	
	//calls draw
	/*	
  	char filepath[100];
	snprintf(filepath, sizeof(filepath), "../drawings/drawJacobi%lu.ppm", sizeInBytes);
	draw_grid(grid_source, width, width, filepath);
	*/

	_mm_free(grid_source);
	_mm_free(grid_target);
	return 0;
}

void * worker_thread(void *void_args) {
	struct work_package_s *args = (struct work_package_s*) void_args;
	for(uint32_t i = 0; i < args->runs; i++){

		jacobi_subgrid(args->grid_source, args->grid_target, args->size_x, args->yStart, args->yStop);

		sync_barrier(THREADS);
		
		//switching the pointers for the next iteration
                double *temp = args->grid_source;
                args->grid_source = args->grid_target;
                args->grid_target = temp;
	}
	if(args->main == false){
		pthread_exit(NULL);
	}else{
		return NULL;
	}
}

