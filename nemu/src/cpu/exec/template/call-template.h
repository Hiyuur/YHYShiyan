#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_i_,SUFFIX)) {
    int len = concat(decode_i_,SUFFIX) (cpu.eip + 1);//decode_i_SUFFIX is a function, (eip+1)//opcode is argument.
    reg_l(R_ESP) -= DATA_BYTE;//stack_top pointer
    swaddr_write(reg_L(R_ESP),4,cpu.eip + len);
    DATA_TYPE_S displacement = op_src->val;
    print_asm("call%x",cou.eip + len + displacement + 1);
    cpu.eip += displament;
    return len + 1;
}

make_helper(concat(call_rm_,SUFFIX)) {

}




#include "cpu/exec/template-end.h"