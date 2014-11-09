#include "exec/helper.h"


#define  DATA_BYTE 1
#include "imul-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "imul-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "imul-template.h"
#undef   DATA_BYTE


extern char suffix;

make_helper(imul_b);
make_helper(imul_w);
make_helper(imul_l);

make_helper(imul_v)
{	return (suffix == 'l'  ?  imul_l(eip)  :  imul_w(eip));	}

make_helper(imul_rm2r_v)
{	return (suffix == 'l'  ?  imul_rm2r_l(eip)    :  imul_rm2r_w(eip));		}
/*
make_helper(imul_rmib2r_v)
{	return (suffix == 'l'  ?  imul_rmib2r_l(eip)  :  imul_rmib2r_w(eip));	}

make_helper(imul_rmi2r_v)
{	return (suffix == 'l'  ?  imul_rmi2r_l(eip)   :  imul_rmi2r_w(eip));	}
*/


#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(imul_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		int8_t mem_v = swaddr_read(mem_i, 1);
		reg_w(R_AX) = reg_b(R_AL) * mem_v;
		cpu.CF = cpu.OF =  reg_b(R_AL) == reg_w(R_AX);
		
		print_asm("imulb " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int8_t reg_v = reg_b(reg_i);
		reg_w(R_AX) = reg_b(R_AL) * reg_v;
		cpu.CF = cpu.OF =  reg_b(R_AL) == reg_w(R_AX);

		print_asm("imulb " "%%%s", regsb[reg_i]);

		return 1 + 1;
	}
}

make_helper(imul_w)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		int16_t mem_v = swaddr_read(mem_i, 2);
		reg_l(R_EAX) = reg_w(R_AX) * mem_v;
		cpu.CF = cpu.OF =  reg_w(R_AX) == reg_l(R_EAX);
		
		print_asm("imulw " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int16_t reg_v = reg_w(reg_i);
		reg_l(R_EAX) = reg_w(R_AX) * reg_v;
		cpu.CF = cpu.OF =  reg_w(R_AX) == reg_l(R_EAX);

		print_asm("imulw " "%%%s", regsw[reg_i]);

		return 1 + 1;
	}
}

make_helper(imul_l)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		int32_t mem_v = swaddr_read(mem_i, 4);
		int64_t value = reg_l(R_EAX) * mem_v;
		reg_l(R_EAX) = value & 0xFFFFFFFF;
		reg_l(R_EDX) = (value >> 32) & 0xFFFFFFFF; 
		cpu.CF = cpu.OF =  reg_l(R_EAX) == value;
		
		print_asm("imull " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int32_t reg_v = reg_l(reg_i);
		int64_t value = reg_l(R_EAX) * reg_v;
		reg_l(R_EAX) = value & 0xFFFFFFFF;
		reg_l(R_EDX) = (value >> 32) & 0xFFFFFFFF;
		cpu.CF = cpu.OF =  reg_l(R_EAX) == value;

		print_asm("imull " "%%%s", regsl[reg_i]);

		return 1 + 1;
	}
}
