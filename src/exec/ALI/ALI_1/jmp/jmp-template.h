#include "exec/template-start.h"

#include "cpu/modrm.h"

make_helper( concat(jmp_i_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;

	int32_t imm = (DATA_TYPE_S)instr_fetch(eip+1, DATA_BYTE);

	cpu.eip += imm; 

	print_asm("jmp   " "0x%x", cpu.eip + instr_len);

	return instr_len;
}

make_helper( concat(jmp_rm_, SUFFIX) )
{
	uint8_t instr_len;

	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		int32_t mem_v = (DATA_TYPE_S) MEM_R(mem_i);
		cpu.eip += mem_v;  
		instr_len = 1 + len;

		print_asm("jmp   " "%s", ModR_M_asm);

		return instr_len;
	}
	else
	{
		uint8_t  reg_i = m.R_M;

		int32_t reg_v = (DATA_TYPE_S) swaddr_read(reg_l(reg_i), DATA_BYTE);
		cpu.eip += reg_v;  
		instr_len = 1 + 1;

		print_asm("jmp   " "%%%s", REG_NAME(reg_i));

		return instr_len;
	}
}


#include "exec/template-end.h"
