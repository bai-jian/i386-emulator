#include "exec/helper.h"

#include "cpu/reg.h"
#include "cpu/modrm.h"

extern char suffix;

make_helper(idiv_b);
make_helper(idiv_w);
make_helper(idiv_l);

make_helper(idiv_v)
{	return  suffix == 'l'  ?  idiv_l(eip)  :  idiv_w(eip);	}

make_helper(idiv_b)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		
		int8_t mem_v = swaddr_read(mem_i, 1);
		int8_t quo = (int16_t)reg_w(R_AX) / mem_v;
		reg_b(R_AL) = quo;
		int8_t rem = (int16_t)reg_w(R_AX) % mem_v;
		reg_b(R_AH) = rem;

		print_asm("idivb " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int8_t reg_v = reg_b(reg_i);
		int8_t quo = (int16_t)reg_w(R_AX) / reg_v;
		reg_b(R_AL) = quo;
		int8_t rem = (int16_t)reg_w(R_AX) % reg_v;
		reg_b(R_AH) = rem;

		print_asm("idivb " "%%%s", regsb[reg_i]);

		return 1 + 1;
	}
}

make_helper(idiv_w)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		
		int16_t mem_v = swaddr_read(mem_i, 2);
		int16_t quo = (int32_t)reg_l(R_EAX) / mem_v;
		reg_w(R_AX) = quo;
		int16_t rem = (int32_t)reg_l(R_EAX) % mem_v;
		reg_w(R_DX) = rem;

		print_asm("idivw " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int16_t reg_v = reg_w(reg_i);
		int16_t quo = (int32_t)reg_l(R_EAX) / reg_v;
		reg_w(R_AX) = quo;
		int16_t rem = (int32_t)reg_l(R_EAX) % reg_v;
		reg_w(R_DX) = rem;

		print_asm("idivw " "%%%s", regsw[reg_i]);

		return 1 + 1;
	}
}

make_helper(idiv_l)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		
		int32_t mem_v = swaddr_read(mem_i, 4);
		int64_t dividend = ( ((int64_t)(reg_l(R_EDX))) << 32) + (int64_t)reg_l(R_EAX);
		int32_t quo = dividend / mem_v;
		reg_l(R_EAX) = quo;
		int32_t rem = dividend % mem_v;
		reg_l(R_EDX) = rem;

		print_asm("idivl " "%s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		uint8_t reg_i = m.R_M;

		int32_t reg_v = reg_l(reg_i);
		int64_t dividend = ( ((int64_t)(reg_l(R_EDX))) << 32) + (int64_t)reg_l(R_EAX);
		int32_t quo = dividend / reg_v;
		reg_l(R_EAX) = quo;
		int32_t rem = dividend % reg_v;
		reg_l(R_EDX) = rem;

		print_asm("idivl " "%%%s", regsl[reg_i]);

		return 1 + 1;
	}
}
