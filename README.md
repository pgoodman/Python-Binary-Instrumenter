Prototype of the Granary-Based Concurrent GC
============================================

This is a user space prototype for developing some of the concepts that will be
needed to implement our GC.

State Space
-----------

  * In kernel code, with no module activation frame(s) on the stack.
  * In kernel code, with module activation frame(s) on the stack. This case
    requires acknowledging that while a snapshot might be taken, the kernel
    code return into module code.
  * At the beginning of a basic block.
  * In the middle of a basic block.
  * At the end of a basic block.
  * Between two basic blocks (e.g. IBL, dispatcher, etc.).
