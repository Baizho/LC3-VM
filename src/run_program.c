#include "core/instructionset/opcodes.h"
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

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
    
    for (int j = 1; j < argc; ++j)
    {
        if (loadfile_from_path(argv[j], memory) != EXIT_SUCCESS)
        {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    /* since exactly one condition flag should be set at any given time, set the Z flag */
    reg[R_COND] = FL_ZRO;

    /* set the PC to starting position */
    /* 0x3000 is the default */
    enum { PC_START = 0x3000 };
    reg[R_PC] = PC_START;

    uint8_t is_running = 1;
    while(is_running) {
        uint16_t instruction = memory_read(reg[R_PC] ++, memory);
        if (execute_instruction(instruction, memory, reg, &is_running) != 0) {
            fprintf(stderr, "Error executing instruction\n");
            restore_input_buffering();
            return EXIT_FAILURE;
        }
    }
    fprintf(stdout, "Execution completed!\n");

    restore_input_buffering();
    return EXIT_SUCCESS;
}
