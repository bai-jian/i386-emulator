#include "exec/helper.h"

#include "nemu.h"

int exec(swaddr_t);

char suffix = 'l';
make_helper(data_size) 
{
	suffix = 'w';
	int instr_len = exec(eip + 1);
	suffix = 'l';
	return instr_len + 1;
}


#include "CTI/jcc/jcc.h"
make_helper(tran_mean)
{
	uint8_t inst = instr_fetch(eip+1, 1);
	if ( inst == 0x86)  return  1 + jbe_l(eip + 1); 
	
	assert(0); 	return 0;
}

