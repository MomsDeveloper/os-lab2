CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
BIN_DIR = bin
INC_DIR = include

SOURCES = $(SRC_DIR)/cache.c $(SRC_DIR)/file_operations.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBRARY = $(LIB_DIR)/liblab2.so
BENCHMARKS = $(BIN_DIR)

all: create_dirs $(LIBRARY) $(BENCHMARKS)

$(LIBRARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LIBRARY) $(OBJECTS)

# $(BENCHMARK): $(LIBRARY) $(SRC_DIR)/benchmark.c
# 	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(BENCHMARK) $(SRC_DIR)/benchmark.c -L$(LIB_DIR) -llab2

$(BENCHMARKS): $(LIBRARY) $(SRC_DIR)/ema-sort-int.c $(SRC_DIR)/ema-with-cache.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(BIN_DIR)/ema-sort-int $(SRC_DIR)/ema-sort-int.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(BIN_DIR)/ema-with-cache -Wl,-rpath ./lib $(SRC_DIR)/ema-with-cache.c -L$(LIB_DIR) -llab2

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ -c $<

create_dirs:
	mkdir -p $(OBJ_DIR) $(LIB_DIR)
	mkdir -p $(BIN_DIR)

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)/*.o $(LIB_DIR)/liblab2.so
	rm -rf $(BIN_DIR)/benchmark

