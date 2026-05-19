# LC3-VM

Implementation of a LC3 Virtual Machine, with a simple Operating System, based on https://www.jmeiners.com/lc3-vm/.

## Build and Run

Build the VM:

```sh
make
```

Run an LC-3 object image:

```sh
./lc3 path/to/program.obj
```

You can also pass arguments through `make run`:

```sh
make run ARGS="path/to/program.obj"
```

Remove the compiled binary:

```sh
make clean
```

# Current Goals

First of all -> Implement the LC3 Virtual Machine.

Then add additional functionality, inspirations will be from:

https://github.com/Samuele95/LC3VM

because it looks cool.
