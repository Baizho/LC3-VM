#include "opcodes.h"
#include <stdlib.h>
#include <stdio.h>


/*******************************************************************************
 ******************** PRIVATE UTILITY FUNCTION DECLARATIONS ********************
 *******************************************************************************/

/*
    * Sign-extends the given uint16_t value, shifting for a number of bits equal to
    * the integer argument passed to this function.
    * @param uint16_t the value to be sign_extended;
    * @param int the number of bits to shift in the sign_extending operation
    * @return the sign_extended value passed as uint16_t argument
 */
static uint16_t sign_extend(uint16_t x, int bit_count);

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

uint16_t execute_instruction(uint16_t instruction, uint16_t* memory, uint16_t* reg, uint8_t* endFLag) {
    switch (opcode(instruction)) {
        case OP_ADD:
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                reg[dr] = reg[sr1] + imm5;
            } 
            else {
                uint16_t sr2 = instruction & 0x7;
                reg[dr] = reg[sr1] + reg[sr2];
            }

            return update_flags(reg, dr);
        case OP_AND:
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                reg[dr] = reg[sr1] & imm5;
            } 
            else {
                uint16_t sr2 = instruction & 0x7;
                reg[dr] = reg[sr1] & reg[sr2];
            }

            return update_flags(reg, dr);
        case OP_NOT:
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            reg[dr] = !reg[sr1];

            return update_flags(reg, dr);
        case OP_BR:
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            uint16_t cond = (instruction >> 9) & 0x7;

            if (cond & reg[R_COND]) {
                reg[R_PC] += pc_offset;
            }

            return EXIT_SUCCESS;
        case OP_JMP:
            uint16_t baser = (instruction >> 6) & 0x7;
            reg[R_PC] = reg[baser];

            return EXIT_SUCCESS;
        case OP_JSR:
            uint16_t bit_flag = (instruction >> 11) & 0x1;
            reg[R_7] = reg[R_PC];

            if (bit_flag) {
                /* Executes the JSR operation*/
                uint16_t pc_offset = sign_extend(instruction & 0x7FF, 11);
                reg[R_PC] += pc_offset;
            }
            else {
                /* Executes JSRR*/
                uint16_t baser = (instruction >> 6) & 0x7;
                reg[R_PC] = reg[baser];
            }

            return EXIT_SUCCESS;
        case OP_LD:
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            
            reg[dr] = memory_read(R_PC + pc_offset, memory);

            return update_flags(reg[dr]);
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

int update_flags(uint16_t* reg, uint16_t r) {
    if (reg == NULL) {
        perror("Cannot update flags on a NULL register referencec");
        return EXIT_FAILURE;
    }
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15) {
        reg[R_COND] = FL_NEG;
    }
    else {
        reg[R_COND] = FL_POS;
    }
    return EXIT_SUCCESS;
}

/*******************************************************************************
 ********************** PRIVATE FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

 uint16_t sign_extend(uint16_t x, uint16_t bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
 }