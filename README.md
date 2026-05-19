# LC3-VM

Compact LC-3 virtual machine written in C. This repository provides a small,
easy-to-read LC-3 VM core suitable for learning, testing, and extending the
architecture with tools such as a disassembler, debugger, or assembler.

Highlights
- 65,536-word LC-3 memory.
- VM core state stored in `lc3_vm_t`.
- Instruction execution for the main LC-3 ISA and basic trap routines.
- Object-file loading with origin handling and endian conversion.
- Memory-mapped keyboard support and simple host I/O traps.

## Build

Build the VM:

```sh
make
```

This creates the `lc3` executable in the project root.

Clean build artifacts:

```sh
make clean
```

## Run

Run an LC-3 object image with the VM:

```sh
./lc3 path/to/program.obj
```

Or via Make:

```sh
make run ARGS="resources/2048.obj"
```

## Tests

This repo includes unit-style test drivers that exercise opcode and memory
behavior. Build and run all tests with:

```sh
make test
```

The Makefile builds small test binaries into `tests/bin/` such as:

- `tests/bin/test_add`
- `tests/bin/test_branch`
- `tests/bin/test_jump`
- `tests/bin/test_load_store`
- `tests/bin/test_logic`
- `tests/bin/test_memory`

Each test driver compiles the core VM sources together with a small test
program located in `tests/` and returns non-zero on failure. See
[tests/test.h](tests/test.h) for the lightweight assertion macros used.

Running tests locally:

```sh
make test
# or run a single test
./tests/bin/test_add
```

## Project Layout

```text
.
|-- Makefile
|-- README.md
|-- resources/
|   |-- 2048.obj
|   `-- helloworld.lc3
|-- src/
|   |-- main.c
|   `-- core/
|       |-- condflags.h
|       |-- memory.c
|       |-- memory.h
|       |-- opcodes.c
|       |-- opcodes.h
|       |-- vm.c
|       `-- vm.h
`-- tests/
    |-- test_add.c
    |-- test_branch.c
    |-- test_jump.c
    |-- test_load_store.c
    |-- test_logic.c
    |-- test_memory.c
    |-- test.h
    `-- fixtures/
```

## Goals

These are my goals for the project. They're listed roughly in priority order so I can focus work where it matters most.

1. VM core cleanup
   - Make the core fully modular (`vm.h`, `memory.h`, `instruction.h`,
     `traps.h`) so multiple VM instances, cleaner interfaces, and better tests
     are easy to write.
2. Disassembler
   - Add a small `lc3-disasm` tool that prints addresses, words, and assembly
     mnemonics to aid debugging and inspection of binary images.
3. Debugger / REPL
   - Add `lc3dbg` with basic commands: `step`, `continue`, `regs`, `mem`,
     `break`, and `disasm` to enable interactive debugging and learning.
4. Assembler (optional)
   - Implement a simple two-pass assembler supporting `.ORIG`, `.END`,
     `.FILL`, `.BLKW`, and `.STRINGZ` so I can write and assemble LC-3 programs
     within this project.
5. Tiny LC-3 monitor/OS
   - Implement LC-3-side monitor code and trap-based runtime services (simple
     I/O, file abstractions, virtual disk area) to experiment with OS-like
     features inside the emulated environment.
6. UX/UI (optional)
   - Consider a TUI (ncurses) or a small web-based UI only after core features
     and tests are solid.

My immediate focus order will be: VM core cleanup, disassembler, debugger/REPL,
then assembler, and later a monitor/OS. GUI work is optional and lower
priority.

## Contributing

Contributions are welcome. To propose changes:

1. Fork the repo and create a feature branch.
2. Add or update tests in `tests/` for any behavioral change.
3. Open a PR describing the change and the rationale.

## Notes

The LC-3 architecture is commonly used for learning about instruction
execution and low-level programming. This implementation is compact and
focused on the VM core so it should be straightforward to extend.