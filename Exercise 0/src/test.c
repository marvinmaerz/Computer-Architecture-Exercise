#include <stdio.h>
#include <stdlib.h>
#include "vec_sum.h"

int main(int argc, char *argv[]){
    float testArray[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    const uint32_t testLength = 7u;

    float result = vec_sum(testArray, testLength);

    printf("result of vecSum: %f \n", result);

}