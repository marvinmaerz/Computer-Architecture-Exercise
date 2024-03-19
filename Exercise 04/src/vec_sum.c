#define _POSIX_C_SOURCE 199309L
#include <immintrin.h>
#include "vec_sum.h"
#include <stdio.h>


#ifndef NUM
	#define NUM 1
#endif

float vec_sum(float *array, uint32_t length) {
	if(NUM < 1 || (NUM > 4 && NUM != 8)){
		fprintf(stderr, "NUM nicht gültig");
	}
	float result[8] = {0.0f};
	uint32_t remainder = length % NUM;
	__m256 accuAVX;
	__m256 resultAVX = _mm256_setzero_ps();

	
	#pragma nounroll
	#pragma novector
	
	#if NUM == 1
	for(uint32_t i = 0; i<length; i++){
		#pragma nounroll
		#pragma novector
		result[0] += array[i];
	}
	#elif NUM == 2
	for(uint32_t i = 0; i < length-remainder; i+=2){
		#pragma nounroll
		#pragma novector
		result[0] += array[i];
		result[1] += array[i+1];
	}
	#elif NUM == 3
	for(uint32_t i = 0; i < length-remainder; i+=3){
		#pragma nounroll
		#pragma novector
	    result[0] += array[i];
        result[1] += array[i+1];
		result[2] += array[i+2];
        }
	#elif NUM == 4 
	for(uint32_t i = 0; i < length-remainder; i+=4){
		#pragma nounroll
		#pragma novector
		result[0] += array[i];
		result[1] += array[i+1];
		result[2] += array[i+2];
		result[3] += array[i+3];
	}

	#elif NUM == 8
	//TODO: Implement Vectorization with AVX 512 (8 doubles!)
	for(uint32_t i = 0; i < length-remainder; i+=8){
		#pragma nounroll
		#pragma novector
		accuAVX = _mm256_loadu_ps(&array[i]);
		resultAVX = _mm256_add_ps(resultAVX, accuAVX);

        /* unrolled but no vectorization:
		result[0] += array[i];
        result[1] += array[i+1];
        result[2] += array[i+2];
        result[3] += array[i+3];
		result[4] += array[i+4];
        result[5] += array[i+5];
        result[6] += array[i+6];
        result[7] += array[i+7];
		*/
    }
	_mm256_storeu_ps(result, resultAVX);
	#endif
	
	result[0] = result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];

	//TODO: convert to for-loop
	/* switch(remainder){
		case 7: result[0] += array[length-7];
		case 6:	result[0] += array[length-6];
		case 5:	result[0] += array[length-5];
                case 4: result[0] += array[length-4];
                case 3: result[0] += array[length-3];
		case 2: result[0] += array[length-2];
		case 1: result[0] += array[length-1];
	} */

	for(int i = remainder; i > 0; i--){
		result[0] += array[length-i];
	}

	return result[0];
}

