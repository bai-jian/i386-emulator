#include "exec/helper.h"


#include "cpu/modrm.h"

int lgdt(swaddr_t eip);
int lidt(swaddr_t eip);
make_helper(SI_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 2)  return lgdt(eip);
	if (m.reg == 3)  return lidt(eip);
	assert(0); return 0;
}

