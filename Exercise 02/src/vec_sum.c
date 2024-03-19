#include "vec_sum.h"
#include <stdio.h>

#ifndef NUM
	#define NUM 1
#endif

float vec_sum(float *array, uint32_t length) {
	if(NUM < 1 || (NUM > 4 && NUM != 8)){
		fprintf(stderr, "NUM nicht gültig");
	}
	float result[NUM] = {0.0f};
	uint32_t remainder = length % NUM;

	
	#pragma nounroll
	#pragma novector
	
	#if NUM == 1
	for(uint32_t i = 0; i<length; i++){
		result[0] += array[i];
	}
	#elif NUM == 2
	for(uint32_t i = 0; i < length-remainder; i+=2){
		result[0] += array[i];
		result[1] += array[i+1];
	}
	#elif NUM == 3
	for(uint32_t i = 0; i < length-remainder; i+=3){
	        result[0] += array[i];
                result[1] += array[i+1];
		result[2] += array[i+2];
        }
	#elif NUM == 4 
	for(uint32_t i = 0; i < length-remainder; i+=4){
		result[0] += array[i];
		result[1] += array[i+1];
		result[2] += array[i+2];
		result[3] += array[i+3];
	}
	#elif NUM == 8
	for(uint32_t i = 0; i < length-remainder; i+=8){
        	result[0] += array[i];
                result[1] += array[i+1];
                result[2] += array[i+2];
                result[3] += array[i+3];
		result[4] += array[i+4];
                result[5] += array[i+5];
                result[6] += array[i+6];
                result[7] += array[i+7];
        }
	#endif
	
	result[0] = result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];
	switch(remainder){
		case 7: result[0] += array[length-7];
		case 6:	result[0] += array[length-6];
		case 5:	result[0] += array[length-5];
                case 4: result[0] += array[length-4];
                case 3: result[0] += array[length-3];
		case 2: result[0] += array[length-2];
		case 1: result[0] += array[length-1];
	}
	return result[0];
}

