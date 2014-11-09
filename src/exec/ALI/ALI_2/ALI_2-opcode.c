#include "exec/helper.h"

#include "cpu/modrm.h"

#include "idiv/idiv.h"

make_helper(ALI_2_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 7 )  return idiv_b(eip);
	assert(0);  return 0;
}

make_helper(ALI_2_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 7 )  return idiv_v(eip);
	assert(0);  return 0;
}
