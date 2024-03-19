.PHONY: all clean vecSum

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := icc 							#(Intel Compiler, used on the Supercluster)
NUM	:= 1
CFLAGS  := -Wall -pedantic -Werror -std=c99 -DNUM=$(NUM)	#(-xHost: Flag for the Intel Compiler; -03: Flag for Optimization (turn off for debugging)) 

LDFLAGS :=


# all (with two colons): multiple branches need to be executed to 'make all' or 'make'
all:: vecSum
all:: jacobi

clean:	# 'make clean'
	rm -rf $(BUILD_PATH) $(BIN_PATH)	

cleanRun: # 'make cleanRun'
	find $(ROOT_PATH)/scripts/ -name 'meggie.sh.*' -exec rm {} \;
	rm $(ROOT_PATH)/scripts/ready
	

vecSum: $(BIN_PATH)/vecSum			# 'make vecSum'

jacobi: $(BIN_PATH)/jacobi			# 'make jacobi'



# builds the jacobi file
$(BIN_PATH)/vecSum: $(BUILD_PATH)/mainVec.o $(BUILD_PATH)/vec_sum.o $(BUILD_PATH)/get_time.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# builds the test file
$(BIN_PATH)/jacobi: $(BUILD_PATH)/mainJacobi.o $(BUILD_PATH)/jacobi.o $(BUILD_PATH)/get_time.o $(BUILD_PATH)/draw.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# compiles the .c files specified above into .o files into BUILD_PATH
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

