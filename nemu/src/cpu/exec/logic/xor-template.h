#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	cpu.ZF = !result;
	int bits_len = DATA_BYTE << 3;
	cpu.SF = result >> (bits_len - 1);
	cpu.OF = 0;
	cpu.CF = 0;
	result = result ^ (result >> 4);
	result = result ^ (result >> 2);
	result = result ^ (result >> 1);
	cpu.PF = !(result & 1);
	//panic("please implement me");

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
