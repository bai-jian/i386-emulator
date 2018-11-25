#include "exec/template-start.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"

make_helper( concat(pop_r_, SUFFIX) )
{
	uint8_t reg_i = instr_fetch(eip, 1) & 0x7;

	REG(reg_i) = MEM_R(cpu.esp);
	cpu.esp += DATA_BYTE;

	print_asm("pop" str(SUFFIX) "   %%%s", REG_NAME(reg_i));

	return 1;
}

make_helper( concat(pop_m_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.mod != 3);

	swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

	MEM_W(mem_i, MEM_R(cpu.esp));
	cpu.esp += DATA_BYTE;

	print_asm("pop" str(SUFFIX) "   %s", ModR_M_asm);

	return 1 + len;
}


#include "exec/template-end.h"
