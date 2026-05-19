#include "opcodes.h"
#include <stdlib.h>
#include <stdio.h>

static uint16_t sign_extend(uint16_t x, int bit_count);

int execute_instruction(uint16_t instruction, uint16_t* memory, uint16_t* reg, uint8_t* end_flag) {
    switch (opcode(instruction)) {
        case OP_ADD: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                reg[dr] = reg[sr1] + imm5;
            } else {
                uint16_t sr2 = instruction & 0x7;
                reg[dr] = reg[sr1] + reg[sr2];
            }

            return update_flags(reg, dr);
        }
        case OP_AND: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            uint16_t imm_flag = (instruction >> 5) & 0x1;

            if (imm_flag) {
                uint16_t imm5 = sign_extend(instruction & 0x1F, 5);
                reg[dr] = reg[sr1] & imm5;
            } else {
                uint16_t sr2 = instruction & 0x7;
                reg[dr] = reg[sr1] & reg[sr2];
            }

            return update_flags(reg, dr);
        }
        case OP_NOT: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t sr1 = (instruction >> 6) & 0x7;
            reg[dr] = (uint16_t)~reg[sr1];
            return update_flags(reg, dr);
        }
        case OP_BR: {
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            uint16_t cond = (instruction >> 9) & 0x7;

            if (cond & reg[R_COND]) {
                reg[R_PC] += pc_offset;
            }

            return EXIT_SUCCESS;
        }
        case OP_JMP: {
            uint16_t base_r = (instruction >> 6) & 0x7;
            reg[R_PC] = reg[base_r];
            return EXIT_SUCCESS;
        }
        case OP_JSR: {
            uint16_t long_flag = (instruction >> 11) & 0x1;
            reg[R_R7] = reg[R_PC];

            if (long_flag) {
                uint16_t pc_offset = sign_extend(instruction & 0x7FF, 11);
                reg[R_PC] += pc_offset;
            } else {
                uint16_t base_r = (instruction >> 6) & 0x7;
                reg[R_PC] = reg[base_r];
            }

            return EXIT_SUCCESS;
        }
        case OP_LD: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            reg[dr] = memory_read(reg[R_PC] + pc_offset, memory);
            return update_flags(reg, dr);
        }
        case OP_LDI: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            reg[dr] = memory_read(memory_read(reg[R_PC] + pc_offset, memory), memory);
            return update_flags(reg, dr);
        }
        case OP_LEA: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            reg[dr] = reg[R_PC] + pc_offset;
            return update_flags(reg, dr);
        }
        case OP_LDR: {
            uint16_t dr = (instruction >> 9) & 0x7;
            uint16_t base_r = (instruction >> 6) & 0x7;
            uint16_t offset = sign_extend(instruction & 0x3F, 6);
            reg[dr] = memory_read(reg[base_r] + offset, memory);
            return update_flags(reg, dr);
        }
        case OP_ST: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            return memory_write(reg[R_PC] + pc_offset, reg[sr], memory);
        }
        case OP_STI: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t pc_offset = sign_extend(instruction & 0x1FF, 9);
            return memory_write(memory_read(reg[R_PC] + pc_offset, memory), reg[sr], memory);
        }
        case OP_STR: {
            uint16_t sr = (instruction >> 9) & 0x7;
            uint16_t base_r = (instruction >> 6) & 0x7;
            uint16_t offset = sign_extend(instruction & 0x3F, 6);
            return memory_write(reg[base_r] + offset, reg[sr], memory);
        }
        case OP_TRAP: {
            reg[R_R7] = reg[R_PC];

            switch (instruction & 0xFF) {
                case TRAP_GETC:
                    reg[R_R0] = (uint16_t)getchar();
                    return update_flags(reg, R_R0);
                case TRAP_OUT:
                    putc((char)reg[R_R0], stdout);
                    fflush(stdout);
                    return EXIT_SUCCESS;
                case TRAP_PUTS: {
                    uint16_t* c = memory + reg[R_R0];
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
                    reg[R_R0] = (uint16_t)c;
                    return update_flags(reg, R_R0);
                }
                case TRAP_PUTSP: {
                    uint16_t* c = memory + reg[R_R0];
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
                    *end_flag = 0;
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

int update_flags(uint16_t* reg, uint16_t r) {
    if (reg == NULL) {
        perror("Cannot update flags on a NULL register reference");
        return EXIT_FAILURE;
    }

    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (reg[r] >> 15) {
        reg[R_COND] = FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
    }

    return EXIT_SUCCESS;
}

static uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}
