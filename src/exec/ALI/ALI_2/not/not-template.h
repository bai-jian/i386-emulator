#include "exec/template-start.h"


make_helper( concat(not_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		DATA_TYPE opd = MEM_R(mem_a);
		DATA_TYPE val = ~opd;
		MEM_W(mem_a, val);

		print_asm("not"str(SUFFIX)"   %s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		DATA_TYPE opd = REG(reg_i);
		DATA_TYPE val = ~opd;
		REG(reg_i) = val;

		print_asm("not"str(SUFFIX)"   %%%s", REG_NAME(reg_i));

		return 1 + 1;
	}
}


#include "exec/template-end.h"
