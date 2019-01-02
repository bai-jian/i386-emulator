#include "helper.h"

make_helper(lidt)
{
	// ModR_M:  mod  reg  R_M
	//           xx  011  xxx
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	assert(m.reg == 3);

	if (m.mod != 3)
	{
		phyaddr_t ph_addr;
		uint8_t len = read_ModR_M(eip + 1, &ph_addr);

		cpu.IDTR.limit = swaddr_read(ph_addr    , 2);
		cpu.IDTR.base  = swaddr_read(ph_addr + 2, 4);

		print_asm("lidt   %s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		phyaddr_t ph_addr = reg_l(m.R_M);

		cpu.IDTR.limit = swaddr_read(ph_addr    , 2);
		cpu.IDTR.base  = swaddr_read(ph_addr + 2, 4);
		
		print_asm("lidt   %%%s", regsl[m.R_M]);

		return 1 + 1;
	}
}
