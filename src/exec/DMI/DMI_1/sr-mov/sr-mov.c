#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"



// movl   r32, %cr0/%cr1/%cr2/%cr3
make_helper(mov_cr2r)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	
	reg_l(m.reg) = cpu.CR[m.R_M];

	print_asm("movl   %%cr%d, %%%s", m.R_M, regsl[m.reg]);

	return 1 + 1;
}

// movl   %cr0/%cr1/%cr2/%cr3, r32
make_helper(mov_r2cr)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);

	cpu.CR[m.R_M] = reg_l(m.reg);
assert(0);
	print_asm("movl   %%%s, %%cr%d", regsl[m.reg], m.R_M);

	return 1 + 1;
}
