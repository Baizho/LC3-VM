#ifndef VM_H
#define VM_H

#include "memory/memory.h"
#include <stdint.h>

/*
    * LC-3 Virtual Machine core state.
    * This struct keeps the VM memory, registers, and run flag together,
    * instead of spreading them as separate global arrays.
*/
struct lc3_vm {
    uint16_t memory[MEMORY_MAX];
    uint16_t reg[R_COUNT];
    uint8_t is_running;
};

/*
    * Initializes the given LC-3 VM state.
    * @param vm: pointer to the VM state to initialize
    * @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
*/
int lc3_vm_init(lc3_vm_t* vm);

#endif // VM_H
