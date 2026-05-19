#ifndef OPCODES_H
#define OPCODES_H

#include "../memory/memory.h"
#include "condflags.h"
#include <stdint.h>

/*
    * Each instruction in the LC-3 System has an opcode which indicates 
    * which task to perform and a set of parameters which provides input
    * to the task being performed.
    * There are 16 opcodes in LC-3, each instruction is 16-bits with the
    * left 4 bits storing the opcode (the rest are for the parameters)
*/
typedef enum opcodes {
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
} opcodes_t;

enum
{
    TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
    TRAP_OUT = 0x21,   /* output a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25   /* halt the program */
};

/*
    * Executes the given instruction as loaded in the Lc-3 System memory.
    * @param uint16_t instruction: the 16-bit LC-3 ISA instruction
    * @param uint16_t* memory: the LC-3 emulated memory
    * @param uint16_t* reg: the LC-3 emulated registers
    * @param uint8_t*: pointer to the flag signaling the end of the running program
    * @return: EXIT_SUCCESS if the operation is successful, EXIT_FAILURE otherwise
*/
int execute_instruction(uint16_t instruction, uint16_t* memory, uint16_t* reg, uint8_t*);

/*
    * Parse the OPCODE from the given instruction, extracting it from the 4 most significant bits
    * of the binary representation of the instruction.
    * LC-3 ISA is a big-endian system, so the OPCODE is placed on the left.
    * @param uint16_t instruction: the 16-bit LC-3 ISA instruction 
    * @return the OPCODE of the given instruction
*/
uint16_t opcode(uint16_t instruction);

/* 
    * Sets the condition flags defined in condflags.h after the execution of certain instruction.
    * @param uint16_t* reg: pointer to the LC-3 emulated registers
    * @param uint16_t r: address of the registry whose value must be checked and evaluated to perform the flags' update.
*/
int update_flags(uint16_t* reg, uint16_t r);

#endif // OPCODES_H
