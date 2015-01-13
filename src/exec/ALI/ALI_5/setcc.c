#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"

make_helper(setne)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		assert(len == 1);
		swaddr_write(mem_a, 1, !cpu.ZF);
		print_asm("setne  %s", ModR_M_asm);
	}
	else
	{
		uint8_t reg_i = m.R_M;
		reg_b(reg_i) = !cpu.ZF;
		print_asm("setne  %%%s", regsb[reg_i]);
	}
	return 2;
}

make_helper(sete)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		assert(len == 1);
		swaddr_write(mem_a, 1, cpu.ZF);
		print_asm("sete   %s", ModR_M_asm);
	}
	else
	{
		uint8_t reg_i = m.R_M;
		reg_b(reg_i) = cpu.ZF;
		print_asm("sete   %%%s", regsb[reg_i]);
	}
	return 2;
}
