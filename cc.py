import sys
import re
import subprocess
import instrument

ASM_FILE_NAME = re.compile(r"([a-zA-Z.0-9_/-]+)\.S")
BASIC_BLOCK = re.compile(r"#\s*BB(.*):(.*)")
FUNC_CALL = re.compile(r"\s*call(.*)")
FUNC_RETURN = re.compile(r"\s*ret(.*)")

def instrument_assembly(in_lines, out_file):
    global BASIC_BLOCK, FUNC_CALL, FUNC_RETURN
    out_line_groups = [[]]
    basic_block_groups = []
    in_bb = False
    for line in in_lines:
        line = line.strip("\r\n\t ")

        if not line:
            continue

        # beginning and end of procedures; these logically delineate
        # basic blocks
        if ".cfi_endproc" in line \
        or ".cfi_startproc" in line:
            out_line_groups.append([])
            out_line_groups[-1].append(line)

        # this is a function call; we will see it as something that ends
        # a basic block, but where the call instructions are part of the
        # basic block being ended.
        elif FUNC_CALL.match(line):
            out_line_groups[-1].append(line)
            basic_block_groups.append(len(out_line_groups))
            out_line_groups.append([])

        # this is a function return; we will see it as ending a basic block,
        # and expect that any instructions reachable from within the current
        # procedure but following the return will be marked appropriately with
        # the `# BB ...` form.
        elif FUNC_RETURN.match(line):
            out_line_groups[-1].append(line)
            out_line_groups.append([])
        
        # beginning of a basic block; remember the index of this
        # group of instructions as a basic block for later 
        # instrumenting
        elif BASIC_BLOCK.match(line):
            out_line_groups.append([line])
            basic_block_groups.append(len(out_line_groups))
            out_line_groups.append([])

        # append this line / instruction to the last line group
        else:
            out_line_groups[-1].append(line)

    # visit each basic block
    for group in basic_block_groups:
        out_line_groups[group] = instrument.visit_basic_block(
            out_line_groups[group])

    # output the instrumented program
    for group in out_line_groups:
        for line in group:
            out_file.write("%s\n" % line)

if "__main__" == __name__:
    new_cmd = "clang++ -S " + " ".join(sys.argv[1:])
    
    # this command is not putting together the binary
    assemble_file = False
    if ".out" not in new_cmd:
        assemble_file = True
        new_cmd = new_cmd.replace(".o", ".S")

    # convert the 
    p = subprocess.Popen(new_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in p.stdout.readlines():
        print line,
    ret_val = p.wait()

    # there was an error, or we don't need to assemble the file.
    if 0 != ret_val or not assemble_file:
        sys.exit(ret_val)

    # process the just-assembled file
    asm_file = ASM_FILE_NAME.search(new_cmd)
    assert asm_file
    file_no_ext = asm_file.group(1)

    instrument_assembly(
        open("%s.S" % file_no_ext, "r"),
        open("%s.pp.S" % file_no_ext, "w"))

    # go compile the assembled file
    p = subprocess.Popen(
        "clang++ -c %s.pp.S -o %s.o" % (file_no_ext, file_no_ext), 
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)

    for line in p.stdout.readlines():
        print line,

    ret_val = p.wait()
    sys.exit(ret_val)