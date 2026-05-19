#include "memory.h"
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 ******************** PRIVATE UTILITY FUNCTION DECLARATIONS ********************
 *******************************************************************************/

/*
    * swap each uint16_t type from big_endian to little-endian because almost 
    * every modern computer uses little-endian
    * @param x: the uint16_t type variable which we will swap 
    * @return the little-endian version of the big_endian variable
*/
uint16_t swap16(uint16_t x);

 /*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

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

int loadfile(FILE* file, uint16_t* memory) {
    if (memory == NULL) {
        perror("Can't load a file in an unitialized memory!");
        return EXIT_FAILURE;
    }
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);

    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), MEMORY_MAX - origin, file);
    
    while (read-- > 0) {
        *p = swap16(*p);
        ++ p;
    }

    return EXIT_SUCCESS;
}

int loadfile_from_path(const char* image_path, uint16_t* memory) {
    if (image_path == NULL) {
        perror("The given file path is not valid");
        return EXIT_FAILURE;
    }
    FILE* file;
    if ((file = fopen(image_path, "rb")) == NULL) {
        perror("Error in opening the file!");
        return EXIT_FAILURE;
    }
    loadfile(file, memory);
    fclose(file);
    return EXIT_SUCCESS;
}

/*******************************************************************************
 ********************** PRIVATE FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}