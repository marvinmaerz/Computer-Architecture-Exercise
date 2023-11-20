#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"

#ifndef NUM
	#define NUM 1
#endif

void jacobi(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	uint32_t remainder = (X-2) % NUM;
	#pragma nounroll
	#pragma novector
	for(uint32_t y = 1; y < Y-1; y++){
		#pragma nounroll
		#pragma novector
		
		#if NUM == 1
		for(uint32_t x = 1; x < X-1; x++){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
		}
		#elif NUM == 2
		for(uint32_t x = 1; x < X-1-remainder; x+=2){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
			grid_target[y*X+(x+1)] = (grid_source[(y-1)*X+(x+1)] + grid_source[(y+1)*X+(x+1)] + grid_source[y*X+x] + grid_source[y*X+(x+2)]) * 0.25;
		}
		#elif NUM == 4
		for(uint32_t x = 1; x < X-1-remainder; x+=4){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
			grid_target[y*X+(x+1)] = (grid_source[(y-1)*X+(x+1)] + grid_source[(y+1)*X+(x+1)] + grid_source[y*X+x] + grid_source[y*X+(x+2)]) * 0.25;
			grid_target[y*X+(x+2)] = (grid_source[(y-1)*X+(x+2)] + grid_source[(y+1)*X+(x+2)] + grid_source[y*X+(x+1)] + grid_source[y*X+(x+3)]) * 0.25;
			grid_target[y*X+(x+3)] = (grid_source[(y-1)*X+(x+3)] + grid_source[(y+1)*X+(x+3)] + grid_source[y*X+(x+2)] + grid_source[y*X+(x+4)]) * 0.25;		
		}
		#endif
		
		switch(remainder){
			case 3: grid_target[y*X+(X-4)] = (grid_source[(y-1)*X+(X-4)] + grid_source[(y+1)*X+(X-4)] + grid_source[y*X+(X-5)] + grid_source[y*X+(X-3)]) * 0.25;
			case 2: grid_target[y*X+(X-3)] = (grid_source[(y-1)*X+(X-3)] + grid_source[(y+1)*X+(X-3)] + grid_source[y*X+(X-4)] + grid_source[y*X+(X-2)]) * 0.25;
			case 1: grid_target[y*X+(X-2)] = (grid_source[(y-1)*X+(X-2)] + grid_source[(y+1)*X+(X-2)] + grid_source[y*X+(X-3)] + grid_source[y*X+(X-1)]) * 0.25;
		}
	}
}

