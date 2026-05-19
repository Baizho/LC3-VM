#include "test.h"
#include "core/opcodes.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PRIVATE FUNCTION IMPLEMENTATIONS **********************
 *******************************************************************************/

static uint16_t branch(uint16_t cond, uint16_t pc_offset9) {
    return (OP_BR << 12) | (cond << 9) | (pc_offset9 & 0x1FF);
}

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    vm.reg[R_PC] = 0x3001;
    vm.reg[R_COND] = FL_POS;
    ASSERT_SUCCESS(execute_instruction(branch(FL_POS, 3), &vm));
    ASSERT_EQ_U16(0x3004, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3001;
    vm.reg[R_COND] = FL_ZRO;
    ASSERT_SUCCESS(execute_instruction(branch(FL_POS, 3), &vm));
    ASSERT_EQ_U16(0x3001, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3005;
    vm.reg[R_COND] = FL_NEG;
    ASSERT_SUCCESS(execute_instruction(branch(FL_NEG, 0x1FE), &vm));
    ASSERT_EQ_U16(0x3003, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3001;
    vm.reg[R_COND] = FL_ZRO;
    ASSERT_SUCCESS(execute_instruction(branch(FL_NEG | FL_ZRO | FL_POS, 1), &vm));
    ASSERT_EQ_U16(0x3002, vm.reg[R_PC]);

    return EXIT_SUCCESS;
}
