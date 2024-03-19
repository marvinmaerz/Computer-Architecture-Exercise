#ifndef JACOBI_H
#define JACOBI_H

#include <stdint.h>

/* This function iterates over a grid to perform a jacobi time step.
 */
void jacobi_subgrid(double* grid_source, double* grid_target, uint32_t X, uint32_t yStart, uint32_t yStop);

#endif
