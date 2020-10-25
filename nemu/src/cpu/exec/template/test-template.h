#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
    DATA_TYPE result = op_src->val & op_dest->val;
    cpu.OF = 0;
    cpu.CF = 0;
    cpu.SF = result >> len;
    cpu.ZF = !result;
    result = result ^ (result >> 4);
    result = result ^ (result >> 2);
    result = result ^ (result >> 1);
    cpu.PF = !(result & 1);
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"