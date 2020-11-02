#include "cpu/exec/template-start.h"

#define instr shl

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest <<= count;
	OPERAND_W(op_dest, dest);

	/* TODO: Update EFLAGS. */
	DATA_TYPE_S result = dest;
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

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
