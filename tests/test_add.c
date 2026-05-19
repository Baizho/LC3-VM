#include "test.h"
#include "core/opcodes.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PRIVATE FUNCTION IMPLEMENTATIONS **********************
 *******************************************************************************/

static uint16_t add_imm(uint16_t dr, uint16_t sr1, uint16_t imm5) {
    return (OP_ADD << 12) | (dr << 9) | (sr1 << 6) | (1 << 5) | (imm5 & 0x1F);
}

static uint16_t add_reg(uint16_t dr, uint16_t sr1, uint16_t sr2) {
    return (OP_ADD << 12) | (dr << 9) | (sr1 << 6) | (sr2 & 0x7);
}

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    vm.reg[R_R1] = 7;
    ASSERT_SUCCESS(execute_instruction(add_imm(R_R2, R_R1, 5), &vm));
    ASSERT_EQ_U16(12, vm.reg[R_R2]);
    ASSERT_EQ_U16(FL_POS, vm.reg[R_COND]);

    vm.reg[R_R0] = 0;
    ASSERT_SUCCESS(execute_instruction(add_imm(R_R0, R_R0, 0), &vm));
    ASSERT_EQ_U16(0, vm.reg[R_R0]);
    ASSERT_EQ_U16(FL_ZRO, vm.reg[R_COND]);

    vm.reg[R_R3] = 0;
    ASSERT_SUCCESS(execute_instruction(add_imm(R_R3, R_R3, 0x1F), &vm));
    ASSERT_EQ_U16(0xFFFF, vm.reg[R_R3]);
    ASSERT_EQ_U16(FL_NEG, vm.reg[R_COND]);

    vm.reg[R_R4] = 10;
    vm.reg[R_R5] = 20;
    ASSERT_SUCCESS(execute_instruction(add_reg(R_R6, R_R4, R_R5), &vm));
    ASSERT_EQ_U16(30, vm.reg[R_R6]);
    ASSERT_EQ_U16(FL_POS, vm.reg[R_COND]);

    return EXIT_SUCCESS;
}
