#include "cpu/exec/template-start.h"

#define instr cltd

static void do_execute () {
    DATA_TYPE bits_len = DATA_BYTE << 3;
	DATA_TYPE temp = (1 << bits_len) - 1;
	if(DATA_BYTE == 2) {
        if((REG(R_EAX >> 15) == 1)) {
            REG(R_EDX) = temp;
        }
        else {
            REG(R_EDX) = 0;
        }
	}
	else if(DATA_BYTE == 4) {
         if((REG(R_EAX >> 31) == 1)) {
            REG(R_EDX) = temp;
        }
        else {
            REG(R_EDX) = 0;
        }	
	}
	print_asm("cltd");
}

make_instr_helper(n)


#include "cpu/exec/template-end.h"
