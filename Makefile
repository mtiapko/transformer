CC := g++
LD := g++

CC_FLAGS := -std=c++20 -Wall -Wextra -Wpedantic -MMD -MP -O3
CC_INCLUDES := -I./include -I./libs/inja/include -I./libs/json/include
CC_DEFINES :=

LD_FLAGS :=
LD_LIBS := -lLLVM -lclang-cpp

SRC_DIR := src src/AST
OBJ_DIR := bin/obj

SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))
OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC)))

OUT_NAME := transformer
OUT_DIR := ./bin/
OUT := $(addprefix $(OUT_DIR), $(OUT_NAME))

all: libs
	@$(MAKE) --no-print-directory --jobs 8 $(OUT)

$(OUT): $(OBJ)
	@mkdir --parent $(@D)
	$(LD) $(LD_FLAGS) $^ $(LD_LIBS) -o $(OUT)

$(OBJ_DIR)/%.o: %
	@mkdir --parent $(@D)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $(CC_DEFINES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)

-include $(OBJ:.o=.d)
