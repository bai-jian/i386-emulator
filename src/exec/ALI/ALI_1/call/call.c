#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper(call_i)
{
	int32_t imm = instr_fetch(eip, 4);

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip+5);
	cpu.eip += (5 + imm);

	return 0;
}

make_helper(call_rm)
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);

	int32_t disp;
	if (m.mod != 3)
	{
		swaddr_t mem_i;	read_ModR_M(eip+1, &mem_i);
		disp = swaddr_read(mem_i, 4);
	}
	else
		disp = reg_l(m.R_M);

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip);
	cpu.eip += disp;

	return 0;
}
