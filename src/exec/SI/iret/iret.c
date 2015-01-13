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

Log("eip 0x%x 0x%x 0x%x\n", cpu.eip, cpu.CS, cpu.eflags); assert(0);

	print_asm("iret");
	swaddr_read(cpu.eip, 1);assert(0);
	return instr_len;
}
