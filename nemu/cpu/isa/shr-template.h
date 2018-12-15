#include "template-start.h"
#include "cpu/reg.h"
#include "modrm.h"

make_helper( concat(shr_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		DATA_TYPE mem_v = MEM_R(mem_i);
		DATA_TYPE value = mem_v >> 1;
		MEM_W(mem_i, value);
		cpu.CF = ((mem_v&0x01) == 1);

		print_asm("shr" str(SUFFIX) "  %s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		DATA_TYPE reg_v = REG(reg_i);
		DATA_TYPE value = reg_v >> 1;
		REG(reg_i) = value;
		cpu.CF = ((reg_v&0x01) == 1);

		print_asm("shr" str(SUFFIX) "  %%%s", REG_NAME(reg_i));

		return 1 + 1;
	}
}

make_helper( concat(shr_CL_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		DATA_TYPE mem_v = MEM_R(mem_i);
		DATA_TYPE value = mem_v >> reg_b(R_CL);
		MEM_W(mem_i, value);

		print_asm("shr" str(SUFFIX) "  %%cl,%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{ 
		uint8_t reg_i = m.R_M;

		DATA_TYPE reg_v = REG(reg_i);
		DATA_TYPE value = reg_v >> reg_b(R_CL);
		REG(reg_i) = value;

		print_asm("shr" str(SUFFIX) "  %%cl,%%%s", REG_NAME(reg_i));

		return 1 + 1;
	}
}

make_helper( concat(shr_ib_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		uint8_t imm = instr_fetch(eip+1+len, 1);

		DATA_TYPE mem_v = MEM_R(mem_i);
		DATA_TYPE value = mem_v >> imm;
		MEM_W(mem_i, value);

		print_asm("shr" str(SUFFIX) "  $0x%x,%s", imm, ModR_M_asm);

		return 1 + len + 1;
	}
	else
	{
		uint8_t reg_i = m.R_M;
		uint8_t imm = instr_fetch(eip+1+1, 1);

		DATA_TYPE reg_v = REG(reg_i);
		DATA_TYPE value = reg_v >> imm;
		REG(reg_i) = value;

		print_asm("shr" str(SUFFIX) "  $0x%x,%%%s", imm, REG_NAME(reg_i));

		return 1 + 1 + 1;
	}
}


#include "template-end.h"
