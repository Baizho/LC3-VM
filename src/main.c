#include "core/opcodes.h"
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <sys/termios.h>

struct termios original_tio;
static int input_buffering_disabled = 0;

void disable_input_buffering()
{
    if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &original_tio) != 0) {
        return;
    }

    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_tio) == 0) {
        input_buffering_disabled = 1;
    }
}

void restore_input_buffering()
{
    if (input_buffering_disabled) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
        input_buffering_disabled = 0;
    }
}

void handle_interrupt(int signal)
{
    (void)signal;
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

int main(int argc, const char* argv[]) {
    if (argc < 2)
    {
        /* show usage string */
        printf("use it like: lc3 [image-file1] ...\n");
        exit(2);
    }

    lc3_vm_t vm;
    if (lc3_vm_init(&vm) != EXIT_SUCCESS) {
        fprintf(stderr, "failed to initialize LC-3 VM\n");
        return EXIT_FAILURE;
    }
    
    for (int j = 1; j < argc; ++j)
    {
        if (loadfile_from_path(argv[j], &vm) != EXIT_SUCCESS)
        {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    while (vm.is_running) {
        uint16_t instruction = memory_read(&vm, vm.reg[R_PC]++);
        if (execute_instruction(instruction, &vm) != 0) {
            fprintf(stderr, "Error executing instruction\n");
            restore_input_buffering();
            return EXIT_FAILURE;
        }
    }
    fprintf(stdout, "Execution completed!\n");

    restore_input_buffering();
    return EXIT_SUCCESS;
}
