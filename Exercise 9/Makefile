.PHONY: all clean

ROOT_PATH 	:= .
SRC_PATH 	:= $(ROOT_PATH)/src
BUILD_PATH 	:= $(ROOT_PATH)/build
BIN_PATH	:= $(ROOT_PATH)/bin

INC_PATH	:= $(SRC_PATH)/include
INC_DIRS	:= $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS	:= $(addprefix -iquote ,$(INC_DIRS))
NVCC_FLAGS	:= $(addprefix -I ,$(INC_DIRS))

CC				:= icc
NVCC			:= nvcc

CFLAGS 			:= -Wall -pedantic -Werror -qopenmp -qopt-streaming-stores always -O3 -diag-disable=10441 -fPIE #-std=c11 # <- causing problem with "sys/time.h" header
CFLAGS_LIKWID 	:= -I/apps/likwid/5.3.0/include -DLIKWID_PERFMON
LDFLAGS_LIKWID 	:= -pthread -L/apps/likwid/5.3.0/lib/ -llikwid

all: jacobiCUDA

jacobiCUDA: $(BIN_PATH)/jacobiCUDA

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

#build jacobiCUDA executable file
$(BIN_PATH)/jacobiCUDA: $(BUILD_PATH)/jacobiCUDA.o $(BUILD_PATH)/get_time.o
	mkdir -p $(dir $@)
	$(NVCC) $(NVCC_FLAGS) $^ -o $@

# builds jacobiCUDA.o file with nvcc
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@)
	$(NVCC) $(NVCC_FLAGS) -O3 -MMD -MP -c $< -o $@

# builds get_time.o file with icc
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@