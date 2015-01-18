#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"


int bt_r_w(swaddr_t eip);
int bt_r_l(swaddr_t eip);


extern char suffix;
make_helper(bt_r_v)
{  return suffix == 'l' ? bt_r_l(eip) : bt_r_w(eip);  }


int bt_r_w(swaddr_t eip)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		uint16_t left = swaddr_read(mem_a, 2);
		uint16_t rigt = reg_w(m.reg);
		cpu.CF = (left >> rigt) & 1;
		print_asm("btw    %s, %%%s", ModR_M_asm, regsw[m.reg]);
		return 1 + len;
	}
	else
	{
		uint16_t left = reg_w(m.R_M);
		uint16_t rigt = reg_w(m.reg);
		cpu.CF = (left >> rigt) & 1;
		print_asm("btw    %%%s, %%%s", regsw[m.R_M], regsw[m.reg]);
		return 1 + 1;
	}
}

int bt_r_l(swaddr_t eip)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		uint32_t left = swaddr_read(mem_a, 4);
		uint32_t rigt = reg_l(m.reg);
		cpu.CF = (left >> rigt) & 1;
		print_asm("btl    %s, %%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
	else
	{
		uint32_t left = reg_l(m.R_M);
		uint32_t rigt = reg_l(m.reg);
		cpu.CF = (left >> rigt) & 1;
		print_asm("btl    %%%s, %%%s", regsl[m.R_M], regsl[m.reg]);
		return 1 + 1;
	}
}
