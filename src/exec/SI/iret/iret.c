#include "exec/helper.h"


#include "memory.h"
#include "cpu/reg.h"

make_helper(iret)
{
	uint8_t instr_len = 1;

	// Pop EIP, CS, EFLAGS
	cpu.eip    = (uint32_t)swaddr_read(cpu.esp, 4) - instr_len;  cpu.esp += 4;
	cpu.CS     = (uint16_t)swaddr_read(cpu.esp, 4);              cpu.esp += 4;
	cpu.eflags = (uint32_t)swaddr_read(cpu.esp, 4);              cpu.esp += 4;

	print_asm("iret");

	return instr_len;
}
