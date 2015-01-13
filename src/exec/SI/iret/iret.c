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

	print_asm("iret");

	return instr_len;
}