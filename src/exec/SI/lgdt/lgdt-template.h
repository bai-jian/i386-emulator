#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"


// lgdt  %GDTR, m16/m32(linear address, not virtual address)
uint32_t swaddr_read(lnaddr_t addr, size_t len);

make_helper( concat(lgdt_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.R_M == R_EBP && m.mod == 0)
	{
		uint32_t lnaddr = swaddr_read(eip+2, 4);
		cpu.GDTR = swaddr_read(lnaddr, DATA_BYTE);

		print_asm("lgdt   0x%x", lnaddr);

		return 1 + 1 + 4;
	}
	assert(0);  // never get here
}


#include "exec/template-end.h"
