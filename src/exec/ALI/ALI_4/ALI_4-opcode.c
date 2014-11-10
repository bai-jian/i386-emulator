#include "exec/helper.h"

#include "cpu/modrm.h"

#include "sal/sal.h"
//#include "shl/shl.h"
#include "shr/shr.h"
#include "sar/sar.h"

make_helper(ALI_4_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_b(eip);
	if ( m.reg == 5 )  return shr_b(eip);
	if ( m.reg == 7 )  return sar_b(eip);
	assert(0);	return 0;
}

make_helper(ALI_4_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_v(eip);
	if ( m.reg == 5 )  return shr_v(eip);
	if ( m.reg == 7 )  return sar_v(eip);
	assert(0);	return 0;
}

make_helper(ALI_4_CL_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_CL_b(eip);
	if ( m.reg == 5 )  return shr_CL_b(eip);
	if ( m.reg == 7 )  return sar_CL_b(eip);
	assert(0);	return 0;
}

make_helper(ALI_4_CL_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_CL_v(eip);
	if ( m.reg == 5 )  return shr_CL_v(eip);
	if ( m.reg == 7 )  return sar_CL_v(eip);
	assert(0);	return 0;
}

make_helper(ALI_4_ib_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_ib_b(eip);
	if ( m.reg == 5 )  return shr_ib_b(eip);
	if ( m.reg == 7 )  return sar_ib_b(eip);
	assert(0);	return 0;
}

make_helper(ALI_4_ib_v)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if ( m.reg == 4 )  return sal_ib_v(eip);
	if ( m.reg == 5 )  return shr_ib_v(eip);
	if ( m.reg == 7 )  return sar_ib_v(eip);
	assert(0);	return 0;
}
