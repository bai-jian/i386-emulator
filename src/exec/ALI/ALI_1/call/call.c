#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper(call_i)
{
	uint8_t instr_len = 5;

	int32_t imm = instr_fetch(eip+1, 4);

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip+instr_len);
	cpu.eip += imm;

	print_asm("call" " %x", cpu.eip + instr_len);

	return instr_len;
}

make_helper(call_rm)
{
	uint8_t instr_len;

	ModR_M m;	m.val = instr_fetch(eip+1, 1);

	int32_t disp;
	if (m.mod != 3)
	{
		swaddr_t mem_i;	uint8_t len = read_ModR_M(eip+1, &mem_i);
		disp = swaddr_read(mem_i, 4);
		instr_len = 1 + len;
	}
	else
		disp = reg_l(m.R_M), instr_len = 2;

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip+instr_len);
	cpu.eip += disp;

	print_asm("call" " %x", cpu.eip + instr_len);

	return instr_len;
}
