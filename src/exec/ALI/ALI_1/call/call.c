#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper(call_i)
{
	int32_t imm = instr_fetch(eip+1, 4);

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip+5);
	cpu.eip += (5 + imm);

	print_asm("call" "0x%x", cpu.eip);

	return 0;
}

make_helper(call_rm)
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);

	int32_t disp; uint8_t len;
	if (m.mod != 3)
	{
		swaddr_t mem_i;	len = 1 + read_ModR_M(eip+1, &mem_i);
		disp = swaddr_read(mem_i, 4);
	}
	else
		disp = reg_l(m.R_M), len = 2;

	cpu.esp -= 4;	swaddr_write(cpu.esp, 4, cpu.eip+len);
	cpu.eip += (len + disp);

	print_asm("call" "0x%x", cpu.eip);

	return 0;
}
