#include "cpu/exec/template-start.h"

#define instr pusha

#if DATA_BYTE == 2
static void pushA(int n) {
	reg_w(R_SP) -= 2;
	swaddr_write(reg_w(R_SP),2,n);
}
static void pushAD(int n) {
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP),4,n);
}
#endif

static void do_execute() {

	if (DATA_BYTE == 2) {
		DATA_TYPE temp = reg_w(R_SP);
		pushA(reg_w(R_AX));
		pushA(reg_w(R_CX));
		pushA(reg_w(R_DX));
		pushA(reg_w(R_BX));
		pushA(temp);
		pushA(reg_w(R_BP));
		pushA(reg_w(R_SI));
		pushA(reg_w(R_DI));
	}
	else {
		DATA_TYPE temp = reg_l(R_ESP);
		pushAD(reg_l(R_EAX));
		pushAD(reg_l(R_ECX));
		pushAD(reg_l(R_EDX));
		pushAD(reg_l(R_EBX));
		pushAD(temp);
		pushAD(reg_l(R_EBP));
		pushAD(reg_l(R_ESI));
		pushAD(reg_l(R_EDI));
	}
	print_asm_template1();
}

make_instr_helper(n)


#include "cpu/exec/template-end.h"