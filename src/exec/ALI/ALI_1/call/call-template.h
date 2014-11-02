#include "exec/template-start.h"


#include "cpu/modrm.h"

make_helper( concat(call_i_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip+1, DATA_BYTE);

	MEM_W(cpu.esp, eip);
	cpu.esp -= 4;
	
	return 1 + DATA_BYTE + imm;
}

make_helper( concat(call_rm_, SUFFIX) )
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);

	if (m.mod != 3)
	{
		swaddr_t  mem_i;	uint8_t len = read_ModR_M(eip+1, &mem_i);
		DATA_TYPE mem_v = MEM_R(mem_i);

		MEM_W(cpu.esp, eip);
		cpu.esp -= 4;

		return 1 + len + mem_v;
	}
	else
	{ 
		uint8_t   reg_i = m.R_M;
		DATA_TYPE reg_v = REG(reg_i);

		MEM_W(cpu.esp, eip);
		cpu.esp -= 4;

		return 1 + 1 + reg_v;
	}
}


#include "exec/template-end.h"
