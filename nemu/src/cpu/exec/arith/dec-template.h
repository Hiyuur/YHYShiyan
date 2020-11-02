#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	cpu.ZF = !result;
	int bits_len = DATA_BYTE << 3;
	cpu.SF = result >> (bits_len - 1);
	if(op_src->val < 1) {
		cpu.CF = 1;
	}
	else {
		cpu.CF = 0;
	}
	int temp1 = op_src->val >> (bits_len - 1);
	int temp2 = 0;
	if(temp1 != temp2 && temp2 == cpu.SF) {
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
