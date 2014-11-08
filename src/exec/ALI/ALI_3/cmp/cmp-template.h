#include "exec/template-start.h"


#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(cmp_i2r0_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip+1, DATA_BYTE);
	DATA_TYPE reg_v = REG(0);

	DATA_TYPE value = reg_v - imm;

	concat(set_SF_, SUFFIX) (value);
	concat(set_PF_, SUFFIX) (value);
	concat(set_ZF_, SUFFIX) (value);
	concat(set_OF_, SUFFIX) (reg_v, imm, 1);
	concat(set_CF_, SUFFIX) (reg_v, imm, 1);

	print_asm("cmp" str(SUFFIX) " 0x%x", imm);

	return 1 + DATA_BYTE;
}

make_helper( concat(cmp_i2rm_, SUFFIX) )
{
	uint8_t imm_byte = DATA_BYTE;
	if ( instr_fetch(eip, 1) == 0x83 ) imm_byte = 1;
 
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{ 
		swaddr_t  mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		DATA_TYPE mem_v = MEM_R(mem_i);
		int32_t   imm = instr_fetch(eip+1+len, imm_byte);

		DATA_TYPE value = mem_v - imm;

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (mem_v, imm, 1);
		concat(set_OF_, SUFFIX) (mem_v, imm, 1);

		print_asm("cmp   " "$0x%x,%s", imm, ModR_M_asm);

		return 1 + len + imm_byte;
	}
	else
	{
		uint8_t   reg_i = m.R_M;
		DATA_TYPE reg_v = REG(reg_i); 
		int32_t  imm = instr_fetch(eip+1+1, imm_byte);

		DATA_TYPE value = reg_v - imm;

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (reg_v, imm, 1);
		concat(set_OF_, SUFFIX) (reg_v, imm, 1);

		print_asm("cmp   " "$0x%x,%%%s", imm, REG_NAME(reg_i));

		return 1 + 1 + imm_byte;
	}  
} 

make_helper( concat(cmp_r2rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{ 
		swaddr_t  mem_i;		uint8_t len = read_ModR_M(eip+1, &mem_i);
		DATA_TYPE mem_v = MEM_R(mem_i);
		uint8_t   reg_i = m.reg;
		DATA_TYPE reg_v = REG(reg_i);

		DATA_TYPE value = reg_v - mem_v; 

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (mem_v, reg_v, 1);
		concat(set_OF_, SUFFIX) (mem_v, reg_v, 1);

		print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(reg_i), ModR_M_asm);

		return 1 + len;
	}
	else
	{  
		uint8_t   s_reg_i = m.reg;
		DATA_TYPE s_reg_v = REG(s_reg_i);
		uint8_t   d_reg_i = m.R_M;
		DATA_TYPE d_reg_v = REG(d_reg_i);

		DATA_TYPE value = d_reg_v - s_reg_v;

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (s_reg_v, d_reg_v, 1);
		concat(set_OF_, SUFFIX) (s_reg_v, d_reg_v, 1);

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(s_reg_i), REG_NAME(d_reg_i));

		return 1 + 1;
	}
}

make_helper( concat(cmp_rm2r_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t  mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		DATA_TYPE mem_v = MEM_R(mem_i);
		uint8_t   reg_i = m.reg;
		DATA_TYPE reg_v = REG(reg_i);

		DATA_TYPE value = reg_v - mem_v; 

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (reg_v, mem_v, 1);
		concat(set_OF_, SUFFIX) (reg_v, mem_v, 1);

		print_asm("cmp" str(SUFFIX) "  %s,%%%s", ModR_M_asm, REG_NAME(reg_i));

		return 1 + len;
	}
	else
	{  
		uint8_t   s_reg_i = m.R_M;
		DATA_TYPE s_reg_v = REG(s_reg_i);
		uint8_t   d_reg_i = m.reg;
		DATA_TYPE d_reg_v = REG(d_reg_i);

		DATA_TYPE value = d_reg_v - s_reg_v;

		concat(set_ZF_, SUFFIX) (value);
		concat(set_SF_, SUFFIX) (value);
		concat(set_PF_, SUFFIX) (value);
		concat(set_CF_, SUFFIX) (s_reg_v, d_reg_v, 1);
		concat(set_OF_, SUFFIX) (s_reg_v, d_reg_v, 1);

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(s_reg_i), REG_NAME(d_reg_i));

		return 1 + 1;
	}
}


#include "exec/template-end.h"
