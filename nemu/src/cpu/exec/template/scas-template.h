#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {
    if(ops_decoded.is_operand_size_16) {
        uint32_t temp = reg_w(R_AX) - swaddr_read(reg_w(R_DI),4);
        cpu.ZF = !temp;
        if(cpu.DF == 0) {
            reg_w(R_DI) += DATA_BYTE;
        }
        else {
            reg_w(R_DI) -= DATA_BYTE;
        }
    }
    else {
        uint32_t temp = reg_l(R_EAX) - swaddr_read(reg_l(R_EDI),4);
        cpu.ZF = !temp;
        if(cpu.DF == 0) {
            reg_l(R_EDI) += DATA_BYTE;
        }
        else {
            reg_l(R_EDI) -= DATA_BYTE;
        }
    }
    print_asm("scas");
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"