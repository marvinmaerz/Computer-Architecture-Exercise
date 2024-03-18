#define _POSIX_C_SOURCE 199309L
#include "jacobi.h"
#include <immintrin.h>

#ifndef NUM
	#define NUM 1
#endif

void jacobi(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	uint32_t remainder = (X-2) % NUM;
	
	// Registers needed for SSE and AVX instructions
	const __m128d onefourthSSE = _mm_set_pd(0.25, 0.25);
	const __m256d onefourthAVX = _mm256_set_pd(0.25, 0.25, 0.25, 0.25);
	__m128d Reg1SSE;
	__m128d Reg2SSE;
	__m128d Reg3SSE;
	__m128d Reg4SSE;
	__m128d resultSSE;
	__m256d Reg1AVX;
	__m256d Reg2AVX;
	__m256d Reg3AVX;
	__m256d Reg4AVX;
	__m256d resultAVX;

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
		// using SSE instructions (128 bit, 2 doubles! per instruction)
		for(uint32_t x = 1; x < X-1-remainder; x+=2){
				
			Reg1SSE = _mm_loadu_pd(& grid_source[(y-1)*X+x]);
			Reg2SSE = _mm_loadu_pd(& grid_source[(y+1)*X+x]);
			resultSSE = _mm_add_pd(Reg1SSE, Reg2SSE);
			Reg3SSE = _mm_loadu_pd(& grid_source[y*X+(x-1)]);
			resultSSE = _mm_add_pd(resultSSE, Reg3SSE);
			Reg4SSE = _mm_loadu_pd(& grid_source[y*X+(x+1)]);
			resultSSE = _mm_add_pd(resultSSE, Reg4SSE);
			resultSSE = _mm_mul_pd(resultSSE, onefourthSSE);
			_mm_storeu_pd(& grid_target[y*X+x], resultSSE);
		}

		#elif NUM == 4
		// using AVX instructions (256 bit, 4 doubles! per instruction)
		for(uint32_t x = 1; x < X-1-remainder; x+=4){
		
			Reg1AVX = _mm256_loadu_pd(& grid_source[(y-1)*X+x]);
			Reg2AVX = _mm256_loadu_pd(& grid_source[(y+1)*X+x]);
			resultAVX = _mm256_add_pd(Reg1AVX, Reg2AVX);
			Reg3AVX = _mm256_loadu_pd(& grid_source[y*X+(x-1)]);
			resultAVX = _mm256_add_pd(resultAVX, Reg3AVX);
			Reg4AVX = _mm256_loadu_pd(& grid_source[y*X+(x+1)]);
			resultAVX = _mm256_add_pd(resultAVX, Reg4AVX);
			resultAVX = _mm256_mul_pd(resultAVX, onefourthAVX);
			_mm256_storeu_pd(& grid_target[y*X+x], resultAVX);	
		}
		#endif

		for(int i = remainder; i > 0; i--){
			grid_target[y*X+(X-i-1)] = (grid_source[(y-1)*X+(X-i-1)] + grid_source[(y+1)*X+(X-i-1)] + grid_source[y*X+(X-i-2)] + grid_source[y*X+(X-i)]) * 0.25;
		}
	}
}

