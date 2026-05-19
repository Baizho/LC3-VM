#include "test.h"
#include "core/opcodes.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PRIVATE FUNCTION IMPLEMENTATIONS **********************
 *******************************************************************************/

static uint16_t jump(uint16_t base_r) {
    return (OP_JMP << 12) | (base_r << 6);
}

static uint16_t jsr(uint16_t pc_offset11) {
    return (OP_JSR << 12) | (1 << 11) | (pc_offset11 & 0x7FF);
}

static uint16_t jsrr(uint16_t base_r) {
    return (OP_JSR << 12) | (base_r << 6);
}

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    vm.reg[R_R2] = 0x3100;
    ASSERT_SUCCESS(execute_instruction(jump(R_R2), &vm));
    ASSERT_EQ_U16(0x3100, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3001;
    ASSERT_SUCCESS(execute_instruction(jsr(5), &vm));
    ASSERT_EQ_U16(0x3001, vm.reg[R_R7]);
    ASSERT_EQ_U16(0x3006, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3001;
    ASSERT_SUCCESS(execute_instruction(jsr(0x7FE), &vm));
    ASSERT_EQ_U16(0x3001, vm.reg[R_R7]);
    ASSERT_EQ_U16(0x2FFF, vm.reg[R_PC]);

    vm.reg[R_PC] = 0x3001;
    vm.reg[R_R3] = 0x3200;
    ASSERT_SUCCESS(execute_instruction(jsrr(R_R3), &vm));
    ASSERT_EQ_U16(0x3001, vm.reg[R_R7]);
    ASSERT_EQ_U16(0x3200, vm.reg[R_PC]);

    return EXIT_SUCCESS;
}
