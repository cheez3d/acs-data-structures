CC := gcc
CFLAGS := -Wall -g
# TODO: fix valgrind recipe problem need to recompile with -g flag

STANDARD := c99
DEFINES := 

INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

$(shell mkdir -p $(INC_DIR) $(SRC_DIR) $(OBJ_DIR))

BIN := main
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRC:%.c=%.o))
DEP := $(OBJ:%.o=%.d)

.PHONY: print-%
print-%:
	@echo $*=$($*)

.DEFAULT_GOAL := build
build: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -lm -o $(BIN)

-include $(DEP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -I$(INC_DIR) -MMD -c $(STANDARD:%=-std=%) $(CFLAGS) $(DEFINES:%=-D%) $< -o $@

.PHONY: run
run: build
	@./$(BIN)

.PHONY: clean
clean:
	@-rm -f $(OBJ) $(DEP) $(BIN)

.PHONY: debug
debug:
	$(eval CFLAGS += -g)

.PHONY: valgrind
valgrind: debug build
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN)
