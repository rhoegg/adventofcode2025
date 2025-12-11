CC      := clang
CFLAGS  := -std=c23 -Wall -Wextra -O3 -Ithird_party/spookyhash
CFLAGS_DEBUG := -std=c23 -Wall -Wextra -O0 -g -fsanitize=address,undefined -Ithird_party/spookyhash
LDFLAGS :=

DAY ?= 01

SRC_COMMON := common.c third_party/spookyhash/spookyhash.c
SRC_DAY    := day$(DAY)/*.c
BIN        := bin/day$(DAY)

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC_COMMON) $(SRC_DAY)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

debug: $(SRC_COMMON) $(SRC_DAY)
	@mkdir -p bin
	$(CC) $(CFLAGS_DEBUG) -o bin/day$(DAY)_debug $^ $(LDFLAGS)
run: $(BIN)
	@mkdir -p inputs
	./$(BIN) inputs/day$(DAY).txt

clean:
	rm -rf bin