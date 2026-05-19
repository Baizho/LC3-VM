#include "test.h"
#include "core/opcodes.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PRIVATE FUNCTION IMPLEMENTATIONS **********************
 *******************************************************************************/

static uint16_t and_imm(uint16_t dr, uint16_t sr1, uint16_t imm5) {
    return (OP_AND << 12) | (dr << 9) | (sr1 << 6) | (1 << 5) | (imm5 & 0x1F);
}

static uint16_t and_reg(uint16_t dr, uint16_t sr1, uint16_t sr2) {
    return (OP_AND << 12) | (dr << 9) | (sr1 << 6) | (sr2 & 0x7);
}

static uint16_t not_reg(uint16_t dr, uint16_t sr) {
    return (OP_NOT << 12) | (dr << 9) | (sr << 6) | 0x3F;
}

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    vm.reg[R_R1] = 0x00FF;
    ASSERT_SUCCESS(execute_instruction(and_imm(R_R2, R_R1, 0x0F), &vm));
    ASSERT_EQ_U16(0x000F, vm.reg[R_R2]);
    ASSERT_EQ_U16(FL_POS, vm.reg[R_COND]);

    vm.reg[R_R3] = 0x00F0;
    vm.reg[R_R4] = 0x0F00;
    ASSERT_SUCCESS(execute_instruction(and_reg(R_R5, R_R3, R_R4), &vm));
    ASSERT_EQ_U16(0x0000, vm.reg[R_R5]);
    ASSERT_EQ_U16(FL_ZRO, vm.reg[R_COND]);

    vm.reg[R_R6] = 0x0000;
    ASSERT_SUCCESS(execute_instruction(not_reg(R_R7, R_R6), &vm));
    ASSERT_EQ_U16(0xFFFF, vm.reg[R_R7]);
    ASSERT_EQ_U16(FL_NEG, vm.reg[R_COND]);

    return EXIT_SUCCESS;
}
