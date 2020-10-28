#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
    DATA_TYPE temp = op_dest->val - op_src->val;
    cpu.ZF = !temp;
    if(op_src->val > op_dest->val) {
        cpu.CF = 1;
    }
    else {
        cpu.CF = 0;
    }
    int bits_len = DATA_TYPE << 3;
    cpu.SF = temp >> (bits_len - 1);
    int flag1 = op_dest->val >> (bits_len - 1);
    int flag2 = op_src-> val >> (bits_len - 1);
    if( flag1 != flag2 && flag2 == cpu.SF) { //signed overflow
        cpu.OF = 1;
    }
    else {
        cpu.OF = 0;
    }
    temp = temp ^ (temp >> 4);
    temp = temp ^ (temp >> 2);
    temp = temp ^ (temp >> 1);
    cpu.PF = !(temp & 1);
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"