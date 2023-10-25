#include <stdio.h>
#include <stdlib.h>
#include "vec_sum.h"

int main(int argc, char *argv[]){
    const float *testArray = [1, 1, 1, 1, 1, 1, 1];
    const uint32_t testLength = 7u;

    flot result = vec_sum(testArray, testLength);

    printf("result of vecSum: %f", result);

}