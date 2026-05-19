CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
CPPFLAGS ?= -Isrc

TARGET := lc3
SRC := src/main.c src/core/vm.c src/core/memory.c src/core/opcodes.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)
