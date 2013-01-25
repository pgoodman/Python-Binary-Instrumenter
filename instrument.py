
import pprint

# Returns the last instruction in the basic block, along with
# its index. Note: the last instruction in a basic block is not
# necessarily the last item in the instr_list.
def get_last_instruction(instr_list):
	for i, line in enumerate(reversed(instr_list)):
		if line.startswith(".") or line.startswith("_"):
			continue
		return len(instr_list) - i, line

def visit_basic_block(instr_list):
	pprint.pprint(instr_list)
	print get_last_instruction(instr_list)
	print

	return instr_list