#include "helper.h"
#include "modrm.h"


int inc_rm_b(swaddr_t eip);
int dec_rm_b(swaddr_t eip);
make_helper(ALI_1_b)
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)  return inc_rm_b(eip);
	if (m.reg == 1)  return dec_rm_b(eip);
	assert(0); return 0;
}

int inc_rm_v(swaddr_t eip);
int dec_rm_v(swaddr_t eip);
int call_rm(swaddr_t eip);
int jmp_rm(swaddr_t eip);
int push_m_v(swaddr_t eip);
make_helper(ALI_1_v)
{
	ModR_M m;   m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)  return inc_rm_v(eip);
	if (m.reg == 1)  return dec_rm_v(eip);
	if (m.reg == 2)  return call_rm(eip);
	if (m.reg == 4)  return jmp_rm(eip);
	if (m.reg == 6)  return push_m_v(eip);
	assert(0); return 0;
}
