#include "exec/helper.h"

#include "cpu/modrm.h"

#include "pop/pop.h"

make_helper(DMI_3_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)  return pop_m_v(eip);
	assert(0);  return 0;
}
