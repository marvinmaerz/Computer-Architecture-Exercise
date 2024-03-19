#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <likwid-marker.h>
#include "jacobi.h"
#include "draw.h"
#include "get_time.h"
#include <likwid-marker.h>

static void usage_msg(void) {
	fprintf(stderr, "Use the right format: ./jacobi <array size in kiB> <minimal runtime in milliseconds (eg. 1000)>\n");
	return;
}

int main(int argc, char *argv[]) {
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
<<<<<<< HEAD:Exercise 4/src/mainJacobi.c

	LIKWID_MARKER_INIT;
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		LIKWID_MARKER_RESET("Compute");
		LIKWID_MARKER_START("Compute");
=======
	
	LIKWID_MARKER_INIT;
	for(runs = 1u; actual_runtime < minimal_runtime; runs = runs << 1u) {
		start = get_time_us();
		LIKWID_MARKER_RESET("MARKER_NAME");
		LIKWID_MARKER_START("MARKER_NAME");
>>>>>>> main:Exercise 4/src/main.c
		for(uint64_t i = 0u; i < runs; i++) {

			jacobi(grid_source, grid_target, width, width);

			//switching the pointers for the next iteration
			double *temp = grid_source;
			grid_source = grid_target;
			grid_target = temp;
		}
<<<<<<< HEAD:Exercise 4/src/mainJacobi.c
		LIKWID_MARKER_STOP("Compute");
=======
		LIKWID_MARKER_STOP("MARKER_NAME");
>>>>>>> main:Exercise 4/src/main.c
		stop  = get_time_us();
		actual_runtime = (stop - start);	
		if(actual_runtime == 0) actual_runtime = 1; 						//prevent division by zero at low size
	}
	LIKWID_MARKER_CLOSE;

	//calculate MUp/s
	elementsProcessed = (runs >> 1) * pow(width-2, 2);					
	MegaUpdatesPerSecond = (elementsProcessed / actual_runtime);

	//converting variables back
	sizeInBytes >>= 10;														// B -> KiB (division by 1024)
	actual_runtime /= 1000;													// us -> ms (division by 1000)
	minimal_runtime /= 1000;												// us -> ms (division by 1000)

	//"ArraySize[KiB],MegaUpdatesPerSecond,ActualRuntime[ms],MinimalRuntime[ms],EdgeSize"
	printf("%lu,%.1lf,%lu,%lu,%i\n", (uint64_t) sizeInBytes, MegaUpdatesPerSecond, (uint64_t) actual_runtime, (uint64_t) minimal_runtime, width);
	
	//calls draw
	
  	char filepath[100];
	snprintf(filepath, sizeof(filepath), "../drawings/%iUnroll/drawJacobi%lu.ppm", NUM,  sizeInBytes);
	draw_grid(grid_source, width, width, filepath);
	

	_mm_free(grid_source);
	_mm_free(grid_target);
	return 0;
}

