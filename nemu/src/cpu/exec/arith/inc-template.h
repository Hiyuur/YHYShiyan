#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	cpu.ZF = !result;
	int bits_len = DATA_BYTE << 3;
	cpu.SF = result >> (bits_len - 1);
	if(result < op_dest->val) {
		cpu.OF = 1;
	}
	else {
		cpu.OF = 0;
	}
	result = result ^ (result >> 4);
	result = result ^ (result >> 2);
	result = result ^ (result >> 1);
	cpu.PF = !(result & 1);
	//panic("please implement me");

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
