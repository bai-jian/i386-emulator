#include "template-start.h"
#include "modrm.h"

make_helper( concat(xchg_r2r0_, SUFFIX) )
{
	uint8_t   reg_i = instr_fetch(eip, 1) & 0x07;

	DATA_TYPE reg_v = REG(reg_i);
	REG(reg_i) = REG(0);
	REG(0) = reg_v;;

	print_asm("xchg" str(SUFFIX) " %%%s", REG_NAME(reg_i));

	return 1;
}

make_helper( concat(xchg_r2rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		uint8_t reg_i = m.reg;

		DATA_TYPE mem_v = MEM_R(mem_i);
		MEM_W(mem_i, REG(reg_i));
		REG(reg_i) = mem_v;

		print_asm("xchg" str(SUFFIX) " %%%s,%s", REG_NAME(reg_i), ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t s_reg_i = m.reg;
		uint8_t d_reg_i = m.R_M;

		DATA_TYPE s_reg_v = REG(s_reg_i);
		REG(s_reg_i) = REG(d_reg_i);
		REG(d_reg_i) = s_reg_v;

		print_asm("xchg" str(SUFFIX) " %%%s,%%%s", REG_NAME(s_reg_i), REG_NAME(d_reg_i));

		return 1 + 1;
	}
}


#include "template-end.h"
