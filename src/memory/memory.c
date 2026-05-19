#include "memory.h"
#include <stdlib.h>
#include <string.h>

uint16_t memory_read(uint16_t address, uint16_t* memory) {
    if (memory == NULL) {
        perror("Can't read from null memory reference");
        exit(EXIT_FAILURE);
    }

    if (address < 0 || address >= MEMORY_MAX) {
        perror("Address out of bounds from memory");
        exit(EXIT_FAILURE);
    }

    return memory[address];
}

int memory_write(uint16_t address, uint16_t val, uint16_t* memory) {
    if (memory == NULL) {
        perror("Can't write to null memory reference");
        return EXIT_FAILURE;
    }
    memory[address] = val;
    return EXIT_SUCCESS;
}