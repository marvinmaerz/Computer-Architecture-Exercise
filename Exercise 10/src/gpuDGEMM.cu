#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <cuda_runtime.h>
#include "cublas_v2.h"
#include <time.h>
#include <sys/time.h>

#define IDX2C(i,j,ld) (((j)*(ld))+(i))


uint64_t get_time_us(void) {
    struct timespec a;
    clock_gettime(CLOCK_MONOTONIC, &a);
    return (uint64_t) (((double) a.tv_nsec / 1000.0) + ((double) a.tv_sec * (1000.0 * 1000.0)));
}

static __inline__ void modify (cublasHandle_t handle, double *m, int ldm, int n, int p, int q, double alpha, double beta){
    cublasDscal (handle, n-q, &alpha, &m[IDX2C(p,q,ldm)], ldm);
    cublasDscal (handle, ldm-p, &beta, &m[IDX2C(p,q,ldm)], 1);
}

int main (void){
    uint64_t total_start        = 0u;
    uint64_t total_stop         = 0u;
    uint64_t total_runtime      = 0u;
    uint64_t size               = 18680;
    uint64_t M = size;
    uint64_t N = size;
    double ergebnis         = 0.0f;


    total_start = get_time_us();

    cudaError_t cudaStat;
    cublasStatus_t stat;
    cublasHandle_t handle;
    int i, j;
    double* devPtrA;
    double* a = 0;
    a = (double *)malloc (M * N * sizeof (*a));
    if (!a) {
        printf ("host memory allocation failed");
        return EXIT_FAILURE;
    }
    for (j = 0; j < N; j++) {
        for (i = 0; i < M; i++) {
            a[IDX2C(i,j,M)] = (double)(i * N + j + 1);
        }
    }
    cudaStat = cudaMalloc ((void**)&devPtrA, M*N*sizeof(*a));
    if (cudaStat != cudaSuccess) {
        printf ("device memory allocation failed");
        return EXIT_FAILURE;
    }
    stat = cublasCreate(&handle);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf ("CUBLAS initialization failed\n");
        return EXIT_FAILURE;
    }
    stat = cublasSetMatrix (M, N, sizeof(*a), a, M, devPtrA, M);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf ("data download failed");
        cudaFree (devPtrA);
        cublasDestroy(handle);
        return EXIT_FAILURE;
    }

    //measure kernel time
    printf("before kernel call!\n");
    modify (handle, devPtrA, M, N, 1, 2, 16.0f, 12.0f);
    cudaDeviceSynchronize();

    stat = cublasGetMatrix (M, N, sizeof(*a), devPtrA, M, a, M);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf ("data upload failed");
        cudaFree (devPtrA);
        cublasDestroy(handle);
        return EXIT_FAILURE;
    }
    cudaFree (devPtrA);
    cublasDestroy(handle);

    for (j = 0; j < N; j++) {
        for (i = 0; i < M; i++) {
            //printf ("%7.0f", a[IDX2C(i,j,M)]);
            ergebnis = a[IDX2C(i,j,M)];
        }
        //printf ("\n");
    }
    printf("(ergebnis:) %lf\n", ergebnis);

    total_stop = get_time_us();

    total_runtime = total_stop - total_start;
    //total_runtime /= 1000000;
    if(total_runtime == 0) total_runtime = 1u;

    
    double FLOPsTotalTime = 2 * pow(size, 3) *1000000 / total_runtime;
    
    printf("Total Runtime(us): %lu\n FLOP/s(total runtime): %.1lf\n", total_runtime, FLOPsTotalTime);

    free(a);
    return EXIT_SUCCESS;
}
