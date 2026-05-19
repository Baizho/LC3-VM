#include "test.h"
#include "core/opcodes.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PRIVATE FUNCTION IMPLEMENTATIONS **********************
 *******************************************************************************/

static uint16_t op_dr_offset9(uint16_t op, uint16_t dr, uint16_t offset9) {
    return (op << 12) | (dr << 9) | (offset9 & 0x1FF);
}

static uint16_t op_sr_offset9(uint16_t op, uint16_t sr, uint16_t offset9) {
    return (op << 12) | (sr << 9) | (offset9 & 0x1FF);
}

static uint16_t op_dr_base_offset6(uint16_t op, uint16_t dr, uint16_t base_r, uint16_t offset6) {
    return (op << 12) | (dr << 9) | (base_r << 6) | (offset6 & 0x3F);
}

static uint16_t op_sr_base_offset6(uint16_t op, uint16_t sr, uint16_t base_r, uint16_t offset6) {
    return (op << 12) | (sr << 9) | (base_r << 6) | (offset6 & 0x3F);
}

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    vm.reg[R_PC] = 0x3001;
    vm.memory[0x3003] = 0x1234;
    ASSERT_SUCCESS(execute_instruction(op_dr_offset9(OP_LD, R_R1, 2), &vm));
    ASSERT_EQ_U16(0x1234, vm.reg[R_R1]);
    ASSERT_EQ_U16(FL_POS, vm.reg[R_COND]);

    vm.reg[R_PC] = 0x3001;
    vm.reg[R_R2] = 0xABCD;
    ASSERT_SUCCESS(execute_instruction(op_sr_offset9(OP_ST, R_R2, 3), &vm));
    ASSERT_EQ_U16(0xABCD, vm.memory[0x3004]);

    vm.reg[R_PC] = 0x3001;
    vm.memory[0x3005] = 0x3100;
    vm.memory[0x3100] = 0xFFFF;
    ASSERT_SUCCESS(execute_instruction(op_dr_offset9(OP_LDI, R_R3, 4), &vm));
    ASSERT_EQ_U16(0xFFFF, vm.reg[R_R3]);
    ASSERT_EQ_U16(FL_NEG, vm.reg[R_COND]);

    vm.reg[R_PC] = 0x3001;
    vm.memory[0x3006] = 0x3101;
    vm.reg[R_R4] = 0x2222;
    ASSERT_SUCCESS(execute_instruction(op_sr_offset9(OP_STI, R_R4, 5), &vm));
    ASSERT_EQ_U16(0x2222, vm.memory[0x3101]);

    vm.reg[R_PC] = 0x3001;
    ASSERT_SUCCESS(execute_instruction(op_dr_offset9(OP_LEA, R_R5, 7), &vm));
    ASSERT_EQ_U16(0x3008, vm.reg[R_R5]);
    ASSERT_EQ_U16(FL_POS, vm.reg[R_COND]);

    vm.reg[R_R6] = 0x3200;
    vm.memory[0x3202] = 0x4444;
    ASSERT_SUCCESS(execute_instruction(op_dr_base_offset6(OP_LDR, R_R0, R_R6, 2), &vm));
    ASSERT_EQ_U16(0x4444, vm.reg[R_R0]);

    vm.reg[R_R6] = 0x3200;
    vm.reg[R_R1] = 0x5555;
    ASSERT_SUCCESS(execute_instruction(op_sr_base_offset6(OP_STR, R_R1, R_R6, 3), &vm));
    ASSERT_EQ_U16(0x5555, vm.memory[0x3203]);

    return EXIT_SUCCESS;
}
