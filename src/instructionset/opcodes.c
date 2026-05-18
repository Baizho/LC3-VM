#include "opcodes.h"
#include <stdlib.h>
#include <stdio.h>

uint16_t execute_instruction(uint16_t instruction, uint16_t* memory, uint16_t* reg, uint8_t* endFLag) {
    switch (opcode(instruction)) {
        case OP_ADD:
        case OP_AND:
        case OP_NOT:
        case OP_BR:
        case OP_JMP:
        case OP_JSR:
        case OP_LD:
        case OP_LDI:
        case OP_LEA:
        case OP_LDR:
        case OP_ST:
        case OP_STI:
        case OP_STR:
        case OP_TRAP:
        case OP_RES:
        case OP_RTI:
        default:
            perror("Wrong opcode format! Operation aborted.");
            abort();
    }
}

uint16_t opcode(uint16_t instruction) {
    return (instruction >> 12) & 0xF;
}