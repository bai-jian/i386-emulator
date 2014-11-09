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
#include "ALI/ALI_2/imul/imul.h"
make_helper(tran_mean)
{
	uint8_t inst = instr_fetch(eip+1, 1);
	if ( inst == 0x82 )  return  1 + jb_v(eip + 1);
	if ( inst == 0x83 )  return  1 + jae_v(eip + 1);
	if ( inst == 0x84 )  return  1 + je_v(eip + 1);
	if ( inst == 0x86 )  return  1 + jbe_v(eip + 1); 
	if ( inst == 0x87 )  return  1 + ja_v(eip + 1);
	if ( inst == 0x8c )  return  1 + jl_v(eip + 1);
	if ( inst == 0x8d )  return  1 + jge_v(eip + 1);
	if ( inst == 0x8e )  return  1 + jle_v(eip + 1);
	if ( inst == 0x8f )  return  1 + jg_v(eip + 1);
	if ( inst == 0xaf )  return  1 + imul_rm2r_v(eip + 1);
	assert(0); 	return 0;
}

