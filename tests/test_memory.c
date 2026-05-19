#include "test.h"
#include "core/condflags.h"
#include "core/memory.h"
#include "core/vm.h"
#include <stdlib.h>

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int main(void) {
    lc3_vm_t vm;
    ASSERT_SUCCESS(lc3_vm_init(&vm));

    ASSERT_EQ_U16(0x3000, vm.reg[R_PC]);
    ASSERT_EQ_U16(FL_ZRO, vm.reg[R_COND]);
    ASSERT_EQ_U16(1, vm.is_running);

    ASSERT_SUCCESS(memory_write(&vm, 0x3000, 0x1234));
    ASSERT_EQ_U16(0x1234, memory_read(&vm, 0x3000));

    ASSERT_SUCCESS(loadfile_from_path("tests/fixtures/simple.obj", &vm));
    ASSERT_EQ_U16(0x1021, memory_read(&vm, 0x3000));
    ASSERT_EQ_U16(0xF025, memory_read(&vm, 0x3001));

    return EXIT_SUCCESS;
}
