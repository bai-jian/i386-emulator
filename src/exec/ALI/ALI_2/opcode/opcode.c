#include "exec/helper.h"


#include "cpu/modrm.h"

/* ALI_2_b: F6 */
int  test_i2rm_b (swaddr_t eip);
int  not_b       (swaddr_t eip);
int  neg_b       (swaddr_t eip);
int  imul_b      (swaddr_t eip);
int  idiv_b      (swaddr_t eip);
make_helper(ALI_2_b)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if ( m.reg == 0 )  return  test_i2rm_b (eip);
	if ( m.reg == 2 )  return  not_b       (eip);
	if ( m.reg == 3)   return  neg_b       (eip);
	if ( m.reg == 5 )  return  imul_b      (eip);
	if ( m.reg == 7 )  return  idiv_b      (eip);
	assert(0);  return 0;
}

/* ALI_2_v: F7 */
int  test_i2rm_v (swaddr_t eip);
int  not_v       (swaddr_t eip);
int  neg_v       (swaddr_t eip);
int  imul_v      (swaddr_t eip);
int  idiv_v      (swaddr_t eip);
make_helper(ALI_2_v)
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if ( m.reg == 0 )  return  test_i2rm_v (eip);
	if ( m.reg == 2 )  return  not_v       (eip);
	if ( m.reg == 3 )  return  neg_v       (eip);
	if ( m.reg == 5 )  return  imul_v      (eip);
	if ( m.reg == 7 )  return  idiv_v      (eip);
	assert(0);  return 0;
}
