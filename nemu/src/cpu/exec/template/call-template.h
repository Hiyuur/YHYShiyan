#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_i_,SUFFIX)) {
    int len = concat(decode_i_,SUFFIX) (eip + 1);

}

make_helper(concat(call_rm_,SUFFIX)) {

}




#include "cpu/exec/template-end.h"