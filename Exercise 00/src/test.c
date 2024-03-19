#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vec_sum.h"
#include "get_time.h"

int main(int argc, char *argv[]){
    float testArray[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    const uint32_t testLength = 7u;

    float result = vec_sum(testArray, testLength);

    printf("result of vecSum: %f \n", result);

    uint64_t start = get_time_us();
    sleep(10);
    uint64_t stop = get_time_us();
    uint64_t time = stop - start;
    printf("time passed in 10 seconds: %lu\n", time);       //time is in microseconds (wow, us)

}