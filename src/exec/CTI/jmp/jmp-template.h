#include "exec/template-start.h"

#include "cpu/modrm.h"

make_helper( concat(jmp_i_, SUFFIX) )
{
	uint32_t imm = instr_fetch(eip+1, DATA_BYTE);

	print_asm("jmp" " 0x%x", eip + 1 + DATA_BYTE + imm);

	return 1 + DATA_BYTE + imm;
}

make_helper( concat(jmp_rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);

	if (m.mod != 3)
	{
		swaddr_t addr;
		uint8_t len = read_ModR_M(eip+1, &addr);
		uint32_t mva = MEM_R(addr);

		print_asm("jmp" " %s", ModR_M_asm);

		return 1 + len + mva;
	}
	else
	{
		uint32_t rva = REG(m.R_M);

		print_asm("jmp" " %%%s", REG_NAME(m.R_M));

		return 1 + 1 + rva;
	}
}


#include "exec/template-end.h"
