#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"
#include "draw.h"

int main(){
    //test 5x5 Grid -> 25 Entries, width = 5
    //uint64_t sizeB = 1024;
    const uint32_t width = 100;
    const uint32_t length = width * width;
    double *grid = (double*) _mm_malloc(length * sizeof(double), 64);
    double *grid_target = (double*) _mm_malloc(length *sizeof(double), 64);
    if(grid == NULL || grid_target == NULL){
        printf("mm-malloc failure\n");
        exit(EXIT_FAILURE);
    }    

    for(uint64_t i = 0; i < length; i++){
		if(i < width || i % width == 0){
			grid[i] = 1.0;
			grid_target[i] = 1.0;		
		} else {
			grid[i] = 0.0;
			grid_target[i] = 0.0;				
		}
    }
    
    /*for(uint32_t y = 0; y < width; y++){
        for(uint32_t x = 0; x < width; x++){
            if(x == 0 || y == 0){
                grid[width * y + x] = 1.0;
                grid_target[width * y + x] = 1.0;
                continue;
            }
            grid[width * y + x] = 0.0;
            grid_target[width * y + x] = 0.0;

        }
    }*/

    for(int i = 0; i < 10; i++){
        jacobi(grid, grid_target, width, width);
        double *temp = grid;
        grid = grid_target;
        grid_target = temp;
    }
    
    //draw_grid(grid, width, width, "DrawJacobi.ppm");

    printf("4096/1024 with bitshift: %u\n", 4096>>10);

    return -1;
}