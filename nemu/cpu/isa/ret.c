#include "helper.h"
#include "memory.h"

make_helper(ret)
{
	uint8_t instr_len = 1;
	uint32_t ret_addr = swaddr_read(cpu.esp, 4);  cpu.esp += 4;
	cpu.eip = ret_addr - instr_len;
	print_asm("ret ");
	return instr_len;
}

make_helper(ret_iw)
{
	uint8_t instr_len = 3;
	uint32_t imm = instr_fetch(eip+1, 2);

	uint32_t disp = swaddr_read(cpu.esp, 4);
	cpu.eip = disp - instr_len;
	cpu.esp += (imm + 4);

	print_asm("ret   " "$0x%x", imm);

	return instr_len;
}
