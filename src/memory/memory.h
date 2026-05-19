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

/*
    * Memory Mapped Registers - commonly used to interact with special hardware devices. 
    * LC-3 has 2 memory mapped registers to be implemented - the keyboard status register KBSR 
    * which indicates whether a key has been pressed and the keyboard data register KBDR which 
    * indicates which key was pressed.
    * Alternative to request keyboard input using GETC because that blocks execution until input is received
    * while KBSR and KBDR allows you to the poll the state of the device and continue execution, so program
    * stays responsive while waiting for input
*/
enum
{
    MR_KBSR = 0xFE00, /* keyboard status */
    MR_KBDR = 0xFE02  /* keyboard data */
};

/*
    * Returns the uint16_t data from the memory location at parameter "address"
    * It should handle the case when accessing memory mapped registers - we can't read or write
    * to the memory array directly, so we call this getter function.
    * @param address: uint16_t value which contains the address we want to read the data from.
    * @param memory: the LC-3 System memory
    * @return the uint16_t instruction or data read from memory
*/
uint16_t memory_read(uint16_t address, uint16_t* memory);

/*
    * Writes instruction or data (encoded as uint16_t type) into the memory address 'address'
    * @param address: memory address/offset in which the instruction/data is written
    * @param val: the actualy value to write to the memory 
    * @param memory: the LC-3 System memory
    @return EXIT_SUCCESS if successfuly written the data, EXIT_FAILURE otherwise
*/
int memory_write(uint16_t address, uint16_t val, uint16_t* memory);

/*
    * Loads the given FILE stream into memory. File must be .obj file representing LC-3 program.
    * FILE stream is read line-by-line, every instruction is saved in a corresponding address.
    * In LC-3 ISA architecture, the first line of the program is the "origin" field containing the
    * memory address value from which the program must be stored.
    * In little-endian host architecture, we must swap to big-endian structure which LC-3 supports only.

    * @param file: the file to be read
    * @param memory: the LC-3 memory
    * @return EXIT_SUCCESS if successful, otherwise EXIT_FAILURE
*/
int loadfile(FILE* file, uint16_t* memory);

/*
    * Opens FILE stream from given path and loads the stream into memory as in the loadfile method.
    * @param image_path: the LC-3 program file path
    * @param memory: the LC-3 system memory
    * @return EXIT_SUCCESS if the operation is successful, EXIT_FAILURE if an error occurred into
    * opening the stream from the path, in loading the stream into memory or if the file
    * path was null
*/
int loadfile_from_path(const char* image_path, uint16_t* memory);

#endif // MEMORY_H