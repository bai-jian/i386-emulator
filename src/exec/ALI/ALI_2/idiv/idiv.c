#include "exec/helper.h"

#include "memory.h"
#include "cpu/modrm.h"


make_helper(idiv_b);
make_helper(idiv_w);
make_helper(idiv_l);


extern char suffix;
make_helper(idiv_v)
{	return  suffix == 'l'  ?  idiv_l(eip)  :  idiv_w(eip);	}


make_helper(idiv_b)
{
	// ModR_M:  mod  reg  R_M
	//           xx  111  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 7);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		
		int16_t dend = reg_w(R_AX);
		int16_t dsor = (int8_t)swaddr_read(mem_a, 1);
		assert(dsor != 0);
		reg_b(R_AL) = dend / dsor;
		reg_b(R_AH) = dend % dsor;

		print_asm("idivb     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int16_t dend = reg_w(R_AX);
		int16_t dsor = (int8_t)reg_b(m.R_M);
		assert(dsor != 0);
		reg_b(R_AL) = dend / dsor;
		reg_b(R_AH) = dend % dsor;

		print_asm("idivb     %%%s", regsb[m.R_M]);
		return 1 + 1;
 	}
}

make_helper(idiv_w)
{
	// ModR_M:  mod  reg  R_M
	//           xx  111  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 7);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		
		int32_t dend = ((int32_t)reg_w(R_DX) << 16) + reg_w(R_AX);
		int32_t dsor = (int16_t)swaddr_read(mem_a, 2);
		assert(dsor != 0);
		reg_w(R_AX) = dend / dsor;
		reg_w(R_DX) = dend % dsor;

		print_asm("idivw     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int32_t dend = ((int32_t)reg_w(R_DX) << 16) + reg_w(R_AX);
		int32_t dsor = (int16_t)reg_w(m.R_M);
		assert(dsor != 0);
		reg_w(R_AX) = dend / dsor;
		reg_w(R_DX) = dend % dsor;

		print_asm("idivw     %%%s", regsw[m.R_M]);
		return 1 + 1;
 	}
}

make_helper(idiv_l)
{
	// ModR_M:  mod  reg  R_M
	//           xx  111  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 7);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);
		
		int64_t dend = ((int64_t)reg_l(R_EDX) << 32) + reg_l(R_EAX);
		int64_t dsor = (int32_t)swaddr_read(mem_a, 4);
		assert(dsor != 0);
		reg_l(R_EAX) = dend / dsor;
		reg_l(R_EDX) = dend % dsor;

		print_asm("idivl     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int64_t dend = ((int64_t)reg_l(R_EDX) << 32) + reg_l(R_EAX);
		int64_t dsor = (int32_t)reg_l(m.R_M);
		assert(dsor != 0);
		reg_l(R_EAX) = dend / dsor;
		reg_l(R_EDX) = dend % dsor;

		print_asm("idivl     %%%s", regsl[m.R_M]);
		return 1 + 1;
 	}
}
