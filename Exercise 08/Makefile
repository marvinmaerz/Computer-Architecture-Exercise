.PHONY: all clean

ROOT_PATH 	:= .
SRC_PATH 	:= $(ROOT_PATH)/src
BUILD_PATH 	:= $(ROOT_PATH)/build
BIN_PATH	:= $(ROOT_PATH)/bin

INC_PATH	:= $(SRC_PATH)/include
INC_DIRS	:= $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS	:= $(addprefix -iquote ,$(INC_DIRS))
NVCC_FLAGS	:= $(addprefix -I ,$(INC_DIRS))

CC			:= icc
NVCC			:= nvcc

CFLAGS 			:= -Wall -pedantic -Werror -qopenmp -qopt-streaming-stores always -O3 -diag-disable=10441 -fPIE #-std=c11 # <- causing problem with "sys/time.h" header
CFLAGS_LIKWID 	:= -I/apps/likwid/5.3.0/include -DLIKWID_PERFMON
LDFLAGS_LIKWID 	:= -pthread -L/apps/likwid/5.3.0/lib/ -llikwid

all: cpuStream gpuStream

cpuStream: $(BIN_PATH)/cpuStream

gpuStream: $(BIN_PATH)/gpuStream

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

#builds cpuStream.o file
$(BIN_PATH)/cpuStream: $(BUILD_PATH)/cpuStream.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

#build gpuStream.o file
$(BIN_PATH)/gpuStream: $(BUILD_PATH)/gpuStream.o $(BUILD_PATH)/get_time.o
	mkdir -p $(dir $@)
	$(NVCC) $(NVCC_FLAGS) $^ -o $@

# builds cpuStream and gpuStream executables from .o files
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@)
	$(NVCC) $(NVCC_FLAGS) -O3 -MMD -MP -c $< -o $@

# builds cpuStream and gpuStream executables from .o files
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@



# an example of a more complex build line for the Intel icc compiler
# stream.icc: stream.c
#	icc -O3 -xCORE-AVX2 -ffreestanding -qopenmp -DSTREAM_ARRAY_SIZE=80000000 -DNTIMES=20 stream.c -o stream.omp.AVX2.80M.20x.icc
