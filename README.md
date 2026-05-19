# LC3-VM

A small LC-3 virtual machine written in C.

This project implements the first complete milestone of an LC-3 emulator: loading
LC-3 object files, executing instructions, maintaining VM memory/register state,
handling condition flags, and supporting the standard trap routines needed by
basic LC-3 programs.

It does not include a custom operating system yet. For now, the VM runs LC-3
program images directly.

## Current Status

Step 1 is complete: the VM builds and can run LC-3 object programs such as the
included `2048.obj` demo.

Implemented pieces:

- 65,536-word LC-3 memory.
- VM core state stored in `lc3_vm_t`.
- General-purpose registers, program counter, and condition register.
- Object file loading with LC-3 origin handling.
- Instruction execution for the main LC-3 instruction set.
- Memory-mapped keyboard registers.
- Trap routines: `GETC`, `OUT`, `PUTS`, `IN`, `PUTSP`, and `HALT`.
- A simple Makefile-based build.

Not implemented yet:

- A custom LC-3 operating system.
- An assembler.
- A debugger or instruction trace mode.
- Automated test suite.
- Privileged `RTI` behavior.

## Build

Build the VM:

```sh
make
```

This creates the `lc3` executable in the project root.

## Run

Run an LC-3 object image:

```sh
./lc3 path/to/program.obj
```

For example, run the included 2048 demo:

```sh
./lc3 resources/2048.obj
```

You can also run through Make:

```sh
make run ARGS="resources/2048.obj"
```

Clean the compiled binary:

```sh
make clean
```

## Project Layout

```text
.
|-- Makefile
|-- README.md
|-- resources/
|   |-- 2048.obj
|   `-- helloworld.lc3
`-- src/
    |-- main.c
    `-- core/
        |-- condflags.h
        |-- memory.c
        |-- memory.h
        |-- opcodes.c
        |-- opcodes.h
        |-- vm.c
        `-- vm.h
```

## Next Steps

The next natural improvements are:

- Add small repeatable VM tests for each opcode.
- Add a trace/debug mode for inspecting `PC`, registers, and instructions.
- Add a tiny assembler workflow or document which external assembler to use.
- Start designing a minimal LC-3 operating-system layer once the VM core is
  easier to test.

## Notes

The LC-3 architecture is commonly used for learning how computers execute
instructions at a low level. This implementation is intentionally compact and
focused on the VM core, so it should be easy to read, modify, and extend.
