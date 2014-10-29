#include "exec/template-start.h"


make_helper( concat(jmp_i_, SUFFIX) )
{
	uint32_t imm = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jmp" " 0x%x", eip + 1 + DATA_BYTE + imm);

	return 1 + DATA_BYTE + imm;
}


#include "exec/template-end.h"
