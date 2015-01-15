#include "exec/helper.h"

#include "memory.h"
#include "cpu/reg.h"
#include "cpu/modrm.h"


#define  DATA_BYTE 2
#include "imul-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "imul-template.h"
#undef   DATA_BYTE


int imul_b(swaddr_t eip);
int imul_w(swaddr_t eip);
int imul_l(swaddr_t eip);


extern char suffix;
make_helper(imul_v)
{	return  suffix == 'l'  ?  imul_l(eip)  :  imul_w(eip);	}

make_helper(imul_rm2r_v)
{	return  suffix == 'l'  ?  imul_rm2r_l(eip) :  imul_rm2r_w(eip);		}

make_helper(imul_i2r_v)
{	return  suffix == 'l'  ?  imul_i2r_l(eip)  :  imul_i2r_w(eip);		}


int imul_b(swaddr_t eip)
{
	assert(0);
	// ModR_M:  mod  reg  R_M
	//           xx  101  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 5);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		int16_t opd1 = (int8_t)reg_b(R_AL);
		int16_t opd2 = (int8_t)swaddr_read(mem_a, 1);
		reg_w(R_AX) = opd1 * opd2;
		cpu.CF = cpu.OF = !(reg_w(R_AX) == (int16_t)(int8_t)reg_b(R_AL));
		
		print_asm("imulb     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int16_t opd1 = (int8_t)reg_b(R_AL);
		int16_t opd2 = (int8_t)reg_b(m.R_M);
		reg_w(R_AX) = opd1 * opd2;
		cpu.CF = cpu.OF = !(reg_w(R_AX) == (int16_t)(int8_t)reg_b(R_AL));

		print_asm("imulb     %%%s", regsb[m.R_M]);
		return 1 + 1;
	}
}

int imul_w(swaddr_t eip)
{
	assert(0);
	// ModR_M:  mod  reg  R_M
	//           xx  101  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 5);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		int32_t opd1 = (int16_t)reg_w(R_AX);
		int32_t opd2 = (int16_t)swaddr_read(mem_a, 2);
		int32_t valu = opd1 * opd2;
		reg_w(R_AX) =  valu & 0x0000FFFF;
		reg_w(R_DX) = (valu & 0xFFFF0000) >> 16;
		cpu.CF = cpu.OF = !(valu == (int32_t)(int16_t)reg_w(R_AX));
		
		print_asm("imulw     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int32_t opd1 = (int16_t)reg_w(R_AX);
		int32_t opd2 = (int16_t)reg_w(m.R_M);
		int32_t valu = opd1 * opd2;
		reg_w(R_AX) =  valu & 0x0000FFFF;
		reg_w(R_DX) = (valu & 0xFFFF0000) >> 16;
		cpu.CF = cpu.OF = !(valu == (int32_t)(int16_t)reg_w(R_AX));

		print_asm("imulw     %%%s", regsw[m.R_M]);
		return 1 + 1;
	}
}

int imul_l(swaddr_t eip)
{
	assert(0);
	// ModR_M:  mod  reg  R_M
	//           xx  101  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 5);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		int64_t opd1 = (int32_t)reg_l(R_EAX);
		int64_t opd2 = (int32_t)swaddr_read(mem_a, 4);
		int64_t valu = opd1 * opd2;
		reg_l(R_EAX) =  valu & 0x00000000FFFFFFFF;
		reg_l(R_EDX) = (valu & 0xFFFFFFFF00000000) >> 32;
		cpu.CF = cpu.OF = !(valu == (int64_t)(int32_t)reg_l(R_EAX));
		
		print_asm("imull     %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		int64_t opd1 = (int32_t)reg_l(R_EAX);
		int64_t opd2 = (int32_t)reg_l(m.R_M);
		int64_t valu = opd1 * opd2;
		reg_l(R_EAX) =  valu & 0x00000000FFFFFFFF;
		reg_l(R_EDX) = (valu & 0xFFFFFFFF00000000) >> 32;
		cpu.CF = cpu.OF = !(valu == (int64_t)(int32_t)reg_l(R_EAX));

		print_asm("imull     %%%s", regsl[m.R_M]);
		return 1 + 1;
	}
}
