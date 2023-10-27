#include "vec_sum.h"

float vec_sum(float *array, uint32_t length) {
	float result = 0.0f;

	for(int i = 0; i < length; i++){
		result += array[i];
	}
	
	return result;
}

