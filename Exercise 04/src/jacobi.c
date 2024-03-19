#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"

<<<<<<< HEAD
void jacobi(double* grid_source, double* grid_target, uint32_t x, uint32_t y) {
	uint32_t width = x;
	for(y = 1; y < width-1; y++){
		for(x = 1; x < width-1; x++){
			double bottom = grid_source[width*(y-1)+x];
			double top = grid_source[width*(y+1)+x];
			double left = grid_source[width*y+(x-1)];
			double right = grid_source[width*y+(x+1)];
			grid_target[width * y + x] = (bottom + top + left + right) / 4;
=======
void jacobi(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	for(int y = 1; y < Y-1; y++){
		for(int x = 1; x < X-1; x++){
			grid_target[y*X+x] = (grid_source[(y-1)*X+x] + grid_source[(y+1)*X+x] + grid_source[y*X+(x-1)] + grid_source[y*X+(x+1)]) * 0.25;
>>>>>>> main
		}
	}
}

