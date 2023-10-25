.PHONY: all clean vecSum

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := gcc # icc (Intel Compiler, used on the Supercluster)
CFLAGS  := -Wall -pedantic -Werror -std=c99 -O3 #-xHost		(Flag for the Intel Compiler)

LDFLAGS :=

all: vecSum # program is named vecSum; you can also type 'make all' instead of 'make'

clean:	# 'make clean'
	rm -rf $(BUILD_PATH) $(BIN_PATH)	

vecSum: $(BIN_PATH)/vecSum			# 'make vecSum'

test: $(BIN_PATH)/test				# 'make test'



# builds the vecSum file
$(BIN_PATH)/vecSum: $(BUILD_PATH)/main.o $(BUILD_PATH)/vec_sum.o $(BUILD_PATH)/get_time.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# builds the test file
$(BIN_PATH)/test: $(BUILD_PATH)/test.o $(BUILD_PATH)/vec_sum.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

