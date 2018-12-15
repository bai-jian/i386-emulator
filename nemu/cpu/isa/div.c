#include "helper.h"
#include "memory.h"
#include "modrm.h"


int div_b(swaddr_t eip);
int div_w(swaddr_t eip);
int div_l(swaddr_t eip);


extern char suffix;
make_helper(div_v)
{	return suffix == 'l' ? div_l(eip) : div_w(eip);	}


int div_b(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  110  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 6);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint16_t dend = reg_w(R_AX);
		uint16_t dsor = swaddr_read(mem_a, 1);
		assert(dsor != 0);
		reg_b(R_AL) = dend / dsor;
		reg_b(R_AH) = dend % dsor;

		print_asm("divb      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint16_t dend = reg_w(R_AX);
		uint16_t dsor = reg_b(m.R_M);
		assert(dsor != 0);
		reg_b(R_AL) = dend / dsor;
		reg_b(R_AH) = dend % dsor;

		print_asm("divb      %%%s", regsb[m.R_M]);
		return 1 + 1;
	}
}

int div_w(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  110  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 6);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint32_t dend = ((uint32_t)reg_w(R_DX) << 16) + reg_w(R_AX);
		uint32_t dsor = swaddr_read(mem_a, 2);
		assert(dsor != 0);
		reg_w(R_AX) = dend / dsor;
		reg_w(R_DX) = dend % dsor;

		print_asm("divw      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint32_t dend = ((uint32_t)reg_w(R_DX) << 16) + reg_w(R_AX);
		uint32_t dsor = reg_w(m.R_M);
		assert(dsor != 0);
		reg_w(R_AX) = dend / dsor;
		reg_w(R_DX) = dend % dsor;

		print_asm("divw      %%%s", regsw[m.R_M]);
		return 1 + 1;
	}
}

int div_l(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  110  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 6);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint64_t dend = ((uint64_t)reg_l(R_EDX) << 32) + reg_l(R_EAX);
		uint64_t dsor = swaddr_read(mem_a, 4);
		assert(dsor != 0);
		reg_l(R_EAX) = dend / dsor;
		reg_l(R_EDX) = dend % dsor;

		print_asm("divl      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint64_t dend = ((uint64_t)reg_l(R_EDX) << 32) + reg_l(R_EAX);
		uint64_t dsor = reg_l(m.R_M);
		assert(dsor != 0);
		reg_l(R_EAX) = dend / dsor;
		reg_l(R_EDX) = dend % dsor;

		print_asm("divl      %%%s", regsl[m.R_M]);
		return 1 + 1;
	}
}
