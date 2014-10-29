#include "exec/template-start.h"


#include "cpu/modrm.h"
#include "exec/ALI/sub/sub.h"
#include "exec/all-instr/cmp/cmp.h"


make_helper( concat(inst_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
//	if (m.reg == 5)  return concat(sub_i2rm_, SUFFIX)(eip);
	if (m.reg == 7)  return concat(cmp_i2rm_, SUFFIX)(eip);
	assert(0);
	return 0;
}


#include "exec/template-end.h"
