#include "exec/helper.h"


#include "cpu/modrm.h"

int lgdt_v(swaddr_t eip);
make_helper(SI_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 2)  return lgdt_v(eip);
	assert(0); return 0;
}

