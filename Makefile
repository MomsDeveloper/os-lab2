CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
BIN_DIR = bin
INC_DIR = include
OUT_DIR = output
TEST_DIR = test

SOURCES = $(SRC_DIR)/cache.c $(SRC_DIR)/file_operations.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBRARY = $(LIB_DIR)/liblab2.so
BENCHMARKS = $(BIN_DIR)

all: create_dirs $(LIBRARY) $(BENCHMARKS) $(TEST_DIR)/test

$(LIBRARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LIBRARY) $(OBJECTS)

$(BENCHMARKS): $(LIBRARY) $(SRC_DIR)/ema-sort-int.c $(SRC_DIR)/ema-with-cache.c
	$(CC) -DBENCH1_MAIN $(CFLAGS) -I$(INC_DIR) -o $(BIN_DIR)/ema-sort-int $(SRC_DIR)/ema-sort-int.c
	$(CC) -DBENCH2_MAIN $(CFLAGS) -I$(INC_DIR) -o $(BIN_DIR)/ema-with-cache -Wl,-rpath ./lib $(SRC_DIR)/ema-with-cache.c -L$(LIB_DIR) -llab2

$(TEST_DIR)/test: $(LIBRARY) $(TEST_DIR)/test.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(BIN_DIR)/test -Wl,-rpath ./lib $(TEST_DIR)/test.c $(SRC_DIR)/ema-with-cache.c -L$(LIB_DIR) -llab2

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ -c $<

create_dirs:
	mkdir -p $(OBJ_DIR) $(LIB_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OUT_DIR)/$(TEST_DIR)

profile: all 
	./run_profiling.sh $(BIN_DIR)/ema-sort-int $(OUT_DIR)/ema-sort-int 1
	./run_profiling.sh $(BIN_DIR)/ema-with-cache $(OUT_DIR)/ema-with-cache 1

test : all 
	./bin/test

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(OUT_DIR)

