#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"


make_helper( concat(cmove_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;  uint8_t len = read_ModR_M(eip + 1, &mem_a);
		if (cpu.ZF)  REG(m.reg) = MEM_R(mem_a);
		print_asm("cmove"str(SUFFIX)" %s, %%%s", ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
	else
	{
		if (cpu.ZF)  REG(m.reg) = REG(m.R_M);
		print_asm("cmove"str(SUFFIX)" %%%s, %%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 1 + 1;
	}
}


#include "exec/template-end.h"
