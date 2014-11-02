#include "exec/helper.h"

#include "inc/inc.h"
#include "dec/dec.h"

//#include "inc/inc.h"
//#include "dec/dec.h"
#include "call/call.h"
#include "jmp/jmp.h"
#include "push/push.h"

#include "cpu/modrm.h"

make_helper(ALI_1_b)
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)  return inc_rm_b(eip);
	if (m.reg == 1)  return dec_rm_b(eip);
	assert(0); return 0;
}

make_helper(ALI_1_v)
{
	ModR_M m;   m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)  return inc_rm_v(eip);
	if (m.reg == 1)  return dec_rm_v(eip);
	if (m.reg == 2)  return call_rm_v(eip);
	if (m.reg == 4)  return jmp_rm_v(eip);
	if (m.reg == 6)  return push_m_v(eip);
	assert(0); return 0;
}
