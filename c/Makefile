CC      := clang
CFLAGS  := -std=c23 -Wall -Wextra -O3
LDFLAGS :=

DAY ?= 01

SRC_COMMON := common.c
SRC_DAY    := day$(DAY)/*.c
BIN        := bin/day$(DAY)

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC_COMMON) $(SRC_DAY)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN)
	@mkdir -p inputs
	./$(BIN) inputs/day$(DAY).txt

clean:
	rm -rf bin