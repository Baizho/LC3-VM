#include "vm.h"
#include "instructionset/condflags.h"
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 *********************** PUBLIC FUNCTION IMPLEMENTATIONS ***********************
 *******************************************************************************/

int lc3_vm_init(lc3_vm_t* vm) {
    if (vm == NULL) {
        return EXIT_FAILURE;
    }

    memset(vm, 0, sizeof(*vm));

    /* since exactly one condition flag should be set at any given time, set the Z flag */
    vm->reg[R_COND] = FL_ZRO;

    /* set the PC to starting position */
    /* 0x3000 is the default */
    enum { PC_START = 0x3000 };
    vm->reg[R_PC] = PC_START;

    vm->is_running = 1;
    return EXIT_SUCCESS;
}
