#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper(call_i)
{
	uint8_t instr_len = 1 + 4;

	int32_t disp = instr_fetch(eip+1, 4);

	// Push the return address into stack
	cpu.esp -= 4;  swaddr_write(cpu.esp, 4, eip + instr_len);

	// Write cpu.eip
	cpu.eip = (int32_t)cpu.eip + disp;

	print_asm("call   %x", disp);

	return instr_len;
}

make_helper(call_rm)
{
	uint8_t instr_len = 1;

	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;	 instr_len += read_ModR_M(eip+1, &mem_i);
		uint32_t addr = swaddr_read(mem_i, 4);

		// Push the return address into stack
		cpu.esp -= 4;
		swaddr_write(cpu.esp, 4, eip+instr_len);
		// Write eip
		cpu.eip = addr - instr_len;

		print_asm("call   *%s", ModR_M_asm);

		return instr_len;
	}
	else
	{
		instr_len += 1;
		// Push the return address into stack
		cpu.esp -= 4;
		swaddr_write(cpu.esp, 4, eip+instr_len);
		// Write eip
		cpu.eip = reg_l(m.R_M) - instr_len;

		print_asm("call   *%%%s", regsl[m.R_M]);

		return instr_len;
	}
}
