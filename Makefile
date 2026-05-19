CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
CPPFLAGS ?= -Isrc

TARGET := lc3
SRC := src/main.c src/core/vm.c src/core/memory.c src/core/opcodes.c
CORE_SRC := src/core/vm.c src/core/memory.c src/core/opcodes.c
TESTS := tests/bin/test_add tests/bin/test_branch

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET) $(ARGS)

test: $(TESTS)
	@for test in $(TESTS); do ./$$test; done

tests/bin:
	mkdir -p tests/bin

tests/bin/test_add: tests/test_add.c tests/test.h $(CORE_SRC) | tests/bin
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ tests/test_add.c $(CORE_SRC)

tests/bin/test_branch: tests/test_branch.c tests/test.h $(CORE_SRC) | tests/bin
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ tests/test_branch.c $(CORE_SRC)

clean:
	rm -f $(TARGET) $(TESTS)
