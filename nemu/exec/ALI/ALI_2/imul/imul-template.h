#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"

make_helper( concat(imul_rm2r_, SUFFIX) )
{
//	assert(0);
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		uint8_t  reg_i = m.reg;

		DATA_TYPE_S mem_v = MEM_R(mem_i), reg_v = REG(reg_i);
		DATA_TYPE_S value = mem_v * reg_v;
		REG(reg_i) = value;
		cpu.CF = cpu.OF = ( value != ((int64_t)(mem_v))*reg_v );

		print_asm("imul" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(reg_i));

		return 1 + len;
	}
	else
	{
		uint8_t s_reg_i = m.R_M, d_reg_i = m.reg;

		DATA_TYPE_S s_reg_v = REG(s_reg_i), d_reg_v = REG(d_reg_i);
		DATA_TYPE_S value = s_reg_v * d_reg_v;
		REG(d_reg_i) = value;
		cpu.CF = cpu.OF = ( value != ((int64_t)(s_reg_v))*d_reg_v );

		print_asm("imul" str(SUFFIX) " %%%s,%%%s", REG_NAME(s_reg_i), REG_NAME(d_reg_i));

		return 1 + 1;
	}
}

make_helper( concat(imul_i2r_, SUFFIX) )
{
//	assert(0);
	uint8_t imm_byte = DATA_BYTE;
	if ( instr_fetch(eip, 1) == 0x6b )  imm_byte = 1;

	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;	 uint8_t len = read_ModR_M(eip+1, &mem_i);
		uint8_t  reg_i = m.reg;
		DATA_TYPE_S imm = instr_fetch(eip+1+len, imm_byte);
		
		DATA_TYPE_S mem_v = MEM_R(mem_i);
		DATA_TYPE_S value = mem_v * imm;
		REG(reg_i) = value;
		cpu.CF = cpu.OF = ( value != ((int64_t)(mem_v))*imm );

		print_asm("imul" str(SUFFIX) " $0x%x,%s,%%%s", imm, ModR_M_asm, REG_NAME(reg_i));

		return 1 + len + imm_byte;
	}
	else
	{
		uint8_t s_reg_i = m.R_M;
		uint8_t d_reg_i = m.reg;
		DATA_TYPE_S imm = instr_fetch(eip+1+1, imm_byte);

		DATA_TYPE_S s_reg_v = REG(s_reg_i);
		DATA_TYPE_S value = s_reg_v * imm;
		REG(d_reg_i) = value;
		cpu.CF = cpu.OF = ( value != ((int64_t)(s_reg_v))*imm );

		print_asm("imul" str(SUFFIX) " $0x%x,%%%s,%%%s", imm, REG_NAME(s_reg_i), REG_NAME(d_reg_i));

		return 1 + 1 + imm_byte;
	}
}


#include "exec/template-end.h"
