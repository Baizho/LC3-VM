#ifndef CONDFLAGS_H
#define CONDFLAGS_H

/*
    * Condition flags stored in the R_COND register.
    * Provides information about the most recently executed calculation.
    * Load instruction (LD, LDR, LDI, and LEA) and operate instructions
    * (ADD, NOT, AND) each load a result into one of the eight general
    * purpose registers.
*/

typedef enum condition_flags {
    FL_POS = 1 << 0, /* P */
    FL_ZRO = 1 << 1, /* Z */
    FL_NEG = 1 << 2, /* N */
} condition_flags_t;

#endif //CONDFLAGS_H