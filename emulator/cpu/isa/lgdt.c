#include "helper.h"

make_helper(lgdt)
{
	// ModR_M:  mod  reg  R_M
	//           xx  010  xxx
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	assert(m.reg == 2);

	if (m.mod != 3)
	{
		phyaddr_t ph_addr;
		uint8_t len = read_ModR_M(eip + 1, &ph_addr);

		cpu.GDTR.limit = swaddr_read(ph_addr    , 2);
		cpu.GDTR.base  = swaddr_read(ph_addr + 2, 4);

		print_asm("lgdt   %s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		phyaddr_t ph_addr = reg_l(m.R_M);

		cpu.GDTR.limit = swaddr_read(ph_addr    , 2);
		cpu.GDTR.base  = swaddr_read(ph_addr + 2, 4);

		print_asm("lgdt   %%%s", regsl[m.R_M]);

		return 1 + 1;
	}
}
