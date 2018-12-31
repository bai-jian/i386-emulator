#include "helper.h"

make_helper(INT)
{
	uint8_t instr_len = 2;
	uint8_t imm = instr_fetch(eip + 1, 1);

	cpu.eip += instr_len;
	int_handle(imm);
	cpu.eip -= instr_len;
	print_asm("int    $0x%x", imm);

	return instr_len;
}

