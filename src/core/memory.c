#include "memory.h"
#include "vm.h"
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

/*******************************************************************************
 ******************** PRIVATE UTILITY FUNCTION DECLARATIONS ********************
 *******************************************************************************/

/*
    * swap each uint16_t type from big_endian to little-endian because almost 
    * every modern computer uses little-endian
    * @param x: the uint16_t type variable which we will swap 
    * @return the little-endian version of the big_endian variable
*/
static uint16_t swap16(uint16_t x);

static uint16_t check_key();

 /*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

uint16_t memory_read(lc3_vm_t* vm, uint16_t address) {
    if (vm == NULL) {
        perror("Can't read from null memory reference");
        exit(EXIT_FAILURE);
    }

    if (address == MR_KBSR)
    {
        if (check_key())
        {
            vm->memory[MR_KBSR] = (1 << 15);
            vm->memory[MR_KBDR] = getchar();
        }
        else
        {
            vm->memory[MR_KBSR] = 0;
        }
    }

    return vm->memory[address];
}

int memory_write(lc3_vm_t* vm, uint16_t address, uint16_t val) {
    if (vm == NULL) {
        perror("Can't write to null memory reference");
        return EXIT_FAILURE;
    }

    vm->memory[address] = val;
    return EXIT_SUCCESS;
}

int loadfile(FILE* file, lc3_vm_t* vm) {
    if (vm == NULL) {
        perror("Can't load a file in an unitialized memory!");
        return EXIT_FAILURE;
    }

    uint16_t origin;
    if (fread(&origin, sizeof(origin), 1, file) != 1) {
        perror("Unable to read image origin");
        return EXIT_FAILURE;
    }
    origin = swap16(origin);

    uint16_t* p = vm->memory + origin;
    size_t read = fread(p, sizeof(uint16_t), MEMORY_MAX - origin, file);
    
    while (read-- > 0) {
        *p = swap16(*p);
        ++ p;
    }

    return EXIT_SUCCESS;
}

int loadfile_from_path(const char* image_path, lc3_vm_t* vm) {
    if (image_path == NULL) {
        perror("The given file path is not valid");
        return EXIT_FAILURE;
    }
    FILE* file;
    if ((file = fopen(image_path, "rb")) == NULL) {
        perror("Error in opening the file!");
        return EXIT_FAILURE;
    }
    int result = loadfile(file, vm);
    fclose(file);
    return result;
}

/*******************************************************************************
 ********************** PRIVATE FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

static uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}

static uint16_t check_key() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}
