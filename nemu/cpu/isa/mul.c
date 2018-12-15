#include "helper.h"
#include "memory.h"
#include "cpu/reg.h"
#include "modrm.h"


int mul_b(swaddr_t eip);
int mul_w(swaddr_t eip);
int mul_l(swaddr_t eip);


extern char suffix;
make_helper(mul_v)
{	return suffix == 'l' ? mul_l(eip) : mul_w(eip);	}


int mul_b(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  100  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 4);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint16_t opd1 = reg_b(R_AL);
		uint16_t opd2 = swaddr_read(mem_a, 1);
		reg_w(R_AX) = opd1 * opd2;
		cpu.CF = cpu.OF = !!reg_b(R_AH);

		print_asm("mulb      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint16_t opd1 = reg_b(R_AL);
		uint16_t opd2 = reg_b(m.R_M);
		reg_w(R_AX) = opd1 * opd2;
		cpu.CF = cpu.OF = !!reg_b(R_AH);

		print_asm("mulb      %%%s", regsb[m.R_M]);
		return 1 + 1;
	}
}

int mul_w(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  100  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 4);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint32_t opd1 = reg_w(R_AX);
		uint32_t opd2 = swaddr_read(mem_a, 2);
		uint32_t valu = opd1 * opd2;
		reg_w(R_AX) =  valu & 0x0000FFFF;
		reg_w(R_DX) = (valu & 0xFFFF0000) >> 16;
		cpu.CF = cpu.OF = !!reg_w(R_DX);
		
		print_asm("mulw      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint32_t opd1 = reg_w(R_AX);
		uint32_t opd2 = reg_w(m.R_M);
		uint32_t valu = opd1 * opd2;
		reg_w(R_AX) =  valu & 0x0000FFFF;
		reg_w(R_DX) = (valu & 0xFFFF0000) >> 16;
		cpu.CF = cpu.OF = !!reg_w(R_DX);

		print_asm("mulw      %%%s", regsw[m.R_M]);
		return 1 + 1;
	}
}

int mul_l(swaddr_t eip)
{
	// ModR_M:  mod  reg  R_M
	//           xx  100  xxx
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.reg == 4);

	if (m.mod != 3)
	{
		swaddr_t mem_a;
		uint8_t len = read_ModR_M(eip + 1, &mem_a);

		uint64_t opd1 = reg_l(R_EAX);
		uint64_t opd2 = swaddr_read(mem_a, 4);
		uint64_t valu = opd1 * opd2;
		reg_l(R_EAX) =  valu & 0x00000000FFFFFFFF;
		reg_l(R_EDX) = (valu & 0xFFFFFFFF00000000) >> 32;
		cpu.CF = cpu.OF = !!reg_l(R_EDX);
		
		print_asm("mull      %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		uint64_t opd1 = reg_l(R_EAX);
		uint64_t opd2 = reg_l(m.R_M);
		uint64_t valu = opd1 * opd2;
		reg_l(R_EAX) =  valu & 0x00000000FFFFFFFF;
		reg_l(R_EDX) = (valu & 0xFFFFFFFF00000000) >> 32;
		cpu.CF = cpu.OF = !!reg_l(R_EDX);

		print_asm("mull      %%%s", regsl[m.R_M]);
		return 1 + 1;
	}
}
