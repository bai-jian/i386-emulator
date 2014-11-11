#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"

extern char suffix;

make_helper(movsbv)
{
	if (suffix == 'w')
	{
		ModR_M m;  m.val = instr_fetch(eip+1, 1);
		if (m.mod != 3)
		{
			swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
			reg_w(m.reg) = (int8_t)swaddr_read(mem_i, 1);
			print_asm("movsbw  " "%s,%%%s", ModR_M_asm, regsw[m.reg]);
			return 1 + len;
		}
		else
		{
			reg_w(m.reg) = (int8_t)reg_b(m.R_M);
			print_asm("movsbw  " "%%%s,%%%s", regsb[m.R_M], regsw[m.reg]);
			return 1 + 1;
		}
	}
	else
	{
		ModR_M m;  m.val = instr_fetch(eip+1, 1);
		if (m.mod != 3)
		{
			swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
			reg_l(m.reg) = (int8_t)swaddr_read(mem_i, 1);
			print_asm("movsbl  " "%s,%%%s", ModR_M_asm, regsl[m.reg]);
			return 1 + len;
		}
		else
		{
			reg_l(m.reg) = (int8_t)reg_b(m.R_M);
			print_asm("movsbl  " "%%%s,%%%s", regsb[m.R_M], regsl[m.reg]);
			return 1 + 1;
		}
	}
}

make_helper(movswl)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		reg_l(m.reg) = (int16_t)swaddr_read(mem_i, 2);
		print_asm("movswl  " "%s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
	else
	{
		reg_l(m.reg) = (int16_t)reg_w(m.R_M);
		print_asm("movswl  " "%%%s,%%%s", regsw[m.R_M], regsl[m.reg]);
		return 1 + 1;
	}
}
