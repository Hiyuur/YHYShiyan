#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute() {
    if(ops_decoded.is_operand_size_16) {
        swaddr_write(reg_w(R_AX),2,reg_w(R_SI));
        if(cpu.DF == 0) {
            reg_w(R_SI) += DATA_BYTE;
        }
        else {
            reg_w(R_SI) -= DATA_BYTE;
        }
    }
    else {
        swaddr_write(reg_l(R_EAX),4,reg_l(R_ESI));
        if(cpu.DF == 0) {
            reg_l(R_ESI) += DATA_BYTE;
        }
        else {
            reg_l(R_ESI) -= DATA_BYTE;
        }
    }
    print_asm("lods");
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"