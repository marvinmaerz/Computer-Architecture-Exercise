#define _POSIX_C_SOURCE 199309L
#include "draw.h"
#include <stdio.h>
#include <stdlib.h>

// If you want you can use the provided struct and static function

typedef struct COLOR_s {
	char r; // red channel
	char g; // green channel
	char b; // blue channel
} COLOR;

static COLOR color_converter(double value) {
	if(value < 0.0 || value > 1.0){
		fprintf(stderr, "Invalid input value");
		exit(EXIT_FAILURE);
	}
	COLOR c;
	if(value <= 0.25){
		c.r = 0;
		c.g = 1020*value;
 		c.b = 255;
	}else if(value > 0.25 && value <= 0.5){
		c.r = 0;
		c.g = 255;
		c.b = -1020*value+510;
	}else if(value > 0.5 && value <= 0.75){
		c.r = 1020*value-510;
		c.g = 255;
		c.b = 0;
	}else if(value > 0.75){
		c.r = 255;
		c.g = -1020*value+1020;
		c.b = 0;
	}

	return c;
}


void draw_grid(double* grid, uint32_t x, uint32_t y, const char* filepath) {
	uint32_t width = x;
	uint64_t gridLength = x * y;
<<<<<<< HEAD

	// Open or create file
	FILE *PPM = fopen(filepath, "w");

=======
	
	// Open or create file
	FILE *PPM = fopen(filepath, "w");
	if(PPM == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
>>>>>>> main
	// Write header with meta information
	fprintf(PPM, "P6 %i %i 255\n", x, y);

	// Write RGB data
	for(uint64_t i = 0; i < gridLength; i++){
		COLOR c = color_converter(grid[i]);
		fprintf(PPM, "%c%c%c", c.r, c.g, c.b);
	}
<<<<<<< HEAD

=======
	
>>>>>>> main
	// Close file
	fclose(PPM);
}
