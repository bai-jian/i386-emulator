#include "exec/helper.h"


#include "cpu/modrm.h"


int  add_i2rm_b  (swaddr_t eip);
int   or_i2rm_b  (swaddr_t eip);
int  adc_i2rm_b  (swaddr_t eip);
int  sbb_i2rm_b  (swaddr_t eip);
int  and_i2rm_b  (swaddr_t eip);
int  sub_i2rm_b  (swaddr_t eip);
int  xor_i2rm_b  (swaddr_t eip);
int  cmp_i2rm_b  (swaddr_t eip);
make_helper(ALI_3_b)
{	
	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)	 return  add_i2rm_b(eip);
	if (m.reg == 1)  return   or_i2rm_b(eip);
	if (m.reg == 2)  return  adc_i2rm_b(eip);
	if (m.reg == 3)  return  sbb_i2rm_b(eip);
	if (m.reg == 4)  return  and_i2rm_b(eip);
	if (m.reg == 5)  return  sub_i2rm_b(eip);
	if (m.reg == 6)  return  xor_i2rm_b(eip);
	if (m.reg == 7)  return  cmp_i2rm_b(eip);
	assert(0);	return 0;
}

int  add_i2rm_v  (swaddr_t eip);
int   or_i2rm_v  (swaddr_t eip);
int  adc_i2rm_v  (swaddr_t eip);
int  sbb_i2rm_v  (swaddr_t eip);
int  and_i2rm_v  (swaddr_t eip);
int  sub_i2rm_v  (swaddr_t eip);
int  xor_i2rm_v  (swaddr_t eip);
int  cmp_i2rm_v  (swaddr_t eip);
make_helper(ALI_3_v)
{	
	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0)	 return  add_i2rm_v(eip);
	if (m.reg == 1)  return   or_i2rm_v(eip);
	if (m.reg == 2)  return  adc_i2rm_v(eip);
	if (m.reg == 3)  return  sbb_i2rm_v(eip);
	if (m.reg == 4)  return  and_i2rm_v(eip);
	if (m.reg == 5)  return  sub_i2rm_v(eip);
	if (m.reg == 6)  return  xor_i2rm_v(eip);
	if (m.reg == 7)  return  cmp_i2rm_v(eip);
	assert(0);	return 0;
}

