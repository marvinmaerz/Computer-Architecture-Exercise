#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"

void newJacobi(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	for(int x = 1; x < X-1; x++){
		for(int y = 1; y < Y-1; y++){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
		}
	}
}

