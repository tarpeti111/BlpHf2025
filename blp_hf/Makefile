CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -lcurl

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
BIN := build/build

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(BIN)

rebuild: clean all

.PHONY: all clean rebuild