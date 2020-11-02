#include "cpu/exec/template-start.h"

#define instr popa

#if DATA_BYTE == 2
static void pop(int n)
{
	if(n != 4) {
        reg_l(n) = swaddr_read(reg_l(R_ESP),4);
    }
	swaddr_write (reg_l(R_ESP),4,0);
	reg_l(R_ESP) += 4;
}
#endif

static void do_execute() {
	int i;
	for(i = 7;i >= 0;i--){
		pop(i);
	}
	print_asm("popa");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"