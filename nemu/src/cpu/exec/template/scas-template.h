#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute() {

        uint32_t temp = reg_l(R_EAX) - swaddr_read(reg_l(R_EDI),4);
        cpu.ZF = !temp;
        int bits_len = DATA_BYTE << 3;
        cpu.SF = temp >> (bits_len - 1);
        cpu.CF = reg_w(R_EAX) < swaddr_read(reg_w(R_EDI),4);
        int temp1 = reg_w(R_EAX) >> (bits_len - 1);
        int temp2 = swaddr_read(reg_w(R_EDI),4) >> (bits_len - 1);
        cpu.OF = (temp1 != temp2 && temp2 == cpu.SF);
        temp = temp ^ (temp >> 4);
        temp = temp ^ (temp >> 2);
        temp = temp ^ (temp >> 1);
        cpu.PF = !(temp & 1);
        if(cpu.DF == 0) {
            reg_l(R_EDI) += DATA_BYTE;
        }
        else {
            reg_l(R_EDI) -= DATA_BYTE;
        }
    print_asm("scas");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"


