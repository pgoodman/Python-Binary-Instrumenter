Prototype of Python Binary Instrumenter
=======================================

This is a user space prototype for doing simple binary instrumentation with Python. This prototype
works by compiling some code to assembly with `clang`. `clang` annotates the generated assembly with
basic block information. A python script then separates the basic blocks, and exposes them as lists
of strings (assembly instructions or assembly directives). The python script then modifies these
instruction lists, and spits out modified (instrumented) lists of instructions, for compiling by
`clang`.
