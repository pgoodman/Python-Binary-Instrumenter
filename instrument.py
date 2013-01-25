

# Returns the last instruction in the basic block, along with
# its index. Note: the last instruction in a basic block is not
# necessarily the last item in the instr_list.
def get_last_instruction(instr_list):
    for i, line in enumerate(reversed(instr_list)):
        if line.startswith(".") \
        or line.startswith("_") \
        or line.startswith("#"):
            continue
        return len(instr_list) - i - 1, line


# Return true iff an instruction is a CTI.
def is_cti(instr):
    if "call" in instr or "ret" in instr:
        return True
    elif instr.startswith("j"):
        return True
    return False


# Add some instructions to the beginning of a basic block
def add_to_beginning(instr_list, *instrs):
    for instr in reversed(instrs):
        instr_list.insert(0, instr)


# Add some instructions to the end of a basic block (before any
# CTI that ends the basic block).
def add_to_end(instr_list, *instrs):
    i, last = get_last_instruction(instr_list)
    if not is_cti(last):
        i += 1

    for instr in reversed(instrs):
        instr_list.insert(i, instr)


# Visit the instructions of a basic block. This needs to return a
# list of instructions in the instrumented block.
def visit_basic_block(instr_list):
    add_to_beginning(instr_list,
        "push %rax",
        "pop %rax")

    add_to_end(instr_list,
        "push %rbp",
        "pop %rbp")

    return instr_list