.PHONY: all clean vecSum

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := icc #-pg							#(Intel Compiler, used on the Supercluster)

CFLAGS_LIKWID := -I/apps/likwid/5.3.0/include -DLIKWID_PERFMON
LDFLAGS_LIKWID := -pthread -L/apps/likwid/5.3.0/lib/ -llikwid

NUM	:= 4
CFLAGS  := -Wall -pedantic -Werror -std=c99 -DNUM=$(NUM) -O3	#(-xHost: Flag for the Intel Compiler; -03: Flag for Optimization (turn off for debugging)) 
LDFLAGS := 


# all (with two colons): multiple branches need to be executed to 'make all' or 'make'
all:: jacobi
all:: jacobiBlocking

clean:	# 'make clean'
	rm -rf $(BUILD_PATH) $(BIN_PATH)	

cleanRun: # 'make cleanRun'
	find $(ROOT_PATH)/scripts/ -name 'meggie.sh.*' -exec rm {} \;
	rm $(ROOT_PATH)/scripts/ready
	

jacobi: $(BIN_PATH)/jacobi			# 'make jacobi'
jacobiBlocking: $(BIN_PATH)/jacobiBlocking

# builds the jacobiBlocking file
$(BIN_PATH)/jacobiBlocking: $(BUILD_PATH)/mainBlocking.o $(BUILD_PATH)/jacobiBlocking.o $(BUILD_PATH)/get_time.o $(BUILD_PATH)/draw.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# builds the jacobi file
$(BIN_PATH)/jacobi: $(BUILD_PATH)/main.o $(BUILD_PATH)/jacobi.o $(BUILD_PATH)/get_time.o $(BUILD_PATH)/draw.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# compiles the .c files specified above into .o files into BUILD_PATH
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

