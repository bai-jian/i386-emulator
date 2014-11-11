#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"

extern char suffix;

make_helper(movzbv)
{
	if (suffix == 'w')
	{
		ModR_M m;  m.val = instr_fetch(eip+1, 1);
		if (m.mod != 3)
		{
			swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
			reg_w(m.reg) = swaddr_read(mem_i, 1);
			print_asm("movzbw  " "%s,%%%s", ModR_M_asm, regsw[m.reg]);
			return 1 + len;
		}
		else
		{
			reg_w(m.reg) = reg_b(m.R_M);
			print_asm("movzbw  " "%%%s,%%%s", regsb[m.R_M], regsw[m.reg]);
			return 1 + 1;
		}
	}
	else
	{
		ModR_M m;  m.val = instr_fetch(eip+1, 1);
		if (m.mod != 3)
		{
			swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
			reg_l(m.reg) = swaddr_read(mem_i, 1);
			print_asm("movzbl  " "%s,%%%s", ModR_M_asm, regsl[m.reg]);
			return 1 + len;
		}
		else
		{
			reg_l(m.reg) = reg_b(m.R_M);
			print_asm("movzbl  " "%%%s,%%%s", regsb[m.R_M], regsl[m.reg]);
			return 1 + 1;
		}
	}
}

make_helper(movzwl)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		reg_l(m.reg) = swaddr_read(mem_i, 2);
		print_asm("movzwl  " "%s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
	else
	{
		reg_l(m.reg) = reg_w(m.R_M);
		print_asm("movzwl  " "%%%s,%%%s", regsw[m.R_M], regsl[m.reg]);
		return 1 + 1;
	}
}
