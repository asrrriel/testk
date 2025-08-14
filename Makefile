SRC_DIR = src
OBJ_DIR = obj

C_SRC = $(shell find $(SRC_DIR) -name "*.c")
C_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRC))

CC = gcc
CFLAGS = -Wall -g -I$(SRC_DIR)

$(shell mkdir -p $(OBJ_DIR))

all: testk

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

testk: $(C_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_DIR)/*.o $(GEN_DIR)/*.c testk

# Phony targets
.PHONY: all clean
