#include "exec/helper.h"


#include "memory.h"
#include "cpu/reg.h"

make_helper(iret)
{
	uint8_t instr_len = 1;

	// Pop EIP, CS, EFLAGS
	cpu.eip = swaddr_read(cpu.esp, 4) - instr_len;     cpu.esp += 4;
	cpu.CS = swaddr_read(cpu.esp, 2);                  cpu.esp += 2;
	cpu.eflags = swaddr_read(cpu.esp, 4);              cpu.esp += 4;
Log("eip 0x%x\n", cpu.eip);
	int i;
	for (i = 0; i < 100; ++i)
		Log("0x%x  %x", swaddr_read(cpu.esp - i, 1), swaddr_read(cpu.esp + i, 1));
assert(0);
	print_asm("iret");
	swaddr_read(cpu.eip, 1);assert(0);
	return instr_len;
}
