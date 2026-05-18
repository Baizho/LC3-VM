#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdint.h>

/* 
    * RAM Memory is emulated through an uint16_t array with size MEMORY_MAX
    * set to the maximum size of the LC-3 System memory hardware (65536 btis)
*/

#define MEMORY_MAX (1 << 16) 
uint16_t memory[MEMORY_MAX];


/*
    * Registers in the LC-3 ISA System. Enumeration is defined with constants 
    * whose numerical constants equal to the binary addresses of the registers
    * in a real LC-3 System.
*/

typedef enum registers {
    R_R0,   /* Bin address: 0000*/
    R_R1,   /* Bin address: 0001*/
    R_R2,   /* Bin address: 0010*/
    R_R3,   /* Bin address: 0011*/
    R_R4,   /* Bin address: 0100*/
    R_R5,   /* Bin address: 0101*/
    R_R6,   /* Bin address: 0110*/
    R_R7,   /* Bin address: 0111 */
    R_PC,
    R_COND,
    R_COUNT
} registers_t;

uint16_t reg[R_COUNT];






#endif // MEMORY_H