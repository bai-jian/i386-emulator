#include "exec/helper.h"


#include "memory.h"

make_helper(ret)
{
	uint8_t instr_len = 1;

	uint32_t disp = swaddr_read(cpu.esp, 4);	cpu.esp += 4;
	cpu.eip = disp - instr_len;

	printf("0x%x/n", cpu.eip);

	print_asm("ret");

	return instr_len;
}
