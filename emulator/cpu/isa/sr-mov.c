#include "helper.h"
#include "cpu/reg.h"
#include "modrm.h"

// movl   r32, %cr0/%cr1/%cr2/%cr3
make_helper(mov_cr2r)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	
	if (m.mod == 3)
	{
		reg_l(m.R_M) = cpu.CR[m.reg];

		print_asm("movl   %%cr%d, %%%s", m.reg, regsl[m.R_M]);

		return 1 + 1;
	}
	assert(0);
}

// movl   %cr0/%cr1/%cr2/%cr3, r32
make_helper(mov_r2cr)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);

	if (m.mod == 3)
	{
		cpu.CR[m.reg] = reg_l(m.R_M);

		print_asm("movl   %%%s, %%cr%d", regsl[m.R_M], m.reg);

		return 1 + 1;
	}
	assert(0);
}

// movw   rm, %ES/CS/SS/DS/FS/GS
make_helper(mov_rm2sreg)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);

	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		uint16_t mem_v = swaddr_read(mem_i, 2);

		cpu.sreg[m.reg].val = mem_v;

		print_asm("movw   %s, %%%s", ModR_M_asm, sregs[m.reg]);

		return 1 + len;
	}
	else
	{
		cpu.sreg[m.reg].val = reg_w(m.R_M);

		print_asm("movw   %%%s, %%%s", regsw[m.R_M], sregs[m.reg]);

		return 1 + 1;
	}
}
	
// movw   %ES/CS/SS/DS/FS/GS, rm
make_helper(mov_sreg2rm)
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);

	if (m.mod != 3)
	{
		swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
		swaddr_write(mem_i, 2, cpu.sreg[m.reg].val);

		print_asm("movw   %%%s, %s", sregs[m.reg], ModR_M_asm);

		return 1 + len;
	}
	else
	{
		reg_w(m.R_M) = cpu.sreg[m.reg].val;

		print_asm("movw   %%%s, %%%s", sregs[m.reg], regsw[m.R_M]);

		return 1 + 1;
	}
}	
