#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
    DATA_TYPE temp = op_src->val;
    if(op_src->size == 1 && op_dest->size != 1) {
        op_src->val = (int8_t)op_src->val;//change into signed
    }
    temp += cpu.CF;
    DATA_TYPE result = op_dest->val - temp;
    int bits_len = DATA_BYTE << 3;
    cpu.ZF = !result;
    if(op_dest->val < temp) {
        cpu.CF = 1;
    }
    else {
        cpu.CF = 0;
    }
    cpu.SF = result >> (bits_len - 1);
    int flag1 = op_dest->val >> (bits_len - 1);
    int flag2 = temp >> (bits_len - 1);
    if(flag1 != flag2 && flag2 == cpu.SF) {
        cpu.OF = 1;
    }
    else {
        cpu.OF = 0;
    }
    DATA_TYPE temp2 = result;
    temp2 = temp2 ^ (temp2 >> 4);
    temp2 = temp2 ^ (temp2 >> 2);
    temp2 = temp2 ^ (temp2 >> 1);
    cpu.PF = !(temp2 & 1);
    OPERAND_W(op_dest,result);
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