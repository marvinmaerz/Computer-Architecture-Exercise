#define _POSIX_C_SOURCE 199309L
#include "jacobiTile.h"
#include <immintrin.h>

#ifndef tbx
	#define tbx 608
#endif

#ifndef by
	#define by 20
#endif



void jacobiTile(double* grid_source, double* grid_target, uint32_t X, uint32_t Y) {
	uint32_t remainder = (X-2) % 4;
	
	// Registers needed for SSE and AVX instructions
	const __m256d onefourthAVX = _mm256_set_pd(0.25, 0.25, 0.25, 0.25);
	__m256d Reg1AVX;
	__m256d Reg2AVX;
	__m256d Reg3AVX;
	__m256d Reg4AVX;
	__m256d resultAVX;

	#pragma nounroll
	#pragma novector
	for(uint32_t j = 0; j*by < Y-1; j++){
		#pragma nounroll
		#pragma novector
		for(uint32_t i = 0; i*tbx < X-1-remainder; i++){
			#pragma nounroll
			#pragma novector
			for(uint32_t y = (j*by)+1; y <= (j+1)*by && y < Y-1; y++){
				#pragma nounroll
				#pragma novector
				for(uint32_t x = (i*tbx)+1; x <= (i+1)*tbx && x < X-1-remainder; x+=4){
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

				for(int i = remainder; i > 0; i--){
					grid_target[y*X+(X-i-1)] = (grid_source[(y-1)*X+(X-i-1)] + grid_source[(y+1)*X+(X-i-1)] + grid_source[y*X+(X-i-2)] + grid_source[y*X+(X-i)]) * 0.25;
				}
			}
		}
	}
}

