#ifndef OPCODES_H
#define OPCODES_H

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

#endif // OPCODES_H