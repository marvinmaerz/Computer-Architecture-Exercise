#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "mkl.h"


uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

int main(){
    uint64_t calculation_start          = 0u;
    uint64_t calculation_stop           = 0u;
    uint64_t total_start        = 0u;
    uint64_t total_stop         = 0u;
    uint64_t calculation_runtime     = 0u;
    uint64_t total_runtime      = 0u;
    double FLOPsTotalRun                = 0.0f;
    double FLOPsCalculationRun  = 0.0f;
    int size = 44870;


    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    total_start = get_time_us();


    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");

    m = size, k = size, n = size;
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    alpha = 1.0; beta = 0.0;

    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl_free(A);
      mkl_free(B);
      mkl_free(C);
      return 1;
    }

    printf (" Intializing matrix data \n\n");
    for (i = 0; i < (m*k); i++) {
        A[i] = (double)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    calculation_start = get_time_us();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    calculation_stop = get_time_us();
    printf ("\n Computations completed.\n\n");

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }



    total_stop = get_time_us();
    calculation_runtime = calculation_stop - calculation_start;
    total_runtime = total_stop - total_start;
    if(total_runtime == 0) total_runtime = 1u;
    if(calculation_runtime == 0) calculation_runtime = 1u;

    calculation_runtime /= 1000000;             //us -> s
    total_runtime /= 1000000;                   //us -> s

    FLOPsTotalRun = 2 * pow(size, 3) / total_runtime;
    FLOPsCalculationRun = 2 * pow(size, 3) / calculation_runtime;


    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
    
    printf("Total Runtime: %lu\n FLOP/s(total time): %lf\n FLOP/s(calculation time): %lf\n", total_runtime, FLOPsTotalRun, FLOPsCalculationRun);
    return 0;
}
