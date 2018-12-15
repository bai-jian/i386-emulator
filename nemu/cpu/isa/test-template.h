#include "template-start.h"
#include "modrm.h"
#include "set_eflags.h"


make_helper( concat(test_i2r0_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);

	DATA_TYPE value = REG(0) & imm;
	cpu.CF = cpu.OF = 0;
	concat(set_SF_, SUFFIX) (value);
	concat(set_ZF_, SUFFIX) (value);
	concat(set_PF_, SUFFIX) (value);

	print_asm("test"str(SUFFIX)"  $0x%x", imm);
	return 1 + DATA_BYTE;
}

make_helper( concat(test_i2rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		DATA_TYPE mem = MEM_R(mem_a);
		DATA_TYPE imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		DATA_TYPE value = mem & imm;
		cpu.CF = cpu.OF = 0;
		concat(set_SF_, SUFFIX) (value);
		concat(set_ZF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);

		print_asm("test"str(SUFFIX)"  $0x%x, %s", imm,  ModR_M_asm);
		return 1 + len + DATA_BYTE;
	}
	else
	{
		DATA_TYPE imm = instr_fetch(eip + 1 + 1, DATA_BYTE);

		DATA_TYPE value = REG(m.R_M) & imm;
		cpu.CF = cpu.OF = 0;
		concat(set_SF_, SUFFIX) (value);
		concat(set_ZF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);

		print_asm("test"str(SUFFIX)"  $0x%x, %%%s", imm, REG_NAME(m.R_M));
		return 1 + 1 + DATA_BYTE;
	}
}

make_helper( concat(test_r2rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		DATA_TYPE mem = MEM_R(mem_a);
		DATA_TYPE reg = REG(m.reg);

		DATA_TYPE value = mem & reg;

		cpu.CF = cpu.OF = 0;
		concat(set_SF_, SUFFIX) (value);
		concat(set_ZF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);

		print_asm("test"str(SUFFIX)"  %%%s, %s", REG_NAME(m.reg), ModR_M_asm);
		return 1 + len;
	}
	else
	{   
		uint8_t   s_reg_i = m.reg;
		DATA_TYPE s_reg_v = REG(s_reg_i);
		uint8_t   d_reg_i = m.R_M;
		DATA_TYPE d_reg_v = REG(d_reg_i);

		DATA_TYPE value = s_reg_v & d_reg_v;

		cpu.CF = cpu.OF = 0;
		concat(set_SF_, SUFFIX) (value);
		concat(set_ZF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);

		print_asm("test"str(SUFFIX)"  %%%s, %%%s", REG_NAME(s_reg_i), REG_NAME(d_reg_i));
		return 1 + 1;
	}
}

#include "template-end.h"
