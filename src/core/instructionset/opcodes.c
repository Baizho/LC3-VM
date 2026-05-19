#include "opcodes.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
 ******************** PRIVATE UTILITY FUNCTION DECLARATIONS ********************
 *******************************************************************************/

/*
    * Sign-extends the given uint16_t value, shifting for a number of bits equal to
    * the integer argument passed to this function.
    * @param x: the value to be sign-extended
    * @param bit_count: the number of bits used by the original immediate value
    * @return the sign-extended value passed as uint16_t argument
 */
static uint16_t sign_extend(uint16_t x, int bit_count);

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int execute_instruction(uint16_t instruction, lc3_vm_t* vm) {
    if (vm == NULL) {
        perror("Cannot execute instruction on a NULL VM reference");
        return EXIT_FAILURE;
    }

    switch (opcode(instruction)) {
        case OP_ADD: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                vm->reg[dr] = vm->reg[sr1] + imm5;
            } else {
                uint16_t sr2 = instruction & 0x7;
                vm->reg[dr] = vm->reg[sr1] + vm->reg[sr2];
            }

            return update_flags(vm, dr);
        }
        case OP_AND: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                vm->reg[dr] = vm->reg[sr1] & imm5;
            } else {
                uint16_t sr2 = instruction & 0x7;
                vm->reg[dr] = vm->reg[sr1] & vm->reg[sr2];
            }

            return update_flags(vm, dr);
        }
        case OP_NOT: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            vm->reg[dr] = (uint16_t)~vm->reg[sr1];
            return update_flags(vm, dr);
        }
        case OP_BR: {
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            uint16_t cond = (instruction >> 9) & 0x7;

            if (cond & vm->reg[R_COND]) {
                vm->reg[R_PC] += pc_offset;
            }

            return EXIT_SUCCESS;
        }
        case OP_JMP: {
            uint16_t base_r = (instruction >> 6) & 0x7;
            vm->reg[R_PC] = vm->reg[base_r];
            return EXIT_SUCCESS;
        }
        case OP_JSR: {
            uint16_t long_flag = (instruction >> 11) & 0x1;
            vm->reg[R_R7] = vm->reg[R_PC];

            if (long_flag) {
                uint16_t pc_offset = sign_extend(instruction & 0x7FF, 11);
                vm->reg[R_PC] += pc_offset;
            } else {
                uint16_t base_r = (instruction >> 6) & 0x7;
                vm->reg[R_PC] = vm->reg[base_r];
            }

            return EXIT_SUCCESS;
        }
        case OP_LD: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            vm->reg[dr] = memory_read(vm, vm->reg[R_PC] + pc_offset);
            return update_flags(vm, dr);
        }
        case OP_LDI: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            vm->reg[dr] = memory_read(vm, memory_read(vm, vm->reg[R_PC] + pc_offset));
            return update_flags(vm, dr);
        }
        case OP_LEA: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            vm->reg[dr] = vm->reg[R_PC] + pc_offset;
            return update_flags(vm, dr);
        }
        case OP_LDR: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t base_r = (instruction >> 6) & 0x7;
            uint16_t offset = sign_extend(instruction & 0x3F, 6);
            vm->reg[dr] = memory_read(vm, vm->reg[base_r] + offset);
            return update_flags(vm, dr);
        }
        case OP_ST: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            return memory_write(vm, vm->reg[R_PC] + pc_offset, vm->reg[sr]);
        }
        case OP_STI: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            return memory_write(vm, memory_read(vm, vm->reg[R_PC] + pc_offset), vm->reg[sr]);
        }
        case OP_STR: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t base_r = (instruction >> 6) & 0x7;
            uint16_t offset = sign_extend(instruction & 0x3F, 6);
            return memory_write(vm, vm->reg[base_r] + offset, vm->reg[sr]);
        }
        case OP_TRAP: {
            vm->reg[R_R7] = vm->reg[R_PC];

            switch (instruction & 0xFF) {
                case TRAP_GETC:
                    vm->reg[R_R0] = (uint16_t)getchar();
                    return update_flags(vm, R_R0);
                case TRAP_OUT:
                    putc((char)vm->reg[R_R0], stdout);
                    fflush(stdout);
                    return EXIT_SUCCESS;
                case TRAP_PUTS: {
                    uint16_t* c = vm->memory + vm->reg[R_R0];
                    while (*c) {
                        putc((char)*c, stdout);
                        ++c;
                    }
                    fflush(stdout);
                    return EXIT_SUCCESS;
                }
                case TRAP_IN: {
                    printf("Enter a character: ");
                    char c = (char)getchar();
                    putc(c, stdout);
                    fflush(stdout);
                    vm->reg[R_R0] = (uint16_t)c;
                    return update_flags(vm, R_R0);
                }
                case TRAP_PUTSP: {
                    uint16_t* c = vm->memory + vm->reg[R_R0];
                    while (*c) {
                        char char1 = (char)(*c & 0xFF);
                        putc(char1, stdout);

                        char char2 = (char)(*c >> 8);
                        if (char2) {
                            putc(char2, stdout);
                        }
                        ++c;
                    }
                    fflush(stdout);
                    return EXIT_SUCCESS;
                }
                case TRAP_HALT:
                    puts("HALT");
                    fflush(stdout);
                    vm->is_running = 0;
                    return EXIT_SUCCESS;
                default:
                    fprintf(stderr, "Unknown trap code: 0x%02X\n", instruction & 0xFF);
                    return EXIT_FAILURE;
            }
        }
        case OP_RES:
        case OP_RTI:
        default:
            fprintf(stderr, "Unknown opcode: 0x%X\n", opcode(instruction));
            return EXIT_FAILURE;
    }
}

uint16_t opcode(uint16_t instruction) {
    return (instruction >> 12) & 0xF;
}

int update_flags(lc3_vm_t* vm, uint16_t r) {
    if (vm == NULL) {
        perror("Cannot update flags on a NULL register reference");
        return EXIT_FAILURE;
    }

    if (vm->reg[r] == 0) {
        vm->reg[R_COND] = FL_ZRO;
    } else if (vm->reg[r] >> 15) {
        vm->reg[R_COND] = FL_NEG;
    } else {
        vm->reg[R_COND] = FL_POS;
    }

    return EXIT_SUCCESS;
}

/*******************************************************************************
 ********************** PRIVATE FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

static uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}
