CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -g -Og
# CFLAGS := -std=c11 -Wall -Wextra -O2 -flto

CFLAGS_1 := -std=gnu11 -Wall -Wextra -g -Og
# CFLAGS_1 := -std=gnu11 -Wall -Wextra -O2 -flto
LDFLAGS_1 := -Wall -Wextra -Og
# LDFLAGS_1 := -Wall -Wextra -O2 -flto
LDLIBS_1 := 

CFLAGS_2 := -std=c11 -Wall -Wextra -g -Og
# CFLAGS_2 := -std=c11 -Wall -Wextra -O2 -flto
LDFLAGS_2 := $(LDFLAGS_1)
LDLIBS_2 := 

INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj

SRC_DIR_1 := pb-1

SRC_DIR_2 := pb-2

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(SRC:%.c=%.o))
DEP := $(OBJ:%.o=%.d)

BIN_1 := movies
SRC_1 := $(wildcard $(SRC_DIR)/$(SRC_DIR_1)/*.c)
OBJ_1 := $(subst $(SRC_DIR)/$(SRC_DIR_1)/,$(OBJ_DIR)/$(SRC_DIR_1)/,$(SRC_1:%.c=%.o))
DEP_1 := $(OBJ_1:%.o=%.d)

BIN_2 := labirint
SRC_2 := $(wildcard $(SRC_DIR)/$(SRC_DIR_2)/*.c)
OBJ_2 := $(subst $(SRC_DIR)/$(SRC_DIR_2)/,$(OBJ_DIR)/$(SRC_DIR_2)/,$(SRC_2:%.c=%.o))
DEP_2 := $(OBJ_2:%.o=%.d)

$(shell mkdir -p $(INC_DIR) $(SRC_DIR) $(OBJ_DIR))

$(shell mkdir -p $(SRC_DIR)/$(SRC_DIR_1) $(OBJ_DIR)/$(SRC_DIR_1))

$(shell mkdir -p $(SRC_DIR)/$(SRC_DIR_2) $(OBJ_DIR)/$(SRC_DIR_2))

.DEFAULT_GOAL := build

$(BIN_1): $(OBJ) $(OBJ_1)
	$(CC) $(LDFLAGS_1) $^ $(LDLIBS_1) -o $(BIN_1)

$(BIN_2): $(OBJ) $(OBJ_2)
	$(CC) $(LDFLAGS_2) $^ $(LDLIBS_2) -o $(BIN_2)

-include $(DEP)

-include $(DEP_1)

-include $(DEP_2)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	$(CC) -iquote $(INC_DIR) -MMD -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/$(SRC_DIR_1)/%.o: $(SRC_DIR)/$(SRC_DIR_1)/%.c Makefile
	$(CC) -iquote $(INC_DIR) -iquote $(INC_DIR)/$(SRC_DIR_1) -MMD -c $(CFLAGS_1) $< -o $@

$(OBJ_DIR)/$(SRC_DIR_2)/%.o: $(SRC_DIR)/$(SRC_DIR_2)/%.c Makefile
	$(CC) -iquote $(INC_DIR) -iquote $(INC_DIR)/$(SRC_DIR_2) -MMD -c $(CFLAGS_2) $< -o $@

build1: $(BIN_1)

build2: $(BIN_2)

build: build1 build2

valgrind2: build2
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_2)

.PHONY: clean
clean:
	@-rm -f $(BIN_1) $(BIN_2) $(OBJ_1) $(OBJ_2) $(OBJ) $(DEP_1) $(DEP_2) $(DEP)
