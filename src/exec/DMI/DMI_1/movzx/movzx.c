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
			swaddr_write(mem_i, 2, reg_b(m.reg));
			print_asm("movzbw  " "%%%s,%s", regsb[m.reg], ModR_M_asm);
			return 1 + len;
		}
		else
		{
			reg_w(m.R_M) = reg_b(m.reg);
			print_asm("movzbw  " "%%%s,%%%s", regsb[m.reg], regsw[m.R_M]);
			return 1 + 1;
		}
	}
	else
	{
		ModR_M m;  m.val = instr_fetch(eip+1, 1);
		if (m.mod != 3)
		{
			swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
			swaddr_write(mem_i, 4, reg_b(m.reg));
			print_asm("movzbl  " "%%%s,%s", regsb[m.reg], ModR_M_asm);
			return 1 + len;
		}
		else
		{
			reg_l(m.R_M) = reg_b(m.reg);
			print_asm("movzbl  " "%%%s,%%%s", regsb[m.reg], regsl[m.R_M]);
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
		swaddr_write(mem_i, 4, reg_w(m.reg));
		print_asm("movzwl  " "%%%s,%s", regsw[m.reg], ModR_M_asm);
		return 1 + len;
	}
	else
	{
		reg_l(m.R_M) = reg_w(m.reg);
		print_asm("movzwl  " "%%%s,%%%s", regsw[m.reg], regsl[m.R_M]);
		return 1 + 1;
	}
}
