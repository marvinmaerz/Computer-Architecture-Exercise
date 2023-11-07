#define _POSIX_C_SOURCE 199309L
#include "draw.h"

// If you want you can use the provided struct and static function

typedef struct COLOR_s {
    char r; // red channel
    char g; // green channel
    char b; // blue channel
} COLOR;

static COLOR color_converter(double value) {
    COLOR c;
    c.r = 0;
    c.g = 0;
    c.b = 0;

    // TODO color mapping

    return c;
}



void draw_grid(double* grid, uint32_t x, uint32_t y, const char* filepath) {
    // TODO mandatory
    // Open or create file
    // Write header with meta information
    // Write RGB data
    // Close file
}
