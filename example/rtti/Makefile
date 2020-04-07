CC := g++
LD := g++

CC_FLAGS := -std=c++2a -Wall -Wextra -Wpedantic -MMD -MP -g
CC_INCLUDES := -I./src -I./test -I../../libs/json/include -I./include
CC_DEFINES :=

LD_FLAGS :=
LD_LIBS :=

SRC_DIR := src src/DB test
OBJ_DIR := bin/obj

SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))
OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC)))

OUT_NAME := rtti
OUT_DIR := ./bin/
OUT := $(addprefix $(OUT_DIR), $(OUT_NAME))

all: gen
	@$(MAKE) --no-print-directory --jobs 8 $(OUT)

test/gen.cpp: template.jinja test/rect.h
	@ ../../bin/transformer ./template.jinja ./test/rect.h -o test/gen.cpp --compiler-args \
		-cxx-isystem /usr/lib/clang/9.0.1/include -std=c++2a -x c++ \
		-I../../libs/json/include \
		-I./include
	@ echo Done!

$(OUT): $(OBJ)
	@mkdir --parent $(@D)
	$(LD) $(LD_FLAGS) $^ $(LD_LIBS) -o $(OUT)

$(OBJ_DIR)/%.o: %
	@mkdir --parent $(@D)
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $(CC_DEFINES) -c $< -o $@

gen: test/gen.cpp

run: all
	@$(OUT)

debug: all
	gdb -ex r $(OUT)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OUT_DIR)

-include $(OBJ:.o=.d)
