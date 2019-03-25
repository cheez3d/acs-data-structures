CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -O2 -flto
LDFLAGS := -Wall -Wextra -O2 -flto
LDLIBS := -lm

INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

BIN := tema1
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRC:%.c=%.o))
DEP := $(OBJ:%.o=%.d)

$(shell mkdir -p $(INC_DIR) $(SRC_DIR) $(OBJ_DIR))

.DEFAULT_GOAL := build
build: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(BIN)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(CFLAGS) $< -o $@

.PHONY: run
run: build
	@./$(BIN)

.PHONY: clean
clean:
	@-rm -f $(OBJ) $(DEP) $(BIN)
